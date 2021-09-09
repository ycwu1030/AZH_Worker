#!/bin/bash
#SBATCH -p batch
#SBATCH -t 96:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

dir=$1
jsonfiles=$(ls $dir/*.json)
for file in $jsonfiles
  do
    sbatch Generate_Events.sh $file
done
