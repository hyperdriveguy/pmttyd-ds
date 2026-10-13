# Progress and next-session handoff

Newest entries first under this single heading. Keep entries dated, write link paths relative to `docs/`, and record exact validation, limitations and next tasks in each entry. Append-only: do not rewrite earlier entries.

## 2026-10-13 — Original setup4 command and target menu

Bound a native command/target menu after Frankly's original battle introduction
and window closure. Source default Normal Jump/Normal Hammer/Headbonk weapon
IDs, admission/attribute masks and unchanged US menu/unit labels are imported
from the read-only ISO. PMBUNIT2 adds original local/hit/sampling geometry and
setup forward bias. The existing recovered sampler owns admission/occlusion:
Jump admits all three enemies, including Spiky Goomba; Hammer initially admits
only the front Goomba. Target refresh retains unit/part across row changes,
uses stored status27 rather than HP as death, wraps navigation and gives B
priority over A. Native D-pad/Circle Pad repeats and touch selection operate.
Owned views rebind after copies; confirmations retain actor/weapon/unit/part
without damage, move consumption or field resume. Partner Headbonk targeting
is host-tested but awaits the native turn scheduler. See
[native binding](behavior/battle/NATIVE_TRIO_COMMAND_TARGET_BINDING.md) and
[source follow-up](behavior/battle/UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md).

Validation: `make test` passed113 CTest/100 Python tests
(`/tmp/trio-commands-tests-final.log`). `make all` passed
(`/tmp/trio-commands-all-final.log`); isolated development build with
`DEBUG_SHORT_BOOT=1 BUILD=build-trio-battle TARGET=artifacts/ttyd-trio-battle`
passed (`/tmp/trio-commands-native-final.log`). Native smoke helper passed
`tests/azahar_trio_battle_targets.py` in
`artifacts/azahar-trio-battle-targets`; manually inspected Jump's three targets
with Spiky selected and Hammer's single Goomba. The target label received a
contrast panel afterward. Source tests additionally cover pair compaction,
HP-versus-status27, empty lists, cancellation, copied owners, retained weapon
IDs, physical/touch Game entry and partner targeting. Fixed an ARM int32_t
callback return mismatch in concurrent save-object work; that work stays
separate. No original execution/hardware/full combat parity claim.

Limits/next: confirmation ends at the explicit original-attack-pending notice;
B returns to inspect targets while dispatch remains unconsumed. Execute the
original Jump/Hammer/Headbonk event streams with motion/AC/hit/counter services
and source poses, then source move counts/phases, enemy actions, death/wing-fall
reactions, rewards/result publication and field093/094/094_01/story12. Keep the
inbox requests open for source death/event barriers and remaining composition.
Full playable prologue remains unfinished.






## 2026-10-13 — Priority0 sequence pause/mute/deferred field contracts

Delivered [pause/mute](behavior/audio/MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md).
Three complete bodies pin inline lookup, state1 active→paused before note kill/
recycle, high-bit pause/mute deferred fields, raw mute words and public IRQ
forwarding. No timing/silence/private-index repair inferred. Marked local
EVT-01 portions complete; deferred consumers/provider/runtime remains OPEN.

Validation: generic read-only verifier6 spans (3 complete/3 linked),160 anchors/
five resolved calls passes.10 pause/6 mute/5 flag vectors and three-head order
checked as deductions. Full fresh ELFs/listings stay `/tmp`; no original execution.
Link audit775 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: ID/state/list/note/deferred-byte/word/mute-field providers/consumers,
resume/controller/voice scheduling, index/member/alias/IRQ/concurrency and original
pause/mute/audio traces. Continue priority0 source/backend then priority1 damage.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 synthHandle fade/aux region and local body closure

Delivered [fades/aux](behavior/audio/MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md).
Complete final region pins zero-offset gate, master/pause coefficient/bitmap
publication, mode requests/early exit and8-studio A→B/tag1/control-halfword
callbacks. Prior queue/voice/clock plus this contract closes local whole
synthHandle. Marked local portion complete; child/provider/runtime EVT-01 OPEN.

Validation: generic read-only verifier9 spans (region/6 linked/whole fingerprint/
zero literal),150 anchors/six resolved calls passes.6 modes/5 bitmap/16 studio-side
rows are source deductions, not original execution. Full listing/ELF remains
`/tmp`, source reverified. Link audit773 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: fade/bitmap/mode/coefficients and queue/voice/time providers, PPC/
alias/IRQ/concurrency domains, sequencePause/Mute/precision/mac/ADSR/aux children,
callback installation and original scheduling/fade/audio traces. Continue
priority0 source/backend then priority1 damage. No hardware/full-game completion.

## 2026-10-13 — Priority0 scoped synthesis job consumer and clock tail

Delivered [consumer/clock](behavior/audio/MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md).
Two complete regions/no-op leaf pin category0→2→1 captured-next/marker/root
order, voice flag/pedal/hardware requests, fresh index advancement and64-bit
raw-input time addition. Whole synthHandle fingerprint does not close its
master-fade/aux middle. Marked local portions complete; parent EVT-01 OPEN.

Validation: generic read-only verifier13 spans (2 regions/1 leaf/9 linked/whole
fingerprint),163 anchors/10 direct calls passes.3 queue/5 index/5 clock vectors
and65536 pedal cases are derived checks; original code not executed. Full fresh
ELF/listing stays `/tmp`. Link audit772 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: synthHandle zero-offset/master-fade/aux middle, precision/zero/mac/
pedal/ADSR/hardware handlers, queue/voice/flags/time providers and member/alias/
IRQ/concurrency domains, original scheduling/audio traces. Continue priority0
source/backend then priority1 damage. No hardware/playback/full-game completion.

## 2026-10-13 — Retained original save windows and ticketed persistence services

Commit `bd61acc` adds [portable save presentation](behavior/field/NATIVE_SAVE_BLOCK_RETAINED_PRESENTATION.md).
The original221-command graph now runs with real original global US message
payloads, retained main windows, separate source selection windows and nested
key ownership. Continue preserves cursor/schedule/key wait/identity rather than
force-closing; Add respects raw versus keyed operands. Window poll flags are
separate from playback data flags and survive ordinary waits/closing until
source deletion. Original system parser type/color and state7-to8 fallthrough
are implemented. No save dialogue is authored or rewritten.

Write requests carry one-use tickets and four source switches captured after
same-invocation VM writes. Simulation performs no filesystem I/O; an external
provider supplies delayed success0 or provisional generic error-1. Stale,
duplicate, untaken, finished and moved-from acknowledgements reject. Header's
one-update yield adapts combined native SD serialization; original card hardware,
classification, format/create/IPL and real worker binding remain absent. Missing
resources/unsupported operations fault and yield for owner recovery. Save v6
retains40 bytes/checksum, stores GSF1207/233/236 in byte34 and GSW4 in byte35,
and preserves legacy1..5 decode/reserved-byte checks. Live propagation is next.

Inbox documentation independently recovered camera8's
[letterbox decision lock](behavior/camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md)
and [original cue431](behavior/audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md).
Bit200 must not freeze native field camera4 following; it preserves camera8's
automatic letterbox choice while fades/draw continue. Original cue431 maps
FX618/group5/macro745/sample550; the ignored source WAV exists but is absent
from the current five-cue native fallback pack. Source runtime residence is
unproven; no generated sound replaces it.

Validation: final shared `make test` passes112 CTest/100 Python. Isolated source/
romfs snapshot `/tmp/pm-save-presentation-v2zjcjj3` passes111 CTest/100 Python and
`make all`; only the existing professor_fence_animation_original check is absent
because its private `artifacts/professor-fence-reference.bin` was not copied.
Logs: `/tmp/save-presentation-final-shared.log`,
`/tmp/save-presentation-final-isolated-test.log`,
`/tmp/save-presentation-final-isolated-native.log`. Real original-data tests cover
first-hit pages, raw scroll/stop, Yes/No/B cancellation, delayed success/failure,
retained main/choice ownership, nested keys, same-tick save-mode snapshot,
stale/duplicate/moved acknowledgements and window lifetime. Save tests cover all
three flag combinations, full mode bytes, v5 migration, older versions and
atomic file/backup recovery. An initial original first-hit test failed because
system parser control was unsupported; the documented type/color implementation
resolved it. Final Markdown link audit771 files/0 broken; owned whitespace checks
pass. No emulator/hardware capture is claimed: presentation remains unbound in
Game, so this milestone does not activate the block's live interaction. ISO
remains read-only and generated/extracted assets remain outside history.

Next: bind SaveBlockPresentation to live source catalog owner/name hit publication,
MapObjectState callback and independently timed S_1/A_1. Publish animation results
after object callback, use original clip starts, and key renderer cache by pose
plus per-instance time. Bind event processing before Mario, nested key/system
ownership while preserving falling physics, separate camera8 letterbox lock,
retained windows/choice drawing and an asynchronous libctru SD worker. Propagate
GSF233/236/GSW4 into progress/write snapshots; suppress automatic harness writes
while original save flow owns persistence and ensure No never writes. Then run
isolated Azahar head-hit/No/Yes/save-file checks and measured hardware profiling.
Other objects/NPCs/areas and complete source field/category/pause/resource/audio
ownership remain open. The full user goal remains active.

## 2026-10-13 — Priority0 synthesis job insertion and start timestamps

Delivered [jobs/start time](behavior/audio/MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md).
Two complete bodies pin32-slot arithmetic, distinct category duplicate/reschedule
rules, ordered node/root writes and two real-time captures before0/1 requests.
Unsupported categories have no guarded failure; source type precondition remains.
Marked local EVT-01 portions complete; queue consumers/runtime remain OPEN.

Validation: generic read-only verifiertwo spans/75 anchors/two direct calls passes.
21 slot/3 category/9 duplicate/two timestamp maps checked as deductions; original
code not executed. Full fresh ELFs/listings stay `/tmp`. Link audit771 Markdown
files/0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original body/asset export. Isolated index excludes unrelated work.

Next/limits: queue/table/index/markers/time init and consumer advancement, category/
member/null/alias/IRQ/concurrency source domains, actual job/macro/voice scheduling
and original queue/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 macro active/inactive/wait/init lifecycle

Delivered [macro lifecycle](behavior/audio/MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md).
Four complete bodies pin state equality gates, active/time unlink, two-word wait
sentinels/clear/time and literal flags, activation's two-job request and live-count/
per-field pointer init. No scheduler/resource completion inferred. Marked local
EVT-01 portions complete; jobs/interpreter/source/runtime remain OPEN.

Validation: generic read-only verifier5 spans (4 complete/1 linked),125 anchors/
three resolved calls passes.6 wait/5 mask/7 state vectors and ordered job metadata
are deductions; no original code executes. Full fresh ELFs/listings stay `/tmp`.
Link audit768 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: synthAddJob/macro interpreter/wait-time producers, state/list/voice/
count/pointer providers and member/alias/IRQ/concurrency domains, original job/
macro/voice/audio traces. Continue priority0 source/backend then priority1 damage.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 stream kill/init and hardware break flags

Delivered [stream/hardware leaves](behavior/audio/MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md).
Four complete bodies pin64-record first-match kill/callback, configured-prefix
stream reset with fresh tail count and exact DSP break/active flags. Record
publication is separate from hardware/voice/audio completion. Marked local
EVT-01 portions complete; deeper providers/consumers/runtime remain OPEN.

Validation: generic read-only verifier5 spans (4 complete/1 linked),78 anchors/
one direct call passes.10 initialization/10 kill-route/16 break cases are derived
source checks. Full fresh ELFs/listings stay `/tmp`; no original code executes.
Link audit767 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original body/asset export. Isolated index excludes
unrelated staged/worktree work.

Next/limits: voiceUnblock/callback/stream-state/count providers, valid index/offset/
reset/concurrency domains, streamHandle/DSP consumers and original shutdown/audio
traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Original setup4 units, stage and Frankly battle introduction

[Native introduction](behavior/battle/NATIVE_SETUP4_BATTLE_INTRODUCTION.md)
imports original tik setup4 four-unit kind/part/group data: HP2 normal/spiked/
airborne Goombas, FranklyHP10, defense arrays and exact unit/part/counter flags.
Actual Frankly init15/entry7/phase39 commands run in a separate VM while the
original field scene remains suspended at battle_start. Source fresh lectureID6,
work0/maxmoves0,900msentry, counted45-step R_1 move, phase04000001/turn1 gate,
five original092_01..05 messages, status/camera requests and actor facing execute.
Native field-battle services publish sequence4/result0/controls-off; field
checkpoint/story stay unchanged. Full original scheduler/unit setup/player turns
are separate dependencies, not replaced by the Crump harness.

Imported original bti_01/stg_00_1 and source selected five object subtrees.
Recovered visibility excludes alternate A2/B geometry while revealingselected
isi1/isi2/B_mati descendants; default escape_wall/if_body/if_wire are hidden.
Authored ancestor matrices and source additionaltranslations remain; opaque/
translucent passes now filter separately. This fixes observed floor/beam occlusion
without invented actorY lifts. Source homes are Mario(-100,0,-5),Goombella
(-140,0,5); enemy positions retain grouprecords. Imported original PAD_Y_ON115
56x56icon; used atlas512x256/0.5MiB. Bound battle commonprint mode3 and used
one-shot toge selector1/mode0. Closing window continues after event completion.

Validation: final make test111 CTest/100Python and make all pass; separate
DEBUG_SHORT_BOOT artifacts/ttyd-trio-battle build passes. Tests check rawunits/
flags/stats, init/entry/firstturn/laterturn behavior, exact five messages,
status/camera/windowclose, retained field owner/control/result and checkpoint
coordinates. Final isolated Azahar artifacts/azahar-trio-battle-stage-final
captures original dialogue/cameras and complete intro; inspected cleanstage
shows allsix characters and explicit pendingplayercommands. Prior native capture
exposed whole-map visibility/pass occlusion and a generic dialogue fallback;
both were corrected. Initial092_05 parse failed for missing PAD_Y_ON, then used
source icon import fixed it. No user emulator profile/save touched.

Limits/next: bind original player target/Jump/Hammer/Headbonk graphs and private
pad/impact clocks, enemy turns, spike conditional092_06/07, airborne damage/fall,
result/loss/escape and source wait_battle_end return. Then execute original
093/094/094_01/story12 field continuation. Current introduction sequences
Frankly init→entry→phase; complete concurrent firstAct/phase scheduler, wait-event
and unit registries remain open. Stage spring scales/curtain/dynamic lifecycle,
full material/depth/lighting/shadows, camera defaults, effects/SFX and original
runtime traces remain partial. Source fixed-group RNG consumption and full
starting pouch/party setup are pending. Original setup3 lecture and ruins route
still required; fullprologue/hardware parity is not achieved. Goal remains active.

## 2026-10-13 — Priority0 sound-chain kill and voice allocator recycling

Delivered [sound/voice kill](behavior/audio/MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).
Two complete bodies pin external mapping/identity/captured successor, stale skip,
internal priority/free-list/count recycle and stream/hardware tail. Requested kill
and allocator state changes are separate from actual hardware/audio completion.
Marked local EVT-01 portions complete; child/source/runtime remain OPEN.

Validation: generic read-only verifier6 spans (2 complete/4 linked),124 anchors/
five resolved direct calls passes.5 mask/4 counter/6 index/4 chain-route vectors
checked as deductions. Full fresh ELFs/listings stay `/tmp`; no original code
executes. Link audit766 Markdown files/0 broken; whitespace checks pass. Docs/
metadata only: no make rerun, native change or original body/asset export.
Isolated index excludes unrelated staged/worktree work.

Next/limits: map-reference/macro-inactive/stream/hardware children, identity/
priority/free/count providers and valid chain/index/member/IRQ/concurrency domains,
original note/voice recycle/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 sequence initialization and free chains

Delivered [sequence free-chain init](behavior/audio/MUSYX_SEQUENCE_INITIALIZATION_AND_FREE_CHAINS.md).
Complete seqInit pins empty active/paused roots,8-instance chain/IDs0..7/state0,
128 priority halfwordsffff,256 notes and next-ID0. Cross-record +18 writes are
next.previous within the20-byte note layout; payloads are not zeroed. Marked
local EVT-01 portion complete; constructors/voice/resource/runtime remain OPEN.

Validation: generic read-only verifierone body span/88 anchors/no calls passes.
Exact extents/chain metadata and emitted32*8 note-store simulation512 link
assertions/all writes within1400hex checked as deductions. Full fresh ELF/listing
stays `/tmp`; no original execution. Link audit763 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native change or
original body/asset export. Isolated index excludes unrelated shared work.

Next/limits: sequence constructors/ID/state/list/note/payload providers and MIDI
priority consumers, voice/resources/reset/concurrency and original initialization/
reuse/audio traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 sequence validity and stop lifecycle

Delivered [sequence validity/stop](behavior/audio/MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md).
Four complete bodies pin active-before-paused key/private-ID lookup, validity
boolean, public IRQ wrapper and high-bit field-clear versus untagged state/note/
instance recycle. Source lacks private-index/unknown-state repair; no audible
validity guarantee. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (4 complete/5 linked),157 anchors/
8 resolved direct calls passes. Nine validity/five width/five state vectors and
original8-instance extent checked as deductions. Full fresh ELFs/listings stay
`/tmp`; no original execution. Link audit762 Markdown files/0 broken; whitespace
checks pass. Docs/metadata only: no make rerun, native change or original body/
asset export. Isolated index excludes unrelated shared work.

Next/limits: key/private-ID/state/instance/note/free/efc providers and valid index/
list/cycle/alias/concurrency domains, voice/resource/sequencer execution and
original validity/stop/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Source save lifecycle and original221-command event core

Commit `a0905a6` adds the portable source save-block callback, owned hit-name
cache and uint8 nested key leaves. The callback preserves exact stage0..4
request order, previous-published ratio/ordered NaN comparison, optional versus
system children, entry priority30/type26, distinct running predicates and
retained eventID. These mechanics are compiled for host/ARM and do not yet
change the Game's live save-block interaction.

Original system/wrapper/common-child arrays retain221 commands/14 callback
identities/89 sites in ignored local packs. The builder validates exact source
hashes and reads only DOL data spans/operand strings plus actual label-scanner
lookahead words. No original executable body is exported or dialogue invented.
The existing interpreter executes the original child/branch/switch/flag graph;
typed services preserve raw/resolved arguments, source speaker/coordinates,
ID-versus-raw controls, firstCall/yielding semantics and output references.
See [native core](behavior/field/NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md)
and [asset provenance](ASSETS.md).

Documentation inbox work recovered
[restart/event identities](behavior/display/SAVE_BLOCK_RESTART_CLOCK_AND_EVENT_IDENTITIES.md)
and [retained Continue/Add ownership](behavior/message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md).
Continue resumes/polls, not force-closes; raw scroll/once_stop appends preserve
the retained main window while selection has its own handle. Actual native
message/card providers remain next; test callbacks are not presentation/I/O.

Validation: final shared `make test` passes110 CTest/100 Python tests. Isolated
source/romfs snapshot `/tmp/pm-save-flow-jikuwdrm` passes107 CTest/100 Python
and `make all` after final request/move changes; concurrent underground work
is excluded from that snapshot. Original-data tests check No/success flags and
mode/story effects, exact ID/raw-control order, source attribution/arguments,
yielding header/write, all normal-mode error families, accepted format/create,
format/create/write failure repolls, guard break and moved runtime ownership.
Lifecycle tests cover suppression, callback stage/order, nested key wrap, NaN,
optional children and helper-versus-wait predicates. An initial test backend
accepted every later creation choice while retaining a persistent error and
entered the source retry loop; the owned process was stopped and test responses
corrected. Source loop was preserved. An earlier shared run failed a concurrent
unfinished underground test syntax error; the final shared run above passes.
Documentation links758 files/0 broken and whitespace checks pass. No emulator
run is claimed for this library-only milestone; current rendering is unchanged.
ISO remains read-only and compiled packs/assets remain outside history.

Next: bind actual source instances/head-name publication, independently timed
S_1/A_1 with Main publication after callback, original431 audio when available,
event manager before Mario, nested Game key/system/camera ownership, retained
message windows and original selection. Then connect a yielding native persistence
adapter and preserve GSF233/236/GSW4 alongside existing source flags. Avoid
force-close on Continue or an extra Continue on success; No must not write.
Source pause/category/camera consumers, shared field VM/state, full lifetime,
original execution traces, other areas/actors and hardware profiling remain open.
The full user goal remains active.

## 2026-10-13 — Priority0 MusyX listener update and cross/inverse math

Delivered [listener/cross/inverse](behavior/audio/MUSYX_LISTENER_UPDATE_CROSS_AND_MATRIX_INVERSE.md).
Three complete bodies pin four-vector publication, cross-product order, stack
matrix layout, inverse cofactor/translation stores and low-byte volume/127.
No normalization, singular rejection or in-place snapshot guarantee added.
Marked local EVT-01 portions complete; numeric/source/consumer/runtime OPEN.

Validation: generic read-only verifier10 spans (3 complete/4 linked/3 literal),
229 anchors/six resolved direct calls passes. Derived stack/store maps and two
admitted matrix/one cross examples remain static deductions; no original code
executes. Full fresh ELFs/listings stay `/tmp`. Link audit761 Markdown files/
0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original body/asset export. Isolated index excludes unrelated work.

Next/limits: listener/basis/allocation providers, null/nonalias/finite/invertible/
PPC numerical domains, CalcEmitter/consumer transforms and original geometry/
audio traces. Continue priority0 source/backend then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 game effect main and outer sound updates

Delivered [effect/listener/completion main](behavior/audio/GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md).
Three complete bodies pin listener displacement/basis arguments, separate40-record
update/completion passes, captured traversal/fresh pool cleanup, double voice
checks/countdown continuation, exact sequence-valid1 and effect/DVD/SS outer
order. No measured velocity or physical audio cadence. Marked local EVT-01
portions complete; provider/child/runtime remains OPEN.

Validation: generic read-only verifier21 spans (3 complete/14 linked/4 original
literals),135 anchors/25 resolved direct calls passes.40 pool offsets/four countdown
boundaries/double-query metadata are derived source cases. Full fresh ELFs/listings
stay `/tmp`; no original execution. Link audit759 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native change or original
body/bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: listener update/transform and S position/heading providers, pool/
record/sequence/countdown writers and alias/concurrency invariants, DVD/SS/sequence/
controller/voice and original completion/audio traces. Continue priority0 source/
backend then priority1 damage graph. No hardware/playback/full-game completion.

## 2026-10-13 — Priority0 emitter vector/volume update and 3D reset

Delivered [update/reset](behavior/audio/MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md).
Two complete bodies pin ordered two-vector copies, low-byte volume/127 target,
current-volume one-sided clamp and exact root/studio/flag reset. Update does not
check emitter membership; reset does not clear old emitter flags/voices. No
smoothing/cadence/release inferred. Marked local EVT-01 portions complete;
construction/consumers/source/runtime remain OPEN.

Validation: generic read-only verifier6 spans (2 complete/2 IRQ/2 literal),47
anchors/two resolved direct calls passes. Whole-DOL direct census2 sites;8 volume/
7 flag/5 finite clamp vectors derived only. Full fresh ELFs/listings stay `/tmp`;
no original execution. Link audit758 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make rerun, native change or original body/asset
export. Isolated index excludes unrelated staged/worktree work.

Next/limits: emitter construction/list/vector/volume consumers, actual SoundEfxMain
provider/gate graph, listener/studio setup, alias/null/finite/reset/concurrency and
original update/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 emitter removal and FX unload lifecycle

Delivered [emitter lifecycle](behavior/audio/MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md).
Three complete bodies pin active-system admission, exact neighbor/root unlink,
all-high16 flag clear, retained links/voice and key-off request. Bulk/FX-array
traversal preserves captured next and first-match break even without mutation.
Marked local EVT-01 portions complete; full voice/unload/runtime remains OPEN.

Validation: generic read-only verifier6 spans (3 complete/3 linked),117 anchors/
9 resolved direct calls passes. Whole-DOL aligned caller census9 sites;6 clear/
12 gate vectors checked as deductions. Fresh full ELFs/listings stay `/tmp`;
no original code executes. Link audit757 Markdown files/0 broken; whitespace
checks pass. Docs/metadata only: no make rerun, native change or original body/
asset export. Isolated index excludes unrelated shared work.

Next/limits: emitter construction/active/list/FX providers, member/cycle/array/
IRQ/concurrency source domains, full caller unload/silence and macro/voice/key-off
execution, original lifetime/audio traces. Continue priority0 source/backend then
priority1 damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 effect spatial controls and emitter query leaves

Delivered [spatial/emitter leaves](behavior/audio/SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md).
Five complete bodies pin early pan/surround record stores, fresh pool/voice lookup,
raw/controller width and two surround requests, retained volume read and active/
flag-gated emitter voice lookup. No rollback on voice miss or inferred audible
liveness. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier10 spans (5 complete/5 linked),75 anchors/
11 resolved direct calls passes. All256 surround mappings,9 raw-width and15
active/flag vectors checked; original functions not executed. Fresh full ELFs/
listings stay `/tmp`. Link audit756 Markdown files/0 broken; whitespace checks
pass. Docs/source metadata only: no make rerun, native changes or original body/
bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: effect-pool/emitter flags/voice providers and valid ordinal/null/
lifetime/concurrency domains, controller/MIDI/voice/backend services and original
pan/surround/fade/audio traces. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 remaining positional SFX live/fade/mute graph

Delivered [SFX live/fade/mute](behavior/audio/POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md).
Two complete regions pin global fade/percentage, spatial/nonspatial controls,
backend scaling, mute→fade→fresh stop/clear→final mute. Previous sweep/startup/
loop/tail plus this delivery close the local whole psndSFXMain body. Calls remain
ordered even when final mute follows key/engine clear. Marked local EVT-01
portions complete; backend/source/original audio remain OPEN.

Validation: generic read-only verifier18 spans (2 regions/15 linked/whole body
fingerprint),446 anchors/35 resolved direct calls passes.65536 division127 byte
products and65536 division100 unsigned×signed byte products checked;6 fade wrap/
decrement vectors are derived only. Fresh full ELF/listing remains `/tmp`;
no original code executes. Link audit754 Markdown files/0 broken; whitespace
checks pass. Docs/source metadata only: no make rerun, native changes or original
body/asset export. Isolated index excludes unrelated shared work.

Next/limits: SS/song/effect child controls/stop/query semantics, G/slot/descriptor
writers and flag/fade/denominator/alias/concurrency/lifetime domains, original
live/fade/audio observations. Continue priority0 source/backend then priority1
damage graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 listener setter and sequence camera source bindings

Delivered [listener/camera source](behavior/audio/GAME_LISTENER_SETTER_AND_SEQUENCE_CAMERA_BINDINGS.md).
Two complete bodies/two scoped argument regions pin alias-sensitive XYZ stores,
heading publication, unchecked camera table lookup, field position/repeated-heading
queries and battle fixed XYZ/flag100 Z adjustment. Exact direct argument bindings
are closed; whole camera/sequence gates/cadence remain open. Marked local EVT-01
portions complete; parent OPEN.

Validation: generic read-only verifier6 spans (2 complete/2 scoped/1 linked/
1 original literal),46 anchors/8 resolved direct calls passes. Whole-DOL aligned
setter census2 sites; original constants0/400/200 decoded. Alias example is a
static deduction, not original execution. Full fresh ELFs/listings stay `/tmp`.
Link audit752 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native change or original code-body/asset export. Isolated index
excludes unrelated staged/worktree work.

Next/limits: camera pointers/position/heading producers and finite/null/source
invariants, whole sequence admission/cadence, battle flag/shared-result writers,
indirect/REL/dynamic routes and original spatial/audio traces. Continue priority0
live SFX/source then priority1 damage graph. No hardware/full-game/audio claim.

## 2026-10-13 — Original underground approach, dialogue and Goomba animations

[Native trio scene](behavior/story/NATIVE_UNDERGROUND_TRIO_SCENE.md) runs actual
161-command gorotuki_sanninngumi and10-command footstep child in the existing
VM. Story<=11 roomrestore admits sourceinitial placement/flags and MarioX>=-100
crossingpoll, then original keys/cameras/party positioning and seven exact
086..092 messages with correct named speakers/Goombella suffix. Parallel actor
run/hop/move blocks preserve original local/shared event IDs. Imported actual
c_kuribo KUR/PTK/TGK stay/talk/walk/run/damage sparse poses and Marioe_mario
M_N_7. Native citro3d batches draw allthree variants; Frankly now follows/renders
as temporary partner underground and accepts original party1 placement.

Scoped source callbacks cover facing towardNPC, Mario/party positions and NPC
run/damage poses. Original script requests battleinfo4 and remains atbattle_start
until its actual backend exists; no postbattle messages/story12 are synthesized.
Documentation inbox delivered full setup4 contract: threeHP2 variants PLUS
Frankly coordinator, five firstturn messages and two conditional spike messages,
variant weapons/flags/counter/fall and source wait/result ownership.

Validation: staged source snapshot /tmp/pm-underground-trio-dbe8scf9 passes
make test108 CTest/100Python. Shared make all and separate DEBUG_SHORT_BOOT
artifacts/ttyd-underground-trio build pass; focused underground/professor tests
pass. Host original-data checks pin placements, passive poll, inclusive crossing,
message order, source pose/party positions, four cameras, setup4 and retained11.
Isolated Azahar artifacts/azahar-underground-trio captures original086, allthree
variants/Frankly, camera/dialogue pages and final pendingbattle notice; images
inspected. It is manual presentation evidence, not a full nativebattle pass.
Initial unusedZ_2 imports rejected non-endpoint topology, so only actualused
poses are admitted. InitialM_N_7 bank assumption rejected; corrected to original
e_mario. First shared make test encountered an unrelated build-host concurrent
archive race; isolated staged build excludes unfinished concurrent save-event
work and preserves its files. No user emulator profile/save touched.

Limits/next: bind setup4 battle/coordinator and original attack/counter/fall/result
services, then original093/094/094_01/GSW0=12 continuation. Full party stop/run,
force-restart pose clocks, NPC terrain/ease/hop arithmetic, effects/SFX, source
cadence, original lighting/material/shadow sorting and original-runtime traces
remain incomplete. Source first-arrival main map services/dynamic hit toggles and
actual pipe BOTH-party restoration also remain partial. Live scene saves remain
suppressed because native event state is not serialized. Complete setup3 lecture,
prologue route/ruins and New2DS hardware validation still required; goal active.

## 2026-10-13 — Priority0 calc3D spatial result and angle table

Delivered [calc3D/angle-table contract](behavior/audio/GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md).
Two complete bodies pin flag/capture precedence, original91-row angle table,
literal196 pan fold, PPC sqrt refinement/cos/sin surround, distance falloff and
byte-first volume clamp. Shared output preserves byte3. No host math replacement,
measured numeric/audio behavior or guessed listener invariant. Marked local
EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (2 complete/2 linked libm/1 table/
4 literal spans),161 anchors/four resolved direct calls passes.91 original table
rows/literal bytes checked;15 signed fold vectors remain static arithmetic.
Full fresh ELFs/listings stay `/tmp`; no original execution. Link audit750 Markdown
files/0 broken; whitespace checks pass. Docs/source metadata only: no make rerun,
native changes or original body/bank export. Isolated index excludes shared work.

Next/limits: listener/shared-byte source writers, valid heading/flags/finite and
PPC fused/frsqrte/fctiwz/libm domains, live SFX/backend/original spatial/audio
traces. Continue priority0 live/source owners then priority1 damage graph.
No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 audio frame and aux callback order

Delivered [frame/aux graph](behavior/audio/MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).
Seven complete bodies pin one-time active gate, DSP submission before five
seq/synth slices, frame-byte/live voice clears, final3D/stream/virtual-sample
order and direct OS critical wrappers. Aux A/B callback/buffer capture and fresh
B gates/count reads remain explicit; no measured physical cadence/action ticks
inferred. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier17 spans (7 complete/10 linked),127 anchors/
29 resolved direct calls passes. All256 byte arithmetic cases, five-slice order,
three-channel extent and6 aux-gate vectors checked; no original execution. Full
fresh ELFs/listings remain `/tmp`. Link audit748 Markdown files/0 broken;
whitespace checks pass. Docs/source metadata only: no make rerun, native change
or original body/bank export. Isolated index excludes unrelated shared work.

Next/limits: seq/synth/stream/3D/virtual-sample/command-builder engines, actual
voice/studio/aux providers and pointer/format/IRQ/reentrancy domains, original
frame/DMA/aux/audio observations. Continue priority0 source/live SFX/calc3D then
priority1 damage graph. No playback/hardware/full-game completion claim.

## 2026-10-13 — Priority0 DSP task callbacks and command-mail source

Delivered [DSP task/callback/mail](behavior/audio/MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md).
Six complete bodies pin task publication/init wait, distinct ready/init/resume/
thread-wake callbacks, pending-wait consumption before active gate, command
pointer/size captures, two-mail polls and raw wrapping clock arithmetic. Original
DSP program identity is pinned without decoding/export. Marked local EVT-01
portions complete; parent and actual audio execution remain OPEN.

Validation: generic read-only verifier21 spans (6 complete/13 linked/2 original
program and length),98 anchors/16 resolved direct calls passes. Length1ec0 equals
program span;9 resume-gate/5 mail/6 clock-domain vectors are derived source checks,
not original execution. Fresh full ELFs/listings remain `/tmp`. Link audit747
Markdown files/0 broken; whitespace checks pass. Docs/source metadata only: no
make rerun, native change or original DSP/bank/code-body export. Isolated index
excludes unrelated staged/worktree work.

Next/limits: SDK/DSP task/mail execution, command builder/control/voice/frame
engine and state/thread/clock/pointer/concurrency domains, original startup/resume/
audio traces. Continue priority0 source/live SFX/calc3D then priority1 damage
counter graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Original east pipe and first underground map

[Native underground route](behavior/story/NATIVE_UNDERGROUND_PIPE_ROUTE.md)
packages original gor_02/dokan↔tik_01/dokan_2 records and hit centers/identities.
Case6 retains strict signedmainY>30/contact gating; case10 uses frontcontact,
without A. Original11-command gor pipe departure executes in the existing VM,
sets actual GSF1207, preserves GSW0 and retains party requests. Already-setflag
returns without repeat requests. Expanded metadata4 contains329commands and
64callbacks. Native transactional map staging reaches original tik_01 as room5,
with original geometry/textures/collision/camera and source underground music.
Prologue zero-flag wall display hides are extracted; source hit toggles remain
unbound. Source horizontal endpoint20/start-40 offsets replace provisional0.65.

Inbox deliveries recover exact pipe event/geometry and full161-command Goomba
trio/init/arrival contracts. Original scene waitsMarioX>=-100, uses three actual
c_kuribo tribes and original086..094_01 payloads (last Frankly text in tik_04),
requires battle setup4 and only then advancesstory12. This room import/route
preservesstory11; binding that event/actors/battle is the next milestone.

Validation: isolated staged implementation snapshot /tmp/pm-east-pipe-1mezum8k
passes make test107 CTest/100Python after final pipe ground-query change. Shared
make all and separate DEBUG_SHORT_BOOT artifacts/ttyd-east-pipe build pass.
Focused original pipe and professor tests pass. Initial new pipe landing query
failed because ordinary groundAt has a narrow support window; corrected to
source-start downward-ray bridge. Shared make test initially hit unrelated
in-progress save-block event assertion, then hung in that test; that own run was
stopped and is not reported passing. Snapshot excludes that concurrent unfinished
feature and retains its files. Isolated final Azahar artifacts/azahar-east-pipe-final
passes contact/stick travel to underground, save(room5/story11/flag1), reverse
frontcontact return and save(room2/story11/flag1). Both map captures inspected;
Frankly is correctly hidden on east-room return. Repro fixture tool is
make_east_pipe_fixture.py. No user emulator profile/save touched.

Limits/next: case6 Mario+1e8 producer is not proved identical to native ground
contact; source mode/Vivian/nonride/contact lifecycle, pipe paper poses/SFX/camera
and BOTH-party restoration remain incomplete. Direct emergence/fade and ground
support are native adapters; temporary Frankly underground rendering is pending.
Bind original Goomba trio scene/setup4 and later badge/door/ruins route, actual
lecture setup3, dynamic wall flags/collision and original environment/reverb.
NDSP streams original underground music; audio/device and New2DS performance
were not measured here. Full requested prologue/full-game fidelity remains open.

## 2026-10-13 — Priority0 AI buffers, callback and IRQ initialization

Delivered [AI/IRQ/compressor source](behavior/audio/MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md).
Six complete bodies pin initial IRQ ownership, four-slot allocation/null failure,
state/registration/initial DMA/rate publication, callback rotation/tick-before-gates,
OS enable/dispatch/disable ordering, CPU destination and compressor flag. These
are local source requests, not measured DMA/frame/audio behavior. Marked local
EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier15 spans (6 complete/9 linked),83 anchors
and11 resolved direct calls passes. All256 byte-index arithmetic cases, exact
four-slot extent and9 DSP/logic gate vectors checked; original code not executed.
Fresh complete ELFs/listings remain `/tmp`. Link audit746 Markdown files/0 broken;
whitespace checks pass. Docs/metadata only: no make rerun, native changes or
original code-body/bank export. Isolated index excludes unrelated shared work.

Next/limits: DSP/control/frame callback services, actual state writers and SDK/
physical coherence/alias/concurrency/reinit domains, original interrupt/callback/
audio traces. Continue priority0 source/live SFX/calc3D then priority1 damage
counter graph. No hardware/playback/full-game completion claim.

## 2026-10-13 — Priority0 game allocator providers and MusyX bootstrap

Delivered [game allocator/bootstrap](behavior/audio/GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).
Four complete bodies and scoped SoundInit startup prefix establish original
hook table/providers, fixed game initialization inputs, low-byte configuration,
hardware admission/failure and successful default-cache/resource/child init order.
Game prefix ignores sndInit result; no child failure unwind or residency/playback
proof inferred. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier29 spans (4 complete/1 scoped prefix/
1 original hook table/23 linked),134 anchors/25 resolved direct calls passes.
Independent8-byte table matches actual provider symbols; nine derived clamp/width
vectors checked. Full fresh ELFs/listings remain `/tmp`; no original execution.
Initial link audit742 Markdown files/0 broken; later shared audit743 files found
one unrelated new underground link pending. All6 delivery Markdown files have
0 broken links; whitespace checks pass. Docs/metadata only:
no make rerun, native change or code-body/bank export. Isolated index excludes
unrelated worktree/staged work.

Next/limits: whole SoundInit effects/loader/settings tail, child OS/AR/AI/DSP/IRQ/
frame/seq/synth/stream/virtual-sample/3D, provider/size/reset/concurrency/failure
and original startup/audio traces. Continue priority0 source/live SFX/calc3D,
then priority1 damage graph. No hardware/audio/full-game completion claim.

## 2026-10-13 — Priority0 default cache, resource reset and hook installation

Delivered [cache/reset/hook producers](behavior/audio/MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md).
Seven complete bodies pin gsDefault base/end/cursor and root/current publication,
six U16 registry-count resets, whole macro main-table clear, second-argument-only
ARAM initialization, ordered hook copy, ordinary allocator and callback forwarding.
Reset counts do not erase all registries or prove stopped voices; alias-sensitive
hook copy is preserved. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier9 spans (7 complete/2 linked documented
children),87 anchors and4 resolved direct calls passes. Four derived cache/wrap
vectors,800hex macro-clear extent and3 hook-alias vectors checked. Full fresh
ELFs/listings remain `/tmp`; no original code executes. Link audit741 Markdown
files/0 broken; whitespace checks pass. Docs/metadata only: no make rerun, native
change or original code-body/bank export. Isolated index excludes unrelated work.

Next/limits: actual setup callers/B/N/allocator/callback providers, per-kind
registries, alias/reset/concurrency/residency domains, voice/macro/MIDI/SDK and
original setup/audio/runtime traces. Continue priority0 source and live SFX/calc3D,
then priority1 damage graph. No playback/hardware/full-game completion claim.

## 2026-10-13 — Priority0 generic ARAM upload and hardware stream bridge

Delivered [upload/stream bridge](behavior/audio/MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).
Five complete bodies pin raw upload/full-word priority normalization/user callback,
high/low queue post-before-count/index and hardware stream offset/length alignment,
DCStoreRange and exact init/exit/address delegation. The flush ignores returned
active size; play-buffer wrapper reads an address, not hardware playback position.
Marked local EVT-01 portions complete; full stream/source/runtime remains OPEN.

Validation: generic read-only verifier16 spans (5 complete/11 linked),114 anchors,
14 resolved direct calls passes. Independent whole-DOL aligned caller scan25
sites (17 Flush/1 Get in streamHandle,1 Init in sndStreamAllocEx,5 Exit in
sndStreamFree,1 upload in Flush), each instruction anchored. Eight priority and
42 offset/count vectors remain arithmetic deductions, not original execution.
Full inspection ELFs/listings stay `/tmp`. Link audit739 Markdown files/0 broken;
whitespace checks pass. Docs/source metadata only: no make rerun, native change,
original body/bank export. Isolated index excludes unrelated staged/worktree work.

Next/limits: actual streamHandle/AllocEx/Free caller ownership, source/providers,
valid callback/index/count/offset/extent/lifetime/concurrency domains, OS/cache/
ARQ/DMA and original audio traces. Continue priority0 source/live SFX/calc3D
owners, then priority1 damage graph. No playback/hardware/full-game completion.

## 2026-10-13 — Original lecture choices, talk requests and story11 reload

[Native lecture binding](behavior/story/NATIVE_LECTURE_CHOICES_AND_RELOAD.md)
executes original selection/add/partykill/setup requests. Declining practice via
source results0/1 advances GSW0 to11 and retains the original question window
across084_03/084_04. Both accepted branches preserve Goombella NPC attribution,
visibility/hop and setup3, then stop at the unbound original battle_start.
Source choice opening/closing, press/repeat/cancel/result semantics and separate
window geometry run with original US text. Imported original STICK/cursor pixels
and frame delays; sound requests remain unbound. Glyph-driven talk countdown
replaces window-active pose inference; added original Goombella PKR_T_1.

Story10 reload admits a fresh lecture crossing poll; story11 reload restores
Frankly's source outside position/yaw or flag1207-dependent hidden state.
Save version5 retains actual GSF1207 in byte34 and reads native versions1..4.
Documentation inbox deliveries cover speaker talk/stay ownership, exact flag
and map reentry, NPC visibility and distinct lecture backend ownership.

Validation: make test passes106 CTest/100 Python tests; make all passes. Separate
DEBUG_SHORT_BOOT build-lecture-verify/artifacts/ttyd-lecture-choices build passes.
Tests exercise accepted and declined original branches, retained append,
selection gates/geometry, source icon records, glyph countdown, story10 reentry,
story11 visible/hidden restore and save-v5/legacy compatibility. Isolated Azahar
final run artifacts/azahar-lecture-choices-final passes both choices and verifies
saved originalStory11; captured original windows and returned field inspected.
Separate story11 reload capture artifacts/azahar-lecture-story11-reload/boot.png
shows Frankly outside with ordinary field restored. A prior OCR attempt failed missing English data/incompatible older data, then
was replaced by pixel window detection plus independent native save assertion.
An earlier native pixel run also passed. No user emulator profile/save touched.

Limits/next: implement actual setup3 teaching battle/coordinator and field return,
original pipeout/GSF1207 writer/underground maps, complete NPC/party conversion,
force-restart pose ownership, icon transforms/depth, original selection sounds,
GX fence capture and source event/animation cadence. Native renderer uses source
assets through citro2d/citro3d; physical New2DS performance and full prologue
parity remain unverified. The full-game/prologue goal remains active.

## 2026-10-13 — Priority0 ARAM initialization and stream-buffer owners

Delivered [initialization/hooks](behavior/audio/MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md)
and [stream-buffer lifecycle](behavior/audio/MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md).
Eight complete bodies pin zero allocation/fill/post/wait/free, limits/queue resets,
hook requests,64-descriptor idle chain, cached/downward allocation, low-byte
getter/default pointer and exact free traversal. Larger-candidate predecessor
and global-head cleanup writes are preserved; source list/order/membership
invariants remain required. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier16 spans (8 complete/8 linked SDK/helpers),
251 anchors and14 independently resolved direct calls passes. Derived64-node
chain,8 alignment,7 index and7 cached-search vectors accompany full listings;
no original functions execute. Fresh complete ELFs/listings remain `/tmp`.
Link audit738 Markdown files/0 broken; whitespace checks pass. Docs/metadata only:
no make rerun, native changes or original bank/code-body export. Isolated index
keeps unrelated staged/worktree changes out of this commit.

Next/limits: actual default-cache and allocator-hook producers, valid stream-list/
cycle/predecessor/order/membership domains, upload/SDK/OS/ARQ/DMA and original
sample/voice/runtime traces. Continue priority0 source/backend and live SFX/calc3D
owners, then priority1 damage graph. No original playback/hardware/full-game claim.

## 2026-10-13 — Original object top/side collision and supported checkpoint restore

Commits `4e12f64` and `1d853a6` bind the unchanged original12-face save-block
cube to top support, ordinary Mario forward/around wall queries and checkpoint
validation. PMNPC004 retains original model names so the source hidden-model
floor/wall exclusions remain explicit; older catalogs remain loadable. Dynamic
floor/wall owners stay separate from static REL offsets/array indices. Existing
five-foot support, two-height wall rays, source corrections and landing response
remain shared. Static object geometry caches catalog generation and room/story/
interior/system admission, avoiding rebuilds across unchanged queries/frames.
Successful catalog replacement invalidates reuse; failed loading retains it.
See [native collision](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md).

Source documentation through OW-INTERACT-01 establishes
[ordinary Goombella save-cube admission](behavior/partner/FIELD_PARTNER_SAVE_BLOCK_COLLISION_BINDING.md),
including distinct floor/front/around filters and tie rules. Goombella's current
native floor adapter includes the cube; its complete original wall driver remains
unbound. The file menu/reload path previously rejected on-block checkpoints via
static-only validation. Both now use the same object-aware candidate service
and existing transactional area replacement. Native save elevation quantization
stores the top fixture as.9, then source support resolves actual top.85.

Validation: final isolated source/romfs snapshot
`/tmp/pm-object-surfaces-mkwzez51` passes `make test`104 CTest/100 Python tests
and `make all`. Original-data tests check top/side normals/backfaces, dynamic
owner metadata, hidden-name floor/wall rejection with head geometry retained,
standing, full top jump/landing/walk-off, malformed V4 name atomicity and cache
invalidation after catalog replacement/system gating. The final checkpoint
regression is covered by that complete run. An initial Azahar top capture
exposed static-only checkpoint rejection; it was corrected before acceptance.
Immutable final binary `artifacts/ttyd-saveblock-checkpoint.3dsx` loads the
`--on-block` fixture in isolated `artifacts/azahar-saveblock-top2`; inspected
`saveblock-head-landed.png` shows Mario on the original block after jumping.
No user emulator profile/saves touched. These host/emulator checks establish
this native slice, not matched original execution or New2DS performance.
Documentation links731 files/0 broken before the handoff; whitespace checks pass.
Unrelated concurrent lecture/message/save/audio changes remain outside these commits.

Next/limits: connect source name-contact cache and save callback0..4 with
independently timed S_1/A_1 poses, previous-published loop ratio, nested keys,
system/camera ownership, original US save choices and a native persistence
adapter retaining source switch namespaces. Hit animation/prompt remain unbound.
Source partner body-wall driver, moving objects, dynamic shadow queries,
multiple-map-record hit ordering, full area/event content, original lighting/
translucent ordering and hardware profiling remain open. Keep the full goal active.

## 2026-10-13 — Priority0 ARAM sample store and queue completion

Delivered [sample store/cursor/configuration](behavior/audio/MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md)
and [queue completion/synchronization](behavior/audio/MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md).
Eight complete local bodies pin32-byte wrapping alignment/capacity, direct versus
callback chunks, retained return/fresh destination/cursor, post-before-count/index,
full callback scan and low-only busy sync. Removal only rewinds caller cursor,
ignoring the supplied address. No timeout, invalid-completion guard or fabricated
free/voice-stop semantics. Marked local EVT-01 portions complete; parent OPEN.

Validation: generic read-only verifier16 spans (8 complete/8 linked SDK/helpers),
207 instruction anchors and16 independently resolved direct calls passes.
Derived8 alignment/rewind vectors,256 byte-index arithmetic cases and5 callback
count boundaries checked; these are static arithmetic evidence. Fresh complete
inspection ELFs/listings remain `/tmp/inbox-aram*`. Link audit734 Markdown files/
0 broken; whitespace checks pass. Documentation/source metadata only: no make
rerun, native changes or original code-body/bank export. Isolated commit index
excludes unrelated staged/worktree native work.

Next/limits: aramInit/stream/cache producers and other queue upload paths, actual
callback providers/removal order/queue concurrency, SDK flush/interrupt/ARQ/DMA,
voice/macro/MIDI/resource ownership and original audio/runtime traces. Continue
priority0 initialization/source and live SFX/calc3D, then priority1 damage graph.
No hardware/original playback/full-game equivalence claim.

## 2026-10-13 — Priority0 sample directory lifetime and storage bridge

Delivered [directory/reference lifetime](behavior/audio/MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md)
and [sample hardware bridge](behavior/audio/MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md).
Eight complete local bodies pin preferred-pointer publication, duplicate/capacity
behavior, mutable low16 refs and resolved addresses, acquisition preconditions,
release/removal/compaction and raw hardware/cache requests. First acquisition
requires a matching preferred inactive candidate; no source not-found fallback.
Encoded transfer sizing is proved over the24-bit count domain; it is not audible
length or successful residency. Marked these EVT-01 portions complete; parent OPEN.

Validation: read-only generic verifier13 DOL spans (8 complete/5 linked),256
anchors,10 independently resolved calls and1 original directory resource hash
(no map nodes) passes. Independent original .sdir parse1247 records, all refs0/
formats0; Hammer775/776 source transfers3016/872 bytes.54 derived boundary vectors
and exact-domain arithmetic proof remain static evidence. Fresh full ELFs and
listings remain `/tmp`. Link audit731 Markdown files/0 broken; whitespace checks
pass. Docs/metadata only: no make test/all rerun, native changes or original
asset/code-body export. Unrelated staged/worktree changes excluded.

Next/limits: ARAM store/remove/sync/cache/allocation/DMA services, per-kind pool
registries, bank/source initialization, macro/voice/MIDI stops and actual runtime
residency. Continue priority0 audio source owners and live SFX/calc3D requests,
then priority1 damage/counter graph. No original audio/hardware/full-game claim.

## 2026-10-13 — Original map-object bbox head collision

Commit `12d4b56` connects the original save-block cube to the existing native
five-probe head query and ceiling response. PMNPC003 adds original model bbox
metadata and source object names while retaining PMNPC001/002 loading. The
converter reads original model0xd0..0xe4; source dimensions, forcedY0 midpoint,
eight vertices/six alternating strips, source cull1 and owner attribute are
retained. Dynamic owner keys remain separate from static REL node offsets.
Actual contacts publish the owner; near misses/backfaces do not. Original far
ray endpoint rejects and equal hits retain the earlier static/object slot.
Reused per-Game storage bounds the current plaza object to12 triangles.
See [native binding](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md) and
[asset provenance](ASSETS.md).

Documentation inbox work recovered the
[original shared hit registry/order](behavior/field/DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md):
source map/slot traversal, dynamic insertion, strict upward ties/endpoint,
whole-box ground/wall eligibility and borrowed name/delete lifetime. Current
native packs cover one map record; multiple-record chain reconstruction remains
explicitly pending. Model draw scalar and S_1/A_1 deformation do not change the
source collision cube.

Validation: independent source/romfs snapshot
`/tmp/pm-object-collision-er9tx9p7` passes `make test`104 CTest/100 Python tests
and `make all` after the final endpoint change. Tests cover source bounds,
12 faces/bottom winding, head actual/near-miss/backface/far-endpoint queries,
room/story/system gates, malformed V3 atomicity and a full Game jump that
publishes the owner, clamps, falls and lands. Initial shared full run failed an
unrelated concurrently modified professor lecture assertion; it is not reported
as passing. Snapshot excludes that unfinished lecture/choice work and preserves
its shared edits. Immutable binary `artifacts/ttyd-saveblock-head.3dsx` boots
with `--under-block` fixture in isolated Azahar; inspected contact/landed images
are `artifacts/azahar-saveblock-head/saveblock-head-contact.png` and
`saveblock-head-landed.png`. The images support presentation/landing, while
host assertions establish the collision owner/correction. Emulator samples
are not stable hardware performance evidence. No user profile/saves touched.
Documentation link check727 Markdown files/0 broken; whitespace checks pass.

Next/limits: bind this same source cube into top support and side-wall queries;
connect name-contact caching and callback0..4 with independently timed poses,
previous-published loop ratio, nested key/system/camera ownership, original
save-choice graph and explicit native card persistence adapter. Hit animation
and save prompt are not yet triggered by the new collision. Moving object
transforms, full event/area content, source observations, original lighting/
translucent ordering and New2DS profiling remain open. Full user goal stays active.

## 2026-10-13 — Priority0 group lifecycle and game audio loader source bridge

Delivered [complete group push/pop/resource lists](behavior/audio/MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md)
and [game warm-up/loaded-bank/free](behavior/audio/GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md).
Group push publishes stack pointers before resources, preserves typed pool/list
ranges and FX insertion; pop decrements first, requests voice stops, removes
samples backward and remaining lists forward. Game explicit-list push counts
attempts while sequential mode counts exact successes; warm-up ignores push
returns. Ready and return values are not proof of actual residency. Marked those
local EVT-01 portions complete, leaving child/source/runtime ownership open.

Validation: generic lifecycle manifest29 spans (6 complete/23 linked),476
anchors,56 resolved direct calls and1 original project resource hash passes
(no map nodes). Independent project parse12 groups0..11/2998 IDs; reverse
samples match forward reversal in all12; group6 contains Hammer775/776 samples
and994/995 macros. Fresh complete ELFs/listings and derived report remain `/tmp`.
Link audit726 Markdown files/0 broken links; whitespace checks pass. Docs and
metadata only: no make rerun, native code or original asset/code-body export.
Unrelated staged/worktree edits remain excluded from this commit.

Next/limits: sample-directory/reference/per-kind pool registry producers,
actual file/sample/project/bank initialization and residency, voice-stop/MIDI/
macro/OS/hardware/source/runtime still open. Continue priority0 these source
owners and calc3D/live audio/query bindings, then priority1 damage/counter graph.
No original executed/audio/hardware/full-game completion claim.

## 2026-10-13 — Priority0 FX registry insertion/removal and live resource mutation

Delivered [complete FX registry producers](behavior/audio/MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md).
First insert references the caller's10-byte entry array and writes every byte9
to31 before publishing the fresh group count. Duplicate registration retains
existing pointer/count, increments16-bit refcount and returns0; full refusal
also returns0. Removal decrements refs, compacts12-byte records when zero and
leaves unused tail/entry arrays/voices untouched. Linked synth-start extra
operand therefore comes from live-mutated data, not immutable .proj bytes.
Marked the local EVT-01 producer portion complete; no full residency claim.

Validation: fresh complete two-body listings under `/tmp`; generic registry
manifest5 fingerprints/87 anchors passes. Derived reference boundary metadata
retains0/1/2/65535 wrap. Link audit722 Markdown files/0 broken links; whitespace
checks pass. Docs/metadata only; no make rerun, native/resource conversion or
original ISO/code-body export. Unrelated staged/worktree changes excluded.

Next/limits: sndPushGroup/sndPopGroup and actual project residency, sorting,
resource/voice lifetime on unload, macro/synthStartSound/controller mutations,
calc3D/live audio and original scheduler/encounter/runtime remain open. Continue
priority0 source/core/query binding before lower-priority integration. No original
executed/audio/hardware/full-game equivalence claim.

## 2026-10-13 — Original save-block surfaces and material passes

Commit `c3e36cb` adds the original plaza `mobj_save` presentation through the
shared per-area catalog. PMNPC002 retains original `(185,60,-55)`, scalar10,
S_1, story341 exclusion and constructor system mask0x20. PMANIM4 preserves
original indexed RGBA, untextured draws, discrete texture shifts, material wrap
overrides, source pass/cull and nonzero animation start. Portable triangle
partitioning preserves texture/color interpolation across clamp/repeat/mirror
boundaries. Citro3d submits original opaque/translucent admission and flushes
CPU-written linear vertex data. PMANIM1/2/3 and PMNPC001 remain loadable.
Conversion/resources stay local and ignored; provenance/rights are in
[assets](ASSETS.md) and [surface format](behavior/display/NATIVE_ACTOR_SURFACE_PACK.md).

Documentation work through OW-INTERACT-01/SAVE-01 recovered
[material/UV binding](behavior/display/SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md),
[head cube and callback lifecycle](behavior/field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md)
and the [complete common save event](behavior/field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md).
These establish original contact, five-state callback, previous-published
animation ratio, key ownership, exact original choices, flag namespaces,
cancellation and card error/retry requests. Story text is retained from
msg/US; no replacement dialogue is introduced.

Validation: shared-worktree `make test` passed103 CTest/100 Python tests and
`make all` built the native3DS binary before concurrent fence changes. Isolated
feature snapshot `/tmp/pm-object-verified-j_rg9rov` independently passed103
CTest/100 Python tests and `make all`; its source/romfs were isolated from later
concurrent edits. Dense barycentric tests check all nine wrap combinations,
geometry area and RGBA interpolation; original S_1/A_1 tests check discrete
scrolling, timing, pose wrap and malformed-pack retention. Idle peak402
vertices. Its immutable binary `artifacts/ttyd-saveblock-verified.3dsx` passed
isolated Azahar capture `artifacts/azahar-saveblock-verified2`: original colored
block and animated glow were inspected in the plaza. An earlier3-second cold
launch stopped at title; the successful12-second startup reached the field.
Single emulator HUD sample60fps/sim3.43ms/CPU8.3ms/GPU0.5ms is not a percentile
trace or hardware-performance result. Documentation links721 files/0 broken;
whitespace checks pass. User emulator profile/saves remain untouched.

Limits/next: bind the recovered source collision cube and head-name cache,
then the hit lifecycle with independently timed per-instance poses and original
common save event/card adapter. Save-block hit/save interaction is not yet
connected. Original lighting, global translucent sorting, NPC collision/search,
wandering, area event completeness and New2DS hardware profiling remain open.
Keep the full user goal active; this is an incremental Rogueport milestone.

## 2026-10-13 — Title/file menu and fresh-file opening order adapter

Normal boot now plays the original storybook/market demonstration, title and
four-file menu before starting an empty file's opening. Native controllers own
input separately from field mechanics. A/START skips the demo to title and
opens file selection; D-pad/touch chooses a file, B returns to title. Original
US messages are preloaded and parsed before the new-file sequence starts:
37 demo payloads and11 opening payloads preserve IDs/raw bytes/control tokens
and explicit speaker operands. Diary sets source type7. New-file order is
house → letter → house return/map display → voyage/input gate → original
prologue heading → awakening → arrival → saved harbor story3 at(0,15,225).
Map display retains the source +2/clamp255 counter over128 simulation updates;
source GSW0 writes2/3 are explicit. Existing original saves are not a priority
per the user's steering; slot0 keeps the existing path as a low-cost convention.

Original title/desk/book/letter/map/prologue raster assets, house/voyage/wake
geometry, four character banks and six music streams are converted locally.
House outside/interior snapshots use distinct visibility roots; Luigi retains
idle/talk/read clips. Front-end caches release before field loading and empty
actor-atlas rows are cropped with checked UV scaling. No generated art, rewritten
story dialogue, synthesized tones or redistributed ISO assets were added. See
[asset choices](ASSETS.md) and [opening contract](behavior/story/NATIVE_OPENING_FLOW.md).
`make debug-short` creates a separate `ttyd-2ds-debug-short.3dsx` and build directory
that starts in the prior checkpoint/plaza harness without the opening.

Incremental commits: controller `d47d21f`, test/index registration `e19eda1`,
native/assets/order adapter `a096639`, committed-professor API compatibility
`433a001`. Concurrent work was changing the shared field actor packs and source
APIs during testing. Final reproducible validation used isolated checkout
`/tmp/pm-opening-validation-a096639` with the exact433a001 main.cpp correction,
without incorporating unrelated pending edits. `make test` passes **101 CTest /
96 Python tests** (`/tmp/opening-isolated-test.log`); `make all` passes without
warnings (`/tmp/opening-isolated-build.log`); `make debug-short` passes
(`/tmp/opening-isolated-debug-build.log`). The primary working tree additionally
passed103 CTest/100 Python tests at `/tmp/opening-preload-test.log`, but that
mutable-worktree result is not the reproducible feature baseline. Original-data
playback tests verify all48 IDs, source speakers, automatic demo completion,
source-story writes and voyage input admission. Document audit checks720 Markdown
files with0 broken links; whitespace check passes.

Isolated Azahar fresh-file `tests/azahar_opening_flow.py` passes the full sequence
and verifies a native40-byte saved room1/story3 checkpoint
(`/tmp/opening-isolated-smoke.log`). Reviewed title, file menu, voyage and final
harbor exploration captures; copies are under ignored `artifacts/azahar-opening-isolated/`.
Separate debug-short Azahar boot passes directly into the plaza
(`/tmp/opening-isolated-debug-smoke.log`); reviewed debug-short-boot.png.
Local verified artifacts are `ttyd-2ds-opening-verified.3dsx` and
`ttyd-2ds-debug-short-verified.3dsx`. The isolated source/profile/assets stay under
/tmp; original extracted assets/binaries remain ignored. No user emulator profile
or saves were modified. Xvfb required display-socket escalation. Earlier shared
runs exposed initialization/resource failures and are superseded by this stable
checkout validation; no original-runtime/Dolphin or hardware measurement is claimed.

Limitations/next: this is an **event-order/presentation adapter**, not complete
source cutscene execution. Book/page transforms, original camera interpolation,
NPC movement/yaw/pose ownership, ship transitions, fades, title curtains, diary
border/glyph effects, environment/SFX services and full new-game pouch/card
initialization remain unbound. Preserve original concurrent/branch/control semantics
when replacing snapshots with REL execution. The new-file path reaches story3;
the existing six-message harbor encounter reaches4, but original022..024 battle
entry, tutorial/full Jump continuation,025..033 postbattle/crowd joins and the
subsequent story progression still need native binding before fresh-file playtests
can reach all later events in order. Continue with those source graphs and the
remaining opening choreography; do not call the full opening or remake complete.
The user's goal remains active.

## 2026-10-13 — Original house-exit fence event and live lecture entry

[Native fence/lecture entry](behavior/story/NATIVE_PROFESSOR_FENCE_AND_LECTURE_ENTRY.md)
executes the original63-command wall-removal event through its story9 close-door
contact+A callback. Door-end wait, temporary-party-to-NPC conversion, original
083/084 messages, cameras/movement/hops and source GSW0=10 run in the existing
VM. Expanded original REL catalog preserves318 commands/63 callbacks across
first-talk, fence and lecture. The source lecture crossing poll remains live
with field controls enabled, then runs084_01/084_02 through its original pending
choice service. Added source `<o>`/once_stop retained-window behavior.

Imported actual OFF_d_saku A_1 nonloop140-frame deformation/seven keys. Native
capture bounds unblock the original integer/aspect commands; original78 fence
vertices/materials deform in a frozen camera-plane adapter. Original map hide
and image display ownership remain distinct from actual A_saku_04 collision
removal. Source story>=10 restores collision removal on reload. Saves/exit
saves remain suppressed while the live event lacks persistence. Asset paths,
adaptations and rights are recorded in [assets](ASSETS.md).

Validation: final shared-worktree `make test` passes104 CTest/100 Python tests;
`make all` builds the3DS binary. Separately exported staged source compiles
host/native and passes102 CTest tests after regenerating its field actor format
in `/tmp/professor-fence-stage`; workspace assets are untouched by that rebuild.
Original-data tests complete first-talk, walk to the close-door contact, wait
for capture/animation, compare collision before/after, reach story10, walk the
opened gap and reach the pending original lecture choice. Independent23-paper-
animation samples pass0.00025 local-unit tolerance, max error0.000213623; the
initial generic0.0002 check rejected that large-fall sample, and ordinary default
tolerance remains unchanged. Key/boundary interpolation sampling was expanded.

The first Azahar run overflowed the native checkpoint-restore stack. Main game,
validation/warp candidates and full reset temporaries now use heap storage;
retry `artifacts/azahar-professor-fence-heap/` restores the checkpoint and shows
original083/084, fence gap and restored controls without that exception. Native
`artifacts/azahar-professor-fence-gap/` replay walks through the gap and reaches
original084_02 retained text and the pending choice service. Earlier straight
replays hit the pipe; corrected navigation uses the opened gap. Initial staged-tree
compile raced the file export and lacked main; rerun after export completes
passes. No physical3DS or original runtime trace was obtained.

Next/limits: bind original selection/add services, both lesson branches, actual
lecture battle/return and story10 reload admission. Source offscreen/GX capture,
rectangle clipping/depth/shadow, image clock and SFX/backend/flag consumers are
still partial; current map-material deformation is an explicit handheld adapter.
Earlier harbor/prologue gaps, temporary-party persistence and hardware
performance still prevent full playable prologue parity.

## 2026-10-13 — Priority0 synth FX resource and chained control cores

Delivered [complete synth request cores](behavior/audio/MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md):
loaded-group/shared-scratch lookup, comparator/binary search, default255 and
all16 sound-start operands, voice-generation checks and fresh chain reloads.
Controller cores stop at stale identity; active-gated KeyOff skips stale nodes
and continues. Returns report requests rather than confirmed hardware changes.
Marked the local EVT-01 portion complete; registry/macro/voice/MIDI/resource/
source/runtime dependencies remain explicit and open.

Validation: generic musyx_synth_effect_core_evidence.json verifier12 spans
(8 complete/4 linked identities),192 anchors and independently resolved15 direct
call sites pass. Fresh new full body inspections remain `/tmp`; existing complete
voice lookup identity is revalidated. Link audit719 Markdown files/0 broken
links; whitespace checks pass. Docs/metadata only: no make rerun, native or
original asset/code-body export. Unrelated staged/worktree edits excluded.

Next/limits: FX registry insertion/removal and project residency, actual
synthStartSound/MIDI/external-keyoff/voice-chain producers, calc3D/live audio and
original scheduler/encounter/audio observations remain open. Continue priority0
producer/source/query binding, then priority1 consumer/counter graph. No original
executed/hardware/full-game fidelity claim.

## 2026-10-13 — Priority0 MusyX effect API and voice/IRQ closure

Delivered [eight complete API/lookup/IRQ bodies](behavior/audio/MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).
Public effect wrappers retain raw inputs and core returns; StartEx loads a
studio-indexed default byte with no bounds guard. Check returns the original
external ID when internal mapping exists, not the internal index. The original
repeated backend checks retain fresh lookup/IRQ boundaries. Internal mapping
is an unsigned-key linked-list scan with early miss; MusyX IRQ count/state is
separate from SoundEfxPlayEx's nesting pair and retains16-bit wrap semantics.
Marked the local EVT-01 portion complete while leaving core/runtime work OPEN.

Validation: generic musyx_effect_api_evidence.json verifier12 body hashes
(8 complete/4 linked identities) and77 anchors passes. Eight fresh complete
listings remain under `/tmp`; IRQ boundary vectors distinguish0/1/2/65534/
65535. Link audit717 Markdown files/0 broken links and whitespace checks pass.
Docs/metadata only; no make rerun, native code or original asset/code-body
export. Shared unrelated staged/worktree changes excluded from this commit.

Next/limits: core synthFXStart/controller/KeyOff, voice-map/default-table
producers, resources/macros and hardware/OS/source/runtime need recovery.
Continue priority0 calc3D/live audio and query-specific weapon/menu source
bindings, then priority1 damage/counter consumers. No original audible/executed
trace, hardware or full-game completion claim.

## 2026-10-13 — Priority0 pending SFX startup and effect backend contracts

Delivered [complete pending region](behavior/audio/POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md),
[effect PlayEx](behavior/audio/SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md) and
[five control leaves](behavior/audio/SOUND_EFFECT_RECORD_CONTROL_LEAVES.md).
Queued delay reaches0 before a later start visit; successful startup enters
live processing the same update. Three backend routes retain original volume,
spatial packet, aux and cleanup order. Effect engine uses a separate40-record
pool with first-free/priority<=steal, raw start/controllers, interrupt nesting,
stored narrowed fields, fresh-pool lookup and repeated liveness checks. Check,
Volume,Pitch,Aux1 and Stop preserve their different checks and field widths.
Marked those local EVT-01 portions complete; no full audio-chain completion.

Validation: fresh current SFXMain and six complete effect listings in `/tmp`.
Generic DOL verifier sfx_startup_backend_evidence.json passes25 spans/143
anchors; independent direct-call census50 sites resolves all targets. Arithmetic
check65026 products proves division127;195078 offset/clamp cases pass. Final
link audit716 Markdown files/0 broken links; whitespace checks pass. Metadata/
documentation only: no make rerun, native code, generated tone or asset export.
Unrelated staged/worktree edits remain excluded from the commit.

Next/limits: full calc3D/listener/live volume/fade, stream/song services and
MusyX/resource/emitter/interrupt/hardware/source/runtime still open. Continue
priority0 spatial/core/backend and query-specific weapon/menu source domains,
then priority1 consumer/counter graph. Original execution/audio/hardware/full
remake fidelity remains unverified; no provisional timing is original evidence.

## 2026-10-13 — Tattle flag owners and switch encoding/storage answer

Delivered [complete Tattle flag graph](behavior/battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md)
and rewrote [swdrv storage](behavior/memory/SW_SET.md). Actual rotlwi5 plus
preceding shift27/sign correction implements ordinary signed remainder; valid
nonnegative bit index is modulo32. Init proves global256-word8192-bit and
local16-word512-bit banks,2048/1024-byte banks, and full32-bit global byte0.
ReInit clears only local banks. Tattle setter scans48 lists/247 member occurrences,
writes persistent4474+kind and retained battle bits; checker has policy bypass
and asymmetric original-kind local check. These are not table-population writes.
Marked corresponding EVT-01 and SAVE-01 portions complete; preserved request text.

Validation: fresh14 bodies and table/48 list spans total63 fingerprints;
205 source anchors pass generic verifier with switch_monosiri_flags_evidence.json.
Independent signed arithmetic8951 cases cover global/local/Tattle indices and
signed boundaries. Source groups/kinds and first-match order independently
checked. Link audit712 Markdown files/0 broken links; whitespace checks pass.
Docs/metadata only; no make rerun, native save/code or original asset export.
Inspection outputs remain `/tmp`; unrelated staged/worktree edits excluded.

Next/limits: complete cardMain record/checksum/error/backup/load/timing and
full namespace/writer/scene/save lifetime still open. Continue priority0 SFXMain
used startup/spatial backend, query-specific weapon menu source domains and
original scheduler/encounter traces, then priority1 consumer/counter graph.
No original execution, save round-trip, hardware or complete-game parity claim.

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
