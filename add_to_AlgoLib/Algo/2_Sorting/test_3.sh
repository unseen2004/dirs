#!/bin/bash

set -e

K=$1  # number of repetitions (default: 10)
if [[ -z "$K" ]]; then K=10; fi

mkdir -p results_natural

for n in $(seq 10 10 50); do
    for sort in mergesort natural_mergesort; do
        out="results_natural/${sort}_n${n}_k${K}.csv"
        echo "run,comparisons,swaps" > "$out"
        for k in $(seq 1 $K); do
            vals=$(./gen_random $n | ./$sort | awk '/Comparisons:/ {c=$2} /Swaps:/ {s=$2} END{print c","s}')
            echo "$k,$vals" >> "$out"
        done
    done
done

for n in $(seq 1000 1000 50000); do
    for sort in mergesort natural_mergesort; do
        out="results_natural/${sort}_n${n}_k${K}.csv"
        echo "run,comparisons,swaps" > "$out"
        for k in $(seq 1 $K); do
            vals=$(./gen_random $n | ./$sort | awk '/Comparisons:/ {c=$2} /Swaps:/ {s=$2} END{print c","s}')
            echo "$k,$vals" >> "$out"
        done
    done
done