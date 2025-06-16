#!/bin/bash

set -e

K=$1  # repetitions
if [[ -z "$K" ]]; then K=10; fi

mkdir -p threshold_results

for t in 2 4 8 12 16 24 32 48 64; do
    for n in 1000 2000 5000 10000 20000 50000; do
        out="threshold_results/hybrid_th${t}_n${n}_k${K}.csv"
        echo "run,comparisons,swaps" > "$out"
        for k in $(seq 1 $K); do
            vals=$(./gen_random $n | ./hybrid_quicksort_2 $t | awk '/Comparisons:/ {c=$2} /Swaps:/ {s=$2} END{print c","s}')
            echo "$k,$vals" >> "$out"
        done
    done
done