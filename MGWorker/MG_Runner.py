import os
from os import getcwd, remove, listdir
from os.path import join, isdir
import subprocess
import time
from shutil import copyfile


class MG_RUNNER(object):

    def __init__(self, MG5DIR, WORKDIR, DEBUG=False):
        self.MG5_DIR = MG5DIR
        self.WORK_DIR = WORKDIR
        self.DEBUG = DEBUG

    def Generate_Process(self, PROC):
        '''
        PROC is a map which has at least following keys:
        - NAME: Name for process
        - UFO: The model name
        - PRES: The command should be run before generating the process
        - PROCS: The processes needed
        '''
        KEYS = PROC.keys()
        if 'NAME' not in KEYS or 'UFO' not in KEYS or 'PROCS' not in KEYS or 'PRES' not in KEYS:
            print("WARNING: Input for generate process is not correct, we need at least 'NAME', 'UFO', 'PRES' and 'PROCS' keys, but we got", KEYS)
            return
        name = PROC['NAME']
        UFO = PROC['UFO']
        pre = PROC['PRES']
        proc = PROC['PROCS']
        CURDIR = getcwd()
        tmpfile = join(CURDIR, 'tmp_%s.dat' % (name))
        PROCDIR = join(self.WORK_DIR, name)
        nproc = len(proc)
        with open(tmpfile, 'w') as TMP:
            TMP.write('import model %s\n' % (UFO))
            for com in pre:
                TMP.write('%s\n' % (com))
            TMP.write('generate %s\n' % (proc[0]))
            for i in range(1, nproc):
                TMP.write('add process %s\n' % (proc[i]))
            TMP.write('output %s -f\n' % (PROCDIR))
        if not self.DEBUG:
            subprocess.call('rm -rf %s' % (PROCDIR), shell=True)
            subprocess.call('%s/bin/mg5_aMC %s' %
                            (self.MG5_DIR, tmpfile), shell=True)
            remove(tmpfile)
            remove(join(CURDIR, 'py.py'))
        else:
            remove(tmpfile)

    def Check_Diagrams(self, PROCNAME, DATADIR):
        procdir = join(self.WORK_DIR, PROCNAME)
        subprocdir = join(procdir, 'SubProcesses')
        channeldir = [d for d in listdir(
            subprocdir) if isdir(join(subprocdir, d)) and d.startswith('P')]
        diagdir = join(DATADIR, PROCNAME)
        if not os.path.exists(diagdir):
            os.makedirs(diagdir)
        chanid = 0
        for chan in channeldir:
            chandir = join(subprocdir, chan)
            diagrams = [dia for dia in listdir(chandir) if dia.startswith(
                'matrix') and dia.endswith('.ps')]
            diagid = 0
            for diag in diagrams:
                copyfile(join(chandir, diag), join(
                    diagdir, 'diagram_%d_%d.ps' % (chanid, diagid)))
                diagid += 1
            chanid += 1

    def Prepare_Cards(self, procdir, CARDS):
        CARDDIR = join(procdir, 'Cards')
        if CARDS:
            copyfile(CARDS['MADSPIN'], join(CARDDIR, 'madspin_card.dat'))
            copyfile(CARDS['DELPHES'], join(CARDDIR, 'delphes_card.dat'))
            copyfile(CARDS['PYTHIA8'], join(CARDDIR, 'pythia8_card.dat'))
        else:
            # remove madspin to avoid decay tops
            try:
                remove('%s/madspin_card.dat' % CARDDIR)
            except OSError:
                pass
            try:
                remove('%s/delphes_card.dat' % CARDDIR)
            except OSError:
                pass
            try:
                remove('%s/pythia8_card.dat' % CARDDIR)
            except OSError:
                pass

    def Run_MadEvent(self, PROCNAME, DATADIR, PARAMS, CARDS=None, SQRTS=14):
        '''
        PROCNAME: the name of the process, used to identify the folder.
        DATADIR: The place to keep the logs and the events.
        PARAMS: a map store the parameters for this run
            - TAG: parameter tag, used to uniquely identify the parameter
            - CHAN: channel id: 3l or 4l for events generation, or cs for cs calculation
            - PARAM: a map containing other parameters in the model
        CARDS: either None, or a list of the cards. When it is None, it means we only care the cross section, otherwise, we need the events (decayed)
        '''

        paramid = PARAMS['TAG']
        chanid = PARAMS['CHAN']
        # * Prepare the running folder
        PROCDIR_ORI = join(self.WORK_DIR, PROCNAME)
        PROCDIR = join(self.WORK_DIR, "%s_%s_%s" % (PROCNAME, paramid, chanid))
        subprocess.call('rm -rf %s' % (PROCDIR), shell=True)
        subprocess.call('cp -r %s %s' % (PROCDIR_ORI, PROCDIR), shell=True)

        # * Prepare the auxillary cards
        self.Prepare_Cards(PROCDIR, CARDS)

        # * Check we are generating events or calculate the cs
        if CARDS:
            log_prefix = 'events'
        else:
            log_prefix = 'xec'

        # * Prepare the Log directory
        timetag = time.strftime("%Y%m%d_%H%M%S", time.localtime(time.time()))
        PROC_DATA_DIR = join(DATADIR, paramid)
        PROC_LOG_DIR = join(PROC_DATA_DIR, 'logs')
        if not os.path.exists(PROC_DATA_DIR):
            os.makedirs(PROC_DATA_DIR)
        if not os.path.exists(PROC_LOG_DIR):
            os.makedirs(PROC_LOG_DIR)
        CMD_FILE_NAME = '%s_command_%s_%s_%s_%s.txt' % (
            log_prefix, PROCNAME, paramid, chanid, timetag)
        LOG_FILE_NAME = '%s_output_%s_%s_%s_%s.txt' % (
            log_prefix, PROCNAME, paramid, chanid, timetag)
        CMDFILE = join(PROCDIR, CMD_FILE_NAME)
        LOGFILE = join(PROCDIR, LOG_FILE_NAME)
        CMDFILE_KEEP = join(PROC_LOG_DIR, CMD_FILE_NAME)
        LOGFILE_KEEP = join(PROC_LOG_DIR, LOG_FILE_NAME)

        # * Prepare the running command
        EBEAM = SQRTS/2*1000
        run_name = 'run_%s_%s_%s' % (log_prefix, paramid, timetag)
        MODEL_PARAMS = PARAMS['PARAM']
        with open(CMDFILE, 'w') as TMP:
            TMP.write('generate_events %s\n' % (run_name))
            TMP.write('0\n')
            TMP.write('set ebeam1 %f\n' % (EBEAM))
            TMP.write('set ebeam2 %f\n' % (EBEAM))
            TMP.write('set nevents 10000\n')
            TMP.write('set iseed %d\n' % (int(time.time()) % 10000000))
            TMP.write('set no_parton_cut\n')
            for param in MODEL_PARAMS.keys():
                TMP.write('set %s %f\n' % (param, MODEL_PARAMS[param]))
            TMP.write('0\n')

        # * Run the MadEvent
        if not self.DEBUG:
            subprocess.call('python %s/bin/madevent %s > %s' %
                            (PROCDIR, CMDFILE, LOGFILE), shell=True)
            res = subprocess.check_output(
                'awk \'$1=="Cross-section" {print $3}\' %s' % (LOGFILE), shell=True)

            # * Moving the events and logs
            copyfile(CMDFILE, CMDFILE_KEEP)
            copyfile(LOGFILE, LOGFILE_KEEP)
            root_file_name = 'N.A.'
            if CARDS:
                root_file_name = 'delphes_%s_%s_%s_%s.root' % (
                    PROCNAME, paramid, chanid, timetag)
                copyfile(join(PROCDIR, 'Events/%s_decayed_1/tag_1_delphes_events.root' %
                              (PROCNAME)), join(PROC_DATA_DIR, root_file_name))
        else:
            time.sleep(1)
            res = 999
            root_file_name = 'delphes_%s_%s_%s_%s.root' % (
                PROCNAME, paramid, chanid, timetag)
        subprocess.call('rm -rf %s' % (PROCDIR), shell=True)
        return float(res), root_file_name
