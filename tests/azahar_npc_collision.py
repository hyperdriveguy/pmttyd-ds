"""Check original Mouser side response and talk in an isolated native fixture."""
from pathlib import Path
import runpy
import struct
import subprocess
import time


def run(env, capture):
    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()
    x('windowfocus', x('search', '--onlyvisible', '--class', 'azahar').splitlines()[-1])
    for _ in range(3):
        x('keydown', 'a'); time.sleep(.22); x('keyup', 'a'); time.sleep(1.2)
    time.sleep(6)  # Cold original imports and source camera settling.
    capture('npc-before-contact')
    x('keydown', 'Left'); time.sleep(1.2); x('keyup', 'Left'); time.sleep(.5)
    capture('npc-contact')
    save = Path(env['PM_PORT_SMOKE_OUTPUT']) / 'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    before = save.stat().st_mtime_ns
    x('keydown', 'x'); time.sleep(.1); x('keyup', 'x'); time.sleep(.5)
    assert save.stat().st_mtime_ns != before, 'Exploration save was not admitted'
    data = save.read_bytes()
    assert len(data) == 40 and data[26] == 2 and struct.unpack_from('<i', data, 28)[0] == 8
    player_x, player_z = struct.unpack_from('<ff', data, 8)
    assert 1.21 < player_x < 1.24 and abs(player_z + 4) < .02, (player_x, player_z)
    checksum = 2166136261
    for byte in data[:36]:
        checksum = ((checksum ^ byte) * 16777619) & 0xffffffff
    assert struct.unpack_from('<I', data, 36)[0] == checksum
    # Existing original-message proof also checks onset/save suppression and return.
    runpy.run_path(str(Path(__file__).with_name('azahar_field_actor_talk.py')))['run'](env, capture)
    print(f'Original Mouser contact held Mario at ({player_x:.6f}, {player_z:.6f}); talk returned')
