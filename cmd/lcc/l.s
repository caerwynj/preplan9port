.global main
.section ".text"
.align 4
main:
save %sp,-96,%sp
set 2,%o0
set .L2,%o1
set 12,%o2
call _write; nop
mov %g0,%i0
.L1:
ret; restore
.type main,#function
.size main,.-main
.section ".rodata"
.align 1
.L2:
.byte 104
.byte 101
.byte 108
.byte 108
.byte 111
.byte 32
.byte 119
.byte 111
.byte 114
.byte 108
.byte 100
.byte 10
.byte 0
