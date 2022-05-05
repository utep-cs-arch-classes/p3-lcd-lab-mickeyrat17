	.arch msp430g2553
	.text

	.extern circle_color
	.extern black
	.extern red
	.extern green
	.extern sienna
max_state:
	.word 0x02
jump_table:
	.word case_0
	.word case_1
	.word case_2
	.word default

	.p2align 1,0
	.p2align 1,0
	.global change_color
change_color:
	cmp r12, &max_state
	jc default

	add r12, r12
	mov r0, r13
	mov jump_table(r12), r0
case_0:
	mov &black, &circle_color
	jmp out
case_1:
	mov &red, &circle_color
	jmp out
case_2:
	mov &green, &circle_color
	jmp out
default:
	mov &sienna, &circle_color
out:
	mov r13, r0
	pop r0
	
