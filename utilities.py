import os
from os import listdir, getcwd, remove
from os.path import isdir, join
import subprocess
import time
from shutil import copyfile

CURDIR = getcwd()
MG5DIR = '/Users/ycwu/Workingspace/MC-Generator/MG5_aMC_v3_1_1'

def GENERATE_PROC(process, WORKDIR, UFO, YUKTYPE = None):
    for tag in ['', '_Z']:
        proc=process['PROC%s'%(tag)]
        nproc=len(proc)
        name=process['NAME%s'%(tag)]
        if YUKTYPE:
            name=process['NAME%s'%(tag)]%(YUKTYPE)
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

def CALCULATE_CS(process, WORKDIR, PARAMS, YUKTYPE = None, SQRTS = 14):
    name=process['NAME_Z']
    if YUKTYPE:
        name=process['NAME_Z']%(YUKTYPE)
    PROCDIR=join(WORKDIR,name)
    tmpfile=join(PROCDIR,'xec_command.in')
    csfile=join(PROCDIR,'xec_output.txt')
    timetag=time.strftime("%Y%m%d_%H%M%S",time.localtime(time.time()))
    runname='run_xec_%s'%(timetag)
    EVEDIR=join(PROCDIR,'Events/%s'%(runname))
    tmpfile2=join(EVEDIR,'xec_command.in')
    csfile2=join(EVEDIR,'xec_output.txt')
    EBEAM=SQRTS/2*1000
    # remove madspin to avoid decay tops
    try:
        remove('%s/Cards/madspin_card.dat'%PROCDIR)
    except OSError:
        pass
    with open(tmpfile, 'w') as OUTXEC:
        OUTXEC.write('generate_events %s\n'%runname)
        OUTXEC.write('0\n')
        OUTXEC.write('set ebeam1 %f\n'%(EBEAM))
        OUTXEC.write('set ebeam2 %f\n'%(EBEAM))
        OUTXEC.write('set nevents 50000\n')
        OUTXEC.write('set no_parton_cut\n')
        for param in PARAMS.keys():
            OUTXEC.write('set %s %f\n'%(param,PARAMS[param]))
        OUTXEC.write('0\n')
    subprocess.call('%s/bin/madevent %s > %s'%(PROCDIR,tmpfile,csfile),shell=True)
    res=subprocess.check_output('awk \'$1=="Cross-section" {print $3}\' %s'%(csfile),shell=True)
    copyfile(tmpfile,tmpfile2)
    copyfile(csfile,csfile2)
    return float(res)

def GENERATE_EVENTS(process, WORKDIR, DATADIR, PARAMS, MADSPINCARD, YUKTYPE = None, SQRTS = 14):
    name=process['NAME']
    if YUKTYPE:
        name=process['NAME']%(YUKTYPE)
    PROCDIR=join(WORKDIR,name)
    CARDDIR=join(PROCDIR,'Cards')
    tmpfile=join(PROCDIR,'events_command.in')
    timetag=time.strftime("%Y%m%d_%H%M%S",time.localtime(time.time()))
    runname='run_xec_%s'%(timetag)
    EVEDIR=join(PROCDIR,'Events/%s'%(runname))
    tmpfile2=join(EVEDIR,'events_command.in')
    copyfile(MADSPINCARD,join(CARDDIR,'madspin_card.dat'))
    EBEAM=SQRTS/2*1000
    with open(tmpfile,'w') as OUTEVE:
        OUTEVE.write('generate_events %s\n'%runname)
        OUTEVE.write('0\n')
        OUTEVE.write('set ebeam1 %f\n'%(EBEAM))
        OUTEVE.write('set ebeam2 %f\n'%(EBEAM))
        OUTEVE.write('set nevents 50000\n')
        OUTEVE.write('set no_parton_cut\n')
        for param in PARAMS.keys():
            OUTEVE.write('set %s %f\n'%(param,PARAMS[param]))
        OUTEVE.write('0\n')
