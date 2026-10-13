"""Source NPC facing/talk pose/height anchor with an isolated original story8 talk."""
import runpy
from pathlib import Path
def run(env,capture):
    scripts=Path(__file__).parent
    runpy.run_path(str(scripts/'azahar_save_block_visual.py'))['run'](env,capture)
    capture('npc-source-idle')
    runpy.run_path(str(scripts/'azahar_field_actor_talk.py'))['run'](env,capture)
