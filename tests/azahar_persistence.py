"""Run twice: warp from a road fixture and exit; reboot and return to plaza.
The second run omits --fresh-save and --save-fixture. Uses only test profile.
"""
from pathlib import Path
import struct
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    save=ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    original=save.read_bytes();startRoom=original[26];hp=struct.unpack_from('<I',original,16)[0]
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    if startRoom==0:
        startX=struct.unpack_from('<f',original,8)[0]
        key,target=('Left',3) if startX<0 else ('Down',1) if startX<1 else ('Right',2)
    else:key,target={1:'Up',2:'Left',3:'Right'}[startRoom],0
    x('keydown',key);time.sleep(.4);x('keyup',key);time.sleep(3)
    data=save.read_bytes();assert data[26]==target,(startRoom,target,data[26])
    assert struct.unpack_from('<I',data,16)[0]==hp,'HP was not restored/preserved'
    capture(f'persistence-{startRoom}-to-{target}')
    x('mousemove','443','514');x('mousedown','1');time.sleep(.08);x('mouseup','1');time.sleep(.5)
    capture(f'persistence-map-{target}')
    x('keydown','m');time.sleep(.08);x('keyup','m');time.sleep(.3)
    print(f'Confirmed room {startRoom} -> {target}, HP {hp}, checkpoint persisted')
