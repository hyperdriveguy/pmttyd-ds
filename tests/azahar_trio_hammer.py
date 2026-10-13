"""Inspect original Hammer poses and its no-input timeout against Spiked Goomba.

Packed held-left success/early-release and damage assertions live in host tests.
"""
import runpy
import subprocess
import time
from pathlib import Path


def run(env, capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_trio_enemy_impacts.py')))['run'](env, capture)

    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()

    def press(key):
        x('keydown', key)
        time.sleep(.1)
        x('keyup', key)
        time.sleep(.4)

    press('h')  # Azahar's default D-pad right selects the Hammer command.
    capture('hammer-command')
    press('a')
    capture('hammer-front-spiked-target')
    press('a')
    for index in range(24):
        time.sleep(.15)
        capture('original-hammer-%02d' % index)
    time.sleep(10)
    capture('hammer-recovered-partner-menu')
