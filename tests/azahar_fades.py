"""Capture dual-screen road fades; validate dimming and saved destination."""
from pathlib import Path
import subprocess
import time
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    samples=[]
    probes=[(470,236),(649,665)] # Paper fill, away from panel edges/text.
    with Image.open(ROOT/'artifacts/azahar/boot.png') as picture:
        assert all(sum(picture.convert('RGB').getpixel(p))/3>220 for p in probes)
    x('keydown','Right')
    try:
        for n in range(12):
            time.sleep(.04);name=f'fade-{n:02d}';capture(name)
            with Image.open(ROOT/'artifacts/azahar'/f'{name}.png') as picture:
                samples.append(tuple(sum(picture.convert('RGB').getpixel(p))/3 for p in probes))
    finally:x('keyup','Right')
    time.sleep(2);capture('fade-finished')
    save=ROOT/'artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    assert save.read_bytes()[26]==2
    with Image.open(ROOT/'artifacts/azahar/fade-finished.png') as picture:
        assert all(sum(picture.convert('RGB').getpixel(p))/3>220 for p in probes)
    print('Fade top/bottom brightness samples:',samples)
    for screen in range(2):
        values=[s[screen] for s in samples]
        assert min(values)<20 and any(20<v<200 for v in values),values
    x('keydown','m');time.sleep(.08);x('keyup','m')
