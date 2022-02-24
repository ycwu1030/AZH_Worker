#!/bin/bash
#SBATCH -p express
#SBATCH -t 1:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
##SBATCH --mail-user=ywu@okstate.edu
##SBATCH --mail-type=end

python -c "from tools import TrimRootFile as TRF; TRF.TrimRootFileAll(\"DATADIR_TMP\")"
