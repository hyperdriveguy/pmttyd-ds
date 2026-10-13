"""Capture original first page deformation; isolated profile, no fidelity assertion."""
import time,subprocess

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    started=time.monotonic();time.sleep(30)
    for n in range(81):
        time.sleep(max(0,started+30+n*.25-time.monotonic()))
        capture('page-sample-%02u'%n)
    x('keydown','a');time.sleep(.1);x('keyup','a');time.sleep(1);capture('title-after-skip')
