"""Inspect the persistent Mario/Goombella command handoff in an isolated profile.

Manual captures only; host integration tests assert packed HP/event ownership.
"""
import runpy
import subprocess
import time
from pathlib import Path

def run(env,capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_trio_jump.py')))['run'](env,capture)
    time.sleep(30)
    capture('partner-command-handoff')
    def press():
        subprocess.run(['xdotool','keydown','a'],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
        time.sleep(.1)
        subprocess.run(['xdotool','keyup','a'],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
        time.sleep(1)
    press()
    capture('partner-target')
    press()
    time.sleep(3)
    capture('headbonk-action')
    time.sleep(25)
    capture('headbonk-return-and-phase-boundary')
