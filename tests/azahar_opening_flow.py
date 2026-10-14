"""Fresh-file title → opening → saved harbor checkpoint, isolated smoke only."""
import os,struct,subprocess,time
from pathlib import Path

def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def press(key):
        x('keydown',key);time.sleep(.08);x('keyup',key)
    press('a');time.sleep(16);capture('title')
    press('a');time.sleep(5);capture('files')
    press('a');time.sleep(3);capture('opening-house')
    checkpoint=Path(env['XDG_DATA_HOME'])/'azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    end=time.monotonic()+160
    for tick in range(800):
        if checkpoint.exists():
            b=checkpoint.read_bytes()
            if len(b)==40 and b[:4]==b'PM2D' and b[26]==1 and b[32]==1 and struct.unpack_from('<i',b,28)[0]==3:
                time.sleep(2);capture('opening-harbor');return
        if time.monotonic()>end:break
        press('a');time.sleep(.20)
        if tick in [35,70,110,150,200,260,350]:capture('opening-step-'+str(tick))
    capture('opening-timeout')
    raise AssertionError('Fresh opening did not reach and save the source story3 harbor checkpoint')
