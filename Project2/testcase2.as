    lw 0 1 one      $1 = 1
    lw 0 2 two      $2 = 2
    lw 0 3 three    $3 = 3
    lw 0 4 four     $4 = 4
loop beq 1 2 done   if $1 == $2 then jump to done, else noop
    noop            insert noop to avoid branch hazard, 3 of them is minimum number needed because branching is decided in MEM stage.
    noop            insert noop to avoid branch hazard, 3 of them is minimum number needed because branching is decided in MEM stage.
    noop            insert noop to avoid branch hazard, 3 of them is minimum number needed because branching is decided in MEM stage.
    add 1 4 1       $1 = $1 + $4 = 1 + 4 = 5
    add 2 3 2       $2 = $2 + $3 = 2 + 3 = 5
    beq 0 0 loop    jump to loop, since $0 and $0 are always same as 0, the branch is always taken.
    noop            insert noop to avoid branch hazard, 3 of them is minimum number needed because branching is decided in MEM stage.
    noop            insert noop to avoid branch hazard, 3 of them is minimum number needed because branching is decided in MEM stage.
    noop            insert noop to avoid branch hazard, 3 of them is minimum number needed because branching is decided in MEM stage.
done halt
one .fill 1
two .fill 2
three .fill 3
four .fill 4
