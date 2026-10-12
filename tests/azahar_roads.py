"""Verify plaza/east-side reciprocal road warps using isolated east-road fixture."""
from pathlib import Path
import struct
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    output=Path(env.get('PM_PORT_SMOKE_OUTPUT',ROOT/'artifacts/azahar'))
    save=output/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def hold(key,seconds):
        x('keydown',key);time.sleep(seconds);x('keyup',key);time.sleep(1.4)
    def room(expected):
        data=save.read_bytes()
        assert data[26]==expected,f'Expected saved room {expected}, got {data[26]}'
    start=save.read_bytes();startX=struct.unpack_from('<f',start,8)[0]
    origin=start[26]
    case,target,outward,inward=('station-reload',3,'Down','Up') if origin==4 else ('station',4,'Up','Down') if origin==3 else ('west',3,'Left','Right') if startX<0 else ('harbor',1,'Down','Up') if startX<1 else ('east',2,'Right','Left')
    for repeat in range(2):
        hold(outward,1.3);time.sleep(2);capture(f'road-{case}-arrival-{repeat}');room(target)
        hold(inward,1.3);time.sleep(2);capture(f'road-{case}-return-{repeat}');room(origin)
    if origin==3:
        hold('Up',1.3);time.sleep(2);room(4);capture('station-save-for-restart')
    hold('m',.08)
