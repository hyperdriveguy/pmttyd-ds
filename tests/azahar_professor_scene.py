"""Isolated original first-talk walkthrough; timings are fixture controls."""
import subprocess,time
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('professor-outside');x('keydown','Up');time.sleep(.8);x('keyup','Up')
    x('keydown','a');time.sleep(.1);x('keyup','a');time.sleep(4)
    capture('professor-first-message')
    for i in range(320):
        x('keydown','a');time.sleep(.12);x('keyup','a');time.sleep(.12)
        if i in (20,60,120,180,240,319):capture('professor-page-'+str(i))
