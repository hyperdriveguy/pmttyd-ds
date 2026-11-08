"""Enter a checkpoint through the normal frontend, then check NPC talk/save.

Use the east-npc-story8 fixture and the default isolated 1000x800 layout.
Tesseract and Pillow gate input on visible UI; screenshots remain reviewable.
"""
import json
from pathlib import Path
import runpy
import subprocess
import time
from PIL import Image

BUTTON_HOLD_SECONDS = 0.15
POLL_SECONDS = 2
ENTRY_DEADLINE_SECONDS = 180
BOTTOM_SCREEN_BOUNDS = (330, 450, 670, 690)
OCR_SCALE = 3
# The installed Afrikaans model recognizes the Latin UI labels used here.
OCR_LANGUAGE = 'afr'


def enter_field(env, capture):
    output = Path(env['PM_PORT_SMOKE_OUTPUT'])

    def x(*args):
        return subprocess.check_output(['xdotool', *args], env=env, text=True,
                                       stderr=subprocess.DEVNULL).strip()

    def press():
        x('keydown', 'a')
        try:
            time.sleep(BUTTON_HOLD_SECONDS)
        finally:
            x('keyup', 'a')

    window = x('search', '--onlyvisible', '--class', 'azahar').splitlines()[-1]
    x('windowfocus', window)
    deadline = time.monotonic() + ENTRY_DEADLINE_SECONDS
    with (output / 'entry-observations.jsonl').open('w') as log:
        attempt = 0
        while time.monotonic() < deadline:
            name = f'entry-{attempt:03d}'
            capture(name)
            with Image.open(output / (name + '.png')) as image:
                bottom = image.crop(BOTTOM_SCREEN_BOUNDS)
                bottom.resize((bottom.width * OCR_SCALE,
                               bottom.height * OCR_SCALE)).save(output / 'entry-ocr.png')
            text = subprocess.check_output(['tesseract', str(output / 'entry-ocr.png'),
                                            'stdout', '-l', OCR_LANGUAGE, '--psm', '6'], text=True,
                                           stderr=subprocess.DEVNULL).lower()
            log.write(json.dumps(dict(capture=name, text=text,
                                      monotonic_seconds=time.monotonic())) + '\n')
            log.flush()
            if 'rogueport field notes' in text:
                capture('loaded-field')
                return
            press()
            time.sleep(POLL_SECONDS)
            attempt += 1
    raise AssertionError('Normal frontend did not expose the checkpoint field UI')


def run(env, capture):
    enter_field(env, capture)
    test = Path(__file__).with_name('azahar_field_actor_talk.py')
    runpy.run_path(str(test))['run'](env, capture)
