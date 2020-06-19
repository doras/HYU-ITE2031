    noop        this testcase is for data hazard.
    lw 0 1 one      $1 = 1
    add 1 1 2       $2 = $1 + $1 = 2 // This occurs load-use data hazard. So this instruction will be stalled by insert one bubble.
    nor 1 2 2       $2 = $1 nor $2 = 1 nor 2 = -4 // This occurs data two hazards. One is for register 1 as regA from lw instruction, another is for register 2 as regB from add instruction. Two forwarding is needed.
    add 2 2 2       $2 = $2 + $2 = -8 // This occurs data hazards. Register 2 is destination of previous add instruction and also destination of previous nor. But to use recent value, the register 2 value of nor instruction will be used(-4).
one .fill 1
two .fill 2
