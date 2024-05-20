# -*- coding: utf-8 -*-
"""
@original author: weismam

lib3mf_integration:
    tests whether a large number of 3MF files is parsed correctly by lib3MF /
    the Example_ExtractInfo from the SDK

"""

import datetime, time
import subprocess
import os


def listFiles(root, extension):
    lFiles = []
    for path, _, files in os.walk(root):
        for name in files:
            [_, fileextension] = os.path.splitext(name)
            if extension == fileextension:
                lFiles.append(os.path.join(path, name))
    return lFiles


# returns a list of files of the following pattern: $root/{*}/$inbetweenFolder/{*}/*.$extension
def listFilesIn(root, inbetweenFolder, extension):
    lFiles = []
    for path, subdirs, _ in os.walk(root):
        for subdir in subdirs:
            if subdir == inbetweenFolder:
                lFiles += listFiles(os.path.join(path, subdir), extension)
    lFiles = list(set(lFiles))
    lFiles.sort()
    return lFiles


def ExtractInfo(execCommand, fileName):
    tStart = time.time()
    proc = subprocess.Popen([execCommand, fileName], stdout=subprocess.PIPE)
    pOut = ""
    errLines = []
    for line in proc.stdout:
        utf8line = line.decode("utf-8")
        if "error" in utf8line:
            errLines.append(utf8line)
        if "warning" in utf8line:
            errLines.append(utf8line)
        pOut += utf8line

    proc.wait()
    info = dict.fromkeys({'success', 'time', 'stdout'})
    info['time'] = time.time() - tStart
    info['returncode'] = proc.returncode
    info['success'] = len(errLines) == 0
    info['errLines'] = errLines
    info['stdout'] = str(pOut)
    return info


if __name__ == "__main__":
    tStart = datetime.datetime.now()

    root = os.path.dirname(os.path.realpath(__file__))

    execCommand = os.path.join(root, "../lib3mf_sdk", "Examples", "CppDynamic", "build", "Example_ExtractInfo")

    positives = []
    negatives = []

    for suite in os.listdir(root):
        suite_path = os.path.join(root, suite)
        if os.path.isdir(suite_path) and suite.startswith("suite"):
            positives += listFilesIn(suite_path, "positive_test_cases", ".3mf")
            negatives += listFilesIn(suite_path, "negative_test_cases", ".3mf")

    nPos = len(positives)
    nNeg = len(negatives)
    nFiles = nPos + nNeg

    print(execCommand)

    brokenPositives = []
    iFile = 0
    for fileName in positives:
        iFile += 1
        print("{:3.0f}%: {:s}".format(100 * (iFile / nFiles), fileName), flush=True)
        info = ExtractInfo(execCommand, fileName)
        if not info['returncode'] == 0:
            print("Fatal Error: MUSTPASS file \"{:s}\" does not work with returncode {:d}:".format(fileName,
                                                                                                   info['returncode']))
            brokenPositives.append(info)
        if not info['success']:
            print("Error: MUSTPASS file \"{:s}\" does not work:".format(fileName))
            print('Contains {:d} problem{:s}:'.format(len(info['errLines']), ['s', ''][len(info['errLines']) == 0]))
            for errLine in info['errLines']:
                print(errLine, end='')
            print('=== Output === ')
            print(info['stdout'])
            print('=== /Output === ')
            brokenPositives.append(info)

    runningNegatives = []
    for fileName in negatives:
        iFile += 1
        print("{:3.0f}%: {:s}".format(100 * (iFile / nFiles), fileName), flush=True)
        info = ExtractInfo(execCommand, fileName)
        if not info['returncode'] >= 0:
            print("Fatal Error: MUSTFAIL file \"{:s}\" does not work with returncode {:d}:".format(fileName,
                                                                                                   info['returncode']))
        if info['success'] and info['returncode'] == 0:
            print("Error: MUSTFAIL file \"{:s}\" works".format(fileName))
            runningNegatives.append(info)

    duration = datetime.datetime.now() - tStart
    print("Tested a total of {:d} = ({:d} positive + {:d} negative) files in {:s}.".format(
        nFiles, nPos, nNeg, str(duration).split('.')[0]))

    if nPos > 0:
        print("  {:3d} / {:3d} MUSTPASS files passed ({:6.2f}%)".format(nPos - len(brokenPositives), nPos,
                                                                        100 * (1 - len(brokenPositives) / nPos)))
    if len(negatives) > 0:
        print("  {:3d} / {:3d} MUSTFAIL files failed ({:6.2f}%)".format(nNeg - len(runningNegatives), nNeg,
                                                                        100 * (1 - len(runningNegatives) / nNeg)))
