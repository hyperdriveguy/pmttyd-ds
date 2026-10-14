"""Verify original ready/advance star with the supported DEBUG_SHORT_BOOT build."""
import subprocess
import time


def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('ready-star-0');time.sleep(.17);capture('ready-star-1')
    x('keydown','a');time.sleep(.08);x('keyup','a');time.sleep(.35);capture('advanced-star-hidden')
    time.sleep(1.6);capture('next-text-typing')
    time.sleep(7);capture('next-page-ready-star')
