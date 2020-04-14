#!/bin/bash

# values for NUMT
for t in 1 2 4 6 8
do
	echo NUMT = $t
	# number of elements
	for n in 1 10 100 1000 10000 100000 1000000
	do
		# echo NUMTRIALS = $n
		g++ -DNUMT=$t -DNUMTRIALS=$n -std=c++11 monteCarlo.cpp -o monteCarlo -O3 -lm -fopenmp
		./monteCarlo
	done
done
