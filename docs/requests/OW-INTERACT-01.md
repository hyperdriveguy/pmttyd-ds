# OW-INTERACT-01 — Mario A/X/B interaction arbitration and playback

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [ ] Bind Darkly's exact later-story talk graph, including leading story175
  flag0xf84060bd/name child and duplicate<215 case. Early0..55 has a verified
  one-print source arm; Chu's generic thresholds cannot substitute globally.

- [ ] Bind original save-block hit flag8 producer, update states/animation and
  common save-window event0x803bac3c to SAVE-01 persistence; original plaza
  spawn/model/system-event owners are recovered, proximity+A is not.

- [ ] Bind ordinary NPC pre-display/pose/facing and `mtx_setup` owners through
  `animPoseDrawMtx`; source dir270 requests facing90, which is an animation
  parameter rather than world yaw. Recover tribe scale/height/talk-anchor
  semantics and tavern first-floor RGB fade before adding interior actors.
- [ ] Bind recovered [original wander driver](../behavior/field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md)
  through regular-event lifetime, paused clock/support, shared RNG and quest
  item/window services. Direct init/main phases, sound tail, movement components
  and Goomba4 quest-before-story arms are recovered; tagged aitypeなし does
  not suppress a separately registered regular event.

- [ ] Mario A/X/B interaction arbitration and priority against jump/movement,
  actor/door/object proximity, hammer/first-strike, selected message/speaker/
  control-token playback and persistent scene conditions. Native development
  notices/menu and Crump proximity are harness behavior.

## Request text (verbatim)

- OW-INTERACT-01: Mario A/X/B interaction arbitration and priority against
  jump/movement, actor/door/object proximity, hammer/first-strike, selected
  message/speaker/control-token playback and persistent scene conditions.
  Native development notices/menu and Crump proximity are harness behavior.

## Answers and delivery notes (append-only history)

No answer has arrived for this request yet.



### 2026-10-12 — First harbor encounter predicates and payload census

Partial answer from the implementation session's explicitly delegated
documentation worker. Independent read-only US ISO recovery supplements
[harbor scene bindings](../behavior/story/STORY_HARBOR_OPENING_BINDINGS.md)
with the exact position polls: story3's `christine_osoware` admits integer
decoded Z<=-100; story4's beforebattle admits integer decoded Z<=-100 AND
X>=-25. Both poll with WAIT_FRAME(1), then disable Mario key before dialogue.
The predicates resolve 1/1024-quantized float locals through integer
`evtGetValue` truncation; comparing raw floats changes boundary behavior.
Neither poll requests A/X/B or actor proximity. Resume positions and actor
placement requests are recorded separately from fresh entry.

The story operand is now composed with the recovered namespace service:
`0xf5de0180` = GSW(0); first admission writes GSF(1179)=1. All 20 original
message calls `mac_0_014`..033 and attribution remain verified. No `<select>`
token exists in those payloads and no `evt_msg_select`/`evt_msg_continue`
call exists in the five scene streams. Story3's 016..021 and story4's 022..024
execute unconditionally after their polls. Original wait/page/key/dynamic/
scale/shake tokens must remain in local message assets and playback.

Validation: `rel_event_inventory.py` re-extracted five scene spans; direct
ISO checks pinned 13 command records, two get_pos imports and raw operands,
20 ordered IDs, two resource hashes and no-select census;
`python3 tools/verify_behavior_contracts.py` passes. No native code changed.
OW-INTERACT-01 stays OPEN: full interaction arbitration, control/key helpers,
general proximity/hammer behavior, postbattle wait predicates, actual playback
timing and original execution remain unclosed. No delivery-log owner/lock is
invented: its protocol requires a user-assigned numeric agent ID, which this
delegated worker has not been assigned.

Native implementation breadcrumb (2026-10-12): complete US message library
and `MessageAssets` resource lookup shipped in `0a94b04`. The original
`evt_mario_get_pos` callback and the story3/story4 integer position predicates
are now portable implementations in `field_story_position.cpp`, using existing
event operand services and the callback registry. They retain encoded float
quantization and original pointer/operand capture order, independently checked
against a fresh US DOL disassembly. This does not activate story scenes in
`Game`; parser/window/camera/NPC services and REL event execution remain open.

### 2026-10-12 — Story3 complete post-trigger request chronology

Explicitly delegated documentation worker delivered the full static
`christine_osoware` post-trigger chronology in
[harbor opening bindings](../behavior/story/STORY_HARBOR_OPENING_BINDINGS.md#story3-post-trigger-choreography-2026-10-12-partial-delivery).
The six original016..021 print requests retain their keys and speakers. Seven
independent inline blocks retain per-block local position reads, integer X
arithmetic, delays and non-joining ownership. The final Goombella movement is
in the owner and completes before021. Camera arguments/waits, nested key
ownership, story4-before-key-restoration and beforebattle RUN ordering are
recorded with every source site. No explicit NPC/Mario pose calls or
post-trigger Mario position/movement requests exist in this stream.

Validation: fresh read-only ISO extraction, independent source-byte comparison
of all93 command headers/operands/imports, seven inline start/end pairs, and
fresh DOL movement/reardir helper inspection. Source artifacts remain `/tmp`
and outside history. Docs-only; no native build/test claim. General A/X/B
arbitration, message timing and original runtime fidelity remain OPEN. No
numeric documentation-agent ownership or delivery-log entry was invented.

Native story3 playback increment (2026-10-12): original mac_0_016..021 now
run from the source position gate when native originalStory=3 in room1,
retaining exact raw payloads and the six call-site speaker keys. The original
US font/widths, opening control-token schedules, key/page waits and source
BGM_EVT_DANGER4 stream are bound. Grounded-only admission was explicitly
removed: the original poll has no air/button condition, and native neutral-input
physics keeps an existing jump progressing while the scene locks new actions.

Remaining requests: complete NPC+432 movement numeric/collision/pose driver
for the supplied six-argument moves (current source-value-per-second adapter is
provisional); Mario set_dir/reardir services; nested byte39 key owners and full
GSF(1179) persistence; harbor NPC sound/flag producers, crowd400 and field
environment; original bubble/history/icons/selection/paper/noise/shake/text SFX
consumers. Story2 arrival/resume and story4 tutorial continuation are still
unbound. This source-derived scene sequencer is not complete REL VM integration
or full prologue/story fidelity, and does not close this request.

### 2026-10-13 — Ordinary Rogueport source NPC catalog

Explicitly delegated documentation worker delivered
[ordinary NPC bindings](../behavior/field/ROGUEPORT_ORDINARY_NPC_BINDINGS.md):
four exact tagged-map placements/directions/tribes, REL init/regular/talk owners,
DOL model/pose pointers, nine ordinary story-first-match dialogue keys per
actor, tavern fade ownership and later scene exceptions. East Chu3 is the
strongest initial stationary outdoor slice. Goomba2 requires the original
wander helpers; Chu2/sailor require tavern visibility/fade. `npcDisp` negates
and normalizes ry into animation draw facing, independently of `mtx_setup`;
raw facing cannot be treated as native world yaw.

Validation: fresh read-only ISO room setup/init/talk/exception event extraction,
tagged-source hashes, direct actor-reference census across annotated gor_01/02
events, and fresh DOL setup/tribe/entry/display/reviseAngle inspection. Local
source artifacts remain /tmp. Documentation links and diff whitespace pass;
docs-only, no native builds or original-runtime claim. Parent request remains
OPEN. No numeric documentation-owner ID/lock is invented.

### 2026-10-13 — Original plaza save-block follow-on

[Save-block source binding](../behavior/field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md)
recovers exact original spawn(185,60,-55), story341 Koopa suppression,
MOBJ_SaveBlock/S_1 assets, async constructor/update/system-event owners and
original first-hit save_first_hit key. Native contact/hit and common save/card
services remain open. Fresh read-only REL/DOL extraction and direct source
reads validate the scoped requests; inspection outputs stay /tmp.


### 2026-10-13 — Native area catalog and save-block conversion dependency

The native implementation compiles stationary east Chu3 original position,
tribe/model and CHU_S_1 animation into a model-namespaced area catalog, with
story0..55 as an explicit native coverage envelope. Frankly story8/10 init
placements share that catalog. Chu3 talk selects original CASE_LESS message
IDs, preserves raw US payloads/tokens and actor attribution, locks actions/saves
through closure and leaves story unchanged. Native proximity0.65/height0.25
and grounded A-before-door/jump admission are provisional pending original
search/interaction priority. NPC collision/facing/wandering remain separate
owners; no replacement dialogue is introduced.
See [native scope](../behavior/field/NATIVE_AREA_ACTOR_CATALOG.md).

Save-block conversion proved original S_1 UVs extend from-2 to2.334 and its
texture keys scroll binding1; the ordinary-animation converter rejects this
domain. Original TPL wrapS/T are clamp0/0. Do not discard texture updates or
clamp only vertex coordinates (that changes interpolation). Need the complete
original animated texture-matrix mapping and clamp/lighting/material pass
contract before packaging the object. Constructor/display scalar10 and the
hit-driven save request graph are delivered separately in
[save-block source binding](../behavior/field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md).
This parent interaction/runtime request remains open.

### 2026-10-13 — Save-block animated UV and material dependency recovery

[Save-block UV/material binding](../behavior/display/SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md)
corrects a crucial source distinction: although TPL defaults are clamp0/0,
BOTH original model binding records override wrapping with3, selecting
REPEAT1/1. S_1 loops start1/end119 (span118); binding1 retains118 cumulative
float tx/ty deltas with NO fractional texture interpolation. Its source UV
mapping is(u+tx,v−ty), followed by sampler wrapping. Native atlas conversion
must preserve within-triangle repeats rather than clamp/fract only vertices.

The same witness recovers visible untextured frame/star/layer draws,196 original
RGBA entries, shape pass0/2/3 and cull-none requests, default material flags0
and lighting flag2. Texture*RGBA is the recovered base-input adapter; original
ambient/near-object lighting/fog, writer domains and exact GX sampling remain
open. Fresh read-only ISO headers, clip records, rounded host replay and DOL
entry/display inspections validate this dependency; three existing complete
animation/material verifiers pass. No native or original-runtime claim; parent
interaction/save/render goal remains open.

### 2026-10-13 — Original save-block head-contact and hit lifecycle

[Save-block head/hit contract](../behavior/field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md)
recovers the original dynamic12-triangle box, bbox-derived XYZ dimensions,
source strip winding and HOBJ_mobj_save identity. Original new-head-name
comparison publishes objectflag8 through flag0x400000; same held contact does
not retrigger. System-level/motion31/32/disabled-object gates precede callback.
A_1 is nonloop0..15; completion reads previous pose+0x84 ratio>=1 because
callback precedes animation Main. Complete0..4 optional/system-event stages,
priority30/type26 object userdata and nested Mario key counter are recorded.
Original common card/event flow remains open. Fresh read-only DOL/model/table
inspections validate this scoped delivery; no native/runtime claim.

### 2026-10-13 — Common save child/choices and source dispatch order

[Common save event/choices](../behavior/field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md)
recovers all197 evt_memcard_save commands, normal/first-save/ending mode gates,
original28 US keyed payload identities, cancellation, status-specific format/
create/IPL branches, retry and return. Corrected operand ownership is GSF233/
236,GSW0/4,LF10/11,LW0/10; LF10 is an event-local flag rather than map storage.
Fresh source Main calls event-manager before Mario, then map objects; head
publication precedes object activation. Source collision template cullbyte1=1
(one-sided) is added to the prior head contract. Original card backend, source
flag persistence and native choice/error adapter remain open. Read-only stream/
message/DOL recovery, with no original execution or native completion claim.

### 2026-10-13 — Shared static/MOBJ hit registry and ray-order dependency

[Dynamic object registry/ray order](../behavior/field/DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md)
recovers one shared active HitObj chain, map-record/slot order, record0 dynamic
allocation after initial static slots, strict-nearer upward winner/tie retention,
and source borrowed name/delete publication lifetime. The save cube has no
per-face gameplay flags: shared cull1/attribute80000000 admits bottom head,
top ground and side wall geometry through their query owners. Implementing only
head remains scoped. Fresh read-only DOL/source-table inspection; original
runtime ordering and complete native ground/wall/registry lifetime remain open.

### 2026-10-13 — Ordinary Goombella save-box floor/wall binding

[Partner save-block collision](../behavior/partner/FIELD_PARTNER_SAVE_BLOCK_COLLISION_BINDING.md)
confirms generic partner high-bit handling accepts original MOBJ_SaveBlock,
rejecting only the three exact hidden-block model names after ordered masks.
Floor/front use partner per-ray callback selection; surrounding body correction
uses global-Mario callbacks and retains save sides as blockers. Shared downward
backend excludes far endpoints and retains earlier static/object ties, while
five-floor-sample and two-wall-height reductions have their own strict ties.
No authored partner exemption or new radius is established. Read-only source
inspection/linked verifier checks; native full dynamic partner collision and
original trajectories remain open.

2026-10-13 Professor/Goombella used message pose delivery:
[Field speaker talk/stay ownership](../behavior/message/FIELD_SPEAKER_TALK_STAY_POSE_OWNERSHIP.md)
traces Generalpre-dispatch15-call countdown,latch andresult0/1 stayrestoration
to NPCforce1 talk/stay andpartyselector7/1 sourcehelpers. _evt_msg_print creates
andbindswindow butdoesnotactivatetalk directly; retained/closingwindows can
alreadyrequeststay. ExplicitNPC andparty speakers retain distinct lifecycle
owners. Native managercadence/actualrenderedcomparison remain open.

### 2026-10-13 — Save animation restart and event-array callback identities

[Restart/event identities](../behavior/display/SAVE_BLOCK_RESTART_CLOCK_AND_EVENT_IDENTITIES.md)
recovers force1 setter reset versus subsequent Main publication for A_1/S_1,
source elapsed/ratio/start semantics and previous-publication completion query.
Pins exact three event spans/hashes and14 unique USER_FUNC callbacks across
89 encoded sites; no event/code bodies are exported. Original clock traces and
complete native/card/event binding remain open. Read-only source reads and
animation selector/Main verifiers pass; no original execution claim.

### 2026-10-13 — Save-message Continue/Add retained-window waits

[Save retained-window contract](../behavior/message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md)
confirms evt_msg_continue resumesstate1 and polls savedwindowflags2 without
force-close/replay/key-release; print_add uses flags1 raw controls versus flags0
US lookup, appends over the prior terminal and polls the same retained owner.
Raw scrl_auto/once_stop yields parser scroll/retained-stop records; clearing
poll2 atstate2 does not delete the window. Selection uses separateE+17c while
printE+178 persists through children. Fresh read-only five-body inspection;
original/native window capture and card services remain open.

### 2026-10-13 — Save camera8 letterbox lock and original cue431

[Camera8 lock](../behavior/camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md)
recovers bit0x200 as camLetterBox's automatic enable/disable decision bypass,
preserving bit0x100 while bar growth/fade/draw continue. Camera8 is a distinct
2d owner; native field camera4 flags must not become a guessed follow freeze.
[Original cue431](../behavior/audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md)
pins SFX_MOBJ_BLOCK_SAVE1 row/FX618/group5/macro745/sample550 and distinguishes
ignored-library availability from selected native residency (currently absent).
Read-only DOL/bank/catalog inspection; no native camera/audio implementation,
original runtime group residence or pixel/audio equivalence claim.

### 2026-10-13 — Portable retained save presentation implemented

[Native provider](../behavior/field/NATIVE_SAVE_BLOCK_RETAINED_PRESENTATION.md)
now executes original print/Add/Continue/selection payloads through retained
windows, separate poll/data flags and delayed ticketed SD write acknowledgement.
Original No/Yes/B/error paths and source nested keys pass native host tests.
Save v6 preserves GSF233/236/GSW4 alongside GSF1207, with legacy migration.
This does not close live object/event/camera/audio/I/O binding, original card
error classification, native capture parity or full field ordering.

### 2026-10-13 — Live source save block, native System rendering and SD worker

[Live binding](../behavior/field/NATIVE_LIVE_SAVE_BLOCK.md) connects actual source
head collision/name publication, independent S_1/A_1 simulation clocks, original
stages/event graph, nested keys and original print/selection windows to Game.
Original System pattern/masks/TEV algebra and selection panel/cursor draw on PICA;
accepted writes use a libctru worker and source switch snapshot. Original431 now
plays its source primary sample; macro/3D/group behavior remains an adapter.
Host and isolated native/first-hit/No/Yes captures pass within this bounded domain.
The broad todo stays OPEN for original manager sequence/pause/shared VM/card state,
SAVE-01 full pouch persistence, camera8 UI, spatial/macro audio and runtime parity.

### 2026-10-13 — NPC dimensions, talk-anchor and display-registration owners

Explicitly delegated read-only documentation delivered
[NPC dimensions/anchor/registration](../behavior/display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md).
Source tribe dimensions are independent of model scale; ordinary NPC print
anchors copy rootXYZ and add height once, gated by EventEntry+180 mask1effffff.
Recovered npcMain's bounded cull/color/alpha/main/blur/offscreen registration
block and ordinary stay/talk pose leaf requests. These local owners answer
the width/height/root-anchor subset; full NPC flags/pre-display lifecycle,
material/shadow/backend, event overrides and interaction timing remain open.
Fresh read-only DOL extraction and constant reads validate the source requests.
No native edits/build/runtime claim or invented numeric lock ownership.

### 2026-10-13 — Native ordinary NPC local presentation bound

[Native presentation](../behavior/field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md)
uses original tribe dimensions/talk poses and angle table, source ordered
matrix/Main/Inner facing and root-plus-height anchors. Original shape culling/
RGBA/pass data now survive in PMANIM4; ordinary talk requests carry simulation
clock/restart generations. Local native tests and isolated visible original
Mouser talk/return pass. Full pre-display/color/blur/shadows, source NPC collision,
interaction priority, wander/tavern fade, numeric/runtime parity and other areas
remain OPEN; this does not close the parent request.

### 2026-10-13 — Mario/NPC collision and ordinary A-talk local owners

Delegated read-only documentation delivered [NPC sphere and Mario side collision](../behavior/field/FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md)
and [ordinary talk candidate/lifecycle](../behavior/field/FIELD_ORDINARY_NPC_TALK_CANDIDATE.md).
Sphere query measures distance to NPC vertical segment and tests halfwidth+
extra radius; NPC-main side response uses inclusive vertical overlap, half
combined width+5, previous-NPC-XZ rollback, Mario response and map-wall retry.
The partner last-horizontal-match query is not interchangeable.

Ordinary talk needs current Mario motion0..2, zero key depth, live flags2/
not8/talkEvent, inclusive vertical overlap, source radius half combined width
plus10, camera-relative center/width-edge directional tests and unobstructed
map ray. Strict nearest wins first ties; A commits fieldstate2. Source
MarioInit width20/height37 and NPCEntry flags3 publication are pinned. Camera
viewYaw and ray(sin,-cos) preserve the original compass. Field root proves
MarioMain→mobjMain→npcMain→caseMain order. fbatTalkMode's complete local
start/wait/return requests retain actor event context6 and source eventID wait.

Validation: fresh read-only ISO disassembly of nine used query/caller bodies,
MarioInit/cam3dMain and field-root call instructions, plus direct dimension/
constant reads. Documentation links and whitespace checks pass. Source
inspection outputs remain /tmp; no native edit/commit/test/build or runtime
claim. Full collision polynomial numerics, event/window/regular-stop lifecycle,
all fbatHitCheck branches, map-object interaction and pause/retrace remain open.

### 2026-10-13 — Side polynomial/caller gates and null talk-ray callback

Delivered [NPC Mario-side response numerics](../behavior/field/FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md):
exact coefficient bits, quadrant reductions, fused polynomial/delta/cap graph,
wall retry and complete direct npcMain gate chain to side call. Activeflags1+2
and source-state+1d4 mask10 precede terminal bypass flags8/20/4000; stationary
zero-speed path reaches collision without npcMoveG. Source SDK/angle/frsqrte
backend/FPSCR remain numeric dependencies.

[Talk-ray callback/display getter](../behavior/field/FIELD_NPC_TALK_RAY_AND_DISPLAY_DIRECTION.md)
corrects earlier scope0 wording: r3 is null callback, so ordinary Mario wall
attribute predicate must not be added. Four backend callback gates skip
invocation on null; generic geometry admission remains separate. Source
marioGetDispDir is wrap((current displayTurn+1ac-90)+camera snapshot+19c),
then talk scan separately subtracts live camera+114. Render yaw/mirror and
current movement heading are not equivalent substitutes.

Validation: fresh read-only DOL inspections and direct coefficient/constant
reads; links/whitespace pass. No native edits/commits/build/runtime claim.
Full source geometry/backend, NPC-state timer producers, collision trajectories
and complete event/window/global cadence remain open.

### 2026-10-13 — Strict-nearest query order and deferred talk construction

Source recheck confirms a radially eligible actor farther than the current
chosen actor still executes facing and occlusion queries; the early distance
comparison skips only nearActor assignment (0x80047908), while chosen-distance
rejection occurs after no-hit ray (0x80047bd8..be0). [Talk contract](../behavior/field/FIELD_ORDINARY_NPC_TALK_CANDIDATE.md)
now states this explicitly. Dispatcher reads fieldstate once, so A's1→2
transition cannot fall through to fbatTalkMode in that invocation: ordinary
talk stage0 starts on the next admitted npcMain invocation. Fresh source
instruction recheck; docs-only, no native changes or runtime claim.

### 2026-10-13 — Native ordinary candidate and retained owner binding

[Native talk selection](../behavior/field/NATIVE_NPC_TALK_SELECTION.md) replaces
provisional0.65-radius/height proximity with source dimensions, live/rounded
camera/display inputs, source halfplanes/width edges, null-callback obstruction
and strict nearest selection. Previous NPC candidate blocks following Mario jump
without a fresh facing/radius scan; A selection starts the simple source talk
on the following NPC invocation, and wait-return clears without rescanning.
Available Mario display/main flags and native key-off feed admission; current
static source Entry flags3 map known catalog lifetime. Prompts reuse retained
simulation state. Original messages/story branches remain unchanged.

Host tests now use full source camera/head geometry and warmup, distinguish
filtered wall versus null-callback talk obstruction, and cover retained/pending/
return cases. Emulator assertions require a visible original message window;
return/save alone is insufficient proof. Full NPC registry/auto-turn/regular-
event locks, transformed/key-depth/control modes, encounters, other events/areas,
source side response and original backend/runtime/hardware parity remain OPEN.

### 2026-10-13 — Side collision wrap/history/wall ABI and authored actor masks

[Side numeric contract](../behavior/field/FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md)
now pins complete compAngle one-adjustment wrap/ties, explicit placement's
previous-first/current-second publication, npcMain's post-response snapshot,
and original around-wall retry(mode0,ignoredSpeed0,theta+160,width20,height).
Source r4 is integer mode, correcting the old structure-sketch output-pointer
label. Source ray radius10 is independent of separation/NPCwidth.

Exact Chu3/Frankly setup flags0x80600 +NPCEntry3 and Frankly's scripted
OR/CLEAR40000020 windows retain collision mask20 and movement mask40000000
separately; fresh reload and same-room continuation have different flag owners.
Re-extracted init/first-talk/fence streams, position/flag/angle/wall bodies and
source snapshot instructions read-only. Documentation links/whitespace pass;
no native edits or commit. Backend second-pass/native numerical differences,
source frame/flag lifetime and original runtime trajectories remain open.

### 2026-10-13 — Retained Mario travel writer and scripted NPC placement distinction

[Side numeric contract](../behavior/field/FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md)
now pins ordinary walk/dash input+198→travel+1a4 publication, absence of
local stay/jump/wall writes, and marioMoveMain's sole specialflag40 forced
writer. Combined-displacement/idle fallback heading is local and must not
replace retained travel each call. Source evt_npc_move_position sets target/
capturedstart/driver state and components, with no current/previous-root
writes or explicit-set-position call; ordinary steps do not reseed history.
Fresh full read-only DOL inspections/write census, links and whitespacepass;
no native edits/commit. Full event/transform/alternate-direction writer graph
and movement driver timing remain open.

### 2026-10-13 — Idle adjustment is a separate retained-travel writer

Fresh marioAdjustMoveDir0x800965fc recheck confirms both snapped-display
branches copy newly computed facing+1a0 into retained travel+1a4. The prior
local stay-body no-write finding does not exclude called idle/presentation
helpers. [Side numeric writer table](../behavior/field/FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md)
now includes that explicit writer and links the already recovered low-HP idle
contract: publish travel when the source adjust service is actually invoked,
while retaining ordinary idle/air history between those service requests.
Docs-only source recheck; no native edits/commit or runtime claim.

### 2026-10-13 — Native ordinary side-response binding

[Native side collision](../behavior/field/NATIVE_NPC_SIDE_COLLISION.md) now
binds current catalog flags/dimensions, retained travel, previous-root rollback,
source finite polynomial response and current-map/object wall retry. PMNPC006
retains Entry/setup/constructor masks; scripted explicit placement and flag
OR/CLEAR owners remain distinct from continuous movement and draw hiding.
Tests include source numerical deductions and live original-asset contact,
placement-history and flag/lifetime cases. Exact build/emulator validation is
recorded in [progress](../PROGRESS.md). Full registry/movement/timer/event
composition, Gekko/SDK/backend numerics and original/hardware traces remain
OPEN; this does not close the full request.

### 2026-10-13 — Additional stationary east NPC: original Darkly

[Darkly static/talk bindings](../behavior/field/ROGUEPORT_DARKLY_STATIC_AND_TALK_BINDINGS.md)
recovers source actorクラガリさん placement(-335,0,-335),direction270,
c_f_mura/S_1/T_1/descriptor0Z_1,30/30 dimensions, setupflags80600 and
init40000000/Entry3, with no regular event. Source early0..55 uniquely prints
gor_02_034_00 unchanged. Full ordered talk thresholds and leading175 special
name branch are retained; later graph cannot use Chu's generic selector.
Goomba4 alternative requires original wander/quest owners.

Validation: read-only fresh REL event extraction, DOL tribe/asset/US key reads,
bounded original sparse evaluator admission and all648 annotated gor event
reference census. Asset hashes/paths and raw controls recorded; no copied
original prose/assets in history. Docs-only, no native edits or commit.
Dynamic/foreign actor lifetime, later name quest and runtime/rendering comparison
remain open.

### 2026-10-13 — Native Darkly and reusable per-actor talk rules

[Native Darkly/rule binding](../behavior/field/NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md)
adds original c_f_mura assets, source placement/dimensions/flags and the unchanged
story<56 print arm. PMNPC007 retains source-order, full-message-ID rules per
actor instead of applying Mouser's thresholds to every NPC. Unmatched/empty
bindings cannot suppress jump or invent a later-story fallback. Generic room
prefixes and model metadata allow additional original area resources to use
existing presentation/collision/talk services. Original later175 flag/name
children and all-story lifetime remain OPEN; exact build/rendered fixture
validation is in [progress](../PROGRESS.md).


### 2026-10-13 — Original wandering driver and Goomba4 bounded delivery

Explicitly delegated documentation-only read-only source recovery delivered
[field wander/Goomba4 contract](../behavior/field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md):
44-byte work schema, four ongoing phases, move countdown multiple20 rounding,
strict readiness/contact branches, min20ms displacement cap, retained component
semantics, full step-sound request tail, frame-rate conversion and shared LCG.
NPC main direct40000000 movement, map bypass, seconds publication and distinct
automatic-pose400 owner are pinned. Init's added00400000 other consumers remain
open. Source Goomba4 init overrides tagged placement to(-370,0,130), radius70,
speed30; original bank/poses/dimensions/sound metadata and quest-before-story
checks guard the early gor_02_030_00 print. Low story alone is not quest closure.

Request remains OPEN for lifetime/restart, clock/gravity/support, shared RNG
seed/cadence, original quest item tables/selection and hardware/runtime parity.
No native code or tests/emulator changed; no numeric documentation-agent ID
assigned, so no delivery-log ownership invented. Static deductions are not
Dolphin/hardware observations.


### 2026-10-13 — Wander lifecycle/ground adapter follow-up

The [wander field contract](../behavior/field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md)
now pins authored Goomba4 npcMoveG netvertical0/no-ray exit, bulk-zero NPCdt
entry versus current clock anchor, setup init-completion→regular construction,
USER_FUNC retained polls and candidate-before-A regular-event pause. Added
mainflag00400000 owns candidate-loss saved-yaw restore/regular resume; stopped
events preserve countdown/work. Mario touch actor is cleared/replaced by NPC
scan and consumed by the preceding next field EVT pass. These are local source
owners, not hardware/runtime measurements or a quest0 fallback. General gravity/
support, clock/pause/manager interleaving, regular restart/work destruction and
quest item/window state remain OPEN.

### 2026-10-13 — Portable original wander-core binding

[Portable wander core](../behavior/field/NATIVE_NPC_WANDER_CORE.md) implements
recovered phases, retained work/components, ordered readiness, capped steps,
clock/RNG helpers and source sound requests. Tests retain exact phase/count/
random/comparison/byte-toggle behavior, without choosing an original shared
seed or replacing paper-facing readiness with clip timing. Original Goomba
resources were prepared locally, not registered as a stationary replacement.
Native regular-event stop/resume, live prior dt/contact, movement/pose services
and quest/item state remain required before activating Goomba4. Exact build
validation is in [progress](../PROGRESS.md); no new original-runtime or hardware
measurement. Parent request remains OPEN.


### 2026-10-13 — Native binding candidate/clock dependency clarification

[Ordinary candidate contract](../behavior/field/FIELD_ORDINARY_NPC_TALK_CANDIDATE.md)
now distinguishes every fully admitted actor's pause from strict-nearest winner,
original event-pointer presence from supported native playback, local failure
cleanup from actor/global skipped paths, and yaw-only save/restore from paper/
movement/phase state. [Wander contract](../behavior/field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md)
pins per-actor dt/movement/clear/pose/snapshot before final candidate scan: a
paused regular event leaves NPC clock active, and its next no-component movement
pass can select idle while preserving driver countdown/phase. No source quest0
or unsupported dialogue fallback is inferred. Static source checks; no native
code/test/emulator or hardware measurement performed by this delivery.


### 2026-10-13 — Native PMNPC008 live wandering binding breadcrumb

[Native live wandering](../behavior/field/NATIVE_LIVE_NPC_WANDER.md) records
PMNPC008 model/actor/rule/wander format, original tagged scan order, live Goomba4
source init/radius/speed/zero-vertical/direct-XZ profile, prior NPCdt/touch
publications, retained candidate pause and actual-speed S/W/R selection. Paper
state advances once per native simulation tick; rendering reads its matrix.
Original event presence is independent of unbound quest playback. Native RNG
seed1 is an explicit adapter, and original left/right sound requests now play their original primary samples
through NDSP; full macro/spatial routing remains unbound. Source complete quest/scheduler/clock/lifetime/audio and
original/hardware numeric/performance comparisons remain OPEN. No PROGRESS,
code or commit changes by this explicitly delegated documentation worker.


### 2026-10-13 — Static candidate-loss interrupt consumer

[Ordinary candidate contract](../behavior/field/FIELD_ORDINARY_NPC_TALK_CANDIDATE.md)
now recovers npcCheckInterrupt800440f8/234 and the +14010000000 regular-script
replacement request. It executes before the later candidate scan, so loss
posts a request for a following admitted NPC pass. Exact trigger priority,
null-script fallthrough and clearing masks are recorded. Fresh read-only REL
bytes plus relocation absence prove Chu3/Darkly regular and alternate interrupt
script pointers are null: ordinary local candidacy loss retains turned yaw,
then clears the unbound request; it does not restore map/saved yaw. Their
mainflags00400000 remain clear. Full foreign script writes, lifetime, EVT
cadence and measured presentation stay OPEN. Documentation-only source checks;
no native build/emulator/hardware claim.

### 2026-10-13 — Goomba4 quest guards and item-window services

[Goomba4 quest contract](../behavior/field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md)
recovers GSW1420/GSF5405..5407 storage and reset preservation, exact key-item
count/removal wrapper results, fixed [95,-1] table, quest13 dynamic table
allocation/update/free, and evt_win_item_select argument/result/key ownership.
This is item selection, not message Yes/No choices. Exact first/continuing
quest graph, original IDs/speakers/poses/waits and conservative known-state
ordinary admission are recorded. Generic quest writers, native saved-bank
ownership, full item-window services/event cadence and measured parity remain
OPEN. Documentation only; ISO read-only, no original binary/dialogue prose,
shared index/PROGRESS or implementation edits.

### 2026-10-13 — Original explicit item-table selection window

Explicitly delegated documentation worker recovered
[item-table window](../behavior/message/ITEM_TABLE_SELECTION_WINDOW.md): exact
(table,0,1) initializer/template0/three-window ownership, source-order duplicate
rows, eight-row geometry/scroll, direction repeat versus trigger endpoint rules,
A-before-B priority, row disable/cancel flags, close-before-ready/delete/key
ownership, original US item names/descriptions and icon307/308 -> TPL312/313.
Also recovered manager sequence/main order, partial native transition boundary,
cursor504, description paging and source draw requests. Read-only ISO DOL and
icon.bin/icon.tpl header checks; inspection artifacts remain /tmp. No original
asset/dialogue prose, source code, shared PROGRESS/index or emulator/profile/save
edit. OW-INTERACT-01 stays OPEN for full rendering/backend/input/cadence and
native quest/interpreter/inventory binding; this is not matched-runtime parity.

### 2026-10-13 — Complete original item presentation metadata

Explicitly delegated read-only documentation work recovered
[item presentation metadata](../behavior/unit/ITEM_PRESENTATION_METADATA.md):
itemDataTable803108a8/3520 has340 physical40-byte records; only zero record339
lacks name/description keys. Every key for0..338 is uniquely in original
msg/US/global.txt. All340 icon fields have bounded descriptors/frame references
into613-image icon.tpl;305 distinct item icons, four animated item records.
Exact consumer anchors, full-span/resource hashes, item95/107 keys/pointers and
record338's distinct description key are pinned. Metadata-only census stays/tmp;
no assets/prose/native implementation/index/PROGRESS/build/profile/save changes.
Native rendering, clock/input ownership, original inventory/quest binding and
matched Dolphin/hardware validation remain OPEN.

### 2026-10-13 — Original item-selector frame and decoration

Explicitly delegated source recovery now pins
[item-selector frame/tab](../behavior/display/ITEM_SELECTOR_WINDOW_FRAME.md):
winMgrDisp Rz*S, per-frame center translation, definitionRGB/managerlow8alpha,
base border20, decoration1 blue150x28 tab/border8 and exact integer centering;
windowDispGX2_Waku_col's36-vertex centered nine-slice UV/FIFO, original
f/msgWindow.tpl descriptor0 and full used GX material program. Complete fresh
spans/hashes plus180-write/36-vertex numeric checker; no native source/assets/
shared index/PROGRESS/ASSETS/build/profile/save edits. Camera/filter/raster,
full list/header/help rendering, original clocks/input/event ownership and
Dolphin/New2DS correctness/performance remain OPEN.

### 2026-10-13 — Native shared source window-frame binding

The recovered frame/tab geometry now has a shared host-testable plan and
preallocated citro3d backend; see
[native binding](../behavior/display/NATIVE_TYPE0_WINDOW_FRAME.md). Original
EXP uses the shared path. Source base/tab centers, signed centering, RGB/low8
alpha and all nine quads are retained; the original panel texture is reused.
Full selector list/header/help/scissor, manager/input/event/inventory ownership
and Goomba4 quest branches remain unbound. This delivers a rendering component
and does not close OW-INTERACT-01 or establish Dolphin/New2DS raster/performance.

### 2026-10-13 — Retained selector owner and window-font used path

[Native retained owner](../behavior/message/NATIVE_ITEM_TABLE_SELECTION_OWNER.md)
now binds template0 traversal/result retention and source-space draw plans.
[Exact draw requests](../behavior/message/ITEM_TABLE_SELECTION_DRAW_REQUESTS.md)
and [font consumers](../behavior/message/WINDOW_FONT_STRING_AND_MESSAGE_CONSUMERS.md)
recover outlined heading/string, row/header matrix-message, help direct-message,
source width/spacing/edge/icon/reset dependencies. Quest95/107's six original
US strings are plain and descriptions have two lines, so their actual used
font path can bind while broader font services remain open. Full native
rendering/manager/input/event/pouch/switch binding and matched raster/hardware
performance remain OPEN. Read-only US startup now pins GP+16c=1 and spacing2; please finish
its broader read/write lifetime and live palette/font mutations.

### 2026-10-13 — Native selector rendering and removal-revision dependency

[Native source-font/icon renderer](../behavior/message/NATIVE_ITEM_SELECTOR_RENDERING.md)
now renders the retained template; private Azahar previews verify actual95/107
art/text, source clipping, main-stick scrolling/wrap and cursorflags20 mirror.
[Exact icon recovery](../behavior/display/ITEM_SELECTOR_ICON_ANCHORS_AND_FLAGS.md)
corrects the earlier camera-number/bottom-anchor misattribution.
[Parameter revision](../behavior/unit/POUCH_PARAMETER_REVISION.md) recovers both
required post-removal leaves and source partner HP table. Native quest events,
keys/pouch/global-switch persistence, status/SFX macros, full manager/GP/RNG/
camera/font lifetimes and matched raster/hardware performance remain OPEN.
Source help masks100/200 are C-stickY; native production of those axes remains
unbound, while the actual95/107 descriptions use only two lines.

2026-10-13 native increment: Goomba4 quest2 now binds original guarded NPC event
entry, `irai_02_03`/`irai_02_04`, retained `[95,-1]` selector, nested talk/selector
keys, remove95 + original Mario/party parameter revision, and unconditional
GSF5406 write after original removal0/-1. Original BP/partner tables are pinned
read-only ISO exports. [Native scope](../behavior/field/NATIVE_GOOMBA4_QUEST2_HAND_IN.md)
and [talk control/pose evidence](../behavior/field/GOOMBA4_QUEST_TALK_CONTROL_AND_POSE_ADMISSION.md).
Parent request stays open: quest13 needs known5407 persistence/migration and
original Mario poses; full pouch/switch-bank checkpoint round-trip, task
acceptance, status/SFX/global ownership and matched raster/hardware performance
remain unbound. This is a native implementation delivery, not an original
execution trace.

### 2026-10-13 — Goomba quest13 native binding and upstream recovery

Delivered [native quest13 hand-in](../behavior/field/NATIVE_GOOMBA4_QUEST13_HAND_IN.md),
[native pouch checkpoint v8](../behavior/format/NATIVE_POUCH_CHECKPOINT_V8.md),
and [Trouble Center writers/acquisition](../behavior/field/TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).
First/continuing source dialogue, duplicate item selector, original Mario poses,
5407 write and checkpoint persistence are bound for explicit known state.
Native Trouble Center acceptance, quest2 item-entry/GetItem presentation and
quest13 acquisition remain pending; parent request stays open.

### 2026-10-13 — Plaza Goomba3 and original task-service recovery

Delivered [Goomba3 setup/talk](../behavior/field/GOOMBA3_SOURCE_SETUP_AND_TALK.md),
[item-get/coin frontiers](../behavior/field/GOOMBA3_ITEM_ENTRY_GET_AND_COIN_SERVICE_FRONTIERS.md),
and [native binding/services](../behavior/field/NATIVE_GOOMBA3_TALK_AND_TROUBLE_SERVICES.md).
Known nonquest2 ordinary talk and quest2 waiting branch bind unchanged US text.
Portable task start/cancel preserve source17-flag/six-byte resets and low-level
all-copy removal for recovered task2/13 tables. Full acquisition/itemseq, coin
acknowledgment, task menu filtering/callers, switch persistence and Goomba3
side/gravity wander remain pending. Parent request remains open.

### 2026-10-13 — Goomba3 source map-side movement delivered and bound

Delivered [original NPC map-side/zero-vertical contract](../behavior/field/FIELD_NPC_MAP_SIDE_MOVEMENT_AND_GOOMBA3_ZERO_VERTICAL.md)
and [native map-side/wander binding](../behavior/field/NATIVE_NPC_MAP_SIDE_AND_GOOMBA3_WANDER.md).
Goomba3 now uses original100/30 shared wander, three ordered null-filter rays,
half slide, first-contact publication before second pass and source zero-vertical
no-op. No invented40000000, floor snap or gravity. Original SDK/ray traversal,
global scheduler/RNG and later vertical/reflection/filter domains remain open.
Full item95 get-item sequence recovery continues; parent request remains open.

Delivered [item95 normal get sequence](../behavior/field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md), including entry mode16/runtime flag distinction, deferred grant, original windows and acknowledgment/cleanup. Native acquisition/presentation and full-storage UI remain pending.

### 2026-10-13 — Portable original item95 receive and source presentation providers

Delivered [portable receive owner](../behavior/field/PORTABLE_ITEM95_RECEIVE_SEQUENCE.md),
[exact packed insertion](../behavior/unit/POUCH_KEY_ITEM95_RECEIVE_INSERTION.md),
[original windows/pose/icon](../behavior/field/ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md)
and [type6 effect bank/update/draw](../behavior/effect/FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md).
Actual low-level95 insertion and receive service order are implemented/tested;
original windows precede grant, acknowledgment precedes cleanup, failure holds
at actual discard sub5. No task completion or native presentation is fabricated.
Corrected the legacy pouchGetItem pointer-lookup stub. Native window/text/effect/
audio/registry/control bindings, discard continuation and upstream quest callers
remain open; parent request stays open.

2026-10-13 follow-up: original US first-byte article/template expansion and
window acknowledgment/close gates recovered in
[message contract](../behavior/message/ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md).
Portable text expansion and source effect6 clock/two-pass draw plan implemented;
original M_I_U and actual base-bank42/43/44 packs prepared with native lazy pose
bank binding. [Native scope](../behavior/display/NATIVE_ITEM_GET_VISUAL_RESOURCES.md).
Quad correction is64x32; startup SDA2 color is zero within the bounded fresh
source state, with indirect/REL writers still unresolved. Native windows,
GX-to-PICA effect raster, camera/audio/control/name-registry integration and
failed-receive discard continuation remain open. Do not enable acquisition or
claim complete item-get fidelity on the strength of these resource providers.

2026-10-13 follow-up: finite original item95 name/help manager, US direct-color
font consumer and native window renderer implemented; source definitions,
frame-before-resize ordering, help prior-display publication, button-repeat
paging and AutoDelete parameter lifetime retain original ownership.
[Native window scope](../behavior/display/NATIVE_ITEM95_WINDOWS.md) and
[source display/color review](../behavior/message/ITEM95_GET_WINDOW_DISPLAY_ORDER_AND_FONT_COLORS.md).
Original color streams compile at preparation, leaving raw tokens retained;
rendering reuses existing font/frame resources and batches glyphs through C3D.
Global manager slot sharing, failure/discard window, native item/effect/audio/
control integration and original outer cadence remain open. This provider alone
does not enable quest acquisition; the parent request remains open.

2026-10-13 follow-up: read-only source recovery pins projected image44 POS
coordinates, direct image42 UV, distinct repeat/mirror wrapping, exact draw
state and actual camera5/class2 dispatch. Camera5 copies camera4 eye/up/target
but owns lens/clip/depth and excludes camera4 final zoom correction.
[Source effect](../behavior/effect/FIELD_ITEM_GET_TYPE6_GX_STATE_AND_PROJECTED_TEXTURE.md),
[camera producer](../behavior/camera/FIELD_ITEM_GET_CAMERA5_PRODUCER_AND_PROJECTION.md),
[native shader/camera scope](../behavior/display/NATIVE_ITEM_GET_EFFECT6.md).
Portable projected matrix and finite camera5 frame implemented; native PICA
consumes original masks with two passes, explicit projection/viewport, no render
clock or tint bake. Source camera4 prepared-vector supply/display sorting,
icon307 binding, original audio/control/name teardown and native quest acquisition
remain open; full-storage discard remains pending. Parent request stays open.

### 2026-10-14 — Original item95 name registry, icon and BGM query

Implemented independent32-slot field-item/icon ownership, source stale-name and
active-duplicate behavior, force/poll/name-wide deletion and the second active
check before icon synchronization. The original parcel307/TPL312 now has a
bounded camera5 PICA consumer with shared textures and source alpha/depth states.
See [entry/icon contract](../behavior/field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md),
[map blend/shadow admission](../behavior/display/ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md),
[native consumer](../behavior/display/NATIVE_ITEM95_REGISTRY_AND_ICON.md), and
[audio routing/query](../behavior/audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).
The portable get owner now accepts the raw source `psndBGMChk` result (-1 vacant,
0 assigned), with a test for assigned-to-vacant resume and once-only restoration.
Isolated native graphics captures show original parcel/name/help/effect and
teardown at60FPS; they do not run an actual acquisition or grant95/write5405.
Native owner dispatch/control/audio, original stream retirement, source camera4
queues/shadows, deferred icon metadata and map40 material remain open. Parent
requests remain open; do not substitute a guessed fanfare timer or missing service.

### 2026-10-14 — Fanfare loop and retirement resolved

[Original container-loop/retirement contract](../behavior/audio/FIELD_KEY_ITEM_FANFARE_CONTAINER_LOOP_AND_RETIREMENT.md)
resolves the prior DSP-header ambiguity: the key-item fanfare plays once from
COMMON restartffffffff. Producer end publishes stop, stream main clears flags0,
then BGM main publishes group1 vacancy. All146 original stream headers were
surveyed (111 loops/35 one-shots); converter now follows these container markers,
validates frame offsets/extents and removes stale loop sidecars. Original fanfare
PCM was checked at stereo32000Hz/83888 frames with no loop sidecar.
No synthetic duration timer is needed. Native original audio routing, retained
field-music attenuation/resume and actual stream retirement still need binding;
parent acquisition/service requests remain open.
