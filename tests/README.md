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
