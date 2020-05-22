	noop		this is test case for jalr instr.
	lw 0 1 num	load num into reg 1
	lw 0 5 addr	load address of addone into reg 5
	jalr 5 6	jump with jalr instruction.
	halt
addone	lw 0 7 one	load one into reg 7
	add 1 7 1	add 1 to reg 1
	jalr 6 5	jump to return address stored in %6
num	.fill 99
one	.fill 1
addr	.fill 5
