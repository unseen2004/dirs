#!/bin/bash

set -e

K=$1  # number of repetitions, e.g. 10
if [[ -z "$K" ]]; then K=10; fi

mkdir -p results

# First, for small n and all algorithms
for n in $(seq 10 10 50); do
    for gen in gen_random; do
        for sort in insertion_sort quicksort hybrid_quicksort; do
            out="results/${sort}_n${n}_k${K}.csv"
            echo "run,comparisons,swaps" > "$out"
            for k in $(seq 1 $K); do
                vals=$("./$gen" $n | ./$sort | awk '/Comparisons:/ {c=$2} /Swaps:/ {s=$2} END{print c","s}')
                echo "$k,$vals" >> "$out"
            done
        done
    done
done

# For large n, only quicksort and hybrid
for n in $(seq 1000 1000 50000); do
    for gen in gen_random; do
        for sort in quicksort hybrid_quicksort; do
            out="results/${sort}_n${n}_k${K}.csv"
            echo "run,comparisons,swaps" > "$out"
            for k in $(seq 1 $K); do
                vals=$("./$gen" $n | ./$sort | awk '/Comparisons:/ {c=$2} /Swaps:/ {s=$2} END{print c","s}')
                echo "$k,$vals" >> "$out"
            done
        done
    done
done