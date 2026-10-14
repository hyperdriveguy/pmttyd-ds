"""Capture source story3/4 actor motion; isolated development checkpoint only.

Active-event saves are suppressed, so progression is assessed in captures and
host event tests instead of requiring a new on-disk story4 checkpoint.
"""
import subprocess
import time


def run(env, capture):
    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()
    window = x('search', '--onlyvisible', '--class', 'azahar').splitlines()[-1]
    x('windowfocus', window)
    capture('opening-dialogue')
    for n in range(160):
        x('keydown', 'a');time.sleep(.10);x('keyup', 'a');time.sleep(.16)
        if n in (4, 12, 24, 36, 48, 60, 80, 100, 130, 159):
            capture('opening-step-' + str(n))
    capture('opening-end')
