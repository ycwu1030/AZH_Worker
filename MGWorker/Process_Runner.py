from .MG_Runner import MG_RUNNER as MGR
import json
import os
from os import getcwd
from os.path import join
import copy


class Process_Runner(object):

    def __init__(self, INFOFILE, MG5DIR, DEBUG):
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
        if not os.path.exists(self.WORK_DIR):
            os.makedirs(self.WORK_DIR)
        self.DATA_DIR = join(getcwd(), 'DATADIR')
        if not os.path.exists(self.DATA_DIR):
            os.makedirs(self.DATA_DIR)

        self.MG_HANDLER = MGR(MG5DIR, self.WORK_DIR, self.DEBUG)
        self.PARAMFILE = None
        self.PARAMS = {}

    def Get_Process_Name(self, process_key, WithDecay=True):
        if process_key in self.SIG_COMPONENTS.keys():
            tmpdata = self.SIG_COMPONENTS[process_key]
            return (tmpdata["NAME"] % (self.YUKTYPE)) if WithDecay else (tmpdata["NAME_Z"] % (self.YUKTYPE))
        elif process_key in self.BKG_PROCS.keys():
            tmpdata = self.BKG_PROCS[process_key]
            return tmpdata["NAME"] if WithDecay else tmpdata["NAME_Z"]

    def READ_PARAM_FROM_JSON(self, PARAMFILE):
        self.PARAMFILE = PARAMFILE
        with open(PARAMFILE, 'r') as f:
            self.PARAMS = json.loads(f.read())

    def DUMP_PARAM_TO_JSON(self):
        with open(self.PARAMFILE, 'w') as f:
            json.dump(self.PARAMS, f, sort_keys=True, indent=4)

    def Check_ROOT_File_Number(self, param_key, special_param_key, chan_id, process_key):
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
        num = len(proc_block[special_param_key])
        return num

    def Update_ROOT_File_Name(self, root_name, param_key, special_param_key, chan_id, process_key):
        if self.DEBUG:
            print("Update to %s with root=%s for %s, %s, %s, %s" % (
                self.PARAMFILE, root_name, param_key, special_param_key, chan_id, process_key))
        tmpdata = self.PARAMS[param_key]
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
        proc_block[special_param_key].append(root_name)
        self.DUMP_PARAM_TO_JSON()
        self.READ_PARAM_FROM_JSON(self.PARAMFILE)

    def Check_CS(self, param_key, special_param_key, process_key):
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

    def Update_CS(self, cs, param_id, special_param_key, process_key):
        if self.DEBUG:
            print("Update to %s with cs=%s for %s, %s, %s" % (
                self.PARAMFILE, str(cs), param_id, special_param_key, process_key))
        tmpdata = self.PARAMS[param_id]
        if "CS" not in tmpdata.keys():
            tmpdata["CS"] = {}
        cs_block = tmpdata["CS"]
        if process_key not in cs_block.keys():
            cs_block[process_key] = {}
        proc_block = cs_block[process_key]
        proc_block[special_param_key] = cs
        self.DUMP_PARAM_TO_JSON()
        self.READ_PARAM_FROM_JSON(self.PARAMFILE)

    def Generate_Process_Folders(self):
        ALL_PROC_LIST = [self.SIG_COMPONENTS, self.BKG_PROCS]
        for PROC_CAT_ID in range(len(ALL_PROC_LIST)):
            PROC_CAT = ALL_PROC_LIST[PROC_CAT_ID]
            for pid in PROC_CAT.keys():
                process = PROC_CAT[pid]
                for tag in ['', '_Z']:
                    NAME = process['NAME%s' % tag] if PROC_CAT_ID == 1 else process['NAME%s' % tag] % (
                        self.YUKTYPE)
                    UFO = self.UFO
                    PRES = process['PRES']
                    PROCS = process['PROC%s' % tag]
                    self.MG_HANDLER.Generate_Process(
                        {'NAME': NAME, 'UFO': UFO, 'PRES': PRES, 'PROCS': PROCS})

    def Check_Diagrams(self):
        ALL_PROC_LIST = [self.SIG_COMPONENTS, self.BKG_PROCS]
        for PROC_CAT_ID in range(len(ALL_PROC_LIST)):
            PROC_CAT = ALL_PROC_LIST[PROC_CAT_ID]
            for pid in PROC_CAT.keys():
                process = PROC_CAT[pid]
                for tag in ['', '_Z']:
                    NAME = process['NAME%s' % tag] if PROC_CAT_ID == 1 else process['NAME%s' % tag] % (
                        self.YUKTYPE)
                    self.MG_HANDLER.Check_Diagrams(NAME, self.DATA_DIR)

    def Calculate_CS(self, process_key, PARAMS, DATADIR, SQRTS=14):
        process_name = self.Get_Process_Name(process_key, WithDecay=False)
        cs, _ = self.MG_HANDLER.Run_MadEvent(
            process_name, DATADIR, PARAMS, SQRTS=SQRTS)
        return cs

    def Generate_Event_Single(self, process_key, PARAMS, CARDS, DATADIR, SQRTS=14):
        process_name = self.Get_Process_Name(process_key, WithDecay=True)
        _, root_file_name = self.MG_HANDLER.Run_MadEvent(
            process_name, DATADIR, PARAMS, CARDS, SQRTS)
        return root_file_name

    def Generate_Event(self, Group_key=None, Signal=True, ROOT_NEEDED=15, SQRTS=14):

        # We are dealing with signal or background
        process_map = self.SIG_COMPONENTS if Signal else self.BKG_PROCS
        process_keys = process_map.keys()
        if Group_key in self.SIG_GROUP.keys() and Signal:
            process_keys = self.SIG_GROUP[Group_key]
        CARDS = {}
        CARDS['MADSPIN'] = join(self.CARD_DIR, 'madspin_card_semilep.dat') if Signal else join(
            self.CARD_DIR, 'madspin_card_semilep_bkg.dat')
        CARDS['DELPHES'] = join(self.CARD_DIR, 'delphes_card_ATLAS.dat')
        CARDS['PYTHIA8'] = join(self.CARD_DIR, 'pythia8_card.dat')
        for param_key in self.PARAMS.keys():
            for process_key in process_keys:
                process_data = process_map[process_key]
                # First get the special parameters
                special_params = process_data["SPECIAL_PARAM"]
                # loop over special parameters
                for special_param_key in special_params.keys():
                    # Update parameter
                    PARAM = {}
                    PARAM["PARAM"] = copy.copy(self.PARAMS[param_key]["PARAM"])
                    PARAM["TAG"] = self.PARAMS[param_key]["TAG"] + "_" + str(self.PARAMS[param_key]["ID"]) + \
                        "_" + special_param_key + "_3l"
                    PARAM["PARAM"].update(special_params[special_param_key])
                    DATADIR = join(
                        self.DATA_DIR, self.PARAMS[param_key]["TAG"], self.PARAMS[param_key]["ID"], process_key)
                    # First check, how many root we have already:
                    root_num = self.Check_ROOT_File_Number(
                        param_key, special_param_key, "3l", process_key)
                    # Second check, whether the cs is calculated
                    cs = self.Check_CS(
                        param_key, special_param_key, process_key)
                    if cs is None:
                        # We have not calculated the cross section
                        cs = self.Calculate_CS(
                            process_key, PARAM, DATADIR, SQRTS)
                        self.Update_CS(
                            cs, param_key, special_param_key, process_key)
                    for run_num in range(root_num, ROOT_NEEDED):
                        root_file_name = self.Generate_Event_Single(
                            process_key, PARAM, CARDS, DATADIR, SQRTS)
                        if self.DEBUG:
                            print('Generated root file with name: %s' %
                                  root_file_name)
                        self.Update_ROOT_File_Name(
                            root_file_name, param_key, special_param_key, "3l", process_key)
