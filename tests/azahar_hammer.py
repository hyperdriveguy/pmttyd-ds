"""Manual Hammer capture sequence for emulator_smoke.py --fresh-save.

Uses the isolated default window/control layout. Wall-clock holds put the input
inside broad charge regions; exact invocation boundaries are covered by host
tests. These screenshots require review and are not pixel assertions or a
comparison with original gameplay.
"""
import subprocess
import time


def run(env, capture):
    def x(*args):
        return subprocess.check_output(
            ['xdotool', *args], env=env, text=True,
            stderr=subprocess.DEVNULL).strip()

    window = x('search', '--onlyvisible', '--class', 'azahar').splitlines()[-1]
    x('windowfocus', window)

    def key(name):
        x('keydown', name)
        time.sleep(.08)
        x('keyup', name)
        time.sleep(.25)

    x('keydown', 'Right')
    time.sleep(.5)
    x('keyup', 'Right')
    time.sleep(.2)
    key('a')
    key('a')
    key('a')
    capture('hammer-select')
    key('h')  # D-pad right: ordinary Hammer in the development harness.
    key('a')
    x('keydown', 'Left')
    time.sleep(1.4)
    capture('hammer-charged')
    x('keyup', 'Left')
    time.sleep(.2)
    capture('hammer-release')
    time.sleep(2.5)
    key('a')  # Selection is retained across turns.
    x('keydown', 'Left')
    time.sleep(.1)
    x('keyup', 'Left')
    time.sleep(.2)
    capture('hammer-early-release')
    time.sleep(2.5)
    key('a')
    x('keydown', 'Left')
    time.sleep(1.9)
    x('keyup', 'Left')
    capture('hammer-overrun')
