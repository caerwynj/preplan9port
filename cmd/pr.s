.section ".data"
.global Balance
.type Balance,#object
.size Balance,4
.align 4
Balance:
.word 0x0
.global Buffer
.type Buffer,#object
.size Buffer,4
.align 4
Buffer:
.word 0x0
.global C
.type C,#object
.size C,4
.align 4
C:
.word 0x0
.global Dblspace
.type Dblspace,#object
.size Dblspace,4
.align 4
Dblspace:
.word 0x1
.global err
.type err,#object
.size err,4
.align 4
err:
.word 0x0
.global error
.type error,#object
.size error,4
.align 4
error:
.word 0x0
.global Etabc
.type Etabc,#object
.size Etabc,4
.align 4
Etabc:
.word 0x9
.global Etabn
.type Etabn,#object
.size Etabn,4
.align 4
Etabn:
.word 0x0
.global Formfeed
.type Formfeed,#object
.size Formfeed,4
.align 4
Formfeed:
.word 0x0
.global Fpage
.type Fpage,#object
.size Fpage,4
.align 4
Fpage:
.word 0x1
.global Head
.type Head,#object
.size Head,4
.align 4
Head:
.word 0x0
.global Itabc
.type Itabc,#object
.size Itabc,4
.align 4
Itabc:
.word 0x9
.global Itabn
.type Itabn,#object
.size Itabn,4
.align 4
Itabn:
.word 0x0
.global Lasterr
.type Lasterr,#object
.size Lasterr,4
.align 4
Lasterr:
.word err
.global Len
.type Len,#object
.size Len,4
.align 4
Len:
.word 0x42
.global Linew
.type Linew,#object
.size Linew,4
.align 4
Linew:
.word 0x0
.global Lnumb
.type Lnumb,#object
.size Lnumb,4
.align 4
Lnumb:
.word 0x0
.global Margin
.type Margin,#object
.size Margin,4
.align 4
Margin:
.word 0xa
.global Multi
.type Multi,#object
.size Multi,4
.align 4
Multi:
.word 0x0
.global Ncols
.type Ncols,#object
.size Ncols,4
.align 4
Ncols:
.word 0x1
.global Nfiles
.type Nfiles,#object
.size Nfiles,4
.align 4
Nfiles:
.word 0x0
.global Nsepc
.type Nsepc,#object
.size Nsepc,4
.align 4
Nsepc:
.word 0x9
.global nulls
.type nulls,#object
.align 1
nulls:
.byte 0
.size nulls,1
.global Offset
.type Offset,#object
.size Offset,4
.align 4
Offset:
.word 0x0
.global Sepc
.type Sepc,#object
.size Sepc,4
.align 4
Sepc:
.word 0x0
.type .18,#object
.size .18,4
.align 4
.18:
.word 0x0
.section ".bss"
.type .19,#object
.size .19,116
.local .19
.common .19,116,4
.type .20,#object
.size .20,116
.local .20
.common .20,116,4
.global getdate
.section ".text"
.align 4
getdate:
save %sp,-96,%sp
set Nfiles,%i5
ld [%i5],%i5
cmp %i5,1; bg .L23; nop
set Files,%i5
ld [%i5],%i5
ld [%i5+4],%i5
set nulls,%i4
cmp %i5,%i4; bne .L21; nop
.L23:
set .18,%i5
ld [%i5],%i5
cmp %i5,%g0; bne .L24; nop
mov %g0,%o0
call time; nop
set .20+100,%i4
st %o0,[%i4]
set .20+100,%i5
ld [%i5],%o0
call ctime; nop
set .18,%i4
st %o0,[%i4]
.L24:
set .18,%i5
ld [%i5],%i0
ba .L17; nop
.L21:
set Files,%i5
ld [%i5],%i5
ld [%i5+4],%o0
set .19,%o1
call dirstat; nop
set .19+100,%i5
ld [%i5],%o0
call ctime; nop
mov %o0,%i0
.L17:
ret; restore
.type getdate,#function
.size getdate,.-getdate
.section ".bss"
.type .30,#object
.size .30,100
.local .30
.common .30,100,1
.global ffiler
.section ".text"
.align 4
ffiler:
save %sp,-96,%sp
set .30,%o0
set .L31,%o1
mov %i0,%o2
call sprint; nop
set .30,%i0
.L29:
ret; restore
.type ffiler,#function
.size ffiler,.-ffiler
.global main
.align 4
main:
save %sp,-216,%sp
mov %g0,%i5
set bout,%o0
set 1,%i4
mov %i4,%o1
mov %i4,%o2
call Binit; nop
set Files,%i4
add -120,%fp,%i3
st %i3,[%i4]
mov %i0,%o0
mov %i1,%o1
call findopt; nop
mov %o0,%i0
ba .L36; nop
.L33:
set Multi,%i4
ld [%i4],%i4
cmp %i4,109; bne .L37; nop
set Nfiles,%i4
ld [%i4],%i4
cmp %i4,9; bl .L39; nop
set .L41,%o0
call die; nop
.L39:
ld [%i1],%o0
set Nfiles,%i4
ld [%i4],%i3
add %i3,1,%i2
st %i2,[%i4]
smul %i3,12,%i4
set Files,%i3
ld [%i3],%i3
add %i4,%i3,%o1
call mustopen; nop
mov %o0,%i4
cmp %i4,%g0; bne .L38; nop
add %i5,1,%i5
ba .L38; nop
.L37:
ld [%i1],%o0
call pr; nop
cmp %o0,0; be .L44; nop
set Files,%i4
ld [%i4],%i4
ld [%i4],%o0
call Bterm; nop
.L44:
add %i5,1,%i5
.L38:
.L34:
sub %i0,1,%i0
add %i1,4,%i1
.L36:
cmp %i0,0; bg .L33; nop
cmp %i5,0; bne .L46; nop
set nulls,%o0
call pr; nop
.L46:
call errprint; nop
set error,%i3
ld [%i3],%i3
cmp %i3,0; be .L50; nop
set .L48,%i4
ba .L51; nop
.L50:
mov %g0,%i4
.L51:
mov %i4,%o0
call exits; nop
.L32:
ret; restore
.type main,#function
.size main,.-main
.global findopt
.align 4
findopt:
save %sp,-96,%sp
mov %i1,%i3
mov %g0,%i4
ba .L54; nop
.L53:
add %i1,4,%l7
mov %l7,%i1
ld [%l7],%l7
ldsb [%l7],%l7
mov %l7,%i5
mov %l7,%i2
cmp %i2,43; be .L62; nop
cmp %i2,45; be .L59; nop
ba .L56; nop
.L59:
ld [%i1],%l7
add %l7,1,%l7
st %l7,[%i1]
ldsb [%l7],%l7
mov %l7,%i5
cmp %l7,0; bne .L60; nop
ba .L57; nop
.L60:
.L62:
.L63:
set _ctype+1,%l7
ldub [%i5+%l7],%l7
and %l7,4,%l7
cmp %l7,0; be .L66; nop
ld [%i1],%l7
add %l7,-1,%l7
st %l7,[%i1]
mov %i1,%o0
call atoix; nop
set Ncols,%l6
st %o0,[%l6]
ba .L67; nop
.L66:
set _ctype+1,%l5
ldub [%i5+%l5],%l5
and %l5,1,%l5
cmp %l5,0; be .L74; nop
mov %i5,%o0
call tolower; nop
mov %o0,%l5
mov %l5,%l6
ba .L75; nop
.L74:
mov %i5,%l6
.L75:
mov %l6,%i5
mov %l6,%l7
cmp %l7,43; be .L76; nop
cmp %l7,43; bl .L69; nop
.L104:
cmp %l7,97; bl .L69; nop
cmp %l7,119; bg .L69; nop
sll %l7,2,%l5
set .L105-388,%l4
ld [%l5+%l4],%l5
jmp %l5; nop
.section ".rodata"
.align 4
.L105:
.word .L91
.word .L101
.word .L69
.word .L79
.word .L80
.word .L83
.word .L69
.word .L84
.word .L87
.word .L69
.word .L69
.word .L90
.word .L91
.word .L98
.word .L92
.word .L102
.word .L69
.word .L69
.word .L93
.word .L96
.word .L69
.word .L69
.word .L97
.section ".text"
.L76:
mov %i1,%o0
call atoix; nop
set Fpage,%l4
st %o0,[%l4]
cmp %o0,1; bge .L64; nop
set Fpage,%l5
set 1,%l4
st %l4,[%l5]
ba .L64; nop
.L79:
set Dblspace,%l5
set 2,%l4
st %l4,[%l5]
ba .L64; nop
.L80:
mov %i1,%o0
set Etabc,%o1
call intopt; nop
set Etabn,%l4
st %o0,[%l4]
cmp %o0,0; bge .L64; nop
set Etabn,%l5
set 8,%l4
st %l4,[%l5]
ba .L64; nop
.L83:
set Formfeed,%l5
ld [%l5],%l4
add %l4,1,%l4
st %l4,[%l5]
ba .L64; nop
.L84:
sub %i0,1,%l5
mov %l5,%i0
cmp %l5,0; ble .L64; nop
set Head,%l5
ld [%i1+4],%l4
st %l4,[%l5]
ba .L64; nop
.L87:
mov %i1,%o0
set Itabc,%o1
call intopt; nop
set Itabn,%l4
st %o0,[%l4]
cmp %o0,0; bge .L64; nop
set Itabn,%l5
set 8,%l4
st %l4,[%l5]
ba .L64; nop
.L90:
mov %i1,%o0
call atoix; nop
set Len,%l4
st %o0,[%l4]
ba .L64; nop
.L91:
set Multi,%l5
st %i5,[%l5]
ba .L64; nop
.L92:
mov %i1,%o0
call atoix; nop
set Offset,%l4
st %o0,[%l4]
ba .L64; nop
.L93:
ld [%i1],%l5
ldsb [%l5+1],%l5
set Sepc,%l4
st %l5,[%l4]
cmp %l5,0; be .L94; nop
ld [%i1],%l5
add %l5,1,%l5
st %l5,[%i1]
ba .L64; nop
.L94:
set Sepc,%l5
set 9,%l4
st %l4,[%l5]
ba .L64; nop
.L96:
set Margin,%l5
st %g0,[%l5]
ba .L64; nop
.L97:
mov %i1,%o0
call atoix; nop
set Linew,%l4
st %o0,[%l4]
ba .L64; nop
.L98:
set Lnumb,%l5
ld [%l5],%l4
add %l4,1,%l4
st %l4,[%l5]
mov %i1,%o0
set Nsepc,%o1
call intopt; nop
set Numw,%l4
st %o0,[%l4]
cmp %o0,0; bg .L99; nop
set Numw,%l5
set 5,%l4
st %l4,[%l5]
.L99:
.L101:
set Balance,%l5
set 1,%l4
st %l4,[%l5]
ba .L64; nop
.L102:
set Padodd,%l5
set 1,%l4
st %l4,[%l5]
ba .L64; nop
.L69:
set .L103,%o0
call die; nop
.L67:
.L64:
ld [%i1],%l7
add %l7,1,%l7
st %l7,[%i1]
ldsb [%l7],%l7
mov %l7,%i5
cmp %l7,0; bne .L63; nop
set Head,%l7
ld [%l7],%l7
ld [%i1+4],%l6
cmp %l7,%l6; bne .L54; nop
add %i1,4,%i1
ba .L54; nop
.L56:
.L57:
mov %i3,%l7
add %l7,4,%i3
ld [%i1],%l6
st %l6,[%l7]
add %i4,1,%i4
.L54:
sub %i0,1,%i2
mov %i2,%i0
cmp %i2,0; bg .L53; nop
set Len,%i2
ld [%i2],%i2
cmp %i2,0; bne .L109; nop
set Len,%i2
set 66,%l7
st %l7,[%i2]
.L109:
set Len,%i2
ld [%i2],%i2
set Margin,%l7
ld [%l7],%l7
cmp %i2,%l7; bg .L111; nop
set Margin,%i2
st %g0,[%i2]
.L111:
set Plength,%i2
set Len,%l7
ld [%l7],%l7
set Margin,%l6
ld [%l6],%l6
sra %l6,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %l6,2,%l6
sub %l7,%l6,%l7
st %l7,[%i2]
set Multi,%i2
ld [%i2],%i2
cmp %i2,109; bne .L113; nop
set Ncols,%i2
st %i4,[%i2]
.L113:
set Ncols,%i2
ld [%i2],%i2
cmp %i2,0; be .L117; nop
cmp %i2,1; be .L116; nop
ba .L115; nop
.L117:
set Ncols,%i2
set 1,%l7
st %l7,[%i2]
ba .L116; nop
.L115:
set Etabn,%i2
ld [%i2],%i2
cmp %i2,0; bne .L119; nop
set Etabn,%i2
set 8,%l7
st %l7,[%i2]
.L119:
.L116:
set Linew,%i2
ld [%i2],%i2
cmp %i2,0; bne .L121; nop
set Ncols,%l7
ld [%l7],%l7
cmp %l7,1; be .L124; nop
set Sepc,%l7
ld [%l7],%l7
cmp %l7,0; bne .L124; nop
set 72,%i2
ba .L125; nop
.L124:
set 512,%i2
.L125:
set Linew,%l7
st %i2,[%l7]
.L121:
set Lnumb,%l7
ld [%l7],%l7
cmp %l7,0; be .L126; nop
set Multi,%l6
ld [%l6],%l6
cmp %l6,109; bne .L129; nop
set Numw,%l6
ld [%l6],%l7
ba .L130; nop
.L129:
set Numw,%l6
ld [%l6],%l6
set Ncols,%l5
ld [%l5],%l5
smul %l5,%l6,%l7
.L130:
set Linew,%l6
ld [%l6],%l5
sub %l5,%l7,%l5
st %l5,[%l6]
.L126:
set Ncols,%l6
ld [%l6],%l6
set Linew,%l5
ld [%l5],%l5
sub %l5,%l6,%l5
add %l5,1,%l5
sra %l5,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %l5,%l6,%l6
set Colw,%l5
st %l6,[%l5]
cmp %l6,1; bge .L131; nop
set .L133,%o0
call die; nop
.L131:
set Ncols,%l6
ld [%l6],%l6
cmp %l6,1; be .L134; nop
set Multi,%l6
ld [%l6],%l6
cmp %l6,0; bne .L134; nop
set 1,%l5
set Plength,%l4
ld [%l4],%l4
set Dblspace,%l3
ld [%l3],%l3
sra %l4,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %l4,%l3,%l4
add %l4,1,%l4
set Linew,%l3
ld [%l3],%l3
add %l3,1,%l3
umul %l3,%l4,%l4
umul %l4,%l5,%l6
sll %l6,1,%o0
call getspace; nop
set Buffer,%l4
st %o0,[%l4]
set Bufend,%l5
sll %l6,1,%l4
set Buffer,%l3
ld [%l3],%l3
add %l4,%l3,%l4
st %l4,[%l5]
set 12,%l5
set Ncols,%l4
ld [%l4],%l4
add %l4,1,%l4
umul %l4,%l5,%o0
call getspace; nop
set Colpts,%l4
st %o0,[%l4]
.L134:
mov %i4,%i0
.L52:
ret; restore
.type findopt,#function
.size findopt,.-findopt
.global intopt
.align 4
intopt:
save %sp,-96,%sp
ld [%i0],%i4
ldsb [%i4+1],%i4
mov %i4,%i5
cmp %i4,%g0; be .L137; nop
set _ctype+1,%i4
ldub [%i5+%i4],%i4
and %i4,4,%i4
cmp %i4,%g0; bne .L137; nop
st %i5,[%i1]
ld [%i0],%i4
add %i4,1,%i4
st %i4,[%i0]
.L137:
mov %i0,%o0
call atoix; nop
mov %o0,%i5
cmp %i5,0; be .L141; nop
mov %i5,%i4
ba .L142; nop
.L141:
set -1,%i4
.L142:
mov %i4,%i0
.L136:
ret; restore
.type intopt,#function
.size intopt,.-intopt
.global pr
.align 4
pr:
save %sp,-96,%sp
mov %g0,%i5
mov %g0,%i4
set Multi,%i3
ld [%i3],%i3
cmp %i3,109; be .L144; nop
mov %i0,%o0
set Files,%i3
ld [%i3],%o1
call mustopen; nop
mov %o0,%i3
cmp %i3,%g0; bne .L144; nop
mov %g0,%i0
ba .L143; nop
.L144:
set Buffer,%i3
ld [%i3],%i3
cmp %i3,%g0; be .L146; nop
set Files,%i3
ld [%i3],%i3
ld [%i3],%o0
call Bungetc; nop
.L146:
set Lnumb,%i3
ld [%i3],%i3
cmp %i3,0; be .L148; nop
set Lnumb,%i3
set 1,%i2
st %i2,[%i3]
.L148:
set Page,%i3
st %g0,[%i3]
.L150:
set C,%i3
ld [%i3],%i3
cmp %i3,-1; bne .L154; nop
ba .L152; nop
.L154:
set Buffer,%i3
ld [%i3],%i3
cmp %i3,%g0; be .L156; nop
call nexbuf; nop
.L156:
mov %g0,%i3
set Inpos,%i2
st %i3,[%i2]
mov %i3,%o0
call get; nop
cmp %o0,-1; bne .L158; nop
ba .L152; nop
.L158:
set bout,%o0
call Bflush; nop
set Page,%i3
ld [%i3],%i2
add %i2,1,%i2
st %i2,[%i3]
ld [%i3],%i3
set Fpage,%i2
ld [%i2],%i2
cmp %i3,%i2; bl .L160; nop
set Margin,%i3
ld [%i3],%i3
cmp %i3,0; bne .L162; nop
ba .L151; nop
.L162:
mov %i5,%i3
cmp %i3,%g0; bne .L164; nop
call getdate; nop
mov %o0,%i5
.L164:
mov %i4,%i3
cmp %i3,%g0; bne .L166; nop
set Head,%i1
ld [%i1],%i1
cmp %i1,%g0; be .L170; nop
set Head,%i1
ld [%i1],%i3
ba .L171; nop
.L170:
set Nfiles,%i1
ld [%i1],%i1
cmp %i1,2; bge .L172; nop
set Files,%i1
ld [%i1],%i1
ld [%i1+4],%i2
ba .L173; nop
.L172:
set nulls,%i2
.L173:
mov %i2,%i3
.L171:
mov %i3,%i4
.L166:
set bout,%o0
set .L174,%o1
call Bprint; nop
set Nspace,%i1
set Offset,%l7
ld [%l7],%l7
st %l7,[%i1]
call putspace; nop
set bout,%o0
set .L175,%o1
add %i5,4,%o2
add %i5,24,%o3
mov %i4,%o4
set Page,%i1
ld [%i1],%o5
call Bprint; nop
.L160:
.L151:
call putpage; nop
ba .L150; nop
.L152:
set Padodd,%i3
ld [%i3],%i3
cmp %i3,0; be .L176; nop
set Page,%i3
ld [%i3],%i3
and %i3,1,%i3
cmp %i3,1; bne .L176; nop
set Line,%i2
st %g0,[%i2]
set Formfeed,%i2
ld [%i2],%i2
cmp %i2,%g0; be .L181; nop
set 12,%o0
call put; nop
ba .L179; nop
.L180:
set 10,%o0
call put; nop
.L181:
set Line,%i3
ld [%i3],%i3
set Len,%i2
ld [%i2],%i2
cmp %i3,%i2; bl .L180; nop
.L179:
.L176:
set C,%i3
st %g0,[%i3]
set 1,%i0
.L143:
ret; restore
.type pr,#function
.size pr,.-pr
.global putpage
.align 4
putpage:
save %sp,-96,%sp
set Line,%i4
set Margin,%i3
ld [%i3],%i3
sra %i3,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %i3,2,%i3
st %i3,[%i4]
.L184:
set Nspace,%i4
set Offset,%i3
ld [%i3],%i3
st %i3,[%i4]
mov %g0,%i5
set Outpos,%i3
st %g0,[%i3]
ba .L191; nop
.L188:
set Lnumb,%i4
ld [%i4],%i4
cmp %i4,0; be .L192; nop
set C,%i4
ld [%i4],%i4
cmp %i4,-1; be .L192; nop
cmp %i5,0; be .L194; nop
set Multi,%i4
ld [%i4],%i4
cmp %i4,97; bne .L192; nop
.L194:
set Page,%i4
ld [%i4],%i4
set Fpage,%i3
ld [%i3],%i3
cmp %i4,%i3; bl .L195; nop
call putspace; nop
set bout,%o0
set .L197,%o1
set Numw,%i3
ld [%i3],%o2
set Buffer,%i3
ld [%i3],%i3
cmp %i3,%g0; be .L199; nop
smul %i5,12,%i3
set Colpts,%i2
ld [%i2],%i2
add %i3,%i2,%i3
add %i3,8,%i3
ld [%i3],%i2
add %i2,1,%i1
st %i1,[%i3]
mov %i2,%i4
ba .L200; nop
.L199:
set Lnumb,%i3
ld [%i3],%i4
.L200:
mov %i4,%o3
call Bprint; nop
set Outpos,%i3
ld [%i3],%i2
set Numw,%i1
ld [%i1],%i1
add %i2,%i1,%i2
st %i2,[%i3]
set Nsepc,%i3
ld [%i3],%o0
call put; nop
.L195:
set Lnumb,%i4
ld [%i4],%i3
add %i3,1,%i3
st %i3,[%i4]
.L192:
set Lcolpos,%i3
st %g0,[%i3]
set Pcolpos,%i3
st %g0,[%i3]
ba .L204; nop
.L201:
set C,%i4
ld [%i4],%o0
call put; nop
.L202:
mov %i5,%o0
call get; nop
.L204:
set C,%i4
ld [%i4],%i4
cmp %i4,10; be .L206; nop
cmp %i4,12; be .L206; nop
cmp %i4,-1; bne .L201; nop
.L206:
set C,%i4
ld [%i4],%i4
cmp %i4,-1; be .L210; nop
add %i5,1,%i3
mov %i3,%i5
set Ncols,%i2
ld [%i2],%i2
cmp %i3,%i2; be .L210; nop
cmp %i4,10; bne .L207; nop
mov %i5,%o0
call get; nop
cmp %o0,-1; bne .L207; nop
.L210:
ba .L190; nop
.L207:
set Sepc,%i4
ld [%i4],%i4
cmp %i4,0; be .L211; nop
set Sepc,%i4
ld [%i4],%o0
call put; nop
ba .L212; nop
.L211:
set Nspace,%i4
ld [%i4],%i3
set Colw,%i2
ld [%i2],%i2
set Lcolpos,%i1
ld [%i1],%i1
sub %i2,%i1,%i2
add %i2,1,%i2
add %i3,%i2,%i3
st %i3,[%i4]
cmp %i3,1; bge .L213; nop
set Nspace,%i4
set 1,%i3
st %i3,[%i4]
.L213:
.L212:
.L189:
.L191:
set C,%i4
ld [%i4],%i4
cmp %i4,12; bne .L188; nop
.L190:
set C,%i4
ld [%i4],%i4
cmp %i4,-1; bne .L215; nop
cmp %i5,0; bne .L215; nop
set Margin,%i4
ld [%i4],%i4
cmp %i4,0; be .L183; nop
ba .L186; nop
.L215:
set C,%i4
ld [%i4],%i4
cmp %i4,12; bne .L219; nop
ba .L186; nop
.L219:
set 10,%o0
call put; nop
set Dblspace,%i4
ld [%i4],%i4
cmp %i4,2; bne .L221; nop
set Line,%i4
ld [%i4],%i4
set Plength,%i3
ld [%i3],%i3
cmp %i4,%i3; bge .L221; nop
set 10,%o0
call put; nop
.L221:
set Line,%i4
ld [%i4],%i4
set Plength,%i3
ld [%i3],%i3
cmp %i4,%i3; bl .L223; nop
ba .L186; nop
.L223:
.L185:
mov %g0,%o0
call get; nop
ba .L184; nop
.L186:
set Formfeed,%i4
ld [%i4],%i4
cmp %i4,0; be .L228; nop
set 12,%o0
call put; nop
ba .L226; nop
.L227:
set 10,%o0
call put; nop
.L228:
set Line,%i4
ld [%i4],%i4
set Len,%i3
ld [%i3],%i3
cmp %i4,%i3; bl .L227; nop
.L226:
.L183:
ret; restore
.type putpage,#function
.size putpage,.-putpage
.global nexbuf
.align 4
nexbuf:
save %sp,-96,%sp
set Buffer,%i0
ld [%i0],%i4
set Colpts,%i0
ld [%i0],%i5
mov %g0,%i2
.L231:
st %i4,[%i5]
st %i4,[%i5+4]
mov %i5,%i0
set Ncols,%l7
ld [%l7],%l7
smul %l7,12,%l7
set Colpts,%l6
ld [%l6],%l6
add %l7,%l6,%l7
cmp %i0,%l7; bne .L235; nop
ba .L230; nop
.L235:
mov %i5,%i0
add %i0,12,%i5
set Lnumb,%l7
ld [%l7],%l7
add %l7,%i2,%l7
st %l7,[%i0+8]
set Len,%i0
ld [%i0],%i0
set Margin,%l7
ld [%l7],%l7
sub %i0,%l7,%i0
set Dblspace,%l7
ld [%l7],%l7
sra %i0,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %i0,%l7,%i1
ba .L240; nop
.L237:
set Inpos,%i0
st %g0,[%i0]
.L241:
set Files,%i0
ld [%i0],%i0
ld [%i0],%o0
call Bgetrune; nop
mov %o0,%i3
cmp %o0,-1; bne .L245; nop
set 0xffffffff,%i0
sth %i0,[%i4]
ba .L250; nop
.L247:
st %i4,[%i5]
st %i4,[%i5+4]
.L248:
add %i5,12,%i5
.L250:
mov %i5,%i0
set Ncols,%l7
ld [%l7],%l7
smul %l7,12,%l7
set Colpts,%l6
ld [%l6],%l6
add %l7,%l6,%l7
cmp %i0,%l7; bleu .L247; nop
set Balance,%i0
ld [%i0],%i0
cmp %i0,0; be .L230; nop
mov %i2,%o0
call balance; nop
ba .L230; nop
.L245:
cmp %i3,32; bl .L253; nop
set Inpos,%i0
ld [%i0],%l7
add %l7,1,%l7
st %l7,[%i0]
.L253:
set Inpos,%i0
ld [%i0],%i0
set Colw,%l7
ld [%l7],%l7
cmp %i0,%l7; ble .L257; nop
cmp %i3,10; bne .L255; nop
.L257:
mov %i3,%i0
sth %i0,[%i4]
add %i4,2,%i0
mov %i0,%i4
set Bufend,%l7
ld [%l7],%l7
cmp %i0,%l7; blu .L258; nop
set .L260,%o0
call die; nop
.L258:
.L255:
cmp %i3,10; bne .L261; nop
ba .L243; nop
.L261:
cmp %i3,8; be .L265; nop
cmp %i3,8; bl .L241; nop
.L271:
cmp %i3,27; be .L268; nop
ba .L241; nop
.L265:
set Inpos,%i0
ld [%i0],%i0
cmp %i0,0; bne .L266; nop
add %i4,-2,%i4
.L266:
.L268:
set Inpos,%i0
ld [%i0],%i0
cmp %i0,0; ble .L241; nop
set Inpos,%i0
ld [%i0],%l7
sub %l7,1,%l7
st %l7,[%i0]
ba .L241; nop
.L243:
.L238:
add %i2,1,%i2
.L240:
sub %i1,1,%i0
mov %i0,%i1
cmp %i0,0; bge .L237; nop
ba .L231; nop
.L230:
ret; restore
.type nexbuf,#function
.size nexbuf,.-nexbuf
.global balance
.align 4
balance:
save %sp,-96,%sp
set Buffer,%l6
ld [%l6],%i5
set Colpts,%l6
ld [%l6],%i2
mov %g0,%i1
set Ncols,%l6
ld [%l6],%l6
sra %i0,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %i0,%l6,%g1
; smul %g1,%l6,%g1; sub %i0,%g1,%l7
add %i0,%l6,%l5
sub %l5,1,%l5
sra %l5,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %l5,%l6,%i3
mov %g0,%i0
.L273:
mov %g0,%i4
ba .L279; nop
.L280:
.L281:
mov %i5,%l6
add %l6,2,%i5
lduh [%l6],%l6
cmp %l6,10; bne .L280; nop
.L277:
add %i4,1,%i4
.L279:
cmp %i4,%i3; bl .L281; nop
add %i2,12,%l6
mov %l6,%i2
add %i0,%i3,%l5
mov %l5,%i0
set Lnumb,%l4
ld [%l4],%l4
add %l4,%l5,%l5
st %l5,[%l6+8]
st %i5,[%i2]
st %i5,[%i2+4]
add %i1,1,%l6
mov %l6,%i1
cmp %l6,%l7; bne .L283; nop
sub %i3,1,%i3
.L283:
.L274:
set Ncols,%l6
ld [%l6],%l6
sub %l6,1,%l6
cmp %i1,%l6; bl .L273; nop
.L272:
ret; restore
.type balance,#function
.size balance,.-balance
.section ".data"
.type .286,#object
.size .286,4
.align 4
.286:
.word 0x0
.global get
.section ".text"
.align 4
get:
save %sp,-96,%sp
set .286,%i2
ld [%i2],%i2
cmp %i2,0; be .L287; nop
set .286,%i2
st %g0,[%i2]
set Etabc,%i2
ld [%i2],%i5
ba .L288; nop
.L287:
set Buffer,%i2
ld [%i2],%i2
cmp %i2,%g0; be .L289; nop
smul %i0,12,%i2
set Colpts,%i1
ld [%i1],%i1
add %i2,%i1,%i3
ld [%i3],%i2
ld [%i3+16],%i1
cmp %i2,%i1; blu .L291; nop
set -1,%i5
ba .L290; nop
.L291:
ld [%i3],%i2
lduh [%i2],%i2
mov %i2,%i5
cmp %i2,-1; be .L290; nop
ld [%i3],%i2
add %i2,2,%i2
st %i2,[%i3]
ba .L290; nop
.L289:
set Multi,%i1
ld [%i1],%i1
cmp %i1,97; bne .L298; nop
mov %g0,%i2
ba .L299; nop
.L298:
mov %i0,%i2
.L299:
smul %i2,12,%i1
set Files,%l7
ld [%l7],%l7
add %i1,%l7,%i1
mov %i1,%i4
ld [%i1+8],%i1
mov %i1,%i5
cmp %i1,-1; bne .L295; nop
set Nfiles,%i1
ld [%i1],%i1
smul %i1,12,%i1
set Files,%l7
ld [%l7],%l7
add %i1,%l7,%i4
.L300:
.L301:
.L303:
add %i4,-12,%i1
mov %i1,%i4
set Files,%l7
ld [%l7],%l7
cmp %i1,%l7; blu .L304; nop
ld [%i4+8],%i1
cmp %i1,-1; be .L300; nop
.L304:
mov %i4,%i1
set Files,%l7
ld [%l7],%l7
cmp %i1,%l7; blu .L296; nop
set 10,%i5
ba .L296; nop
.L295:
ld [%i4],%o0
call Bgetrune; nop
st %o0,[%i4+8]
.L296:
.L290:
.L288:
set Etabn,%i1
ld [%i1],%i1
cmp %i1,0; be .L307; nop
set Etabc,%i1
ld [%i1],%i1
cmp %i5,%i1; bne .L307; nop
set Inpos,%i1
ld [%i1],%l7
add %l7,1,%l7
st %l7,[%i1]
set .286,%l7
ld [%i1],%i1
set Etabn,%l6
ld [%l6],%l6
sra %i1,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %i1,%l6,%g1
; smul %g1,%l6,%g1; sub %i1,%g1,%i1
st %i1,[%l7]
set 32,%i5
ba .L308; nop
.L307:
cmp %i5,32; bl .L309; nop
set Inpos,%i1
ld [%i1],%l7
add %l7,1,%l7
st %l7,[%i1]
ba .L310; nop
.L309:
cmp %i5,8; bl .L311; nop
cmp %i5,13; bg .L320; nop
sll %i5,2,%i1
set .L321-32,%l7
ld [%i1+%l7],%i1
jmp %i1; nop
.section ".rodata"
.align 4
.L321:
.word .L313
.word .L311
.word .L319
.word .L311
.word .L316
.word .L319
.section ".text"
.L320:
cmp %i5,27; be .L313; nop
ba .L311; nop
.L313:
set Inpos,%i1
ld [%i1],%i1
cmp %i1,0; ble .L312; nop
set Inpos,%i1
ld [%i1],%l7
sub %l7,1,%l7
st %l7,[%i1]
ba .L312; nop
.L316:
set Ncols,%i1
ld [%i1],%i1
cmp %i1,1; bne .L317; nop
ba .L312; nop
.L317:
set 10,%i5
.L319:
set Inpos,%i1
st %g0,[%i1]
.L311:
.L312:
.L310:
.L308:
set C,%i1
st %i5,[%i1]
mov %i5,%i0
.L285:
ret; restore
.type get,#function
.size get,.-get
.global put
.align 4
put:
save %sp,-96,%sp
cmp %i0,27; be .L334; nop
cmp %i0,27; bg .L344; nop
.L343:
cmp %i0,8; bl .L324; nop
cmp %i0,13; bg .L324; nop
sll %i0,2,%i4
set .L345-32,%i3
ld [%i4+%i3],%i4
jmp %i4; nop
.section ".rodata"
.align 4
.L345:
.word .L327
.word .L324
.word .L335
.word .L324
.word .L336
.word .L336
.section ".text"
.L344:
cmp %i0,32; be .L326; nop
ba .L324; nop
.L326:
set Nspace,%i4
ld [%i4],%i3
add %i3,1,%i3
st %i3,[%i4]
set Lcolpos,%i4
ld [%i4],%i3
add %i3,1,%i3
st %i3,[%i4]
ba .L323; nop
.L327:
set Lcolpos,%i4
ld [%i4],%i4
cmp %i4,0; bne .L328; nop
ba .L323; nop
.L328:
set Nspace,%i4
ld [%i4],%i4
cmp %i4,0; ble .L330; nop
set Nspace,%i4
ld [%i4],%i3
sub %i3,1,%i3
st %i3,[%i4]
set Lcolpos,%i4
ld [%i4],%i3
sub %i3,1,%i3
st %i3,[%i4]
ba .L323; nop
.L330:
set Lcolpos,%i4
ld [%i4],%i4
set Pcolpos,%i3
ld [%i3],%i3
cmp %i4,%i3; ble .L332; nop
set Lcolpos,%i4
ld [%i4],%i3
sub %i3,1,%i3
st %i3,[%i4]
ba .L323; nop
.L332:
.L334:
set -1,%i5
ba .L325; nop
.L335:
set Line,%i4
ld [%i4],%i3
add %i3,1,%i3
st %i3,[%i4]
.L336:
set Pcolpos,%i3
st %g0,[%i3]
set Lcolpos,%i3
st %g0,[%i3]
set Nspace,%i3
st %g0,[%i3]
set Outpos,%i3
st %g0,[%i3]
.L324:
cmp %i0,32; bl .L341; nop
set 1,%i3
ba .L342; nop
.L341:
mov %g0,%i3
.L342:
cmp %i3,0; be .L339; nop
set 1,%i4
ba .L340; nop
.L339:
mov %g0,%i4
.L340:
mov %i4,%i5
.L325:
set Page,%i4
ld [%i4],%i4
set Fpage,%i3
ld [%i3],%i3
cmp %i4,%i3; bge .L347; nop
ba .L323; nop
.L347:
set Lcolpos,%i3
ld [%i3],%i3
cmp %i3,%g0; bg .L351; nop
cmp %i5,%g0; ble .L349; nop
.L351:
set Lcolpos,%i4
ld [%i4],%i3
add %i3,%i5,%i3
st %i3,[%i4]
.L349:
set Lcolpos,%i4
ld [%i4],%i4
set Colw,%i3
ld [%i3],%i3
cmp %i4,%i3; bg .L352; nop
call putspace; nop
set bout,%o0
mov %i0,%o1
call Bputrune; nop
set Pcolpos,%i4
set Lcolpos,%i3
ld [%i3],%i3
st %i3,[%i4]
set Outpos,%i4
ld [%i4],%i3
add %i3,%i5,%i3
st %i3,[%i4]
.L352:
.L323:
ret; restore
.type put,#function
.size put,.-put
.global putspace
.align 4
putspace:
save %sp,-96,%sp
ba .L358; nop
.L355:
set Itabn,%i4
ld [%i4],%i4
cmp %i4,0; ble .L359; nop
set Outpos,%i3
ld [%i3],%i3
sra %i3,31,%g1; wr %g0,%g1,%y; nop; nop; nop; sdiv %i3,%i4,%g1
; smul %g1,%i4,%g1; sub %i3,%g1,%i3
sub %i4,%i3,%i4
mov %i4,%i5
set Nspace,%i3
ld [%i3],%i3
cmp %i3,%i4; bl .L359; nop
set bout,%o0
set Itabc,%i4
ld [%i4],%o1
call Bputc; nop
ba .L360; nop
.L359:
set 1,%i5
set bout,%o0
set 32,%o1
call Bputc; nop
.L360:
.L356:
set Outpos,%i4
ld [%i4],%i3
add %i3,%i5,%i3
st %i3,[%i4]
set Nspace,%i4
ld [%i4],%i3
sub %i3,%i5,%i3
st %i3,[%i4]
.L358:
set Nspace,%i4
ld [%i4],%i4
cmp %i4,0; bg .L355; nop
.L354:
ret; restore
.type putspace,#function
.size putspace,.-putspace
.global atoix
.align 4
atoix:
mov %g0,%o5
ba .L363; nop
.L362:
smul %o5,10,%o3
add %o3,%o4,%o3
sub %o3,48,%o5
.L363:
ld [%o0],%o3
add %o3,1,%o3
st %o3,[%o0]
ldsb [%o3],%o3
mov %o3,%o4
set _ctype+1,%o2
ldub [%o3+%o2],%o3
and %o3,4,%o3
cmp %o3,0; bne .L362; nop
ld [%o0],%o3
add %o3,-1,%o3
st %o3,[%o0]
mov %o5,%o0
.L361:
retl; nop
.type atoix,#function
.size atoix,.-atoix
.global mustopen
.align 4
mustopen:
save %sp,-96,%sp
ldsb [%i0],%i5
cmp %i5,0; bne .L367; nop
set nulls,%i5
st %i5,[%i1+4]
set 8244,%o0
call malloc; nop
st %o0,[%i1]
ld [%i1],%i5
cmp %i5,%g0; bne .L369; nop
set 2,%o0
set .L371,%o1
set .L372,%o2
call fprint; nop
.L369:
ld [%i1],%o0
mov %g0,%i5
mov %i5,%o1
mov %i5,%o2
call Binit; nop
ba .L368; nop
.L367:
st %i0,[%i1+4]
mov %i0,%o0
mov %g0,%o1
call Bopen; nop
st %o0,[%i1]
mov %o0,%i5
cmp %i5,%g0; bne .L373; nop
ld [%i1+4],%o0
call ffiler; nop
mov %o0,%i0
mov %i0,%o0
call strlen; nop
add %o0,1,%i5
mov %i5,%o0
call getspace; nop
mov %o0,%i5
mov %i0,%o1
call strcpy; nop
mov %o0,%i0
.L373:
.L368:
ld [%i1],%i5
cmp %i5,%g0; be .L375; nop
ld [%i1],%o0
call Bgetrune; nop
st %o0,[%i1+8]
cmp %o0,0; bge .L379; nop
set Multi,%i5
ld [%i5],%i5
cmp %i5,109; bne .L377; nop
.L379:
ld [%i1],%i0
ba .L366; nop
.L377:
ld [%i1+4],%o0
call strlen; nop
add %o0,1,%i5
add %i5,14,%i5
mov %i5,%o0
call getspace; nop
mov %o0,%i5
mov %i5,%i0
set .L380,%o1
ld [%i1+4],%o2
call sprint; nop
ld [%i1],%o0
call Bterm; nop
.L375:
set error,%i5
set 1,%i4
st %i4,[%i5]
set 2,%o0
set .L371,%o1
mov %i0,%o2
call fprint; nop
set 2,%o0
set .L381,%o1
call fprint; nop
mov %g0,%i0
.L366:
ret; restore
.type mustopen,#function
.size mustopen,.-mustopen
.global getspace
.align 4
getspace:
save %sp,-96,%sp
mov %i0,%o0
call malloc; nop
mov %o0,%i5
mov %o0,%i4
cmp %i4,%g0; bne .L383; nop
set .L385,%o0
call die; nop
.L383:
mov %i5,%i0
.L382:
ret; restore
.type getspace,#function
.size getspace,.-getspace
.global die
.align 4
die:
save %sp,-96,%sp
set error,%i5
ld [%i5],%i4
add %i4,1,%i4
st %i4,[%i5]
call errprint; nop
set 2,%o0
set .L371,%o1
mov %i0,%o2
call fprint; nop
set bout,%o0
set 10,%o1
call Bputc; nop
set .L48,%o0
call exits; nop
.L386:
ret; restore
.type die,#function
.size die,.-die
.global errprint
.align 4
errprint:
save %sp,-96,%sp
set bout,%o0
call Bflush; nop
ba .L391; nop
.L388:
set 2,%o0
set .L371,%o1
set err,%i5
ld [%i5],%i5
ld [%i5+4],%o2
call fprint; nop
set 2,%o0
set .L381,%o1
call fprint; nop
.L389:
set err,%i5
ld [%i5],%i4
ld [%i4],%i4
st %i4,[%i5]
.L391:
set err,%i5
ld [%i5],%i5
cmp %i5,%g0; bne .L388; nop
.L387:
ret; restore
.type errprint,#function
.size errprint,.-errprint
.section ".bss"
.global Plength
.type Plength,#object
.size Plength,4
.common Plength,4,4
.global Pcolpos
.type Pcolpos,#object
.size Pcolpos,4
.common Pcolpos,4,4
.global Page
.type Page,#object
.size Page,4
.common Page,4,4
.global Padodd
.type Padodd,#object
.size Padodd,4
.common Padodd,4,4
.global Outpos
.type Outpos,#object
.size Outpos,4
.common Outpos,4,4
.global Numw
.type Numw,#object
.size Numw,4
.common Numw,4,4
.global Nspace
.type Nspace,#object
.size Nspace,4
.common Nspace,4,4
.global Line
.type Line,#object
.size Line,4
.common Line,4,4
.global Lcolpos
.type Lcolpos,#object
.size Lcolpos,4
.common Lcolpos,4,4
.global Inpos
.type Inpos,#object
.size Inpos,4
.common Inpos,4,4
.global Files
.type Files,#object
.size Files,4
.common Files,4,4
.global Colw
.type Colw,#object
.size Colw,4
.common Colw,4,4
.global Colpts
.type Colpts,#object
.size Colpts,4
.common Colpts,4,4
.global Bufend
.type Bufend,#object
.size Bufend,4
.common Bufend,4,4
.global bout
.type bout,#object
.size bout,8244
.common bout,8244,4
.section ".rodata"
.align 1
.L385:
.byte 111
.byte 117
.byte 116
.byte 32
.byte 111
.byte 102
.byte 32
.byte 115
.byte 112
.byte 97
.byte 99
.byte 101
.byte 0
.align 1
.L381:
.byte 10
.byte 0
.align 1
.L380:
.byte 37
.byte 115
.byte 32
.byte 45
.byte 45
.byte 32
.byte 101
.byte 109
.byte 112
.byte 116
.byte 121
.byte 32
.byte 102
.byte 105
.byte 108
.byte 101
.byte 10
.byte 0
.align 1
.L372:
.byte 110
.byte 111
.byte 32
.byte 109
.byte 101
.byte 109
.byte 111
.byte 114
.byte 121
.byte 0
.align 1
.L371:
.byte 112
.byte 114
.byte 58
.byte 32
.byte 37
.byte 115
.byte 0
.align 1
.L260:
.byte 112
.byte 97
.byte 103
.byte 101
.byte 45
.byte 98
.byte 117
.byte 102
.byte 102
.byte 101
.byte 114
.byte 32
.byte 111
.byte 118
.byte 101
.byte 114
.byte 102
.byte 108
.byte 111
.byte 119
.byte 0
.align 1
.L197:
.byte 37
.byte 42
.byte 108
.byte 100
.byte 0
.align 1
.L175:
.byte 37
.byte 49
.byte 50
.byte 46
.byte 49
.byte 50
.byte 115
.byte 32
.byte 37
.byte 52
.byte 46
.byte 52
.byte 115
.byte 32
.byte 32
.byte 37
.byte 115
.byte 32
.byte 80
.byte 97
.byte 103
.byte 101
.byte 32
.byte 37
.byte 100
.byte 10
.byte 10
.byte 10
.byte 0
.align 1
.L174:
.byte 10
.byte 10
.byte 0
.align 1
.L133:
.byte 119
.byte 105
.byte 100
.byte 116
.byte 104
.byte 32
.byte 116
.byte 111
.byte 111
.byte 32
.byte 115
.byte 109
.byte 97
.byte 108
.byte 108
.byte 0
.align 1
.L103:
.byte 98
.byte 97
.byte 100
.byte 32
.byte 111
.byte 112
.byte 116
.byte 105
.byte 111
.byte 110
.byte 0
.align 1
.L48:
.byte 101
.byte 114
.byte 114
.byte 111
.byte 114
.byte 0
.align 1
.L41:
.byte 116
.byte 111
.byte 111
.byte 32
.byte 109
.byte 97
.byte 110
.byte 121
.byte 32
.byte 102
.byte 105
.byte 108
.byte 101
.byte 115
.byte 0
.align 1
.L31:
.byte 99
.byte 97
.byte 110
.byte 39
.byte 116
.byte 32
.byte 111
.byte 112
.byte 101
.byte 110
.byte 32
.byte 37
.byte 115
.byte 10
.byte 0
