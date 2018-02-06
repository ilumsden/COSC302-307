#! /bin/bash

# No argument testing
for input in *.txt ; do
    ./quicksort < $input > $input.out.noin1
    ./Quicksort < $input > $input.out.noin2
    if diff $input.out.noin1 $input.out.noin2 > /dev/null ; then
        echo Programs did not produce the same output for $input with no arguments
    else
        echo Programs produced the same output with no arguments.
        rm $input.out.noin1
        rm $input.out.noin2
    fi
done

# Two argument testing
./quicksort 3 6 < list1.txt > list1.txt.out.in1
./Quicksort 3 6 < list1.txt > list1.txt.out.in2
if diff <(sed -n '4,7p' list1.txt.out.in1) <(sed -n '4,7p' list1.txt.out.in2) > /dev/null ; then
    echo Programs did not produce the same output for list1.txt with 2 arguments
else
    echo Programs produced the same output with 2 arguments.
    rm list1.txt.out.in1
    rm list1.txt.out.in2
fi
./quicksort 200 413 < list2.txt > list2.txt.out.in1
./Quicksort 200 413 < list2.txt > list2.txt.out.in2
if diff <(sed -n '201,414p' list2.txt.out.in1) <(sed -n '201,414p' list2.txt.out.in2) > /dev/null ; then
    echo Programs did not produce the same output for list2.txt with 2 arguments
else
    echo Programs produced the same output with 2 arguments.
    rm list2.txt.out.in1
    rm list2.txt.out.in2
fi
./quicksort 1027 1593 < list3.txt > list3.txt.out.in1
./Quicksort 1027 1593 < list3.txt > list3.txt.out.in2
if diff <(sed -n '1028,1594p' list3.txt.out.in1) <(sed -n '1028,1594p' list3.txt.out.in2) > /dev/null ; then
    echo Programs did not produce the same output for list3.txt with 2 arguments
else
    echo Programs produced the same output with 2 arguments.
    rm list3.txt.out.in1
    rm list3.txt.out.in2
fi
