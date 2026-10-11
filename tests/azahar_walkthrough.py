"""Input/capture sequence for tools/emulator_smoke.py; review captures manually.
Uses the isolated profile's default Azahar keyboard mappings. Requires a fresh
exploration save and the current small centered default screen layout.
"""
import subprocess
import time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    windows=x('search','--onlyvisible','--class','azahar').splitlines()
    window=windows[-1];x('windowfocus',window)
    def key(name):
        x('keydown',name);time.sleep(.08);x('keyup',name);time.sleep(.25)
    def click(px,py):x('mousemove',str(px),str(py));x('mousedown','1');time.sleep(.08);x('mouseup','1');time.sleep(.3)
    click(443,514);capture('touch-map')
    click(608,514);capture('touch-settings')
    click(395,514)
    x('keydown','Right');time.sleep(.5);x('keyup','Right');time.sleep(.2)
    capture('movement')
    key('a');capture('dialogue')
    key('a');key('a');capture('battle-select')
    for n in range(5):
        key('a')
        # No action-command press: one damage. The core suite tests precise windows.
        time.sleep(2.7)
        capture(f'battle-turn-{n+1}')
    key('a');capture('return-plaza')
    key('x');capture('save-feedback')
    key('m');time.sleep(.3)
