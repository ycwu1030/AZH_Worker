from os import getcwd, remove, listdir, makedirs
from os.path import join, isdir, exists
import subprocess
import json
import multiprocessing as mp

NCORES = 32


def CheckingRootFile(paramfile):
    INFO = {}
    with open(paramfile, 'r') as f:
        INFO = json.loads(f.read())
    parameters_key = INFO.keys()
    for param in parameters_key:
        ROOTFILES = INFO[param]["ROOT"]["3l"]
        procs = ROOTFILES.keys()
        for proc in procs:
            proc_root = ROOTFILES[proc]
            tb_choices = proc_root.keys()
            for tb in tb_choices:
                root_file_list = proc_root[tb]
                bad_root_file_list = []
                for root_file in root_file_list:
                    root_file_with_path = join(
                        'DATADIR/%s/%d/%s' % (INFO[param]["TAG"], INFO[param]["ID"], proc), root_file)
                    output = subprocess.check_output(
                        'root -l -q tools/checking_root_file.cpp\\(\\"%s\\"\\) | awk \'$1=="RES" {print $2}\'' % (root_file_with_path), shell=True)
                    res = int(output)
                    if res == 0:
                        try:
                            print("REMOVING %s" % (root_file_with_path))
                            # subprocess.call('rm %s'%(root_file_with_path),shell=True)
                        except:
                            pass
                        bad_root_file_list.append(root_file)
                for file in bad_root_file_list:
                    print("REMOVING %s from JSON" % (file))
                    # root_file_list.remove(file)


def CheckingRootFileAll(param_dir):
    param_jsons = [d for d in listdir(param_dir) if d.endswith('.json')]
    pool = mp.Pool(NCORES)
    NLIST = 0
    ARGUMENT_LIST = []
    for paramfile in param_jsons:
        ARGUMENT_LIST.append([join(param_dir, paramfile)])
        NLIST += 1

        if NLIST == NCORES:
            RES_LIST_ASYNC = pool.starmap_async(
                CheckingRootFile, ARGUMENT_LIST)
            RES_LIST = RES_LIST_ASYNC.get()
            ARGUMENT_LIST = []
            NLIST = 0

    if NLIST != 0:
        RES_LIST_ASYNC = pool.starmap_async(CheckingRootFile, ARGUMENT_LIST)
        RES_LIST = RES_LIST_ASYNC.get()
        ARGUMENT_LIST = []
        NLIST = 0
