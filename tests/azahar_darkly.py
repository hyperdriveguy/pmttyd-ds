"""Verify original Darkly art/contact/early US talk using an isolated fixture."""
from pathlib import Path
import struct
import subprocess
import time
from PIL import Image


def run(env, capture):
    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()
    def press(key):
        x('keydown', key); time.sleep(.12); x('keyup', key)
    x('windowfocus', x('search', '--onlyvisible', '--class', 'azahar').splitlines()[-1])
    for _ in range(3):
        press('a'); time.sleep(1.2)
    time.sleep(6)
    capture('darkly-idle')
    # This original alley camera settles at90degrees: right moves world+Z.
    x('keydown', 'Right'); time.sleep(1.2); x('keyup', 'Right'); time.sleep(.5)
    capture('darkly-contact')
    save = Path(env['PM_PORT_SMOKE_OUTPUT']) / 'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    previous = save.stat().st_mtime_ns
    press('x'); time.sleep(.5)
    assert save.stat().st_mtime_ns != previous, 'Contact exploration save was not admitted'
    data = save.read_bytes()
    player_x, player_z = struct.unpack_from('<ff', data, 8)
    assert abs(player_x + 3.35) < .02 and -3.64 < player_z < -3.61, (player_x, player_z)
    previous = save.stat().st_mtime_ns
    press('a'); time.sleep(.6); capture('darkly-original-talk')
    with Image.open(Path(env['PM_PORT_SMOKE_OUTPUT']) / 'darkly-original-talk.png') as shot:
        pixels = shot.convert('RGB').crop((300,235,700,325)).get_flattened_data()
        assert sum(min(rgb)>210 and max(rgb)-min(rgb)<12 for rgb in pixels)>3500, 'Original Darkly message window did not open'
    assert save.stat().st_mtime_ns == previous, 'Talk onset wrote the checkpoint'
    for n in range(20):
        press('a'); time.sleep(.5)
        if n in (2,5): capture('darkly-page-' + str(n))
        press('x'); time.sleep(.3)
        if save.stat().st_mtime_ns != previous: break
    else: raise AssertionError('Original Darkly talk did not return to exploration/save admission')
    capture('darkly-talk-return')
    data = save.read_bytes()
    assert len(data)==40 and data[26]==2 and data[32] and struct.unpack_from('<i',data,28)[0]==8
    checksum = 2166136261
    for byte in data[:36]: checksum = ((checksum ^ byte) * 16777619) & 0xffffffff
    assert struct.unpack_from('<I',data,36)[0]==checksum
    print(f'Original Darkly contact ({player_x:.6f}, {player_z:.6f}), visible talk and return passed')
