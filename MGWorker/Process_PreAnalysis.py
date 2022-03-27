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
    'gg_BOXxZ': 6,
    'bb_Z': 7,
    'bb_S': 8,
    'bb_T': 9,
    'bb_ZxS': 10,
    'bb_ZxT': 11,
    'bb_SxT': 12,
    'qq_DY': 13
}


class Process_PreAnalysis(object):

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
        self.OUTPUT_DIR = join(self.DATA_DIR, 'PreAnalysis')

        self.PARAMFILE = None
        self.PARAMS = {}

    def READ_PARAM_FROM_JSON(self, PARAMFILE):
        self.PARAMFILE = PARAMFILE
        with open(PARAMFILE, 'r') as f:
            self.PARAMS = json.loads(f.read())

    def Get_ROOT_File_Directory(self, param_tag, param_id, process_key):
        return join(self.DATA_DIR, param_tag, str(param_id), process_key)

    def Get_ROOT_File_List(self, param_key, special_param_key, chan_id, process_key):
        tmpdata = self.PARAMS[param_key]
        num = -1
        if "ROOT" not in tmpdata.keys():
            tmpdata["ROOT"] = {}
        root_block = tmpdata["ROOT"]
        if chan_id not in root_block.keys():
            root_block[chan_id] = {}
        chan_block = root_block[chan_id]
        if process_key not in chan_block.keys():
            chan_block[process_key] = {}
        proc_block = chan_block[process_key]
        if special_param_key not in proc_block.keys():
            proc_block[special_param_key] = []
        # num = len(proc_block[special_param_key])
        return proc_block[special_param_key]

    def Get_CS(self, param_key, special_param_key, process_key):
        tmpdata = self.PARAMS[param_key]
        if "CS" not in tmpdata.keys():
            return None
        cs_block = tmpdata["CS"]
        if process_key not in cs_block.keys():
            return None
        proc_block = cs_block[process_key]
        if special_param_key not in proc_block.keys():
            return None
        cs = proc_block[special_param_key]
        return cs

    def Generate_Output_Name(self, param_key, special_param_key, chan_id, process_key):
        return "AZH_Pre_%s_%s_%s_%s" % (process_key, param_key, special_param_key, chan_id)

    def Perform_Single(self, process_key, param_key, special_param_key, chan_id):
        process_id = process_id_map[process_key]
        param = self.PARAMS[param_key]
        OUTPUT_DIR = join(self.OUTPUT_DIR, param["TAG"], str(
            param["ID"]))
        if not os.path.exists(OUTPUT_DIR):
            os.makedirs(OUTPUT_DIR)
        LOG_FILE = join(OUTPUT_DIR, 'PreAnalysis.log')
        OUTPUT_FILE = self.Generate_Output_Name(
            param_key, special_param_key, chan_id, process_key)
        cs = self.Get_CS(param_key, special_param_key, process_key)
        ROOT_DIR = self.Get_ROOT_File_Directory(
            param["TAG"], param["ID"], process_key)
        ROOT_LIST = self.Get_ROOT_File_List(
            param_key, special_param_key, chan_id, process_key)
        ROOT_LIST_WITH_PATH = [join(ROOT_DIR, d) for d in ROOT_LIST]
        COMMAND = './PreAnalysis/AZHPreAnalysis.x %d %s %s %f %s %s > %s' % (
            process_id, process_key, param_key, cs, join(OUTPUT_DIR, OUTPUT_FILE), ' '.join(ROOT_LIST_WITH_PATH), LOG_FILE)
        exitcode = 1
        if not self.DEBUG:
            exitcode = subprocess.call(COMMAND, shell=True)
        cs_input = 0
        cs_used = 0
        n_total = 0
        n_passed = 0
        if exitcode == 0:
            tmp = subprocess.check_output(
                'awk \'$1=="Input-CS" {print $2}\' %s' % (LOG_FILE), shell=True)
            try:
                cs_input = float(tmp)
            except:
                pass
            tmp = subprocess.check_output(
                'awk \'$1=="Used-CS" {print $2}\' %s' % (LOG_FILE), shell=True)
            try:
                cs_used = float(tmp)
            except:
                pass
            tmp = subprocess.check_output(
                'awk \'$1=="Input-Events" {print $2}\' %s' % (LOG_FILE), shell=True)
            try:
                n_total = int(tmp)
            except:
                pass
            tmp = subprocess.check_output(
                'awk \'$1=="Passed-Events" {print $2}\' %s' % (LOG_FILE), shell=True)
            try:
                n_passed = int(tmp)
            except:
                pass

        return cs_input, cs_used, n_total, n_passed

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
                    cs_input, cs_used, n_total, n_passed = self.Perform_Single(
                        process_key, param_key, special_param_key, '3l')
                    print(process_key, param_key, special_param_key,
                          cs_input, cs_used, n_total, n_passed)
