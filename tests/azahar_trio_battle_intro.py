"""Isolated native original field scene followed by Frankly's battle introduction."""
import subprocess,time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    x('keydown','Right');time.sleep(.6);x('keyup','Right');time.sleep(2)
    for i in range(65):
        x('keydown','a');time.sleep(.1);x('keyup','a');time.sleep(.4)
        if i in (18,24,30,36,42,48,54,60):capture('battle-intro-'+str(i))
    time.sleep(3);capture('battle-intro-complete')
