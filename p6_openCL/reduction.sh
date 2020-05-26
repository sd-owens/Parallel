#!/bin/bash

# select device and print gpu information to console
g++ -std=c++11 -o printinfo printinfo.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
./printinfo

# values for NUM_ELEMENTS
for b in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608
do
	#echo NUM_ELEMENTS = $b
	# values for LOCAL_SIZE
	for s in 32 64 128 256
	do
		# echo LOCAL_SIZE = $s
		g++ -std=c++11 -DNUM_ELEMENTS=$b -DLOCAL_SIZE=$s -o reduction reduction.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
		./reduction
	done
done