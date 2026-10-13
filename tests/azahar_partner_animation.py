"""Inspect source Goombella poses with longer holds under software emulation.

Capture timings are fixture controls, not original animation timing evidence.
"""
from pathlib import Path
import struct
import subprocess
import time


def run(env, capture):
    output = Path(env['PM_PORT_SMOKE_OUTPUT'])
    save = output/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    initial = save.read_bytes()
    initial_x = struct.unpack_from('<f', initial, 8)[0]
    assert initial[26] == 0 and initial_x < -3, 'Use the west-road fixture'

    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()

    x('windowfocus', x('search', '--onlyvisible', '--class', 'azahar').splitlines()[-1])
    capture('partner-idle-before')
    x('keydown', 'Right')
    try:
        time.sleep(.6)
        capture('partner-run-early')
        time.sleep(1.2)
        capture('partner-run-later')
    finally:
        x('keyup', 'Right')
    time.sleep(1.5)
    capture('partner-idle-after')
    x('keydown', 'Left')
    try:
        time.sleep(.6)
        capture('partner-run-left')
        time.sleep(.2)
    finally:
        x('keyup', 'Left')
    time.sleep(1.5)
    capture('partner-idle-left')
    x('keydown', 'a')
    try:
        time.sleep(.2)
        capture('mario-jump-partner-grounded')
    finally:
        x('keyup', 'a')
    time.sleep(2)
    capture('landed-with-partner')
    x('keydown', 'x')
    time.sleep(.25)
    x('keyup', 'x')
    time.sleep(1)
    saved = save.read_bytes()
    assert saved[26] == 0 and struct.unpack_from('<f', saved, 8)[0] > initial_x+.1
