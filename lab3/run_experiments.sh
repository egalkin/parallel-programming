#!/bin/bash

N1=1000
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))
START_CORES=1
END_CORES=4

make clean
make all

for ((CORE = START_CORES; CORE <= END_CORES; CORE++))
  do
    rm -rf "ompM${CORE}"
    mkdir "ompM${CORE}"
    for ((N = N1; N <= N2; N+=DELTA))
      do
        ./lab3-omp $N $CORE "ompM${CORE}/ompM${CORE}.out" "ompM${CORE}/X.out"
      done
  done

python3 build_plot.py



