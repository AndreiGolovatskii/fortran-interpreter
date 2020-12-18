program MMMM
    INTEGER :: a
    CHARACTER(4) :: c

    a = 1 + 3
    print *, a, 40 + 1 + 1
    a = a + 1
    print *, a
    print *, 1 + a + 1

    print *, "THIS IS TEXT"
    print *, "WE CAN PRINT", " ", "TEXT"

    c = "AbCdEfG"
    print *, "THIS IS HOW CHARACTER(LEN=4) = AbCdEfG WORKS: c = ", c
end
