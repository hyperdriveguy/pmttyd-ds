# Source subsystem map

Portable mechanics and the native 3DS runtime share this directory. Implementation
files use subsystem prefixes; pair them with the corresponding public header in
`include/` and the contract indexed by [`docs/behavior/README.md`](../docs/behavior/README.md).

| Prefix or name family | Subsystem | Start here |
| --- | --- | --- |
| `battle_`, `trio_`, `harbor_tutorial*` | Battle mechanics, tutorial integration, presentation adapters | `docs/behavior/battle/`, `docs/behavior/story/` |
| `effect_`, `recovery_effect*` | Effect lifecycle, Maya timing and original embedded resource views (`effect/`) | `docs/behavior/effect/`, `docs/behavior/display/` |
| `event_` | Event VM, memory, callbacks, operands and opcode dispatch | `docs/behavior/event/` |
| `field_`, `npc_`, `goomba_`, `road_`, `floor_`, `wall_`, `water_` | Field state, movement, collision, actors and room transitions | `docs/behavior/field/`, `docs/behavior/overworld/`, `docs/behavior/partner/` |
| `opening_`, `demo_`, `prologue_`, `professor_` | Opening sequence and scripted story scenes | `docs/behavior/story/` |
| `message_`, `window_`, `item_get_text` | Message assets, playback and UI | `docs/behavior/message/`, `docs/behavior/display/` |
| `audio_`, `positional_sfx_` | Music, sample playback and sound ownership | `docs/behavior/audio/` |
| `render*`, `actor_*`, `map_*`, `camera_*`, `visibility*`, `uv_*` | Rendering, animation, map presentation and cameras | `docs/behavior/display/`, `docs/behavior/camera/` |
| `save_`, `pouch_`, `key_item_`, `item_*`, `trouble_*` | Persistence and inventory/item services | `docs/behavior/format/`, `docs/behavior/unit/`, `docs/behavior/field/` |
| `game*`, `main*`, `platform*`, `physical_input*`, `frontend*` | Runtime composition and 3DS platform services | [`docs/ARCHITECTURE.md`](../docs/ARCHITECTURE.md) |

Find implementations and their tests by the same prefix, for example:

```sh
rg --files source include tests | rg '/(battle|event|field)_'
rg -n 'function_or_type_name' source include tests
```

Implementation files now live under the subsystem directories in `source/`;
headers mirror those groups under `include/`. Module basenames and include names
stay stable. New modules should use the owning directory and prefix, and
document recovered behavior in its subsystem folder; do not add contracts at
the `docs/behavior/` root.

For inline documentation conventions and implementation walkthroughs, see the
[port code guide](../docs/code/README.md).
