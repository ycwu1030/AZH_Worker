#!/usr/bin/env python

from __future__ import print_function, with_statement
import os
from os.path import isdir, join
import stat
import sys
import shutil
import subprocess
import time
import math
import argparse
import json

tag = "BKG_PARAM"
for i in range(30):
    params = {}
    key = "%s_%d" % (tag, i)
    params[key] = {
        'TAG': tag,
        'ID': i,
        'PARAM': {
        }
    }
    with open("param_bkg_%d.json" % (i), 'w') as f:
        json.dump(params, f, sort_keys=True, indent=4)
