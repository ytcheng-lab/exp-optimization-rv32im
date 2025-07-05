.section .data
.align 8
.global N
N: .word 512

.global i
i: .float [512個輸入數據]

.global o_g
o_g: .space 512*4

.global o_t
o_t: .space 512*4

.global buf
buf: .space 512*4
