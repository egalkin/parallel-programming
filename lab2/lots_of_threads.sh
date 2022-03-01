#!/bin/bash

N1=1000
N2=40000
DELTA_N=$((N2 - N1))
DELTA=$((DELTA_N / 10))
START_CORES_NUMBER=401
MAX_CORES_NUMBER=404

make clean

export FW_HOME=FW_1.3.1_Lin64
make all
export LD_LIBRARY_PATH=$FW_HOME/lib:$LD_LIBRARY_PATH

for ((CORE = START_CORES_NUMBER; CORE <= MAX_CORES_NUMBER; CORE++))
  do
    rm -rf "fwM${CORE}"
    mkdir "fwM${CORE}"
    for ((N = N1; N <= N2; N+=DELTA))
      do
        ./lab2-fw $N $CORE "fwM${CORE}/fwM${CORE}.out" "fwM${CORE}/X.out"
      done
  done

python3 build_extra_plots.py
