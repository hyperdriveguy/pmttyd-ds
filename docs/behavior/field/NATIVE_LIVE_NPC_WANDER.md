# Native live wandering NPCs and PMNPC008

2026-10-13; bounded early Rogueport native binding of
[original wander/Goomba4 owners](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md) and
[portable wander core](NATIVE_NPC_WANDER_CORE.md). This activates original east
Goomba4 art and motion within explicit story0..55 coverage. It is not the
complete original NPC registry, event scheduler, quests, audio driver or a
Dolphin/New2DSXL fidelity/performance measurement.

## Extensible catalog and original actor admission

PMNPC008 retains the16-byte magic/model-count/actor-count header and100-byte
PMNPC005+ model records. Each actor retains the200-byte PMNPC006 prefix and
PMNPC007 variable rules(count uint32 plus40-byte comparison/story/fullID
records), then adds188 bytes:

| Added field order | Encoding |
| --- | --- |
| Original talk event present | uint32 boolean |
| Wander enabled | uint32 boolean |
| Original wander work flags | uint32 |
| Radius,speed,tribe run threshold | Three binary32 floats |
| Tribe step period | uint32,validated<=65535 |
| Idle,walk,run,left sound,right sound | Five32-byte names |

Numbers remain little endian and source engine units. Original talk pointer
presence is independent from bound native talk rules: Goomba4 has presence1
with guarded ordinary rules/prefix; earlier packs retained zero rules and an
empty prefix. [Known-state ordinary playback](NATIVE_GOOMBA4_ORDINARY_TALK.md)
is now bound. Unsupported quest playback returns no native message; source candidacy/auto-turn can still pause the driver. Versions1..7
remain readable; legacy source presence is inferred from a nonempty prefix,
without inventing legacy wander fields. See [catalog](NATIVE_AREA_ACTOR_CATALOG.md)
and [per-actor rules](NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md).

The loader stages atomically and rejects malformed boolean/string/numeric/
count/rule fields, enabled object wander, nonpositive/out-of-range movement
metadata, nonempty disabled wander data, absent source event with bound talk
rules and profiles outside the currently bound direct horizontal subset.
Enabled profiles require mainflag40000000 and reject flags100000,20000 and
08000000, whose paused-clock/gravity/Z-suppression owners are not bound here.
Future areas can supply more model/actor records and typed profiles without a
renderer actor-name branch; extending admission beyond this subset requires
binding those original owners, not dropping their flags.

The compiler reads original tagged gor_02 registry order and sorts actor
records by room and tagged position in that order. In east source order this
places Frankly records,Goomba4,Chu3,Darkly; mutually exclusive Frankly records
retain their stable relative order. Collision, candidacy and regular scans use
this vector order rather than unordered-map iteration or model index order.

Goomba4 uses source init(-370,0,130),direction270, overriding tagged(-375,0,150).
The constructor flags40080603 and wander initialization OR00400000 to40480603.
Radius70,speed30,workflags0,tribe width/height30/30,run threshold20,period12000
and S_1/W_1/R_1 plus original left/right movement sound names come from the
recovered owners. Root and previous root seed together on instantiation.
Story0..55 is native coverage, not an original spawn visibility condition.
Original quest/inventory branches precede ordinary gor_02_030_00: missing quest
state is not0. [Ordinary playback](NATIVE_GOOMBA4_ORDINARY_TALK.md) now requires explicit
known quest/flag state excluding both quest arms; full quest playback remains
unbound and legacy unknown state remains unavailable.

## Retained events, clocks, movement and source pose requests

Game stores one FieldNpcInstance per source actor key: collision roots/flags,
44-byte wander work, movement components/sound counters/yaw/heading, pose
request time/phase, paper presentation and paused state. Room/catalog generation
changes discard instances; ordinary candidate pause preserves them.

At the beginning of an admitted simulation tick, updateFieldNpcEvents runs the
regular core before current Mario/object/NPC movement. It consumes the previous
NPC seconds publication and previous scan's Mario touch actor, along with the
retained paper readiness. Initialization starts native movement dt0, vertical
component0 and contact0; it captures current simulation milliseconds as clock
anchor and starts the original idle pose. Native budget frame conversion uses
TickRate60, producing360/540/90; source uses live GP frame rate. A Game-owned
LCG seed starts1 and is shared among native wander instances. This is an explicit
deterministic native adapter; original global seed writers/interleaving and
random trajectories are not claimed.

After current Mario/object updates, updateFieldNpcCollision publishes NPCdt
from integer simulation milliseconds elapsed since that instance's clock,
using16ms when elapsed>500ms, then seconds=milliseconds/1000. The regular core
therefore sees the preceding publication, not the newly computed current dt.
Under source state10/active flags it applies components directly to XZ through
mainflag40000000 and clears them only on dt>0. Source authored Goomba4 has no
gravity flag20000 or vertical writer, so its recovered npcMoveG zero-displacement
branch does no Y update/floor probe; the native subset preserves that deduction.
It does not impose map-wall or floor snapping on this actor. Shared
[Mario side response](NATIVE_NPC_SIDE_COLLISION.md) remains active with original
flags/dimensions and can roll back actor movement before actual-speed selection.

Actual current-minus-previous horizontal distance/current dt selects S_1 below
0.10000000149011612, W_1 below tribe20, otherwise R_1 when mainflag400 set.
Only phase change restarts the pose clock. Previous root snapshots follow that
response/pose selection; side response affects the measured native speed.
Driver work.phase and automatic posePhase are separate, so a paused moving
phase can render S_1 without discarding its countdown. Driver early exits retain
components until the separate NPC movement owner consumes them.

Candidate scan after movement uses original source event presence for every
eligible actor, independent of native message support; per-actor clear-ray
admission is separate from strict-nearest winner. Original candidate masks,
saved yaw and regular pause/resume preserve work. Global skipped paths and
source skipped actor slots do not infer candidate-loss resume. Supported Chu/
Darkly playback still follows its exact rules. A native playback provider must
belong to the same source nearest winner; an unbound Goomba4 winner does not
fall back to farther supported actors. Goomba4 is not given fabricated
quest state/dialogue. [Candidate source details](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md)
and [ordinary native talk](NATIVE_NPC_TALK_SELECTION.md) retain the broader
arbitration and lifecycle limits.

## Simulation-owned live presentation and 3DS resources

updateFieldNpcPresentation prepares live wandering actors once per admitted
simulation tick using current root/yaw/flags and source matrix/camera services.
Main publishes sign before Inner retained-facing advance; source pose snap100
follows NPC8000. Fresh pose flags9/angles0 are retained. Renderer reads the
prepared matrix, live roots and selected pose through fieldNpcState; it does
not advance live wandering paper facing. Pose sample time uses simulation
milliseconds since the last phase request at source60 animation frames/sec.
Extra renderer calls therefore cannot change this actor's wander readiness or
trajectory. This is a deliberate native cadence adapter: original display queue/
multiple-pass scheduling and numeric backend still need runtime comparison.
Stationary actors retain their existing renderer/presentation path.

Original a/c_kuribo_n and a/c_kuribo_n- supply Z_1,S_1,T_1,W_1,R_1; descriptor0
fallback is original Z_1. Source hierarchy,sparse keys,RGBA,shape pass/culling and
sampler/wrap metadata use the existing PMANIM4 converter. Current ignored
manifest records14 texture cells,1024x256 runtime atlas,UV4,124px decoded
textures in128px gutter cells. Its uncompressed RGBA8 texture payload is
1MiB(1024*256*4), a size calculation rather than measured memory residency.
tex3ds packsRGBA8 with automatic compression.
Source NPC model scalar2 and engine-to-world*.01 remain distinct from tribe
width/height30. No replacement artwork or generated sounds are introduced.

Resident model/texture packs are shared and staged with destination geometry;
GPU resources synchronize through citro3d before disposal. Shared pose/time
sampling caches and the existing linear vertex buffer submit original surface
batches through the PICA shader. libctru/citro3d/tex3ds and host/ARM math provide
native services, not evidence of equivalent Gekko/GX results. CPU sparse replay,
area staging memory peak, draw overflow and hardware profiling retain the
[catalog renderer limits](NATIVE_AREA_ACTOR_CATALOG.md).

The portable driver computes original sound threshold/toggle/elapsed behavior
and queues original SFX_FLD_NPC_KURI_MOVE1L/R names,mask and root in
Game::fieldNpcSounds. The queue resets at the next regular update and remains
observable for checks. Native Audio resolves these two names to original
primary samples993/994 through config/audio_bindings.json and the ignored
audio-map.json catalog: clips6/7 play on libctru NDSP channels8/9 as monoPCM16
one-shots at source16000Hz. Left/right decoded source lengths are540/672
samples; source volume45 maps to45/127 times the existing native master0.65
gain. Original names/mask/root remain retained in the queue.

This primary-sample playback is an adapter, not complete MusyX macro execution.
Source sound/material flag producers, request mask effects, spatial routing/
attenuation, child voices and full audio timing remain unbound. NDSP clears
each selected channel before adding its wave; no claim of source voice-budget/
polyphony or matched original audio execution follows from this mapping.

## Validation boundary and next areas

Code/manifest inspection and static source contracts substantiate these bindings;
implementation host/build/emulator results belong to [progress](../../PROGRESS.md).
This documentation delivery does not itself run native tests/emulator or claim
original Dolphin observations, matching pixels, hardware speed or full remake
completion. Needed next: quest globals/item tables/selection/control tokens,
complete regular init/restart/delete/work lifetime, pause/system-clock/scheduler
integration, shared original seed consumers, complete sound macro/spatial playback and matched
numeric/render/runtime/performance observations. New actors with gravity/map
movement or different lifetime flags require their source providers before
admission; do not widen this authored zero-vertical profile by assumption.
