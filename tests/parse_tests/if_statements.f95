program MMMM
    INTEGER :: a, x
    INTEGER :: t
    a = 1 + 3

    IF (123) THEN
        PRINT *, 123
        if (123) THEN
            t = 1111
            print *, 123
        else
            print *, 321
        end if
    END IF

    IF (101) THEN
        PRINT *, 1
    ELSE IF (2) THEN
        PRINT *, 2
    ELSE
        PRINT *, 3
    END IF

    IF (1) THEN
        x = 11
    ELSE IF (2) THEN
        print *, 1, 2, 3, 4
    END IF
end
