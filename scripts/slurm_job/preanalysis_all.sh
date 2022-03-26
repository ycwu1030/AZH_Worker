#!/bin/bash

group=$1
dir=$2
jsonfiles=$(ls $dir/*.json)
for file in $jsonfiles
  do
    RES=$(sbatch --parsable ./scripts/slurm_job/preanalysis_individual.sh ${group} ${file})
    echo "${file} $RES" >> JOBID_PREANALYSIS_TMP_${group}.log
done
