"""Inspect the original no-input enemy impacts and recovery in an isolated profile.

HP, private guard history and event ownership are asserted by host tests.
"""
import subprocess
import time


def run(env, capture):
    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()

    def press():
        x('keydown', 'a')
        time.sleep(.1)
        x('keyup', 'a')
        time.sleep(.4)

    x('windowfocus', x('search', '--onlyvisible', '--class', 'azahar').splitlines()[-1])
    x('keydown', 'Right')
    time.sleep(.6)
    x('keyup', 'Right')
    time.sleep(2)
    for _ in range(57):
        press()
    time.sleep(35)
    capture('partner-menu')
    press()
    time.sleep(1)
    capture('partner-ground-target')
    press()
    for index in range(16):
        time.sleep(1)
        capture('enemy-impact-recovery-%02d' % index)
    time.sleep(15)
    capture('source-next-player-command')
