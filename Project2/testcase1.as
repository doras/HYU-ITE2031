    lw 0 1 one      $1 = 1
    lw 0 2 two      $2 = 2
    lw 0 3 three    $3 = 3
    lw 0 4 four     $4 = 4
    add 1 1 1       $1 = $1 + $1 = 1 + 1 = 2
    add 2 2 2       $2 = $2 + $2 = 2 + 2 = 4
    nor 3 3 3       $3 = $3 nor $3 = not 3 = -4
    nor 4 4 4       $4 = $4 nor $4 = not 4 = -5
    sw 0 1 one      one <- 2
    sw 0 2 two      two <- 4
    sw 0 3 three    three <- -4
    sw 0 4 four     four <- -5
    halt
one .fill 1
two .fill 2
three .fill 3
four .fill 4
