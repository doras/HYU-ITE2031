	lw 0 1 five	load reg1 with 5(symbolic addr)
	lw 1 2 3	load reg1 with -1(numeric addr)
start 	add 1 2 1 	decrement reg1
	beq 0 1 65536	65536 doesn't fit in 16-bits so cause an error.
	beq 0 0		start go back to the beginning of the loop
	noop  	 	
done 	halt   		end of prog
five	.fill 5
neg1	.fill -1  
stAddr	.fill start
