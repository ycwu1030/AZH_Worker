#!/usr/bin/env python

from __future__ import print_function, with_statement
import os
import stat
import sys
import shutil
import subprocess
import numpy as np
import time
import math
import pickle
import argparse
import simplejson
import multiprocessing
from utilities import GENERATE_PROC

CURDIR = os.getcwd()

parser = argparse.ArgumentParser(prog='THDM_Worker')
parser.add_argument('-g', dest='FLAG_GEN', action='store_true')
parser.add_argument('-x', dest='FLAG_XEC', action='store_true')
parser.add_argument('-d', dest='FLAG_DEL', action='store_true')
parser.add_argument('-i', dest='infofile',default='AZH.json')

args = parser.parse_args()

INFOFILE = args.infofile
FLAG_GEN = args.FLAG_GEN
FLAG_XEC = args.FLAG_XEC
FLAG_DEL = args.FLAG_DEL

with open(INFOFILE,'r') as f:
    INFO = simplejson.load(f)

YUKTYPE = INFO['TYPE']
UFO = INFO['UFO']%(YUKTYPE)
SQRTS = INFO['SQRTS']
DECAYS = INFO['DECAY']

SIG_COMPONENTS = INFO['SIGNAL_COMPONENT']
SIG_TOTAL = INFO['SIGNAL_TOTAL']
BKG_PROCS = INFO['BACKGROUND']

WORKDIR = '%s/%s'%(CURDIR,'WORKDIR')
if not os.path.exists(WORKDIR):
    os.makedirs(WORKDIR)

if FLAG_GEN:
    for pid in SIG_COMPONENTS.keys():
        GENERATE_PROC(SIG_COMPONENTS[pid],WORKDIR,UFO,YUKTYPE)
    for pid in SIG_TOTAL.keys():
        GENERATE_PROC(SIG_TOTAL[pid],WORKDIR,UFO,YUKTYPE)
    for pid in BKG_PROCS.keys():
        GENERATE_PROC(BKG_PROCS[pid],WORKDIR,UFO)
