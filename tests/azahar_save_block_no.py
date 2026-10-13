"""Original repeat-hit prompt and No with isolated SD byte comparison."""
import runpy
import subprocess
import time
from pathlib import Path
def run(env,capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_save_block_visual.py')))['run'](env,capture)
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    before=save.read_bytes()
    def press(key,after=.5):
        subprocess.run(['xdotool','keydown',key],env=env,check=True);time.sleep(.1)
        subprocess.run(['xdotool','keyup',key],env=env,check=True);time.sleep(after)
    press('a',6);capture('saveblock-original-choice')
    press('g',1);capture('saveblock-no-selected');press('a',3)
    capture('saveblock-no-return')
    assert save.read_bytes()==before,'No wrote the checkpoint'
    assert not Path(str(save)+'.tmp').exists(),'No started a write'
    assert not Path(str(save)+'.bak').exists(),'No reached the native writer'
