#!/usr/bin/env bash
make clean
make timing0.exe timing3.exe efficiency0.exe efficiency3.exe
./timing0.exe 1000000000
./timing3.exe 1000000000
./efficiency0.exe 1000000000
./efficiency3.exe 1000000000
1 000 000 000
