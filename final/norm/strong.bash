#!/bin/bash

echo "Strong scaling tests"
make clean
make mpi_norm.exe

for size in 24 26 28
do

/bin/rm -f strong${size}.out.txt
touch strong${size}.out.txt

printf "${size}\n" | tee -a strong${size}.out.txt
printf "size\tprocs\ttime\n" | tee -a strong${size}.out.txt
for tasks in 1 2 4 8 16 32
do
    let "nodes = ${tasks}/2"
    if [ $nodes == 0 ]
    then
	nodes=1
    fi
    echo nodes $nodes
  job=`sbatch -N ${nodes} --ntasks ${tasks} mpi_norm.bash ${size} | awk '{ print $4 }'`
  echo job ${job}

  while [ `squeue -h -j${job} -r | wc -l` == "1" ]
  do
      printf "."
      sleep 2
  done
  printf "\n"
  time=`fgrep elapsed slurm-${job}.out | awk '{print $5}'`
  printf "${size}\t${tasks}\t${time}\n" | tee -a strong${size}.out.txt

done

done

python3 plot.py strong24.out.txt strong26.out.txt strong28.out.txt
mv time.pdf strong.pdf
