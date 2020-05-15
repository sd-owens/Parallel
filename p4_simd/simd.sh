#!/bin/bash

# values for NUMT
for t in 8
do
	echo NUMT = $t
	# number of elements
	for n in 1024 2048 4096 8192 16384 #32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608
	do
		# echo NUMTRIALS = $n
		g++ -DNUMT=$t -DWIDTH=$n -std=c++11 p4_simd.cpp -o simd -lm -fopenmp
		./simd
	done
done
