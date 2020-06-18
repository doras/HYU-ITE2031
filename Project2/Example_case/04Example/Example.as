	lw	0	2	one	
	lw	0	3	data1	
	lw	0	4	data2	Example Hazard case1
	add	2	4	5	
	noop
	noop
	noop
	add	3	4	5	Example Hazard case2
	add	3	5	6	
	nor	0	5	7       
	noop
	noop
	noop
loop	beq	1	2	done	Example Hazard case3
	nor	5	6	7
	nor	3	4	7	
	add	3	2	7	
	add	1	5	7	
	add	1	2	1	
	beq	0	0	loop	Example Hazard case3
done	halt				
one	.fill	1			
data1	.fill	5			
data2	.fill	10			
