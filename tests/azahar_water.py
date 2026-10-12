"""Verify water recovery or zero-HP checkpoint reload in isolated Azahar."""
from pathlib import Path
import struct
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def hold(keys,seconds):
        # Preserve the established route distances after replacing prototype .065 speed.
        travel=seconds*(.065/.0225) if any(key in ('Up','Down','Left','Right') for key in keys) else seconds
        x('keydown',*keys);time.sleep(travel);x('keyup',*keys);time.sleep(.3)
    save=ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    checkpoint=save.read_bytes();hp=struct.unpack_from('<I',checkpoint,16)[0]
    assert checkpoint[26]==1 and hp in (1,7)
    hold(['Right'],.5);hold(['Right','Down'],80/60);hold(['Down'],.3)
    hold(['a'],.08);hold(['Down'],.6*(.0225/.065)) # Jump across the low dock edge.
    capture('water-enter');hold(['x'],.08)
    assert save.read_bytes()==checkpoint,'Recovery must not save an intermediate position'
    time.sleep(1);capture('water-motion');time.sleep(6);capture('water-result')
    if hp==1:
        assert save.read_bytes()==checkpoint,'Game over must preserve the checkpoint'
        hold(['a'],.08);time.sleep(2);capture('water-checkpoint-reloaded')
        hold(['x'],.08);data=save.read_bytes()
        assert struct.unpack_from('<I',data,16)[0]==1,'Loading checkpoint must not heal Mario'
        px,pz=struct.unpack_from('<ff',data,8);assert abs(px)<.2 and abs(pz+5.1)<.2
    else:
        hold(['x'],.08);data=save.read_bytes()
        assert struct.unpack_from('<I',data,16)[0]==6
        px,pz=struct.unpack_from('<ff',data,8)
        assert data[26]==1 and ((px-5.5)**2+(pz+1.9)**2)**.5<.12,(px,pz)
    hold(['m'],.08)
