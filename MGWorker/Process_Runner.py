from .MG_Runner import MG_RUNNER as MGR
import json
import os
from os import getcwd
from os.path import join


class Process_Runner(object):

    def __init__(self, INFOFILE, MG5DIR):
        with open(INFOFILE, 'r') as f:
            self.INFO = json.loads(f.read())

        self.YUKTYPE = self.INFO['TYPE']
        self.UFO = self.INFO['UFO'] % (self.YUKTYPE)
        self.SQRTS = self.INFO['SQRTS']
        self.DECAYS = self.INFO['DECAY']
        self.SIG_COMPONENTS = self.INFO['SIGNAL_COMPONENT']
        self.BKG_PROCS = self.INFO['BACKGROUND']
        self.WORK_DIR = join(getcwd(), 'WORKDIR')
        if not os.path.exists(self.WORK_DIR):
            os.makedirs(self.WORK_DIR)
        self.DATA_DIR = join(getcwd(), 'DATADIR')
        if not os.path.exists(self.DATA_DIR):
            os.makedirs(self.DATA_DIR)

        self.MG_HANDLER = MGR(MG5DIR, self.WORK_DIR)

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
