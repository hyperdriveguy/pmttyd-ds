# JUMP-01 — delivery notes and implementation breadcrumbs

Append-only history for [JUMP-01](JUMP-01.md): documentation-agent replies, native implementation breadcrumbs, validation/host-gate notes and corrections, in the order they were recorded in the request inbox. Moved verbatim on 2026-10-12; nothing was edited or removed. The five native source-audit asks at the end of the inbox section were relocated to FIELD-RECOVERY-01, MAINLOOP-01 and SAVE-01 (audit request 1 stays in JUMP-01).

Recovery implementation breadcrumb (2026-10-12): `battle_badge_recovery.cpp`
and its suite port raw-actor amount queries and resolved HP/FP recovery events.
Commit8366eb6 corrects shared partner FP recovery's repeated Mario lookup/maximum
capture. Event wrappers preserve translated-1 early return,discarded part and
post-resolution HP bookkeeping gate;they do not schedule delayed effects or
reset retained damage. Body/scale/height/parts/effect wrappers and the original
child/brother recovery tail remain next. Existing producer/pose/effect/pouch/
scheduler and executed recovery trace requests remain open through EVT-01,
STATUS-01 and DMG-02.

Private takeoff binding breadcrumb (2026-10-12): the new composed
jumpTakeoffPositionEvent overload uses real sampling/choice cores and a local
CandidateBuffer per invocation. CandidateChoiceOutputs preserves independent
service context while routing local unit/part destinations. Integration tests
cover geometry/filter/sort/choice/full coordinate output and nested calls with
different selections;outer candidates survive inner sampling. No native fixed
takeoff policy or RNG service was supplied. Registry/source/math/audio/scene
bindings and original execution remain open. EVT-01 scheduler recovery is now
reserved by ID2 and remains the original Jump timer replacement dependency.

Sampling implementation breadcrumb (2026-10-12): `battle_candidate_sampling.cpp`
and its suite implement delivered row geometry,ordered A/C compaction,part
suppression,occlusion and permutation ordering. Tests compose actual sampler with
chooser under takeoff's exact masks/null weapon/mode4,without RNG service. Native
registry/status/part/source/geometry/information/numeric providers and original
execution/domain traces remain unbound. Next is a private-buffer takeoff adapter,
then original Jump graph/EVT-01 scheduling rather than a fixed impact timer.
Read-only body inspection observed selection reset at800ff94c after construction,
initial count store800ff950 and signed reload800ff954;please pin that mutation
order in the sampler contract/verification anchors. Inspection artifacts remain
under /tmp. Continue existing requests for original source/mutation/pause/input/
position/RNG traces and actual native owners.

Candidate-choice breadcrumb (2026-10-12): `battle_candidate_choice.cpp` and
its suite implement all documented weights/weapon overrides and deterministic/
weighted final branches. Takeoff mode4/null weapon is tested without RNG draws;
physical ties and the unusual R<=0 draw boundary are preserved. Numeric,status,
resource,item data and shared RNG services remain explicit. Sampler implementation
is next;native full unit/part/row/menu/source-state binding and actual original
choice/takeoff traces remain requested. EVT-01 remains necessary before replacing
prototype impact scheduling with original Jump execution.

Selection implementation breadcrumb (2026-10-12): `battle_selection.cpp` and
its host suite implement signed retained-target/permutation lookup and ordered
outputs,full cursor table and queried-actor/captured-menu/current-acting-unit
ownership. Address providers retain wrapped unsigned32 arithmetic;unsupported
cursor selection reports explicit native invalid status without fallback weapon.
Native original menu entry/cursor/type/target producers remain requested;these
helpers do not infer eligibility or resample. Newly delivered candidate sampling/
choice contracts are next,followed by original Jump graph scheduling. EVT-01
remains the critical native impact-timer replacement dependency.

Cost implementation breadcrumb (2026-10-12): `battle_weapon_cost.cpp` and its
host suite implement delivered scaling,reduction,affordability,payment and byte/
pouch arithmetic. Payment tests reuse the existing shared partner FP owner service
and a nonzero Mario registry slot. Original menu/equipment/pouch/AP/action-record
owners and pow/fctiwz/FPSCR extremes remain unbound. Please retain the existing
requests for those producers/lifetimes and original affordability/payment traces;
zero costs still request action-count/AP services and payment does not perform
CheckWeaponCost. Next native dependency is selected-target/cursor ownership plus
EVT-01 original event scheduling;default harness fields are not original menu data.

Takeoff implementation breadcrumb (2026-10-12): `battle_jump_takeoff.cpp`
and `battle_jump_takeoff_tests.cpp` port the complete documented local wrapper.
It captures work,keeps private sample identity/chooser mode4,branches on signed
buffer count,retains requested Z/homeY,uses pointer identity and rereads signed
side bytes before offsets. Sampling/chooser services must initialize count and
readable outputs;no native candidate policy was invented. Original sample/choice
core closure and RNG/output/return ownership remain explicitly requested below.
The resulting integer coordinates compose with the approach query in host tests,
but native script scheduling and actual original scene/unit positions remain
unbound. EVT-01 scheduler/dispatcher closure is the next integration dependency.

Native position breadcrumb (2026-10-12): `source/battle/battle_position.cpp` and
`tests/battle_position_tests.cpp` now implement home/visible/part world/hit leaves,
linked part lookup and integer GetHitPos event outputs. Semantic views preserve
owner-supplied storage and scale/output aliasing;native coordinates remain
unbound. Takeoff wrapper is next. Please recover BattleSamplingEnemy and
BattleChoiceSamplingEnemy core admission,capacity/count/return/output initialization,
mode4 and RNG policy requested by BATTLE_JUMP_TAKEOFF_POSITION.md;do not substitute
nearest displayed target for the original private sampling buffer. Source scale/
facing/part-world/pose writers and actual original target/takeoff traces remain
requested through JUMP-01/DMG-02.

Implementation breadcrumb (2026-10-12): `battle_move_position.cpp` now ports
GetMoveFrame/MovePosition,including distinct count/speed policies and the raw
query-output sentinel. Commit24baff7 fixes MarioJumpPosition's baseline setter
sentinel handling;approach module commit:e2754c0. `battle_movement_sound.cpp`
ports empty/retained/alternating sound control,end and unit wrappers,with an
adapter tested against MovePosition. This supplies observable original request
order,not actual decoded audio. BtlUnit_snd_se_pos,psndSFX_pos/Off,original keys/
macro banks,record source writers and execution traces remain requested through
DMG-02/JUMP-01;please provide the missing called-service contracts. The approach
and motion helpers remain unbound to native original scripts pending EVT-01,
unit/part/position owners and numeric services. Do not infer native impact timing
from the derived query21/motion20 example or regenerate samples as replacements.

Further ID2 answer:[Takeoff](../behavior/battle/BATTLE_JUMP_TAKEOFF_POSITION.md) closes
the mixed target-hit X/Z + actor-home Y output,private null-weapon candidate
sample masks/choice4,signed count gate,closer-X tie rule and pointer/return/side
offset branches. Candidate cores/RNG/validity remain service dependencies.
[GetMoveFrame/MovePosition](../behavior/battle/BATTLE_MOVE_FRAME_AND_POSITION.md) close
ceil-like query versus floor-and-speed-adjusted motion counts,raw/float argument
differences,first-call integration,horizontal-only baseline and final XYZ snap/
yield. Query fallback speed does not mutate/repair unit speed. Actual ordinary
calls are automatic movement,type0/-1 and query speed0;abs(query-22) is retained.
[Position queries](../behavior/battle/BATTLE_UNIT_PART_POSITION_QUERIES.md) and
[movement sound](../behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md) close called local
geometry/core/wrappers and their ordered count/retained voice/stop interfaces.
TAKEOFF/MOVE/POS/MOVE-SE requirements give acceptance cases. These complete
local callbacks do not close candidate/math/audio/source/domain/scheduling
dependencies,selected target/cost/stylish/aftereffect/badge tail or original traces.

Native implementation breadcrumb (2026-10-12): `source/battle/battle_jump_motion.cpp`
and `tests/battle_jump_motion_tests.cpp` implement the delivered local motion,
numerics and sound-loader operations with explicit service callbacks. Event
operands resolve on every invocation,phase1 advances/yields at countdown0,and
mode1 retains prior baseline. No native second-hit timer was added. Original
angle/distance/sin/fmod/fctiwz/FPSCR behavior,sound source writers/playback,
unit position/status owners and VM/request cadence remain necessary bindings.
Existing requests for original trajectory/input/AC traces remain open.
MarioJumpParam is also implemented in the same module,including discarded Y
resolution,raw count destination,rounded distance correction,wrapped count
comparison and resulting-count sentinel. Its output composes with motion in the
host tests. Please supply implementation-ready original numeric helper/FPSCR
contracts or differential traces for distABf/angleABf/sin/fmod/fctiwz;current
fingerprints and injected host services do not prove original numerical equality.

Further ID2 answer:[MarioJumpPosition motion](../behavior/battle/BATTLE_MARIO_JUMP_MOTION.md)
and [rounded numerical operations](../behavior/battle/BATTLE_MARIO_JUMP_NUMERICS.md)
close801093a8/8c0. All seven arguments resolve as integers on every call;first
ordinary setup falls through into its first integration. Modes0/3 both use
phase1,but use floating versus integer360/n and phase coefficients0.53/0.8.
BaselineXYZ and sine-offset visibleY remain separate. Exact countdown0 returns1
(advance+yield),snaps only targetY,and resets Phi/G for later owners;negative
counts have no n<=0 completion. Mode1 skips setup and queries AC possibility
before its distinct phase2/3 route. There are no direct AC count/result/main/
option stores or pad-history calls in this body. Original ResultAC remains later
in the script,after its window/2 wait. MJP-MOTION/MJP-NUM requirements provide
implementation cases without invented rendering-frame/impact ticks.

[Sound-mode loader](../behavior/battle/BATTLE_UNIT_SOUND_MODE.md) closes the local record
schema,type2/-1 source words and conditional setup playback request. Word18c
is playback input,not proved an allocated voice handle. Body-local sound return
is ignored;source writers,__psndSFXOn resources/voice lifetime and audio traces
remain open. Next graph dependencies are Takeoff/MoveFrame/MovePosition and
selected-target/cost/stylish/aftereffect/badge-tail services,original reachability/
domain/numeric/input/state traces. Generic and Mario local motion contracts do
not by themselves close those services or the original encounter/game.

Further ID2 answer:[MarioJumpParam](../behavior/battle/BATTLE_MARIO_JUMP_PARAMETER.md)
closes80109c68/19c. It resolves X/Z via evtGetValue as signed integers,resolves
but discards Y,reads visible current X/Z and requests distABf. Requested count36
is a minimum candidate,not fixed:when distance>60,add truncated binary32
0.15000000596046448*(D-60),then preserve signed maximum with original request.
Zero request starts from truncated distance/current speed. Write the chosen count
back through the raw operand except when resulting value is sentinel-250000000.
No unit remaining-count/speed/position/AC state field is written by this body.
The subsequent stylish brother copies the **updated** count,so8/20/32 applies
only if36 remains36. MJP-PARAM-01..04 give scoped acceptance cases. The distance
helper's frsqrte/FPSCR fidelity,MarioJumpPosition and original traces remain open.

Partial answer from ID2 (2026-10-12):
[complete268-command Jump request graph](../behavior/battle/BATTLE_NORMAL_JUMP_EVENT_FLOW.md)
pins target/cost/setup/approach/precheck/motion/result/damage/feedback/recovery/
tail branches. Shared AC count is distinct from script local10:count<=1 success
requests CheckDamage flags20000,wait2,local10++,then repeats label10 without
new SetupAC/StartAC;count>1 success requests20100 and finishes;failure requests
100 and declares-1 only when local10==0. Next-pass precheck uses100100. No direct
target-HP getter or post-damage result gate occurs before repeating. Preserve
repeated precheck rather than guaranteeing a second damaging hit. Missing target
reaches actual label99;avoidance skips result/damage and converges on label95.

[Result/count/grade/stop services](../behavior/battle/BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md)
close raw destination readers,raw mask1 update,window output,code30 prize-1/
unsigned byte1cbd grade clamp,and TimingA's deferred StopAC state1000. The stream
has no AcSetParamAll/AcSetFlag call and does not write count-window selector2.
[VM/concurrency](../behavior/event/EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md) pins signed
comparisons,raw IF mask and four attached brothers versus one independent camera
inline. Copied stylish locals do not overwrite parent motion values;camera wait10
does not block the parent. JUMP-EVT/JAC/JVM requirements give scoped acceptance
cases,not measured impact ticks or a complete executed tutorial.

MarioJumpParam and MarioJumpPosition are now answered by the newer linked
contracts above,with numeric/audio/execution dependencies still explicit.
Takeoff/MoveFrame/MovePosition local bodies are now answered by the newer linked
contracts above;remaining their candidate/math/audio/domain service composition,
selected target/cost/power-child/stylish/prize-byte/aftereffect/badge
tail internals and original tutorial/menu/input/state traces. Do not implement
the request graph with the native fixed48/42 schedule or copied generic jump
physics. The parent request remains open for that runtime composition.

The implementation now uses the [recovered power callbacks](../behavior/battle/BATTLE_WEAPON_POWER_CALLBACKS.md)
in `source/battle/battle_weapon_power.cpp`. The native harness executes one documented
Jump hit with level1 supplied as an explicit fixture;successful timing records
success but no longer fabricates a two-power hit. This is incomplete attack
sequencing,not a faithful finished Jump or a claim of original starting loadout.

TimingA portable contract tests are `tests/battle_timing_a_tests.cpp`; native
integration boundaries are in `tests/core_tests.cpp`'s `timing()` scenarios.
The recovered `rand`/`irand` arithmetic has an explicit caller-owned seed. Its
presence does not establish complete shared RNG or original save/load ownership.
The harness takes no forced/random routes currently. Battle pad snapshots are
semantic native state,not a claim of original packed ABI compatibility;unknown
snapshot fields still need their own contract. Pad repeat16/8 and inclusive
main/C-stick thresholds are tested independently of UI/global physical repeats.

Next implementation work: use AC-02 answers to bind Hammer's cadence/stop/
damage/presentation to actual scripts and resolve original sound macros.
Use AC-01 answers to
replace harness scheduling and connect original tutorial/story events. Defense,
damage, inventory/event/UI/audio/field systems and all remaining content still
require implementation/closure work; this request file is not a full-game spec.

ID1 build-validation note (2026-10-12,lecture-helper delivery):initial make test
fails in tests/battle_action_services_tests.cpp:91 because raw0xfe363c86 narrows
from unsigned to int in an aggregate;use an explicit signed-word conversion for
that fixture. make all fails in source/battle/battle_action_services.cpp:61 because
std::clamp(signedWord(result),0,6) deduces long int versus int on devkitARM;
explicit int32_t template/bounds preserve the intended signed domain. These are
concurrent native compile errors;lecture/helper binary checks pass. Re-run both
commands after correcting them before recording a successful native validation.

ID1 build-validation follow-up:the native fixture now uses the signed local-work
encoding and clamp uses an explicit int32_t domain. Final make test passed all12
CTest suites plus58 Python tests;make all exited0 with no rebuild. The earlier
compile failures above are resolved. ID1 changed no native files for those fixes.

ID2 selection/cost delivery (2026-10-12), JUMP-01/DMG-01 partial answer:
[selection and weapon](../behavior/battle/BATTLE_SELECTED_TARGET_AND_WEAPON.md) specifies
retained signed target/permutation reads and output callback order, player versus
held-item weapon lookup and all15 cursor dispatch entries. Unsupported cursor
types leave output untouched; do not supply a fabricated default Jump weapon.
[cost and payment](../behavior/battle/BATTLE_WEAPON_COST_AND_PAYMENT.md) supplies every
FP scaling class/reduction/cap, shared Mario FP lookup and low16 stores, separate
affordability query, ordered unchecked payment, pouch AP narrowing/clamps and
byte-before-cap action-record helpers. Zero FP cost still counts an action.
Default Jump/Hammer item/FP/AP fields are zero; payment does not invoke the
check. These are complete local static contracts, not evidence of menu-check
cadence or original execution. `tools/verify_weapon_selection_cost.py` passes
16 complete body fingerprints,11 anchors,two constants,15 cursor targets,two
ordinary weapon checks and three ordinary Jump service calls. Own scoped lock
released; parent requests and other owners' locks remain open.

ID2 candidate service delivery (2026-10-12), JUMP-01/DMG-02 partial answer:
[sampling](../behavior/battle/BATTLE_ENEMY_CANDIDATE_SAMPLING.md) closes all local passes,
metadata/partial row writes, signed count and capacity invariants, side-mask swap
return, geometry and permutation sorting. Return Q is the swap indicator, not
nonempty/count. [choice/RNG](../behavior/battle/BATTLE_ENEMY_CANDIDATE_CHOICE.md) closes
(-1,0) output initialization, ordered weights and weapon overrides, deterministic
physical-index ties and weighted subtraction boundaries. Takeoff mode4/null
weapon consumes no RNG. irand/rand leaves specify shared LCG and zero-bound
no-consumption; seed writers/interleaving and original traces remain open. No
nearest-target or bounded-capacity fallback is established. The standalone
verify_enemy_candidate_sampling.py passes seven complete body fingerprints,
48 anchors and14 constants, including SDA initialization. Own lock released;
lecture/event-scheduler reservations and parent requests remain independent/open.

ID2 ongoing stylish validation note (2026-10-12):new13-body stylish verifier,
existing Jump/candidate verifiers and make all passed. make test currently fails
at CMakeLists.txt:71 because concurrent native registration references missing
`tests/battle_candidate_choice_tests.cpp`;ID2 made no native changes. Please
finish that in-progress test file and re-run before recording a host pass.

ID2 stylish delivery (2026-10-12), JUMP-01/AC-01 partial answer:
[timing/results](../behavior/battle/BATTLE_STYLISH_TIMING_AND_RESULTS.md) specifies the
shared W271c..2734 record, first-call initialization plus same-call t1 polling,
exclusive lower/inclusive upper success, opening history rejection, autoInput,
no-input timeout and ordered result outputs. Both ordinary brothers pass history
count0; timing is callback invocations, not rendering frames. Main return0
repeats/yields,1 completes/yields;GetResult returns2 and does not poll/reset.
[icon](../behavior/battle/BATTLE_STYLISH_TIMING_ICON.md) closes local eligibility and
baseline depth queue versus visible draw requests, icon0xa2 and original scale.
Audience services/source writers/scheduler/display internals/assets and original
execution remain open. Standalone verify_stylish_timing.py passes13 containing
body fingerprints (dispatcher recovery scoped),48 anchors,five constant/dispatch
spans and four ordinary call bindings. ID2 lock released; parent requests and
ID1's lecture scope remain open.

ID2 validation follow-up:the concurrent candidate-choice test file now exists.
Re-run make test passed19 CTest suites and58 Python tests. Initial missing-file
failure above is resolved;ID2 changed no native source/build registration.

ID2 aftereffect delivery (2026-10-12), JUMP-01/DMG-02 partial answer:
[dispatch](../behavior/battle/BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md) closes every local
weighted/chance/force decision and exact RNG/read/call order,with nine actual
Jump/Hammer chance bytes and both ordinary Jump callers. Aftereffect dispatches
stage hazards,not a generic status/resource cleanup. Zero chances still consume
mandatory draws. [stage leaves](../behavior/battle/BATTLE_STAGE_AFTEREFFECT_REQUESTS.md)
closes A1/B shared replacement,A2 ignored handle, nozzle weighted/type/liveness
helpers and frame/object gating/registration. Nozzle and fall constructors have
explicit unguarded event-ID reads;do not invent local rollback/fallback policy.
Frame sets stage0x20 before the subsequent object test;opening boss flag0x1000
suppresses object registration. Queued event bodies,stage/source/allocator domain,
seed interleaving and executed hazards remain open. Standalone verifier passes
13 complete body fingerprints,56 anchors,seven data spans and two ordinary
callers. ID2 lock released;parent requests/other owners remain open.

ID2 badge-tail delivery (2026-10-12), JUMP-01/DMG-02/STATUS-01 partial answer:
[32-command tail](../behavior/battle/BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md) closes both
copied-local brothers, sequential HP/FP >=1 gates, effects, brother120/owner125
waits and resource writes/delete. [recovery/geometry](../behavior/battle/BATTLE_BADGE_RECOVERY_SERVICES.md)
closes raw-actor point readers signed min(badgeByte,damageWord), resource wrappers
(including resolved but ignored part), HP bookkeeping attribute and geometry
outputs. [coercion/effect route](../behavior/battle/BATTLE_BADGE_RECOVERY_PRESENTATION.md)
closes scoped1024 float encoding/MULF then INTEGER ADD and recovery effect catalog/
argument binding. Do not use unquantized host floating placement or detached
recovery timers. Full renderer/audio, VM/source/delayed-validity and original
traces remain open. Standalone verifier passes18 containing-body fingerprints
(scoped interpreter/effect routes),71 anchors,13 data spans and all32 event
commands/two brothers;actual Jump waiting-child site80356be0 pinned. Own lock
released;parent requests and ID1's lecture-field reservation remain open.

ID2 EVT-01 validation note (2026-10-12):make test currently passes21/22 CTest
suites but core fails tests/core_tests.cpp:137 at
`jumping.jumpHeight>.07f && jumping.progress.position.x>-.99f` during concurrent
native field movement changes. Python suite did not run after CTest failure.
New event-manager evidence passes;ID2 has not changed native code. Please resolve
the fixture/behavior mismatch and re-run before claiming a full host pass.

ID2 EVT-01 manager delivery (2026-10-12), partial answer:
[complete manager](../behavior/event/EVENT_MANAGER_COMPLETE_LOOP.md) closes context,
raw64-bit tick delta/clamp/substitution, unstable unsigned-priority exchange,
active/live-ID/0x92 eligibility, lifetime once per visit and single-rounded
credit add/subtract-before-dispatch. Dispatcher0 can run again under remaining
credit;1 aborts the whole manager,-1 ends only that entry. The count is live.
[dispatch/waits](../behavior/event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) closes fetch,
common handler outcomes,exact first-call waits/cached USER_FUNC,frame decrement
and msec sign-extended LOW lifetime difference/unsigned division. No fixed one-
physical-frame policy follows from yield. Standalone verifier passes seven
containing-body fingerprints (dispatcher/numeric scope limited),70 anchors and
eight constants/dispatch spans. Constructors/current-table append/initial credit/
restart/type/run-check producers,other opcodes and original traces remain open.
ID2 scoped lock released;EVT-01 stays priority0 and open. make all passed;host
21/22 CTest failure is the concurrent field-movement assertion noted above;
58 Python tests passed independently. No full host-pass claim.

ID2 EVT-01 root-constructor delivery (2026-10-12), partial answer:
[root/typed constructors](../behavior/event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) closes
first-free slot scan,clear/all explicit fields,priority/type/flags narrowing,
global speed copy/credit0,ID wrap ordering and conditional live append. Only
manager-active AND entry.flags0x20 appends;no immediate dispatch or resort.
[labels/run-check](../behavior/event/EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md) closes
signed-count label scanning,opcode1 versus2 termination,low8 label writes and
unused comparison16 (no actual cap). Run-check ignores its entry argument and
invokes evtStopAll for current global level,after append. Valid free-capacity/
label/script domains are invariants,not original null/error fallbacks. Verifier
passes five complete containing-body fingerprints (linked stop identities),62
anchors,four data spans. Own scoped lock released;EVT-01 remains priority0/open
for child/brother/inline/restart and original traces. Current make test passes24
CTest suites and58 Python tests;make all passed. Previous concurrent core:137
field-movement failure is resolved by current validation;ID2 changed no native
files for it.

ID2 EVT-01 descendant/restart delivery (2026-10-12), partial answer:
[descendants/107/110](../behavior/event/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md)
closes full child/brother inherited/reset maps,parent waiting versus owner-only
links,priority narrowing,global speed/credit0,labels and active-manager append.
Child/brother append has no root flag0x20 gate. It specifies zero-ID/run-check
order differences,post-constructor wrapper recopies,brother150 versus inline
message metadata and independent inline ownership. [restart](../behavior/event/EVENT_RESTART_PRESERVATION.md)
closes exact stores/label rebuild/preserved locals,ID/links/flags/timers and global
speed reset without append/cancel. First-call leftovers reset at subsequent
fetch,not restart itself. Standalone verifier passes seven complete containing
fingerprints,81 anchors,five spans;plain107/110 recovered,ID variants108/111 and
other child/control/deletion opcode contracts remain open. Own lock released;
EVT-01 stays open. make test passes25 CTest suites and58 Python tests;make all
passed. No native changes by ID2.

ID2 control-wrapper validation note (2026-10-12):make test currently stops at
CMakeLists.txt:103 because concurrent native registration references missing
`tests/event_dispatch_tests.cpp`. CTest/Python checks were not reached by that
command. make all and new16-body control verifier pass. Please finish the test
file and re-run before recording a full host pass;ID2 changed no native files.

ID2 EVT-01 run/control delivery (2026-10-12), partial answer:
[run/ID variants](../behavior/event/EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) closes independent
92/93 roots with flags0/no same-pass append, copied fields and ID publication;
108/111 end scanners/output order and brother111's omitted150 copy.
[controls/ID scans](../behavior/event/EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) closes94..106
wrappers,child255 yield WITHOUT owner deletion,setter rounding/narrowing,
Other-pointer exclusion,all-match stop/start IDs with maskef and first-match
active CheckID. Raw output is distinct from resolved input. Verifier passes16
containing fingerprints (dispatcher scoped/constructors linked),72 anchors and
25 data/dispatch spans. Own lock released;EVT-01 remains priority0/open for deep
stop/start/delete traversal,remaining opcode/namespace/source/domain policies
and original traces. make all passed;make test's missing concurrent dispatch
fixture is noted above;58 Python tests passed independently. No full host-pass
claim until that fixture is present and checked.

ID2 control validation follow-up:the concurrent dispatch fixture now exists.
Fresh make test passes26 CTest suites and58 Python tests;the missing-file
configuration failure above is resolved. ID2 changed no native files for it.

ID2 EVT-01 lifetime delivery (2026-10-12), partial answer:
[deletion/copy-back](../behavior/event/EVENT_DELETE_AND_COPYBACK.md) closes active gate,
child-before-brother recursion,live slot scans,parent copy-back even on ancestor
cancellation,paired metadata,active clear/memset/counter order and all-match
DeleteID. [stop/start](../behavior/event/EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md)
closes full descendant-before-mask behavior and original compiler-inline context
capture policy,All wrappers and same-pass credit implications. No host rollback/
immediate suspend/refcount behavior is original. Verifier passes six full body
fingerprints,70 anchors,SDA span and paired245-instruction comparison:only seven
flag mutations differ between Stop/Start. Own lock released;other EVT-01 scope
and ID1 reservations remain open. make test passes26 CTest suites and58 Python
tests;make all passed. No native changes by ID2.

ID2 EVT-01 operand namespace delivery (2026-10-12), partial answer:
[integer APIs](../behavior/event/EVENT_INTEGER_OPERAND_NAMESPACES.md),[float APIs](../behavior/event/EVENT_FLOAT_OPERAND_ROUTING.md)
and [saved leaves](../behavior/event/EVENT_SAVED_SWITCH_STORAGE.md) close all four local
reader/setter bodies,GetWork and ten called switch leaves. Signed exact routing
bands differ by API;float APIs must not share an integer-family decoder. Fetched-
word lower-bound coercion also differs. Setters'old-word/old-boolean/raw-value/
float return policies and read-before-store are explicit. GSW0 uses GP174 WORD,
nonzero indices GP578 bytes;LSW0 is a byte. No automatic output sentinel,namespace
bounds or disk write is established. Verifier passes15 full body fingerprints,
94 anchors and seven constants/SDA spans. Own lock released;other opcode/numeric/
capacity/source/persistence and original traces remain EVT-01 work. make test
passes28 CTest suites and58 Python tests;make all passed. ID2 made no native edits.

ID2 EVT-01 loop/arithmetic delivery (2026-10-12), partial answer:
[GOTO/loops](../behavior/event/EVENT_GOTO_AND_LOOP_OPCODES.md) closes4..8/NEXT including
signed pointer/label gate,first16 signed-byte lookup/missing fallback,raw count
push,deferred variable decrement,literal0,threshold-10000000 and BREAK/CONTINUE
lexical/stack distinctions. [assignment/arithmetic/cursor](../behavior/event/EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md)
closes50..64:SET resolved/raw/float,all RHS-first updates,wrapped arithmetic,
MOD's signed-to-single/+0.5/fctiwz before remainder,float single operations and
cursor advance-before-read/output with reload between lanes. No native for-loop
count snapshot,ordinary integer MOD or implicit iteration yield is original.
Verifier passes six containing hashes (NEXT full,dispatcher scoped/APIs linked),
73 anchors and24 spans (20 dispatch targets). Own lock released;other predicates,
readers/numeric/source/domain and original traces remain EVT-01 work. make test
passes29 CTest suites and58 Python tests;make all passed. ID2 made no native edits.

ID2 EVT-01 switch/case delivery (2026-10-12), partial answer:
[Switch/stack/scans](../behavior/event/EVENT_SWITCH_STACK_AND_CASE_SCANS.md) closes34..49
actual dispatch table,raw/resolved pushes,signed stack,state gates and next-header
scans. [Grouped/mask/range](../behavior/event/EVENT_GROUPED_MASK_AND_RANGE_CASES.md)
closes OR43/AND44 sentinels,raw any-bit mask45,group end46,inclusive range47,
BREAK48 and END49. Table order is independently pinned;function address order
must not supply opcode numbering. Scans count34/1 but ignore raw35;case search
stops36..47 and skips BREAK48. Queries precede consumed/group gates;mask is raw,
range loads its second operand after first query. Own lock released;EVT-01 stays
priority0/open for remaining IF/readers/other opcodes,numeric/source/domain and
original traces. Verifier passes eleven full CASE and containing dispatcher
hashes,267 anchors,16 actual dispatch words. make test passes30 CTest suites and
58 Python tests;make all passed. ID2 changed no native code.

ID2 EVT-01 predicate delivery (2026-10-12), partial answer:
[Integer/mask/continuations](../behavior/event/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md)
and [float/string](../behavior/event/EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) close12..33,
all twenty IF helpers and full strcmp. Operand0 is resolved first;operand1 is
loaded afterward from captured base. Masks are raw,strings resolve both/null→
empty,and float NaN acceptance follows exact CR negation (all except equality
accept unordered when comparison proceeds). False scan consumes same-level
ELSE/END;ELSE scan ignores all ELSE markers. All twenty29-instruction false
scanners are byte-identical. String comparator's byte-difference versus word±1
returns,aligned read-through-NUL extent and unsigned byte order are explicit.
Verifier passes22 full/containing hashes,416 anchors,25 spans and shared scanner
comparison. Own lock released;remaining reader/bitwise/other VM opcodes,numeric/
source/pointer/domain and original traces remain EVT-01 work. make test passes30
CTest suites and58 Python tests;make all passed. ID2 made no native edits.

ID2 reply to EVT-01 deletion-counter breadcrumb (2026-10-12):
The already delivered [deletion](../behavior/event/EVENT_DELETE_AND_COPYBACK.md) step5 and
[root constructor](../behavior/event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) allocation
step identify the SAME global word8041e860 = r13(8041cf20)+1940. evtDelete loads
at800368a4,decrements at800368a8 and stores at800368ac after memset. Constructor
store anchors8003752c/800372a4 use900d1940;increment precedes memset. This is
not a captured context member or manager live schedule count8041e85c. Deletion
captures GP-selected context before active testing;its brother array capture
survives recursive calls,while each recursive invocation selects context anew.
Fresh lifetime/root verifiers pass. Original capacity/relationship/clock/context/
cancellation/pause traces remain requested.

ID1 validation breadcrumb,partner direction research (2026-10-12):make test
currently fails CMake generation atCMakeLists.txt:127 because the concurrent
native target references missing tests/event_controls_tests.cpp. make all built
successfully with event_controls.cpp. No native file/placeholder test changed by
ID1;please complete that test source and notify here. ID1 direction/executor
binary verifiers and Gekko audit passed. Source log:/tmp/id1-partner-dir-test.log.

ID2 EVT-01 data/memory delivery (2026-10-12), partial answer:
[Readers](../behavior/event/EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) closes65..73,
all cursor lane/query/load/output ordering;73 is indexed SIGNED integer→single
float,not IEEE indexed load. [Bitwise/time/memory](../behavior/event/EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md)
closes77..90,raw/resolved RHS,captured/delayed destinations,wrapped products
before signed60/1000 division and raw-address versus one/two-query API routing.
[User buffers/allocator](../behavior/event/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md)
closes74..76 and complete local best-fit/split/zero/cache/publish path:failure0
is published,old E154 not freed locally,zero remainder header still written.
No host pointer decoder,length check or automatic per-entry heap ownership is
originally established. Verifier passes nine containing/linked/complete hashes,
269 anchors,28 spans including26 actual table targets. Deterministic100013
inputs per time conversion matched instruction-form versus wrapped-product
truncation formulas (host arithmetic check,no original execution). Own lock
released;remaining VM blocks,heap/source/pointer/numeric/context/capacity domains
and original traces remain EVT-01 work. make test passes31 CTest suites and58
Python tests;make all passed. ID2 made no native edits.

ID1 native-validation follow-up (2026-10-12):tests/event_controls_tests.cpp
is now present. make test passes31 CTest and58 Python tests;the prior missing-
source CMake failure is resolved. Original binary/documentation evidence was not
contradicted by that transient native worktree state.

ID2 native validation breadcrumb (2026-10-12):current make test cannot configure
because CMakeLists.txt:132 references missing tests/event_control_opcodes_tests.cpp.
make all rebuilt event_control_opcodes/event_dispatch and3DSX successfully.
This is a concurrent native fixture arrival dependency;ID2 is changing no
native/CMake/test files under documentation-only scope. Tail/opcode inventory
binary verification passes;full host gate remains pending until fixture arrives.

ID2 EVT-01 tail/inventory delivery (2026-10-12), partial answer:
[Tail/debug](../behavior/event/EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) closes113..118,
WAIT_WHILE11 and implicit1/3 runtime no-ops.118 queries active slots0..255 with
fresh GP context per helper then handler1 regardless of found/not-found;no
capacity gate.115 formats into shared scratch only,uses independent signed
routing/raw-mask values and exact UW/GSW/LSW hex argument anomalies. All23
original diagnostic templates and formatter CR6 inputs are explicit.
[Coverage inventory](../behavior/event/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md) maps
EVERY119 actual target to recovered contracts. Generic missing local opcode
bodies are now closed;EVT-01 stays priority0/open for source/address/callback/
context/capacity/time/heap/numeric domains and original execution acceptance.
Verifier passes nine containing/complete/linked hashes,164 anchors,145 spans
including119 table words and23 templates. Own binary-validated documentation
lock released;make all passed,58 Python tests passed;full make test awaits the
missing concurrent native fixture described above. ID2 changed no native files.

ID1 ray ABI correction (2026-10-12):OVERWORLD_WALL_MOVEMENT.md had the
HitQuery output labels reversed. Original backend computes origin+scaled
ray into query+0x24 at80016454/58;triangle normal is copied/normalized into
query+0x30 at80016474..80 and80016530..68. Normal=+30,hit point=+24.
Partner caller reads+30 normal for its projection. Corrected the doc;native
high-level ray provider should be checked against this ABI,without swapping
already correctly named native vector fields. Full backend/numeric traversal
remains separate. ID1 verifier pins these result-layout anchors.

ID2 native validation follow-up (2026-10-12,context-init research):the previously
missing event_control_opcodes_tests.cpp now exists;configuration/build succeed.
Fresh make test runs32 CTest suites:30 pass,including event_control_opcodes;
core fails tests/core_tests.cpp:387 groundAt({-.5f,2},0,h) expected h≈.2;
field_movement fails its line48 close(m.slope.inclination,30). Current terrain/
slope native edits are concurrent;ID2's documentation-only task changes no
native fixtures/mechanics. Full host gate remains pending for these actual
assertion failures;this supersedes the older missing-file blocker.

ID2 EVT-01 context/init delivery (2026-10-12), partial answer:
[Init/reset](../behavior/event/EVENT_CONTEXT_INITIALIZATION_AND_RESET.md) closes both
complete bodies:Init allocates TWO256*1b0 arrays (216KiB payload combined),
zeros32-word/three-flag banks and resets three scheduling/allocation globals;
ReInit clears ONLY selected existing array,keeps work/flag banks and live schedule
count,sets shared allocation count/manager-active0. Neither resets global ID/
speed,performs entry deletion/copy-back or frees old buffers. Initialized DOL
ID starts1,speed1.0;those values are not assignments in Init/ReInit.
[SDK heap/callers](../behavior/event/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md) closes
__memAlloc/SDK first-fit split/whole-block/zero/cache policy and four direct
DOL calls. battle_init stores GP14=1 immediately BEFORE reset;battle_exit resets
BEFORE its explicit GP14=0 and field StartAll4 request. No full caller/REL/scene
teardown inference. evt_msg_init is a one-instruction no-op. Own binary-validated
lock released;remaining capacity/context/heap/source/clock/platform/domain and
original traces stay EVT-01 work. Verifier passes eight hashes,139 anchors,two
spans and four exact direct call sites;general/root verifiers pass. Native
missing-file gate is resolved;current full host terrain assertion failures are
recorded above. ID2 changed no native code.

ID2 context validation completion (2026-10-12):concurrent native terrain/fixture
updates now resolve both assertion failures above. Fresh make test passes32
CTest suites and58 Python tests;make all rebuilt game/native3DSX successfully.
Missing-fixture and terrain failures are historical/resolved;ID2 changed no
native files. Context/init evidence and documentation link checks also pass.

ID1 urgent surrounding-wall correction (2026-10-12):original marioChkWallAround
800891e0 fcmpo;800891e4 cror2,1,2;800891e8 bf2 makes ordered dot>=0
SKIP second correction;negative/unordered applies half second push (if FP
exceptions permit continuation). PSVECDotProduct8029ec84 confirms an ordinary
positive dot,not a negated dot. First vector copied to stack54 at80088db4–bc
BEFORE cap at80088dd8;dot uses this UNCLAMPED copy. Existing wall doc corrected.
Native source/field/field_wall.cpp currently tests >0 to apply,and field_wall_tests
expect same-direction second push to move -2.5→-7 and reverse push to stay-2.5;
these two outcomes must reverse for original parity. Native owner should update
policy/fixtures;ID1 does not edit concurrent native files. Detailed partner binding
and pinned evidence follow under the ID1 lock.

ID2 EVT-01 time/update delivery (2026-10-12), partial answer:
[Retrace clock](../behavior/platform/EVENT_RETRACE_TIME_PUBLICATION.md) supplies actual
GP38/3c producer:OSGetTime raw-TB delta,GP10/window gate,unconditional raw-baseline
publication,masked auxiliary clock banks,event bank independent of low system
mask,and field-only40 bank. Freeze discards later catch-up ticks because baseline
continues updating. Window null/1000/state0 or>=200 does not freeze;nonzero<200
including negatives does. Scoped bootstrap zero-bank/VI refresh0→60 else50/
post-retrace registration and full raw reader are pinned;hardware rate unmeasured.
[Outer loop](../behavior/platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) closes main,
StMain/display local requests and all33 calls:makeKey,battleMain,evtmgrMain order,
readiness yield barrier,raw profile intervals and complete idle channel/time/
sequence/flag request. Original action commands remain simulation-tick owned;
render/profile callbacks are not their timing source by inference. Verifier
passes nine containing/complete hashes,247 anchors and SDA startup span. Own
binary-validated lock released;VI/thread/clock/window/GP/gate/context/source/
numeric and original input/pause/retrace/multicredit traces remain EVT-01 work.

ID2 timing validation completion (2026-10-12):make test passes33 CTest suites
and58 Python tests;make all rebuilt the existing original asset pack and3DSX.
All ID2 new/updated timing contract links resolve. Repository-wide link check
currently scans1511 local links and finds one concurrent missing target:
behavior/overworld/OVERWORLD_WALL_MOVEMENT.md→FIELD_PARTNER_SURROUNDING_WALL.md. ID2 leaves
that ID1/terrain contract work untouched;this is a shared-document arrival
dependency,not an original-game evidence failure. Timing/general/manager binary
verifiers and Python compilation pass. ID2 changed no native or asset tooling.

ID1 surrounding-wall delivery and validation (2026-10-12):
[Binding/core/numerics](../behavior/partner/FIELD_PARTNER_SURROUNDING_WALL.md) now exists;
the concurrent missing-link notice is resolved. Seven complete local hashes,
83 instructions,13 constants,five spans and two lecture imports pass. Native
owner still needs the exact reversed dot-admission fixture updates above and
uncapped first-vector operand. make all succeeded/rebuilt3DSX;initial make test
failed CMakeLists.txt:142 missing tests/event_data_opcodes_tests.cpp during
concurrent native work (exact log /tmp/id1-around-test.log). This missing-file
failure is separate from ID1's binary checks. Only ID1 scoped lock released;
full terrain/numeric/EVT-01 and original traces remain open.

ID1 surrounding-wall host validation completion (2026-10-12):the missing
concurrent native tests/event_data_opcodes_tests.cpp fixture arrived. Rerun
make test now passes34 CTest suites and63 Python tests;make all succeeded.
Historical missing-file log:/tmp/id1-around-test.log;passing rerun:
/tmp/id1-around-test-rerun.log. Passing tests do not resolve the reversed
native dot predicate:source/field/field_wall.cpp still applies when dot>0 and saves
a capped first push at this check. Exact original acceptance cases are in the
new contract;native owner review remains required for parity.

ID2 EVT-01 heap/release delivery (2026-10-12), partial answer:
[Map release/battle pool](../behavior/memory/MAP_BUFFER_FREE_AND_BATTLE_POOL.md) closes
successor-then-predecessor merge,header32 accounting,early null/used gates and
400KiB primary-map reservation/nested root/free order. [Pool bootstrap/reset](../behavior/memory/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md)
closes five original config records,fixed/percentage two-pass assignment,SDK
creation/destroy/recreation,ignored replacement handles and memClear index1
primary root rebuilding. [SDK free/census](../behavior/memory/SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md)
closes allocated unlink,address-sorted insertion and exact adjacency coalescing,
with50 direct sites (45 map free). SDK free has no map-style null gate and its
sizes include headers;do not share generic free/size semantics. No per-event
ownership/free or complete scene/REL/alias validity is inferred. Own lock
released;remaining source/ownership/arena/platform/numeric/scene/original traces
stay EVT-01 work. Verifier passes20 linked/complete hashes,205 anchors,two
spans,50 exact direct sites. make test passes35 CTest suites and63 Python tests;
make all passed. Previous concurrent surrounding-wall link now resolves;current
full local link check passes. ID2 changed no native or asset pipeline code.

ID1 partner stay postpass delivery (2026-10-12):
[Fly](../behavior/partner/FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md) and
[ground](../behavior/partner/FIELD_PARTNER_GROUND_POSTPASS.md) specify actual mobility==1
vs all-other tail binding,head-before-suppression asymmetry,ordered [-5,0)
no-correction interval,captured owner vs fresh admission pointer,floor snap/fall,
attribute priority and exact25-name support-triggered motion. Floor query can
return null on P0&02000000 before output reset:retain cached e4/e8 semantics.
Ground attr0200 discards even with nonzero P4c;zero byte only adds latches.
partyChgMot uses old-substate3 cleanup and always sets entry marker;no immediate
recursive fall/jump dispatch. Six identities/four complete local bodies plus
selected query regions,78 anchors,12 constants,three spans/two lecture imports
pass;make test35 CTest/63 Python tests and make all pass. Only ID1 scoped lock
released;complete head/floor/appear and motion/geometry/domain/original traces
remain open. Original surrounding-dot native correction request remains separate.

ID1 partner ceiling/scalar ray delivery (2026-10-12):
[Ceiling query](../behavior/partner/FIELD_PARTNER_CEILING_QUERY.md) and
[scalar wrapper](../behavior/field/FIELD_SCALAR_RAY_WRAPPER.md) establish four samples,
first nonnull short-circuit (not nearest across probes),no local distance
quantization,raw ordered height cap and success/failure writes. Success V is
sampled X/Z plus captured INPUT Y;failure still writes FOURTH sample X/Z,inputY,
height0. Both postpasses use local V initialized from P:ground ignores V/height;
fly uses V.Y but ignores height. Do not substitute intersection-point Y for
that field. Wrapper is hitCheckFilter8001577c (initial lock name was wrong),
callback0 for all these rays,point24/normal30 ABI and null-output preservation.
Four containing identities/three complete bodies,89 anchors,six constants,two
spans/two lecture imports pass. make test37 CTest/63 Python and make all pass.
Only ID1 scoped lock released;full backend/floor/source/math/trajectory and
EVT-01 remain open. The earlier surrounding-dot native correction stays separate.

ID2 readiness validation breadcrumb (2026-10-12):make test currently cannot
configure because CMakeLists.txt:162 references missing
tests/event_memory_opcodes_tests.cpp. make all rebuilt event_memory_opcodes and
native3DSX successfully. This is concurrent native fixture arrival;ID2 changes
no CMake/native/test files. Readiness/time/general original-binary verifiers
pass;full host gate remains pending until the named fixture is available.

ID2 EVT-01 readiness delivery (2026-10-12), partial answer:
[Disc thread](../behavior/platform/EVENT_DISC_THREAD_READINESS_GATE.md) closes GP10 producer,
all13 numeric statuses (1 preserves gate),A/B/C query order,retrace-change counting,
count10/11 boundary,card override and alternate input/reset requests. [System
presentation](../behavior/platform/DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md) closes local
ROM-font routing/init,width/projection/color/request ordering and four exact
US-default DOL resources. V changes from selector to pointer during rendering;
status1 retains it,with no automatic repair. [Audio cover state](../behavior/audio/AUDIO_DISC_COVER_STATE_REQUESTS.md)
closes installed/open/close and three-record local state predicate;downstream
stream pause/resume is not inferred. Scoped thread startup/GP10 consumers and
13-status table are pinned. Verifier passes11 containing/whole hashes,184 anchors,
13 spans and four message hashes. Own binary-validated lock released;SDK media/
thread/VI/ROM-font/render/audio/source/domain and original interruption/recovery/
pause/input traces remain open. make all and63 Python tests pass;full host test
awaits the missing concurrent native fixture described above. No native changes
or original message body/bitmap conversion committed by ID2.

ID2 readiness validation completion (2026-10-12):native memory fixture has
arrived. Fresh make test passes38 CTest suites and63 Python tests;the missing-
fixture failure above is resolved. make all passes;current1618 local links all
resolve. ID2 changes no native/test code;readiness/time/general evidence checks
and source resource hashes pass. Only original domain/runtime gates remain open.

ID1 partner floor query delivery (2026-10-12):
[Query](../behavior/partner/FIELD_PARTNER_FLOOR_QUERY.md),
[slope numerics](../behavior/partner/FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md) and
[heading](../behavior/partner/FIELD_PARTNER_FLOOR_HEADING.md) specify complete local
five-ray/global-filter,pointY quantization and highest-support admission.
Projected angles are written BEFORE steep>=50 rejection;admitted candidate
executes the pipeline AGAIN and publishes inclination before vertical acceptance.
Last slope metadata can differ from retained support. Below-root drop>=2 uses
abs(dropLimit);other candidates use inclusive11. Excessive drops overwrite e8
in ray order,not min/max;any final support clears e8. Early flag02000000 bypasses
all output initialization. Four identities/three complete local bodies plus
selected backend,100 anchors,28 constants,four spans/two lecture imports pass.
Initial make test failed CMakeLists.txt:167 missing tests/event_tail_opcodes_tests.cpp
during concurrent native work (log /tmp/id1-ground-test.log);make all passed.
Only ID1 scoped lock released;full backend/math/source/domain/EVT-01 and original
floor/slope/transition traces remain open.

ID1 floor-query host gate update (2026-10-12):missing native tail fixture
arrived. Rerun make test now reaches compilation but fails
 tests/event_tail_opcodes_tests.cpp:31:entryAddress lambda accepts only(void*)
while current service type is uint32_t(*)(void*,EventEntry&). Exact log:
/tmp/id1-ground-test-rerun.log. Native owner should align this fixture signature;
ID1 did not edit concurrent native files. Static floor evidence still passes;
this is a host validation failure,not an original-game evidence failure.

ID1 floor-query host validation completion (2026-10-12):native owner aligned
the tail fixture callback signature. Fresh make test passes39 CTest suites and
64 Python tests;exact passing log /tmp/id1-ground-test-final.log. Earlier missing-
file and callback-mismatch failures are resolved historical runs. make all passed;
static ground evidence remains green. ID1 changed no native files.

ID2 DMG-01/DMG-02 unit-entry delivery (2026-10-12), partial answer:
[Full Entry](../behavior/battle/BATTLE_UNIT_ENTRY_COMPLETE.md) closes complete local6b4
identity/geometry/resource-call order:64-slot first free,checked unit allocation,
early registry publication,unchecked part allocation/zero-count path,kind/setup
copies,pouch overrides,gauge-before-import,body first versus cached last,display/
special-kind allocation and final asynchronous init script with actor/event IDs.
[Registry/scalar/geometry](../behavior/battle/BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md)
closes registry bounds asymmetry,U8 GetBelong versus S8 consumers/enemy mapping,
S16 height/body lookup,all called vector setters/angle wrapper and HpGaugeInit.
Direct damage explicitly extsb AFTER the U8 getter;its wording is clarified,
without changing recovered zero-test behavior. [Pose/display](../behavior/battle/BATTLE_UNIT_POSE_AND_DISPLAY_INITIALIZATION.md)
closes fallback-first category lookup versus bounded28-record display lookup,
borrowed tables/names,copy/resource-entry order and effect/icon init leaves.
Field114 initializes1,vector scale separate;field144 is borrowed kind.b0 pointer,
its pointed schema/lifetime/other writers still open. Whole local entry gap is
closed;kind/resource/pose/render/status/turn/source/domain and original traces
keep parents open. Own lock released;verifier passes33 containing/complete/linked
hashes,441 anchors,11 spans. make test passes40 CTest suites and64 Python tests;
make all passed. ID2 changed no native or asset conversion code.

ID1 partner appearance/screen recovery delivery (2026-10-12):
[Appearance](../behavior/partner/FIELD_PARTNER_SUPPORT_APPEARANCE.md),
[vector ray](../behavior/partner/FIELD_PARTNER_VECTOR_RAY_WRAPPER.md),
[screen recovery](../behavior/partner/FIELD_PARTNER_SCREEN_RECOVERY.md) specify complete
local12-angle search,sequential origin persistence,sampled output and null-down
acceptance;ground ignores failure,overwrites preparedY and requests motion2.
Vector wrapper quantizes distance+pointXYZ only,normal raw,miss preserves outputs.
Screen reset gates signed Ctrl/Key bytes,camera4 projection,early placement3
request and bank0 reset on BOTH flag branches;up to101 moves,five heights plus
raised74 retry,root commits only on projectedX acceptance. Seven identities/
six complete local bodies plus selected backend,105 anchors,18 constants,six
spans/two lecture imports pass;make test40 CTest/68 Python and make all pass.
Only ID1 scoped lock released;placement3/full projection/backend/motion/domain/
EVT-01 and original traces remain open. Shared-link snapshot1334 found one
concurrent ID2 target missing:BATTLE_UNIT_ENTRY_COMPLETE.md→UNIT_BADGE_IMPORT_AND_SETUP.md;
all new ID1 contract links resolve. ID1 leaves that active ID2 document untouched.

ID1 placement3 output contract delivery (2026-10-12):
[Placement](../behavior/partner/FIELD_PARTNER_PLACEMENT3.md),
[math](../behavior/partner/FIELD_PARTNER_PLACEMENT3_MATH.md),
[NPC scan](../behavior/partner/FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md) close initialized-
output plus possible moved output on return0,nine-angle direct/null-filter rays,
duplicate named scans,strict>60 inclination rejection and separate success paths.
NPC scan returns LAST active horizontal d<radius record despite computing unused
3D intermediate;placement records largest3D distance with duplicate SDK requests.
NPC-free success raw floorY/around;fallback ownerY/no around. Screen-reset ignore-
return/root copy is now locally explained,not treated as uninitialized by default.
Six identities/five complete local bodies plus backend selected regions,108 anchors,
22 constants,nine spans/two lecture imports pass. make test42 CTest/69 Python
and make all pass. All1356 behavior links resolve at snapshot,including the prior
concurrent ID2 missing-target report. Only ID1 scoped lock released;full source/
scene/NPC/numeric/caller/EVT-01 and original trajectories remain open.

ID2 DMG-02/STATUS-01 record/turn delivery (2026-10-12), partial answer:
[Byte updates](../behavior/battle/BATTLE_ACTION_RECORD_BYTE_UPDATES.md) closes AddPoint's
store-low8/reload/THEN100 cap and AddCount's>=100 no-write guard,requested-HP
versus paid-FP caller order/fields and34 direct DOL calls. Wide saturation would
be wrong:100+200→44;prior count255 stays255. [Turn rules](../behavior/battle/BATTLE_ACTION_RECORD_TURN_RULES.md)
closes60..65 exact sum/read orders and helper's live global recapture/two threshold
phases/status2→6/bit2-preservation policy;255 threshold bypass independent.
[First process](../behavior/battle/BATTLE_TURN_FIRST_PROCESS_COMPLETE.md) closes complete
caller:judge BEFORE turn-half increment/count-byte update;all64 nonnull units,
unit24 increment before Slow reset,RNG/retained-kind priority loads and status
mask clear,global fog timer/conditional deletion with ignored result. No once-
per-turn frequency or generic record reset is inferred. Own lock released;
general rule judge,attack-command classification,raw writers/reset/persistence/
fortune/effect/turn/source domains and original traces remain open. Verifier
passes12 containing/complete/linked hashes,203 anchors,SDA span and34 exact
DOL direct sites. make test passes43 CTest suites and69 Python tests;make all
passed. ID2 changed no native/test/asset conversion code.

ID1 motion2 jump/jump2 delivery (2026-10-12):
[Destination](../behavior/partner/FIELD_PARTNER_JUMP_DESTINATION.md),
[init](../behavior/partner/FIELD_PARTNER_JUMP_INITIALIZATION.md),
[flight/landing](../behavior/partner/FIELD_PARTNER_JUMP_FLIGHT_AND_LANDING.md) close both
complete local handlers. Selector1 uses main jump,2/3/6 jump2;target-preserve
00400000 belongs to main init. Count+24 is NOT decremented locally. Main
short-history branch adds displacement from TARGET;long branch from ROOT.
Main floor-before-horizontal and early -2000 gate differ from jump2 horizontal-
before-floor/final gate;main scratch flag01000000 does not publish rootXZ.
Main landing0/00800000 vs jump2 distance<5→0 else1/no marker are explicit.
Six complete identities,136 anchors,17 constants,two spans/two lecture imports
and four dispatch sites pass;make test44 CTest/72 Python pass. Initial make all
failed source/runtime/audio.cpp:18 Audio::play(Sound) vs platform.hpp:66 play(const Game&)
during concurrent native edits;exact log /tmp/id1-jump-build.log. Native owner
subsequently updated that signature;rerun passed/rebuilt3DSX (log /tmp/id1-jump-build-rerun.log). Build invoked existing
3218-identity/five-fallback audio map packing;provenance recorded in ASSETS.
Only ID1 scoped lock released;full service/source/EVT-01 and original traces open.

ID2 JUMP-01/DMG-02/STATUS-01 attack-selector delivery (2026-10-12), partial answer:
[Selection](../behavior/battle/BATTLE_ATTACK_EVENT_SELECTION.md) closes full5b4 body,
14 outer/nine subcommand/seven count dispatch entries,selection-time records,
kind222 versus every-other split,retained weapon versus null-event fallback,
exact8036aa84 exemption and output-write order. ChangeParty reads FIXED80354208
within UltraHammerFinish,not an inferred global flag. [Classification/leaves](../behavior/battle/BATTLE_WEAPON_ATTACK_CLASSIFICATION.md)
closes full signed short-circuit predicate,borrowed zero-sentinel data lookup,
audience global recapture and actual fallback80373d00 (named symbol+4),
present-word1 versus other scripts. No event execution/cost/damage is inferred
from selecting/counting. Own lock released;general rule judge,menu/source/category
consumers,raw record/reset/save/turn/audience/backend and original traces remain
open. Verifier passes11 complete/linked hashes,301 anchors,eight spans and seven
exact DOL calls;prior record/cursor/general verifiers pass. make test passes46
CTest suites and77 Python tests;make all passed;1825 local links resolved.
ID2 changed no native/test/asset conversion code.

ID2 DMG-02/STATUS-01 general-rule delivery (2026-10-12), partial answer:
[All68 rules](../behavior/battle/BATTLE_GENERAL_RULE_JUDGMENT.md) closes full7f0 judge,
unconditional Mario→partner searches,all byte/HP sums/read orders,rule54/55
repeated4b/4d,direct58/59 and66/67 branches,scalar capturedP write versus end-
helper currentP2 writes. [Threshold/end/query leaves](../behavior/battle/BATTLE_RULE_THRESHOLD_AND_UNIT_QUERIES.md)
closes signed no_use/use short-circuit compares WITHOUT255 bypass,complete
end two-phase status graph and first-match unit selectors. End/turn helper
bodies are byte-identical;call cadence is not inferred. Clarified prior “bit2”
wording to NUMERIC0x02 (zero-based index1),not0x04;prior status2/6 survives
nonzero phase2 when phase1 excluded,but status4 becomes1. Own lock released;
raw record initialization/writers/reset/save,descriptor/threshold/status consumers,
full btlseqEnd/scene/turn/source/domain and original traces remain open.
Verifier passes eight complete/linked hashes,390 anchors,two spans,60 exact
DOL calls;all68 documented entry addresses reconcile with original table.
make test46 CTest/78 Python and make all pass;1860 local links resolve.
ID2 changed no native/test/asset conversion code.

ID1 live target/front-floor delivery (2026-10-12):
[Direction/activity](../behavior/partner/FIELD_PARTNER_LIVE_DIRECTION_AND_ACTIVITY.md),
[front-floor](../behavior/partner/FIELD_PARTNER_FRONT_FLOOR.md),
[status/clearance](../behavior/partner/FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md) close
local q0 direction partner admission versus distance's Mario shortcut;TargetDistY
returns activity fields rather than coordinate difference. Front-floor takes the
live f1 heading from prior TargetDir,casts three UPWARD rays then downward300,
uses horizontal normal magnitude<45 (not angle),writes current rootXYZ on miss,
quantized pointXYZ on hit and returns ATTRIBUTES (success attr0 also returns0).
Jump caller dependency/heading notes updated. Guarded kind2/flag/code20..21
status3 and float60 SDA setter/one ReInit tail call pinned. Eight identities/
six complete bodies plus selected backend/reinit,98 anchors,16 constants,three
spans/two imports pass;recomputed census matches eight loads/one store/one call.
Negative omitted-caller check rejects. make test47 CTest/78 Python and make all
pass;audio-map packing provenance recorded in ASSETS. All1446 behavior links
resolve at snapshot. Only ID1 scoped lock released;full source/alias/REL/backend/
math/EVT-01 and original target/clearance traces remain open.

ID2 EVT-01 packed-entry consolidation delivery (2026-10-12), answer to the
original1b0 map request:
[Packed map](../behavior/event/EVENT_ENTRY_PACKED_LAYOUT.md) accounts for all432 bytes
through49 spans:388 recovered-role bytes and44 unknown raw bytes preserved
without invented reserved-zero/unused claims. Includes widths/signedness/raw32/
original pointer words,all stack/local/metadata/continuation fields,adjacent
context/global banks,constructor/restart/delete/bulk-clear distinctions and
links to delivered contracts. [Alias/translator requirements](../behavior/event/EVENT_ENTRY_ADDRESS_AND_ALIAS_REQUIREMENTS.md)
provides physical cross-field examples (LW16→flags,switch-value8→data cursor,
label pointer16→waiting parent),big-endian partial writes,shared user pointers,
callback registry/USER_FUNC polling,paired metadata reads and child copy-back.
Machine-readable tools/event_entry_layout.json matches the document exactly;
verifier checks byte accounting plus ten existing ISO evidence suites. No native
packed translator or original-execution equivalence is claimed. This consolidated
map request is answered and own lock released;NormalJump registration/callback
source,storage/address/context/heap/time/platform/numeric/scene domains and
original traces remain EVT-01 work. make test48 CTest/78 Python and make all
pass;1955 local links resolve;general1980-anchor verifier passes. Repeat map
JSON reports match;no native/test/asset conversion code changed by ID2.

ID2 EVT-01 heap-order addendum delivery (2026-10-12), answer to the explicit
SDK/primary-map reload request:
[Live read order](../behavior/memory/SDK_HEAP_BOOTSTRAP_LIVE_READ_ORDER.md) pins original
OSInitAlloc count reload80296670,per-iteration/final base80296654/67c while
metadata size retains ORIGINAL count*12 captured BEFORE global publication.
CreateHeap captures count/base once but reloads descriptor.size802966e8 AFTER
header previous/next stores. Both primary paths capture start→end→handle BEFORE
length store;old end/start/handle-after-length summary corrected. Local fill/
flush length differs from final reloaded root-header length. Addendum supplies
concrete alias/observation cases and preserves no-null-recovery/ignored replacement
handle policy. Existing bootstrap contract and verifier updated:20 body hashes,
243 anchors (38 added),two spans,50 direct sites;repeat reports match. Own lock
released;this specific order request answered. Native arena/cache/address/alias/
scene/source and original traces remain EVT-01 requirements. make test48 CTest/
78 Python and make all pass;1963 local links resolve;general1980 anchors pass.
No native/test/asset conversion edits by ID2.

ID1 fall validation native gate (2026-10-12):initial make test failed CMake
missing tests/event_entry_addresses_tests.cpp;fixture arrived,rerun now reaches
link but fails undefined Game::rebaseFieldCamera/cacheFieldScreen/
publishFieldCameraTarget/beginFieldCameraTick. make all also fails those same
four camera methods after compiling native sources. Exact logs:
/tmp/id1-fall-test.log,/tmp/id1-fall-test-rerun.log,/tmp/id1-fall-build.log.
ID1 changed no native files;source owner should complete camera definitions.
Static fall evidence remains green;no original-runtime fidelity claim follows
from native build status. Existing audio-map pipeline packed five fallbacks;
provenance will be recorded with the scoped research delivery.

ID1 fall validation follow-up (2026-10-12):camera definitions arrived;make
all rerun passes/rebuilt3DSX (log /tmp/id1-fall-build-rerun.log). Host rerun
now fails tests/field_movement_tests.cpp:126 because Game::fieldAirOnScreen was
removed and the fixture still assigns it;log /tmp/id1-fall-test-final.log.
Camera linker failures above are resolved historical runs. Native fixture owner
should update the stale on-screen test to the new camera API;ID1 changed no
native tests. Original static fall evidence remains green.

ID2 EVT-01/JUMP-01 registration/registry delivery (2026-10-12), partial answer:
[Source/root registration](../behavior/battle/NORMAL_JUMP_ROOT_REGISTRATION.md) pins
ordinary weapon803538a8+b0→80355cf0,selected-event publication and complete
state6 creation region:root priority10,flags0,typeef,current GP context,global
pre-increment ID/global speed,actor initially-1 then E.ID→U2a8 followed by
U0→E160 AFTER label scan/run-check,wait-ID deletion afterward. Flags0 does not
append a root mid-manager. Three literal16-byte wrappers invoke Jump as opcode94
child,then delete;child inherits actor/type,priority+1 and current GLOBAL speed.
[Registry census](../behavior/battle/NORMAL_JUMP_CALLBACK_REGISTRY.md) and machine-readable
 tools/jump_callback_registry.json pin all156 direct USER_FUNC sites/72 addresses
in three packed streams,all source/function body hashes,127 registration anchors
and seven spans. Full callback bodies are identities here,not newly closed
implementations;menu/global writer paths,dynamic data7/transitive sources and
original runtime remain open. Own scoped lock released;parents remain open.

ID2 urgent native source correction in the same delivery:evtEntryRunCheck80037744
loads REQUESTED8041e940 directly (r13+1a20),not GP18/marioStGetSystemLevel result.
Current include/event/event_creation.hpp says systemLevel is whole GP18 mask;that comment/
binding must be corrected by native owner. Request3 versus GP low nibble7 is a
concrete differing case:constructor dispatches StopAll10 from3,while7 matches no
case. Source enum and getter mask remain separate;root verifier/full listing pin
this evidence. ID2 changes no concurrent native source. make test50 CTest/78
Python and make all pass;general1980 anchors/root verifier pass;all72 table rows
match census;repeat JSON reports equal;1999 local links resolve. Passing host
fixtures do not establish this source-binding fidelity.

ID1 fall delivery/host validation completion (2026-10-12):
[Air](../behavior/partner/FIELD_PARTNER_FALL_AIR.md),
[landing](../behavior/partner/FIELD_PARTNER_FALL_LANDING.md),
[facing](../behavior/partner/FIELD_PARTNER_FALL_FACING.md) specify floor before integration
(depth10,drop abs oldV),conditional default-1.4 only with flag20 clear/acc0,
velocity floor-16,key-gated speed-.2,placement4 request and two distinct landing
cleanup paths. Return2 air/3 land;walkMain ignores it. Landing precedes integration,
retains acceleration and chooses0/1 by speed equality;flag20 clear alone adds
mask2000/footprint cleanup. Full placement4 remains separate. Six complete hashes,
101 anchors,21 constants,one span/two imports/five dispatches pass. Native fixture
now updated;make test passes50 CTest/78 Python (log /tmp/id1-fall-test-current.log),
make all rerun passes/rebuilt3DSX. Missing fixture/camera linker/stale fieldAirOnScreen
failures above are resolved historical runs. ID1 changed no native files. All1541
behavior links resolve at snapshot. Only ID1 scoped lock released;full service/
source/EVT-01 and original trajectories remain open.

ID1 commit provenance correction (2026-10-12):shared Git index changed after
staged-stat review. Commit8664e85 contains ID1 fall work AND concurrent
OVERWORLD_CAMERA_ROAD_STATE.md,overworld_camera_road_state_evidence.json and
verify_overworld_camera_road_state.py staged by another owner. ID1 did not
write/edit those three paths. They are preserved in the commit;history is not
rewritten. Native uncommitted files and ID2 pending work remain preserved.
Earlier statement that every concurrent change stayed outside the scoped commit
must be read with this recorded shared-index exception.

ID2 EVT-01/JUMP-01 data-child source delivery (2026-10-12), partial answer:
[Complete callback/Mario key7](../behavior/battle/BATTLE_UNIT_DATA_CHILD_CONTINUATION.md)
closes full RunDataEventChild8010a010/a0,live actor→translation→unit→key→table
query order,zero-result2 versus ChildEntry flags0,parent opcode clear BEFORE
child actor=retained translated I,return255 without parent deletion. Original
kind80372afc+c0→803729c8,key7→80416bd0 tiny DELETE/terminal script closes
static Mario source. Tiny child still allocates ID/appends/waits/copies back.
[Translation/system query](../behavior/battle/BATTLE_ACTOR_SELECTOR_TRANSLATION.md)
closes all signed selectors,unconditional global capture,-3/-5 unchecked null
versus-4 fallback,and first kind220 query. Old recovery wrapper size9c corrected
toa0. Own lock released;dynamic unit/table/source/scene/backend/runtime remain open.

Native source owner:current three-stream pack omits child80416bd0 plus original
trailing word80416bd8 required by constructor label scan. Existing packer with
FOUR explicit names reproduced /tmp/id2-datachild-jump-with-key7.pmevt:four
segments,4564 bytes;added80416bd0/12 span matches original byte-for-byte.
Reproduction command/provenance in linked contract. No additional USER_FUNC
address is needed for this child;actual data table and VM source binding are.
Registry/alias/heap/time/original execution remain required;do not stub lookup
as immediate2. Prior requested-enum run-check correction also remains pending:
include/event/event_creation.hpp still describes whole GP18 at this check.
Verifier passes ten complete/linked hashes,109 anchors,five spans including
trailing bytes;repeat reports equal;constructor/lifetime/general1980 evidence
passes. make test50 CTest/78 Python and make all pass;2038 local links resolve.
ID2 changed no native/test/asset-packer source;original extracts remain /tmp.

ID2 INPUT-01 WorkInit clarification delivery (2026-10-12), exact map answered:
[Reset/raw preservation](../behavior/battle/BATTLE_PAD_RESET_AND_RAW_PRESERVATION.md)
closes complete8011a38c/b0 body:two snapshot loops store halfword0 and bytes2..a,
NO byteb store;all90 history words clear in index-interleaved18→90→108 order;
repeat bytes1f8/1f9 clear. Preserves currentb/priorb independently,entire120-byte
release bank and raw1fa/1fb;384 cleared/124 preserved within1fc stride. Update
retains currenta/b while three word copies move old currenta/b into prior.
Older “zeroes both snapshots” and ambiguous untouched-byte initialization wording
corrected in defense/cadence docs. Fresh allocation zeroing is separate evidence.
Native reported byteb-preserving init/bytea clear agrees with the local contract;
no native packed ABI/hardware/cadence proof inferred. Own lock released;specific
byte request answered. Remaining INPUT-01 SDK/physical/error/pause/control/motor/
frame-rate/caller/original input/impact traces stay open. Evidence verifier now
passes11 hashes,154 anchors (90 added),28 direct calls,three data checks;repeat
reports equal. make test50 CTest/78 Python and make all pass;2048 local links
resolve;general1980 anchors pass. No native/test/asset edits by ID2.

ID2 EVT-01/JUMP-01/DMG-02 Majinai-child delivery (2026-10-12), partial answer:
[Power-check graph/state](../behavior/battle/JUMP_MAJINAI_POWER_CHECK_CHILD.md) closes all
14 commands and complete query/Done/EndCheck:pouch5ba exact1,output before
capturedB18ff9=1,query-false NO flag clear,byte count decrement/wrap (0→255),
unconditional5b9ff/5ba0,EndCheck00400000 OR only count0. Current pouch/battle
captures remain independent. [Effect/nested graphs](../behavior/battle/JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md)
closes unused actor operand/Mario query,constant vector(0,10,0),E78 gate and
ANY global same-name mjef_coinup polling,first-match/name-copy helpers;complete
12-command status-gated jump and17-command independent five-key sound root.
ftomsec uses wrapped*1000/SIGNED hardcoded60,not GP refresh;WAIT_MSEC remains
lifetime-owned. Original message ID btl_msg_mj_power_up and request preserved.
Own lock released;fortune producer/pouch writers,effect/audio/message/announcement/
pose/source/backend and original execution remain open.

Native source/registry dependency:pack must add MajinaiJump80363414/ac and
sound80306874/10c plus trailing words,in addition to previously requested key7
child. Six explicit source names reproduced /tmp/id2-majinai-jump-six-sources.pmevt:
6 events/5 merged segments/5020 bytes;all three Majinai ranges byte-exact.
Nested graphs add three callback identities beyond prior72:80103dc0 ftomsec,
80105cb4 CheckActStatus,801521dc evt_snd_sfxon_3d;extra identities are pinned,
full audio/backend policy remains unbound. Do not replace name wait,independent
root or tiny/animated child with immediate success. Verifier passes15 whole/
linked hashes,240 anchors,nine spans,three full event+trailing hashes;repeat
reports equal. General1980/root-run/prior registry checks pass;make test51 CTest/
78 Python and make all pass;2091 local links resolve. No native/test/asset-packer
source edits by ID2;original extracts/packs stay /tmp.

ID2 EVT-01/JUMP-01/STATUS-01 fortune producer delivery (2026-10-12), partial answer:
[BattleMajinaiCheck](../behavior/battle/BATTLE_FORTUNE_PRODUCER.md) closes full150 body:
count0 preserves stale mode/countdown;negative S8 countdown precedes mode query,
clears mode,draws irand5 and publishesR+6→reload→minus1 (final5..9);nonnegative
active mode returns raw/no draw;mode0 decrements/reloads signed countdown before
ready reset/selection. Setup pointer pathB2738→descriptor.c→flags4 mask20 picks
INLINE45/90 or30/60/80 thresholds with ONE irand100;final mode reload returned.
[Init/call/RNG policy](../behavior/battle/BATTLE_FORTUNE_INIT_AND_CALL_BINDINGS.md) closes
pouch initializer/wrapper:old U8 count SIGNED-compared to full request,old>=N
no writes,otherwise low8 store then separate pointer reloads forff/0. N256 can
write0;do not replace with saturating/max-U8 reset. Scoped DOL census exactly
wrapper800f08c4→init and first-process8011b21c→Check after signed turn>=2.
Derived modulo counts are explicitly conditional enumeration,not measured rates.
Own lock released;full field transaction/REL/script operands,pouch/save/raw writers,
setup descriptor/flags,other mode consumers/turn/scene and original traces remain
open. Verifier passes eight complete/linked hashes,108 anchors,SDA span and two
exact direct sites;repeat reports equal;enumerated32768-output counts reconcile.
Prior child/turn/general1980 checks pass;make test52 CTest/78 Python and make all
pass;2118 local links resolve. No native/test/asset conversion code changed by ID2.

ID2 EVT-01/JUMP-01/DMG-02 Majinai construction delivery (2026-10-12), partial answer:
[Full constructor](../behavior/effect/MAJINAI_EFFECT_CONSTRUCTION.md) closes effMajinaiEntry
f4:pool record before heap allocation,borrowed Majinai resource/count1/reload,
heap3/count<<5 request,returned payload pointer,update callback/flags before
payload,all eight32-byte initial fields,then mapGetWork and WORDffffffff at
map+1e. It is a four-byte request,not an inferred halfword/aligned field.
[Acquisition/map helper](../behavior/effect/EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md)
closes full effEntry/mapGetWork:captured signed pool count/array,EXACT0 free,
exhaustion one-past initialization/no null return,flags1 before GP14 normalization,
partial fields/name first-byte clear with15 tail bytes preserved;independent
map index-before-base/stride2f4/wrap. No automatic allocator failure rollback,
instance/resource-name conflation or lifetime semantics invented. Three exact
DOL constructor callers pinned;field/generic callers beyond annotations open.
Own lock released;effect main/draw/delete/resources,pool/map/heap/scene/source/
numeric/platform and original frame/exception traces remain open. Verifier
passes five complete/linked hashes,92 anchors,five spans and three direct sites;
repeat reports equal. Prior caller/heap/general1980 checks pass;make test52 CTest/
78 Python and make all pass;2156 local links resolve. No native/test/asset edits.

ID1 flying delivery (2026-10-12),AC-01/AC-03 partial answer:
The linked flyMain@party_move.o,behind_fly,homing_fly and two speed-writer
contracts now close their complete LOCAL request/update graphs. Selector0/1
splash/countdown/hazard differs from2/3/6;homing can consume multiple records
and update speed several times in one call. Special kind domains can read
inherited FPR25/FPR26:do not fill invented defaults without source proof.
Behind completion blends floorY on initially-nonnull paths even when H is later
discarded. Main/ray/height/slot/capture/write ordering is specified in six files.
Five complete hashes plus selected actual partyFlyMain caller tail,869 anchors,
34 constants,four spans,two imports and nine direct sites validated;53 CTest/
78 Python tests and3DS build passed. Parent requests remain open for full
flying history/source/other motion/pose/backend/numeric/runtime composition.

ID2 EVT-01/JUMP-01/DMG-02 effect deletion delivery (2026-10-12), partial answer:
[Complete soft/hard/Maya leaves](../behavior/effect/EFFECT_SOFT_AND_HARD_DELETION.md)
closes null/exact-zero gates,soft numeric02→OR04 with NO payload read/free,
soft no02 fallback and hard __memFree(3,E.c)→flags0 ONLY,raw Maya pointer free
without record/null gate. Stale payload/name/count/callback/resource survive;
record bank/event/audio/map ownership not automatically freed. [Caller/lifetime](../behavior/effect/EFFECT_DELETE_CALLERS_AND_LIFETIME.md)
includes271 annotated/pinned DOL direct sites (30 soft/241 hard) and name-visible
soft requests versus hard-hidden slot reuse. Own lock released;effect-specific
04 consumption/update/draw/driver/free cadence,pool/scene/raw/REL/platform and
original traces remain open.

ID2 correction to prior fog wording:BattleFogEndCheck call80147c80 targets
8005e9c0 effSoftDelete,not effDelete (hard is8005ea30). Fog clears its own pointer
AFTER soft request;mask02 record/payload can remain allocated/name-visible.
Native owner must bind that exact soft service,not unconditional payload free.
Numeric target/timer body evidence was correct;helper-name interpretation is
corrected in first-process doc. Majinai ordinary flags3 soft request becomes7;
its update still needs separate04-consumption evidence. Verifier passes six
complete/linked hashes,57 anchors,SDA span and271 exact pairs;annotated census
reconciles;repeat reports equal. Prior fog/heap/general1980 checks pass;make test53
CTest/78 Python and make all pass;2216 local links resolve. No native/test/asset
edits by ID2;original ELF/listings remain /tmp/id2-fxdelete-*.

ID1 flying history/source delivery (2026-10-12),AC-01/AC-03 partial answer:
Complete local writer/init/Cloud/Vivian callbacks and three source leaves now
close record production and its actual kind5/6 bindings. Writer nonzero-slot
null-party fallback writes bank0;initializer Mario fallback still resets bank1.
Append flag4 is always cleared locally;bit0 and other flags preserve ordered
ownership. Initializer has selector2 early return and transient selectors later
overwritten after pose. Cloud gate/phase/offset and Vivian trailing/coincident
angle requests are specified. Live target-speed selection has one owner/buddy
pass;absolute Y error has repeated global/buddy roles. Seven complete hashes,
467 anchors,ten constants,two kind spans,two imports and four actual caller
sites pass;54 CTest/78 Python tests and3DS build pass. Only ID1 lock released;
source/alias/index/flag/kind/FP/pose/ability/backend/runtime parent gaps remain.

ID2 EVT-01/JUMP-01/DMG-02 Majinai Main/Disp delivery (2026-10-12), partial answer:
[Complete Main](../behavior/effect/MAJINAI_EFFECT_UPDATE_STATE_MACHINE.md) closes394 body:
soft04 clear→pose release→map08 clear→hard delete without phase3 restoration;
four phases with async MOBJ_EFF_majinai/A_1 setup,word map grayscale ±5/clamps,
current-GP field five poses versus battle Mario-body one,exact progress3f6e147b
(~.93)/1.0 ordered gates,fused55*scale+Y and45*scale Stardust request,live phase
increment/no same-call redispatch,alpha255→0 queued then-5 cleanup on52nd phase3
CALL (not observed frames),early-coordinate depth snapshot and queue omissions.
[Complete Disp/leaves](../behavior/effect/MAJINAI_EFFECT_DRAW_AND_POSE_REQUESTS.md) closes
12c draw:-1 guard,translation*camera-relative Y rotation*uniform scale alias
requests,On840/white-low8alpha/PoseMain/three pass1/2/3 draws with float0/10,
full raw pose On/Off/LoopTimes/Colour and camera pointer-table leaves. Backend
identities are linked,not newly recovered engines. Own lock released;animation/
resource/matrix/display/driver/particle/scene/FPSCR/alignment/source and original
visual/frame traces remain open. Verifier passes26 complete/linked hashes,338
anchors,five spans;repeat reports equal. Prior construction/delete/general1980
checks pass;make test56 CTest/78 Python and make all pass;2314 local links resolve.
No native/test/asset conversion code changed by ID2;original listings remain /tmp.

ID1 slit/roll delivery (2026-10-12),AC-01/AC-03 partial answer:
Complete local gates and four floor/activation/forced-off plus four paper-wrapper
bodies now specify state37/phase3a/count24,ordered angle/rotation/interpolation,
strict-negative versus<=0 transitions,same-call fallthrough/next-call phases,
return0 blocking and exact cleanup masks. Slit/forced-off clear P+4 mask0x3c0,
including40/80;PaperOff requires flag AND nonnull name before releasing handle.
Global floor getter preserves output on miss,clear preserves coordinate words;
activation can set2000 while owner gate skips pending/state writes. Ten complete
hashes,482 anchors,18 constants,four spans,two imports and22 actual direct sites
validated;56 CTest/78 Python tests and3DS build passed. Parent remains open for
collision helper/animation/resource/source/alias/numeric/runtime composition.

ID2 EVT-01/JUMP-01/DMG-02 effect driver/init/release delivery (2026-10-12), partial answer:
[Driver/cache](../behavior/effect/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md) closes
full effMain170:captured signed count/array but per-record current GP14/exact
context0/1,active01,nonnull callback/ignored result,ascending live slots and
same-pass later allocation;NO local GP18/time/credit scaling. Cache runs AFTER
callbacks,independent manager/GP/prefix/root async/sync/tag captures,async0 retry
versus alloc0 still tag publication. Exact seven prefix pointers/root ./format
and read-only e/us/effect_us.tpl size106880/hash are pinned;provenance in ASSETS.
[Init/release](../behavior/effect/EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md)
closes512*40 heap0 request/live array clear/multiple manager reloads,no old-state
free,N64 independent8-byte reset,active01/exact context hard heap3 payload free
without effect callbacks/pose/map/event cleanup. Four DOL direct sites and
_unload context0/battle_exit context1 effect→animPoseAutoRelease→event ReInit
request order pinned. Own lock released;file/TPL consumers/pose/display/scene/
raw writer/platform and original update/resource/frame traces remain open.
Verifier passes14 complete/linked hashes,186 anchors,eight spans,four exact
DOL sites and US texture FST fingerprint;repeat reports equal. Prior delete/
outer-order/general1980 checks pass;make test58 CTest/78 Python and make all
pass;2373 local links resolve. No native/test/asset conversion code changed by ID2;
texture only hashed read-only;original listings/ELFs stay /tmp/id2-fxdriver-*.

ID1 slit-collision validation breadcrumb (2026-10-12):new original-binary
checks pass;make test currently fails CMakeLists.txt:272 because concurrent
native target references missing tests/event_native_runtime_tests.cpp. Exact
log:/tmp/id1-slit-collision-test.log. ID1 did not modify CMake/native tests;
continue source-owner integration,then rerun the host gate. This is separate
from binary evidence and not a full-goal blocker.

ID2 file-wrapper validation breadcrumb (2026-10-12):make test currently cannot
configure because CMakeLists.txt:277 references missing
 tests/event_callback_registry_tests.cpp. Exact log:/tmp/id2-filewrap-test.log.
This is a concurrent native fixture dependency;ID2 changes no CMake/native/test
files. make all succeeded (/tmp/id2-filewrap-build.log). File-wrapper/driver/
general binary evidence and2400 local links pass;full host gate remains pending
until the named fixture is available.

ID1 slit collision/math delivery (2026-10-12),AC-01/AC-03 partial answer:
Complete local unk_800c2010 probes center/plus/minus until first hit on each
side,with fresh global filter choice and raw normalXZ angles. It changes neither
P nor input V locally;both sides must hit and fabs(180-0.5*round(A+B))<=10
with original fnmsubs/ordered comparison. No shortest-arc/dot-product policy.
Complete toMovedirSimple has no facing override;system angleABf is91-coefficient
table/quadrants,zero vector0,not host atan2. Nine hashes(eight complete/one
selected backend),408 anchors,19 constants,seven spans,two imports and single
actual slit caller pass. Earlier CMake missing event_native_runtime_tests.cpp
breadcrumb is RESOLVED after file arrival:rerun60 CTest/78 Python passes;3DS
build passes. Only ID1 collision lock released;parent backend/source/runtime open.

ID1 generated-inventory correction breadcrumb (2026-10-12):
[Authoritative loader/census correction](../behavior/meta/US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md)
supersedes unvalidated identifier-only/31-numeric-ID/story-prefix claims in new
BINARY_DOCUMENTATION.md,MESSAGE_FILES_ANALYSIS.md and related PROGRESS entries.
Current read-only schema3 census is260 resources/13186 complete key/text pairs;
gor_00 has328 pairs,first key mac_0_014 and247-byte paired text. Filename digit
regex in doc_binary_iso.py is NOT message-key parsing. Prefix semantics/speaker/
scene order and unused-disc interpretation are not proven by inventory counts.
Visible banners added to the two pending drafts;those drafts/tool remain unstaged
by ID1. Source owner should reconcile them against recovered loader/event evidence
and fix generator labels;do not use these guesses to implement story dialogue.

ID2 file-wrapper delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial answer:
[Formatted requests](../behavior/file/FILE_FORMATTED_REQUEST_FORWARDING.md) close
fileAsyncf/fileAllocf/fileAlloc;[handle release](../behavior/file/FILE_HANDLE_RELEASE_AND_EFFECT_CACHE_OWNERSHIP.md)
closes fileFree and effect-cache ownership. Shared1024-byte filename,original
varargs saves/descriptor,ignored vsprintf result,direct _fileAlloc forwarding and
signed16 store/reread/exact0→state2 are explicit. No deallocation occurs;the
word at F+a0 is a zero/nonzero admission gate,whose backend role remains open.
Seven linked/complete hashes,122 instruction anchors and one source span pass;
repeat JSON reports match. Earlier missing event_callback_registry_tests.cpp
breadcrumb is RESOLVED:rerun61 CTest/78 Python tests passes and make all passes.
Logs:/tmp/id2-filewrap-test-rerun.log and /tmp/id2-filewrap-build.log. Only ID2's
wrapper lock released. Underlying fileAsync/_fileAlloc,shared file-manager
acquisition/collection,source/TPL/platform/scene and original traces remain open.

ID2 shared-file validation breadcrumb (2026-10-12):binary evidence and make all
pass. make test fails compiling tests/field_camera_tests.cpp because concurrent
camera interfaces no longer expose adaptedFieldCameraFrame/bottomlessProbeHeight;
see /tmp/id2-filealloc-test.log. ID2 changes no camera/native/test files. Native
owner should finish matching fixtures;the documentation lock remains held until
evidence review and exact gate status are recorded,then only that scope releases.

ID2 shared-file acquisition delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Cache/reference/publication](../behavior/file/FILE_ACQUISITION_AND_REFERENCE_REUSE.md)
and [archive/DVD requests](../behavior/file/FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md)
close _fileAlloc/arcOpen plus five narrow leaves. Filename-only first-match,
pending ac yield,state3 count reset versus state2 preservation,signed16 wrap,
retained free-node next/live manager writes,mode1/2 identical retries,archive
embedded holder versus smart header,unchecked read and deferred-close bit08 are
explicit. F+a0 holder role reconciled in previous release documentation.
Sixteen hashes(seven complete/nine linked),281 instruction anchors,two globals/
startup spans,repeat reports,related/general binary checks pass. make all passes;
make test rerun63 CTest/78 Python passes after transient camera-interface mismatch
RESOLVED. Logs:/tmp/id2-filealloc-test.log (historical failure),
/tmp/id2-filealloc-test-rerun.log and /tmp/id2-filealloc-build.log. Own lock released;
fileAsync/_fileGarbage/relocation/TPL/source/platform/scene/original traces stay open.

ID2 async-file delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial answer:
[Request/cache](../behavior/file/FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md) and
[completion/request state](../behavior/file/FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md)
close three complete local bodies. Cache readiness adds no reference;pending
returns0,first DVD-open failure-1,archive publication uses live head.next AFTER
selected E.next clear,DVD uses retained NEXT. Callback matches pending+40 without
state/not-found guards;only-3 marks state2/type0b,all other raw results relocate
signed-byte type,close and read live user callback after pending clear. Request
leaf preserves two flag read/modify/store operations. Existing effect driver
nonzero gate explicitly admits-1 before its separate synchronous allocation.
Verifier13 hashes(three complete/ten linked),294 anchors,two startup/global spans,
repeat reports,prior acquisition/wrapper/driver/general1980 checks and2473 local
links pass. make test64 CTest/78 Python and make all pass. Logs:
/tmp/id2-fileasync-test.log and /tmp/id2-fileasync-build.log. Only ID2's async
lock released;worker/_fileGarbage/relocation/TPL/source/scene/platform/runtime open.

ID1 partner presentation delivery (2026-10-12),AC-01/AC-03 partial answer:
Complete pre-display/pose-name-ID/facing/draw and three source leaves close local
marker0x6000 consumption,combined animation reset,shared unsigned frame20,
always animPoseMain before mask1 gate,sort-depth5000*Z+5000 and conditional
registration,Vivian tail/size/source/shadow order. Draw matrix uses aliased B as
second input/output with exact reflection/pivot/Z/X/Y/extra-Y/translation order,
mode2 then1 draw requests,time-rate write AFTER draws. Pose source has HP remap,
dot/color table and unguarded adjacent color7 reads;no defaults were fabricated.
Eight complete hashes,515 anchors,32 decoded constants,seven spans,two imports
and three direct pre-display/facing sites pass;64 CTest/78 Python/3DS build pass.
Only ID1 lock released;backend/source/queue/matrix/FP/original visuals remain open.

ID1 BTLNO draft correction follow-up (2026-10-12):visible evidence boundary
added to pending BTLNO_MESSAGE_MAPPING.md,which remains unstaged by ID1.
Its Message Text column holds resource keys;string/prefix association is NOT
proof of runtime BTLNO→dialogue lookup or a complete story dictionary. Related
PROGRESS milestone statements remain unvalidated. The committed
[message correction](../behavior/meta/US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md)
now explicitly supersedes those linkage/completion claims;source owner should
trace original event/service calls before native story implementation.

ID2 file collection/init delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Collection/aging](../behavior/file/FILE_CACHE_COLLECTION_AND_PRELOAD_AGING.md) and
[pool/init/mode](../behavior/file/FILE_CACHE_POOL_INITIALIZATION.md) close _fileGarbage,
fileInit,setter and current-thread leaf. Mode0 worker OVERWRITES pending flags08;
mode1 initially pending wait skips aging,initially ready count stores/rereads
signed16 and retires<=-2. State2 pass marks0 then routes embedded holder skip
versus smartFree(header),live unlink/retained traversal/available append and
stale raw fields. Init heap0 180224 bytes yields1024*176 records,128 eight-store
iterations,temporary one-past link and live tail termination. No old-pool free.
Verifier11 hashes(four complete/seven linked),189 anchors,two startup/global
spans,all17 DOL direct collection/init/setter sites,repeat JSON,related/general
1980 checks and2493 local links pass. make test65 CTest/78 Python,make all pass;
logs /tmp/id2-filegc-test.log and /tmp/id2-filegc-build.log. Own ID2 lock released;
smart heap/relocation/DVD worker/source/platform/TPL/scene/original traces stay open.

ID2 smart-release delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Header release/accounting](../behavior/memory/SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md)
and [automatic release](../behavior/memory/SMART_AUTOMATIC_TAG_RELEASE.md) close two full
local bodies. Header tag4 only changes to3;other tags unlink/update reusable
space/recycle metadata with stale payload pointer and no heap deallocation.
Collector file-entry recycle remains independent of deferred smart header.
Automatic first pass compares low16 input to byte tag;only FULL input3 starts
second pass marking tag4→3,without same-call removal. Exact live rereads/free
chain/count and raw alias requirements are supplied for implementation.
Verifier seven hashes(two complete/five linked),126 anchors,two startup/global
spans and repeat reports pass;collection/async/acquisition/general1980 checks
and2539 local links pass. make test66 CTest/78 Python and make all pass;logs:
/tmp/id2-smartfree-test.log,/tmp/id2-smartfree-build.log. Only ID2 lock released;
allocator/compaction/tag producers/relocation/source/scene/platform/runtime open.

ID2 smart-compaction validation breadcrumb (2026-10-12):original-binary checks
and make all pass. make test currently fails field_camera_tests.cpp:79 assertion
close(events.follow.x,250) && events.look.x==events.follow.x && close(events.dolly,25).
See /tmp/id2-smartgc-test.log. This is a concurrent camera native/fixture issue;
ID2 changes no camera/native/test files. Native owner should reconcile that
camera expectation;the compaction evidence is independently passing.

ID2 smart-compaction delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Compactor/barriers](../behavior/memory/SMART_HEAP_COMPACTION_AND_PENDING_FILES.md) and
[file moves](../behavior/file/FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md) close two
full local bodies. Draw-sync precedes manager reads,equal-source bypass skips
pending inspection,nonworker state3/pending barriers preserve location/gaps,
worker moves retarget DVD7c. File clear takes ENTRY/setup takes PAYLOAD with
signed type;live holder publication/state/next/tail and full-arena flush pinned.
Private contracts/verifier committed42486ae. Ten hashes(two complete/eight
linked),164 anchors,two startup/global spans,repeat JSON,related/general1980
checks and2555 local links pass. make all passes;make test66/67 suites passes
with field_camera assertion79 failing,so its Python stage was not reached.
Separately78 Python tests pass. Logs:/tmp/id2-smartgc-test.log,
/tmp/id2-smartgc-build.log,/tmp/id2-smartgc-python.log. Only ID2's documentation
lock released;camera native owner must resolve its assertion. Allocator/relocation/
worker/source/platform/scene/original runtime remain open.

ID1 animation setter delivery (2026-10-12),AC-01/AC-03 partial answer:
[Selector/restart](../behavior/display/ANIMATION_SELECTOR_AND_RESTART.md),
[paper-group reuse](../behavior/display/ANIMATION_PAPER_GROUP_REUSE.md),
[local time/clock](../behavior/display/ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md)
close five complete used local setters. Negative signed selector means string;
nonnegative means index,missing names map0;ordinary force1 restarts same index;
paper third argument ignored. Group null/failure differ;first same-mode scan then
mode2 reuse;failed nonnull still stamps/flags/index/ec. Local time SINGLE-scales
16.66666603088379 before64-bit conversion,adjusts shared timestamps and conditionally
copies linked ordinary stamp;rate writes only+44. No defaults/allocation/free
were invented. Five hashes/423 words/two constants/two pointer spans/two imports/
299 direct DOL sites pass;68 CTest/78 Python/3DS build pass. Static census is not
US runtime frequency. Only ID1 lock released;Main/draw/resource/clock/helper/
source/lifetime/numeric/original visuals remain open.

ID2 smart-allocation delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Header/gap placement](../behavior/memory/SMART_ALLOCATION_HEADER_AND_GAP_PLACEMENT.md)
and [retry/failure](../behavior/memory/SMART_ALLOCATION_RETRIES_AND_FAILURE_OWNERSHIP.md)
close full smartAlloc. First flag→auto3,nonzero e018 draw-sync→reset0,unguarded
available-header reserve,word-wrap32 alignment/low8 tag,leading unsigned priority
then first-fit used gaps,three compaction/collection1/collection0 tail-only
retries and final0 without reserved-header restoration are explicit. e018 is a
release/synchronization counter,not free stock;previous label corrected.
Verifier six hashes(one complete/five linked),206 anchors,two startup/global
spans,repeat reports,prior compaction/release/collection/general1980 checks and
2611 local links pass. make test68 CTest/78 Python and make all pass;logs:
/tmp/id2-smartalloc-test.log,/tmp/id2-smartalloc-build.log. Prior compaction's
field_camera assertion79 is RESOLVED in current host run. Own allocation lock
released;arena/relocation/worker/source/platform/scene/original traces remain open.

ID2 smart initialization delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Initializer](../behavior/memory/SMART_ARENA_AND_HEADER_INITIALIZATION.md) and
[scoped provisioning](../behavior/memory/SMART_RESOURCE_PROVISIONING_REQUIREMENTS.md)
close smartInit;private commit bbdb4fb. Heap4 start/end/handle order,retained
first conditional clear/flush versus live unconditional second clear,2048*28
headers,256 eight-header link groups/live termination and flag/counter reset
are explicit. Allocation0 still reaches later unconditional clear. Provisioning
requirements separate header/file pools from unknown full-game RAM/workload.
Five hashes(one complete/four linked),121 anchors,two startup/global spans,
repeat JSON,prior allocation/compaction/release/general1980 checks pass.
make test69 CTest/78 Python and make all pass;logs /tmp/id2-smartinit-test.log,
/tmp/id2-smartinit-build.log. Nine new-contract links resolve. Full scan2646 links
finds six unresolved references in REFERENCE_FILES_INVENTORY.md during another
session's staged subsystem reorganization;ID2 preserves that staging and records
shared handoff in worktree. Only ID2 lock released. Bootstrap/relocation/worker/
platform/source/scene/original runtime and full-game requirements stay open.

ID2 relocation validation breadcrumb (2026-10-12):make test currently cannot
configure CMakeLists.txt:322 because tests/battle_unit_event_tests.cpp is missing.
Log:/tmp/id2-reloc-test.log. New relocation/related binary evidence passes;ID2
changes no CMake/native/test files. Native target owner should finish that fixture,
then host gate can rerun. Inventory owner still has five unresolved binary
inventory links after subsystem reorganization;new relocation links resolve.

ID2 relocation dispatch/flat delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Dispatch/type map](../behavior/file/FILE_RELOCATION_TYPE_DISPATCH.md) and
[flat handlers](../behavior/file/FILE_FLAT_POINTER_RELOCATION.md) supply all11
routes and eight complete flat type handlers,not complete nested4/5 bodies.
Set accepts full unsigned0..10;Clear sign-extends entry type and reads holder
payload before rejection. First unsigned field alone guards whole block;
retained first value then ordered word add/subtract,no per-field null handling.
Two containing hashes,481 selected anchors,two11-word tables,repeat JSON,
prior smart init/compaction/async/general1980 checks pass. make all passes;
make test configure fails missing tests/battle_unit_event_tests.cpp;separate78
Python tests pass. Logs:/tmp/id2-reloc-test.log,/tmp/id2-reloc-build.log,
/tmp/id2-reloc-python.log. New-contract links resolve;five unrelated inventory
references remain pending reorganization. Own lock released;types4/5 nested
handlers/source/format consumers/platform/original runtime/full-game requirements
stay open. Preserve staged reorganization;ID2 shared handoff remains in worktree.

ID2 relocation host-gate completion (2026-10-12):missing battle_unit_event_tests.cpp
fixture arrived;make test rerun passes70 CTest suites/78 Python tests. Log:
/tmp/id2-reloc-test-rerun.log. Earlier configure-failure breadcrumb is RESOLVED.
Private contracts/verifier commit9ac794c;make all already passed. Types4/5 nested
handlers remain open. Shared handoff remains separate from staged reorganization.

ID1 Main/init delivery (2026-10-12),AC-01/AC-03 partial answer:
[Entry/time](../behavior/display/ANIMATION_MAIN_ENTRY_AND_TIME.md),
[sampling](../behavior/display/ANIMATION_TIMELINE_SAMPLING.md),
[cache/replay](../behavior/display/ANIMATION_COMPRESSED_DELTA_CACHE.md),
[interpolation](../behavior/display/ANIMATION_OUTPUT_INTERPOLATION.md),
[facing](../behavior/display/ANIMATION_MAIN_CAMERA_FACING.md),
[initialization](../behavior/display/ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md)
close complete Main/init local bodies:recursive link before null gate,GQR2..5
writes,clock/rate/span/loop/predecessor graph,base rebuild/forward replay/five
relative-index streams,linear triples/cubic node deltas,no end-seam interpolation,
optional facing,allocation/table/file request order. Original M_Z_1 zero-span/
one-key resource pinned without claiming runtime scene selection. 1773 reviewed
words/17 constants/five spans/two imports/65 direct sites/example pass;70 CTest,
78 Python,3DS build pass. Full clock/helpers/copy/resource/pool/draw/libm/source/
visual domains remain open. Only ID1 scoped lock released.

ID1 ownership note:concurrent reorganization76263bb included the six ID1-authored
Main/init documents while moving the directory tree. Their recovery/validation
belongs to ID1;that commit also contains other owners' moves/content. Preserve
history;this follow-up adds ID1 evidence tool/manifest,numeric notation refinement
and exact delivery/provenance. Whole-tree link scan has five unrelated forward
references in pending binary/REFERENCE_FILES_INVENTORY.md;new Main/init links
resolve. Do not interpret those draft inventory promises as recovered systems.

ID2 type4 validation breadcrumb (2026-10-12):make test cannot configure
CMakeLists.txt:327 missing tests/battle_packed_status_tests.cpp. Log:
/tmp/id2-type4-test.log. Binary/type4 US source fingerprint and make all pass;
ID2 changes no CMake/native/tests. Native owner should finish fixture,then
rerun host gate. Six unresolved inventory links remain owned by that document's
source session;new type4 links resolve.

ID2 type4 relocation delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Setup](../behavior/file/FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_SETUP.md) and
[clear](../behavior/file/FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_CLEAR.md) close complete
nested type4;private commit66b533e. Signature diagnostic before root guard,
low16 index/live32 count,descriptor/table rereads,data flags23/02 and opposite
setup/clear order/root-last clear are explicit. US effect TPL106880-byte source
fingerprint/count82/table0c/distinct82 image headers/zero palette slots pinned;
no native execution or nonzero-palette source witness is claimed.
Verifier two containing/one panic hashes,165 selected anchors,four tables/strings
spans,repeat JSON and prior dispatch/compaction/general1980 pass;make all passes.
make test configure fails missing battle_packed_status_tests.cpp;separate78
Python tests pass. Logs:/tmp/id2-type4-test.log,/tmp/id2-type4-build.log,
/tmp/id2-type4-python.log. New links resolve;global2755-link snapshot has six
unrelated inventory missing references. Own lock released;type5/consumers/
platform/source/scene/original traces remain open. Shared handoff and previous
relocation host completion remain separate from ID1's staged documentation.

ID2 type4 host completion (2026-10-12):fixture arrived;make test rerun passes71
CTest suites/78 Python tests (/tmp/id2-type4-test-rerun.log). Earlier missing
battle_packed_status_tests.cpp breadcrumb is RESOLVED. make all passed and private
contracts/verifier are66b533e. Shared ID2 handoff/previous relocation completion
now prepared as a scoped patch after other session staging clears. Type5 and
source/texture consumers/platform/original runtime/full-game requirements remain open.

ID2 type5 relocation delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Setup](../behavior/file/FILE_TYPE5_NESTED_POINTER_SETUP.md) and
[clear](../behavior/file/FILE_TYPE5_NESTED_POINTER_CLEAR.md) close type5 and complete
all local Set/Clear routes through linked flat/type4/dispatch contracts. Root25
fields,captured signed count,64-byte records/root-relative parent links,
child-relative8 words,retained absolute child despite parent clear,root-last
fresh reread are explicit. No source enum/format completeness/runtime inferred.
Verifier two complete bodies/one panic identity,all842 body anchors,four tables/
diagnostic spans,repeat JSON,prior type4 source/dispatch/general1980 checks pass.
make test71 CTest/78 Python and make all pass;logs /tmp/id2-type5-test.log,
/tmp/id2-type5-build.log. Scoped links resolve;global2782-link snapshot has three
unresolved inventory/reference-tool-tree links owned by their source session.
Only ID2 lock released;source formats/consumers/platform/worker/scene/original
runtime/full-game requirements stay open.

ID2 DVD manager request delivery (2026-10-12),EVT-01/JUMP-01/DMG-02 partial:
[Open/global callback](../behavior/file/DVD_MANAGER_SLOT_OPEN_AND_GLOBAL_CALLBACK.md)
and [synchronous read](../behavior/file/DVD_MANAGER_SYNCHRONOUS_READ_PROTOCOL.md)
close three complete bodies. Open path-1 only,first256-slot8000 selection,
152-byte clear/name copy,LOW8 A→halfword/LOW16 C,occupied-before-SDK-open and
zero-result reset are explicit. Read publishes words/two flag RMW pairs,clears
per-request callback,wakes only on0002 and returns CURRENT length D74,not a read
result count. SetupCallback writes GLOBAL8041ebf4;worker/consumer remains open.
Verifier nine hashes(three complete/six linked),133 anchors,one startup span,
repeat JSON,prior async/acquisition/general1980 checks pass. make test72 CTest/
78 Python and make all pass;logs /tmp/id2-dvdread-test.log,/tmp/id2-dvdread-build.log.
All2823 checked local links resolve;prior inventory link breadcrumbs RESOLVED
in current tree. Own ID2 lock released;worker/init/delete/direct SDK/source/
platform/scene/original runtime/full-game requirements remain open.

ID1 draw validation failure notification (2026-10-12):make test in
/tmp/id1-draw-test.log reached72 CTest suites;70 passed,core and field_movement
failed. core_tests.cpp:131 expects lowWalk X step0.0113125;field_movement_tests.cpp:39
expects movementRate()==1. Concurrent include/field/field_movement.hpp now separates
facingDegrees from travel heading and changes movementRate ownership. ID1 did
not change native code or these assertions. Native owner should reconcile the
original-facing contract/tests;ID1 binary draw/address/resource checks pass.
Will rerun host validation after actual native source/test changes;do not label
this run green. Build is separately running;Python stage was skipped by make's
CTest failure. This notification does not reserve native code or request revert.

ID2 priority steering (2026-10-12):user explicitly requested prioritizing
DOCUMENTATION for functions the native port needs. Defer unrequested deeper
DVD/SDK recovery;own DVD-dispatch lock released without a completion claim.
Reserve native EVT-01 GetFirstAttackTarget request first,then pending movement
wrappers. Continue communication and narrow numeric ID2 locks here.

ID1 draw delivery (2026-10-12),AC-01/AC-03 partial answer:the six display
contracts linked in the released row close wrapper/inner/eval/disp/mapTestXLU
local graphs. Linked paper index/time feeds ordinary linked Main;ring/oversize
and cleanup differences explicit. Original pass/resource admission precedes
translucency forcing;GX callback and built-in states differ. Captured indices/
fraction survive late Main;group signed-nonzero visibility/current-parent sibling
arguments,cache requests,special Vivian branch and ordinary callback/render
requests are specified. No original SDK/render/material/runtime parity claimed.
Five hashes/1396 words/20 constants/five spans/two imports/148 direct sites,
original Mario mask1/root metadata and three derived addresses pass. Only ID1
lock released;70/72 host tests passed with the above two movement failures,
independent78 Python tests and3DS build passed. Logs in PROGRESS.

ID1 numeric correction:signed lis/addi low half places original tangent table
at803d9580 and zero-angle row803d96e4,not prior803e addresses. Corrected earlier
Main interpolation/init docs and ASSETS. New verifier recomputes original
Main/init tangent and modeling-stack producers from binary words;wrong unsigned-
low expectation is rejected. Native implementations should use corrected locations.

ID1 host rerun update (2026-10-12):after actual edits to
native tests/field_movement_tests.cpp,make test was rerun. It stopped during
camera_road_assets_tests linking with undefined reference to main,BEFORE CTest/
Python;exact log /tmp/id1-draw-test-rerun.log. No claim that the original two
movement failures are resolved or still reproduced by this interrupted run.
Independent78 Python tests,3DS build and binary draw/address/resource checks
passed as recorded. Native/shared-build owner should finish the fixture/build
updates before another full test run;ID1 did not edit native/test files.

ID1 final host validation update (2026-10-12):the camera fixture object now
contains main (nm/read-only object inspection confirmed an actual shared-build
state change),so make test was run again. It reached73 CTest suites:71 passed,
core_tests.cpp:131 still failed the0.0113125 lowWalk X assertion;
field_movement_tests.cpp:158 now failed standing X step0.002625 (the original
line39 rate assertion was changed by native owner). The earlier camera linking
failure is resolved for this run. Exact log /tmp/id1-draw-test-final.log.
Python stage was again skipped;the independent78-test Python run and earlier
successful3DS build remain the reported validations. No further repeat without
source/test/build-state changes;native movement owner should reconcile these
remaining expectations. ID1 binary/body/address/resource checks remain passing.

ID2 first-attack validation breadcrumb (2026-10-12):new seven-body evidence/
general checks/all2878 links and make all pass. make test passes71/73 suites;
core and field_movement fail in concurrent field changes. Exact failures/log:
/tmp/id2-firsttarget-test.log. ID2 changes no native/test files. Native field owner
should reconcile its fixtures/implementation;first-attack documentation evidence
is independently passing. Parent source/Game/runtime requirements remain open.

ID2 first-attack delivery/validation (2026-10-12):user prioritizes documentation
needed by native port. Own deeper DVD-dispatch scope deferred/lock released;
native requested first-attack local graph delivered and own lock released.
Verifier eight hashes(seven complete/one setter identity),185 anchors,one startup
span,repeat JSON/general1980/initial part source checks and all2878 local links
pass. make all passes;make test first71/73 with core/field_movement failures,
rerun72/73 with core RESOLVED and field_movement still failing in concurrent
native field changes. Separate78 Python tests pass. Logs:
/tmp/id2-firsttarget-test.log,/tmp/id2-firsttarget-test-rerun.log,
/tmp/id2-firsttarget-build.log,/tmp/id2-firsttarget-python.log. ID2 changes no
native/test files. Field owner resolves its fixture;source/Game/original traces
remain open. Next documentation:pending native movement wrappers.

ID2 movement-wrapper validation breadcrumb (2026-10-12):make test configure
fails CMakeLists.txt:341 missing tests/battle_packed_position_tests.cpp. Log:
/tmp/id2-movewrap-test.log. Four-wrapper/leaf evidence,eight source tokens,
float reader/general checks/all2903 links and make all pass. ID2 changes no
native/CMake/tests;native target owner should finish fixture,then host can rerun.
Earlier Majinai token f24a7c80 prose is corrected to+0.5,bits3f000000.

ID2 movement-wrapper validation/delivery (2026-10-12):eight hashes(four complete/
four linked),64 anchors,ten source/startup/coefficient spans,eight original
requests/six exact literal outputs,repeat JSON,float reader/first-target/general
1980 checks and all2903 local links pass. make all passes. make test configure
fails missing concurrent tests/battle_packed_position_tests.cpp;separate78 Python
tests pass. Logs:/tmp/id2-movewrap-test.log,/tmp/id2-movewrap-build.log,
/tmp/id2-movewrap-python.log. Only ID2 wrapper lock released;native owner finishes
fixture/reruns host gate. Source/Game/runtime/full-game requirements stay open.
Continue prioritizing explicit native port requests rather than deeper SDK work.

ID2 movement-wrapper host completion (2026-10-12):fixture arrived;make test
rerun passes74 CTest suites/78 Python tests (/tmp/id2-movewrap-test-rerun.log).
Earlier missing battle_packed_position_tests.cpp is RESOLVED. Prior first-attack
core/field_movement failure breadcrumbs are also RESOLVED by this current full
run. make all passed;contracts/sign correction committed ef07139. Shared request
reply records local wrappers ready for native binding;source/Game/runtime remain open.

ID1 node/matrix delivery (2026-10-12),AC-01/AC-03 partial answer:five display
contracts linked in the released row close both node builders and seven called
SDK matrix primitives plus sinf/cosf wrappers. Literal joint parent/own scale
gates have Z-axis asymmetry;all-axis zero parent fallback;secondary Z/Y/X then
doubled primary Z/Y/X. Transform builder ignores secondary angles,conditionally
admits pivot offsets only when pivot nonzero,and retains concat grouping/alias.
Dispatch tables16/8,stack publication before final concat and SDK snapshot/
paired arithmetic/store order are specified. Original Mario source has13 joint,
29 transform groups. Current bake/mesh helper's generic matrix formula requires
reconciliation before full node/animation fidelity claims;ID1 has not edited it.
Eleven complete bodies/861 words/six constants/three spans/two imports/four
actual builder sites/seven derived addresses/original resource example pass.
make test74 CTest/78 Python and make all3DSX pass;prior movement/fixture failures
are resolved in this current run. Only ID1 lock released;libm/GQR0/FPSCR/
render/material/GX/resource/source/lifetime/original visuals remain open.

ID2 menu setup/domain validation (2026-10-12):15 hashes(two complete/thirteen
linked),234 instruction anchors,two startup/Input-state table spans,all16 direct
DOL call sites,repeat JSON,prior selection/general1980 checks pass. make test75
CTest/86 Python and make all pass;logs /tmp/id2-menutype-test.log,
/tmp/id2-menutype-build.log. All2971 checked local links resolve. Only scoped ID2
lock released;native source-domain request remains partial pending full type/
class/menu/event reachability. No native/test/assets or original runtime claim.

ID1 shape/render material delivery (2026-10-12),AC-01/AC-03 partial answer:
five linked display contracts close renderProc/materialProc local request bodies.
Second integer arg ignored;matrix/GX setup before shape filter;captured shape
versus current draw/pose/resource offsets;raw e4/P4 guards;default origin/light/
texture/blend/fog/TEV order;kind1 precedence even NULL override;render-only alpha
adjustment;extra-stage smart return nonuse and fresh kind1 rebind;16-bit fan
FIFO index streams and reversed signed coordinate selectors are specified.
Original Mario24 shapes/draws,150 type0 textures,70 source calls/297 indices/
157 ALL-CALL fan triangles are metadata BEFORE visibility,not the102-triangle
idle fixture or an observed frame. Two hashes/1310 words/five spans/two imports/
three actual DOL sites/resource checks pass. make test76 CTest/86 Python and
make all3DSX pass. Only ID1 lock released;animSetMaterial_Texture/map material/
smart texture/normal matrix/GX/resource/source/alias/numeric/original visuals open.
Native mesh should preserve color/normal/UV/pass/override domains and treat its
ordinary single-texture subset as partial,not complete-game material fidelity.

ID1 texture/light/color/fog delivery (2026-10-12),AC-01/AC-03 partial answer:
seven detailed contracts in the released row close animSetMaterial_Texture,
smartTexObj/GXInitTexObjData,TEXGet,map material Light/LastStageBlend/Fog,six
light/preset/map-selector queries and comparator local bodies. Two texture passes,
reverse IDs/kind0/TLUT0/wrap/LOD,matrix grouping and unusual sy translation gate,
holder dereference/same-object data patch,color C2+0 alpha factor/staged floor255,
source-order <=8 versus sorted >8 query/postwrite capacity,replacement presets,
type priority/transforms/sticky attenuation/unrecognized-type mask and fog/channel
order are specified. Fourteen hashes/1189 words/11 constants/eight spans/two
imports/41 direct DOL sites/65536 host arithmetic cases pass. make test78 CTest/
90 Python and make all pass. Only ID1 scoped lock released;map TEV/GX/normal/
resource/pose/link/source/lifetime/numeric/original images remain open. Native
ordinary geometry/texture subset is not complete-game material fidelity.

ID1 ordinary TEV/character-shadow delivery (2026-10-12),AC-01/AC-03 partial:
three detailed display contracts in released row. Original a/a_mario24 draws
ALL mode0 (all one texture previously checked) binds actual source to base
request path. Complete mode0/generic/no-texture/constant/character-shadow/final
callback/count blocks and four shadow getter local bodies are specified.
Projection and texture admission differ;texture getter invoked twice;smart
holder patch applies to embedded shadow object;callback nil still resets its
indirect publication field,then only K/C/I reload. Six middle branch masks/
skip destinations and all13 base dispatch targets pinned as OPEN internals.
Containing TEV hash is not whole-body semantic completion. Five identities
(one partial/four complete),303 scoped words,three spans,two imports,six direct
sites/source hist pass. make test80 CTest/90 Python,make all pass. Only ID1 lock
released;other shader modes/source/GX/runtime and complete-game requirements open.
Native actor-shadow initialization/admission/resource/capture ownership is still
separate from this documented projected character-shadow material branch.

ID1 fixed TEV delivery (2026-10-12),AC-01/AC-03 partial:two detailed contracts
in released row specify modes1..7/12,25 complete stage rows/150 exact requests,
operation/output/color/alpha/order/swap and fixed K stores. Mode5 has four stages,
first outputs1/2 and third texture2;mode12 differs in alpha routing and is
explicitly requested with N2 by renderProc8003cf34/materialProc8003dba0 after
extra-stage setup. Authored Mario draw modes remain0;procedural mode12 is a
separate caller binding. Checker statically reconstructs literal arguments/
request order/final count/exit without executing original instructions or GX;
903 scoped words/dispatch/two imports/six direct sites pass. make test81 CTest/
90 Python and make all pass. Only ID1 lock released;time-driven base8/9,six
middle branches,SDK/GX/source/override/pool/resource/lifetime/original images and
whole-game requirements remain open. Other fixed mode scene execution is not
claimed merely from executable dispatch presence.

ID1 time-TEV validation breadcrumb (2026-10-12):binary pulse/scroll/constant/startup
checks and make test82 CTest/90 Python pass. make all fails current concurrent
source/display/render.cpp:342/343 invalid int→GPU_TEVSRC parameters. Log:
/tmp/id1-tev-time-build.log;renderer source SHA256 at failure inspection
39547ecf474f1fc22c5264c063377577936e5e94fcf97109761011082e000a62.
Native renderer owner should finish its enum-typed arguments,then host/build
can be revalidated. ID1 changes no native render code or asset policy.

ID1 time-driven TEV delivery (2026-10-12),AC-01/AC-03 partial:three contracts
in released row close mode8 raw GP1c/low32 double/single/sin/FMA/truncation/byte1
color-register publication and mode9 four shared phase updates/strict single
wraps/live increment/matrix-before-generator requests/three TEV rows. Mode9
mutates per material call;mode8 consumes raw clock without updating it. Startup
zero phases/increment bits are evidence,not immutable state or measured speed.
All base dispatch request paths now have local contracts;whole TEV still lacks
six middle branch internals/source/lifetime/GX/libm/original image closure.
Verifier266 words/six constants/four spans/two imports/six direct sites passes.
82 CTest/90 Python pass;3DS build concurrent renderer enum failure recorded
above. Only ID1 scoped lock released;parent full-game requirements remain open.

ID1 time-TEV build completion:renderer owner changed source hash from39547ecf
above to e2bd560e25b6c43a9f833b7b4a8efdf87d38f1aeaffa9bc2e2f8e0472e2dba70
and replaced raw0 arguments with typed GPU_PRIMARY_COLOR. make all rerun PASSES,
built3DSX;log /tmp/id1-tev-time-build-rerun.log. Prior82 CTest/90 Python pass.
Earlier compile failure is RESOLVED;ID1 made no native source change.

ID1 projected-TEV validation breadcrumb (2026-10-12):233-word/five-identity/
three-span/projected-source and prior TEV/general checks pass;make all3DSX pass.
make test configure fails missing tests/battle_packed_movement_sound_tests.cpp,
CMakeLists.txt:389. Log:/tmp/id1-tev-proj-test.log. Native battle target owner
should finish its fixture;ID1 has not changed CMake/native/tests and will rerun
after the file actually arrives.

ID1 projected-TEV delivery (2026-10-12),AC-01/AC-03 partial:two detailed
contracts in released row close flags0x400/0x20000 and four pswp query leaves.
0x400 rewrites preceding stage output1 then projected stage/swap3/shared swap
-table3/TEV register2 seed000000ff/resolve stage;K+2,B/C+1. 0x20000 double
texture getter/retained second pointer/live matrix/color/ignored config return,
request order/KColor1 selectors and K/B/C+1. cswp/pswp states stay distinct.
Containing body remains PARTIAL semantics;four middle internals/GX/source/
lifetime/original images remain open. Verifier233 words/three spans/two imports/
six direct sites passes;make all passes/separate90 Python tests pass;host gate
configure missing fixture recorded above. Only ID1 scoped lock released.

ID1 projected-TEV host completion:missing movement-sound fixture arrived,
SHA256 c200a5800f3222ee2db8593d4e5be3caad807eda898d3258917caa1d5f7caaf3.
After that actual state change,make test rerun PASSES84 CTest/90 Python;log
/tmp/id1-tev-proj-test-rerun.log. Earlier configure failure is RESOLVED. make all
and all projected/static checks pass. ID1 changes no native/CMake/tests.

ID1 terminal-TEV validation breadcrumb (2026-10-12):255-word/constant/span/
texture/byte arithmetic/static checks and84 CTest/90 Python pass. make all fails
concurrent source/display/render.cpp:341:GPU_WRITE_RGB is not declared by installed SDK.
Log:/tmp/id1-tev-terminal-build.log. Native renderer owner should use its SDK's
RGB write-mask combination/finish source validation;ID1 changes no native render
code and reruns only after actual source change.

ID1 terminal-TEV delivery/build resolution (2026-10-12),AC-01/AC-03 partial:
two detailed contracts in released row close masks0x80100000/0x800000 context/
cosine/alpha multiplication and original pulse texture/synthetic view/ortho/
clock reads/matrix/register/counter requests. Retained seed alpha255 and repeated
KColor1 overwrite are explicit;raw input alpha is not output alpha. 255 words/
11 constants/five spans/two imports/six sites/65536 host integer cases pass.
84 CTest/90 Python pass. Renderer owner fixed unsupported mask with typed RGB
combination,source SHAf2081e0ed34beac9bca8ad1407175f87b05bc54c8cf2149816dc548f57ec26ab;
make all rerun passes/log /tmp/id1-tev-terminal-build-rerun.log. Earlier failure
RESOLVED;ID1 changes no native code. Only ID1 lock released;two depth middle
branches/Ortho/LookAt/GX/libm/source/lifetime/original images/full-game open.

ID1 depth-TEV validation breadcrumb (2026-10-12):depth/source/static coverage
checks pass1115 words/eight identities/six spans/all2927 TEV words. make test
configure fails missing tests/battle_mario_jump_parameter_event_tests.cpp at
CMakeLists.txt:399;log /tmp/id1-tev-depth-test.log. Native battle target owner
should finish fixture;ID1 changes no CMake/native/tests and reruns after actual
file arrival. This does not invalidate the independent binary contracts.

ID1 depth-TEV host rerun breadcrumb (2026-10-12):Jump-parameter fixture arrived,
SHA2569963a93c4a5b2ea561e8458df55f59dd4cd917f9bcf5d0a85b49c193d9fa032c.
make test rerun reaches86 suites:85 pass;new battle_mario_jump_parameter_event
fails tests/battle_mario_jump_parameter_event_tests.cpp:46,invoke address
80109c68/result2/local6==42/initialized0 assertion. Log:
/tmp/id1-tev-depth-test-rerun.log. Earlier missing-file configure failure is
RESOLVED;this is the current native assertion failure. Separate90 Python tests,
make all3DSX and all depth/coverage evidence pass. ID1 does not edit native/tests.

ID1 complete LOCAL TEV request delivery (2026-10-12),AC-01/AC-03 partial:
four contracts in released row close final two depth blocks/seven leaves and
index all local branches. 0x10000 six routes/0x40000 only0/3,previous output
rewrite/compare op8 versus10/shared swap tables/ramp-depth matrix pairs/resolve
ordering/counters specified. Depth color returns inverted alpha-scaled RGB/
alpha0;two initial config records both3. Verifier1115 new words/eight identities/
six spans/two imports/six sites/65536 byte cases plus full rechecked composed
2927-word coverage pass;coverage hole/conflict/wrong route negatives reject.
Native owner changed fixture to SHA4f593c0194c9075b92c530b37b75bb10d062b3c2d7ee36152d2ba1067495a558;
final make test86 CTest/90 Python PASS/log /tmp/id1-tev-depth-test-final.log.
Earlier missing file/assertion RESOLVED;make all3DSX pass. Only ID1 lock released.
This closes mapSetMaterialTev LOCAL REQUEST documentation,not SDK/GX/numeric/
source/capture/cache/lifetime/US scene/original image or whole-game equivalence.

ID1 map-material source delivery (2026-10-12),AC-01/AC-03 partial:five detailed
contracts in released row close six local caller/texture-matrix/source bodies.
TEV inputs are M+0b count,descriptor M110 first byte mode,RAW O0 flags and O1c
matrix ADDRESS. Map lighting differs from character helper;named-mask/nearest
append can duplicate,missing light name dereferenced. Map texture miss uses
TEXGet-1 without fallback,ordinary object only,wrap table and forced filters1/1.
Matrix helper has descending IDs/UV sources,scale!=0 gate/all-zero unwritten
result;projected setter copies32 bytes in pairs then publishes holder pointer.
806 words/six constants/six spans/two imports/five sites pass;setter has zero
DIRECT DOL sites and no actual usage claim. make test87 CTest/90 Python and
make all pass. Only ID1 lock released;loader/source/caller/GX/numeric/lifetime/
original visuals and complete-game requirements remain open.

Documentation answer (2026-10-12, no lock, static artifacts only):
[Jump SFX provenance](../behavior/battle/BATTLE_JUMP_SFX_PROVENANCE.md). 1a: the
0x802EE470 block is `vec3_802ee470` (battle_event_cmd.o, .rodata vec3) reading
all-zero from the ISO by two independent offsets — zero scratch, not a key
row; the packed JumpPosition body never references 0x802E absolutely, so
runtime writers are unidentified and the neighbor words it loads once are dead
before the call. 1b: U+0x18C key via LoadSeMode (existing sound-mode doc) with
a two-call psndSFXOn_3D record (key + unit-340/344/348 position block; second
call inherits r3, no landing cue in-body). 1c: no Gekko PS math ops in
801093a8 (paired-singles are spill/restore only); trig via system_angleABf/
distABf; FPSCR entry unobservable. Runtime writers, signature, value domain
and audio output remain open. No native/test/CMake/asset change by this answer.

Native implementation source-audit follow-up (2026-10-12),JUMP-01:request1 is
answered by [Jump SFX provenance](../behavior/battle/BATTLE_JUMP_SFX_PROVENANCE.md)
(commit044ba5c),and two native premises are corrected.0x802EE470/474/478 is
`vec3_802ee470` zero scratch (battle_event_cmd.o .rodata),not a jump key row;
the reference JumpPosition body has no absolute0x802E read,so the neighbor
dead loads are0x802EE458/45C/460 on the double base.801093a8 contains no Gekko
PS arithmetic(paired-singles are spill/restore only);trig/distance arrive via
system_angleABf8000703c and distABf8000722c,so the "PS-family sin/cos"
premise is unsupported. Native follow-ups for the packed Jump owner:drop the
three spurious0x802ee470 reads in the battle_packed_mario_jump.cpp setup
branch,bind the numeric providers to the same-TU angleABf/distABf helpers
instead of an assumed PS sin/cos (MJP-NUM-04 differential bits still wanted),
and model BOTH psndSFXOn_3D records0x8010d594/690(inherited r3 key,unit+340/
344/348 position block,second-call position rebuild) rather than a single
play-at-setup;return handling and any landing cue remain outside the body.
Request2/3 are answered as above;request1 answered.The water/void motion,
pose-domain,marioMain and save/persistence requests remain open.No native/
test/CMake/asset change in this entry(docs-only).

Native implementation source-audit correction (2026-10-12),JUMP-01:the
SFX-provenance answer and the follow-up above were derived from the GENERIC
btlevtcmd_JumpPosition0x8010d208/0x4c4 listing (artifacts/reference/behavior/
btlevtcmd_JumpPosition.disasm,311 lines).The native owner binds
btlevtcmd_MarioJumpPosition0x801093a8/0x8c0 (us_symbols.csv;ordinary callback
at0x80356434/0x803564a0 per tools/verify_mario_jump_motion.py),a distinct
battle_event_cmd.o body.Reproduced from the read-only ISO DOL with
tools/inspect_dol_function.py btlevtcmd_MarioJumpPosition:
- r30=0x802ee458 (lis0x802f at0x801093d4,addi -7080 at0x801093e0).The Mario
  body's dead neighbor loads are24/28/32(30)=0x802EE470/474/478,exactly the
  native reads;they feed a scratch block then are overwritten by the unit
  floats340/344/348 at0x801098c4-f8 before the call,so dead.The zero-scratch
  conclusion is confirmed for THIS body;the "neighbors0x802EE458/45C/460"
  wording is the generic body's offsets0/4/8.
- The Mario body has exactly ONE psndSFXOn_3D0x800d9848,at0x801098fc:
  key=word[unit+0x18c] (lwz3,396(29) at0x801098a4,nonzero-gated at0x801098ac);
  position block=bit-copies of unit+340/344/348 (0x801098b0-f8).No second call
  and no terminal/landing SFX in the body.The "two records0x8010d594/690" and
  "second call inherits r3" claims are the generic body's;even there r3 is
  reloaded from word[unit+0x190] at0x8010d630 and gated at0x8010d634,so
  "inherits the first call's r3-out" is unsupported for either body.
- (c)confirmed for the Mario body:six psq_l/psq_st spill/restore only,no Gekko
  PS arithmetic;heading=system_angleABf0x8000703c at0x80109528 (store unit+
  0x17c),distance=distABf0x8000722c at0x80109540 (store unit+0x180),sine=MSL
  sin0x80272410 (seven sites),which the native sineDouble provider stands in
  for.
Native action applied this session:removed the three dead0x802ee470 reads in
the battle_packed_mario_jump.cpp setup branch and kept the single play call
(the Mario body has one),so the earlier "model BOTH records" follow-up is
withdrawn.make test87 CTest/90 Python pass and make all3DSX pass;diff --check
clean.Still open for the Mario body:runtime writers of the vec3 scratch,
psndSFXOn_3D full signature/return handling,the unit+0x18c/0x190 key domain,
and audio output/cadence.The generic btlevtcmd_JumpPosition0x8010d208 has no
contract of its own;if one is wanted,file it separately.No lock reserved.

2026-10-13 NormalJump used-callback source follow-up:
[Used pose/facing/declaration/AC setup](../behavior/battle/NORMAL_JUMP_USED_POSE_FACING_DECLARE_AND_AC_SETUP.md)
closes exactGetWeaponActionLv3/SetDifficulty andSetupAC(-2,1,raw1,0) reset
scope versusseparateTimingAmain0initializer; no inventedflag/parameter/history
reset. Mario42=M_R_1,40=M_W_1; usedfacing1/16/U189/Reset/subtraction; fresh
AttackDeclare no-protectorroute andWaitGuardMoveflag1; originalemptydatakey7
child, genericrecoverycounts/quantizedaccels/strictJumpContinuefloorvalues.
Remaining executedACmanager/event/pad/pose clocks andtransitivehit/motion/
audio/camera services areboundedrequests; no nativeedit/commit/runtimeclaim.


2026-10-13 NormalJump victim hit/death/variant source follow-up:
[Setup4 hit/death lifecycle](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#original-used-hit-death-and-variant-lifecycle-2026-10-13-follow-up)
pins allthree installeddamage/waitpointers, original defaultdamage/subsetdamage/
subsetdead/deadcore childgraph, 1000ms reaction and attackerU2a8 barrier,
scriptownedstatus27 afterreaction+30frames, KillUnit modebits/deferredU1c deletion.
Paragoomba originalkey0e part/kind/event replacement and Spikedkey25 Frankly
09206/07 warning admission included; namedAnimeChangePose force0 andsame-index
clockretention recovered. Remaining rewards/effects/audio/fulltransitivedisplay/
customevent/registry teardown services remainbounded; no nativeedit/commit.


2026-10-13 fresh Goomba ordinary CheckDamage source composition:
[Setup4 tuple/callback boundary](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#fresh-ordinary-goomba-checkdamage-tuple-and-callback-boundary)
pins perhitF100/20000/20100→directtupleH1,Q0,C117/20017/20117,
HP1lethalC717/20717onlyfinalF100, D260/A280/D273orderedbookkeeping,
CheckDamagewrapperrawflags/result18versusVM2/255, installedscript/rootlocals/
owner/sharedD278/currentpartD258 andU29cphaseversusU2b4hit, originalattacker
WaitAttackEnd andboundedminimalgenericDamage/deadcorecallbackaddresscatalog.
Derivedfreshscopeuseslevel1noequipment/status/bonus; noexecution/hardwareclaim.
Rewards/effects/fulltransitivecallbacksretaindocumentationrequests; no nativeedit.


2026-10-13 death-leaf/reward/ordinarywait source follow-up:
[Used leaves and rewards](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#used-death-leaves-reward-storage-and-automatic-wait-pose)
closes _disable_restore_command_cursor B1c70/B1c74zero/return2, unitattack/hit
waithelper and CheckCommandUnit gates, SetStatus27singlebyteU135write, original
coinRNG/expformula andBf00/work14stores, backitem gatingandstarpointoffsetmath.
Corrects earlier inferredwaitname: literal1 ispartID; automaticnostatuspose28
selectsKUR_S_1, notKUR_N_1. Rewardsrequireoriginalpouch/stage/currentitemowners;
no nativeedit/commit/executionclaim, boundedtransitivesremainopen.


2026-10-13 native hit/reaction increment: original Goomba damage/wait wrappers
and DOL generic reaction now execute with source HP/accounting and event
ownership. First missed timing completes one-damage recovery/idle; two hits
reach the original death child and pending AddRotate801115f8. Source calculation
retains the10000 code bit when flags100 and defender attribute100 is absent;
native fresh missed-hit words are10117/10717. The earlier simplified tuple
summary117/717 omitted that calculator bit. Full successful timings preserve
20000/20100 requests. StopAC countdown/callback cleanup and stylish windows
are bound. Spike counter, wing-fall, death presentation/rewards/teardown, original
seed ownership and turn scheduler remain unfinished. This is implementation
progress against the delivered contracts, not completion of JUMP-01.


2026-10-13 rotation/deletion/packedclear/reward-pointer follow-up:
[Rotation/teardown](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#rotation-deletion-order-packed-clear-and-reward-pointer-correction)
pinsusedintegerAddRotate/SetRotateOffset, reviseAngle/sentinelpolicy,initialunit
matrix/pivotorderwithfinaldrawbranchesretained,64unitbroken scan/exactDelete
wait-phase-hitcleanup/lateunregister, sixstatusinfo recordword8preservation.
Correctsrewardpointerdomain: FieldInfo+c→NpcInfo, NpcInfo18→group26460(count4),
NpcInfo0→stage2619c. GetExpfactor1.3usesgroupcount,notstage0. Earlierstageclass
proseissuperseded; numericalGetExpaccessesareunchanged. No nativeedit/commit.


2026-10-13 extraSetRotate/removalcadence source evidence:
[Additionaldelivery](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#rotation-deletion-order-packed-clear-and-reward-pointer-correction)
pins rawoperandtoken sentinelSetRotate(oldaxis substitutionthenallaxisnormalize),
original marioStMain animMain→lowercasebattleMain→uppercaseFIRSTUnitBroken→
otherbattleproviders→camera/window/map/hit→evtmgrMain. EventpassN KillUnitmark
thereforeretiresbeforeeventpassN+1,notinsidecallback. Fullfinalpart/billboard/
projectionconsumerremainsboundedrequest; nativeallocationadapterisnotoriginal.


2026-10-13 requested final used Goomba death display branch:
[Final used path](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#final-used-ordinary-goomba-death-draw-path)
closes sourceP4fc0 initialization, attr9 normal branch, death08000000 versus
mirror01000000 gate, sourcefacingangle/camera inclusive90..270 negativeZscale,
unit/part ordered concatenations, depthTz-5/modelscale2 andfreshphase0offsets.
Earlier initial-matrix-only boundary is superseded for this fresh used algebra.
Numericalcamera/pose-flag/retained animation/hardware consumers remain bounded,
no nativeedit/commit/runtimeclaim.


2026-10-13 fresh c_kuribo battle poseflag/inner draw gate:
[Fresh inner admission](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#fresh-battle-c_kuribo-inner-draw-admission-flag8-clear)
pins animPoseEntry freshflags1/mask8and100clear/linkedID-1, absence ofbattle
PeraOn versusfieldNPC producer, usedSetAnim/Main/material/statusproducer scope.
OriginalfreshinnerusesdirectA right-Yrotation anduniformscale2, notretainedP74
camera smoothing. FullusedMdraw equationincluded; priorfreshposeflagboundary
superseded. Camera numericalunitmirrorgateandrenderingdomain remainbounded.
No nativeedit/commit/executionclaim.


2026-10-13 native death/reward follow-through: fresh Goomba Jump now executes
original48-spin/30-tip/death/reward/KillUnit streams, packed status preservation
and deferred next-call registry removal. PMTHIT2 retains the original group
pointer; GetExp uses source group count4/factor1.3 and kindlevel7/pouch1,
GetCoin retains its draw, and stores retain source owners. Source unit/mirror/
direct-A draw algebra is implemented for the recovered fresh branch. Mario's
original80417c38 wait stream now returns to M_S_1. Original particle/audio
requests are retained; turn/variant/end/pouch/seed/hardware providers remain
open. This advances the native implementation, not full JUMP-01 closure.


2026-10-13 requested source Mario→Goombella progression composition:
[Source handoff](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#setup4-first-mario-action-to-goombella-command-handoff)
pinsAct7..14all-unit/battle/postbarriers,count1→0/state10/Mario31196/reservations,
Move10→8nextoutercall selectionfromexistingphase2table, noFranklyreplaybetween
actors, exactphasehookgates, Goombella originalinit/wait/HPsnapshot/Headbonk
selectedweaponversusemptydefaultattack. Newlyinspectedannouncementunitliteral0,
emptymsgworkpath, actualAudience/BreakSlotreadinessownerfieldsincluded.
Openuseddependencies: Audience_PerAct801a7e94/440reaction/RNGwriters, slot-token/
state producers, nonemptyMsgAdjust/Main/queue/string/effectservices, fullselected
Headbonkcommandgraph/camera/input/pouchowners. These arenotclosedbygeneric
controllerdiagrams oradapter0readiness. No nativeedit/commit/runtimeclaim.


2026-10-13 Headbonk used transitive callback delivery:
[Headbonk source callbacks](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#headbonk-used-command-coordinate-and-rotation-callbacks)
closes CommandCost/ActionLv/PreCheck/Check selectedweapon/currentmenu ownership,
rawflags/output/2vs255; typedGetPosFloat/GetRotate rawsentinels, direction-local
add andface/scaling fields, ChristinekrbXYdelta→origin math, actualsource227cmd
PKR_A_2B/PKR_J_1A brother/inline rotationcounts/pivot/recovery arithmetic.
Existingcost/selector/numericcontractsremainauthority. Used ACR/stylish/prize/
help/audience/audio/camera/menu ownersretaintheirrequests; no nativeedit/commit.


2026-10-13 next-step evidence persisted before native Headbonk commit:
[Phase11/variants](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#next-source-step-move11-to-enemy-phase-and-variant-callback-admission)
closes exactphaseaudience mask8-clearearlyreturn, PerPhase2versus4flags/state
writes, playerphase2→emptyphase3→enemyphase4 (no turnreset), Turn4c/4d storage,
sourceenemyAct3/installattackrootadmission. Headbonkspike tuple/warningowner,
Parafallrawtable/current-kind-only/partreplacement field preservation recovered.
Pending native: Move11/audience/camera/phase3/4 ownership, source enemyitem/
samplingchoice/RNG/attack/guard paths, spikecounter andwingfall execution/audio/
effects/pose recreation. Fullmask8SETphaseaudience/controller andsourceflag
producersremainrequests; useoriginalcleargateasboundednextstep,notalwaysready.
Documentation-only; no nativeedits/commit. Existingnativeappendsectionsunchanged.


2026-10-13 fresh Move0..7/EnemyAct0..6 source delivery:
[Exact phase scheduling](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#exact-fresh-phase34-scheduling-and-first-enemy-command-abi)
pinsphase3emptyactorvsphase4actorlist, selectors/IDmark/keypreservation,
Move6none→10predicate→8nextoutercall, PerPhaseendtiming, enemyAct0→3→6 root
owner/waitcancel andnewAudienceActInit flagclearbody. Firstenemy nohelditem
leaf, GetEnemyBelong, packedSampling/Choice ABI/weaponflags documented.
Correction: Sampling biasflip W68&10000000 (PPCMSBbit3), never20000000.
Spiked/Para choice80000000 meanssource deterministicmax, nochoiceRNG;
actualcandidates/weightsremainoriginal. All admittedunclosedbranches have
sourceaddresses; no nativeedits/commit ornewexecution/fidelityclaim.


2026-10-13 Spiked used callback/posefallback follow-up:
[Face/scale/defense](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#spiked-original-attack-used-facescaleoffsetdefense-and-pose-fallback)
closesFaceAdd local-token/signedU189/wraporder, actualAddScale801103dc typed
quantized4tickdeformation, integerSetDispOffset U54fields, originalJumpContinue
firstcall/strictfloor/return1 reuse andResultACDefence storedB1cb0/privatepad.
Immediateblockingmissingtype43 resolvedbyoriginalcategory69fallback TGK_S_1;
forcepolicy29..39=1 versus43=0documented, noinventedpose. No nativeedits/commit.


2026-10-13 blocked relocated Spiked krb helper closed:
[Verified helper alias](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#verified-relocated-spiked-krb_get_dir-semantic-alias)
pins textsection1+5b2c/file5bf0/native93105b2c/sourceVA805c0590/rawSHA,
all8DOLcallrelocations andlocal+0constant, identicalusedfiveoperand/math/output
semanticsversusDOLChristine80189910. Typedproviderreusevalidatedbyoriginal
operations,notnameguess. No nativeedit/commit/originalexecutionclaim.

### 2026-10-13 delivery: live unit scale/display matrix placement

Rechecked original btlUnitPartsDisp80101774 used P4fc0 branch and appended
`Live unit scale and display offset placement during Spiked attack` to
[the trio contract](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md).
U54/58/5c translation is left of unit rotations; U90/94/98 scale is right of
Rx*Rz*Ry, left of incoming animation yaw/model scale2. Source mirror rebuild
negates live Z scale, preserving current X/Y. Fresh reduction and exact load/
concat addresses are recorded; full nonzero pivot/part algebra remains above.
Numerical camera provider remains bounded. Docs-only; no native edits/commit.

### 2026-10-13 delivery: fresh enemy impact and installed player hit roots

Appended `Enemy impact: private defense state and installed player reactions`
to [trio contract](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md).
Rechecked full800fa1b8: fresh no-input result2 storedB1cb0; target305/306/307,
override signedB1d18, windows and common private reset pinned. Actual private
pad producer/cadence already closed in input subsystem; do not request it as
undocumented or merge it with general attack history. Remaining override/auto
writers and hardware/paused timing retain bounded input requests.

Mario80417c08 and Goombella80393e68 installed damage roots plus hashes/order
recorded; both original defaultDamage, Goombella firstrotationreset. SpikeW20=2,
ParaW20=1, callback80189d30 literalwordreturn; player elemental defense0 and
originalattrs yield fresh F100code10117, positive HPsubtract2/1, result18.
Current pouch HP remainsowner. Original generic type39 M_D_1/PKR_D_1 wait1000ms,
missingdatakey0a and used helper80113f1c U16+1fe OR1 are pinned. Smallest next
native boundary listed; lethal/guard/status/physics/sound/effect obligations
remain explicit. Documentation-only, no native edits/commit; git diff --check.

### 2026-10-13 delivery: enemy source recovery and Para used DivePosition

Appended `Enemy recovery: source Spiked bounce and Para DivePosition` to
[trio contract](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md).
Original Spiked impact→brotherrotationreset→28/12/6bounce→homewalk graph pinned.
JumpContinue reuses complete jumpauthority (strictfloor,return1,noNdecrement).
Para nineoperand intXYZ ABI, overwriteN0 autoduration from3Ddistance/speed,
retained origin/current/target/distance/elapsed fields, usedmode0 sineYoffset,
endpoint substitution and finalelapsed>N return2 pinned; originalintpl_sub
used0/1/4 PPC signedinteger products/order documented. Numeric PPCsqrt provider
is explicitly bounded, mode!=0 and groundclampedcurves excluded from usedscope.
Player original fresh hit child callbacks and existing U1fe helper listed.
Spiked/Para statuspayloadzero/power2/1 and usedguard/superguard originaldirect
counter tuples/return/cancellation responsibilities pinned. No native edits,
commit or original execution claim; git diff --check validation only.

### 2026-10-13 delivery: SetMoveSoundDataWork source producer

Closed native pending80105020 in
[unit sound mode](../behavior/battle/BATTLE_UNIT_SOUND_MODE.md#source-event-producer-setmovesounddatawork)
and linked from trio contract. Complete wrapper/ec + SetSeMode80126558/30:
eightoperands allGetValue (keys included), shortnarrowparameters; exactU18c
writeorder and preservation pinned. Important originalr8/operand5 ignored;
operand4 reused for bothperiodhalfwords, half12 preserved until laterLoad−2.
Noaudio/Load/Offinsideproducer; retainedvoicewordreset doesnotstopvoice.
ConcreteSpiked/Para authoredkeys/3and15periodoperands pinned. Native integration
and originalaudio playback remain nextboundaries. Docs-only/no nativeedit/commit;
git diff --check passed.

### 2026-10-13 delivery: phase4 audience empty drain and superguard termination

Appended `Phase4 audience drain and superguard callback cancellation` to
[trio contract](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md).
PerPhase4 mandatoryirand100/throwmax/reset fields thenflag8/state0 pinned.
Original21-entry table8039ceac mapped; zero-record/queue path follows threepolls
0→5→7→13(1),13→12→15(1),15→BreakSlotDec/clear8/count0(0). Spectator
GetExist/SysCtrl masks/stride and retainedqueue/statefields pinned. Empty native
inventory isexplicitadapter, notoriginalpopulation; nonemptygraph/creation/
resources/SP/items/bingo remainboundedrequests. Originalbreakturnsubtraction
wrap/signedclamp and state20cleanup distinction recorded.

SuperguardW13=1 A280/D273 BEFOREdefenderDirect thenattackerDirect and−1return
confirmed; nosecondcommontailincrement. NegativeCheckDamage wrapperreturns255
NOoutputwriteandNOevtCheckID; nonnegativewritesoutputbeforeIDcheck. Original
ownattackcancellation remainsVM/lifecycleowner. Docs-only/no nativeedits/commit;
git diff --check passed.

### 2026-10-13 follow-up: phase6 turn boundary and player level byte

Appended exactTurnbank3Bf14/Phase4Bf18/countS16B0, phase6→Turn4d, ordinary
BattleAudience_Case_TurnEnd8019f298(void) identity/graph and nextTurnFirstProcess
providerorder totrio contract. OriginalplayerU+d producer recoveredEntry801290f0:
pouchS16+8a→lowbyteU+d forMario/partners (kindbase+0efallbackpreceded).
Currentnativefreshpouchlevel1adaptercanpopulateit; zeroU+dwouldinvalidate
TurnEnddivision. TurnEndoriginalgroupmath,mandatoryRNG andtargetcount/category
leafprovidersbounded; emptyadapterdoesnotmakethosecallsabsent. No native edits/
commit; git diff --check passed.

### 2026-10-13 delivery: used WaitEventEnd original polling ABI

Closed source btlevtcmd_WaitEventEnd8011389c/3c (SHA256291f1a6b795d1e9b69a1413fb71db88f1d3d7946c07eda59a2ac59aac6f3d95e)
in [trio contract](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md).
OneGetValueintID→originalevtCheckID currentcontext activebit1/ID15c scan;
live→0,missing→2. No unittranslation/firstcall/output/reset/delete/return1.
Zero followsnormalchecker, no specialbranch. UsedsubsetDamage80350cd8 polls
precedingbrotherLW0; its originalmotion/interpolation dependenciesremainowners.
Docs-only/no nativeedits/commit; git diff --check passed.

### 2026-10-13 delivery: confirmed superguard interpolation8034f4e8

Closedusedinit80056120(5,0,1800,40) andactualgetter80055f90 in
[defaultactionevents](../behavior/battle/BATTLE_DEFAULT_ACTION_EVENTS.md#used-superguard-counter-avoid-spin-and-shared-interpolation-state),
linkedfromtriocontract. Originalavoidcounter8034f4cc/88SHA6a79e8caf897eaa527b0840f0954471e69800775942383c4c5703112752cc0ab
source12cmdstream/40brotheriterations/parentpollpinned. SourceLW11..15shared;
paramgetterRAWoutputs floatLW1 thenintLW2,step++unconditional,2. Usedmode5
originalfloatcubic path80006eb4,table8030434c,exactoperationorderdocumented;
notintegerintpl_sub. Lastsamplet39trunc1799→yaw359,no fabricatedfinal0.
Prior EVENT_INTERPOLATION... c8/cc labels explicitlysuperseded; mode5 scoped
cameraInterpolation adapterpossible, compiler/FPSCRvalidationremaining.
No nativeedits/commit;git diff --check passed.

### 2026-10-13 delivery: counter recovery floor8010510c and pose49

Closed sourceTransStageFloorPosition8010510c/c0 evaluatedintX/Z, RAWYoutput,
intfloortrunc/return2 in
[jumpcallbacks](../behavior/battle/BATTLE_JUMP_EVENT_CALLBACKS.md#used-counter-recovery-stage-floor-wrapper-and-original-initialization).
GlobalembeddedstageB163fc; sourceInit B16f28=90,B16f30=−50 pins/constants;
usedZ<=90floor0provablewithoutinventedheight. Fullstagewriters/transitive
init remainbounded. Triocontractlinksclosure/pendingnativeboundary and
originalpose49MarioM_J_1B/GoombellaPKR_A_1 banks/force0; nofallbackidleinvented.
No nativeedits/commit; git diff --check passed.

### 2026-10-13 delivery: exact TurnEnd queue/zero population and nextTurn owner

Appended `TurnEnd audience queue leaves and bounded next-round controller`
to [trio contract](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md).
Originalgroupsum23/level1threshold46; Turn1A13908+3,RNGchance/categories;
AddPhaseEvtListduplicateNOwrite/order/reloadedcount/noexplicitcap pinned.
NumToTargetA1377c+13780/13790,GetExistflag1excludemasks100/2,statusbyteHEX19
excludeDEC12,AddAudienceNum P8c/amount0/Bef4100gates pinned. Freshdelta0 has
noRNG/write; populationnonzero remainsrequest. Emptyrecords canfaithfullydrain
actualTurnEndqueuedcategories throughsourceeligibility, notwholesalequeueclear.

ExistingnativeBattleTurnFirstServices/battleTurnFirstProcess APIs identified;
sourcefreshrule/fortune/fog/priority0gates pinned. Turn2→3→5→6Poisonowner
8011d898 exactrecursion documented; existingperiodic/regen/recoveryauthorities
and40..4b tailpackedwrites linked fornextactualround/phase2 readiness.
No nativeedits/commit, no fabricatedmenu/return2/victory;git diff --check passed.

### 2026-10-13 follow-up: Frankly original kind resolution for TurnFirst

Triocontractpinsfourthoriginalgrouprecord93518a00→kind9352ff60 (file3d990),
attrs12000000, levelbyte1, signedpriority/span88/890, parts/init/data pointers;
SetMaxMoveCount0→sourceTurnFirstreset0/state10. Native dummyKinds6shouldbe
replacedthroughimportedgroupowner beforeusingfreshoriginalTurnFirstservices.
No nativeedit/commit;git diff --check passed.

### 2026-10-13 delivery: ordinary Hammer used underground trio binding

New [battle contract](../behavior/battle/ORDINARY_HAMMER_USED_TRIO_BINDING.md)
pins originalW80353de8→wrapper80416c40→387cmdcore8035d6d4; freshmask2clear
N4/T25/difficulty3,approach/precheck,level1M_H_1A/B/C,actualResultAC→WAIT7
impact and100/20100branches. ExistingHammerCommand/ActionCommandView/shared
VM/damage APIs mapped; packedID3main/result/delete lifecycle/heldgeneralpad/
sharedforcedcounter/levelP99/Hammerpower80189c38 remain concreteintegration
requirements. No fabricatedfixedimpact schedule orautoselectedHammer;
originalJumpSpikedFranklywarningremainsconditionalsourcegraph. Display/audio/
nonfresh/stylish/firstattackproviders remainexplicitrequests. Documentation-only,
no nativeedits/commit;git diff --check validation only.

### 2026-10-13 delivery: Hammer used evt_snd_sfxon8015235c

ClosedoriginaleventtwoRAWoperands/playback/output0discard/noGetValue/return2
in [audio wrapper](../behavior/audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md#original-evt_snd_sfxon-raw-event-wrapper),
linkedfromHammertrio contract. Actual8035e128 keySWING1 output0requiresno
fakehandleforretainedoriginalsoundrequest; nonzeroRAWoutputneedsactualvoice
providerorremainspending. Underlyingmacro/sample/playbackremainseparate.
Docs-only/no nativeedits/commit;git diff --check passed.


### 2026-10-13 delivery: original Paragoomba wing-fall used binding

[Wing-fall contract](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
pins W74/part800/F100 admission, actual code410117 fresh failed H1,
tracked generic damage→data0e→fall93521ee4 child and full26-command order.
Grounded pose/data/parts93522728/27c8/2800 recreate original c_kuribo KUR poses;
ChangeKind writes only U8, preserves original U10/HP/actor identity. ReplaceParts
reset/preservation fields, OffAttribute/OffParts RAW masks, OnUnitFlag U1c,
all grounded event-install and evaluated sound-key packed writes are closed.
Ground init9352290c exact wait/attack/damage/confusion/sound/StartWait sequence
included. Air admission becomes partattrs9 at replacement; no invented hide-wing
or HP-based scene trigger. Original last-enemy result selection enters end7
07000000 then FieldBattleInfo+3c callback or audience-end admission8021290c;
full end/reward/field return remains a concrete dependency. Docs-only/no native
edits/commit; read-only source recovery and git diff --check validation.


### 2026-10-13 delivery: wing-fall SetHomePos801123d0

[Wing-fall ABI](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
closes reached SetHomePos801123d0/128: four evtGetValue operands in order,
actor translated, signed integer XYZ converted to float; leaf801289a0/10
writes U30/34/38 only. No sentinel, evtGetFloat, output, first-call/current
position/offset/floor query. Native must preserve original integer GetPos
locals before storing home, not invent fractional inputs. Read-only source
listings /tmp/trio-para-sethome.dis and leaf listing. Docs-only/no native
edits/commit; git diff --check passed.


### 2026-10-13 delivery: grounded Paragoomba attack93105a14 angle helper

[Wing-fall contract](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
pins reached93105a14 to original krb_get_dir@unit_patakuri.o, not distance.
Four float inputs old/newXY and evaluated prior integer output; dx/dy bothzero
writespriorangle, else systemangleABf→fctiwz→integer output. Exact REL imports,
zero constant, raw helper hash and used groundattack306f4 five locals recorded.
Semantic provider reuse with Spike/Headbonk validated; no invented sqrt/pow or
names-only alias. Docs-only/no native edits/commit; git diff --check passed.


### 2026-10-13 delivery: battle missing PTK_R_2 source fallback art

[Wing-fall resource contract](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
pins shared battle animPoseSetAnim exact-search/exhaustion→descriptor0 policy.
Original a/c_kuribo row0 KUR_Z_2, descriptor6b74, nonloop0..20/one key0,
three visibility updates only, visible static135-triangle geometry. PTK_R_2 is
absent; KUR_R_2 exists and is a separate original10-frame run. Bank/hash/source
provenance and host evaluated geometry bounds recorded; numerical host adapter
not claimed as original raster observation. Native subset fallback must retain
source KUR_Z_2 rather than current packfirst KUR_S_1. Docs-only/no native edits
or commit; git diff --check passed.


### 2026-10-13 delivery/request: original trio ending controller

[Ending contract](../behavior/battle/UNDERGROUND_TRIO_VICTORY_CONTROLLER_AND_FIELD_RETURN.md)
pins exact32-entry end-bank jump table, distinct F/I/NPC setup owners, fresh
constructor/preserved F3c path and alternate actual callback/resume branch.
Closed00/05/06 order, source audience Great/Fast bonus+complete AddTargetNum/
Sub float ABI/clamps, Koura originalchild/freshU10420clear leaf, resetcolorU311ff,
ordered per-ID SetStatus0..24 with Charge retained/19,20move recalculation,
owned-event barriers/remaining all-unit3f keys, DropMaterial owner and source
EXP/reward/export/fade/publication/field continuation dependencies. Original
MarioM_V_2/GoombellaPKR_Y_1 victory scripts identified. No result/pouch/story
completion fabricated from nativeHP0. Next bounded requests: generalF3c producer
outsidefreshscope, exactDropMaterial RNG/equipment/fielddrop consumers, remaining
end display/effect/audio/EXP/rank/level services and actual field reconstruction.
Docs-only/read-onlyISO, no native edits/commit; git diff --check passed.


### 2026-10-13 delivery/request: complete trio DropMaterial and pouch prerequisites

[Drop/entry-pouch contract](../behavior/battle/UNDERGROUND_TRIO_DROP_MATERIAL_AND_ENTRY_POUCH.md)
closes full801141bc HP→FP→item pass, source current unit ratios/upper-only cap,
actual five used rows, exact draw order and314 badge queries/fallback25 words.
SourceI14 coin versusS14 flower owner corrected in ending contract. Original
setup item producers drawSpike/Para229, selected/retained/back-item slots,
fieldgetter masks/311,312 badge RNG/countcoin hierarchy and generic death
consumer/actualtrio separate admission pinned. Sourcestg0_00 callback grants
6/0x32/9, source keyinsert/tier writes and freshRevise/import/partnerFP facts
closed. Actualordinarytitle/newgame/boots6 lifecycle remains bounded request:
onlyseq_e3Main directstage-table query callers were found, not production
newgame proof. Native providedentrypouch is explicit persistent-owner adapter,
not authorization to reset liveHPFP or invent save ownership. Sourcehashes,
listings/fieldinventories recorded; no native edits/commit; git diff --check.


### 2026-10-13 delivery/request: trio EXP source presentation/counting

[EXP contract](../behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md)
pins08FogForceStop/StockExp1..100/fortune/level gates, samecall09timer119,
0a actualvictorychildren/PointGet,0b originalheldbattlepad300 counting/RNGfree
pouch96 progression. CompleteDispMain has noE0=Bf24 write; sourceWhistle E0
mustnotberepaired. All19staricon fields/init/Main/end and original404 four
40pxframes418..421 delay6, sourceframewindowrequest pinned. PointGet phase11
cos easing/hold/softdelete and originalUSphrase+digit texture124..131/US6..13
sourceart inspected. MarioM_V_1/2 b_mario andGoombellaPKR_Y_1 c_pkr verified;
type43actualcategory69→PKR_S_1 clarified, notfirstrow/directoryfallback.
Remaining exact innericon clock/window/GXquad matrices/GPU viewport/parity,
MSLcos numeric provider, completeeffectaudio and laterEXP/level/export/field
providers remain boundedrequests. Docs-only/no native edits/commit;
git diff --check validation.

### 2026-10-13 delivery: fresh ten-point no-level-up ending tail

[Ordinary EXP ending tail](../behavior/battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md)
pins per-invocation0c..1f, ordinary skip0f, five-call effect pointer release
independent of PointGet leave/deallocation, exact voice/bodytype42 gates,
source fade10 (200ms, strict ratio>1, GP4 refresh, selected-slot flags2),
ordered actualunit→pouch export and field-party reconstruction. Rule0 writes
I19=6, not invented return2. Normal ending does not heal HP/FP or directly
unlock/story-advance; original battle/sequence/field wait remains required.
Other level-up, zeroEXP, fortune-end, fade/GX/audio/field constructor services
remain bounded dependencies. Read-only original bodies/constants; docs-only,
no native edits/build/commit; git diff --check.

### 2026-10-13 delivery: original EXP icon creation clock/window frame

[Icon clock and frame](../behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md)
pins iconEntry2D memset/localframe0/currentimage418/delay6, per-record Main
countdown/wrap/reload and hiddenclock advancement. Original outerupdate
creates in battleMain then decrements in iconMain samepass. Windowtype0
uses original f/msgWindow.tpl descriptor0 (72x72CMPR), exactnine-slice36vertices
UV0/thirds/1, color*texture alpha andsourcecamera. Native raster/palette/camera
services remain explicit; no globalclock or substitute rectangle needed.
Readonly bodies/descriptor/TPL header and180floatFIFO symbolic census;
docs-only/no native edits/build/commit; git diff --check.

### 2026-10-13 delivery: complete used US PointGet layout/lifecycle

[PointGet](../behavior/effect/POINT_GET_US_LAYOUT_AND_LIFECYCLE.md) pins original
36-byte payload, creation/update same-outer-pass ownership, pre-update depth
versus live draw XYZ,45enter/softdeletehold/60leave. CompleteUS10 widthL322/
scale200/322, two ordered passes/black128shadow offsets4,-4 versuswhiteFG,
FOREGROUNDdimensions forbothpasses, exactphrase/digit cursor/quad/UV matrices
and originaldigit1.42857146 scale. No generatedlayout or extraeffectcompletion
barrier. Displaycallback inherits blend/cull/depth fromoriginaldispatcher;
sourcecamera/GXsampling/numeric/raster/provider lifetimes remainbounded.
Read-only whole localbodies/constants;docs-only/no native edits/build/commit;
git diff --check.

### 2026-10-13 delivery: exact EXP404 bottom anchor

[Icon draw follow-up](../behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md#2026-10-13-used404-draw-anchor-follow-up)
confirms actual1011 iconDisp→iconGX XY(-W/2,H),(W/2,H),(W/2,0),(-W/2,0):
centerX/bottomY. BigY25/.9 spans25..61;smallY0/.45 spans0..18; usedmask10
bypasses.6/special403/405. Exactcamera*T*Ry(-yaw)*scale andclearflipmasks
retain sourceadapter boundaries. Docs-only/read-only/no native edit/commit;
git diff --check.

### 2026-10-13 delivery: independent EXP count/pad/icon-end audit

[Count follow-up](../behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md#2026-10-13-independent0a0b0c-audit)
pins0a noE4reset/nofallthrough; sourceheldB1034&300; onceonly1000fastlatch;
onepointpercall, originalSFX_STAR_POINT_GET1 onlytransferscounterlow3zero;
thresholdcheckevenno-transfer, actualP96reset/E0c/P8a+1>=99. Lasttransfer
remains0b;nextcallsets0c delay. Freshnormal10delay100/fiveSFX;firstheld
delay10/oneSFX. ExpIconEnd clearsE30mask2, iconDelete onlyactive1bit, retains
all19E pointers/recordbytes; nofree/wholememset. Docs-only/read-only/no native
edits/build/commit; git diff --check.

### 2026-10-13 delivery/request: ordinary trio field-return handshake

[Field handshake](../behavior/battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md)
pins source sequence5→6/counter>30/GP alternatives, seq-game originalretained
fade presets/constructor9/300, BattleExit ordered originalresource/context/event
restoration and actualwaitseq2&&I10nonzero. Authoredgor_01 christine_nakama
374b0 evt_pouch_party_join(1)→P0eOR1 belongsjoininghistory, not1f/newgame
heal. marioCtrlOn byte38nestedcount, globalpartyCtrlOnBOTHslots20000000,
Postoriginalappearance/flags andunchanged161-commandmessage/story12tail.
Nextboundednativeproviders: BattleEnd800f8da4/transitivecontextrelease/field
rootpause-resume; actualpartyEntry/HelloMain/partyGetAppearPos800c6c9c;
originalGPpresets/sequenceexit-init ordering and Progresspouchjoin/saveowner.
Read-onlycompletebodies/tables/joinrecord/nativeAPIcheck; docs-only/no native
edit/build/commit; git diff --check.

### 2026-10-13 delivery: original used fade10 texture/NPC namespace

[Fade10 consumer](../behavior/display/BATTLE_END_FADE10_TEXTURE_CONSUMER.md)
pins completeused disp_texture: embedded1x1I4FF80415020, NO externalTPL/palette
lookup, identityUV/matrix, alphaByte=low8trunc(single255*slotratio), blackRGB,
sourcealpha blend, camera8projection/view,608-squarequad±304 andprojection
restore. Type10doesnotcompletedflag2skip; solidblackprimitivecanmatchthis
usedwhite-texture*blackmaterialunderdeclaredcamera/coverageadapter. Actual
GX/camera/clip/rasterparity andotherfadeart remainbounded. Ending1c loadsF8
thenNPC140OR100 at8021654c..54, separatefromNPC0/visibility. Readonlywhole
body/table/constants/texel; docs-only/no nativeedits/build/commit;diffcheckpass.

### 2026-10-13 delivery/request: ending mapfree/fieldparty fresh constructor

[BattleFree/End](../behavior/battle/ENDING_HEAP_FREE_AND_REMAINING_UNIT_TEARDOWN.md)
closesBattleFree(pointer):nullskip,actualmapheapbase8041e80c/_mapFree,notSDK
__memFree; header32/live8/size4/next0 orderedneighborcoalescing, noEpayload
clear. LaterBattleEnd64slotliveDelete thenicon/menu/stage/audience/breakslot,
optionalB1c78free/Bfree/globalB0.
[Constructorcomposition](../behavior/partner/FIELD_PARTY_POSITION_CONSTRUCTORS.md#2026-10-13-battle1f-ordinary-goombella-constructor-composition)
resolveskind1c_pkr/PKRS1versusHP<=5PKRD2,f4=26,length56,partyEntryposition
helper2twiceviafreshHelloMain10,not16-countsubstate0jump. ExistingMain/general
appearance/fallbackcontractsauthority; GetAppearPos2_800c6718 remainingfull
typedray/classification/caller contract/nativeproviderrequested, plusactual
registry/allocator/model/context. Readonlywholebodies;docs-only/no nativeedit
build/commit; git diff --check.

### 2026-10-13 delivery: source E BattleAlloc/mapheap bootstrap pairing

[Mapallocationbinding](../behavior/memory/BATTLE_END_MAP_ALLOCATION_BINDING.md)
pinsending05actualcall802154e0→BattleAlloc800f8934(2ac),not88d8; wrapper
usesroot8041e80c/_mapAlloc8002ff58. Complete32-byte roundedbestfit/firsttie/
mandatorysplit/zero/DCflush andE2ac→2c0 payload/2e0 span, nestedsourcearena
64000/root63fe0. Existing eventmapallocation/memorybootstrap/subpoolcontracts
areauthority; parentprimaryrootandSDKheaders remainseparate. Correctfreepair
usesinnerroot; no12-byteSDKheader, noE-onlyarenaoriginalhistoryclaim or
staledouble-free/idempotenceassumption. Docs-only/read-only/no nativecode
build/commit; git diff --check.

### 2026-10-13 delivery/request: preservedtrio postwait callbacks/nativegaps

[Returnaudit](../behavior/story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md#2026-10-13-preserved-native-return-callback-audit)
correctstag61fieldwait/62prev-partyrestore(no62triousage); orderedpost093/
094_kur/094_01extparty/story12sourcegraph;fieldNPCtribePTK persistsbattle
wingfall anddamageforce1. Concretecurrentnativegaps27/29partyRunStopdefault
success,54a_recnotapplied,70BGMon_fwithoutactualfadeowner; originalfield
MarioWord4/currentindices/partyconstructor/controlcounter/pouchjoinand
separatefieldVMpausedchildren ownershiprequired. Noforcedunlock orstorywrite.
Read-onlysource/nativeaudit; docs-only/no nativecode/build/commit;diffcheckpass.
