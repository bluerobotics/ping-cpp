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
templateFile = "%s/ping-message-.h.in" % templatePath

g = Generator()

definitions = [ "bluebps",
                "common",
                "ping1d",
                "ping360"]

# Create global json
mergedJson = {"global": {}}
for definition in definitions:
    definitionFile = "%s/%s.json" % (definitionPath, definition)
    mergedJson["global"][definition] = json.loads(open(definitionFile, 'r').read())

# Create message definitions for each sensor
for definition in definitions:
    f = open("%s/ping-message-%s.h" % (args.output_directory, definition), "w")
    f.write(g.generate(mergedJson["global"][definition], templateFile, {"definition": definition}))
    f.close()

# Create header with global information
templateFileAll = "%s/ping-message-all.h.in" % templatePath
f = open("%s/ping-message-all.h" % (args.output_directory), "w")
f.write(g.generate(mergedJson, templateFileAll))
f.close()
