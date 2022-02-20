#!/bin/bash
#SBATCH -p batch
#SBATCH -t 24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

./PostAnalysis/AZH_Distribution_Grid.x DATADIR/PreAna 202109090949 DATADIR/mtt_mztt_202109090949
