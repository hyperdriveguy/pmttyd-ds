"""Descend original harbor stairs, save on the dock, and return to the plaza."""
from pathlib import Path
import struct
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def hold(keys,seconds):
        x('keydown',*keys);time.sleep(seconds);x('keyup',*keys);time.sleep(1)
    save=ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    assert save.read_bytes()[26]==1
    hold(['Right'],.5);hold(['Right','Down'],50/60);hold(['Left'],1);hold(['Down'],10/60)
    hold(['x'],.08) # Save current dock position through the physical menu control.
    capture('harbor-dock-0');data=save.read_bytes()
    px,pz=struct.unpack_from('<ff',data,8)
    assert data[26]==1 and abs(px)<1 and -3<pz<-1.5,(data[26],px,pz)
    # The y=0.15 floor lies on a 0.1 save-quantization midpoint; interpolation
    # can place it just above/below that midpoint. Both decoded values fit it.
    assert struct.unpack_from('<b',data,27)[0] in (1,2)
    assert struct.unpack_from('<I',data,16)[0]==7
    time.sleep(2);capture('harbor-dock-120');time.sleep(2);capture('harbor-dock-240')
    hold(['Up'],10/60);hold(['Right'],1);hold(['Left','Up'],50/60);hold(['Left'],.5);hold(['Up'],1)
    # Source scripted departure/fade/arrival now precedes the checkpoint write.
    deadline=time.monotonic()+6
    while time.monotonic()<deadline:
        try:data=save.read_bytes()
        except FileNotFoundError:time.sleep(.1);continue # Atomic replacement window.
        if data[26]==0:break
        time.sleep(.1)
    capture('harbor-stairs-return');assert data[26]==0
    assert struct.unpack_from('<I',data,16)[0]==7
    hold(['m'],.08)
