#! /bin/bash

for input in *.txt ; do
    ./sort_slist < $input > $input.out1
    ./Sort_slist < $input > $input.out2
    if diff $input.out1 $input.out2 > /dev/null ; then
        echo Programs did not produce same output for $input
    else
        echo Programs produced the same output for all inputs.
    fi
done
