"""Capture original ordinary idle/run/jump presentation in an isolated profile."""
from pathlib import Path
import subprocess
import time


def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    x('keydown','Right')
    try:
        time.sleep(.18);capture('run-early')
        time.sleep(.17);capture('run-later')
    finally:x('keyup','Right')
    time.sleep(.5);capture('idle-front')
    x('keydown','a')
    try:
        time.sleep(.12);capture('jump-rise')
        time.sleep(.30);capture('jump-later')
    finally:x('keyup','a')
    time.sleep(1.2);capture('landed')
    # Native persistence provides an additional lifecycle check after motion.
    output=Path(env['PM_PORT_SMOKE_OUTPUT'])
    x('keydown','x');time.sleep(.08);x('keyup','x');time.sleep(.5)
    save=output/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    assert save.is_file() and save.stat().st_size>=32
