import os
from os import listdir, getcwd, remove
from os.path import isdir, join
import subprocess

CURDIR = getcwd()
MG5DIR = '/Users/ycwu/Workingspace/MC-Generator/MG5_aMC_v3_1_1'

def GENERATE_PROC(process, WORKDIR, UFO, YUKTYPE = None):
    proc=process['PROC']
    nproc=len(proc)
    name=process['NAME']
    if YUKTYPE:
        name=process['NAME']%(YUKTYPE)
    tmpfile=join(CURDIR,'tmp_mg5.dat')
    PROCDIR=join(WORKDIR,name)
    with open(tmpfile,'w') as OUTGEN:
        OUTGEN.write('import model %s\n'%(UFO))
        OUTGEN.write('generate %s\n'%(proc[0]))
        if nproc > 1:
            for i in range(1,nproc):
                OUTGEN.write('add process %s\n'%(proc[i]))
        OUTGEN.write('output %s -f\n'%(PROCDIR))
    subprocess.call('rm -rf %s'%PROCDIR,shell=True)
    subprocess.call('%s/bin/mg5_aMC %s'%(MG5DIR,tmpfile),shell=True)
    remove(tmpfile)
    remove(join(CURDIR,'py.py'))
