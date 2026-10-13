"""Exercise the native story3 checkpoint; not an original save/runtime trace."""
from pathlib import Path
import struct
import subprocess
import time


def run(env,capture):
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('first-original-dialogue')
    for n in range(100):
        x('keydown','a');time.sleep(.10);x('keyup','a');time.sleep(.14)
        if n in (12,30,48):capture('scene-step-'+str(n))
        b=save.read_bytes()
        if len(b)==40 and b[26]==1 and b[32] and struct.unpack_from('<i',b,28)[0]==4:
            capture('story4-reached');return
    raise AssertionError('Original story3 checkpoint did not reach story4')
