"""Isolated original ground-contact pipe admission and underground save capture."""
from pathlib import Path
import subprocess,time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('east-pipe-contact')
    x('keydown','Up');time.sleep(.35);x('keyup','Up')
    time.sleep(6);capture('underground-arrival')
    x('keydown','x');time.sleep(.12);x('keyup','x');time.sleep(.5)
    capture('underground-saved')
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    raw=save.read_bytes()
    assert raw[26]==5 and raw[34]==1 and int.from_bytes(raw[28:32],'little')==11
    x('keydown','Left');time.sleep(.6);x('keyup','Left')
    time.sleep(6);capture('east-pipe-return')
    x('keydown','x');time.sleep(.12);x('keyup','x');time.sleep(.5)
    raw=save.read_bytes()
    assert raw[26]==2 and raw[34]==1 and int.from_bytes(raw[28:32],'little')==11
