#!/bin/bash
#SBATCH -p batch
#SBATCH -t 24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

# files=("param_signal_202108290019_0x005.json" "param_signal_202108290019_0x010.json" "param_signal_202108290019_0x025.json" "param_signal_202108290019_0x050.json" "param_signal_202108290019_0x100.json" "param_signal_202108290019_0x250.json")
procfile=$1
files=$(ls DATADIR/*.json)
cd PreAnalysis; make clean; make; cd -
for file in ${files}
  do
    python Process_Worker.py -i $procfile -a -s -p $file&
done

# python Process_Worker.py -i $procfile -a -p DATADIR/param_bkg.json&
wait
