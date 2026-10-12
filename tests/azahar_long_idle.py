"""Capture idle effect poses after --seconds50 with the west-road fixture."""
import subprocess
import time


def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    # Startup/load/fade time precedes the source idle counter. Capture a window
    # instead of claiming one wall-clock timestamp is the simulation boundary.
    for frame in range(10):
        time.sleep(.2);capture(f'long-idle-transition-{frame}')
    time.sleep(3);capture('long-idle-sleep')
    x('keydown','Right')
    try:time.sleep(.25);capture('long-idle-resume-run')
    finally:x('keyup','Right')
    time.sleep(.5);capture('long-idle-resume-idle')
