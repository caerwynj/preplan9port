	.file	"test1.c"
	.version	"01.01"
.stabs "/mntx/huygens/sape/src/plan9/libthread/",100,0,0,.Ltext0
.stabs "test1.c",100,0,0,.Ltext0
.text
.Ltext0:
	.stabs	"gcc2_compiled.", 0x3c, 0, 0, 0
.stabs "int:t1=r1;-2147483648;2147483647;",128,0,0,0
.stabs "char:t2=r2;0;127;",128,0,0,0
.stabs "long int:t3=r1;-2147483648;2147483647;",128,0,0,0
.stabs "unsigned int:t4=r1;0;-1;",128,0,0,0
.stabs "long unsigned int:t5=r1;0;-1;",128,0,0,0
.stabs "long long int:t6=r1;01000000000000000000000;0777777777777777777777;",128,0,0,0
.stabs "long long unsigned int:t7=r1;0000000000000;01777777777777777777777;",128,0,0,0
.stabs "short int:t8=r1;-32768;32767;",128,0,0,0
.stabs "short unsigned int:t9=r1;0;65535;",128,0,0,0
.stabs "signed char:t10=r1;-128;127;",128,0,0,0
.stabs "unsigned char:t11=r1;0;255;",128,0,0,0
.stabs "float:t12=r1;4;0;",128,0,0,0
.stabs "double:t13=r1;8;0;",128,0,0,0
.stabs "long double:t14=r1;12;0;",128,0,0,0
.stabs "complex int:t15=s8real:1,0,32;imag:1,32,32;;",128,0,0,0
.stabs "complex float:t16=r16;4;0;",128,0,0,0
.stabs "complex double:t17=r17;8;0;",128,0,0,0
.stabs "complex long double:t18=r18;12;0;",128,0,0,0
.stabs "void:t19=19",128,0,0,0
.stabs "uint:t4",128,0,99,0
.stabs "uchar:t11",128,0,100,0
.stabs "ushort:t9",128,0,101,0
.stabs "ulong:t5",128,0,102,0
.stabs "vlong:t6",128,0,103,0
.stabs "uvlong:t7",128,0,104,0
.stabs "__gnuc_va_list:t20=*19",128,0,52,0
.stabs "va_list:t20",128,0,150,0
.stabs "Rune:t9",128,0,7,0
.stabs " :T21=eOREAD:0,OWRITE:1,ORDWR:2,\\",128,0,0,0
.stabs "ERRLEN:64,;",128,0,0,0
.stabs " :T22=eUTFmax:3,Runesync:128,Runeself:128,\\",128,0,0,0
.stabs "Runeerror:128,;",128,0,0,0
.stabs "Lock:t23=xsLock:",128,0,63,0
.stabs "Lock:T23=s4l:3,0,32;;",128,0,0,0
.stabs "Fconv:t24=xsFconv:",128,0,78,0
.stabs "Fconv:T24=s24out:25=*2,0,32;eout:25,32,32;\\",128,0,0,0
.stabs "f1:1,64,32;f2:1,96,32;f3:1,128,32;\\",128,0,0,0
.stabs "chr:1,160,32;;",128,0,0,0
.stabs "Waitmsg:T26=s112pid:27=ar1;0;11;2,0,96;time:28=ar1;0;35;2,96,288;\\",128,0,0,0
.stabs "msg:29=ar1;0;63;2,384,512;;",128,0,0,0
.stabs "Waitmsg:t26",128,0,136,0
.stabs "__jmp_buf:t30=ar1;0;5;1",128,0,13,0
.stabs "__sig_atomic_t:t1",128,0,23,0
.stabs "__sigset_t:t31=s128__val:32=ar1;0;31;5,0,1024;;",128,0,31,0
.stabs "__jmp_buf_tag:T33=s156__jmpbuf:30,0,192;__mask_was_saved:1,192,32;\\",128,0,0,0
.stabs "__saved_mask:31,224,1024;;",128,0,0,0
.stabs "jmp_buf:t34=ar1;0;0;33",128,0,43,0
.stabs "sigjmp_buf:t34",128,0,96,0
.stabs "Proc:t35=xsProc:",128,0,10,0
.stabs "Thread:t36=xsThread:",128,0,11,0
.stabs "Tqueue:t37=xsTqueue:",128,0,12,0
.stabs "Alt:t38=xsAlt:",128,0,13,0
.stabs "Channel:t39=xsChannel:",128,0,14,0
.stabs "Ref:t40=xsRef:",128,0,15,0
.stabs "Alt:T38=s20c:41=*39,0,32;v:20,32,32;\\",128,0,0,0
.stabs "op:1,64,32;tag:42=*41,96,32;q:5,128,32;;",128,0,0,0
.stabs "Ref:T40=s4ref:3,0,32;;",128,0,0,0
.stabs " :T43=eNone:0,Running:1,Runnable:2,\\",128,0,0,0
.stabs "Rendezvous:3,Exiting:4,;",128,0,0,0
.stabs "State:t43",128,0,10,0
.stabs " :T44=eCallnil:0,Callalt:1,Callsnd:2,\\",128,0,0,0
.stabs "Callrcv:3,;",128,0,0,0
.stabs "Callstate:t44",128,0,17,0
.stabs " :T45=eDOEXEC:1,DOEXIT:2,DOPROC:3,;",128,0,0,0
.stabs "Tqueue:T37=s12lock:23,0,32;head:46=*36,32,32;\\",128,0,0,0
.stabs "tail:46,64,32;;",128,0,0,0
.stabs "Thread:T36=s204lock:23,0,32;id:1,32,32;\\",128,0,0,0
.stabs "grp:1,64,32;state:43,96,32;call:44,128,32;\\",128,0,0,0
.stabs "cmdname:25,160,32;next:46,192,32;nextt:46,224,32;\\",128,0,0,0
.stabs "proc:47=*35,256,32;tag:5,288,32;alt:48=*38,320,32;\\",128,0,0,0
.stabs "value:5,352,32;env:34,384,1248;;",128,0,0,0
.stabs "Proc:T35=s216lock:23,0,32;pid:1,32,32;\\",128,0,0,0
.stabs "oldenv:34,64,1248;nthreads:1,1312,32;threads:37,1344,96;\\",128,0,0,0
.stabs "runnable:37,1440,96;curthread:46,1536,32;blocked:1,1568,32;\\",128,0,0,0
.stabs "nextID:4,1600,32;next:47,1632,32;arg:20,1664,32;\\",128,0,0,0
.stabs "udata:5,1696,32;;",128,0,0,0
.stabs "Newproc:T49=s20stack:50=*11,0,32;stacksize:4,32,32;\\",128,0,0,0
.stabs "stackptr:51=*5,64,32;launcher:5,96,32;grp:1,128,32;;",128,0,0,0
.stabs "Newproc:t49",128,0,72,0
.stabs "Execproc:T52=s8file:25,0,32;arg:53=*20,32,32;;",128,0,0,0
.stabs "Execproc:t52",128,0,77,0
.stabs "Pqueue:T54=s12lock:23,0,32;head:47,32,32;\\",128,0,0,0
.stabs "tail:47,64,32;;",128,0,0,0
.globl memset
.section	.rodata
.LC0:
	.string	"s_rthread starting, from %d to %d"
.LC1:
	.string	"alt send %d receive %d"
.LC2:
	.string	"Thread %lud received %lud\n"
.LC3:
	.string	"Thread %lud sent %lud\n"
.LC4:
	.string	"Alt error %r in thread %d"
.text
	.align 4
.stabs "altthread:F19",36,0,17,altthread
.stabs "xid:p20",160,0,17,8
.globl altthread
	.type	 altthread,@function
altthread:
.stabn 68,0,17,.LM1-altthread
.LM1:
	pushl %ebp
	movl %esp,%ebp
	subl $184,%esp
	pushl %edi
	pushl %esi
.stabn 68,0,18,.LM2-altthread
.LM2:
.LBB2:
	movl 8(%ebp),%eax
	movl %eax,-4(%ebp)
.stabn 68,0,19,.LM3-altthread
.LM3:
	movl -4(%ebp),%ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%eax
	movl $3,%ecx
	cltd
	idivl %ecx
	movl gc(,%edx,4),%ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%ecx
	movl %ecx,-8(%ebp)
.stabn 68,0,20,.LM4-altthread
.LM4:
	movl -4(%ebp),%ecx
	incl %ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%eax
	movl $3,%ecx
	cltd
	idivl %ecx
	movl gc(,%edx,4),%ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%ecx
	movl %ecx,-12(%ebp)
.stabn 68,0,22,.LM5-altthread
.LM5:
	leal -160(%ebp),%eax
	pushl $20
	pushl $0
	pushl %eax
	call memset
	addl $12,%esp
	movl -8(%ebp),%eax
	movl %eax,-160(%ebp)
	leal -16(%ebp),%ecx
	movl %ecx,-156(%ebp)
	movl $2,-152(%ebp)
	leal -140(%ebp),%edi
	leal -160(%ebp),%esi
	cld
	movl $5,%ecx
	rep
	movsl
	leal -180(%ebp),%eax
	pushl $20
	pushl $0
	pushl %eax
	call memset
	addl $12,%esp
	movl -12(%ebp),%eax
	movl %eax,-180(%ebp)
	leal -20(%ebp),%ecx
	movl %ecx,-176(%ebp)
	movl $2,-172(%ebp)
	leal -120(%ebp),%edi
	leal -180(%ebp),%esi
	cld
	movl $5,%ecx
	rep
	movsl
	leal -100(%ebp),%eax
	pushl $20
	pushl $0
	pushl %eax
	call memset
	addl $12,%esp
	movl $0,-100(%ebp)
	movl $0,-96(%ebp)
	movl $0,-92(%ebp)
	leal -80(%ebp),%edi
	leal -140(%ebp),%esi
	cld
	movl $15,%ecx
	rep
	movsl
.stabn 68,0,28,.LM6-altthread
.LM6:
	movl $0,-16(%ebp)
.stabn 68,0,29,.LM7-altthread
.LM7:
	movl -4(%ebp),%edx
	incl %edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	leal 0(,%eax,4),%edx
	addl %edx,%eax
	leal 0(,%eax,4),%edx
	movl %edx,-20(%ebp)
.stabn 68,0,30,.LM8-altthread
.LM8:
	movl -4(%ebp),%ecx
	incl %ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%eax
	movl $3,%ecx
	cltd
	idivl %ecx
	pushl %edx
	movl -4(%ebp),%ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%eax
	movl $3,%ecx
	cltd
	idivl %ecx
	pushl %edx
	pushl $.LC0
	pushl $1
	call _threaddebug
	addl $16,%esp
.stabn 68,0,32,.LM9-altthread
.LM9:
.L2:
	jmp .L4
	.align 4
	jmp .L3
	.align 4
.L4:
.stabn 68,0,33,.LM10-altthread
.LM10:
	movl -4(%ebp),%ecx
	incl %ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%eax
	movl $3,%ecx
	cltd
	idivl %ecx
	pushl %edx
	movl -4(%ebp),%ecx
	movl %ecx,-184(%ebp)
	movl -184(%ebp),%eax
	movl $3,%ecx
	cltd
	idivl %ecx
	pushl %edx
	pushl $.LC1
	pushl $1
	call _threaddebug
	addl $16,%esp
.stabn 68,0,34,.LM11-altthread
.LM11:
	leal -80(%ebp),%eax
	pushl %eax
	call alt
	addl $4,%esp
	movl %eax,%eax
	testl %eax,%eax
	je .L6
	cmpl $1,%eax
	je .L7
	jmp .L8
	.align 4
.L6:
.stabn 68,0,36,.LM12-altthread
.LM12:
	movl -16(%ebp),%eax
	pushl %eax
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC2
	call print
	addl $12,%esp
.stabn 68,0,37,.LM13-altthread
.LM13:
	jmp .L5
	.align 4
.L7:
.stabn 68,0,39,.LM14-altthread
.LM14:
	movl -20(%ebp),%eax
	pushl %eax
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC3
	call print
	addl $12,%esp
.stabn 68,0,40,.LM15-altthread
.LM15:
	jmp .L5
	.align 4
.L8:
.stabn 68,0,42,.LM16-altthread
.LM16:
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC4
	call fatal
	addl $8,%esp
.L5:
.stabn 68,0,44,.LM17-altthread
.LM17:
	jmp .L2
	.align 4
.L3:
.stabn 68,0,45,.LM18-altthread
.LM18:
	call abort
	.align 4
.stabn 68,0,46,.LM19-altthread
.LM19:
.LBE2:
.stabn 68,0,46,.LM20-altthread
.LM20:
.L1:
	leal -192(%ebp),%esp
	popl %esi
	popl %edi
	leave
	ret
.Lfe1:
	.size	 altthread,.Lfe1-altthread
.stabs "id:1",128,0,18,-4
.stabs "cfrom:41",128,0,19,-8
.stabs "cto:41",128,0,20,-12
.stabs "vin:5",128,0,21,-16
.stabs "vout:5",128,0,21,-20
.stabs "a:55=ar1;0;2;38",128,0,22,-80
.stabn 192,0,0,.LBB2-altthread
.stabn 224,0,0,.LBE2-altthread
.section	.rodata
.LC5:
	.string	"send %d"
.LC6:
	.string	"Send error %r in thread %d"
.LC7:
	.string	"receive %d"
.LC8:
	.string	"Receive error %r in thread %d"
.text
	.align 4
.stabs "s_rthread:F19",36,0,49,s_rthread
.stabs "xid:p20",160,0,49,8
.globl s_rthread
	.type	 s_rthread,@function
s_rthread:
.stabn 68,0,49,.LM21-s_rthread
.LM21:
	pushl %ebp
	movl %esp,%ebp
	subl $16,%esp
	pushl %esi
.stabn 68,0,50,.LM22-s_rthread
.LM22:
.LBB3:
.stabn 68,0,52,.LM23-s_rthread
.LM23:
	movl 8(%ebp),%eax
	movl %eax,-16(%ebp)
.stabn 68,0,54,.LM24-s_rthread
.LM24:
	movl -16(%ebp),%ecx
	movl %ecx,%eax
	movl $3,%esi
	cltd
	idivl %esi
	movl gc(,%edx,4),%ecx
	movl %ecx,-4(%ebp)
.stabn 68,0,55,.LM25-s_rthread
.LM25:
	movl -16(%ebp),%ecx
	incl %ecx
	movl %ecx,%eax
	movl $3,%esi
	cltd
	idivl %esi
	movl gc(,%edx,4),%ecx
	movl %ecx,-8(%ebp)
.stabn 68,0,56,.LM26-s_rthread
.LM26:
	movl -16(%ebp),%ecx
	incl %ecx
	movl %ecx,%eax
	movl $3,%esi
	cltd
	idivl %esi
	pushl %edx
	movl -16(%ebp),%ecx
	movl %ecx,%eax
	movl $3,%esi
	cltd
	idivl %esi
	pushl %edx
	pushl $.LC0
	pushl $1
	call _threaddebug
	addl $16,%esp
.stabn 68,0,57,.LM27-s_rthread
.LM27:
	movl -16(%ebp),%edx
	incl %edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	leal 0(,%eax,4),%edx
	addl %edx,%eax
	leal 0(,%eax,4),%edx
	movl %edx,-12(%ebp)
.stabn 68,0,59,.LM28-s_rthread
.LM28:
.L11:
	jmp .L13
	.align 4
	jmp .L12
	.align 4
.L13:
.stabn 68,0,60,.LM29-s_rthread
.LM29:
	movl -16(%ebp),%ecx
	incl %ecx
	movl %ecx,%eax
	movl $3,%esi
	cltd
	idivl %esi
	pushl %edx
	pushl $.LC5
	pushl $1
	call _threaddebug
	addl $12,%esp
.stabn 68,0,61,.LM30-s_rthread
.LM30:
	leal -12(%ebp),%eax
	pushl %eax
	movl -8(%ebp),%eax
	pushl %eax
	call send
	addl $8,%esp
	movl %eax,%eax
	cmpl $1,%eax
	je .L14
.stabn 68,0,62,.LM31-s_rthread
.LM31:
	movl -16(%ebp),%eax
	pushl %eax
	pushl $.LC6
	call fatal
	addl $8,%esp
.L14:
.stabn 68,0,64,.LM32-s_rthread
.LM32:
	movl -12(%ebp),%eax
	pushl %eax
	movl -16(%ebp),%eax
	pushl %eax
	pushl $.LC3
	call print
	addl $12,%esp
.stabn 68,0,65,.LM33-s_rthread
.LM33:
	movl -16(%ebp),%ecx
	movl %ecx,%eax
	movl $3,%esi
	cltd
	idivl %esi
	pushl %edx
	pushl $.LC7
	pushl $1
	call _threaddebug
	addl $12,%esp
.stabn 68,0,66,.LM34-s_rthread
.LM34:
	leal -12(%ebp),%eax
	pushl %eax
	movl -4(%ebp),%eax
	pushl %eax
	call recv
	addl $8,%esp
	movl %eax,%eax
	cmpl $1,%eax
	je .L15
.stabn 68,0,67,.LM35-s_rthread
.LM35:
	movl -16(%ebp),%eax
	pushl %eax
	pushl $.LC8
	call fatal
	addl $8,%esp
.L15:
.stabn 68,0,69,.LM36-s_rthread
.LM36:
	movl -12(%ebp),%eax
	pushl %eax
	movl -16(%ebp),%eax
	pushl %eax
	pushl $.LC2
	call print
	addl $12,%esp
.stabn 68,0,70,.LM37-s_rthread
.LM37:
	jmp .L11
	.align 4
.L12:
.stabn 68,0,71,.LM38-s_rthread
.LM38:
	call abort
	.align 4
.stabn 68,0,72,.LM39-s_rthread
.LM39:
.LBE3:
.stabn 68,0,72,.LM40-s_rthread
.LM40:
.L10:
	movl -20(%ebp),%esi
	leave
	ret
.Lfe2:
	.size	 s_rthread,.Lfe2-s_rthread
.stabs "cfrom:41",128,0,50,-4
.stabs "cto:41",128,0,50,-8
.stabs "v:5",128,0,51,-12
.stabs "id:1",128,0,52,-16
.stabn 192,0,0,.LBB3-s_rthread
.stabn 224,0,0,.LBE3-s_rthread
	.align 4
.stabs "testproc:F19",36,0,75,testproc
.stabs "xid:p20",160,0,75,8
.globl testproc
	.type	 testproc,@function
testproc:
.stabn 68,0,75,.LM41-testproc
.LM41:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
.stabn 68,0,76,.LM42-testproc
.LM42:
.LBB4:
.stabn 68,0,78,.LM43-testproc
.LM43:
	movl 8(%ebp),%eax
	movl %eax,-12(%ebp)
.stabn 68,0,80,.LM44-testproc
.LM44:
	call getthread
	movl %eax,%eax
	movl 32(%eax),%edx
	movl %edx,-8(%ebp)
.stabn 68,0,82,.LM45-testproc
.LM45:
	movl $1,-4(%ebp)
.L17:
	cmpl $2,-4(%ebp)
	jle .L20
	jmp .L18
	.align 4
.L20:
.stabn 68,0,83,.LM46-testproc
.LM46:
	pushl $524288
	movl -12(%ebp),%edx
	movl %edx,%eax
	addl %eax,%eax
	addl %edx,%eax
	movl %eax,%edx
	addl -4(%ebp),%edx
	pushl %edx
	pushl $s_rthread
	call threadcreate
	addl $12,%esp
.stabn 68,0,84,.LM47-testproc
.LM47:
	pushl $524288
	movl -12(%ebp),%edx
	movl %edx,%eax
	addl %eax,%eax
	addl %edx,%eax
	movl -4(%ebp),%edx
	addl $3,%edx
	addl %edx,%eax
	pushl %eax
	pushl $altthread
	call threadcreate
	addl $12,%esp
.stabn 68,0,82,.LM48-testproc
.LM48:
.L19:
	incl -4(%ebp)
	jmp .L17
	.align 4
.L18:
.stabn 68,0,86,.LM49-testproc
.LM49:
	pushl $524288
	movl -12(%ebp),%edx
	movl %edx,%eax
	addl %eax,%eax
	addl %edx,%eax
	pushl %eax
	pushl $s_rthread
	call threadcreate
	addl $12,%esp
.stabn 68,0,87,.LM50-testproc
.LM50:
	movl -12(%ebp),%edx
	movl %edx,%eax
	addl %eax,%eax
	addl %edx,%eax
	movl -4(%ebp),%edx
	addl $3,%edx
	addl %edx,%eax
	pushl %eax
	call altthread
	addl $4,%esp
.stabn 68,0,88,.LM51-testproc
.LM51:
	call abort
	.align 4
.stabn 68,0,89,.LM52-testproc
.LM52:
.LBE4:
.stabn 68,0,89,.LM53-testproc
.LM53:
.L16:
	leave
	ret
.Lfe3:
	.size	 testproc,.Lfe3-testproc
.stabs "i:1",128,0,76,-4
.stabs "p:47",128,0,77,-8
.stabs "id:1",128,0,78,-12
.stabn 192,0,0,.LBB4-testproc
.stabn 224,0,0,.LBE4-testproc
.section	.rodata
.LC9:
	.string	"Newchan returned nil"
.text
	.align 4
.stabs "main:F19",36,0,92,main
.stabs "argc:p1",160,0,92,8
.stabs "argv:p56=*25",160,0,92,12
.globl main
	.type	 main,@function
main:
.stabn 68,0,92,.LM54-main
.LM54:
	pushl %ebp
	movl %esp,%ebp
	subl $116,%esp
.stabn 68,0,93,.LM55-main
.LM55:
.LBB5:
.stabn 68,0,95,.LM56-main
.LM56:
	movl 12(%ebp),%eax
	movl (%eax),%edx
	movl %edx,progname
.stabn 68,0,97,.LM57-main
.LM57:
	movl $-1,_threaddebuglevel
.stabn 68,0,98,.LM58-main
.LM58:
	call threadinit
.stabn 68,0,100,.LM59-main
.LM59:
	movl $0,-4(%ebp)
.L22:
	cmpl $2,-4(%ebp)
	jle .L25
	jmp .L23
	.align 4
.L25:
.stabn 68,0,101,.LM60-main
.LM60:
	movl -4(%ebp),%eax
	xorb $1,%al
	movl %eax,%edx
	andl $1,%edx
	pushl %edx
	pushl $4
	call chancreate
	addl $8,%esp
	movl %eax,%eax
	movl -4(%ebp),%edx
	movl %eax,gc(,%edx,4)
	cmpl $0,gc(,%edx,4)
	jne .L24
.stabn 68,0,102,.LM61-main
.LM61:
	pushl $.LC9
	call fatal
	addl $4,%esp
.L26:
.stabn 68,0,100,.LM62-main
.LM62:
.L24:
	incl -4(%ebp)
	jmp .L22
	.align 4
.L23:
.stabn 68,0,105,.LM63-main
.LM63:
	pushl $0
	call testproc
	addl $4,%esp
.L27:
.stabn 68,0,118,.LM64-main
.LM64:
.LBE5:
.stabn 68,0,118,.LM65-main
.LM65:
.L21:
	leave
	ret
.Lfe4:
	.size	 main,.Lfe4-main
.stabs "i:1",128,0,93,-4
.stabs "w:26",128,0,94,-116
.stabn 192,0,0,.LBB5-main
.stabn 224,0,0,.LBE5-main
.stabs "gc:G57=ar1;0;2;41",32,0,13,0
	.comm	gc,12,4
.stabs "progname:G25",32,0,14,0
	.comm	progname,4,4
	.text
	.stabs "",100,0,0,.Letext
.Letext:
	.ident	"GCC: (GNU) 2.7.2.3"
