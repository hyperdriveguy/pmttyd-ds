"""Verify a structurally valid checkpoint without a floor falls back coherently."""
from pathlib import Path
import struct
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('invalid-room-fallback')
    x('keydown','Right');time.sleep(.2);x('keyup','Right');time.sleep(.2)
    x('keydown','x');time.sleep(.08);x('keyup','x');time.sleep(.5)
    data=(ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin').read_bytes()
    assert data[26]==0,'Fallback did not restore plaza collision/progress'
    px,pz=struct.unpack_from('<ff',data,8)
    assert 1.2<px<2.5 and abs(pz-3.5)<.05,(px,pz)
    assert struct.unpack_from('<I',data,16)[0]==10
    capture('invalid-room-movement-save')
    x('keydown','m');time.sleep(.08);x('keyup','m');time.sleep(.3)
    print('Rejected invalid harbor position; verified plaza movement and save')
