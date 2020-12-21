program factorization
    integer :: i, num

    read *, num
    do while (num > 0)
        print *, "factors of ", num, "is "

        i = 2
        do while (i * i <= num)
            do while ((num / i) * i == num)
                print *, i
                num = num / i
            end do
            i = i + 1
        end do

        if (num > 1) then
            print *, num
        end if
        read *, num
    end do

end program factorization