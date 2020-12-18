program DO_PARSE
    INTEGER i, to, step, fac

    to = 5
    step = 1

    print *, "FROM 1 to 5, step 1"
    do i = 1, to, step
        step = step + 3
        to = to + 8
        PRINT *, i
    end do

    print *, "FROM -1 to -5, step -2"
    do i = -1, -5, 0 - 2
        print *, i
    end do


    print *, "FACTORIAL OF 5"
    fac = 1
    do i = 1, 5, 1
        fac = fac * i
    end do
    print *, fac
end program DO_PARSE