#!/usr/bin/env python3

import argparse
import json
import os
from pathlib import Path

scriptPath = Path(__file__).parent.absolute()
generatorPath = "%s/../../lib/ping-protocol/src/" % scriptPath

import sys
sys.path.append(generatorPath)

from generator import Generator

parser = argparse.ArgumentParser(description="generate markdown documentation files for message definitions")
parser.add_argument('--output-directory', action="store", default="./", type=str, help="directory to save output files")
args = parser.parse_args()

if not os.path.exists(args.output_directory):
    os.makedirs(args.output_directory)

definitionPath = "%s/../../lib/ping-protocol/src/definitions" % scriptPath
templatePath = "%s/templates" % scriptPath

g = Generator()

f = open("%s/ping-device-ping1d.h" % args.output_directory, "w")
f.write(g.generate("%s/ping1d.json" % definitionPath, "%s/ping-device-ping1d.h.in" % templatePath))
f.close()

f = open("%s/ping-device-ping1d.cpp" % args.output_directory, "w")
f.write(g.generate("%s/ping1d.json" % definitionPath, "%s/ping-device-ping1d.cpp.in" % templatePath))
f.close()
