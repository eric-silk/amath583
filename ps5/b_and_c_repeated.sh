#!/usr/bin/env bash

make clean
make -j9 bonnie_and_clyde.exe

for i in {1..5}
do
    echo "======================================="
    ./bonnie_and_clyde.exe
done
