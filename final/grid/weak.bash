#!/bin/bash

echo "Weak scaling tests"
make clean
make grid.exe

for size in 128 256 512
do

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
    let "xsize = ${size} * ${tasks}"

  job=`sbatch -N ${nodes} --ntasks ${tasks} grid.bash -m 128 -j -x $xsize -y $size | awk '{ print $4 }'`
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

python3 plot.py weak128.out.txt weak256.out.txt weak512.out.txt
mv time.pdf weak.pdf
