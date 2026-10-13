"""Inspect native original NormalJump from the isolated underground checkpoint."""
import subprocess,time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    x('keydown','Right');time.sleep(.6);x('keyup','Right');time.sleep(2)
    for i in range(65):
        x('keydown','a');time.sleep(.1);x('keyup','a');time.sleep(.4)
        if i in (42,44,46,48,50,52,54,56,60):capture('jump-'+str(i))
    time.sleep(2);capture('original-jump-damage-boundary')
