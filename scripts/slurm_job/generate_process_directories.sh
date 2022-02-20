#!/bin/bash
#SBATCH -p batch
#SBATCH -t 4:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

curdir=$(pwd)
export PYTHONPATH=${curdir}:$PYTHONPATH
python ./scripts/python/Generate_Process_Directories.py -i './Processes/AZH.json'
