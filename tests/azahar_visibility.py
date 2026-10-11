"""Capture moving/jumping through foreground cutaways; review images manually."""
from pathlib import Path
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def hold(key,seconds,pause=.35):
        x('keydown',key);time.sleep(seconds);x('keyup',key);time.sleep(pause)
    capture('visibility-start')
    hold('Down',.35);capture('visibility-front')
    hold('a',.08,.2);capture('visibility-jump');time.sleep(.5)
    hold('Left',.65);capture('visibility-left')
    hold('Up',.6);capture('visibility-rear')
    hold('m',.08)
