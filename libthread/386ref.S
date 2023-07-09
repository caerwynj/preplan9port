.text
.align 4
.globl _xinc
_xinc:
	movl	4(%esp),%edx
	lock
	incl	0(%edx)
	ret

.globl _xdec
_xdec:
	movl	4(%esp),%ecx
	lock
	decl	0(%ecx)
	jz	iszero
	jl	isless
	movl	$1, %eax
	ret
iszero:
	xorl	%eax,%eax
	ret
isless:
	movl	$-1, %eax
	ret
