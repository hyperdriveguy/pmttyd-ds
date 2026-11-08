# Test map

Host C++ tests are registered from `CMakeLists.txt`; Python tests use unittest
(`make test`). Names generally share the module's subsystem prefix. Azahar
input scripts are `azahar_*.py`; reusable native service doubles are under
[`stubs/`](stubs/).

| Test family | Coverage |
| --- | --- |
| `battle_*`, `trio_*`, `harbor_tutorial_*` | Portable battle mechanics and native tutorial adapters |
| `event_*` | Event VM, dispatch, callbacks, memory and script assets |
| `field_*`, `npc_*`, `goomba_*`, `road_*`, `floor_*`, `wall_*` | Field movement, actors, collision, items and transitions |
| `opening_*`, `demo_*`, `professor_*` | Opening and scripted-scene mechanics |
| `message_*`, `window_*`, `item_get_*` | Message and UI behavior |
| `audio_*`, `positional_sfx_*` | Audio ownership and routing |
| `actor_*`, `map_*`, `camera_*`, `vertex_*` | Rendering, animation, map and camera behavior |
| `save_*`, `pouch_*`, `key_item_*`, `trouble_*` | Checkpoint, inventory and item services |
| `azahar_*` | Isolated emulator input/capture scripts; manual review may still be required |

Use `make test` for the full host suite. Use `tools/emulator_smoke.py` with an
isolated fixture/profile for native capture; never point automated runs at the
user's emulator profile or saves. The project handoff records the exact latest
validated command and known limits in [`docs/PROGRESS.md`](../docs/PROGRESS.md).

Normal production-menu spot checks use `azahar_playability_field.py` with the
east-NPC story8 fixture and `azahar_playability_encounter.py` with the harbor
story4 fixture. Field admission reads the visible UI with Pillow/Tesseract
(the installed `afr` Latin-script model); default isolated window geometry is
required. The encounter script captures physical input and requires manual
review: process exit0 is not a battle-playability pass.

`emulator_playability_spots.py` replays explicit steps from `scenario.json` in
the selected isolated output directory and writes `inputs.jsonl`; durations
are wall time, not original simulation measurements. See the
[2026-11-07 comparison](../docs/mechanics/PLAYABILITY_2026_11_07.md) for exact
scenarios, passing opening/talk checks, the tutorial blocker and validation
limits. The fresh opening test accepts the existing v10 checkpoint length.
