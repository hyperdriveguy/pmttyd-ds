"""Inspect original stationary NPC art/talk after ordinary frontend checkpoint load."""
from pathlib import Path
import struct
import subprocess
import time
from PIL import Image

def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def press(key):
        x('keydown',key);time.sleep(.1);x('keyup',key)
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    initial=save.read_bytes()
    # Asset loading can outlast a fixed boot delay. Require both the field HUD
    # and shaded Rogueport geometry; title/file artwork is not a field result.
    for _ in range(20):
        capture('stationary-npc-load')
        with Image.open(Path(env['PM_PORT_SMOKE_OUTPUT'])/'stationary-npc-load.png') as shot:
            rgb=shot.convert('RGB')
            hud=sum(min(c)>180 and max(c)-min(c)<45 for c in rgb.crop((310,219,475,241)).get_flattened_data())
            brick=sum(70<r<210 and r>g+10 and g>b+15 for r,g,b in rgb.crop((500,325,650,400)).get_flattened_data())
        if hud>1800 and brick>500:break
        press('a');time.sleep(2)
    else:raise AssertionError('Original Rogueport field did not load')
    time.sleep(6)
    if struct.unpack_from('<f',initial,8)[0]<0:
        # Original alley camera: right advances toward Darkly to source contact.
        x('keydown','Right');time.sleep(1.2);x('keyup','Right');time.sleep(.5)
    capture('stationary-npc-field')
    previous=save.stat().st_mtime_ns
    press('a');time.sleep(.6);capture('stationary-npc-talk')
    with Image.open(Path(env['PM_PORT_SMOKE_OUTPUT'])/'stationary-npc-talk.png') as shot:
        pixels=shot.convert('RGB').crop((300,235,700,325)).get_flattened_data()
        assert sum(min(rgb)>210 and max(rgb)-min(rgb)<12 for rgb in pixels)>3500, 'Original message window did not open'
    assert save.stat().st_mtime_ns==previous, 'Talk onset wrote the checkpoint'
    for n in range(12):
        press('a');time.sleep(.5)
        if n in (2,5):capture('stationary-npc-page-'+str(n))
        press('x');time.sleep(.3) # Native save is ignored until the source window closes.
        if save.stat().st_mtime_ns!=previous:break
    else:raise AssertionError('Original talk did not return to exploration/save admission')
    capture('talk-return')
    data=save.read_bytes()
    px,pz=struct.unpack_from('<ff',data,8)
    if struct.unpack_from('<f',initial,8)[0]<0:
        assert abs(px+3.35)<.02 and -3.64<pz<-3.61,(px,pz)
    else:
        assert abs(px-1.2)<.02 and abs(pz+4)<.02,(px,pz)
    assert len(data)==40 and data[26]==2 and data[32] and struct.unpack_from('<i',data,28)[0]==8
    checksum=2166136261
    for byte in data[:36]:checksum=((checksum^byte)*16777619)&0xffffffff
    assert struct.unpack_from('<I',data,36)[0]==checksum

    print('Original stationary NPC talk, return and unchanged story/checksum passed')
