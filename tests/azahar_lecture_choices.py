"""Isolated story10 reload, original choices, declined lesson and save/reload."""
import subprocess,time
from pathlib import Path
from PIL import Image

def run(env,capture):
    out=Path(env['PM_PORT_SMOKE_OUTPUT'])
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def press(key):x('keydown',key);time.sleep(.12);x('keyup',key);time.sleep(.16)
    def until(word):
        # Original choice windows occupy the central lower dialogue region.
        # Keep A released when the opaque rectangle is present, then inspect
        # the saved image; the resulting save is independently asserted below.
        for i in range(45):
            capture('lecture-live')
            pic=Image.open(out/'lecture-live.png').convert('RGB')
            row=[pic.getpixel((x,340)) for x in range(455,545)]
            white=sum(min(c)>200 and max(c)-min(c)<12 for c in row)
            if white>45:
                time.sleep(.5);return
            press('a');time.sleep(.45)
        raise AssertionError('Original choice rectangle not reached: '+word)
    capture('lecture-checkpoint');x('keydown','Down');time.sleep(1.2);x('keyup','Down')
    until('ofcourse');capture('lecture-first-choice');time.sleep(.3)
    press('Up');press('a');time.sleep(.5) # Original index0: knows the commands.
    until('nothanks');capture('lecture-second-choice');time.sleep(.3)
    press('Down');press('a') # Original index1: decline practice.
    for i in range(24):
        press('a')
        if i in (4,8,12):capture('lecture-decline-'+str(i))
    capture('lecture-story11');press('x');time.sleep(.5);capture('lecture-saved')
    save=out/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    raw=save.read_bytes();assert int.from_bytes(raw[28:32],'little')==11 and raw[32]==1
