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
import json


timetag=time.strftime("%Y%m%d%H%M",time.localtime(time.time()))

rws=[0.005,0.01,0.025,0.05,0.1,0.25]
pid=0
params={}
for MHA in range(500,1050,50):
    for MHH in range(400,MHA-100+50,50):
        for rw in rws:
            WHA=MHA*rw
            WHH=MHH*rw
            tag="%s_%d"%(timetag,pid)
            params[tag]={
                'ID': tag,
                'PARAM':{
                    'MHA':MHA,
                    'MHH':MHH,
                    'MHp':MHA,
                    'tb':1,
                    'WHA':WHA,
                    'WHH':WHH,
                    'WHp':WHA
                },
                'AUX':{
                    'WidthRatio':rw
                }
            }
            pid+=1

with open("param_signal.json",'w') as f:
    json.dump(params,f,sort_keys=True,indent=4)
