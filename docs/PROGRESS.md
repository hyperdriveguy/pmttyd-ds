# Progress and next-session handoff

Newest entries first under this single heading. Keep entries dated, write link paths relative to `docs/`, and record exact validation, limitations and next tasks in each entry. Append-only: do not rewrite earlier entries.

## 2026-10-13 — Temporary Frankly follows the party

Bound [temporary Frankly follow](behavior/partner/NATIVE_TEMPORARY_FRANKLY_FOLLOW.md)
after the original first-talk. He now advances independently through imported
terrain and door travel with original W_1/S_1 poses. Source kind11 uses the same
partyMoveWalk callback; recovered secondary source selection follows Goombella
when ordinary and within150 original units of Mario, otherwise Mario. Added
[used lifecycle/fence admission](behavior/partner/FIELD_PROFESSOR_TEMPORARY_PARTY_AND_FENCE_ADMISSION.md)
and inbox evidence. This does not advance story9 or enable unsupported saves.

Validation: `make test` passes103 CTest and96 Python tests; `make all` builds
the3DS binary. Original-data test completes29-message first-talk then walks100
ticks, checks independent Frankly history/movement, story9 and save suppression.
Selection tests cover the150-unit threshold, missing/special party state and
unordered floating comparison. Fixed the actor test's closed catalog count to
allow additional models while asserting required original model identities;
the initial full test run exposed concurrent save-block catalog expansion.
No new emulator capture or hardware test for following.

Next/limits: bind the source story9 house close callback to fence removal,
original OFF_d_saku paper/image/offscreen animation and A_saku_04 collision
removal, then source story10 lecture/choices/battle. Current portable follower
uses adapted trail/terrain stepping and explicit ordinary Goombella mode/status;
complete source ring/flags/pose timing and temporary save lifecycle remain open.
Earlier harbor story gaps and measured handheld performance still block full
prologue parity.

## 2026-10-13 — Original professor first-talk runs natively

[Native scene](behavior/story/NATIVE_PROFESSOR_FIRST_TALK.md) executes the
original180-command gor REL event in EventNativeRuntime, preserving brothers,
shared waits, actual GSW0 writes, all29 original message IDs/control bytes and
119 callback starts. The original story8 house door starts the event before
its door-end wait; completion reaches story9 and restores field controls.
Bound source cameras, seven Mario event poses, original Frankly/Goombella
animation assets, source map icon314/TPL319 and Frankuri1 stream. Corrected
the source hop to225ms/height30 and scripted travel facing. Save/exit-save
guards suppress unsupported temporary-party checkpoints. Asset provenance and
handheld atlas resampling are recorded in [assets](ASSETS.md).

Validation: full shared-worktree `make test` passes101 CTest/96 Python tests;
`make all` builds the3DS binary. Independent staged-source host/native compile
passes; after regenerating staged-format field actor assets in a separate
`/tmp/professor-stage-verify` tree, all100 staged CTest tests pass. The first
staged test run exposed the concurrent PMANIM-format cache mismatch, rather
than a scene failure; the isolated rebuild does not modify workspace assets.
Original-data tests check29 ordered messages,119 callbacks,17 cameras, source
item/hop/music, door admission, story9 and save suppression. Independent Mario
animation comparison passes102 samples, maximum error8.58307e-06. Azahar
isolated replay/captures at `artifacts/azahar-professor-scene-memory/` shows
original attributed dialogue and restored field HUD. Initial4MiB scene atlas
run failed to restore the east checkpoint; retry with1MiB succeeds. Final
scripted-facing/save-guard changes were rebuilt and host-tested after capture.

Limits/next: temporary Frankly currently remains at his saved source position;
follow/lifecycle/persistence and original story9 door-close fence event are
next. Paper fence animation, reaction/SFX/display-flag consumers, exact turn
interpolation and source movement/collision stepping remain unfinished. Story10
lecture/choice/battle continuation, earlier harbor gaps and hardware performance
remain required. This increment is not complete prologue or original parity.

## 2026-10-13 — Priority0 Monosiri table, consumer and census answer

Delivered [Tattle table/consumers](behavior/battle/MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md)
and rewrote [lookup](behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md). Corrected
spatial-sound misclassification, lis arithmetic, impossible-null and safe-caching
claims. Original sampling actually tests pointer0, reloads kind and looks up
again; arbitrary invalid kind can wrap to0. Initialized231-record table supplies
six name/help/log/model/pose/additional-key fields. Complete message-key callback
resolves its unused second argument then raw output before fresh unit lookup,
without actor translation. Direct census:17 DOL calls,2 REL24 imports.

Validation: generic metadata verifier13 spans/40 anchors; sampling verifier7
bodies/48 anchors/14 constants passes. Independent DOL scan and US usage audit
agree17 linked/0 nonlink/0 data-word candidates plus2 imports across29 RELs.
All231 table records/column counts and original example identifiers inspected;
wrapped-zero derivation checked. Inspection files and reports remain `/tmp`.
Link audit708 Markdown files/0 broken links; whitespace check passes. Docs and
metadata only: no make rerun, native change or original asset/dialogue export.
Unrelated staged/worktree edits are preserved outside the commit.

Next/limits: table mutation/alias/valid-kind and full Tattle/log/AC/REL/event/
save/runtime binding remain open. Separately observed flag setters/readers use
persistent switches and battle work, not table-population writes. Recover that
shared flag graph and correct the SW_SET rotate-by-five interpretation, then
continue priority0 source/menu/audio/runtime requests. No original gameplay/
hardware/full-game equivalence claim.

## 2026-10-13 — Original Hammer charge cue resource answer

Delivered [two charge cue bindings](behavior/audio/HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md)
and checked that portion of AC-02. Actual callers8012a4b0/4d8 resolve PI1/PONE1
through table ordinals2619/2620, FX862/861, group6/macros995/994 and original
samples776/775. Directory rate16000 and nonloop1526/5276 frame lengths are
static dependencies, not a complete cue duration or substitute WAV playback.
Three original bank asset hashes and derived metadata stay in a source manifest;
no original macro bytes/PCM/ISO or new asset conversion enters history.

Validation: `/tmp/verify_hammer_evidence.py` reparses original ISO rows/project/
pool/directory, passing3 assets/2 cues/2 macro bindings/2 sample records and
all1247 directory records. Generic DOL verifier passes14 spans/85 anchors,
including full Hammer main/two caller instructions; complete behavior verifier
passes (`/tmp/inbox-hammer-behavior-check.log`). Both existing ignored PCM
counterparts exist. Link audit706 Markdown files/0 broken links; whitespace
check passes. Documentation/metadata only; no make test/all rerun. Unrelated
staged/worktree edits preserved outside this commit.

Next/limits: full SFXMain spatial/start/volume/effect backend/MusyX execution,
EVT-01 query-specific menu/source/global domains and original scheduler/audio
traces remain open. AC-02 lifecycle/display/forced/suppression writers and
original charge playback remain unchecked. Continue by inbox priority; no
original runtime/hardware/full-game equivalence claim.

## 2026-10-13 — Priority0 positional-SFX correction and update producers

Reinspected and rewrote original [provisioning](behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
[stop](behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md) and [accessors](behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md).
[Update/correction delivery](behavior/audio/POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
closes full psndMain cooldown producer and scoped SFXMain completion/pending/
preference-reset paths. Corrected low13 table index, pending flag direction,
row+4 high backend bits, numeric/name ABI, mute override,40-slot free search,
strict reclaim priority, live255 defaults, independent delay/offset stores and
coherent16-bit generation wrap. Prior conflicting historical deliveries are
explicitly superseded; original request text remains unchanged.

Validation:9 complete body listings independently regenerated under `/tmp`.
`python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`
passes13 DOL body/table/numeric spans and78 instruction anchors. Link audit
checks704 Markdown files with0 broken links; `git diff --check` passes.
Metadata only; no tool/native code change, so make test/all not rerun. Read-only
ISO and user emulator profiles/saves untouched; no original code/assets exported
into history. Commit excludes unrelated staged/worktree changes.

Limits/next: SFXMain middle/spatial/startup and Sound/MusyX resources/macros remain
partially recovered; original execution and other source writers stay open.
Next recover full used effect startup and two Hammer charge cue macro bindings,
then continue EVT-01 menu/global/source and original scheduling evidence.
Full game documentation/remake and hardware parity remain incomplete.

## 2026-10-13 — Original east Mouser talk and per-area actor resources

Feature commit `8cdab4a` adds stationary east Chu3/Mouser using original
`m/gor_02/s` XYZ/tribe/direction, `a/c_tyusan` / texture bank and `CHU_S_1`
sparse animation. Its simple original talk event selects CASE_LESS branches
at56/112/164/215/261/339/373/401; original US IDs/payloads/control tokens and
actual actor attribution are retained with no story write. Current runtime
coverage is explicitly story0..55, not a recovered visibility rule. Native
proximity0.65/height0.25 and grounded A-before-door/jump priority remain
provisional until original search/arbitration binds. No original story is
inferred from absent native save data. Original Frankly story8/10 init positions
move into the same catalog; his complete scenes remain separate owners.

The portable catalog validates model namespaces, story/interior gates and
whole records atomically. Destination-area models stage with geometry; failed
imports retain the resident area. Animation samples persist per model and cache
simulation ticks/poses; hierarchy arrays retain capacity, recursive dispatch
avoids std::function allocation, and root trig runs once per instance. GPU
texture batches use citro3d linear buffers and whole-mesh capacity admission.
Visible actor/vertex/sample-failure/overflow counters expose bounded submission.
See [native scope](behavior/field/NATIVE_AREA_ACTOR_CATALOG.md),
[source bindings](behavior/field/ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[provenance](ASSETS.md). Initial transform helper commit is `c46ee29`.

Validation: isolated source snapshot `/tmp/pm-field-verified-at8uf8ls` of the
feature changes passes `make test`: **99 CTest / 96 Python tests**
(`/tmp/goal-field-isolated-test.log`), and `make all` without warnings
(`/tmp/goal-field-isolated-build.log`). Snapshot excludes concurrent opening/
professor additions and uses local ignored original resources. Original-data
tests cover18 branch-boundary stories/all9 unchanged US payloads, actual talk
entry/closure, speaker identity, no story/save mutation while talking, original
room fixture ground/wall admission, and sampled model streams. Catalog tests
cover malformed/unsafe/overlapping records and retained state on failed reads.
Earlier shared runs also passed100 CTest/96 Python; shared native builds briefly
failed during in-flight frontend/audio/professor edits. Those intermediate
failures are not claimed as feature validation; the isolated build is passing.

Immutable ignored `artifacts/ttyd-field-actors.3dsx` is the validated snapshot
binary. Isolated Azahar run uses `tools/make_field_actor_fixture.py` and
`tests/azahar_field_actor_talk.py`, output
`artifacts/azahar-field-actors-verified/`, log
`/tmp/goal-field-verified-smoke.log`. Reviewed original-Mouser talk and final
return captures: source art, speaker-reaching tail, restored exploration/menu
and actual manual save with unchanged story8/checksum. The script requires a
new save timestamp after source-window closure, avoiding repeated-A reopening
as false success. Xvfb required display-socket escalation; user profiles/saves
and ISO were untouched. This is native scene validation, not Dolphin or hardware
parity/performance evidence. No new Dolphin observations were taken.

Documentation worker recovered ordinary NPC and save-block bindings and added
inbox breadcrumbs. Save-block original spawn `(185,60,-55)`, scalar10 and
hit/system-event graph are documented in
[source binding](behavior/field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md).
Conversion exposed S_1 UVs outside0..1 plus scrolling texture keys; the ordinary
compiler correctly rejects that domain. The incomplete probe pack was removed
and no save-block record ships. Bind original animated texture matrices and
clamp/material services before adding its visual/hit/save implementation.

Next: close original NPC search/priority/facing/collision and shadow/material
services; bind Goomba wandering and tavern NPC fade; integrate professor/fence/
lecture scenes through their source owners; implement save-block texture/hit/
card lifecycle. Gather dense-room/movement timing traces on New2DS hardware and
matched Dolphin observations. Broader prologue/game fidelity remains incomplete;
the user's goal stays active. Other concurrent worktree/staged changes are
preserved outside this feature commit.

## 2026-10-13 — Priority1 initialization/count and priority2/3 inbox portions

Delivered [DMG-01 initialization/count composition](behavior/battle/DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md):
full local Entry/UnitSetup, signed HP/FP copy and later imports, distinct body-part
selectors, setup count0/result1, four encoded TimingA increment routes and
read-only shared count query. Independently corrected result-reader battle/output
capture order and pending bit0/mask1 wording. Marked scoped delivered portions
in DMG-01, AC-03, AC-01 and the shared STATUS-01 initialization prerequisite;
request text remains unchanged and parent integration/runtime work stays OPEN.
Priority4 AC-02 full lifecycle/display/macro bindings remain pending.

Validation: full Entry verifier33 bodies/441 anchors/11 spans; UnitSetup8 bodies/
190 anchors/5 spans; Jump graph268 commands/19 bodies/57 anchors/5 concurrent
blocks; input cadence11 hashes/154 anchors/28 calls/3 data bindings; boss Jump
10 fingerprints/55 instructions/9 constants; lecture helper roles and complete
general behavior verifier pass. Setup/TimingA/count/result reader listings
regenerated to `/tmp/inbox-*.elf`, read-only ISO. `python3 tools/check_doc_links.py`:703 Markdown files,0 broken links;
`git diff --check` passes. Documentation only:
`make test`/`make all` not rerun. Unrelated staged/native edits remain excluded.

Limitations/next: actual loadout/stat/bonus and remaining counter writers, full
consumer/reward/defeat/tutorial eligibility graph, query-specific cursor domains,
source/platform services and original multihit/input/RNG/pause/turn traces still
need recovery. Continue priority0 remaining source/runtime acceptance, then
priority1 consumer/counter owners, priority2 global defense/forced-counter gates,
priority3 tutorial writers, priority4 Hammer display/audio and priority5 turn
lifetime. No new Dolphin observations, hardware or complete-game fidelity claim.

## 2026-10-13 — Priority0 documentation inbox delivery

Consolidated [EVT-01's local service contract](behavior/event/EVT_01_SERVICE_CONTRACT_AND_REMAINING_BINDINGS.md)
and corrected the [six unit-query leaves](behavior/battle/BATTLE_UNIT_QUERY_EVENT_LEAVES.md).
Independent read-only US ISO inspection confirms HP's retained early battle,
ID-1-before-lookup/output gate, work's late lookup, and distinct status/data
capture order. Split EVT-01 todos into checked static portions and open runtime/
source requirements; preserved request text and registered the delivery.

Validation: nine source verifiers pass: event manager/root/descendants/dispatch
 tail/run-controls, weapon menu domains, ActMenu, menu interruptions and Jump
source registry. Registry confirms3 streams,156 direct sites and72 callbacks.
Six complete leaf listings were regenerated to `/tmp/inbox-*.elf` only.
`python3 tools/check_doc_links.py`:701 Markdown files,0 broken links.
`git diff --check` passes. Docs only; no make rerun or native code changes.
Existing unrelated worktree edits were preserved and excluded from this commit.

Limitations/next: EVT-01 stays OPEN for original pause/multicredit/creation/
deleting/wait traces, query-specific unsupported cursor slots, global/dynamic/
transitive sources and backend domains. No original observation or hardware/
full-game equivalence claim. Continue priority1 DMG-01 allocation/stat/init and
attack-count ownership, then AC-03 defense integration in inbox order.

## 2026-10-13 — Rebuilt movement timing sample

Added the renderer declaration required by in-flight front-end code so the
current tree builds; no performance behavior changed. `make all` succeeds. The
rebuilt 3DSX capture under `artifacts/azahar-perf-current-plaza/` holds60 FPS
after a camera move and reports0.79ms simulation,5.1ms CPU and0.5ms GPU. The
ordinary plaza scene includes Mario, Goombella and nearby NPCs. This is a single
software-emulator sample, not a percentile trace or hardware measurement.

An east-room fixture attempt under `artifacts/azahar-perf-current-east-valid/`
was rejected by native restore validation and displayed “Saved room unavailable;
started in plaza”; it is not an east-actor timing sample. No substantial
optimization is indicated by the valid capture. Next: construct a verified
current-format east-room fixture, then gather repeated timing samples across
animated harbor/east scenes and cutaway-active movement. Hardware profiling is
still required; performance review remains open.

## 2026-10-13 — Current renderer workload and build limitation

After the field actor/frontend source update, the current `romfs/manifest.json`
records plaza at40,714 triangles/150 batches, harbor22,922/87, east33,444/145,
west34,885/127 and station41,084/54. Plaza has26 cutaway groups with1,425
triangles total (maximum186 in one group). The renderer performs bounds checks
before triangle intersection tests, so cutaway testing is a bounded workload;
full map geometry and batch submission are more plausible GPU/driver costs.
This is a static workload assessment, not a measured bottleneck. Existing
128-pixel map texture caps and atlas reductions remain in effect. No behavior or
asset changes are justified without profiling evidence.

Ran `make all` against the current shared worktree; it fails in
`source/render.cpp:794` because the new `Renderer::renderFrontend` definition has
no declaration in `include/platform.hpp`. This frontend integration is in-flight
work outside the performance review; no source files were changed to work around
it. Because the build failed, the prior Azahar captures exercise an earlier
executable and cannot verify this updated renderer. Next: once the interface
integration is complete, rebuild and repeat moving-scene measurements with the
timing overlay visible, then profile on the target 2DS XL before selecting any
substantial optimization. Do not mark performance complete yet.

## 2026-10-13 — Emulator performance review

Reviewed the renderer's per-frame costs: map animation vertex updates/cache flushes,
material batch submission, actor pose evaluation, and triangle-level exterior
cutaway checks. Existing size reductions are documented in [asset provenance](ASSETS.md):
source maps cap textures at128 pixels, and original character atlases trim unused
regions. No code optimization was justified by this pass; preserving original
geometry, animation, culling and gameplay behavior takes priority until a measured
hotspot is found.

Isolated Azahar software-renderer captures use separate profiles under
`artifacts/azahar-perf-plaza/` and `artifacts/azahar-perf-professor/`. The plaza
boot overlay showed60 FPS,1.63ms simulation,5.1ms CPU and0.5ms GPU. The professor
story8 fixture showed60 FPS,4.15ms simulation,5.3ms CPU and0.5ms GPU. The captures
are brief overlay samples, not percentile/frame-time traces. The attempted
walkthrough completed, but its movement capture hid the timing overlay behind a
development notice, so it provides no moving-camera timing result.

No `make test` or `make all` was run because this was a read-only performance
review and the shared worktree contains unrelated in-flight implementation edits.
The emulator reports steady60 FPS with CPU/GPU headroom in these scenes; no
reproducible emulator dip was observed. Software Azahar timing does not establish
real 2DS XL performance. Next: collect repeated per-frame timings on hardware and
capture cutaway-active movement without overlays before changing hot paths. If
original-equivalent behavior and measured hardware performance conflict, track
the affected behavior as a high-priority fidelity/performance task rather than
silently changing it.








## 2026-10-13 — Original Frankly animation and first-talk message prerequisites

Added the original c_kuri_h model/texture bank and six real clips
Z_1,S_1,T_1,W_1,R_1,D_1 to the local animation pipeline. Native story8 professor
interior draws original animated S_1 at source init(-135,0,-240). The12-cell
RGBA8 atlas crops unused rows to1024x256 without resizing those cells or changing
colors, saving3MiB of texture memory; a checked V scale adapts the existing
animation pack UV domain. Original animated geometry/hierarchy/visibility stays
intact. Actual bank D_1 versus tribe literal d_1 is explicitly recorded with no
invented alias. See [assets](ASSETS.md) and
[complete scene evidence](behavior/story/PROFESSOR_FIRST_TALK_AND_FENCE.md).

Message parser/playback now binds dkey and its close: source0xfff5 payload1/0,
nonzero sets0x80/clears acceleration/anchors elapsed to record schedule; zero
clears0x80 only. Timed source waits stay active under repeated A input. Tests now
parse/play all29 original first-talk payloads, including resolved party `_kur`
keys, original malformed-looking `</dynamic 3>`, styles/colors and unchanged
raw bytes. No narrative text or substitute scene is authored.

Validation: `make test` passes **96 CTest / 96 Python tests**
(`/tmp/professor-tests.log`); `make all` builds native3DSX with no warnings
(`/tmp/professor-build.log`). Explicit original-data message test passes20
harbor+29 professor payloads. Independent animation reference generation and
`actor_visibility_tests romfs/professor.pmanim artifacts/professor-animation-reference.bin`
pass45 source samples, maximum host model error1.90735e-06. Native input-gate
regression verifies acceleration reset, real elapsed wait and timed reopening.
Document audit checks695 Markdown files with0 broken links; diff whitespace
check passes. Isolated Azahar story8/interior2 fixture under
`artifacts/azahar-professor-idle/` boots inside the original house with Frankly
visible; reviewed boot.png. Fixture is a native development save, not original
GameCube progress. User emulator profiles and read-only ISO are untouched;
all original/generated assets stay ignored and outside source history.

Limits/next: this is character/message prerequisite work, NOT complete
first-talk/fence execution. NPC yaw/clock/scalar and init visibility are scoped
native adapters awaiting producer/backend binding in OW-ROAD-01. Source
first-talk's initial position override, concurrent brothers/shared wait, actor
movement/poses, cameras/music/effects/item display, temporary Frankly party1 and
story8→9 remain to execute. Fence removal must proceed9→10 and dispatch the
original lecture, whose common return is11. Consider the existing EventNativeRuntime
for the original REL script rather than manually flattening concurrent commands;
DOL-only pack_event_scripts needs REL-section relocation/data provisioning first.
Broader arrival/tutorial/story progression and hardware performance remain
unfinished. Goal stays active.


## 2026-10-13 — Harbor partner fall-through and original follow distances

Reproduced the user's harbor ledge complaint on original converted geometry:
the old center/crossing landing query could reject valid support and fall below
the dock until recovery copied Mario's root. Read-only original contracts prove
five-probe support BEFORE fall integration, current-velocity drop admission,
normal homing floor-loss acceleration-1 versus stay-1.5, velocity cap-16 and
horizontal damping0.2 followed by1.3*retained speed/direction. Native support now
uses Goombella's original width26/height24, ordinary partner attribute filtering,
Q1000 pointY and highest admitted support. Wall occupancy no longer rejects
valid landing; only Mario's jump flag marks history jumps.

Corrected an initial distance interpretation during validation:45/50 are speed
thresholds, not a fixed trailing gap. Original q0 activity normally remains
nonzero through dash-base2.25 fallback even at idle. Replaced fixed0.5 trail
spacing with width/speed history reach13+speed, separate wake-up50/idle100,
source speed selection/thresholded bonus and ordinary overlap box/corrections.
Restore now uses a0.3 leftward native seed based on general placement's30-unit
magnitude instead of the provisional0.8; full facing/search placement is unbound.
Evidence/contract commit `41b0e6d`, implementation/fixtures `9672cd6`; see
[ground-follow distances](behavior/partner/FIELD_PARTNER_GROUND_FOLLOW_DISTANCES.md)
and [native binding scope](behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md).

Validation: final `make test` passes **95 CTest / 96 Python tests**, log
`/tmp/partner-fall-final-test.log`; final `make all` builds3DSX without warnings,
`/tmp/partner-fall-final-build.log`. Actual harbor regression uses two deck
positions,90 idle ticks, a verified admitted Mario jump and300 following ticks;
partner minimumY>=0.149, zero resets and grounded0.15 arrival. Portable tests
cover support-before-gravity, side-probe landing independent of wall occupancy,
retained speed damping, history reach, overlap, dense jump marks and blockages.
The old idle catch-up test's100-tick dash assumption was changed to260 ticks
for the original1.5-unit idle bonus cap. `core_tests --original` passes18,315
indexed/full-scan probes and all original-room scenarios, log
`/tmp/partner-fall-original-final.log`. Extended homing verifier passes64
instructions/12 constants; ground, direction, fall, jump, lifetime and front-wall
verifiers pass against the read-only ISO.695 document links and diff check pass.
One intermediate Python visibility test failed during concurrent map changes;
the final full run passes after that concurrent fix. Door/save/message work was
preserved and excluded from the staged partner hunks.

Isolated Azahar final capture passes with15-second startup, original converted
harbor geometry and `tests/azahar_partner_harbor_fall.py`, under ignored
`artifacts/azahar-partner-harbor-final/`; log `/tmp/partner-fall-final-smoke.log`.
The input script samples the short fall window and then reveals Goombella by
moving Mario sideways. Inspected `partner-dock-side-view.png` confirms both
characters grounded on the dock. Earlier straight-line pictures hid Goombella
behind Mario; temporary isolated tracing proved minimumY0.15 and zero resets.
All temporary instrumentation was removed before the final build. A reproducible
native fixture generator is `tools/make_partner_harbor_fixture.py`; no original
save/ISO or compiled asset is committed. All profiles/saves are isolated; Xvfb
needed display-socket escalation and the user's emulator state was untouched.

Limitations/next: this fixes the reproduced dry harbor descent using source
physics and distance rules, not full instruction-exact party locomotion. Native
vector history, exact smoothing-call multiplicity/trig/rounding, ray/cache/flag
ownership, forced/fly/ability modes and dynamic surfaces remain adapters. Bind
full80-record ground homing, partner wall services and placement3/4 (+37 for fall
recovery), then compare original Dolphin jump/fall/cadence and partner distances.
No new Dolphin observations or hardware claim; full prologue/game fidelity remains
incomplete. Original art and existing original sound assets were retained.

## 2026-10-13 — Stretch bubble tails to the visible speaker

User requested a reaching tail rather than the original fixed window-relative
length. Native story drawing now extends the tail from its existing bubble
base to four screen pixels before the projected talk anchor. Original sheared
rows, base/width, placement and suppression gates remain; too-close anchors
suppress the tail. White/shadow geometry independently compensates the original
48x64 masks' transparent margins, so both last opaque texel centers reach the
same visible endpoint and the shadow does not overshoot the head. Original
raster assets, IDs/raw dialogue/controls/timing and4:3 proportions are unchanged.
The four-pixel gap is an authored native adaptation, not measured original
behavior. See[native contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md)
and[asset conversion notes](ASSETS.md).

Validation: `make test` passes **96 CTest / 96 Python tests**
(`/tmp/tail-stretch-test.log`); `make all` passes without warnings
(`/tmp/tail-stretch-build.log`). Original-data20-payload playback and six-speaker
harbor integration tests pass. Added left/right/inverted stretch checks for
retained base, four-pixel gap, white/shadow endpoint equality and close-anchor
suppression. Document audit checks694 Markdown files with0 broken links;
`git diff --check` passes.

Isolated `tests/azahar_textboxes.py` capture with the native story3 fixture and
immutable `artifacts/ttyd-tail-stretched.3dsx` completes and saves story4 under
`artifacts/azahar-tail-stretched/`. Reviewed Goombella page08 and X-Naut page14:
the visible tails now extend from the bubble toward their heads. No diagnostic
marker remains; user emulator profiles/saves and original ISO are untouched.

Limits/next: this tail extension follows the user's presentation preference,
not original fixed-endpoint parity. Pose/image-derived talk anchors and
handheld aspect remain adapters. Original NPC heights/projection, full window
services/indicator and matched Dolphin/hardware verification remain open.
Broader dialogue/story/full-game fidelity remains incomplete.


## 2026-10-13 — Save and reload playable interiors

Native PM2D version4 adds the completed inn/professor interior mask while
retaining40-byte records and version1/2/3 loading. Story remains a separate
optional signed word; no original story is inferred or changed. Unknown masks,
room/mask mismatches and reserved bytes reject atomically. Native temporary-file,
backup recovery and checksum behavior is preserved. This is a native format,
not a recovered original card format; [SAVE-01](requests/SAVE-01.md) stays open.
See [format contract](behavior/format/NATIVE_SAVE_FORMAT.md).

Game restore applies saved door collision/display state before floor/wall
validation in the native startup transaction. Completed interiors permit the
existing manual save; in-flight door choreography still suppresses saves. A
room transition resets the previous room's door mask. Original-data tests enter,
save/reload and exit both buildings through their actual close contacts.

Validation: final `make test` passes **96 CTest / 96 Python tests**
(`/tmp/indoor-save-final-tests.log`); `make all` builds native3DSX
(`/tmp/indoor-save-final-build.log`); `build-host/field_door_tests original`
passes both source rooms after save/reload. New native-save tests cover old
versions, invalid masks/reserved bytes, actual version3 file loading, version4
writing/loading and backup recovery without mutating decoded state on failure.
Document audit checks694 Markdown files with0 broken links; diff whitespace
checks pass. Isolated Azahar `tests/azahar_indoor_save.py` asserts version4,
room0/interior1 in the actual saved file. A second launch with that isolated
save reloads inside the original inn; reviewed
`artifacts/azahar-indoor-save/boot.png`. User profiles/saves remain untouched.
Original assets/packs remain local and ignored. Validation runs on the current
shared worktree; concurrent partner/message changes are kept separate from
this incremental save commit.

Next: implement the original professor story8 first-talk with recovered
ordered US message keys (party print expands to `_kur`), original Frankly
`c_kuri_h` art/poses and source camera/actor chronology. Source `dkey` parser/
playback behavior is documented but not yet bound; `col` is already supported.
Then implement story9 close/wall-removal and its story10 lecture dispatch, not a
fabricated terminal story state. Door animation/light/audio, indoor NPC/item
activation and upstairs doors remain unfinished, as do the broader prologue
arrival/tutorial/story transitions and hardware performance. Goal stays active.


## 2026-10-13 — Correct tail direction and preserve original4:3 bubble proportions

Fixed the observed Goombella tail miss in79c15b1. Tail masks now draw through
source sheared quads (two textured triangles per pass), retaining horizontal
base/end edges and avoiding rotated transparent-margin expansion. A dedicated
12-vertex buffer preserves the separate shadow/white submissions. Flush and
restore citro2d around the tail GPU pass. Bubble/glyph scaling is uniformly0.5,
fitting original640x480 presentation into centered320x240 with40-pixel margins
on the400-wide top display. World drawing still fills the wide viewport.

Speaker projection now reuses the actual tilted world transform and converts
its clip coordinates to the 2D screen plane. Goombella talk position uses her
evaluated original pose bounds; Crump/X-Naut use opaque bitmap top/center
instead of padded billboard tops. Original print-entry roots/placement caching,
speaker/message attribution/order and control playback remain intact. Ignored
PMHNPC2 actor metadata stores the new offsets; source paths/conversion/rights
are recorded in[assets](ASSETS.md). See the updated
[native bubble contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md).

Validation: latest shared `make test` passes **95 CTest / 96 Python tests**
(`/tmp/tail-fix-test.log`); final production `make all` passes without warnings
(`/tmp/tail-fix-final-build.log`). Original-data playback20-payload and harbor
six-speaker integration tests pass. Added steep left/right/inverted tail cases
verify horizontal edges, width, mask coordinates and base-to-speaker direction.
Document audit checks693 Markdown files with0 broken links; diff check passes.
An earlier shared run failed the concurrently changing partner core check.
A clean archived baseline plus the tail host changes independently reproduced
partner/core failures (`/tmp/tail-fix-isolated-test.log`); message tests passed.
The later shared run after partner/test updates is the passing full validation.

Isolated captures in `artifacts/azahar-tail-fix-aspect/` complete story4.
`artifacts/azahar-tail-anchor-diagnostic/` temporarily marks the talk anchor:
reviewed Goombella's marker on her visible head and the white mask centerline
aiming there. Original shadow artwork extends beyond the white mask. The marker
was removed before the final production build. Final marker-free captures use
`tests/azahar_textboxes.py`, native fixture and immutable
`artifacts/ttyd-tail-fixed-final.3dsx` under `artifacts/azahar-tail-fixed-final/`;
script checks story4 persistence and overlay restoration. User emulator profiles
and saves are untouched; ISO input stays read-only and packs/images ignored.

Limits/next: pose/image-derived anchors are native adaptations; original NPC
tribe height+150 and original getScreenPoint services remain unbound. Original
shared RNG, full window types/indicator/history/selection/icon/pose/paper/audio/
noise and matched Dolphin/hardware comparison remain open. Check additional
actors/cameras as more story content is bound. Full dialogue/full-game fidelity
is not complete.


## 2026-10-13 — Original same-map inn and professor door access

Bound the plaza inn/bar ground-floor and east professor house as original
same-map doors, preserving the five existing room IDs. The documentation
agent delivered [door evidence](behavior/overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md)
and appended the [request inbox](requests/OW-ROAD-01.md). Source admission uses
registered forward hit identity plus A and ordinary motion/common case gates,
with no invented radial door trigger. Extracted first hit-triangle normals and
original hit centers produce40-unit-offset, EVT-integer destinations. The native
fixed-tick bridge uses the original135-unit/sec request and500ms waits; it
consumes A before the native jump harness and preserves the story word.

Original inner subtrees start hidden and are revealed on entry. The professor
interior collision subtree enables/disables across wall, floor, overhead and
shadow queries, with a cached mask change rather than per-frame asset rebuilds.
Inn's original null collision field stays null. Source geometry is retained;
outer subtrees use an explicit provisional indoor cutaway until the original
flag2/0x800 fade consumers are documented. See [conversion/licensing](ASSETS.md).
The ignored doors.pmdoor pack and doors.json manifest contain only locally
converted original data; neither the ISO nor extracted assets are committed.

Validation: final `make test` passes **95 CTest / 96 Python tests**
(`/tmp/interior-final-tests.log`); final `make all` builds the native3DSX
(`/tmp/interior-final-all.log`). `build-host/field_door_tests original` tests
actual original contact admission, both buildings' entry/exit, unchanged story,
and interior-save suppression. Geometry partition tests preserve all vertex
records and byte-identical collision packs. Document link audit checks693
Markdown files with0 broken links; working/index diff whitespace checks pass.

An initial default-binary Azahar run displayed a generic load error and is NOT
counted as a gameplay pass. An isolated build then loaded and revealed an
exterior-wall occlusion problem; the explicit cutaway fixes it. Reviewed
`artifacts/azahar-inn-cutaway/inn-inside.png` and `inn-exited.png` under the
isolated profile: original bar interior, visible Mario/Goombella and restored
exterior on exit. Professor access is original-data host tested, not yet visually
compared against Dolphin. Captured cutaway build precedes the final central save
suppression guard; final `make all` and tests include that guard. User emulator
profiles/saves remain untouched. Concurrent partner/message refinements remain
separate working-tree work; this increment stages only its own changes.

Limits/next: original door rotation/light/dark/sound/letterbox services, indoor
NPCs/items, inn upstairs callbacks, professor story8 first-talk and story9 wall
removal are not executed yet. Camera and relative partner movement are explicit
native adapters. Interior saves are suppressed to preserve the existing outside
save until a versioned door-state extension is implemented. Next bind those
services and original dialogue, expand door registration to the other source
records and verify professor captures. Harbor arrival/tutorial/plaza/east story
transitions and hardware performance remain incomplete; the goal stays active.


## 2026-10-13 — Original-style speaker bubbles and dialogue overlay suppression

The harbor checkpoint now renders the original `f/msgWindow.tpl` ordinary
border/shadow/tail masks, with source default(-280,225,560,176) geometry adapted
to the400x240 top display. Print-entry speaker roots and cached left/right/
offscreen placement use the recovered contracts; source tail endpoints aim
at the attributed speaker through the current native camera. Bubble opening/
closing interpolates from the speaker and ordinary anchored glyphs draw at
full opacity. See [native contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md)
and [local asset conversion/licensing](ASSETS.md).

Expanded parser controls with pos, scaleX, wave, col and closing controls,
small, clear and wpos. Existing key/page/wait/speed/scale/dynamic/shake tokens
and original ID/raw-byte preservation remain intact. Shake is now rendered;
wave uses unaccelerated simulation-clock time and dynamic follows original
draw-time relaxation. Partial glyph UV cropping keeps enlarged effects visible
at the window boundary without leaking GPU scissor state. Ordinary source
vertical admission precedes effects; clipped dynamic records still relax.
Unsupported service controls reject atomically instead of dropping tokens.

Top HP/location/interaction prompts and bottom menu/captions/buttons/notices/
FPS disappear for ordinary dialogue and the entire locked harbor scene.
A/B/touch still advance the original scene, and overlays restore on completion.
Development notices use the original ordinary border without fabricated NPC
attribution. No original dialogue was authored or rewritten.

Validation: final `make test` passes **94 CTest / 96 Python tests**
(`/tmp/textbox-test.log`); final `make all` builds3DSX without warnings
(`/tmp/textbox-build.log`). Original-data `message_playback_tests original`
parses/plays all20 opening payloads; `harbor_story_tests original` passes six
source speakers/order/positions and story4 completion. Source contract verifier
passes (`/tmp/textbox-source-contracts.log`); document audit checks691 Markdown
files with0 broken links; `git diff --check` passes. Tests include control layout,
rollback, source geometry/selector/equality/offscreen/tail gates, partial clipping
and speaker snapshots. An initial new test incorrectly assumed right-side
placement implied an endpoint on the right of its base; corrected it to check
the recovered base-to-speaker interval. The initial assertion failure is not
counted as a passing validation.

Final isolated Azahar capture runs `tests/azahar_textboxes.py` using the native
story3 fixture under `artifacts/azahar-textboxes-final/`. Reviewed opening,
Crump page/Goombella responses, and final story4 overlay restoration; the script
checks saved story4. The earlier rapid walkthrough also passes under
`artifacts/azahar-textboxes/`. Sandbox Xvfb first failed display-socket access;
the approved isolated run succeeds. User emulator profiles/saves are untouched.
Original ISO/extracted raster assets/packs remain read-only-source/local/ignored.

Limits/next: native eye/aim/FOV projection, Goombella0.55 height, extracted
Crump/X-Naut billboard tops and separate shake RNG are explicit adapters,
not measured source behavior. The tail uses a rotated original mask rather
than the exact sheared GX quad. Complete NPC tribe heights/getScreenPoint,
original shared RNG, full window types/history/selection/icon/pose/paper/audio/
noise, original star indicator and matched Dolphin image comparison remain
open. Arrival/tutorial/full-game story services and hardware performance remain
unfinished. Next bind those consumers and compare original captures; do not
claim complete dialogue or game equivalence from this slice.


## 2026-10-13 — Combined dialogue-window support and isolated validation

Concurrent work committed the harbor/font foundation as48d8c73 and extended
the shared message renderer while this session was preparing its commit.
ff225fa picked up that extension; bd4abf8 adds its required
`message_window.hpp` and `message_window_assets.py` support files, so the
committed dependency set is complete. Original ordinary bubble/tail masks now
replace the plain native panel. Source-derived anchor/box/tail behavior and
additional retained style controls improve presentation; specialized window,
NPC anchor/pose and original shared RNG/material fidelity remain unfinished.

Validation of the combined snapshot: **94 CTest / 96 Python passing**,
original20-payload and six-speaker harbor integration checks pass. A shared
native build raced another linker and produced an invalid-ELF packaging error;
isolated `make BUILD=build-harbor-verify
TARGET=artifacts/ttyd-harbor-verified all` passes. The immutable output at
`artifacts/ttyd-harbor-verified.3dsx` completes the original story3 checkpoint
and saves story4 in `artifacts/azahar-harbor-combined-verified/`; manually
inspected first-dialogue image shows original font/bubble masks. New
`emulator_smoke.py --binary` supports such isolated build verification without
replacing another workstream's artifact or touching user emulator profiles.
Current concurrent uncommitted refinements are separate from that validated
snapshot. Next: validate those refinements, then continue arrival/tutorial,
original NPC/event/window services and indoor access. Full prologue parity
remains incomplete.

## 2026-10-12 — Original harbor dialogue checkpoint and US font

Added a source-derived story3 harbor sequencer for original mac_0_016..021,
with the recovered position gate (integer decoded Z<=-100), exact message IDs/
raw payloads/speaker order, independent delayed actor moves, source camera
requests and terminal story4 write. It is enabled only by explicit native
originalStory=3 in harbor; legacy/new harness state does not invent a story
checkpoint. The gate has no added A/grounded condition, and neutral field
physics continues an existing jump while the scene blocks new player actions.

Opening glyph/key/page/wait/speed/scale/dynamic records and finite ordinary
window fade/acceleration/scroll behavior are portable. Original US BFN glyph
masks, mappings, width patches and row offsets render through citro2d, retaining
all original message bytes/tokens. Speaker captions are presentation labels,
not rewritten dialogue. Original c_kanbu_ibt/c_zako_n setup art and the exact
BGM_EVT_DANGER4 stream are packaged locally; see
[asset provenance](assets/SCRIPTS_MESSAGES_EFFECTS.md) and
[source choreography](behavior/story/STORY_HARBOR_OPENING_BINDINGS.md).

This remains a development checkpoint, not complete scene/REL VM fidelity:
NPC numeric stepping/collision/automatic poses use a documented temporary
speed adapter; Crump/subordinates use original static setup art. Mario
set_dir/reardir, nested key/GSF persistence, environmental/NPC/text SFX, crowd,
original bubble/history/selection/paper/shake/noise services remain unbound.
Story2 arrival/resume and story4 tutorial continuation/restore are unfinished.
Fresh launch remains the plaza harness. No new fabricated dialogue was added.

Native checks parse/play all20 opening payloads with their exact source font
and retained bytes; integration checks the six speaker/ID order, position-only
and airborne admission, independent movement/owner completion, save suppression
and story3→4 boundary. Isolated Azahar captured original font/dialogue and
persisted story4. The first test checkpoint was on water and lost1 HP; the
final helper places it on independently probed dry harbor ground. Source camera
and NPC timing are static-derived/adapted, not Dolphin observations.

Validation: `make test` **93 CTest / 96 Python passing**, `make all` builds
3DSX without warnings; original-data `message_playback_tests original` and
`harbor_story_tests original` pass. `check_doc_links.py` checks690 files with
0 broken links. Final isolated checkpoint capture is
`artifacts/azahar-harbor-story3-physics/`, driven by
`tests/azahar_harbor_story.py`; final story4 persistence passes. An airborne
regression test first exposed a missing air-motion dispatch in the dialogue
physics bridge; adding that ordinary dispatch lets the jump land while the
original key wait remains active. User emulator profiles/saves were untouched.
No original-runtime or hardware performance observation was made.

Next: bind original arrival and tutorial continuation, full message/window/NPC
services, original event runtime composition and indoor entrances/maps. Full
prologue parity and hardware performance remain incomplete.


## 2026-10-12 — Partner 3D following, jumps, falls and wall/path recovery

Recovered selected missing `party_motion_homing_walk` branches directly from
read-only US G8ME01: grounded history flag1 admits jump preparation, repeated
wall contacts use a signed byte counter greater than8, and ground homing calls
screen recovery. Added pinned body identity/26 instruction anchors and
[native contract with explicit fidelity boundaries](behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md)
in `432293c`. Independent existing source verifiers still pass.

Replaced the ground-only XZ follower in `55d00bb` with portable 3D history,
independent ground/jump/fall state, source-derived ordinary jump initialization
and fall coefficients, swept wall/axis sliding, supported stale-path pruning,
and screen candidate recovery. The partner can traverse air without floor,
land independently, and recover without the old queue-size teleport. Original
`PKR_J_1A` now joins the existing original Goombella pack; provenance is in
[assets](ASSETS.md). Native motion uses actual post-collision Mario output.
Refinement retains stationary-hop spacing, continues over gaps, handles a jump
behind dense short trail marks and caps append pressure without teleporting.
Concurrent harbor/message work was preserved and excluded from partner commits.

Validation: final `make test` passes **93 CTest / 96 Python tests** in
`/tmp/partner-validation-test.log`; `make all` builds3DSX without warnings in
`/tmp/partner-validation-build.log`. `build-host/core_tests --original` passes
all original-room scenarios and **18,315** indexed/full-scan collision probes
(`/tmp/partner-original-room.log`), including partner stair support. New partner
scenarios cover stationary/dense-history jumps, retained spacing, gap crossing,
ledge fall, thin walls, stale path, persistent wall/capacity nonteleport, screen
recovery and Game binding. Homing/jump/fall/recovery/history static verifiers
pass;690 document links and `git diff --check` pass.

Initial full runs exposed a concurrent harbor test failure, independently
reproduced with pre-partner Game sources under `/tmp/partner-preexisting-check`;
that concurrent work subsequently resolved it. One rebuild hit a transient
unrelated field_battle_tests link failure; targeted relinking and the final
full run pass. Neither intermediate run is claimed as a passing validation.

Isolated Azahar movement/jump/save capture passes with15-second startup and
`tests/azahar_partner_follow.py`. Final files are in ignored
`artifacts/azahar-partner-follow-spacing/`, log `/tmp/partner-smoke-final.log`.
Inspected `partner-air-early.png` and `partner-follow-landed.png`: Goombella
uses her original air pose and returns to the floor with following space.
Earlier captures exposed an overlapping stationary landing, fixed and covered
by a regression. Xvfb required display-socket escalation; all profiles and
saves are project-isolated. No user's profile/save was modified.

Limitations/next: this implements equivalent native behavior categories,
not full instruction-exact party locomotion. Native vector scheduling, scale1,
terrain landing/sweeps and path lookahead remain adapters. Complete original
homing targets/ring indices, five-ray partner floors/front walls, near-Mario
placement3, fall placement4/+37, dynamic platforms, ability modes and partner
sound playback remain unbound. Record original Dolphin jump/ledge/wall/warp
trajectories before claiming measured parity; see [mechanics](MECHANICS.md).
Hardware behavior and full prologue/full-game fidelity remain incomplete.


## 2026-10-12 — Prologue map object visibility from original events

Implemented recovered ordinary-entry visibility for harbor ship subtrees and
wakes, east interior/gate and west alternate foliage in commit `8c6c5fa`.
The pirate ship group `S_fune_c` is hidden in prologue states2..6; original
init reveals it only for172..229 except the earlier separate217 case.
The later `S_fune_b` group uses>=268 except402/404. Other initialized-hidden
ship/wake groups remain hidden until their scripted overrides are bound.
See [visibility contract](behavior/field/ROGUEPORT_STORY_OBJECT_VISIBILITY.md)
and [asset provenance](ASSETS.md). Source subtrees inherit visibility into
children without affecting siblings; batches split by event group, retaining
geometry for subsequent story states. Both drawing passes use the same policy.
Missing native story uses a conservative story0 rendering envelope without
changing persistent story storage. Pre-existing concurrent prologue work was
preserved and excluded from this feature commit.

Validation: `make test` passes **92 CTest / 96 Python tests** (log
`/tmp/pm-visibility-test.log`); final `make all` builds3DSX without warnings
(`/tmp/pm-visibility-build.log`). New tests cover source geometry partition,
prologue subtree omission, unchanged collision packs, metadata fields and
story boundaries/special cases. `check_doc_links.py` checks689 files with0
broken links; `git diff --check` passes. Isolated Azahar captures use synthetic
native story3/6 saves at harbor(0,-2), with18/15-second startup windows;
`artifacts/azahar-prologue-visibility/boot.png` shows source dialogue and
`artifacts/azahar-prologue-visibility-6/boot.png` shows unobstructed dock/water.
The initial5-second capture was too early (black); no visual pass was claimed
for it. Xvfb needed display-socket escalation. All profiles/saves are isolated.

Limitations/next: this is recovered map initialization, not complete event VM
activation. Bind story2 arrival, ship motion/departure/wake overrides, remaining
plaza/NPC/item/door conditions and original collision activation; verify scene
transitions against Dolphin. No original-runtime observation or hardware
performance claim. Full prologue/full-game fidelity remains incomplete.

## 2026-10-12 — Goombella original animations, visibility and facing correction

Field Goombella now renders original `PKR_S_1`/`PKR_W_1`/`PKR_R_1` idle,
walk and run clips from `a/c_pkr` + `a/c_pkr-`, using original scalar2 and a
separate texture atlas. Static zero-span `PKR_Z_1` is also packaged for setup.
PMANIM3 adds original visibility bases, group indices and sparse full-key
modulo256 updates. Hidden ancestors suppress descendants while preserving
siblings; shapes hidden at frame0 remain available for later keys. Corrected
the offline bake's old `delta==1` assignment. PMANIM1/2 remain readable.
Source clips replace her field billboard/procedural bob. See
[asset provenance](assets/MARIO_AND_POSES.md) and
[Goombella binding](behavior/partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md).

The user caught reversed travel-facing in the initial native adapter. Replaced
that snap mapping with the recovered ordinary `partyMakeDispDir` target ranges,
four interpolation/latch branches and wrapped Z-reflection. Positive X uses
source heading90 / target180; negative X targets0. Original moving pose
selection keeps partner speed, owner scale1 and Mario stick class distinct.
Native displacement still adapts source speed/heading; original homing, special
turn flags, sound helper, HP/dot-table and event/battle poses remain unbound.
This is animation progress, not completed partner/story fidelity.

Validation: `make test` **90 CTest / 93 Python passing**; `make all` builds
3DSX without warnings. Independent original-data comparison passes **39
Goombella samples**, max model error5.53811e-6, and **251 Mario samples**,
max error3.05176e-5, including texture/visibility boundaries. Synthetic tests
cover byte wrapping, hidden descendants/siblings/cycles, rewind, invalid pack
rollback, zero-duration poses and both facing endpoints. Source checks pass
47 scale anchors/13 DOL spans/2 converter models and515 partner instructions/
32 constants/7 spans;688 document links pass. Refreshed a previously stale
converter-derived Mario numeric baseline without changing source fingerprints.

An initial isolated west-road capture passed movement/jump/save. A later short
hold failed its0.1-unit distance assertion (actual0.072); the longer dedicated
`azahar_partner_animation.py` capture passed, saving room0 X=-3.20559 from
-5.05. These are software-emulator fixture checks, not measured original timing
or hardware performance. Final facing capture under `artifacts/azahar-partner-facing`
passes the expanded right/left movement/jump/save script; manually inspected
`partner-run-later.png` and `partner-idle-left.png` confirm correct facing in
both directions. All emulator profiles/saves are isolated from the user's profile.

Next: native original message parser/window playback, harbor REL scene
activation and indoor doors/maps; remaining partner motion/poses and prologue
visibility conditions. The playable prologue / original-game parity goal
remains incomplete.

## 2026-10-12 — Harbor position gates and original Mario position callback

Documentation cross-check recovered original story3's integer Z<=-100 gate,
and story4's nested Z<=-100 / X>=-25 gate, with original message IDs, speakers
and control-token inventory. Added `fieldMarioGetPosition` with callback-registry
binding at US0x800e71f8 and `harborPositionGate`; both compose with the existing
event operand services, preserving 1/1024 float writes and integer truncation.
Independently re-disassembled the complete US callback: argument pointer is
captured before marioGetPtr, Mario pointer before mode resolution, and each
destination/position write remains ordered. All nonzero modes subtract1 from Y.
See [harbor bindings](behavior/story/STORY_HARBOR_OPENING_BINDINGS.md) and
[inbox breadcrumb](requests/OW-INTERACT-01.md).

Validation: `make test` **89 CTest / 92 Python passing**, `make all` builds
3DSX; `verify_field_lecture_callbacks.py` verifies15 bodies/54 instruction
anchors/6 constants/10 imports; `verify_behavior_contracts.py` passes;
`check_doc_links.py` checks687 files with0 broken links. Tests cover fractional
gate boundaries, mode behavior, quantization, pointer capture, ordered field
reads and registry dispatch. No emulator run: these portable services are not
yet invoked by `Game`, so current development notices/trigger remain unchanged.

Next: original message parser/window presentation, REL harbor scene activation,
NPC poses/choreography and indoor entrances. Goombella's PKR_Z_1 pack probe
encountered original animated visibility, which PMANIM2 explicitly rejects;
implement that domain before replacing its baked idle image. The playable
prologue / original-game parity goal remains incomplete.

## 2026-10-12 — Native original US message library and resource lookup

Imported all 260 `msg/US` resources / 13,186 key/text pairs byte for byte
into ignored `romfs/messages/`, with hashes, source offsets and licensing in
its local manifest. `make all` now packages this library. Portable
`MessageResource` retains original IDs/control tokens/high bytes, builds an
offset index, and atomically rejects invalid files. `MessageAssets` provides
room-before-global lookup. Missing/ambiguous keys deliberately return null;
original fallback and duplicate-key qsort behavior remain unbound. See
[asset provenance](assets/SCRIPTS_MESSAGES_EFFECTS.md).

Validation: `make test` **88 CTest / 92 Python passing**, `make all` builds
3DSX, `build-host/message_assets_tests romfs/messages/gor_00.txt` verifies
328 actual harbor pairs and all 20 opening IDs. No story playback or emulator
behavior changed in this increment. Next: original-token presentation and
story-call bindings, position-triggered harbor scenes, indoor entrances.
The playable prologue / original-game parity goal remains incomplete.

## 2026-10-12 — Native cross-check fixes: sink guard + default lanes

The native session's independent re-read falsified my water doc's sink
guard direction ([124]==0.0 initializes −6.0, not !=; fixed and credited)
and flagged the undocumented r4-sourced default fall lanes — added to the
bottomless doc ([r13−28176]+16 vs absolute 0x80416208+16 by [46]==5,
verified by direct reads). Both fixes verified against the listings. This
is the inbox cross-check loop working as designed: independent re-reads
catch single-bit errors that self-review missed.

Validation: direct-instruction re-checks, diff --check clean. Docs-only.
Next: triangle interiors, pose domains. Full-game incomplete.

## 2026-10-12 — HitVec region-graph correction (false asymmetry withdrawn)

Re-verified the traversal doc's region claims against the listing and found
my own region-1 asymmetry finding was backwards: R1 allocates counter=r30 /
pointer=r31 (inits `lwz 31,172` / `li 30,0`), regions 2–4 the reverse — same
loop semantics, no executability consequence. Withdrew the pointer-size
claim; kept the verified cascade edges (R1-hit→R2/R3-gateway, drains) and
the airtight R4-unreachable finding (no entry edge, dead fallthrough).
Two crashed verification scripts produced zero passing asserts this turn —
all claims above are re-verified by direct reads instead.

Validation: direct-instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains. Incomplete.

## 2026-10-12 — Water/void recovery: apply recovered bottomless drop-back lanes

Reviewed the
[water](behavior/overworld/OVERWORLD_WATER_RECOVERY_NUMERICS.md) and
[bottomless](behavior/overworld/OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS.md)
numerics deliverables against the `WaterRecovery` prototype. Independently
re-read the ISO DOL and confirmed the mot_bottomless drop-back lanes
(`0x80091bd4`: `[124]/[128]/[132]/[136] = 0.34/-0.002/-0.01/0.0`), so replaced
the prototype's authored `-.001/-.002/+.00024/-.00004` cascade (which matched
only `-0.002`) and pinned it in `tests/field_camera_tests.cpp`. Did **not**
apply the `-6.0` sink velocity, the `0.08` travel step or the `state==15`
handling: the deliverable leaves them unestablished, the `0.08` constant has no
SDA source, there is no original stage-15 slot, and the prologue guard reads as
`[Mario+124]==0.0` on the DOL (the contract states `!= 0.0`). The prototype
still omits the `[Mario+552]`-mismatch condition and the `r4` default lanes.
Breadcrumb: `requests/FIELD-RECOVERY-01.md`.

Validation: `make test` 87 CTest / 90 Python pass, `make all` 3DSX pass,
warning-free compile of the changed TU. Docs mid-restructure (inbox split into
`requests/`); breadcrumbs went to the per-request file. Full-game incomplete.

## 2026-10-12 — Merge the overworld request inbox into docs/requests

Migrated `OVERWORLD_DOCUMENTATION_REQUESTS.md` into the standard inbox style:
seven per-request files under `requests/` (OW-MOVE-01, OW-WALL-01,
OW-SUPPORT-01, OW-PARTNER-01, OW-ROAD-01, OW-INTERACT-01, OW-CAMERA-01), each
with a status line, an Open-todos checklist, verbatim request text and an
append-only answer/delivery history. The seven rows join the status table in
`DOCUMENTATION_REQUESTS.md`, which also absorbs the story-integrity and
static/runtime rules that headed the separate file; the "tracked separately"
pointer is gone. Cross-links in `behavior/overworld/OVERWORLD_MARIO_MOVEMENT.md`
and `behavior/battle/BATTLE_HIT_VEC_TRAVERSAL_ORDER.md` now target the
per-request files.

Validation: `python3 tools/check_doc_links.py` (686 markdown files, 0 broken)
plus a line-by-line comparison of the old file against the new files — every
original line survives; only heading levels, the three header provenance lines
and the relative link prefixes differ. Docs-only, no make rerun. Limitation:
overworld deliveries stay recorded in each request's history and are not
back-filled as `requests/DELIVERY_LOG.md` rows, because the original inbox
never named a numeric documentation-agent ID for them. Next: answer the OW-*
open todos and register new deliveries/locks in the delivery log.
Full-game incomplete.

## 2026-10-12 — EVT-01 accessors, start wrappers, Monosiri, and an address correction (ID 3)

Second EVT-01 pass as documentation agent ID 3, continuing the inbox queue.

**A correction that outranks the new work.** I had published three data
bases computed **without the sign extension `lis` performs**, so all three
were wrong by `0x3B0000`:

| Published earlier | Correct | Symbol now confirmed |
| --- | --- | --- |
| `0x8002F988` | **`0x803DF988`** | `pssfx` (`.bss`, 1600 B = **40 slots** x 40) |
| `0x80016B28` | **`0x80316B28`** | `pssfxlist` (`.data`, 84840 B) |
| `0x8004CF40` | **`0x8036CF40`** | `monosiriMessageTable` (`.data`, 5544 B = **exactly 231 records** x 24) |

The symbol names and sizes are the decisive part: `1600/40` and
`5544/24` both divide exactly, which independently confirms both strides
and turns the slot count (40) and table extent (231) into facts rather than
guesses.

The same error had reached seven field-collision contracts from the previous
session, where `lfs` float constants were given as `0x800494xx`. Those were
internally inconsistent with each other, which is what exposed them. All are
now in the authoritative `N(r2)` form, read from the instruction bytes
(a contiguous pool from `-27544` to `-27380`), and the `lis`-derived globals
correct to `0x80343B78` / `0x80343D88`, both resolving into `.data`. A
correction note heads each affected document.

I also **reverted one of my own "corrections"**: the three values
`0x80091434` / `0x80091504` / `0x800915e4` are not debug strings but the
symbol entry points of `chkfilterVecVivian`, `chkfilterVecRoll` and
`chkfilterVec`, passed to `hitCheckVecFilter` as filter labels. Two were
already right in the original text; I had briefly "fixed" them wrongly.

**New complete bodies**, all in `docs/behavior/audio/`:

- [Slot accessors](behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) —
  `psndSFX_get_vol`, `psndSFXChk`, `psndSFX_dist`, `psndSFX_pit`,
  `psndSFX_vol`. Completes the accessor family. Key finding: the handle
  packs **slot index in bits 0..7 and channel allocation in bits 8..23**,
  validated against `slot+0x04` with a `-1` return. Also `stb`/`sth`
  truncation with no clamp, and the local `slot+0x14` vs `SoundEfxSetPitch`
  pitch split gated on the same low-5 bits `psndSFXOff` uses.
- [Start wrappers](behavior/audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md) —
  `psndSFXOn`, `psndSFXOnEx_3D`, `psndSFXOn_3D`, plus the **derived
  eight-argument `__psndSFXOn` ABI**. `arg5` and `arg7` are always `0`;
  `psndSFXOn_3D` silently discards the caller's `r5`.
- [Monosiri lookup](behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md) —
  `battleGetUnitMonosiriPtr` 8012ef20, the item EVT-01 called
  unclaimed. Pure signed-multiply address computation with no bounds,
  no negative-kind check and no null return. Noted that the caller's
  `== null` test is therefore reading a **record field**, not a failure.

Also noteworthy: **40 slots exist but the handle masks with `& 0xFF`**, so
indices 0..255 are all reachable and nothing bounds them.

Updated [behavior/README](behavior/README.md) (its rows were overwritten
twice by concurrent commits and were re-added each time), appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md), marked the Monosiri todo
answered with the residual work, and registered three validated delivery
rows.

Validation: bodies re-disassembled per function with ELFs in `/tmp` only;
every callee resolved against `us_symbols.csv`; `lis`/`addi`/`addis`
arithmetic re-derived in Python and cross-checked against named symbols;
repo-wide sweep for the four wrong address patterns returns clean;
`git diff --check` clean; link audits clean. Docs-only, so `make test` /
`make all` were not rerun. Full-game incomplete.

**Process lesson worth keeping:** the fabricated addresses survived a
commit and a README row because they were plausible-looking. The two
cross-checks that caught them were (a) does the address land in the
section the symbol table says it should, and (b) do claimed strides divide
the claimed symbol sizes evenly. Both are cheap and should be applied to
any future address claim before it is written down.

Next tasks:

1. **`__psndSFXOn` 800d9934 (1344 bytes)** — now the only writer of
   `pssfx` fields `+0x00`/`+0x04`/`+0x06`/`+0x18` and the place the
   handle is packed. With `pssfx`/`pssfxlist` named and the layout fixed,
   this is the last structural gap in the audio chain.
2. **`psndSFXMain` (2964 bytes)** — reader of volume, distance and the
   published position; closes the update/stop loop.
3. Monosiri record layout beyond `+0x04`, and its table writers
   (`battleAcMain_MonosiriTarget`, the `monosiriExit` call at `8017225c`).
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` is a contended file: three concurrent commits
   have dropped rows from it. Worth an owner check before it is treated as
   a reliable index.

## 2026-10-12 — `__psndSFXOn` slot provisioning decoded (ID 3)

Third EVT-01 pass as documentation agent ID 3, taking the top item from
the previous queue: `__psndSFXOn` (`0x800d9934`, 1344 bytes), the last
structural gap in the positional-SFX chain because it is the **only**
writer of the `pssfx` fields the accessors read.

Delivered [POSITIONAL_SFX_SLOT_PROVISIONING](behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md),
decoded end-to-end. Evidence class B; the `make test` / `make all` gates
were not rerun because this is docs-only.

Recovered:

- **The `key` argument is a string.** Name resolution is a `strcmp` scan
  over `pssfxlist` (stride 20, up to **4242** entries) against
  `word0`. Key `0` skips the scan; a scan that runs off the end resolves to
  index 0. There is no not-found failure.
- **Three pre-allocation gates**: global `+0x56` bit 29 as a mute that
  applies only when `arg7 != 0`; a signed-byte **cooldown of 6** at global
  `+0x5c` that rejects while nonzero; and for index 869 an event flag at
  `0xF5DE0180` that must equal 2. Indices 517 and 869 are the only
  per-index specials.
- **Two-stage slot search**: 20 same-key slots first (with a `+8`
  preference cursor at global `+0x54`), then a 40-slot reclaim pass that
  **inlines a full copy of `psndSFXOff`** — same low-5 gate, same
  `key >> 19` table index, same two-bit SoundSS/Song/Efx stop — and
  additionally requires `slot+0x0c == pssfxlist[+0x10]`. Exhaustion
  returns `-1`.
- **Channel bits are a pure function of the pssfxlist index**: base
  `0x101`, plus bits 5/13/14/15 of the index and per-index/per-field
  overrides. This is the routing mechanism.
- **Handle packing confirmed**: `channel = ++global[0x50]`,
  `slot[+0x04] = (u16)channel`, return `slot_index | (channel << 8)`.
  Independently confirms what `psndSFXChk` validates. Recorded asymmetry:
  `slot+0x04` gets the full 16-bit counter but only `<<8` reaches the
  handle, so the two diverge when the counter wraps.
- **Two unreachable volume clamps** at `0x800d9c60`/`0x800d9c70` — the
  `clrlwi ...,24` then `cmplwi ...,255` can never be greater. Recorded as
  observed rather than "fixed" into live clamps.

**One conflict deliberately left open.** `__psndSFXOn` writes `slot+0x00`
as the bare pssfxlist index (0..4241), but `psndSFXOff` and `psndSFX_pit`
derive their pssfxlist row from `(slot+0x00) >> 19`, which is always 0 for
that range. Read literally the accessors always resolve `pssfxlist[0]`.
The bytes are unambiguous (`stw 18, 0(29)`) and no second writer is
visible, so **no reconciliation was invented**. Flagged in the contract and
in the EVT-01 reply as something to settle before either side is relied
on.

**The practical dependency this created:** nothing in `__psndSFXOn`
decrements the cooldown. If `psndSFXMain` is ported without its decrement,
`global[0x5c]` stays nonzero and every later start fails permanently. That
makes `psndSFXMain` (2964 bytes) the single remaining item in the chain.

Registered one validated delivery row and appended the reply to
[requests/EVT-01.md](requests/EVT-01.md).

Next tasks:

1. **`psndSFXMain`** (2964 bytes) — cooldown decrement, preference-cursor
   expiry, and the reader of volume, distance and the published position.
   This is the last item in the positional-SFX chain.
2. Resolve the `slot+0x00` index-versus-packed-key conflict above.
3. Monosiri record layout beyond `+0x04` and its table writers.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, NormalJump
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. `docs/behavior/README.md` remains a contended file; verify rows survive.

## 2026-10-12 — OW-MOVE-01 input remainder: key-data update

Re-extracted marioUpdateKeyData (0x80095cb0, ISO read-only) and contracted
it in behavior/overworld/OVERWORLD_MARIO_KEY_DATA_UPDATE.md: six GP pad
bytes with alternating negation into +594..599, [600/601] zeroing, flag
halfwords, fixed-order per-flag pair zeroing, key-check gate and the
0x803DA398 rising-edge word. No deadzone/clamp exists in-body (native .15
is authored). Reference pair placed under ignored artifacts/.

Validation: instruction re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: triangle interiors, pose domains, deadzone
ownership elsewhere. Full-game incomplete.

## 2026-10-12 — Full-tree gap re-audit plus lightSort alias

Re-ran the name-mention audit over all 664 reference binaries against the
current docs tree: exactly one unnamed file (`lightSort`), verified as alias
of contracted `lightdrv _sort` @8001bccc (three-way float compare plus
`sorter800064fc` caller in behavior/display/MAP_CHARACTER_LIGHT_QUERIES.md)
and recorded in the register. Re-added the plane-motion README row dropped
by the recent restructure. Zero unnamed-and-uncontracted binaries remain.

Validation: audit script, listing-vs-doc structural match, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains,
pipe lifecycle. Full-game incomplete.

## 2026-10-12 — Inbox request: EVT-01 positional SFX chain (ID 3)

Worked the [request inbox](DOCUMENTATION_REQUESTS.md) as documentation
agent **ID 3** (the ID the user assigned; ID 1 and 2 were already taken).
Picked the priority-0 EVT-01 audio chain, which had **no dedicated contract
at all** and was explicitly listed as open by three existing documents.

First surveyed the inbox: `docs/requests/` per-request files plus the
[delivery log](requests/DELIVERY_LOG.md) lock table. Only one lock was
active (ID 2, audience target accumulator), so EVT-01's SFX subtask was
free. Confirmed the manager loop and constructor policies making up the rest
of EVT-01 todo 1 are already complete in `EVENT_MANAGER_COMPLETE_LOOP.md`,
`EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md`,
`EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md` and
`EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md`; the genuinely unclaimed work was
the audio chain.

Delivered three **complete bodies** in `docs/behavior/audio/`:

- [Unit positional SFX trigger](behavior/audio/BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md)
  — `BtlUnit_snd_se_pos` 80126088/114
- [Positional SFX position update](behavior/audio/BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md)
  — `psndSFX_pos` 800d96f8/4c
- [Positional SFX stop](behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md)
  — `psndSFXOff` 800d95f8/c8

New facts, none previously recorded anywhere in the repo:

- The shared **positional-SFX slot record**: base `0x803df988`, **stride
  40**, selected by the **low 8 bits** of the handle. Fields `+0x00` key,
  `+0x06` channel bits, `+0x18` handle, `+0x1c/+0x20/+0x24` position.
- The **channel table** at `0x80316b28`, **stride 20**, indexed by
  `(slot.key >> 19)` — by the key, not the handle. Two bits of `word0`
  select `SoundSSStopCh` / `SoundSongStopCh` / `SoundEfxStop`.
- **Stop-without-stop**: `psndSFXOff` clears the record without calling any
  stop service when `(halfword6 & 0x1F) == 0` or `handle == -1`; and an
  already-free slot returns *without writing*, so a double stop is a true
  no-op rather than a re-clear.
- `psndSFX_pos` writes **exactly three words** and never the key or handle,
  so it cannot start or stop a sound.
- `BtlUnit_snd_se_pos` status 10 **overrides** status 11 when both are set
  (sequential `if`s overwriting one register), and its start id is
  `(int16)(irand(41) + belong + offset - 20)`.
- The volume predicate is `r0 < 0x4D80` unsigned, where
  `r0 = (T & 0xFFFF0000) + 0xE7700000`. For the sentinel the movement-sound
  callers actually pass, `250000000`, this is the **preserve-current-volume**
  path. Derived from the compared words, not an observed trace.
- The wrapper's return is the **handle**, confirming the existing R18 store
  in `BATTLE_MOVEMENT_SOUND_CONTROL`.

Updated [BATTLE_MOVEMENT_SOUND_CONTROL](behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md)
and [OVERWORLD_MARIO_IDLE_SOUND_CUES](behavior/overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md)
to link the new bodies instead of listing them open; no contradiction found
with the existing low-8-handle-bits claim. Registered and then **released**
the lock in the [delivery log](requests/DELIVERY_LOG.md), marked the EVT-01
todo answered with an explicit list of what stays open, and appended the full
reply to [requests/EVT-01.md](requests/EVT-01.md).

Validation: all three bodies disassembled from the read-only ISO with ELFs
written to `/tmp` only; every callee resolved against `us_symbols.csv`;
disassembly re-read against each written instruction block; `git diff
--check` clean; README, cross-link and request-file link audits all clean.
Docs-only, so `make test` / `make all` were not rerun. Full-game incomplete.

Note: while this session ran, commits ec92b10/990b12e/488983a landed from
other work and prepended entries above this one. No conflict; this session's
field-collision entry is intact at its original position.

Next tasks, in the order the inbox implies:

1. **`__psndSFXOn` provisioning** (`0x800d6934`, 1344 bytes) — the slot
   allocation and macro/voice/resource chain that fills the record layout
   recovered above. Now the biggest blocker for the audio chain, and the
   known layout makes it tractable.
2. **`battleGetUnitMonosiriPtr` 8012ef20** — still explicitly unclaimed,
   with no dedicated contract. Small and bounded.
3. **`psndSFXMain`** (2964 bytes) — the reader of the published slot
   position, which closes the update/stop loop from this session.
4. Remaining EVT-01 todos: `evtmgrCmd` opcode bodies, `NormalJump`
   registration writers, `CommandGetWeaponAddress`/`SelectWeapon` cursor
   modes, and the `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose reconciliation.
5. The constant block `0x80049468`..`0x8004950c` from the field-collision
   session is still unextracted.

## 2026-10-12 — Re-extracted placeholders: plane flyMain contract, two aliases

Re-extracted three zero-byte placeholders read-only via
tools/inspect_dol_function.py into behavior/: mot_plane_flyMain (mot_plane.o
flyMain @8009c9b8, contracted in behavior/overworld/OVERWORLD_PLANE_FLIGHT_MOTION.md
with ISO-read SDA set; fixed my own hex/decimal stage confusion pre-commit),
btlevtcmd_AnnounceMessage (matches contracted announcement doc — alias),
PSVECDotProduct (9-instruction leaf contracted in the traversal doc).
Remaining placeholders (_announceMessage/BattleGetFirstAttackTarget/
partyGetOtherPtr/partyGetTargetNum cases) have no DOL text symbols and cannot
be extracted by name.

Validation: extraction outputs verified (ELF identity, entry/size match),
stage immediates re-checked after the hex fix, link audit, diff --check
clean. Docs-only, no make rerun. Next: triangle interiors, pose domains.
Full-game incomplete.

## 2026-10-12 — Fifth gap pass: guard wrapper, act-status publisher, small-data setter and float-to-unsigned

- [Small unit/runtime leaves](behavior/unit/SMALL_UNIT_RUNTIME_AND_MOTION_LEAVES.md):
  `BtlUnit_CanGuardStatus` (801272dc) is a frame-only forwarder to
  `BtlUnit_CanActStatus` (8012724c), so a `bl 801272dc` site is a `bl 8012724c`
  site for call-graph purposes. `btlevtcmd_CheckActStatus` (80105cb4) resolves a
  unit id, calls that same `BtlUnit_CanActStatus`, and publishes a fixed literal
  1 or 0 through `evtSetValue` using the **raw** second operand word as the
  destination. `unk_80151d20` (party_motion.o) is a three-instruction setter
  that writes the constant `60.0` (0x42700000 at 0x8042318c) to the small-data
  slot at `0x8041ec10` and returns. `__cvt_fp2unsigned` (80264860, runtime.c)
  maps `f1 < 0` to 0, `0 <= f1 <= 2^32` to `0xFFFFFFFF`, `f1 > 2^32` to
  `trunc(f1)` and NaN to `0xFFFFFFFF`; the 2^31 constant at `0x80303a30` is
  loaded but dead on every reachable path because the `f1 > 2^31` branch is
  always true once execution passes the 2^32 test.

Register state after this pass: 360 reference listings have no snapshot
definition; 31 of them still have no substantive prose contract. The largest
remaining unaddressed bodies are `seq_battleMain` (253 lines), `partyReInit`
(461), `_getSickStatusParam` (286), `JUTFontSetup` (196),
`FontDrawStringVecPitch` (211), `pouchRemoveItemIndex` (183),
`npcSetupBattleInfo` (166), `vivian_move`/`cloud_move`, the pad readers
(`DEMOPadRead`/`DEMOPadCopy`), `btlevtcmd_ConsumeItemReserve` and
`btlevtcmd_AnimeChangePoseType`. `marioLandOn` and `marioSearchHead` are now
carried by multiple contracts and are no longer counted as open.

Validation: docs-only; `make test` and `make all` were run for the previous pass
and are unchanged by this one (logs `/tmp/pm-gap3-test.log`,
`/tmp/pm-gap3-build.log`). `python3 tools/check_doc_links.py` reports 0 broken
links. No native/test/asset conversion change, hence no runtime, cadence,
emulator or original-execution claim. EVT-01/JUMP-01/DMG-02 and the full-game
requirements remain incomplete.

## 2026-10-12 — Native JUMP-01 correction: packed Mario jump SFX

Verified the JUMP-01 SFX-provenance answer against the read-only ISO and found
it analysed the generic `btlevtcmd_JumpPosition` (0x8010d208/0x4c4), not the
body the native port binds, `btlevtcmd_MarioJumpPosition` (0x801093a8/0x8c0).
Reproduced the Mario body from the DOL (`tools/inspect_dol_function.py`):
r30=0x802ee458, so the only 0x802E reads are the dead 24/28/32(r30)=
0x802EE470/474/478 loads (overwritten by unit floats before the call); one
`psndSFXOn_3D` (0x801098fc) keyed on `word[unit+0x18c]`, position
unit+0x154/158/15c; heading/distance from `system_angleABf` 0x8000703c /
`distABf` 0x8000722c and sine from MSL `sin` 0x80272410. Applied the one
native fix that survives: removed the three dead `0x802ee470` reads in
`source/battle_packed_mario_jump.cpp`; kept the single `play` call (the "model
both records" follow-up came from the generic body and is withdrawn). Filed the
correction breadcrumb in `docs/requests/JUMP-01_HISTORY.md`.

Validation: `make test` 87 CTest / 90 Python pass; `make all` 3DS build pass;
`diff --check` clean. Docs are mid-restructure (inbox split into
`docs/requests/`), so the breadcrumb went to the per-request history file, not
the index. Full game still incomplete.

## 2026-10-12 — Medium-size field collision functions (mario_hit.o)

Continued the medium-size sweep requested for a US port. First verified
that the 359-function fingerprint set in
`artifacts/reference/behavior/evidence.json` is now fully referenced by
existing docs (its `documented` flag is stale and was not used), then
widened the search to the full `us_symbols.csv`: 1098 game-text functions
in the 500-1500 byte band, 810 of them not referenced anywhere.

Worked the Mario field-collision family, picked because it is directly
needed for movement fidelity. Five new contracts in `docs/behavior/field/`:

- `MARIO_SLIT_CHK_WALL_AROUND.md` - complete structural read of the 8
  direction slit probe: two sweeps, three-way per-direction
  classification, and the two-group triple-run rule that returns 0 on
  three adjacent blocked directions.
- `MARIO_CHECK_WALL_ROLL.md` - the roll wall check, including the 14-tick
  wall-hold re-arm record at `mario+0x1cc..0x1d4`.
- `MARIO_SEARCH_HEAD_ROLL.md` - the 5-iteration indexed direction dispatch,
  with the `frsp` roundings between trig and scale called out.
- `MARIO_SEARCH_FRONT_WALL_STAYING.md` and `MARIO_CHECK_WALL.md` - both
  documented as **complete bodies** (every branch and call site).
  The staying search contributes the `mario[+0x2e] == 19` veto, the
  field-name gate and the triple `vivianGetStatus` gate; `marioCheckWall`
  contributes the 5-char hit-name hysteresis (thresholds 10/20) on a
  **signed** byte at `mario+0x3f` and the in-place `movePos` slide.
- `SEARCH_FRONT_WALL.md` and `MARIO_CHK_WALL_AROUND.md` - the two largest
  callees (2404 and 2460 bytes), documented at **structure level only**:
  entry gates, probe grouping, SDK vector math. Their probe sequences and
  result reduction are explicitly not decoded.

Cross-references in the earlier three docs were corrected where they listed
these two callees as undocumented, including one `marioChgWallAround` /
`marioChkWallAround` transcription error.

Newly recorded facts: the field literal at `0x800c3d88` gates both
`searchFrontWall` and stage 3 of `marioSearchFrontWall_staying`, so those
two paths are tied to one specific field; the 5-char hit-name literal
compared in `marioCheckWall` lives at `0x80049504`.

Validation: all callees resolved against `us_symbols.csv`; disassembly
regenerated per function into `/tmp` only; every `git diff --check` clean;
README link audit and field cross-link audit both clean. Docs-only, so
`make test` / `make all` were not rerun. Full-game work remains
incomplete.

Limitations: no Dolphin observations were taken this session, so every
claim is class B. The two structure-level docs must not be read as
equivalent to the original implementations.

Next tasks:

1. Decode the 9-probe reduction in `searchFrontWall` and the 12-probe
   reduction in `marioChkWallAround` - these block full movement parity.
2. Read the two unresolved literals from the ISO: `0x800c3d88` (the
   gated field name) and `0x80049504` (the hysteresis hit-name prefix).
3. Extract the `lfs` constants at `0x80049468`..`0x8004950c` - they are
   referenced by nearly every function documented in this session and are
   currently the largest single source of provisional numbers.
4. Continue the 810-function medium backlog; `party/*` and
   `npcCheckHitMarioSide` / `mobjHitEntry` are the next highest-value
   targets after collision.
5. `docs/PROGRESS.md` tables should be reconciled against actual file
   contents if any further drift is found.

## 2026-10-12 — Native request 3 slice: pose call-site survey

Delivered OVERWORLD_POSE_CALL_SITE_SURVEY: 18 marioChgPose sites with
ISO-read literals (KPA_D_1/D_2 table pair, M_D_2/M_U_3 recovery pair,
M_J_1B/M_J_1C, M_W/M_O/M_R alternation) plus gate/ordering notes; all
surveyed sites are marioChgPose (no EvtPose). Remaining request-3 items
(named domains, pipe-paper asset work) explicitly scoped out with a
non-finding (no reference body) for the pipe lifecycle.

Validation: all 18 call sites re-checked, literal strings ISO-read, link
audit, diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — Native request 2 remainder: bottomless numerics

Delivered OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS: first-tick gate,
win_03/win_01/tik_01 lane selection (rodata-read), angle setup, stage
machine via ISO-read table (0/2/10/11/12 live, 13 terminal-by-gate),
fallSpeed-shape integrator, min −10.0 snap, land/HP chain, and the
0.34/−0.002/−0.01 drop-back triple (port's jerk cascade matches one lane).
Dead −2000.0 load and SDA set pinned from ISO. Request 2 fully answered
except float-chain interiors.

Validation: call/SDA/stage-write re-checks, link audit, diff --check clean.
Docs-only, no make rerun. Next: pose-table slice (request 3). Incomplete.

## 2026-10-12 — Native request 2: water recovery numerics

Delivered OVERWORLD_WATER_RECOVERY_NUMERICS for field/AC-03: full
stage/counter machine ([68]=stage via ISO-read table 0x8036ED4C, [72]=
counter), 12-slot ISO-read SDA set, sink −6.0 init, 40-count stage-1 hold,
two-tick stage-10 arm, HP-penalty tick at stage-31 expiry, 12-count stage-32
window, and named gaps where the port authored ratios/jerk (no 0.08-class
constant, no jerk lane in-body). Stale "stage15" has no slot (no-op).

Validation: dispatch/stage-write/SFX-ID/HP-call re-checks with int/float
register-file care; link audit; diff --check clean. Docs-only, no make
rerun. Next: bottomless motion33/34, pose-table slice. Full-game incomplete.

## 2026-10-12 — Native JUMP-01 request 1 answered (SFX provenance)

Fulfilled the packed-Jump provenance request from the native inbox with
BATTLE_JUMP_SFX_PROVENANCE: vec3 scratch proven zero-bytes (not a key row),
U+0x18C key domain with two-call psndSFXOn_3D record (second inherits r3),
and no-PS-math finding for 801093a8. Recorded the answer in
DOCUMENTATION_REQUESTS.md without touching the request text. Water/motion
and pose-table requests (2/3) remain open — mot bodies located for next.

Validation: dual-offset ISO reads, call-site arg traces, PS-op sweep,
link audit, diff --check clean. Docs-only, no make rerun. Full-game
incomplete.

## 2026-10-12 — Third and fourth gap passes: event leaves and the shadow driver

- [Weapon/status/wait leaves](behavior/battle/BTLEVTCMD_WEAPON_STATUS_WAIT_LEAVES.md):
  six `battle_event_cmd.o` bodies with no snapshot definition.
  `btlevtcmd_GetWeaponActionLv` publishes `lbz [resolved+0x6E]` through
  `evtSetValue` with a **raw** destination word; `btlevtcmd_AcSetDifficulty`
  drives `BattleActionCommandSetDifficulty(B, unit, value)`; and
  `btlevtcmd_AnnounceSetParam` writes into an 8-byte-stride record array at
  `BattleWork+0x18C8C` at the `+4` value word of `4*index`, with no bounds check.
  `btlevtcmd_AfterReactionEntry` resolves operands into
  `BattleTransID`/`evtGetValue` and calls the queue helper from the
  after-reaction contract. `btlevtcmd_StatusWindowOnOff` is a signed four-way
  dispatch writing `word[B+0x2748]` (modes 0/1) or `word[B+0x2744]` (mode 3) and
  then always calling `BattleStatusWindowCheck`; those are exactly the two words
  whose nonzero state forces that checkpoint's result to 0, which is recorded as
  a static inference. `btlevtcmd_StartWaitEvent` calls `BattleRunWaitEvent` only
  when `word[unit+0x28C]` is nonzero, pairing with the existing stop leaf.
- [Shadow driver leaves](behavior/display/SHADOW_DRIVER_ACCESSOR_LEAVES.md):
  the whole fifteen-body `shadowGet*` family in `shadowdrv.o`, previously absent
  from every prose file. Three SDA words at `0x804150c8/cc/d0` select the
  depth/projected/character shadow records; two static texture objects live at
  `0x8041e738`/`0x8041e758`; the three `ProjTexMtx` getters share relative
  `+124`; the two `Config` getters index a 72-byte-stride table at `0x80304a60`
  from `word[record+8]` with no bounds check; the three `TexObj` getters gate on
  bit 0 of `word[record+0]` and tail-call `smartTexObj(record+220, word[record+
  252])`, returning NULL when the gate is clear. `shadowGetDepthShadowColor` is
  the only value-producing body: it packs `(255-c)*alpha/255` for the three
  component bytes at `record+256..259` using the `0x80808081` signed-division
  idiom into one word with a zero high byte.

`BattleAfterReactionMain` was checked against the existing
[waits and conclusion](behavior/battle/BATTLE_WAITS_AND_CONCLUSION.md)
contract instead of being rewritten: the queue scan, `BattleConsumeReserveItem`,
`BattleCheckConcluded`, `BtlUnit_GetData`, `evtEntry` and the recursive restart
on a missing unit all match that prose, so the queue contract now links to it.
`btlevtcmd_AcSetParamAll` was likewise left alone: it stores eight resolved
operands into `word[B+0x1CC8+4*i]`, skipping the `0xf1194d80` sentinel, which the
action-command callback contract already documents.

Validation: `make test` passes and `make all` passes; logs
`/tmp/pm-gap3-test.log`, `/tmp/pm-gap2-build.log` (docs-only pass, no native
change). `python3 tools/check_doc_links.py` reports one pre-existing broken link
owned by another agent (`partner/FIELD_PARTNER_FLY_MAIN_DISPATCH.md`); every link
added by this pass resolves.

Next: remaining unaddressed rows are larger bodies — `BattleCheckPikkyoro`
(112 lines), `battleMakePhaseEvtTable`/`battleSortPhaseMoveTable`,
`partyDoWork`, `_getRegistStatus` and `marioLandOn`/`marioSearchHead`.
EVT-01/JUMP-01/DMG-02 and the full-game requirements remain incomplete.

## 2026-10-12 — Source audit: documentation requests for packed Jump and recovery

Observed the implementations under `source/` and filed three native
documentation requests in docs/DOCUMENTATION_REQUESTS.md (appended at the end;
no docs-agent lock reserved). Reviewed the recent native commits that shipped
without leaving requests (b79bbe2 packed Mario Jump services, 81c143d
water/void/pipe recovery poses, 778add7 idle/low-HP pose scheduling,
e1257d8/357a656 field shadows).

- JUMP-01: the packed Mario Jump position binding (commit79bbe2) requests the
  0x802ee470/474/478 sound-key block, the original jump psndSFXOn_3D
  invocation record (timing/vector/return/landing cue), and the PS-family
  sin/cos identity plus FPSCR entry state. Local bodies were already recovered
  (BATTLE_MARIO_JUMP_MOTION/NUMERICS, BATTLE_UNIT_SOUND_MODE,
  BATTLE_DAMAGE_ORCHESTRATION for the mode1 AC-possibility fallback).
- field/AC-03: `source/water_recovery.cpp` water/void return-fall parameters
  (mot_damage motion31 / mot_bottomless motion33 stages) are authored and
  source-derived per include/water_recovery.hpp; pose timings are recovered,
  the numeric stage contract is not. Requested measured original sink/rise/arc/
  travel/return counters, HP-penalty tick, and bottomless placement/fall.
- field/AC-03: recovery pose-table domains 4..40 and pipe paper (p_dokan_x)
  lifecycle requested per the OVERWORLD_MARIO_RECOVERY_POSES open tail.
- P0c main-loop/field-dispatch: the native fixed-60Hz accumulator and
  Game::tick/move/startBattle substitutes have no original marioMain
  (8005a7e0) request; asked for the motion-ID→subsystem dispatch chain,
  battle-transition construction, input gate and marioStMain retrace/pause
  cadence (MARIO_MAIN.md sketch gaps), leaving existing pause/RNG asks open.
- P1b save/persistence: source/save.cpp uses a native PM2D format and one
  opaque originalStory int; asked for the complete original cardMain save
  record/state machine and the swSet/swGet story-switch bank, since CARD_MAIN.md
  and SW_SET.md are early sketches with explicit remaining gaps and no inbox
  entry.

Address-coverage check: of every original 0x8xxxxxxx address referenced by
source/, only 0x802ee470 (sound-key block, requested) lacked a contract; the
memory-bootstrap table, game-pointer clock block and sampling constants are
already documented.

Follow-up: the documentation agent answered the JUMP-01 request in commit
044ba5c with behavior/battle/BATTLE_JUMP_SFX_PROVENANCE.md. It corrects two
premises — 0x802EE470/474/478 is `vec3_802ee470` zero scratch (not a key row),
and 801093a8 has no Gekko PS math (trig via system_angleABf/distABf). Recorded
the resulting native action items (drop the spurious 0x802ee470 reads, bind the
same-TU angle/dist helpers, model both psndSFXOn_3D calls) as a ledger
breadcrumb; no native code changed.

Validation: docs-only change; no native/test/CMake edits and no make rerun
(latest recorded host state remains 87 CTest/90 Python and make all passing).

Next: documentation agent answers the five requests; native closures for
packed Jump sound cadence, water/void physics replacement, pose-table driven
recovery presentation, the original field main-loop/pause cadence and original
save persistence; full-game story/encounter closure stays open.

## 2026-10-12 — Getter plus status dispatch; case-table rescue

Closed BattleGetUnitPtr (NULL-domain table, −1-first, asymmetric setter,
487-site US row; snapshot definition with JP/US caveat) and BtlUnit_GetStatus
(28-slot ISO-read jump table, paired sign gates, nullable outputs).
Verification caught a badly misaligned case table in my own draft (wrong
lanes for statuses 0/2/3/16–24/27, inverted pair directions); rewrote it
from a machine dump of every block and re-verified all 20 lane blocks plus
6 paired blocks. Also resolved a phantom read-tool/listing discrepancy by
re-reading bytes from disk (disk won).

Validation: full re-verification, link audit, diff --check clean. Docs-only,
no make rerun. Next: zero-byte placeholder re-extraction; triangle/math
interiors. Full-game incomplete.

## 2026-10-12 — OW-WALL-01 traversal half: hit-vector skeleton

Delivered BATTLE_HIT_VEC_TRAVERSAL_ORDER for the overworld wall inbox:
4-region/call-census (49 bl all resolved: PSVEC family, checkTriVec_xz,
mapGetWork)/return skeleton with ISO-read SDA values, NaN-safe gate
patterns, and a verified region-1 counter/pointer asymmetry (r30+1/r31+84
vs r31+1/r30+84 elsewhere) reported without inference. Triangle interiors,
chunk ownership, and callback identity stay open per the request's own
remainder. Fixed two of my own early assertions (stride count, uniform-loop
claim) during verification.

Validation: machine-extracted census/edges/strides/counts, link audit,
diff --check clean. Docs-only, no make rerun. Full-game incomplete.

## 2026-10-12 — New denomination: flagged-callee contracts (CheckStatus, filter, search)

Survey found the register's flagged open callees now have reference pairs but
no dedicated contracts. Closed three: BATTLE_UNIT_CHECK_STATUS (null gate,
GetStatus byte, 6-compare, +759 test, exact {0,1}), BATTLE_HIT_CHECK_FILTER
(8-int/6-float marshal, hitVec call, six outputs, residue return),
MESSAGE_MSG_SEARCH (two-table binary search with gp fallback, no miss-null).
Caught two wrong relative links pre-commit (battle/→overworld, message/→battle).

Validation: full instruction re-checks (incl. corrected stfs count and
output-opcode proof), link audit, diff --check clean. Docs-only, no make
rerun. Next: BtlUnit_GetStatus, BattleGetUnitPtr dedicated contracts;
zero-byte placeholder re-extraction. Full-game incomplete.

## 2026-10-12 — Register complete: landing query 2, hitVec alias (43/43)

Closed checkLand2 as marioChkLandon2 (five-probe loop, variant dispatch,
lane quantization, angle tail; doc overworld/OVERWORLD_LANDING_QUERY_2.md)
and resolved hitVec as alias of partially-contracted hitCheckVecFilter
(SLOPES doc covers orientation/normalization; triangle phases stay open).
Final tally: 38 behavior contracts + 5 alias resolutions = all 43 register
rows addressed. Residual open work outside the register: angleABf/
system_angleABf bodies, hitVec triangle phases, and the usual lifetimes/
cadences. Fixed two of my own errors pre-commit (wrong bl census, wrong
callee-share claim).

Validation: instruction re-checks, ISO SDA reads, link audit incl. a
repaired register link; diff --check clean. Docs-only, no make rerun.
Full-game goal stays incomplete.

## 2026-10-12 — Porting-critical large function documentation

Added detailed documentation for high-priority functions needed for porting,
as identified in `docs/behavior/meta/US_COMPLETION_AND_PRIORITIES.md`. All
new files follow the per-subsystem folder requirement from AGENTS.md.

### Overworld category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioMain` | `0x8005a7e0` | 2528 B | `overworld/MARIO_MAIN.md` |

Primary field player update: motion dispatch, gravity, subsystem ordering,
battle transitions, and save-slot management.

### Battle category

| Function | Address | Size | File |
|----------|---------|------|------|
| `battleMain` | `0x800f904c` | 144 B | `battle/BATTLE_MAIN.md` |
| `BattleCalculateDamage` | `0x8005f958` | 1692 B | `battle/BATTLE_CALCULATE_DAMAGE.md` |
| `BattleSetStatusDamage` | `0x800fcf04` | 4220 B | `battle/BATTLE_SET_STATUS_DAMAGE.md` |
| `BattleDamageDirect` | `0x800feaa8` | 2852 B | `battle/BATTLE_DAMAGE_DIRECT.md` |
| `BattleCheckDamage` | `0x800fb9cc` | 2560 B | `battle/BATTLE_CHECK_DAMAGE.md` |
| `BattleCheckCounter` | `0x800fabcc` | 1444 B | `battle/BATTLE_CHECK_COUNTER.md` |

Battle system entry, damage calculation pipeline, status damage application,
direct damage, damage validation, and counter checking.

### Event category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evtmgrCmd` | `0x800316b8` | 5048 B | `event/EVTMGR_CMD.md` |
| `btlseqTurn` | `0x80042b94` | 7896 B | `event/BTLSEQ_TURN.md` |
| `btlseqAct` | `0x8011bb38` | 2220 B | `event/BTLSEQ_ACT.md` |
| `btlseqMove` | `0x8011c264` | 1680 B | `event/BTLSEQ_MOVE.md` |
| `evt_npc_wait_battle_end` | `0x8004efbc` | 184 B | `event/EVT_NPC_WAIT_BATTLE_END.md` |
| `evt_npc_set_battle_info` | `0x8004f0a4` | 100 B | `event/EVT_NPC_SET_BATTLE_INFO.md` |
| `evt_npc_battle_start` | `0x8004efbc` | 184 B | `event/EVT_NPC_BATTLE_START.md` |

Event VM command interpreter, battle turn/action/move sequence controllers.

### Message category

| Function | Address | Size | File |
|----------|---------|------|------|
| `msgAnalize` | `0x80081de0` | 5796 B | `message/MSG_ANALIZE.md` |
| `msgDisp` | `0x80050b88` | 2900 B | `message/MSGDISP.md` |
| `msgWindow_Main` | `0x8011bb38` | 2052 B | `message/MSGWINDOW_MAIN.md` |
| `msgMain` | `0x8008460c` | 1832 B | `message/MSG_MAIN.md` |
| `msgWindow_Disp` | `0x800808ec` | 1756 B | `message/MSGWINDOW_DISP.md` |
| `_evt_msg_print` | `0x800d2d60` | 1244 B | `message/EVT_MSG_PRINT.md` |

Message text analyzer, dialogue display, window update, message main loop,
window rendering, and event-driven message printing.

### Input category

| Function | Address | Size | File |
|----------|---------|------|------|
| `btlevtcmd_ResultACDefence` | `0x80107694` | 144 B | `input/BTLEVTCMD_RESULT_AC_DEFENCE.md` |

Defense action-command result handler.

### Field category

| Function | Address | Size | File |
|----------|---------|------|------|
| `evt_bero_mapchange` | `0x8007aad0` | 176 B | `field/EVT_BERO_MAPCHANGE.md` |
| `evt_mario_jump_pos` | `0x800e4c54` | 1984 B | `field/EVT_MARIO_JUMP_POS.md` |

Room transition event command and scripted Mario movement.

### Memory category

| Function | Address | Size | File |
|----------|---------|------|------|
| `swSet` | `0x800858fc` | 64 B | `memory/SW_SET.md` |
| `cardMain` | `0x800b2508` | 1692 B | `memory/CARD_MAIN.md` |

Story switch setter and save/load state machine.

### Unit category

| Function | Address | Size | File |
|----------|---------|------|------|
| `pouchGetItem` | `0x800d5520` | 1188 B | `unit/POUCH_GET_ITEM.md` |
| `BtlUnit_Entry` | `0x80129644` | 1716 B | `unit/BTL_UNIT_ENTRY.md` |

Item retrieval from pouch and battle unit initialization.

### Format category

| Function | Address | Size | File |
|----------|---------|------|------|
| `__dec2num` | `0x80266834` | 4224 B | `format/DEC2NUM.md` |
| `__strtold` | `0x8026e68c` | 4108 B | `format/STRTOLD.md` |
| `__sformatter` | `0x8026cdf4` | 2476 B | `format/SFORMATTER.md` |
| `__ieee754_pow` | `0x8027067c` | 2096 B | `format/IEEE754_POW.md` |
| `__pformatter` | `0x8026ab78` | 1908 B | `format/PFORMATTER.md` |
| `float2str` | `0x8026b294` | 1820 B | `format/FLOAT2STR.md` |
| `__minus_dec` | `0x800d2d60` | 1276 B | `format/MINUS_DEC.md` |

Decimal conversion, string-to-float, printf/scanf formatters, power function,
float-to-string, and decimal subtraction.

### Platform category

| Function | Address | Size | File |
|----------|---------|------|------|
| `marioStMain` | `0x8006faec` | 176 B | `platform/MARIO_ST_MAIN.md` |

Main field system update orchestrator.

### Validation

All functions inspected with `tools/inspect_dol_function.py` against read-only
US G8ME01 ISO. Documentation records exact address, byte size, disassembly,
called functions, and data offsets. No original executable code linked or
copied.

### Limitations

- Many called helper functions remain undocumented
- Full structure layouts partially documented via field references
- Motion ID semantics and subsystem chains need further recovery
- Battle transition conditions require original event script analysis
- Shared helpers across commands need individual documentation

### Next tasks

- Document remaining medium-size functions (500-1500 bytes) by priority
- Recover full structure layouts for battle work, unit, and event contexts
- Cross-reference shared helper functions across command implementations
- Document remaining event command handlers and their argument processing
- Recover complete motion ID table and associated behavior chains
