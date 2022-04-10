#!/bin/bash
#SBATCH -p batch
#SBATCH -t 120:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

curdir=$(pwd)
param=$1
export PYTHONPATH=${curdir}:$PYTHONPATH
echo ${param}
python ./scripts/python/PreAnalysis.py -i './Processes/AZH.json' -p ${param} -g "BKG" -b
