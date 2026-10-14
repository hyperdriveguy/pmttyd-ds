"""Review the original animated advance star in isolated harbor checkpoint UI."""
import subprocess
import time


def run(env,capture):
    def x(*args):
        return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def press():
        x('keydown','a');time.sleep(.08);x('keyup','a')
    from pathlib import Path
    from PIL import Image
    out=Path(env['PM_PORT_SMOKE_OUTPUT'])
    for n in range(100):
        press();time.sleep(1)
        capture('navigation')
        image=Image.open(out/'navigation.png').convert('RGB')
        gray=sum(1 for y in range(240,295,2) for x in range(380,620,2)
                 if all(221<=c<=227 for c in image.getpixel((x,y))))
        if gray>800:
            break
    else:
        raise AssertionError('Did not enter the harbor dialogue window')
    time.sleep(3);capture('ready-star-0')
    time.sleep(.35);capture('ready-star-1')
    press();time.sleep(.35);capture('advanced-star-hidden')
    time.sleep(1.6);capture('next-text-typing')
    time.sleep(7);capture('next-page-ready-star')
