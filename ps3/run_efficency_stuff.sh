#!/usr/bin/env bash
make clean
make efficiency0.exe efficiency3.exe

echo "Efficiency 0"
./efficiency0.exe 1000000000
echo "Efficiency 3"
./efficiency3.exe 1000000000


