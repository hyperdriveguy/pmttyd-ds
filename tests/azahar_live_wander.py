"""Inspect original live wandering art and retained candidate pause in Azahar."""
from pathlib import Path
import struct
import subprocess
import time
from PIL import Image


def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    def press(key):x('keydown',key);time.sleep(.12);x('keyup',key)
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    initial=save.read_bytes();observing=struct.unpack_from('<f',initial,8)[0]>-3.3
    for _ in range(3):press('a');time.sleep(1.2)
    time.sleep(6);capture('goomba-idle-or-pause')
    if observing:
        time.sleep(3);capture('goomba-wander-1')
        time.sleep(3);capture('goomba-wander-2')
        time.sleep(3);capture('goomba-wander-3')
        centers=[]
        for name in ['goomba-idle-or-pause','goomba-wander-1','goomba-wander-2','goomba-wander-3']:
            with Image.open(Path(env['PM_PORT_SMOKE_OUTPUT'])/(name+'.png')) as shot:
                rgb=shot.convert('RGB');points=[]
                for y in range(320,390):
                    for px in range(315,430):
                        r,g,b=rgb.getpixel((px,y))
                        if g>r+20 and g>b+20 and g>70 and r<140:points.append(px)
                assert len(points)>10,'Original Goomba hat/body was not visible in the fixture'
                centers.append(sum(points)/len(points))
        assert max(centers)-min(centers)>12,'Original NPC did not visibly move in the stationary-camera fixture'
        print('Original sprite movement centers:',centers)
    else:
        press('a');time.sleep(.6);capture('goomba-unbound-event')
        with Image.open(Path(env['PM_PORT_SMOKE_OUTPUT'])/'goomba-unbound-event.png') as shot:
            pixels=shot.convert('RGB').crop((300,235,700,325)).get_flattened_data()
            assert sum(min(rgb)>210 and max(rgb)-min(rgb)<12 for rgb in pixels)<3500, 'Unsupported quest opened substitute dialogue'
    previous=save.stat().st_mtime_ns;press('x');time.sleep(.5)
    assert save.stat().st_mtime_ns!=previous,'Exploration did not admit save'
    data=save.read_bytes();assert len(data)==40 and data[26]==2 and struct.unpack_from('<i',data,28)[0]==8
    px,pz=struct.unpack_from('<ff',data,8)
    assert abs(px-struct.unpack_from('<f',initial,8)[0])<.02 and abs(pz-1.3)<.02,(px,pz)
    checksum=2166136261
    for byte in data[:36]:checksum=((checksum^byte)*16777619)&0xffffffff
    assert struct.unpack_from('<I',data,36)[0]==checksum
    capture('goomba-field-return')
    print('Original live wandering captures, unavailable quest admission and unchanged field save passed',observing)
