"""Manually review east material animation at a stationary road arrival."""
from pathlib import Path
import subprocess,time,struct
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    x('keydown','Right');time.sleep(.4);x('keyup','Right');time.sleep(2)
    save=ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    assert save.read_bytes()[26]==2
    x('keydown','Right');time.sleep(.4);x('keyup','Right');time.sleep(1)
    capture('east-ambient-0');time.sleep(1.5);capture('east-ambient-90');time.sleep(1.5);capture('east-ambient-180')
    x('keydown','Left');time.sleep(.85);x('keyup','Left');time.sleep(2)
    assert save.read_bytes()[26]==0
    x('keydown','m');time.sleep(.08);x('keyup','m')
