#!/bin/bash

N1=1000
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))
CORE=4
OPTIMIZATION_LEVELS=(0 1 2 3)

make clean
make all

for OL in "${OPTIMIZATION_LEVELS[@]}"
  do
    rm -rf "ompO${OL}"
    mkdir "ompO${OL}"
    for ((N = N1; N <= N2; N+=DELTA))
      do
        ./lab3-omp-o${OL} $N $CORE "ompO${OL}/ompO${OL}.out" "ompO${OL}/X.out"
      done
  done

python3 build_plot.py
