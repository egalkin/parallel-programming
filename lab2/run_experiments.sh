#!/bin/bash

N1=1000
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))
CORES_NUMBER=4

make clean
rm -rd fwM*

rm -rf fwM*
export FW_HOME=FW_1.3.1_Lin64
make all
export LD_LIBRARY_PATH=$FW_HOME/lib:$LD_LIBRARY_PATH

for ((CORE = 1; CORE <= CORES_NUMBER; CORE++))
  do
    mkdir "fwM${CORE}"
    for ((N = N1; N <= N2; N+=DELTA))
      do
        ./lab2-fw $N $CORE "fwM${CORE}/fwM${CORE}.out" "fwM${CORE}/X.out"
      done
  done

python3 build_plot.py



