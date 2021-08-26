#!/bin/bash
#SBATCH -p batch
#SBATCH -t 4:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

python Process_Worker.py -i Processes/AZH.json -g
