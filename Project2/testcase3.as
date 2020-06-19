    lw 0 1 one      $1 = 1
    lw 0 2 two      $2 = 2
    noop            insert noop to avoid data hazard, 3 of them is minimum number needed because there is no internal forwarding in register file.
    noop            insert noop to avoid data hazard, 3 of them is minimum number needed because there is no internal forwarding in register file.
    noop            insert noop to avoid data hazard, 3 of them is minimum number needed because there is no internal forwarding in register file.
    add 1 2 3       $3 = $1 + $2 = 1 + 2 = 3
    noop            insert noop to avoid data hazard, 3 of them is minimum number needed because there is no internal forwarding in register file.
    noop            insert noop to avoid data hazard, 3 of them is minimum number needed because there is no internal forwarding in register file.
    noop            insert noop to avoid data hazard, 3 of them is minimum number needed because there is no internal forwarding in register file.
    nor 3 3 4       $4 = $3 nor $3 = not 3 = -4
    halt
one .fill 1
two .fill 2
