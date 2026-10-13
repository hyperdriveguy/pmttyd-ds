# OW-INTERACT-01 — Mario A/X/B interaction arbitration and playback

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [ ] Bind original save-block hit flag8 producer, update states/animation and
  common save-window event0x803bac3c to SAVE-01 persistence; original plaza
  spawn/model/system-event owners are recovered, proximity+A is not.

- [ ] Bind ordinary NPC pre-display/pose/facing and `mtx_setup` owners through
  `animPoseDrawMtx`; source dir270 requests facing90, which is an animation
  parameter rather than world yaw. Recover tribe scale/height/talk-anchor
  semantics and tavern first-floor RGB fade before adding interior actors.
- [ ] Recover `gor_villager_urouro` init/main numeric/collision/pose/timing
  helpers before activating Goomba2 regular motion. Tagged aitypeなし does
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
