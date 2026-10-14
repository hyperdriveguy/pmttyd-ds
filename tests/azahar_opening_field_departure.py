"""Fresh original opening; observe field control during the detached ship child."""
import subprocess,time
from pathlib import Path
from PIL import Image

def run(env,capture):
 def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
 output=Path(env['PM_PORT_SMOKE_OUTPUT'])
 x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
 def press():x('keydown','a');time.sleep(.15);x('keyup','a')
 press();time.sleep(16);capture('title');press();time.sleep(5);capture('files');press();time.sleep(3);capture('house')
 start=time.monotonic();found=False
 for n in range(181):
  press();time.sleep(max(0,start+n-time.monotonic()))
  if n%3==0 or n>=90:
   name='opening-%03d'%n;capture(name)
   if n>=90:
    image=Image.open(output/(name+'.png')).convert('RGB');r,g,b=image.getpixel((350,460))
    if r<90 and g<90 and b<110:
     found=True;capture('field-release-with-ship')
     x('keydown','Left');time.sleep(.6);capture('field-left-with-ship');x('keyup','Left')
     time.sleep(1);capture('field-child-continuing')
     time.sleep(5);capture('field-child-retired');break
 if not found:raise RuntimeError('Fresh route did not expose ordinary field control; inspect captures/admission notice')
 capture('final')
