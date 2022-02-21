#!/bin/bash
#SBATCH -p batch
#SBATCH -t 120:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

curdir=$(pwd)
group=$1
param=$2
export PYTHONPATH=${curdir}:$PYTHONPATH
python ./scripts/python/Generate_Events.py -i './Processes/AZH.json' -p ${param} -g ${group}
