"""Capture source save-block idle layers with an isolated native fixture."""
import subprocess
import time

def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def press(key):
        x('keydown',key);time.sleep(.22);x('keyup',key)
    # Current native frontend: Demo -> Title -> Files -> saved checkpoint.
    for _ in range(3):press('a');time.sleep(1.2)
    time.sleep(6) # Allow cold room/animation imports and emulator shader setup.
    capture('saveblock-idle')
    time.sleep(.7);capture('saveblock-scroll')
    time.sleep(.7);capture('saveblock-glow')
