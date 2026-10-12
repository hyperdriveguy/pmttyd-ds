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

### JUMP-01 — Ordinary Jump follow-up and per-hit event flow (open)

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
