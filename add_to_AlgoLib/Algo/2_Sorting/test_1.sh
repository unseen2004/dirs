#!/bin/bash

set -e

# Compile all programs
g++ -O2 -std=c++11 -o insertion_sort insertion_sort.cpp
g++ -O2 -std=c++11 -o quicksort quicksort.cpp
g++ -O2 -std=c++11 -o hybrid_quicksort hybrid_quicksort.cpp
g++ -O2 -std=c++11 -o gen_random gen_random.cpp
g++ -O2 -std=c++11 -o gen_sorted gen_sorted.cpp
g++ -O2 -std=c++11 -o gen_reverse gen_reverse.cpp

# Array of test sizes
for n in {8..32}
do
    echo "====== n = $n ======"
    for gen in gen_random gen_sorted gen_reverse
    do
        for sort in insertion_sort quicksort hybrid_quicksort
        do
            echo "--- $gen | $sort ---"
            ./$gen $n | ./$sort
            echo
        done
    done
done