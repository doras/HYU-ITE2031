	noop		this is test case for nor and sw instr.
	lw 0 1 one 	load with symbolic address
	lw 0 2 two 	load with symbolic address
	lw 0 3 three	load with symbolic address
	nor 1 2 4	~(%1 | %2) = ~(0b110 | 0b101) = ~7 = -8 -> %4
	sw 0 4 four	store with symbolic address
	nor 1 3 5	~(0b110 | 0b100) = ~6 = -7 -> %5
	sw 1 5 7	store with numeric address
	halt
one		.fill 6
two		.fill 5
three	.fill 4
four	.fill 999
five 	.fill 999
