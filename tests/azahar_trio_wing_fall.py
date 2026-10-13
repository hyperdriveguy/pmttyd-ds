"""Inspect the original Paragoomba part/pose transition and grounded targeting.

HP, retained definition, installed events and final removal have host assertions.
"""
import runpy
import subprocess
import time
from pathlib import Path


def run(env, capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_trio_hammer.py')))['run'](env, capture)

    def press(key):
        subprocess.check_output(['xdotool', 'keydown', key], env=env, stderr=subprocess.DEVNULL)
        time.sleep(.1)
        subprocess.check_output(['xdotool', 'keyup', key], env=env, stderr=subprocess.DEVNULL)
        time.sleep(.4)

    press('a')
    press('h')
    capture('headbonk-flying-paragoomba-target')
    press('a')
    for index in range(30):
        time.sleep(.2)
        capture('original-wing-fall-%02d' % index)
    time.sleep(12)
    capture('next-mario-menu-after-grounded-attack')
    press('a')
    press('h')
    capture('grounded-goomba-name-and-target')
