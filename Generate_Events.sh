#!/bin/bash
#SBATCH -p batch
#SBATCH -t 96:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

paramfile=$1
python Process_Worker.py -i Processes/AZH.json -d -s -n 2 -p $paramfile
