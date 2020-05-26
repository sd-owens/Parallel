#!/bin/bash

# values for BLOCKSIZE
for s in 16 32 64 128
do
	echo BLOCKSIZE = $s
	# number of trials
	for n in 16384 32768 65536 131072 262144 524288 1048576
	do
		# echo NUMNODES = $n
		/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DBLOCKSIZE=$s -DNUMTRIALS=$n -w -o monteCarlo monteCarloCUDA.cu -lm
		./monteCarlo
	done
done