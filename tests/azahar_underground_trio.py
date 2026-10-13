"""Capture the original underground approach event and prebattle dialogue."""
import subprocess,time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('trio-before-crossing')
    x('keydown','Right');time.sleep(.6);x('keyup','Right');time.sleep(2)
    capture('trio-original-086')
    for i in range(28):
        x('keydown','a');time.sleep(.1);x('keyup','a');time.sleep(.4)
        if i in (3,6,9,12,15,19,23):capture('trio-page-'+str(i))
    time.sleep(3);capture('trio-battle-boundary')
