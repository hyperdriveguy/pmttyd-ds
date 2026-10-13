"""Isolated native follow/air pose capture; timings are fixture controls only."""
import runpy
import subprocess
import time


def run(env, capture):
    runpy.run_path('tests/azahar_partner_animation.py')['run'](env, capture)
    subprocess.run(['xdotool', 'keydown', 'a'], env=env, check=True)
    try:
        time.sleep(.12)
        capture('partner-air-early')
        time.sleep(.18)
        capture('partner-air-later')
    finally:
        subprocess.run(['xdotool', 'keyup', 'a'], env=env, check=True)
    time.sleep(1.5)
    capture('partner-follow-landed')
