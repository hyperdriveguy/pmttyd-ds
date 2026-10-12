"""Walk beyond a valid plaza floor, recover, and preserve lethal-fall checkpoint."""
from pathlib import Path
import struct
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    def hold(key,seconds):
        x('keydown',key);time.sleep(seconds);x('keyup',key);time.sleep(.3)
    output=Path(env.get('PM_PORT_SMOKE_OUTPUT',ROOT/'artifacts/azahar'))
    save=output/'data/azahar-emu/sdmc/3ds/ttyd-2ds/progress.bin'
    checkpoint=save.read_bytes();hp=struct.unpack_from('<I',checkpoint,16)[0]
    assert checkpoint[26]==0 and hp in (1,7)
    hold('Down',.5);capture('void-falling');hold('x',.08)
    assert save.read_bytes()==checkpoint,'Unsupported fall must preserve the checkpoint'
    time.sleep(2);capture('void-return');time.sleep(3);capture('void-result')
    if hp==1:
        assert save.read_bytes()==checkpoint,'Fatal void must preserve checkpoint'
        hold('a',.08);time.sleep(2);capture('void-reloaded');hold('x',.08)
        expected=1
    else:
        hold('x',.08);expected=6
    data=save.read_bytes();px,pz=struct.unpack_from('<ff',data,8)
    assert data[26]==0 and struct.unpack_from('<I',data,16)[0]==expected
    assert abs(px-.35)<.08 and abs(pz-6.95)<.08,(px,pz)
    hold('m',.08)
