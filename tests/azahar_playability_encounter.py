"""Capture normal-file harbor story4 admission and physical tutorial inputs.

Use tools/make_harbor_encounter_fixture.py --resume and --seconds 35.
The route goes north of the pier railing before approaching Crump. Durations
are test input policy, not measured original mechanics. Exit0 means captures
completed; reviewing the pending notice is required to assess playability.
"""
from pathlib import Path
import runpy
import subprocess
import time

APPROACH_ROUTE = [('Up', 3), ('Right', 5)]
APPROACH_SETTLE_SECONDS = 1
DIALOGUE_PRESS_COUNT = 180
BUTTON_HOLD_SECONDS = 0.15
DIALOGUE_SETTLE_SECONDS = 0.35
CAPTURE_EVERY_PRESSES = 6
PHYSICAL_INPUTS = ['Right', 'a', 'Left', 'a']
PHYSICAL_HOLD_SECONDS = 0.2
PHYSICAL_SETTLE_SECONDS = 2
ENDING_WAIT_SECONDS = 10


def run(env, capture):
    field = Path(__file__).with_name('azahar_playability_field.py')
    runpy.run_path(str(field))['enter_field'](env, capture)

    def hold(key, seconds):
        def x(action):
            subprocess.run(['xdotool', action, key], env=env, check=True,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        x('keydown')
        try:
            time.sleep(seconds)
        finally:
            x('keyup')

    for key, seconds in APPROACH_ROUTE:
        hold(key, seconds)
        time.sleep(APPROACH_SETTLE_SECONDS)
        capture('approach-' + key)
    for press in range(DIALOGUE_PRESS_COUNT):
        hold('a', BUTTON_HOLD_SECONDS)
        time.sleep(DIALOGUE_SETTLE_SECONDS)
        if press % CAPTURE_EVERY_PRESSES == 0:
            capture('encounter-' + str(press))
    for key in PHYSICAL_INPUTS:
        hold(key, PHYSICAL_HOLD_SECONDS)
        time.sleep(PHYSICAL_SETTLE_SECONDS)
        capture('input-' + key)
    time.sleep(ENDING_WAIT_SECONDS)
    capture('ending-wait')
