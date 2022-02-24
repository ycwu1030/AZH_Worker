from os import getcwd, remove, listdir, makedirs
from os.path import join, isdir, exists
import subprocess
import multiprocessing as mp

NCORES = 32


def TrimRootFile(param, dest_dir):
    curdir = getcwd()
    datadir = join(curdir, 'DATADIR')
    # paramdirs = [d for d in listdir(datadir) if isdir(
    #     join(datadir, d)) and d.startswith('2022')]
    # for param in paramdirs:
    paramdir = join(datadir, param)
    paramids = [d for d in listdir(paramdir) if isdir(join(paramdir, d))]
    for pid in paramids:
        piddir = join(paramdir, pid)
        procs = [d for d in listdir(piddir) if isdir(join(piddir, d))]
        for proc in procs:
            procdir = join(piddir, proc)
            destdir = join(curdir, dest_dir, param,
                           pid, proc)
            if not exists(destdir):
                makedirs(destdir)
            rootfiles = [d for d in listdir(
                procdir) if d.endswith(".root")]
            for rootfile in rootfiles:
                rootfilepath = join(procdir, rootfile)
                tmpfilepath = join(destdir, "delphes_tmp.root")
                destfilepath = join(destdir, rootfile)
                print(rootfilepath + ' -> ' +
                      tmpfilepath + ' -> ' + destfilepath)
                exitcode = subprocess.call(
                    'root -l -q tools/remove_unused_branches.cpp\\(\\"%s\\",\\"%s\\"\\)' % (rootfilepath, tmpfilepath), shell=True)
                if exitcode == 0:
                    subprocess.call("cp %s %s; rm %s" %
                                    (tmpfilepath, destfilepath, tmpfilepath), shell=True)


def TrimRootFileAll(dest_dir):
    curdir = getcwd()
    datadir = join(curdir, 'DATADIR')
    paramdirs = [d for d in listdir(datadir) if isdir(
        join(datadir, d)) and d.startswith('2022')]
    pool = mp.Pool(NCORES)
    NLIST = 0
    ARGUMENT_LIST = []
    for param in paramdirs:
        ARGUMENT_LIST.append([param, dest_dir])
        NLIST += 1

        if NLIST == NCORES:
            RES_LIST_ASYNC = pool.starmap_async(TrimRootFile, ARGUMENT_LIST)
            RES_LIST = RES_LIST_ASYNC.get()
            ARGUMENT_LIST = []
            NLIST = 0

    if NLIST != 0:
        RES_LIST_ASYNC = pool.starmap_async(TrimRootFile, ARGUMENT_LIST)
        RES_LIST = RES_LIST_ASYNC.get()
        ARGUMENT_LIST = []
        NLIST = 0
