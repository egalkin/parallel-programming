#!/bin/bash

N1=1000
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))

make clean
rm -rd fwM1 fwM2 fwM5

make all
mkdir fwM1 fwM2 fwM5
export LD_LIBRARY_PATH=FW_1.3.1_Lin64/lib:$LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH

for ((N = N1; N <= N2; N+=DELTA))
  do
    ./lab2-fw $N 1 fwM1/fwM1.out fwM1/X.out
  done

for ((N = N1; N <= N2; N+=DELTA))
  do
    ./lab2-fw $N 2 fwM2/fwM2.out fwM2/X.out
  done

for ((N = N1; N <= N2; N+=DELTA))
  do
    ./lab2-fw $N 5 fwM5/fwM5.out fwM5/X.out
  done

python3 build_plot.py



