	lw 0 1 five	load reg1 with 5(symbolic addr)
	add 1 1 0	add reg1 with 0(register zero)
	nor 1 1 0	not reg1
	sw 0 1 five	store reg1 at location of five(symbolic addr)
	jalr 2 2 jump to next instruction
	noop
	lw 0 1 five	load reg1 with 5(symbolic addr)
	lw 1 2 9	load reg1 with -1(numeric addr)
start 	add 1 2 1 	decrement reg1
	beq 0 1 2	goto end of program when reg1==0
	beq 0 0		start go back to the beginning of the loop
	noop  	 	
done 	halt   		end of prog
five	.fill 5
neg1	.fill -1  
stAddr	.fill start
