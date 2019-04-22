#!/usr/bin/env bash
make clean
make timing0.exe timing3.exe efficiency0.exe efficiency3.exe
echo "Timing 0"
./timing0.exe 1000000000
echo "Timing 3"
./timing3.exe 1000000000
echo "Efficiency 0"
./efficiency0.exe 1000000000
echo "Efficiency 3"
./efficiency3.exe 1000000000
