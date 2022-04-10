#!/bin/bash

dir=$1
jsonfiles=$(ls $dir/*.json)
for file in $jsonfiles
  do
    RES=$(sbatch --parsable ./scripts/slurm_job/preanalysis_individual_bkg.sh ${file})
    echo "${file} $RES" >> JOBID_PREANALYSIS_TMP_BKG.log
done
