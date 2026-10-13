"""Capture original speaker/page bubbles using a project-isolated story3 save."""
from pathlib import Path
import struct
import subprocess
import time


def run(env, capture):
    save = Path(env['PM_PORT_SMOKE_OUTPUT']) / 'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True, stderr=subprocess.DEVNULL).strip()
    x('windowfocus', x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    capture('speaker-opening')
    for n in range(50):
        x('keydown','a'); time.sleep(.1); x('keyup','a'); time.sleep(.9)
        capture(f'bubble-page-{n:02d}')
        b = save.read_bytes()
        if len(b)==40 and b[26]==1 and b[32] and struct.unpack_from('<i',b,28)[0]==4:
            capture('story4-overlays-restored')
            return
    raise AssertionError('Textbox playback did not reach story4')
