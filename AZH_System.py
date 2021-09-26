from Process_Worker import BKG_PROCS
import os
import json
from copy import copy, deepcopy
from os.path import dirname, abspath, join
from MG_Runner import MG_RUNNER


class AZH_System(object):
    __BASE_DIR__ = dirname(abspath(__file__))
    __MG5_DIR__ = join(__BASE_DIR__, 'MG5DIR/MG5_aMC_v3_1_1')
    # __MG5_DIR__ = '/Users/ycwu/Workingspace/MC-Generator/MG5_aMC_v3_1_1'

    def __init__(self, INFOFILE, DEBUG=False):
        with open(INFOFILE, 'r') as f:
            self.INFO = json.loads(f.read())

        self.YUKTYPE = self.INFO['TYPE']
        self.UFO = self.INFO['UFO'] % (self.YUKTYPE)
        self.SQRTS = self.INFO['SQRTS']
        self.DECAYS = self.INFO['DECAY']
        self.SIG_COMPONENTS = self.INFO['SIGNAL_COMPONENT']
        self.SIG_TOTAL = self.INFO['SIGNAL_TOTAL']
        self.BKG_PROCS = self.INFO['BACKGROUND']

        self.WORK_DIR = join(self.__BASE_DIR__, 'WORKDIR')
        if not os.path.exists(self.WORK_DIR):
            os.makedirs(self.WORK_DIR)
        self.DATA_DIR = join(self.__BASE_DIR__, 'DATADIR')
        if not os.path.exists(self.DATA_DIR):
            os.makedirs(self.DATA_DIR)
        self.MG_HANDLER = MG_RUNNER(self.__MG5_DIR__, self.WORK_DIR, DEBUG)
        self.CARDS = {
            'DELPHES': join(self.__BASE_DIR__, 'tmp_cards/delphes_card.dat'),
            'PYTHIA8': join(self.__BASE_DIR__, 'tmp_cards/pythia8_card.dat')
        }
        self.CARDS_DECAY = {
            '3l': join(self.__BASE_DIR__, 'tmp_cards/madspin_card_semilep.dat'),
            '4l': join(self.__BASE_DIR__, 'tmp_cards/madspin_card_dilepton.dat')}
        self.INPUT_PARAMETERS = {}
        self.RESULT_PARAMETERS = {}

    def Generate_Process_Folders(self):
        ALL_PROC_LIST = [self.SIG_COMPONENTS, self.SIG_TOTAL, self.BKG_PROCS]
        for PROC_CAT_ID in range(len(ALL_PROC_LIST)):
            PROC_CAT = ALL_PROC_LIST[PROC_CAT_ID]
            for pid in PROC_CAT.keys():
                process = PROC_CAT[pid]
                for tag in ['', '_Z']:
                    NAME = process['NAME%s' % (
                        tag)] if PROC_CAT_ID is 2 else process['NAME%s' % (tag)] % (self.YUKTYPE)
                    UFO = self.UFO
                    PROCS = process['PROC%s' % (tag)]
                    self.MG_HANDLER.Generate_Process(
                        {'NAME': NAME, 'UFO': UFO, 'PROCS': PROCS})

    def Generate_Events(self, PROCNAME, PARAMS, CHAN):

        # * Prepare the Cards for Event Generation
        CARDS = copy(self.CARDS)
        CARDS['MADSPIN'] = self.CARDS_DECAY[CHAN]

        # * Prepare the PARAMETERS
        PARAMS['CHAN'] = CHAN

        # * Run the MadEvent
        cs, filename = self.MG_HANDLER.Run_MadEvent(
            PROCNAME, self.DATA_DIR, PARAMS, CARDS)
        return filename

    def Calculate_CS(self, PROCNAME, PARAMS):

        PARAMS['CHAN'] = 'cs'

        cs, filename = self.MG_HANDLER.Run_MadEvent(
            PROCNAME, self.DATA_DIR, PARAMS)
        return cs

    def Read_Parameters(self, PARAMFILE):
        with open(PARAMFILE, 'r') as f:
            self.INPUT_PARAMETERS = json.load(f)
        TMPNAME = os.path.basename(PARAMFILE)
        RESULTFILE = join(self.DATA_DIR, TMPNAME)
        if os.path.isfile(RESULTFILE):
            with open(RESULTFILE, 'r') as f:
                self.RESULT_PARAMETERS = json.load(f)
        else:
            self.RESULT_PARAMETERS = {}

    def Dump_Parameters(self, PARAMFILE):
        TMPNAME = os.path.basename(PARAMFILE)
        RESULTFILE = join(self.DATA_DIR, TMPNAME)
        with open(RESULTFILE, 'w') as f:
            json.dump(self.RESULT_PARAMETERS, f, sort_keys=True, indent=4)

    def Prepare_Parameters(self, param_key, proc_key, chan_id):

        PARAM = copy(self.INPUT_PARAMETERS[param_key])
        if param_key in self.RESULT_PARAMETERS.keys():
            PARAM = copy(self.RESULT_PARAMETERS[param_key])
        RUN_CS = False
        if 'CS' not in PARAM.keys():
            PARAM['CS'] = {}
            RUN_CS = True
        elif proc_key not in PARAM['CS'].keys():
            PARAM['CS'][proc_key] = 0
            RUN_CS = True
        N_ROOT_FILES = 0
        if 'ROOT' not in PARAM.keys():
            PARAM['ROOT'] = {}

        if chan_id not in PARAM['ROOT'].keys():
            PARAM['ROOT'][chan_id] = {}

        if proc_key not in PARAM['ROOT'][chan_id].keys():
            PARAM['ROOT'][chan_id][proc_key] = []

        N_ROOT_FILES = len(PARAM['ROOT'][chan_id][proc_key])

        return RUN_CS, N_ROOT_FILES, PARAM

    def Scan_Parameters(self, PARAMFILE, NRUNS, FLAG_SIG=True, FLAG_SIG_COMPONENT=True, CHAN='3l'):

        PROCS_TO_BE_RUN = {}
        if FLAG_SIG:
            if FLAG_SIG_COMPONENT:
                PROCS_TO_BE_RUN = self.SIG_COMPONENTS
            else:
                PROCS_TO_BE_RUN = self.SIG_TOTAL
        else:
            PROCS_TO_BE_RUN = BKG_PROCS

        self.Read_Parameters(PARAMFILE)

        for param_key in self.INPUT_PARAMETERS.keys():
            for pid in PROCS_TO_BE_RUN.keys():
                # * Checking for the specific parameter and the process and the channel, whether we need to calculate the CS, and how many root files we already have and prepare the parameters
                RUN_CS, N_ROOT_FILES, PARAM = self.Prepare_Parameters(
                    param_key, pid, CHAN)

                USED_PARAM = copy(PARAM)
                if RUN_CS:
                    PROCNAME = PROCS_TO_BE_RUN[pid]['NAME_Z']
                    if FLAG_SIG:
                        PROCNAME = PROCNAME % (self.YUKTYPE)
                    PARAM['CS'][pid] = self.Calculate_CS(
                        PROCNAME, USED_PARAM)
                    self.RESULT_PARAMETERS[param_key] = PARAM
                    self.Dump_Parameters(PARAMFILE)

                for ntimes in range(N_ROOT_FILES, NRUNS):
                    PROCNAME = PROCS_TO_BE_RUN[pid]['NAME']
                    if FLAG_SIG:
                        PROCNAME = PROCNAME % (self.YUKTYPE)
                    root_file_name = self.Generate_Events(
                        PROCNAME, USED_PARAM, CHAN)
                    PARAM['ROOT'][CHAN][pid].append(root_file_name)
                    self.RESULT_PARAMETERS[param_key] = PARAM
                    self.Dump_Parameters(PARAMFILE)

    def Pre_Analysis(self, PARAMFILE, FLAG_SIG=True, FLAG_SIG_COMPONENT=True, CHAN='3l'):

        self.Read_Parameters(PARAMFILE)
