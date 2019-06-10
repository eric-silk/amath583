#!/bin/bash

make clean

make pingpong.exe
sbatch -N 2 pingpong.bash 2
sbatch -N 4 pingpong.bash 2
sbatch -N 2 pingpong.bash 8
sbatch -N 2 pingpong.bash 64

make ring.exe
sbatch -N 2 ring.bash 2
sbatch -N 4 ring.bash 2
sbatch -N 4 ring.bash 8
sbatch -N 4 ring.bash 64
sbatch -N 16 ring.bash 64

