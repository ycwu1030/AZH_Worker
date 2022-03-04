#!/bin/bash
#SBATCH -p batch
#SBATCH -t 24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

source /opt/cern-root/6.22.06/prebuilt/bin/thisroot.sh
module load anaconda3/2020.07
python -c "from tools import TrimRootFile as TRF; TRF.TrimRootFileAll(\"DATADIR_TMP\")"
