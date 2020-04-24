#!/bin/bash

# values for NUMT
for t in 1 2 4 #8  //8 only on FLIP
do
	echo NUMT = $t
	# number of nodes
	for n in 10 100 1000 10000
	do
		# echo NUMNODES = $n
		g++ -DNUMT=$t -DNUMNODES=$n -std=c++11 superQuadratic.cpp -o superQuadratic -O3 -lm -fopenmp
		./superQuadratic
	done
done