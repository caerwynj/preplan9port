.data
.align 4
curthread:
	.long 0
.text
.align 4
.globl getthread
getthread:
	movl	curthread,%eax
//	movl	%ebx,%eax
	ret

.globl setthread
setthread:
	movl	4(%esp),%ebx
	movl	%ebx,curthread
	ret
