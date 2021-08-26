#!/usr/bin/env python

from __future__ import print_function, with_statement
import os
from os.path import isdir, join
import stat
import sys
import shutil
import subprocess
import time
import math
import argparse
# import simplejson
import json
import multiprocessing
from utilities import GENERATE_EVENTS, GENERATE_PROC, CALCULATE_CS

CURDIR = os.getcwd()

parser = argparse.ArgumentParser(prog='THDM_Worker')
parser.add_argument('-g', dest='FLAG_GEN', action='store_true')
parser.add_argument('-x', dest='FLAG_XEC', action='store_true')
parser.add_argument('-d', dest='FLAG_DEL', action='store_true')
parser.add_argument('-n', dest='NRUNS', default=1, type=int)
parser.add_argument('-i', dest='infofile',default='AZH.json')

args = parser.parse_args()

INFOFILE = args.infofile
FLAG_GEN = args.FLAG_GEN
FLAG_XEC = args.FLAG_XEC
FLAG_DEL = args.FLAG_DEL
NRUNS = args.NRUNS

with open(INFOFILE,'r') as f:
    # INFO = simplejson.load(f)
    INFO = json.loads(f.read())

YUKTYPE = INFO['TYPE']
UFO = INFO['UFO']%(YUKTYPE)
SQRTS = INFO['SQRTS']
DECAYS = INFO['DECAY']

SIG_COMPONENTS = INFO['SIGNAL_COMPONENT']
SIG_TOTAL = INFO['SIGNAL_TOTAL']
BKG_PROCS = INFO['BACKGROUND']

WORKDIR = join(CURDIR,'WORKDIR')
if not os.path.exists(WORKDIR):
    os.makedirs(WORKDIR)
DATADIR = join(CURDIR,'DATADIR')
if not os.path.exists(DATADIR):
    os.makedirs(DATADIR)

if FLAG_GEN:
    for pid in SIG_COMPONENTS.keys():
        GENERATE_PROC(SIG_COMPONENTS[pid],WORKDIR,UFO,YUKTYPE)
    for pid in SIG_TOTAL.keys():
        GENERATE_PROC(SIG_TOTAL[pid],WORKDIR,UFO,YUKTYPE)
    for pid in BKG_PROCS.keys():
        GENERATE_PROC(BKG_PROCS[pid],WORKDIR,UFO)

if FLAG_XEC: # Calculate Cross Section, keep the value for future reference
    res1=CALCULATE_CS(SIG_COMPONENTS['TRI'],WORKDIR, {'MHH':550,'MHA':700,'MHp':700,'tb':1,'WHH':27.5,'WHA':35,'WHp':35},YUKTYPE)
    res2=CALCULATE_CS(BKG_PROCS['TOTAL'],WORKDIR, {'MHH':550,'MHA':700,'MHp':700,'tb':1,'WHH':27.5,'WHA':35,'WHp':35})
    print(res1)
    print(res2)

if FLAG_DEL:
    CARDS={}
    CARDS['MADSPIN']=join(CURDIR,'tmp_cards/madspin_card_semilep.dat')
    CARDS['DELPHES']=join(CURDIR,'tmp_cards/delphes_card.dat')
    CARDS['PYTHIA8']=join(CURDIR,'tmp_cards/pythia8_card.dat')
    for i in range(NRUNS):
        for pid in BKG_PROCS.keys():
            GENERATE_EVENTS(BKG_PROCS[pid],WORKDIR,DATADIR,{'TAG':'bkg','PARAM':{}},CARDS)
