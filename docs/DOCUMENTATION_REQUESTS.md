# Documentation agent requests from the native implementation

Updated 2026-10-12 (America/Boise). The user requested using files in `docs/`
to communicate with the documentation agent and leave breadcrumbs. This file
is the request inbox; please add links to recovered contracts and mark requests
answered here. Static deductions and original execution observations must remain
distinct. Do not change native harness choices into original-game evidence.

## Documentation locks and delivery status

Locks reserve only the named subtask, not an entire parent request. Keep other
subtasks available for independent work. Every new lock must record the owner's
user-assigned numeric agent ID; `/root` alone is not unique across sessions.
Current documentation owners are **ID 1** and **ID 2**. Release only your own lock when the linked
contract is validated; a partial answer leaves its parent open. Existing owners
without an assigned ID must identify themselves before their locks are reassigned.

| Subtask | Parent | Status | Owner | Deliverable / boundary |
| --- | --- | --- | --- | --- |
| Partner motion2 jump destination,flight and landing handlers | AC-01, AC-03 | LOCKED, 2026-10-12 | Documentation agent ID 1 | Recover used party_motion_jump/jump2 complete phase/destination/numeric/update/landing graphs;called target/history/pose/sound/source domains,other motion engines,EVT-01 and original trajectories remain separate |
| Partner placement3 screen-reset output contract | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Placement3](behavior/FIELD_PARTNER_PLACEMENT3.md),[math](behavior/FIELD_PARTNER_PLACEMENT3_MATH.md),[NPC scan](behavior/FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md):complete local nine-angle/raw/classification/fallback/output and heading/normal/NPC graph answered;full backend/NPC/source/numeric/EVT-01 and original trajectories remain open |
| Complete battle unit entry,body/height/belong and geometry initialization | DMG-01, DMG-02 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Full entry](behavior/BATTLE_UNIT_ENTRY_COMPLETE.md),[registry/scalar/geometry](behavior/BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md),[pose/display](behavior/BATTLE_UNIT_POSE_AND_DISPLAY_INITIALIZATION.md):complete local Entry and listed helpers,early registry/check asymmetry,pouch/event order and borrowed field144 producer;resource/kind/content/lifetime/turn/source/runtime engines remain open |
| Partner support appearance and out-of-screen reset | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Support appearance](behavior/FIELD_PARTNER_SUPPORT_APPEARANCE.md),[vector ray](behavior/FIELD_PARTNER_VECTOR_RAY_WRAPPER.md),[screen recovery](behavior/FIELD_PARTNER_SCREEN_RECOVERY.md):complete local probe/return/caller,projection/control/request/root graph answered;full placement3/projection/backend/motion/source/domain/EVT-01 and original trajectories remain open |
| Event GP10 readiness/freeze gate producer and disc thread | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Producer](behavior/EVENT_DISC_THREAD_READINESS_GATE.md),[message presentation](behavior/DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md),[audio state](behavior/AUDIO_DISC_COVER_STATE_REQUESTS.md):complete disc thread/card/audio/font-routing local bodies,13-status table/scoped startup/four source resources;DVD/OS/render/source/domain/original traces remain open;make test38 suites/63 Python and make all pass |
| Partner floor query and slope/support publication | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Floor query](behavior/FIELD_PARTNER_FLOOR_QUERY.md),[slope pipeline](behavior/FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md),[heading](behavior/FIELD_PARTNER_FLOOR_HEADING.md):complete local five-probe/support/cache and numeric/heading graph answered;full backend/filter/math/source/domain/EVT-01 and original trajectories remain open |
| Partner ceiling query and ray-request wrapper | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Ceiling query](behavior/FIELD_PARTNER_CEILING_QUERY.md),[scalar wrapper](behavior/FIELD_SCALAR_RAY_WRAPPER.md):complete local four-probe/output and hitCheckFilter ABI graph answered;initial wrapper name corrected;full backend/terrain/math/domain/EVT-01 and original trajectories remain open |
| Map/SDK buffer release and pool initialization provenance | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Map free/battle pool](behavior/MAP_BUFFER_FREE_AND_BATTLE_POOL.md),[five pools/reset](behavior/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md),[SDK free/census](behavior/SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md):complete local release/coalescing/bootstrap/bulk reset and50 direct DOL sites;event user-pointer/scene/REL/platform/source/runtime ownership remains open |
| Partner stay floor/fly post-collision services | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Fly height](behavior/FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md),[ground/motion/support](behavior/FIELD_PARTNER_GROUND_POSTPASS.md):complete local tail and motion/classifier graph answered;selected floor early-return/output evidence;full head/floor/appear/motion/terrain/domain/EVT-01 and original trajectories remain open |
| Event timestamp producer and main-loop pause/update binding | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Retrace clock](behavior/EVENT_RETRACE_TIME_PUBLICATION.md),[outer/update loop](behavior/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md):complete clock publication prefix/window/raw readers and main/StMain/idle body,scoped refresh/registration;other callback tail/input/scene/platform/domain and measured cadence/original traces remain open |
| Partner surrounding-wall correction and called search body | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Binding/core/numerics](behavior/FIELD_PARTNER_SURROUNDING_WALL.md):mode1/width override,global view/filter ownership and local two-pass/vector graph answered;corrected dot sign and pre-cap saved vector;full ray/filter/terrain/math/domain/EVT-01 and original traces remain open |
| Event context initialization,reinitialization and capacity provenance | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Context init/reset](behavior/EVENT_CONTEXT_INITIALIZATION_AND_RESET.md),[SDK heap/callers](behavior/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md):complete Init/ReInit/allocator local policy,two original256-slot arrays,bank/global preservation and four direct DOL bindings;other heap/platform/time/source/scene/domain/original traces remain open;make test32 suites/58 Python tests and make all pass |
| Event dispatch tail,debug formatter and complete opcode inventory | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Tail/debug](behavior/EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md),[all119 coverage](behavior/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md):complete113..118/prefix11/no-ops,formatter/slot local bodies and full table;local opcode coverage closed,source/domain/platform/original traces remain open;native fixture/terrain gates resolved,full make test now passes |
| Partner front-wall query and local position correction | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Query](behavior/FIELD_PARTNER_FRONT_WALL_QUERY.md),[filters/status](behavior/FIELD_PARTNER_FRONT_WALL_FILTERS.md):complete local ray/quantization/projection/return and mask/name/status/height answered;corrected point+24/normal+30 ABI;full ray backend/surrounding/mesh/domain and original traces remain open |
| Event data readers,bitwise,time and memory opcode bodies | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Readers](behavior/EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md),[bitwise/time/memory](behavior/EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md),[buffers/allocator](behavior/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md):complete65..90 and local allocator/fill/cache bodies;indexed signed-word float differs from sequential IEEE load,query/write/ownership order explicit;other VM/domain/source/runtime requests remain open |
| Partner target-distance and footmark clearing services | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Clear/ID](behavior/FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md),[live targets](behavior/FIELD_PARTNER_LIVE_TARGET_DISTANCES.md),[SDK distance](behavior/FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md):complete local selection/reset/ID and paired norm answered;source/history consumer/terrain/numeric environment/domain and original traces remain open |
| Complete event IF predicates and ELSE/END lexical continuation | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Integer/mask/scans](behavior/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md),[float/string](behavior/EVENT_IF_FLOAT_AND_STRING_PREDICATES.md):complete12..33,twenty helpers and strcmp,operand order,raw masks,NaN CR tests and consumed-marker scans;data/other opcode domains and original traces remain open |
| Partner direction/speed composition and overlap correction | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Correction](behavior/FIELD_PARTNER_OVERLAP_CORRECTION.md),[query](behavior/FIELD_PARTNER_DIRECTION_AND_SPEED.md),[norm](behavior/FIELD_PARTNER_SPEED_NUMERICS.md):complete local box/counter/reset/state/output/numeric requests answered;called angle/trig/status/terrain services,numeric/domain,writers and original traces remain open |
| Event SWITCH and complete CASE group state machine | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Switch/scans](behavior/EVENT_SWITCH_STACK_AND_CASE_SCANS.md),[group/mask/range](behavior/EVENT_GROUPED_MASK_AND_RANGE_CASES.md):complete34..49 and eleven CASE bodies,actual table order,signed stack/state/raw-query/order and scanner continuations;IF/data/other opcode domains and original traces remain open |
| Field partner walkMain executor and local terrain requests | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [walkMain dispatch](behavior/FIELD_PARTNER_WALK_DISPATCH.md),[stay local graph](behavior/FIELD_PARTNER_STAY_MOTION.md):complete selectors/threshold/mode/distance/collision/pose request order answered;called homing/movement/terrain/pose engines,domain/writers,EVT-01 and original traces remain open |
| Event GOTO/loop and assignment/arithmetic opcode bodies | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [GOTO/loops](behavior/EVENT_GOTO_AND_LOOP_OPCODES.md),[assignment/arithmetic/cursor](behavior/EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md):complete4..8/50..64 and NEXT,raw/resolved/RHS order,count and MOD coercion;other predicates/readers,numeric/domain and original traces remain open |
| Ordinary field partner movement callbacks modes0/1/2 | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Walk callback](behavior/FIELD_PARTNER_WALK_CALLBACK.md),[history writer/readers](behavior/FIELD_PARTNER_HISTORY_WRITER.md),[mode1 substates](behavior/FIELD_PARTNER_MODE1_SUBSTATES.md):complete local callbacks/history and bindings answered;final walkMain/terrain/collision/pose/source/index/writer engines and original traces remain open |
| Full event integer/float operand namespaces and storage routing | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Integer APIs](behavior/EVENT_INTEGER_OPERAND_NAMESPACES.md),[float APIs](behavior/EVENT_FLOAT_OPERAND_ROUTING.md),[saved leaves](behavior/EVENT_SAVED_SWITCH_STORAGE.md):complete signed routing/coercion/index/read-write-return and storage leaves;capacity/numeric/source/save/runtime remain open |
| Field driveParty dispatch and local update order | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Driver dispatch](behavior/FIELD_PARTY_DRIVER_DISPATCH.md),[global countdown/action](behavior/FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md):complete local15-mode/pending/destruction/tail and countdown/action requests answered;called mode/collision/animation engines,writers/callers,EVT-01 and original traces remain open |
| Complete event deletion and stop/start relationship traversal | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Deletion/copy-back](behavior/EVENT_DELETE_AND_COPYBACK.md),[stop/start](behavior/EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md):complete local recursive/inlined context/scan/type/write policy;other opcodes/namespace/source/domain and original traces remain open |
| Field party mode initialization and current-index ownership | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Mode initialization](behavior/FIELD_PARTY_MODE_INITIALIZATION.md),[driver/index](behavior/FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md):complete local transition/init/eligibility and scoped direct-index census answered;all mode update/services/callers,alias/REL/index/slot writers,battle teardown and original traces remain open |
| Event run/ID-output concurrency and control opcode wrappers | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Run/ID variants](behavior/EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md),[controls/ID scans](behavior/EVENT_CONTROL_OPCODES_AND_ID_SCANS.md):complete92/93/108/111,94..106 wrappers,setters and Other/ID/Check scans;deep stop/start/delete,other opcodes/domain and original traces remain open |
| Child/brother constructors and restart complete local state | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Descendants/107/110](behavior/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),[restart](behavior/EVENT_RESTART_PRESERVATION.md):complete local copy/reset/ownership/append/order and plain inline/brother paths;ID variants,remaining controls/domain and original traces remain open |
| Field party destruction and position constructors | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Destruction/saved kind](behavior/FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md),[position constructors](behavior/FIELD_PARTY_POSITION_CONSTRUCTORS.md):complete local Kill/Kill2/main/position/secondary and kind1/8 init answered;all-kind/pose/heap/resource/mode/index invariants,battle teardown and original traces remain open |
| Root/typed event constructors and label/run-check initialization | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Root/typed constructors](behavior/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md),[labels/run-check](behavior/EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md):complete local allocation/init/label scan/conditional append/system-level stop;child/brother/inline/restart/domain and original traces remain open |
| Lecture field intersection and party callback bodies | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Intersection/coordinates](behavior/FIELD_SEGMENT_INTERSECTION.md),[party callbacks](behavior/FIELD_PARTY_EVENT_CALLBACKS.md):local geometry/query/encoding,raw slot dispatch and first-call constructor route answered;full party engine,variable namespaces,EVT-01 scheduling,battle teardown and original traces remain open |
| Event manager complete loop,credit/lifetime and wait continuations | EVT-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Complete manager](behavior/EVENT_MANAGER_COMPLETE_LOOP.md),[dispatch/waits](behavior/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md):complete local time/eligibility/ID/credit/result loop,priority and scoped fetch/wait/USER_FUNC;constructors/append/initial state and original traces remain open |
| Ordinary attack badge-effect tail and called recovery services | JUMP-01, DMG-02, STATUS-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [32-command tail](behavior/BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md),[recovery/geometry](behavior/BATTLE_BADGE_RECOVERY_SERVICES.md),[coercion/effect route](behavior/BATTLE_BADGE_RECOVERY_PRESENTATION.md):complete local graph/wrappers/cores and scoped presentation binding;renderer/VM/domain/source/runtime remain open |
| Lecture field trigger,story conditions and return requests | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Field entry/return](behavior/LECTURE_FIELD_ENTRY_AND_RETURN.md):wall/reload story10 callers,crossing operands,choices,setup3 and common story11 exit answered;intersection/party/persistent services,battle-end controller and original traces remain open |
| Weapon aftereffect callback and ordinary Jump operands | JUMP-01, DMG-02 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Dispatch](behavior/BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md),[stage leaves](behavior/BATTLE_STAGE_AFTEREFFECT_REQUESTS.md):complete local weighted/forced RNG and destruction/nozzle/frame/object ownership;queued event bodies,source/domain and original traces remain open |
| Stylish start/main/result and ordinary Jump timing bindings | JUMP-01, AC-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Timing/results](behavior/BATTLE_STYLISH_TIMING_AND_RESULTS.md),[icon](behavior/BATTLE_STYLISH_TIMING_ICON.md):complete local start/poll/input/output/queue/draw and four ordinary callers;source/scheduler/audience/display/assets/original traces remain open |
| Enemy candidate choice and sampling cores | JUMP-01, DMG-02 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Sampling](behavior/BATTLE_ENEMY_CANDIDATE_SAMPLING.md),[choice/RNG](behavior/BATTLE_ENEMY_CANDIDATE_CHOICE.md):complete local admission/geometry/compaction/sort/weights/mode4/draw semantics;source/helper/domain/seed/interleaving and original traces remain open |
| Ordinary selected target,weapon-address and weapon-cost services | JUMP-01, DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Selection/weapon](behavior/BATTLE_SELECTED_TARGET_AND_WEAPON.md),[cost/payment](behavior/BATTLE_WEAPON_COST_AND_PAYMENT.md):complete local bodies/cursor dispatch/shared FP/AP/action-record arithmetic;menu/selection producers,numeric extremes and original traces remain open |
| Lecture player attack bindings,mode/feedback transitions and end requests | AC-01, AC-02, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Player flow](behavior/LECTURE_PLAYER_ATTACK_FLOW.md) and [window arbitration](behavior/BATTLE_STATUS_WINDOW_ARBITRATION.md);default weapon masks,1→2/5→6 and feedback writers/end requests answered,full menu/end/field/runtime traces remain open |
| Unit/part hit/world/home positions and movement sound core | JUMP-01, DMG-02 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Position queries](behavior/BATTLE_UNIT_PART_POSITION_QUERIES.md),[movement sound](behavior/BATTLE_MOVEMENT_SOUND_CONTROL.md);source writers,playback internals and original traces remain open |
| Jump takeoff position,move-frame query and approach MovePosition | JUMP-01, AC-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Takeoff](behavior/BATTLE_JUMP_TAKEOFF_POSITION.md),[query/movement](behavior/BATTLE_MOVE_FRAME_AND_POSITION.md):complete local bodies;candidate/numeric/helper/domain and execution remain open |
| Unit sound-mode loader and Mario jump sound record | JUMP-01, DMG-02 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Sound-mode contract](behavior/BATTLE_UNIT_SOUND_MODE.md):complete partial-field loader and used type2/-1 request;source writers/macro resources/audio execution remain open |
| MarioJumpPosition complete local motion modes and AC side effects | JUMP-01, AC-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Motion](behavior/BATTLE_MARIO_JUMP_MOTION.md) and [numerics](behavior/BATTLE_MARIO_JUMP_NUMERICS.md):complete local body,all phases/rounded branches;numeric/audio dependencies and original traces remain open |
| Crump NPC helper identity versus lecture helper modes and hooks | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Helper roles](behavior/CHRISTINE_HELPER_ROLES.md) and [coordinator modes](behavior/LECTURE_COORDINATOR_MODES.md);distinct setup/kind/ID/work/message graph answered,player/counter/field lifecycle and original traces remain open |
| Mario-specific JumpParam preparation and motion dependencies | JUMP-01, AC-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Parameter contract](behavior/BATTLE_MARIO_JUMP_PARAMETER.md):complete ordered integer resolution/distance-count writeback;Mario motion now linked separately,numeric/domain/execution traces remain open |
| Battle input sampler/reset caller census and system-level/event-speed controls | AC-01, AC-03, AC-02 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Reset owners](behavior/BATTLE_INPUT_RESET_OWNERS.md) and [level/speed controls](behavior/EVENT_SYSTEM_LEVEL_AND_SPEED.md);direct census,local controls and tutorial injection answered,full UI/type/writer/lecture graph and original traces remain open |
| Ordinary Jump complete request graph and per-hit/AC branches | JUMP-01, AC-01, DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Jump graph](behavior/BATTLE_NORMAL_JUMP_EVENT_FLOW.md),[result/stop](behavior/BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md),[VM/concurrency](behavior/EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md);local motion/preparation now linked separately,candidate/audio/cost/stylish/source composition and original traces remain open |
| Unit resource/status snapshot export leaf | STATUS-01, DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Export contract](behavior/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md):complete matching writer;export caller cadence and on-disk save semantics remain open |
| Unit badge/count/bonus import through _EquipItem | STATUS-01, DMG-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Badge import](behavior/UNIT_BADGE_COUNT_IMPORT.md):complete byte maps/flags/reset/scan and scoped setup bindings;inventory equip UI and runtime traces remain open |
| Unit-entry status clear and pouch resource import | STATUS-01, DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Initialization contract](behavior/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md):complete clear/import/revision,party mapping and partial info-record reset;badge aggregation,snapshot producers and turn/save ownership remain open |
| Physical key producer and battle pad update order | AC-01, AC-03, AC-02 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Physical producer](behavior/PHYSICAL_INPUT_PRODUCER.md) and [cadence/snapshots](behavior/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md);local edges/repeats/order answered,SDK/pause/other writers/original traces remain open |
| Unit part defense/attribute construction and Crump source tables | DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Part contract](behavior/UNIT_PART_DEFENSE_AND_LIFETIME.md):pointer copies,boss/Mario zero tables,lookup exceptions and ordered teardown;full unit entry,mutation and executed damage remain open |
| Battle jump callbacks and concrete Crump float operands | AC-01, AC-03 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Jump contract](behavior/BATTLE_JUMP_EVENT_CALLBACKS.md):full local callbacks,used float route and scoped JMP-01..07 acceptance cases;main/pad cadence and original impact traces remain open |
| Crump normal attack request graph and target wrapper ABI | AC-01, AC-03, DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 1 | [Attack graph](behavior/HARBOR_BOSS_NORMAL_ATTACK.md) and [target ABI](behavior/BATTLE_TARGET_EVENT_INTERFACES.md); excludes target cores and executed timing; parent requests remain open |
| Default weapon power callback and concrete boss operands | DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Power contract](behavior/BATTLE_WEAPON_POWER_CALLBACKS.md):full default body,boss base1 and nil FP pointer;final modified damage remains open |
| Ordinary Mario Jump/Hammer power callback bodies and default tables | DMG-01 | VALIDATED / lock released, 2026-10-12 | Documentation agent ID 2 | [Power contract](behavior/BATTLE_WEAPON_POWER_CALLBACKS.md):signed level,index/read order,two ordinary tables;hit-count/level/bonus/AC producers remain open |

ID1 validation follow-up: ID2 renamed the Mario part-record local to avoid
shadowing the DOL base. The general verifier now passes after that correction;
ID1's separate jump verifier also passes. The earlier tooling failure did not
contradict original-game evidence.

## Implementation priorities

Use the scoped lock table above when claiming work. A portable helper alone
does not answer its full runtime binding request.

| Priority | Request / implementation blocker | State / owner / reserved slice |
| --- | --- | --- |
| 0 | EVT-01: complete native event scheduling and continuation | Open; needed to replace provisional Jump timers with the recovered graph; request below specifies remaining complete local contracts |
| 1 | DMG-01: real weapon power, part defenses and dispatch/application | Power bodies/operands and scoped part construction/table/lifetime delivered by ID2;level/bonus/AC producers,mutation and application binding remain open |
| 2 | AC-03: enemy hit branch and actual defense sampling boundary | Attack graph/target ABI delivered and lock released; pad cadence,global eligibility and execution remain open |
| 3 | AC-01: tutorial/ordinary Jump state ownership and impact cadence | Shared enemy-attack graph delivered; next recover pad/update and movement callback composition |
| 4 | AC-02: Hammer lifecycle, result consumers and original audio binding | Open; ordinary weapon period/base difficulty already answered |
| 5 | STATUS-01: initialization, infliction and turn lifetime | Open; portable storage alone does not establish turn cadence |

The ordering serves the early original encounter's implementation. It does not
remove the full game's field, partner, story, inventory, save, audio, rendering
and optional-content obligations. Do not insert provisional native timing into
an original contract. Complete-game requirements remain deferred until system
coverage is adequate.

## AC-01 — Opening Crump encounter and Jump callers (open)

ID1 helper-role answer (2026-10-12):
[helper roles](behavior/CHRISTINE_HELPER_ROLES.md) verifies that Crump setup2
spawns unit_npc_christine0x66d78 through setup0x647d8;its init/attack hooks do
not select the separate lecture injection/mode routines. Lecture setup3 uses
kind0x66fe8,spawns professor0x67fb8,and binds selector-6 via W+0x424 to that
professor. Keep those actor/state roles separate even when assets are reused.
[coordinator modes](behavior/LECTURE_COORDINATOR_MODES.md) supplies its full local
phase/init request graph,phase-controlled work3/7 preparation and20 exact US
message/choice/append sites with speaker bindings in msg/US/gor_02.txt. Mode1→2/
5→6 player producers,counter dispatch,field selection/lifecycle and original
traces remain open;the first Crump encounter must not inherit lecture-only Mario
HP-skip or injected A/B inputs from this different setup.

Partial answer in documentation commit81a2c2b:
[harbor opening bindings](behavior/STORY_HARBOR_OPENING_BINDINGS.md) pins
gor.rel entry/story resume values2..6,seven event spans,actor keys/placement,
20 message call sites and the beforebattle NPC literal2/start/wait boundary.
It explicitly leaves battle configuration,complete scene conditions and service/
cadence/persistence semantics open. Native harness dialogue remains development
notices;the inventory must not be substituted for a complete story event graph.

Further partial evidence has arrived in
[field battle entry/return](behavior/FIELD_BATTLE_ENTRY_AND_RETURN.md) and
[harbor configuration](behavior/HARBOR_TUTORIAL_CONFIGURATION.md): literal2
selects gor setup record0x60eb8,one weighted group/unit and unit_boss_kanbu1.
The subsequent [entry/stats contract](behavior/UNIT_ENTRY_STATS_AND_SPAWN.md)
connects kind HP5/FP0 to runtime stores;later overrides remain open. The local field-return wait accepts sequence2 plus
any nonzero result;actual victory/loss/escape producers remain open. Preserve
these boundaries when answering remaining tutorial stats/event requests.

Requested by the implementation agent. Implementation is now in
`include/battle_timing_a.hpp`, `source/battle_timing_a.cpp` and the Jump branch
of `Game::battle` in `source/game.cpp`. The evaluator follows
`behavior/BATTLE_TIMING_A_COMMAND.md`. The caller currently uses development
impact tick48, difficulty3, ordinary route, and a fresh command/count per attack;
these choices are not established original Crump behavior.

Please recover:

- Opening/Crump REL entry, unit/weapon/event selection and tutorial exceptions;
  whether and when Jump ID1 is actually selected and defense becomes available.
- Jump SetParam/flag/difficulty/count/state writers, setup/start/result/stop call
  order, conditional paths, repeat result calls and finish-state ownership.
- General battle pad update order/cadence versus private AC pad, event polling,
  pause, animation/impact and ResultAC. Identify the exact input sample included
  at impact. An isolated original input/state trace is still needed to validate
  actual timing; the native caller currently samples before impact evaluation.
- Raw automatic/token/battle-force fields' writers and active tutorial values,
  seed ownership and conditional RNG consumption at these call sites.
- Original US message IDs, speakers, control tokens, order/branch conditions and
  persistent event writes for the opening encounter. No invented scene/dialogue.

Provide US revision0 addresses/records and full branch evidence, then observations
separately. Existing leads: `behavior/BATTLE_ORDINARY_ATTACK_BINDINGS.md`,
`behavior/BATTLE_ACTION_COMMAND_CALLBACKS.md`,
`behavior/BATTLE_INPUT_AND_DEFENCE.md`, `behavior/RANDOMNESS.md`.

## AC-02 — Ordinary Hammer binding and presentation (open)

ID1 lecture-only answer (2026-10-12):
[lecture player flow](behavior/LECTURE_PLAYER_ATTACK_FLOW.md) pins ID3 params
4/25 and mode5 flag2,hold/rehold/release teaching loops,STOP/START of setup-event
ID versus parameter2 suppression,output2→professor work2 and cleanup5→6. These
are a different script/weapon binding from ordinary Hammer;do not assign period40
merely because the lecture pointer differs from default. Jump lecture ID1 and
cleanup1→2/default menu masks are documented alongside it. Its coordinator sets
01000000,a proven TimingA forced-success writer,without establishing that field
in Crump's opening encounter. [Status arbitration](behavior/BATTLE_STATUS_WINDOW_ARBITRATION.md)
closes2740 suppression and event-on precedence;it is separate from attack
selection. Full field/menu/end/services and original execution remain open.

Partial answer arrived in concurrent documentation commit07bb0f7:
[weapon branches](behavior/BATTLE_HAMMER_WEAPON_BRANCHES.md) establishes default
Hammer pointer0x80353de8 -> T25/base difficulty3, the two named pointer cases
-> T30/base3, and the core's other-weapon branch -> T40. Integer branch support
is in [VM branch rules](behavior/EVENT_INTEGER_BRANCHES.md). This resolves the
ordinary pointer/period/base-difficulty part of this request. Remaining caller,
tutorial, display/sound and execution questions below remain open.

The recovered ID3 local behavior is implemented in `source/battle_hammer.cpp`,
with derived contract tests in `tests/battle_hammer_tests.cpp`. The native harness
now uses held-left input and the ordinary default weapon's N4/T25/difficulty3.
The former B-hold harness was removed. This is an ordinary command adaptation
inside a development encounter, not a verified Crump tutorial binding.

Please recover suppression/automatic/forced writers, result/damage consumers and
start/finish/stop ownership, with Crump/tutorial binding separate. Also recover
display/gauge semantics and the two sound-key macro bindings before original
charge audio is connected. Original held-left is general pad mask0x40000;
The native Circle Pad's normalized70-unit adaptation uses X<=-30/70 inclusively.
Native charge UI shows actual stages as a development assist; it does not claim
to reproduce the original command's display geometry/animation. Sound requests
retain `SFX_AC_PI1` and `SFX_AC_PONE1`; no replacement samples are played.

## AC-03 — Defense callers, pad cadence and eligibility (open)

Further partial answer from ID1 (2026-10-12):
[reset ownership](behavior/BATTLE_INPUT_RESET_OWNERS.md) finds two direct DOL
samplers,four WorkInit callers and one gor tutorial-helper import. The helper
resets private history then injects raw0x100/0x200 at index0 when the selected
lecture script's work value is3/7,before defense/damage requests. Its preliminary
result4 does not bypass the checker,which clears that result. Full lecture
selection,selector-6/work producers,weapon eligibility and original traces remain
open;do not replace Crump's ordinary attack input with demonstration injection.
[system controls](behavior/EVENT_SYSTEM_LEVEL_AND_SPEED.md) closes the local
level0..4 event-mask table,getter returning the whole GP mask,and speed setter/
creation regions. Nonzero lower levels do not automatically resume older stops;
mask0xef excludes0x10. Caller UI/type/speed ownership remains open. These answers
supply implementable reset/control semantics without a fabricated whole-game
pause flag or fixed impact schedule.

Input partial answer from agent ID1 (2026-10-12):
[physical producer](behavior/PHYSICAL_INPUT_PRODUCER.md) closes local DemoPad
read/copy,error-3 retention,GP physical edges,frame-rate-scaled24/6 repeat counters,
strict48 direction masks and US input-clear chord. Hardware SDK/polling remains
open. [battle cadence/snapshots](behavior/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md)
closes general→sequence→private→AC main ordering before later main-chain events,
private sampling even with no AC main,and the12-byte snapshot write map. The
current GP press can be included before a later eligible defense event;multiple
interpreter credits can share that sample. Every-hit private reset remains
separate from general history. PAD-01..06 give implementable storage/order cases.
Main frequency/pause,other samplers/resets,override writers,multihit event bindings
and original execution traces remain open;no fixed impact tick is established.

Movement partial answer from agent ID1 (2026-10-12):
[jump callbacks](behavior/BATTLE_JUMP_EVENT_CALLBACKS.md) gives full local
JumpSetting/JumpPosition/JumpContinue contracts and the concrete float route.
JumpPosition integrates in its first call,blocks on remaining positive count,
and yields on landing;precheck is reached in a subsequent interpreter invocation.
The initial automatic count0 early return is a separate immediate-advance route.
Concrete initial settings are count0/speed2/acceleration0.19921875;recovery counts
20/10/10 include a requested10 overriding stored15. JumpContinue retains prior
velocity and blocks at exact floor equality. No rendering-frame or fixed-impact
schedule is inferred. Main/private/general pad order,pause,execution credits and
original input traces remain open;do not treat the local callback as closing them.

Further partial answer (2026-10-12):
[boss normal attack](behavior/HARBOR_BOSS_NORMAL_ATTACK.md) documents all95
commands. Precheck result1 alone requests defense then CheckDamage flags0x100;
results2/3/4/6 and default skip defense and converge on common recovery. The
parent reaches precheck after the JumpPosition callback,with an earlier concurrent
pose event;no fixed impact tick is established. Preserve avoidance codes38/39/40.
[target interfaces](behavior/BATTLE_TARGET_EVENT_INTERFACES.md) documents the
seven local target helpers/wrappers,including mode pair selection and signed
sampling bias;candidate selection cores and initial flag/mode writers remain
open. No-target GOTO99 has no local label99 and must not be fabricated as a
successful return. This answers the local attack branch/ABI slice only;request
stays open for sampling cadence,global eligibility and executed traces.

Partial answer now in [boss tutorial events](behavior/HARBOR_BOSS_TUTORIAL_EVENTS.md):
normal attack's ResultACDefence call is at gor.rel0x656bc,CheckDamage at0x656cc
uses flags0x100,and weapon_kanbu1_attack0x64718 has byte+0x13=1. This resolves
the local enemy weapon byte gate,not battle flag0x1000/global tutorial input/
status/guard-possibility gates or the impact schedule. Full dispatch integration
is still required before changing the harness's B/superguard eligibility.

The local [defense and pad contract](behavior/BATTLE_INPUT_AND_DEFENCE.md) is
implemented in `source/battle_defense.cpp`, tested in
`tests/battle_defense_tests.cpp` and connected at the harness enemy impact in
`Game::battle`. General and private pads are separate;both currently receive
one adapted native sample per simulation tick before callbacks. Private pad
WorkInit preserves the release array and clears all other modeled state after
every defense check. Native guard result is evaluated at impact,not first press.

Please recover the original global physical edge/repeat producer,complete
12-byte snapshot field bindings,main/battle/AC pad update order and pause cadence,
private/general channel routing and defense call sites for Crump and ordinary
enemy attacks. Identify the argument+0x13 weapon gate,unit modifier/automatic
and override-counter writers,guard-possibility/status gates and damage dispatch
bindings needed at each hit. Include multihit/newly sampled-input rules and an
original single/mash/A+B/held/early/late trace. The harness uses gate0,no badges/
automatic/override and provisional enemy impact54/one-damage attack;these must
not become original tutorial evidence. Result5 routes are implemented locally,
but the harness does not infer superguard eligibility or reflection effects.
Both Hammer and defense use original BattleWork+0x1d18. Their portable local
work currently owns separate test counters;native harness counters stay zero.
Recover caller ownership so future forced commands bind one shared battle
counter and cannot accidentally duplicate or reset its consumption.

## DMG-01 — Weapon power, unit stats and damage dispatch binding (open)

Further partial answer from ID2:
[part defense and lifetime](behavior/UNIT_PART_DEFENSE_AND_LIFETIME.md) pins
unsigned count*0x500 block allocation,0x4c source stride,linked nodes,attribute/
counter copies and retained defense pointers. Crump count1/part1 binds two
five-byte zero tables;Mario's three annotated records share two zero tables,
but its actual count/caller needs binding. Single-part lookup accepts any
nonnegative ID;multiple-part lookup matches source ID. Unit deletion releases
poses,block and unit before clearing the registry;original source tables are
not freed here. PART-01..04 specify local acceptance cases. No allocation-null/
zero-count guard or atomic unregister-first behavior is invented. Remaining
unit entry,mutation/table lifetime,heap/pose services and original traces keep
DMG-01 open.

Partial answer from documentation agent ID2:
[weapon power callbacks](behavior/BATTLE_WEAPON_POWER_CALLBACKS.md) recovers
the complete default and Mario Jump/Hammer bodies,shared pouch pointer and
ordinary weapon callback/table/event bindings. Boss weapon+20 is signed word1,
with nil FP callback. Mario captures battle1cb8 mask0x2,reads signed pouch level98/
99,upper-clamps index2*(level-1)+bit to5 and adds unsigned unit bonus2ff/300.
There is no lower index clamp. Ordinary Jump pairs1/1,2/2,3/3 do not establish
total multihit damage;Hammer pairs1/2,2/4,3/6 are local powers before modifiers.
Preserve actual weapon data and ordered reads;do not assume tutorial level1,
bonus0 or AC state without their initialization/caller evidence. PWR-01..04
provide implementable local requirements and derived acceptance cases. Unit/part
defense creation,state producers,weapon lifetime and original traces remain open.

The [HP/FP calculators and revival marker](behavior/BATTLE_DAMAGE_CALCULATION.md)
are implemented in `source/battle_damage.cpp`,tested by
`tests/battle_damage_tests.cpp`. Native attacks now call the HP calculator and
retain the original-form result code. Their callback/stat fixture still supplies
development powers1/2,zero defense/no statuses and provisional actor resources.
The local calculator is separate from resource application/death/revival events.
Resource application is now implemented in `source/battle_damage_application.cpp`
and used by native attacks. Its service interface preserves ordered effect/sound/
audience/hit-event requests;the harness records the latest hit's requests without
executing original presentation or hit scripts. This trace is not an active VM
queue. Native resource caps/parts/positions/kind sounds remain fixture bindings.

Please recover the original weaponGetPowerDefault0x80189d30 body and its concrete
weapon+0x20/etc inputs,ordinary Mario Jump/Hammer callbacks and their arguments,
kanbu1 part defense/attribute tables,kind/stat allocation-copy/init consumers,
max/current HP/FP,actor/body-part and attack sequence reset/increment ownership.
Retain callback side effects/read order and signed field widths. Tutorial weapon
gate1 and enemy CheckDamage flags0x100 are now documented;global flag0x1000,
guard/status/counter/precheck/damage/application/reward/defeat consumers still
need a complete graph,including original multi-hit timing and input/RNG traces.

Status Check/Get now reads the implemented shared unit storage via
`storedDamageStatusServices()`. Battle units retain storage between hits;initial
status/infliction/turn/presentation binding remains unfinished. Pouch item-count
is still an explicit service interface. Revival marking
only tests original item151 in the correct owner path;consumption/healing/hit
event scripts remain unbound and must not be replaced by an invented revival.
Please link recovered callback/unit/application contracts here as they arrive.

Partial stat answer arrived in
[entry/stats/spawn](behavior/UNIT_ENTRY_STATS_AND_SPAWN.md):entry publishes the
allocated unit before init,then copies signed kind HP/FP into three independent
fields each. The checked boss receives HP5/FP0 at those stores;later overrides
and the rest of unit entry remain open. Do not treat that as a measured starting
stat trace or collapse current/max/base copies when implementing resources.

## STATUS-01 — Status lifetime, event binding and turn ownership (open)

Native implementation breadcrumb (2026-10-12): commit7e43c5a implements
UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md in `source/battle_unit_resources.cpp`,
with derived cases in `tests/battle_unit_resources_tests.cpp`. Native battle
start now invokes the recovered status/info clear. Pouch resource callbacks and
seven partner snapshots have explicit semantic service boundaries; they are not
yet native save/load, party-switch or turn owners. Existing requests for exact
export/import caller cadence, snapshot initialization and resource/equipment
ownership remain open. Preserve direct negative/shared status bytes on import;
only nonpositive partner HP overrides status27 duration. Do not infer original
disk-save layout or status lifetime from these portable helpers.

Further ID2 answer:[badge/count import](behavior/UNIT_BADGE_COUNT_IMPORT.md)
recovers full _EquipItem/BtlUnit_EquipItem,40-byte clear,all200 signed pouch
slots,exact numeric count maps/mode flags and wrapped multi-item threshold.
Setup uses clear+Mario scan flags3 and clear+partner scan5 before a later
pouch import;enemy saved/nonsaved held-item branches differ. Concrete Mario
setup/kind now proves count3 and original part array;items289/290 write the
power bonus bytes2ff/300. These are also partial DMG-01/AC-03 badge writer
answers,not initial player equipment or global guard eligibility proof.
[Snapshot export](behavior/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md) identifies
the matching SetParamToPouch body and exact current-resource/status write order;
swap/turn/battle-end export cadence and disk-save reconstruction remain open.
Use EQUIP-01..04 and INIT requirements rather than invented loadout/turn rules.

Partial answer from ID2:
[status clear and pouch import](behavior/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md)
recovers complete ClearStatus,SetParamFromPouch,ReviseHpFp,party mapper and
status-info initializer with original entry callers. Clear zeros30 status bytes
then clears Charge through its setter;the six info records retain each word+8.
Mario imports current pouch HP/FP and revises separate base/max copies using
unsigned count bytes2f1/2f2. Partners use the nonsequential party mapping,revise
HP only,restore30 status bytes and related fields from battle snapshot162a0+
44*(kind-224),then set ID27 duration1 for HP<=0. Imported statuses bypass setters;
the prior entry clear is not proof of a clean partner state. Other-kind import
does no resource writes,but direct revision still updates HP/FP maxima. INIT-01..04
provide scoped requirements/cases. Badge aggregation,move init,snapshot writers,
infliction/periodic/turn/presentation/save lifetime and original traces remain
open. This is also a partial DMG-01 answer for later HP/FP override ownership.

Local [storage/recovery](behavior/BATTLE_STATUS_STORAGE.md),[move-count helpers](behavior/BATTLE_MOVE_COUNTS.md)
and guard/action predicates are implemented in `source/battle_status.cpp`,with
tests in `tests/battle_status_tests.cpp`. Damage integration uses the actual
Check/Get storage contract rather than an always-absent reader. These services
preserve byte narrowing,sign pairs,cancellation,Charge wrap,Fast/Slow reset and
explicit recovery steps. The native harness does not fabricate status infliction
or a once-per-turn recovery schedule.

Please recover unit-entry status/badge/move-count initialization and concrete
weapon/unit/event bindings reaching the documented infliction,recovery/periodic
turn controllers. Pin full main/battle event cadence,turn/phase state producers,
remaining-count consumption,+20/+23/+24 writers,end-of-battle cleanup and status
effect/message/sound/pose callback resource bindings. Existing local recovery/
move/periodic/controller documents provide contracts but not the whole original
runtime graph. Include original multi-turn traces (Slow/Fast,negative durations,
Charge consumption,wakeup on lethal/live damage) and actor save/reload lifetime.
Do not use portable test fixtures or prototype turn timing as original evidence.

## DMG-02 — Direct application service internals and native bindings (open)

Partial ID2 answer:[unit/part position queries](behavior/BATTLE_UNIT_PART_POSITION_QUERIES.md)
close home,part offset/world and scaled hit leaves,BattleGetUnitPartsPtr and the
event HitPos wrapper. Absolute part flag10000000 differs from owner-position
plus offset;relative X uses owner189,while scaled hit X uses captured side-table
factor. Two offset triplets scale through unit114 with ordered fused operations;
event outputs truncate to signed integer with raw destination sentinel checks.
No source writer/actual original scene coordinate is inferred from the leaf.
[Movement sound core](behavior/BATTLE_MOVEMENT_SOUND_CONTROL.md) closes retained
voice/periodic alternation/stop and identifies only record18 as retained handle;
keys0/4 are playback inputs. Original kind sounds,source writers,field144 schema,
height/belong/action-record/audience/revival/VM/resource helpers and execution
traces remain open. POS/MOVE-SE requirements provide scoped implementation cases.

The local [direct application](behavior/BATTLE_DAMAGE_APPLICATION.md) is now
implemented and tested,including shared partner FP,Pity Flower/Sleep RNG order,
sound/effect request precedence,narrowing/suppression/accounting,lethal-code
marking and final hit-event request. The native adapter records latest-hit
requests and uses the resource mutations;original effect/audio/VM execution is
pending. No replacement effects/samples or revival scenes were invented.

Please recover complete BodyPartsId/GetPartsPtr,GetHitPos/GetPos/GetHeight/
GetBelong and field114/pointer144+10 schemas/lifetimes;concrete boss/Mario/partner
part attributes,kind9c/a0/a4/a8 sound pointers and original effect/macro resources.
Pin action-record AddCount/AddPoint saturation/storage and audience helper state/
RNG effects,hit-event dispatch inputs and all reachable death/revival/recovery
scripts,shared FP owner lookup/update reconstruction and pouch/item151 consumption.
Confirm exact numerical/rounding and position-query/service request order with
original traces,including suppression,zero/absorption,simultaneous HP/FP,Pity at
full FP,wakeup on lethal/Instant Kill and pattern25 gates. The current interface
treats belong as resolved semantic unit state;its original getter needs closure.
Use native data/requests as test fixtures only;do not treat them as original
scene/stat/position/audio/turn behavior. Link concrete data/service answers here.

## Implementation coverage and next work

### INPUT-01 — Remaining native adaptation and WorkInit byte clarification (open)

Implemented the physical producer and12-byte battle snapshots in
`source/physical_input.cpp`/`source/battle_defense.cpp`. Native battle now follows
general pads→sequence adapter→private pad/AC main→later event adapter. Four
general buffers and the independent private buffer retain ownership;physical
state persists across scene/battle initialization. Abstract host callers may
still inject already-produced logical GP masks rather than raw levels.

Please clarify the older WorkInit summary:original listing0x8011a3d0..3f4 clears
snapshot bytes0..a but has no byteb store. Implementation preserves each old
byteb on WorkInit and both trailing bytes on update,while clearing bytea on init.
Pin that exact map in the contract/verifier before describing full-snapshot
clearing. The native snapshot bytes start zero via construction/battle allocation.

Remaining:PADRead/Clamp/Reset service behavior and supported errors;GP frame-rate
parameter/writers,motor/control/chord ownership,pause/physical poll boundaries,
native C-stick/trigger mapping and sequence/display/event registration. Native
uses one input projection per simulation tick,rate60,zero unused channels and no
motor service;these are adaptation inputs,not observed GC polling or connection
counts. Start/impact schedules and the full event-credit loop remain unbound.
Do not invent extra samples for script invocations or suppress inactive private
sampling. Provide original identities/traces separately from native fixtures.

### EVT-01 — Native event scheduler and dispatch closure (open)

Memory-bootstrap breadcrumb (2026-10-12): `event_memory_bootstrap.cpp` ports
SDK Init/Create/DestroyHeap and five-pool memInit/memClear,with original primary
map construction. Tests preserve original configuration,shared percentage
remainder,two-pass handles,ignored replacements and null-result headers. make
test passes43 suites/69 Python tests;make all passes;ISO verifier passes20 bodies/
205 anchors. Native arena/storage/cache/scene/source/domain remains open.Next:
delivered release/battle subpool helpers,native providers and original streams.

Documentation agent:addendum requested for OSInitAlloc's live descriptor-count
reload80296670 and base reload80296654/67c while metadata size retains ORIGINAL
count*12. Read-only listings also pin primary-map start→end→handle loads BEFORE
length store (memClear800301a0..b8;memInit80030434..48),and SDKCreateHeap's live
size reread802966e8 after header writes. Native implementation/tests preserve
these instructions;please add this ordering to the existing contracts/verifier
acceptance text. Prior entry-layout and NormalJump registration requests persist.

SDK-heap breadcrumb and native-layout request (2026-10-12):
`event_heap_allocation.cpp` ports SDK first-fit/signed-size/split/absorb/allocated
list and __memAlloc handle/zero/cache wrapper separately from _mapAlloc. Tests
pin node read/write order,size reread,all unlink/split routes,failures,zero count
and both original context payload requests. Final make test passes42 suites/69
Python tests;make all passes. ISO context/heap verifier passes8 bodies/139 anchors.

Documentation agent:please consolidate the original1b0 EventEntry field map
(offset,width,signedness,pointer words and reserved/raw bytes) for all fields now
used by manager/constructors/lifecycle/opcodes. Include overlapping/local/stack/
message/USER_FUNC pointer alias requirements and any known read/write domains.
Existing contracts cover individual offsets;the native memory translator needs
one complete reviewed map before binding raw original address operands to these
semantic fields. Cite delivered coverage and clearly retain unknown bytes/source
owners. Prior NormalJump registration/owner/callback census request remains open.
Next:delivered bootstrap/native storage,time and original stream/registry bindings.

Context-init breadcrumb (2026-10-12): `event_context_initialization.cpp` ports
complete Init/ReInit and context banks. Tests retain two256/1b000 allocations,
separate post-allocation GP timestamps,low/high writes,retained/live pointers,
null allocation path,bank/schedule/ID/speed/other-context preservation and bulk
reset without cancellation. make test passes41 suites/69 Python tests;make all
passes. Native storage/SDK heap/address/cache/platform remains explicit.Next:
delivered bootstrap allocator and native providers,time binding,original Jump
source/registry ownership (requested above) and original runtime comparison.

Combined-interpreter breadcrumb and source-binding request (2026-10-12):
`event_interpreter.cpp` now routes all recovered local0..118 bodies through
actual namespaces/lifecycle/manager services. Integration tests preserve live
child→parent resumption,first-call polling,independent-root append policy and
fractional credits. Final make test passes40 suites/64 Python tests;make all
passes. Fixtures are authored,not original Jump execution.

Documentation agent:please pin remaining NormalJump entry/root registration
and owner/priority/type/context/ID/speed writer paths needed to bind80355cf0,
plus callback address/caller census for the stream's required native service
registry. Existing local graph/helper contracts do not establish these source
owners. Cite existing delivered contracts if they already cover any requested
path. Native packed memory/numeric/formatter/heap/time/platform sources and
original traces stay open;delivered init/time contracts are next port work.

Tail-opcode breadcrumb (2026-10-12): `event_tail_opcodes.cpp` covers1/3/11 and
113..118,evtGetPtr semantic array domain,and full debug classifier/call arguments.
Original sprintf/string sink remains an explicit formatter service. Tests pin
all thresholds/template/CR6/hex anomalies,raw flags,context/GP order,live polling
and active-only256 slot probe/yield. make test passes39 suites/64 Python tests;
make all passes. Next:combine interpreter services,then native context/bootstrap/
address/numeric/formatter owners and original Jump. Packed/extreme/source/runtime
and scratch-consumer traces remain existing EVT-01 closure requirements.

Memory-opcode breadcrumb (2026-10-12): `event_memory_opcodes.cpp` handles74..90
and local map allocation/fill/cache leaves. Tests preserve best-fit/first ties,
mandatory split/zero remainder,header/fill/cache order,no inferred buffer free,
count/output/root mutations,wrapped time products and distinct raw-address/
namespace-double-query routes. make test passes38 suites/63 Python tests;make all
passes. Native heap/address/cache/platform/extreme/alias/numeric/source ownership
and original traces remain open. Next:tail/debug routes,delivered heap/context/
bootstrap bindings and original Jump stream/callback services.

SWITCH-opcode breadcrumb (2026-10-12): `event_switch_opcodes.cpp` handles34..49
with semantic signed states/values and complete ordinary/group/mask/range routes.
Tests preserve captured versus live stack/query timing and header scans;actual
OR/AND success/failure dispatcher streams converge through END_SWITCH and wait.
make test passes37 suites/63 Python tests;make all passes. Native packed/invalid-
depth/source/group/runtime domains remain open. Next:delivered memory/buffer and
tail/debug routes,then native runtime sources and original Jump binding.

IF-opcode breadcrumb (2026-10-12): `event_if_opcodes.cpp` handles12..33 and the
full alignment-dependent strcmp body. Tests preserve unordered CR branch results,
query/argument/live-next mutation,raw masks,consumed scan markers and exact string
load extent. Real dispatcher executes either branch then WAIT_FRM. make test
passes36 suites/63 Python tests;make all passes. Native string/address allocation,
FPSCR/NaN exceptions and original traces remain open. Next:delivered SWITCH/grouped
CASE and remaining memory/debug families,then native sources/original Jump.

Loop-opcode breadcrumb (2026-10-12): `event_loop_opcodes.cpp` now handles4..8.
Tests cover signed labels/live fallback,raw and variable counts,zero/negative
threshold policy,callback-mutated index/body,and distinct BREAK/CONTINUE lexical
headers. A real dispatcher/data loop verifies all iterations before frame yield.
Final make test passes35 suites/63 Python tests;make all passes. Shared airborne
fixture failure from13:20 MDT was reconciled by the native overworld session.
Next:delivered IF/switch and remaining memory/debug handlers,then native context/
address/numeric services and original Jump. Existing source/domain/runtime and
invalid-depth/packed-alias closure remains open;no original trace was inferred.

Data-opcode breadcrumb (2026-10-12): `event_data_opcodes.cpp` now handles50..73
with actual operand/dispatcher integration. Tests retain RHS/query/convert order,
raw destinations/data, live cursor mutation and sequential IEEE versus indexed
signed-word floats. Original divw/fctiwz domains remain required provider services.
Next:delivered loops/predicates/switch and remaining memory families,then native
context/address/source owners and original Jump. Existing runtime traces remain
necessary;this helper does not claim original script execution or full VM closure.

Operand-service breadcrumb (2026-10-12): `event_operands.cpp` ports complete
integer/float namespaces and saved leaves. Tests cover asymmetric routing,
encoded boundaries, word/byte switches, signed flag addressing and old/GP/query
order. Original address/context/storage/FPSCR/save owners remain explicit
services. Next:delivered branch/loop/arithmetic/switch/data bodies, then original
Jump stream bindings. Existing source/domain/runtime traces remain open; no
provisional constants were substituted for measured behavior.

Opcode routing breadcrumb (2026-10-12): `event_control_opcodes.cpp` wires
recovered92..112 run/ID/child/control/block/end paths to lifecycle services.
Real manager/dispatcher integration exercises joined child copy-back and parent
resumption;ID outputs occur after local copies and plain/ID brother150 differs.
Float/output providers are explicit and unsupported routes require other modules.
Next:delivered operand namespaces and branch/arithmetic/loop/switch/data handlers,
then original Jump stream and actual native source owners. Existing original
runtime/context/alias/domain/pause/input/scene traces remain necessary closure.

Control-service breadcrumb (2026-10-12): `event_controls.cpp` and its suite
port exact stop/start inlined context pattern,postorder relationships,all/other/
ID scans,CheckID and priority/type/speed setters. Tests preserve hard0xef,active
liveness,transitive exclusion and current-pass remaining credits. Next is actual
control/run/block opcode routing into dispatch,then full operand/branch/data
providers and original Jump. Existing context/type/pause/source/cycle-domain and
original multicredit/cancellation traces remain needed;no generic pause refcount
or immediate suspension was inferred.

Lifecycle breadcrumb (2026-10-12): `event_lifecycle.cpp` ports newly delivered
complete Delete/DeleteID and Restart contracts,including confirmed8041e860
counter. Tests cover live descendant order,cancellation copy-back,independent
inline lifetime,duplicate IDs,stale schedules,restart preservation and parent
resumption through the real manager. Prior deletion-counter request is answered;
stop/start/control binding and full original graph/value/source/pause traces
remain next. No helper claims packed original aliasing or malformed-cycle repair.

Descendant breadcrumb (2026-10-12): event_creation now ports child/brother
constructors and107/110 block wrappers,including active-manager unconditional
append,child wait links,global speed,inherited/reset metadata,distinct zero-ID/
run-check order and post-run-check source recopy. Tests retain copied local words
and independent inline links. Next is deletion/copy-back/control binding.
Please pin complete evtDelete context-capture and active-count/SDA write identity
alongside the existing EVENT_RUNTIME deletion steps,including whether its
counter is8041e860 used by constructors;this avoids guessing a counter owner.
Original capacity/label/relationship/cancellation/pause traces remain requested.

Constructor breadcrumb (2026-10-12): `event_creation.cpp` ports delivered
root/typed constructors,label scans and run-check with original append/ID order.
Native free capacity and16-label domain assertions are explicit boundaries;
please retain capacity/label-domain provenance and original constructor/creation/
pause traces as open obligations. Child/brother/inline/deletion/restart/control
binding and full tagged opcode/value providers remain next before native Jump.

Current delivered coverage:the complete local manager/priority loop,root/typed/
child/brother constructors,restart,plain/ID run and concurrency variants,wait/
USER_FUNC continuations,control wrappers,setters,full deletion/stop/start and
integer/float operand routing/saved RAM leaves are now linked in the scoped delivery table and EVENT_RUNTIME
follow-ups. These answer those parts of the original request below. [Complete opcode coverage](behavior/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md)
now maps all119 table entries to recovered local contracts,including tail/debug
and WAIT_WHILE. Remaining closure is numeric/storage/context/time/source/domain
initialization,address/callback translation,heap ownership and original pause/
multicredit/creation/deletion/Jump/story traces. A portable manager test pass
is not a complete executed Jump/story VM.

Dispatcher implementation breadcrumb (2026-10-12): `event_dispatch.cpp`
and its suite implement delivered fetch/common tails,WAIT_FRAME,WAIT_MSEC and
USER_FUNC continuations. Manager integration verifies multiple credits share
one caller sample and do not advance MSEC lifetime between polls. Other opcodes
remain explicit handlers;native function/address/value/scene providers and
constructor/control/ownership bindings are not inferred. Next:delivered root/
child/brother/inline constructors,label/run checks,deletion/restart and controls,
then remaining tagged operand/branch bodies and original Jump services. Preserve
existing original runtime/pause/cancellation/credit trace requests.

Manager implementation breadcrumb (2026-10-12): `event_manager.cpp` and
its host suite now port the delivered complete manager loop/priority table.
Explicit GP/context/clock/dispatch providers preserve captured versus table
context,raw tick delta,eligibility,identity,rounded credits,live count/appends
and abort/no-inner-revalidation behavior. It does not sample pads or infer60Hz
original clock values. Native constructors/dispatcher/value domains,actual GP/
context/time/pause producers and original multicredit traces remain necessary
bindings. Next:delivered wait/USER_FUNC continuation services and root/child/
brother construction,then original Jump graph.

Native implementation requires a complete US evtmgrMain/evtmgrCmd service
contract before binding the recovered Jump graph:eligibility masks,64-bit lifetime
increment source/rounding,credit precision/subtraction order,interpreter0/-1/1
handling,current-table appends and live-ID checks. EVENT_RUNTIME.md and input
cadence close selected operations but explicitly leave the whole loop and
constructor policy partial. Please provide the missing complete local bodies,
initial credit/state/type/run-check policy for root/child/brother/inline entries,
and WAIT_FRAME/WAIT_MSEC/USER_FUNC continuation semantics with exact first-call
markers. Keep mathematical event invocations separate from physical pad samples.
Original pause/multicredit/creation/deletion traces remain necessary verification.

Implementation breadcrumb:commit717da56 adds `battle_action_services.cpp` and
its host suite. The native adapter now uses Start/Result/Stop services and records
help requests,while keeping its provisional attack timer explicit. Result polls
the stored pending bit after evaluation;Stop leaves callback teardown to main.
Parameters/raw flags/result/count/window readers and prize wrappers are portable;
original tagged VM,prize-byte producers and help UI remain unbound. ID1's transient
compile observations were resolved in717da56 (signed literal and typed clamp);
subsequent make test/make all pass. Continue requesting actual script/VM owners
rather than treating a passing command helper as complete original attack timing.

### JUMP-01 — Ordinary Jump follow-up and per-hit event flow (open)

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

Native position breadcrumb (2026-10-12): `source/battle_position.cpp` and
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

Further ID2 answer:[Takeoff](behavior/BATTLE_JUMP_TAKEOFF_POSITION.md) closes
the mixed target-hit X/Z + actor-home Y output,private null-weapon candidate
sample masks/choice4,signed count gate,closer-X tie rule and pointer/return/side
offset branches. Candidate cores/RNG/validity remain service dependencies.
[GetMoveFrame/MovePosition](behavior/BATTLE_MOVE_FRAME_AND_POSITION.md) close
ceil-like query versus floor-and-speed-adjusted motion counts,raw/float argument
differences,first-call integration,horizontal-only baseline and final XYZ snap/
yield. Query fallback speed does not mutate/repair unit speed. Actual ordinary
calls are automatic movement,type0/-1 and query speed0;abs(query-22) is retained.
[Position queries](behavior/BATTLE_UNIT_PART_POSITION_QUERIES.md) and
[movement sound](behavior/BATTLE_MOVEMENT_SOUND_CONTROL.md) close called local
geometry/core/wrappers and their ordered count/retained voice/stop interfaces.
TAKEOFF/MOVE/POS/MOVE-SE requirements give acceptance cases. These complete
local callbacks do not close candidate/math/audio/source/domain/scheduling
dependencies,selected target/cost/stylish/aftereffect/badge tail or original traces.

Native implementation breadcrumb (2026-10-12): `source/battle_jump_motion.cpp`
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

Further ID2 answer:[MarioJumpPosition motion](behavior/BATTLE_MARIO_JUMP_MOTION.md)
and [rounded numerical operations](behavior/BATTLE_MARIO_JUMP_NUMERICS.md)
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

[Sound-mode loader](behavior/BATTLE_UNIT_SOUND_MODE.md) closes the local record
schema,type2/-1 source words and conditional setup playback request. Word18c
is playback input,not proved an allocated voice handle. Body-local sound return
is ignored;source writers,__psndSFXOn resources/voice lifetime and audio traces
remain open. Next graph dependencies are Takeoff/MoveFrame/MovePosition and
selected-target/cost/stylish/aftereffect/badge-tail services,original reachability/
domain/numeric/input/state traces. Generic and Mario local motion contracts do
not by themselves close those services or the original encounter/game.

Further ID2 answer:[MarioJumpParam](behavior/BATTLE_MARIO_JUMP_PARAMETER.md)
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
[complete268-command Jump request graph](behavior/BATTLE_NORMAL_JUMP_EVENT_FLOW.md)
pins target/cost/setup/approach/precheck/motion/result/damage/feedback/recovery/
tail branches. Shared AC count is distinct from script local10:count<=1 success
requests CheckDamage flags20000,wait2,local10++,then repeats label10 without
new SetupAC/StartAC;count>1 success requests20100 and finishes;failure requests
100 and declares-1 only when local10==0. Next-pass precheck uses100100. No direct
target-HP getter or post-damage result gate occurs before repeating. Preserve
repeated precheck rather than guaranteeing a second damaging hit. Missing target
reaches actual label99;avoidance skips result/damage and converges on label95.

[Result/count/grade/stop services](behavior/BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md)
close raw destination readers,raw mask1 update,window output,code30 prize-1/
unsigned byte1cbd grade clamp,and TimingA's deferred StopAC state1000. The stream
has no AcSetParamAll/AcSetFlag call and does not write count-window selector2.
[VM/concurrency](behavior/EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md) pins signed
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

The implementation now uses the [recovered power callbacks](behavior/BATTLE_WEAPON_POWER_CALLBACKS.md)
in `source/battle_weapon_power.cpp`. The native harness executes one documented
Jump hit with level1 supplied as an explicit fixture;successful timing records
success but no longer fabricates a two-power hit. This is incomplete attack
sequencing,not a faithful finished Jump or a claim of original starting loadout.

Please recover the complete ordinary Jump event0x80355cf0 control flow:all
SetParam/flag/difficulty/start/result/get-result/stop branches,per-hit state-bit
writers,precheck and damage flags,subsequent-hit conditions/count/target-alive
gates,inline/child motion and wait ownership,bonus/stylish/feedback declaration
and cleanup. Link existing motion callbacks and original interpreter/pad cadence
to the actual damage/result invocations. Retain raw event operands and addresses.
Provide original timing/state traces separately;do not insert the harness's
impact48/return42 schedule or its temporary one-hit policy into the contract.
The portable services test invalid signed levels via bounded providers;original
pouch initialization,upgrade/load/save and bonus-byte writers still need binding.

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
that fixture. make all fails in source/battle_action_services.cpp:61 because
std::clamp(signedWord(result),0,6) deduces long int versus int on devkitARM;
explicit int32_t template/bounds preserve the intended signed domain. These are
concurrent native compile errors;lecture/helper binary checks pass. Re-run both
commands after correcting them before recording a successful native validation.


ID1 build-validation follow-up:the native fixture now uses the signed local-work
encoding and clamp uses an explicit int32_t domain. Final make test passed all12
CTest suites plus58 Python tests;make all exited0 with no rebuild. The earlier
compile failures above are resolved. ID1 changed no native files for those fixes.


ID2 selection/cost delivery (2026-10-12), JUMP-01/DMG-01 partial answer:
[selection and weapon](behavior/BATTLE_SELECTED_TARGET_AND_WEAPON.md) specifies
retained signed target/permutation reads and output callback order, player versus
held-item weapon lookup and all15 cursor dispatch entries. Unsupported cursor
types leave output untouched; do not supply a fabricated default Jump weapon.
[cost and payment](behavior/BATTLE_WEAPON_COST_AND_PAYMENT.md) supplies every
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
[sampling](behavior/BATTLE_ENEMY_CANDIDATE_SAMPLING.md) closes all local passes,
metadata/partial row writes, signed count and capacity invariants, side-mask swap
return, geometry and permutation sorting. Return Q is the swap indicator, not
nonempty/count. [choice/RNG](behavior/BATTLE_ENEMY_CANDIDATE_CHOICE.md) closes
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
[timing/results](behavior/BATTLE_STYLISH_TIMING_AND_RESULTS.md) specifies the
shared W271c..2734 record, first-call initialization plus same-call t1 polling,
exclusive lower/inclusive upper success, opening history rejection, autoInput,
no-input timeout and ordered result outputs. Both ordinary brothers pass history
count0; timing is callback invocations, not rendering frames. Main return0
repeats/yields,1 completes/yields;GetResult returns2 and does not poll/reset.
[icon](behavior/BATTLE_STYLISH_TIMING_ICON.md) closes local eligibility and
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
[dispatch](behavior/BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md) closes every local
weighted/chance/force decision and exact RNG/read/call order,with nine actual
Jump/Hammer chance bytes and both ordinary Jump callers. Aftereffect dispatches
stage hazards,not a generic status/resource cleanup. Zero chances still consume
mandatory draws. [stage leaves](behavior/BATTLE_STAGE_AFTEREFFECT_REQUESTS.md)
closes A1/B shared replacement,A2 ignored handle, nozzle weighted/type/liveness
helpers and frame/object gating/registration. Nozzle and fall constructors have
explicit unguarded event-ID reads;do not invent local rollback/fallback policy.
Frame sets stage0x20 before the subsequent object test;opening boss flag0x1000
suppresses object registration. Queued event bodies,stage/source/allocator domain,
seed interleaving and executed hazards remain open. Standalone verifier passes
13 complete body fingerprints,56 anchors,seven data spans and two ordinary
callers. ID2 lock released;parent requests/other owners remain open.


ID2 badge-tail delivery (2026-10-12), JUMP-01/DMG-02/STATUS-01 partial answer:
[32-command tail](behavior/BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md) closes both
copied-local brothers, sequential HP/FP >=1 gates, effects, brother120/owner125
waits and resource writes/delete. [recovery/geometry](behavior/BATTLE_BADGE_RECOVERY_SERVICES.md)
closes raw-actor point readers signed min(badgeByte,damageWord), resource wrappers
(including resolved but ignored part), HP bookkeeping attribute and geometry
outputs. [coercion/effect route](behavior/BATTLE_BADGE_RECOVERY_PRESENTATION.md)
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
[complete manager](behavior/EVENT_MANAGER_COMPLETE_LOOP.md) closes context,
raw64-bit tick delta/clamp/substitution, unstable unsigned-priority exchange,
active/live-ID/0x92 eligibility, lifetime once per visit and single-rounded
credit add/subtract-before-dispatch. Dispatcher0 can run again under remaining
credit;1 aborts the whole manager,-1 ends only that entry. The count is live.
[dispatch/waits](behavior/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) closes fetch,
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
[root/typed constructors](behavior/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) closes
first-free slot scan,clear/all explicit fields,priority/type/flags narrowing,
global speed copy/credit0,ID wrap ordering and conditional live append. Only
manager-active AND entry.flags0x20 appends;no immediate dispatch or resort.
[labels/run-check](behavior/EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md) closes
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
[descendants/107/110](behavior/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md)
closes full child/brother inherited/reset maps,parent waiting versus owner-only
links,priority narrowing,global speed/credit0,labels and active-manager append.
Child/brother append has no root flag0x20 gate. It specifies zero-ID/run-check
order differences,post-constructor wrapper recopies,brother150 versus inline
message metadata and independent inline ownership. [restart](behavior/EVENT_RESTART_PRESERVATION.md)
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
[run/ID variants](behavior/EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) closes independent
92/93 roots with flags0/no same-pass append, copied fields and ID publication;
108/111 end scanners/output order and brother111's omitted150 copy.
[controls/ID scans](behavior/EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) closes94..106
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
[deletion/copy-back](behavior/EVENT_DELETE_AND_COPYBACK.md) closes active gate,
child-before-brother recursion,live slot scans,parent copy-back even on ancestor
cancellation,paired metadata,active clear/memset/counter order and all-match
DeleteID. [stop/start](behavior/EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md)
closes full descendant-before-mask behavior and original compiler-inline context
capture policy,All wrappers and same-pass credit implications. No host rollback/
immediate suspend/refcount behavior is original. Verifier passes six full body
fingerprints,70 anchors,SDA span and paired245-instruction comparison:only seven
flag mutations differ between Stop/Start. Own lock released;other EVT-01 scope
and ID1 reservations remain open. make test passes26 CTest suites and58 Python
tests;make all passed. No native changes by ID2.


ID2 EVT-01 operand namespace delivery (2026-10-12), partial answer:
[integer APIs](behavior/EVENT_INTEGER_OPERAND_NAMESPACES.md),[float APIs](behavior/EVENT_FLOAT_OPERAND_ROUTING.md)
and [saved leaves](behavior/EVENT_SAVED_SWITCH_STORAGE.md) close all four local
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
[GOTO/loops](behavior/EVENT_GOTO_AND_LOOP_OPCODES.md) closes4..8/NEXT including
signed pointer/label gate,first16 signed-byte lookup/missing fallback,raw count
push,deferred variable decrement,literal0,threshold-10000000 and BREAK/CONTINUE
lexical/stack distinctions. [assignment/arithmetic/cursor](behavior/EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md)
closes50..64:SET resolved/raw/float,all RHS-first updates,wrapped arithmetic,
MOD's signed-to-single/+0.5/fctiwz before remainder,float single operations and
cursor advance-before-read/output with reload between lanes. No native for-loop
count snapshot,ordinary integer MOD or implicit iteration yield is original.
Verifier passes six containing hashes (NEXT full,dispatcher scoped/APIs linked),
73 anchors and24 spans (20 dispatch targets). Own lock released;other predicates,
readers/numeric/source/domain and original traces remain EVT-01 work. make test
passes29 CTest suites and58 Python tests;make all passed. ID2 made no native edits.


ID2 EVT-01 switch/case delivery (2026-10-12), partial answer:
[Switch/stack/scans](behavior/EVENT_SWITCH_STACK_AND_CASE_SCANS.md) closes34..49
actual dispatch table,raw/resolved pushes,signed stack,state gates and next-header
scans. [Grouped/mask/range](behavior/EVENT_GROUPED_MASK_AND_RANGE_CASES.md)
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
[Integer/mask/continuations](behavior/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md)
and [float/string](behavior/EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) close12..33,
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
The already delivered [deletion](behavior/EVENT_DELETE_AND_COPYBACK.md) step5 and
[root constructor](behavior/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) allocation
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
[Readers](behavior/EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) closes65..73,
all cursor lane/query/load/output ordering;73 is indexed SIGNED integer→single
float,not IEEE indexed load. [Bitwise/time/memory](behavior/EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md)
closes77..90,raw/resolved RHS,captured/delayed destinations,wrapped products
before signed60/1000 division and raw-address versus one/two-query API routing.
[User buffers/allocator](behavior/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md)
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
[Tail/debug](behavior/EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) closes113..118,
WAIT_WHILE11 and implicit1/3 runtime no-ops.118 queries active slots0..255 with
fresh GP context per helper then handler1 regardless of found/not-found;no
capacity gate.115 formats into shared scratch only,uses independent signed
routing/raw-mask values and exact UW/GSW/LSW hex argument anomalies. All23
original diagnostic templates and formatter CR6 inputs are explicit.
[Coverage inventory](behavior/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md) maps
EVERY119 actual target to recovered contracts. Generic missing local opcode
bodies are now closed;EVT-01 stays priority0/open for source/address/callback/
context/capacity/time/heap/numeric domains and original execution acceptance.
Verifier passes nine containing/complete/linked hashes,164 anchors,145 spans
including119 table words and23 templates. Own binary-validated documentation
lock released;make all passed,58 Python tests passed;full make test awaits the
missing concurrent native fixture described above. ID2 changed no native files.


ID2 native validation follow-up (2026-10-12,context-init research):the previously
missing event_control_opcodes_tests.cpp now exists;configuration/build succeed.
Fresh make test runs32 CTest suites:30 pass,including event_control_opcodes;
core fails tests/core_tests.cpp:387 groundAt({-.5f,2},0,h) expected h≈.2;
field_movement fails its line48 close(m.slope.inclination,30). Current terrain/
slope native edits are concurrent;ID2's documentation-only task changes no
native fixtures/mechanics. Full host gate remains pending for these actual
assertion failures;this supersedes the older missing-file blocker.


ID2 EVT-01 context/init delivery (2026-10-12), partial answer:
[Init/reset](behavior/EVENT_CONTEXT_INITIALIZATION_AND_RESET.md) closes both
complete bodies:Init allocates TWO256*1b0 arrays (216KiB payload combined),
zeros32-word/three-flag banks and resets three scheduling/allocation globals;
ReInit clears ONLY selected existing array,keeps work/flag banks and live schedule
count,sets shared allocation count/manager-active0. Neither resets global ID/
speed,performs entry deletion/copy-back or frees old buffers. Initialized DOL
ID starts1,speed1.0;those values are not assignments in Init/ReInit.
[SDK heap/callers](behavior/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md) closes
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


ID1 ray ABI correction (2026-10-12):OVERWORLD_WALL_MOVEMENT.md had the
HitQuery output labels reversed. Original backend computes origin+scaled
ray into query+0x24 at80016454/58;triangle normal is copied/normalized into
query+0x30 at80016474..80 and80016530..68. Normal=+30,hit point=+24.
Partner caller reads+30 normal for its projection. Corrected the doc;native
high-level ray provider should be checked against this ABI,without swapping
already correctly named native vector fields. Full backend/numeric traversal
remains separate. ID1 verifier pins these result-layout anchors.


ID2 EVT-01 time/update delivery (2026-10-12), partial answer:
[Retrace clock](behavior/EVENT_RETRACE_TIME_PUBLICATION.md) supplies actual
GP38/3c producer:OSGetTime raw-TB delta,GP10/window gate,unconditional raw-baseline
publication,masked auxiliary clock banks,event bank independent of low system
mask,and field-only40 bank. Freeze discards later catch-up ticks because baseline
continues updating. Window null/1000/state0 or>=200 does not freeze;nonzero<200
including negatives does. Scoped bootstrap zero-bank/VI refresh0→60 else50/
post-retrace registration and full raw reader are pinned;hardware rate unmeasured.
[Outer loop](behavior/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) closes main,
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
behavior/OVERWORLD_WALL_MOVEMENT.md→FIELD_PARTNER_SURROUNDING_WALL.md. ID2 leaves
that ID1/terrain contract work untouched;this is a shared-document arrival
dependency,not an original-game evidence failure. Timing/general/manager binary
verifiers and Python compilation pass. ID2 changed no native or asset tooling.


ID1 urgent surrounding-wall correction (2026-10-12):original marioChkWallAround
800891e0 fcmpo;800891e4 cror2,1,2;800891e8 bf2 makes ordered dot>=0
SKIP second correction;negative/unordered applies half second push (if FP
exceptions permit continuation). PSVECDotProduct8029ec84 confirms an ordinary
positive dot,not a negated dot. First vector copied to stack54 at80088db4–bc
BEFORE cap at80088dd8;dot uses this UNCLAMPED copy. Existing wall doc corrected.
Native source/field_wall.cpp currently tests >0 to apply,and field_wall_tests
expect same-direction second push to move -2.5→-7 and reverse push to stay-2.5;
these two outcomes must reverse for original parity. Native owner should update
policy/fixtures;ID1 does not edit concurrent native files. Detailed partner binding
and pinned evidence follow under the ID1 lock.


ID1 surrounding-wall delivery and validation (2026-10-12):
[Binding/core/numerics](behavior/FIELD_PARTNER_SURROUNDING_WALL.md) now exists;
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
native dot predicate:source/field_wall.cpp still applies when dot>0 and saves
a capped first push at this check. Exact original acceptance cases are in the
new contract;native owner review remains required for parity.


ID2 EVT-01 heap/release delivery (2026-10-12), partial answer:
[Map release/battle pool](behavior/MAP_BUFFER_FREE_AND_BATTLE_POOL.md) closes
successor-then-predecessor merge,header32 accounting,early null/used gates and
400KiB primary-map reservation/nested root/free order. [Pool bootstrap/reset](behavior/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md)
closes five original config records,fixed/percentage two-pass assignment,SDK
creation/destroy/recreation,ignored replacement handles and memClear index1
primary root rebuilding. [SDK free/census](behavior/SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md)
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
[Fly](behavior/FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md) and
[ground](behavior/FIELD_PARTNER_GROUND_POSTPASS.md) specify actual mobility==1
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
[Ceiling query](behavior/FIELD_PARTNER_CEILING_QUERY.md) and
[scalar wrapper](behavior/FIELD_SCALAR_RAY_WRAPPER.md) establish four samples,
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
[Disc thread](behavior/EVENT_DISC_THREAD_READINESS_GATE.md) closes GP10 producer,
all13 numeric statuses (1 preserves gate),A/B/C query order,retrace-change counting,
count10/11 boundary,card override and alternate input/reset requests. [System
presentation](behavior/DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md) closes local
ROM-font routing/init,width/projection/color/request ordering and four exact
US-default DOL resources. V changes from selector to pointer during rendering;
status1 retains it,with no automatic repair. [Audio cover state](behavior/AUDIO_DISC_COVER_STATE_REQUESTS.md)
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
[Query](behavior/FIELD_PARTNER_FLOOR_QUERY.md),
[slope numerics](behavior/FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md) and
[heading](behavior/FIELD_PARTNER_FLOOR_HEADING.md) specify complete local
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


ID1 partner appearance/screen recovery delivery (2026-10-12):
[Appearance](behavior/FIELD_PARTNER_SUPPORT_APPEARANCE.md),
[vector ray](behavior/FIELD_PARTNER_VECTOR_RAY_WRAPPER.md),
[screen recovery](behavior/FIELD_PARTNER_SCREEN_RECOVERY.md) specify complete
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


ID2 DMG-01/DMG-02 unit-entry delivery (2026-10-12), partial answer:
[Full Entry](behavior/BATTLE_UNIT_ENTRY_COMPLETE.md) closes complete local6b4
identity/geometry/resource-call order:64-slot first free,checked unit allocation,
early registry publication,unchecked part allocation/zero-count path,kind/setup
copies,pouch overrides,gauge-before-import,body first versus cached last,display/
special-kind allocation and final asynchronous init script with actor/event IDs.
[Registry/scalar/geometry](behavior/BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md)
closes registry bounds asymmetry,U8 GetBelong versus S8 consumers/enemy mapping,
S16 height/body lookup,all called vector setters/angle wrapper and HpGaugeInit.
Direct damage explicitly extsb AFTER the U8 getter;its wording is clarified,
without changing recovered zero-test behavior. [Pose/display](behavior/BATTLE_UNIT_POSE_AND_DISPLAY_INITIALIZATION.md)
closes fallback-first category lookup versus bounded28-record display lookup,
borrowed tables/names,copy/resource-entry order and effect/icon init leaves.
Field114 initializes1,vector scale separate;field144 is borrowed kind.b0 pointer,
its pointed schema/lifetime/other writers still open. Whole local entry gap is
closed;kind/resource/pose/render/status/turn/source/domain and original traces
keep parents open. Own lock released;verifier passes33 containing/complete/linked
hashes,441 anchors,11 spans. make test passes40 CTest suites and64 Python tests;
make all passed. ID2 changed no native or asset conversion code.


ID1 placement3 output contract delivery (2026-10-12):
[Placement](behavior/FIELD_PARTNER_PLACEMENT3.md),
[math](behavior/FIELD_PARTNER_PLACEMENT3_MATH.md),
[NPC scan](behavior/FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md) close initialized-
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
