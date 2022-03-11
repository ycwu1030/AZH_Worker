from MGWorker import Process_Runner as PR
import os
import argparse

CURDIR = os.getcwd()
parser = argparse.ArgumentParser(prog='Generate_Events')
parser.add_argument('-i', dest='infofile', default='Processes/AZH.json')
parser.add_argument('-p', dest='paramfile',
                    default='DATADIR/PARAMS/param_signal.json')
parser.add_argument('-g', dest='group', default='LOOP')
parser.add_argument('-b', dest='FLAG_BKG', action='store_false')
args = parser.parse_args()
INFOFILE = args.infofile
PARAMFILE = args.paramfile
GROUP = args.group
BKG = args.FLAG_BKG
CATE = not BKG

pr = PR.Process_Runner(INFOFILE, os.path.join(
    CURDIR, 'MG5DIR/MG5_aMC_v3_1_1'), False)
pr.READ_PARAM_FROM_JSON(PARAMFILE)
pr.Generate_Event(GROUP, CATE, ROOT_NEEDED=15)
