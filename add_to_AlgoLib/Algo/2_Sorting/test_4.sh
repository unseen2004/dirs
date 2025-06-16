#!/bin/bash

set -e

K=$1  # repetitions (default: 10)
if [[ -z "$K" ]]; then K=10; fi

mkdir -p results_dual_vs_quick

for n in $(seq 8 1 32); do
    for gen in gen_random gen_sorted gen_reverse; do
        for sort in quicksort dual_pivot_quicksort; do
            out="results_dual_vs_quick/${sort}_${gen}_n${n}_k${K}.csv"
            echo "run,comparisons,swaps" > "$out"
            for k in $(seq 1 $K); do
                vals=$(./$gen $n | ./$sort | awk '/Comparisons:/ {c=$2} /Swaps:/ {s=$2} END{print c","s}')
                echo "$k,$vals" >> "$out"
            done
        done
    done
done