"""Allow the native event-bank setup to finish, then inspect original Jump recovery."""
import time
import runpy
from pathlib import Path

def run(env,capture):
    runpy.run_path(str(Path(__file__).with_name("azahar_trio_jump.py")))["run"](env,capture)
    time.sleep(30)
    capture("jump-reaction-and-return")
