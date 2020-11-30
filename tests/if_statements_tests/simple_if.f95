program MMMM
    INTEGER :: a
    a = 1 + 3
    IF (123) THEN
        PRINT *, 123
        if (123) THEN
            print *, 123
        else
            print *, 321
        end if
    END IF

    IF (0) THEN
        PRINT *, 1
    ELSE IF (2) THEN
        PRINT *, 2
    ELSE
        PRINT *, 3
    END IF
end
