#!/bin/python3
import sys
import os
import platform
import subprocess

sources = [
    "main.c",
]
name = "prog"
objs: list[str] = []
firstArg = 1

normalAnsi = "\x1B[0m"
greenAnsi = "\x1B[32m"
redAnsi= "\x1B[31m"

if (platform.system() == "Linux"):
    cc = "cc"
    cflags = "-Wall -Wextra -O3 -march=native"
    ldflags = "-lSDL2 -lSDL2_ttf"
    outTac = "-o"
    objectExt = ".o"
    compileTac = "-c"
    platformTac = ""
    rmCmd = "rm -f"
elif (platform.system() == "Windows"):
    name += ".exe"
    cc = "cl"
    cflags = "/W3 /IC:\\libs\\include"
    # SDL2main.lib needs to be before SDL2.lib
    # Shell32.lib is needed for SDL2main.lib
    ldflags = "/link Shell32.lib SDL2main.lib SDL2.lib SDL2_ttf.lib /SUBSYSTEM:CONSOLE"
    outTac = "/Fe:"
    objectExt = ".obj"
    compileTac = "/c"
    platformTac = "/DPLATFORM_WINDOWS"
    rmCmd = "del"
else:
    print("system type not detected")
    quit()

objRecompiled: bool = False

def runCommand(command: str):
    print(f"{greenAnsi}{command}{normalAnsi}")
    if (subprocess.run(command, shell=True).returncode != 0):
        print(f"{redAnsi}failed command:\n{command}{normalAnsi}")
        sys.exit(1)

def compileSources():
    global objRecompiled
    for i, item in enumerate(sources):
        # cfileTime = os.path.getmtime(item)
        # try:
        #     ofileTime = os.path.getmtime(objs[i])
        # except FileNotFoundError:
        #     ofileTime = 0
        # if (cfileTime < ofileTime):
        #     continue
        objRecompiled = True
        command = " ".join([cc, compileTac, item, cflags, platformTac])
        runCommand(command);

def link():
    global name, outTac
    compileSources()
    if (objRecompiled):
        command = " ".join([cc, " ".join(objs), outTac, name, ldflags])
        runCommand(command);
    else:
        print("no linking necesary")

def clean():
    command = " ".join([rmCmd, " ".join(objs), name])
    runCommand(command);

def craftObjs():
    for i in sources:
        objs.append(i.replace(".c", objectExt))

def default():
    link()

def argumentCheck():
    if (len(sys.argv) > 1):
        for i in sys.argv[1:]:
            if (i == "clean"):
                clean()
            elif (i in ["default", "def"]):
                default()
            else:
                print(f"unkown target: {redAnsi}{i}{normalAnsi}")
                sys.exit(1)
    else:
        default()

if __name__ == "__main__":
    craftObjs()
    argumentCheck()
