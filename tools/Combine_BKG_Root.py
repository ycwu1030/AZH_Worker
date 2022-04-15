import json
from os import getcwd, listdir, makedirs
from os.path import join, splitext, exists
from shutil import copyfile


def Combine_Background(INFOFILE, PARAMDIR):
    INFO = {}
    with open(INFOFILE, 'r') as f:
        INFO = json.loads(f.read())
    BKG_PROCS = INFO['BACKGROUND']
    DATA_DIR = join(getcwd(), 'DATADIR')
    OUTPUT_DIR = join(DATA_DIR, 'BKG', 'COMB', 'bkg_total')
    if not exists(OUTPUT_DIR):
        makedirs(OUTPUT_DIR)
    OUTPUT_PARAM_FILE = join(PARAMDIR, 'param_bkg_combined.json')
    OUTPUT_PARAM = {
        "BKG_COMB": {
            "ID": "COMB",
            "PARAM": {},
            "TAG": "BKG",
            "CS": {"bkg_total": {"DEFAULT": 0}},
            "ROOT": {"3l": {"bkg_total": {"DEFAULT": []}}}
        }
    }

    PARAMFILES = [d for d in listdir(PARAMDIR) if d.endswith('.json')]
    cs_accumulated = 0
    cs_number = 0
    root_file_list_combined = []
    for file in PARAMFILES:
        param_infile = {}
        with open(join(PARAMDIR, file)) as f:
            param_infile = json.loads(f.read())
        for param_key in param_infile.keys():
            param_individual = param_infile[param_key]
            cs = param_individual['CS']['bkg_total']['DEFAULT']
            cs_accumulated += cs
            cs_number += 1
            root_file_list = param_individual['ROOT']['3l']['bkg_total']['DEFAULT']
            root_dir = join(DATA_DIR, 'BKG_PARAM', str(
                param_individual['ID']), 'bkg_total')
            for file in root_file_list:
                file_name, ext = splitext(file)
                file_out = file_name + '_' + str(cs_number) + ext
                copyfile(join(root_dir, file), join(OUTPUT_DIR, file_out))
                root_file_list_combined.append(file_out)
    cs_averaged = cs_accumulated/cs_number
    OUTPUT_PARAM['BKG_COMB']['CS']['bkg_total']['DEFAULT'] = cs_averaged
    OUTPUT_PARAM['BKG_COMB']['ROOT']['3l']['bkg_total']['DEFAULT'] = root_file_list_combined
    with open(OUTPUT_PARAM_FILE, 'w') as f:
        json.dump(OUTPUT_PARAM, f, sort_keys=True, indent=4)
