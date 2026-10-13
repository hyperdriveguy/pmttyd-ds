"""Source story4 resume/admission and original022..024; isolated Azahar only."""
import subprocess,time,struct
from pathlib import Path

def run(env,capture):
 def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
 x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
 def press(key):x('keydown',key);time.sleep(.10);x('keyup',key);time.sleep(.30)
 press('a');press('a');press('a');time.sleep(2);capture('story4-resume')
 save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
 if struct.unpack_from('<i',save.read_bytes(),28)[0]==4:
  x('keydown','Right');time.sleep(1.8);x('keyup','Right')
  x('keydown','Up');time.sleep(1.8);x('keyup','Up')
 time.sleep(1);capture('source-admission')
 for n in range(180):
  press('a')
  if n in [8,20,36,60,90,120,160]:capture('beforebattle-'+str(n))
 capture('original-tutorial-intro-or-pending-combat')
