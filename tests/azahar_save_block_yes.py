"""Original repeat-hit Yes, source write acknowledgement and persisted flags."""
import runpy
import struct
import subprocess
import time
from pathlib import Path
def run(env,capture):
    runpy.run_path(str(Path(__file__).with_name('azahar_save_block_visual.py')))['run'](env,capture)
    save=Path(env['PM_PORT_SMOKE_OUTPUT'])/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    def press(key,after=.5):
        subprocess.run(['xdotool','keydown',key],env=env,check=True);time.sleep(.1)
        subprocess.run(['xdotool','keyup',key],env=env,check=True);time.sleep(after)
    press('a',6);capture('saveblock-original-choice')
    press('a',6);capture('saveblock-written')
    data=save.read_bytes();assert len(data)==40 and struct.unpack_from('<I',data,4)[0]==6
    assert data[34]&6==6 and data[35]==0
    assert Path(str(save)+'.bak').exists(),'Yes did not reach the native atomic writer'
    press('a',1);capture('saveblock-success-return')
