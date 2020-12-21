program MMMM
    INTEGER :: A
    CHARACTER(4) :: C

    a = 1 + 3
    print *, a, 40 + 1 + 1
    a = a + 1
    print *, a
    print *, 1 + a + 1

    print *, "THIS IS TEXT"
    print *, "WE CAN PRINT", " ", "TEXT"

    c = "AbCdEfG"
    print *, "THIS IS HOW CHARACTER(LEN=4) = AbCdEfG WORKS: c = ", c

    print *, "negative integers also supports a.e. ", -42

    print *, "also, operation priority "
    print *, "4 + 2 * 2 = ", 4 + 2 * 2
    print *, "4 + 2 > 4 + 1 = ", 4 + 2 > 4 + 1
end
