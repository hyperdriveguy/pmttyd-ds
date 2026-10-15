# Professor and underground event bridge

[ProfessorStory](../../source/story/professor_story.cpp) and its
[public state](../../include/story/professor_story.hpp) bridge original first-talk,
fence, lecture, underground and scoped battle-intro scripts to native services.
The interpreter owns original command order, message IDs, branches, waits and
saved-switch writes. This file also contains bounded presentation adapters;
it is not a complete original field or battle manager.

The three callback catalogs are ordered extensions:

| Indices | Catalog and responsibility |
| --- | --- |
| 0..63 | [Professor converter](../../tools/professor_event_assets.py): cameras, items, movement, dialogue, capture, fence, choices and field battle boundaries |
| 64..70 | [Underground converter](../../tools/underground_event_assets.py): facing, explicit party placement, trio poses and music restoration |
| 71..94 | [Trio converter](../../tools/trio_battle_assets.py): scoped Frankly lecture initialization, work, movement, phase and camera/message projection |

These are metadata service indices, not original event opcodes. Source addresses
are guest identities. Battle callbacks take precedence over underground, then
ordinary professor callbacks. Unknown addresses fail; admitted but unhandled
presentation services return immediate acceptance while preserving the raw
first-admission trace. Acceptance does not prove hardware/audio/display work.
Explicit field battle and previous-party placement barriers remain distinct.

Current professor metadata is PMPROG4 with 64 entries; earlier behavior notes
mention older pack versions. Loading checks the first-talk/fence/lecture entry
words; it retains the pipe entry without that same word probe. Fence resources
must include nonlooping A_1 frames0..140, sorted collision nodes and message
icons before publishing the new bank. Underground text searches tik_01 before
tik_04, sharing the professor font without its global message bank. The battle-unit loader reads
individual packed fields, avoiding C++ structure padding; unit order differs
from scene slots and is explicitly remapped on battle-intro entry.

Entry transitions matter:

| Entry | Initial state and ownership |
| --- | --- |
| First talk | story8; Frankly at (-135,0,-240); source placement can run before the door-end barrier |
| Fence | story9; replaces a finished owner only with temporary Frankly present; preserves his full actor snapshot |
| Lecture reload | story10; fence hidden/collision off; Frankly at (-220,0,305); passive source crossing poll |
| Underground | supplied story with upper bound11; departure flag controls Frankly presence; trio initially hidden at Y=-1000 |
| Battle intro | story11; init, entry and phase roots execute separately, advancing only after all allocations retire |
| Pipe departure | one immediate manager visit; succeeds only if the source event finished and set the actual departed flag |
| Story11 restore | presentation-only; no VM or replayed dialogue; departed Frankly is inactive/hidden |

Each interpreter has its own authored guest GP and 1 MiB heap, original SDK
allocator globals, GP+4=60 and the original 243 MHz clock conversion from ms.
GSW0 and departed GSF1207 are seeded, then read back from actual VM state.
The bridge never infers story progression from animation or message count.

Actor `npcActive`, hiding, flags, explicit placement and continuous movement
are separate. Source Y=-1000 and NPC bit0x80 hide; neither alone means deletion.
`placementGeneration` changes for explicit placements. `flagMask` tells consumers
which source bits this bridge owns, so merging does not erase unrelated flags.
Field and battle rotations/scales likewise have distinct consumers.

Native walking uses elapsed ms and a linear speed adapter; near-depth headings
retain prior yaw. Owner event deletion cancels motion before the next update.
The no-hit hop uses a quadratic with coefficient -4 times apex and no collision
query. Battle lecture movement instead adds one X/Z step per callback visit;
the final visit snaps XYZ and returns advance-and-yield (1). Replacing it with
elapsed-time interpolation or immediate return2 changes scheduling.

Print captures speaker root once. Party printing resolves the original `_kur`
suffix; `party`/`extparty` select Goombella/Frankly, and battle intro uses Frankly.
An original once_stop releases the callback while keeping the window available
for append. Selection owns a different window and writes its signed result to
LW0. Battle placement controls preserve high bits when set, then reset after
print. Phase comparison deliberately uses a raw phase operand, bypassing variable
decoding. Camera moveto converts source frames to integer ms at 60 Hz.

Fence capture waits for actual renderer-published integer bounds; source aspect
arithmetic remains in the event. The animation wait must yield on admission,
then requires the A_1 140-frame span. Display hiding, capture release and the
literal A_saku_04 collision-off request remain independent. Crossing retains
ordered endpoint rejection and fused cross products, with inclusive endpoint/
collinear cases; a generic epsilon intersection changes original branching.

Tick order is movement, message, selection, VM, saved-state readback. Passive
underground or fence/story10+ events may allow field control through source keys;
`running()` alone does not imply locked input. After VM finish, existing text
can drain. Battle intro starts its next root after allocation count reaches zero,
so children retire before the next phase; it does not run that root immediately.

See [first talk](../behavior/story/NATIVE_PROFESSOR_FIRST_TALK.md),
[fence/lecture](../behavior/story/NATIVE_PROFESSOR_FENCE_AND_LECTURE_ENTRY.md),
[choices/reload](../behavior/story/NATIVE_LECTURE_CHOICES_AND_RELOAD.md),
[underground](../behavior/story/NATIVE_UNDERGROUND_TRIO_SCENE.md) and
[temporary-party admission](../behavior/partner/FIELD_PROFESSOR_TEMPORARY_PARTY_AND_FENCE_ADMISSION.md)
for evidence and integration limits. The implementation notes describe current
service boundaries without claiming complete original stepping, GPU capture,
audio or battle parity.
