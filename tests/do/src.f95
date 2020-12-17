program DO_PARSE
    INTEGER i, to, step

    to = 5
    step = 1

    do i = 1, to, step
        step = step + 1
        to = to + 1
        print *, i, to, step
    end do

    do i = -1, 5 + 2, 2 - 1 + 3
        print *, i
    end do
end program DO_PARSE