#!/bin/bash

N1=10
N2=1000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))

make clean
rm -rd seqN par*

make all
mkdir seqN par1N par2N par4N par8N

for ((N = N1; N <=N2; N+=DELTA))
  do
    ./lab1-seq $N seqN/seqN.out seqN/X.out
  done

for ((N = N1; N <=N2; N+=DELTA))
  do
    ./lab1-par-1 $N par1N/par1N.out par1N/X.out
  done

for ((N = N1; N <=N2; N+=DELTA))
  do
    ./lab1-par-2 $N par2N/par2N.out par2N/X.out
  done

for ((N = N1; N <=N2; N+=DELTA))
  do
    ./lab1-par-4 $N par4N/par4N.out par4N/X.out
  done

for ((N = N1; N <=N2; N+=DELTA))
  do
    ./lab1-par-8 $N par8N/par8N.out par8N/X.out
  done

python3 build_plot.py
