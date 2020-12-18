program priority_check
    INTEGER :: a, b, c
    CHARACTER(10) :: s
    LOGICAL :: l

    if (10 + 1 < 0 * 0 + 0 + 20) THEN
        print *, "CASE 1 OK"
    end if

    if (10 < 10 .EQV. 0 > 0) THEN
        print *, "CASE 2 OK"
    end if

    if (.FALSE. .EQV. .FALSE. .EQV. .TRUE.) THEN
        print *, "CASE 3 OK"
    end if
end program priority_check