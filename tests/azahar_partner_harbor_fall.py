"""Capture the actual harbor deck jump over its brief fall window.

Native fixture timings only; original binary contracts supply the behavior.
Use artifacts/fixtures/partner-harbor-ledge.bin, generated with encodeSave or
the command in docs/behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md.
"""
from pathlib import Path
import struct
import subprocess
import time


def run(env,capture):
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    initial=save.read_bytes()
    assert initial[26]==1 and struct.unpack_from('<b',initial,27)[0]==12
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('harbor-deck-before')
    x('keydown','Down')
    try:
        time.sleep(.05)
        x('keydown','a')
        try:
            for n in range(3):
                time.sleep(.055);capture(f'harbor-jump-{n:02d}')
        finally:
            x('keyup','a')
        for n in range(12):
            time.sleep(.045);capture(f'partner-ledge-{n:02d}')
    finally:
        x('keyup','Down')
    time.sleep(2)
    capture('partner-dock-landed')
    # Reveal her beside Mario after a naturally occluded straight-line view.
    x('keydown','Left');time.sleep(.09);x('keyup','Left');time.sleep(.05)
    capture('partner-dock-side-view')
    x('keydown','x');time.sleep(.15);x('keyup','x');time.sleep(.6)
    final=save.read_bytes()
    assert final[26]==1 and struct.unpack_from('<b',final,27)[0] in (1,2)
    assert struct.unpack_from('<I',final,16)[0]==struct.unpack_from('<I',initial,16)[0]
