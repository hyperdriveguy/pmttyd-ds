"""Original three-page first-hit explanation, then No without a checkpoint write."""
import runpy
import subprocess
import time
from pathlib import Path
def run(env,capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_save_block_visual.py')))['run'](env,capture)
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    before=save.read_bytes()
    def press(key,after):
        subprocess.run(['xdotool','keydown',key],env=env,check=True);time.sleep(.1)
        subprocess.run(['xdotool','keyup',key],env=env,check=True);time.sleep(after)
    press('a',6);capture('saveblock-first-page1')
    press('a',4);capture('saveblock-first-page2')
    press('a',4);capture('saveblock-first-page3')
    press('a',5);capture('saveblock-first-choice')
    press('g',1);capture('saveblock-first-no');press('a',3)
    capture('saveblock-first-return')
    assert save.read_bytes()==before and not Path(str(save)+'.bak').exists()
