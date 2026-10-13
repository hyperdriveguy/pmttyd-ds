"""Save inside the inn; retain the isolated save for a subsequent reload check."""
from pathlib import Path
import subprocess
import time

def run(env, capture):
    # Use only the entry half of the existing fixture controls.
    def key(name, seconds):
        subprocess.run(['xdotool','keydown',name],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
        try:time.sleep(seconds)
        finally:subprocess.run(['xdotool','keyup',name],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
    windows=subprocess.check_output(['xdotool','search','--onlyvisible','--class','azahar'],env=env,text=True,stderr=subprocess.DEVNULL).splitlines()
    subprocess.run(['xdotool','windowfocus',windows[-1]],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
    key('Up',.8);key('a',.1);time.sleep(3)
    capture('inn-before-save')
    key('x',.1);time.sleep(1)
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    b=save.read_bytes()
    assert len(b)==40 and b[4]==4 and b[26]==0 and b[33]==1, 'Indoor v4 state was not saved'
    capture('inn-saved')
