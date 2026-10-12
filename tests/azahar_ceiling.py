"""Repeat native jumps beneath original plaza geometry; verify stable landed save."""
from pathlib import Path
import struct
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('ceiling-start')
    for n in range(5):
        x('keydown','a');time.sleep(.14)
        if n==0:capture('ceiling-jump')
        time.sleep(.5);x('keyup','a');time.sleep(.5)
    capture('ceiling-landed')
    x('keydown','x');time.sleep(.1);x('keyup','x');time.sleep(.5)
    save=ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    data=save.read_bytes();px,pz=struct.unpack_from('<ff',data,8)
    assert data[26]==0 and data[27]==0 and struct.unpack_from('<I',data,16)[0]==7
    assert abs(px+6.7)<.01 and abs(pz-.1)<.01,(px,pz)
    x('keydown','m');time.sleep(.1);x('keyup','m')
