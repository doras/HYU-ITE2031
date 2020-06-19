    noop            this testcase is for branch hazard.
    lw 0 1 one      $1 = 1
    lw 0 2 two      $2 = 2
    lw 0 3 three    $3 = 3
    lw 0 4 four     $4 = 4
loop beq 1 2 done   if $1 == $2 then jump to done, else noop
    add 1 4 1       $1 = $1 + $4 = 1 + 4 = 5 // This instruction may be changed to noop when previous branch is taken.
    add 2 3 2       $2 = $2 + $3 = 2 + 3 = 5 // This instruction may be changed to noop when previous branch is taken.
    beq 0 0 loop    jump to loop, since $0 and $0 are always same as 0, the branch is always taken. // This instruction may be changed to noop when previous branch is taken.
done halt           This will not be accepted when previous branch 0 0 loop is taken. Only halt when loop beq 1 2 done instruction is taken.
one .fill 1
two .fill 2
three .fill 3
four .fill 4
