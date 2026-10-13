"""Inspect native setup4 source targeting after the original Frankly intro."""
import subprocess,time

def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    def press(key):
        x('keydown',key);time.sleep(.1);x('keyup',key);time.sleep(.4)
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    x('keydown','Right');time.sleep(.6);x('keyup','Right');time.sleep(2)
    for _ in range(65):press('a')
    time.sleep(2)
    # Repeated intro advances can reach the unconsumed dispatch notice. Its B
    # adapter returns to target selection, then B returns to command selection.
    press('s');press('s');capture('commands')
    press('a');capture('jump-goomba')
    press('h');capture('jump-spiked')
    press('h');capture('jump-paragoomba')
    press('s');press('h');press('a');capture('hammer-front-goomba')
    press('a');capture('retained-attack-request')
