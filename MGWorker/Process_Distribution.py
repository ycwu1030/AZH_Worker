import subprocess
import json
import os
from os import getcwd
from os.path import join

process_id_map = {
    'bkg_total': 0,
    'ggF_TRI': 1,
    'ggF_BOX': 2,
    'ggF_Z': 3,
    'ggF_TRIxBOX': 4,
    'ggF_TRIxZ': 5,
    'ggF_BOXxZ': 6,
    'bb_Z': 7,
    'bb_S': 8,
    'bb_T': 9,
    'bb_ZxS': 10,
    'bb_ZxT': 11,
    'bb_SxT': 12,
    'qq_DY': 13
}


class Process_Distribution(object):

    def __init__(self, INFOFILE, DEBUG):
        with open(INFOFILE, 'r') as f:
            self.INFO = json.loads(f.read())

        self.DEBUG = DEBUG
        self.YUKTYPE = self.INFO['TYPE']
        self.UFO = self.INFO['UFO'] % (self.YUKTYPE)
        self.SQRTS = self.INFO['SQRTS']
        self.DECAYS = self.INFO['DECAY']
        self.SIG_GROUP = self.INFO['SIGNAL_GROUPS']
        self.SIG_COMPONENTS = self.INFO['SIGNAL_COMPONENT']
        self.BKG_PROCS = self.INFO['BACKGROUND']
        self.CARD_DIR = join(getcwd(), 'tmp_cards')
        self.WORK_DIR = join(getcwd(), 'WORKDIR')
        self.DATA_DIR = join(getcwd(), 'DATADIR')
        self.INPUT_DIR = join(self.DATA_DIR, 'PreAnalysis')
        self.OUTPUT_DIR = join(self.DATA_DIR, 'Distribution')

        self.PARAMFILE = None
        self.PARAMS = {}

    def READ_PARAM_FROM_JSON(self, PARAMFILE):
        self.PARAMFILE = PARAMFILE
        with open(PARAMFILE, 'r') as f:
            self.PARAMS = json.loads(f.read())

    def Get_ROOT_File_Name(self, param_key, special_param_key, chan_id, process_key):
        return "AZH_Pre_%s_%s_%s_%s" % (process_key, param_key, special_param_key, chan_id)

    def Get_Parameters(self, param_key, special_param_key, process_key):
        paramdata = self.PARAMS[param_key]
        procdata = self.INFO["SIGNAL_COMPONENT"]
        MHA = paramdata['PARAM']['MHA']
        MHH = paramdata['PARAM']['MHH']
        WHA = paramdata['PARAM']['WHA']
        WHH = paramdata['PARAM']['WHH']
        cba = 0
        tb = 1
        if special_param_key != "DEFAULT":
            special_param = procdata[process_key]['SPECIAL_PARAM'][special_param_key]
            tb = special_param['tb']
            cba = special_param['cba']
        return MHA, MHH, WHA, WHH, tb, cba

    def Generate_Output_Name(self, param_key, special_param_key, chan_id, process_key):
        return "AZH_Distribution_%s_%s_%s_%s" % (process_key, param_key, special_param_key, chan_id)

    def Perform_Single(self, process_key, param_key, special_param_key, chan_id):
        process_id = process_id_map[process_key]
        param = self.PARAMS[param_key]
        INPUT_DIR = join(self.INPUT_DIR, param["TAG"], str(param["ID"]))
        OUTPUT_DIR = join(self.OUTPUT_DIR, param["TAG"], str(
            param["ID"]))
        if not os.path.exists(OUTPUT_DIR):
            os.makedirs(OUTPUT_DIR)
        LOG_DIR = join(OUTPUT_DIR, 'logs')
        if not os.path.exists(LOG_DIR):
            os.makedirs(LOG_DIR)
        infilename = self.Get_ROOT_File_Name(
            param_key, special_param_key, chan_id, process_key)
        outfilename = self.Generate_Output_Name(
            param_key, special_param_key, chan_id, process_key)
        ROOT_FILE = join(INPUT_DIR, '%s.root' % infilename)
        LOG_FILE = join(LOG_DIR, '%s.log' % outfilename)
        OUTPUT_FILE = join(OUTPUT_DIR, '%s.root' % outfilename)
        MHA, MHH, WHA, WHH, tb, cba = self.Get_Parameters(
            param_key, special_param_key, process_key)
        COMMAND = './PostAnalysis/AZH_Dump_Distribution.x %s %s %s %s %s %s %s %s > %s' % (str(
            MHA), str(MHH), str(WHA), str(WHH), str(tb), str(cba), ROOT_FILE, OUTPUT_FILE, LOG_FILE)
        exitcode = 1
        if not self.DEBUG:
            exitcode = subprocess.call(COMMAND, shell=True)
        return exitcode

    def Perform(self, Group_key=None, Signal=True):
        # We are dealing with signal or background
        process_map = self.SIG_COMPONENTS if Signal else self.BKG_PROCS
        process_keys = process_map.keys()
        if Group_key in self.SIG_GROUP.keys() and Signal:
            process_keys = self.SIG_GROUP[Group_key]

        PARAMS = self.PARAMS
        for param_key in PARAMS.keys():
            for process_key in process_keys:
                process_data = process_map[process_key]
                special_params = {
                    'DEFAULT': {}
                }
                if process_data['SPECIAL_PARAM']:
                    special_params = process_data["SPECIAL_PARAM"]
                for special_param_key in special_params.keys():
                    exitcode = self.Perform_Single(
                        process_key, param_key, special_param_key, '3l')
                    print(process_key, param_key, special_param_key,
                          exitcode)
