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
rwstag=['0x005','0x010','0x025','0x050','0x100','0x250']

for i in range(len(rws)):
    for j in range(len(rws)):
        pid=0
        params={}
        rwA=rws[i]
        rwAtag=rwstag[i]
        rwH=rws[j]
        rwHtag=rwstag[j]
        for MHA in range(500,850,50): #range(500,1050,50):
            for MHH in range(400,MHA-100+50,50):
                WHA=MHA*rwA
                WHH=MHH*rwH
                tag="%s_HA%s_HH%s_%d"%(timetag,rwAtag,rwHtag,pid)
                params[tag]={
                    'ID': tag,
                    'PARAM':{
                        'MHA':MHA,
                        'MHH':MHH,
                        'MHp':MHA,
                        'WHA':WHA,
                        'WHH':WHH,
                        'WHp':WHA
                    },
                    'AUX':{
                        'HA_WidthRatio':rwA,
                        'HH_WidthRatio':rwH
                    }
                }
                pid+=1
        with open("param_signal_%s_HA%s_HH%s.json"%(timetag,rwAtag,rwHtag),'w') as f:
            json.dump(params,f,sort_keys=True,indent=4)
