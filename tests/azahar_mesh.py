"""Capture original harbor wave playback; room/save assertions, manual visuals."""
from pathlib import Path
import struct,subprocess,time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def hold(key,seconds):x('keydown',key);time.sleep(seconds);x('keyup',key);time.sleep(1.5)
    save=ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    hold('Down',.4);assert save.read_bytes()[26]==1
    hold('Down',.8);capture('harbor-mesh-0');time.sleep(2);capture('harbor-mesh-120');time.sleep(2);capture('harbor-mesh-240')
    hold('Up',1.2);capture('harbor-mesh-return')
    assert save.read_bytes()[26]==0,'Harbor retreat did not return to the plaza'
    assert struct.unpack_from('<I',save.read_bytes(),16)[0]==7
    hold('m',.08)
