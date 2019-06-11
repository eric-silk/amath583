#!/usr/bin/env bash

echo "Hello from Slurm!"
srun cat /proc/cpuinfo | grep "model name"
echo "Goodbye."
