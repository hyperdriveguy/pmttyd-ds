"""Manual defense captures for emulator_smoke.py --fresh-save.

Wall-clock inputs target the development harness's provisional impact schedule;
the host suites assert exact simulation/history boundaries. Review captures;
this is not an original-game comparison or a pixel acceptance test.
"""
import subprocess
import time


def run(env, capture):
    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
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
    capture('defense-select')
    key('a')  # No Jump timing press: first attack deals one damage.
    time.sleep(2.0)
    x('keydown', 'a')
    time.sleep(.08)
    x('keyup', 'a')
    time.sleep(.15)
    capture('defense-single')
    time.sleep(.7)
    key('a')
    time.sleep(1.92)
    for _ in range(2):
        x('keydown', 'a')
        time.sleep(.025)
        x('keyup', 'a')
        time.sleep(.025)
    time.sleep(.35)
    capture('defense-mash')
    time.sleep(.2)
    capture('defense-mash-after')
