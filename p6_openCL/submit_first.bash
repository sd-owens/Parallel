#!/bin/bash
#SBATCH -J openCL_MatrixMult
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o first.out
#SBATCH -e first.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=owenss@oregonstate.edu
./openCL.sh
