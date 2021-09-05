#!/bin/bash
#SBATCH -p batch
#SBATCH -t 24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

files=("param_signal_202108290019_0x005.json" "param_signal_202108290019_0x010.json" "param_signal_202    108290019_0x025.json" "param_signal_202108290019_0x050.json" "param_signal_202108290019_0x100.json" "p    aram_signal_202108290019_0x250.json")
for file in ${files[@]}
  do
    python Process_Worker.py -i Processes/AZH.json -a -s -p DATADIR/$file&
done

python Process_Worker.py -i Processes/AZH.json -a -p DATADIR/param_bkg.json&
wait
