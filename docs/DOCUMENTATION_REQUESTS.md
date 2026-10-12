# Documentation agent requests from the native implementation

Updated 2026-10-12 (America/Boise). The user requested using files in `docs/`
to communicate with the documentation agent and leave breadcrumbs. This file
is the request inbox; please add links to recovered contracts and mark requests
answered here. Static deductions and original execution observations must remain
distinct. Do not change native harness choices into original-game evidence.

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
The kind's raw halfword5 remains explicitly unclosed as HP until stat-copy/init
consumers are recovered. The local field-return wait accepts sequence2 plus
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

The [HP/FP calculators and revival marker](behavior/BATTLE_DAMAGE_CALCULATION.md)
are implemented in `source/battle_damage.cpp`,tested by
`tests/battle_damage_tests.cpp`. Native attacks now call the HP calculator and
retain the original-form result code. Their callback/stat fixture still supplies
development powers1/2,zero defense/no statuses and provisional actor resources.
The local calculator is separate from resource application/death/revival events.

Please recover the original weaponGetPowerDefault0x80189d30 body and its concrete
weapon+0x20/etc inputs,ordinary Mario Jump/Hammer callbacks and their arguments,
kanbu1 part defense/attribute tables,kind/stat allocation-copy/init consumers,
max/current HP/FP,actor/body-part and attack sequence reset/increment ownership.
Retain callback side effects/read order and signed field widths. Tutorial weapon
gate1 and enemy CheckDamage flags0x100 are now documented;global flag0x1000,
guard/status/counter/precheck/damage/application/reward/defeat consumers still
need a complete graph,including original multi-hit timing and input/RNG traces.

Status Check/Get readers and pouch item-count services are explicit portable
interfaces. The native fixture currently reports no statuses;it does not stand
in for the documented original status storage/recovery services. Revival marking
only tests original item151 in the correct owner path;consumption/healing/hit
event scripts remain unbound and must not be replaced by an invented revival.
Please link recovered callback/unit/application contracts here as they arrive.

## Implementation coverage and next work

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
