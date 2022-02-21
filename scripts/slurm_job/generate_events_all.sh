#!/bin/bash

group=$1
dir=$2
jsonfiles=$(ls $dir/*.json)
for file in $jsonfiles
  do
    RES=$(sbatch --parsable ./scripts/slurm_job/generate_events_individual.sh ${group} ${file})
    echo "${file} $RES" >> JOBID_TMP_${group}.log
done
