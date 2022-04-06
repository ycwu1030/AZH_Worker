from MGWorker import Process_Distribution as PD
import os
import argparse

CURDIR = os.getcwd()
parser = argparse.ArgumentParser(prog='Generate_Distribution')
parser.add_argument('-i', dest='infofile', default='Processes/AZH.json')
parser.add_argument('-p', dest='paramfile',
                    default='DATADIR/PARAMS/param_signal.json')
parser.add_argument('-g', dest='group', default='LOOP')
parser.add_argument('-b', dest='FLAG_BKG', action='store_true')
args = parser.parse_args()
INFOFILE = args.infofile
PARAMFILE = args.paramfile
GROUP = args.group
BKG = args.FLAG_BKG
CATE = not BKG

pr = PD.Process_Distribution(INFOFILE, False)
pr.READ_PARAM_FROM_JSON(PARAMFILE)
pr.Perform(GROUP, CATE)
