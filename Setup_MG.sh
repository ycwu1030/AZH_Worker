#!/bin/bash
#SBATCH -p batch
#SBATCH -t 4:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --mail-user=ywu@okstate.edu
#SBATCH --mail-type=end

MG5URL="https://launchpad.net/mg5amcnlo/3.0/3.1.x/+download/MG5_aMC_v3.1.1.tar.gz"

curdir=$(pwd)
MG5DIR=$curdir/MG5DIR
mkdir -p $MG5DIR
cd $MG5DIR
wget $MG5URL
tar -xzvf MG5_aMC_v3.1.1.tar.gz
cd MG5_aMC_v3_1_1

cat << EOF > SETUP_PACKAGE.in
install Delphes
install Pythia8
install ExRootAnalysis
generate g g > t t~ [QCD]
output TEDWORK/gg_tt_NLO_test
launch -f
exit
EOF

python2 ./bin/mg5_aMC SETUP_PACKAGE.in
