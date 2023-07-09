
#line	2	"cc.y"
#include "cc.h"

#line	4	"cc.y"
typedef union 	{
	Node*	node;
	Sym*	sym;
	Type*	type;
	struct
	{
		Type*	t;
		char	c;
	} tycl;
	struct
	{
		Type*	t1;
		Type*	t2;
	} tyty;
	struct
	{
		char*	s;
		long	l;
	} sval;
	long	lval;
	double	dval;
	vlong	vval;
} YYSTYPE;
extern	int	yyerrflag;
#ifndef	YYMAXDEPTH
#define	YYMAXDEPTH	150
#endif
YYSTYPE	yylval;
YYSTYPE	yyval;
#define	LPE	57346
#define	LME	57347
#define	LMLE	57348
#define	LDVE	57349
#define	LMDE	57350
#define	LRSHE	57351
#define	LLSHE	57352
#define	LANDE	57353
#define	LXORE	57354
#define	LORE	57355
#define	LOROR	57356
#define	LANDAND	57357
#define	LEQ	57358
#define	LNE	57359
#define	LLE	57360
#define	LGE	57361
#define	LLSH	57362
#define	LRSH	57363
#define	LMM	57364
#define	LPP	57365
#define	LMG	57366
#define	LNAME	57367
#define	LCTYPE	57368
#define	LSTYPE	57369
#define	LFCONST	57370
#define	LDCONST	57371
#define	LCONST	57372
#define	LLCONST	57373
#define	LUCONST	57374
#define	LULCONST	57375
#define	LVLCONST	57376
#define	LUVLCONST	57377
#define	LSTRING	57378
#define	LLSTRING	57379
#define	LAUTO	57380
#define	LBREAK	57381
#define	LCASE	57382
#define	LCHAR	57383
#define	LCONTINUE	57384
#define	LDEFAULT	57385
#define	LDO	57386
#define	LDOUBLE	57387
#define	LELSE	57388
#define	LEXTERN	57389
#define	LFLOAT	57390
#define	LFOR	57391
#define	LGOTO	57392
#define	LIF	57393
#define	LINT	57394
#define	LLONG	57395
#define	LREGISTER	57396
#define	LRETURN	57397
#define	LSHORT	57398
#define	LSIZEOF	57399
#define	LUSED	57400
#define	LSTATIC	57401
#define	LSTRUCT	57402
#define	LSWITCH	57403
#define	LTYPEDEF	57404
#define	LUNION	57405
#define	LUNSIGNED	57406
#define	LWHILE	57407
#define	LVOID	57408
#define	LENUM	57409
#define	LSIGNED	57410
#define	LCONSTNT	57411
#define	LVOLATILE	57412
#define	LSET	57413
#define	LSIGNOF	57414
#define YYEOFCODE 1
#define YYERRCODE 2

#line	1171	"cc.y"

short	yyexca[] =
{-1, 1,
	1, -1,
	-2, 178,
-1, 35,
	4, 8,
	5, 8,
	6, 9,
	-2, 5,
-1, 47,
	93, 191,
	-2, 190,
-1, 50,
	93, 195,
	-2, 194,
-1, 52,
	93, 199,
	-2, 198,
-1, 69,
	6, 9,
	-2, 8,
-1, 304,
	4, 96,
	93, 82,
	-2, 0,
-1, 321,
	6, 22,
	-2, 21,
-1, 326,
	93, 82,
	-2, 96,
-1, 332,
	4, 96,
	93, 82,
	-2, 0,
-1, 382,
	4, 96,
	93, 82,
	-2, 0,
-1, 384,
	4, 96,
	93, 82,
	-2, 0,
-1, 386,
	4, 96,
	93, 82,
	-2, 0,
-1, 396,
	4, 96,
	93, 82,
	-2, 0,
-1, 402,
	4, 96,
	93, 82,
	-2, 0,
};
#define	YYNPROD	240
#define	YYPRIVATE 57344
#define	YYLAST	1195
short	yyact[] =
{
 169, 323, 207, 327, 254, 320, 201, 205,  80, 325,
 264, 340, 255,   4,  15,  38, 203, 341, 127,  78,
 263,   5,  82, 129,  72, 208,  40, 368, 118, 199,
 276,  83, 199,  79,  75,  47,  50,  52, 191, 115,
 202,  41,  42,  43, 117,  96, 252, 266,  73,  49,
 252,  35, 309, 307, 287,  41,  42,  43,  14, 136,
  81,   6, 114, 402, 388, 387, 315, 314, 308,  45,
 292, 130, 123, 289, 122, 110, 286,  60, 134, 132,
  41,  42,  43, 125, 121, 111, 374,  61, 189, 396,
   5,  53, 252, 252, 188, 362, 252, 252,  41,  42,
  43, 119, 168,  69,  74,  49, 214, 213, 109, 176,
 177, 178, 179, 180, 181, 182, 183, 385, 302, 167,
 364, 198, 363, 128, 250, 123, 184, 186, 215,  34,
 110, 134, 251, 294, 218,  81, 398, 359, 219, 220,
 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
 231, 232, 233, 234, 235, 236, 216, 238, 239, 240,
 241, 242, 243, 244, 245, 246, 247, 248, 217, 204,
 212, 256,  74, 237, 196, 211, 190, 194, 386, 384,
  61, 215, 382, 253,  13,  68, 249, 130, 356, 257,
  59,  58,  46, 355, 134, 132,  41,  42,  43, 258,
 259, 272, 354, 168, 250, 168, 250, 123, 267, 277,
  81, 134, 251, 134, 251,  81,   7, 135, 281, 367,
 260, 252, 261,  44, 366, 300, 270, 110, 271,  46,
 370, 280, 278,  48, 288, 200, 283, 282, 284,  71,
  26,  27,  37,  51, 137, 138, 139,  46, 285, 252,
  39,  41,  42,  43, 269, 346, 347,  26,  27,  74,
 109,  17,  62, 274, 275,  24,  54,  55,  23,  81,
 291, 306,  19,  20, 299, 216,  18, 301, 267, 295,
 296, 293, 279, 297,  22, 126,  25, 303,  21, 310,
 290, 319, 305,   5, 256,  26,  27, 112, 401, 116,
 268,  81, 397, 395, 394, 110, 313, 383, 377, 318,
 317, 375, 311, 123, 283, 267,  37, 361, 360, 352,
 345,  37,  33, 204,  39,  41,  42,  43, 349,  39,
  41,  42,  43, 316, 357, 353, 351, 358, 350, 130,
  67, 298, 197, 365,  66,  65, 134, 132,  41,  42,
  43, 321,  37, 369, 140, 141, 137, 138, 139, 372,
  39,  41,  42,  43, 256, 256,  63,  64, 378, 379,
  37, 123, 371, 381, 373, 328, 262, 376,  39,  41,
  42,  43, 192, 113, 389, 348, 391, 390, 393, 175,
 174, 172, 173, 171, 170, 321,  87,  57, 399, 120,
  70, 400,  56, 392, 403,  88,  89,  86,   3,   2,
  93,  92,   1, 133, 131,  84,  77,  11,  12, 102,
 101,  97,  98,  99, 100, 103, 104, 107, 108,  28,
 206, 265,  17, 326,  36, 324,  24, 106,  30,  23,
 105, 304, 273,  19,  20,  32,  85,  18,  94, 193,
  29,   8, 195,  31,   9,  22,  87,  25,  10,  21,
  26,  27,  16,  95,   0,  88,  89,  86,  90,  91,
  93,  92,   0,   0,   0,  84, 344,   0,   0, 102,
 101,  97,  98,  99, 100, 103, 104, 107, 108,   0,
 335, 342,   0, 336, 343, 332,   0,   0,   0,   0,
 330, 337, 329, 324,   0,   0, 333,   0,  94, 338,
   0,   0, 334,   0,   0,   0, 331,   0,   0,   0,
   0,   0, 339,  95,  87,   0,   0, 322,  90,  91,
   0,   0,   0,  88,  89,  86,   0,   0,  93,  92,
   0,   0,   0,  84, 344,   0,   0, 102, 101,  97,
  98,  99, 100, 103, 104, 107, 108,   0, 335, 342,
   0, 336, 343, 332,   0,   0,   0,   0, 330, 337,
 329,   0,   0,   0, 333,   0,  94, 338,   0,   0,
 334,   0,   0,   0, 331,   0,   0,   0,   0,  87,
 339,  95,   0,   0,   0,   0,  90,  91,  88,  89,
  86,   0,   0,  93,  92,   0,   0,   0,  84, 344,
   0,   0, 102, 101,  97,  98,  99, 100, 103, 104,
 107, 108,   0, 335, 342,   0, 336, 343, 332,   0,
   0,   0,   0, 330, 337, 329,   0,   0,   0, 333,
   0,  94, 338,   0,   0, 334,   0,   0,   0, 331,
   0,   0,   0,   0,  87, 339,  95,   0,   0,   0,
   0,  90,  91,  88,  89,  86,   0,   0,  93,  92,
   0, 210, 209,  84,  77,   0,   0, 102, 101,  97,
  98,  99, 100, 103, 104, 107, 108,   0,  87, 143,
 142, 140, 141, 137, 138, 139,   0,  88,  89,  86,
   0,   0,  93,  92,   0,   0,  94,  84,  77,   0,
   0, 102, 101,  97,  98,  99, 100, 103, 104, 107,
 108,  95,  87,   0, 124,   0,  90,  91,   0,   0,
   0,  88,  89,  86,   0,   0,  93,  92,   0,   0,
  94,  84,  77,   0,   0, 102, 101,  97,  98,  99,
 100, 103, 104, 107, 108,  95,  87,   0, 124,   0,
  90,  91,   0,   0,   0,  88,  89,  86,   0,   0,
  93,  92,   0,   0,  94,  84,  77,   0,   0, 102,
 101,  97,  98,  99, 100, 103, 104, 107, 108,  95,
  87,   0, 312,   0,  90,  91,   0,   0,   0,  88,
  89,  86,   0,   0,  93,  92,   0,   0,  94, 187,
  77,   0,   0, 102, 101,  97,  98,  99, 100, 103,
 104, 107, 108,  95,  87,   0,   0,   0,  90,  91,
   0,   0,   0,  88,  89,  86,   0,   0,  93,  92,
   0,   0,  94, 185,  77,   0,   0, 102, 101,  97,
  98,  99, 100, 103, 104, 107, 108,  95,   0,   0,
   0,   0,  90,  91,   0,   0,   0,   0,   0,  76,
   0,   0,  77,  11,  12,   0,  94,   0,   0,   0,
   0,   0,   0,   0,   0,  28,   0,   0,  17,   0,
   0,  95,  24,   0,  30,  23,  90,  91,   0,  19,
  20,  32,   0,  18,   0,   0,  29,   8,   0,  31,
   9,  22,   0,  25,  10,  21,  26,  27,  11,  12,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  28,   0,   0,  17,   0,   0,   0,  24,   0,  30,
  23,   0,   0,   0,  19,  20,  32,   0,  18,  11,
  12,  29,   8,   0,  31,   9,  22,   0,  25,  10,
  21,  26,  27,   0,  17,   0,   0,   0,  24,   0,
   0,  23,   0,   0,   0,  19,  20,   0,   0,  18,
   0,   0,   0,   8,   0,   0,   9,  22,   0,  25,
  10,  21,  26,  27,  28,   0,   0,  17,   0,   0,
   0,  24,   0,  30,  23,   0,   0,   0,  19,  20,
  32,   0,  18,   0,   0,  29,   0,   0,  31,   0,
  22,   0,  25,   0,  21,  26,  27, 156, 157, 158,
 159, 160, 161, 163, 162, 164, 165, 166, 155, 380,
 154, 153, 152, 151, 150, 148, 149, 144, 145, 146,
 147, 143, 142, 140, 141, 137, 138, 139, 156, 157,
 158, 159, 160, 161, 163, 162, 164, 165, 166, 155,
   0, 154, 153, 152, 151, 150, 148, 149, 144, 145,
 146, 147, 143, 142, 140, 141, 137, 138, 139, 155,
   0, 154, 153, 152, 151, 150, 148, 149, 144, 145,
 146, 147, 143, 142, 140, 141, 137, 138, 139, 153,
 152, 151, 150, 148, 149, 144, 145, 146, 147, 143,
 142, 140, 141, 137, 138, 139, 152, 151, 150, 148,
 149, 144, 145, 146, 147, 143, 142, 140, 141, 137,
 138, 139, 151, 150, 148, 149, 144, 145, 146, 147,
 143, 142, 140, 141, 137, 138, 139, 150, 148, 149,
 144, 145, 146, 147, 143, 142, 140, 141, 137, 138,
 139, 148, 149, 144, 145, 146, 147, 143, 142, 140,
 141, 137, 138, 139, 144, 145, 146, 147, 143, 142,
 140, 141, 137, 138, 139
};
short	yypact[] =
{
-1000, 874,-1000, 318,-1000,-1000, 938, 874,  12,  12,
  -2,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000, 262,-1000, 149,-1000,-1000, 336,
-1000,-1000,-1000,-1000, 938, 938,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000, 336,-1000, 233, 829, 733,
 208,  -6, 938, -44,-1000, -44, -49,  58,-1000,-1000,
 874, 665,  -8, 280,-1000, 305, 177,-1000,-1000, -33,
-1000,1052,-1000,-1000, 373, 352, 733, 733, 733, 733,
 733, 733, 733, 733, 801, 767,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,  40,  33,-1000,-1000,-1000,
-1000,-1000,-1000, -56,-1000, 905,-1000,-1000,  27, 229,
 -53, 336,-1000,1052, 631,-1000, 829,-1000,-1000,-1000,
-1000,  65,  37,-1000, 733,  94,-1000, 733, 733, 733,
 733, 733, 733, 733, 733, 733, 733, 733, 733, 733,
 733, 733, 733, 733, 733, 733, 733, 733, 733, 733,
 733, 733, 733, 733, 733, 733, 733, 172,  92,1052,
 733, 733,  55,  55,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000, 373,-1000, 373,-1000,-1000,
-1000,-1000, 282,-1000,-1000, 202,-1000,  58,-1000,  58,
 733,-1000,-1000, 259,-1000, -64, 631, 277, 225, 733,
  55,-1000, 153, 829, 733,-1000, -15, -38,-1000,-1000,
-1000,-1000, 210, 210, 322, 322, 659, 659, 659, 659,
1158,1158,1147,1134,1120,1105,1089, 216,1052,1052,
1052,1052,1052,1052,1052,1052,1052,1052,1052, -18,
-1000,  90, 733,-1000, -21, 276,1052,  41,-1000,-1000,
 172, 172, 282, 337, 269,-1000,-1000, 207, 733,-1000,
  24,-1000,1052, 874,-1000, 336,-1000, 266, 225,-1000,
-1000, -39,-1000,-1000, -23, -40,-1000,-1000, 733, 699,
 170,-1000,-1000, 733,-1000, -24, -25, 329,-1000, 282,
 733,-1000,-1000, 287, 433,-1000,-1000,-1000,-1000,-1000,
1072,-1000, 631,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
 251,-1000,-1000,-1000, 324,-1000, 566, 315, -53, 160,
 151, 146, 501, 733,  95, 314, 313,  52,  80,  78,
-1000, 244, 733, 206, 201, -67,-1000, 336, 224,-1000,
-1000,-1000,-1000,-1000, 733, 733, 733,   3, 307, 733,
-1000,-1000, 304, 733, 733,1021,-1000,-1000,-1000,-1000,
 665,  91, 303,  88,  75,-1000,  87,-1000, -26, -27,
-1000,-1000, 501, 733, 501, 733, 501, 300, 299,  25,
 298,-1000,  45,-1000,-1000,-1000, 501, 733, 294,-1000,
 -28,-1000, 501,-1000
};
short	yypgo[] =
{
   0,  26, 184, 462,  14,  58, 216,  39, 452,  34,
  62, 233,  61,  19, 449,  24,  48,   3,  45,   6,
   1,  17,   0,  22, 446,   4,  12, 442, 441,  31,
 440, 437,  47, 434, 433,  11,   9,   5, 431,  15,
  25, 430,  18,  23, 414, 413,  33,   8,   2,   7,
 412, 409, 408, 129, 402, 400, 399, 397,  13, 385,
  16, 383, 382,  20, 376,  10, 375, 367, 366, 345,
 344, 342,  28, 340
};
short	yyr1[] =
{
   0,  50,  50,  51,  51,  54,  56,  51,  53,  57,
  53,  53,  32,  32,  33,  33,  33,  33,  27,  27,
  27,  37,  59,  37,  37,  55,  55,  60,  60,  62,
  61,  64,  61,  63,  63,  65,  65,  38,  38,  38,
  42,  42,  43,  43,  43,  44,  44,  44,  45,  45,
  45,  48,  48,  40,  40,  40,  41,  41,  41,  41,
  49,  49,  49,  15,  15,  16,  16,  16,  16,  16,
  19,  28,  28,  34,  34,  35,  35,  35,  20,  20,
  20,  36,  66,  36,  36,  36,  36,  36,  36,  36,
  36,  36,  36,  36,  36,  36,  17,  17,  46,  46,
  47,  21,  21,  22,  22,  22,  22,  22,  22,  22,
  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,
  22,  22,  22,  22,  22,  22,  22,  22,  22,  22,
  22,  22,  22,  22,  23,  23,  23,  29,  29,  29,
  29,  29,  29,  29,  29,  29,  29,  29,  24,  24,
  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,
  24,  24,  24,  24,  24,  24,  24,  24,  30,  30,
  31,  31,  25,  25,  26,  26,  67,  11,  52,  52,
  14,  14,  13,  13,  13,  13,  13,  10,   9,  58,
  12,  68,  12,  12,  12,  69,  12,  12,  12,  70,
  71,  12,  73,  12,  12,  12,   8,   8,   6,   6,
   7,   7,   2,   2,   2,  72,  72,  72,  72,   5,
   5,   5,   5,   5,   5,   5,   5,   5,   3,   3,
   3,   3,   3,   4,   4,  18,  39,   1,   1,   1
};
short	yyr2[] =
{
   0,   0,   2,   2,   3,   0,   0,   6,   1,   0,
   4,   3,   1,   3,   1,   3,   4,   4,   0,   3,
   4,   1,   0,   4,   3,   0,   4,   1,   3,   0,
   4,   0,   5,   0,   1,   1,   3,   1,   3,   2,
   0,   1,   2,   3,   1,   1,   4,   4,   2,   3,
   3,   1,   3,   3,   2,   2,   2,   3,   1,   2,
   1,   1,   2,   0,   1,   1,   2,   2,   3,   3,
   4,   0,   2,   1,   2,   3,   2,   2,   2,   1,
   2,   2,   0,   2,   5,   7,   9,   5,   7,   3,
   5,   2,   2,   3,   5,   5,   0,   1,   0,   1,
   1,   1,   3,   1,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   5,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   1,   5,   7,   1,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   3,   5,
   5,   4,   4,   3,   3,   2,   2,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,
   1,   2,   0,   1,   1,   3,   0,   4,   0,   1,
   1,   1,   1,   2,   1,   2,   3,   2,   1,   1,
   2,   0,   4,   2,   2,   0,   4,   2,   2,   0,
   0,   7,   0,   5,   1,   1,   1,   2,   1,   2,
   0,   2,   1,   1,   1,   1,   3,   2,   3,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1
};
short	yychk[] =
{
-1000, -50, -51, -52, -58, -13, -12,  -6,  78,  81,
  85,  44,  45,  -2,  -5,  -4,  -3,  59,  74,  70,
  71,  86,  82,  66,  63,  84,  87,  88,  56,  77,
  65,  80,  72,   4, -53, -32, -33,  34, -39,  42,
  -1,  43,  44,  45,  -6, -12,  -2,  -1, -11,  93,
  -1, -11,  -1,  93,   4,   5, -54, -57,  42,  41,
  -7, -32,  -6, -68, -67, -69, -70, -73, -53, -32,
 -55,   6, -15, -16, -18,  -9,  40,  43, -13, -46,
 -47, -22, -23, -29,  42, -24,  34,  23,  32,  33,
  95,  96,  38,  37,  75,  90, -18,  48,  49,  50,
  51,  47,  46,  52,  53, -30, -31,  54,  55, -32,
  -4,  91, -11, -61, -10,  -7, -11,  93, -72,  43,
 -56, -58, -48, -22,  93,  91,   5, -42, -32, -43,
  34, -44,  42, -45,  41,  40,  92,  34,  35,  36,
  32,  33,  31,  30,  26,  27,  28,  29,  24,  25,
  23,  22,  21,  20,  19,  17,   6,   7,   8,   9,
  10,  11,  13,  12,  14,  15,  16,  -9, -21, -22,
  42,  41,  39,  40,  38,  37, -23, -23, -23, -23,
 -23, -23, -23, -23, -29,  42, -29,  42,  54,  55,
 -10,  94, -62, -14, -12,  -8,  -5, -71,  94,   5,
   6, -19,  93, -60, -32, -49, -41, -48, -40,  41,
  40, -16,  -7,  42,  41,  91, -43, -46,  40, -22,
 -22, -22, -22, -22, -22, -22, -22, -22, -22, -22,
 -22, -22, -22, -22, -22, -22, -22, -21, -22, -22,
 -22, -22, -22, -22, -22, -22, -22, -22, -22, -42,
  34,  42,   5,  91, -25, -26, -22, -21,  -1,  -1,
  -9,  -9, -64, -63, -65, -38, -32, -39,  18,  -5,
 -72, -72, -22, -27,   4,   5,  94, -48, -40,   5,
   6, -47,  -1, -43, -15, -46,  91,  92,  18,  91,
  -7, -21,  91,   5,  92, -42, -42, -63,   4,   5,
  18, -47,  94, -58, -28, -60,   5,  92,  91,  92,
 -22, -23,  93, -26,  91,  91,   4, -65, -47,   4,
 -37, -32,  94, -20,   2, -36, -34, -17, -66,  69,
  67,  83,  62,  73,  79,  57,  60,  68,  76,  89,
 -35, -21,  58,  61,  43, -49,   4,   5, -59,   4,
 -35, -36,   4, -19,  42,  42,  42, -20, -17,  42,
   4,   4,  43,  42,  42, -22,  18,  18,  94, -37,
   6, -21, -17, -21,  83,   4, -21,   4, -25, -25,
  18, -48,  91,   4,  91,  42,  91,  91,  91, -20,
 -17, -20, -21, -20,   4,   4,  64,   4,  91, -20,
 -17,   4,  91, -20
};
short	yydef[] =
{
   1,  -2,   2,   0, 179, 189, 182, 184,   0,   0,
   0, 204, 205, 208, 212, 213, 214, 219, 220, 221,
 222, 223, 224, 225, 226, 227, 233, 234, 228, 229,
 230, 231, 232,   3,   0,  -2,  12, 210,  14,   0,
 236, 237, 238, 239, 183, 185, 209,  -2, 193, 176,
  -2, 197,  -2, 202,   4,   0,  25,   0,  63,  98,
   0,   0, 186,   0, 210,   0,   0,   0,  11,  -2,
   6,   0,   0,  64,  65,  40,   0, 235, 188,   0,
  99, 100, 103, 134,   0, 137,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 157, 158, 159, 160,
 161, 162, 163, 164, 165, 166, 167, 168, 170,  13,
 211,  15, 192, 210,  29,   0, 196, 200,   0, 215,
   0,   0,  10,  51,   0,  16,   0,  66,  67,  41,
 210,  44,   0,  45,  98,   0,  17,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,  40,   0, 101,
 172,   0,   0,   0, 155, 156, 138, 139, 140, 141,
 142, 143, 144, 145, 146,   0, 147,   0, 169, 171,
  31, 177,  33, 187, 180, 181, 206,   0, 203, 217,
   0,   7,  18,   0,  27,   0,  60,  61,  58,   0,
   0,  69,  42,  63,  98,  48,   0,   0,  68, 104,
 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
 115, 116, 117, 118, 119, 120, 121,   0, 123, 124,
 125, 126, 127, 128, 129, 130, 131, 132, 133,   0,
 210,   0,   0, 148,   0, 173, 174,   0, 153, 154,
  40,  40,  33,   0,  34,  35,  37,  14,   0, 207,
   0, 218, 216,  71,  26,   0,  52,  62,  59,  56,
  55,   0,  54,  43,   0,   0,  50,  49,   0,   0,
  42, 102, 151,   0, 152,   0,   0,   0,  30,   0,
   0,  39, 201,   0,  -2,  28,  57,  53,  46,  47,
 122, 135,   0, 175, 149, 150,  32,  36,  38,  19,
   0,  -2,  70,  72,   0,  79,  -2,   0,   0,   0,
   0,   0,  -2,  96,   0,   0,   0,   0,   0,   0,
  73,  97,   0,   0, 235,   0,  20,   0,   0,  78,
  74,  80,  81,  83,   0,  96,   0,   0,   0,   0,
  91,  92,   0, 172, 172,   0,  76,  77, 136,  24,
   0,   0,   0,   0,   0,  89,   0,  93,   0,   0,
  75,  23,  -2,  96,  -2,   0,  -2,   0,   0,  84,
   0,  87,   0,  90,  94,  95,  -2,  96,   0,  85,
   0,  88,  -2,  86
};
short	yytok1[] =
{
   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  95,   0,   0,   0,  36,  23,   0,
  42,  91,  34,  32,   5,  33,  40,  35,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  18,   4,
  26,   6,  27,  17,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  41,   0,  92,  22,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  93,  21,  94,  96
};
short	yytok2[] =
{
   2,   3,   7,   8,   9,  10,  11,  12,  13,  14,
  15,  16,  19,  20,  24,  25,  28,  29,  30,  31,
  37,  38,  39,  43,  44,  45,  46,  47,  48,  49,
  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
  90
};
long	yytok3[] =
{
   0
};
#define YYFLAG 		-1000
#define	yyclearin	yychar = -1
#define	yyerrok		yyerrflag = 0

#ifdef	yydebug
#include	"y.debug"
#else
#define	yydebug		0
char*	yytoknames[1];		/* for debugging */
char*	yystates[1];		/* for debugging */
#endif

/*	parser for yacc output	*/

int	yynerrs = 0;		/* number of errors */
int	yyerrflag = 0;		/* error recovery flag */

char*
yytokname(int yyc)
{
	static char x[10];

	if(yyc > 0 && yyc <= sizeof(yytoknames)/sizeof(yytoknames[0]))
	if(yytoknames[yyc-1])
		return yytoknames[yyc-1];
	sprint(x, "<%d>", yyc);
	return x;
}

char*
yystatname(int yys)
{
	static char x[10];

	if(yys >= 0 && yys < sizeof(yystates)/sizeof(yystates[0]))
	if(yystates[yys])
		return yystates[yys];
	sprint(x, "<%d>\n", yys);
	return x;
}

long
yylex1(void)
{
	long yychar;
	long *t3p;
	int c;

	yychar = yylex();
	if(yychar <= 0) {
		c = yytok1[0];
		goto out;
	}
	if(yychar < sizeof(yytok1)/sizeof(yytok1[0])) {
		c = yytok1[yychar];
		goto out;
	}
	if(yychar >= YYPRIVATE)
		if(yychar < YYPRIVATE+sizeof(yytok2)/sizeof(yytok2[0])) {
			c = yytok2[yychar-YYPRIVATE];
			goto out;
		}
	for(t3p=yytok3;; t3p+=2) {
		c = t3p[0];
		if(c == yychar) {
			c = t3p[1];
			goto out;
		}
		if(c == 0)
			break;
	}
	c = 0;

out:
	if(c == 0)
		c = yytok2[1];	/* unknown char */
	if(yydebug >= 3)
		fprint(2, "lex %.4lux %s\n", yychar, yytokname(c));
	return c;
}

int
yyparse(void)
{
	struct
	{
		YYSTYPE	yyv;
		int	yys;
	} yys[YYMAXDEPTH], *yyp, *yypt;
	short *yyxi;
	int yyj, yym, yystate, yyn, yyg;
	long yychar;
	YYSTYPE save1, save2;
	int save3, save4;

	save1 = yylval;
	save2 = yyval;
	save3 = yynerrs;
	save4 = yyerrflag;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyp = &yys[-1];
	goto yystack;

ret0:
	yyn = 0;
	goto ret;

ret1:
	yyn = 1;
	goto ret;

ret:
	yylval = save1;
	yyval = save2;
	yynerrs = save3;
	yyerrflag = save4;
	return yyn;

yystack:
	/* put a state and value onto the stack */
	if(yydebug >= 4)
		fprint(2, "char %s in %s", yytokname(yychar), yystatname(yystate));

	yyp++;
	if(yyp >= &yys[YYMAXDEPTH]) {
		yyerror("yacc stack overflow");
		goto ret1;
	}
	yyp->yys = yystate;
	yyp->yyv = yyval;

yynewstate:
	yyn = yypact[yystate];
	if(yyn <= YYFLAG)
		goto yydefault; /* simple state */
	if(yychar < 0)
		yychar = yylex1();
	yyn += yychar;
	if(yyn < 0 || yyn >= YYLAST)
		goto yydefault;
	yyn = yyact[yyn];
	if(yychk[yyn] == yychar) { /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if(yyerrflag > 0)
			yyerrflag--;
		goto yystack;
	}

yydefault:
	/* default state action */
	yyn = yydef[yystate];
	if(yyn == -2) {
		if(yychar < 0)
			yychar = yylex1();

		/* look through exception table */
		for(yyxi=yyexca;; yyxi+=2)
			if(yyxi[0] == -1 && yyxi[1] == yystate)
				break;
		for(yyxi += 2;; yyxi += 2) {
			yyn = yyxi[0];
			if(yyn < 0 || yyn == yychar)
				break;
		}
		yyn = yyxi[1];
		if(yyn < 0)
			goto ret0;
	}
	if(yyn == 0) {
		/* error ... attempt to resume parsing */
		switch(yyerrflag) {
		case 0:   /* brand new error */
			yyerror("syntax error");
			yynerrs++;
			if(yydebug >= 1) {
				fprint(2, "%s", yystatname(yystate));
				fprint(2, "saw %s\n", yytokname(yychar));
			}

		case 1:
		case 2: /* incompletely recovered error ... try again */
			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */
			while(yyp >= yys) {
				yyn = yypact[yyp->yys] + YYERRCODE;
				if(yyn >= 0 && yyn < YYLAST) {
					yystate = yyact[yyn];  /* simulate a shift of "error" */
					if(yychk[yystate] == YYERRCODE)
						goto yystack;
				}

				/* the current yyp has no shift onn "error", pop stack */
				if(yydebug >= 2)
					fprint(2, "error recovery pops state %d, uncovers %d\n",
						yyp->yys, (yyp-1)->yys );
				yyp--;
			}
			/* there is no state on the stack with an error shift ... abort */
			goto ret1;

		case 3:  /* no shift yet; clobber input char */
			if(yydebug >= 2)
				fprint(2, "error recovery discards %s\n", yytokname(yychar));
			if(yychar == YYEOFCODE)
				goto ret1;
			yychar = -1;
			goto yynewstate;   /* try again in the same state */
		}
	}

	/* reduction by production yyn */
	if(yydebug >= 2)
		fprint(2, "reduce %d in:\n\t%s", yyn, yystatname(yystate));

	yypt = yyp;
	yyp -= yyr2[yyn];
	yyval = (yyp+1)->yyv;
	yym = yyn;

	/* consult goto table to find next state */
	yyn = yyr1[yyn];
	yyg = yypgo[yyn];
	yyj = yyg + yyp->yys + 1;

	if(yyj >= YYLAST || yychk[yystate=yyact[yyj]] != -yyn)
		yystate = yyact[yyg];
	switch(yym) {
		
case 3:
#line	74	"cc.y"
{
		dodecl(xdecl, lastclass, lasttype, Z);
	} break;
case 5:
#line	79	"cc.y"
{
		lastdcl = T;
		firstarg = S;
		dodecl(xdecl, lastclass, lasttype, yypt[-0].yyv.node);
		if(lastdcl == T || lastdcl->etype != TFUNC) {
			diag(yypt[-0].yyv.node, "not a function");
			lastdcl = types[TFUNC];
		}
		thisfn = lastdcl;
		markdcl();
		firstdcl = dclstack;
		argmark(yypt[-0].yyv.node, 0);
	} break;
case 6:
#line	93	"cc.y"
{
		argmark(yypt[-2].yyv.node, 1);
	} break;
case 7:
#line	97	"cc.y"
{
		Node *n;

		n = revertdcl();
		if(n)
			yypt[-0].yyv.node = new(OLIST, n, yypt[-0].yyv.node);
		if(!debug['a'])
			codgen(yypt[-0].yyv.node, yypt[-4].yyv.node);
	} break;
case 8:
#line	109	"cc.y"
{
		dodecl(xdecl, lastclass, lasttype, yypt[-0].yyv.node);
	} break;
case 9:
#line	113	"cc.y"
{
		yypt[-0].yyv.node = dodecl(xdecl, lastclass, lasttype, yypt[-0].yyv.node);
	} break;
case 10:
#line	117	"cc.y"
{
		doinit(yypt[-3].yyv.node->sym, yypt[-3].yyv.node->type, 0L, yypt[-0].yyv.node);
	} break;
case 13:
#line	125	"cc.y"
{
		yyval.node = new(OIND, yypt[-0].yyv.node, Z);
		yyval.node->garb = simpleg(yypt[-1].yyv.lval);
	} break;
case 15:
#line	133	"cc.y"
{
		yyval.node = yypt[-1].yyv.node;
	} break;
case 16:
#line	137	"cc.y"
{
		yyval.node = new(OFUNC, yypt[-3].yyv.node, yypt[-1].yyv.node);
	} break;
case 17:
#line	141	"cc.y"
{
		yyval.node = new(OARRAY, yypt[-3].yyv.node, yypt[-1].yyv.node);
	} break;
case 18:
#line	149	"cc.y"
{
		yyval.node = Z;
	} break;
case 19:
#line	153	"cc.y"
{
		yyval.node = dodecl(adecl, lastclass, lasttype, Z);
		if(yypt[-2].yyv.node != Z)
			if(yyval.node != Z)
				yyval.node = new(OLIST, yypt[-2].yyv.node, yyval.node);
			else
				yyval.node = yypt[-2].yyv.node;
	} break;
case 20:
#line	162	"cc.y"
{
		yyval.node = yypt[-3].yyv.node;
		if(yypt[-1].yyv.node != Z) {
			yyval.node = yypt[-1].yyv.node;
			if(yypt[-3].yyv.node != Z)
				yyval.node = new(OLIST, yypt[-3].yyv.node, yypt[-1].yyv.node);
		}
	} break;
case 21:
#line	173	"cc.y"
{
		dodecl(adecl, lastclass, lasttype, yypt[-0].yyv.node);
		yyval.node = Z;
	} break;
case 22:
#line	178	"cc.y"
{
		yypt[-0].yyv.node = dodecl(adecl, lastclass, lasttype, yypt[-0].yyv.node);
	} break;
case 23:
#line	182	"cc.y"
{
		long w;

		w = yypt[-3].yyv.node->sym->type->width;
		yyval.node = doinit(yypt[-3].yyv.node->sym, yypt[-3].yyv.node->type, 0L, yypt[-0].yyv.node);
		yyval.node = contig(yypt[-3].yyv.node->sym, yyval.node, w);
	} break;
case 24:
#line	190	"cc.y"
{
		yyval.node = yypt[-2].yyv.node;
		if(yypt[-0].yyv.node != Z) {
			yyval.node = yypt[-0].yyv.node;
			if(yypt[-2].yyv.node != Z)
				yyval.node = new(OLIST, yypt[-2].yyv.node, yypt[-0].yyv.node);
		}
	} break;
case 27:
#line	207	"cc.y"
{
		dodecl(pdecl, lastclass, lasttype, yypt[-0].yyv.node);
	} break;
case 29:
#line	217	"cc.y"
{
		lasttype = yypt[-0].yyv.type;
	} break;
case 31:
#line	222	"cc.y"
{
		lasttype = yypt[-0].yyv.type;
	} break;
case 33:
#line	228	"cc.y"
{
		lastfield = 0;
		edecl(CXXX, lasttype, S);
	} break;
case 35:
#line	236	"cc.y"
{
		dodecl(edecl, CXXX, lasttype, yypt[-0].yyv.node);
	} break;
case 37:
#line	243	"cc.y"
{
		lastbit = 0;
		firstbit = 1;
	} break;
case 38:
#line	248	"cc.y"
{
		yyval.node = new(OBIT, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 39:
#line	252	"cc.y"
{
		yyval.node = new(OBIT, Z, yypt[-0].yyv.node);
	} break;
case 40:
#line	260	"cc.y"
{
		yyval.node = (Z);
	} break;
case 42:
#line	267	"cc.y"
{
		yyval.node = new(OIND, (Z), Z);
		yyval.node->garb = simpleg(yypt[-0].yyv.lval);
	} break;
case 43:
#line	272	"cc.y"
{
		yyval.node = new(OIND, yypt[-0].yyv.node, Z);
		yyval.node->garb = simpleg(yypt[-1].yyv.lval);
	} break;
case 46:
#line	281	"cc.y"
{
		yyval.node = new(OFUNC, yypt[-3].yyv.node, yypt[-1].yyv.node);
	} break;
case 47:
#line	285	"cc.y"
{
		yyval.node = new(OARRAY, yypt[-3].yyv.node, yypt[-1].yyv.node);
	} break;
case 48:
#line	291	"cc.y"
{
		yyval.node = new(OFUNC, (Z), Z);
	} break;
case 49:
#line	295	"cc.y"
{
		yyval.node = new(OARRAY, (Z), yypt[-1].yyv.node);
	} break;
case 50:
#line	299	"cc.y"
{
		yyval.node = yypt[-1].yyv.node;
	} break;
case 52:
#line	306	"cc.y"
{
		yyval.node = new(OINIT, invert(yypt[-1].yyv.node), Z);
	} break;
case 53:
#line	312	"cc.y"
{
		yyval.node = new(OARRAY, yypt[-1].yyv.node, Z);
	} break;
case 54:
#line	316	"cc.y"
{
		yyval.node = new(OELEM, Z, Z);
		yyval.node->sym = yypt[-0].yyv.sym;
	} break;
case 57:
#line	325	"cc.y"
{
		yyval.node = new(OLIST, yypt[-2].yyv.node, yypt[-1].yyv.node);
	} break;
case 59:
#line	330	"cc.y"
{
		yyval.node = new(OLIST, yypt[-1].yyv.node, yypt[-0].yyv.node);
	} break;
case 62:
#line	338	"cc.y"
{
		yyval.node = new(OLIST, yypt[-1].yyv.node, yypt[-0].yyv.node);
	} break;
case 63:
#line	343	"cc.y"
{
		yyval.node = Z;
	} break;
case 64:
#line	347	"cc.y"
{
		yyval.node = invert(yypt[-0].yyv.node);
	} break;
case 66:
#line	355	"cc.y"
{
		yyval.node = new(OPROTO, yypt[-0].yyv.node, Z);
		yyval.node->type = yypt[-1].yyv.type;
	} break;
case 67:
#line	360	"cc.y"
{
		yyval.node = new(OPROTO, yypt[-0].yyv.node, Z);
		yyval.node->type = yypt[-1].yyv.type;
	} break;
case 68:
#line	365	"cc.y"
{
		yyval.node = new(ODOTDOT, Z, Z);
	} break;
case 69:
#line	369	"cc.y"
{
		yyval.node = new(OLIST, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 70:
#line	375	"cc.y"
{
		yyval.node = invert(yypt[-1].yyv.node);
		if(yypt[-2].yyv.node != Z)
			yyval.node = new(OLIST, yypt[-2].yyv.node, yyval.node);
	} break;
case 71:
#line	382	"cc.y"
{
		yyval.node = Z;
	} break;
case 72:
#line	386	"cc.y"
{
		yyval.node = new(OLIST, yypt[-1].yyv.node, yypt[-0].yyv.node);
	} break;
case 74:
#line	393	"cc.y"
{
		yyval.node = new(OLIST, yypt[-1].yyv.node, yypt[-0].yyv.node);
	} break;
case 75:
#line	399	"cc.y"
{
		yyval.node = new(OCASE, yypt[-1].yyv.node, Z);
	} break;
case 76:
#line	403	"cc.y"
{
		yyval.node = new(OCASE, Z, Z);
	} break;
case 77:
#line	407	"cc.y"
{
		yyval.node = new(OLABEL, dcllabel(yypt[-1].yyv.sym, 1), Z);
	} break;
case 78:
#line	413	"cc.y"
{
		yyval.node = Z;
	} break;
case 80:
#line	418	"cc.y"
{
		yyval.node = new(OLIST, yypt[-1].yyv.node, yypt[-0].yyv.node);
	} break;
case 82:
#line	424	"cc.y"
{
		markdcl();
	} break;
case 83:
#line	428	"cc.y"
{
		yyval.node = revertdcl();
		if(yyval.node)
			yyval.node = new(OLIST, yyval.node, yypt[-0].yyv.node);
		else
			yyval.node = yypt[-0].yyv.node;
	} break;
case 84:
#line	436	"cc.y"
{
		yyval.node = new(OIF, yypt[-2].yyv.node, new(OLIST, yypt[-0].yyv.node, Z));
	} break;
case 85:
#line	440	"cc.y"
{
		yyval.node = new(OIF, yypt[-4].yyv.node, new(OLIST, yypt[-2].yyv.node, yypt[-0].yyv.node));
	} break;
case 86:
#line	444	"cc.y"
{
		yyval.node = new(OFOR, new(OLIST, yypt[-4].yyv.node, new(OLIST, yypt[-6].yyv.node, yypt[-2].yyv.node)), yypt[-0].yyv.node);
	} break;
case 87:
#line	448	"cc.y"
{
		yyval.node = new(OWHILE, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 88:
#line	452	"cc.y"
{
		yyval.node = new(ODWHILE, yypt[-2].yyv.node, yypt[-5].yyv.node);
	} break;
case 89:
#line	456	"cc.y"
{
		yyval.node = new(ORETURN, yypt[-1].yyv.node, Z);
		yyval.node->type = thisfn->link;
	} break;
case 90:
#line	461	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->vconst = 0;
		yyval.node->type = types[TINT];
		yypt[-2].yyv.node = new(OSUB, yyval.node, yypt[-2].yyv.node);

		yyval.node = new(OCONST, Z, Z);
		yyval.node->vconst = 0;
		yyval.node->type = types[TINT];
		yypt[-2].yyv.node = new(OSUB, yyval.node, yypt[-2].yyv.node);

		yyval.node = new(OSWITCH, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 91:
#line	475	"cc.y"
{
		yyval.node = new(OBREAK, Z, Z);
	} break;
case 92:
#line	479	"cc.y"
{
		yyval.node = new(OCONTINUE, Z, Z);
	} break;
case 93:
#line	483	"cc.y"
{
		yyval.node = new(OGOTO, dcllabel(yypt[-1].yyv.sym, 0), Z);
	} break;
case 94:
#line	487	"cc.y"
{
		yyval.node = new(OUSED, yypt[-2].yyv.node, Z);
	} break;
case 95:
#line	491	"cc.y"
{
		yyval.node = new(OSET, yypt[-2].yyv.node, Z);
	} break;
case 96:
#line	496	"cc.y"
{
		yyval.node = Z;
	} break;
case 98:
#line	502	"cc.y"
{
		yyval.node = Z;
	} break;
case 100:
#line	509	"cc.y"
{
		yyval.node = new(OCAST, yypt[-0].yyv.node, Z);
		yyval.node->type = types[TLONG];
	} break;
case 102:
#line	517	"cc.y"
{
		yyval.node = new(OCOMMA, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 104:
#line	524	"cc.y"
{
		yyval.node = new(OMUL, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 105:
#line	528	"cc.y"
{
		yyval.node = new(ODIV, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 106:
#line	532	"cc.y"
{
		yyval.node = new(OMOD, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 107:
#line	536	"cc.y"
{
		yyval.node = new(OADD, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 108:
#line	540	"cc.y"
{
		yyval.node = new(OSUB, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 109:
#line	544	"cc.y"
{
		yyval.node = new(OASHR, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 110:
#line	548	"cc.y"
{
		yyval.node = new(OASHL, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 111:
#line	552	"cc.y"
{
		yyval.node = new(OLT, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 112:
#line	556	"cc.y"
{
		yyval.node = new(OGT, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 113:
#line	560	"cc.y"
{
		yyval.node = new(OLE, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 114:
#line	564	"cc.y"
{
		yyval.node = new(OGE, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 115:
#line	568	"cc.y"
{
		yyval.node = new(OEQ, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 116:
#line	572	"cc.y"
{
		yyval.node = new(ONE, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 117:
#line	576	"cc.y"
{
		yyval.node = new(OAND, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 118:
#line	580	"cc.y"
{
		yyval.node = new(OXOR, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 119:
#line	584	"cc.y"
{
		yyval.node = new(OOR, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 120:
#line	588	"cc.y"
{
		yyval.node = new(OANDAND, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 121:
#line	592	"cc.y"
{
		yyval.node = new(OOROR, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 122:
#line	596	"cc.y"
{
		yyval.node = new(OCOND, yypt[-4].yyv.node, new(OLIST, yypt[-2].yyv.node, yypt[-0].yyv.node));
	} break;
case 123:
#line	600	"cc.y"
{
		yyval.node = new(OAS, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 124:
#line	604	"cc.y"
{
		yyval.node = new(OASADD, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 125:
#line	608	"cc.y"
{
		yyval.node = new(OASSUB, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 126:
#line	612	"cc.y"
{
		yyval.node = new(OASMUL, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 127:
#line	616	"cc.y"
{
		yyval.node = new(OASDIV, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 128:
#line	620	"cc.y"
{
		yyval.node = new(OASMOD, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 129:
#line	624	"cc.y"
{
		yyval.node = new(OASASHL, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 130:
#line	628	"cc.y"
{
		yyval.node = new(OASASHR, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 131:
#line	632	"cc.y"
{
		yyval.node = new(OASAND, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 132:
#line	636	"cc.y"
{
		yyval.node = new(OASXOR, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 133:
#line	640	"cc.y"
{
		yyval.node = new(OASOR, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 135:
#line	647	"cc.y"
{
		yyval.node = new(OCAST, yypt[-0].yyv.node, Z);
		dodecl(NODECL, CXXX, yypt[-3].yyv.type, yypt[-2].yyv.node);
		yyval.node->type = lastdcl;
	} break;
case 136:
#line	653	"cc.y"
{
		yyval.node = new(OSTRUCT, yypt[-1].yyv.node, Z);
		dodecl(NODECL, CXXX, yypt[-5].yyv.type, yypt[-4].yyv.node);
		yyval.node->type = lastdcl;
	} break;
case 138:
#line	662	"cc.y"
{
		yyval.node = new(OIND, yypt[-0].yyv.node, Z);
	} break;
case 139:
#line	666	"cc.y"
{
		yyval.node = new(OADDR, yypt[-0].yyv.node, Z);
	} break;
case 140:
#line	670	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->vconst = 0;
		yyval.node->type = types[TINT];
		yypt[-0].yyv.node = new(OSUB, yyval.node, yypt[-0].yyv.node);

		yyval.node = new(OCONST, Z, Z);
		yyval.node->vconst = 0;
		yyval.node->type = types[TINT];
		yyval.node = new(OSUB, yyval.node, yypt[-0].yyv.node);
	} break;
case 141:
#line	682	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->vconst = 0;
		yyval.node->type = types[TINT];
		yyval.node = new(OSUB, yyval.node, yypt[-0].yyv.node);
	} break;
case 142:
#line	689	"cc.y"
{
		yyval.node = new(ONOT, yypt[-0].yyv.node, Z);
	} break;
case 143:
#line	693	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->vconst = -1;
		yyval.node->type = types[TINT];
		yyval.node = new(OXOR, yyval.node, yypt[-0].yyv.node);
	} break;
case 144:
#line	700	"cc.y"
{
		yyval.node = new(OPREINC, yypt[-0].yyv.node, Z);
	} break;
case 145:
#line	704	"cc.y"
{
		yyval.node = new(OPREDEC, yypt[-0].yyv.node, Z);
	} break;
case 146:
#line	708	"cc.y"
{
		yyval.node = new(OSIZE, yypt[-0].yyv.node, Z);
	} break;
case 147:
#line	712	"cc.y"
{
		yyval.node = new(OSIGN, yypt[-0].yyv.node, Z);
	} break;
case 148:
#line	718	"cc.y"
{
		yyval.node = yypt[-1].yyv.node;
	} break;
case 149:
#line	722	"cc.y"
{
		yyval.node = new(OSIZE, Z, Z);
		dodecl(NODECL, CXXX, yypt[-2].yyv.type, yypt[-1].yyv.node);
		yyval.node->type = lastdcl;
	} break;
case 150:
#line	728	"cc.y"
{
		yyval.node = new(OSIGN, Z, Z);
		dodecl(NODECL, CXXX, yypt[-2].yyv.type, yypt[-1].yyv.node);
		yyval.node->type = lastdcl;
	} break;
case 151:
#line	734	"cc.y"
{
		yyval.node = new(OFUNC, yypt[-3].yyv.node, Z);
		if(yypt[-3].yyv.node->op == ONAME)
		if(yypt[-3].yyv.node->type == T)
			dodecl(xdecl, CXXX, types[TINT], yyval.node);
		yyval.node->right = invert(yypt[-1].yyv.node);
	} break;
case 152:
#line	742	"cc.y"
{
		yyval.node = new(OIND, new(OADD, yypt[-3].yyv.node, yypt[-1].yyv.node), Z);
	} break;
case 153:
#line	746	"cc.y"
{
		yyval.node = new(ODOT, new(OIND, yypt[-2].yyv.node, Z), Z);
		yyval.node->sym = yypt[-0].yyv.sym;
	} break;
case 154:
#line	751	"cc.y"
{
		yyval.node = new(ODOT, yypt[-2].yyv.node, Z);
		yyval.node->sym = yypt[-0].yyv.sym;
	} break;
case 155:
#line	756	"cc.y"
{
		yyval.node = new(OPOSTINC, yypt[-1].yyv.node, Z);
	} break;
case 156:
#line	760	"cc.y"
{
		yyval.node = new(OPOSTDEC, yypt[-1].yyv.node, Z);
	} break;
case 158:
#line	765	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TINT];
		yyval.node->vconst = yypt[-0].yyv.vval;
	} break;
case 159:
#line	771	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TLONG];
		yyval.node->vconst = yypt[-0].yyv.vval;
	} break;
case 160:
#line	777	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TUINT];
		yyval.node->vconst = yypt[-0].yyv.vval;
	} break;
case 161:
#line	783	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TULONG];
		yyval.node->vconst = yypt[-0].yyv.vval;
	} break;
case 162:
#line	789	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TDOUBLE];
		yyval.node->fconst = yypt[-0].yyv.dval;
	} break;
case 163:
#line	795	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TFLOAT];
		yyval.node->fconst = yypt[-0].yyv.dval;
	} break;
case 164:
#line	801	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TVLONG];
		yyval.node->vconst = yypt[-0].yyv.vval;
	} break;
case 165:
#line	807	"cc.y"
{
		yyval.node = new(OCONST, Z, Z);
		yyval.node->type = types[TUVLONG];
		yyval.node->vconst = yypt[-0].yyv.vval;
	} break;
case 168:
#line	817	"cc.y"
{
		yyval.node = new(OSTRING, Z, Z);
		yyval.node->type = typ(TARRAY, types[TCHAR]);
		yyval.node->type->width = yypt[-0].yyv.sval.l + 1;
		yyval.node->cstring = yypt[-0].yyv.sval.s;
		yyval.node->sym = symstring;
		yyval.node->etype = TARRAY;
		yyval.node->class = CSTATIC;
	} break;
case 169:
#line	827	"cc.y"
{
		char *s;
		int n;

		n = yypt[-1].yyv.node->type->width - 1;
		s = alloc(n+yypt[-0].yyv.sval.l+MAXALIGN);

		memcpy(s, yypt[-1].yyv.node->cstring, n);
		memcpy(s+n, yypt[-0].yyv.sval.s, yypt[-0].yyv.sval.l);
		s[n+yypt[-0].yyv.sval.l] = 0;

		yyval.node = yypt[-1].yyv.node;
		yyval.node->type->width += yypt[-0].yyv.sval.l;
		yyval.node->cstring = s;
	} break;
case 170:
#line	845	"cc.y"
{
		yyval.node = new(OLSTRING, Z, Z);
		yyval.node->type = typ(TARRAY, types[TUSHORT]);
		yyval.node->type->width = yypt[-0].yyv.sval.l + sizeof(ushort);
		yyval.node->rstring = (ushort*)yypt[-0].yyv.sval.s;
		yyval.node->sym = symstring;
		yyval.node->etype = TARRAY;
		yyval.node->class = CSTATIC;
	} break;
case 171:
#line	855	"cc.y"
{
		char *s;
		int n;

		n = yypt[-1].yyv.node->type->width - sizeof(ushort);
		s = alloc(n+yypt[-0].yyv.sval.l+MAXALIGN);

		memcpy(s, yypt[-1].yyv.node->rstring, n);
		memcpy(s+n, yypt[-0].yyv.sval.s, yypt[-0].yyv.sval.l);
		*(ushort*)(s+n+yypt[-0].yyv.sval.l) = 0;

		yyval.node = yypt[-1].yyv.node;
		yyval.node->type->width += yypt[-0].yyv.sval.l;
		yyval.node->rstring = (ushort*)s;
	} break;
case 172:
#line	872	"cc.y"
{
		yyval.node = Z;
	} break;
case 175:
#line	880	"cc.y"
{
		yyval.node = new(OLIST, yypt[-2].yyv.node, yypt[-0].yyv.node);
	} break;
case 176:
#line	886	"cc.y"
{
		yyval.tyty.t1 = strf;
		yyval.tyty.t2 = strl;
		strf = T;
		strl = T;
		lastbit = 0;
		firstbit = 1;
	} break;
case 177:
#line	895	"cc.y"
{
		yyval.type = strf;
		strf = yypt[-2].yyv.tyty.t1;
		strl = yypt[-2].yyv.tyty.t2;
	} break;
case 178:
#line	902	"cc.y"
{
		lastclass = CXXX;
		lasttype = types[TINT];
	} break;
case 180:
#line	910	"cc.y"
{
		yyval.tycl.t = yypt[-0].yyv.type;
		yyval.tycl.c = CXXX;
	} break;
case 181:
#line	915	"cc.y"
{
		yyval.tycl.t = simplet(yypt[-0].yyv.lval);
		yyval.tycl.c = simplec(yypt[-0].yyv.lval);
	} break;
case 182:
#line	922	"cc.y"
{
		yyval.tycl.t = yypt[-0].yyv.type;
		yyval.tycl.c = CXXX;
	} break;
case 183:
#line	927	"cc.y"
{
		yyval.tycl.t = yypt[-1].yyv.type;
		yyval.tycl.c = simplec(yypt[-0].yyv.lval);
		if(yypt[-0].yyv.lval & ~BCLASS & ~BGARB)
			diag(Z, "illegal combination of types 1: %Q/%T", yypt[-0].yyv.lval, yypt[-1].yyv.type);
	} break;
case 184:
#line	934	"cc.y"
{
		yyval.tycl.t = simplet(yypt[-0].yyv.lval);
		yyval.tycl.c = simplec(yypt[-0].yyv.lval);
		yyval.tycl.t = garbt(yyval.tycl.t, yypt[-0].yyv.lval);
	} break;
case 185:
#line	940	"cc.y"
{
		yyval.tycl.t = yypt[-0].yyv.type;
		yyval.tycl.c = simplec(yypt[-1].yyv.lval);
		yyval.tycl.t = garbt(yyval.tycl.t, yypt[-1].yyv.lval);
		if(yypt[-1].yyv.lval & ~BCLASS & ~BGARB)
			diag(Z, "illegal combination of types 2: %Q/%T", yypt[-1].yyv.lval, yypt[-0].yyv.type);
	} break;
case 186:
#line	948	"cc.y"
{
		yyval.tycl.t = yypt[-1].yyv.type;
		yyval.tycl.c = simplec(yypt[-2].yyv.lval|yypt[-0].yyv.lval);
		yyval.tycl.t = garbt(yyval.tycl.t, yypt[-2].yyv.lval|yypt[-0].yyv.lval);
		if((yypt[-2].yyv.lval|yypt[-0].yyv.lval) & ~BCLASS & ~BGARB || yypt[-0].yyv.lval & BCLASS)
			diag(Z, "illegal combination of types 3: %Q/%T/%Q", yypt[-2].yyv.lval, yypt[-1].yyv.type, yypt[-0].yyv.lval);
	} break;
case 187:
#line	958	"cc.y"
{
		yyval.type = yypt[-0].yyv.tycl.t;
		if(yypt[-0].yyv.tycl.c != CXXX)
			diag(Z, "illegal combination of class 4: %s", cnames[yypt[-0].yyv.tycl.c]);
		yyval.type = garbt(yyval.type, yypt[-1].yyv.lval);
	} break;
case 188:
#line	967	"cc.y"
{
		yyval.type = yypt[-0].yyv.tycl.t;
		if(yypt[-0].yyv.tycl.c != CXXX)
			diag(Z, "illegal combination of class 4: %s", cnames[yypt[-0].yyv.tycl.c]);
	} break;
case 189:
#line	975	"cc.y"
{
		lasttype = yypt[-0].yyv.tycl.t;
		lastclass = yypt[-0].yyv.tycl.c;
	} break;
case 190:
#line	982	"cc.y"
{
		dotag(yypt[-0].yyv.sym, TSTRUCT, 0);
		yyval.type = yypt[-0].yyv.sym->suetag;
	} break;
case 191:
#line	987	"cc.y"
{
		dotag(yypt[-0].yyv.sym, TSTRUCT, autobn);
	} break;
case 192:
#line	991	"cc.y"
{
		yyval.type = yypt[-2].yyv.sym->suetag;
		if(yyval.type->link != T)
			diag(Z, "redeclare tag: %s", yypt[-2].yyv.sym->name);
		yyval.type->link = yypt[-0].yyv.type;
		suallign(yyval.type);
	} break;
case 193:
#line	999	"cc.y"
{
		taggen++;
		sprint(symb, "_%d_", taggen);
		yyval.type = dotag(lookup(), TSTRUCT, autobn);
		yyval.type->link = yypt[-0].yyv.type;
		suallign(yyval.type);
	} break;
case 194:
#line	1007	"cc.y"
{
		dotag(yypt[-0].yyv.sym, TUNION, 0);
		yyval.type = yypt[-0].yyv.sym->suetag;
	} break;
case 195:
#line	1012	"cc.y"
{
		dotag(yypt[-0].yyv.sym, TUNION, autobn);
	} break;
case 196:
#line	1016	"cc.y"
{
		yyval.type = yypt[-2].yyv.sym->suetag;
		if(yyval.type->link != T)
			diag(Z, "redeclare tag: %s", yypt[-2].yyv.sym->name);
		yyval.type->link = yypt[-0].yyv.type;
		suallign(yyval.type);
	} break;
case 197:
#line	1024	"cc.y"
{
		taggen++;
		sprint(symb, "_%d_", taggen);
		yyval.type = dotag(lookup(), TUNION, autobn);
		yyval.type->link = yypt[-0].yyv.type;
		suallign(yyval.type);
	} break;
case 198:
#line	1032	"cc.y"
{
		dotag(yypt[-0].yyv.sym, TENUM, 0);
		yyval.type = yypt[-0].yyv.sym->suetag;
		if(yyval.type->link == T)
			yyval.type->link = types[TINT];
		yyval.type = yyval.type->link;
	} break;
case 199:
#line	1040	"cc.y"
{
		dotag(yypt[-0].yyv.sym, TENUM, autobn);
	} break;
case 200:
#line	1044	"cc.y"
{
		en.tenum = T;
		en.cenum = T;
	} break;
case 201:
#line	1049	"cc.y"
{
		yyval.type = yypt[-5].yyv.sym->suetag;
		if(yyval.type->link != T)
			diag(Z, "redeclare tag: %s", yypt[-5].yyv.sym->name);
		if(en.tenum == T) {
			diag(Z, "enum type ambiguous: %s", yypt[-5].yyv.sym->name);
			en.tenum = types[TINT];
		}
		yyval.type->link = en.tenum;
		yyval.type = en.tenum;
	} break;
case 202:
#line	1061	"cc.y"
{
		en.tenum = T;
		en.cenum = T;
	} break;
case 203:
#line	1066	"cc.y"
{
		yyval.type = en.tenum;
	} break;
case 204:
#line	1070	"cc.y"
{
		yyval.type = tcopy(yypt[-0].yyv.sym->type);
	} break;
case 205:
#line	1074	"cc.y"
{
		yyval.type = tcopy(yypt[-0].yyv.sym->type);
	} break;
case 207:
#line	1081	"cc.y"
{
		yyval.lval = typebitor(yypt[-1].yyv.lval, yypt[-0].yyv.lval);
	} break;
case 209:
#line	1088	"cc.y"
{
		yyval.lval = typebitor(yypt[-1].yyv.lval, yypt[-0].yyv.lval);
	} break;
case 210:
#line	1093	"cc.y"
{
		yyval.lval = 0;
	} break;
case 211:
#line	1097	"cc.y"
{
		yyval.lval = typebitor(yypt[-1].yyv.lval, yypt[-0].yyv.lval);
	} break;
case 215:
#line	1108	"cc.y"
{
		doenum(yypt[-0].yyv.sym, Z);
	} break;
case 216:
#line	1112	"cc.y"
{
		doenum(yypt[-2].yyv.sym, yypt[-0].yyv.node);
	} break;
case 219:
#line	1119	"cc.y"
{ yyval.lval = BCHAR; } break;
case 220:
#line	1120	"cc.y"
{ yyval.lval = BSHORT; } break;
case 221:
#line	1121	"cc.y"
{ yyval.lval = BINT; } break;
case 222:
#line	1122	"cc.y"
{ yyval.lval = BLONG; } break;
case 223:
#line	1123	"cc.y"
{ yyval.lval = BSIGNED; } break;
case 224:
#line	1124	"cc.y"
{ yyval.lval = BUNSIGNED; } break;
case 225:
#line	1125	"cc.y"
{ yyval.lval = BFLOAT; } break;
case 226:
#line	1126	"cc.y"
{ yyval.lval = BDOUBLE; } break;
case 227:
#line	1127	"cc.y"
{ yyval.lval = BVOID; } break;
case 228:
#line	1130	"cc.y"
{ yyval.lval = BAUTO; } break;
case 229:
#line	1131	"cc.y"
{ yyval.lval = BSTATIC; } break;
case 230:
#line	1132	"cc.y"
{ yyval.lval = BEXTERN; } break;
case 231:
#line	1133	"cc.y"
{ yyval.lval = BTYPEDEF; } break;
case 232:
#line	1134	"cc.y"
{ yyval.lval = BREGISTER; } break;
case 233:
#line	1137	"cc.y"
{ yyval.lval = BCONSTNT; } break;
case 234:
#line	1138	"cc.y"
{ yyval.lval = BVOLATILE; } break;
case 235:
#line	1142	"cc.y"
{
		yyval.node = new(ONAME, Z, Z);
		if(yypt[-0].yyv.sym->class == CLOCAL)
			yypt[-0].yyv.sym = mkstatic(yypt[-0].yyv.sym);
		yyval.node->sym = yypt[-0].yyv.sym;
		yyval.node->type = yypt[-0].yyv.sym->type;
		yyval.node->etype = TVOID;
		if(yyval.node->type != T)
			yyval.node->etype = yyval.node->type->etype;
		yyval.node->xoffset = yypt[-0].yyv.sym->offset;
		yyval.node->class = yypt[-0].yyv.sym->class;
		yypt[-0].yyv.sym->aused = 1;
	} break;
case 236:
#line	1157	"cc.y"
{
		yyval.node = new(ONAME, Z, Z);
		yyval.node->sym = yypt[-0].yyv.sym;
		yyval.node->type = yypt[-0].yyv.sym->type;
		yyval.node->etype = TVOID;
		if(yyval.node->type != T)
			yyval.node->etype = yyval.node->type->etype;
		yyval.node->xoffset = yypt[-0].yyv.sym->offset;
		yyval.node->class = yypt[-0].yyv.sym->class;
	} break;
	}
	goto yystack;  /* stack new state and value */
}
