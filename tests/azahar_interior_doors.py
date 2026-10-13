"""Isolated inn door capture. Hold durations are fixture controls only."""
import subprocess
import time

def run(env, capture):
    def key(name, seconds):
        subprocess.run(['xdotool', 'keydown', name], env=env, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        try:time.sleep(seconds)
        finally:subprocess.run(['xdotool', 'keyup', name], env=env, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    window = subprocess.check_output(['xdotool', 'search', '--onlyvisible', '--class', 'azahar'], env=env, text=True, stderr=subprocess.DEVNULL).splitlines()[-1]
    subprocess.run(['xdotool', 'windowfocus', window], env=env, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    capture('inn-outside')
    key('Up', .8);key('a', .1);time.sleep(3)
    capture('inn-inside')
    key('Down', .8);key('a', .1);time.sleep(3)
    capture('inn-exited')
