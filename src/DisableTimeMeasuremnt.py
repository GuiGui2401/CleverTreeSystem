#/**
#*@Copyright DTTA Maker Factory Tech. All Rights reserved.
#*It is not allowed, whether to copy this document, or to pass it even in extracts to others, without the approval DTTA Maker Factory Tech.
#*All copyrights for this document are the exclusive property of DTTA Maker Factory Tech.
#*/ 
import glob
import fileinput
import sys

def replaceAll(file,searchExp,replaceExp):
    for line in fileinput.input(file, inplace=1):
        if searchExp in line:
            line = line.replace(searchExp,replaceExp)
        sys.stdout.write(line)

path = glob.glob('**/additional_types.h')
path = glob.glob('**/**/additional_types.h') + path
path = glob.glob('**/**/**/additional_types.h') + path

for i in range(len(path)):
    replaceAll(path[i], '#define TIME_MEASUREMENT', '//#define TIME_MEASUREMENT')