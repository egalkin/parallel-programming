#!/bin/bash

N1=1000
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))
CORES_NUMBER=8

make clean
rm -rd fwM1 fwM2 fwM5

make all
rm -rf fwM*
export LD_LIBRARY_PATH=FW_1.3.1_Lin64/lib:$LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH

for ((CORE = 1; CORE <= CORES_NUMBER; CORE++))
  do
    mkdir "fwM${CORE}"
    for ((N = N1; N <= N2; N+=DELTA))
      do
        ./lab2-fw $N $CORE "fwM${CORE}/fwM${CORE}.out" "fwM${CORE}/X.out"
      done
  done

python3 build_plot.py



