#!/bin/bash
#SBATCH -p batch
#SBATCH -t 24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

module load anaconda3/2020.07
python -c "from tools import Combine_BKG_Root as CBR; CBR.Combine_Background(\"Processes/AZH.json\",\"DATADIR/BKGPARAM\")"
