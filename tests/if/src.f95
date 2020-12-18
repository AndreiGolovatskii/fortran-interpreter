program MMMM
    INTEGER :: a, b
    LOGICAL :: g
    CHARACTER(10) :: str1

    a = 2
    b = 3
    if (a > -1) THEN
        PRINT *, "a = 3 > -1"
    end if

    if (b < 0) THEN
        PRINT *, "b = 3 < ", 0
    else if (b < 4) THEN
        PRINT *, "b = 3 < ", 4
    end if

    g = .TRUE.

    if (g) THEN
        print *, "LOGICAL j is TRUE"
    else
        print *, "LOGICAL g is FALSE"
    end if

    g = .FALSE.
    if (g .eqv. .FALSE.) THEN
        PRINT *, "LOGICAL g is eqv to .FALSE."
    end if

    if (.not. g) THEN
        PRINT *, ".not. g is .TRUE."
    end if

    g = .TRUE.
    if ((.not. g) .and. a < b) THEN
        PRINT *, "(.not. g) .and. a < b is .TRUE."
    else
        PRINT *, "(.not. g) .and. a < b is .FALSE."
    end if

    if (((.not. g) .and. a < b) .eqv. (.not. g .and. a < b)) THEN
        PRINT *, "(.not. g) .and. a < b is eqv to .not. g .and. a < b"
    end if

    str1 = "123456789"

    if ("12349" > str1) THEN
        print *, "string 12349 great than 123456789"
    end if

    print *, "g is", g
end
