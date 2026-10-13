#!/usr/bin/env python3
"""Launch Azahar with an isolated profile and capture a virtual X display.
Writes only artifacts/azahar. Requires Xvfb, xdotool, ImageMagick, Azahar.
This boot capture is not a claim that gameplay acceptance checks passed.
"""
import argparse
import os
from pathlib import Path
import signal
import select
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'artifacts/azahar'

def check_native_exception(output):
    """A visible exception dialog leaves Azahar running; process liveness alone fails."""
    log=Path(output)/'data/azahar-emu/log/azahar_log.txt'
    if not log.exists():return
    with log.open('rb') as file:
        file.seek(max(0,log.stat().st_size-65536))
        tail=file.read().decode('utf-8',errors='replace')
    if 'core/arm/exception_handler.cpp:LogException:' in tail:
        raise RuntimeError('Native application exception; inspect isolated Azahar log and latest capture')

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--save-fixture',help='Copy a versioned native test save into the isolated profile before boot')
    parser.add_argument('--fresh-save',action='store_true',help='Reset only the generated project-local Azahar test save')
    parser.add_argument('--reference-dolphin',action='store_true',help='Capture the original ISO using an isolated Dolphin profile instead')
    parser.add_argument('--seconds',type=int,default=12)
    parser.add_argument('--input-script',help='Python file with run(env, capture) function; isolated emulator only')
    parser.add_argument('--output-dir',help='Separate capture/profile directory under project artifacts')
    parser.add_argument('--binary',help='Native 3DSX artifact to inspect; defaults to the project build')
    args=parser.parse_args()
    if args.reference_dolphin and args.binary:raise ValueError('--binary is for native Azahar captures only')
    global OUT
    if args.reference_dolphin:OUT=ROOT/'artifacts/dolphin'
    if args.output_dir:
        OUT=Path(args.output_dir).resolve()
        if (ROOT/'artifacts').resolve() not in OUT.parents:raise ValueError('Smoke output must be under project artifacts')
    OUT.mkdir(parents=True,exist_ok=True)
    env=dict(os.environ)
    env['PM_PORT_SMOKE_OUTPUT']=str(OUT)
    for key,name in [('XDG_CONFIG_HOME','config'),('XDG_DATA_HOME','data'),('XDG_CACHE_HOME','cache')]:
        directory=OUT/name;directory.mkdir(exist_ok=True);env[key]=str(directory)
    if args.fresh_save and not args.reference_dolphin:
        directory=OUT/'data/azahar-emu/sdmc/3ds/ttyd-2ds'
        for name in ['progress.bin','progress.bin.bak','progress.bin.tmp']:(directory/name).unlink(missing_ok=True)
    if args.save_fixture:
        if args.reference_dolphin:raise ValueError('Save fixtures are for the native Azahar build only')
        fixture=Path(args.save_fixture).read_bytes()
        if len(fixture) not in (32,40):raise ValueError('Expected a legacy32-byte or version3 40-byte project save fixture')
        directory=OUT/'data/azahar-emu/sdmc/3ds/ttyd-2ds';directory.mkdir(parents=True,exist_ok=True)
        (directory/'progress.bin').write_bytes(fixture)
    env['QT_QPA_PLATFORM']='xcb' ;env['LIBGL_ALWAYS_SOFTWARE']='1'
    readfd,writefd=os.pipe()
    with (OUT/'xvfb.log').open('w') as xlog,(OUT/'launch.log').open('w') as elog:
        server=subprocess.Popen(['Xvfb','-displayfd',str(writefd),'-screen','0','1000x800x24','-nolisten','tcp'],pass_fds=(writefd,),stdout=xlog,stderr=xlog)
        os.close(writefd)
        app=None
        try:
            if not select.select([readfd],[],[],8)[0]:
                os.close(readfd)
                raise RuntimeError('Xvfb did not start within 8 seconds')
            with os.fdopen(readfd) as f: display=f.readline().strip()
            if not display: raise RuntimeError('Xvfb failed; inspect artifacts/azahar/xvfb.log')
            env['DISPLAY']=':'+display
            command=['/usr/bin/azahar',str(Path(args.binary).resolve() if args.binary else ROOT/'ttyd-2ds.3dsx')]
            if args.reference_dolphin:
                profile=OUT/'user';config=profile/'Config';config.mkdir(parents=True,exist_ok=True)
                (config/'GCPadNew.ini').write_text('[GCPad1]\nDevice = XInput2/0/Virtual core pointer\nButtons/A = A\nButtons/B = S\nButtons/Start = Return\nMain Stick/Up = Up\nMain Stick/Down = Down\nMain Stick/Left = Left\nMain Stick/Right = Right\n')
                command=['/usr/bin/dolphin-emu','-u',str(profile),'-b','-e',os.environ.get('TTYD_ISO','/home/carson/games/roms/G8ME01.iso')]
            app=subprocess.Popen(command,env=env,stdout=elog,stderr=elog)
            def capture(name='boot'):
                subprocess.run(['import','-window','root',str(OUT/f'{name}.png')],env=env,check=True)
                if not args.reference_dolphin:check_native_exception(OUT)
            time.sleep(args.seconds)
            if app.poll() is not None: raise RuntimeError(f'Emulator exited with {app.returncode}; inspect launch.log')
            capture()
            if args.input_script:
                import runpy
                script=runpy.run_path(args.input_script);script['run'](env,capture)
            print(f'Captured {OUT}/boot.png')
        finally:
            if app is not None and app.poll() is None:
                app.send_signal(signal.SIGTERM)
                try: app.wait(timeout=5)
                except subprocess.TimeoutExpired: app.kill();app.wait()
            server.terminate()
            try:server.wait(timeout=5)
            except subprocess.TimeoutExpired:server.kill();server.wait()
if __name__=='__main__':main()
