#!/bin/bash

dir=$1
jsonfiles=$(ls $dir/*.json)
for file in $jsonfiles
  do
    RES=$(sbatch --parsable ./scripts/slurm_job/generate_events_bkg_individual.sh ${file})
    echo "${file} $RES" >> JOBID_TMP_${group}.log
done
