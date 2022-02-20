from MGWorker import Process_Runner as PR
import os
import argparse

CURDIR = os.getcwd()
parser = argparse.ArgumentParser(prog='Generate_Process_Directories')
parser.add_argument('-i', dest='infofile', default='Processes/AZH.json')
args = parser.parse_args()
INFOFILE = args.infofile

pr = PR.Process_Runner(INFOFILE, os.path.join(
    CURDIR, 'MG5DIR/MG5_aMC_v3_1_1'), False)
pr.Generate_Process_Folders()
