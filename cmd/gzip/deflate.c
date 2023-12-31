#include <u.h>
#include <libc.h>
#include "deflate.h"

typedef struct Chain	Chain;
typedef struct Chains	Chains;
typedef struct Dyncode	Dyncode;
typedef struct Huff	Huff;
typedef struct LZblock	LZblock;
typedef struct LZstate	LZstate;

enum
{
	/*
	 * deflate format paramaters
	 */
	DeflateUnc	= 0,			/* uncompressed block */
	DeflateFix	= 1,			/* fixed huffman codes */
	DeflateDyn	= 2,			/* dynamic huffman codes */

	DeflateEob	= 256,			/* end of block code in lit/len book */
	DeflateMaxBlock	= 64*1024-1,		/* maximum size of uncompressed block */

	DeflateMaxExp	= 10,			/* maximum expansion for a block */

	LenStart	= 257,			/* start of length codes in litlen */
	Nlitlen		= 288,			/* number of litlen codes */
	Noff		= 30,			/* number of offset codes */
	Nclen		= 19,			/* number of codelen codes */

	MaxOff		= 32*1024,
	MinMatch	= 3,			/* shortest match possible */
	MaxMatch	= 258,			/* longest match possible */

	/*
	 * huffman code paramaters
	 */
	MaxLeaf		= Nlitlen,
	MaxHuffBits	= 16,			/* max bits in a huffman code */
	ChainMem	= 2 * (MaxHuffBits - 1) * MaxHuffBits,

	/*
	 * internal lz paramaters
	 */
	DeflateOut	= 4096,			/* output buffer size */
	MinMatchMaxOff	= 4096,			/* max profitable offset for small match;
						 * assumes 8 bits for len, 5+10 for offset */
	HistSlop	= 4096,			/* slop for fewer calls to lzcomp */
	HistBlock	= 64*1024,
	HistSize	= HistBlock + HistSlop,

	HashLog		= 13,
	HashSize	= 1<<HashLog,
	HashMask	= HashSize - 1,

	MaxOffCode	= 256,			/* biggest offset looked up in direct table */

	EstLitBits	= 8,
	EstLenBits	= 4,
	EstOffBits	= 5,
};

/*
 * knuth vol. 3 multiplicative hashing
 * each byte x chosen according to rules
 * 1/4 < x < 3/10, 1/3 x < < 3/7, 4/7 < x < 2/3, 7/10 < x < 3/4
 * with reasonable spread between the bytes & their complements
 *
 * the 3 byte value appears to be as almost good as the 4 byte value,
 * and might be faster on some machines
 */
/*
#define hashit(c)	(((ulong)(c) * 0x6b43a9) >> (24 - HashLog))
*/
#define hashit(c)	((((ulong)(c) & 0xffffff) * 0x6b43a9b5) >> (32 - HashLog))

/*
 * lempel-ziv style compression state
 */
struct LZstate
{
	uchar	hist[HistSize];
	uchar	*epos;				/* end of history buffer */
	uchar	*pos;				/* current location in history buffer */
	int	eof;
	int	hash[HashSize];			/* hash chains */
	int	nexts[MaxOff];
	int	now;				/* pos in hash chains */
	int	dot;				/* dawn of time in history */
	int	prevlen;			/* lazy matching state */
	int	prevoff;
	int	maxcheck;			/* compressor tuning */

	uchar	obuf[DeflateOut];
	uchar	*out;				/* current position in the output buffer */
	uchar	*eout;
	ulong	bits;				/* bit shift register */
	int	nbits;
	int	wbad;				/* got an error writing the buffer */
	int	(*w)(void*, void*, int);
	void	*wr;

	ulong	totr;				/* total input size */
	ulong	totw;				/* total output size */
	int	debug;
};

struct LZblock
{
	uchar	litlen[DeflateMaxBlock+1];
	ushort	off[DeflateMaxBlock+1];
	ulong	litlencount[Nlitlen];
	ulong	offcount[Noff];
	int	entries;			/* entries in litlen & off tables */
	int	bytes;				/* consumed from the input */
	int	excost;				/* cost of encoding extra len & off bits */
};

/*
 * huffman code table
 */
struct Huff
{
	short	bits;				/* length of the code */
	ushort	encode;				/* the code */
};

/*
 * encoding of dynamic huffman trees
 */
struct Dyncode
{
	int	nlit;
	int	noff;
	int	nclen;
	int	ncode;
	Huff	codetab[Nclen];
	uchar	codes[Nlitlen+Noff];
	uchar	codeaux[Nlitlen+Noff];
};

static	int	deflateb(LZstate *lz, LZblock *lzb, void *rr, int (*r)(void*, void*, int));
static	void	lzcomp(LZstate*, LZblock*, uchar*, ushort*);
static	int	wrblock(LZstate*, int, uchar*, ushort*, Huff*, Huff*);
static	int	bitcost(Huff*, ulong*, int);
static	int	huffcodes(Dyncode*, Huff*, Huff*);
static	void	wrdyncode(LZstate*, Dyncode*);
static	void	lzput(LZstate*, ulong bits, int nbits);
static	void	lzflushbits(LZstate*);
static	void	lzflush(LZstate *lz);
static	void	lzwrite(LZstate *lz, void *buf, int n);

static	int	hufftabinit(Huff*, int, ulong*, int);
static	int	mkgzprecode(Huff*, ulong *, int, int);

static	int	mkprecode(Huff*, ulong *, int, int, ulong*);
static	void	nextchain(Chains*, int);
static	void	leafsort(ulong*, ushort*, int, int);

/* conversion from len to code word */
static int lencode[MaxMatch];

/*
 * conversion from off to code word
 * off <= MaxOffCode ? offcode[off] : bigoffcode[(off-1) >> 7]
*/
static int offcode[MaxOffCode + 1];
static int bigoffcode[256];

/* litlen code words LenStart-285 extra bits */
static int litlenbase[Nlitlen-LenStart];
static int litlenextra[Nlitlen-LenStart] =
{
/* 257 */	0, 0, 0,
/* 260 */	0, 0, 0, 0, 0, 1, 1, 1, 1, 2,
/* 270 */	2, 2, 2, 3, 3, 3, 3, 4, 4, 4,
/* 280 */	4, 5, 5, 5, 5, 0, 0, 0
};

/* offset code word extra bits */
static int offbase[Noff];
static int offextra[] =
{
	0,  0,  0,  0,  1,  1,  2,  2,  3,  3,
	4,  4,  5,  5,  6,  6,  7,  7,  8,  8,
	9,  9,  10, 10, 11, 11, 12, 12, 13, 13,
	0,  0,
};

/* order code lengths */
static int clenorder[Nclen] =
{
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

/* static huffman tables */
static	Huff	litlentab[Nlitlen];
static	Huff	offtab[Noff];
static	Huff	hofftab[Noff];

/* bit reversal for brain dead endian swap in huffman codes */
static	uchar	revtab[256];
static	ulong	nlits;
static	ulong	nmatches;

void
deflateinit(void)
{
	ulong bitcount[MaxHuffBits];
	int i, j, ci, n;

	/* byte reverse table */
	for(i=0; i<256; i++)
		for(j=0; j<8; j++)
			if(i & (1<<j))
				revtab[i] |= 0x80 >> j;

	/* static Litlen bit lengths */
	for(i=0; i<144; i++)
		litlentab[i].bits = 8;
	for(i=144; i<256; i++)
		litlentab[i].bits = 9;
	for(i=256; i<280; i++)
		litlentab[i].bits = 7;
	for(i=280; i<Nlitlen; i++)
		litlentab[i].bits = 8;

	memset(bitcount, 0, sizeof(bitcount));
	bitcount[8] += 144 - 0;
	bitcount[9] += 256 - 144;
	bitcount[7] += 280 - 256;
	bitcount[8] += Nlitlen - 280;

	hufftabinit(litlentab, Nlitlen, bitcount, 9);

	/* static offset bit lengths */
	for(i = 0; i < Noff; i++)
		offtab[i].bits = 5;

	memset(bitcount, 0, sizeof(bitcount));
	bitcount[5] = Noff;

	hufftabinit(offtab, Noff, bitcount, 5);

	bitcount[0] = 0;
	bitcount[1] = 0;
	mkgzprecode(hofftab, bitcount, 2, MaxHuffBits);

	/* conversion tables for lens & offs to codes */
	ci = 0;
	for(i = LenStart; i < 286; i++){
		n = ci + (1 << litlenextra[i - LenStart]);
		litlenbase[i - LenStart] = ci;
		for(; ci < n; ci++)
			lencode[ci] = i;
	}
	/* patch up special case for len MaxMatch */
	lencode[MaxMatch-MinMatch] = 285;
	litlenbase[285-LenStart] = MaxMatch-MinMatch;

	ci = 1;
	for(i = 0; i < 16; i++){
		n = ci + (1 << offextra[i]);
		offbase[i] = ci;
		for(; ci < n; ci++)
			offcode[ci] = i;
	}

	ci = (ci - 1) >> 7;
	for(; i < 30; i++){
		n = ci + (1 << (offextra[i] - 7));
		offbase[i] = (ci << 7) + 1;
		for(; ci < n; ci++)
			bigoffcode[ci] = i;
	}
}

static void
deflatereset(LZstate *lz, int level, int debug)
{
	memset(lz->nexts, 0, sizeof lz->nexts);
	memset(lz->hash, 0, sizeof lz->hash);
	lz->totr = 0;
	lz->totw = 0;
	lz->pos = lz->hist;
	lz->epos = lz->hist;
	lz->out = lz->obuf;
	lz->eout = &lz->obuf[DeflateOut];
	lz->prevlen = MinMatch - 1;
	lz->prevoff = 0;
	lz->eof = 0;
	lz->now = HistBlock;
	lz->dot = lz->now;
	lz->bits = 0;
	lz->nbits = 0;
	lz->maxcheck = (1 << level);
	lz->maxcheck -= lz->maxcheck >> 2;
	if(lz->maxcheck < 2)
		lz->maxcheck = 2;
	else if(lz->maxcheck > 1024)
		lz->maxcheck = 1024;

	lz->debug = debug;
}

int
deflate(void *wr, int (*w)(void*, void*, int), void *rr, int (*r)(void*, void*, int), int level, int debug)
{
	LZstate *lz;
	LZblock *lzb;
	int ok;

	lz = malloc(sizeof *lz + sizeof *lzb);
	if(lz == nil){
		werrstr("out of memory");
		return 0;
	}
	lzb = (LZblock*)&lz[1];

	deflatereset(lz, level, debug);
	lz->w = w;
	lz->wr = wr;
	lz->wbad = 0;
	while(!lz->eof)
		if(!deflateb(lz, lzb, rr, r))
			return 0;
	ok = !lz->wbad;
	free(lz);
	return ok;
}

static int
deflateb(LZstate *lz, LZblock *lzb, void *rr, int (*r)(void*, void*, int))
{
	Dyncode dyncode, hdyncode;
	Huff dlitlentab[Nlitlen], dofftab[Noff], hlitlentab[Nlitlen];
	ulong litcount[Nlitlen];
	long nunc, ndyn, nfix, nhuff;
	uchar *slop, *hslop;
	int i, n, m, mm, nslop;

	memset(lzb->litlencount, 0, sizeof lzb->litlencount);
	memset(lzb->offcount, 0, sizeof lzb->offcount);
	lzb->litlencount[DeflateEob]++;

	lzb->bytes = 0;
	lzb->entries = 0;
	lzb->excost = 0;
	lz->eof = 0;

	slop = lz->pos;
	n = lz->epos - lz->pos;
	while(n < DeflateMaxBlock && !lz->eof){
		/*
		 * wrap position if the buffer if full.
		 */
		if(lz->pos >= &lz->hist[HistBlock] && lz->epos == &lz->hist[HistSize]){
			lz->pos -= HistBlock;
			lz->epos -= HistBlock;
		}

		/*
		 * fill the buffer as much as possible,
		 * while leaving room for MaxOff history behind lz->pos,
		 * and not reading more than we can handle.
		 */
		m = HistSize - (lz->epos - lz->hist);
		if(m > HistBlock - MaxOff - (lz->epos - lz->pos))
			m = HistBlock - MaxOff - (lz->epos - lz->pos);
		if(m > DeflateMaxBlock - n)
			m = DeflateMaxBlock - n;
		mm = (*r)(rr, lz->epos, m);
		if(mm != m)
			lz->eof = 1;
		if(mm > 0){
			lz->totr += mm;
			n += mm;
			lz->epos += mm;

			/*
			 * wrap data from the end;
			 * this way, we don't have to wrap searches.
			 */
			if(lz->epos > &lz->hist[HistBlock]){
				if(mm > lz->epos - &lz->hist[HistBlock])
					mm = lz->epos - &lz->hist[HistBlock];
				memmove(lz->epos - mm - HistBlock, lz->epos - mm, mm);
			}
		}

		lzcomp(lz, lzb, lzb->litlen + lzb->entries, lzb->off + lzb->entries);
	}

	nunc = lzb->bytes;

	if(!mkgzprecode(dlitlentab, lzb->litlencount, Nlitlen, MaxHuffBits)
	|| !mkgzprecode(dofftab, lzb->offcount, Noff, MaxHuffBits))
		return 0;

	ndyn = huffcodes(&dyncode, dlitlentab, dofftab);
	if(ndyn < 0)
		return 0;
	ndyn += bitcost(dlitlentab, lzb->litlencount, Nlitlen)
		+ bitcost(dofftab, lzb->offcount, Noff)
		+ lzb->excost;

	memset(litcount, 0, sizeof litcount);

	nslop = nunc;
	if(nslop > &lz->hist[HistSize] - slop)
		nslop = &lz->hist[HistSize] - slop;

	for(i = 0; i < nslop; i++)
		litcount[slop[i]]++;
	hslop = &lz->hist[HistSlop];
	for(i = 0; i < nunc - nslop; i++)
		litcount[hslop[i]]++;
	litcount[DeflateEob]++;

	if(!mkgzprecode(hlitlentab, litcount, Nlitlen, MaxHuffBits))
		return 0;
	nhuff = huffcodes(&hdyncode, hlitlentab, hofftab);
	if(nhuff < 0)
		return 0;
	nhuff += bitcost(hlitlentab, litcount, Nlitlen);

	nfix = bitcost(litlentab, lzb->litlencount, Nlitlen)
		+ bitcost(offtab, lzb->offcount, Noff)
		+ lzb->excost;

	lzput(lz, lz->eof && lz->pos == lz->epos, 1);

	if(lz->debug){
		fprint(2, "block: %lud bytes %d entries %d extra bits\n\tuncompressed %lud fixed %lud dynamic %lud huffman %lud\n",
			nunc, lzb->entries, lzb->excost, (nunc + 4) * 8, nfix, ndyn, nhuff);
		fprint(2, "\tnlit=%lud matches=%lud\n", nlits, nmatches);
	}

	if((nunc + 4) * 8 < ndyn && (nunc + 4) * 8 < nfix && (nunc + 4) * 8 < nhuff){
		lzput(lz, DeflateUnc, 2);
		lzflushbits(lz);

		lzput(lz, nunc & 0xff, 8);
		lzput(lz, (nunc >> 8) & 0xff, 8);
		lzput(lz, ~nunc & 0xff, 8);
		lzput(lz, (~nunc >> 8) & 0xff, 8);
		lzflush(lz);

		lzwrite(lz, slop, nslop);
		lzwrite(lz, &lz->hist[HistSlop], nunc - nslop);
	}else if(ndyn < nfix && ndyn < nhuff){
		lzput(lz, DeflateDyn, 2);

		wrdyncode(lz, &dyncode);
		wrblock(lz, lzb->entries, lzb->litlen, lzb->off, dlitlentab, dofftab);
		lzput(lz, dlitlentab[DeflateEob].encode, dlitlentab[DeflateEob].bits);
	}else if(nhuff < nfix){
		lzput(lz, DeflateDyn, 2);

		wrdyncode(lz, &hdyncode);
		memset(lzb->off, 0, sizeof lzb->off);

		wrblock(lz, nslop, slop, lzb->off, hlitlentab, hofftab);
		wrblock(lz, nunc - nslop, &lz->hist[HistSlop], lzb->off, hlitlentab, hofftab);

		lzput(lz, hlitlentab[DeflateEob].encode, hlitlentab[DeflateEob].bits);
	}else{
		lzput(lz, DeflateFix, 2);

		wrblock(lz, lzb->entries, lzb->litlen, lzb->off, litlentab, offtab);
		lzput(lz, litlentab[DeflateEob].encode, litlentab[DeflateEob].bits);
	}

	if(lz->eof){
		lzflushbits(lz);
		lzflush(lz);
	}

	return 1;
}

static void
lzwrite(LZstate *lz, void *buf, int n)
{
	int nw;

	if(n && lz->w){
		nw = (*lz->w)(lz->wr, buf, n);
		if(nw != n){
			lz->w = nil;
			lz->wbad = 1;
		}else
			lz->totw += n;
	}
}

static void
lzflush(LZstate *lz)
{
	lzwrite(lz, lz->obuf, lz->out - lz->obuf);
	lz->out = lz->obuf;
}

static void
lzput(LZstate *lz, ulong bits, int nbits)
{
	bits = (bits << lz->nbits) | lz->bits;
	for(nbits += lz->nbits; nbits >= 8; nbits -= 8){
		*lz->out++ = bits;
		if(lz->out == lz->eout)
			lzflush(lz);
		bits >>= 8;
	}
	lz->bits = bits;
	lz->nbits = nbits;
}

static void
lzflushbits(LZstate *lz)
{
	if(lz->nbits)
		lzput(lz, 0, 8 - (lz->nbits & 7));
}

/*
 * write out a block of n samples,
 * given lz encoding and counts for huffman tables
 */
static int
wrblock(LZstate *out, int n, uchar *litlen, ushort *off, Huff *litlentab, Huff *offtab)
{
	int i, offset, lit, c;

	for(i = 0; i < n; i++){
		offset = *off++;
		lit = *litlen++;
		if(out->debug > 2){
			if(offset == 0)
				fprint(2, "\tlit %.2ux %c\n", lit, lit);
			else
				fprint(2, "\t<%d, %d>\n", offset, lit + MinMatch);
		}
		if(offset == 0)
			lzput(out, litlentab[lit].encode, litlentab[lit].bits);
		else {
			c = lencode[lit];
			lzput(out, litlentab[c].encode, litlentab[c].bits);
			c -= LenStart;
			if(litlenextra[c])
				lzput(out, lit - litlenbase[c], litlenextra[c]);

			if(offset <= MaxOffCode)
				c = offcode[offset];
			else
				c = bigoffcode[(offset - 1) >> 7];
			lzput(out, offtab[c].encode, offtab[c].bits);
			if(offextra[c])
				lzput(out, offset - offbase[c], offextra[c]);
		}
	}

	return n;
}

/*
 * look for the longest, closest string which matches
 * the next prefix.  the clever part here is looking for
 * a string 1 longer than the previous best match.
 *
 * follows the recommendation of limiting number of chains
 * which are checked.  this appears to be the best heuristic.
 */
static int
lzmatch(int now, int then, uchar *p, uchar *es, int *nexts, uchar *hist, int runlen, int check, int *m)
{
	uchar *s, *t;
	int ml;

	ml = check;
	if(runlen >= 8)
		check >>= 2;
	*m = 0;
	if(p + runlen >= es)
		return runlen;
	for(; check-- > 0 && now - then <= MaxOff; then = nexts[then & (MaxOff-1)]){
		s = p + runlen;
		t = hist + (then & (HistBlock-1));
		t += runlen;
		for(; s >= p; s--){
			if(*s != *t)
				goto matchloop;
			t--;
		}

		/*
		 * we have a new best match.
		 * extend it to it's maximum length
		 */
		t += runlen + 2;
		s += runlen + 2;
		for(; s < es; s++){
			if(*s != *t)
				break;
			t++;
		}
		runlen = s - p;
		*m = then;
		if(s == es || runlen > ml)
			break;
matchloop:;
	}
	return runlen;
}

static void
lzcomp(LZstate *lz, LZblock *lzb, uchar *slitlen, ushort *off)
{
	ulong cont, excost, *litlencount, *offcount;
	uchar *p, *ep, *q, *s, *es, *litlen;
	int *nexts, *hash;
	int i, h, runlen, n, now, then, m, prevlen, prevoff, maxdefer;

	litlencount = lzb->litlencount;
	offcount = lzb->offcount;
	litlen = slitlen;

	nexts = lz->nexts;
	hash = lz->hash;
	now = lz->now;

	p = lz->pos;
	ep = lz->epos;
	if(lz->prevlen != MinMatch - 1)
		p++;

	/*
	 * hash in the links for any hanging link positions,
	 * and calculate the hash for the current position.
	 */
	n = MinMatch;
	if(n > ep - p)
		n = ep - p;
	cont = 0;
	for(i = 0; i < n - 1; i++){
		m = now - ((MinMatch-1) - i);
		if(m < lz->dot)
			continue;
		s = lz->hist + (m & (HistBlock-1));

		cont = (s[0] << 16) | (s[1] << 8) | s[2];
		h = hashit(cont) & HashMask;
		for(then = hash[h]; now - then < now - m; then = nexts[then & (MaxOff-1)])
			;
		if(then == m)
			continue;
		nexts[m & (MaxOff-1)] = hash[h];
		hash[h] = m;
	}
	for(i = 0; i < n; i++)
		cont = (cont << 8) | p[i];

	/*
	 * now must point to the index in the nexts array
	 * corresponding to p's position in the history
	 */
	prevlen = lz->prevlen;
	prevoff = lz->prevoff;
	maxdefer = lz->maxcheck >> 2;
	excost = 0;
	for(;;){
		es = p + MaxMatch;
		if(es > ep){
			if(!lz->eof || p >= ep)
				break;
			es = ep;
		}

		h = hashit(cont) & HashMask;
		runlen = lzmatch(now, hash[h], p, es, nexts, lz->hist, prevlen, lz->maxcheck, &m);

		/*
		 * back out of small matches too far in the past
		 */
		if(runlen == MinMatch && now - m >= MinMatchMaxOff){
			runlen = MinMatch - 1;
			m = 0;
		}

		/*
		 * record the encoding and increment counts for huffman trees
		 * if we get a match, defer selecting it until we check for
		 * a longer match at the next position.
		 */
		if(prevlen >= runlen && prevlen != MinMatch - 1){
			/*
			 * old match at least as good; use that one
			 */
			n = prevlen - MinMatch;
			*litlen++ = n;
			n = lencode[n];
			litlencount[n]++;
			excost += litlenextra[n - LenStart];

			*off++ = prevoff;
			if(prevoff <= MaxOffCode)
				n = offcode[prevoff];
			else
				n = bigoffcode[(prevoff - 1) >> 7];
			offcount[n]++;
			excost += offextra[n];

			runlen = prevlen - 1;
			prevlen = MinMatch - 1;
			nmatches++;
		}else if(runlen == MinMatch - 1){
			/*
			 * no match; just put out the literal
			 */
			n = *p;
			*litlen++ = n;
			litlencount[n]++;
			*off++ = 0;
			nlits++;
			runlen = 1;
		}else{
			if(prevlen != MinMatch - 1){
				/*
				 * longer match now. output previous literal,
				 * update current match, and try again
				 */
				n = p[-1];
				*litlen++ = n;
				litlencount[n]++;
				*off++ = 0;
				nlits++;
			}

			prevoff = now - m;

			if(runlen < maxdefer){
				prevlen = runlen;
				runlen = 1;
			}else{
				n = runlen - MinMatch;
				*litlen++ = n;
				n = lencode[n];
				litlencount[n]++;
				excost += litlenextra[n - LenStart];

				*off++ = prevoff;
				if(prevoff <= MaxOffCode)
					n = offcode[prevoff];
				else
					n = bigoffcode[(prevoff - 1) >> 7];
				offcount[n]++;
				excost += offextra[n];
				prevlen = MinMatch - 1;
				nmatches++;
			}
		}

		/*
		 * update the hash for the newly matched data
		 * this is constructed so the link for the old
		 * match in this position must be at the end of a chain,
		 * and will expire when this match is added, ie it will
		 * never be examined by the match loop.
		 * add to the hash chain only if we have the real hash data.
		 */
		for(q = p + runlen; p != q; p++){
			if(p + MinMatch <= ep){
				h = hashit(cont) & HashMask;
				nexts[now & (MaxOff-1)] = hash[h];
				hash[h] = now;
				if(p + MinMatch < ep)
					cont = (cont << 8) | p[MinMatch];
			}
			now++;
		}
	}

	/*
	 * we can just store away the lazy state and
	 * pick it up next time.  the last block will have eof
	 * so we won't have any pending matches
	 * however, we need to correct for how much we've encoded
	 */
	if(prevlen != MinMatch - 1)
		p--;

	lzb->excost += excost;
	lzb->entries += litlen - slitlen;
	lzb->bytes += p - lz->pos;

	lz->pos = p;
	lz->now = now;
	lz->prevlen = prevlen;
	lz->prevoff = prevoff;
}

/*
 * make up the dynamic code tables, and return the number of bits
 * needed to transmit them.
 */
static int
huffcodes(Dyncode *dc, Huff *littab, Huff *offtab)
{
	Huff *codetab;
	uchar *codes, *codeaux;
	ulong codecount[Nclen], excost;
	int i, n, m, v, c, nlit, noff, ncode, nclen;

	codetab = dc->codetab;
	codes = dc->codes;
	codeaux = dc->codeaux;

	/*
	 * trim the sizes of the tables
	 */
	for(nlit = Nlitlen; nlit > 257 && littab[nlit-1].bits == 0; nlit--)
		;
	for(noff = Noff; noff > 1 && offtab[noff-1].bits == 0; noff--)
		;

	/*
	 * make the code-length code
	 */
	for(i = 0; i < nlit; i++)
		codes[i] = littab[i].bits;
	for(i = 0; i < noff; i++)
		codes[i + nlit] = offtab[i].bits;

	/*
	 * run-length compress the code-length code
	 */
	excost = 0;
	c = 0;
	ncode = nlit+noff;
	for(i = 0; i < ncode; ){
		n = i + 1;
		v = codes[i];
		while(n < ncode && v == codes[n])
			n++;
		n -= i;
		i += n;
		if(v == 0){
			while(n >= 11){
				m = n;
				if(m > 138)
					m = 138;
				codes[c] = 18;
				codeaux[c++] = m - 11;
				n -= m;
				excost += 7;
			}
			if(n >= 3){
				codes[c] = 17;
				codeaux[c++] = n - 3;
				n = 0;
				excost += 3;
			}
		}
		while(n--){
			codes[c++] = v;
			while(n >= 3){
				m = n;
				if(m > 6)
					m = 6;
				codes[c] = 16;
				codeaux[c++] = m - 3;
				n -= m;
				excost += 3;
			}
		}
	}

	memset(codecount, 0, sizeof codecount);
	for(i = 0; i < c; i++)
		codecount[codes[i]]++;
	if(!mkgzprecode(codetab, codecount, Nclen, 8))
		return -1;

	for(nclen = Nclen; nclen > 4 && codetab[clenorder[nclen-1]].bits == 0; nclen--)
		;

	dc->nlit = nlit;
	dc->noff = noff;
	dc->nclen = nclen;
	dc->ncode = c;

	return 5 + 5 + 4 + nclen * 3 + bitcost(codetab, codecount, Nclen) + excost;
}

static void
wrdyncode(LZstate *out, Dyncode *dc)
{
	Huff *codetab;
	uchar *codes, *codeaux;
	int i, v, c;

	/*
	 * write out header, then code length code lengths,
	 * and code lengths
	 */
	lzput(out, dc->nlit-257, 5);
	lzput(out, dc->noff-1, 5);
	lzput(out, dc->nclen-4, 4);

	codetab = dc->codetab;
	for(i = 0; i < dc->nclen; i++)
		lzput(out, codetab[clenorder[i]].bits, 3);

	codes = dc->codes;
	codeaux = dc->codeaux;
	c = dc->ncode;
	for(i = 0; i < c; i++){
		v = codes[i];
		lzput(out, codetab[v].encode, codetab[v].bits);
		if(v >= 16){
			if(v == 16)
				lzput(out, codeaux[i], 2);
			else if(v == 17)
				lzput(out, codeaux[i], 3);
			else /* v == 18 */
				lzput(out, codeaux[i], 7);
		}
	}
}

static int
bitcost(Huff *tab, ulong *count, int n)
{
	ulong tot;
	int i;

	tot = 0;
	for(i = 0; i < n; i++)
		tot += count[i] * tab[i].bits;
	return tot;
}

static int
mkgzprecode(Huff *tab, ulong *count, int n, int maxbits)
{
	ulong bitcount[MaxHuffBits];
	int i, nbits;

	nbits = mkprecode(tab, count, n, maxbits, bitcount);
	for(i = 0; i < n; i++){
		if(tab[i].bits == -1)
			tab[i].bits = 0;
		else if(tab[i].bits == 0){
			if(nbits != 0 || bitcount[0] != 1){
				werrstr("deflate internal error: bad singleton huff code");
				return 0;
			}
			bitcount[1] = 1;
			bitcount[0] = 0;
			nbits = 1;
			tab[i].bits = 1;
		}
	}
	if(bitcount[0] != 0){
		werrstr("deflate internal error: bad huff bitcount[0]");
		return 0;
	}
	return hufftabinit(tab, n, bitcount, nbits);
}

static int
hufftabinit(Huff *tab, int n, ulong *bitcount, int nbits)
{
	ulong code, nc[MaxHuffBits];
	int i, bits;

	code = 0;
	for(bits = 1; bits <= nbits; bits++){
		code = (code + bitcount[bits-1]) << 1;
		nc[bits] = code;
	}

	for(i = 0; i < n; i++){
		bits = tab[i].bits;
		if(bits){
			code = nc[bits]++ << (16 - bits);
			if(code & ~0xffff){
				werrstr("deflate internal error: huff code too big");
				return 0;
			}
			tab[i].encode = revtab[code >> 8] | (revtab[code & 0xff] << 8);
		}
	}
	return 1;
}


/*
 * this should be in a library
 */
struct Chain
{
	ulong	count;				/* occurances of everything in the chain */
	ushort	leaf;				/* leaves to the left of chain, or leaf value */
	char	col;				/* ref count for collecting unused chains */
	char	gen;				/* need to generate chains for next lower level */
	Chain	*up;				/* Chain up in the lists */
};

struct Chains
{
	Chain	*lists[(MaxHuffBits - 1) * 2];
	ulong	leafcount[MaxLeaf];		/* sorted list of leaf counts */
	ushort	leafmap[MaxLeaf];		/* map to actual leaf number */
	int	nleaf;				/* number of leaves */
	Chain	chains[ChainMem];
	Chain	*echains;
	Chain	*free;
	char	col;
	int	nlists;
};

/*
 * fast, low space overhead algorithm for max depth huffman type codes
 *
 * J. Katajainen, A. Moffat and A. Turpin, "A fast and space-economical
 * algorithm for length-limited coding," Proc. Intl. Symp. on Algorithms
 * and Computation, Cairns, Australia, Dec. 1995, Lecture Notes in Computer
 * Science, Vol 1004, J. Staples, P. Eades, N. Katoh, and A. Moffat, eds.,
 * pp 12-21, Springer Verlag, New York, 1995.
 */
static int
mkprecode(Huff *tab, ulong *count, int n, int maxbits, ulong *bitcount)
{
	Chains cs;
	Chain *c;
	int i, m, em, bits;

	/*
	 * set up the sorted list of leaves
	 */
	m = 0;
	for(i = 0; i < n; i++){
		tab[i].bits = -1;
		tab[i].encode = 0;
		if(count[i] != 0){
			cs.leafcount[m] = count[i];
			cs.leafmap[m] = i;
			m++;
		}
	}
	if(m < 2){
		if(m != 0){
			tab[cs.leafmap[0]].bits = 0;
			bitcount[0] = 1;
		}else
			bitcount[0] = 0;
		return 0;
	}
	cs.nleaf = m;
	leafsort(cs.leafcount, cs.leafmap, 0, m);

	for(i = 0; i < m; i++)
		cs.leafcount[i] = count[cs.leafmap[i]];

	/*
	 * set up free list
	 */
	cs.free = &cs.chains[2];
	cs.echains = &cs.chains[ChainMem];
	cs.col = 1;

	/*
	 * initialize chains for each list
	 */
	c = &cs.chains[0];
	c->count = cs.leafcount[0];
	c->leaf = 1;
	c->col = cs.col;
	c->up = nil;
	c->gen = 0;
	cs.chains[1] = cs.chains[0];
	cs.chains[1].leaf = 2;
	cs.chains[1].count = cs.leafcount[1];
	for(i = 0; i < maxbits-1; i++){
		cs.lists[i * 2] = &cs.chains[0];
		cs.lists[i * 2 + 1] = &cs.chains[1];
	}

	cs.nlists = 2 * (maxbits - 1);
	m = 2 * m - 2;
	for(i = 2; i < m; i++)
		nextchain(&cs, cs.nlists - 2);

	bits = 0;
	bitcount[0] = cs.nleaf;
	for(c = cs.lists[cs.nlists - 1]; c != nil; c = c->up){
		m = c->leaf;
		bitcount[bits++] -= m;
		bitcount[bits] = m;
	}
	m = 0;
	for(i = bits; i >= 0; i--)
		for(em = m + bitcount[i]; m < em; m++)
			tab[cs.leafmap[m]].bits = i;

	return bits;
}

/*
 * calculate the next chain on the list
 * we can always toss out the old chain
 */
static void
nextchain(Chains *cs, int list)
{
	Chain *c, *oc;
	int i, nleaf, sumc;

	oc = cs->lists[list + 1];
	cs->lists[list] = oc;
	if(oc == nil)
		return;

	/*
	 * make sure we have all chains needed to make sumc
	 * note it is possible to generate only one of these,
	 * use twice that value for sumc, and then generate
	 * the second if that preliminary sumc would be chosen.
	 * however, this appears to be slower on current tests
	 */
	if(oc->gen){
		nextchain(cs, list - 2);
		nextchain(cs, list - 2);
		oc->gen = 0;
	}

	/*
	 * pick up the chain we're going to add;
	 * collect unused chains no free ones are left
	 */
	for(c = cs->free; ; c++){
		if(c >= cs->echains){
			cs->col++;
			for(i = 0; i < cs->nlists; i++)
				for(c = cs->lists[i]; c != nil; c = c->up)
					c->col = cs->col;
			c = cs->chains;
		}
		if(c->col != cs->col)
			break;
	}

	/*
	 * pick the cheapest of
	 * 1) the next package from the previous list
	 * 2) the next leaf
	 */
	nleaf = oc->leaf;
	sumc = 0;
	if(list > 0 && cs->lists[list-1] != nil)
		sumc = cs->lists[list-2]->count + cs->lists[list-1]->count;
	if(sumc != 0 && (nleaf >= cs->nleaf || cs->leafcount[nleaf] > sumc)){
		c->count = sumc;
		c->leaf = oc->leaf;
		c->up = cs->lists[list-1];
		c->gen = 1;
	}else if(nleaf >= cs->nleaf){
		cs->lists[list + 1] = nil;
		return;
	}else{
		c->leaf = nleaf + 1;
		c->count = cs->leafcount[nleaf];
		c->up = oc->up;
		c->gen = 0;
	}
	cs->free = c + 1;

	cs->lists[list + 1] = c;
	c->col = cs->col;
}

static int
pivot(ulong *c, int a, int n)
{
	int j, pi, pj, pk;

	j = n/6;
	pi = a + j;	/* 1/6 */
	j += j;
	pj = pi + j;	/* 1/2 */
	pk = pj + j;	/* 5/6 */
	if(c[pi] < c[pj]){
		if(c[pi] < c[pk]){
			if(c[pj] < c[pk])
				return pj;
			return pk;
		}
		return pi;
	}
	if(c[pj] < c[pk]){
		if(c[pi] < c[pk])
			return pi;
		return pk;
	}
	return pj;
}

static	void
leafsort(ulong *leafcount, ushort *leafmap, int a, int n)
{
	ulong t;
	int j, pi, pj, pn;

	while(n > 1){
		if(n > 10){
			pi = pivot(leafcount, a, n);
		}else
			pi = a + (n>>1);

		t = leafcount[pi];
		leafcount[pi] = leafcount[a];
		leafcount[a] = t;
		t = leafmap[pi];
		leafmap[pi] = leafmap[a];
		leafmap[a] = t;
		pi = a;
		pn = a + n;
		pj = pn;
		for(;;){
			do
				pi++;
			while(pi < pn && (leafcount[pi] < leafcount[a] || leafcount[pi] == leafcount[a] && leafmap[pi] > leafmap[a]));
			do
				pj--;
			while(pj > a && (leafcount[pj] > leafcount[a] || leafcount[pj] == leafcount[a] && leafmap[pj] < leafmap[a]));
			if(pj < pi)
				break;
			t = leafcount[pi];
			leafcount[pi] = leafcount[pj];
			leafcount[pj] = t;
			t = leafmap[pi];
			leafmap[pi] = leafmap[pj];
			leafmap[pj] = t;
		}
		t = leafcount[a];
		leafcount[a] = leafcount[pj];
		leafcount[pj] = t;
		t = leafmap[a];
		leafmap[a] = leafmap[pj];
		leafmap[pj] = t;
		j = pj - a;

		n = n-j-1;
		if(j >= n){
			leafsort(leafcount, leafmap, a, j);
			a += j+1;
		}else{
			leafsort(leafcount, leafmap, a + (j+1), n);
			n = j;
		}
	}
}
