	lw 0 2 mplier	load mplier to register 2
	lw 0 3 end	load 0xffffbfff to register 3
	lw 0 4 mask	load 0xfffffffe to register 4
	lw 0 5 mcand	load mcand to register 5
	lw 0 6 one 	load 1 to register 6
	nor 2 0 2	invert mplier in register 2
	nor 4 2 7	nor(%4, %2) -> %7; e.g. ~(0xfffe | ~mplier) == (1 & mplier)
	beq 0 7 iszero	if(nor(%4, %2) == 0) goto iszero
	add 5 1 1	add %5 to %1
start	add 4 4 4	shift left %4
	add 4 6 4	add 1 to %4 for masking also LSB
	add 5 5 5  	shift left %5
	nor 4 2 7	nor(%4, %2) -> %7; e.g. ~(0xffef | ~mplier) == (0x10 & mplier)
	beq 0 7 iszero	if(nor(%4, %2) == 0) goto iszero
	add 5 1 1	else add %5 to %1
iszero	beq 3 4 exit	if(%4 == 0xffffbfff) goto exit
	beq 0 0 start	jump to start
exit	halt		halt the machine
	noop
mcand	.fill	32766
mplier	.fill	10383
one	.fill	1
end	.fill	-16385
mask	.fill	-2
