#!/usr/bin/env python

from __future__ import print_function, with_statement
import os
from os.path import isdir, join
import copy
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
parser.add_argument('-i', dest='infofile',default='Processes/AZH.json')
parser.add_argument('-p', dest='paramfile', default='PARAM/param_signal.json')
parser.add_argument('-s', dest='FLAG_SIG', action='store_true')
parser.add_argument('-sc', dest='FLAG_SIG_COMPONENTS', action='store_true') # Default calculate the total for signal, not components

args = parser.parse_args()

INFOFILE = args.infofile
FLAG_GEN = args.FLAG_GEN
FLAG_XEC = args.FLAG_XEC
FLAG_DEL = args.FLAG_DEL
NRUNS = args.NRUNS
FLAG_SIG = args.FLAG_SIG
FLAG_SIG_COMPONENTS = args.FLAG_SIG_COMPONENTS
PARAMFILE = args.paramfile

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
    res2=CALCULATE_CS(BKG_PROCS['TOTAL'],WORKDIR, DATADIR, {'ID': 'bkg','PARAM':{}})
    print(res2)


if FLAG_DEL:
    with open(PARAMFILE,'r') as f:
        PARAMS = json.load(f)
    TMPNAME = os.path.basename(PARAMFILE)
    RESULTFILE = join(DATADIR,TMPNAME)
    if os.path.isfile(RESULTFILE):
        with open(RESULTFILE,'r') as f:
            PARAMSRES = json.load(f)
    else:
        PARAMSRES = {}
    CARDS={}
    CARDS['DELPHES']=join(CURDIR,'tmp_cards/delphes_card.dat')
    CARDS['PYTHIA8']=join(CURDIR,'tmp_cards/pythia8_card.dat')
    for ntimes in range(NRUNS):
        if FLAG_SIG:
            if FLAG_SIG_COMPONENTS:
                SIG_TO_BE_CALCULATED = SIG_COMPONENTS
            else:
                SIG_TO_BE_CALCULATED = SIG_TOTAL
            for PARAM_KEY in PARAMS.keys():
                PARAM=PARAMS[PARAM_KEY]
                if 'CS' not in PARAM.keys():
                    PARAM['CS'] = {}
                CS = PARAM['CS']
                for procid in SIG_TO_BE_CALCULATED.keys():
                    if procid not in CS.keys():
                        CS[procid] = CALCULATE_CS(SIG_TO_BE_CALCULATED[procid],WORKDIR,DATADIR,PARAM,YUKTYPE)
                if 'ROOT' not in PARAM.keys():
                    PARAM['ROOT'] = {'3l':{}, '4l': {}}
                CHANLIST={'3l': 'tmp_cards/madspin_card_semilep.dat', '4l': 'tmp_cards/madspin_card_dilepton.dat'}
                for chan in CHANLIST.keys():
                    CARDS['MADSPIN']=join(CURDIR,CHANLIST[chan])
                    USEDPARAM=copy.copy(PARAM)
                    USEDPARAM['CHAN'] = chan
                    if chan not in PARAM['ROOT'].keys():
                        PARAM['ROOT'][chan] = {}
                    ROOTLISTCUR = PARAM['ROOT'][chan]
                    for procid in SIG_TO_BE_CALCULATED.keys():
                        tmp = GENERATE_EVENTS(SIG_TO_BE_CALCULATED[procid],WORKDIR,DATADIR,USEDPARAM,CARDS,YUKTYPE)
                        if procid not in ROOTLISTCUR.keys():
                            ROOTLISTCUR[procid]=[tmp]
                        else:
                            ROOTLISTCUR[procid].append(tmp)
                PARAMSRES[PARAM_KEY] = PARAM
                with open(RESULTFILE,'w') as f:
                    json.dump(PARAMSRES,f,sort_keys=True,indent=4)
        else:
            for pid in BKG_PROCS.keys():
                CARDS['MADSPIN']=join(CURDIR,'tmp_cards/madspin_card_semilep.dat')
                GENERATE_EVENTS(BKG_PROCS[pid],WORKDIR,DATADIR,{'ID': 'bkg', 'CHAN': '3l','PARAM':{}},CARDS)
                CARDS['MADSPIN']=join(CURDIR,'tmp_cards/madspin_card_dilepton.dat')
                GENERATE_EVENTS(BKG_PROCS[pid],WORKDIR,DATADIR,{'ID': 'bkg', 'CHAN':'4l','PARAM':{}},CARDS)
