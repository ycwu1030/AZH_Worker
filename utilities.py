import os
from os import listdir, getcwd, remove
from os.path import isdir, join
import subprocess
import time
from shutil import copyfile

CURDIR = getcwd()
MG5DIR = join(CURDIR,'MG5DIR/MG5_aMC_v3_1_1')

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
        subprocess.call('python %s/bin/mg5_aMC %s'%(MG5DIR,tmpfile),shell=True)
        remove(tmpfile)
        remove(join(CURDIR,'py.py'))

def CALCULATE_CS(process, WORKDIR, DATADIR, PARAMS, YUKTYPE = None, SQRTS = 14):
    name=process['NAME_Z']
    if YUKTYPE:
        name=process['NAME_Z']%(YUKTYPE)
    PROCDIR_ORI=join(WORKDIR,name)
    paramid=PARAMS['ID']
    PROCDIR=join(WORKDIR,"%s_%s"%(name,paramid))
    subprocess.call('cp -r %s %s'%(PROCDIR_ORI,PROCDIR),shell=True)
    DATAPROCDIR=join(DATADIR,paramid)
    LOGDIR=join(DATAPROCDIR,'logs')
    if not os.path.exists(DATAPROCDIR):
        os.makedirs(DATAPROCDIR)
    if not os.path.exists(LOGDIR):
        os.makedirs(LOGDIR)
    timetag=time.strftime("%Y%m%d_%H%M%S",time.localtime(time.time()))
    runname='run_eve_%s_%s'%(paramid,timetag)
    tmpfile=join(PROCDIR,'xec_command_%s_%s.in'%(name,paramid))
    csfile=join(PROCDIR,'xec_output_%s_%s.txt'%(name,paramid))
    tmpfile2=join(LOGDIR,'xec_command_%s_%s.in'%(name,paramid))
    csfile2=join(LOGDIR,'xec_output_%s_%s.txt'%(name,paramid))
    EBEAM=SQRTS/2*1000
    MODELPARAMS=PARAMS['PARAM']
    # remove madspin to avoid decay tops
    try:
        remove('%s/Cards/madspin_card.dat'%PROCDIR)
    except OSError:
        pass
    try:
        remove('%s/Cards/delphes_card.dat'%PROCDIR)
    except OSError:
        pass
    try:
        remove('%s/Cards/pythia8_card.dat'%PROCDIR)
    except OSError:
        pass
    with open(tmpfile, 'w') as OUTXEC:
        OUTXEC.write('generate_events %s\n'%runname)
        OUTXEC.write('0\n')
        OUTXEC.write('set ebeam1 %f\n'%(EBEAM))
        OUTXEC.write('set ebeam2 %f\n'%(EBEAM))
        OUTXEC.write('set nevents 50000\n')
        OUTXEC.write('set no_parton_cut\n')
        for param in MODELPARAMS.keys():
            OUTXEC.write('set %s %f\n'%(param,MODELPARAMS[param]))
        OUTXEC.write('0\n')
    subprocess.call('python %s/bin/madevent %s > %s'%(PROCDIR,tmpfile,csfile),shell=True)
    res=subprocess.check_output('awk \'$1=="Cross-section" {print $3}\' %s'%(csfile),shell=True)
    copyfile(tmpfile,tmpfile2)
    copyfile(csfile,csfile2)
    subprocess.call('rm -rf %s'%(PROCDIR),shell=True)
    return float(res)

def GENERATE_EVENTS(process, WORKDIR, DATADIR, PARAMS, CARDS, YUKTYPE = None, SQRTS = 14):
    name=process['NAME']
    if YUKTYPE:
        name=process['NAME']%(YUKTYPE)
    PROCDIR_ORI=join(WORKDIR,name)
    paramid=PARAMS['ID']
    chanid=PARAMS['CHAN']
    PROCDIR=join(WORKDIR,"%s_%s_%s"%(name,paramid,chanid))
    subprocess.call('cp -r %s %s'%(PROCDIR_ORI,PROCDIR),shell=True)
    DATAPROCDIR=join(DATADIR,paramid)
    LOGDIR=join(DATAPROCDIR,'logs')
    if not os.path.exists(DATAPROCDIR):
        os.makedirs(DATAPROCDIR)
    if not os.path.exists(LOGDIR):
        os.makedirs(LOGDIR)
    timetag=time.strftime("%Y%m%d_%H%M%S",time.localtime(time.time()))
    runname='run_eve_%s_%s_%s'%(paramid,chanid,timetag)
    CARDDIR=join(PROCDIR,'Cards')
    tmpfile=join(PROCDIR,'events_command_%s_%s_%s_%s.in'%(name,paramid,chanid,timetag))
    evefile=join(PROCDIR,'events_output_%s_%s_%s_%s.txt'%(name,paramid,chanid,timetag))
    EVEDIR=join(PROCDIR,'Events/%s_decayed_1'%(runname))
    tmpfile2=join(LOGDIR,'events_command_%s_%s_%s_%s.in'%(name,paramid,chanid,timetag))
    evefile2=join(LOGDIR,'events_output_%s_%s_%s_%s.txt'%(name,paramid,chanid,timetag))
    copyfile(CARDS['MADSPIN'],join(CARDDIR,'madspin_card.dat'))
    copyfile(CARDS['DELPHES'],join(CARDDIR,'delphes_card.dat'))
    copyfile(CARDS['PYTHIA8'],join(CARDDIR,'pythia8_card.dat'))
    EBEAM=SQRTS/2*1000
    MODELPARAMS=PARAMS['PARAM']
    with open(tmpfile,'w') as OUTEVE:
        OUTEVE.write('generate_events %s\n'%runname)
        OUTEVE.write('0\n')
        OUTEVE.write('set ebeam1 %f\n'%(EBEAM))
        OUTEVE.write('set ebeam2 %f\n'%(EBEAM))
        OUTEVE.write('set nevents 50000\n')
        OUTEVE.write('set no_parton_cut\n')
        for param in MODELPARAMS.keys():
            OUTEVE.write('set %s %f\n'%(param,MODELPARAMS[param]))
        OUTEVE.write('0\n')
    subprocess.call('python %s/bin/madevent %s > %s'%(PROCDIR,tmpfile,evefile),shell=True)
    copyfile(tmpfile,tmpfile2)
    copyfile(evefile,evefile2)
    rootorig=join(EVEDIR,'tag_1_delphes_events.root')
    rootdest=join(DATAPROCDIR,'delphes_%s_%s_%s_%s.root'%(name,paramid,chanid,timetag))
    copyfile(rootorig,rootdest)
    subprocess.call('rm -rf %s'%(PROCDIR),shell=True)
    return 'delphes_%s_%s_%s_%s.root'%(name,paramid,chanid,timetag)

def AZH_Pre_Analysis(process, DATADIR, PARAMS, YUKTYPE = None, CHAN='3l'):
    name=process['NAME']
    cate=0
    if YUKTYPE:
        name=process['NAME']%(YUKTYPE)
        cate=1
    if CHAN == '3l':
        decay_id = 0
    else:
        decay_id = 1
    paramid=PARAMS['ID']
    DATAPROCDIR=join(DATADIR,paramid)
    CS=PARAMS['CS']['TOTAL']
    OUTPUTDIR=join(DATADIR,'PreAna')
    if not os.path.exists(OUTPUTDIR):
        os.makedirs(OUTPUTDIR)
    OUTPUTNAME=join(OUTPUTDIR,'AZH_PreAna_%s_total_%s_%s.root'%(name,paramid,CHAN))
    ROOT_FILE_PREFIX='delphes_%s_total_%s_%s'%(name,paramid,CHAN)
    COMMAND='./PreAnalysis/AZHPreAnalysis.x %s %s %d %d %s %s %f'%(name,name,cate,decay_id,paramid,OUTPUTNAME,CS)
    INPUT_FILES = [ f for f in listdir(DATAPROCDIR) if ROOT_FILE_PREFIX in f ]
    for f in INPUT_FILES:
        COMMAND += ' %s'%(f)
    subprocess.call(COMMAND,shell=True)
