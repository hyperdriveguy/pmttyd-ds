"""Capture missed Jump/Headbonk followed by the original enemy approach.

This is a manual inspection sequence; packed phase/HP assertions live in host tests.
"""
import subprocess
import time

def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,
            stderr=subprocess.DEVNULL).strip()
    def press():
        x('keydown','a');time.sleep(.1);x('keyup','a');time.sleep(.4)
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    x('keydown','Right');time.sleep(.6);x('keyup','Right');time.sleep(2)
    for _ in range(57):press()
    capture('first-player-command')
    time.sleep(35)
    capture('partner-menu-after-missed-jump')
    press();time.sleep(1);capture('partner-ground-target')
    press();time.sleep(30)
    capture('enemy-phase-approach-and-impact-boundary')
