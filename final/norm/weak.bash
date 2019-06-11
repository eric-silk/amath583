#!/bin/bash

echo "Weak scaling tests"
make clean
make mpi_norm.exe

for size in 20 23 26
do

  xsize=${size}

  /bin/rm -f weak${size}.out.txt
  touch weak${size}.out.txt

  printf "${size}\n" | tee -a weak${size}.out.txt
  printf "size\tprocs\ttime\n" | tee -a weak${size}.out.txt
  for tasks in 1 2 4 8 16 32
  do
      let "nodes = ${tasks}/2"
      if [ $nodes == 0 ]
      then
          nodes=1
      fi
      echo nodes $nodes
      let "xsize = xsize + 1"

    job=`sbatch -N ${nodes} --ntasks ${tasks} mpi_norm.bash $xsize | awk '{ print $4 }'`
    echo job ${job}

    while [ `squeue -h -j${job} -r | wc -l` == "1" ]
    do
        printf "."
        sleep 2
    done
    printf "\n"
    time=`fgrep elapsed slurm-${job}.out | awk '{print $5}'`
    printf "${size}\t${tasks}\t${time}\n" | tee -a weak${size}.out.txt

  done

done

python3 plot.py weak20.out.txt weak23.out.txt weak26.out.txt
mv time.pdf weak.pdf
