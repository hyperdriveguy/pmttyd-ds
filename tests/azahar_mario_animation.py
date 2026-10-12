"""Capture ordinary poses using the isolated west-road fixture, away from Crump."""
from pathlib import Path
import struct
import subprocess
import time


def run(env,capture):
    output=Path(env['PM_PORT_SMOKE_OUTPUT'])
    save=output/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    initial=save.read_bytes();initial_x=struct.unpack_from('<f',initial,8)[0]
    assert initial[26]==0 and initial_x<-3,'Use the west-road fixture to avoid the battle prompt'
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
    x('keydown','x');time.sleep(.08);x('keyup','x');time.sleep(.5)
    assert save.is_file() and save.stat().st_size>=32
    saved=save.read_bytes()
    assert saved[26]==0 and struct.unpack_from('<f',saved,8)[0]>initial_x+.1
