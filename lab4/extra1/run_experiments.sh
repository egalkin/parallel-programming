#!/bin/bash

N1=75
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))
START_CORES=1
END_CORES=4
TIMES_TO_REPEAT=10

make clean
make all

for ((CORE = START_CORES; CORE <= END_CORES; CORE++))
  do
    rm -rf "omp2M${CORE}"
    mkdir "omp2M${CORE}"
    for ((N = N1; N <= N2; N+=DELTA))
      do
        for ((I = 1; I <= TIMES_TO_REPEAT; I++))
        do
            ./lab4-omp $N $CORE "omp2M${CORE}/omp2M${CORE}.out" "omp2M${CORE}/X.out"
        done
      done
  done

python3 build_plot.py



