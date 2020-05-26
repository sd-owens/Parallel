#!/bin/bash
#SBATCH -J monteCarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o monteCarlo.out
#SBATCH -e monteCarlo.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=owenss@oregonstate.edu
./CUDA.sh
