# Port code guide

This directory explains implementation choices and how to read the port's code.
Recovered original behavior and its evidence remain in
[behavior contracts](../behavior/README.md); conversion/provenance remain in
[assets](../ASSETS.md). Start with the [source map](../../source/README.md),
[header map](../../include/README.md) and [architecture](../ARCHITECTURE.md).

| Guide | Implementation covered |
| --- | --- |
| [Audio services](audio-services.md) | Sound slots/handles, pending starts, fades, fanfares and opening adapters |
| [Wander/animation services](wander-animation-services.md) | Event continuations, cold pools/tangents and recursive scratch recovery |
| [Runtime ownership](runtime.md) | Native main loop, physical input, audio and frontend |
| [Game composition](game-composition.md) | Room collision, units, tick order, restore and follower ownership |
| [Checkpoint bytes](save-format.md) | Native save encoder, decoder and file replacement |
| [Battle entry/startup](battle-entry.md) | Packed actors, construction, pouch/resources, poses, events and sequence visits |
| [Battle core](battle-core.md) | Action commands, private input, status, resources and damage accounting |
| [Pouch/selection](pouch-selection.md) | Packed key operations, revision, source row/window sequencing and borrowed text |
| [Field session](field-session.md) | Setup/deletion preflight, actor cursors and shared pose aliases |
| [Trouble Center](trouble-center.md) | Saved-state knowledge, row order, original board VM and split UI visits |
| [Field items/encounters](field-items.md) | Item/coin pickup, borrowed windows, audio envelopes, quest delivery and packed battle information |
| [Map NPC/area lifetime](map-lifecycle.md) | Parsed tags, history/skip masks, stable registry aliases and ordered room retirement |
| [NPC/partner/object owners](npc-objects.md) | Packed registration/setup, CPU poses, talk/paper state, save blocks and map matrices |
| [Field entry/event leaves](field-entry.md) | Bero metadata, position/control callbacks, cases, map flags and door-record ownership |
| [Field motion/contact](field-motion.md) | Units, jump/idle timing, wall/NPC query order, camera clocks and entry bridges |
| [Draw pipeline](draw-pipeline.md) | MODEL/TPL records, suspended traversal, material/shadow ordering and batch construction |
| [Retained display state](display-state.md) | Logical publication, GX numeric domains, icon/letterbox continuation and replay |
| [Battle/item/window adapters](render-adapters.md) | Provider holds, icon residency, GPU declarations, UI transforms and second views |
| [Opening/frontend scenes](scene-rendering.md) | Capture completion, paper UVs, scene residency and ship handoff |
| [Native renderer](renderer.md) | GPU buffers, pack flags, residency, field masks, text reflow and frame boundaries |
| [Display services](display-services.md) | Camera/queue/shadow continuations and authored road numerics |
| [Animation data](animation-data.md) | Pose/cache/surface formats and map upload helpers |
| [Event VM](event-vm.md) | Scheduler, operand tags, opcodes, address mappings and allocator services |

## Comment conventions

Explain units, ownership, valid domains and reasons for unusual ordering at the
code that needs them. Give repeated policy values descriptive constants/enums
when that improves the API; a nearby comment/table is sufficient for binary
layout offsets, original opcode numbers and recovered masks. Preserve source
numeric ordering and bit patterns when documenting recovered routines.

Distinguish original constants from native presentation/scheduling policy and
provisional adapters. An original address is an emulated 32-bit token, not a
host pointer. Keep unidentified fields as `field<offset>` and explain the known
read/write behavior; do not invent a semantic name. Explain why a hold resumes
at a particular operation, what state survives, and which provider is missing.
Avoid comments that merely repeat the C++ statement.

## Documentation audit status

The current pass covers runtime main/input/save, the event VM implementation,
core battle action/status/resource/damage helpers, and Game/portable runtime
helper composition, portable display/camera/animation helpers, and main renderer/
platform resource ownership and current scene-specific render adapters.
Retained camera/display/icon/letterbox helpers are also covered. Audio/frontend
service notes are included in the runtime guide. Animation/material/shadow and
display batch and ordinary field motion/contact helpers now have inline notes.
Bero entry metadata, case/map overlays and position/preparation leaves are covered.
Packed NPC entry/setup, catalog/talk/paper and typed partner/object helpers are covered.
Map-NPC parsing/history/bootstrap and retained registry/area lifetime are covered.
Finite item/coin/Goomba delivery and field battle-information helpers are covered.
Trouble Center state/menu/window and board/counter event adapters are covered.
Committed setup/actor leaves, shared field pose integration and pouch/selection helpers are covered.
Packed battle actor entry/setup, resource/pouch leaves and startup controllers are covered.
Packed wander events, typed wandering, cold animation/scratch parents and audio helpers are covered.
Remaining field/NPC/partner modules, battle, message, inventory, story, effect
and port tooling still
require a systematic inline numeric/behavior audit. Concurrent additions to
already covered modules also need review;
existing comments and contracts in those areas do not imply complete coverage.
Progress and exact validation are recorded in [PROGRESS](../PROGRESS.md).
