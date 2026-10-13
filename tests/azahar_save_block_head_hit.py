"""Inspect original save-block head collision in an isolated native checkpoint."""
import runpy
from pathlib import Path
import subprocess
import time

def run(env,capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_save_block_visual.py')))['run'](env,capture)
    subprocess.run(['xdotool','keydown','a'],env=env,check=True)
    time.sleep(.065);capture('saveblock-head-contact')
    subprocess.run(['xdotool','keyup','a'],env=env,check=True)
    time.sleep(.8);capture('saveblock-head-landed')
