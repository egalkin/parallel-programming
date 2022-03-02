#!/bin/bash

N1=1000
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))
CORE=4
CHUNK_SIZES=(1 2 4 8)

make clean
make all

for CS in "${CHUNK_SIZES[@]}"
  do
    rm -rf "ompGuided${CS}"
    mkdir "ompGuided${CS}"
    for ((N = N1; N <= N2; N+=DELTA))
      do
        ./lab3-omp-guided-${CS} $N $CORE "ompGuided${CS}/ompGuided${CS}.out" "ompGuided${CS}/X.out"
      done
  done




#python3 build_plot.py



