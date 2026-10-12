# Progress and next-session handoff

## 2026-10-12 — Remaining event tail routes and diagnostic call preparation

Implemented `event_tail_opcodes.cpp` for runtime1/3/11/113..118 and the complete
local debug-register classification/call preparation. WAIT_WHILE resolves each
poll and only zero continues. Actual no-op routes query nothing;116 stores raw
metadata.118 independently selects GP/context for each active-only pointer probe,
ignores live capacity and yields after a match or256 probes. Diagnostics retain
context-before-argument order,saved GP reloads,raw flag intersections,direct/
stored float boundaries,template addresses/CR6 and UW/GSW/LSW hex vararg anomalies.
The required formatter owns original sprintf semantics and shared scratch output;
no invented screen/log emission is supplied.

Validation: `make test` passed39/39 CTest suites and64 Python tests; `make all`
passed with current ARM tail object and3dsx. Logs:/tmp/pm-event-tail-test.log and
/tmp/pm-event-tail-build.log. Fixtures cover every tail route,all signed diagnostic
boundaries/normal families/hex/float/integer values,bit31 raw masks,captured/live
context mutation,slot activity/context switches/unbounded-by-capacity probes,and
real dispatcher polling→slot yield→delete. A fixture callback signature typo was
corrected before the successful complete run. No emulator run for unbound services.

Native packed pointer/array/address domains,FPSCR,sprintf/runtime scratch consumers
and original traces remain open. Next:one combined interpreter service path,
native context/bootstrap/storage and original Jump stream/callback binding.
The full remake remains incomplete;EVT-01 documentation breadcrumbs updated.

## 2026-10-12 — Complete original SFX sample library

Completed the user's follow-up to extract GameCube sound effects. Previous
conversion covered only 212 looped samples; `make audio-assets` now extracts
and converts every one of the 1,247 pmario bank samples from read-only G8ME01.
Ignored `local-assets/audio/pcm/samples/` contains 1,035 nonlooped and 212 looped
PCM16 WAVs, named by exact hexadecimal sample ID, retaining source rates/lengths.
Only looped samples have `.wav.loop` sidecars. All six original MusyX bank
resources and their provenance are preserved. Full sample WAV size is
39,383,724 bytes; combined music/sample library is 1,393 WAVs/930,759,732 bytes.
Manifest schema 2 reports converted/looped/nonlooped counts. Music reuse checks
source/decoder fingerprints, WAV metadata/sidecars and PCM hash before reuse;
authored integration fixture proves all-nonloop bank conversion and corrupt
music-cache repair. Implementation commit: 7a9cb6c.

Validation: `make audio-assets` exited 0 with all 1,247 samples and 146 verified
cached music tracks. Independent audit verified all 1,393 WAV formats/full data
lengths/frame counts/hashes/loop sidecars, exact bank-ID coverage and 152 raw
resource hashes. All 2,057 library files are ignored; none is tracked.
Final `make test` passed 38/38 CTest suites and 64 Python tests
(/tmp/pm-sfx-test-final.log). Initial full run failed a core ceiling-contact
assertion during concurrent field edits; rerun after those edits passed.
Final `make all` exited 0 (/tmp/pm-sfx-build-final.log). `git diff --check` passed.
Concurrent field/event/documentation edits were preserved separately. No ISO
write, emulator run or user profile/save mutation was performed.

Next: resolve original effect IDs/macros to samples and implement source-owned
envelopes, pitch, layering and event playback; compare actual effects/loops with
original-game audio. Extracted samples include shared voice/instrument sources
and are not a claim of one finished effect per WAV. Full bank sample extraction
is complete; faithful in-game SFX playback and the full remake remain unfinished.

## 2026-10-12 — Original event memory, user buffers and map allocation

Implemented `event_memory_opcodes.cpp` for74..90 and complete documented local
map allocation/fill/cache leaves. User-buffer assignment/publication retains
count/output/root order and replaces pointers without inferred ownership/free.
Allocator uses best-fit unsigned spans/first ties,mandatory remainder header
including size0,ordered header writes,then zero-fill and cache/syscall services.
Fill preserves byte alignment/word bursts and low8 fill;cache retains original
operand addresses and wrapped CTR behavior. Bitwise raw/resolved RHS,wrapped
60/1000 conversion,raw-address IEEE/word transfer and double-query namespace
operations remain distinct. No ordinary host-pointer dereference is substituted.

Validation: `make test` passed38/38 CTest suites and63 Python tests; `make all`
passed and the ARM memory object/3dsx are newer than source. Logs:
/tmp/pm-event-memory-test.log and /tmp/pm-event-memory-build.log. Fixtures cover
aligned/unaligned fill and extents,cache ordering,best-fit/ties/used/no-fit/zero/
mandatory zero remainder,delayed allocation output/root mutation/replacement,
all opcode routes/raw captures,negative/wrapped time products,IEEE transfer and
resolved/double-resolved query effects. Prior IF/SWITCH streams cover actual
same-dispatch branch/group execution. No emulator run for unbound VM services.

Native memory/heap/cache/platform ownership,wrapped extreme/cycle/alias/numeric
and original script/runtime domains remain open. Delivered heap bootstrap/free
contracts remain next binding work;no per-event automatic free was inferred.
Next:tail/debug opcode routes,context/bootstrap/native address providers,original
Jump streams/callback owners and runtime comparison. Goal remains incomplete;
EVT-01 breadcrumbs updated for the documentation agent.

## 2026-10-12 — Ordinary air phases, landing and fall-base ownership

Previous overworld goal turn made source/test/commit progress;this continuation
inspected the current shared worktree and README/handoff before changes. Goal
remains active/incomplete. Native97fd570 and db71cca replace the provisional
air boolean/counter model with separate current/previous motion, pending entry,
active jump/fall flags, phase/countdown, signed16 counter/peak counter, takeoff
root/base snapshot and vertical tuple. Launch is phase0/counter1 without ascent;
the next manager invocation enters phase10/counter2. Apex integrates the normal
fall tuple once and requests10 late, retaining jump flags for that same call.
Fall entry preserves counter/tuple and integrates even with its camera phase0.
Ceiling-fall entry clears air flags immediately without an extra vertical update
or counter reset. Air cap is now selected after the ascent/head owners.

No-contact ascent/apex retain current support and its separate previous-node
snapshot. Active fall reuses five-foot SearchGround with D=V and V=V, including
its.1-original-unit ray tail, signed candidate outputs and last-query slope
record. Hit snaps root to Q1000 support, clears previous support and calls the
late landing owner;miss integrates velocity. Ground's independently rounded
metadata is retained until its own next writer. Ordinary landing clears the
tuple/counters, clears drift0x80 and selects pending ground motion while air
flags/phase await next dispatch. Attribute0x200 local tuple/root exceptions are
represented, but full water/event routes remain separate. Idle snapshots root
into takeoff height;surviving launch snapshots the final same-call ground motion.

Fall moves horizontally before its base damping/vertical query, so damping
affects next horizontal movement. Native default on-screen state and bypass/
key-control inputs are explicit adapters pending source projection/control
owners. Portable off-screen policy also sets drift suppression. Damping bypass
0x20 survives landing;original LandOn masks clear0x80/0x40. Independent takeoff
base remains unchanged when the retained movement base is damped.

Documentation agent delivered35f82d8/a4df746/ce29235 with
OVERWORLD_AIRBORNE_AND_LANDING.md and read-only source verifier. During recovery,
literal decode corrected draft transition-.298 to-.280 original units;
SearchGround alternate height admission remains literal11 rather thanabs(D);
ceiling correction is Q10plus-minus1, and wrapping mask decode corrected the
LandOn clear from0x20 to0x80. Native binding uses the corrected pinned contract.

Validation:
- Final make test passed36/36 CTest suites and63 Python tests, including evolving
  concurrent event suites. /tmp/pm-air-tests-final.log. New source-derived
  fixtures cover apex versus ceiling flags/pending/counters, phase0 fall gravity,
  retained/peak/signed counter wrap, late landing tuple/flags, idle root snapshot,
  takeoff base independence, native post-move damping order/off-screen next-call
  drift suppression, ground-motion launch matrix and landing's near-drop ray tail.
- Final make all rebuilt native sources,linked elf and built ttyd-2ds.3dsx.
  /tmp/pm-air-build-final.log.
- build-host/core_tests original passed18,315 indexed/full-scan collision probes
  and original-room arrival/return/void/ceiling fixtures;field_wall_tests original
  passed800 directional rays across five original room meshes.
- Current tools/verify_overworld_airborne_and_landing.py passed24 containing
  bodies,170 anchors and18 literals/data spans against read-only G8ME01 ISO.
- On final db71cca build,isolated Azahar passed ceiling-road.bin with
  azahar_ceiling.py (five jumps,landed(-6.7,.1),HP7),harbor-arrival-road.bin with
  azahar_harbor_stairs.py (dock descent/save/return,HP7),and east-road.bin with
  azahar_roads.py (two reciprocal road pairs). Logs:/tmp/pm-air-ceiling-final.log,
  /tmp/pm-air-harbor-final.log,/tmp/pm-air-roads.log;captures/test saves remain
  ignored in artifacts/azahar. User profiles/saves untouched.
- git diff --check passed on the native changes and current documentation.

These are native/static checks,not original Dolphin gameplay comparisons or
New2DS XL hardware measurements. Host float/trig/geometry/quantization remain
adaptations;full command/input/camera/pose/audio/special/dynamic owners are open.

Next native work is now documented by ce29235:bind ordinary low-ceiling ground
arbitration and head-provider projected-tilt/cone/first-admitted-ray near-miss
rules. Current head response still always takeoff-clamps/requestsfall, whereas
the original low-clearance ground request can skip actualroot/tuple/flag writes.
Do not claim head physics closed because the repeated-jump fixture passes.
Documentation agent is recovering requested actual Rogueport road triggers/
conditions and arrival/departure ownership to replace radius.7,dot,cooldown and
endpoint extrapolation guesses. ID1 owns continuing partner terrain/motion docs;
native .08-spaced trail/512-point teleport still needs the composed original
follower. Preserve concurrent event/battle/audio/documentation work in this
shared worktree. Original dialogue/story and runtime/hardware fidelity remain
incomplete;do not fabricate closures or call the remake complete.

## 2026-10-12 — Original SWITCH and grouped CASE state machines

Implemented `event_switch_opcodes.cpp` for34..49 using delivered switch/group
contracts. Eight signed-byte states and signed-word values distinguish ordinary
search/consumed,OR success-1 and AND failure-2. Queries precede state gates and
retain the selected stack offset. BETWEEN reloads its second operand after the
first query;FLAG uses raw any-bit masks. Resolved/raw push capture indices at
original different times. END clears state before pop and retains the value.
Scanners stop on headers,ignore raw35,count34/terminal1,and skip BREAK48.

Validation: `make test` passed37/37 CTest suites and63 Python tests; `make all`
rebuilt the ARM modules and3dsx. Logs:/tmp/pm-event-switch-test.log and
/tmp/pm-event-switch-build.log. Fixtures cover all table routes,signed comparisons,
mutated index/state/value/query ordering,unexpected group states,raw masks,
inclusive/reversed ranges,nested/raw/terminal scans,signed strides and real
OR/AND success/failure streams through dispatcher/assignment/END/wait.

Native packed alias/invalid-depth and original source/group/runtime domains
remain open. Next:memory/user-buffer and tail/debug handlers,then native context/
bootstrap/address/numeric services and original Jump stream/callback binding.
The remake is incomplete;EVT-01 breadcrumbs retain remaining documentation needs.

## 2026-10-12 — Original IF predicates and string memory comparison

Implemented `event_if_opcodes.cpp` for all12..33 from the complete IF contracts.
Integer,raw-mask,string and float routes preserve captured argument base,left
value and post-query second operand/live scan start. Float20..23 retain CR
negations that accept unordered values. False scans consume ELSE/END markers;
ELSE scans ignore ELSE markers,with original low16 opcode/signed16 stride and
terminal1 rules. String comparison ports original alignment-dependent byte/word
loads,unsigned bytes,word mismatch return signs and8041fa2c null replacement.

Validation: `make test` passed36/36 CTest suites and63 Python tests; `make all`
compiled the ARM IF module and rebuilt3dsx. Logs:/tmp/pm-event-if-test.log and
/tmp/pm-event-if-build.log. Fixtures cover all predicates,NaN on either side,
zeros/infinities,signed extrema,raw masks,mutating query/base/next-pointer order,
lexical nesting/signed strides/all string alignment pairs/exact load extent,and
both branches through real dispatcher/assignment/wait integration.

Native address/string allocations,original FPSCR exception behavior and source
script/runtime traces remain open. No story text or original assets were changed.
Next:SWITCH/grouped CASE,remaining memory/debug families,native runtime/context/
address/numeric owners and original Jump stream/callback bindings. Remake goal
remains active and incomplete;EVT-01 documentation breadcrumbs updated.

## 2026-10-12 — Original GOTO and loop continuations

Implemented `event_loop_opcodes.cpp` for original4..8. GOTO retains signed8
label matching, duplicate-first selection, direct negative pointers and live
argument fallback. DO stores raw counts/body addresses. NEXT distinguishes raw0,
wrapped literal countdown and live variable decrement/writeback;its selected
stack offset survives callbacks while zero completion pops the live byte index.
BREAK skips the closing command/count write;CONTINUE fetches the closing header.
Both lexical scans retain original nesting,terminal1/delete2 and signed strides.
Eight semantic body/count slots are cleared by constructors and retained by
restart. Native invalid-depth/packed-alias behavior remains an explicit domain.

Validation: final `make test` passed35/35 CTest suites and63 Python tests; final
`make all` passed. Logs:/tmp/pm-event-loop-test-final.log and
/tmp/pm-event-loop-build-final.log. The earlier full run failed only an airborne
core fixture during concurrent overworld work;that session reconciled the
fixture with documented support retention before the successful rerun. Event
fixtures cover raw/variable/literal0 and negative counts,threshold crossing,
callback-mutated index/body,lexical scans and a same-dispatch variable-count loop
executing arithmetic until a frame wait. No emulator run for unbound VM services.

The remake remains incomplete. Next:delivered IF/string/float predicates and
SWITCH/grouped CASE handlers,remaining memory/user-buffer/debug opcodes,native
context/bootstrap/address/numeric owners,and original Jump stream/callback
bindings. Existing EVT-01 source/runtime/alias/domain requests remain open;
breadcrumbs are in docs/DOCUMENTATION_REQUESTS.md for the documentation agent.

## 2026-10-12 — Assignment, arithmetic and original data cursor handlers

Implemented `event_data_opcodes.cpp` for original50..73 from the assignment and
multiword/float reader documentation. Raw destinations, RHS-before-LHS queries,
wrapped integer arithmetic, MOD's separate+0.5 conversions, single float results
and setter quantization remain distinct. Data lanes advance before source reads,
reload live cursors between outputs and retain the captured argument base.
Indexed73 converts a signed word to a number; sequential69..72 load IEEE bits.
Division and float-to-word conversion require explicit original numeric services.

Validation: `make test` passed34/34 CTest suites and63 Python tests; `make all`
passed. The ARM data-opcode object and3dsx are newer than the added source. Logs:
/tmp/pm-event-data-test.log and /tmp/pm-event-data-build.log. New fixtures cover
raw/resolved assignment, overflow, signed division, MOD conversion side effects,
float rounding/quantization, cursor/output mutation, all reader lane counts,
negative indexed offsets and a complete dispatcher/operand/wait command stream.

Native script/address/cursor sources and original numeric domains remain unbound;
this is not an executed original Jump or story graph. Next:delivered GOTO/loop,
IF/switch and remaining memory opcode bodies, then native runtime/context and
original Jump callback/stream owners. Existing EVT-01 requests remain open.

## 2026-10-12 — Extracted and converted loopable original audio

Completed the requested local audio extraction/conversion from read-only
`/home/carson/games/roms/G8ME01.iso`. Ignored `local-assets/audio/` now holds all146
original STM streams, all six pmario MusyX bank resources,146 PCM16 music WAVs
and212 looped sample WAVs, each with native-compatible `.wav.loop` frame bounds.
The1,247-entry bank directory and358-output manifest retain source IDs/paths,
rates/channels/loops/hashes and original copyright provenance. PCM files total
903,557,248 bytes; raw resources total271,317,240 bytes. Full library is outside
RomFS/source history; current native slice continues to package three tracks.

Added `tools/extract_audio.py` and `make audio-assets` for reproducible conversion.
Shared converter now emits MusyX loop sidecars, reports actual loop flags,
removes stale sidecars for nonloops, rejects invalid bounds/channel loop flags
and protects sources against overwrite. Authored fixtures cover stereo sample
order, intro/loop frame intervals, nonloop/single-sample policy, truncated data,
invalid metadata, missing IDs and source preservation. Incremental implementation
commit61b98a2; concurrent event/field/documentation work preserved separately.

Validation: batch command exited0 with146 streams/212 samples. Separate audit
verified all358 WAV formats/full lengths/frame counts/SHA-256/sidecar bounds and
all152 extracted-resource hashes. Rogueport/battle/victory match existing RomFS
PCM and loop sidecars byte-for-byte. `make test` passed33 CTest suites plus63
Python tests; `make all` exited0 and repackaged3dsx. `git diff --check` passed.
No emulator/hardware listening run, user profile/save write or ISO mutation.

Limitations/next: bind original stream IDs/event conditions and MusyX macros,
envelopes/pitch/voices before using bank samples as faithful game effects. Compare
original ADPCM loop-history playback/audible seams in an isolated original-game
session; PCM region repetition alone is not that verification. Nonlooped bank
samples remain preserved/indexed and individually convertible. Choose only needed
assets for RomFS instead of embedding the full903MB PCM library. Original story,
audio event scheduling, full-game fidelity and remake completion remain open.

## 2026-10-12 — Original event operand namespaces

Implemented `event_operands.cpp` from the complete integer, float and saved-switch
contracts in docs/behavior. Integer and float APIs retain their different signed
routing, encoded-value boundaries, flag semantics, raw/old setter returns and
single-precision quantization. Saved GSW0 remains a full word; other switch bytes
narrow on store. Separate old-value/read/write GP captures and immediate context
lookups remain observable through providers. Address arithmetic and PPC shift
counts wrap explicitly; original float-to-word conversion is a required service.

Validation: `make test` passed33/33 CTest suites and58 Python tests; `make all`
passed and the ARM operand object and3dsx were built. Logs:
/tmp/pm-operands-test.log and /tmp/pm-operands-build.log. Fixtures cover encoded
boundaries, float/integer namespace asymmetry, signed flag addressing, NaN/zero,
old-read-before-conversion and changing saved GP between read/write.

The goal remains incomplete. This host-testable layer needs native address,
context, numeric and save owners; it does not claim packed-layout aliasing or
Gekko FPSCR equivalence. Next:branch/loop/arithmetic/switch/data opcode bodies,
then original Jump stream and native service binding. Existing EVT-01 requests
cover missing source/domain/runtime traces; documentation breadcrumbs updated.

## 2026-10-12 — Ordinary support, slope and upstairs binding

Overworld goal remains active and incomplete. Native commit78eb86b binds the
ordinary domain of OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md. Foot support now runs
before front-step admission while the actual root remains unsnapped. Selected
node, raw support height, separately rounded metadata height, ordered signed
delta and query-owned slope angles have separate storage/outputs. Forward
queries reset/overwrite the slope record even on failure; steep foot rejects
write projected tilts without replacing the previous overall inclination.
Previous angles reduce each enabled movement axis before the air cap and
direction gain. The undocumented center-rise XZ veto was removed.

Known lower-floor loss selects11 and suppresses current-stick drift; all-miss
loss selects10 and admits it. Upstairs begins on the following invocation,
retains support node/base/travel across its first two rises, and immediately
dispatches current input/new A on the third rise before horizontal movement
and resumed ground queries. Vertical velocity persists into walk/dash support
queries after completion; idle clears it. Three additions are retained without
an invented final target snap. Native phase/counter abstractions outside this
bounded controller slice remain provisional.

Asked the documentation agent to resolve double-sided downward-ray orientation
and steep-query write ordering before finalizing the native choices. Delivered
addendum/verifier commit b110e63 establishes that downward and general ray back
hits orient their normals toward the origin before normalization, and pins the
partial slope writes. Native finite-ray and floor providers now orient normals
accordingly. No source executable code or asset format was copied/changed.

Exact validation on the committed native source:
- make test:32/32 CTest suites and58 Python tests passed. New fixtures cover
  independent slope-axis gates before cap,10/11 drift, signed metadata rounding,
  steep query partial writes/front admission, near-drop/lower-floor/all-miss
  support, later rejected-probe delta with retained winning node, first-two
  stair travel/node retention, third-call current input/A and post-stair
  retained velocity versus idle clearing. Log:/tmp/pm-support-tests-final.log.
- make all rebuilt game.cpp,linked ttyd-2ds.elf and built ttyd-2ds.3dsx.
  Log:/tmp/pm-support-build-final.log.
- build-host/core_tests original passed18,315 indexed/full-scan collision
  probes and original-room arrival/return/void/ceiling fixtures.
- build-host/field_wall_tests original passed800 directional rays against
  independent full-scan queries across five original room meshes.
- python3 tools/verify_overworld_support_and_steps.py passed13 DOL bodies,
  132 anchors and24 constants/data spans against the read-only original ISO.
- Isolated Azahar passed harbor-arrival-road.bin with azahar_harbor_stairs.py
  (dock descent/save/return,HP7);ceiling-road.bin with azahar_ceiling.py
  (five jumps,landed checkpoint(-6.7,.1),HP7);east-road.bin with azahar_roads.py
  (two reciprocal plaza/east-road pairs). Logs:/tmp/pm-support-harbor.log,
  /tmp/pm-support-ceiling.log,/tmp/pm-support-roads.log. Captures and test saves
  remain ignored under artifacts/azahar;the user's profile/saves were untouched.
- git diff --check passed for this change before commit.

These are native regression/static-evidence checks,not Dolphin original-game
runtime comparisons or New2DS XL hardware measurements. Host trig/intersection,
scaled float/quantization and spatial traversal remain adaptations. Camera yaw,
full input eligibility/poses/attributes,hazards,dynamic geometry/root matrices,
sound/animation/story owners and actual original traces remain open.

Next:documentation agent is recovering the requested ordinary airborne and
landing contract (mot_jump/mot_fall phase/counter ownership,pending10/11 entry,
motion3 descent,marioJump/marioFall/marioChkLandon query composition and landing
tuple/node/angle writes). Bind that before claiming ordinary air physics closed;
the current landing bounds,fall counter reset and descent-motion collapse are
still partial. Then replace the provisional partner .08-spaced path/512-point
teleport with the composed documented original follower owners;ID1 continues
partner terrain/motion recovery. Road radius/cooldown/endpoints,camera tracking,
original interaction/story/event ownership also require recovered contracts.
Preserve concurrent event/battle/documentation changes in the shared worktree.

## 2026-10-12 — Run/control/block opcode routing and child join

Implemented EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md and control92..112 dispatch
bindings in event_control_opcodes.hpp/cpp. RUN creates independent root flags0
without active-pass append,copies fields after run-check,and publishes ID after
local copies. ID inline/brother capture raw destinations before first-end scans;
brother-ID deliberately omits150 while plain110 copies it. Child94 resolves script,
creates flags0 child,clears owner opcode and returns255 without deleting owner.
Controls preserve integer versus float resolution,setter/stop/start/ID/restart
semantics,raw CheckID output and no self-delete wrapper repair. Ends109/112 delete
and return255/0 respectively. Unsupported routing returns optional empty so other
opcode modules remain required,not no-ops. EventDispatchServices now supports
explicit float resolution and raw event output services.

Validation:make test passed thirty-two CTest suites plus58 Python tests;
make all exited0/no rebuild,with ARM control-opcode object/final3dsx timestamps
following the source edit. New suite covers post-run-check copies,independent
RUN versus joined child,active append policies,output-local aliases,inline/
brother metadata/150 differences,raw sentinel ID output,all setter/control
routes,delete/end/restart return codes and unsupported delegation. Full real
manager→dispatcher→94→child USER_FUNC→delete/copy-back→parent USER_FUNC path
keeps parent alive,propagates child locals and resumes its next command on the
next eligible pass. git diff --check passed. No emulator or original executed
control/menu/lifecycle trace claim. Concurrent field edits preserved separately.

Next:tagged integer/float namespaces,saved storage and branch/arithmetic/loop/
switch/data opcodes,then original Jump event stream and native service owners.
Fixed native battle timers and full remake/story/asset/trace closure remain open.

## 2026-10-12 — Directional walls, launch input and movement-rate binding

Previous goal turn made concrete native/doc/test/commit progress;this continuation
revalidated current README/PROGRESS,worktree and newly delivered contracts rather
than assuming prior state. Goal remains active/incomplete.

Implemented OVERWORLD_WALL_MOVEMENT.md in field_wall.hpp/cpp and bound ordinary
player displacement to whole-vector directional rays instead of X-then-Z circle
endpoints in loaded rooms. Forward heights/third air ray,source-unit signed
quantization,strict closest/tie retention,half-tangent slide,asymmetric side-token
tail,pass-through classifier and first-capped/second-uncapped Around correction
follow recovered local order. Original view flag chooses8/2 rays;ordinary source
init/default0 is explicit,with later writers unbound. Staying probes disposable
positions and leaves actual root untouched. Normal forward contact metadata and
side-distance output remain separate from movement. Complete original-winding
.head geometry supplies finite 3D ray queries with cull/attributes and stable row
order across spatial-index cells. Legacy segment fixtures use double-sided
extrusion;placeholder bounds and partner eligibility remain separate adaptations.
Unsupported named high-bit NPC owners assert at the geometry boundary;all five
current meshes were verified to have none. Main native wall commit:cb114f1.

Implemented OVERWORLD_MOTION_ARBITRATION.md's plain current-input classification
BEFORE surviving jump entry. All nine idle/walk/dash neutral/weak/strong paths
finish base/heading selection under one A sample;ground displacement is discarded
and launch uses air policy. Native no longer replays an airborne/landing-call A
trigger after physics. Physical Explore consumers read published signed axes and
A held/edges,including input-clear behavior;groundedStick preserves integer
radius²>3025 and equality33²+44². Circle Pad mapping/per-axis18 deadzone remain
native device adaptations. Added threshold/current-turn,landing-edge,held-A and
published-byte/chord regressions. Commit:ef37083.

Implemented OVERWORLD_DIRECTION_MOVEMENT_RATE.md's retained-heading/camera-offset
rate AFTER air cap and BEFORE wall movement. Current native fixed Z-facing camera
uses explicit offset0;original camera defaults/writers are not inferred. Cached
unit vectors/trig are numerical adapters. Depth gain is approximately19%,diagonal
approximately13.44%;no second dash cap. Tests distinguish retained heading from
current air drift and camera-offset rotation. Source1/2.25 describe BASE speeds,
not a constant final world rate. Commit:09f162f. Updated mechanics/architecture,
behavior index and original-asset provenance;no conversion or asset distribution.

Validation:final make test passed31 CTest suites plus58 Python tests;make all
rebuilt ttyd-2ds.elf/3dsx after native changes and final call exited0/no rebuild.
Directional original-mesh check passed800 indexed-versus-full-scan rays across
map/gor_00/gor_02/gor_03/gor_04;core original-room check passed18,315 full-scan
collision probes and room arrivals. Static evidence verifiers passed movement
19 bodies/29 anchors/6 constants;wall9/48/17;arbitration20/94/15;direction-rate
6/27/8;new pending support/stairs11/111/22. git diff --check passed. Intermediate
CMake missing-test registration from concurrent creation work resolved when its
file appeared. Old overlap fixture was corrected to source Around preserving
upper support near a wall;floor equality uses appropriate float tolerance.

Isolated Azahar validation,all exit0 with fixture saves/captures only:
- tools/emulator_smoke.py --save-fixture artifacts/fixtures/east-road.bin
  --seconds 4 --input-script tests/azahar_roads.py:two reciprocal east-road pairs.
- tools/emulator_smoke.py --save-fixture artifacts/fixtures/harbor-arrival-road.bin
  --seconds 4 --input-script tests/azahar_harbor_stairs.py:harbor descent/dock/save/
  return,HP7;passed both wall-only and final direction-rate build.
- tools/emulator_smoke.py --save-fixture artifacts/fixtures/ceiling-road.bin
  --seconds 4 --input-script tests/azahar_ceiling.py:five repeated jumps,preserved
  HP7 and position(-6.7,.1),after published-input/launch correction.
Logs stayed in/tmp;captures/test profiles in ignored artifacts/azahar. Display
socket escalation was authorized. User emulator profile/saves untouched. These
are native smoke/host/static checks,not original Dolphin or hardware fidelity.
No ISO write,original binary/dialogue inclusion or invented story content.

Next concrete native task:bind newly delivered OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md
(bc23a45) as a coherent source-owned support/angle/vertical-motion controller.
Feet query precedes front-step while rootY remains unsnapped;angle record is
last-query-owned,not winner-floor normal. Apply previous per-axis slope factors
before air cap/direction rate;distinguish positive-delta fall11 (drift suppressed)
from unknown-support fall10. First two upstairs calls retain base/travel;third
reprocesses current input/newA/ground queries,with retained vertical+7c after
completion unless another owner clears it. Current FieldStep/input/ground paths
still diverge in those cases;do not claim stairs/floors complete from smoke passes.
Documentation ID1 is separately recovering original partner history/homing/
movement;inspect newly delivered FIELD_PARTNER_* contracts before replacing the
provisional .08 sampling/512-point follower. Original ray numerics/traversal,
contact counters/balloons/hazards,full control/pose/ability/transform gates,
actual camera/road/story/dialogue/audio/animation owners and original traces
remain necessary. Preserve concurrent battle/event and documentation edits;
full overworld fidelity and the remake are incomplete.

## 2026-10-12 — Event stop/start traversal,ID scans and setters

Implemented EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md and control-service leaves
from EVENT_CONTROL_OPCODES_AND_ID_SCANS.md in event_controls.hpp/cpp. Traversal
preserves documented compiler-inlined D/G/B/K/S context captures rather than
reloading GP at every conceptual node,uses child-first/live ascending brother
scans,and mutates type-matching flags after descendants. Direct inactive entries/
children still traverse;only slot-scanned attached entries require active1.
All/other/ID scans retain their distinct pointer/ID gates,repeated visits and
captured arrays/live capacities;CheckID captures capacity once and returns first
active match,including stopped/waiting entries. ID controls use hardcoded0xef.
Setters preserve low8 priority/type and global single speed multiplier without
resort/run-check/credit changes. No restart,refcount,catch-up or immediate dispatch
was added by Start. Valid relationship/context domains remain caller obligations.

Validation:make test passed thirty-one CTest suites plus58 Python tests;
make all rebuilt event_controls and ttyd-2ds.elf/3dsx. Suite covers inline versus
recursive GP capture,postorder,nonmatching root/matching descendants,inactive
child versus inactive scanned brother,high masks,preserved flags/time/credit/
callback/local state,repeat stop then single start,all repeated traversal,other
pointer exclusion with transitive reach,duplicate IDs/hard0xef/stopped liveness,
context changes at an inlined boundary and setter narrowing/multiplier. Real
manager stops an entry during dispatch yet consumes remaining credits,skips next
visit and resumes without adding skipped lifetime. git diff --check passed.
No emulator run or original executed pause/context/relationship trace claim.
Concurrent field and documentation edits remain separate.

Next:wire recovered control and run/block opcodes to dispatcher,then tagged
operand/branch/arithmetic/data services and original Jump stream binding. Native
fixed battle timers and full remake/source/scene/asset/trace closure remain open.

## 2026-10-12 — Event deletion,copy-back and restart preservation

Implemented EVENT_DELETE_AND_COPYBACK.md and EVENT_RESTART_PRESERVATION.md in
event_lifecycle.hpp/cpp. Delete captures context before active gate,deletes
waiting child before captured-array ascending live brother scan,reloads parent,
clears its wait/child link and copies local/flag/message words before active clear/
semantic entry reset and8041e860 decrement. Child cancellation can propagate
new grandchild locals upward;brother locals do not copy back and independent
inline entries survive owner deletion. DeleteID removes every live exact match,
retains schedule arrays/count and rereads captured-context capacity each test.
Restart preserves locals/flags/ownership/ID/user/timers/callback/old marker while
resetting script/opcode,speed/credit,lifetime,label tables and run-check. The
intermediate speed1.0 is observable before global speed capture;fetch later resets
old callback state. No immediate parent execution or implicit resource/audio
cleanup was added;valid ownership/context domains remain required.

Validation:final make test passed thirty CTest suites plus58 Python tests;
make all built event_lifecycle and ttyd-2ds.elf/3dsx,then exited0/no rebuild after
extra integration tests. Suite covers inactive preservation,child-before-live-
brother order,cancellation copy-back,independent inline survival,parent inactive/
link mismatch behavior,no copied user/owner/ID/speed fields,duplicate-ID scans,
count underflow,stale schedule retention,restart reset/preserved fields/global
speed observation,all-event run-check and preserved marker until real fetch.
Manager integration deletes a child then resumes parent either later in the same
pass or next pass depending on schedule position;no recursive parent dispatch.
git diff --check passed. No emulator run or original executed lifecycle/capacity/
packed alias/pause trace claim. Concurrent field/docs edits preserved separately.

Next:stop/start relationship traversal and control opcode binding,then remaining
tagged values/branches/arithmetic and original Jump stream/service integration.
Native fixed battle timers and full story/assets/source-owner/trace closure remain
incomplete;goal remains active.

## 2026-10-12 — Child,brother and independent inline construction

Implemented EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md in event_creation.
Common allocation retains the chosen pointer before allocation-counter service
access. Child publishes parent wait-link/flag before clearing destination,wraps
priority+1,copies message/local/user/source fields and links waiting-parent.
Brother retains priority,does not change owner,links brother-owner and leaves
message metadata clear. Both use global speed/zero credit and active-manager
append without root's flags20 gate. Child repairs zero global ID after run-check;
brother repairs before. Root constructor behavior remains covered by its suite.

Attached110/independent107 wrappers scan to first112/109 without nesting,advance
owner before construction and preserve post-run-check recopy. Brother recopies
owner/type/user/170/150 after construction but keeps earlier local copies and
zero metadata. Inline uses independent root links,then copies current metadata/
locals/flags after run-check and does not copy150 or recopy type. These helpers
return handler2;end/deletion/control bodies remain next. No immediate script
execution,joined brother or owner-link for independent inline was invented.

Validation:make test passed twenty-nine CTest suites plus58 Python tests;
make all rebuilt event_creation and ttyd-2ds.elf/3dsx,then exited0/no rebuild.
The prior core overlap landing failure is cleared by current concurrent field
work;core and field_wall suites now pass. New descendant tests cover inherited/
cleared fields,encoded local value copies/shared user tokens,child255→0 and
brother255,parent existing-child overwrite,active/outside append,zero-ID versus
run-check ordering,source mutation during run-check,wrapper recopy distinctions,
label scans past end markers and first-end non-nesting. git diff --check passed.
No emulator run or original executed lifecycle/packed alias/capacity trace claim.

Next:full deletion/copy-back and controls/restart,remaining opcode/operand routing,
then original Jump stream/service binding. Native fixed battle timing and full
remake remain incomplete. Existing source/context/ID/type/time/trace requests
remain needed before claiming original scene equivalence.

## 2026-10-12 — Root event construction,label scanning and run-check

Implemented EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md and
EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md in event_creation.hpp/cpp. EventEntry
now carries constructor/ownership/local/label/message fields semantically.
Constructor scans first inactive slot,increments allocation before clearing,
initializes retained script/type/priority/owner/stack/speed/credit/local/label
state,assigns pre-increment ID and performs final zero-ID repair only after
run-check. Label scan uses low16 opcode,signed count,raw low8 label IDs and
unconditional first-operand reads,including terminal1;opcode2 does not terminate
scan. Append follows label scan,reads live entry flags/ID and does not sort.
Run-check issues current-context StopAll by system level,affecting existing events
and never implicitly resuming. Native free-slot/16-label/script-domain assertions
are boundaries,not original allocation failure or label overflow repair.

Validation:make test ran27 CTest suites;26 passed,including new event_creation,
manager/dispatcher and all battle suites. The core suite currently fails
`tests/core_tests.cpp:543 !overlap.airborne() && overlap.groundHeight==0 &&
overlap.groundContact.sourceNode==2` during concurrent field-wall/ray changes.
Repeated full run retained that failure. Recorded it in the overworld inbox;
those active field source edits remain separate from this constructor commit.
Ran Python checks independently after CTest stopped make test:58 tests passed.
make all rebuilt constructor/event/game objects and ttyd-2ds.elf/3dsx,then exited0
with no rebuild after extra integration tests. git diff --check passed.

Constructor tests cover dirty inactive reset,ascending allocation,narrowed flags/
type/priority,zero locals/links/cache/credit,speed capture,terminal operand read,
labels after opcode2,raw duplicate IDs/all16 slots/low16 distinction,append gates,
post-scan changed flags/ID,all run levels,existing-event stop,independent context,
ID wrap and retained0. Real manager creates a priority255 root during dispatch;
its fractional initial credit delays execution until next pass. No emulator or
original runtime/pause/capacity trace claim. Next:child/brother/inline/delete/
restart/control ownership,tagged values/remaining opcodes,then original Jump.
Full remake and native timer replacement remain incomplete.

## 2026-10-12 — Event fetch,wait and USER_FUNC continuations

Implemented EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md in event_dispatch.hpp/cpp.
EventEntry now retains semantic opcode/argument count/marker,command/argument
address tokens,cached function and shared wait fields. Fetch preserves signed
high16 count,low8 opcode/count,wrapped next pointer and marker reset. Common
handler tails distinguish retained yield,complete/yield,same-invocation advance,
entry termination and manager abort. Unknown opcode>118 follows original default2
advance;all other unsupported opcode bodies require explicit handlers. No
instruction budget,extra synthetic tick or function/operand snapshot was added.
USER_FUNC resolves/caches once,decrements byte argument count and advances its
argument pointer;resumptions pass firstCall0 even for callback phase marker3.
Frame wait uses exact-zero/post-decrement word rules. Millisecond wait captures
lifetime before resolver calls,uses low-word signed delta then unsigned64 division,
ignores saved high on polls,and preserves original quotient/sign/carry formula.

Validation:final make test passed twenty-six CTest suites plus58 Python tests;
make all exited0/no rebuild. ARM event_dispatch.o and final3dsx timestamps follow
the source edit. Tests cover waits0/1/3/negative/INT_MIN,mutated operands without
re-resolution,cached function/live callback args,non1 phase markers,all handler
result classes,unknown high opcode,signed negative fetch count,marker reset,
MSEC zero/no-clock,captured lifetime before mutation,low-word wrap/ignored high,
negative elapsed unsigned-division behavior and negative wait word. Real manager
integration uses two credits on one caller sample and proves MSEC lifetime stays
constant between polls in a visit. git diff --check passed. No emulator run,
original execution/pause/numeric trace claim or complete native interpreter claim.
Manager implementation commit:b31c899.

Next:delivered constructors,label scans,run-check and child/brother/inline/delete/
restart/control ownership,then remaining tagged values/opcodes and actual Jump
stream/service binding. Native fixed battle timers and full remake remain
incomplete;recovery presentation helpers remain pending.

## 2026-10-12 — Original event manager loop and scheduling table

Implemented EVENT_MANAGER_COMPLETE_LOOP.md in event_manager.hpp/cpp. Manager
captures its GP/context independently of priority-table context,reproduces raw
wrapping/signed-clamped tick delta,original clock conversion and large-gap refresh
substitution,and retains captured original timestamp. Schedule sorting is the
unsigned-priority nested exchange algorithm,including unstable equal priorities.
Eligibility preserves active/ID/0x92 checks before lifetime/credit additions.
Credit is rounded/stored then subtracted before dispatch;return1 aborts the pass,
-1 stops one entry,and other results repeat without inner flag/ID revalidation.
Live schedule count permits same-pass appends without resorting. Lifetime advances
once per eligible visit,not once per credit;the manager has no input sampler.
Clock/timestamp/refresh/context producers and dispatch remain explicit services.
Native bounds and valid division/credit domains are invariants,not original clamps.
Semantic64-bit timestamps are not a claim of packed original alias/store ABI.

Validation:final make test passed twenty-five CTest suites plus58 Python tests;
make all rebuilt event_manager.cpp and ttyd-2ds.elf/3dsx. Tests cover clock formula,
negative/large/exact500/wrapped deltas,original timestamp retention,lifetime wrap,
field/battle and changed table-context captures,unsigned unstable ties,each skip
flag/inactive state,stale identity,fractional/multiple credits,store-before-call,
no inner revalidation,per-entry termination/global abort,and live high-priority
append running last. git diff --check passed. No emulator run or original executed
clock/pause/multicredit trace claim. Default refresh is unspecified0;caller must
supply actual source when large-gap substitution needs it. Native Game still uses
its explicit provisional battle adapter until actual script dispatch is bound.

Next:fetch/wait/USER_FUNC continuations,then constructors/ownership/tagged values
and original Jump graph. Recovery geometry/effect wrappers remain pending;new
manager documentation made scheduler implementation the critical next integration
step. Full remake,source owners/story/assets and original traces remain incomplete.

## 2026-10-12 — Overworld air movement, startup and field-battle control

Implemented the documented field battle start/resume/return protocol in
field_battle.hpp/cpp and bound it to native harness control locks. Initial entry
captures NPC, disables Mario/partner, resets result0 and requests sequence4;
return admits cleanup only for sequence2 plus any nonzero result, without
consuming/clearing it. Ordered service tests cover no partner, resumed lookups,
wrong sequences, result0, negative/noncanonical nonzero results and repeat cleanup.
Native harness NPC/party tokens and outcome1/-1 remain development fixtures;
original preparation/post/audio internals, NPC configuration, outcome producers,
sequence lifetime and story/VM integration remain unbound. Commit8d6bf51.

Requested focused missing movement documentation from the documentation agent,
who delivered OVERWORLD_MARIO_MOVEMENT.md and its read-only US ISO verifier
(commits1447844/987bca2). Applied retained base/travel plus current-stick drift
and dash cap to ordinary scale1/no-badge jump/fall. Neutral input preserves travel;
walking/full forward air input gives.012625 instead of selecting dash. Corrected
launch-before-horizontal policy, launch-call overhead query and vertical startup:
begin represents phase0/counter1; next call phase10/counter2 integrates; release
shortening begins at counter4. Head/floor probes retain travel heading when base
is nonzero instead of rotating with current input/combined drift. Zero-base facing
uses native left/right conversion. Added startup/momentum/drift/cap/reset/native
integration tests and a ceiling-edge probe-owner regression. Updated ledgers and
running-jump collision fixtures. Main native movement commit1d2e82a.

Validation: final make test passed25 CTest suites and58 Python tests; make all
rebuilt ttyd-2ds.elf/3dsx after native changes and final call exited0/no rebuild.
Final build-host/core_tests original passed original-room arrivals and18,315
indexed-versus-full-scan collision probes. Movement evidence verifier passed19
containing bodies,29 instruction anchors,six constants. git diff --check passed.
Final isolated Azahar run used tools/emulator_smoke.py --save-fixture
artifacts/fixtures/ceiling-road.bin --seconds4 --input-script
 tests/azahar_ceiling.py: five repeated jumps landed with HP7 and saved position
(-6.7,.1) unchanged; exit0 and ceiling/boot captures in artifacts/azahar. Xvfb
initially failed under sandbox display restrictions; authorized escalation passed.
An intermediate make test hit another contributor's not-yet-created
 event_manager_tests.cpp; after it appeared the complete final run passed. A
separate temporary host rebuild also passed core/movement/original-room checks
while that unrelated registration was incomplete. No user emulator profile/save
was touched; project fixture saves/captures remain ignored. No assets/ISO writes,
original runtime/Dolphin comparisons,story/dialogue rewrites or hardware tests.

Next: docs/OVERWORLD_DOCUMENTATION_REQUESTS.md asks the documentation agent for
complete jump/idle/threshold arbitration,input adapter,wall deflection/slope rate,
modifiers/ledge/landing ownership,partner following/abilities,road triggers and
story gates,interactions and camera. Native vector/hypot math and facing conversion
remain adaptations; arbitrary native interaction priority,partner history/.08
sampling/512-point fallback,road.7 trigger/cooldown60 and camera.07 smoothing are
not recovered original contracts. Implement delivered owners and compare isolated
original traces before claiming complete overworld fidelity. Original story/event/
message/audio/animation and full remake remain incomplete. Concurrent native
battle/event-manager and documentation work was preserved outside these commits.

## 2026-10-12 — Badge recovery amount and HP/FP event wrappers

Implemented BATTLE_BADGE_RECOVERY_SERVICES.md amount/resource wrappers in
battle_badge_recovery.hpp/cpp. Heart/flower queries translate raw actor operands,
retain captured work/raw outputs,return0 for null actors,and take the signed
minimum of unsigned hpDrain/fpDrain counts and retained dealtHp without resetting
that damage accumulator. Resource wrappers capture work before actor resolution,
return2 immediately for translated-1 without unit/remaining-operand reads,resolve
then discard part,and resolve amount before testing HP attributes. HP recovery
heals under attribute40000000 but skips before/after/copy/flag bookkeeping;
ordinary path preserves all those ordered stores. FP delegates to corrected
shared-owner recovery and performs no HP bookkeeping. No effects,delays or
invented availability checks are added;original graph/concurrency still owns them.
Core correction commit:8366eb6.

Validation:final make test passed twenty-four CTest suites plus58 Python tests;
make all built badge recovery and ttyd-2ds.elf/3dsx,then rebuilt current concurrent
field/game edits successfully. New tests cover raw actor/no resolver,null query,
negative retained-damage minimum,raw sentinel output,translated-1 early return,
unit lookup using captured work,discarded part resolution,post-resolution
attribute gate,HP before/after/halfword-copy/flag behavior,partner FP two lookups
and halfword HP overflow. Initial full run failed the core suite during concurrent
field-movement edits;current core executable passed after that work's tests were
updated,and the final full make test/make all rerun passed. Concurrent field paths
are preserved outside this recovery commit. git diff --check passed. No emulator
run or original executed recovery/numerical/presentation equivalence claim.

Next:body/scale/height/parts-position and encoded float/effect wrappers,then the
exact delayed badge recovery child/brother graph through EVT-01 scheduling.
Native badge/pouch/pose/effect/unit owners and full story/assets remain incomplete.

## 2026-10-12 — Resource recovery core ordering correction

Updated recoverUnitFp to match the newly delivered badge-recovery contract:
partner recovery resolves Mario for maximum capture,then resolves Mario again
for writable current FP. The captured signed-halfword maximum survives registry
changes between lookups. Other units retain own-resource behavior. Added the
recovered HP core:wrapped addition,narrow16 before upper-only cap,no display
bookkeeping or lower clamp. These are shared recovery primitives for direct
application and forthcoming original badge/turn event wrappers.

Validation:make test passed twenty-two CTest suites plus58 Python tests,including
the concurrently committed field_battle suite;make all rebuilt resource/game
objects and ttyd-2ds.elf/3dsx. New regression changes the Mario owner between
lookups and verifies second-owner FP capped against first-owner maximum.
HP cases cover upper cap,negative result,halfword overflow and no bookkeeping
mutation. Existing direct damage/resource/targeting suites remain green. No
emulator run for this service-ordering correction;no original execution claim.
Next:recover HP/FP event wrappers and badge recovery amount queries,then original
recovery/Jump graph scheduling. Full remake remains incomplete.

## 2026-10-12 — Private takeoff sampler/chooser binding

Connected recovered sampler and chooser to jumpTakeoffPositionEvent through an
explicit overload. Each invocation owns its CandidateBuffer on its stack;the
existing injected sample/choose API remains available. Shared takeoff logic
preserves original input/service/output order. CandidateChoiceOutputs separates
output destination ownership from math/registry/status service context,so local
chosen-ID sinks do not replace the original callbacks' user state. After choice,
the wrapper rereads private signed count before its branch. Exact takeoff masks,
null weapon,mode4 and independent work captures flow through the recovered cores.
Fresh native buffer initialization is not evidence of original stack padding;
valid recovered paths consume initialized fields only.

Validation before concurrent field-battle drafts:make test passed twenty-one
CTest suites plus58 Python tests;make all exited0/no rebuild. Source/object/build
timestamps confirm takeoff and choice ARM objects were rebuilt after the edits
and ttyd-2ds.3dsx was linked afterward. Integration suite uses real row construction,
admission,occlusion,permutation,weight selection and hit positions to check full
outputs50/7/40,swapped-Q negative offset,empty count with/without Q,and nesting.
A nested invocation chooses a different row and emits160/7/40,then outer output
remains50/7/40;shared candidate bytes remain unchanged. No choice-output or RNG
hooks are supplied to the composed binding. Existing injected-wrapper and chooser
suites still pass. git diff --check passed. No emulator run or original executed
position/numerical/pause trace claim. Concurrent field entry code/docs/test drafts
arrived after validation and are preserved outside this targeting commit.

Next:bind actual registry/part/source/menu/pouch/math/audio owners and original
Jump graph through EVT-01 scheduler. ID2 now holds the scheduler recovery lock.
Native provisional battle timer/one-hit Jump and full remake remain incomplete.

## 2026-10-12 — Candidate sampling and choice composition

Implemented BATTLE_ENEMY_CANDIDATE_SAMPLING.md in battle_candidate_sampling.hpp/
cpp. Sampling retains metadata and distinct swap-Q,constructs registry/linked
part rows with independent hit/base/sort coordinates,bias/forward/lane fields,
and retains unused padding. Ordered A/C admission compacts whole rows,re-fetches
units/parts,preserves repeated information lookup and self exception;later
same-unit part dominance and occlusion operate on retained rows with actor-data
pointer exception. Only permutation is exchange-sorted using wrapped keys and
positive versus nonpositive equal-key rules. Update uses retained metadata
without repeating the entry swap. High-bit admission sets signed count-1 while
leaving selection/rows/permutation. Native record/link/numeric asserts are valid-
domain boundaries,not invented original74-row capacity enforcement. Providers
retain actual unit/status/part/geometry/info ownership;weapon pointer is semantic.

Read-only US ISO inspection with inspect_dol_function.py resolved one ambiguous
reset order:800ff94c writes selection0 after construction,800ff950 publishes initial
count,then800ff954 reloads it. Listings/ELF remain /tmp/pm-sampling-order.*;no ISO
write or original data commit. Requested documentation-agent confirmation/pinning
in the inbox;this is static evidence,not Dolphin/runtime observation.

Validation:final make test passed twenty CTest suites plus58 Python tests;
make all rebuilt sampler and ttyd-2ds.elf/3dsx without warnings,then exited0/no
rebuild after final integration tests. Initial misleading-indentation warnings
were corrected and checks rerun. Tests cover swap/no-swap Q and update ownership,
high-bit retention,signed row geometry/bias/padding/forward cap,all A admission
classes and C attribute checks,information lookup repetitions,status27/part skips,
ordered compaction,same-unit dominance after earlier removal,normal/reversed/lane/
equal-X occlusion,actor pointer exception,biased permutation and equal-key tie
order. Real sampler output composes with chooser using exact takeoff masks,null
weapon and mode4;it selects registry2/part1 without any supplied RNG service.
git diff --check passed. No emulator run for unbound registry/source/event paths
or original execution/numerical/domain equivalence claim.

Next:connect sample/choice to the takeoff wrapper through a private per-invocation
buffer,then bind original Jump graph and EVT-01 scheduler with real unit/menu/
pouch/math/audio owners. Native harness's fixed timers and one-hit Jump remain
provisional. Full remake,story/assets and original trace comparison remain open.

## 2026-10-12 — Candidate choice weights,overrides and RNG boundary

Implemented BATTLE_ENEMY_CANDIDATE_CHOICE.md in battle_candidate_choice.hpp/cpp.
Chooser captures work and emits initial(-1,0),retains actor/first-row bonus,
reloads signed count/permutation while building74 physical weights,and applies
all enabled modifiers with separate binary32 operations/fused formulas and
immediate fctiwz-service truncation. Weapon overrides preserve status/recovery
short circuits,flag overwrite followed by later item handling,repeated FP/table
reads and reloaded weapon item. Deterministic scan uses strict signed greater
and lower physical-row ties;weighted scan retains word sums and R<=0 subtraction
boundary,including draw0 and possible row74 metadata reads. It does not modify
buffer/permutation or own/reseed shared RNG. Signed numeric/status/FP/data/RNG
services stay explicit;invalid weight-index/conversion domains are native asserts,
not original capacity clamps. Semantic weapon pointer is separate from original
record byte storage;this is not a packed original pointer ABI.

Validation:make test passed nineteen CTest suites plus58 Python tests;make all
exited0/no rebuild,with build/battle_candidate_choice.o and ttyd-2ds.3dsx present
at the current build timestamp. Suite covers empty/negative count outputs,ordered
aliased row writes,mode4 null-weapon zero RNG use,each modifier and ordered
intermediate truncations,physical/permutation ties,negative/no-positive weights,
draw0/prefix/exhausted-row boundaries,zero-sum irand without state consumption,
recovery/status short circuits,all listed status items,flag1 override continuing
into FP scaling,repeated FP reads and reloaded/short-circuited ItemData queries.
git diff --check passed. No emulator run or original runtime/numerical/RNG
interleaving equivalence claim for these still-unbound choice providers.

Next:implement delivered candidate row construction/admission/occlusion/permutation
sampler,then compose it and choice with takeoff and actual Jump graph/EVT-01
scheduler. Native unit/part/source/status/pouch/menu/math/audio ownership and
original traces remain needed;full remake remains incomplete.

## 2026-10-12 — Retained selected target and weapon cursor services

Implemented BATTLE_SELECTED_TARGET_AND_WEAPON.md in battle_selection.hpp/cpp.
Retained target output preserves signed count/cursor/permutation/row arithmetic,
raw destinations,and the part-ID read after the first output callback. Negative
count produces two ordered-1 writes without sampling. Command weapon captures
work before resolution but actor lookup reloads current work. Mario/partners
read captured menu/type while the cursor helper unconditionally fetches the
current acting unit;types5/14 use that unit's kind,not the queried actor's kind.
Other kinds use wrapped held-item ItemData weapon-pointer arithmetic. Cursor
mapping preserves all documented branches and untouched output on unsupported
modes. Unsupported selected-weapon domains return optional invalid status as an
explicit host boundary,not an invented original return or default weapon.

Original address arithmetic uses unsigned32 tokens with provider-owned byte/
halfword/word access;no original RAM address is dereferenced natively or copied
as an executable. Native menu/selection producers and original asset-to-record
mapping still need binding. Services never resample or perform eligibility checks.

Validation:final make test passed eighteen CTest suites plus58 Python tests;
make all built ttyd-2ds.elf/3dsx,then exited0/no rebuild after final extra test.
New suite covers count0/-1/-128,negative cursor/permutation indices,signed IDs,
part-ID mutation between ordered aliased output writes,all cursor table modes,
unchanged invalid cursor,types5/14 kind branches,captured versus current work,
queried actor versus distinct acting-unit kind,wrapped negative row/held-item
addresses,zero weapon pointers and raw sentinel weapon output destinations.
git diff --check passed. No emulator run for these still-unbound native menu
records or claim of original UI/runtime equivalence.

Next:implement newly delivered candidate sampling/choice contracts,then compose
original Jump graph through EVT-01 scheduler/dispatcher services and real shared
unit/menu/pouch owners. Elevated EVT-01 visibility in the documentation inbox:
fixed native attack timers remain provisional until actual scheduling is bound.
Original UI/equipment/pose/audio/math and full story/assets remain incomplete.

## 2026-10-12 — Weapon costs,affordability and ordered payment

Implemented BATTLE_WEAPON_COST_AND_PAYMENT.md in battle_weapon_cost.hpp/cpp.
Weapon fields now retain signed item ID and distinct FP/AP bytes;unit retains
unsigned2f5 cost-reduction byte. Cost captures initial item/base FP,queries badge
copies then reloads item before exponential/linear dispatch,uses wrapped words,
zero-preserving reduction and final signed999 cap. Numeric pow/fctiwz remain
explicit original-service dependencies. Affordability captures FP before cost,
skips AP read when FP fails,and rereads AP cost byte after AP query. Payment
captures work,reads FP/cost,sets wrapped FP,then rereads kind for point/count
records and always requests AddAP,including zero. It does not check affordability.

Added full200 signed pouch-slot badge count,low16-before-lower/upper AP addition,
byte-wrap-before-point-cap and corrupt-count-preserving helpers. Pouch helper is
semantic byte-record access,not packed original global-pointer aliasing or disk
save serialization. Event payment preserves actor/weapon resolution before actor
translation/lookup and delegates resolved weapon-address ownership to its adapter.

Validation:final make test passed seventeen CTest suites plus58 Python tests;
make all built ttyd-2ds.elf/3dsx,then exited0/no rebuild after the final event test.
Tests cover every scaling group,zero copies/zero cost,reduction floors,item reload
with captured base,999 cost and231 point delta,FP-before-AP and AP-byte rereads,
unaffordable payment,partner FP routed through the existing resource service to
Mario slot37,negative FP halfword wrap,zero-cost action counts,kind mutation after
FP setter,200-slot signed/zero badge counting,AP overflow/negative maximum clamp,
action byte overflow/corrupt counts and payment event ordering. git diff --check
passed. No emulator run for unbound payment/menu/pouch routes. Native default
weapon cost fields remain0;no fabricated equipment,payment UI or save ownership.

Next:implement delivered selected-target/weapon cursor services,then bind native
original menu/pouch/equipment and Jump graph/EVT-01 scheduling. Badge2f5 producer,
AP/pouch/action-record owners and numerical exceptional/FPSCR comparisons remain
needed. Full remake remains incomplete.

## 2026-10-12 — Original Jump takeoff-position wrapper

Implemented BATTLE_JUMP_TAKEOFF_POSITION.md in battle_jump_takeoff.hpp/cpp.
Captures shared work before ordered input resolution;gets actor currentX and
homeY,requested target hitXZ,then requests a private sample with null weapon,
exact masks01001240/01000000,signed narrowed enemy side and wrapped negated bias.
Chooser mode4 always runs;its return is ignored and positive signed sample count
alone selects candidate processing. Closest-X comparison preserves ties/unordered
cases,part pointer identity selects60/30,and a fresh signed side byte controls
the sampled-return offset. Y/Z stay tied to actor home/requested target. All three
raw output destinations are converted/stored in order,including sentinels/aliases.
Private candidate/count/output initialization remains a sampling-service
obligation;there is no fabricated nearest-target policy or shared attack buffer.

Validation:final make test passed sixteen CTest suites plus58 Python tests;
make all built takeoff and ttyd-2ds.elf/3dsx,then exited0 without rebuild after
final tests. Suite covers request/resolution/work-capture order,literal sampling
arguments,positive/zero/negative sample counts,ignored chooser return,closest/
tie/farther/unordered X,pointer-equal offsets,reversed sampled return,signed byte
narrowing/wrap and side rereads,no-skip/aliased outputs,and takeoff integer outputs
composed with the existing approach query. Initial test expected22 calls for the
nonpositive branch;corrected to the observed/documented20 (candidate queries are
omitted),then reran all checks. git diff --check passed. No emulator run for this
still-unbound service-level path or original position/RNG/timing equivalence claim.

Next:recover/bind sampling and choice core services,original event scheduler and
Jump graph,then connect actual shared unit/part/scale/math/audio ownership.
Existing JUMP-01/DMG-02/EVT-01 requests remain necessary. Native harness still uses
its provisional schedule and one-hit policy;full remake remains incomplete.

## 2026-10-12 — Unit and part position services

Implemented BATTLE_UNIT_PART_POSITION_QUERIES.md in battle_position.hpp/cpp.
Semantic views point at owner-supplied home/visible/scale/facing/belong fields,
so motion and resource owners need not have copied position state. Ordered leaf
getters preserve output/source aliasing. World position distinguishes absolute
parts from relative offsets using owner189;hit position captures its separate
side factor and applies both scaled offset layers with scale rereads per axis,
rounded X/Y products and fused Z products. Linked part lookup preserves negative
head selection and single-part nonmatching-ID behavior. Event wrapper keeps
separate unit/registry requests,raw sentinel outputs,integer truncation and
ordered writes. No native Crump coordinate fixture was promoted to original data.

Validation:make test passed fifteen CTest suites plus58 Python tests;make all
built ttyd-2ds.elf/3dsx,then exited0 with no rebuild after the final extra test.
Tests cover home versus visible sources,ordered aliased getters,absolute/relative
world positions,distinct facing/side factors,explicit hit unit versus part owner,
scale/output alias rereads,fused Z versus separately rounded X/Y (8193 versus8192),
linked/single-part lookup,negative fractional integer outputs,skipped and aliased
raw destinations,and original Jump's integer hitZ+5 input preparation.
git diff --check passed. No emulator run for these still-unbound position paths,
original trajectory/FPSCR comparison or complete-game claim.

Next:implement delivered takeoff wrapper with explicit private sampling/choice
services,then bind original approach/Jump graph and scheduler. Candidate admission,
chooser/RNG/count outputs,unit/part source writers,pose/scale lifetime and executed
positions remain pending documentation/trace dependencies. Native damage/effect
coordinates and original script/scene ownership still need binding. Full remake
and goal remain incomplete.

## 2026-10-12 — Movement sound control and approach composition

Implemented BATTLE_MOVEMENT_SOUND_CONTROL.md in battle_movement_sound.hpp/cpp.
The complete local core distinguishes empty records,single-key retained voices
and periodic alternation. Single-key trigger returns are stored even when-1,
then fresh unit/part geometry feeds the position request. Periodic triggers
retain signed-halfword countdown/wrap/parity/reset behavior and ignore returned
handles. Unit wrapper performs its own position read before core's potential
second read;end requests Off before assigning handle-1 and leaves keys/counters.
Adapter composes these services with the recovered MovePosition callback.
Original trigger/position/off and geometry bindings are explicit service hooks.

Validation:make test passed fourteen CTest suites plus58 Python tests;make all
built ttyd-2ds.elf and ttyd-2ds.3dsx. Sound suite covers empty no-op,retained voice
trigger/fresh-position/stop order,failed-handle update/retrigger,part geometry,
period2 three-call expiry,negative disable,zero selected key,32767/65535 counter
wrap,ignored periodic handles and actual loader→blocked move control→terminal
snap/end composition. git diff --check passed. No emulator run for these still-
unbound callback services or claim of original audio/trajectory/FPSCR equality.
No generated samples,source asset changes or user profile/save modifications.

Updated README and JUMP-01 with implementation paths and explicit pending
BtlUnit_snd_se_pos/psndSFX_pos/Off,bank/macro/source-writer and executed-trace
requests. Documentation-agent research commits preserved. Next:bind takeoff/
unit/part hit-position services,original event scheduler and Jump request graph;
replace the native timer only with supported composition. Native math/audio,
pouch/equipment/partner lifecycle and full story/assets remain incomplete.

## 2026-10-12 — ID2: requested Jump preparation,positions and movement sound

Added BATTLE_MOVE_FRAME_AND_POSITION.md with complete query/movement bodies.
Query resolves integer X/Z,ignored Y,float speed and raw destination;zero speed
uses unit speed then local1 fallback. Its fused residual gives a ceiling-like
estimate,exact-zero count1 and no unit mutation. MovePosition resolves integer
XYZ/count/sound on every call,uses truncated automatic count/exact-zero1 and
speed adjustment,integrates on first call,horizontal baseline only,then snaps
all targetXYZ and returns1 for n<=0. Query and motion counts differ;first-attack
abs(query-22) delay remains its original script rule,not a fixed timing claim.

Added BATTLE_JUMP_TAKEOFF_POSITION.md with complete mixed target-hit X/Z plus
actor-home Y output,private null-weapon candidate sampling/choice4,signed count,
closer-X tie rule,part-pointer/return/sign offsets and ordered integer outputs.
Candidate core/RNG/validity remain unclosed;no guessed deterministic selection.
Added BATTLE_UNIT_PART_POSITION_QUERIES.md with complete home/offset/world/
scaled-hit leaves,registry-part helper and event integer/raw-sentinel wrapper.
Relative part X uses owner189;scaled hit X uses captured side byte,with two
ordered triplets through unit114. Source/world/scale/lifetime writers remain open.

Added BATTLE_MOVEMENT_SOUND_CONTROL.md with complete retained voice/position
route,alternating signed16 period/parity route and stop wrappers. Only record18
retains a trigger return;keys0/4 are inputs. Movement invokes normal sound only
while blocked and ends retained voice after final snap. Original trigger/resource/
macro/audio internals remain open. MOVE/TAKEOFF/POS/MOVE-SE requirement cases
are implementable local contracts,not original executed motion/audio traces.

Linked partial JUMP-01/DMG-02 answers and released only ID2's validated preparation/
geometry/sound locks. Updated old local-motion dependency summaries to prevent
duplicate recovery;independent ID1 lecture-player work and native motion/resource/
action-service work preserved. ID2 authored no native gameplay implementation.

Validation:tools/verify_jump_preparation.py passed13 whole-body fingerprints,
68 instruction anchors,five constants and six concrete ordinary caller records;
two fresh metadata reports repeated byte-for-byte. Mario motion verifier passed
five bodies/76 anchors/20 constants;Jump graph verifier passed19 bodies/57 anchors/
all268 requests/census;general verifier passed prior contracts. make test passed
13 CTest suites including concurrent movement/action/motion implementation and
58 Python tests;make all exited0,no rebuild;744 behavior local links resolved;
git diff --check passed. /tmp holds all original inspections/reports;none staged.
No original execution/emulator,ISO/profile/save mutation,asset conversion or
dialogue rewrite by ID2. Only explicit documentation/verifier paths committed.

Next:JUMP-01 selected-target/weapon/cost/power-child/stylish/prize-byte/aftereffect/
badge-tail internals and candidate/math/audio/source/domain composition;DMG-02
height/belong/action-record/audience/revival/resource bindings and STATUS turn/
snapshot callers. Original input/state/RNG/position/audio/scene comparisons
remain necessary. Full-system documentation and complete-game requirements
remain active/incomplete;completed local contracts do not close all requests.

## 2026-10-12 — Portable approach query and MovePosition

Implemented BATTLE_MOVE_FRAME_AND_POSITION.md in battle_move_position.hpp/cpp.
GetMoveFrame resolves discardedY and float speed separately,uses local signed-
zero fallbacks without changing unit speed,preserves fused residual/positive
increment/word wrap/exact-zero minimum,and skips only raw sentinel destinations.
MovePosition resolves integer XYZ/count/sound arguments on every invocation,
initializes once,uses its own automatic truncated count/speed residual adjustment,
loads the selected sound record,integrates on the first call and snaps all visible
axes at signed n<=0. BaselineY stays independent. Shared horizontal helper
performs both sin/cos requests before ordered separate-product X/Z writes,
including aliased destinations. Sound control/end remain explicit service hooks.

Validation:make test passed thirteen CTest suites plus58 Python tests;make all
built ttyd-2ds.elf and ttyd-2ds.3dsx. Derived tests cover query21 versus movement20
atD101/S5,zero/negative/overflow/NaN count paths,speed fallback versus movement's
unrepaired zero speed,sentinel output versus computed value,resolver order,
first/resumed integration,full terminal snap with retained baseline,negative n,
sound-request order,raw sentinel coordinates and trig-call/alias write order.
No emulator run for still-unbound event services;no original numerical/FPSCR/
trajectory equivalence claim. Sentinel correction commit:24baff7. Next:implement
new movement sound core,then bind takeoff/approach/original Jump graph and event
scheduler. Original numeric/audio/unit/pouch/story ownership remains incomplete.

## 2026-10-12 — Mario movement-baseline sentinel correction

Review of the delivered approach contract exposed a MarioJumpPosition bug:
its SetMoveCurrentPos was represented as whole-vector assignment. It now uses
the original per-axis binary32 sentinel rule,retaining prior baseline axes when
visible coordinates equal-250000000. Heading/distance/direction still use the
raw visible position temporaries,not the sentinel-preserved baseline. Target
setter now shares the same explicit helper. Added a derived test with all three
visible axes equal sentinel and retained prior baseline. New optional cos and
float-resolver service slots support the next recovered approach implementation.

Validation:make test passed twelve CTest suites plus58 Python tests;make all
rebuilt motion and ttyd-2ds.3dsx. No emulator run for this unbound service path.
Original math,FPSCR and movement/event ownership remain open. Next:GetMoveFrame
query and MovePosition approach;retain their distinct count policies and sound
callbacks. Full remake remains incomplete.

## 2026-10-12 — Native command-service request binding

Native event adapter now uses StartAC/ResultAC/StopAC portable services for Jump
and Hammer instead of direct state/evaluation/pending checks. Callback pointers
follow the installed command's live flags;shared fields stay referenced.
Start requests state100 after the manager initializer,and Jump requests help2
unless actor statusFlags10 suppresses it. Result invokes evaluator then polls
stored pending bit;Stop invokes deletion then help0. lastActionHelpRequest records
requests for the still-unbound original help UI. Harness operand resolver handles
only its literals;this is not the original tagged event interpreter. Existing
impact/return schedules,temporary one-hit Jump policy and full original script
request order remain provisional. Portable service commit:717da56.

Validation:final make test passed twelve CTest suites plus58 Python tests;
make all rebuilt ttyd-2ds.elf/3dsx. Core assertions cover native Jump start help2/
state100 and completed result/stop help0/state1000 with callbacks still live.
A new assertion initially used a nonexistent getter;corrected to explicit new
jumpAction accessor and reran all checks. Isolated emulator command
`python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_hammer.py`
exited0;reviewed Hammer screenshots show charged release Nice2/CrumpHP3,
early release Hit1/HP2,and overrun Hit1/HP1. These broad wall-time screenshots
are not tick assertions,original equivalence evidence or hardware measurements.
No user emulator profile/save modification. git diff --check passed.

EVT-01 requests complete scheduler/dispatch/constructor/continuation semantics
needed before native original Jump request scheduling. Existing math/audio,
approach/position,unit/pouch/equipment and lecture/tutorial requests remain open.
Concurrent documentation-agent locks/drafts preserved. Next:implement recovered
scheduler and original Jump graph,then bind approach/position/math/audio/feedback
services and compare original per-hit input/trajectory traces. Full-game story,
assets and native resource lifecycle remain incomplete;goal remains active.

## 2026-10-12 — Portable action-command script services

Implemented BATTLE_ACTION_COMMAND_CALLBACKS.md and
BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md in include/battle_action_services.hpp
and source/battle_action_services.cpp. Shared command fields are referenced,
not copied. Parameter stores remain observable between operand resolutions;
sentinel values retain prior fields. Raw setup flags/options/output destinations
remain distinct. Start requests state100 and conditional help2. Result invokes
the evaluator but polls the stored pending bit independently of callback return.
Stored result/count and success-window readers preserve raw destinations. Stop
calls the installed delete callback before help0 without clearing command data.
Prize helper preserves low-code30 early return,late unsigned-byte read,wrapped
arithmetic/clamp,and distinct captured-work versus actor-lookup service reads.

Validation:make test passed twelve CTest suites plus58 Python tests;make all
built ttyd-2ds.elf and ttyd-2ds.3dsx. New suite covers ordered writes/raw fields,
callback-return versus pending-bit behavior,readers without evaluation,window
output,conditional help,delayed/repeated TimingA stop,prize early-code/byte/wrap,
null actor aliases and work capture/lookup ordering. Initial test literal needed
signed encoding;3DS int32_t differs from host int,so clamp now explicitly uses
int32_t. Final tests/build pass. No emulator run for these still-unbound services.
Next:bind native request adapter,then original Jump graph/VM cadence and help/
prize/UI producers. Full remake remains incomplete.

## 2026-10-12 — Mario Jump count preparation and motion composition

Added MarioJumpParam to source/battle_jump_motion.cpp using
BATTLE_MARIO_JUMP_PARAMETER.md. Resolves actor/X/discardedY/Z/count in order,
uses visible X/Z and explicit original distance/conversion services,applies
separately rounded distance-minus60 times binary32 0.15 correction,wraps the
candidate addition before signed comparison,and writes the raw count destination
unless resulting count is-250000000. The leaf does not mutate movement or AC
fields. Tests compose derived count42 with the existing motion callback (first
step leaves41),instead of freezing ordinary Jump at36. Motion commit:27b1a0c.

Validation:make test passed eleven CTest suites plus58 Python tests;make all
rebuilt the motion module and ttyd-2ds.3dsx. Added exact-distance60/100 cases,
zero-request speed division,word-overflow lower-bound retention,discarded Y and
resolver/store ordering,sentinel before/after correction,and unit-state retention.
Updated README/JUMP-01 with implementation coverage and request for original
math-helper/FPSCR contracts or differential traces. Native harness continues
using provisional attack schedule pending approach/event/VM binding;no emulator
run or original numeric-equivalence claim for these service-level changes.
Next:bind original Jump request graph,approach services and scheduler/callback
ownership,then compare original per-hit trajectories/input/result traces. Native
pouch/equipment/partner lifecycle and full story/assets remain incomplete.

## 2026-10-12 — Portable Mario jump motion and sound-mode loader

Implemented complete local MarioJumpPosition phases1/2/3 in
include/battle_jump_motion.hpp and source/battle_jump_motion.cpp using delivered
BATTLE_MARIO_JUMP_MOTION.md and BATTLE_MARIO_JUMP_NUMERICS.md contracts.
Retains independent visible/baseline/target/delta state;first-call integration,
zero-count early return,negative-count wrap,raw sentinel heading/distance,
mode-specific integer/float division,rounded height/phase formulas,strict
late target-Y clamp,unsnapped X/Z landing,negative-zero/self-subtraction and
mode1 eligibility route. Event wrapper resolves all seven operands on every
call with immediate signed-integer coordinate conversion. Local reviseAngle
uses an explicit original-math service and preserves signed zero/NaN branches.

BATTLE_UNIT_SOUND_MODE.md now has an ordered partial-field byte-record loader,
including overlapping source/destination,retained gaps,-2 reuse and type maps.
Motion requests only nonzero loaded word0 at initial baseline position. Unit,
position,eligibility,heading,distance,sin,fmod,fctiwz and playback are explicit
services;no synthetic sound or host sqrt equivalence claim. The portable model
is built for3DS but native attack/VM ownership remains unbound. Original numeric
services/FPSCR/audio source writers and original trajectories remain open.

Validation:make test passed eleven CTest suites plus58 Python tests;make all
built ttyd-2ds.elf and ttyd-2ds.3dsx. Motion tests cover first/resumed resolution,
42-call flight,independent baseline,landing/clamp/count wrap,mode0/2/3/4 shape,
mode1 retained baseline/eligibility,zero setup,exceptional phase/angle values,
conditional one-time playback and ordered aliased sound-loader fields. Tests use
injected deterministic math services;they are derived contract checks,not
Dolphin measurements. No emulator run for the still-unbound callback. Next:
implement MarioJumpParam writeback,then original approach/Jump event graph and
scheduler services;replace provisional native impact timer only with supported
composition. Full remake remains incomplete.

## 2026-10-12 — ID2: requested Mario jump motion,numerics and sound mode

Added BATTLE_MARIO_JUMP_MOTION.md with the complete801093a8/8c0 body.
All seven arguments resolve as integers every invocation;normal first call
initializes target/baseline/heading/distance/count/curve/sound and integrates on
that same call. Visible sine-offset Y remains separate from linearly advanced
baseline. Strict targetY clamp requires pre-decrement n<3;exact countdown0
snaps only targetY,resets Phi/G and returns1 (advance+yield). Negative counts
have no n<=0 completion. Mode1 skips setup and queries eligibility before
its retained-baseline phase2/3 path. No direct shared AC result/count/main/
option writes or pad-history calls occur;ResultAC remains a later script request.

Added BATTLE_MARIO_JUMP_NUMERICS.md with ordered single/double/FMA operations,
mode0 floating360/n versus mode3 integer360/n,distinct0.53/0.8 coefficients,
height stages and2/4 multipliers,initial/per-step sine phase,terminal negative
self-difference and full local reviseAngle wrapper. Original sin/fmod/frsqrte/
FPSCR/angle/distance execution remains a dependency;no host curve equality claim.
Added BATTLE_UNIT_SOUND_MODE.md with full80126588/14c type/mode loader,retained
record gaps,unconditional reset even at-2,and actual type2/-1 source/playback
request. U18c/U190 are record inputs,not proved live voice handles;corrected
the generic jump field description. No replacement sound samples were introduced.

Linked MJP-MOTION/MJP-NUM/SE-MODE requirements and released only ID2's validated
motion/sound locks in DOCUMENTATION_REQUESTS.md. Earlier Jump graph/parameter
documents now reference closed local motion while preserving numeric/audio/
domain/caller/execution gaps. Independent ID1 Crump/lecture helper work and
native resource initialization work preserved;ID2 changed no native implementation.

Validation:tools/verify_mario_jump_motion.py passed five body fingerprints,
76 instruction anchors,20 original constants and both ordinary mode/count/sound
records;two fresh metadata reports repeated byte-for-byte. Jump graph verifier
passed19 bodies/57 anchors/all268 requests/census;general verifier passed prior
contracts. make test passed ten CTest suites (including concurrent resource
implementation) and58 Python tests;make all exited0,no rebuild;710 behavior local
links resolved;git diff --check passed. All original inspection/report artifacts
remain under /tmp. No original execution/emulator,ISO/profile/save mutation,
asset conversion,story rewrite or native implementation by ID2.

Next:JUMP-01 Takeoff/MoveFrame/MovePosition preparation callbacks,selected target/
cost/stylish/aftereffect/badge-tail and shared grade/option/result producers;
original scene/domain/input/state/RNG/trajectory/audio comparisons. Continue
DMG/STATUS caller/turn/reward service requests. Full-system documentation and
complete-game requirements remain active/incomplete;closed local motion does
not establish a faithful complete original encounter or game.

## 2026-10-12 — Battle-start status clearing binding

Native battle start now calls clearUnitStatus for both semantic units after
initial kind/resource fixture assignment. This initializes the six status-info
types to0xff through the recovered helper;the existing construction reset does
not replace that operation. Pouch imports remain unbound until actual runtime
pouch, equipment and partner lifetime owners are implemented. Updated README
and STATUS-01 breadcrumbs with portable implementation paths and remaining
caller requests. Implementation module commit:7e43c5a.

Validation after binding: make test passed ten CTest suites plus58 Python tests;
make all rebuilt game.cpp and ttyd-2ds.3dsx successfully. git diff --check passed.
No emulator run or original execution claim. Documentation agents' in-progress
Jump motion/numerics/sound and lecture-helper files remain separate. Next:review
those delivered contracts,implement actual motion/Jump request scheduling and
bind badge/resource initialization. Full remake remains incomplete.

## 2026-10-12 — Portable unit clearing and pouch resource lifecycle

Implemented UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md in
include/battle_unit_resources.hpp and source/battle_unit_resources.cpp.
Status clearing retains move/badge/flag state and each info record's last word.
Mario import/revision preserves separate current/base/max HP and FP, signed
halfword narrowing before upper-only caps, and independent pouch requests.
Partner mapping, kind rereads, HP-only revision, direct thirty-byte snapshot
restoration and nonpositive-HP status27 override follow the recovered body.
Export writes current resources and partner snapshots while retaining snapshot
gap bytes; other-kind import/export remains a service-requesting no-op, while
direct revision applies both count bytes. These are semantic runtime records,
not packed original ABI or original disk saves.

Validation: make test passed ten CTest suites plus58 Python tests; make all built
ttyd-2ds.elf and ttyd-2ds.3dsx. New tests cover partial info clearing, signed
resource overflow/negative values, separate pouch pointers and mutation between
requests, partner mapping/kind rereads, dead/alive snapshot import, direct byte
roundtrip, retained gaps, Mario current-only export and enemy no-op/revision.
The initial roundtrip assertion incorrectly expected dead reentry to preserve
status27; corrected the expectation to its documented duration1 override.
No emulator run for these still-unbound resource services. Native player stats,
pouch/loadout ownership, unit spawning and switch/turn/export callers remain
fixtures or unbound. Jump motion drafts arrived during implementation; numerical
and sound companion contracts are pending. Next: bind recovered badge import,
then unit setup to its owners; implement the original Jump motion/request graph
and event cadence rather than inventing a second-hit timer. Goal remains active.

## 2026-10-12 — Native physical input and battle sampling order

Implemented PHYSICAL_INPUT_PRODUCER.md in include/physical_input.hpp and
source/physical_input.cpp:raw/DemoPad/GP ownership,error-3 retained sample,
signed strict48 direction thresholds,button/direction edges and axis deltas,
four-channel error counts/reset masks,scaled24/6 repeats,whole-mask add/remove
reset,input-clear chord,analog publication,forced/change-only motor requests
and ready flag. PADRead/Clamp values are explicit caller inputs;hardware behavior
is not fabricated. Optional reset/motor services run at their documented request
positions. Portable implementation commit:04bde4e.

Battle snapshots now retain12 semantic bytes,copy L/R trigger bytes,zero analog
A/B and preserve trailing bytes on update. WorkInit's inspected stores clear
bytes0..a but not b;the implementation retains old byteb in both snapshots and
still leaves the release array untouched. Asked INPUT-01 to clarify the older
whole-snapshot summary and independently pin this byte map. Native construction/
battle allocation starts these bytes at zero;this is not packed PowerPC ABI.

Native HID held masks project to original button bits. The physical producer
updates before scene processing and persists across battle start,so held A does
not generate another physical edge merely because battle pads were initialized.
Native catch-up carries held snapshots and uses the producer once per simulation
tick. Abstract host callers can still supply already-produced logical GP masks.
Rate60,zero unused channels,C-stick/trigger/error/motor limitations and poll timing
are native adaptation inputs,not original hardware/connection observations.

Native battle ordering now follows the recovered general pads→sequence adapter→
private pad/AC main→later event adapter. All four general buffers update;private
sampling continues with inactive commands and when an item selection is rejected.
Commands initialize in the manager before the harness's later StartAC request.
Original sequence/controller/display registration and event-credit loop remain
unbound;we do not synthesize extra pad samples for multiple script invocations.
The harness retains provisional impact/return scheduling,not a complete original
battle main or tutorial. Original story,assets,music and sounds were unchanged.

Validation:make test passed9 CTest executables and58 Python tests;make all rebuilt
ttyd-2ds.3dsx;git diff --check passed. Producer tests cover all strict48 boundaries,
error-3 snapshot retention,other-error copy,signed axis deltas,press/hold/release,
repeat24/30 at60 and scaled50/30,mask add/remove,zero countdown,error counts/
reset-before-GP publication,chord versus retained DemoPad,motor gates/latches and
snapshot trigger/analog/trailing-byte behavior. Core tests add native physical
level timing,held continuity into battle,inactive private sampling,four-channel
separation and rejected-item sampling,plus all prior mechanics. A reused local
test variable name caused one compile failure;renamed it before final successful
host/native validation. Derived tests are not original executed comparisons.

Final isolated regression command:
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_defense.py
Reviewed defense-single showing Guard/Mario10HP/Crump4HP and defense-mash showing
Hit taken/Mario9HP/Crump3HP after the final rebuilt integration. Earlier captures
preceded the last inactive-callback/test changes;the final rerun is the reviewed
validation above. Manual captures do not establish original polling/latency/
impact cadence or hardware performance. Only generated test save reset;user
profiles/saves and ISO untouched. Git/display escalations automatically approved.

INPUT-01 in docs/DOCUMENTATION_REQUESTS.md records byte-map clarification and
remaining native/hardware,pause,frame-rate/control,chord/motor and event/display
bindings. Next consume the incoming Mario Jump motion and normal event-flow
contracts,complete JUMP-01 follow-up ownership,bind actual pouch/badge/part/unit
data and original event/message/tutorial handlers. Full implementation goal
remains active/incomplete;concurrent documentation edits were preserved.

## 2026-10-12 — ID2: requested Mario-specific jump parameter preparation

Added BATTLE_MARIO_JUMP_PARAMETER.md with the complete80109c68/19c body.
Actor resolves/translates,coordinates use signed integer evtGetValue rather
than evtGetFloat,Y resolves but is discarded,raw operand4 doubles as count
destination. Visible X/Z feed original distABf. Nonzero requested count starts
the candidate;zero uses truncated binary32 distance/current speed. Ordered D>60
adds truncated binary32(0x3e19999a*(D-60)) with signed word wrap,then preserves
the signed maximum with the nonzero requested count. Result sentinel-250000000
suppresses destination write;no unit motion/AC field is directly written.

Ordinary request36 can increase,so it is not fixed impact duration. Subsequent
stylish brother copies the parameter output;its8/20/32 operands require count
remaining36,whereas a derived42 gives11/23/38. Updated graph/inbox boundaries
and released only ID2's validated parameter lock. MJP-PARAM-01..04 provide
implementation cases. MarioJumpPosition801093a8/8c0 still needs full recovery;
original distABf frsqrte/FPSCR and execution/domain traces remain dependencies.
No generic jump or prototype timing substituted for that missing body.

Validation:extended standalone verifier passed19 body fingerprints,57 anchors
(19 new),three original parameter constants and all268-command/caller/census
checks. Original r2 startup lis/ori pinned0x80427440 before deriving60/0.15
constant addresses. Two fresh metadata reports repeated byte-for-byte;692
behavior local links resolved at validation. make test passed nine CTest suites
and58 Python tests;make all rebuilt ttyd-2ds.3dsx after concurrent native edits;
git diff --check passed. ID2 staged only documentation/verifier paths. Original
inspections/reports stayed under /tmp;no original execution/emulator,ISO/profile/
save write,asset conversion,story rewrite or native implementation by ID2.

Next:JUMP-01 complete MarioJumpPosition modes0/3,first/resumed-call numerical
integration,remaining count/landing/block/yield and AC work side effects;then
Takeoff/MoveFrame/MovePosition and branch/cadence composition/original traces.
Continue other request-scoped DMG/STATUS service/turn/reward work with numeric
ID2 locks and inbox communication. Full documentation/complete-game requirements
objective remains active/incomplete.

## 2026-10-12 — ID2: requested ordinary Jump graph and result/VM services

Answered JUMP-01's complete request-graph slice in BATTLE_NORMAL_JUMP_EVENT_FLOW.md:
all268 original commands,first-attack versus selected-weapon target/cost/setup,
delayed setup using abs(moveFrame-22),precheck masks100/100100,distinct Mario
motion modes0/3,window/2 wait before evaluator,persistent AC count versus script
local10 and damage flags20000 (repeat),20100 (final success),100 (failure).
Repeat has no new SetupAC/StartAC and no direct HP/output-result gate;it runs
precheck again. Failure declares-1 only at local10==0;avoidance bypasses damage/
result and takes shared return95. Actual missing target reaches real99,then
reset face,child badge effect,start wait event and delete. Parent/brother/inline
presentation and cleanup order retained without fixed impact/return ticks.

Added BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md with complete raw result/count/
window readers,raw mask option update,prize code30 early-1 versus unsigned1cbd
grade clamp and delayed TimingA stop. Stop requests delete/state1000;it does
not immediately clear callbacks. Added EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md
with signed IF/CASE>=,raw IF mask,full used brother/inline helpers and constructor
ownership-zeroing region. Four attached brothers copy locals and are cancelled
with their owner;the camera107 block is an independent event,not a parent wait.
Only the used constructor region is specified despite whole-body fingerprint.
JUMP-EVT/JAC/JVM requirements give implementable local acceptance cases.

Released only ID2's validated graph lock and linked partial JUMP-01/AC-01/DMG-01
answers in DOCUMENTATION_REQUESTS.md. JUMP-01 remains open:MarioJumpPosition
801093a8/8c0 and MarioJumpParam80109c68/19c,selected-target/cost/power/stylish/
aftereffect/badge service composition and original state/input traces remain.
These Mario-specific bodies must not be substituted with generic JumpPosition
or the native48/42 schedule. Independent ID1 sampler/system-level work and native
input/power/part changes preserved;ID2 authored no native gameplay code.

Validation:tools/verify_jump_event_flow.py passed17 body fingerprints,38 instruction
anchors,nine VM dispatch bindings,original268-command/labels10/90/95/99 census,
five concurrent block pairs and concrete setup/precheck/result/damage/loop/tail
records. Two fresh metadata reports matched byte-for-byte;fresh original annotated
Jump inventory matched the established stream hash. General behavior verifier
passed;generic jump verifier passed10 bodies/55 anchors/nine constants/harbor
bindings;input verifier passed10 bodies/64 anchors/28 calls/three data bindings.
make test passed nine CTest suites and58 Python tests;make all exited0,no rebuild;
673 behavior local links resolved;git diff --check passed. /tmp holds all original
inspection/inventory/report artifacts. No original execution/emulator,ISO/profile/
save mutation,asset conversion or dialogue rewrite by ID2.

Next reserve and recover Mario-specific jump parameter/motion/state callbacks,
then ordinary Jump reachability/target/precheck/hit/feedback/stop composition and
original traces. Continue STATUS/DMG service/turn/reward/request closure alongside
ID1's input cadence work. Full-system documentation and complete-game system
requirements remain active/incomplete;request-stream closure is not full runtime
or tutorial equivalence.

## 2026-10-12 — ID2: requested badge/count import and snapshot export

Added UNIT_BADGE_COUNT_IMPORT.md with complete _EquipItem/BtlUnit_EquipItem
contracts:all numeric byte maps,40-byte reset,kind220 exclusion,200 signed
pouch slots,mode-mask order,per-copy unsigned byte wrap and narrowed2f3/2f4
threshold. Items289/290 now pin Mario power bonus2ff/300 writers. Scoped
Btl_UnitSetup callers prove Mario flags3/partner flags5 before later resource/
status import,distinct enemy held-item branches and current=max writes.
Original entryunit_mario->unitdata_Mario->count3/part array closes the concrete
Mario part binding. Remaining setup services/final battle flag scan are explicitly
partial;whole containing-function hashes do not claim full semantic recovery.

Extended UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md with complete SetParamToPouch:
Mario writes current HP/FP only;partners write current pouch HP and the exact
44-byte battle snapshot fields,retaining its two-byte gap. Export caller/switch/
turn/end/save ownership remains open. Cross-linked earlier power/part/init
documents and corrected now-resolved producer/count gaps. EQUIP-01..04 provide
scoped implementation/acceptance requirements. Released only ID2's validated
badge/export locks in DOCUMENTATION_REQUESTS.md;all parent requests remain open.
No native code was authored by ID2. Concurrent ID1 input documentation and
native power/part changes were preserved.

Validation:general verifier passed1,980 instruction anchors (32 new),whole
import/setup/export fingerprints,Mario setup/kind hashes and concrete three-part
pointer chain,plus all previous checks. Two fresh368-function evidence reports
matched byte-for-byte from read-only ISO.651 behavior local links resolved;
make test passed eight CTest suites and58 Python tests;make all exited0 with
no rebuild;git diff --check passed. /tmp contains original inspection artifacts/
reports/logs;none committed. No original execution/emulator,ISO/profile/save
mutation,asset conversion or story/dialogue rewrite by ID2.

Next:STATUS-01 move/count consumption and concrete infliction/turn owners;
snapshot export/import callers and partner reentry/reset lifetime;DMG-01 actual
per-hit AC/counter/status/weapon mutations and damage/reward/field results;
DMG-02 native service effect/pose/pouch/revival bindings. Initial equipment and
original input/state/RNG/resource traces remain necessary. Continue request-
scoped locks with numeric ID2,communicating via DOCUMENTATION_REQUESTS.md.
Full-system documentation and complete-game requirements remain active/incomplete.

## 2026-10-12 — Original weapon power callbacks in native attacks

Implemented BATTLE_WEAPON_POWER_CALLBACKS.md in include/battle_weapon_power.hpp
and source/battle_weapon_power.cpp. Default returns the signed weapon word
unchanged. Mario services capture action-result bit1 before pouch access,read
signed level,compute the wrapped index with an upper-only clamp,read unsigned
bonus before the power word and wrap addition. Negative indices are passed to
an explicit bounded provider rather than clamped to level1. Semantic factories
retain ordinary Jump1/1,2/2,3/3 and Hammer1/2,2/4,3/6 tables,their documented
special flags,element0 and nil FP callbacks;boss default uses base1/flags1087.
These are minimal behavior parameters,not copies of full original weapon assets.
Portable module incremental commit:33294a0.

Removed the native synthetic power callback and success?2:1 power assignment.
Native attacks now bind original callbacks to battle state,actor bonus bytes,
weapon tables and explicit development level1 inputs. The original pouch/loadout/
upgrade/save owner remains unbound;level1/bonus0 are not claimed measured initial
state. Calculator special flags now come from the documented weapon parameters.
Successful TimingA remains distinct from Jump's single-hit power1. Native Jump
temporarily executes one documented hit;its original subsequent-hit/event flow
is missing. We no longer hide that gap by doubling one hit. This is incomplete
attack sequencing,not a faithful complete Jump/tutorial. Hammer's ordinary
single-hit power follows its table;actual Crump command availability and global
event/state/impact conditions remain open.

Validation:make test passed8 CTest executables and58 Python tests;make all rebuilt
ttyd-2ds.3dsx;git diff --check passed. Power tests cover all levels1..3/both bits/
bonuses0,255,upper clamp,signed negative/zero levels via bounded providers,
captured state before a mutating pouch getter,bonus-before-word read order,
signed32 wrap,default0/-1/150 returns,callback ABI and actual HP/FP calculator
composition. Core timing now checks success flags separately from equal Jump
power,anti-mash failure explicitly,and five single-hit harness turns for victory;
Hammer boundaries retain two-power success and the recovered Charge-use flag.
Existing damage/resource/guard/field/save suites also pass. Derived contract
tests are not original execution comparisons or full attack equivalence.

Isolated regression commands:python3 tools/emulator_smoke.py --fresh-save
--seconds4 --input-script tests/azahar_defense.py and the same command with
tests/azahar_hammer.py. Captures are under artifacts/azahar for manual review;
reviewed Hammer full-charge/release (Nice/2,Crump3HP),early release (Hit/1,2HP)
and overrun (Hit/1,1HP),and defense Guard/Mario10HP versus rejected mash/Mario9HP.
These validate the development adaptation,not original attack timing or starting
loadout. Existing presentation,hit-event and tutorial limitations remain. Only
generated test saves were reset;user emulator profile/saves and ISO untouched.
Git/display escalations were automatically approved. Concurrent documentation
research/files were preserved and excluded from implementation staging.

Added JUMP-01 to docs/DOCUMENTATION_REQUESTS.md for full ordinary Jump branches,
per-hit result/damage flags,subsequent-hit/target/count conditions,inline/child/
wait ownership,feedback/stylish/stop and cleanup. This request deliberately
excludes substituting the harness's one-hit or48/42 schedule for original flow.
Next bind the recovered physical/pad cadence documentation arriving from agent1,
complete original Jump event flow and integrate actual pouch/bonus/part/unit data
from agent2. Continue original scene/message/VM and service handler integration.
Full implementation goal remains active/incomplete.

## 2026-10-12 — ID2: requested status clear and pouch resource initialization

Added UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md. Complete ClearStatus zeros30
status bytes,clears Charge through SetStatus(16,1,-99),then resets six info
records while retaining each word+8. Complete SetParamFromPouch/ReviseHpFp
preserve separate base/max/current resources,signed16 narrowing/upper-only caps,
unsigned HP/FP count bonuses and ordered repeated pouch reads. Mario imports
current70/74 with base8e/90;partners map kind224..230 to nonsequential pouch
indices1,2,4,5,6,3,7 and revise HP only. Partner entry then directly restores
30 status bytes and related fields from battle162a0+44*(kind-224),overwriting
the earlier clear,and requests Instant Kill duration1 for nonpositive HP.
Other-kind import is locally no-op after reads;direct resource revision still
has its HP/FP path. Snapshot producer/save lifetime and badge aggregation remain
open. INIT-01..04 make this scoped answer implementable without invented turn rules.

Pinned original entry calls ClearStatus29330 and SetParamFromPouch29634;
linked partial STATUS-01/DMG-01 answer and released only ID2's validated lock.
Independent ID1 physical-key/pad-order lock and implementation edits preserved.
No native implementation was authored by ID2 in this step.

Validation:general verifier passed1,948 instruction anchors (32 new),five new
whole-body hashes and all prior contracts. Two fresh364-function evidence
reports repeated byte-for-byte from read-only ISO.626 behavior local links
resolved;make test passed eight CTest suites including concurrent weapon-power
implementation and58 Python tests;make all exited0 with no rebuild;git diff
--check passed. Inspection ELFs/reports/logs remain under /tmp;no original
execution/emulator,ISO/profile/save mutation,asset conversion or dialogue rewrite.

Next:STATUS-01/DMG-01 badge/count/bonus and move initialization,snapshot producer/
partner switch lifetime,concrete Mario kind/count binding;per-hit AC/turn/infliction/
periodic and damage/reward/field-return graph. Full-system documentation and
complete-game requirements remain unfinished;scoped local closure is not a
finished faithful encounter or complete-game specification.

## 2026-10-12 — ID2: requested unit-part defense and lifetime contracts

Added docs/behavior/UNIT_PART_DEFENSE_AND_LIFETIME.md with the scoped original
unit-entry part allocation/copy region and complete GetPartsPtr/Delete/BattleAlloc/
BattleFree request contracts. Count is unsigned;parts use one cleared count*500
block with500 runtime stride and4c source stride. Defense/attribute pointers are
retained from source records rather than copied arrays;attributes/counter words
are independent. Crump count1/part1 binds two signed five-byte zero tables;
Mario's three annotated records share separate zero tables and retain their
distinct flags. Its actual kind/count entry still needs binding.

Single-part lookup returns the head for any nonnegative ID without matching;
negative ID returns head before dereference;multi-part traversal matches the
first source ID or returns null. Entry's saved body ID is last flagged part,
whereas the body helper finds the first. Part allocation has no local null/
zero-count pre-loop guard. Deletion retains event/ordering/status/custom/pose
service request order,one block/unit free and final registry clear;source tables
are not locally freed. PART-01..04 define portable acceptance cases and required
runtime boundaries without inventing invalid-input or unregister-first behavior.

Released only ID2's validated part lock in DOCUMENTATION_REQUESTS.md and linked
the answer. Native code and ID1's jump contract/lock were preserved. Parent
DMG-01 remains open for all mutation/state/service/runtime composition evidence.

Validation:general verifier passed40 new anchors and prior checks,part-region
and complete helper/delete/allocator hashes,Mario array/table values and Crump
count/part/defense relocation bindings. Two fresh359-function evidence reports
matched byte-for-byte from read-only ISO.621 behavior local links resolved.
make test passed seven CTest suites and58 Python tests;make all exited0 with
no rebuild;git diff --check passed. Initial verifier failure was a local variable
shadowing the DOL read offset;renamed it to part_record and reran successfully.
ID1 recorded that tooling breadcrumb in the shared request inbox. No original
execution/emulator,ISO/profile/save writes,asset conversion,native implementation
or story rewrite by ID2;inspection/report artifacts remain under /tmp.

Next:DMG-01/STATUS-01 unit parameter/pouch/badge/status initialization and concrete
Mario entry binding;per-hit AC/count/bonus/pointer mutation and full target/
damage/death/reward/field-return ownership. ID1's movement callback research
remains independent. Keep full-game requirements deferred until all systems are
documented;the overall objective remains active/incomplete.

## 2026-10-12 — Native direct damage and shared FP resource application

Implemented docs/behavior/BATTLE_DAMAGE_APPLICATION.md in
include/battle_damage_application.hpp/source/battle_damage_application.cpp.
Preserves attacker lookup/-5 sentinel,requested HP clamp99 and attacker
accumulator,part/pattern preparation,ordered original effect and sound requests,
Pity Flower before FP subtraction,byte/halfword accounting and suppression,
HP before/after/caps,shared partner FP owner,action-record requests,Sleep wake
roll/field writes,recovery/audience order,lethal code/pattern/revival marking and
final hit-event request. Resource words wrap/narrow explicitly. FP helpers scan64
slots for first Mario kind222/belong0 and preserve the valid-owner invariant;
no independent partner FP pool is invented. Service internals remain unbound.
Portable module incremental commit:648dfb4.

DamageUnit retains separate base/max/current resources,requested/consumed
accounting,HP snapshots,narrow amount/flag fields,last-part/pattern and kind
sound/service inputs. This is a semantic model,not full unit-entry ABI. Native
attacks use direct application instead of hand-written HP subtraction and display
the clamped requested HP amount. Calculator/application share stable owned body
parts;progress/enemy HP synchronize from mutated units. Fixture caps remain
player10/boss5 with pending loadout/entry overrides;position/height fixture0,
unresolved kind sounds,revival item-count fixture0 and reaction pattern fixture0.

The native adapter records each latest hit's ordered effect,sound,audience/action
record and hit-event requests. It does not execute original effects,MusyX macros,
audience handlers or hit scripts,and is not an active VM queue. No synthesized
audio or fabricated story/revival scene was added. Harness victory/defeat/
controller flow remains provisional;original script/scene binding is unfinished.

Validation:make test passed7 CTest executables and58 Python tests;make all rebuilt
ttyd-2ds.3dsx;git diff --check passed. Application tests cover clamp/overkill/
healing/halfword wrap,HP suppression with effects/accounting/FP updates,field/
flag writes,FP128/256/negative byte distinctions,shared FP owner and recovery
cap/negative/wrap,element effects/star positions/code25 sound difference,
override priority/original-pattern suppression,Pity29/30 and Sleep49/50 draws,
full-FP-before-incoming loss,suppressed-HP Pity draws,lethal/Instant Kill wakeup,
absorption's flag/sign distinction,lethal kind endpoints,pattern25 gate,revival
mark without healing/consumption,action-record offsets and final event order.
Core checks requested/dealt amounts,HP mirrors and final recorded requests along
with prior action/guard/field/save scenarios. These are derived contract tests,
not original executed comparisons or whole-damage-system acceptance.

Isolated regression:python3 tools/emulator_smoke.py --fresh-save --seconds4
--input-script tests/azahar_defense.py. Reviewed defense-single with Guard/Mario
10HP/Crump4HP and defense-mash with Hit taken/Mario9HP/Crump3HP after resource
application. Original effects,sounds/hit scripts,FP gameplay,tutorial timing and
hardware remain unverified;stage/camera/animation limitations remain visible.
Only generated test save reset;user emulator profiles/saves and ISO untouched.
Git/display escalations automatically approved;concurrent documentation preserved.

Added DMG-02 in docs/DOCUMENTATION_REQUESTS.md for body/position/height/belong
helpers,field114/pointer144+10 schemas,concrete parts/sounds/effects,action-record/
audience semantics and hit/death/revival/FP runtime bindings. New
BATTLE_WEAPON_POWER_CALLBACKS.md arrived and answers default/Mario callbacks and
tables:ordinary Jump's successful single-hit callback does not double power.
Next replace fixture callbacks with those contracts and bind Jump's separate
subsequent-hit/event lifecycle;do not carry its current aggregated two-damage
harness behavior into a claimed faithful attack. Bind incoming unit stat/part
data and direct-application service handlers;continue original event/message/
tutorial/turn orchestration. Full implementation goal remains active/incomplete.

## 2026-10-12 — ID2: requested default/Mario weapon power contracts

Answered the two ID2 power subtasks of DMG-01 in
docs/behavior/BATTLE_WEAPON_POWER_CALLBACKS.md. The complete default body
returns weapon signed word20 unchanged;the original boss weapon binds it and
supplies base1 with no FP callback. Complete Mario Jump/Hammer callbacks capture
battle1cb8 mask2 before pouch access,read signed level98/99,form upper-only
clamped six-word index and add unsigned unit bonus2ff/300. Concrete ordinary
tables are Jump1,1,2,2,3,3 and Hammer1,2,2,4,3,6;these are powers before damage
modifiers,not attack totals or an executed tutorial loadout. Invalid low levels
retain negative indices;do not silently insert a lower clamp. Added scoped
implementation requirements PWR-01..04 and derived acceptance cases.

DOCUMENTATION_REQUESTS.md now identifies my locks/deliveries as agent ID2,
releases only my validated power locks,and links the answer with exact remaining
dependencies. Agent ID1's movement lock and delivered attack graph were
preserved. No native implementation files were changed by ID2. Existing native
damage-application integration and independent documentation work remain separate.

Validation:python3 tools/verify_behavior_contracts.py passed32 new instruction
anchors and all previous checks,including four whole-body hashes,two whole
ordinary weapon hashes,six-word tables,event/FP pointers,boss weapon hash/base1
and its callback relocation. Two fresh355-function behavior_evidence reports
matched byte-for-byte using read-only ISO.610 behavior local links resolved.
make test passed seven CTest suites and58 Python tests;final make all exited0,
recompiling audio/game/main/render/save and rebuilding ttyd-2ds.3dsx after
concurrent native edits;git diff --check passed. /tmp holds inspection ELFs/reports/logs;
none are staged. No original execution,emulator/profile/save changes,ISO write,
asset conversion or story/dialogue rewrite occurred in this documentation step.

Next DMG-01 tasks:unit/part defense construction and original defense/attribute
tables;unit bonus and pouch level initialization/upgrades/reload;per-hit AC-bit
and count ownership;weapon mutation lifetime;precheck/damage/application/death/
reward/field-return composition and original traces. AC/STATUS parent requests
remain open. The full documentation and complete-game requirements objective
remains active/incomplete;this scoped callback answer does not close all systems.

## 2026-10-12 — Native status storage/recovery and move helpers

Implemented docs/behavior/BATTLE_STATUS_STORAGE.md and the required local
move helpers from BATTLE_MOVE_COUNTS.md in include/battle_status.hpp and
source/battle_status.cpp. Shared semantic unit state retains original status
byte offsets118..135,move bytes20..24,HP,kind/belong,Zap Tap and unit/status flags;
this is not original packed ABI or complete unit initialization. Getters preserve
signed8 ID narrowing,shared-pair sign filtering and zero/default output;presence
preserves negative durations and badge-only Electric. Setters preserve no-write
success,opposite Burn/Freeze cancellation,size/attack overwrite versus defense
sign cancellation,zero-side clears,Charge byte wrap before signed/cap checks,
Sleep auxiliary reset and positive-only Fast/Slow cancellation/reset side effects.
Portable status/move incremental commit:cbf1a33.

Recovery is one explicit callable step:HP/Instant Kill gates,signed durations,
Sleep's negative-duration clear/auxiliary preservation,retained Poison/regen
strength,direct size/attack pair writes,other setter calls and their side effects,
local-expiry return value and Charge/Flipped no-op. No invented once-per-turn
schedule,infliction chance,immunity or periodic effect is added. Move reset
preserves Fast's unsigned shift/narrowing,Slow's signed/threshold branch,zero
fallback,untouched-count mutation gate and signed max-count cap without a reset.
CanAct/CanGuard preserve status/flag order and the documented null arithmetic
result;AC possibility adds membership/kind gates and still has no HP/count check.

DamageUnit now shares the status unit fields;storedDamageStatusServices supplies
actual Check/Get readers. Removed the always-absent native status callbacks.
The harness owns persistent player/enemy battle-unit storage instead of recreating
it on every attack,and keeps HP synchronized after attack/damage/item healing.
Mario kind222 is used;enemy kind/other initial stats remain explicitly pending
unit data binding. Guard dispatch now includes the documented AC possibility
predicate. Status infliction/event/turn/lifetime initialization is still unbound,
so the harness starts with no inflicted effects;we do not add a fabricated status
demo or tutorial branch. Save/story/dialogue/art/audio behavior was not rewritten.

Validation:make test passed6 CTest executables (core,TimingA,Hammer,defense/pad,
damage,status/move) plus58 Python tests;make all rebuilt ttyd-2ds.3dsx;git diff
--check passed. Status tests cover all28 ID getter/setter routes,byte ID/value
narrowing,sign pairs/zero durations,no-write returns,negative and zero incoming
cancellation,Charge wrap/saturation,explicit recovery/mutation versus expiry,
Sleep auxiliary preservation/live/lethal/Instant Kill gates,Fast/Slow side
effects,partial-count/flag suppression,base0/1 thresholds,Fast64 wrap,signed
max-count behavior and action/guard/null/AC predicates. Damage integration tests
compose actual stored Charge/size/attack/defense setters,recovery,Burn/Freeze
cancellation and badge-only Electric readers with the recovered calculator.
Core scenarios assert persistent HP mirrors alongside all prior behavior.
These are derived contract tests,not original executed comparisons.

Isolated regression:python3 tools/emulator_smoke.py --fresh-save --seconds4
--input-script tests/azahar_defense.py. Reviewed defense-single (Guard,Mario10HP,
Crump4HP) and defense-mash (Hit taken,Mario9HP,Crump3HP) after shared status-reader
and persistent-unit integration. Manual captures validate the native harness,
not original status gameplay/turn timing or hardware performance. Existing stage
props/camera/animation limitations remain visible. Only generated test save was
reset;no user emulator profile/save or ISO write. Git/display escalations were
automatically approved. Concurrent documentation work was preserved.

Added STATUS-01 to docs/DOCUMENTATION_REQUESTS.md for unit status/badge/move
initialization,infliction/periodic/turn/event ownership,count consumption and
effect/audio/pose bindings. Linked incoming UNIT_ENTRY_STATS_AND_SPAWN as partial
DMG-01 stat-copy evidence:boss HP5/FP0 writes are established,but later override/
full entry/unit registry/init remain open. New feedback/stage-gate documentation
arrived during work;inspect its exact caller/reset conditions next.

Next implement original direct damage application/resource ownership with
ordered presentation/event interfaces;connect real weapon/unit stat data as
documentation arrives. Bind actual infliction/recovery/controllers rather than
adding prototype turn rules. Continue original tutorial/story/message/event
runtime work and file-based documentation requests. Full implementation goal
remains active/incomplete;local status services do not make the game complete.

## 2026-10-12 — Native ordered HP/FP calculators and revival marker

Implemented docs/behavior/BATTLE_DAMAGE_CALCULATION.md in
include/battle_damage.hpp/source/battle_damage.cpp. Typed semantic unit/part/
weapon/battle inputs retain signed fields,byte badge counts,callback interfaces
and explicit Check/GetStatus reader services. This is not original packed ABI
or complete unit/status storage. HP calculation preserves pre-callback default
code/element,callback mutations,repeated All or Nothing gate,ordered attacker/
defender modifiers and intermediate clamps,Charge flag mutation,element code
ORs,guard/drain/counter/badge reductions,linear Double Pain,counted Last Stand,
defense attributes and zero/nonzero/absorption result codes. Signed32 arithmetic
wraps explicitly;there is no invented minimum1 or99 calculator cap. FP keeps
the HP absorption gate,distinct All or Nothing bypass behavior,one guarded
callback/clamp and low-byte code rewrite without copying the HP pipeline.
Portable calculator commit:138ed7c.

Added the documented emergency-revival marker:kind222/224..230 query pouch
item151;other kinds use the unit's held-item word. Success ORs800;failure leaves
the code unchanged. No held-item fallback after a failed pouch query,no HP
eligibility check,item consumption,healing or revival event is invented.
Inventory remains an explicit service boundary. This local marker does not
implement actual death/revival handling.

Native player and enemy harness attacks now call HP then FP with one shared
result-code word;Game::damageCode exposes that transient code for validation.
The stat/weapon fixture still provides provisional powers1/2,zero defense/no
statuses and no FP callback. Resource application is still the harness's bounded
HP update,not the full BattleDamageDirect/effect/accounting/event pipeline.
Enemy calculation receives documented normal-attack flags100 plus the harness's
guard dispatch flag40000. Original tutorial unit/weapon power,global eligibility,
counter/precheck/hit/reward/death event binding remains unfinished. No story,
dialogue,original asset or sound was changed or converted.

Validation:make test passed5 CTest executables (core,TimingA,Hammer,defense/pad,
damage) and58 Python tests;make all rebuilt ttyd-2ds.3dsx;git diff --check passed.
The new calculator scenarios exercise callback argument/order/mutation gates,
entry versus post-callback All or Nothing behavior (including negative defense
after the second gate),danger/peril equality,badge/status modifier gates and
intermediate clamps,burn element caching,mode/guard/defense flags,negative defense,
all five element codes/default,absorption including zero and FP suppression,
attribute1/2/4 behavior,linear Double Pain and Last Stand,counter minimum followed
by guard-zero,raw flag ORs/FP code preservation,signed wrap and uncapped150 damage.
Revival tests check kind endpoints,pouch/held-item ownership,unchanged failure
flags/no fallback,negative pouch result and nonconsuming/nonhealing behavior.
Core scenarios now assert original-form attack/guarded/unguarded calculation
codes alongside existing timing/damage/field/save scenarios. Derived contract
tests are not original execution comparisons or complete battle acceptance.

Isolated regression command:
python3 tools/emulator_smoke.py --fresh-save --seconds4 --input-script
tests/azahar_defense.py. Reviewed defense-single showing Guard and Mario10HP,
Crump4HP;defense-mash showing Hit taken and Mario9HP,Crump3HP after calculator
integration. These manual wall-clock screenshots check the native adaptation,
not original impact timing or full-game fidelity. The final explicit nil-FP
invocation was covered by final host/native builds;it returns0 without changing
the shared HP code. Existing stage/camera/animation limitations remain. No user
emulator profile/save or ISO write;only generated test save reset. Sandbox Git/
display escalations were automatically approved.

Added DMG-01 in docs/DOCUMENTATION_REQUESTS.md for weaponGetPowerDefault,
ordinary Mario callbacks,tutorial kind/stat-copy/part tables,hit-counter lifetime,
damage/application/reward/defeat/FP/inventory/status bindings. Linked incoming
HARBOR_BOSS_TUTORIAL_EVENTS/BATTLE_TUTORIAL_STATE_SERVICES as partial AC-03
answers:the normal boss weapon local byte gate is1,CheckDamage uses flags100,
but flag1000 consumers/impact/full dispatch remain open. Do not claim this makes
the prototype a faithful opening scene or enable a guessed superguard outcome.

Next:implement documented status storage/presence and direct damage application
with resource ownership plus ordered presentation/event service boundaries;
replace explicit fixture powers/stats as DMG-01 answers arrive. Implement the
original tutorial/unit/event/message graph from full recovered conditions,not
message IDs alone. Record additional missing service requests in the inbox.
The full implementation goal stays active/incomplete. Concurrent documentation
agent edits were preserved and excluded from implementation commits.

## 2026-10-12 — Native defense checks and separate battle pads

Implemented the documented US BtlPad_WorkUpdate/WorkInit and defense result
contract in include/battle_defense.hpp/source/battle_defense.cpp. Portable pad
work retains current/prior semantic snapshots,30-entry trigger/repeat/held
histories,unshifted release array and byte repeat phase/counter. Preserves
inclusive main/C-stick thresholds,virtual edges,producer-supplied physical
edges/repeats,whole-mask change reset,16/8 virtual repeat delays,analogA/B zeroing
and WorkInit's release-array retention. This is semantic state,not original
packed ABI or a closed global physical input producer. Unknown snapshot fields
and native physical repeat/C-stick binding remain outside this implementation.

Defense preserves independent badge-derived clamp/windows,HP/weapon-byte gate,
override/automatic/B-window order,15-entry A+B anti-mash count (simultaneous A/B
counts twice),normal A-result/absence classification and unconditional private
pad reset on every return. Local result1/2/4/5 is stored and returned;normal auto
does not force guard when the superguard gate is ineligible. Separate general
and private pad ownership is explicit. Portable module commit:55d4d75.

Native battle now samples both pads per simulation tick before callbacks,reads
general held-left for Hammer and general triggers for TimingA,and calls defense
at the harness enemy impact. Removed provisional first-press guard latching and
five-tick window. At no-badge difficulty3,one A edge at index7 succeeds,at8 fails;
two edges in0..14 reject,while an older edge at15 does not reject a newer edge.
Held A is not a trigger. Native A/B held/release data is carried across catch-up
ticks while edges are consumed once. Circle Pad floats are clamped to70 units
and truncated to signed stick coordinates;this adaptation is not an original
input measurement. Field movement/save layout is unchanged.

The current harness uses weapon gate0,no badge/automatic/override values,
provisional enemy impact54 and attack1. Guard reduces that harness damage by1;
the complete damage dispatcher and tutorial eligibility are still unbound.
Do not claim Crump superguard availability or reflection from the portable
result5 path. On-screen feedback now identifies GUARD and Guard/Hit taken during
enemy impact instead of showing the preceding Jump/Hammer prompt. Original
story/dialogue,assets and sounds were not rewritten or converted.

Validation:make test passed4 CTest executables (core,TimingA,Hammer,defense/pad)
and58 Python tests;make all rebuilt ttyd-2ds.3dsx successfully;git diff --check
passed. New module tests cover all seven A/B windows over30 history positions,
eligibility/HP/automatic/override precedence,combined presses and14/15 boundary,
general/private isolation,all return resets/release retention,producer-mask
preservation,30-entry shift,all eight virtual direction threshold/release pairs
and16/8 repeat/physical-change reset. Core scenarios cover native impact47/54
success,46 failure,39/40 mash span boundary,simultaneous A/B,B gate rejection,
held-A absence and delayed evaluation,plus existing action/save/field scenarios.
These are derived contract tests;no original execution equivalence is asserted.

Added tests/azahar_defense.py;isolated Azahar command:
python3 tools/emulator_smoke.py --fresh-save --seconds4 --input-script
tests/azahar_defense.py. First single-press capture showed GUARD and HP10/10;
the initial mash capture did not establish the outcome. Adjusted mash hold/
capture timing;reviewed repeated defense-mash and defense-mash-after showing
Ouch and HP9/10. Final UI capture rerun reviewed: defense-single shows GUARD/
Guard and HP10/10;defense-mash shows GUARD/Hit taken and HP9/10.
Manual wall-clock inputs/screenshots are not exact-tick assertions or original
gameplay comparisons. Stage props/character animation/camera limitations remain.
Only generated test saves were reset;user emulator profile/saves and ISO remain
untouched. Git/display sandbox escalations were automatically approved.

Added AC-03 to docs/DOCUMENTATION_REQUESTS.md for original physical producer,
snapshot fields,private/general cadence,defense call sites,Crump weapon gate,
shared+1d18 counter ownership and damage/multihit/eligibility bindings. Linked
incoming harbor opening/configuration/field-return evidence as partial AC-01
answers. Raw kind halfword5 is not yet a closed HP contract;field-return nonzero
result gate is not proof of loss/victory behavior. Next implement documented
damage/application and event services and connect the tutorial/unit consumers
as recovered. Do not replace story scenes with message lists. Full implementation
goal remains active/incomplete. Concurrent documentation edits were preserved
and excluded from implementation commits.

## 2026-10-12 — Native held-left Hammer command

Implemented the recovered US ID3 main/result/delete behavior in
include/battle_hammer.hpp and source/battle_hammer.cpp, based on
BATTLE_HAMMER_STICK_KEEP_LEFT.md, BATTLE_HAMMER_WEAPON_BRANCHES.md and the
command callback/pad contracts. Preserves initialization,120-call entry wait,
held-left charge,negative-countdown stage advance,release allowance tables,
retry/overrun/automatic/forced/token/RNG ordering,suppression latch,same-call
redispatch and60-call finish plus deferred callback cleanup. The forced route
continues countdown/stage/sound processing before redispatch. Ordinary success
and overrun do not increment the success count. Main/result/display/delete
liveness are separate;return0 clears only main in the caller,while1007 clears
all four. The original1003 busy loop makes no yield or sound-stop request.

Original charge requests retain SFX_AC_PI1/SFX_AC_PONE1 and per-stage handles
through an injectable platform sink. Native sound macro playback remains unbound;
no generated tone or arbitrary sample was substituted. Native API rejects N
outside1..12,negative period and difficulty outside0..6;retail ordinary N4 is
covered. Original unchecked invalid-memory behavior is not reproduced. Added
weapon-pointer period selection and original word-wrapped badge-adjusted attack
difficulty. The ARM int32_t/host int typedef difference exposed a clamp template
error during the first native build;explicit typing fixed it before final checks.
Portable implementation incremental commit:90144cf.

Replaced the provisional native B-hold/60-tick release harness with the ordinary
default weapon policy: N4,T25,base difficulty3,no badges,mode0,setup option1 and
actor token clear. Circle Pad X<=-30/70 maps to general held-left mask40000;
B is no longer Hammer input. README and on-screen prompts reflect the control.
One command call occurs per simulation tick;pending bit gates damage,then the
harness requests stop. Beginning from its first windup call,stage5 occurs on
call81 and releases82..106 succeed with this policy;late entry shifts the
schedule. These are derived native invocation boundaries,not original animation/
impact measurements. Crump/tutorial weapon/availability/flags/cadence,damage and
story event bindings remain open. The bottom-screen guide now shows actual four
charge stages and a release prompt;it is still a development assist,not the
recovered original display. No original dialogue/story or asset conversions changed.

Validation:final make test passed all3 CTest executables (core,TimingA,Hammer)
and58 Python tests;make all rebuilt ttyd-2ds.3dsx successfully;git diff --check
passed. Contract tests exercise seven allowance boundaries across periods25,
30,40,90,stage-increment release deferral,period0,supported N1..12,120-call wait,
suppression,original sound request/handle order,automatic/retry/forced/token
routes,RNG draw timing,difficulty wrap and exact finish cleanup. Core scenarios
cover native release81 deferral/82 success/106 success/107 overrun,inclusive
stick threshold,late charge entry and no-input timeout alongside existing Jump,
item,victory/defeat/save behavior. These are derived tests,not original execution.

Added tests/azahar_hammer.py and ran the isolated capture sequence twice:
python3 tools/emulator_smoke.py --fresh-save --seconds4 --input-script
tests/azahar_hammer.py. Reviewed hammer-charged/release/early-release/overrun
captures under artifacts/azahar: full charge showed the release prompt,release
displayed Nice/2 and Crump HP3,early release displayed Hit/1 and HP2,overrun
displayed Hit/1 and HP1. Repeated after fixing the lingering post-attack release
prompt;updated captures show Nice or Attack finished after resolution. Later
callback-liveness/setup-context refinements were covered by final host/native
checks. Wall-clock input holds/manual screenshots are not exact-tick assertions,
matched original-gameplay evidence or hardware performance validation. Existing
stage props/animation/camera limitations remain visible.

Updated docs/DOCUMENTATION_REQUESTS.md with implemented Hammer locations and
remaining AC-02 cadence/damage/tutorial/display/sound requests. Concurrent original
harbor-opening documentation is arriving;inspect it next rather than inventing
story sequences. Next implement documented private-pad defense and damage/event
services,then bind the original opening/tutorial contracts as recovered. Record
future missing behavior requests in the shared inbox. Full-game goal stays active
and incomplete. ISO/user emulator profiles/saves untouched;only generated test
saves were reset. Git/X display escalations were automatically approved. Concurrent
documentation-agent edits were excluded from native implementation commits.

## 2026-10-12 — Native US TimingA implementation and documentation requests

Implemented the recovered ordinary Jump ID1 local contract in
include/battle_timing_a.hpp and source/battle_timing_a.cpp. General battle press
history shifts newest samples to index0 and retains30 entries; it is explicitly
distinct from the private AC/defense pad. The evaluator preserves seven exact
threshold/window tables, signed count selection and100000 cutoff, ordered
token/RNG/automatic/battle-force routes, classification and count effects,
20-sample anti-mash rejection, repeated-result behavior and nonconsuming input.
The lifecycle preserves initialization scratch words, option bit0 clearing,
state99/100 waits and same-call finish decrement with deferred callback cleanup.
Native difficulty values outside0..6 are rejected before mutation; the original
unchecked getter's invalid-memory behavior is not emulated.

Added US rand/irand arithmetic with explicit caller-owned seed and zero-bound
no-draw behavior. No original initial seed, full shared RNG ownership or save
serialization is inferred. The ordinary native harness uses no RNG/force route.
Portable module/test/build commit:9811fe0.

Integrated TimingA into Game::battle: one A-edge sample per simulation tick,
result evaluation at the harness impact call instead of first-press latching.
At selected difficulty3, index7 succeeds and8 fails; two edges in0..19 reject,
but an edge at20 or older does not reject a later valid edge. Input sampling,
impact48, difficulty3, per-attack command/count reset and finish scheduling are
explicit harness choices, not established original tutorial/event bindings.
Crump HP/damage, Hammer B-hold and guard remain provisional. No original story
or dialogue was altered, and no original asset conversion was needed.

User directed file-based communication with the documentation agent. Added
docs/DOCUMENTATION_REQUESTS.md as the request inbox, linked from README:
AC-01 requests opening/Crump and Jump parameter/flag/difficulty/count/state/
cadence/impact/tutorial/story bindings; AC-02 requests ordinary Hammer branch/
period/difficulty/caller/display/sound and separate tutorial bindings. Please
answer with evidence links and mark requests resolved there. This is a request
left for the documentation agent, not a claim its answers have arrived.

Validation: make test passed both CTest executables (core and new TimingA
contract suite), plus52 Python tests. New tests cover all difficulty first-window
edges, mash boundaries19/20, count equality/missing thresholds/cutoff, forced
route precedence, RNG vectors/consumption, classification/history retention,
word wrap and exact lifecycle countdown/cleanup. Integration scenarios exercise
the new impact/window/mash behavior plus existing battle/victory/retry coverage.
make all rebuilt ttyd-2ds.3dsx successfully for both implementation steps.
git diff --check passed. These are derived contract tests, not original execution.

python3 tools/emulator_smoke.py --seconds4 booted the updated native build using
only its isolated profile. Reviewed artifacts/azahar/boot.png: both screens
rendered the plaza and restored the isolated7/10HP save. This is boot/reload
evidence, not Jump timing/gameplay acceptance or hardware performance evidence.
Xvfb required sandbox escalation after display-socket failure. Incremental Git
commit also required escalation because .git is mounted read-only in the sandbox;
automatic approval accepted both actions. User emulator profiles/saves and the
read-only ISO were untouched. Concurrent documentation/research edits in the
workspace were left to their author and excluded from implementation commits.

Next: implement documented ID3 Hammer local behavior and requests without
inventing sound playback; bind correct period/difficulty/cadence/scene only after
AC-02 answers. Use AC-01 answers to replace Jump harness scheduling and connect
original Crump events/story. Then implement the documented private-pad defense
contract and damage/event/inventory/UI systems with explicit caller boundaries.
Original input/impact comparisons in isolated Dolphin and native interactive
acceptance checks remain needed. All full-game closure gates remain open; the
user's implementation goal remains active and incomplete.

Updated 2026-10-12 (America/Boise). This is a working native development slice,
not a completed remake or fidelity-verified port.

## User decisions

Target New 2DS XL, 60 Hz gameplay/60 fps presentation target; emulator-only
validation for now. Start with an early Rogueport plaza slice and introductory
Crump encounter. Touch-centered menus retain physical button access. The user's
latest correction requires very similar original art and preservation of
original sounds/music. Earlier redrawn sprites and generated tones were removed.

## Implemented

- Reproducible devkitARM/libctru/citro3d/citro2d build, embedded RomFS `.3dsx`.
- Host-testable simulation, fixed ticks, bounded catch-up, input edge consumption,
  original-wall collision, following partner, camera, jumping and paper turns.
- Dialogue, touch and button menus, jump/hammer/guard battle harness, mushroom,
  victory, defeat/retry and return to exploration. Parameters remain provisional.
- Versioned save encoding, validation, temporary-file replacement and backup
  recovery, plus on-screen save failure reporting.
- Read-only inventory of 3,204 ISO files; exact-path extraction and provenance.
- GX tiled/paletted/CMPR texture conversion; original AnimGroup idle-pose baking;
  original Mario full-body idle/jump/hammer textures in a shared atlas.
- Original Rogueport plaza and common battle stage geometry/materials/textures.
  Plaza: 40,714 triangles, 79 batches, 60 textures; 711 near-ground wall segments.
  Stage: about 10,000 triangles, 14 material batches before visibility overrides;
  consult romfs/manifest.json for the final count.
- Original Rogueport, normal-battle and victory music decoded to PCM16 without
  resampling; NDSP streaming with original loop metadata.
- Original MusyX banks preserved and 1,247 samples indexed. Standalone sample
  decoding works; sound-event macro mapping/playback is unfinished.
- Isolated Azahar and Dolphin capture tooling, durable architecture/asset/mechanics
  notes and project instructions. No existing emulator settings/saves modified.

## Validation

`make all` builds ttyd-2ds.3dsx with devkitARM. `make test` passes the portable core
scenario suite and 16 Python codec/extraction/topology tests. Core scenarios cover
movement, collision imports, partner following, dialogue, menu input, action timing
boundaries/mashing, damage, item use, victory, defeat/retry and save recovery.

The isolated Azahar 2126.2 OpenGL/llvmpipe profile booted both screens and reported
60 fps during typical captures. It exercised touch tabs, movement, dialogue,
battle selection, five unsuccessful jump attacks, enemy damage, victory, return
to plaza and save creation. The saved result had Crump defeated and Mario at 6 HP.
Screenshots/logs are under artifacts/azahar. Capture sequence:

```sh
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_walkthrough.py
```

Final verification repeated `make -j2 all` (up to date) and `make test` (core
suite and all 16 Python tests passed). Running
`python3 tools/emulator_smoke.py --seconds 4` without resetting the save visibly
restored Mario at 6/10 HP and the completed Crump encounter. The final battle
capture still contains an upper curtain clipping Crump and a central prop despite
the hidden-node overrides: investigate the actual node hierarchy/visibility before
claiming that these overrides resolve presentation. Plaza foreground occlusion
also remains visible in the reload capture.

The input sequence is not a pixel-asserted acceptance test. Review the captures.
Precise action/guard timing and defeat were verified in host tests; their full
emulator acceptance checklist remains below. Occasional capture/save transitions
showed lower emulator frame rates; no sustained performance benchmark or physical
hardware validation has been performed. Do not claim verified hardware 60 fps.

Dolphin successfully booted the original ISO in an isolated profile and captured
its initial health/safety screen. No original gameplay measurements were obtained.

```sh
python3 tools/emulator_smoke.py --reference-dolphin --seconds 12
```

SDL/X socket creation fails under the restricted sandbox; the isolated virtual
screen checks succeeded with sandbox escalation. No automatic review rejection
occurred. Tools write profiles/captures only beneath project artifacts.

## Immediate next work

1. Finish original sound effects: resolve event IDs through pmario.proj/pool macros
   to the indexed sdir/samp sources, including pitch/envelopes/loops; bind cursor,
   jump, hammer, damage, guard and success events. Do not substitute synthesized
   effects or arbitrarily select bank samples. Start with tools/audio_convert.py,
   local-assets/sample-directory.json and docs/ASSETS.md.
2. Capture actual US GameCube tutorial gameplay in Dolphin. Confirm available
   commands, initial stats, timing windows, damage, guard restrictions, defeat,
   music selection, original dialogue and victory sequence. Replace provisional
   BattleRules and record measured evidence in docs/MECHANICS.md.
3. Implement original character animations and direction variants. The converter
   currently applies passed-keyframe deltas but does not interpolate full original
   animations. Vertex, visibility, texture-matrix and node tracks need full support. Ensure model sizing is correct and faithful.
4. Finish battle presentation: original area backdrop, opening-curtain animation,
   audience, correct prop visibility and camera. The common stage currently uses
   explicit hidden-node overrides for the mask, closed upper curtain and inactive
   switch props; these are inferred defaults, not Dolphin-verified visibility rules.
5. Improve plaza camera/foreground occlusion, original ground/slope/trigger semantics,
   partner navigation around obstacles and collision broad-phase performance.
6. Complete Azahar checks for successful/failed actions and guards, hammer release,
   touch battle selection, healing, defeat/retry and repeated transitions.
   Extend captures with explicit checks rather than treating screenshots as assertions.
7. Profile physical New 2DS XL when available. Optimize/decimate geometry and tune
   texture budgets; add room asset unload/load before expanding story content.

## Build/tool details

- Default ISO: /home/carson/games/roms/G8ME01.iso; override with TTYD_ISO.
- Azahar: /usr/bin/azahar; Dolphin: /usr/bin/dolphin-emu.
- Runtime save: sdmc:/3ds/ttyd-2ds/progress.bin with .bak and temporary .tmp.
- Isolated test save: artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/.
- --fresh-save resets only that generated test save; omit it to check reload.
- Source, generated/extracted assets and emulator profiles remain in the workspace.
  Original/compiled assets are ignored by .gitignore. The read-only .git placeholder
  is not a functional checkout; no commits exist. Do not delete it or claim commits.
- Build-time texture cap is 128px for maps; original source data is retained locally.
  Music keeps original sample rates/channels. Audio service failure is nonfatal.
- Room meshes remain resident and are reused through transitions; true room streaming
  is not implemented. No CIA packaging, full opening, additional chapters, or broad
  partner combat systems yet.

## Story/dialogue requirement update

User requires the original story and original US English dialogue, without
paraphrasing or invented substitute scenes. Recorded in AGENTS.md and README.md.
Removed authored NPC/Crump/Goombella dialogue from the harness; explicit
development notices now mark the missing implementation. Read-only extraction
of msg/US/gor_00.txt, gor_01.txt and global.txt with SHA256 provenance is in
local-assets/messages. Next: parse message IDs/control tokens, connect the
original intro event sequence and branches, and compare playback with Dolphin.
Do not treat merely inserting original lines into the fabricated plaza encounter
as faithful story reproduction. Original message playback is still unfinished.

Validation for this update: `make test` passed the core suite and 16 Python
tests; `make -j2 all` rebuilt the native .3dsx successfully. Emulator dialogue
playback was not rechecked in this update.

## Overworld continuation: original ground and partner traversal

Implemented original near-horizontal collision triangle export as map.ground,
packaged in RomFS. Portable groundAt performs barycentric height queries with
step-height constraints so ramps update actor elevation, missing floors block
movement and roofs cannot pull actors upward. Actor and shadow rendering use
player/partner elevation. Goombella follows a bounded path of Mario's travelled
positions instead of cutting directly toward Mario through obstacles. This is
still provisional: jump traversal, stairs, falling, dynamic geometry and partner
recovery need further work. Walls are still flattened and fixed plaza bounds
remain; collision broad-phase acceleration is not implemented.

Fixed restore when called with the game's own Progress reference. Added host
coverage for slope heights, inaccessible roofs, missing floor, step constraints
and self-reference restore. `make test` passed core scenarios and 16 Python
tests; `make -j2 all` rebuilt the embedded original asset pack and .3dsx.

Read-only extraction of rel/gor.rel and provenance is in local-assets. Indexed
39 original map/bero string occurrences in rogueport-event-strings.json. This
is evidence to resolve original exits, not a complete warp table. There are no
overworld room warps yet. Next concrete task: resolve gor_01 exit/event records
(including REL relocations), original node trigger positions and destination
arrival IDs; add native room asset loading and transactional room transitions,
then test reciprocal warps and save compatibility. Avoid inventing destinations
or treating original string occurrences alone as a verified connection graph.

The workspace now has a functioning git checkout (git log shows bf9fea8). Older
statements about a nonfunctional placeholder describe the earlier environment.

Azahar verification: `python3 tools/emulator_smoke.py --seconds 4` booted
the updated build and restored the existing 6 HP completed-encounter save.
Capture reviewed at artifacts/azahar/boot.png; foreground wall occlusion
remains. This boot check does not verify movement, slopes or room transitions.

## Overworld continuation: reciprocal Rogueport roads

Original roads now connect gor_01 plaza to gor_00 harbor, gor_02 east side
and gor_03 west side, in both directions. The asset pipeline parses original
REL self-relocations and BeroEntry records (0x3c bytes), matches contiguous road
tables to original map marker names, and rejects ambiguous/nonreciprocal links.
Geometry markers supply original AUTO-position centers. Native spawn inset .9,
trigger radius .7 and cooldown 45 ticks are provisional adaptation values.
Evidence and original record offsets are in local-assets/room-links.json and
romfs/manifest.json. Original reference layout was inspected at
https://github.com/jdaster64/ttyd-tot/blob/main/ttyd-tools/rel/include/ttyd/evt_bero.h;
no code from that header is compiled or copied into the runtime.

Three additional original rooms and texture packs are converted locally.
Renderer stages a destination room before freeing the previous one, synchronizing
GPU work before disposal. Main loop stages simulation arrival, validates target
collision/floor and swaps rendering before committing progress; failed loads
restore previous collision. Catch-up/input are reset across I/O. Partner follows
after entering, and transitions save the checkpoint. Only plaza shows the
provisional NPC/Crump encounter. Interior doors, pipes, story gates and gor_04
are still unsupported; original cutscenes/door animation/fades are unfinished.

Walls now retain vertical min/max extents (count high bit marks six-float wall
records); elevated harbor platforms are no longer blocked by lower walls.
Save version 2 keeps 32 bytes, using byte26 for room and signed byte27 for
elevation quantized to .1. Version 1 migrates to plaza/elevation0. Structural
decoding is independent of the currently loaded room; loading validates original
target collision/ground. Tests cover legacy decoding, elevation and invalid IDs.

Validation: make test passed core scenarios and 16 Python tests;
`build-host/core_tests --rooms` validated all six arrivals with original room
collision/ground, HP preservation, checkpoint encoding and cooldown. Azahar
asserted saved destination IDs on east/plaza round trip and two harbor/plaza
and two west/plaza round trips. Captures under artifacts/azahar were reviewed.
Room I/O produces transient frame drops; no hardware benchmark. A rendering
regression appears after streaming: some bottom-screen panels become black,
while save feedback remains visible. Additional C2D state reset before the bottom
screen is under verification; do not claim the rendering is fully accepted.

Reproduce isolated road tests:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_roads.py
```

Next: verify/fix Citro2D state after room streaming; expand reciprocal warp
testing to failed loads and restart in each destination. Improve camera Z and
foreground occlusion per original camera/event data. Add collision spatial
indexing and preserve distinct vertical segments sharing X/Z. Resolve original
interior/pipe triggers, conditions, door visibility and animation before expanding
connections. Original dialogue/story playback, sound events, full animation and
hardware validation remain required for the full playable remake.

Final streaming verification: resetting C2D state again before the bottom-screen
scene fixed the black-panel regression. Repeated the east-side test on the final
build (two round trips, saved-room assertions passed); reviewed
road-east-return-1.png with the full status/menu visible at reported 60 fps.
The earlier harbor/west runs predated this C2D-only fix. `make test`,
`build-host/core_tests --rooms` and `git diff --check` all passed again.
`make -j2 all` rebuilt successfully. The next agent should verify harbor/west
bottom-screen presentation and destination restart persistence, then camera and
foreground occlusion. No original story dialogue has been restored yet.

## Overworld continuation: framing, indexed collision and destination reload

Cameras now follow X and Z plus smoothed floor height. The fixed horizontal
clamp and fixed depth target were removed: restored/warped rooms initialize
the camera around the arrival, and exploration follows movement throughout
room depth. Field eye offset (0,4.8,6.5), target (0,1,-1.4), smoothing .07
are adaptation values, not measured original camera rails. Harbor arrival
now centers Mario instead of leaving him at the distant top edge. Foreground
geometry still needs occlusion handling; this does not solve the original
scripted camera system or all visibility problems.

Walls and floor triangles now build a two-unit spatial grid per successful
load. Runtime queries test nearby candidates instead of the entire room.
Coordinates/reference elevations are validated before cell lookup, oversized
indexes are rejected and failed indexing keeps the previously loaded data.
Empty imported wall sets are distinguished from the synthetic host fallback.
Converter deduplication now preserves distinct wall heights sharing X/Z,
fixing accidental replacement of lower/upper walls. Python regression coverage
checks stacked walls. Exported wall/surface counts: plaza 1240/1010, harbor
748/463, east 1555/878, west 1509/1037.

The touchscreen map now scales each room's original collision footprint into
its panel, filters walls to Mario's elevation, and displays Mario/Goombella
markers. It no longer assumes plaza bounds or shows Crump in other rooms.
Field labels use readable area names instead of internal pack filenames.

Verification:
- make test: portable core scenarios and 17 Python tests passed.
- build-host/core_tests --rooms: all six original-data arrivals passed;
  14,652 indexed collision/floor probes across four rooms matched exhaustive
  scans at multiple elevations, including cell boundaries.
- make -j2 all: original packs and native .3dsx rebuilt successfully.
- Harbor road sequence completed two round trips on the new camera/indexed
  build, with saved-room assertions. Reviewed harbor arrival capture: centered
  Mario, complete touch menu, reported 60 fps.
- West destination persistence sequence asserted room0 -> room3, HP7, exited;
  a second emulator launch without fixture/reset asserted room3 -> room0, HP7.
  This verifies destination restore through an actual usable return warp.
  Touch map capture reviewed in west room: original footprint and markers fit
  its panel. Bottom-screen menu remains visible after streaming.

Persistence reproduction (isolated test profile only):
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_persistence.py
```

Next concrete work: foreground occlusion / original scene visibility and
original camera/event data; destination restart checks for harbor elevation
and east room, failed-load recovery and broader room traversal. Resolve gor_04
and original interior/pipe triggers and story conditions before enabling them.
Native door/pipe animations, original dialogue/event sequencing, character
animation, SFX and hardware profiling remain incomplete. No full-remake or
hardware 60 fps acceptance claim is made.

## Character proportion correction

User reported characters very large relative to original maps. Applied one
shared .5 visual scale to character billboard height/width and matching shadow
footprints in field and battle. Map geometry, camera framing, source texture
resolution, actor positions and collision are unchanged. Relative character
proportions are preserved. This is an explicit visual calibration in response
to feedback, not a measured original actor-instance scale. Original AnimGroup
pose bounds alone do not prove game-instance sizing; compare the actual US
GameCube actors/doorways in Dolphin before claiming exact fidelity.

Validation: `make -j2 all`, `make test` (core + 17 Python tests), and
`git diff --check` passed. `python3 tools/emulator_smoke.py --seconds 4`
booted; reviewed artifacts/azahar/boot.png showing Mario smaller beside the
original plaza building/doorway. Battle sizing uses the same factor but was
not separately captured in this update.

## Overworld continuation: atomic collision rollback and saved-room recovery

Room collision now stages through an RAII CollisionTransition. Previous walls,
vertical intervals, floors, spatial indexes and map bounds stay in memory until
destination collision, arrival and rendering have all succeeded. Failed staging
or rejected arrival/rendering restores the old data without filesystem access.
Native warp failure no longer attempts an unchecked disk reload of the source
room. Startup save restoration also stages collision, rejects unavailable
positions coherently, initializes the default plaza camera/floor and shows an
explicit notice. Renderer synchronizes GPU work before disposing partial loads
on failure as well as before replacing a successfully loaded room.

Host tests cover truncated destination ground, previous room files removed
before rollback, failed commit, rejected staged arrival/graphics analogue, nested
rollback, bounds preservation, spatial-query restoration and successful commit.
`make test` passed core scenarios and 17 Python tests.
`build-host/core_tests --rooms` passed all six arrivals and 14,652 indexed-vs-full
scan probes. `make -j2 all` rebuilt the .3dsx; `git diff --check` passed.

Azahar persistence runs confirmed plaza -> harbor and restart harbor -> plaza
with HP7 preserved, exercising the elevated arrival. East-side persistence
confirmed plaza -> east with HP7; restart verification follows. Existing west
restart evidence remains in the preceding entry. No physical hardware tested.

Invalid-room fixture uses a structurally valid room1 save at (50,50), outside
its floor. Azahar displayed the fallback notice in a coherent plaza scene;
the script then moved Mario and saved, asserting room0, default HP10 and
expected plaza coordinates. Capture invalid-room-fallback.png was reviewed.
This verifies invalid-arrival recovery; it is not a native injected texture-load
failure test. Foreground wall occlusion is still visible in the plaza capture.

Reproduce saved-room / recovery checks using only isolated test profiles:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-road.bin --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/invalid-room-road.bin --seconds 4 --input-script tests/azahar_invalid_room.py
```

Next concrete task: foreground wall/prop occlusion and original visibility
rules, because Mario can still be obscured at normal plaza positions. Then
original door/pipe event triggers and room conditions (gor_04 and interiors),
original English story/dialogue, character animation and original SFX. Preserve
the shared .5 character correction from user feedback; original actor sizing
still needs Dolphin measurement. Successful outdoor warps are not proof of
full story, world rendering or gameplay acceptance. Goal remains active.

Final persistence result: restarted east room -> plaza with HP7 preserved.
Both harbor and east restart tests completed on this transaction build. The
final native rebuild includes GPU synchronization before partial-load cleanup;
normal east persistence succeeded afterward. No GPU failure was deliberately
injected. All reported source changes and reproducible fixture scripts are
recorded in the incremental commit for this update.

## Overworld continuation: exterior-wall cutaways

Implemented rendering cutaways for original exterior-wall S_gai_kabe scene
subtrees. Walls keep their original geometry/materials/textures; the converter
separates wall owners into batches, including inherited nested drawable pieces.
Runtime builds bounding boxes, probes camera-to-Mario foot/head and Goombella
body segments against original triangles, and skips only obstructing wall
groups. Eight simulation ticks of hold reduce toggling when a probe leaves an
edge. Ground, buildings, other props and collision are not removed by this
mechanism. Battle uses its normal room rendering. This is a handheld visibility
adaptation, not a measured reconstruction of original event-controlled cutaways.
Some caps or decorative pieces in separate original nodes may still require
original visibility rules; interior roofs/props are outside this classification.

Final packs retain original triangle counts: plaza 40,714, harbor 22,922,
east 33,444, west 34,885. Batch counts are now 104/78/98/115, with
26/6/14/13 tagged wall groups respectively. Data is recorded in manifest.json.
New portable visibility helpers use finite, two-sided segment/triangle tests
and slab bounds tests. Core scenarios cover winding, misses, behind-camera or
beyond-actor geometry, endpoint contact, parallel/degenerate triangles and
invalid bounds/NaNs. `make test` passed core scenarios and 17 Python tests;
`build-host/core_tests --rooms` passed six arrivals and 14,652 collision probes.
`make -j2 all` rebuilt the packs and native .3dsx.

Azahar fresh plaza boot was reviewed: Mario visible at the default position
previously obscured by the front wall, while nonblocking walls remain. Field
visibility sequence captured start, front movement, jump, left and rear movement;
reviewed front capture with Mario and partner visible and reported 60 fps.
Those movement captures are manual visual evidence, not pixel assertions.
The full existing encounter harness sequence reached victory at HP6 and returned
to the plaza; reviewed battle-select and battle-turn-5 captures at the corrected
shared character scale. West road script completed two reciprocal round trips,
asserting saved room IDs and exercising cutaway metadata after room streaming.
No hardware benchmark or original-story/mechanics acceptance claim.

Removed the leftover authored victory narrative line (plaza safe) in favor of
an explicit development-encounter notice. Partner display is a neutral status
label. Battle/victory/defeat now consistently display the battle-stage label.
Original English dialogue playback and story sequencing remain unfinished.

Reproduce:
```sh
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_visibility.py
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_walkthrough.py
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_roads.py
```

Next: reproduce original map initialization visibility from REL event records
(including apparent helper/trigger geometry now visible near front openings),
original camera rails, and doors/pipes with their event conditions. Map helper
geometry should not be broadly hidden based only on appearance. Resolve gor_04
and interior links from original records; retain atomic room acceptance.
Original character animations, SFX, English dialogue/story, battle observations
and physical hardware acceptance are still required. The full goal remains active.

## Overworld continuation: original station road

Packaged original gor_04 station geometry/textures/collision and enabled the
original reciprocal west n_bero / station s_bero road. Five rooms now have eight
directed roads. Global distinct-table/reciprocity matching replaces ambiguous
per-room marker-subset selection: station contains an extra north marker, which
must not be confused with harbor's table. Station event/train travel is not
implemented. Preserved room IDs 0–3; new ID4 supports save v2 with registry-based
validation and a host save roundtrip test. Invalid ID5 remains rejected.

Found and corrected marker-midpoint arrival elevation: station s_bero spans
Y0–2, so its midpoint caused ground validation failure. Arrival uses original
marker base height; all eight destinations now pass ground/collision acceptance.
`make test`: core suite and 17 Python tests passed. `make -j2 all` passed.
`build-host/core_tests --rooms`: 18,315 indexed/full-scan collision probes and
eight original room arrivals passed. Azahar station road script completed two
round trips with saved room IDs asserted and HP7 preserved. Reviewed
road-station-arrival-0.png: station art and Mario entrance visible, emulator
reported 60 fps. Train components/static scene show rendering/visibility issues;
this is not original event-state or hardware acceptance. Initial midpoint build
failed both host arrival and native station entry; corrected build passed.

Reproduce:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_roads.py
```
First script also leaves a station checkpoint for restart; second detects room4
and exercises station->west->station. Restart result is recorded below when
verified. Next: original station train/map event visibility and original camera
behavior; doors/pipes/interior connections with event conditions; original
English story/message playback, full character animation and original SFX.
The full playable-game objective remains active.

Restart verification completed: updated station script left room 4/HP 7 checkpoint;
second invocation without fixture or save reset loaded station, completed two
station->west->station round trips and exited with room 4/HP 7 saved. Both scripts
returned exit0. Physical hardware, train travel, story conditions and original
station event/visibility fidelity remain unverified.

## Overworld continuation: original mesh sidedness

Converted original per-node GX face-culling settings into batch metadata.
Original drawable nodes use back culling or no culling in the packaged rooms;
front culling remains supported by the format. Materials now split by sidedness
as well as cutaway group; unchanged triangles/collision, modestly more batches.
Metadata packs original layer in low byte, group in bits8–23 and cull index in
bits24–25. Runtime masks fields separately, rejects unknown high bits and resets
culling for paper actors/2D rendering. Null draw-mode pointers on hierarchy-only
world_root nodes are valid (initial strict pointer reading rejected them).

First direct GX/PICA enum mapping removed plaza floor; inspected Azahar capture
and reversed enum mapping for current view transform. Corrected plaza boot
shows original ground/building fronts, and the moving/jumping cutaway capture
sequence completed. Station script completed two round trips plus station save
on corrected build, asserting room IDs. Reviewed station entrance image: static
train geometry still has malformed/black components; sidedness does not fix
original animation/event state, which remains next fidelity work.

Some Xvfb screenshots contain partial black areas in bottom UI. Temporary
command-buffer probe measured 0.1992 usage, ruling out exhaustion in that plaza
case; diagnostic code was removed before final build. Fresh boot capture showed
full bottom UI. A separate isolated station run forcing Qt window resize/repaint
showed restored menu tabs/status; resize clipped the lower screen in the fixed
1000x800 capture surface. Evidence points to virtual-display repaint/capture
behavior, but no broad menu-rendering acceptance claim is made from partial
captures. Do not silently ignore future reproducible native UI failures.

`make test` passed core and 18 Python tests (batch field isolation and invalid
metadata boundaries added). `make -j2 all` passed;
`build-host/core_tests --rooms` passed all eight arrivals and 18,315 probes.
Reproduce corrected field/station checks with the existing visibility and road
commands above. Battle-stage verification result follows below.
Next: original train/map event visibility and animations, original camera rails,
door/pipe and interior event links, original English message/story playback, SFX
and measured mechanics. Hardware acceptance still unavailable; goal active.

Final battle check: existing walkthrough returned exit0, reached victory at HP6
and returned/saved plaza. Reviewed battle-select and battle-turn-5 captures: stage
floor and curtains retain visible faces, bottom command/result menu fully drawn,
reported 60fps. Goombella remains partly behind the left curtain, and stage props
are still static; original scene presentation remains incomplete. No hardware
performance or observed-original-mechanics claim. Final code contains no temporary
command-buffer probe.

## Overworld continuation: original map mesh animation extraction

Added bounds/count/finite-validated original map mesh animation reader and
Hermite/step frame sampler. Conversion writes original mesh tracks into ignored
local JSON sidecars, records inventories in manifest, and accepts explicit
--animation/--frame visual previews. Original translation/rotation offsets and
scale dividers are respected; hierarchy bind*delta transform order follows
existing format reference. Collision remains static in previews; runtime packs
remain at bind pose. Original event activation, loop modes and playback are not
invented. Material animation table offsets are inventoried, not decoded.

Evidence changes the next station task: gor_04 mesh animation table contains
shadanki and n_1_door_open/close, with remaining entries having no mesh tracks.
There is no train-body mesh track in this table; the malformed/black locomotive
needs original material/scene/event inspection rather than an assumed idle clip.
The door clip targets node tobira. Sampling frames 0/23 changes exactly 702
vertices; UV/colors, batches, collision wall and ground files remain identical.
54 finite original samples across 18 tracks in all five maps and battle stage
passed `python3 tools/verify_map_animations.py`. Duplicate harbor fune_b names
are preserved and name-based preview rejects ambiguous selection.

`make test` passed core plus 20 Python tests, including real-layout synthetic
parser fixtures, truncation/count/nonfinite/step/divisor rejection, clamp behavior
and interpolation. `build-host/core_tests --rooms` passed 18,315 collision probes
and all 8 original arrivals. Native build result recorded below on completion.
No runtime animation/emulator/hardware acceptance claim for this offline step.

Reproduce original asset validation after `make all`:
```sh
python3 tools/verify_map_animations.py
python3 tools/map_convert.py local-assets/gor_04.d /tmp/ttyd-door-open --animation n_1_door_open --frame 23
```
Next concrete tasks: export original node hierarchy/vertex bindings for runtime
mesh playback; decode material animation tracks; recover REL door/event
selection and collision/visibility rules before enabling transitions. Inspect
train material modes, source vertex colors and node transforms to explain the
remaining locomotive defects. Continue English dialogue/story, original SFX
and measured combat; full playable objective remains active.

Final `make -j2 all` completed successfully with animation extraction included in
the asset fingerprint. No runtime mesh playback has been enabled by this change.

## Overworld continuation: material tracks and original init-call evidence

Implemented original material animation decoding and explicit offline UV frame
previews, sharing static/animated texture matrix math. Supported texgen0 material
binding with original names and _v/_x aliases; higher texgens produce a limitation
record. East mizu animation contains both texgen0 and1 tracks on lambert119, so
full water fidelity still requires second-layer rendering. Runtime animation
selection/rates/loop semantics remain unfinished. No default scene animations
were guessed or automatically activated.

`make test` passed core plus 21 Python tests: original-layout material fixture,
invalid texgen rejection, independent S/T scale interpolation and matrix/finite
boundaries. `make -j2 all` passed. `build-host/core_tests --rooms` passed all 8
arrivals and 18,315 collision probes. `python3 tools/verify_map_animations.py` passed
63 material samples across 21 tracks plus 54 mesh samples. East water midpoint
changes 135 UV vertices; west neon_01 midpoint changes 2,010; geometry/colors,
collision and batches remain identical. Existing 702-vertex station door sample
also passed. No new emulator or hardware animation acceptance claim.

Added independent read-only map event inventory, using publicly available
jdaster64/ttyd-utils US symbol CSV (external, ignored). Original REL event headers
provide argc/opcode; full command order/operands/relocation provenance retained,
no script interpreter or branch evaluation. Inventoried gor init scripts: harbor
starts nami/nami_02/hiki_nami; plaza hae; east mizu/noname/ie_01_light; west mizu.
These calls do not alone prove unconditional activation. Station init has
evt_mapobj_flag_onoff at 0x6073c with operands (1,0,train_2,2), and S_bero at 0x6051c
with (1,0,S_bero,2). Determine flag semantics and enclosing conditions before using
these as rendering defaults. This is concrete evidence for the remaining train
visibility issue, rather than assuming an absent train mesh clip.

Reproduce after obtaining external public US symbol CSV:
```sh
python3 tools/map_event_inventory.py --symbols artifacts/reference/us_symbols.csv --output local-assets/map-init-events.json
python3 tools/verify_map_animations.py
python3 tools/map_convert.py local-assets/gor_02.d /tmp/ttyd-water --animation mizu --frame 499.5
```
Next: interpret original map flag/animation-call semantics and branch conditions;
export runtime node/material bindings, enable proven ambient tracks, and repair
train scene visibility. Continue original camera/door/pipe/interior events and
English story/dialogue, SFX and measured combat. Full playable objective active.

## Overworld continuation: source-derived station visibility

Added read-only DOL function inspection with external US symbols and llvm-objdump
to resolve map visibility semantics from actual original bytes. Original
evt_mapobj_flag_onoff 0x80038468: selector0 chooses object flags, selector1 group
flags; onoff0 calls Off, nonzero On. mapGrpFlagOff 0x8001ecb8 calls mapFlgOff
0x8001eee8 recursively over group descendants, leaving owner siblings alone.
mapDisp 0x80024b08 skips nodes with bit0x4000, or flag 0x1 without flag 0x80.
Bit2 gates clipping calculations at 0x80024c54; when clear, branch to0x8002510c
still reaches rendering. Therefore train_2's unconditional bit2 clear is NOT
a hide instruction. Earlier possible train-visibility interpretation is ruled
out; do not suppress train_2 based on that call.

Verified opcode identities against primary evt_cmd.h: 12 IF_STR_EQUAL,32 ELSE,
33 END_IF. Original station init branch 0x604d0 compares entername with
hikousen_return; ELSE 0x604e4 sets group hidden flag 0x1 on gondora_2/hako_1.
Current station road arrives s_bero, so this branch applies. Implemented those
original visibility defaults for the packaged ordinary-road slice. Converter
now suppresses descendants: both group owners have zero own parts, so prior
leaf-only hiding could not implement this rule. Station now 41,084 triangles
(314 fewer), 54 batches; collision and texture inventory unchanged. Remaining
locomotive appearance has not been corrected by this rule or compared with
Dolphin station footage; original camera/lighting/transforms still need evidence.

`make test` passed core plus 21 Python tests; `make -j2 all` passed.
`python3 tools/verify_map_visibility.py` passed exact 314-triangle descendant
suppression and unchanged collision/textures. `build-host/core_tests --rooms`
passed all 8 arrivals and 18,315 collision probes. Original animation verifier
still passes 63 material/54 mesh samples and door/water/neon previews.
Azahar station road result recorded below after terminal completion.

Reproduce source inspection (requires ignored public US symbol CSV):
```sh
python3 tools/inspect_dol_function.py evt_mapobj_flag_onoff --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/evt-map-flags.elf
python3 tools/inspect_dol_function.py mapGrpFlagOff --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/group-flags.elf
python3 tools/inspect_dol_function.py mapDisp --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/map-display.elf
python3 tools/verify_map_visibility.py
```
Primary opcode reference:
https://github.com/jdaster64/ttyd-tot/blob/main/ttyd-tools/rel/include/evt_cmd.h
No original/DOL/decomp executable implementation imported into runtime.
Next: preserve runtime scene nodes and bindings, prove ambient-animation
activation/loop semantics, original camera framing, and expand door/pipe/interior
event conditions. Build-time station subset must be replaced with dynamic
visibility when implementing hikousen_return. Original story/English messages,
SFX, measured combat and hardware acceptance remain incomplete; full goal active.

Final native check: station road script returned exit0, completed two reciprocal
round trips and final station entry; saved room 4/HP 7 verified. Reviewed
road-station-arrival-0.png: Mario entrance and full bottom status menu visible,
reported 60 fps. No claim that the remaining locomotive appearance or original
station story/event behavior is accepted.

## Overworld continuation: native original east material playback

Enabled native texture-zero playback for original east init's mizu/noname/
ie_01_light clips. Verified init call offsets 0x4d694/0x4d6a8/0x4d6bc are outside
conditional blocks. Inspected original US DOL evt_map_playanim, mapPlayAnimationLv,
mapMain and marioStInit: mode1 loops full duration; clock0; default speed1;
elapsed-ms frame calculation uses framerate field initialized 60 for NTSC
(VIGetTvFormat0), 50 otherwise. Upstream viewer duration-1 loop shortcut is NOT
used. Port follows fixed60Hz simulation ticks and resets epoch on room load.
This does not implement original pause/clock behavior during broader story events.

Original Hermite/step matrices baked per integer frame with original raw-UV
bindings. New portable UVAN loader rejects invalid format/version/counts,
nonfinite matrix/UVs, out-of-range/duplicate indices and trailing bytes; output
remains unchanged on failure. Native updates 369 vertices, flushes contiguous
cache ranges and streams/disposes animation metadata with rooms. East file
41,856 bytes; durations999/159/400. All rooms/stage have mandatory valid files,
empty where no proven material-only clip is enabled. Texture-zero east water
is animated; original texgen1 water layer remains missing. No mesh clip enabled
partially: ambient exporter rejects selected clips with mesh tracks.

`make test` passed core plus 21 Python tests. Core now tests UV coordinate
transforms, full-duration loop boundary, invalid-file atomic recovery.
`build-host/core_tests --rooms` passed original all 8 arrivals, 18,315 collision
probes, all room UV metadata imports and three east track loop probes.
Original animation/visibility verifier scripts passed. Native build passed
without warnings after correcting an initial indentation warning.

Azahar existing east road script completed two reciprocal round trips with
saved room IDs asserted. New stationary ambient capture script completed
east entry, movement away from marker, three captures at 1.5 s spacing and return
to plaza. Reviewed east-ambient-180: field/status render and reported 60fps,
sim 0.17/CPU 3.9/GPU 0.5 ms displayed. Transition capture temporarily reported 20fps;
steady capture reported 60. Partial black repaint artifacts persist in some
virtual-display screenshots; no pixel assertion or Dolphin side-by-side visual
acceptance claim. Native runtime update logic/asset imports are verified, but
original complete material appearance and hardware performance remain unproven.

Reproduce:
```sh
make test
make -j2 all
build-host/core_tests --rooms
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_ambient.py
```
Next: preserve runtime scene nodes for original mesh tracks (harbor water/boat,
plaza hae, west mizu), implement second-layer material rendering, original camera
framing and event visibility. Door/pipe/interior event links, original English
story/dialogue, SFX and measured combat remain necessary. Full goal active.

## Character size correction

Reduced the shared paper character scale from 0.5 to 0.35 following the
reported oversized characters. Heights, widths, paper tilt and shadows are now
30% smaller across field and battle; original map geometry remains at its
existing scale. This is a visual calibration, not measured original actor size.

Validation: `make test` passed core and 21 Python tests; `make -j2 all` built
`ttyd-2ds.3dsx`. `python3 tools/emulator_smoke.py --fresh-save --seconds 4`
exited successfully. Reviewed `artifacts/azahar/boot.png`: smaller field actors,
full dual-screen UI, reported 60 fps. Hardware performance is unverified.
Next: compare character proportions and camera framing against Dolphin.
Existing unfinished ambient-animation edits remain in the worktree separately.

## Native plaza and harbor ambient animation; harbor retreat fix

Completed the prior worktree's MSAN mesh animation path. Plaza original hae
animates12 vertices across2 owners (399 frames). Harbor original nami animates
4,581 vertices across3 owners (979 frames), alongside nami/nami_02/hiki_nami
texture-zero material playback (4 tracks,5,451 UV vertices). Original ancestry
is retained during conversion and world delta matrices are baked at integer
frames; native mesh/UV updates share the original full-duration loop epoch.
No cumulative transform drift; mandatory empty headers packaged for unused
rooms/stage. Matrices selected once per track before bound-vertex updates.

Native harbor verification initially FAILED return-to-plaza after walking away
from the marker. Host reproduction proved Mario could change from the entrance
floor to a lower surface inside a wall, then could not retreat. Walking now
checks collision at sampled destination elevation as well as previous elevation.
Added original entrance regression:60 ticks toward harbor, each position stays
walkable, then retreat requests the plaza warp within120 ticks. Map collision
data is unchanged. This does not establish complete staircase/prop semantics;
center harbor entrance is obstructed by original posts and railing.

Verified commands:
```sh
python3 tools/verify_mesh_animation.py
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-road.bin --seconds 4 --input-script tests/azahar_mesh.py
```
Mesh verifier passed five original poses per room (plaza0/1/27/155/398;
harbor0/1/155/400/978), unchanged collision and batch files. Core plus21 Python
tests passed; original room checks passed all8 arrivals and18,315 indexed vs
full-scan collision probes, actual vertex-count animation bounds, loop checks
and new harbor retreat regression. Native build passed. Final harbor script
passed entry/HP7/three stationary captures/return-to-plaza assertions after fix.
Reviewed harbor-mesh-240: full status UI, reported60fps, sim0.20/CPU5.2/GPU0.5ms.
The current entrance camera obscures the dock/waves; these captures do not prove
visual wave fidelity. Original sampled mesh math and native loading are verified.

Limits: second texture layers and nami/fune_b_lambert83 missing visible material
binding remain recorded in manifests. No conditional boat animation, animated
collision, dynamic visibility or original event/pause behavior added. Physical
hardware and full original presentation remain unverified.
Next concrete task: original harbor traversable floor/wall attributes and camera
framing, then second-layer materials. Continue original English dialogue/story
playback, SFX, measured combat, doors/pipes/interiors; full objective remains active.

Additional final native road checks passed two reciprocal round trips each for
east, west and station, with saved room assertions. Station script also entered
the station for its final checkpoint. `python3 tools/verify_map_visibility.py`
passed original road-entry314-triangle omission/collision/texture invariants.
Reproduce:
```sh
python3 tools/verify_map_visibility.py
for route in east west station; do
  python3 tools/emulator_smoke.py --fresh-save --save-fixture "artifacts/fixtures/$route-road.bin" --seconds 4 --input-script tests/azahar_roads.py
done
```

Final `python3 tools/verify_map_animations.py` passed63 material/54 mesh
finite samples,702 moved station door vertices and prior east/west UV probes.
Final `python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script
tests/azahar_walkthrough.py` completed the development battle sequence and
return. Reviewed return-plaza.png: smaller actors/full status UI; saved32-byte
checkpoint has room0/HP6/victory flag1. This validates stage resource loading and
return flow, not original battle mechanics or story acceptance.

## Harbor staircase traversal

Investigated original harbor geometry instead of deleting collision or moving
the warp. The entrance stairs have0.1-unit risers. Existing ground sampling
accepted0.25-unit ascent but wall clearance used0.1, so an actor's projected
footprint collided with nearby risers during descent. Unified both through
`StepHeight=0.25` and retained source geometry. This is a provisional native
allowance, NOT a Dolphin-measured original rule. Partner destination elevation
now also receives collision validation, as Mario's does.

Host regression follows the original right-hand staircase, crosses onto the
solid central dock floor at y0.15, checks Mario/partner positions stay walkable
and both reach that height, then retraces stairs and requests the plaza warp.
Additional synthetic collision cases ensure0.1 risers clear,0.3 obstacles block
and overhead walls do not block floor movement. All8 road arrival tests and
18,315 indexed/full-scan collision probes pass under the new clearance.

Initial traversal probe continued into water-side surfaces at x5.6/z2.4/y-0.1;
that did NOT prove solid dock traversal. Revised accepted route stays on the
solid dock, with native saved elevation0.2 (quantized original y0.15) asserted.
Water/fall/recovery behavior remains unfinished: raw original A_mizu collision
flags are0x200 at node drawmode+8, A_mizu_kabe0x5. These values were read from
the user's m/gor_00/d; their runtime semantics have NOT been recovered. Do not
infer universal hazard semantics from names or the unrelated0x40000000 viewer
flag. hitGetAttr/hitCheckAttr and Mario motion in original DOL are next evidence
sources. Current floor packs still lack original per-node attributes.

Validation (all exited0):
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core plus21 Python tests passed. Native harbor script asserted solid dock
position/elevation/HP7, captured three stationary frames, and returned to plaza.
Station passed two reciprocal round trips and final station checkpoint. Reviewed
final harbor-dock-240: full menu, reported60fps; sim0.23/CPU5.1/GPU0.5ms.
Boat geometry obscures Mario at this central camera position; no original-camera
or complete harbor visual acceptance claim. Hardware remains unverified.

Next concrete task: retain original collision-node attributes with geometry and
recover water/fall recovery semantics, then original harbor camera/framing and
boat visibility/second-layer materials. Original English dialogue/story events,
SFX, broader traversal/doors/pipes, and measured combat remain incomplete.

## Original floor attributes retained through native movement

Converter now retains original collision node's drawmode+8 attribute word and
data-relative source-node ID for each exported floor triangle. Tagged high-bit
count selects44-byte records (9 floats +2 uint32); untagged36-byte legacy packs
still load with zero metadata. Coordinate/normal selection and wall geometry
remain unchanged. Manifest indexes original collision-node names and flags.

Native GroundContact is selected with the same highest supported floor as
height, then carried through Mario/partner movement, save restoration and warp
arrival. Contacts are reconstructed from geometry on load, not persisted in
saves. Import/staging owns attributes with geometry/index and preserves the old
room on failure. Query misses preserve caller outputs. No guessed water action
or collision-mask meaning was introduced; wall attributes and event mutations
remain missing from runtime.

Original DOL inspection establishes `_hitEnt`80018aa8–80018ac0 initializes
hit-entry+4 from node drawmode+8 (or0 when absent), and hitGetAttr80014310
returns it. marioLandOn80092de4–80092e98 tests0x200 then enters damage motion31
under additional state conditions. Original motion table31 points to
mot_damage8013ae04. Recovery timing/position/HP/effects still require full source
trace and Dolphin observation. Original DOL bytes were only read/disassembled;
none executed or linked into native runtime.

Validation:
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/verify_ground_attributes.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core/21 Python tests passed. Legacy zero metadata, tagged floor selection,
query-miss output preservation, invalid tagged import retention, and failed
room-transition attribute rollback/commit are tested. All8 original arrivals
and18,315 collision probes pass; exhaustive floor oracle now also compares
selected attributes/source IDs. Source verifier passed all3,576 triangle
attributes across five rooms (1010/463/878/1037/188). Harbor has16 A_mizu floors
with0x200. Native harbor solid dock/save/return and station two round trips plus
final checkpoint passed with tagged packs. Original water behavior is incomplete.

Reproduce byte inspections using tools/inspect_dol_function.py, external ignored
artifacts/reference/us_symbols.csv and ignored ELF outputs, with functions
`hitGetAttr`, `hitCheckAttr`, `_hitEnt`, and `marioLandOn`. Original motion table
was inspected read-only from DOL file bytes; no external implementation reused.
Next concrete task: trace mot_damage/mot_bottomless and observe harbor water
fall/recovery, then reproduce the original sequence using retained floor flags.
Continue original camera/boat visibility, layered materials and English story
events; these remain essential unfinished work.

Final native east check also passed two reciprocal round trips with saved-room
assertions, covering east/plaza streaming with tagged floor metadata:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```

## Original water-damage trace and authored return-table extraction

Traced original mot_damage, hitGetDamageReturnPos, evt_hit_damage_return_set,
hitGrpDamageReturnSet, hitDamageReturnSet, hitObjGetPos/Sub and pouchAddHP.
Detailed addresses/control-flow evidence now in docs/MECHANICS.md. Source
shows ordinary character-byte0 loses1 HP after landing/recovery counter50;
zero HP follows a different state. Full timing and observed presentation remain
unverified. Return selection uses saved hit object+0x208 or source map fallbacks;
it does not choose an arbitrary nearby point. Initial return binding sets
0x40000000 on the original hit group/descendants.

Added bounded return-table parser to the asset pipeline, using US DOL import
address and REL relocation provenance without making the external symbol table
a build dependency. Two init calls,17 ordered source entries and5 Y=-1000
automatic-position markers are retained in local ignored JSON and pack manifest.
Literal harbor return points and sentinel cases are verified against original
source. Automatic positions are NOT guessed or replaced. East A_ground_01 is
a hierarchy-only owner absent from the current geometry-owner index: retain full
hit hierarchy before propagating return bindings. Native water recovery is still
unimplemented; this step supplies its original rules and authored inputs.

Validation:
```sh
python3 tools/verify_damage_returns.py
make test
make -j2 all
build-host/core_tests --rooms
python3 tools/verify_ground_attributes.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
```
All exited0. Core plus25 Python tests passed, including valid literal/sentinel
tables, invalid command/missing relocation/nonfinite/truncated/unrelocated
termination rejection, and unrelated import exclusion. Native pack rebuilt and
manifest contains2 tables/17 entries. All8 road arrivals,18,315 probes, and3,576
source floor attributes passed. Final Azahar solid-dock/save/return passed after
REL helper refactoring. No new gameplay acceptance or hardware performance claim.

Next concrete task: retain original hit hierarchy/leaf centers, apply the
verified init return-binding mutations, trace saved-hit+0x208 writers and
implement water damage/recovery using that evidence. Obtain actual Dolphin
fall/recovery observations before declaring timing/animations accepted. Original
camera/boat visibility, layered rendering, English story events, SFX and broader
traversal remain incomplete; full objective active.

## Original hit hierarchy and native authored return bindings

Retained all original collision hierarchy owners/parents/part counts/transformed
hit centers, including hierarchy-only A_ground_01. Automatic return resolution
averages transformed leaf centers; ordered group overrides bind descendants
without leaking to the owner's siblings. Pack activates only the two reviewed
unconditional init calls. No guessed safe point or whole-room center substituted.

Found and corrected a prior coordinate assumption BEFORE enabling recovery:
original _mapLoad uses uniform10x model-to-engine scale, read directly from US
DOL at800294dc–80029514 and constant8041f944=10.0. Literal event positions
therefore use native map scale0.1/10=0.01; previously mechanics notes incorrectly
suggested0.1. Model-derived centers already include native scale. Harbor authored
returns now correctly(.84,.15,2.2)/(4.75,.15,-1.8), automatic A_yuka approximately
(.85,.15,2.2). Original return point geometry stays in-room.

New mandatory per-room RPOS files load atomically with collision. Harbor5
bindings92 bytes; east16 inherited bindings268 bytes; other rooms12-byte empty
headers. Ground contacts include original init return-enabled bit0x40000000
and active-room groundReturn queries retrieve authored XYZ. Snapshot rollback
includes return maps, so failed load/staging preserves previous room bindings.
Raw source flags in ground packs remain unchanged. Runtime currently uses baked
init centers; dynamic hit transforms and repeat return-table events remain open.
Water damage/animation/HP/recovery and saved-hit selection are NOT implemented.

Initial build failed from a shadowing local struct import; fixed by retaining
one module import. Initial native solid-dock test reached correct floor but
failed its strict save byte==2 assertion: original y0.15 lies on a0.1-unit
quantization midpoint, and floating interpolation can save it as0.1 or0.2.
Changed that assertion to accept both quantized values; coordinate/HP/room
assertions retained. Final native script passed. No movement rule was changed
to satisfy that test.

Validation (final commands exited0):
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/verify_ground_attributes.py
python3 tools/verify_damage_returns.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core plus27 Python tests passed. Tests cover original leaf averaging vs owner
center/sibling exclusion, ordered overrides, malformed hierarchy, RPOS version/
finite/duplicate/EOF/atomic output preservation, return-map staging failure
rollback and commit. All8 road arrivals and18,315 indexed/full-scan probes
passed, now including effective init flags. All3,576 raw floor records and21
native return bindings verified; source golden checks distinguish event/model
units. Azahar solid dock/save/return and east two reciprocal round trips passed.
Hardware and full original presentation remain unverified.

Next concrete task: trace original writers of Mario saved-hit+0x208 and remember
return-enabled floor exactly, then implement source-derived water damage/recovery
sequence with authored bindings. Dolphin fall/recovery observation remains
required for timing/animation acceptance. Original camera/boat visibility,
layered rendering, English story events, original SFX and broader traversal
remain essential unfinished work; full objective active.

## Original return-floor selection and normal trigger-floor filtering

Recovered the actual saved-hit+0x208 writer in original marioMoveMain; it
remembers current floor only if0x40000000 is set, before horizontal movement.
Added native returnContact/recoveryReturn using that rule. Unbound/water contact
does not overwrite the remembered owner, and room restoration resets it.
Source-derived normal-Mario bit4 floor filter now excludes two west trigger
floors; original geometry/attributes remain packed unchanged. No guessed
respawn, water damage, camera freeze, animation or HP rule added yet.

A host path expectation initially failed: route crosses Ship_Change02 after
A_yuka_04, so the latest eligible source return is(5.5,.05,-1.9), not the earlier
dock point. Corrected that expected original owner; implementation unchanged.
Regression proves current water floor remains unbound, remembered eligible
return persists through idle/water, query returns authored XYZ, and a new room
clears it. Synthetic filter tests cover bit4 rejection and all identified
inclusion alternatives. Source pointer/ground-filter/camera-freeze evidence
is recorded in docs/MECHANICS.md.

Validation:
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
All exited0. Core plus27 Python tests passed; all8 arrivals and18,315 indexed/
full-scan probes plus new original return-selection/reset scenario passed. Native
build passed. Azahar west two reciprocal round trips passed after filtering.
Native harbor stair verification is also run below. Source geometry unchanged;
no hardware or complete original-mechanics acceptance claim.

Next concrete task: implement source-derived water recovery stages with eligible
return selection, field camera/NPC freeze, source one-HP/zero-HP behavior and
original presentation. Need original map fallback when no eligible owner exists,
jump/ground contact behavior and actual Dolphin timing/animation observation.
Water remains walkable in the development slice until that state machine is
implemented. Original story/English messages, SFX, camera/boat visibility,
layered rendering and broader traversal remain incomplete; full objective active.

Final native harbor solid-dock position/elevation/HP/save and return-to-plaza
assertions also passed (exit0):
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
```

## Native water damage/recovery and checkpoint reload

Implemented portable original-derived water motion stages and integrated them
with grounded0x200 contacts. Uses remembered authored return or original ordinary
map fallback, source sink/hold/rise/travel/fall parameters, camera/follower freeze
and blocked menus/input/warps. Horizontal movement uses source speed8 and
truncated tick count, leaving less than one step of residual distance instead
of snapping exactly to the point. Source landing counter fall-through and HP
penalty boundaries are tested. One HP lost; HP0 enters FieldDefeat and offers
checkpoint load, preserving saved HP instead of healing. Native save-load stages
room collision/returns/rendering atomically; water positions cannot save or
write on START exit. Existing battle defeat harness remains separate.

Original marioJumpData four fall parameters read from US DOL(.sdata source):
-.1,-.2,.024,-.004; terminal-18 and mot_damage cap-10. Source camera/NPC calls
were traced. Implementation math is native, without executing/linking original
code. Original source counters are not claimed observed timings. Original splash/
chomp/pose/SFX/game-over assets and dynamic landing ray remain unfinished.
Current controller lands using the static supporting floor at authored target;
wall widths/flags and full original air contacts are still fidelity limitations.

Validation:
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-last-hp-road.bin --seconds 4 --input-script tests/azahar_water.py
```
All exited0: core plus27 Python tests; all8 room arrivals/18,315 probes; water
source-stage progression and bounded completion, landing/HP counter boundaries,
zero HP, camera/follower freeze, ignored controls, no intermediate save, fallback
and field-defeat reload-request checks. Azahar HP7 run returned near the authored
Ship_Change02 position and savedHP6. HP1 run reached game over, preserved original
checkpoint bytes, reloaded HP1/entrance position and saved without healing.
Reviewed positive water-result (copy water-positive-result.png): actor returned
near original dock, full status UI, reported60fps. Reviewed
water-checkpoint-reloaded.png: HP1 and original harbor entrance, full UI. No
hardware performance or full original water-presentation acceptance claim.

User expanded priorities now explicitly include warp fade effects and proper
trigger/arrival distances, render proportions/distances, and correct physics.
These remain active requirements. Next concrete task: source-derived road fade
and entry/exit travel distances, then correct actor collision widths and remaining
water presentation/ray behavior. Preliminary original evt_bero inventory shows
road Bero records use auto direction20000/position100000/length-1; do NOT treat
those sentinels as literal lengths. Native .7 trigger/.9 inset/45-tick cooldown
remain provisional; fade is currently absent. evt_bero_info_run_road at8030e97c,
bero_case_exec_road and evt_fade_set_mapchange_type are next evidence sources.
Full English story/dialogue, original SFX, camera/boat visibility, layered
materials and broader traversal remain incomplete; full objective active.

Final road regression after water integration passed two reciprocal east/plaza
round trips with saved-room assertions (exit0):
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Normal gravity1.0 was additionally verified in original marioMain for Rogueport;
moon-only0.5 overrides do not apply. See mechanics notes for original addresses.

## Default road fades and landing gate

Added original-default300ms black linear fade-out/in to both screens. Queued
road crossings disable controls and wait for the existing jump to land first.
Only after an opaque frame is submitted does main stage/load the next room.
Successful commit fades in the destination; rejected load restores original
room resources and fades back in. Input/menus/camera/partner movement stay
frozen during fades. START does not save a partial departure. No original
assets were changed. Your separate AGENTS.md edit remains outside this feature
commit.

Verified defaults from original US DOL marioStInit/seq_mapChangeMain/evt_fade_in
and fadeMain (addresses in mechanics notes): type9 in/type10 out,300ms, black
RGBA, linear progress. Native18 ticks corresponds NTSC60Hz. Source road-event
landing gate also recovered. This does not claim original complete transition
choreography: scripted exit/entry travel and sensor/arrival distances remain
unfinished/provisional. Rendering proportions and general physics still require
original-game comparison.

Validation (all final commands exited0):
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_fades.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core plus27 Python tests passed; all8 arrivals/18,315 collision probes passed.
Fade tests cover monotonic out/in, endpoints, no loading before opaque-render
acknowledgment, blocked controls/saves, failed-load fade-in and airborne landing
gate. East and station native scripts each passed two reciprocal trips; station
also entered its final checkpoint. Visual fade script passed partial/black/
restored paper-fill brightness and destination-room assertions on both screens.
Final samples for both screens included238.7→159→79.7→0; final fills restored
above220. Initial probes were panel-border pixels; corrected probes to paper
fill and reran before claiming that result. Reviewed fade-02 and fade-finished:
old room dims, new room/status appears; known Xvfb repaint artifacts remain
possible, so no whole-frame/Dolphin visual-fidelity claim. Hardware unverified.

Next concrete task: implement original scripted road travel and correct initial/
arrival placement using bero_get_position_normal, evt_bero_initial_pos and
evt_mario_mov_pos2. Source30-unit offset and FLOAT150 rate/length are now traced;
DO NOT substitute them directly for all distances without the entry sequence
and floor/ray checks. Native .7 trigger/.9 inset/45-tick cooldown remain
provisional. Then recover actual actor collision widths/ground physics and
compare render proportions/camera in Dolphin. Original English story/dialogue,
SFX, layered materials, water presentation and broader traversal remain
incomplete; full expanded objective active.

## Further character size reduction

Following the latest oversized-character report, reduced the shared render
scale from 0.35 to 0.24 (31.4% smaller in each dimension). Mario, Goombella,
NPCs, battle actors, tilt and shadows use the same calibration. Map geometry
and collision dimensions are unchanged. This remains visual calibration;
original actor proportions need a matched Dolphin camera comparison.

Validation: `make test` passed core and 27 Python tests; `make -j2 all`
produced `ttyd-2ds.3dsx`. `python3 tools/emulator_smoke.py --fresh-save
--seconds 4` exited 0. Reviewed `artifacts/azahar/boot.png`: smaller actors
against plaza buildings and dual-screen rendering, with emulator reporting
60 FPS. Hardware performance is unverified.

Next: compare actor/map proportions with Dolphin under matched framing before
further scale changes. Existing uncommitted road-motion work is separate and
still needs its own documentation and emulator acceptance; do not assume this
boot check validates that work.

## Scripted road departure and arrival

Implemented recovered ordinary-road distances/rate (see MECHANICS.md):
0.3-native inside arrival endpoints,0.6-native entry travel at0.025/tick,
1.5-native outside departure endpoint and9-tick pre-fade wait. Queued travel
waits for landing and blocks input/saving. Arrival movement overlaps fade-in;
saving resumes only at the validated endpoint. Failed staging restores the
saved departure position and fades the retained room back in. Successful
transitions no longer use the provisional45-tick cooldown; movement gates them.
Party following threshold is0.5 after the source road homing value.

Validation: `make test` passed core and27 Python tests;
`build-host/core_tests --rooms` passed all8 arrivals,24-tick arrival movement,
all8 failed-load rollback scenarios and18,315 indexed/full-scan probes.
`make -j2 all` built the native binary in the character-size session with these
road changes present. Isolated east-road Azahar script passed two round trips:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py`.
Further native room/fade results are recorded below as completed.

Known limitations: original party choreography/camera are incomplete; scripted
Y uses fixed support height, trigger radius/body dimensions remain provisional.
Next: verify station/harbor native trips, then recover wall attributes and
original collision/sensor dimensions. Original dialogue/story, SFX, layered
materials, doors/interiors and measured battle mechanics remain outstanding.
Full objective remains active; hardware performance remains unverified.

Additional completed checks: dual-screen fade script exited0, with both paper
fill probes ranging0→238.67 and intermediate alpha values. Station and harbor
road scripts each exited0 with two reciprocal trips; station also saved its
final entry. Station's first run failed the second entry while a concurrent
native rebuild changed the embedded RomFS binary; captured UI showed graceful
room-load failure/rollback. Repeated on the finished, unchanged binary passed.
Do not rebuild the mounted binary during emulator acceptance runs.

Final host checks additionally exercise jumping via input before all8 queued
roads: no departure movement, fade or save occurs until landing. Final
`make test`, `build-host/core_tests --rooms`, and `make -j2 all` exited0.
West road script also exited0 with two reciprocal plaza/west trips. Combined
with east, harbor and station scripts, all8 directed outdoor roads now have
native repeated-transition coverage on this implementation. Fixtures used:
artifacts/fixtures/{east,harbor,station,west}-road.bin; command pattern above.

## Preserve original wall collision attributes

Implemented ordinary-Mario wall filtering from original US `marioHitCheck` /
`chkfilterVec`. Converter retains source-node IDs and raw attributes in32-byte
wall records; runtime excludes bit4 sensor walls unless original inclusion masks
apply. Overlapping walls with distinct owners retain separate records. Added
metadata to collision snapshot/rollback and legacy-pack loading with zero flags.
Truncated metadata cannot replace valid collision data. No collision radius,
body height or step-height calibration changed in this step.

Verification completed:
- `make test`: core scenarios and28 Python tests passed, including inclusion
  overrides, rejected sensors, owner deduplication, malformed data and rollback.
- `python3 tools/verify_wall_attributes.py`: all5,264 wall records match original
  node bytes;81 non-solid records across five rooms; no bit31 cases present.
- `python3 tools/verify_ground_attributes.py`: all3,576 floor records match.
- `build-host/core_tests --rooms`:18,315 indexed/exhaustive probes, all8 road
  arrivals, airborne waits and failed-load rollback passed with regenerated packs.
- `make -j2 all`: rebuilt assets and native `.3dsx` successfully.
- Native harbor stairs/dock/return passed on updated binary:
  `python3 tools/emulator_smoke.py --fresh-save --save-fixture
  artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
  tests/azahar_harbor_stairs.py`. Initial test stopped mid-black fade with the
  old fixed wait; now polls completed arrival checkpoint with a6-second timeout.
  The rerun exited0 and preserved HP7.

Next: recover actual normal-Mario collision width/height semantics from
marioHitCheckWidth and wall-ray helpers before replacing provisional .28 radius,
1.2 height and .25 step threshold. Recover original sensor activation and
camera/party choreography. Wall segmentation, transformed-Mario callbacks,
runtime event collision-flag changes, original story/dialogue, SFX and broader
playability remain incomplete. Full objective active; hardware unverified.
Native water-return regression also exited0 using the same fixture and
`--input-script tests/azahar_water.py`: checkpoint HP7→6, original harbor
return destination accepted, partial-recovery save suppressed. Final host,
original-room and unchanged native build checks exited0. Native tests verify
these scenarios; they do not establish complete original physics fidelity.

## Recover normal actor collision clearance and wall rays

Read-only source trace recovered normal width20,height37,half-width10 and
wall-ray heights11/26.64 engine units. Added reproducible source verifier
`tools/verify_actor_collision.py` (constant and instruction-site checks).
Changed normal collision clearance0.28→0.1 native; replaced1.2-high interval
wall blocking with separate0.11/0.2664 ray-height samples. Grid expansion follows
that clearance. Airborne Mario now queries walls using his raised position;
floor height queries remain separate. Nominal height37 was not substituted for
the source wall-ray table. Existing visual scale0.24 remains unchanged.

Validation: `python3 tools/verify_actor_collision.py` exited0; `make test`
passed core and28 Python tests; `build-host/core_tests --rooms` passed18,315
indexed/exhaustive probes and all8 arrivals, landing gates and rollback checks.
Host tests cover radius boundaries, walls between samples, upper-ray-only walls,
low/overhead obstacles, dock-edge jump and original water return owner selection.
Water route now stops at water entry instead of silently advancing recovery
while holding movement; it explicitly jumps over the newly detected dock edge.
`make -j2 all` built native `.3dsx` successfully.

Native water script exited0:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_water.py`.
It jumps across the low edge, preserves partial-recovery checkpoint, returns
to original Ship_Change02 destination and saves HP6 from HP7.

Next: replace wall height interval/segment proxies with actual triangle/ray
geometry and sliding; recover floor search, jump kinematics and ordinary walk
speed. Sensor activation/camera/party choreography, original story/dialogue,
SFX, interiors and broader playability remain incomplete. Full goal active;
no hardware performance or complete original-physics fidelity claim.
Native ordinary harbor descent/dock save/return also exited0 with HP7 using
`tests/azahar_harbor_stairs.py` and the same fixture/launch options. Initial run
caught FileNotFoundError inside the save replacement window; polling now retries
that transient read until the completed room0 checkpoint or the existing bounded
timeout. Final `make test`, original-room checks, source verifier and native
build exited0. An inspection-only `artifacts/reference/marioMove.disasm` was
prepared for next-session walking/floor work; no speed conclusion is established.

## Preserve wall triangles and resolve exact height slices

Implemented portable wall-plane intersection/clearance against retained original
triangle geometry, replacing full-height projected-segment blocking for new
packs. Removes false walls in empty triangular bounding regions. New68-byte
records preserve9 source-transformed coordinates, owner attributes and legacy
summary geometry. Distinct triangles sharing a projected summary no longer
collapse. Index bounds include all vertices; geometry survives room rollback.
Malformed/truncated geometry or inconsistent Y bounds cannot replace valid data.
Older wall formats remain loadable.

Validation: `make test` passed core and28 Python tests; analytical cases cover
sloped cutoffs, height-plane edges, a vertex-only slice, above-triangle clearance,
actual loader queries, rollback and malformed bounds/truncation. Converter fixture
checks original coordinate retention. `build-host/core_tests --rooms` passed all8
arrivals/airborne gates/rollback and18,315 indexed/full-scan probes. Full scan
uses the same triangle-slice primitive (it verifies indexing, not an independent
geometric algorithm); analytical tests separately verify the primitive.
`python3 tools/verify_wall_attributes.py` passed all7,830 triangle records against
original owner flags: gor_01=1920,gor_00=1068,gor_02=2232,gor_03=2277,
gor_04=333;92 non-solid records. This verifies attributes/source IDs, not an
independent original-coordinate extraction. `make -j2 all` rebuilt the RomFS
and native binary successfully. Native harbor water jump/return script exited0
with original return destination and HP7→6; launched with harbor-arrival-road.bin,
--seconds4,--fresh-save and tests/azahar_water.py as in prior entries.

Next: recover swept direction rays, sliding/contact response, coverage of angled
surfaces, floor search and original speed/jump parameters. Near-vertical wall
selection threshold remains0.2, minimap draws summary segments, dynamic hit meshes
and transformed-Mario cases remain incomplete. Original camera/party choreography,
story/dialogue, SFX and broader playability remain outstanding. Full objective
active; hardware performance remains unverified.
Final native harbor stairs/dock-save/return test also exited0 with HP7 on the
finished build: `python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_harbor_stairs.py`. No native rebuild overlapped emulator execution.

## Source-derived ordinary walking and dash distances

Recovered ordinary rates from marioInit,mot_walk,mot_dash and marioMoveMain:
walk1.0 and dash2.25 engine units/tick (native0.01/0.0225). Replaced provisional
0.065 speed. Direction normalizes independently of magnitude; walk/dash selection
uses original55-unit threshold mapped to source70-unit stick cap. Full stick now
travels1.35 native units per second rather than3.9. Road scripted speed remains
0.025 and original distance rules remain unchanged. Follower budget follows
DashSpeed, while partner-specific source movement remains unfinished.

Extended source verifier checks speed floats, initialization stores, motion loads
and55² threshold. Host tests prove threshold boundary,60-tick distance and diagonal
normalization. Preserved stair/water fixture distances by extending directional
holds; .065 remains only as a legacy fixture-distance conversion, never runtime
movement. Incline test now walks far enough at corrected speed to reach its same
height assertions. `make test` passed core/28 Python tests;
`build-host/core_tests --rooms` passed all8 arrivals, airborne gates/rollback and
18,315 collision probes; `make -j2 all` rebuilt native binary successfully;
`python3 tools/verify_actor_collision.py` exited0 with walk1,dash2.25,cap70.
Native water route/jump/return script exited0 with HP7→6 at corrected speed.
Launch: harbor-arrival-road.bin,--fresh-save,--seconds4,tests/azahar_water.py.

Next: recover original walk/dash state transition delays, source jump/gravity and
airborne drift, plus swept wall/sliding response and floor search. Current Circle
Pad dead zone and partner catch-up are provisional. Original camera/party movies,
story/English dialogue, SFX and interiors remain incomplete; full goal active.
Hardware performance and complete original-movement fidelity remain unverified.
Native harbor stairs/dock-save/return exited0 with HP7 using the same fixture and
`tests/azahar_harbor_stairs.py`. Native east-road script passed two round trips:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py`.
This verifies source-trigger movement reaches existing exits; original sensor
activation is still provisional. Final host/original-room checks exited0.
Inspection-only marioJump and mot_jump disassemblies were prepared under
artifacts/reference for next-session recovery; no new jump constants are claimed.

## Source-derived field jump motion and hold behavior

Replaced36-tick1.1-height sine jump with FieldJump derivative integration from
ordinary US ascent/fall tuples, gravity1, counter>=4 A-release reduction,
source apex transition and terminal fall rule. Tap/held jumps now differ;
static-support tests land at21/27 updates after begin. Same integration advances
queued road landing waits; restore/battle/water clear it. Rendering and airborne
wall sampling consume the resulting jumpHeight. Battle harness motion unchanged.

Validation: `make test` passed core and28 Python tests; tap/held four-tick
heights0.17882/0.1946,21/27-update flat-support landing, all8 road landing gates,
original water return and18,315 indexed probes passed through
`build-host/core_tests --rooms`. `make -j2 all` built native `.3dsx`.
`verify_actor_collision.py` extended to verify original jumpData, release
constant and ordinary jump dispatch directly from read-only ISO.

Next: recover original floor and ceiling search using world Y (current jump is
relative to live support), edge falling, airborne horizontal drift, motion-state
transition delays and swept wall sliding. Source-derived constants do not establish
full original physics. Original camera/party choreography, story/English dialogue,
SFX, interiors and broader playable content remain unfinished. Full goal active;
hardware performance remains unverified.
Recovered startup ordering too: mot_jump countdown2/phase0, begin counter1,
next update waits at counter2, first integration counter3. Release reduction
begins at counter4. This corrects intermediate timing estimates without startup.
Final source verifier and host/original-room/build commands exited0.

Final native water route exited0 on this startup-aware build:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_water.py`. Preserved partial-recovery checkpoint, original return
point and HP7→6. Initial test's separate A pulse followed by a0.3-second pause
let the shorter jump land before movement; capture showed Mario still on dock,
HP7. Test now presses A+Down together during the jump. No game timing was
extended to fit the old test. Updated tests and source evidence committed together.

## Recover landing rays and add world-Y floor query primitive

Traced marioFall→marioChkLandon→marioSearchGround. Verified velocity-dependent
center ray: worldY-abs(v)-0.0001 through worldY+abs(v)+0.05 in native units.
Recovered five-probe footprint and ordinary radius0.075, plus source update
order: query before applying fall displacement, then snap accepted floor or add
velocity when no contact exists. Source acceptance also uses slope/attributes;
do not substitute raw intersection success for the entire landing state machine.

Implemented landingGround center primitive through a shared bounded ground query;
legacy groundAt behavior preserved. Tests verify descending support detection,
rejecting a distant roof, exact stationary upper bound, missed-query output
preservation and invalid velocities. Extended read-only source verifier with ray
constants, arithmetic instruction sites and five-probe loop bound. Source
inspection artifacts checkLand/checkLand2/marioFall/landOn now exist under
artifacts/reference for continuation.

Validation: `make test` passed core and28 Python tests;
`build-host/core_tests --rooms` passed all8 arrivals/landing gates/rollback and
18,315 indexed probes; `make -j2 all` built native `.3dsx`.
`python3 tools/verify_actor_collision.py` exited0:5/0.01 engine-unit margins,
7.5-engine-unit normal footprint radius and5 probes verified against ISO.
No new emulator acceptance claim: Game has not switched to this primitive.

Next concrete implementation: preserve Mario world Y independently of support;
implement original five-probe orientation and candidate acceptance, use the ray
before fall displacement, clear lost floor contacts, and test jumping between
unequal floors and walking off edges. Then ceiling search and airborne horizontal
response. Original story/English dialogue, SFX, camera/party movies, interiors
and broader content remain incomplete. Full goal active; no complete physics or
hardware-performance claim. Source recovery and query foundation are progress;
world-height gameplay behavior remains unfinished.

## Integrate world Y, five-point landing and edge falling

Mario now preserves takeoff world height through air movement and can land on a
higher or lower floor instead of shifting the whole jump with support. Game uses
source derivative motion without base-plane clamping, queries from pre-displacement
world Y, snaps accepted contact and clears lost contact IDs. Added oriented center
plus4 footprint rays (radius0.075), generic0.11 snap-distance/highest-candidate
selection. Grounded query loss checks footprint support then begins falling instead
of blocking movement at an unsupported edge. Original remembered return contact is
retained for water recovery. Airborne saves/START writes are suppressed.

Verification: `make test` passed core/28 Python tests. New scenarios prove jump
world height across a1-unit drop, landing below takeoff, jumping onto a0.2-higher
platform only during descent, rotated footprint edge coverage, lost contact IDs,
edge fall and blocked transient saves. Floor acceptance survives horizontal overlap
instead of falling through a valid floor (horizontal resolution remains incomplete).
`build-host/core_tests --rooms` passed all8 arrivals/landing gates/rollback and
18,315 indexed probes. `make -j2 all` built native `.3dsx`; source verifier passed.
The old straight harbor ledge return assertion depended on movement blocking at
the drop: it now checks the lower dock and inability to walk straight up the1.05
height change. The separate stair route still verifies correct return to plaza.

Native water route exited0 with partial-checkpoint protection, original return
point and HP7→6. Native stair/dock-save/return repeat exited0 with HP7. An earlier
stair run was interrupted; its handle was missing and no Azahar/python process
remained, so its terminal result was not inferred from captures. Repeat command:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_harbor_stairs.py`.
Water uses the same fixture/options with tests/azahar_water.py. Final east-road
script exited0 with two round trips, using east-road.bin and tests/azahar_roads.py.
Native code later centralized identical floor constants; final build/host checks
are recorded below, with no behavioral constant change.

Limitations requiring next work: original slope/attribute acceptance branches and
angle mapping remain simplified; grounded support still uses legacy0.25/0.5 band;
no ceilings, no-floor void recovery, or wall push-out/sliding. Unsupported space
without a lower floor can fall indefinitely. Partner stays on legacy stepping;
shadow/camera remain at last support during a drop. Source void recovery and
ceiling search are the next concrete physics tasks before stabilization. Original
story/English dialogue, SFX, camera/party movies, interiors and broader game content
remain incomplete. Full objective active; hardware performance unverified.
Final native lethal-water/reload regression exited0 on the centralized-constant
build: `python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-last-hp-road.bin --seconds 4 --input-script
 tests/azahar_water.py`. Game over preserved checkpoint; reload retained HP1
and original entrance position. Final core/28 Python tests, all original-room
checks and native build exited0. Generated ttyd-2ds.lst is now ignored; user-owned
AGENTS.md edits remain separate from this feature commit.

## Bound void falls with source-derived return and damage

Recovered marioBottomless/mot_bottomless and respawn setter functions. Native
world Y<=-20 now starts recovery instead of falling indefinitely when a validated
return exists. Return chooses valid remembered original recovery binding, then
room/load entry; HP never resets to entry HP. Camera-aware offscreen probe uses
source3-unit start/0.1 increments; horizontal motion0.08/tick; normal fall tuple
with0.1 maximum drop,50 landing ticks,HP-1 and12 injury ticks reuse shared damage
logic. Controls/saves/road requests stay blocked during recovery. Lethal falls use
existing checkpoint-preserving defeat/reload. Companion is grounded/regrouped;
actual recovery height drives renderer cutaways. Framing constants are shared
with renderer but unchanged. Original reload/BG policies and dynamic return-target
selection are not complete, nor are exact camera/partner choreography.

Tests passed: `make test` (core plus28 Python tests),
`build-host/core_tests --rooms` (all8 arrivals,18,315 indexed probes, original water
and new original-plaza void return), `make -j2 all` and
`python3 tools/verify_actor_collision.py`. New host scenarios check inclusive
threshold crossing, nonlethal/lethal returns, ignored input/partial saves, restored
position/partner and defeat reload request. Source verifier reads original-2000,
37,300,10,8 constants and50/12 counter instructions directly from ISO.

Native nonlethal void script exited0: checkpoint protected while falling, HP7→6,
return within0.08 of entry. Reviewed void-return capture; companion hover caused
by copying source lift without NPC gravity was corrected to grounded presentation.
Final lethal script exited0 after render correction: checkpoint preserved on death,
reload at HP1 without healing and original entry position. Commands:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/void-road.bin --seconds 4 --input-script tests/azahar_void.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/void-last-hp-road.bin --seconds 4 --input-script tests/azahar_void.py
```
Fixture is valid original plaza floor at(.35,6.95), deliberately away from active
road approach, moving down beyond floor. A temporary host scan located it; durable
fixture generator and original-room test now reproduce it without the scan.
All extracted/generated originals and captures remain ignored.

Next: source ceiling search and grounded slope/contact rules; optional original
bottomless map reload and exact fallback/partner choreography; horizontal push-out
and sliding; dynamic support. Original story/English dialogue, SFX, camera paths,
interiors and wider playable content remain incomplete. Full objective active;
hardware performance remains unverified. Missing/invalid dynamic return anchors
are not solved by this static slice implementation.
Final source/host/original-room/build checks exited0 after the rendering and
companion correction (last header edit was descriptive only). Commit selected
feature paths only; agent-instruction changes remain in the user's separate
fe63afb commit. No native build overlapped emulator execution.

## 2026-10-12 — Reduce oversized characters again

User reports characters remain very large relative to the map. Reduced shared
PaperCharacterScale from0.24 to0.16 (one-third smaller in both dimensions).
Mario, Goombella, NPCs, Crump, battle actors, tilt and shadows share the change.
This is a visual calibration, not measured original proportions. Map, camera,
collision, movement, jump heights and asset conversion are unchanged.

Validation: `make test` passed core and28 Python tests; `make -j2 all` built
`ttyd-2ds.3dsx`; `python3 tools/emulator_smoke.py --fresh-save --seconds 4`
exited0. Reviewed `artifacts/azahar/boot.png` showing smaller actors against
original plaza geometry and both screens; emulator reports60 FPS. Hardware
performance remains unverified. No new tests for this render constant change.

Next: compare actor/map proportions under matched Dolphin framing, including
door heights and atlas transparent margins. Resume source ceiling search and
grounded contact work. Original story/dialogue, sound-effect events, animation,
camera and broader playable content remain incomplete; remake is not complete.

## 2026-10-12 — Recover ordinary ceiling contact policy

Previous goal turn made verified progress: reduced render scale and committed
2c4f5df. Continued physics work by reading original head search/overhead, filter
and triangle-ray routines. Added portable head attribute/contact policy and
analytical boundary tests (collision override differences, velocity cutoff,
contact rounding, negative-height truncation). Extended original ISO verifier
with100/10 precision,1 clearance,-10 velocity cutoff and original instruction
sites. Corrected an initially mistyped verifier opcode against original bytes;
final verifier passes. Detailed source locations and missing prerequisites are
in docs/MECHANICS.md. Runtime still lacks ceilings: helper is not connected yet.

Validation: `make test` passed core and28 Python tests; `make -j2 all` exited0
(existing native output up to date; new header used only by host tests).
`python3 tools/verify_actor_collision.py` exited0 against read-only original ISO.
No runtime/emulator changes or new emulator acceptance claim this step.

Next concrete task: extend collision pack with source drawmode byte1 and verify
original normal orientation (hitCheckVecFilter, hitEntrySub800186f4,
_hitEnt8001896c). Converter already reads drawmode for rendering but drops it
from collision metadata. Build upward-ray query over appropriate triangles,
five-probe head selection, then integrate post-jump bump/fall with takeoff clamp.
Verify low ceiling, sloped ceiling, one-sided platform underside and original
room jump in host/Azahar. Do not reuse floor attribute filter for head rays.
Grounded slopes, wall sliding, original story/dialogue/SFX/animations and camera
fidelity remain unfinished. Full goal active; no hardware acceptance.

## 2026-10-12 — Integrate source-derived static ceiling collisions

Previous goal turn made progress with source head-policy helpers (e5172f6).
This step adds complete per-room HEAD mesh/index with raw source cull metadata
and source collision strip order. Closer packed/unpacked hitCalcVtxPosition
trace corrected the initial assumption that render/collision winding match:
source swaps the even strip pair in the opposite direction to GX rendering.
Conversion tests and original instruction verifier now lock down this difference.
Runtime performs five rotated upward probes after jump integration, uses original
head-specific attribute filter, source face rejection and verified rounded
contact/clearance, then restarts normal falling at clamped root. New head mesh
loads with room resources, participates in rollback and clears on room release.
Original graphics/scale0.16, roads and existing floor/wall packs are unchanged.

Validation: `make test` passed core and29 Python tests. Host analytical scenarios
exercise low/sloped/one-sided/two-sided ceilings, rotated footprint, rejected
attributes, invalid pack retention, rollback and bump/landing without damage.
`build-host/core_tests --rooms` passed all8 road arrivals,18,315 wall index probes,
original water/void regressions and new original plaza ceiling jump.
`make -j2 all` regenerated locally converted packs and built ttyd-2ds.3dsx; final
repeat up to date. `python3 tools/verify_head_mesh.py` validated12,259 triangles'
source owner/attribute/cull metadata; `python3 tools/verify_actor_collision.py`
passed constants and parity/cross instruction sites. These verify metadata/
instruction evidence, not independent converted positions or Dolphin behavior.

Native boot smoke exited0. New repeated-ceiling script exited0 after5 held jumps,
landing/save at original position/elevation and HP7. Reviewed ceiling-jump
capture; source timing/proportions remain unmeasured. Native east road script
exited0 with two reciprocal trips after adding mandatory head resource loading.
Commands:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --seconds 4
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/ceiling-road.bin --seconds 4 --input-script tests/azahar_ceiling.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
No native compilation overlapped emulator execution (final make was up to date).
All original extracts/packs/captures remain ignored. Head fixture plaza(-6.7,.1)
was located by temporary host scan; durable fixture/test now reproduces it.

Next: recover grounded slope/contact and head-clearance behavior when walking
under low geometry, original takeoff/fall flag variants, wall push-out/sliding
and dynamic collision/block events. Ceiling source SFX342 is not yet mapped.
Compare jumps/camera/proportions in Dolphin; character calibration still0.16.
Original dialogue/story, animation, SFX, interiors and complete content remain
incomplete. Full objective active; hardware performance/acceptance unverified.

## 2026-10-12 — Original behavior documentation and binary evidence

User requested original code behavior documentation suitable for a port or
strongly equivalent reimplementation. Added docs/ORIGINAL_BEHAVIOR.md: evidence
classes, exact US baseline fingerprints, public reference provenance, original
sequence/subsystem ordering, event context/scheduling/continuation and wait
semantics, field behavior index, story contract, subsystem coverage gaps and
an equivalence-validation procedure. This is a partial specification, not full
game recovery. Switch is an explicitly separate future reference; no Switch
code or behavior was analyzed. Accessible private evidence was the supplied
local ISO and existing project artifacts; no private external sources used.

New tools/behavior_evidence.py reads the ISO only as rb and emits metadata to
ignored artifacts/reference/behavior/evidence.json. Identified US G8ME01 disc 0
revision 0, 3,204 files, 29 RELs, 260 US message files. Report fingerprints ten
DOL functions, all 119 event dispatcher targets, the DOL span, gor.rel and US
message files. It also records direct branch-link sites; this is not a complete
call graph or execution trace. Public doldecomp/ttyd MIT source revision
62131fc31866f85a12e4817a8dc37f0c25eb5f8c targets Japan; selected findings were
independently checked in the US binary. Source/ELFs/disassembly stay ignored;
no original code or dialogue was added to source history or native runtime.

Validation completed: make test passed portable core and 29 Python tests;
make all exited 0 with native output up to date; verify_actor_collision.py
passed; git diff --check passed. New report was generated and independently
regenerated identically; checked expected DOL hash and opcode 2/9/10/91 targets.
Temporary altered-symbol fixture proved mismatched file/RAM mapping rejection;
CLI input-overwrite test proved nonzero exit and unchanged input. These latter
checks were session Python assertions, not new persistent unit tests. No new
Dolphin or Azahar execution was performed, and no original gameplay observations
or hardware claims were added. Commit 7d216de records research/tool changes.

Next: close US priority/tie, variable-namespace, pause, child/brother lifecycle
contracts; recover opening harbor/Crump event graph and exact US message bindings;
recover tutorial battle/action-command data and compare controlled Dolphin traces.
Continue field fidelity work described above independently. During this session,
unrelated edits appeared in include/game.hpp, source/game.cpp, tests/core_tests.cpp
and tools/verify_actor_collision.py; they were preserved and excluded from the
research commit. Validation reflects the shared working tree at execution time,
not an isolated checkout of that commit.

## 2026-10-12 — Correct ascending movement/collision order

Previous goal turn made progress with active static ceiling collisionsccf13ff.
Source marioMain tracing found ascent must integrate Mario's jump and head
check BEFORE horizontal movement. Native previously queried walls using prior
tick Y. Split vertical integration from landing completion; ascent wall rays
now use current integrated jump height and landing still selects after XZ.
Normal falling retains move-before-gravity. A new ledge fall begins integration
next tick. Source source-byte verifier locks down both branches.

Host regression: low wall blocks standing Mario, while first rising integration
clears it and advances XZ in the same tick. `make test` passes core and29 Python
tests; `build-host/core_tests --rooms` passes all8 arrivals,18,315 indexed wall
probes and original water/void/ceiling checks. `make -j2 all` built native3dsx;
`python3 tools/verify_actor_collision.py` passed original read-only instruction
checks. Initial regression compilation had a local Input name collision, fixed
with local scope; final checks passed.

Important source distinction recovered: marioChkGnd2 is not ordinary falling
landing. Its .1engine fixed-depth/penetration rule must not replace the velocity
ray from marioChkLandon, which marioFall calls before displacement. Experimental
fixed-depth change failed existing high-to-low landing test and was removed.
Final native falling query retains abs(v)+.01 below/abs(v)+5 above engine units.
Verifier now proves both callers separately; docs/MECHANICS.md records locations.

Final native water route exited0 with checkpoint protection, original return
and HP7→6 after corrected ascent order. Exact command:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py`.
No build overlapped emulator execution. Source extracts/captures remain ignored.

Next concrete task: move falling overhead check before fall gravity/search,
matching marioMain fall branch; recover marioChkLandon floor selection/acceptance
branches and source grounded marioChkGnd. Grounded overhead/legacy stepping
remain unchanged. Exact source flag transitions, slope/face filters, wall
push-out/sliding and dynamic support are unfinished. Original story/dialogue,
SFX, animation, camera, interior/story gates and complete playable content still
require work. Full goal active; hardware performance unverified.
Final native ceiling repeat exited0:5 held jumps, landed save at fixture
position/elevation and HP7. Command: `python3 tools/emulator_smoke.py --fresh-save
--save-fixture artifacts/fixtures/ceiling-road.bin --seconds 4 --input-script
tests/azahar_ceiling.py`. This verifies repeat/save stability, not measured
Dolphin action timing or source camera fidelity.

## 2026-10-12 — Detailed undecompiled-behavior research, first system set

Previous research turn made progress: original behavior baseline and reproducible
binary identity were committed. Current user goal is broader: thoroughly document
behavior not established by decompilation in multiple detailed files, then derive
complete-game system requirements assuming original data reuse. Goal remains
active; this step does not claim all systems documented or create a prematurely
complete requirements document. Native runtime was not changed in this step.

Added docs/behavior/README.md as the full-scope coverage/completion register and
four detailed files: EVENT_RUNTIME.md, BATTLE_INPUT_AND_DEFENCE.md,
STATE_AND_INVENTORY.md and RANDOMNESS.md. Original baseline links to the detail
and updates now-resolved priority/copy-back/RNG findings. Commit ef962d6 records
this system set, independent audit/verifier tools and three tooling tests.

Downloaded pinned MIT public doldecomp/ttyd source/config/REL research snapshot
62131fc31866f85a12e4817a8dc37f0c25eb5f8c into ignored artifacts/reference.
New AST-only coverage audit found 427 configured game-category objects: 421
NonMatching and 6 Matching declarations, 134 with source and 293 without.
These are configuration/source-presence counts, not verified matching functions
or a behavior-completion percentage. The public baseline is Japan; US original
bytes remain authoritative. No private external source or Switch code used.

New concrete US findings: defense checker is only a stub in public source;
original checks difficulty-dependent A/B history windows (default 8/3 entries),
15-entry A/B anti-mash count, eligibility/override branches and input-history
reset on every return. Public history-shift loop disagrees with US instructions.
Source defense caller gates and video-frame alignment remain unknown. Event
priority uses an unstable exchange sort (equal-priority order can reverse);
child creation appends to an active pass, child deletion copies locals back,
owner deletion recursively removes children/attached brothers. Saved-work index0
is a word while ordinary work entries are bytes; resource setters have different
lower-clamp rules; item insertion preserves ordered arrays and can mutate badge
seen flags before capacity failure. rand/irand arithmetic and zero-bound no-draw
behavior recovered. Detailed addresses, limits and derived examples are in docs.

Validation: make test passed portable core and 32 Python tests (three new audit
tests distinguish declaration from source presence, prove upstream config is not
executed, and reject escaping paths/unsupported config). make all exited0 with
native output up to date. verify_behavior_contracts.py passed 56 selected original
instruction anchors and both seven-entry defense tables. Expanded binary report
reproduced exactly with 51 function fingerprints; AST audit report also reproduced
exactly. All local links in baseline/new docs resolved; git diff --check passed.
These static checks do not execute original routines or establish gameplay
equivalence. No new Dolphin/Azahar/hardware validation performed. All downloaded
source and original inspection binaries/disassembly remain ignored.

Reproduce:
```sh
python3 tools/decomp_coverage.py --snapshot artifacts/reference/decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c --output artifacts/reference/behavior/decomp-coverage.json
python3 tools/behavior_evidence.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evidence.json
python3 tools/verify_behavior_contracts.py
make test
make all
```

Next research: trace defense checker callers/override writers and complete
BattleCheckDamage (800fb01c), BattleSetStatusDamage (800fbed8), and battle turn/
action-command lifecycles; recover VM operand namespaces/all opcodes and original
harbor story graph. Then inventory persistent-state writers and original save
records; extend to UI/message interpreter, audio, camera/animation, all partners,
chapters, side content and minigames. Detailed full-scope register lists every
remaining system and requirements-document completion gate. Reused assets do not
remove the need to recover their runtime semantics. Do not mark the full goal
complete based on this first system set or the prototype's green tests.

Shared-workspace note: after validation, unrelated edits appeared in
source/game.cpp, tests/core_tests.cpp, tools/map_convert.py and
tools/verify_ground_attributes.py. They were preserved and excluded from the
research/handoff commits. Test/build results above describe execution-time state;
they do not certify those later edits or an isolated checkout.

## 2026-10-12 — Correct falling overhead phase and takeoff clamp

Previous goal turn made verified progress with ascent wall-query orderc4b2fc6.
This step moves falling head checks before fall derivative integration/search,
matching original move→overhead→marioFall order. A falling bump installs source
normal fall derivatives, then performs their first update in the same tick.
Head ray extension is now separate from actual velocity: falling uses source
10engine extension, preserves actual velocity contact cutoff. Falling bumps
bypass saved takeoff-Y minimum; ascent retains it. Apex-to-fall head query also
uses the non-ascent extension. This fixes missed pre-gravity contacts and upward
teleports when bumping below the takeoff floor.

Validation: `make test` passed core and29 Python tests. New host cases insert
overhead geometry at descending head height and assert clamp followed by-.003
normal fall movement; falling from height1 then bumping below it stays below
takeoff and lands on lower floor. Actual fast-fall cutoff still rejects contact
with explicit positive ray extension. Existing low/sloped/one-sided/two-sided
ceiling, ascent low-wall, lower/higher platform and invalid-pack tests pass.
`build-host/core_tests --rooms` passed all8 road arrivals,18,315 wall-index probes
and original water/void/ceiling checks. `make -j2 all` built ttyd-2ds.3dsx;
`python3 tools/verify_actor_collision.py` passed source move/head/fall calls,
ray-extension load, fall-flag takeoff bypass and fall-derivative instruction
sites. Source was read-only. No extracted originals/packs/captures enter Git.

Native ceiling repeat exited0:5 held jumps with landed position/elevation/HP7
save. Native water script exited0: partial checkpoint protection, original
return and HP7→6. No native build overlapped emulator execution. Commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/ceiling-road.bin --seconds 4 --input-script tests/azahar_ceiling.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
```
These are repeat/recovery checks, not matched Dolphin timing/proportion proof.

Next: source marioChkGnd grounded support/head behavior and marioSearchGround
normal slope/attribute acceptance; replace generic.11 landing footprint snap
and legacy grounded.25/.5 band only with traced branches. Preserve distinction
between predictive marioChkLandon and marioChkGnd2 penetration checks. Floor
face culling, horizontal push-out/sliding, dynamic support and original contact
block/SFX events remain incomplete. Original dialogue/story, music/SFX event
fidelity, animation, camera, interiors and wider content still need work. Full
goal active; hardware acceptance remains unverified.

## 2026-10-12 — Source-derived floor-candidate reach

Previous goal turn made progress correcting falling head phase048ff7a. This
step replaces symmetric.11 landing footprint selection with original branches:
floor>=2engine below root must be reachable by abs(vertical velocity); remaining
near/above candidates use11engine snap. New include/floor_contact.hpp documents
the source policy, and unused FloorSnapDistance constant is removed. Original
source verifier now checks2/11 values and branch sites. No grounded stepping,
geometry export, camera, map warps or render scale changes.

`make test` passed core and32 Python tests (additional research tests appeared
in user worktree). New host boundaries cover2engine inclusive transition, zero/
small fall velocity, reachable18engine terminal step and11engine above contact.
New3.14→0 drop lands on lower floor with HP7 and no void recovery. Existing
original-room checks passed: all8 arrivals,18,315 indexed wall probes, water/
void/ceiling. `python3 tools/verify_actor_collision.py` passed source constants
and branches. `make -j2 all` built native3dsx; final header cleanup build recorded
below. Source ISO/extracted maps read-only; generated packs remain ignored.

Native water regression exited0 with checkpoint protection, original return
point and HP7→6: `python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
tests/azahar_water.py`. Header-only unused constant removal followed this run;
no behavior changed afterward. No build overlapped emulator execution.

Next: recover normal slope/attribute branches before candidate acceptance,
source floor culling, grounded marioChkGnd support/head behavior and replace
legacy.25/.5 stepping. Original distance/angle outputs for rejected candidates
are not consumed yet. Wall sliding/push-out, dynamic support, original story/
dialogue/SFX, camera, animation, interiors and full playable content remain
incomplete. Full goal active; hardware acceptance unverified. Preserve unrelated
user work in tools/behavior_evidence.py and new research tools/tests; only stage
explicit feature paths.
Final header-cleanup `make -j2 all`, `make test` (core+32 Python) and
`build-host/core_tests --rooms` all exited0. Unrelated user research changes
remain outside this incremental feature commit.

## 2026-10-12 — Runtime ordinary slope acceptance

Previous turn made progress with source floor-reach branchescf633ee. Original
marioSearchGround normal slope threshold is50degrees; special states use75.
Added normal slope rejection after nearest solid plane selection. Removed
converter.65 normal-ratio cutoff, which incorrectly removed some valid near50
slopes; preserve all projected nondegenerate planes, including steep blockers.
Legacy ground format/face orientation remain unchanged. Native uses atan2
rather than source angleABf lookup arithmetic; boundary bit-exactness is not
established. Constants/load/comparison/reset instructions are read-only verified.

`make test` passed core and32 Python tests. Added49.7degree accepted import,
50.1degree rejection, vertical/degenerate normal and steep-over-flat masking.
Original-room full-scan oracle now independently applies squared-cosine cone
comparison after nearest-hit selection; initial stale oracle failed as expected
for newly rejected steep planes, updated against the intended rule.
`build-host/core_tests --rooms` passed18,315 indexed wall/floor probe cases,
all8 arrivals and original water/void/ceiling tests. `make -j2 all` regenerated
original-derived packs and built ttyd-2ds.3dsx. `python3 tools/verify_actor_collision.py`
and `python3 tools/verify_ground_attributes.py` passed50/75 source constants/
branch sites and all4,946 floor attributes/source nodes. Full-scan comparison
proves index/query consistency, not original collision fidelity.

Native harbor stairs script exited0: dock traversal/save at original elevation,
HP7 preserved, return to plaza. Exact command:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py`.
No build overlapped emulator execution. Source inputs read-only; all extracts,
generated geometry and captures stay ignored. Unrelated user research work
remains separate. No character-scale/camera/road-policy changes.

Next: add source cull byte and hit_topology winding to floor pack (new record
version with legacy compatibility), reject one-sided downward backfaces before
nearest hit selection and orient two-sided normals. Then recover grounded
marioChkGnd/head rules and replace legacy.25/.5 stepping. Source angleABf lookup
arithmetic, special states, wall push-out/sliding, dynamic supports, original
dialogue/story/SFX/animations/camera/interiors and full content remain incomplete.
Full goal active; hardware acceptance unverified.

## 2026-10-12 — Source floor winding and downward face culling

Previous goal turn made progress with runtime slope filter0b2f479. Floor export
now uses original hit_topology ordering (same as head geometry) and includes
raw source drawmode byte1. New header bits31+30 /48-byte records preserve
attributes/source node/cull. Loader supports legacy36/44-byte packs as two-sided;
invalid cull modes and truncated metadata preserve previous room. Cull travels
with GroundTriangle through snapshots/rollback. Downward source mode1 backfaces
(normalY<=0) are rejected before nearest hit; modes0/2/3 are two-sided. Original
1,867 downward backfaces across five rooms no longer act as candidate floors.
No render/camera/character-scale change.

Validation: `make test` passed core and32 Python tests. New analytical cases
verify culled upper underside allows lower floor, reversed upper winding is
solid, two-sided faces remain solid, invalid/truncated packs retain geometry,
rollback restores cull metadata. A new assertion initially used exact float
equality for interpolated.2 height; changed to numeric tolerance, final tests
passed. `build-host/core_tests --rooms` passed18,315 full-scan/index comparisons,
all8 arrivals and original water/void/ceiling checks. Oracle now reads face
metadata and rejects backfaces before nearest/slope selection.
`make -j2 all` regenerated source-facing floor packs and built ttyd-2ds.3dsx.
`python3 tools/verify_ground_attributes.py` verifies all4,946 records' source
attributes/cull/node bytes, reporting1,867 backfaces. Actor source instruction
verifier passes. These checks do not independently prove transformed source
positions or complete original behavior. ASSETS.md records pack/provenance and
legacy behavior; prior separate research commits remain intact.

Native harbor stairs exited0 (dock traversal/save, HP7, return to plaza). Native
east roads exited0 (two reciprocal trips, arrival saves). Exact commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
No build overlapped emulator execution. All original extracts/generated packs
remain ignored. Physical New2DS performance/acceptance remains unverified.

Next: recover grounded marioChkGnd reach arguments and support acceptance,
including upstream stair/step correction before changing legacy.25/.5 band.
Keep loader/arrival validation distinct from active grounded movement queries.
Grounded overhead still absent; original lookup angle arithmetic/special states,
signed contact normals/animation, wall sliding/push-out, dynamic supports/block
events and SFX remain incomplete. Original story/English dialogue, camera,
animations, interiors and complete playable content still require work. Full
objective remains active; no completion claim.

## 2026-10-12 — Original hit precheck and HP/FP calculation contracts

Research commit 05c59d3 adds docs/behavior/BATTLE_HIT_PRECHECK.md and
BATTLE_DAMAGE_CALCULATION.md. They document US branch order, numeric flags,
signed loads, conditional clamps, callback gates, result codes and explicit
unresolved dependencies. Precheck includes ordered per-copy evasion RNG draws,
accuracy reductions and early-exit side effects. HP/FP contracts distinguish
All or Nothing bypass behavior, absorption, modifier order, linear Double Pain
stacking and Last Stand integer division. Defense documentation now records the
confirmed btlevtcmd_ResultACDefence event entry without claiming attack impact
timing. Public names are annotations; US original bytes establish these local
contracts. No Switch-version equivalence or original execution was measured.

Evidence tooling now fingerprints 55 functions. Static verifier checks 78
instruction anchors, both seven-entry guard tables and binary32 accuracy
multiplier 0.5 at US RAM0x804221c4. tools/behavior_evidence.py rerun was identical
to its saved JSON across two reads. All docs/behavior relative links resolve;
git diff --check passed. make test passed the core test and32 Python tests;
make all exited0 with nothing to rebuild. These are checks of execution-time
shared-workspace state, not isolated checkout validation: concurrent runtime
edits in include/floor_contact.hpp, source/game.cpp, tests/core_tests.cpp and
tools/verify_actor_collision.py were present and were not staged by research.
No emulator launched, profile/save modified, or runtime behavior implemented by
this research step. ISO stayed read-only; extracted ELF/disassembly and reports
remain ignored. Provenance and licensing recorded in ASSETS.md.

Next research: recover BattleCheckCounter (0x800fa678), BattleCheckDamage
(0x800fb01c), BattleDamageDirect (0x800fdfd4), status application and emergency
revival into separate contracts; connect all callers and original weapon data.
Ignored annotated disassemblies for these and the status routines are available
under artifacts/reference/behavior but are not substitutes for reviewed docs.
Trace hit-counter reset lifetimes, battle mode/accuracy-effect producers and
weapon callbacks before claiming a complete combat contract. Add original
execution traces for differential validation. Continue the full-system register
in docs/behavior/README.md: story/event services, partners, audience/rewards,
save records, UI, animation/audio and complete content remain incomplete.
The complete-game requirements document remains gated on that recovery; full
research objective is active and has not been achieved by these local contracts.

## 2026-10-12 — Counter selection and revival eligibility research

Previous research turn was progress:05c59d3/20ac72e added hit/damage contracts.
This turn's4568b28 adds detailed BATTLE_COUNTERS.md:48-byte initialization,
all exclusive/accumulating selector branches, resistance/badge/status gates,
last-writer classification priority, exact12-entry return mapping and consumer
half-damage/status/direct-damage/event order. Binary code48 maps to return0
while retaining a populated counter record; the main caller uses work fields.
Separately rounded half contributions and the equality-only zero→one fallback
are explicit. Damage documentation adds complete local revival code-marking
helper:inventory vs unit item paths, inclusive kind ranges and unchanged failure
codes. Actual item consumption/HP restoration remains unrecovered.

Validation: read-only US ISO verifier passed95 instruction anchors, both guard
window tables, accuracy constant and all12 counter jump-table targets.
Evidence report fingerprints58 functions and repeated identically across two
reads. Relative links in docs/behavior resolve; git diff --check passed.
make test passed core plus32 Python tests; make all exited0, nothing to rebuild.
Checks used shared workspace at execution time, with concurrent runtime edits
in include/floor_contact.hpp, include/game.hpp, source/game.cpp,
tests/core_tests.cpp, tools/verify_actor_collision.py and untracked
include/field_step.hpp. Research did not stage/change those files. No emulator
or original executable was run, and no profile/save changed. Source ISO opened
read-only; original extracts/disassemblies remain ignored; ASSETS.md updated.
Static anchor checks validate cited bytes, not full dynamic equivalence.

Next: full BattleCheckDamage orchestration, BattleDamageDirect application,
BattleSetStatusDamage and supporting status getters/setters/lifecycle; original
attack callbacks and data bindings; death/revival event and item handling.
Counter presentation still needs exact sound/effect bindings. Capture original
execution for differential tests when contracts and a useful harness permit.
Continue all remaining systems in docs/behavior/README.md before deriving the
complete-game requirements document. Full objective remains active/incomplete;
these local combat contracts do not establish complete battle or full-game
behavior, story fidelity or New2DS performance acceptance.

## 2026-10-12 — Grounded height limits and three-tick upstairs motion

Previous goal turn progressed source floor facesf268eaf. Recovered original
front-step query/caller and mot_upstairs motion12. Active movement now blocks
rise>.11native; drops>=.02 fall once footprint support ends. Qualifying.05.. .11
rises on surface<=30degrees interpolate root over3 ticks instead of instant
snap. New FieldStep continues horizontal collision at interpolated Y and checks
overhead; transient saves/warps/jump/interaction guarded, lifecycle resets clear
state. Original query width*.265 forward distance, rise5..11/angle30 and three
counter/equal-displacement instructions are source verified.

Scope limit: current stair trigger samples candidate floor rather than original
forward point. Full grounded ray reach, quantization and XZ/motion flags are
not integrated. Room/load groundAt keeps legacy broad band; partner does too.
The main player height acceptance now bounds that candidate before applying it.
Original stair pose/shadow/camera and ceiling cancellation/floor reacquisition
need matched verification. This is further playable physics progress, not final
source/Dolphin fidelity.

Validation: `make test` passes core and32 Python tests; added exact3-step helper
and Game interpolation, partial-save/jump guards, blocking.2 ledge and.08 drop
fall/landing. `build-host/core_tests --rooms` passes all8 arrivals,18,315 indexed
wall/floor probes and original water/void/ceiling checks. `make -j2 all` builds
native3dsx; `python3 tools/verify_actor_collision.py` passes original step slope/
distance, motion-table target,3-count and velocity/division instructions.

Native harbor stair traversal/dock-save/return exited0 with HP7; native water
route exited0 with partial-checkpoint protection, original return and HP7→6.
Commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
```
No build overlapped emulator execution. Original binary inspections/packs stay
ignored; separate untracked research notes are left untouched. Architecture and
mechanics notes describe state and adaptations. Hardware performance unverified.

Next concrete task: implement original forward step ray at width*.265,
root+half-height with downward reach half-height+11+.1engine, plus source filter
and caller step selection; replace candidate trigger. Then main grounded
marioChkGnd five-probe ray/height quantization and contact outputs, keeping
entry validation separate. Source move-state XZ rates during stairs and low
ceiling cancellation need review. Grounded overhead, wall sliding/push-out,
dynamic support, original story/English dialogue/SFX/camera/animations/interiors
and complete playable content remain incomplete. Full objective stays active.

## 2026-10-12 — Direct damage application and shared FP research

Previous goal turn progressed via4568b28/0ab7bdb counter contracts. Research
commit41f6e27 now adds BATTLE_DAMAGE_APPLICATION.md: full local ordering of
BattleDamageDirect, ±99 HP clamp, signed-byte amount accounting, conditional HP
mutation, original/unbounded FP argument handling, shared partner FP owner,
Pity Flower before FP subtraction, Sleep recovery request RNG, exact effect
parameters/sound priority, action-record calls and lethal-event flag selection.
Five supporting resource/owner helpers were inspected from original US bytes.
Death/removal, revival consumption and HP restoration are explicitly left with
unrecovered hit-event handlers rather than inferred from the marking flags.
Coverage links and ASSETS.md provenance were updated. No runtime implementation
was changed by this research.

Validation: static ISO checks passed119 instruction anchors, both guard tables,
accuracy constant,12 counter jump targets,9 damage-effect constants and7 sound
keys. Evidence report now fingerprints63 functions and matched a repeat read.
All behavior relative links resolved; git diff --check passed. make test passed
core plus32 Python tests; make all exited0 with nothing to rebuild. Checks were
against execution-time shared workspace, with unrelated runtime edits remaining
in include/floor_contact.hpp, include/game.hpp, source/game.cpp,
tests/core_tests.cpp, tools/verify_actor_collision.py and untracked
include/field_step.hpp. Those files were not staged. ISO stayed read-only;
extracted helper ELFs/disassembly and reports are ignored. No original binary or
emulator was run, and no user profile/save was touched. Static anchors verify
selected bytes, not complete dynamic equivalence or tested gameplay timing.

Next: recover BattleCheckDamage orchestration and weapon/event callers;
BattleSetStatusDamage, status getter/setter/recovery/turn lifecycle; actual hit
event dispatch and death/revival event bodies. Trace unit/part/data lifetimes,
attack callback catalog, action-record and audience helpers, complete effect and
sound-macro execution. Add original execution traces and differential tests.
The full-system register remains open for story/events, partners, field forms,
save/UI, assets/animation/audio and complete content. Derive the complete-game
requirements only after those contracts are documented. Full objective remains
active; no complete-game or complete-battle equivalence claim.

## 2026-10-12 — Battle reaction event dispatch research

Previous goal turn progressed with41f6e27/8de16ad direct damage contracts.
Commit b897a67 adds BATTLE_EVENT_DISPATCH.md: normal/direct hit dispatch,
low-code40 data-key46 gate, eight-byte terminated data lookup, ordered four-slot
cancellation, null-script distinctions, wait/phase event differences, same-pass
scheduler insertion and raw-vs-resolved service operands. Full stored damage
code, low-byte getter and any-bit mask test are distinguished. Public header
US offset mismatch is explicitly recorded. These contracts connect damage
application to scheduling without claiming death/revival script bodies are
recovered. No runtime code changed by this research.

Validation:141 static instruction anchors and previously documented tables/
constants/sound keys passed against read-only US ISO. Evidence report contains
74 function fingerprints and reproduced identically across two reads. All
behavior relative links resolve; git diff --check passed. make test passed
core plus36 Python tests at execution time; make all exited0, up to date.
Concurrent runtime and Switch-research changes were present and not staged:
include/floor_contact.hpp, include/game.hpp, source/game.cpp,
tests/core_tests.cpp, tools/verify_actor_collision.py, and new Switch tools/tests.
Other concurrent docs/ARCHITECTURE.md and docs/MECHANICS.md edits were also left
unstaged. Thus validation describes shared workspace, not an isolated commit.
No emulator/original code executed; profile/saves unchanged. Original extracts
and disassembly remain ignored. Provenance/fingerprint counts updated in ASSETS.

Next: recover complete BattleCheckDamage orchestration and status application/
recovery/turn lifecycle. Enumerate actual damage-script installations, relocated
script/data bindings and reachable death/revival bodies; inspect turn-controller
wait/ID ownership before claiming event lifetime equivalence. Constructor
capacity handling and remaining event services stay open. Continue full-system
coverage register (story, field/forms, partners, economy, save/UI, animation/
audio and all content) and derive complete-game requirements after recovery.
Full objective remains active; documentation and end-to-end equivalence are
incomplete, with static evidence not replacing original execution traces.

## 2026-10-12 — Activate original forward step ray

Previous goal turn progressed bounded grounded motion/FieldStep7857a24.
Replaced candidate-floor step activation with forward query after horizontal
movement: normalized direction*.053native, origin root+.185, bottom root-.111.
This corresponds to source baseWidth20*.265 and halfHeight18.5 downward reach
18.5+11+.1engine. Source attribute/face/nearest selection is reused; caller
requires5..11engine rise and<=30degree surface. Direct larger candidate rises
remain blocked until the forward ray starts the3-tick motion. Probe can now
anticipate a stair while actor center remains on lower floor. Existing loader
and room-arrival ground validation remain separate.

Validation: `make test` passed core and32 Python tests initially,36 in final
repeat after separate research inventory tests appeared. New host tests cover
forward5.3engine crossing threshold, reverse direction, zero-direction output
retention, rejecting a surface above18.5engine origin, rise/angle caller bounds
and early activation fromlowerx.07 before crossingupperx<=0. Existing3-tick
state/save/jump guards and tall ledge/drop tests pass.
`build-host/core_tests --rooms` passes all8 arrivals,18,315 index/full-scan
probes, original water/void/ceiling checks. `make -j2 all` builds native3dsx.
`python3 tools/verify_actor_collision.py` passes half-height/width multiplier,
ray-origin/length instruction sites and source constants/caller/motion proof.
No extracted originals/generated packs are committed.

Native harbor stairs/dock-save/return exited0 with HP7; native east roads
(two reciprocal trips/arrival saves) exited0. Exact commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
No build overlapped emulator execution. Separate Switch/package research tools
and tests remain outside this feature commit. Architecture/mechanics notes
record ordinary caller behavior and limitations. No hardware/Dolphin-fidelity
acceptance claim from these checks.

Next concrete task: replace main grounded center-first support with original
five rotated probes at7.5engine radius, source grounded ray reach and selected
floor quantization. Preserve separate predictive falling/forward ray/arrival
queries. Complete marioChkFrontStep drop-distance/angle outputs and source
movement-angle rounding, staircase XZ rate/state chaining and low-ceiling
cancellation under actual source framing. Wall push-out/sliding, dynamic
support, grounded overhead, signed normal animation outputs, original story/
English dialogue/SFX/animations/camera/interiors and full playable content remain
unfinished. Full goal active; hardware performance unverified.

## 2026-10-12 — Status storage and explicit recovery-step research

Previous turn progressed with b897a67/a6a19cd event dispatch contracts. This
turn adds BATTLE_STATUS_STORAGE.md: complete local getter/setter cases for
IDs0–27, shared signed fields, badge-only Electric presence, Burn/Freeze and
Defense cancellation, size/attack replacement, Charge narrowing-before-clamp,
Fast/Slow move-count gate, all recovery-step groups and HP/Instant Kill gates.
Direct-damage Sleep recovery now links to its verified local clearing outcome.
Recovery is specified as an explicit helper call, not assumed once per turn.
Infliction rates/immunity and complete turn scheduling remain unrecovered.

Validation:153 instruction anchors and all81 status dispatch targets passed
against read-only US ISO, alongside previous tables/constants/sound keys.
Evidence now fingerprints79 functions and reproduced identically across reads.
Behavior relative links resolve; git diff --check passed. make test passed
core plus36 Python tests; make all exited0, nothing to rebuild. This validates
execution-time shared workspace, not an isolated commit: unrelated runtime,
architecture/mechanics documentation and Switch inventory/tools/tests edits
were present and left unstaged. No native runtime behavior changed in this
research. Original helper ELF/disassembly/report files stay ignored; no
original executable/emulator run or profile/save change. ASSETS.md updated.

Next: BattleSetStatusDamage and weapon status parameter/rate helpers, complete
BattleCheckDamage orchestration, recovery-call schedule and periodic status
HP/FP/action effects. Recover actual unit death/revival event graphs and item
consumption. _CheckMoveCount's downstream BtlUnit_ResetMoveStatus remains open.
Continue full-system register beyond combat and derive complete-game system
requirements only once those original contracts are adequately documented.
Full objective remains active/incomplete; static checks are not original
execution tests or end-to-end fidelity proof.

## 2026-10-12 — Weapon status parameters and infliction admission

Previous turn progressed via5874375/cc82bc9 status storage/recovery contracts.
This turn adds BATTLE_STATUS_INFLICTION.md: complete weapon wrapper iteration,
base duration/strength/rate fields, equipped-badge parameter changes, original
weapon-pointer Charge cases, ordered removal/immunity gates, susceptibility
lookup, exact chance arithmetic/RNG draw and mutation/result-bit prefix.
Application presentation/announcement/return tail from0x800fc198 onward remains
explicitly outside detailed coverage. No complete status-system claim.

Validation:165 instruction anchors plus previous tables/constants/sound keys
passed; eight additional inspected infliction table fingerprints cover201
entries. Evidence report fingerprints84 functions and matched a repeat read.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests. make all compiled game.cpp, linked and built ttyd-2ds.3dsx,
exit0 (session77701 completed). Validation was execution-time shared workspace:
concurrent floor/game/core-test/actor-verifier edits and new Switch research
tools/tests were present and not staged. No runtime changes authored by this
research. ISO opened read-only; original extracts/disassemblies/reports remain
ignored. No original code/emulator executed or user profile/save modified.
ASSETS.md records updated evidence scope. Fingerprints identify inspected data;
they do not prove semantic interpretation or dynamic equivalence.

Next: finish BattleSetStatusDamage effect/cleanup/announcement/sound and return
contracts, then complete BattleCheckDamage orchestration and all status turn
callers. Resolve weapon identities/data bindings, equipment-count helper,
periodic effects and actual death/revival event graphs. Continue other systems
in behavior/README before deriving complete-game requirements. Full objective
remains active and incomplete; original execution traces and end-to-end fidelity
checks are still required.

## 2026-10-12 — Status application presentation and completion

Commit0eb2aa1 completes local BattleSetStatusDamage control-flow documentation
with BATTLE_STATUS_PRESENTATION.md. It specifies position corrections, all stamp
cases and payload bytes, up/down effects, cancellation cleanup, signed result
announcement gates, unchanged Charge mode, sound bindings and final return.
Whole-call return1 after admission/chance is distinct from setter success;
opposite-status cancellation can return1 without an active requested status.
Infliction contract and behavior index now link this tail. Effect rendering,
announcement queue internals and lifecycle scheduling are still unrecovered.

Validation:177 instruction anchors, ten infliction/presentation table hashes
(254 entries), five status position constants and all prior checks passed
against the read-only US ISO. Evidence report identifies84 functions and the
unchanged US DOL digest. Behavior links resolve and git diff --check passed.
make test passed core plus36 Python tests. make all exited0, nothing to rebuild.
Checks used the shared workspace with concurrent runtime, architecture/mechanics,
and Switch research changes present; those edits were not staged. No runtime
change authored, original executable/emulator run, or profile/save modification.
Extracts/disassembly/report remain ignored; ASSETS.md records evidence scope.
Static checks do not establish dynamic or visual equivalence.

Next: complete BattleCheckDamage orchestration, status turn callers and periodic
HP/FP/action effects; recover announcement/effect internals, weapon bindings,
equipment-count semantics and actual death/revival event graphs. Continue the
full-system behavior register before deriving complete-game requirements. The
full objective remains active and incomplete; original execution traces and
end-to-end fidelity comparisons are still required.

## 2026-10-12 — Main five-probe grounded support

Previous turn progressed original forward step trigger03dc212. Added normal
groundedFootprint: five rotated7.5engine probes, depth11+.01engine, above5
for ordinary walking/standing velocity0. Source nearest face/slope/candidate
acceptance precedes selecting highest result across all probes. Game now
updates support once after full horizontal movement, instead of center-first
acceptance/fallback; forward stair ray observes old root before support update.
Missing support starts falling. Broad groundAt remains for entry/partner and
coarse unwalled tall-ledge guard; it no longer snaps main support via.25/.5 band.
Removed superseded groundedHeightCanSnap policy and retained source2/11 bounds
where consumed.

Important source trace: rounded floor height is actor+0x1c4, while actualY
+0x90 receives raw search output at8008fae8. No direct root quantization added.
A separate original floor cache is still unrepresented. Byte verifier checks
grounded11/current-v arguments, rounded cache store and raw actualY store.

Validation: `make test` passed core and36 Python tests; added highest-side
support despite lower center, rotated footprint changing selection, zero-vector
output retention,4.9engine accepted/5.1engine rejected upward probe reach and
Game edge-height/contact update. Existing stair anticipation/interpolation,
blocked tall ledge, fall landing and partial-save guards pass.
`build-host/core_tests --rooms` passes all8 arrivals,18,315 indexed/full-scan
probes and original water/void/ceiling checks. `make -j2 all` builds native3dsx;
`python3 tools/verify_actor_collision.py` passes source arguments/store evidence.

Native harbor traversal/dock-save/return exited0 with HP7; native water route
exited0 with checkpoint protection, original return and HP7→6. Commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
```
No build overlapped emulator execution. Extracted originals/compiled artifacts
stay ignored. Separate research files remain outside this feature commit.
Architecture/mechanics notes record source cache distinction and scope.

Next: grounded head/clearance after movement and exact source contact/cache/
quantization consumers. Review stair state chaining/XZ rate, nonzero residual
velocity and source direction rounding; source wall push-out should replace
the coarse tall-ledge guard. Partner still needs original NPC grounding.
Dynamic support, block/SFX events, original English story/dialogue, animation,
camera, interiors and complete playable content remain unfinished. Full goal
active; hardware/Dolphin fidelity acceptance unverified.

## 2026-10-12 — Damage orchestration research validation

The preceding goal turn made progress with5813bb3, then was interrupted before
this handoff. BATTLE_DAMAGE_ORCHESTRATION.md specifies all local BattleCheckDamage
branches, superguard variants, guard eligibility, status/damage ordering,
code rewrites, audience calls, reaction selection, result precedence and
Pikkyoro selection. Two retained RNG draws must not be optimized away: the
Freeze event comparison against zero and pattern selection after immunity.

Completed validation before interruption:203 instruction anchors and previous
data checks passed, plus orchestration presentation bindings/zero initializer.
Evidence report fingerprints87 functions and matched a second read byte for
byte. make test passed core plus36 Python tests; make all exited0, no rebuild.
Behavior links resolved; git diff --check passed. Concurrent runtime work was
committed separately asb182246; this research did not author or stage it.
Original extracts/disassembly remain ignored. No original execution, emulator,
profile/save modification or dynamic-equivalence claim in this research.

Next research: status recovery scheduling and periodic effects, event waits,
actual death/revival scripts, weapon bindings and full-system coverage. Complete
game requirements remain deferred until the systems are adequately documented.

## 2026-10-12 — Turn status recovery passes and wait boundary

Added BATTLE_TURN_RECOVERY.md:23 ordered recovery phases,64-slot iteration,
generic recovery event registration/sound, allocation failure handling, Slow
move reset, Freeze expiry damage/effect and event-wait transitions. Changed
Sleep exits before polling its wait state; other changed recovery phases recurse
into the wait. Empty phases skip waits recursively. Documented outer active-event
wait protocol; its initial unit/background helper still needs recovery.

Read-only DOL scan found24 direct branches to CheckRecoveryStatus:23 in turn
controller and one Sleep-interruption call in direct damage. Report explicitly
excludes indirect calls, REL relocations and script operands. Static validation
passed212 instruction anchors plus23 recovery calls,78-entry dispatch-table hash,
recovery sounds/scale and previous checks. Evidence92 function fingerprints
matched a repeat report byte for byte. Links and git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild. A final
Sleep transition anchor was added afterward and the full static verifier rerun.
No native gameplay code authored, original execution/emulator run, or profile/
save changes. Concurrent Switch tools were committed separately asdfc276e.
Source extracts/disassemblies/reports remain ignored; ASSETS.md updated.

Next: periodic Poison/Burn/HP/FP phases before recovery, generic recovery event,
ResetMoveStatus, CheckShadowGuard, initial unit wait helper and conclusion
handling. Recover outer turn/phase cadence and actual death/revival event bodies.
Continue all other systems in behavior/README before complete-game requirements.
Full objective active/incomplete; static fingerprints do not prove original/native
runtime or rendering equivalence.

## 2026-10-12 — Periodic Poison/Burn requests and delayed Burn effects

Previous goal turn progressed witheff4d81 recovery scheduling. Added
BATTLE_PERIODIC_DAMAGE.md for turn states06..09: Poison slot scan and signed
strength damage, Burn presence/registration/polling/substate waits, complete
local Burn effect state machine and shadow-guard predicate. Burn does not
recheck HP/status at final damage; protection is checked after deleting the
effect. Fresh registration requires62 helper invocations absent external changes,
not a measured62-frame claim. HP/FP regeneration remains next recovery work.

Validation:222 instruction anchors and four added Burn binary32 constants plus
all prior checks passed against read-only US ISO. Evidence report95 function
fingerprints reproduced byte-identically. Behavior links resolve; git diff
--check passed. make test passed core plus36 Python tests; make all exited0,
nothing to rebuild. No runtime code changes, original execution/emulator run,
or profile/save modifications. Concurrent Switch docs/tools were left unstaged.
Original extracts/disassemblies/reports remain ignored; ASSETS.md updated.
Checks establish static identity/anchors, not executed-original equivalence.

Next: periodic HP/FP regeneration, sinfd numeric contract and effect internals,
recovery event body, move reset, unit-event wait/conclusion helpers, outer
turn/phase cadence and actual death/revival scripts. Continue full-system
register before deriving complete-game requirements. Objective remains active
and incomplete; original/native trace and rendering comparisons still needed.

## 2026-10-12 — Switch 1.0.0 baseline and static mechanics research

User changed this research goal's reference from GameCube to the supplied
Switch NSP, then instructed **static analysis only for now**. Honor that limit:
no further emulator launch/capture or firmware work. Native prototype development
and GameCube contracts remain separate; do not relabel their constants as Switch
findings. New entry point is docs/switch/README.md, linked from README and the
GameCube behavior index.

Confirmed package SHA25604fba23ab27de88276d2a04eaf69619cd09044acb41de62c1cb2f89a8de110e4,
application0100ecd018ebe000, CNMT version0, NACP1.0.0 and main NSO build
78f37bb55d015be3b368ec22af595455f1544dc1000000000000000000000000.
PFS0 has6 entries/5,456,787,792bytes. Decrypted using user-supplied local keys;
title.keys lacked this game's rights ID, resolved privately from the package's
common ticket. Original package/key files unchanged. hactool archive signature,
PFS0 and RomFS hash checks report GOOD; all3 CNMT content hashes/sizes match the
PFS0 inventory. First unmatched-key attempt reported corrupted sections and is
explicitly superseded, not misreported as success.

Extracted9,959 RomFS files, including1,246 script files,1,582 compressed data
ELFs and47 msg/US_English_Final resources. Added docs/switch/BINARY_BASELINE.md,
BATTLE_PARAMETERS.md, FIELD_PARAMETERS.md and VERSION_DIFFERENCES.md. Documents
separate binary values, reviewed native contracts, publisher descriptions and
unknowns. Nintendo1.0.1 fixes are not treated as behavior of the supplied1.0.0.
No story dialogue copied into source or invented.

Recovered661 ordered rows:360 battle parameters,18 player states,30 jumps,
197 Mario and56 partner action commands. Preserve26 null battle IDs and repeated
names. Row layout/pointer relocation verification does not imply runtime semantic
completion. Concrete native main queries779ff0/77a180 select seven defense/counter
values using clamp(3+unit168c-unit1688,0,6), scan64 slots for the first object
matching words68==4/6c==0, and return float0 if none. Other context cleanup side
effects and invalid-state assumptions documented. Equal modifiers select16 and3;
neither is claimed measured rendered frames. Partial update7225c4 loads these
rows and subtracts a runtime binary32 decrement from timers, disproving a basis
for blindly assuming decrement1. Modifier writers, timer clock and full input/
impact/reset gates remain unrecovered.

Tools switch_package_inventory.py/switch_table_evidence.py and4 PFS0 tests were
already present in commit dfc276e after the interrupted first part of this work.
Added switch_nso_evidence.py to validate the reviewed build,11 instruction anchors,
3 code-range hashes and generate the inspection ELF. Public tool snapshots pinned:
hactool1d64a83450e025622f3468c28fc4164dad2c5ef6 (ISC),
Unsimplifiere2bf56c22ae527560602a9b063bccb1bf03dac16 (Unlicense).
Assets/provenance recorded in ASSETS.md. All extracts/decompressed originals,
reports, source snapshots and disassembly remain ignored under local-assets and
artifacts; native runtime unchanged by this research.

Validation: make test passed the portable core and36 Python tests; make all
exited0/up to date. Table report regenerated identically with cmp. Native report
regenerated identically with cmp;11 anchors and3 range fingerprints pass.
git diff --check passed. These validate tooling and the current shared worktree,
not gameplay equivalence, complete mechanics, an isolated checkout or hardware.

Before static-only steering, isolated Ryujinx1.3.3 showed RYU-0002 firmware missing
and later RYU-0001 keys-not-found during installer attempts. No game execution was
obtained. User supplied Firmware.23.0.1.zip but successful installation was not
established. Normal user profile/saves unchanged. No emulator launched after the
static-only correction; a scoped cleanup check found no remaining research
Ryujinx process. Do not keep troubleshooting firmware under this instruction.

Next static work:
1. Trace main7225c4's timer decrement producer (+4a78 via pointer slotf0c0f0),
   input production and full impact-check candidate723bd8. Recover modifier1688/
   168c writers and script registration names; then document bounded defense
   algorithm instead of assigning frame/second durations prematurely.
2. Trace player/jump table consumers to recover units, clock, integrator, release,
   mode gates and collision order. Do not substitute GameCube motion equations.
3. Obtain/recover a compatible Switch script-bytecode decoder and native API
   registry; inventory story/service calls. Ascend is a discovery lead, not an
   installed/validated decoder. Preserve MSBT labels, tokens, order and speakers;
   Switch English path is msg/US_English_Final, not GameCube msg/US.
4. Continue damage/status/turns, audience/RNG, inventory/save/UI/audio/graphics,
   partners, all chapters/interludes and optional content. Current register lists
   gaps; the full documentation goal is active and not complete.

## 2026-10-12 — HP/FP regeneration and Happy badge phases

Previous goal turn progressed with36d17dc Poison/Burn contracts. Added
BATTLE_REGENERATION.md covering states0a..11: status23/24 recovery, HP display
bookkeeping attribute gate, shared FP destination, requested-amount effects,
regeneration wait event, Happy Heart/Flower per-copy draws, integer-divide-first
thresholds, local Flower probability inputs versus resolved FP owner, signed16
narrowing/capping and changed-phase wait differences. Recovered RecoverHp helper.
Happy Heart defers its wait; Flower/status regeneration recurse immediately.

Validation:236 instruction anchors, regeneration event fingerprint and prior
data checks passed against read-only US ISO. Evidence96 function fingerprints
matched a repeat byte for byte. Behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild. Final
verifier rerun passed after representing the tiny event as a hash rather than
embedding its complete bytecode. No original executable/emulator run or profile/
save changes; no native runtime code authored. Concurrent Switch baseline was
committed separately asfc91c72. Original extracts/disassembly remain ignored;
ASSETS.md updated. Static evidence is not dynamic-equivalence proof.

Next: Happy/recovery event bodies and associated sound/pose callbacks, display
field consumers, remaining turn initialization/counters, unit-event waits and
conclusion/phase handling, move reset and actual death/revival scripts. Continue
full-system coverage before complete-game requirements. Full objective remains
active/incomplete; original/native execution and rendering comparisons needed.

## 2026-10-12 — Recovery/Happy event bodies and service operands

Previous turn progressed with9977046 regeneration phases. Added
BATTLE_RECOVERY_EVENTS.md: all local branches/calls in status recovery and
Happy Heart/Flower scripts, data-key8 override/null-value behavior, fallback
wait cancellation/pose/jump/start ordering, Happy pose support/status gates and
60-count wait. Recovered13 additional wrapper/helper contracts, including
special unit-ID translation, raw signed-byte status operand, pose-table support
scan and data/child dispatch. Fresh entry local0 differs from bound owner: LW10
and LW3 queries are preserved, not rewritten to self. Encoded fall acceleration
is retained exactly; public fixed-point interpretation is explicitly R evidence.
Motion, float coercion, actual pose/sound and key8 bindings remain open.

Validation:250 instruction anchors plus three script-span hashes and prior
data checks passed against read-only US ISO. Report109 function fingerprints
matched a repeat byte for byte. Behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, nothing to rebuild.
No runtime changes, original execution/emulator run, or profile/save changes.
Only research/provenance/tools edited; original scripts/disassembly remain
ignored. ASSETS.md records scope. Hashes prove byte identity, not interpretation
or dynamic equivalence of a replacement.

Next: JumpPosition and float resolution, pose/sound implementations, data-key8
bindings; move reset and remaining unit-event waits/conclusion handling; outer
turn/phase/action cadence and actual death/revival event scripts. Continue
full-system coverage before complete-game requirements. Full objective remains
active/incomplete; original/native execution and rendering comparisons required.

## 2026-10-12 — Unit-event barriers, reaction queue and conclusion predicate

Previous turn progressed with9317c39 recovery event scripts. Added
BATTLE_WAITS_AND_CONCLUSION.md: full unit-handle scan/cleanup, outer barrier,
64-pair reaction initialization/insertion/all-match compaction/dispatch,
missing-unit recursion, allocation failure readiness, four reservation slots
and consumption before conclusion, three-group configuration scans/result
writes and final forced-flag predicate. Duplicate unit insertion ignores a
new key; unit barrier visits all handles despite an earlier live event.
Queue/configuration/reservation producers and victory/defeat execution remain
open. Turn recovery's obsolete helper boundaries now link the new contract.

Validation:266 instruction anchors and prior table/constant/script checks passed
against read-only US ISO. Evidence117 function fingerprints reproduced exactly.
Behavior links resolve and git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research
files edited. No runtime code authored, original executable/emulator run, or
profile/save modification. Extracts/disassembly/reports remain ignored; ASSETS.md
updated. Static anchors/fingerprints do not prove dynamic equivalence.

Next: queue/configuration/reservation writers and pouchRemoveItem semantics,
conclusion sequence/rewards, move reset and outer turn/phase/action cadence;
JumpPosition, float/pose services and actual death/revival scripts. Continue all
systems before deriving complete-game requirements. Full objective remains
active/incomplete; original/native traces and rendering comparisons required.

## 2026-10-12 — Inventory removal, reservation insertion and refunds

Previous turn progressed with729928b barriers/reaction queue. Added
INVENTORY_REMOVAL_AND_RESERVATIONS.md: complete local pouchRemoveItem and indexed
variant, array/capacity selection, first-match ordered compaction with badge
parallel array, unchecked index read order, reservation callback kind/item gates,
slotwise duplicate/empty checks, independent binary32 refund path and held-item
151 cleanup. Reservation removal now links exact inventory semantics. The
reservation writer was located through DOL immediate-operand scan; that scan
is a candidate locator, not a complete writer/caller census.

Validation:278 instruction anchors and two refund constants plus prior checks
passed against read-only US ISO. Evidence120 function fingerprints reproduced
byte-identically. Behavior links resolve and git diff --check passed. make test
passed core plus36 Python tests; make all exited0, no rebuild. Workspace started
clean; no runtime code authored, original executable/emulator run or profile/
save changes. Extracts/disassembly/reports remain ignored; ASSETS.md updated.
Static checks do not establish dynamic-equivalence or legal-call reachability
of indexed boundary cases.

Next: all reservation/consumption callers, item execution/cancellation and badge
parallel-array consumers; queue/configuration producers, conclusion/rewards,
move reset and outer turn/action cadence; motion/float/pose services and actual
death/revival event scripts. Continue full-system documentation before complete-
game requirements. Objective remains active/incomplete; original/native traces
and rendering comparisons still required.

## 2026-10-12 — Move-count reset and action/guard predicates

Previous goal turn progressed withf448f56 inventory/reservation contracts. Added
BATTLE_MOVE_COUNTS.md: Fast byte doubling/narrowing, Slow signed division and
low-count thresholds, reset snapshots/zero fallback, conditional setter reset,
max-count callback without reset, act/guard predicates and turn-first reset/
priority caller segment. DOL direct-call inventory identifies three reset sites.
Turn-first increments+24 before resetting all nonnull units; other+24/count
writers and outer cadence remain unrecovered. Null CanActStatus returns1 by
helper composition, without claiming a missing unit can act in the controller.

Validation:290 instruction anchors plus all prior checks passed read-only US ISO.
Evidence126 function fingerprints reproduced byte-identically; three direct
reset link sites checked. Behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, nothing to rebuild.
Workspace started clean; only research/tool metadata changed. No original code
execution/emulator run or profile/save modifications. Extracts/disassembly/reports
remain ignored; ASSETS.md updated. Static identity/anchors do not prove a native
implementation equivalent or establish complete Slow turn cadence.

Next: count consumption and+20/+23/+24 writers/consumers, full turn-first service
ordering and outer phase/action scheduler; queue/configuration/item lifecycle
callers, conclusion/rewards and actual death/revival scripts; motion/float/pose
services. Continue all systems before complete-game requirements. Objective
active/incomplete; original/native execution/rendering comparisons still needed.

## 2026-10-12 — Phase tables, actor ordering and count consumption regions

Previous turn progressed with14dddc1 move reset. Added BATTLE_PHASE_TABLES.md:
complete local phase-first table construction, stable absolute-home-X then
priority sort, separate slot/side-key phase-event table, phase wrapper and
phase-end predicate. Documented selected btlseqMove actor-selection and
btlseqAct count-consumption regions, including state normalization, one-use
count bypass, forced exhaustion and+24 reset/preserve flag. The full move/act
controllers remain explicitly outside detailed coverage despite fingerprinting.
This adds evidence toward Slow cadence without assuming all state producers.

Validation:304 instruction anchors plus15-entry move table hash, two phase-sort
constants and all prior checks passed read-only US ISO. Evidence134 function
fingerprints matched a repeat byte-identically. Behavior links resolve; git diff
--check passed. make test passed core plus36 Python tests; make all exited0,
no rebuild. Workspace started clean; only research/provenance/tools edited.
No original execution/emulator run or profile/save modification. Original
extracts/disassembly/reports remain ignored; ASSETS.md updated. Static identity
and local checks do not prove full scheduler or native dynamic equivalence.

Next: complete btlseqMove/btlseqAct state maps, command/attack and phase-event
services; state-byte/flag producers and outer turn progression, conclusion/
rewards and actual death/revival scripts; motion/float/pose services. Continue
all systems before complete-game requirements. Objective remains active and
incomplete; original/native trace/rendering comparisons still required.

## 2026-10-12 — Complete local btlseqMove state map

Previous turn progressed withce9b8a0 phase tables/consumption regions. Added
BATTLE_MOVE_CONTROLLER.md: all15 local states, immediate loop versus explicit
fallthrough versus outer-call return, optional Majinai event, selector1 fan-out,
selector0 serial phase events, bookkeeping masks/typed clears, actor handoff,
kind222 command flags, post-action conclusion/phase-end path, audience and dual
announcement drain. Recovered SetTotalHitDamage's signed16-to-word write. Phase
table contract and index now link complete local move coverage. Full action
controller/services/outer cadence remain open; no complete battle claim.

Validation:320 instruction anchors and prior table/constant/script checks passed
against read-only US ISO. Evidence135 function fingerprints reproduced exactly.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only
research/provenance/tool metadata edited. Original disassembly/report files
remain ignored. No original execution/emulator run or profile/save changes.
ASSETS.md updated. Static checks do not prove callee semantics or dynamic/native
scheduler equivalence.

Next: complete btlseqAct, command/menu/attack and Majinai services/script,
audience/announcement internals and external channel/flag writers, outer turn
progression, conclusion/rewards and death/revival scripts; motion/float/pose
services. Continue all-system register before complete-game requirements.
Objective active/incomplete; original/native execution comparisons required.

## 2026-10-12 — Complete local btlseqAct state map

Previous turn progressed withb567a35 move controller. Added
BATTLE_ACTION_CONTROLLER.md covering all15 local states: kind/status/attribute
routing, camera/command gates, alternate data-key paths, priority-ordered event
selection, retained Electric draw and Confuse threshold, wait-event deletion,
barrier/announcement fallthrough, actor cleanup/count consumption, reserve-item
consumption, audience/Break Slot gating and explicit post-event+2cc polling.
Move/phase contracts now link complete local action coverage. Callee/script
bodies and outer scheduler remain open; local coverage is not full-game fidelity.

Validation:336 instruction anchors plus action15-entry table fingerprint and
camera2.0 constant, alongside prior checks, passed read-only US ISO. Evidence135
function fingerprints reproduced byte-identically. Behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Workspace started clean; only research/provenance/verifier
files edited. No original execution/emulator run or profile/save modifications.
Extracts/disassembly/reports stay ignored; ASSETS.md updated. Static checks do
not prove call interpretation or native dynamic-equivalence.

Next: command/menu and camera helpers, default/status/action script bodies and
flag writers, audience/announcement/Break Slot services, outer turn/controller
cadence, conclusion/rewards and death/revival execution; motion/float/pose
services. Continue all systems before complete-game requirements. Full objective
active/incomplete; original/native trace/rendering comparisons required.

## 2026-10-12 — Default Electric/zero-gravity scripts and entry services

Previous turn progressed witha82ddfc action state map. Added
BATTLE_DEFAULT_ACTION_EVENTS.md: complete local Electric interpolation/display/
wait script and zero-gravity pose/camera/message/wait sequence, original message
ID binding, shared interpolation locals/continuation/truncation, mode8 binary32
operation order/constants, display-offset sentinel and integer wrapper, command
initialization leaf and camera-speed vector/sqrt caller. Electric's final LW10
wait target remains distinct from owner.17 samples/yields are derived call counts,
not measured frames. Other curves and cosine/sqrt library semantics remain open.

Validation:348 instruction anchors, two default-script hashes, five float
bindings, mode8 dispatch target and original message ID plus previous checks
passed read-only US ISO. Evidence142 function fingerprints reproduced exactly.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research/
provenance/tool metadata edited. No original executable/emulator run or profile/
save changes. Script/code extracts and reports remain ignored; ASSETS.md updated.
Hashes/static checks do not prove dynamic or visual replacement equivalence.

Next: camera/message callbacks and original US message/control service, full
CommandInput, display/pose services, interpolation modes and library numeric
contracts; outer battle/turn cadence, audience/announcement and conclusion/
reward/death/revival events. Continue all systems before complete-game system
requirements. Objective active/incomplete; original/native traces still needed.

## 2026-10-12 — Camera request wrappers and setter semantics

Previous turn progressed with18dc31c default action scripts. Added
BATTLE_CAMERA_REQUESTS.md: mode/speed/zoom/homing callbacks, signed admission
threshold, mode reset and independently gated axis-reset calls, invalid speed
fallback, event zoom truncation versus direct float setter, first home versus
second hit-position cache, ignored first homing operand and−1 preserving second
target. Derived zero-gravity request outcomes retain these differences. Main
camera modes, producers and rendering remain explicitly open.

Validation:364 instruction anchors, mode-reset0.0 constant and prior checks
passed read-only US ISO. Evidence152 function fingerprints reproduced exactly.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research,
provenance and evidence tooling edited. No original execution/emulator run or
profile/save changes. Extracts/disassembly/reports remain ignored; ASSETS.md
updated. Static evidence does not establish actual camera shots or native fidelity.

Next: original message callback/control service, main camera update modes and
priority/vector producers, full CommandInput; audience/announcement services,
outer battle/turn progression, conclusion/rewards and death/revival events;
motion/float/pose services. Continue all systems before complete-game requirements.
Full objective active/incomplete; original/native execution comparisons needed.

## 2026-10-12 — Battle announcement timer, shared text and draw callback

Previous turn progressed with5a231ae camera requests. Added
BATTLE_ANNOUNCEMENT_MESSAGES.md: five resolved operands, first-call-only text/
position/timer initialization, mode1 original item-name branches, unchecked
parameter setter, per-invocation display registration, mode2 trigger gate,
post-decrement word/yield protocol and local draw geometry. Per-event timer and
shared global text/position ownership remain distinct. Added namespace-qualified
symbol selection in evidence tooling to identify battle_message.o's local _disp.
Original story text is not copied or rewritten; lookup/control semantics remain
open, along with unknown substitution helper and renderer/display-list lifetime.

Validation:380 instruction anchors, four announcement floats/window-color word
and prior checks passed read-only US ISO. Evidence157 fingerprints reproduced
exactly; qualified draw address/size checked explicitly. Behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Workspace started clean; only research/provenance/evidence
files changed. No original execution/emulator run or profile/save modification.
Original extracts/disassembly/reports stay ignored; ASSETS.md updated. Static
checks do not prove actual message timing, text rendering or native equivalence.

Next: msgSearch resources/fallback/encoding and exact US control-token service,
substitution helper, font/window/display lifetime, announcement parameters and
caller census; camera modes/producers, full command input, outer battle/turn
cadence and conclusion/reward/death/revival events. Continue all systems before
complete-game requirements. Objective active/incomplete; original/native traces
and rendering comparisons needed.

## 2026-10-12 — US message resources, loading and lookup

Previous turn progressed with3561cd8 announcement messages. Added
MESSAGE_RESOURCES_AND_LOOKUP.md: two-slot resource records, seven language paths,
slot1 maximum-language allocation, slot0 map allocation, retained-buffer failure
path, pair-index sorting, exact slot precedence and binary-search bounds,
localized fallback, 128-byte command split and language-dependent lead predicate.
Duplicate ordering, complete control dispatch, glyph decoding, caller lifetimes
and story conditions remain explicitly open. No original dialogue is rewritten.

Added read-only message_resource_evidence.py. Census of260 US files found13,186
key/text pairs, two duplicate key occurrences, zero unmatched keys/nonzero tails
and4,366 high bytes. Metadata retains offsets, raw key hex and text hashes;
literal tag lexemes are not interpreted controls. Reports and original extracts
remain ignored. No assumed UTF-8 normalization or dialogue export is committed.

Validation:392 instruction anchors and prior constants/tables/script checks
passed against the read-only US ISO. Evidence162 function fingerprints and full
message census each reproduced byte-for-byte in a second report. Five bounded
parser cases and all behavior document links passed. make test passed core plus36
Python tests; make all exited0, no rebuild; git diff --check passed. Workspace
started clean; only research/provenance/evidence tooling changed. No original
execution, emulator run or profile/save changes. Static checks do not establish
original text rendering, timing or native-game equivalence.

Next: msgAnalize command dispatch/variables/branches, font byte-to-glyph mapping,
resource initialization/release/callers and original qsort duplicate ordering;
story attribution/order/event conditions, substitution and display lifetime.
Continue camera/input/outer battle and other field/content/save services before
complete-game requirements. Full objective active/incomplete; original/native
execution comparisons still needed.

## 2026-10-12 — Message parser layout, control records and initialization

Previous turn progressed with8e00ce7 resource lookup. Added
MESSAGE_PARSER_RECORDS.md from the US msgAnalize binary: append-only 24-byte
record layout, terminal record outside the count, persisted coordinates and
local style/speed state, US versus Japanese glyph paths, line rounding,
post-command LF suppression, waits/style/color/audio controls, window types,
positioning and selection setup. Recorded the five-conversion/four-prepared-
destination sscanf ambiguity rather than inventing a safe grammar. Icon/pose/
annotation branches and consumers remain explicitly unfinished. Extended lookup
documentation with msgInit's original key-wait pose setup and global slot load.
Original dialogue and story conditions are not rewritten or inferred.

Validation:408 instruction anchors and prior checks passed against read-only
US ISO, plus two command/format block hashes, layout floats/colors and pose
bindings. An initially mistyped anchor address was corrected to the inspected
instruction at0x8008204c before the passing run. Evidence164 fingerprints
reproduced byte-for-byte. All behavior document links resolve; git diff --check
passed. make test passed core plus36 Python tests; make all exited0, no rebuild.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification. Original
extracts/disassembly and reports remain ignored. Static checks do not prove
consumer timing, rendered dialogue or complete-game equivalence.

Next: remaining icon/anim/paper/annotation parser paths, font glyph/width tables,
msgMain record consumption and window/key/audio/pose lifetimes; resolve select
format ABI against original resource data. Continue story attribution/order/
conditions and remaining camera/input/outer battle/field/content/save systems
before complete-game requirements. Full objective active/incomplete.

## 2026-10-12 — Embedded message objects and original control shapes

Previous turn progressed withcc0a578 parser records. Added
MESSAGE_EMBEDDED_OBJECTS.md: all18 US icon name/ID bindings, numeric/name/HM
routing, fused icon geometry and independent advance, animation pose setup
including global-versus-new callback ID, paper NPC records and annotation
anchor/gates/centering/halfword rules. Literal one-byte-versus0x2f72 comparison
makes the apparent closing annotation gate unreachable. Local msgAnalize
branches are now covered across two files; services/consumers remain incomplete.

Census schema2 now records selected control argument shapes. US resources have
394 icon tags, five anim tags and280 select tags, each select with four lexical
fields:278 all-integer, two containing a %d placeholder. No paper/r/R-prefix
lexemes were found in this bounded US scan. This narrows ordinary resource
selection shape without asserting substituted runtime values, library failure
behavior or absence of generated/non-US inputs. Dialogue remains reused data.

Validation:424 instruction anchors and prior checks passed read-only US ISO,
plus original icon table hash/all18 string-ID bindings and geometry constants.
Corrected a mistyped icon-advance anchor to inspected0x80082cbc before passing.
Evidence165 fingerprints and census schema2 each reproduced byte-for-byte;
aggregate resource counts/shapes were independently asserted. Eight synthetic
control-shape categories and all behavior links passed. make test passed core
plus36 Python tests; make all exited0, no rebuild; git diff --check passed.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save changes. Original code/data
extracts and evidence reports remain ignored. Static anchors do not prove visual
geometry, pose lifetime or original/native behavior equivalence.

Next: msgMain record playback and window/key/audio/pose lifetimes; font glyph
mapping/width data and icon dimensions; substitution callers for selection
placeholders. Continue story attribution/order/conditions and the remaining
battle/camera/input/field/content/save systems before complete-game requirements.
Full objective remains active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Message playback and record admission

Previous turn progressed withad14ca1 embedded objects. Added MESSAGE_PLAYBACK.md:
complete local msgMain branch/14-opcode map, clock quotient/low-word schedule
advancement, two separately rounded scroll additions, input masks and latched
acceleration, wait-state/resume return codes, callbacks/history, terminal outside
parser count, unchecked schedule admission and paper-end index/pointer divergence.
Documented paper pose/group bindings and generic-record sound admission once per
invocation, including color/noise records and refreshed-versus-cached handle data.
No delays/rates are claimed as measured. Caller/state producers remain open.

Validation:440 instruction anchors and prior checks passed against read-only
US ISO, plus all14 playback targets, scroll coefficient and seven paper pose/
group bindings. Evidence167 function fingerprints reproduced byte-for-byte.
All behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only
research/provenance/evidence tooling changed. No original execution/emulator run
or profile/save modification. Code/data extracts and reports remain ignored.
Static checks do not establish clock/callback cadence, renderer fidelity or
original/native dialogue behavior equivalence.

Next: message creation/event callbacks and wait/input state producers, render/
selection/window lifetimes; clock snapshots and numeric helpers; font/icon/pose/
NPC/sound dependencies. Continue full story attribution/order/conditions and
remaining battle/camera/input/field/content/save systems before complete-game
requirements. Full objective active/incomplete; executed comparisons needed.

## 2026-10-12 — General/clear message states and history controls

Previous turn progressed withba1d207 selection. Added MESSAGE_WINDOW_STATES.md:
all12 general dispatch targets, talk countdown before dispatch, type-indexed
opening/closing sounds, integer elapsed fade/narrowing, retained stop-state
poll completion, wait-state resume/acceleration gating, history count-2 entry,
signed target scroll and ordered navigation. Recovered separate clear-state
behavior, playback during opening, no local wait input and forced-versus-terminal
closing timestamp differences. Renderer and event/callback producers remain open.

Validation:496 instruction anchors and prior checks passed read-only US ISO,
plus all12 general dispatch targets,24 sound table values and history binary32
coefficient. Evidence188 fingerprints reproduced byte-for-byte. All behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts/reports remain ignored. Static checks do not prove
measured fades/history/input cadence, presentation or original/native equivalence.

Next: message/selection/key-wait rendering and manager scheduling; event print
identity/callback/substitution producers; pose/font/icon/audio services and clock/
input numeric dependencies. Continue full story attribution/order/conditions and
all remaining systems before complete-game requirements. Objective remains
active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Message window creation and lifecycle controls

Previous turn progressed with2f17da5 playback. Added MESSAGE_WINDOW_LIFECYCLE.md:
seven-slot first-free pool and allocation/poll flags, fixed work zeroing and
creation timestamps/defaults, temporary strlen-sized strcpy allocation,
type/wpos/selection/small geometry, append over terminal, Continue/Repeat partial
reset, deletion's empty record-count loop, identity-based force-close/NPC versus
battle stay restoration and first-call event repeat/continue polling. Normal
constructor zeroing explains untouched parser schedule fields; allocator slack,
callback identity and main-state completion remain explicit dependencies.

Validation:456 instruction anchors and prior checks passed against read-only
US ISO, plus four default geometry floats and small-window multiplier.
Evidence179 function fingerprints reproduced byte-for-byte. All behavior links
resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Code/data extracts and reports remain ignored. Static checks do not
prove allocator failure behavior, window timing or original/native equivalence.

Next: msgWindow_Main/Clear/selection state machines, input wait-state transitions
and renderer; message event print/identity/callback producers and substitution;
allocator/embedded-pose ownership, font/icon/sound services. Continue story
attribution/order/conditions and all remaining battle/camera/input/field/content/
save systems before complete-game requirements. Full objective active/incomplete.

## 2026-10-12 — Selection window input and event result lifetime

Previous turn progressed with49af82c lifecycle. Added MESSAGE_SELECTION.md:
unconditional playback before selection state dispatch, strict opening clamp,
extra zero-opacity closing invocation, trigger wrap versus repeat endpoint guards,
independent opposite-direction blocks/net movement sound, cancel-before-confirm
including disabled-cancel precedence, signed cancel results, display registration,
first-call direct/key identity creation, saved +17c window index and signed local
work0 result before deletion. Recovered the three literal input snapshot readers
and both guarded-ID/unguarded-pointer window deletion helpers. Renderer, input
repeat producers/cadence and story caller/branch mapping remain explicitly open.

Validation:472 instruction anchors and prior checks passed read-only US ISO,
plus selection display-order base and original direct-input identity.
Evidence186 function fingerprints reproduced byte-for-byte. All behavior links
resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts and evidence reports remain ignored. Static checks
do not establish actual navigation cadence/highlighting or native equivalence.

Next: general/clear message window states and wait/input transitions; selection/
message renderer, event print callback/identity/substitution producers; clock and
input snapshot cadence, font/icon/pose/NPC/audio dependencies. Continue story
attribution/order/conditions and all remaining systems before complete-game
requirements. Full objective active/incomplete; executed comparisons needed.

## 2026-10-12 — Common print event identity and speaker placement

Previous turn progressed with9aa89de general/clear states. Added
MESSAGE_PRINT_EVENTS.md: four resolved operands, first-call-only window creation,
direct/key identity and event ownership, consumed layer/placement words,
event NPC owner versus named override, ten special-name/sentinel/relationship
routes, partner fallback/Mario/NPC anchors, battle speaker fields and projected
placement/ordered overrides. Preserved literal0x1effffff mask and completion on
poll flag rather than allocation/deletion. Caller story conditions and helper
services remain explicit gaps; original dialogue is not rewritten.

Validation:512 instruction anchors and prior checks passed read-only US ISO,
plus projected threshold and all ten special NPC strings. Evidence191 function
fingerprints reproduced byte-for-byte. All behavior links resolve; git diff
--check passed. make test passed core plus36 Python tests; make all exited0,
no rebuild. Workspace started clean; only research/provenance/evidence tooling
changed. No original execution/emulator run or profile/save modification.
Original extracts/reports stay ignored. Static checks do not prove actual speaker
attribution in every scene, projection rendering or original/native equivalence.

Next: print partner/insert/add variants, placement/layer producers and substitution
callbacks; renderer/key-wait indicator/manager cadence and speaker/font/icon/pose/
audio dependencies. Continue original story call-graph attribution/order/branches/
conditions and all other systems before complete-game requirements. Full objective
active/incomplete; executed-original comparisons still needed.

## 2026-10-12 — Partner print variants and placement setters

Previous turn progressed with2713628 common print. Added MESSAGE_PRINT_VARIANTS.md:
NPC name-only mutation, pending layer, accumulated orientation flags and literal
preserved placement mask/float operands; all11 field/seven battle suffix bindings,
signed field index versus e0-based battle kind, three distinct64-byte buffers,
variant first-call gates/shared identity lifetime, resumed null battle-unit early
completion and direct/key generic append. Original keys/text are retained; suffix
character/caller attribution and insertion substitutions remain open.

Validation:528 instruction anchors and prior checks passed read-only US ISO,
plus all18 suffix bindings and original field speaker literal. Evidence199
function fingerprints reproduced byte-for-byte. All behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Workspace started clean; only research/provenance/evidence
tooling changed. No original execution/emulator run or profile/save changes.
Original extracts/reports remain ignored. Static checks do not prove concurrent
buffer ownership, story branch attribution or original/native equivalence.

Next: insert/add-insert substitutions and callback bodies; partner/battle key
caller census and character mappings; renderer/key-wait indicator/manager cadence
and service dependencies. Continue full story attribution/order/branches/conditions
and all remaining systems before complete-game requirements. Full objective
active/incomplete; executed-original comparisons still needed.

## 2026-10-12 — Insert formatting callbacks and percent lexeme census

Previous turn progressed withaa1e5d4 variants. Added MESSAGE_INSERT_FORMATTING.md:
signed encoded insertion count, fixed/resumed operand resolution, one-through-four
word-register sprintf dispatch, raw/key format source, unchecked transient output,
no zero-count copy path, forced-direct new-window identity and first-call-only
append consumer. Original printf ABI/call-site types remain explicitly open.
No general token replacement behavior is inferred from these wrappers.

Census schema3 adds per-pair/aggregate printf-like metadata:93 US pairs/119 matches,
including one prose-like percent match. Lexical counts are not formatting-call
or story execution evidence; no dialogue/format text is rewritten or exported.

Validation:544 instruction anchors and prior checks passed read-only US ISO.
Evidence201 fingerprints and census schema3 each reproduced byte-for-byte.
Six synthetic lexical categories, complete US aggregate counts and all behavior
links passed; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts/reports remain ignored. Static checks do not prove
printf/library equivalence, valid insertion callers or native dialogue behavior.

Next: remaining locale/replacement helpers/callback bodies and original sprintf
library route; insert format/type/count call-site census; renderer/manager cadence
and service dependencies. Continue original story attribution/order/branches/
conditions and all remaining systems before complete-game requirements. Full
objective active/incomplete; executed-original comparisons still needed.

## 2026-10-12 — Original format output and varargs consumers

Previous turn progressed with1fa2d28 insertion wrappers. Added FORMAT_OUTPUT_RUNTIME.md:
sprintf/vsprintf register-save setup and return-index termination (including-1),
string sink clipping/requested-versus-copied counts, varargs general/floating/
64-bit slot and overflow alignment, formatter literal streaming/consumer dispatch,
string precision/length-prefix/empty-null behavior, n stores, padding/sign order
and literal-tail fallback. Grammar/numeric/wide conversion helpers and actual
call-site validity remain explicitly open. No host-libc equivalence is assumed.

Validation:560 instruction anchors and prior checks passed read-only US ISO,
plus original narrow-null string's empty sentinel. Evidence206 fingerprints
reproduced byte-for-byte. All behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification. Code/data
extracts/reports remain ignored. Static checks do not prove library execution,
valid formatting callers or original/native dialogue equivalence.

Next: printf parse_format grammar and integer/decimal/hex-float helpers, wide
conversion/memory services; replacement/callback bodies and insertion call-site
census; renderer/manager cadence and original story attribution/order/branches/
conditions. Continue all remaining systems before complete-game requirements.
Full objective active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Original printf directive grammar and normalization

Previous turn progressed withf1ccbdc output/varargs. Added FORMAT_DIRECTIVE_PARSER.md:
complete local repeated flag precedence, dynamic/digit width and509 cap,
precision-presence/negative-star/word-wrap rules, h/hh/l/ll/L enums, complete
conversion admission/normalization map and pointer formatting. Preserved unusual
lc/hhn consumer interactions and argument consumption before later rejection.
Corrected format-record byte3 from string-only naming to alternate form, including
its existing length-prefixed string interpretation. Numeric converters remain open.

Validation:576 instruction anchors and prior checks passed read-only US ISO,
plus both parser dispatch hashes and byte-classification hash/ASCII-digit mask.
Evidence207 fingerprints reproduced byte-for-byte; qualified printf address/size
verified explicitly against the report, avoiding scanf's same local symbol name.
All behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research/
provenance/evidence tooling changed. No original execution/emulator run or profile/
save changes. Original extracts/reports remain ignored. Static checks do not prove
executed formatting/call-site validity or original/native equivalence.

Next: integer/decimal/hex-float conversion algorithms and wide/memory services,
format/type/count caller census, replacement/callback bodies and rendering/manager
cadence. Continue original story attribution/order/branches/conditions and all
remaining systems before complete-game requirements. Full objective active/
incomplete; executed-original comparisons still needed.

## 2026-10-12 — Original integer text conversion stages

Previous turn progressed withc0e0499 directive parser. Added FORMAT_INTEGER_CONVERSION.md:
32/64-bit ABI, NUL/reverse storage, zero-precision early return, signed minimum
modulo negation/unsigned division, base/sign-policy cases, digits/case, alternate
octal counting, mode2 width-to-precision mutation, literal capacity expression,
unconditional admitted hex prefixes and final sign order. Caller-copy mutations
and helper-failure literal-tail behavior remain distinct. Floating/wide/helper
arithmetic execution and original formatting caller census remain open.

Validation:592 instruction anchors and prior checks passed read-only US ISO,
plus both33-entry integer dispatch-table hashes. Evidence209 fingerprints
reproduced byte-for-byte. All behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification. Original
extracts/reports remain ignored. Static checks do not prove executed formatting,
valid caller ranges or original/native equivalence.

Next: decimal/hex-float conversion and rounding, unsigned64 and wide/memory
services; actual format/type/count caller census, replacement/callback bodies,
renderer/manager cadence and original story attribution/order/branches/conditions.
Continue all remaining systems before complete-game requirements. Full objective
active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Decimal record rounding and preparation

Previous turn progressed withc66da66 integer conversion. Added FORMAT_DECIMAL_ROUNDING.md:
record ownership/count/ASCII digits, negative/no-op retention gates, discarded-tail
ties, zero-retention count-byte parity, carry propagation/logical trimming,
canonical zero/exponent wrap and sign retention. Recorded float2str entry cap,
32-digit generation request, trailing-zero preparation and original signed/case
infinity/NaN string routes. Full finite layout and binary decimal generation
remain explicit gaps; derived record examples are not executed original tests.

Validation:608 instruction anchors and prior checks passed read-only US ISO,
plus special-value string-block hash, binary64 sign-comparison zero and uppercase
classification. Evidence211 fingerprints reproduced byte-for-byte. All behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts/reports remain ignored. Static checks do not prove
binary decimal generation, executed tie cases or original/native equivalence.

Next: finite float2str layout/mode selection/capacity, __num2dec generation and
hex-float output, runtime helper/wide/memory contracts; actual format/type/count
call-site census, replacement/callback bodies, renderer/manager cadence and story
attribution/order/branches/conditions. Continue all remaining systems before
complete-game requirements. Full objective active/incomplete; executed-original
comparisons needed.

## 2026-10-12 — Finite decimal formatting layout

Previous turn progressed withd4949a0 rounding/preparation. Added
FORMAT_FINITE_DECIMAL_LAYOUT.md: leading-exponent normalization, post-round g/G
mode boundaries and alternate precision rewriting, scientific exponent/mantissa
storage, fixed retention budget/rounding/digit-zero placement, sign policy,
caller-copy mutations and distinct capacity predicates. Kept fixed admission's
omission of requested trailing-zero bytes explicit, with derived record shape
rather than an asserted executed overrun or supported caller. Binary generation
and hex-float/service/caller contracts remain open.

Validation:624 instruction anchors and prior checks passed read-only US ISO.
Fresh evidence211 fingerprint report matched the previous repeat report exactly.
All behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research/
provenance/verifier tooling changed. No original execution/emulator run or profile/
save changes. Original extracts/reports remain ignored. Static checks do not prove
generated decimal record invariants, executed output/capacity or native equivalence.

Next: __num2dec binary generation, hex-float output and runtime/wide/memory services;
actual format/type/count callers, replacement/callback bodies, rendering/manager
cadence and original story attribution/order/branches/conditions. Continue all
remaining systems before complete-game requirements. Full objective active/
incomplete; executed-original comparisons needed.

## 2026-10-12 — Binary decimal entry and numeric records

Added FORMAT_BINARY_DECIMAL_ENTRY.md for __num2dec and __num2dec_internal:
numeric versus ASCII record phases, special-value bypass, signed request cap,
positive-retention ties/carry/padding, exponent conversion, raw sign and signed
zero, finite mantissa trailing-bit count, helper ordering, integer decimal
reversal and final sign overwrite. Power-of-two decimal, multiplication and
runtime numeric helper internals remain explicit dependencies. Conventional
significand factorization is labeled inference pending those helper contracts.

Validation:640 instruction anchors and prior checks passed against the read-only
US ISO, including the distinct binary decimal input-zero constant. Two fresh
213-function fingerprint reports matched byte-for-byte. All local behavior
documentation links resolve; git diff --check passed. make test passed core plus
36 Python tests; make all exited0, no rebuild. Only research/provenance/evidence
tooling changed. No original execution/emulator run or profile/save modification.
Original extracts/reports remain ignored. Static checks do not establish complete
binary decimal generation, executed caller behavior or original/native equivalence.

Next: numeric decimal multiplication and power-of-two generation; frexp/ldexp/
modf and integer conversion/division contracts; hex-float and remaining formatting
services. Continue original call-site, renderer, story and other system recovery
before complete-game requirements. Full objective active/incomplete; executed
original comparisons still required.

## 2026-10-12 — Numeric decimal multiplication

After b7934cc, added FORMAT_DECIMAL_MULTIPLICATION.md for __timesdec: numeric
record domain/sign0, coefficient traversal and unsigned quotient/remainder carry,
scratch order, leading exponent, 36-digit copy, complete discarded-tail tie scan,
retained parity and count-preserving carry. Recorded lack of trimming/canonical
zero and unchecked scratch-count domain. Updated binary entry cross-links to
separate the now-recovered multiplication from open power-generation invariants.
Examples are static derivations, not executed original traces.

Validation:656 instruction anchors and prior checks passed against read-only US
ISO, including both halves of the quotient reciprocal constant. Fresh214-function
evidence reports matched byte-for-byte. All local behavior links resolve; git
diff --check passed. make test passed core plus36 Python tests; make all exited0,
no rebuild. Only research/provenance/evidence tooling changed. No original game
execution/emulator run or profile/save changes; original extracts/reports remain
ignored. Static verification does not prove valid power records, executed tie
cases/capacity or original/native equivalence.

Next: __two_exp table/recursion and numeric runtime services, then hex-float,
remaining formatter services/caller corpus; continue renderer, story and all
other missing original systems before complete-game requirements. Full objective
active/incomplete; original execution comparisons still needed.

## 2026-10-12 — Decimal powers of two

Previous goal turn made progress through b7934cc and480f34c. This turn began
clean and added FORMAT_DECIMAL_POWERS.md for __two_exp:73-entry sparse dispatch,
21 literal seed bindings, numeric record copy, original signed ASCII-byte versus
numeric5 comparison, both long-seed increments, identical seed-copy bodies,
signed halving, recursive square/odd correction,42-byte copy and per-step rounding.
Recovered power digit/count bounds establish normal combined multiplication
scratch<=56 digits, conditional on a valid integer record. Updated entry and
multiplication dependencies; numeric services/caller power range remain open.
The-53 seed's discarded ASCII '2' still increments; this static consequence is
explicitly distinguished from an executed-original trace or modern nearest seed.

Validation:672 instruction anchors and all prior checks passed read-only US ISO.
Independent verifier checks all73 targets,21 exponent/string bindings, mathematical
source digit identities and184-byte seed-only block hash. Inspected all21 seed
copy/round instruction bodies: identical after common-epilogue branch normalization.
Fresh215-function evidence reports matched byte-for-byte. All247 local behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save change; original instruction/
seed inspection outputs remain ignored. Static checks do not prove executed
accuracy, original caller domain or full conversion/native equivalence.

Next: frexp/ldexp/modf and unsigned-double/integer division services to resolve
finite conversion power range; hex-float, remaining formatter services and actual
caller corpus. Continue renderer, story and other uncovered original systems
before complete-game requirements. Full objective active/incomplete; original
execution comparisons still needed.

## 2026-10-12 — Binary64 services and finite generator bounds

Previous goal turn made progress through8e2c774. Began clean and added
FORMAT_BINARY_NUMERIC_SERVICES.md: full local frexp/ldexp/modf/copysign and
__cvt_dbl_usll contracts. Recorded subnormal normalization, exponent writes,
wrapped scale gates and floating overflow/underflow operations, integral masks,
raw signed-zero fractional return for NaN/infinity, and converter signed-range
saturation despite symbol naming. Recovered helpers establish positive odd
significand<=2^53-1 and finite generator power[-1074,1023]. Updated prior entry/
power/multiplication dependencies. Floating environment and division remain open.

Validation:696 instruction anchors and all prior checks passed read-only US ISO,
including six binary64 normalization/scaling constants. Fresh220-function evidence
reports matched byte-for-byte. Independent integer bit-position enumeration of
109816 normal/subnormal exponent/trailing-position classes confirmed exponent
[-1073,1024], width[1,53], power[-1074,1023]; this is a derived-domain check, not
an original execution or exhaustive input trace. All local behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Only research/provenance/evidence tooling changed. No original
execution/emulator run or profile/save modification. Original disassemblies and
reports remain ignored. Static evidence does not establish FPSCR policy, executed
special cases/output accuracy or original/native equivalence.

Next: unsigned64 division/remainder, hex-float and remaining wide/memory/format
services, original call-site types/counts and floating environment. Continue
renderer, story and other missing original systems before complete-game
requirements. Full objective active/incomplete; original execution comparisons
still needed.

## 2026-10-12 — Unsigned64 quotient and remainder

Previous goal turn made progress through db98d01. Began clean and added
FORMAT_UNSIGNED64_DIVISION.md: word ABI, leading-zero/bit-length early return,
word-shift construction, shared restoring loop and XER carry pipeline, signed
trial rejection, final quotient insertion versus remainder return. Recorded no
zero-divisor guard and derived zero-divisor outputs, including65 iterations and
source clearing at a high-bit numerator. Linked recovered arithmetic into integer
and decimal conversion. Valid finite significand needs<=16 decimal digits,
refining multiplication scratch to<=52 digits; arbitrary unsigned64 remains<=20.

Validation:712 instruction anchors and all prior checks passed read-only US ISO.
Fresh222-function evidence reports matched byte-for-byte. Authored word-shift/
trial-result recurrence matched integer divmod for105350 nonzero-divisor pairs
(small complete pair domain, power/adjacent boundaries and5000 seeded pairs,
deduplicated); nine zero-divisor examples were derived. These are contract-model
checks, not original instruction execution or exhaustive64-bit equivalence.
All local behavior documentation links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild. Only
research/provenance/evidence tooling changed. No original execution/emulator run
or profile/save change; original disassemblies/reports remain ignored. Static
checks do not prove executed ABI flags, actual caller domain or native equivalence.

Next: hex-float and remaining wide/memory/formatter services; original format
caller census, floating environment and message renderer/manager cadence. Continue
story attribution/order/branches/conditions and other missing original systems
before complete-game requirements. Full objective active/incomplete; original
execution comparisons still needed.

## 2026-10-12 — Hexadecimal float construction

Previous goal turn made progress through ba057ec. Began clean and added
FORMAT_HEXADECIMAL_FLOAT.md for full local double2hex: signed precision cap,
decimal-generator special recognition, raw special-value signs/case, signed
integer exponent suffix, raw fraction nibble order/truncation, fixed leading1,
zero/subnormal construction, alternate point, value sign and output displacement.
Recorded precision>13's actual stack-address formula, initialized control bytes
versus padding/other locals and maximal stack+263 read address at precision509.
Examples/address consequences are derived, not executed originals or supported
caller/overrun claims. Updated prior formatter documents and index to link the
now-recovered local numeric stages and remove stale recovery gaps.

Validation:728 instruction anchors and all prior checks passed read-only US ISO;
special strings reuse the previously verified original block. Fresh223-function
evidence reports matched byte-for-byte. All293 local behavior documentation links
resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save changes; original disassemblies/
reports remain ignored. Static evidence does not establish original caller domain,
executed stack reads/output accuracy or original/native equivalence.

Next: return to message rendering/manager cadence and record consumers, while
retaining wide/memory services, real insertion format/type/count callers and
floating environment as open formatter dependencies. Continue original story
attribution/order/branches/conditions and other uncovered systems before
complete-game requirements. Full objective active/incomplete; original execution
comparisons still needed.

## 2026-10-12 — Window manager and selection draw callback

Previous goal turn made progress through d6d6ed4. Began clean and added
MESSAGE_WINDOW_MANAGER.md and MESSAGE_SELECTION_DRAWING.md: partial pool init/
reinit, ordered live-slot callback traversal and caller placement, archive versus
DVD palette setup, rounded read size/ignored completion status, completion-word
path distinction and unchecked texture lookup. Selection draw contract records
fog/color sources, border/text/icon call order, geometry/low-byte opacity and
stored fused cursor smoothing during draw rather than main update. Pinned public
Japanese windowdrv.c supplies structural corroboration; US instructions establish
the local contracts. Updated lifecycle/state/selection cross-links.

Validation:756 instruction anchors and prior checks passed against read-only US
ISO, including two palette path strings, RGBA/seven selection floats and integer
conversion bias. Fog-color source is BSS and explicitly not an ISO constant;
runtime writers remain open. Fresh230-function evidence reports matched byte-for-
byte. All local behavior links resolve; git diff --check passed. make test passed
core plus36 Python tests; make all exited0, no rebuild. Only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
modification; original disassemblies/reports remain ignored. Static checks do not
prove global update/draw frequency, resource lifetime/error behavior, executed
images/cursor motion or original/native equivalence.

Next: msgWindow_Disp and msgDisp record consumers, display queue sorting/dispatch
and outer cadence, then border/icon/font service bindings and real message/story
callers. Formatter wide/memory/caller/floating-environment dependencies remain
open. Continue original story attribution/order/branches/conditions and all
other uncovered systems before complete-game requirements. Full objective active/
incomplete; original execution comparisons still needed.

## 2026-10-12 — Display queue and per-camera dispatch

Previous goal turn made progress through8985d33. Began clean and added
DISPLAY_QUEUE_AND_DISPATCH.md: buffer/record ownership, NaN registration gate,
class-dependent keys, count-only reset, sort wrapper/comparator, live camera
traversal and callback borrowing/current pointer, complete GX request map and
fused position-derived order helper. Recovered all eleven offset floats despite
US metadata declaring only ten. Pinned Japanese source corroborates the retail
eleventh element; US reads establish it. Updated window/selection cross-links.
qqsort outer body was inspected locally to locate its sorting dependency, but
its internal sorting/tie and permutation contract remains unfinished here.

Validation:780 instruction anchors and prior checks passed read-only US ISO,
including all eleven offset floats, eleven GX dispatch targets and five depth/
common-state constants. Fresh238-function evidence reports matched byte-for-byte.
All local behavior links resolve; git diff --check passed. make test passed core
plus36 Python tests; make all exited0, no rebuild. Only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
modification; original disassemblies/reports remain ignored. Static evidence does
not establish equal-key draw order, outer camera/reset cadence, service numerical
behavior, actual callback mutations/lifetimes or original/native equivalence.

Next: qqsort internal sorting/permutation/ties, msgWindow_Disp and msgDisp record
consumers, actual outer reset/sort/camera dispatch cadence and camera/matrix/GX
services. Continue real message/story attribution/order/branches/conditions and
all other uncovered systems before complete-game requirements. Full objective
active/incomplete; original execution/images comparisons still needed.

## 2026-10-12 — Display sort implementation and equal-key behavior

Previous goal turn made progress through25005b8. Began clean and added
DISPLAY_SORT_IMPLEMENTATION.md: qqsort comparator write/admission, global scratch,
fsort small pivot branches and strided sample quickselect, shared asymmetric
partition and zero-pivot relocation, smaller-side recursion/tie order, tail
iteration and cycle copying/null markers. Pinned Japanese system.c provides
complete structural guidance, checked against US branches. Recorded a derived
three-entry tie reversal[1A,1B,0C]->[0C,1B,1A], not host stable-sort semantics;
all-equal identity is only a special case. Updated display/window cross-links.

Validation:800 instruction anchors and prior checks passed read-only US ISO.
Fresh240-function evidence reports matched byte-for-byte. Authored contract
models checked3549 sort inputs, all5914 cycle permutations of lengths0..7 and
8072 bit-mask counts (1..3072 plus5000 seeded nonzero32-bit words). Sorting gave
ascending permutations and the tie example; cycles/bit lengths matched integer
reference results. These do not execute original instructions/memcpy or prove
every input/comparator. All local behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild. Only
research/provenance/evidence tooling changed. No original execution/emulator run
or profile/save change; disassemblies/reports remain ignored. Static/model checks
do not prove actual callback/caller domain, outer sorting/draw cadence, memory
service behavior or original/native equivalence.

Next: msgWindow_Disp and msgDisp record consumers; actual outer reset/sort/camera
cadence, matrix/GX/font/icon/copy services and real message/story callers. Continue
all original attribution/order/branches/conditions and other uncovered systems
before complete-game requirements. Full objective active/incomplete; original
execution/images comparisons still needed.

## 2026-10-12 — General message-window drawing

Previous goal turn made progress through247b864. Began clean and added
MESSAGE_WINDOW_DRAWING.md for full local msgWindow_Disp: twelve type routes,
current-camera getter, raw/projected anchors, mode0 interpolation arithmetic,
fused geometry, type1 draw-time stored position changes, exactly255 text gates,
direct text at any opacity, clipping/conversion order, common camera-scissor
restoration and retained-return indicator/pose update. Also recovered the local
unsigned32 clipping converter, including NaN/saturation, and linked the existing
hoshi_2/HOS_C_1 initialization binding. Border/text/projection/pose service bodies
and indicator result semantics remain open. Updated lifecycle/state cross-links.

Validation:824 instruction anchors and prior checks passed read-only US ISO.
Verified all twelve type targets, fifteen geometry/indicator floats, mode0 target,
three unsigned32 conversion constants and original msgw pointer initializer.
Fresh243-function evidence reports matched byte-for-byte. All local behavior links
resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification; original
inspection/report artifacts remain ignored. Static verification does not prove
actual current-camera/caller domain, projection/service arithmetic, indicator
semantics, executed scissor/images or original/native equivalence.

Next: msgDisp record consumers/return values and msgDispKeyWait_render; border/
font/icon/pose/GX/projected-camera services, actual outer reset/sort/camera cadence
and real story callers. Continue original attribution/order/branches/conditions
and other uncovered systems before complete-game requirements. Full objective
active/incomplete; original execution/images comparisons still needed.

## 2026-10-12 — Message draw admission, history and indicator callback

Previous goal turn made progress through c82cf29. Began clean and added
MESSAGE_DRAW_RECORD_ADMISSION.md: exposed live record prefix rather than parsed
count/schedules, reserved controls, signed/unsigned key-match result and terminal
cancellation, history visibility/emphasis/fades, pose transform/update requests,
icon clip/alpha/font-color restoration and ordinary glyph admission/emphasis.
Recovered msgDispKeyWait_render's ordered GX requests and incoming-word store
through animGetPtr. Updated window result/callback, parser and embedded-object
links. Normal msgDisp glyph effects after0x800839f4 remain explicitly open;
full-function fingerprinting is not a claim of full glyph presentation coverage.

Validation:848 instruction anchors and all prior checks passed read-only US ISO,
including twelve reserved-control targets, pose/icon/glyph constants and emphasis
color/vector bindings. Fresh246-function evidence reports matched byte-for-byte.
All local behavior links resolve; git diff --check passed. make test passed core
plus36 Python tests; make all exited0, no rebuild. Only research/provenance/evidence
tooling changed. No original execution/emulator run or profile/save modification;
original disassemblies/reports remain ignored. Static checks do not prove normal
glyph effects, inner services, active/reset/camera/caller cadence, actual indicator
images or original/native equivalence.

Next: msgDisp normal dynamic/shake/wave/selection/plain glyph tail, including
RNG/clock ordering and draw-time scale mutation; inner font/icon/matrix/pose/GX
services and actual outer cadence/story callers. Continue original attribution/
order/branches/conditions and all other uncovered systems before complete-game
requirements. Full objective active/incomplete; original execution/images
comparisons still needed.

## 2026-10-12 — Normal message glyph effects

Previous goal turn made progress through c898247. Began clean and added
MESSAGE_GLYPH_EFFECTS.md: normal glyph priority gates, two-draw one-sided shake,
dynamic width/matrix requests and stored factor relaxation, clock/divisor/numeric
helper order, fused phase/trig motion and selection two-draw/color/edge/rainbow/
index-reset sequence. Together with admission document, local msgDisp branches
are covered; inner services remain explicit dependencies. Updated prior coverage
links and RNG ledger to record original render-dependent shared RNG consumption.
No helper time unit is described as measured milliseconds/ticks.

Validation:864 instruction anchors and prior checks passed read-only US ISO,
including eight effect/color literals, five scale-vector templates and unsigned
conversion bias. Initial vector check failed because of a mistaken fifth-template
expectation; independent re-read confirmed all five(0,1,1), corrected the verifier
and full re-run passed. Existing246-function fingerprint coverage is unchanged;
two fresh reports matched byte-for-byte. All local behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Only research/provenance/evidence tooling changed. No original
execution/emulator run or profile/save modification; original inspection/report
artifacts remain ignored. Static checks do not prove numerical helper/trig/font/
matrix behavior, actual cadence/seeding, executed images or native equivalence.

Next: font glyph draw/state services and tables, signed64 clock division/float
conversion and trig/matrix helpers; actual outer reset/sort/camera cadence and
message/story callers. Continue original attribution/order/branches/conditions
and all other uncovered systems before complete-game requirements. Full objective
active/incomplete; original execution/image/seed comparisons still needed.

## 2026-10-12 — Shared font draw state

Previous goal turn made progress through afcc859. Began clean and added
FONT_DRAW_STATE.md for fourteen state helpers: global scale/raw-color/alpha/edge,
mutable color-pointer alpha composition, palette indexing, local Color_ versus
caller mutation, start resets and delegated JUT setup, exact noise/rainbow GX
request order and differing off-reset scope. Recorded original texture object50
and1/64 matrix request; no substitute noise asset. Updated message-effect/admission
links. Glyph/JUT/GX raster bodies and asset descriptor binding remain open.

Validation:896 instruction anchors and prior checks passed read-only US ISO,
including three palette words, default scale and noise-matrix constants.
Fresh260-function evidence reports matched byte-for-byte. An authored signed-
reciprocal interpretation matched floor(A*G/255) for all65536 byte pairs; this
is a derivation check, not original instruction execution. All local behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification; original
inspection/report artifacts remain ignored. Static checks do not prove delegated
font/GX behavior, texture50 image binding, current-state writers/cadence, executed
images or original/native equivalence.

Next: JUTFont_DrawStart and FontDrawCode/Mtx primitives, glyph/width/texture data
and descriptor50, signed64 clock/trig/matrix services and actual outer draw/story
callers. Continue original attribution/order/branches/conditions and all other
uncovered systems before complete-game requirements. Full objective active/
incomplete; original execution/image/seed comparisons still needed.

## 2026-10-12 — Font glyph requests and delegated GX setup

Previous goal turn made progress through b46e73f. Began clean and added
FONT_GLYPH_REQUESTS_AND_SETUP.md for FontDrawCode/CodeMtx and JUTFont_DrawStart:
ordinary scale/translation/current-camera requests, caller-matrix in-place output,
shared nonzero edge branch with four offset draws then center, raw-color/effective
alpha restoration versus inherited no-edge GX color, full delegated GX setup
order/arguments, static font texture object and word+16c texture-matrix mode.
Updated font state to resolve delegated active stage/generator reset scope while
retaining unknown inner GX/raster behavior and saved-state differences.

Validation:920 instruction anchors and prior checks passed against read-only US
ISO, including original1/512,1/1024,1/256 and zero constants. Fresh263-function
evidence reports matched byte-for-byte. All local behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Only research/provenance/evidence tooling changed. No original
execution/emulator run or profile/save modification; original disassemblies/
reports remain ignored. Static verification does not prove inner glyph geometry/
page vertices, concat/GX numerics, state producers/cadence, executed images or
original/native equivalence.

Next: _JUTFont_DrawPos glyph geometry/page lookup/vertices, font loading/width/
glyph texture data and descriptor50, signed64 clock/trig/matrix services and
actual outer draw/story callers. Continue original attribution/order/branches/
conditions and all other uncovered systems before complete-game requirements.
Full objective active/incomplete; original execution/image/seed comparisons
still needed.

## 2026-10-12 — Inner font glyph quads and mandatory RNG

Previous goal turn made progress through38471f5. Began clean and added
FONT_GLYPH_QUADS.md for _JUTFont_DrawPos and kanjiGetWidth: first inclusive WID1
lookup/null path, unsigned width/offset bytes, active descriptor/signed grid,
float/truncation/narrowing stages, mandatory two irand64 calls, hue requests and
all four FIFO position/color/UV orders. Recorded asymmetric bottom UV1 subtraction
versus addition and lack of local texture page switching. HSV2RGB entry was
inspected to establish pointer-only input; its full conversion remains open.

Crucial correction: nonshaking inner glyph quads also consume shared RNG even
with noise off. Updated outer-effect document and RNG ledger: shaking's two
irand10000 calls are additional to two irand64 per inner quad; plain/outlined
wrappers issue one/five quads and normal selected glyphs six. These are recovered
call-path counts, not a complete renderer/full-game RNG census or measured rate.

Validation:940 instruction anchors and prior checks passed read-only US ISO,
including HSV word, two-unit inset and signed/unsigned conversion biases.
Fresh265-function evidence reports matched byte-for-byte. All local behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification; disassemblies/
reports remain ignored. Static checks do not prove actual font data/page coverage,
HSV/GX/FIFO interpretation, clock/seeding/cadence, executed pixels or native
image/seed equivalence.

Next: font loading/setup/encoding and glyph/page data, HSV2RGB conversion and
texture descriptor50; signed64 clock/trig/matrix services, actual outer draw/
story callers. Continue original attribution/order/branches/conditions and all
other uncovered systems before complete-game requirements. Full objective active/
incomplete; original execution/image/seed comparisons still needed.

Concurrent-work note: after committing c1282fb, git status showed unstaged changes
in include/game.hpp, include/head_collision.hpp, source/game.cpp,
tests/core_tests.cpp and tools/verify_actor_collision.py. They were not authored,
staged or modified by this research step. The make test/make all results above
record the state when those checks ran, not validation of subsequent concurrent
native/collision changes. Preserve that work.

## 2026-10-12 — US HSV conversion and binary64 floor contract

Recovered HSV2RGB0x800752fc,size22c and floor0x80271f7c,size148 from the
read-only US DOL. Added detailed HSV_COLOR_CONVERSION.md and
FLOOR_NUMERIC_SERVICE.md: pointer-only four-byte input, input preservation,
alpha copy, packed return, division-by256 hue endpoint, fused single-precision
subtraction, six sector assignments and truncation; floor exponent/mantissa
regions, signed zero, negative carries, huge-plus-input probes and special-value
arithmetic. Linked the glyph contract and index and recorded provenance.
These are static contracts and exact-case derivations, not original execution.

Validation:964 instruction anchors and existing tables/constants checks passed
against the read-only ISO, including added HSV255/360/60 and floor1e300/zero
constants. Fresh267-function reports reproduced byte-for-byte. All378 local
behavior links resolved; git diff --check passed. make test passed core plus36
Python tests; make all exited0 with no rebuild. Those results describe the
workspace at the time of validation, not later concurrent native changes.
No original execution/emulator run, profile/save modification, asset conversion
or native implementation change by this research step. Reports/disassemblies
remain ignored; only authored contracts and evidence metadata are committed.

Unrelated unstaged work in include/game.hpp, include/head_collision.hpp,
source/game.cpp, tests/core_tests.cpp, tools/verify_actor_collision.py and now
docs/ARCHITECTURE.md/docs/MECHANICS.md was preserved and excluded from staging.

Next: font loading/setup/encoding, actual glyph/page data and texture descriptor50;
original FPSCR and executed hue/RGBA comparisons, GX raster/image evidence;
signed64 clock/trig/matrix services and story callers. Continue all uncovered
story/event/field/battle/audio/save/content systems before complete-game
requirements. Full documentation objective remains active/incomplete; no
complete-game or standalone numeric execution equivalence is claimed.

## 2026-10-12 — US font resource setup and code mapping

Recovered JUTFontSetup0x80075004,size2f8, JUTFont_CodeToGlyph0x80074e90,size174,
kanjiSearch0x800758e4,size20 and hankakuSearch0x80075904,size24. Added detailed
FONT_RESOURCE_SETUP.md and FONT_CODE_MAPPING.md: lazy nonnull guard, low16
selector table, chosen /f path versus seven /font probes, aligned allocation/
read/publish order, last INF1/GLY1 descriptors, texture/LOD requests, first WID1
and ordered width writes; first matching MAP1, all four types, sparse-search
inclusive upper bound, replacement semantics and byte/halfword wrappers.

Read all three BFN resources in-place without exporting payloads. Recorded
block/grid/dimension metadata and full-file hashes. US/EU space maps to glyph0;
0x8140 falls back to glyph0, so setup's width7 write is replaced by width9.
Japanese0x8140 maps to0x5d. These are static code/data deductions, not executed
font observations. Original /font path resolution, load/caller/error behavior,
text stream decoding and GX raster appearance remain open.

Validation:988 instruction anchors and prior tables/constants passed read-only
ISO; added seven name pointers, five path/name bindings and three complete BFN
hashes. Fresh271-function reports matched byte-for-byte. All390 local behavior
links resolved; git diff --check passed. make test passed core plus36 Python
tests; make all exited0 with no rebuild. Results describe the workspace when
checked, not subsequent concurrent changes. No original execution/emulator,
profile/save modification, font conversion or native implementation work.
Original ELFs/disassemblies/font metadata/reports stay ignored.

Unrelated unstaged docs/ARCHITECTURE.md, docs/MECHANICS.md, include/game.hpp,
include/head_collision.hpp, source/game.cpp, tests/core_tests.cpp and
tools/verify_actor_collision.py were preserved and excluded from this commit.

Next: width/line/string layout and byte-stream decoding consumers, real selector/
load/cadence callers and DVD service contracts; executed width/mapping/texture
comparisons and GX raster data. Continue uncovered story/event/field/battle/
audio/save/content systems before complete-game requirements. Full objective
remains active/incomplete; local contracts do not prove complete dialogue or
full-game equivalence.

## 2026-10-12 — Correct GameCube overhead response gate

Previous goal turn made progress with five-probe grounded supportb182246.
Current README also names Switch1.0.0 as the active STATIC documentation
reference; existing prototype is GameCube-based and Switch field parity remains
unverified. This step corrects a discovered bug in that existing prototype,
without pretending GameCube contracts establish Switch behavior.

Complete marioChkOverhead response trace shows8008cc9c–8008ccac accepts
ONLY motion3 jump or6 jumpStand, otherwise skipping SFX/clear/fall/Y-write.
Motion table verifies0 stay,1 walk,2 dash,10 fall,12 upstairs. Earlier
048ff7a/7857a24 falling/upstairs clamp behavior and supporting tests were too
broad; they validated implementation, not this omitted source motion gate.
Removed those responses. Current-frame overheadContact records queries for
normal standing/walking, falling and stairs without modifying trajectory.
Normal ascending jump still responds; apex/fall maps to10. Helper documents
3/6 gate, but jumpStand itself is not implemented.

Validation: `make test` passed core and36 Python tests; new low-roof standing/
walking checks preserve root/control, upstairs preserves3-tick rise despite
contact, falling under contact matches an unobstructed control tick. Query
reset and jump-response gate IDs tested; prior falling-clamp assertions replaced
with source-correct expectation. Initial new fixture variable-name collision
was fixed; source verifier's initially mistyped motion0 pointer was corrected
against CSV/table bytes, final checks pass. `build-host/core_tests --rooms`
passes all8 arrivals,18,315 indexed/full-scan probes and original water/void/
ceiling checks. `make -j2 all` builds native3dsx; actor verifier passes motion
table entries and3/6 comparisons/skip branch.

Native ordinary ceiling jumps/landed save exited0; native harbor stair/dock-
save/return exited0 with HP7. Commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/ceiling-road.bin --seconds 4 --input-script tests/azahar_ceiling.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
```
Ceiling run preceded equivalent explicit constexpr gate refactor; final harbor
run used final native build. No build overlapped emulator execution; no original
Switch execution or profile/save modification. Extracts/captures remain ignored.
Mechanics/architecture append clear superseding corrections. Separate research
changes are excluded from this feature commit.

Next: static Switch field motion/integrator/collision contracts before claiming
new-reference parity; preserve GameCube comparison provenance. Existing native
wall push-out/sliding, contact owner/cache outputs, direction rounding, stair
XZ/chaining, partner grounding and special states remain incomplete. Original
English story/dialogue, SFX/animations/camera/interiors and full playable content
still require work. Full goal active; hardware/complete fidelity unverified.

## 2026-10-12 — US message-width scanner and icon dimensions

Recovered FontGetMessageWidthLine0x8007554c,size320 and its null-output wrapper
FontGetMessageWidth0x80075528,size24, iconGetWidthHight0x8001a19c,size100 and
TEXGet0x80037b5c,size10. Added detailed FONT_MESSAGE_WIDTH.md and
ICON_DIMENSIONS.md: lead-byte/halfword versus single-byte routes, LF count,
wrapping glyph/max width, final spacing subtraction, first-WID1 lookup,
command/R-prefix/LF suppression and impossible byte-versus0x2f72 branch;
icon argument routing and fused arithmetic, ready gates, locale remaps,
binary offset directory, output alias/order and both-zero original-ID TPL
fallback. Linked the embedded-object contract and evidence index.

The draft initially inherited the pinned Japanese source's two-entry spacing
array. US CSV size and read-only bytes instead establish seven entries
4,2,2,2,2,2,2; corrected the draft before commit and verify all seven words.
No original execution or dynamic width/icon observation was performed. Scanf
conversion semantics, loading/relocation, runtime configuration/callers and
actual rendered/layout equivalence remain open. Story text was not rewritten.

Validation:1012 instruction anchors and prior tables/constants/resource hashes
passed read-only ISO, with seven spacing words, three icon/scanf bindings and
all twelve remap halfwords. Fresh275-function evidence reports reproduced
byte-for-byte. All403 local behavior links resolved; git diff --check passed.
make test passed core plus36 Python tests; make all exited0 with no rebuild.
No original execution/emulator run, profile/save modification, native code
change, asset conversion or payload export by this research step. Local
inspection ELFs/disassemblies/reports remain ignored.

At turn start the prior unrelated native/collision work was already committed
as398b4e0; this increment changes only authored research/provenance/evidence
files. Next: original scanf services for icon arguments and drawing string/
line consumers; actual font/icon loading/cadence callers and executed numeric/
layout/image comparisons. Continue uncovered story/event/field/battle/audio/
save/content systems before complete-game requirements. Full goal active/
incomplete; recovered local contracts do not prove complete-game fidelity.

## 2026-10-12 — US sscanf entry, reader and formatter consumers

Recovered sscanf0x8026c344,sizec4, __StringRead0x8026c408,size90 and complete
local __sformatter0x8026c498,size9ac. Added SCAN_INPUT_RUNTIME.md and
SCAN_FORMAT_CONSUMERS.md: varargs ABI and empty-input -1 gate; unsigned reader,
EOF flag/unread protocol; descriptor-consumer fields, argument acquisition,
whitespace/literal routes, integer/floating helper requests and storage widths,
partial c and suppressed c accounting/high-byte difference, s/[ acceptance and
wide pointer strides, n storage and assignment/success/EOF return separation.
Linked icon-width dependency without claiming scanf grammar/numeric equivalence.

Validation:1040 instruction anchors and prior tables/constants/font checks passed
against read-only ISO; complete256-byte classification hash and six whitespace
codes checked. Fresh278-function reports reproduced byte-for-byte. All415 local
behavior links resolved; git diff --check passed. make test passed core plus36
Python tests; make all exited0 with no rebuild. No original execution/emulator,
profile/save modification, native implementation, asset conversion or message
rewrite. Original ELFs/disassemblies/reports remain ignored; authored contracts
and evidence metadata only. Working tree was clean before this increment.

Remaining: scanf-specific parse_format0x8026ce44, numeric __strtoul/__strtoull/
__strtold and mbtowc internals, actual icon argument corpus and executed callback/
destination/count comparisons; drawing/string layout consumers and runtime
resource/cadence callers. Continue uncovered story/event/field/battle/audio/
save/content systems before complete-game requirements. Full goal active/
incomplete; static local contracts do not prove complete scanner or full-game
fidelity. Source text, message IDs/control tokens and story conditions must remain
faithful reused data.

## 2026-10-12 — US scanf directive grammar

Recovered scanf-specific parse_format0x8026ce44,size548; added namespace-qualified
fingerprint separate from printf's same-name parser. Added detailed
SCAN_DIRECTIVE_PARSER.md:40-byte defaults, immediate percent shortcut,
suppression, decimal width accumulation without cap and with word wrap/zero
rejection, six parsed length selectors,52-slot conversion routing/validation,
p normalization, s acceptance bitmap, scanset first-bracket/complement handling,
signed-byte ranges and descriptor/continuation returns. Updated scan dependency
links/index without equating this service to host scanf.

Validation:1064 instruction anchors and prior tables/constants/resource checks
passed read-only ISO. Added all40 initializer bytes, full52-target dispatch hash
and ten digit classification codes. Fresh279-function reports reproduced
byte-for-byte. All424 local behavior links resolved; git diff --check passed.
make test passed core plus36 Python tests; make all exited0 with no rebuild.
No original execution/emulator, profile/save modification, native implementation,
asset conversion or story/message rewriting. Local filtered symbol CSV, ELF,
disassembly and reports remain ignored; authored prose/evidence tooling only.
Working tree was clean at turn start.

Next: __strtoul/__strtoull/__strtold and mbtowc dependencies; executed descriptor/
bitmap/continuation and actual icon argument comparisons; drawing string/line
consumers and runtime resource/cadence callers. Continue uncovered story/event/
field/battle/audio/save/content systems before complete-game requirements.
Full objective active/incomplete; static parser/consumer contracts alone do not
prove complete scanner, layout or full-game fidelity. Original data, message
IDs/control tokens and story attribution/order/branches must remain preserved.

## 2026-10-12 — US unsigned32 scan helper

Recovered __strtoul0x8026eae8,size378. Added detailed SCAN_INTEGER32.md:
callback/output ABI, independent magnitude/count/sign/overflow, entry base/width
gates and unconditional invalid-entry unread, leading whitespace outside field
width, sign and zero/hex-prefix states, base inference, ASCII digit/letter
routing, quotient/product/add overflow checks with sticky flag and wrapped
magnitude, lookahead/width/EOF stops and consumed-count/unread order. Recorded
exact branch-derived partial-prefix/width/overflow examples without claiming
executed results or host/public strtoul equivalence. Linked scan consumers/index.

Validation:1084 instruction anchors and prior tables/constants/font checks
passed read-only ISO. Added complete17-entry state-table and256-byte uppercase
hashes. Fresh280-function reports reproduced byte-for-byte. All430 local behavior
links resolved; git diff --check passed. make test passed core plus36 Python
tests; make all exited0 with no rebuild. No original execution/emulator,
profile/save modification, native code/asset conversion or story rewriting.
ELF/disassembly/reports remain ignored; authored research/evidence tooling only.
Working tree was clean before this increment.

Next: __strtoull, __strtold and mbtowc dependencies, actual icon argument corpus
and executed callback/output/count comparisons; drawing/string layout and
runtime resource/cadence callers. Continue uncovered story/event/field/battle/
audio/save/content systems before complete-game requirements. Full objective
active/incomplete; this local helper does not establish complete scanner/layout
or full-game fidelity. Original message IDs/tokens, attribution/order/branches
and event conditions remain required.

## 2026-10-12 — US unsigned64 scan helper

Recovered __strtoull0x8026e6dc,size40c. Added detailed SCAN_INTEGER64.md:
callback/high-low return ABI, sign/base/prefix/count states, two __div2u threshold
sites, unsigned subtract/borrow comparisons, low/high/cross-word multiply and
carry addition, sticky overflow with wrapped magnitude, width/EOF count and
unread order. Invalid entry forwards incoming r20 (not initialized locally) to
unread, unlike unsigned32's incoming width value. Linked prior division and scan
consumer contracts. Derived64-bit boundary examples remain explicitly static.

Validation:1108 instruction anchors and existing tables/constants/font checks
passed read-only ISO; full17-entry unsigned64 state-table hash added. Fresh
281-function reports reproduced byte-for-byte. All438 local behavior links
resolved; git diff --check passed. make test passed core plus36 Python tests;
make all exited0 with no rebuild. No original execution/emulator, profile/save
modification, native code change, asset conversion or story/message rewriting.
ELFs/disassemblies/reports remain ignored. mbtowc entry was also inspected locally
for the next dependency; its decoder/complete contract remains unrecovered here.
Working tree was clean before this authored research increment.

Next: mbtowc and its length/byte validation helper, __strtold floating scan and
numeric conversion dependencies; executed integer callback/output/count/carry
comparisons and original format/argument corpus. Continue drawing/string layout,
runtime resource/cadence callers and uncovered story/event/field/battle/audio/
save/content systems before complete-game requirements. Full objective remains
active/incomplete; no complete scanner/layout or full-game fidelity claim.
Original data and story attribution/order/branches/conditions remain mandatory.

## 2026-10-12 — US multibyte validation and halfword conversion

Recovered mbtowc0x80269a5c,size120 and is_utf8_complete0x80269b7c,sizeec.
Added detailed MULTIBYTE_CONVERSION.md: null/zero-size routes, complete versus
incomplete/error lengths, one-/two-/three-byte forms, permissive high-bit-only
continuations, exact halfword decode masks (seven bits on final byte), canonical
length test and output/no-write behavior. Recorded absence of four-byte and
surrogate checks. Composed with inspected scan selector7 calls that supply one
byte, ignore failure and use differing c versus s/[ destination strides. These
are static byte/branch deductions, not executed UTF-8 equivalence observations.
Updated scan dependency links and provenance/index.

Validation:1132 instruction anchors and all prior tables/constants/resource
checks passed read-only ISO. Fresh283-function reports reproduced byte-for-byte.
All445 local behavior links resolved; git diff --check passed. make test passed
core plus36 Python tests; make all exited0 with no rebuild. No original execution/
emulator, profile/save modification, native implementation, asset conversion or
story/message rewriting. ELFs/disassemblies/reports remain ignored. Working tree
was clean before this authored research increment.

Next: __strtold floating scanner and conversion dependencies, executed byte/
access/output and integer callback corpus, actual wide-format callers and icon
arguments. Continue drawing/string layout, runtime resource/cadence and uncovered
story/event/field/battle/audio/save/content systems before complete-game
requirements. Full goal active/incomplete; static library contracts alone do not
prove complete scanner/layout or full-game fidelity. Original message IDs/tokens
and story attribution/order/branches/conditions remain mandatory reused data.

## 2026-10-12 — US floating scan entry and special values

Inspected __strtold0x8026d6d0,size100c. Added detailed
SCAN_FLOAT_ENTRY_SPECIAL_VALUES.md for entry, outer gate, leading state and
INF/INFINITY/NAN paths: initial read before width validation, whitespace/sign
state, repeated-sign boolean/count effect, token loops without inner width gate,
payload character/bound/closing rules and successful early returns without
pushback. Recorded special source floats and limits on inferred NaN payload/
FPSCR. Explicitly left all finite decimal/hex states and conversion exits open;
whole-function fingerprint denotes identity, not complete behavior recovery.
Linked scan consumer and evidence index/provenance.

Validation:1156 instruction anchors and prior tables/constants/resource checks
passed read-only ISO, including42-byte zero initializer, two token strings and
two special source words. Fresh284-function reports reproduced byte-for-byte.
All450 local behavior links resolved; git diff --check passed. make test passed
core plus36 Python tests; make all exited0 with no rebuild. No original execution/
emulator, profile/save modification, native implementation, asset conversion or
story rewrite. Inspection ELF/disassembly/report outputs remain ignored; working
tree was clean before this authored research increment.

Next: remaining finite __strtold states, decimal record/digit/exponent accumulation,
hexadecimal mantissa/exponent bit construction, __dec2num and sign/range exits;
executed callbacks/count/status/FPSCR and original icon float argument corpus.
Continue drawing/string layout, runtime resource/cadence and uncovered story/
event/field/battle/audio/save/content before complete-game requirements. Full
objective active/incomplete; no complete floating scanner or full-game fidelity
claim. Original message IDs/control tokens and story attribution/order/branches/
event conditions remain mandatory reused data.

## 2026-10-12 — US finite decimal floating scan

Added detailed SCAN_FLOAT_DECIMAL.md for the already-fingerprinted __strtold:
all decimal mantissa/exponent states,20-digit retention and discarded-digit scale,
leading/trailing zero behavior, decimal-point initializer dependency, exponent
word wrap/sticky status, acceptance/count/pushback and record normalization.
Recovered local __dec2num handoff, pre-status zero/infinity exits and post-call
normal/finite bounds plus acceptance-dependent sign. Exact converter internals
and all hex states remain open; no executed floating accuracy/equivalence claim.
Linked the entry/special contract, index and provenance.

Validation:1180 instruction anchors and prior tables/constants/resource checks
passed read-only ISO, adding initial decimal-point binding and four binary64
range constants. Fresh284-function reports reproduced byte-for-byte. All455
local behavior links resolved; git diff --check passed. make test passed core
plus36 Python tests; make all exited0 with no rebuild. No original execution/
emulator, profile/save modification, native implementation, asset conversion or
story rewriting. Original disassembly/reports remain ignored; working tree was
clean at turn start.

Next: hexadecimal states/bit construction/exits and __dec2num, executed decimal
records/callback/count/status/sign/rounding corpus and actual icon float arguments.
Continue drawing/string layout, runtime resource/cadence and uncovered story/
event/field/battle/audio/save/content before complete-game requirements. Full
goal active/incomplete; this local decimal contract does not establish complete
floating scanner, layout or full-game fidelity. Reuse original data and preserve
message IDs/tokens, attribution/order/branches/event conditions.

## 2026-10-12 — US hexadecimal floating scan

Added detailed SCAN_FLOAT_HEXADECIMAL.md for all local hex __strtold regions:
prefix/init boundary, dual states, leading zeros,15 nibble slots including an
adjacent-stack byte outside returned eight-byte scratch, capacity/truncation,
exponent unread/reread and signed16 wrap with wrong decimal-register range test,
raw-zero versus nonzero exits, logical normalization, exponent bit insertion,
packing-counter count rewrite and range/sign rules. Recorded uninitialized
short-prefix scratch and conditional normalization nontermination deductions;
original denormal/FPSCR/caller execution is explicitly unverified. Linked prior
entry/decimal contracts; __dec2num remains an open dependency.

Validation:1212 instruction anchors and prior tables/constants/resource checks
passed read-only ISO; all22 hex-digit classifications added. Fresh284-function
reports reproduced byte-for-byte. All462 local behavior links resolved; git diff
--check passed. make test passed core plus36 Python tests; make all exited0 with
no rebuild. No original execution/emulator, profile/save modification, native
implementation, asset conversion or story rewrite. Local disassembly/reports
remain ignored; working tree was clean at turn start.

Next: __dec2num and numerical dependencies, executed callback/raw-bit/count/
status/FPSCR comparisons for finite/hex/special cases and actual icon arguments;
then drawing/string layout, resource/cadence callers and uncovered story/event/
field/battle/audio/save/content systems before complete-game requirements.
Full objective active/incomplete; local __strtold recovery is not executable
numeric/layout or full-game equivalence. Original IDs/tokens and story
attribution/order/branches/event conditions remain required reused data.

## 2026-10-12 — US decimal-to-binary entry and special records

Inspected __dec2num0x80265804,size1080 entry through802659e8. Added detailed
DECIMAL_TO_BINARY_SPECIAL_VALUES.md: count/first-byte precedence,nonzero-sign
selection and copysign zero/infinity, N seed/sign,count1 quiet bit,13-byte
payload cap, unchecked digit/lowercase conversion,parity packing,all-zero quiet
bit and direct binary64 reload. Recorded malformed NG can construct infinity
bits despite decoded nonzero payload. Whole-function identity is explicitly
not finite conversion recovery;finite copying/approximation/refinement remains
open. No original execution/NaN-FPSCR observation occurred.

Validation:1236 instruction anchors and prior tables/constants/resource checks
passed read-only ISO; three sign/zero doubles and full256-byte lowercase hash
added. Fresh285-function reports reproduced byte-for-byte. All467 local behavior
links resolved; git diff --check passed. make test passed core plus36 Python
tests; make all exited0 with no rebuild. No original execution/emulator,
profile/save modification,native code,asset conversion or story/message rewrite.
ELF/disassembly/reports remain ignored;working tree was clean at turn start.

Next: finite __dec2num copy/digit preparation,approximation/comparison/refinement/
rounding and boundary/sign exits;executed record/scanner/count/status/FPSCR corpus
and actual icon inputs. Continue drawing/string layout,resource/cadence callers
and uncovered story/event/field/battle/audio/save/content before complete-game
requirements. Full objective active/incomplete;no complete numeric/layout or
full-game fidelity claim. Original data,IDs/tokens and story attribution/order/
branches/event conditions remain mandatory.

## 2026-10-12 — US decimal conversion preparation/approximation

Added detailed DECIMAL_TO_BINARY_APPROXIMATION.md for __dec2num finite
preparation through candidate classification:full private42-byte copy,unchecked
ASCII-to-numeric conversion,leading-digit exponent,1..8-digit chunk choice/
integer accumulation,separate multiply/add and early exit that skips P/Q update,
pow(5,Q)/division then ldexp scaling and raw-bit class selection. Preserved
caller-record immutability and distinction between candidate and final result.
Boundary selection/refinement/final rounding and pow internals remain open;
no floating accuracy or executed equivalence claim. Linked entry contract/index.

Validation:1256 instruction anchors and prior tables/constants/resource checks
passed read-only ISO,adding eight chunk powers and three base/bias constants.
Fresh285-function reports reproduced byte-for-byte. All473 local behavior links
resolved;git diff --check passed. make test passed core plus36 Python tests;
make all exited0 with no rebuild. No original execution/emulator,profile/save
modification,native implementation,asset conversion or story rewriting. Original
disassembly/reports stay ignored;working tree was clean at turn start.

Next: __dec2num boundary records/comparisons,refinement/ties/final sign/return and
pow dependencies;executed private-record/candidate/helper/FPSCR/final-bit corpus
and actual scan inputs. Continue drawing/string layout,resource/cadence callers
and uncovered story/event/field/battle/audio/save/content before complete-game
requirements. Full objective active/incomplete;initial approximation is not
complete numeric/layout or full-game fidelity. Original IDs/tokens and story
attribution/order/branches/event conditions remain mandatory reused data.

## 2026-10-12 — US decimal converter infinity boundary

Added DECIMAL_TO_BINARY_INFINITY_BOUNDARY.md for __dec2num infinity-candidate
branch and shared sign exit:original36-digit/exponent308 record preparation,
inactive source-tail rounding/parity/carry path,strict first-digit/exponent/
common-prefix/remaining-nonzero comparison,infinity retention versus maximum-
finite substitution and continued refinement. Equality does not return infinity
here. Preserved seed as original reused data and distinguished local boundary
from measured/correct-rounded universal overflow behavior. General refinement/
ties and pow remain open. No original execution/FPSCR observation occurred.

Validation:1280 instruction anchors and prior tables/constants/resource checks
passed read-only ISO;complete boundary seed hash/NUL and maximum-finite fallback
checked. Fresh285-function reports reproduced byte-for-byte. All477 local
behavior links resolved;git diff --check passed. make test passed core plus36
Python tests;make all exited0 with no rebuild. No emulator/profile/save
modification,native implementation,asset conversion or message/story rewrite.
Original disassembly/reports remain ignored;working tree clean at turn start.

Next: general candidate decimalization/equality,refinement direction,neighbor
construction,difference/tie rounding and final selection;pow/numeric dependencies
and executed record/candidate/boundary/FPSCR corpus. Continue drawing/string
layout,resource/cadence callers and uncovered story/event/field/battle/audio/
save/content before complete-game requirements. Full objective active/incomplete;
no complete numerical/layout or full-game fidelity claim. Original IDs/tokens
and story attribution/order/branches/event conditions remain mandatory.

## 2026-10-12 — US decimal equality and refinement admission

Recovered complete __equals_dec0x802672d8,size10c;added detailed
DECIMAL_RECORD_EQUALITY_AND_REFINEMENT_ENTRY.md. Covers first-byte zero shortcuts,
sign-ignored exponent/digit equality and trailing-zero remainder; __dec2num
candidate decimalization through already-recovered __num2dec_internal,equality
fast exit,inlined initial magnitude direction and raw-word first upward neighbor/
infinity early exit. Later up/down search,differences/ties/final selection remain
open;no complete numeric or original execution equivalence claim.

Validation:1304 instruction anchors and existing tables/constants/resource
checks passed read-only ISO. Fresh286-function reports reproduced byte-for-byte.
All482 local behavior links resolved;git diff --check passed. make test passed
core plus36 Python tests;make all exited0 with no rebuild. No original execution/
emulator,profile/save modification,native implementation,asset conversion or
story/message rewrite. Inspection ELF/disassembly/reports remain ignored;working
tree was clean at turn start.

Next: remainder of upward/downward neighbor search,operand-specific comparisons,
difference helpers,tie/parity rounding and final selection;pow dependencies and
executed generated-record/neighbor/FPSCR corpus. Continue drawing/string layout,
resource/cadence and uncovered story/event/field/battle/audio/save/content
systems before complete-game requirements. Full objective active/incomplete;
local equality/admission does not establish complete numerical/layout or full-
game fidelity. Original data,IDs/tokens and story attribution/order/branches/
event conditions remain mandatory.

## 2026-10-12 — US decimal neighbor refinement and ties

Added DECIMAL_NEIGHBOR_REFINEMENT_AND_TIES.md for remaining __dec2num local
regions:up/down encoding steps,full42-byte record transfers,operand-specific
loop comparison,direct infinity exits,four __minus_dec requests,record equality
and low-bit parity ties,asymmetric lower/upper distance selections and shared
sign exit. All containing __dec2num local flow is now documented across its
contracts; __minus_dec/pow remain open dependencies and no exact-distance or
complete correct-rounding/execution claim is made. Updated prior entry link.

Validation:1336 instruction anchors and prior tables/constants/resource checks
passed read-only ISO. Fresh286-function reports reproduced byte-for-byte. All489
local behavior links resolved;git diff --check passed. make test passed core
plus36 Python tests;make all exited0 with no rebuild. No original execution/
emulator,profile/save modification,native implementation,asset conversion or
story/message rewrite. Original disassembly/reports remain ignored;working tree
was clean at turn start.

Next: __minus_dec numeric subtraction and pow dependencies;executed generated/
difference records,neighbor bits/iterations,tie/FPSCR/final output at boundaries
and actual scanner inputs. Continue drawing/string layout,resource/cadence
callers and uncovered story/event/field/battle/audio/save/content before complete-
game requirements. Full objective active/incomplete;local converter control
recovery does not prove complete numeric/layout or full-game fidelity. Original
data/IDs/tokens and story attribution/order/branches/event conditions remain
mandatory.

## 2026-10-12 — US decimal record subtraction

Recovered complete __minus_dec0x80266ddc,size4fc. Added detailed
DECIMAL_RECORD_SUBTRACTION.md:interleaved42-byte copy/aliasing,zero-right shortcut,
signed alignment target and36 cap,padding/reverse cursors,borrow chain,omitted
right-tail correction and below-five versus exact-five target pointer distinction,
unbounded leading normalization and bounded trailing trim. Linked converter
requests and distinguished literal local flow from exact-distance/correct-rounding
claims. No original execution/caller invariant or zero-result behavior measured.

Validation:1360 instruction anchors and prior tables/constants/resource checks
passed read-only ISO. Fresh287-function reports reproduced byte-for-byte. All496
local behavior links resolved;git diff --check passed. make test passed core
plus36 Python tests;make all exited0 with no rebuild. No original execution/
emulator,profile/save modification,native implementation,asset conversion or
story rewrite. Original ELF/disassembly/reports remain ignored;working tree was
clean at turn start.

Next: pow/numeric dependencies and composed execution validation for record
alignment,borrow/omitted tails,zero/alias cases,neighbor distances/ties/FPSCR/final
bits and actual scanner inputs. Continue drawing/string layout,resource/cadence
callers and uncovered story/event/field/battle/audio/save/content before complete-
game requirements. Full goal active/incomplete;static subtraction/converter flow
is not complete numerical/layout or full-game fidelity. Original data/IDs/tokens
and story attribution/order/branches/event conditions remain mandatory.

## 2026-10-12 — US pow entry and special cases

Inspected pow0x80272620,size20 and __ieee754_pow0x8026fe9c,size830 entry
through8027015c. Added POWER_ENTRY_AND_SPECIAL_CASES.md:direct wrapper delegation,
y-zero priority,arithmetic NaN propagation,negative-base integer/parity word
classification,exact y shortcuts,zero/one/infinity base requests and general
noninteger-negative-base errno33/NaN gate. Documented decimal base-five caller
intersection without claiming larger-power accuracy. Finite/log/exp/range core
remains open despite containing-body identity. No original execution/FPSCR study.

Validation:1384 instruction anchors and prior tables/constants/resource checks
passed read-only ISO,including three power special doubles. Fresh289-function
reports reproduced byte-for-byte. All498 local behavior links resolved;git diff
--check passed. make test passed core plus36 Python tests;make all exited0 with
no rebuild. No original execution/emulator,profile/save modification,native
implementation,asset conversion or story/message rewrite. ELFs/disassembly/
reports remain ignored;working tree was clean at turn start.

Next: remaining power large-exponent/logarithm/exponential/range/sign core,
coefficient bits and helper dependencies;executed pow(5,Q),decimal/scanner/
record-distance/FPSCR corpus. Continue drawing/string layout,resource/cadence
callers and uncovered story/event/field/battle/audio/save/content before complete-
game requirements. Full objective active/incomplete;local special-case recovery
is not full power/numeric/layout or complete-game fidelity. Original data/IDs/
tokens and story attribution/order/branches/event conditions remain required.

## 2026-10-12 — US power large-exponent path

Added POWER_LARGE_EXPONENT_PATH.md for __ieee754_pow8027015c..80270268:
ordered high-word admission/return gates,extreme block's AX-versus43f00000
literal,positive zero/infinity requests,original signed-base reload and exact
fused near-one polynomial/split high-low handoff. Recorded six coefficient bits
and derived scanner base-five exponent bound that excludes this branch. General
logarithm/common exp/range/sign tail remains open;no numerical accuracy or
original execution/FPSCR claim.

Validation:1404 instruction anchors and prior tables/constants/resource checks
passed read-only ISO,including full48-byte/six-coefficient hash. Fresh289-function
reports reproduced byte-for-byte. All501 local behavior links resolved;git diff
--check passed. make test passed core plus36 Python tests;make all exited0 with
no rebuild. No original execution/emulator,profile/save modification,native
implementation,asset conversion or story/message rewrite. Original disassembly/
reports remain ignored;working tree was clean at turn start.

Next: general logarithm normalization/coefficient path and common exponentiation/
range/sign tail,then helper dependencies and executed base-five/general power/
FSPCR/numeric/scanner corpus. Continue drawing/string layout,resource/cadence
callers and uncovered story/event/field/battle/audio/save/content before complete-
game requirements. Full objective active/incomplete;local branch recovery does
not establish full power/numerical/layout or complete-game fidelity. Original
IDs/tokens and story attribution/order/branches/event conditions remain required.

## 2026-10-12 — US power general logarithm

Added POWER_GENERAL_LOGARITHM.md for __ieee754_pow80270268..80270450:
absolute/subnormal scaling,high-word normalization/reference thresholds,
synthetic denominator words,ratio compensation,fused Horner polynomial,
product split and final corrected logarithm high/low pair. Recorded six table
entries,eleven scale/coefficient words and signed conversion bias;preserved
separate/fused operations and integer low-word clears. Common exponentiation/
range/sign tail remains open;no numerical accuracy or original execution claim.

Validation:1428 instruction anchors and prior tables/constants/resource checks
passed read-only ISO,with two complete table/coefficient hashes and signed bias.
Fresh289-function reports reproduced byte-for-byte. All505 local behavior links
resolved;git diff --check passed. make test passed core plus36 Python tests;
make all exited0 with no rebuild. No original execution/emulator,profile/save
modification,native implementation,asset conversion or story/message rewrite.
Original disassembly/reports remain ignored;working tree clean at turn start.

Next: common y/log multiplication,overflow/underflow gates,exponential polynomial,
exponent reconstruction/sign and dependencies;executed base-five/general power/
FPSCR and composed scanner/converter comparisons. Continue drawing/string/
resource/cadence and uncovered story/event/field/battle/audio/save/content
before complete-game requirements. Full objective active/incomplete;local log
recovery is not full power/numerical/layout or full-game fidelity. Original data/
IDs/tokens and story attribution/order/branches/event conditions remain required.

## 2026-10-12 — US power exponentiation/range tail

Added POWER_EXPONENTIATION_AND_RANGE.md for __ieee754_pow80270450..80270694:
result sign,split y/log product,raw/exact overflow and underflow gates,arithmetic
huge/tiny requests,integer bit reduction,fused natural-exponent/polynomial,
exponent insertion versus ldexp and final sign multiplication. Pinned thirteen
constant bits and preserved low-word split/fused boundaries. Local pow/helper
body now documented across contracts;sqrt and composed execution remain open.
No accuracy/correct-rounding/original FPSCR claim. Linked general-log tail.

Validation:1460 instruction anchors and prior tables/constants/resource checks
passed read-only ISO,including full104-byte/thirteen-constant hash. Fresh289-
function reports reproduced byte-for-byte. All510 local behavior links resolved;
git diff --check passed. make test passed core plus36 Python tests;make all
exited0 with no rebuild. No original execution/emulator,profile/save modification,
native implementation,asset conversion or story/message rewrite. Disassembly/
reports remain ignored;working tree was clean at turn start.

Next: sqrt dependency and executed base-five/general power/FPSCR/returned-bit
corpus,then composed scanner/converter/distance validation. Continue drawing/
string/resource/cadence callers and uncovered story/event/field/battle/audio/
save/content before complete-game requirements. Full objective active/incomplete;
local power recovery is not complete numerical/layout or full-game fidelity.
Original data/IDs/tokens and story attribution/order/branches/event conditions
remain mandatory.

## 2026-10-12 — US square-root numeric service

Recovered sqrt0x802728d0,size20 and complete __ieee754_sqrt0x80272640,size224.
Added SQUARE_ROOT_NUMERIC_SERVICE.md:wrapper,special exponent fused x*x+x and
errno33 (including infinities),signed-zero preservation,negative finite NaN,
subnormal word shifts,exponent parity,both integer digit-extraction loops,
carry/borrow,remainder correction without floating rounding-mode probe and
final binary64 assembly. Linked power helper dependency. No numerical accuracy,
original FPSCR/error observation or composed equivalence claim.

Validation:1484 instruction anchors and prior tables/constants/resource checks
passed read-only ISO;remainder-path+1 double checked. Fresh291-function reports
reproduced byte-for-byte. All513 local behavior links resolved;git diff --check
passed. make test passed core plus36 Python tests;make all exited0 with no rebuild.
No original execution/emulator,profile/save modification,native implementation,
asset conversion or story/message rewrite. Original ELF/disassembly/reports
remain ignored;working tree was clean at turn start.

Next: executed standalone sqrt/power/scanner/converter/record-distance corpus
with actual FPSCR/errno/final bits and caller input invariants;continue original
drawing/string layout and resource/cadence callers plus uncovered story/event/
field/battle/audio/save/content before complete-game requirements. Full objective
active/incomplete;closed local numeric call-chain documentation does not prove
execution accuracy,complete layout or full-game fidelity. Original data/IDs/
tokens and story attribution/order/branches/event conditions remain mandatory.

## 2026-10-12 — US string pitch drawing

Recovered FontDrawStringVecPitch0x80077034,size334,FontDrawStringPitch
0x80077368,size68 and FontDrawString0x800773d0,size68. Added
FONT_STRING_PITCH_DRAWING.md:vector/scalar ABI and matrix setup,lead/byte/tag
routing without LF line branch,first-WID1 width,proportional versus fixed-cell
translation/advance,ordered comparisons,outline/raw-effective-color restoration
and two/ten known inner RNG draws per glyph. Preserved distinctions from message
width/parser and caller-vector/current scale behavior. No original rendering,
RNG seed/cadence or layout/image equivalence measured.

Validation:1508 instruction anchors and prior tables/constants/resource checks
passed read-only ISO;two zero-vector templates,half/default pitch checked.
Fresh294-function reports reproduced byte-for-byte. All523 local behavior links
resolved;git diff --check passed. make test passed core plus36 Python tests;
make all exited0 with no rebuild. No original execution/emulator,profile/save
modification,native implementation,asset conversion or story/message rewrite.
ELFs/disassembly/reports remain ignored;working tree clean at turn start.

Next: string matrix/center/shake and message-draw callers,matrix service contracts,
resource/cadence and executed positions/images/RNG;composed numerical validation
remains needed. Continue uncovered story/event/field/battle/audio/save/content
before complete-game requirements. Full objective active/incomplete;local string
wrapper coverage is not complete text/layout or full-game fidelity. Original
IDs/tokens and story attribution/order/branches/event conditions remain required.

## 2026-10-12 — US affine matrix drawing services

Recovered complete PSMTXScale0x8029e2b0,size28,PSMTXTransApply0x8029e264,
size4c and PSMTXConcat0x8029dd64,sizecc. Added AFFINE_MATRIX_SERVICES.md:
row-major ABI,paired-single lane/arithmetic interpretation,scale write order,
translation frsp/add and late reads,exact in-place versus partial overlap,
concatenation fused order/all-input capture/output pair order and Unit01 special-
value consequences. Linked pitch caller. Public Dolphin interpreter is explicitly
reference evidence,not original execution;no implementation copied or linked.
GQR0/FPSCR and actual returned matrix/raster behavior remain unmeasured.

Validation:1532 instruction anchors and prior tables/constants/resource checks
passed read-only ISO,including matrix positive zero and Unit01 pair. Fresh297-
function reports reproduced byte-for-byte. All527 local behavior links resolved;
git diff --check passed. make test passed core plus36 Python tests;make all
exited0 with no rebuild. No original execution/emulator,profile/save modification,
native implementation,asset conversion or story/message rewrite. Original ELF/
disassembly/reports remain ignored;working tree clean at turn start.

Next: string matrix/center/shake/message callers,remaining camera/GX/vector
services and executed matrix/GQR0/FPSCR/positions/images/RNG corpus;composed
numerical comparisons remain needed. Continue uncovered story/event/field/
battle/audio/save/content before complete-game requirements. Full objective
active/incomplete;three helper contracts do not establish full rendering/game
fidelity. Original data/IDs/tokens and story attribution/order/branches/event
conditions remain required.

## 2026-10-12 — US completion assessment and shipped-usage priorities

Evaluated actual documentation versus full-game closure;added
US_COMPLETION_AND_PRIORITIES.md and reprioritized the root/index references to
US GameCube first,Switch comparisons separate. Existing85 behavior documents
(before assessment) include32 numeric/format/scan documents;297 fingerprints
and1532 anchors do not imply gameplay completion. All16 system closure gates
remain open;no defensible overall percentage or matched gameplay equivalence.
Corrected stale message/render coverage labels and51-function baseline count.

Added read-only us_usage_audit.py scanning all DOL text direct branches,
initialized-data address candidates and all29 shipped REL module0 imports.
Report identifies7061 unique annotated text starts,4187 direct-call referenced,
1471 data candidates,1608 REL imported and6229 with any audited reference.
Classes overlap;832 without references are unresolved,not proven dead. Tool
also decodes20 registered attack-command callback entries from80364348;none
of their main bodies is currently fingerprinted. Registration/REL references
are static leads,not demonstrated branch selection or execution frequency.

Priority now opening/Crump original event/message/state graph,selected attack
command contracts and defense impact timing,main/input/RNG ownership,then field/
partners,rewards/progression/inventory/save,battle/audience and required camera/
animation/audio,followed by full chapter/optional-content coverage. General
library edge branches require demonstrated shipped input/observable dependency
before further recovery. Full scope preserved;no dead-code exclusion inferred.

Validation:original DOL hash matches prior baseline;two complete usage reports
reproduced byte-for-byte;three synthetic tests distinguish call/tail/data/REL
references,assert ISO immutability and reject revision/sentinel errors.
make test passed core plus39 Python tests;make all exited0,no rebuild. Existing
1532 instruction/data/resource checks passed;532 behavior links resolved;
git diff --check passed. No original gameplay execution/emulator/profile/save
modification,native implementation,asset conversion or story/dialogue rewrite.
Metadata reports remain ignored;only audit tooling/tests/authored notes committed.

Next: generalize original REL event binding inventory beyond Rogueport init;
resolve opening/Crump entry/callbacks/unit/weapon/SetupAC operands and exact US
message/speaker/conditions,then recover selected attack commands and compare
isolated original timing/state traces. Expand through all US playable/optional
content before deriving complete-game requirements. Full objective active and
incomplete;assessment and static reference counts do not satisfy closure.

## 2026-10-12 — Correct GameCube paired-single disassembly

Fixed the requested LLVM mislabels using independent Gekko field decoder
(tools/gekko_disasm.py),integrated into inspect_dol_function.py. Existing LLVM
CPU750/vector-feature options did not resolve the mislabels. Decoder handles
paired arithmetic/comparison/move/merge,quantized immediate/indexed/update
load/store and dcbz_l,proper D/A/C/B syntax,Rc,W/I and signed12 displacement.
Reserved/unrecognized paired-space encodings emit .long,not modern operations.
Added standalone check/repair CLI preserving original prefixes/comments/endings.
Tooling/tests incremental commit:e4b9816.

Repaired354 instructions in47 ignored original listings after scanning388
PowerPC listings (61863 instruction rows),including annotated/text copies.
Every changed instruction's address/bytes/prefix/endings were preserved;all
unrelated lines unchanged. All386 inspection ELF hashes unchanged. Repair
report remains ignored. Documented usage/scope/reference/licensing in
GEKKO_DISASSEMBLY.md;updated prior affine-matrix mislabel warning.

Validation:ten regression tests cover all paired arithmetic families,record/
comparison/move/merge/cache forms,quantized W/I and displacement bounds,indexed/
update variants,reserved encodings,known mislabels,comments/CRLF/no-final-newline,
idempotence,read-only check and binary rejection. make test passed core plus49
Python tests;make all exited0,no rebuild. Second full listing check:zero needed
changes,zero unrecognized paired encodings. Fresh PSMTXScale/TransApply/Concat
inspection reproduced all corrected instruction rows. Existing1532 instruction/
data/resource checks passed;533 behavior links resolved;git diff --check passed.
No original execution/emulator,ISO write,profile/save change,native gameplay
change,asset conversion or story/message rewrite. This fixes inspection text,
not a claim of executed numerical or gameplay equivalence.

Next:continue US opening/Crump shipped event/callback/unit/weapon/SetupAC binding
inventory and selected attack mechanics/original timing comparisons per current
priority register. Use corrected listings for subsequent binary analysis. Full
behavior/complete-game requirements objective remains active/incomplete.

## 2026-10-12 — US ordinary attack bindings and TimingA mechanic

Added BATTLE_ORDINARY_ATTACK_BINDINGS.md and BATTLE_TIMING_A_COMMAND.md.
Original DOL Jump stream (268 commands) selects ID1 at80355ebc/80356038;
Hammer-core stream (387 commands) selects ID3 at8035d994/8035dc28. Checked
complete stream hashes and exact setup records. Complete SetupAC callback
resolves operands0/1/3 but passes operand2 raw. New read-only DOL event inventory
retains raw operands/annotations in ignored reports;no branch execution claim.

Recovered complete TimingA main/result/window helpers and two general pad
record-trigger wrappers. Documented initialization,state99/100 waiting,1000
same-call countdown decrement and1002 callback cleanup,seven difficulty tables,
count-dependent bit1 selection,ordered forced-result/RNG routes and ordinary
A-trigger/mash/window checks. Distinguishes general battle history+0f44 from
private AC history+1d34. Ordinary checks count A edges0..19,reject>=2 then search
0..W-1. No measured impact alignment or Crump/tutorial equivalence is claimed.
ID3 Hammer main is inspected locally but its contract remains next work.

Validation:1564 instruction anchors and prior checks passed read-only ISO,
including seven tables/pointers,registration,two floats,two event hashes/four
setup sites. Fresh303-function and two-event reports repeated byte-for-byte.
make test passed core plus52 Python tests;make all exited0,no rebuild.
Gekko listing check clean;git diff --check passed. Three synthetic event-parser
tests cover raw operands/annotations,truncation and dispatch-domain rejection.
No original gameplay execution/emulator,ISO/profile/save mutation,native gameplay
change,asset conversion or dialogue/story rewrite;all original artifacts ignored.

Next:normal Hammer ID3 lifecycle/results;trace Jump SetParam/flags/difficulty/
state/result and general pad update callers,unit/weapon selectors and surrounding
script control flow. Resolve opening/Crump REL/tutorial entry and overrides,
then compare isolated original impact/input/cadence/state traces. Continue full
US playable/optional systems/content before complete-game requirements. Full
objective remains active/incomplete;local command recovery is not full combat
or scene equivalence.

## 2026-10-12 — US ordinary Hammer command and callback protocol

Added BATTLE_HAMMER_STICK_KEEP_LEFT.md and BATTLE_ACTION_COMMAND_CALLBACKS.md.
Recovered complete ID3 main/result/delete,held-pad wrappers,eight-parameter/raw-
flag setters,start and result wrappers/services. Documented suppression/automatic
captures,initialization,120-call wait,held-left charge,negative-countdown stage
advance,sound handle requests,release/stage/difficulty rules,forced routes,
same-call redispatch,1004 external wait and deferred finish cleanup. General
held-left mask40000 is distinct from private AC trigger history. Seven release
allowances80/70/60/40/20/10/5 verified;actual eligible span also depends on period.

Original Hammer-core sites set mode0,N4,period25/30/30/40,zero suppression and
three trailing sentinel operands. Resolved parameter sentinel preserves prior
field;flag setter takes raw whole word. Result event polls stored pending bit,
discards callback register return and blocks until main clears it. No animation
impact or tutorial equivalence measured;branch selection and force writers open.
Main normal paths do not locally increment result count. Finish state1005 sets
60 without decrement,unlike TimingA;1003's local busy loop is not a frame wait
or sound-stop request. Updated registered-command coverage to two main bodies.

Validation:1612 instruction anchors and all prior checks passed read-only ISO;
new registration,seven allowances,two floats/two sound keys,eight parameter/two
flag records checked. Fresh314-function reports repeated byte-for-byte. All553
behavior links resolved;Gekko disassembly check clean;git diff --check passed.
make test passed core plus52 Python tests;make all exited0,no rebuild. No original
execution/emulator,ISO/profile/save mutation,native gameplay changes,asset
conversion or story/message rewrite. Original listings/reports remain ignored.

Next:full ordinary attack event branches and unit/weapon/difficulty/output/
automatic/forced/suppression writers;opening/Crump REL/tutorial selection and
conditions,then original impact/input/state/cadence traces. Presentation/sound
services remain separate. Continue all US gameplay/optional-content systems
before complete-game requirements. Objective remains active/incomplete;two
command contracts do not establish full combat/story/game fidelity.

## 2026-10-12 — US Hammer weapon branches and used integer VM rules

Added BATTLE_HAMMER_WEAPON_BRANCHES.md and EVENT_INTEGER_BRANCHES.md.
Resolved actual US weapon-pointer cases:default Hammer period25,ConfuseHammer/
TsuranukiNaguri annotations period30,other weapons entering this core period40.
All three identified weapon records have unsigned action-level byte3 and checked
wrapper event references. Wrappers set local14=0/1 and run the shared core child.
Mask2 separates GetFirstAttackTarget versus selected-command paths and immediate
parent versus delayed inline-region setup;complete flag lifetime/inline cadence
remains open. Complete weapon-level,difficulty and raw-mask boolean callbacks
recovered. Existing difficulty modifier/clamp is connected to actual weapon data.

Recovered used integer IF equal/not-equal and case-equal bodies plus switch/
default/pop dispatcher regions. Records variable resolution order,branch scanner
nesting/pointer destinations,snapshot value,matched-case state0 preventing
fallthrough/default,and literal opcode1/34/35 scanner distinctions. Scoped to
supported Hammer blocks;not claimed as a complete VM or event graph interpreter.
No original execution/timing/story/tutorial equivalence observed.

Validation:1652 instruction anchors and all prior checks passed read-only ISO,
including eight VM targets,condition/case operands,three weapon difficulty/event
links and two wrapper streams. Fresh320-function and wrapper inventories repeated
byte-for-byte. All561 behavior links resolved;Gekko listing check clean;
git diff --check passed. make test passed core and battle_timing_a host tests plus
52 Python tests;make all passed. Concurrent native TimingA work was present
(commit9811fe0 plus ongoing game/header/test edits);research commit stages only
explicit research/docs paths and preserves that work. No original emulator/
ISO/profile/save mutation,asset conversion or dialogue/story rewrite by this
research step;all original inspection artifacts remain ignored.

Next:remaining Hammer parent/inline waits and damage/result/stylish/stop branches,
menu/unit/weapon consumers,mask2/automatic/forced writers,and opening/Crump
REL/tutorial binding. Obtain original input/state/animation cadence traces;
continue all reachable US gameplay/optional content before final complete-game
requirements. Full objective remains active/incomplete.

## 2026-10-12 — Original US harbor opening story bindings

Added STORY_HARBOR_OPENING_BINDINGS.md and generic read-only REL event inventory.
Reads shipped gor.rel directly from ISO without extracted module writes;bounded
section/import/ADDR32/event parsing preserves raw operands,self/module0 targets,
foreign module identities and BSS/one-past identities without dereferencing them
as file data. External names remain annotations. Six synthetic tests cover local/
DOL/foreign/BSS/one-past binding,invalid bounds/streams,duplicates and input
immutability. Original records remain ignored.

Recovered seven original harbor event spans (918 commands),story operand
f5de0180 case2..5 resume entry requests,writes3/4/5/6,actor initialization and
setup coordinates/flags,20 exact mac_0_014..033 message calls with original
speaker-key offsets,and beforebattle set-info literal2/start/wait boundary.
No dialogue paraphrase/transcript written;fresh US resource census retains full
payload/token hashes/offsets and confirms all20 IDs uniquely exist in gor_00.
Battle configuration operand semantics,unit/tutorial overrides,full conditions/
inline scheduling/input/camera/audio and original scene execution remain open.

Validation:gor REL and seven whole event spans plus US gor_00 message resource
hashes verified directly in ISO. Two complete event reports byte-identical;
fresh message census260 resources/13186 pairs;20 target IDs each match once.
Prior1652 instruction/data checks pass;563 behavior links resolve;Gekko listing
check clean;git diff --check passed. Initial make test hit transient concurrent
native Input/core_tests Hammer interface mismatch (hammerHeld/hammerReleased);
research tools separately passed. After concurrent updates settled,make test
passed core,battle_timing_a,battle_hammer plus58 Python tests;make all passed
with no rebuild. Separate native/README/documentation-request edits preserved;
research commit stages only explicit research/docs paths.

No original gameplay execution/emulator/profile/save mutation,ISO write,new
asset conversion,original code linking/distribution or story rewrite by this
step. Full objective active/incomplete. Next:recover evt_npc_set_battle_info/
battle_start/wait_battle_end and actual gor battle configuration2/unit/tutorial
script bindings;close scene conditions,locks/persistent namespace and original
timing comparisons,then expand all reachable US chapters/optional mechanics
before complete-game requirements.

## 2026-10-12 — Harbor configuration and field/battle transition contracts

Added FIELD_BATTLE_ENTRY_AND_RETURN.md and HARBOR_TUTORIAL_CONFIGURATION.md.
Recovered ten complete DOL bodies:three NPC event callbacks,NPC set/setup,
field-battle pointer/result accessors and area lookup/registration. Documents
44-byte numeric setup indexing,current area prefix,200-byte information clear/
associated-NPC copy,audience/weighted group/item draws,encoded saved-switch
selection,first-call battle transition and sequence2/nonzero-result return gate.
Original start callback reads a second operand despite this scene supplying one;
next command header1005b is resolved then ignored by field pointer getter.
No invented extra scene argument or victory-only interpretation.

Verified gor prolog HA/LO/call relocations install area gor's setup/number tables;
literal index2 selects REL60eb8. Its single weight50 group60c90 selects one
unit setup60c60,kind unit_boss_kanbu1 at645c8,init_event64808 and original stage/
music/data bindings. Unit's DOL item table has eight zero bytes. Known local
setup chain consumes17 irand calls (16 audience plus one group,zero item draws),
not a full encounter seed/cadence claim. Raw kind halfword+8=5 remains a data
observation until runtime stat/init consumers establish HP/override behavior.

Validation:1700 instruction anchors and prior checks pass read-only ISO;
seven whole configuration/prolog spans,setup/group/kind chain,prolog partial-
address/call relocations and zero item table checked. Fresh330-function evidence
reports repeat byte-for-byte;571 behavior links resolve;Gekko listing check
clean;git diff --check passed. make test passed core,battle_timing_a,battle_hammer,
battle_defense plus58 Python tests;make all exited0,no rebuild. Concurrent native/
README/documentation-request changes preserved;only explicit research paths
staged. No original execution/emulator,ISO/profile/save mutation,asset conversion,
native implementation or story/dialogue rewrite by this step. Artifacts ignored.

Next:original gor unit_boss_kanbu1 init_event/data_table/parts and stat-entry
consumers,tutorial command/forced-result/availability/reward/result writers,
sequence/Mario/party/audio lifetime and full original scene/input/RNG traces.
Continue all reachable US systems/optional content before complete-game
requirements. Full objective remains active/incomplete;config pointers/local
transition contracts do not establish faithful completed tutorial or full game.

## 2026-10-12 — Harbor boss tutorial event and feedback contracts

Added HARBOR_BOSS_TUTORIAL_EVENTS.md and BATTLE_TUTORIAL_STATE_SERVICES.md.
Original boss init installs wait/attack/damage/confusion/phase hooks,sets battle
flag1000,spawns the helper actor and stores its unit ID in work0,zeros feedback
latches1/2/3 and configures movement sounds. Seven original hook/event spans
(18/84/5/11/3/75/95 commands) hash-checked;phase/death tutorial message guards
and actor IDs recovered. Exact six US mac_0_b_t1_000..005 IDs preserved;no
paraphrase/transcript. Half-HP lesson003 deletes phase event before success001/
failure002,which have independent once-latches and can both execute otherwise.
Initial lesson000 gated by turn1/exact phase04000001;feedback phase04000003.

Recovered complete unit-work read/write,exact CheckPhase bank4 comparison,turn
halfword read and resolved operation/raw-mask flag update. Two60-byte original
REL helpers read battle-wide unsigned bytes16f58/16f59 through checked HA/LO
and evtSetValue REL24 bindings,not the current move result. Inspection ELFs use
file offsets and label raw branch targets unrelocated. Boss data key49 points
to dead_event;dummy key48 and enemy defense gate+13=1/default power callback
verified. Local superguard gate pass does not establish global tutorial policy.
Attack/death hooks and reward/message/deletion requests documented with open
service/condition boundaries;no runtime stats/defeat equivalence claim.

Validation:1724 instruction anchors and all prior resource/configuration checks
passed read-only ISO. Fresh335-function and six-event reports repeated byte-for-
byte;all six tutorial keys uniquely match original US payload metadata;578
behavior links resolved;git diff --check passed. Initial make test hit a transient
concurrent missing tests/battle_damage_tests.cpp target;after file creation,
make test passed core,TimingA,Hammer,defense,damage host tests plus58 Python
tests;make all exited0,no rebuild. Separate native damage/CMake edits preserved,
only explicit research paths staged. No original gameplay execution/emulator,
ISO/profile/save writes,native implementation,asset conversion or story rewrite
by this step. Original inspections/reports remain ignored.

Next:feedback byte writers/reset,battle flag1000 consumers,unit allocation/stat-
copy/SpawnUnit/part state and full normal attack/defense/forced/reward/result
paths;original tutorial input/state/animation/audio traces and chapter coverage.
Continue all reachable US mechanics/optional content before complete-game
requirements. Full objective active/incomplete;installed tutorial graph is not
an executed faithful completed encounter or full game.

## 2026-10-12 — Tutorial feedback writers/reset and stage-fall gate

Added BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md and linked prior tutorial
contracts. Recovered complete event/inner declaration,stage-fall registration
check and BattleInit. Declaration resolves weapon/result,exact-1 increments
failure16f59;every other value increments success16f58. Counts stop incrementing
at200;last-classification19074 updated independently. No actor filter/current-move
reader or local first-call gate. Four actual Jump/Hammer declaration sites pinned;
failure branches pass-1. Inner success stores weapon,quarter*grade+half fused
multiplier and original weapon bytes before audience-good request;bad path
requests audience-bad. Audience effects/other count resets remain open.

BattleInit allocates/publishes/clears19098-byte work,covering feedback counters,
then performs ordered initializer/sequence/field stores including clearing battle
flags before unit/party/stage/audience/break-slot init. One proven battle1000
consumer returns0 without registering stage-fall event or clearing old handle;
clear-bit route registers script803771bc,priority10 and stores returned event
word15c at work182c8. Same-mask GP/audience flags are not this battle flag.
This does not establish command-menu or guard restrictions. Direct displacement
scan was a discovery aid,not complete bulk/indexed/REL writer/consumer coverage.

Validation:1772 instruction anchors and prior checks passed read-only ISO,
including original quarter/half/signed bias and four declaration records.
Fresh339-function reports repeated byte-for-byte;584 behavior links resolved;
Gekko listing check clean;git diff --check passed. make test passed core,TimingA,
Hammer,defense,damage plus58 Python tests;make all exited0,no rebuild. Separate
native work preserved;working tree at validation contained only research edits.
No original gameplay execution/emulator,ISO/profile/save writes,native
implementation,asset conversion or story/dialogue rewrite. Artifacts ignored.

Next:unit allocation/stat-copy/part/SpawnUnit and actual boss attack/HP/defeat/
reward consumers;audience good/bad services,counter lifetimes and remaining
battle1000 gates,then original tutorial input/state/RNG/audio/animation traces.
Continue all reachable US systems and optional content before final complete-
game requirements. Full objective active/incomplete;local writer/gate closure
does not prove a faithful complete encounter or full game.

## 2026-10-12 — Original unit HP/FP entry and tutorial spawn ABI

Added UNIT_ENTRY_STATS_AND_SPAWN.md. Recovered the BtlUnit_Entry slot/allocation/
publication,HP/FP copy and init-registration regions,plus complete spawn helper/
callback and current/max HP callbacks. Entry publishes the0xb34-byte unit before
later initialization;kind signed HP+8 copies to runtime10a/108/10c,FP+a to110/
10e/112. Harbor kind5/0 now connected to original HP5/FP0 writes. Getter maximum
108/current10c and original signed DIV establish half-HP threshold2 when max5
is unmodified. This is static source/data behavior,not measured tutorial HP or
proof of no later overrides;rest of large entry/parts remains explicitly partial.

Getters translate actor-1 to completion without destination write. Spawn callback
resolves setup but passes flags raw,writes-1 on null or integer unit ID on success.
Flags2/1 insert forward/backward into a separate64-word ordering array;flag2
wins and flags0 does no extra insertion. Tutorial helper uses flags0,retains
spawned ID as message speaker work0. Entry init event registration priority10/
flags0 sets event owner160 and stores handle288;does not synchronously execute
whole init script. Fingerprinting containing body is not complete entry coverage.

Validation:1804 instruction anchors and prior checks passed read-only ISO;
original kind HP5/FP0 and helper/GetMaxHp/GetHp/DIV bindings independently pinned.
Fresh344-function reports repeated byte-for-byte;588 behavior links resolved;
Gekko listing check clean;git diff --check passed. make test passed six host
suites(core,TimingA,Hammer,defense,damage,status) plus58 Python tests;make all
exited0,no rebuild. Concurrent gameplay/damage/header/test edits preserved;only
explicit research paths staged. No original execution/emulator,ISO/profile/save
mutation,native implementation,asset conversion or story/dialogue rewrite by
this step. Inspection ELFs/disassembly/reports remain ignored.

Next:remaining unit entry/part/resource/status and HP/pouch/bonus overrides,
SpawnUnit helper kind/lifetime,full boss normal attack and damage/reward/death/
result consumers,then original tutorial state/input/timing traces. Continue all
reachable US systems and optional content before complete-game requirements.
Full objective active/incomplete;stat copy and spawn/owner links do not prove
complete unit mechanics,faithful tutorial execution or full game.

## 2026-10-12 — Rechecked GameCube paired-single disassembly repair

Verified the requested listing repair is already implemented in commits e4b9816
and 5837be7. tools/gekko_disasm.py decodes Gekko opcode4 and quantized memory
opcodes56/57/60/61 from the displayed original instruction words;the DOL
inspection pipeline applies it automatically after LLVM disassembly. Reserved
encodings remain explicit .long words instead of newer PowerPC operations.
Existing repair previously corrected354 instruction rows across47 listings.

Current validation: all449 PowerPC listings under artifacts/reference checked,
64,886 instruction rows and362 paired-single instructions;zero corrections
needed and zero unrecognized paired encodings. Fresh read-only ISO inspection
of PSMTXScale into /tmp emitted four correct psq_st instructions and passed the
same check. make test passed all six host suites and58 Python tests;make all
exited0 with no rebuild. No binary/listing/ISO mutation was necessary this turn.
Existing in-progress behavior tooling edits were preserved. This verification
covers text decoding,not execution or complete original-game equivalence.

Next: use the corrected inspection pipeline for subsequent US behavior research;
continue the unfinished boss normal attack/target contracts and broader reachable
systems before final complete-game requirements.

## 2026-10-12 — Boss attack graph and implementation request priorities

Added BATTLE_TARGET_EVENT_INTERFACES.md and HARBOR_BOSS_NORMAL_ATTACK.md.
Seven complete local target wrappers/helpers now document first-attack pointer
pairing,part ID lookup,signed side inversion and candidate-service ABI. The
first-target callback dereferences its selected unit without a null guard;the
sampling core remains open. Signed bias narrows twice when weapon flag10000000
selects inversion;do not replace it with unrestricted signed arithmetic.

The original95-command normal attack stream now has a complete request/branch
contract:flag2 chooses first-target versus sampling;precheck1 alone requests
ResultACDefence then CheckDamage flags100. Results2/3/4/6/default skip defense
and enter shared recovery;avoidance codes40/38/39 remain distinct. Presentation
retains pose/sound keys,explicit parent and concurrent inline waits,and raw
float operands. No fixed impact tick or total duration is inferred. No-target
GOTO99 has no label99 in this span;local GOTO handling does not invent an event
return. Feasibility/label ownership must be resolved before replacing that path.

Updated DOCUMENTATION_REQUESTS.md with implementation priorities,contract links,
scoped locks and partial-answer boundaries. Released the validated attack/target
slice's lock;parent requests remain open. Preserved the separately reserved
default-power slice and concurrent native damage-application work. Completion
assessment still records no complete full-system closure;full-game requirements
remain deferred until sufficient original contracts exist.

Validation:1,844 original instruction anchors and all prior/data checks passed;
seven attack import targets,result cases,95-command/label census independently
checked. Fresh351-function reports repeated byte-for-byte;fresh original normal
attack inventory matched prior report.604 behavior local links resolved;Gekko check covered449 files,64,886 instruction
rows,362 paired instructions,zero changes/unrecognized. make test passed seven
host suites(core,TimingA,Hammer,defense,damage,status,damage application) plus58
Python tests;make all exited0,no rebuild. git diff --check passed. Only explicit
research/documentation paths staged. No original execution/emulator,ISO/profile/
save writes,asset conversion,native implementation or story rewrite by this step.

Next:default power and concrete weapon/part inputs;target sampling cores and
flag/mode producers;JumpPosition/float/camera/audio/inline callback composition;
main/general/private pad update order and original defense-impact traces;damage/
death/reward/field result producers. Continue shipped US systems and optional
content. The documentation and complete-game objective remain incomplete.

## 2026-10-12 — Numeric ownership for documentation locks

User assigned this documentation agent ID1. Updated DOCUMENTATION_REQUESTS.md
so new locks require a user-assigned numeric agent ID rather than /root alone;
recorded ID1 on this agent's delivered Crump attack/target slice. Other existing
power-research reservations retain their owners pending identification;this
change does not claim or release another agent's work.

Validation: reviewed lock ownership and git diff --check passed. Documentation-
only change;no tests/build rerun or ISO/native/profile/save changes. Concurrent
native game/header/test edits preserved. Next:use ID1 for this agent's future
scoped reservations and continue prioritized original US behavior contracts.

## 2026-10-12 — ID1: battle jump callbacks and Crump count boundaries

Added BATTLE_JUMP_EVENT_CALLBACKS.md:complete JumpSetting/JumpPosition/
JumpContinue,move-current/target/visible accessors,horizontal integration and
floor-height local bodies. Used evtGetFloat literal routes are documented;
containing whole-body fingerprint is not full resolver coverage. Original
encoded0xf24a7b4c yields0.19921875,not exact0.2. Actual initial jump settings are
count0/speed2/acceleration0.19921875;count0 preserves an existing stored count,
deriving from distance/speed only when it is also0. Recovery counts20/10/10
include requested10 overriding the final setting15.

JumpPosition integrates on initialization and every resumed call;remaining
positive count blocks,completion snaps visible target then yields result1.
Automatic derived count0 returns2 after initialization without integration,
sound setup or target snap. Visible/current/target remain distinct. JumpContinue
copies visible→current initially,retains velocity/speed/heading,integrates then
finishes only below the stage-derived floor;exact equality blocks. Added scoped
JMP-01..07 implementation requirements and derived acceptance cases,with original
comparison obligations kept separate from controlled finite tests.

Dedicated tools/verify_battle_jump_contracts.py avoids editing ID2's concurrently
reserved general tooling. Ten containing-body hashes,55 instruction anchors,nine
constants,nine harbor imports and original settings/count operands pass read-only
ISO. Three jump metadata reports match byte-for-byte. General verifier initially
hit ID2's in-progress part-loop base-name collision;inbox note identified it,ID2
renamed the local,and final general checks passed1,916 anchors/data checks.
Fresh general report has359 function fingerprints;the separate jump report adds
ten bodies without silently changing general counts.619 behavior links resolved.
Gekko check covered459 listings,65,705 instructions,376 paired instructions,
zero changes/unrecognized. make test passed seven CTest suites plus58 Python
tests;make all exited0,no rebuild. git diff --check passed.

Updated attack-reference links,asset provenance and request inbox;released only
ID1's validated jump slice. AC-01/03 remain open for original cadence/eligibility/
execution. Preserved ID2's part reservation and concurrent native changes. No
original execution/emulator/profile/save mutation,ISO write,asset conversion,
native implementation or story/dialogue rewrite by this research step.

Next ID1 priorities:general/private/global input and battle/event update order,
execution-credit/pause ownership,approach/hit-position and angle/dist/audio/stage
services,then original impact/precheck/defense input traces. Coordinate through
numeric-ID scoped locks in DOCUMENTATION_REQUESTS.md. Continue all reachable US
systems and optional content;full-game documentation and final requirements
remain incomplete.

## 2026-10-12 — ID1: physical producer and battle input cadence

Added PHYSICAL_INPUT_PRODUCER.md and BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md.
Complete local makeKey/DemoPad read/copy,battle wrapper/engine/pad manager now
connect GP publication→four general histories→sequence controller→private
channel0 history→selected AC main→later main-chain event processing. Private pad
updates even without an AC main callback. The full12-byte snapshot write map
copies buttons/axes/triggers,zeros8/9,and retains10/11. Physical direction masks
use strict48 thresholds;battle virtual directions use inclusive30/different bits.

US producer repeats scale24/6 by frameRate/60,with whole-mask change ownership;
these differ from battle virtual16/8 pad repeats. Raw error-3 retains old held/
analog data while clearing derived edges/deltas. Original0x1c00 chord clears GP
input after publication but retains DemoPad;motor requests and last-mode latch
order documented. Lower SDK polling/clamping/reset services and their timing
remain open. Battle engine tests its sequence/GP flag gates only after all local
subsystem calls;no invented whole-battle early pause gate added.

Event execution credits can resume a yielded movement command again in the same
manager pass without a new physical/battle sample. Current producer input can
reach a later eligible defense event,subject to actual intermediate reset/writers;
each-hit private WorkInit and general history remain distinct. Added implementable
PAD-01..06 requirements and controlled acceptance boundaries. These static
contracts do not establish fixed tutorial impact ticks or observed input latency.

Validation:dedicated verify_input_cadence_contracts.py passed ten containing-body
hashes(six new local bodies/four prior identities),64 instructions,28 direct calls
and three data bindings;two metadata reports matched byte-for-byte. Existing
jump verifier and general verifier passed.647 behavior local links resolved.
Gekko check covered465 listings,66,469 instructions,376 paired instructions,
zero changes/unrecognized. make test passed eight CTest suites plus58 Python
tests;make all exited0,no rebuild. git diff --check passed. Shared general tooling
and concurrent native/ID2 badge documentation edits were left unstaged by ID1.
No original execution/emulator/profile/save mutation,ISO write,asset conversion,
native implementation or story/dialogue rewrite by this research step.

Updated input/reference/provenance links and request answers;released only ID1's
physical/battle input slice. AC-01/02/03 remain open for other writers,lifecycle,
pause/global overrides,multihit graph and original traces. Next ID1 work:actual
pause/stop/timescale and other sampler/reset caller census,then original input/
state observations and remaining target/movement/audio composition. Continue
all reachable US systems and optional content;full-game documentation and final
complete-game requirements remain incomplete.

## 2026-10-12 — ID1: input reset owners,system levels and tutorial injection

Added BATTLE_INPUT_RESET_OWNERS.md and EVENT_SYSTEM_LEVEL_AND_SPEED.md.
The static sampler census confirms two direct DOL WorkUpdate sites and no
module0 REL sampler imports. WorkInit has four direct DOL calls and one gor
REL24 import. Complete BattlePadInit/AC manager init/reset-result bodies preserve
general/private ownership,callback clearing and result/reset store order.
The gor _config_key_record helper is now fully specified with original HA/LO/
REL24 bindings:write result4,reset private pad,inject operand0 raw at latest
trigger history,return2. The subsequent defense checker clears the preliminary
result;history injection supplies the concrete input,not a checker bypass.

Two shipped lecture_zutsuki_attack_event calls inject A100/B200 under work
cases3/7 before adjacent defense/damage requests. Its162-command whole hash and
selected branch/import slice are pinned;full lecture graph,selector-6/work
producers,weapon eligibility and reachability remain open. This must not be
substituted for Crump's separate normal-attack physical input route.

Complete marioStSystemLevel/getter and evtSetSpeed now distinguish requested
level,GP18 mask,selected-context event stop bits,global multiplier,per-entry speed
and existing credits. Levels0..4 give exact start/stop masks;lower nonzero levels
need not resume older stops,EF excludes10,and getter returns the full GP mask.
Root/child/brother speed initialization reads global M rather than parent speed.
Zero speed and stopped-event lifetime suppression have different contracts.
No direct r13-relative stfs writer of M was found in DOL text;this is not proof
of no pointer/indexed/REL/bulk writers or a constant runtime multiplier.

Validation:input_ownership_audit.py passed six DOL body hashes,30 instruction
anchors,two data bindings,one REL helper hash/global/reset relocations,lecture
hash and selected case/mask/defense/damage bindings. Two final metadata reports
matched byte-for-byte. Existing input/jump verifiers and general1,980-anchor/
data checks passed.145 links in ID1-edited behavior files resolved. The whole
collection check encountered ID2's concurrently drafted Mario-parameter link
before its destination existed;no full-collection clean-link claim made here.
Gekko check covered470 listings,66,583 instructions,376 paired instructions,
zero changes/unrecognized. make test passed nine CTest suites plus58 Python
tests;make all exited0,no rebuild. git diff --check passed. Only ID1 research/
handoff paths staged;concurrent native and ID2 motion work preserved. No
original execution/emulator/profile/save mutation,ISO write,asset conversion,
native implementation or story/dialogue rewrite by ID1. Reports/listings ignored.

Updated inbox/reference/provenance links and released only ID1's validated lock.
AC-01/02/03 stay open. Next:lecture/helper mode selection and actual tutorial
input/eligibility writers;full UI/type/pause graph and original normal/multihit/
held/A+B traces;remaining target/audio/field/reward composition. Use numeric-ID
scoped reservations. Full US systems and optional content remain necessary before
final complete-game requirements;the overall objective is still incomplete.

## 2026-10-12 — ID1: separate helper roles and lecture coordinator modes

Added CHRISTINE_HELPER_ROLES.md and LECTURE_COORDINATOR_MODES.md.
Crump setup2 spawns NPC kind66d78 via647d8;lecture setup3 selects kind66fe8 via
60cb0 and weighted/group chain60de8/60ce0. NPC init requests maximum moves0;
its attack only restarts wait. The different lecture init sets Mario's HP-skip
attribute40000000/W2740,spawns professor67fb8 and creates reciprocal work0 ID
links. Selector-6 now connects through the proven W424 setter to that professor.
Asset reuse must not merge actor kind/state or apply lecture-only input/HP writes
to Crump's NPC helper. Initial kind10/0 values remain stat-copy inputs,not measured
final resources.

Complete professor init and200-command phase request graph specifies exact
0/2/4/6/8/9 work/phase branches,mode writes1/3/5/7/9,explicit rotations/joins/
brothers,status/camera/flag requests,early deletes and selection0/nonzero paths.
Its20 message/select/append sites bind16 original US gor_02 keys with exact
speaker operands/payload hashes. Three complete REL leaves resolve lecture mode,
ID ownership and defense classifications. Empty classifier case0 does not fall
through to work3=4;counter7→8 and attack-wrapper3→4 remain different tails.
Player1→2/5→6 producers,counter dispatch,field selection and full original lecture
execution remain open. No dialogue was paraphrased,invented or committed.

Validation:verify_lecture_helper_roles.py passed eight full data spans,three REL
leaf hashes,ten event hashes/counts,23 local/ten service relocations,work writers,
selector-6 and20 US message/speaker/uniqueness/hash checks. Two reports matched
byte-for-byte. Existing input/jump and general1,980-anchor/data checks passed.
149 links in ID1-edited behavior files resolved;whole collection had735 links,
with one missing target in ID2's in-progress movement-sound draft;no full clean
collection claim made. Gekko check covered470 listings,66,583 instructions,376
paired instructions,zero changes/unrecognized. Initial make test/all failed on
concurrent native action-service narrowing/clamp types;exact inbox notes enabled
native fixes. Final make test passed12 CTest suites plus58 Python tests;make all
exited0,no rebuild. git diff --check passed. Only ID1 research/handoff paths
staged;native and ID2 motion/tooling work preserved. No original execution,
emulator/profile/save/ISO mutation,asset conversion or native changes by ID1.
Inspection ELFs/reports stay ignored or in/tmp.

Released only ID1's helper-role lock and linked partial answers in the inbox.
Next:close missing player/lecture mode producers and W2740/counter consumers;
actual field trigger/return/story persistence;original input/state/US-message/
animation/audio comparison traces. Continue remaining reachable US systems and
optional content before final complete-game requirements. Full objective remains
active/incomplete;local coordinator closure is not a complete lecture or game.

### 2026-10-12 — Documentation ID2: selected target, weapon and payment

Delivered BATTLE_SELECTED_TARGET_AND_WEAPON.md and
BATTLE_WEAPON_COST_AND_PAYMENT.md for the reserved JUMP-01/DMG-01 slice.
Complete local contracts cover ordered signed retained-selection reads/output
callbacks, player command versus item weapon lookup, all15 cursor dispatch
entries, cost scaling classes/reduction/cap, partner FP ownership/Mario registry
lookup, separate affordability query, unchecked ordered FP/AP payment and
wrap-before-cap action-record bytes. Default Jump/Hammer zero-cost operands and
three ordinary Jump event bindings are verified. Corrected an eight-byte weapon
address transcription during verifier construction, before delivery. No native
code, ISO, asset, emulator profile or save changes by this documentation slice.

Validation: new verify_weapon_selection_cost.py passed16 complete body hashes,
11 instruction anchors,two constants,15 cursor targets,two weapon field checks
and three event calls; report at/tmp/id2-selection-cost.json. Existing general
behavior and19-body/268-command Jump graph evidence checks passed. make test
passed16 CTest executables and58 Python tests;make all exited0,no rebuild.
All750 local links across docs/behavior resolved;git diff --check passed.
Inspection outputs/reports stay outside source history.

Released only ID2's selected-target/cost lock and linked contracts in the inbox
and behavior index. Parent requests remain open. Next requested dependencies:
command-menu/selection producers,cost-check call cadence,numeric extremes,
Jump stylish/aftereffect/badge tails and original execution traces. ID1's lecture
player/mode/teardown reservation remains independent. Complete-game system
requirements remain deferred pending adequate system coverage;goal incomplete.

### 2026-10-12 — Documentation ID2: candidate sampling and choice cores

Delivered BATTLE_ENEMY_CANDIDATE_SAMPLING.md and
BATTLE_ENEMY_CANDIDATE_CHOICE.md for requested Takeoff/JUMP-01/DMG-02
sampling dependencies. Complete local wrapper/update/core contracts specify
partial buffer/geometry writes, all ordered admission masks, four compaction
passes, lane/X occlusion, count narrowing and permutation tie sorting. Complete
chooser covers initialized outputs, all ordered weights and weapon overrides,
deterministic physical-index maximum, weighted draw boundaries and shared
irand/rand LCG leaves. Actual Takeoff mode4/null weapon is deterministic with
no RNG use; sampler Q is side-mask swap rather than count/nonempty. Original
capacity is not enforced. No native timing or invented target fallback added.

Validation: verify_enemy_candidate_sampling.py passed seven complete body
fingerprints,48 instruction anchors and14 constants/SDA initialization spans;
report/tmp/id2-sampling-evidence.json. Existing13-body preparation and16-body
selection/payment verifiers passed. make test passed18 CTest suites and58
Python tests;make all exited0,no rebuild. All771 local behavior-document links
resolved;git diff --check passed. No native code, ISO, asset, emulator profile or
save mutations by ID2;inspection ELFs/disassemblies and reports remain in/tmp.
Concurrent native selection and ID1 lecture research preserved.

Released only ID2's candidate-service lock,linked both contracts in the inbox
and behavior index,and added the Takeoff core follow-up. Parent requests remain
open. Next:source/helper/numeric/domain validity and RNG seed/interleaving;
Jump stylish/aftereffect/badge-tail services;original input/target/state/motion/
audio execution traces. EVT-01 scheduling and ID1 lecture work remain separate.
Full documentation/system-requirements objective remains active and incomplete.

### 2026-10-12 — Documentation ID2: stylish timing and icon contracts

Delivered BATTLE_STYLISH_TIMING_AND_RESULTS.md and BATTLE_STYLISH_TIMING_ICON.md
for the reserved JUMP-01/AC-01 service slice. Complete local start/main/readers
cover shared record, first-call resolution/initialization plus same-call t1,
exclusive lower/inclusive upper success, opening history rejection, automatic
eligibility, timeout/terminal/repeated invocation and raw destination sentinel/
aliasing. Ordinary Jump has two exact ACRStart/GetResult pairs and history0.
Scoped USER_FUNC dispatcher sites establish0 repeat/yield versus1 complete/yield.
Local presentation contract separates baseline queue depth from visible draw
positions, height and original icon0xa2/scale/alpha requests. No audience service
internals, original execution or fixed render/impact timing claim.

Validation: verify_stylish_timing.py passed13 containing body fingerprints,
48 instruction anchors,five constant/dispatch spans and four ordinary callers;
dispatcher fingerprint does not imply full dispatcher recovery. Report at
/tmp/id2-stylish-evidence.json. Existing19-body Jump flow and seven-body candidate
checks passed. make all built concurrent candidate-choice/native target,exited0.
Initial make test failed because concurrent CMake registration referenced a
missing candidate-choice test;after its file appeared,re-run passed19 CTest
suites and58 Python tests. All789 then-current behavior links resolved;git diff
--check passed. ID2 made no native changes;preserved native and ID1 lecture work.
ISO stays read-only;inspection ELFs/disassemblies/reports in/tmp,no assets/saves/
emulator/profile mutations.

Released only ID2's stylish lock and linked contracts in inbox/index/Jump graph.
Next:Jump weapon-aftereffect/badge-tail/audience services, stylish eligibility
source writers and collision ownership, complete EVT-01 scheduling and original
input/state/motion/audio/presentation traces. Full documentation and eventual
complete-game requirements objective remains active/incomplete.

## 2026-10-12 — ID1: lecture player transitions and status arbitration

Added LECTURE_PLAYER_ATTACK_FLOW.md and BATTLE_STATUS_WINDOW_ARBITRATION.md.
Professor keys44/45 bind distinct lecture weapons and197-command Jump/193-command
Hammer scripts. Selected default-menu construction regions substitute pointers
under battle masks09000000/0a000000 plus professor ID!=-1;large menu body remains
partial. Both records retain Mario-level callbacks/six-word power tables,not a
fabricated constant tutorial power. W2740's proven consumer is status-window
suppression,separate from those weapon selection masks.

Closed missing mode1→2/5→6 cleanup writers,including their non-success-only
placement. Jump first-hit feedback reads output0;Hammer reads2;both mode9 maps
3/1/2→professor work2=1/2/3 after reset. Lecture Jump uses ID1 and actual pedagogical
A-trigger wait;coordinator01000000 is now a shipped producer of the recovered
TimingA forced classification3/result2/count route,subject to earlier gates.
Lecture Hammer uses ID3/literal4/25,mode5 flag2,hold/rehold/release polling and
explicit setup-event stop/start versus parameter2 suppression. All clocks and
callback effects remain separate from measured input/render timing.

Four complete window bodies preserve five-field priority and argument0/1/2
mutation/recompute policy. Lecture2740 blocker cannot be overridden by event-on
2748. The13-command end stream prints original024 and sets battle20,but does not
locally clear lecture suppression/Mario HP-skip or export resources/rewards.
Full end/key dispatch,field restoration,menu availability and original execution
remain open;partial end requests are not complete teardown.

Validation:verify_lecture_player_flow.py passed seven DOL containing hashes
(four window/two timing services complete;menu partial),31 instructions,three
403-command event hashes/counts,three full weapon/table spans,mode/feedback/AC
bindings and six exact US message/speaker/uniqueness/payload-hash sites. Two reports
matched byte-for-byte. Helper-role verifier and general1,980-anchor/data checks
passed. All789 behavior local links resolved. Gekko check covered478 listings,
67,944 instructions,376 paired instructions,zero changes/unrecognized. make test
passed18 CTest suites plus58 Python tests;make all exited0,no rebuild. git diff
--check passed. Only ID1 research/handoff paths staged. No original execution,
ISO/profile/save mutation,asset conversion,native implementation or story rewrite
by ID1. Original reports/ELFs/listings remain ignored.

Updated prior mode/helper/TimingA contracts,inbox answers and provenance;released
only ID1's lecture-player/window lock. Parent requests remain open. Next:complete
lecture field trigger/end-key/return/persistence and counter dispatch;actual menu
availability and input/resource/message/audio/animation traces;expand through
remaining US progression,field/partner/UI/save/chapter/optional systems. Final
complete-game requirements remain deferred until all systems are well documented.
The whole objective remains active/incomplete.

### 2026-10-12 — Documentation ID2: weapon aftereffect and stage requests

Delivered BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md and
BATTLE_STAGE_AFTEREFFECT_REQUESTS.md for reserved JUMP-01/DMG-02 service scope.
Complete local callback specifies four-weight selection,force/inhibition/B
restoration, all mandatory/conditional random draws,ordered stage calls and
nine original Jump/Hammer chance bytes. Jump initial draw is mandatory despite
zero destruction weights;Hammer sum106 is not normalized100. Complete called
leaves/helpers cover A1/B shared replacement versus A2 ignored handle, nozzle
weighted direction/type selection and liveness, frame flag-before-registration
and object tutorial gate. Actual Jump serialized sites80356668/803568ec are verified. No status/resource cleanup,allocator rollback or native
fixed hazard timer was invented. Queued script internals remain open.

Validation: verify_weapon_aftereffect.py passed13 complete body fingerprints,
56 instruction anchors,seven original weapon/default-event data spans and two
ordinary callers;report/tmp/id2-aftereffect-evidence.json. Existing13-body
stylish and19-body/268-command Jump graph verifiers passed. make test passed19
CTest suites and58 Python tests;make all exited0,no rebuild. All801 local links
in then-current behavior docs resolved;git diff --check passed. ID2 made no
native,asset,ISO,save or emulator/profile changes;inspection ELFs/disassemblies
and reports remain in/tmp. Concurrent native candidate and ID1 lecture work
preserved and not staged by ID2.

Released only ID2's aftereffect lock and linked contracts in inbox/index/Jump
graph. Next:queued stage hazard execution/source/domain/seed interleaving;
Jump badge-effect tail and audience services;complete EVT-01 scheduling and
original input/state/trajectory/audio/hazard traces. Complete-game requirements
remain deferred until adequate system coverage;full objective stays incomplete.

### 2026-10-12 — Documentation ID2: ordinary attack badge recovery tail

Delivered BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md,
BATTLE_BADGE_RECOVERY_SERVICES.md and BATTLE_BADGE_RECOVERY_PRESENTATION.md.
Complete32-command graph includes ordered HP/FP amount gates,two copied-local
brothers, effect requests,brother120/owner125 waits and delayed resource writes/
deletion. Complete amount/resource/geometry wrappers specify raw-actor translation,
signed damage/count minimum,ignored but resolved part,HP bookkeeping attribute,
FP shared-owner rereads,body list and integer/sentinel axis outputs. Scoped
local-float encoding/MULF/INTEGER ADD and recovery effect catalog/request route
avoid unquantized placement or detached native recovery timers. Actual Jump
waiting-child call80356be0 pinned;corrected a four-byte transcription while
building the verifier before delivery. Effect renderer/full VM/domain/source/
original timing and presentation remain open.

Validation: verify_badge_recovery_tail.py passed18 containing-body fingerprints,
71 instruction anchors,13 data spans and exact32-command graph/two brothers;
containing interpreter/effect hashes do not establish all namespaces/effects.
Report/tmp/id2-badge-tail-evidence.json. Existing19-body/268-command Jump and
13-body aftereffect verifiers passed. make test passed20 CTest suites and58
Python tests;make all built concurrent candidate/takeoff modules and3DSX,exited0.
All820 local behavior links resolved;git diff --check passed. ID2 made no native,
asset,ISO,save or emulator/profile edits. Inspection artifacts/reports stay/tmp;
concurrent native and ID1 lecture-field work preserved and not staged by ID2.

Released only ID2's badge-tail lock,linked all three contracts in inbox/index/
Jump graph. Next requests: audience feedback/prize/source writers, complete
EVT-01 scheduling and numerical/domain contracts, stage queued event execution,
original input/state/motion/audio traces. Whole documentation and eventual
complete-game requirements objective remains active/incomplete.


## 2026-10-12 — ID1 lecture field entry, choices and return requests

Completed a scoped static US contract in
[LECTURE_FIELD_ENTRY_AND_RETURN.md](behavior/LECTURE_FIELD_ENTRY_AND_RETURN.md).
The wall-event handoff and story10 map-reload callers bind setup3;both accepting
and skipping the lesson reach the common story11 write. Preserved the original
crossing loop's LV5<-LV1 update,exact numeric selection branches,Professor and
Goombella US speaker bindings,NPC/party conversion order and post-wait requests.
Professor init positions/flag guard are explicit;all-chapter talk is not closed.

Validation:verify_lecture_field_flow.py passed four whole event spans/346
commands,332 scoped ADDR32 bindings,22 operand anchors,four entry/NPC links and
11 exact US message payload/service/speaker sites. Repeated metadata report was
byte-identical;Python compilation passed. General behavior verification passed
1980 instruction anchors;all826 behavior Markdown links resolved at this check.
Gekko check passed478 listings/67944 instructions/376 paired-single operations,
zero changes or unknowns. make test passed20 CTest and58 Python tests;make all
built ttyd-2ds.3dsx successfully. Concurrent native and ID2 changes preserved.
No original gameplay/emulator execution,ISO/profile/save mutation or original
binary/dialogue inclusion in history occurred;the ISO was opened read-only.

Released only ID1's lecture field lock. Next:intersection callback and encoded
variable namespace;field party clear/kill/previous-position/run lifetime;complete
battle-end and lecture-mode/window/HP immunity teardown;flag writers and original
accepted/declined/interrupted/reentry observations. Full game documentation and
remake remain incomplete;complete-game requirements remain deferred until system
coverage is sufficient.

### 2026-10-12 — Documentation ID2: complete manager and wait continuations

Delivered EVENT_MANAGER_COMPLETE_LOOP.md and
EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md for EVT-01 priority0 request slice.
Whole local manager/context/priority bodies specify tick delta signed clamp and
large-gap substitution,previous-time stores,eligibility/live ID, lifetime once
per visit,single-rounded credit subtract-before-dispatch and live-count loop.
Scoped dispatcher fetch/common tail distinguishes handler0/1/2/3/255/negative
from dispatcher0/-1/1:yield can run again with remaining credit in the same pass.
Complete MSEC helper uses sign-extended low lifetime difference and unsigned
64-bit division;frame wait decrements on its first invocation. Constructor/
append/initial state and broader opcode/source/domain/runtime scope remains open.

Validation: verify_event_manager_loop.py passed seven containing fingerprints,
70 anchors,eight constants/dispatch spans;dispatcher/division fingerprints do
not claim all opcode/numeric recovery. Report/tmp/id2-event-loop-evidence.json.
Existing stylish/badge-tail verifiers passed. make all built concurrent native
changes and3DSX,exited0. make test failed core tests/core_tests.cpp:137 jump
height/position assertion during concurrent field movement work;other21/22
CTest suites passed. Python suite then ran independently,58 tests passed.
Failure recorded in inbox;no full host green claim. All836 local links in
then-current behavior collection resolved;git diff --check passed. No native,
ISO,asset,save/emulator/profile mutations by ID2;inspection/reports in/tmp.
Other agents' native and lecture-field work preserved and not staged by ID2.

Released only ID2's manager/wait scoped lock,linked contracts in inbox/index/
EVENT_RUNTIME. Next EVT-01 work:root/child/brother/inline constructors,current-
table append/allocation/initial credit/type/run-check, restart and remaining
opcode closures;original pause/multicredit/creation/deletion traces. Full-game
documentation and eventual requirements objective remains active/incomplete.


## 2026-10-12 — ID1 field intersection and party callback contracts

Documented [segment/query](behavior/FIELD_SEGMENT_INTERSECTION.md) and
[party callback](behavior/FIELD_PARTY_EVENT_CALLBACKS.md) local behavior from
original US DOL bodies and actual gor imports. Closed strict coordinate
rejection,closed endpoints,rounded fused cross-products,live position offsets
and used LV encoding at1/1024. Preserved the lecture's Y-copy operand;its
stationary-position example is a static deduction,not observed gameplay.
Previous-position resolves encoded coordinates through integer truncation and
can request creation;updated the earlier field document accordingly. Clear/kill
nonzero-slot dispatch differs;run is conditional on mode0. Small pointer and
mode-change bodies now specify ordered local writes;larger constructors,
destructors,mode-init and saved-kind lifetimes remain open.

Validation:verify_field_lecture_callbacks.py passed15 containing DOL hashes,
54 selected instructions,six constants and10 actual gor imports. Twelve bodies
fully specified;three general variable bodies cover only used local branches.
Repeated metadata was byte-identical;Python compilation and prior field-flow
verifier passed. General verifier passed1980 instruction anchors;all850 behavior
Markdown links resolved. Gekko check passed492 listings/68908 instructions/
396 paired-single operations with zero changes or unknowns. make test passed
24 CTest and58 Python tests;make all succeeded with no rebuild required.
No original execution/emulator observation,ISO/profile/save mutation,asset
conversion or original binary/dialogue inclusion in history. Concurrent native
and ID2 work preserved;only ID1's scoped field callback lock released.

Next:partyKill/partyKill2 saved-kind/pointer lifetimes and partyEntryPos/
partyEntry2Pos construction;mode initialization/global-index writers;complete
battle-end/lecture-mode/window/HP immunity teardown and original lesson traces.
General variable namespaces,coordinate writers and scheduler composition remain
separate work. Full game documentation/remake remain incomplete;complete-game
requirements remain deferred until sufficient system coverage.

### 2026-10-12 — Documentation ID2: root/typed constructor closure

Delivered EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md and
EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md for EVT-01 scoped initialization request.
Complete root/typed bodies specify first-inactive slot scan,allocation counter
increment/1b0 clear,all explicit fields,low8 flags/type/priority,global speed
capture/initial credit0,actor/stack sentinels and ID assignment/wrap ordering.
Full label scan distinguishes terminal opcode1 from delete2,reads first operand
unconditionally,stores low8 IDs/after-command pointers,and has no effective
16-label cap. Live append requires BOTH manager-active and flags0x20;run-check
then invokes all-event stop masks and ignores its argument. No constructor
immediate dispatch,sort,capacity error or rollback was invented.

Validation: verify_event_root_constructors.py passed five containing-body
fingerprints (three newly recovered full bodies plus linked stop identities),
62 instruction anchors,four data/SDA spans. Report/tmp/id2-event-roots-evidence.json.
Existing seven-body manager verifier passed. make test passed24 CTest suites
and58 Python tests;make all exited0,no rebuild. Earlier concurrent core:137
field-movement failure is resolved in current worktree validation. All854 local
behavior links resolved;git diff --check passed. ID2 made no native/asset/ISO/
save/emulator-profile edits;inspection ELFs/disassemblies/reports remain/tmp.
Concurrent native field/recovery and ID1 research preserved and not staged.

Released only ID2's root-constructor lock;linked both contracts in inbox/index/
EVENT_RUNTIME. EVT-01 remains open at priority0. Next:child/brother/inline full
constructors,inherited versus reset fields,current-table append/type/run-check,
restart ownership and remaining opcode closure;original multicredit/context/
creation/deletion/pause traces. Complete-game documentation and subsequent
requirements objective remains active/incomplete.

### 2026-10-12 — Documentation ID2: descendants and restart closure

Delivered EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md and
EVENT_RESTART_PRESERVATION.md for EVT-01 scoped request. Complete child/brother
bodies specify parent wait versus owner links,full inherited/reset maps,priority
wrap,global speed/credit0,message metadata difference,labels and unconditional
active-manager append. Zero-ID/run-check order differs between constructors.
Complete plain107/110 wrappers retain first-end scanning,post-construction
recopies,brother150 versus inline metadata and independent inline ownership.
Restart resets pointers/opcode/stacks/labels/time/credit but preserves locals,
identity/links/flags,timers and cached callback/first-call fields until fetch.
No implicit child cancel,local rollback,immediate dispatch or capacity fallback.

Validation: verify_event_descendant_constructors.py passed seven containing
fingerprints,five newly recovered complete bodies plus root/run-check identities,
81 anchors,five data spans;report/tmp/id2-event-descendants-evidence.json.
Existing root and manager verifiers passed. make test passed25 CTest suites
and58 Python tests;make all exited0,no rebuild. All867 local behavior links
resolved;git diff --check passed. ID2 made no native/asset/ISO/save/emulator-profile
changes;inspection ELFs/disassemblies/reports remain/tmp. Concurrent native event/
field work and ID1 documentation preserved and not staged.

Released only ID2's descendant/restart lock;linked both contracts in inbox/index/
EVENT_RUNTIME. Next EVT-01:ID-output inline/brother variants,child-call and other
control/deletion opcode closures,remaining initialization/domain/source owners
and original multicredit/creation/deletion/pause traces. Full-game documentation
and eventual requirements objective remains active/incomplete.


## 2026-10-12 — ID1 field party lifetime and position construction

Added [destruction/saved kind](behavior/FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md)
and [position construction](behavior/FIELD_PARTY_POSITION_CONSTRUCTORS.md)
from complete US partyKill/partyKill2/partyEntryMain/partyEntryPos/partyEntry2Pos
and kuribo_init local bodies. Kill preserves a post-hook linked kind;Kill2
clears it. Both clear the current slot/table pointer before pose release/free.
Main allocation scans two slots;secondary requires slot1 free. Position wrappers
specify animation/HP/color/init requests,primary/secondary ordered coordinates,
mode tails and implicit failure/domain requirements. Kind1/8 records pin their
init and zero exit hooks. Linked earlier callback docs;full called-driver,
all-kind,mode/index/resource and original transitions remain incomplete.

Validation:verify_field_party_lifetime.py passed six whole DOL hashes,43
instructions,three constants,five data-span hashes and three field bindings.
Repeated metadata was byte-identical;existing field-callback and field-event
verifiers passed after the reusable verifier gained optional data-span checks.
Python compilation passed. General verifier passed1980 instruction anchors;
all876 behavior Markdown links resolved. Gekko check passed499 listings/69785
instructions/408 paired-single operations with zero changes or unknowns.
make test passed25 CTest and58 Python tests;make all succeeded with no rebuild.
Original ISO read-only;no original execution/emulator observation,asset
conversion,profile/save mutation or original binary/dialogue inclusion in source.
Only ID1 lifetime lock released;concurrent ID2/native work preserved.

Next:mode initialization/global current-index and owner-slot writers;all-kind
init/exit hooks and pose/heap/resource lifetime;complete battle-end/lecture-mode/
window/HP immunity teardown and original lesson transition traces. Full game
system documentation/remake remain incomplete;complete-game requirements remain
deferred until adequate coverage.

### 2026-10-12 — Documentation ID2: run/ID variants and event controls

Delivered EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md and
EVENT_CONTROL_OPCODES_AND_ID_SCANS.md. Complete92/93/108/111 helpers specify
independent run flags0/no same-pass append,post-constructor copies,raw ID output
and first-end scanners;brother111 omits plain110's150 copy. Scoped94..106
blocks specify child255 yield without owner deletion,restart/setters/stop/start/
check calls and outputs. Complete setter and Other/ID/Check leaves retain global
speed multiply/credit preservation,pointer exclusion,all-match ID maskef and
first-match active liveness,live versus captured capacity. No immediate child
recursion,ID-as-slot,per-ID eligibility bypass or invented cancellation policy.

Validation: verify_event_run_and_controls.py passed16 containing fingerprints
(four helpers,eight leaves,scoped dispatcher and three linked constructors),72
anchors,25 data/dispatch spans;report/tmp/id2-event-controls-evidence.json.
Existing descendant and manager verifiers passed. make all built concurrent
event modules and3DSX,exited0. make test configuration failed at CMakeLists.txt:103
because concurrent registration referenced missing tests/event_dispatch_tests.cpp;
CTest/Python phases were not reached by make test. Independent Python suite
passed58 tests. Inbox records the native fixture dependency;no full host-pass
claim. All889 behavior local links resolved;git diff --check passed. No native,
asset,ISO,save/emulator-profile changes by ID2;inspection/report artifacts/tmp.
Concurrent native dispatcher and ID1 research preserved and not staged by ID2.

Released only ID2's run/control lock,linked contracts in inbox/index/EVENT_RUNTIME.
EVT-01 remains open:deep stop/start/delete traversal,other opcode/namespace/
source/domain initialization and original pause/creation/deletion/multicredit
traces. Whole documentation and eventual complete-game requirements objective
remains active/incomplete.

Final ID2 run/control validation follow-up:concurrent dispatch test fixture
appeared before commit. Re-run make test passed26 CTest suites and58 Python
tests. The earlier missing-fixture failure is resolved;final host validation
passes. ID2 changed no native file for that resolution.


## 2026-10-12 — ID1 party mode initialization and driver/index ownership

Added [mode initialization](behavior/FIELD_PARTY_MODE_INITIALIZATION.md) and
[driver/index census](behavior/FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md).
Complete local old-mode graph distinguishes1/2 slit cleanup,3 kind hook,4
restore,11/new8 exception and12 flag clear;wrapper order preserves requested
final mode. Jump parameter clear and rounded revise360 loops are explicit.
Party init clears two pointers only;partyMain counts enabled objects then
rereads slots in order and gates driveParty on Mario mask02000000. Ordinary
stop/run does not clear object enable mask1. Control-index word8041ead0 lies
in original BSS;zero direct integer stores is a bounded census,not live-value
or all-writer proof. Linked earlier callback notes to the newly closed bodies.

Validation:verify_field_party_modes.py passed eight whole DOL hashes,44
instructions,six constants,three data spans,two actual lecture imports,BSS span
and the seven-slot DOL text direct-store census. Repeated metadata was
byte-identical;Python compilation and existing callback/lifetime verifiers passed.
General verifier passed1980 instruction anchors;all909 behavior links resolved.
Gekko check passed508 listings/70191 instructions/408 paired-single operations
with zero changes or unknowns. make test passed26 CTest and58 Python tests;
make all rebuilt ttyd-2ds.3dsx successfully. Original ISO read-only;no original
execution/emulator observation,asset conversion,profile/save mutation or original
binary/dialogue distribution. Only ID1 scoped mode/index lock released;
concurrent native and ID2 changes preserved.

Next:driveParty/global updater and partyMain/partyInit caller cadence;all mode
update bodies,pose/paper and kind-hook services;alias/REL/index/owner-slot writers;
complete battle-end/lecture-mode/window/HP immunity teardown and original
transition observations. Full documentation/remake remain incomplete;complete-
game requirements remain deferred until sufficient system coverage.

### 2026-10-12 — Documentation ID2: complete event lifetime services

Delivered EVENT_DELETE_AND_COPYBACK.md and
EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md for EVT-01 reserved lifetime scope.
Complete Delete/DeleteID cover active gate,child-before-brother traversal,
context/live scans,parent copy-back even on ancestor cancellation,paired metadata,
clear/memset/allocation counter order and all-match IDs. Complete Stop/Start/All
retain descendant-before-type-mask policy,active scan versus unguarded direct
child distinction and precise compiler-inline/recursive context captures.
Start/Stop differ only in seven flag mutations;no refcount/restart/local rollback
or immediate dispatcher suspension was introduced. Updated EVT-01 inbox's
current-delivery summary so historical partial requests do not hide answered
local manager/constructor/lifetime obligations.

Validation: verify_event_lifetime_services.py passed six full body fingerprints,
70 instruction anchors,SDA span and all245 paired Stop/Start instructions with
seven expected flag mutation differences. Report/tmp/id2-event-lifetime-evidence.json.
Existing run/control and manager verifiers passed. make test passed26 CTest
suites and58 Python tests;make all rebuilt concurrent native files and3DSX,
exited0. All913 then-current behavior local links resolved;git diff --check
passed. ID2 made no native,asset,ISO,save or emulator-profile edits;inspection/
reports remain/tmp. Concurrent native and ID1 research preserved and not staged.

Released only ID2's lifetime lock,linked both documents in inbox/index/runtime.
Next EVT-01:other reachable opcodes and full variable namespaces,source/context/
time/domain initialization and original pause/multicredit/creation/deletion
traces. Requested Jump/audience/numeric/source composition remains separate.
Whole documentation and eventual complete-game requirements objective remains
active/incomplete.


## 2026-10-12 — ID1 field party driver and global countdown/action

Added [driver dispatch](behavior/FIELD_PARTY_DRIVER_DISPATCH.md) and
[global countdown/action](behavior/FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md).
Complete local15-mode graph covers shadow bypass,prefix/scratch writes,pending
entry/pose/pipe/ride requests,kind callbacks and inline mode14 saved-kind/
slot/table removal/pose release/free. Most paths share effect/screen/projection/
presentation requests;mode0 still calls kind record+8. Global updater specifies
mask/key-off cancellation,unsigned decrement/signed byte expiry and primary
mode0/1/2→3 action request. Called service bodies,writers and original runtime
remain separate;no button meaning,measured duration or executed effect invented.

Validation:verify_field_party_drive.py passed four whole DOL hashes,58
instructions,one zero constant,four data-span hashes with15 explicit dispatch
words,and two actual lecture callback imports. Repeated metadata byte-identical;
Python compilation and field callback/mode/lifetime verifiers passed after the
reusable checker gained optional explicit data-word binding validation. General
verifier passed1980 instruction anchors;all933 behavior links resolved. Gekko
check passed512 listings/70718 instructions/410 paired-single operations,zero
changes or unknowns. make test passed28 CTest and58 Python tests;make all
succeeded with no rebuild required. Original ISO read-only;no original execution/
emulator observation,asset conversion,profile/save mutation or original binary/
dialogue inclusion in history. Only ID1's scoped driver lock released;concurrent
native and ID2 changes preserved.

Next:actual mode0 kind callback and mode1 following/entry engines;kind abilities,
mode/collision/effect/pose/projection services;countdown/input-off/flag and slot/
index writers;complete field/battle teardown and original transition traces.
Full system documentation/remake remain incomplete;complete-game requirements
remain deferred until adequate coverage.

### 2026-10-12 — Documentation ID2: full operand namespace bodies

Delivered EVENT_INTEGER_OPERAND_NAMESPACES.md,EVENT_FLOAT_OPERAND_ROUTING.md
and EVENT_SAVED_SWITCH_STORAGE.md for EVT-01 scoped namespace request.
Complete four APIs plus context getter and ten saved RAM leaves specify signed
bands/bases,indexed storage,flag quotient/remainder/PPC shift,numeric coercion,
read/store/return policy and context/GP capture. Integer versus float bands are
asymmetric:float APIs omit UF/saved-flag branches,and SetFloat broad GF routing
covers integer saved namespaces. No shared semantic tag decoder was invented.
GSW0 has separate full-word GP174 storage;nonzero bytes and LSW differ. Setter
old-word/old-bool/raw-value/float returns and1024 encoding are explicit. No array
bounds/null/output sentinel or on-disk persistence inference. Earlier scoped
badge presentation now links the full namespace contracts.

Validation: verify_event_operand_namespaces.py passed15 complete body hashes,
94 instruction anchors,seven numeric/SDA spans;report/tmp/id2-namespace-evidence.json.
Existing lifetime and badge-tail evidence checks passed. make test passed28
CTest suites and58 Python tests;make all exited0,no rebuild. All942 then-current
behavior local links resolved;git diff --check passed. ID2 made no native,asset,
ISO,save/emulator-profile mutations;inspection/report artifacts remain/tmp.
Concurrent native creation/field and ID1 research preserved and not staged.

Released only ID2's namespace lock,linked three contracts in inbox/index/runtime
and scoped presentation;updated EVT-01 current-delivery summary. Next:remaining
reachable opcode bodies,numerical/conversion and storage-domain/source/persistence
ownership,original script/pause/multicredit/creation/deletion traces. Complete-game
documentation and subsequent requirements objective remains active/incomplete.

### 2026-10-12 — Documentation ID2: GOTO/loops and arithmetic/cursor blocks

Delivered EVENT_GOTO_AND_LOOP_OPCODES.md and
EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md for EVT-01 reserved opcode slice.
Complete4..8/NEXT retain pointer/label threshold,first16 signed-byte lookup and
missing fallback,raw DO count/unconditional first body,deferred variable decrement,
literal0 and-10000000 routing threshold,lexical BREAK versus CONTINUE and byte
stack updates. Complete50..64 retain raw/resolved/float SET,RHS-first arithmetic,
wrapped integer ops,MOD's single/+0.5 conversion before signed remainder,float
rounding and62..64 cursor advance-before-read with fresh second-lane cursor.
Corrected negative-counter prose during review to retain threshold-crossing
behavior,not assume indefinite private literal decrement. No implicit loop yield,
count snapshot or simplified MOD/data-prefetch was invented.

Validation: verify_event_loop_and_arithmetic.py passed six containing hashes
(NEXT full,dispatcher scoped,four operand APIs linked),73 anchors and24 spans,
including20 exact opcode bindings;report/tmp/id2-loop-arithmetic-evidence.json.
Existing namespace and ordinary Jump flow verifiers passed. make test passed29
CTest suites and58 Python tests;make all rebuilt concurrent native game target/
3DSX,exited0. All964 behavior local links resolved;git diff --check passed. No
native,asset,ISO,save/emulator-profile changes by ID2;inspection/reports remain
/tmp. Concurrent native and ID1 research preserved and not staged.

Released only ID2's loop/arithmetic lock,linked both contracts in inbox/index/
runtime. Next EVT-01:remaining predicates/switches,multiword/float data readers,
other reachable opcodes,numeric/source/domain and original script/pause/input/
multicredit traces. Full documentation and eventual complete-game requirements
objective remains active/incomplete.


## 2026-10-12 — ID1 ordinary partner walk/history and mode1 callbacks

Added [walk callback](behavior/FIELD_PARTNER_WALK_CALLBACK.md),
[history writer/readers](behavior/FIELD_PARTNER_HISTORY_WRITER.md) and
[mode1 substates](behavior/FIELD_PARTNER_MODE1_SUBSTATES.md). Complete local
callbacks specify marker-based bank initialization,80-record layouts,source
selection,rounded target/speed formulas and mode1 splash/hazard requests.
History append preserves unwrapped read==write+1 before increment/wrap and
flag/coordinate comparison order. Slot-based writer bank selection differs
from flag-based initialization;valid-state invariants remain open. The local
unreachable cached-substate3 test is recorded without inventing an executed
cleanup path. Final walkMain movement/terrain and called abilities remain open.

Validation:verify_field_party_follow.py passed seven whole DOL hashes,66
instructions,ten constants,two kind-record hashes and two actual lecture imports.
Repeated metadata byte-identical;Python compilation and previous driver verifier
passed. General verifier passed1980 instruction anchors;all962 behavior links
resolved. Gekko check passed517 listings/71477 instructions/410 paired-single
operations,zero changes or unknowns. make test passed29 CTest and58 Python
tests;make all succeeded with no rebuild required. Original ISO read-only;no
original execution/emulator observation,asset conversion,profile/save mutation
or original binary/dialogue distribution. Only ID1's movement callback lock
released;concurrent native and ID2 edits preserved outside the scoped commit.

Next:final walkMain and read-index/path/terrain/pose services;source getters,
status/distance/flag/slot invariants;mode1 substates/cloud/fall/shadow/splash
engines and original partner trajectory/stop/run/hazard traces;complete field/
battle teardown. Full game documentation/remake remain incomplete;complete-game
requirements remain deferred until adequate coverage.

## 2026-10-12 — ID2 complete local SWITCH/CASE state machine

Added [switch stack/scanners](behavior/EVENT_SWITCH_STACK_AND_CASE_SCANS.md)
and [group/mask/range](behavior/EVENT_GROUPED_MASK_AND_RANGE_CASES.md).
Complete local34..49 and eleven CASE helpers specify actual dispatch ordering,
resolved/raw pushes,signed8 stack/state storage,all six signed predicates,
OR/AND success/failure latches,group termination,raw any-bit masks,inclusive
range queries,DEFAULT/BREAK/END and exact lexical header continuations.
Verifier checks the table directly rather than assigning opcodes in executable
function order. Scanners count34/1 but ignore35;case search includes36..47 and
skips BREAK48. Query/capture/state-read order and same-invocation return2 are
explicit. Only ID2's scoped switch lock released;EVT-01 stays open.

Validation:verify_event_switch_cases.py passed eleven whole CASE hashes plus
containing dispatcher hash,267 instruction anchors and16 dispatch entries;
Python compilation passed. General behavior verifier passed1980 instruction
anchors;all1186 local documentation links resolved. make test passed30 CTest
suites and58 Python tests;make all succeeded with no rebuild required.
`git diff --check` passed. ISO read-only;no original execution/emulator trace,
asset conversion,profile/save mutation or original binary/dialogue distribution.
ID2 changed no native files;concurrent ID1/native work preserved.

Next:remaining IF/predicate/data-read/bitwise/other opcode bodies and exact
numeric/pointer/source/capacity domains;original script composition and group/
nesting traces,then runtime integration acceptance. Complete-game behavior and
requirements remain incomplete and must not be presented as finished.


## 2026-10-12 — ID1 partner walkMain dispatch and stay local motion

Added [walkMain dispatcher](behavior/FIELD_PARTNER_WALK_DISPATCH.md) and
[stay motion](behavior/FIELD_PARTNER_STAY_MOTION.md). Complete local dispatcher
separates motion/substate/driver-mode bytes and selects ground/fly/homing/
jump/fall/behind/damage services;stay supplies actual threshold/map/distance
branches,mode0 admission,ordered wall/surrounding X/Z stores and kind7 pose
cycle. Mode0 does not skip all correction;special owner-state return occurs
before its distance guard. Out-of-classification floating threshold paths retain
the original incoming-register dependency;no fallback constant was invented.
Called geometry/direction/terrain/pose and other motion bodies remain open.

Validation:verify_field_partner_executor.py passed two whole DOL hashes,66
instructions,nine constants,five data spans/eight explicit substate targets and
two lecture imports. Repeated metadata byte-identical;Python compilation and
prior following verifier passed. General verifier passed1980 instruction
anchors;all991 behavior links resolved. Gekko check passed520 files/72569
instructions/420 paired-single operations with zero changes or unknowns.
make test passed30 CTest and58 Python tests;make all succeeded with no rebuild.
Original ISO read-only;no original execution/emulator observation,asset
conversion,profile/save mutation or original binary/dialogue distribution.
Only ID1 scoped executor/stay lock released;concurrent native and ID2 edits
preserved outside this scoped commit.

Next:partyGetMoveDirSpd and target/footmark helpers;front-wall/surrounding/
floor/fly correction and homing/fall/jump/behind bodies;state/flag/index/owner
writers and original trajectories/stop/run traces;complete field/battle teardown.
Full documentation/remake remain incomplete;complete-game requirements remain
deferred until sufficient system coverage.

## 2026-10-12 — ID2 complete IF predicates and branch continuations

Added [integer/mask/continuations](behavior/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md)
and [float/string predicates](behavior/EVENT_IF_FLOAT_AND_STRING_PREDICATES.md).
Complete12..33,twenty IF helpers and called strcmp retain query-before-second-
operand-load,captured base,raw masks,signed comparisons,null-to-empty pointer
replacement,byte/word read order and comparison return magnitude. Float CR
negation accepts unordered on all predicates except equality when execution
proceeds;no host relational substitute or extra precision conversion is inferred.
False scan consumes same-level ELSE/END;ELSE scan ignores ELSE markers;END_IF
only supplies2. All remain in the same dispatcher invocation. Existing root/
lifetime evidence also answers the new deletion-counter breadcrumb:both touch
8041e860 through r13+1940;this is distinct from schedule live count8041e85c.
Only ID2's predicate lock released;EVT-01 remains open.

Validation:verify_event_if_predicates.py passed twenty whole IF hashes,whole
strcmp hash and containing dispatcher fingerprint,416 instruction anchors and
25 spans (22 dispatch targets,NUL and r2 setup). All twenty29-instruction false
scanners are byte-identical. Python compilation passed;general behavior verifier
passed1980 anchors;root/lifetime/operand namespace verifiers passed. make test
passed30 CTest suites and58 Python tests;make all succeeded with no rebuild.
All1218 local documentation links resolved;git diff --check passed. Original
ISO read-only;no original execution/emulator observation,asset conversion,
profile/save mutation or original binary/dialogue distribution. ID2 changed
no native files;concurrent ID1/native edits preserved.

Next:remaining data-reader/bitwise/user-buffer/memory/other opcode bodies and
numeric/pointer/source/capacity domains;original script composition,NaN/FPSCR
and branch/pause/multicredit traces,then runtime binding acceptance. Complete
game documentation and requirements remain incomplete.


## 2026-10-12 — ID1 partner direction/speed and overlap correction

Added [overlap correction](behavior/FIELD_PARTNER_OVERLAP_CORRECTION.md),
[direction/speed query](behavior/FIELD_PARTNER_DIRECTION_AND_SPEED.md) and
[norm arithmetic](behavior/FIELD_PARTNER_SPEED_NUMERICS.md). The called helper
is partyMarioHosei and returns a signed overlap counter,not a target count.
Complete local graphs specify box admission,counter wrap/decay,history reset,
30-counter state publication,first-direction/second-speed outputs and fused
vector/norm composition with hardware estimate/three double refinements. Stay's
two stateful calls are linked;no getter caching or host sqrt equivalence claimed.
Called trig/angle/status/geometry/terrain and numeric/domain evidence remain open.

Validation:verify_field_partner_direction.py passed three whole DOL hashes,64
instructions,19 constants and two actual lecture imports. Repeated metadata
byte-identical;Python compilation and prior executor verifier passed. General
verifier passed1980 instruction anchors;all1015 behavior links resolved. Gekko
check passed522 files/72733 instructions/426 paired-single operations with zero
changes or unknowns. make all built ttyd-2ds.3dsx successfully. make test FAILED
CMake generation atCMakeLists.txt:127 because concurrent native target source
 tests/event_controls_tests.cpp was absent. Exact log:/tmp/id1-partner-dir-test.log;
inbox breadcrumb requests its completion. No placeholder/native fix written by
ID1. Original ISO read-only;no original execution/numeric seed measurement,
asset conversion,profile/save mutation or original binary/dialogue distribution.
Only ID1's binary-validated scoped lock released;native full-test gate remains
pending and concurrent native/ID2 work preserved.

Next:rerun make test after the missing native test source arrives;target/
footmark and front-wall/surrounding/floor/fly helpers;angle/trig and exact Gekko
estimate/FPSCR/domain;owner/flag/index writers and original trajectories;complete
field/battle teardown. Full system documentation/remake remain incomplete;
complete-game requirements remain deferred until sufficient system coverage.

## 2026-10-12 — ID2 data readers,bitwise/time/memory and user allocation

Added [multiword/float readers](behavior/EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md),
[bitwise/time/memory](behavior/EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) and
[user buffers/map allocation](behavior/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md).
Complete local65..90 preserve cursor advance-before-load/reload between outputs,
indexed73 signed-integer→single conversion versus sequential IEEE float load,
raw/resolved RHS and destination capture,double-query source routing and word-
wrapped60/1000 time products before signed truncation. Allocation76 publishes
null failure,replaces E154 without local free,and uses global root8041e808.
Complete local best-fit allocator keeps first equal fits,always writes remainder
header,zeroes payload then flushes cache. Complete memset/fill/cache leaf policy
and syscall/domain boundaries are recorded. Only ID2 scoped lock released;
EVT-01 remains open and full runtime/ownership integration is not claimed.

Validation:verify_event_data_memory_opcodes.py passed nine containing/linked/
complete body fingerprints,269 anchors,28 spans including26 dispatch entries;
repeated reports byte-identical;Python compilation passed. Deterministic100013
inputs per time conversion matched instruction-form versus wrapped-product/
signed-truncation formulas;this was host arithmetic,not original execution.
General behavior verifier passed1980 anchors;full operand verifier passed.
make test passed31 CTest suites and58 Python tests;make all succeeded without
rebuild. All1274 local documentation links resolved;git diff --check passed.
Original ISO read-only;no original execution/emulator observation,asset
conversion,profile/save mutation or original binary/dialogue distribution.
ID2 changed no native files;concurrent ID1/native work preserved.

Next:remaining dispatch tail/debug opcodes and full opcode-coverage inventory;
heap initialization/free/ownership,source/pointer/numeric/FPSCR/capacity/context/
time domains and original data/memory/allocation/branch/multicredit traces.
Complete-game documentation and requirements remain incomplete.


## 2026-10-12 — ID1 partner target/footmark and SDK distance contracts

Added [footmark clear/ID](behavior/FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md),
[live target wrappers](behavior/FIELD_PARTNER_LIVE_TARGET_DISTANCES.md) and
[SDK paired distance](behavior/FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md).
Complete local bodies separate flag-word ID,bank reset and live-target selection;
preserve repeated candidate/Mario reads and3D/planar captured-pointer differences.
SDK distance uses paired accumulation and one single refinement,distinct from
the direction-query norm. Public Dolphin paired interpreter semantics were
cross-checked and linked;no emulator code copied into native implementation.
Source/numeric environment/domain and actual following trajectories remain open.

Validation:verify_field_partner_targets.py passed six whole DOL hashes,55
instructions,three constants and two actual lecture imports. Repeated metadata
byte-identical;Python compilation and prior direction/executor verifiers passed.
General verifier passed1980 instruction anchors;all1052 behavior links resolved.
Gekko check passed528 files/73097 instructions/439 paired-single operations with
zero changes or unknowns. Native test source event_controls_tests.cpp arrived;
make test passed31 CTest and58 Python tests,resolving the previous CMake failure.
make all succeeded with no rebuild required. Original ISO read-only;no original
execution/numeric seed measurement,asset conversion,profile/save mutation or
original binary/dialogue distribution. Only ID1 target/footmark lock released;
concurrent native and ID2 changes preserved outside this scoped commit.

Next:front-wall/surrounding/floor/fly and homing/fall/jump/behind helpers;
read-index/source/status/flag/slot/owner writers;planar angle/distance/trig and
exact Gekko GQR/FPSCR/estimate domain;original trajectories and full field/battle
teardown. Full documentation/remake remain incomplete;complete-game requirements
remain deferred until sufficient system coverage.

## 2026-10-12 — ID2 event tail/debug and complete local opcode inventory

Added [tail/debug contracts](behavior/EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md)
and [all119 coverage inventory](behavior/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md).
Complete113..118 plus WAIT_WHILE11 and implicit1/3 cover no-op/raw metadata,
repeated wait resolution,unchecked active slot lookup/256 probes and distinct
handler1 yield. Complete debug formatter uses its own signed operand routing,
raw flag intersections,exact23 templates,CR6 varargs and original UW/GSW/LSW
hex argument anomalies;it formats shared scratch without local screen output.
All119 actual table targets map to reviewed local contracts. Generic “other
local opcode bodies” blocker is now closed;EVT-01 remains open for full source/
address/callback/platform/domain/runtime binding. Only ID2's binary-validated
scoped documentation lock released;native full host gate remains pending.

Validation:verify_event_dispatch_tail.py passed nine containing/complete/linked
hashes,164 anchors,145 spans including119 table words and23 NUL-terminated
formats. Repeated reports byte-identical;Python compilation passed. All119
inventory rows match actual pinned targets and linked contracts. General
behavior verifier passed1980 anchors;IF and data/memory verifiers passed;
all1446 local documentation links resolved;git diff --check passed. make all
rebuilt concurrent event_control_opcodes/event_dispatch/native3DSX successfully.
make test cannot configure:CMakeLists.txt:132 references missing
`tests/event_control_opcodes_tests.cpp`. Fixture still absent at final check;
58 Python tests passed independently. Exact failure is communicated in inbox;
ID2 changed no native/CMake/test files to repair concurrent implementation work.

Original ISO read-only;no original execution/emulator observation,asset
conversion,profile/save mutation or original binary/dialogue distribution.
Concurrent ID1/native files and reservations preserved. Next:retry full host
test when native fixture arrives;manager/context/capacity initialization,heap
free/sharing,source/callback/address translation,clock/pause/input/numeric/FPSCR
domains and original119-opcode/Jump/story traces. Complete-game documentation
and requirements remain incomplete.

## 2026-10-12 — ID2 event initialization/reset and SDK allocation provenance

Added [context init/reset](behavior/EVENT_CONTEXT_INITIALIZATION_AND_RESET.md)
and [bootstrap heap/callers](behavior/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md).
Complete Init establishes two256*1b0 entry arrays (216KiB payload),separate
clock snapshots,32-word/three-flag clear extents and shared reset order. ReInit
bulk-clears selected array only,preserves work/flags/live schedule count/ID/speed,
resets shared allocation count/manager-active and invokes no-op evt_msg_init.
It does not perform cancellation copy-back or buffer release. Initialized ID1/
speed1.0 are DOL data rather than init writes. Complete SDK first-fit allocator/
wrapper contrasts with map best-fit;four direct DOL call sites pin battle-init
selector-before-reset and exit reset-before-field-selector/StartAll4 request.
Only ID2 scoped lock released;remaining source/context/heap/domain/runtime
requirements stay open and unrelated ID1 work is preserved.

Validation:verify_event_context_initialization.py passed eight whole/linked
hashes,139 anchors,two static/startup spans and all four branch-shaped direct
DOL call sites across seven text sections. Repeated metadata byte-identical;
Python compilation,general behavior verifier1980 anchors and root verifier
passed;all1483 local documentation links resolved;git diff --check passed.
Initial make test configured32 suites but failed terrain assertions in core:387
and field_movement:48. After concurrent native updates,fresh make test passes
ALL32 CTest suites and58 Python tests. Earlier missing fixture and terrain
assertions are now resolved,as recorded in inbox. make all rebuilt concurrent
native game/3DSX successfully. ID2 changed no native/CMake/test code.

Original ISO read-only;no original initialization/heap/scene execution,
emulator/profile/save mutation,asset conversion or original binary/dialogue
distribution. Next:clock/GP/pause/input/context producers,capacity/stack/label/
free-slot/source restrictions;SDK/map heap initialization/free/coalescing/sharing;
REL/indirect scene/callback/address translation and original reset/cancellation/
Jump/story traces. Complete-game documentation and requirements remain incomplete.


## 2026-10-12 — ID1 partner front-wall query and ordered filters

Added [front-wall query](behavior/FIELD_PARTNER_FRONT_WALL_QUERY.md) and
[filters/status](behavior/FIELD_PARTNER_FRONT_WALL_FILTERS.md). Complete local
query covers upper/lower origins,independent filter choice,distance quantization,
normal projection,asymmetric hit admission and X/Z stores. Ordered attribute/
hidden-name/global-status/height and owner leaves are explicit. Corrected older
OVERWORLD_WALL_MOVEMENT labels from selected original backend stores:hit point
is query+24,normal+30. Inbox breadcrumb requests native provider ABI review;
no already-correct native vector fields swapped by ID1. Full ray backend is
fingerprinted but only selected result-layout/filter regions documented here.

Validation:verify_field_partner_front_wall.py passed eight containing DOL
hashes(seven local bodies complete,backend selected regions),62 instruction
anchors,16 constants,four data spans and two lecture imports. Repeated metadata
byte-identical;Python compilation and prior executor verifier passed. General
verifier passed1980 instruction anchors;all1208 behavior links resolved at the
check. Gekko audit passed536 files/74519 instructions/455 paired-single ops,
zero changes or unknowns. Initial make test failed core_tests.cpp:609
stepJump.jumpHeight>0 during concurrent native edits;after its new assertions/
state arrived,rerun passed32 CTest and58 Python tests. make all succeeded with
no rebuild required. Exact logs:/tmp/id1-front-wall-test.log and
/tmp/id1-front-wall-test-rerun.log. Original ISO read-only;no original execution/
emulator observation,asset conversion,profile/save mutation or original binary/
dialogue distribution. Only ID1 scoped front-wall lock released;concurrent
native/ID2 changes preserved outside this scoped commit.

Next:surrounding/floor/fly and full ray backend/traversal/mesh numerics;
homing/fall/jump/behind services and source/owner/flag/index domains;original
collision trajectories and complete field/battle teardown. Full documentation/
remake remain incomplete;complete-game requirements deferred until sufficient
system coverage.

## 2026-10-12 — ID2 event timestamp producer and full outer update order

Added [retrace clock publication](behavior/EVENT_RETRACE_TIME_PUBLICATION.md)
and [outer/update order](behavior/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md).
Complete callback clock prefix publishes raw-TB delta,clamped total bank,
GP10/window-gated admitted clocks,independent low-system-mask banks,ungated
by those masks event38 bank,field-only40 bank and unconditional raw baseline/
callback argument stores. Freeze prevents later tick catch-up. Complete raw
reader/window predicate and scoped bootstrap zero-bank/refresh0→60 else50/VI
registration are pinned. Complete main/StMain/display local bodies fix initial
update,33-call order,input-before-battle-before-event,profiling words,readiness
yield and idle channel/time/sequence/flag tail. Callback reset/button/rumble/
audio tail,hardware cadence and called subsystem bodies remain separate.
Only ID2 scoped lock released;EVT-01 integration/source/domain/traces stay open.

Validation:verify_event_time_publication.py passed nine complete/containing
hashes,247 anchors and SDA setup span;repeated metadata byte-identical;
Python compilation,general verifier1980 anchors and manager verifier passed.
make test passed33 CTest suites and58 Python tests. make all rebuilt the existing
original asset pack (40714 map triangles,1247 preserved samples) and3DSX;
compiled packs remain outside source history. New/updated timing contract links
all resolve. Repository-wide scan1511 local links found one concurrent missing
FIELD_PARTNER_SURROUNDING_WALL.md link from OVERWORLD_WALL_MOVEMENT.md;inbox
records the exact dependency. git diff --check passed. ID2 changed no native,
asset pipeline,conversion choices,profile or saves. ISO remained read-only;
no original execution/cadence measurement or binary/dialogue distribution.

Next:resolve shared contract link on arrival;hardware clock/VI/thread/GP10/
window/state/system-mask producers and publication/interrupt domains;callback
reset tail,SDK/map heap free/sharing,source/REL/callback/address translation and
original input/pause/idle/retrace/multicredit/Jump/story traces. Complete-game
documentation and requirements remain incomplete.


## 2026-10-12 — ID1 partner surrounding-wall binding and dot correction

Added [surrounding-wall contract](behavior/FIELD_PARTNER_SURROUNDING_WALL.md):
actual stay→wrapper binding,mode1,width6 override,speed-independent rays,
global Mario view/filter ownership,fixed two-pass origins,ordered candidates,
local numerical rules and four complete SDK vector leaves. Corrected existing
OVERWORLD_WALL_MOVEMENT claims from the original US body:the first vector is
saved UNCLAMPED before cap,and ordered dot>=0 SKIPS the second correction;
negative/unordered takes half-add under nontrapping continuation. Inbox gives
native owner concrete same/opposite-direction fixtures;native code was not
edited by ID1 and still had the reversed predicate/capped dot operand at check.

Validation:verify_field_partner_around.py passed seven complete body hashes,
83 instruction anchors,13 constants,five data spans and two lecture imports.
Repeated metadata byte-identical;Python compilation,prior front-wall verifier
and existing wall verifier passed. General verifier passed1980 instruction
anchors. All1237 behavior links resolved at the snapshot;new contract also
resolves ID2's historical concurrent missing-target notice. Gekko audit passed
543 files/75201 instructions/496 paired-single ops,zero changes or unknowns.
Initial make test failed CMakeLists.txt:142 because concurrent native source
referenced absent tests/event_data_opcodes_tests.cpp;after it arrived,rerun
passed34 CTest suites and63 Python tests. make all succeeded/rebuilt3DSX.
Exact logs:/tmp/id1-around-test.log,/tmp/id1-around-test-rerun.log and
/tmp/id1-around-build.log. Passing native tests are not an original execution
oracle or proof of corrected surrounding-wall parity. ISO remained read-only;
no original execution,asset conversion,profile/save mutation or original
binary/dialogue distribution. Only ID1 scoped lock released;concurrent native/
ID2 changes remain outside this scoped work.

Next:native owner applies documented dot/operand correction;full scene/ray
backend,special filters,floor/fly/homing/fall/jump and state/math domains;
original collision trajectories and field/battle teardown. Complete-game
behavior documentation and requirements remain incomplete.

## 2026-10-12 — ID2 map/SDK release,heap bootstrap and pool reset

Added [map free/battle pool](behavior/MAP_BUFFER_FREE_AND_BATTLE_POOL.md),
[five-pool bootstrap/clear](behavior/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md) and
[SDK release/census](behavior/SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md). Complete
local map free merges one successor then a linked predecessor with header32
accounting;SDK allocated unlink/address insertion uses exact physical adjacency
and total-size addition. Their null/double-free/stale-header policies differ.
Complete memory init/clear and SDK init/create/destroy/arena leaves pin five
config records,two-pass fixed/percent assignment,ignored recreated handles,
primary root inside SDK heap1 and400KiB nested battle reservation. Fifty direct
DOL sites constrain caller breadcrumbs;full scene/REL/indirect/event-pointer
ownership is not inferred. Only ID2 scoped lock released;EVT-01 remains open.

Validation:verify_event_heap_release_and_bootstrap.py passed20 complete/linked
body hashes,205 instruction anchors,two SDA/config spans and all50 branch-shaped
DOL direct sites,including45 _mapFree callers. Repeated metadata byte-identical;
Python compilation,general verifier1980 anchors,prior data/memory and context
verifiers passed. make test passed35 CTest suites and63 Python tests;make all
rebuilt concurrent native render/save/3DSX successfully. All1559 local
documentation links resolved;the previous surrounding-wall missing target has
arrived and is now resolved. git diff --check passed.

Original ISO read-only;no original free/heap/scene execution or ownership trace,
profile/save mutation,asset pipeline/conversion choice changes or original
binary/dialogue distribution by ID2. Concurrent native/ID1 files and locks
preserved. Next:per-event/shared user-pointer/scene/REL ownership and alias/cycle/
capacity/numeric/arena/platform domains;clock/VI/thread/gate/state writers and
original reset/free/cancellation/Jump/story traces. Complete-game documentation
and requirements remain incomplete.


## 2026-10-12 — ID1 used partner stay floor/fly postpasses

Added [fly height](behavior/FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md) and
[ground/motion/support](behavior/FIELD_PARTNER_GROUND_POSTPASS.md). Complete
local stay-tail graphs cover head/floor request order,suppression asymmetry,
ordered [-5,0) smoothing interval,owner pointer/read admission,direct floor
snap,discard/latches,fall request and named-support preparation. Complete local
partyChgMot/classifier include old-substate cleanup,always-set marker and exact
25-name/null table. Selected floor-query entry proves mask02000000 null return
before output reset,so cached e4/e8 remain observable. Full head/floor query
algorithms and appear-position engine remain explicitly open,not inferred from
fingerprinting their containing bodies.

Validation:verify_field_partner_postpass.py passes six containing DOL hashes
(four complete local bodies,two selected query regions),78 independently pinned
instructions,12 constants,three data spans including26 exact table words,and
two lecture imports. Repeated metadata byte-identical;Python compilation and
prior surrounding verifier passed. General verifier passed1980 instruction
anchors;all1257 behavior links resolved at snapshot. Gekko audit passed549
files/76334 instructions/536 paired-single ops,zero changes or unknowns. make
test passed35 CTest suites and63 Python tests;make all succeeded with no rebuild
needed. Logs:/tmp/id1-postpass-test.log and /tmp/id1-postpass-build.log. These
are native/static checks,no original execution or measured flight/timing evidence.
ISO stayed read-only;no asset conversion,profile/save mutation or original
binary/dialogue distribution. Only ID1 scoped lock released;concurrent native/
ID2 work preserved outside this commit.

Next:complete head/floor queries and appear-position,out-of-screen services,
all homing/fall/jump/fly handlers,state/flag/source/numeric domains and original
collision/flight/transition observations;native surrounding-dot parity correction.
Full documentation and complete-game requirements remain incomplete.


## 2026-10-12 — ID1 complete partner ceiling query and scalar ray wrapper

Added [ceiling query](behavior/FIELD_PARTNER_CEILING_QUERY.md) and
[scalar ray wrapper](behavior/FIELD_SCALAR_RAY_WRAPPER.md). Complete local
head body specifies .3-width four sample offsets,first-success short-circuit,
raw ordered height cap and all success/failure stores. V is sampled X/Z plus
captured input Y,not intersection point;all-null still writes fourth sample
and height0. Actual ground/fly consumption is explicit,including fly ignoring
height and using preserved inputY. Complete scalar wrapper covers six-float/
callback/seven-pointer ABI,null-output preservation and ordered length/point/
normal publication. Corrected initial lock's inferred wrapper name to actual
hitCheckFilter. Backend hash/anchors close selected regions only,not traversal.
Updated prior postpass links without changing concurrent native behavior.

Validation:verify_field_partner_head.py passes four containing hashes (three
complete local bodies,backend selected regions),89 instructions,six constants,
two zero-template spans and two lecture imports plus actual postpass call sites.
Repeated metadata byte-identical;Python compilation and prior postpass verifier
passed. General verifier passed1980 anchors;all1275 behavior links resolved at
snapshot. Gekko audit passed550 files/76408 instructions/536 paired-single ops,
zero changes or unknowns. make test passed37 CTest suites and63 Python tests;
make all succeeded with no rebuild needed. Logs:/tmp/id1-head-test.log and
/tmp/id1-head-build.log. ISO read-only;no original execution/ceiling trajectory
measurement,asset conversion,profile/save mutation or original binary/dialogue
distribution. Only ID1 scoped lock released;concurrent native/ID2 changes kept
outside this scoped commit.

Next:complete partner floor query,appear-position/out-of-screen services,
homing/fall/jump/fly handlers and state/source/numeric domains;original ceiling/
floor/transition traces and native surrounding-dot parity correction. Full-game
documentation and eventual complete-game requirements remain incomplete.

## 2026-10-12 — ID2 disc-thread GP10 readiness and system/audio requests

Added [disc-thread gate](behavior/EVENT_DISC_THREAD_READINESS_GATE.md),
[system-message presentation](behavior/DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md)
and [audio cover requests](behavior/AUDIO_DISC_COVER_STATE_REQUESTS.md).
Complete local thread/status table distinguishes preserve status1 versus clears/
error selectors,A/B/C live queries,retrace change observations,count10/11,
card override,audio-before-override and alternate error-mode key/reset requests.
Complete card/audio/font routing/init leaves,scoped thread startup and exact
matrix/color/position/request bindings are pinned. Four original US-default
system-message ranges/lengths/hashes are recorded in docs/ASSETS.md;message
bodies/font bitmaps are not extracted or invented in source. Retained selector
becomes a pointer and is not repaired on preserve paths. Only ID2 scoped lock
released;SDK/ROM-font/VI/media/thread/render/audio/source/domain/traces stay open.

Validation:verify_event_readiness_gate.py passed11 containing/complete/linked
body hashes,184 anchors,13 data spans and four whole message resource hashes;
repeated metadata byte-identical;Python compilation,general verifier1980 anchors
and prior time verifier passed. Initial make test hit missing concurrent
 event_memory_opcodes_tests.cpp at CMakeLists.txt:162. After the fixture arrived,
fresh make test passed ALL38 CTest suites and63 Python tests,so that missing-
file gate is resolved. make all rebuilt event_memory_opcodes/native3DSX and
passed. All1618 local documentation links resolved;git diff --check passed.

ISO read-only;no original interruption/media/thread execution or cadence trace,
profile/save mutation,native/test/asset pipeline/conversion choice changes or
original binary/dialogue distribution by ID2. Source-versioned resource evidence
contains addresses/hashes only;inspection and metadata remain /tmp/id2-gate-*.
Concurrent work/locks preserved. Next:other GP10/window/gate/state writers,
VI/OS/media/ROM-font/font-layout/projection/playback backend,source transitions/
selector/alias/interrupt domains and original freeze/recovery/input/audio/Jump/
story traces. Complete-game documentation and requirements remain incomplete.


## 2026-10-12 — ID1 complete partner floor/support and slope pipeline

Added [floor query](behavior/FIELD_PARTNER_FLOOR_QUERY.md),
[slope numerics](behavior/FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md) and
[heading conversion](behavior/FIELD_PARTNER_FLOOR_HEADING.md). Complete local
five-sample/global-filter graph covers fixed origins,raw pointY quantization,
partial projected-angle writes before50-degree rejection,duplicated admitted-
normal pipeline,ordered drop/near-height tests,strict highest-support replacement
and cache outputs. Metadata can come from a later rejected sample;selected
support resets a later excessive-drop e8 write. Complete heading/height leaves
and exact numerical constants are pinned;full backend/library/platform/domain
and original trajectories remain open. Prior postpass links now point to the
floor contract. No concurrent native policy was changed by ID1.

Validation:verify_field_partner_ground.py passes four containing hashes (three
complete local bodies,backend selected regions),100 instruction anchors,28
constants,four vector/numeric spans and two lecture imports plus actual postpass
calls. Repeated metadata byte-identical;Python compilation,prior front-wall
filter/height and postpass verifiers passed. General verifier passed1980 anchors;
all1304 behavior links resolved at snapshot. Gekko audit passed551 files/76486
instructions/536 paired-single ops,zero changes or unknowns. make all succeeded
with no rebuild needed. make test failed CMakeLists.txt:167 missing concurrent
native tests/event_tail_opcodes_tests.cpp;no host-pass claim for that run.
Exact logs:/tmp/id1-ground-test.log and /tmp/id1-ground-build.log. ISO read-only;
no original execution/slope trajectory measurement,asset conversion,profile/save
mutation or original binary/dialogue distribution. Only ID1 scoped lock released;
concurrent native/ID2 work preserved outside this scoped commit.

Next:recheck native host tests when the missing fixture arrives;appear-position/
out-of-screen services,homing/fall/jump/fly handlers,state/cache/flag/source and
numeric domains;original support/flight/transition observations and full ray
backend. Full-game documentation and complete-game requirements remain incomplete.


ID1 floor-query validation follow-up:the missing native tail fixture arrived,
so make test was rerun. It now fails compilation at tests/event_tail_opcodes_tests.cpp:31:
entryAddress lambda has(void*) but service expects(void*,EventEntry&). Exact
rerun log:/tmp/id1-ground-test-rerun.log. No passing host-test claim. Inbox
records the concrete native fixture mismatch;ID1 leaves concurrent native code
untouched and continues original behavior recovery.


ID1 floor-query host validation completion:the native tail callback fixture
signature was corrected by its owner. Fresh make test now passes39 CTest suites
and64 Python tests (log /tmp/id1-ground-test-final.log). Earlier missing-file
and callback mismatch failures above are historical/resolved;make all succeeded.
No native files were changed by ID1. Next recovery proceeds with appear-position/
out-of-screen and remaining movement/backend/domain/original trajectory work.
