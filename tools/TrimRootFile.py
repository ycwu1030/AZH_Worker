from os import getcwd, remove, listdir
from os.path import join, isdir
import subprocess


def TrimRootFile():
    curdir = getcwd()
    datadir = join(curdir, 'DATADIR')
    paramdirs = [d for d in listdir(datadir) if isdir(
        join(datadir, d)) and d.startswith('2022')]
    for param in paramdirs:
        paramdir = join(datadir, param)
        paramids = [d for d in listdir(paramdir) if isdir(join(paramdir, d))]
        for pid in paramids:
            piddir = join(paramdir, pid)
            procs = [d for d in listdir(piddir) if isdir(join(piddir, d))]
            for proc in procs:
                procdir = join(piddir, proc)
                rootfiles = [d for d in listdir(
                    procdir) if d.endswith(".root")]
                for rootfile in rootfiles:
                    rootfilepath = join(procdir, rootfile)
                    print(rootfilepath)
