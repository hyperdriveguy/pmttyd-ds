"""Replay explicit keyboard/capture steps in an isolated emulator_smoke profile.

Place scenario.json in --output-dir. Steps use key (optional), hold_seconds,
wait_seconds, repeat and capture. Captures require human scene review; elapsed
wall time is never reported as original simulation timing.
"""
import json
from pathlib import Path
import subprocess
import time

POINTER_FOCUS_X = '320'
POINTER_FOCUS_Y = '260'
DEFAULT_PRESS_SECONDS = 0.15
DEFAULT_SETTLE_SECONDS = 1


def run(env, capture):
    output = Path(env['PM_PORT_SMOKE_OUTPUT'])
    scenario = json.loads((output / 'scenario.json').read_text())

    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()

    window = x('search', '--onlyvisible', '--class', scenario['window_class']).splitlines()[-1]
    x('windowfocus', window)
    # Dolphin's configured XInput2 pointer device needs an entered render window.
    x('mousemove', '--window', window, POINTER_FOCUS_X, POINTER_FOCUS_Y)
    x('click', '1')
    started = time.monotonic()
    with (output / 'inputs.jsonl').open('w') as log:
        for index, step in enumerate(scenario['steps']):
            for repetition in range(step.get('repeat', 1)):
                key = step.get('key')
                record = dict(step=index, repetition=repetition, key=key,
                              down_wall_seconds=time.monotonic() - started)
                if key:
                    x('keydown', key)
                    try:
                        time.sleep(step.get('hold_seconds', DEFAULT_PRESS_SECONDS))
                    finally:
                        x('keyup', key)
                record['up_wall_seconds'] = time.monotonic() - started
                time.sleep(step.get('wait_seconds', DEFAULT_SETTLE_SECONDS))
                if step.get('capture'):
                    name = f"{index:02d}-{repetition:03d}-{step['capture']}"
                    capture(name)
                    record['capture'] = name + '.png'
                record['end_wall_seconds'] = time.monotonic() - started
                log.write(json.dumps(record) + '\n')
                log.flush()
