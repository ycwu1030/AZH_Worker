#!/bin/bash
#SBATCH -p batch
#SBATCH -t 72:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

paramfile=$1
python Process_Worker.py -i Processes/AZH.json -d -s -n 4 -p $paramfile
