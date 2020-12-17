program MMMM
    INTEGER :: res

    res = 0
    IF (123) THEN
        res = res + 10
        if (123) THEN
            res = res + 10
        else
            print *, 321
        end if
    END IF

    IF (0) THEN
        PRINT *, 1
    ELSE IF (2) THEN
        res = res + 2
    ELSE
        PRINT *, 3
    END IF


    IF (0) THEN
        PRINT *, 1
    ELSE IF (0) THEN
        PRINT *, 2
    ELSE
        res = res + 3
    END IF

    PRINT *, res
end
