"""Reach original EXP icons and victory poses through actual trio attacks.

This capture includes original PointGet art/counting and stops before field return.
"""
import runpy
import subprocess
import time
from pathlib import Path


def run(env, capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_trio_wing_fall.py')))['run'](env, capture)

    def press(key):
        subprocess.check_output(['xdotool', 'keydown', key], env=env, stderr=subprocess.DEVNULL)
        time.sleep(.1)
        subprocess.check_output(['xdotool', 'keyup', key], env=env, stderr=subprocess.DEVNULL)
        time.sleep(.4)

    press('s')  # Return from the preceding Jump target menu.
    press('h')
    press('a')
    capture('remaining-spiked-goomba-hammer-target')
    press('a')  # The earlier missed Hammer left this enemy at one HP.
    time.sleep(15)
    capture('partner-menu-after-original-spiked-removal')
    press('a')
    capture('last-grounded-goomba-headbonk-target')
    press('a')
    for index in range(55):
        time.sleep(.2)
        capture('original-exp-and-victory-%02d' % index)
    time.sleep(3)
    capture('original-pointget-counted-return-boundary')
