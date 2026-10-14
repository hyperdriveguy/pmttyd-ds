"""First-contact source lecture capture, isolated v10 room2/mask4 fixture."""
import subprocess,time

def run(env,capture):
 def key(action,name):subprocess.run(['xdotool',action,name],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
 def press(name):
  key('keydown',name);time.sleep(.12);key('keyup',name);time.sleep(.7)
 window=subprocess.check_output(['xdotool','search','--onlyvisible','--class','azahar'],env=env,text=True,stderr=subprocess.DEVNULL).splitlines()[-1]
 subprocess.run(['xdotool','windowfocus',window],env=env,check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
 key('keydown','Up');time.sleep(.5);key('keydown','a');key('keyup','Up');time.sleep(.12);key('keyup','a');time.sleep(.5);time.sleep(5)
 capture('original-first-lecture')
 for _ in range(2):press('a');time.sleep(5)
 capture('original-first-lecture-selection')
