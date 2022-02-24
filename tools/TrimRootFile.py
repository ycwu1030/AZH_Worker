from os import getcwd, remove, listdir, makedirs
from os.path import join, isdir, exists
import subprocess


def TrimRootFile(dest_dir):
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
                    tmpdir = join(curdir, dest_dir, param,
                                  pid, proc)
                    if not exists(tmpdir):
                        makedirs(tmpdir)
                    tmpfilepath = join(procdir, "delphes_tmp.root")
                    destfilepath = join(tmpdir, rootfile)
                    print(rootfilepath)
                    exitcode = subprocess.call(
                        'root -l tools/remove_unused_branches\\(\\"%s\\",\\"%s\\"\\)' % (rootfilepath, tmpfilepath), shell=True)
                    if exitcode == 0:
                        subprocess.call("cp %s %s; rm %s" %
                                        (tmpfilepath, destfilepath, tmpfilepath))
