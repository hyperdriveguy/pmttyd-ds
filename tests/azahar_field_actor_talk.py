"""Capture original east Mouser talk with an isolated native story8 fixture."""
from pathlib import Path
import struct
import subprocess
import time

def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def press(key):
        x('keydown',key);time.sleep(.1);x('keyup',key)
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    previous=save.stat().st_mtime_ns
    press('a');time.sleep(.6);capture('original-mouser-talk')
    for n in range(12):
        press('a');time.sleep(.5)
        if n in (2,5):capture('mouser-page-'+str(n))
        press('x');time.sleep(.3) # Native save is ignored until the source window closes.
        if save.stat().st_mtime_ns!=previous:break
    else:raise AssertionError('Original talk did not return to exploration/save admission')
    capture('talk-return')
    data=save.read_bytes()
    assert len(data)==40 and data[26]==2 and data[32] and struct.unpack_from('<i',data,28)[0]==8
    checksum=2166136261
    for byte in data[:36]:checksum=((checksum^byte)*16777619)&0xffffffff
    assert struct.unpack_from('<I',data,36)[0]==checksum
