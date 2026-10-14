"""Isolated source-board contact/list/diary/reopen capture; v10 mask4 fixture.
Timing controls are fixture policy. Assertions wait for actual original UI pixels,
not an assumed callback duration. Screenshots still require visual inspection.
"""
from pathlib import Path
import subprocess,time
from PIL import Image

def run(env,capture):
 output=Path(env['PM_PORT_SMOKE_OUTPUT'])
 def key(action,name):subprocess.run(['xdotool',action,name],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
 def press(name):
  key('keydown',name);time.sleep(.12);key('keyup',name);time.sleep(.5)
 def wait_menu():
  for _ in range(30):
   capture('waiting-for-menu');image=Image.open(output/'waiting-for-menu.png').convert('RGB')
   if all(min(image.getpixel(p))>170 and max(image.getpixel(p))-min(image.getpixel(p))<20 for p in [(600,380),(620,390)]):return
   time.sleep(.5)
  raise RuntimeError('Original task list did not become visible; inspect waiting-for-menu.png')
 window=subprocess.check_output(['xdotool','search','--onlyvisible','--class','azahar'],env=env,text=True,stderr=subprocess.DEVNULL).splitlines()[-1]
 subprocess.run(['xdotool','windowfocus',window],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
 capture('board-start')
 key('keydown','Up');time.sleep(.5);key('keydown','a');key('keyup','Up');time.sleep(.12);key('keyup','a');time.sleep(.5)
 wait_menu();capture('original-task-list')
 press('Down');time.sleep(.6);capture('original-task2-highlight')
 press('a');time.sleep(4);capture('original-task2-diary')
 for page in (2,3):
  press('a');time.sleep(3);capture(f'original-task2-diary-page{page}')
 press('a');time.sleep(3);capture('original-task2-confirmation')
 press('s');time.sleep(3);wait_menu();capture('original-task2-declined-reopen')
