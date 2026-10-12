# US completion assessment and research priorities

Assessed2026-10-12 against US GameCube G8ME01 disc0/revision0. This assessment
supersedes older research-next lists where they continue into generic library
edge cases without establishing a shipped gameplay dependency. The GameCube US
version is the primary mechanics baseline;Switch evidence remains separate.

## Completion judgment

The full documentation objective is **incomplete**. There is a useful partial
reference for battle damage/status/turn helpers,message processing,event
scheduling and ordinary field movement. There is not yet a complete specification
of even one original playable story encounter from entry conditions through
persistent results and repeat-entry behavior. No matched original-game gameplay
trace establishes attack timing or end-to-end equivalence. The native Rogueport
slice is a development harness with provisional battle rules,not evidence of
retail behavior. See [mechanics status](../../MECHANICS.md).

The eventual complete-game requirements document cannot yet be derived from
closed behavior contracts. An interim coverage/priority register is useful,but
must not be presented as that final deliverable. Reusing assets does not remove
the need to specify their loading,event bindings,animation and audio semantics.

| Evidence dimension | Current evidence | What it does not establish |
| --- | --- | --- |
| Detailed notes | 85 Markdown behavior documents before this assessment,excluding README;32 cover numeric/format/scan helpers | Number of complete gameplay systems |
| Binary checking | 297 function fingerprints;1,532 instruction anchors plus data/resource checks | Semantic completeness,reachability or executed branch behavior |
| US executable reference audit | 7,061 unique annotated DOL text starts;6,229 have at least one audited static reference | A complete function census or6,229 actually executed functions |
| REL/content inventory | 29 shipped RELs;260 US message files;13,186 key/text pairs in the prior message census | Scene order,speaker attribution,branch feasibility or full optional content |
| Concrete attack commands | 20 nonzero entries in the original command registration table | Which attacks select each ID,success timing or tutorial exceptions |
| Original comparisons | Isolated original boot/health screen recorded;no matched gameplay timing corpus | Movement/combat/story/audio equivalence |
| Full-system closure | None of the16 full-scope system rows meets its documentation/validation gate | A defensible overall completion percentage |

A fingerprint is a reproducible byte identity,not a completed behavior contract.
The297/7,061 ratio must not be reported as percentage game completion. Function
sizes vary;some notes cover additional field functions outside the fingerprint
list;some fingerprinted bodies are only partially documented. The pinned public
Japanese source audit (427 object declarations,134 source files) is likewise
not a US completion denominator. Existing scope gaps are tracked in
[the work register](../README.md#full-scope-completion-register).

## Reproducible US usage evidence

Run:

```sh
python3 tools/us_usage_audit.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/us-usage.json
```

The tool reads the ISO only. Original table/executable/event bytes are not
exported. Its ignored report contains names,addresses,counts,relocation metadata
and the original20 command IDs/callback annotations. DOL span SHA-256 is
`93c79aa312ca57c2785d0854a516c554e7683b525f8c3456dad568f4c440451a`.
External symbols are annotations from the existing research table,not proof.

The audit scans opcode18 direct branches in all DOL text,aligned candidate
function-address words in initialized DOL data,and module0 imports in all29
shipped RELs. It separates calls from nonlink branches. It does not resolve
indirect calls,computed addresses,branch feasibility or execution counts.
An imported function address may be a data reference rather than a script call.
Data words can coincidentally equal function addresses. Partial-address
relocations,entry aliases and interior entry points require separate analysis.

| Static-reference class | Unique annotated DOL text starts |
| --- | --- |
| At least one direct branch-and-link | 4,187 |
| At least one initialized-data word candidate | 1,471 |
| At least one shipped REL import | 1,608 |
| Any audited reference,including nonlink branches | 6,229 |

Classes overlap. The remaining832 entries are **unresolved by this audit**,not
proven unused. Direct references behind impossible conditions can also be dead.
Ranking by raw reference count alone would overvalue generic getters and library
routines while missing registered callbacks.

Selected concrete priorities from the US audit:

| Function | US address | Evidence | Consequence |
| --- | --- | --- | --- |
| marioMain | 0x8005a7e0 | One DOL direct call;already identified in original main-pass order | Recover player-state dispatch and input gates before special-case numeric helpers |
| battleMain | 0x800f904c | One DOL direct call in main-pass order | Close battle lifecycle/cadence and callers of recovered controllers |
| cardMain | 0x800b2508 | Two DOL direct calls | Recover save state machine and persistent reconstruction |
| evt_msg_print | 0x800d284c | 5,817 module0 REL references across26 modules;67 DOL data-word candidates | Bind message calls to actual event conditions/speakers instead of extending generic formatting blindly |
| btlevtcmd_ResultACDefence | 0x80107694 | 515 REL references across19 modules;four DOL data-word candidates | Trace attack impact sampling and tutorial gates for the recovered defense checker |
| evt_bero_mapchange | 0x8007aad0 | 238 REL references across26 modules;13 DOL data-word candidates | Expand entrance/exit/story gates beyond the road prototype |
| evt_mario_jump_pos | 0x800e4c54 | 63 REL references across19 modules;one DOL data-word candidate | Recover scripted movement alongside ordinary player jumps |
| swSet | 0x800858fc | 258 direct calls;15 REL references across3 modules | Trace persistent writes to story,progression and save/load owners |

Counts are address-reference occurrences,not numbers of executed scenes. The
three main functions above are not in the current297-function fingerprint list;
this does not negate partial notes on their call chains.

## Registered attack commands need gameplay bindings

BattleActionCommandSetup0x800fa45c reads the original20-byte entry table at
0x80364348,selects an ID and installs main/result/display/delete callbacks.
BattleActionCommandManager0x800fa5b0 invokes the selected main callback indirectly.
Their bodies are already inspected in [input/defense](../input/BATTLE_INPUT_AND_DEFENCE.md).
This establishes a retail dispatch path for the following callbacks,despite
**zero direct DOL calls** to each main routine in the audit:

| ID | Registered main callback |
| --- | --- |
| 1 | battleAcMain_TimingA |
| 11 | battleAcMain_TimingWide |
| 3 | battleAcMain_StickKeepLeft |
| 4 | battleAcMain_LstRcKeep |
| 5 | battleAcMain_Repeatedly |
| 6 | battleAcMain_RepeatedlyLv |
| 7 | battleAcMain_Shot |
| 8 | battleAcMain_ShotTarget |
| 9 | battleAcMain_StickRotate |
| 10 | battleAcMain_ButtonDown |
| 12 | battleAcMain_PendulumCraneTiming |
| 13 | battleAcMain_CraneTiming |
| 14 | battleAcMain_AirGauge |
| 15 | battleAcMain_PowerGauge |
| 16 | battleAcMain_PowerGaugeLv |
| 17 | battleAcMain_PowerGaugeLv2 |
| 18 | battleAcMain_SignalTiming |
| 19 | battleAcMain_GaugeTiming |
| 20 | battleAcMain_GaugeTiming2 |
| 21 | battleAcMain_MonosiriTarget |

Table order above is original. ID2 has no entry in this table. At assessment time none of these20 main bodies was fingerprinted. Subsequent
[ordinary attack bindings](../battle/BATTLE_ORDINARY_ATTACK_BINDINGS.md) establish Jump ID1
and Hammer-core ID3;[TimingA recovery](../battle/BATTLE_TIMING_A_COMMAND.md) now fingerprints
the ID1 main/result/window bodies. [Hammer ID3](../battle/BATTLE_HAMMER_STICK_KEEP_LEFT.md) subsequently recovers the second
registered main;the other18 main bodies remain outside the fingerprint set. Registration does not prove every ID
is selected in reachable US content. First resolve SetupAC arguments through
shipped unit/party/weapon event graphs;then document and execute each selected
command's inputs,states,tick boundaries,difficulty,result and cleanup. Do not
assign Jump/Hammer mappings from callback names or Switch behavior alone.
Special-attack/stylish commands need separate registration/binding inventories;
this20-entry table is not the complete set of combat input mechanics.

## Research order

Priorities below are based on player-visible consequences,shipped references
and dependencies needed to close complete behavior sequences. References are
leads until event selection and execution are established.

| Priority | Work | Deliverable / closure evidence |
| --- | --- | --- |
| P0a | Opening harbor→Crump→first Rogueport story graph | Original REL/module/section offsets,used VM opcodes and callbacks,exact US message IDs/tokens/speakers,conditions,joins,control locks and persistent writes;original branch/re-entry trace |
| P0b | Original encounter and common attack inputs | Resolve actual unit/weapon data and command IDs;document selected command main/result and defense impact caller;record simulation-tick input boundaries and tutorial overrides |
| P0c | Main-loop,input and ownership foundation | Recover marioMain/battleMain cadence,input snapshot/gates,pause/timebase,field/battle transition and RNG initialization/consumption on the P0 sequences |
| P1a | Ordinary exploration and partners | Close movement/collision/hazards/interactions,first-strike and follower/field abilities;test original positions and state transitions |
| P1b | Progression,rewards,inventory and saves | Level-up,SP/XP/coins/drops,equipment/items/shops,persistent namespaces,save record/lifecycle/failure/load reconstruction;verify complete state round trips |
| P1c | Battle breadth | Unit/part/target lifecycle,all shipped command bindings,status/badge callers,audience/stage/bingo/special/stylish,escape/loss/win and rewards;complete outcome traces |
| P1d | Required presentation | Scene-selected camera/actor animation,paper transforms,visibility and event→audio bindings;matched scenes with original timing and resource lifetime |
| P2 | All remaining chapters/interludes/optional content | Reachable map/event/encounter/quest/minigame census;document modes and conditions,then close coverage gaps and endings/postgame |
| P3 | Remaining platform/library branches | Analyze only when a shipped call/input domain,observable result or required hardware/service adaptation demonstrates relevance |

P0 tasks should advance together through one complete original sequence. Do not
finish every VM handler before examining the first scene;the scene supplies the
used opcode/service list. After the opening slice,the same coverage procedure
must expand through the whole US game,including optional and late-game mechanics.
The full objective is unchanged by choosing an initial validation slice.

## Rules for selecting the next function

1. Identify a shipped caller,event operand,registered table or resource binding.
2. Resolve the actual US arguments,preconditions and downstream state consumer.
3. Document the ordered behavior needed by that mechanic,including reachable
   failures and supported edge cases. Preserve exact original story content.
4. Validate the mechanic with original state/input/output checkpoints before
   declaring the call chain equivalent. Separate static proof from observation.
5. Expand branch/input coverage where shipped content or player inputs require
   it;record unresolved cases rather than calling them unused.

Defer generic NaN payload parsing,malformed scanf formats,unsupported Unicode,
large-exponent power branches and exact hardware exceptional arithmetic unless
a supported US input/caller reaches them or they alter a required observable
result. Preserve existing notes as research;do not make their further closure a
prerequisite for common combat/story mechanics without such evidence. Likewise,
finish font helper branches when a required scene/UI actually selects them.

Each new work item should record `US entry/caller → shipped binding → mechanic
→ observed checkpoint`,plus unresolved gates. A function-level no-reference
result alone cannot justify omission. Dead/debug/non-US branches may be excluded
only with explicit evidence and an exclusion note;shared code still requires
all behavior actually selected by the US version.

## Next concrete slice and completion gates

Start with gor.rel opening/Crump entry and its DOL/unit script dependencies.
Produce a reusable relocation/event inventory beyond the existing limited
Rogueport init scanner. Resolve script USER_FUNC pointers and SetupAC operands,
record exact scene state/message/actor bindings and extract the necessary attack
command contracts. Then record an isolated original encounter for timing and
branches. Generic library and unbound font analysis yields to this work.

A system is closed only when inputs/state/update/output/persistence and supported
US branches have contracts,shipped content bindings are covered,and original
comparisons address observable/timing uncertainties. The final requirements
will need stable IDs,contract/data-interface traceability and full-game acceptance
cases. The present assessment and tool do not close those gates.

## Implementation-request follow-up (2026-10-12)

The active handoff and scoped research reservations now live in
[documentation requests](../../DOCUMENTATION_REQUESTS.md). Implementation priority:
real weapon power/part/dispatch binding,enemy-hit defense and sampling cadence,
Jump/tutorial ownership,Hammer lifecycle/presentation,then status turn binding.
These are dependent slices of P0/common combat,not a replacement for full-game
coverage. Claim a specific subtask in that file before researching it;release
its lock after validated delivery and retain parent requests as open when
runtime composition or original comparisons are missing.

Current reproducibility has advanced to351 function fingerprints and1,844
instruction anchors. The original assessment counts above describe their earlier
snapshot. [Boss normal attack](../story/HARBOR_BOSS_NORMAL_ATTACK.md) now specifies its
95-command request stream;[target interfaces](../battle/BATTLE_TARGET_EVENT_INTERFACES.md)
adds seven complete local bodies. Their internal services,missing-target path,
input/impact cadence and original traces remain open. No full-system row has
closed and no overall completion percentage is justified. Final complete-game
requirements remain deferred;asset reuse does not supply these missing contracts.

## ID1 completion reassessment after used partner recovery work

2026-10-12 update:the table above retains its explicitly historical assessment
counts. Subsequent contracts close more local battle/event/field functions,
including the used fall→[placement4](../partner/FIELD_PARTNER_PLACEMENT4.md) call and its
[angle/output alias rules](../partner/FIELD_PARTNER_PLACEMENT4_ANGLE_LAYOUT.md). Current
listing audit covers575 files/80256 instructions/608 paired-single operations
with no pending corrections;the general contract verifier covers1980 anchors.
Neither count measures gameplay completion. Local partner recovery still depends
on called backend/libm and original-runtime/source-domain evidence. The general
fallback now has a [complete local writer contract](../partner/FIELD_PARTNER_GENERAL_PLACEMENT.md)
and [output/retry specification](../partner/FIELD_PARTNER_GENERAL_PLACEMENT_FALLBACK.md).

The full objective remains incomplete:no full-system closure or matched complete
original playable encounter is established. Prioritize shipped US call chains,
request-inbox blockers and an original encounter's story/event/persistence path.
For this recovered field chain,next resolve used movement handlers and remaining
backend/source domains,then compare original recovery trajectories. Switch
comparison and incidental library edge cases do not replace that US evidence.
Final complete-game requirements remain premature.

ID1 flying coverage update (2026-10-12):
[used flying executor](../partner/FIELD_PARTNER_FLY_DISPATCH.md),
[behind](../partner/FIELD_PARTNER_BEHIND_FLY.md) and
[homing](../partner/FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md) now have complete local
request graphs with two speed writers. The verifier pins nine actual direct
sites;original execution,full flying history and source/FP/terrain/pose domains
remain open. Current listing audit584 files/83372 instructions/638 paired-single
ops is clean;this is disassembly integrity,not gameplay completion. No full-system
closure or complete original playable encounter is established. Continue inbox
blockers and shipped US caller/source composition;final complete-game requirements
remain premature.

ID1 flying history/source update (2026-10-12):
[complete writer](../partner/FIELD_PARTNER_FLY_HISTORY_WRITER.md),
[initializer](../partner/FIELD_PARTNER_FLY_HISTORY_INITIALIZATION.md),
[actual Cloud/Vivian callback bindings](../partner/FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md)
and [source leaves](../partner/FIELD_PARTNER_FLY_SOURCE_LEAVES.md) now close their local
contracts. Kind5/6 records and four direct init/writer call sites establish
shipped US bindings;they do not establish runtime frequency or all source domains.
Current listing audit590 files/83916 instructions/640 paired-single operations
is clean. Broader pose/ability/terrain/source/numeric/original trace and complete
encounter gaps remain;no full-system closure or final requirements gate is met.

ID1 slit/roll coverage update (2026-10-12):
[slit](../partner/FIELD_PARTNER_SLIT_GATE.md),[roll](../partner/FIELD_PARTNER_ROLL_GATE.md),
[floor/forced exit](../partner/FIELD_PARTNER_SLIT_FLOOR_AND_FORCED_EXIT.md) and
[paper wrappers](../partner/FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md) now close ten local
bodies with22 direct US bindings. Collision helper,animation/resource/transform
backend,source domains and original form traces remain open. Listing audit599
files/84667 instructions/640 paired-single operations is clean;that is byte/text
integrity,not full-system completion. Complete original encounter/persistence and
broader shipped-system gaps still prevent final complete-game requirements.

ID1 collision/math and inventory-evidence update (2026-10-12):
[slit membership](../partner/FIELD_PARTNER_SLIT_COLLISION.md),
[camera direction](../camera/FIELD_SIMPLE_CAMERA_DIRECTION.md) and
[system angle table](../field/FIELD_SYSTEM_ANGLE_TABLE.md) now have complete local
contracts and actual slit caller binding. Listing audit602 files/85319 instructions/
654 paired-single ops is clean. Backend/global/source/FP/original trace domains
remain open. [Message inventory correction](US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md)
rechecks260 resources/13186 key/text pairs;new filename-number/story-prefix guesses
are not authoritative completion or dialogue evidence. No full-system/encounter
closure establishes the final complete-game requirements gate.

ID1 partner presentation update (2026-10-12):
[pre-display](../partner/FIELD_PARTNER_PRE_DISPLAY.md),
[pose selection](../partner/FIELD_PARTNER_POSE_REQUEST_SELECTION.md),
[facing](../partner/FIELD_PARTNER_DISPLAY_FACING.md),
[matrix/draw](../partner/FIELD_PARTNER_DISPLAY_MATRIX_AND_PASSES.md) and
[Vivian/shadows](../partner/FIELD_PARTNER_VIVIAN_PRESENTATION_AND_SHADOWS.md) now close
local eight-body graphs;the register correctly marks partial partner coverage.
Animation/GX/matrix/shadow/queue/source/numeric/original visual domains remain open.
Listing audit610 files/86164 instructions/656 paired-single ops is clean.
Unvalidated BTLNO string-name mapping/completion claims remain superseded by
[message evidence correction](US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md).
No complete system/original encounter establishes final game requirements.

ID1 animation setter update (2026-10-12):five complete used local setters are
now covered by [selection](../display/ANIMATION_SELECTOR_AND_RESTART.md),
[group reuse](../display/ANIMATION_PAPER_GROUP_REUSE.md) and
[time/clock](../display/ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md). Main/draw/resource/pool/
clock/helper/source/numeric/original visuals remain open. Static299-call census
is not game completion or original reachability/frequency. Full-game system
requirements remain premature until these and other system/story gaps close.

ID1 animation evaluator/init update (2026-10-12):six display contracts now close
Main/init local graphs:
[entry/time](../display/ANIMATION_MAIN_ENTRY_AND_TIME.md),
[sampling](../display/ANIMATION_TIMELINE_SAMPLING.md),
[cache](../display/ANIMATION_COMPRESSED_DELTA_CACHE.md),
[interpolation](../display/ANIMATION_OUTPUT_INTERPOLATION.md),
[facing](../display/ANIMATION_MAIN_CAMERA_FACING.md),
[init](../display/ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md).
Original M_Z_1 metadata is checked;loaded-resource/draw/clock/helper/copy/libm/
source/lifetime/original visual composition remains open. Complete-game system
requirements still require remaining system/story closure,not static call counts.
