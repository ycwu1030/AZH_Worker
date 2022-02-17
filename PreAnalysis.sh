#!/bin/bash
#SBATCH -p batch
#SBATCH -t 24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

procfile=$1
files=$(ls DATADIR/param_signal_202109*.json)
cd PreAnalysis; make clean; make; cd -
nid=0
for file in ${files}
  do
    python Process_Worker.py -i $procfile -a -s -sc -p $file&
    nid=$[$nid+1]
    if [ $nid -eq 32 ]
      then
        sleep 10m
    fi
done

# python Process_Worker.py -i $procfile -a -p DATADIR/param_bkg.json&
wait
