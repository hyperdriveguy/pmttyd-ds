"""Isolated original conversation, exit callback, fence and lecture-entry replay."""
import subprocess,time
from pathlib import Path
import runpy

def run(env,capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_professor_scene.py')))['run'](env,capture)
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    def a():x('keydown','a');time.sleep(.12);x('keyup','a');time.sleep(.18)
    time.sleep(1);x('keydown','Left');time.sleep(.33);x('keyup','Left');x('keydown','Down');time.sleep(2);x('keyup','Down');capture('professor-exit-contact')
    a();time.sleep(7);capture('professor-fence-first-message')
    for i in range(45):
        a()
        if i in (3,7,11,15,23,31,44):capture('professor-fence-'+str(i))
    capture('professor-fence-done');x('keydown','Left');time.sleep(.33);x('keyup','Left');x('keydown','Down');time.sleep(6);x('keyup','Down')
    for i in range(35):a()
    capture('professor-lecture-choice-request')
