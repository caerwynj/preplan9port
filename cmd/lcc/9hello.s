.global main
.section ".text"
.align 4
main:
save %sp,-96,%sp
set .L17,%o0
call print; nop
mov %g0,%i0
.L16:
ret; restore
.type main,#function
.size main,.-main
.section ".rodata"
.align 1
.L17:
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
