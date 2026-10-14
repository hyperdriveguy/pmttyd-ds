# DMG-01 — Weapon power, unit stats and damage dispatch binding

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [x] Full local kind/stat allocation-copy/init and max/current HP/FP
  constructor/setup consumers — [initialization/count answer](../behavior/battle/DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md).
- [x] Ordinary Jump shared AC count reset/increment/read ownership — same
  answer, independently reinspected setup/result/getter bodies.
- [ ] Remaining actor/body-part/other-attack counter owners, later mutations,
  actual equipment/level/bonus producers and original initialization traces.
- [ ] Complete guard/status/counter/precheck/damage/application/reward/defeat
  consumer graph, including original multi-hit timing and input/RNG traces;
  global flag `0x1000` and tutorial gate interactions remain open.
- [ ] Callback side effects/read order and signed field widths retained; do not
  assume tutorial level 1, bonus 0 or AC state without initialization/caller
  evidence.
- [x] Link current callback/unit/application deliveries — linked below and in
  the initialization/count answer. Future contracts should still be appended.
- [x] Default and Mario Jump/Hammer power callback bodies and tables —
  delivered ([power callbacks](../behavior/battle/BATTLE_WEAPON_POWER_CALLBACKS.md)).
- [x] Kanbu1 part defense/attribute tables and unit part lifetime — delivered
  ([part defense](../behavior/unit/UNIT_PART_DEFENSE_AND_LIFETIME.md)).
- [x] HP/FP calculators, resource application and entry stats — delivered
  (see the [delivery log](DELIVERY_LOG.md)).

## Request text (verbatim)

The [HP/FP calculators and revival marker](../behavior/battle/BATTLE_DAMAGE_CALCULATION.md)
are implemented in `source/battle/battle_damage.cpp`,tested by
`tests/battle_damage_tests.cpp`. Native attacks now call the HP calculator and
retain the original-form result code. Their callback/stat fixture still supplies
development powers1/2,zero defense/no statuses and provisional actor resources.
The local calculator is separate from resource application/death/revival events.
Resource application is now implemented in `source/battle/battle_damage_application.cpp`
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

## Answers and delivery notes (append-only history)

Further partial answer from ID2:
[part defense and lifetime](../behavior/unit/UNIT_PART_DEFENSE_AND_LIFETIME.md) pins
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
[weapon power callbacks](../behavior/battle/BATTLE_WEAPON_POWER_CALLBACKS.md) recovers
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

Partial stat answer arrived in
[entry/stats/spawn](../behavior/unit/UNIT_ENTRY_STATS_AND_SPAWN.md):entry publishes the
allocated unit before init,then copies signed kind HP/FP into three independent
fields each. The checked boss receives HP5/FP0 at those stores;later overrides
and the rest of unit entry remain open. Do not treat that as a measured starting
stat trace or collapse current/max/base copies when implementing resources.

### 2026-10-13 — Priority1 initialization and shared-count delivery

[Initialization/count bindings](../behavior/battle/DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md)
composes full local Entry/UnitSetup, parts, stat, pouch and badge contracts;
independently pins setup reset, four encoded TimingA success-increment sites
and read-only count query. Corrected result-reader battle/output capture order
and clarified pending bit0/mask1 in the linked result/stop contract. Count is
shared across the ordinary Jump repeat path, not a per-hit fresh command.

Additional current consumer deliveries: [HP/FP calculation](../behavior/battle/BATTLE_DAMAGE_CALCULATION.md),
[application](../behavior/battle/BATTLE_DAMAGE_APPLICATION.md),
[damage orchestration](../behavior/battle/BATTLE_DAMAGE_ORCHESTRATION.md),
[power callbacks](../behavior/battle/BATTLE_WEAPON_POWER_CALLBACKS.md) and
[ordinary Jump graph](../behavior/battle/BATTLE_NORMAL_JUMP_EVENT_FLOW.md).
These local contracts do not close the complete reward/defeat/tutorial graph.

Validation: complete unit-entry, unit-setup, Jump-graph and general behavior
source verifiers pass. Four AC setup/result/count/result-reader complete listings
were regenerated in `/tmp/inbox-*.elf` from the read-only ISO. No native code,
original trace or asset change; parent remains OPEN for listed integration,
mutation, other counters and runtime evidence.

2026-10-13 native original-Jump handoff: `TrioAttack` executes the unchanged
NormalJump stream through fresh precheck, approach/MarioJumpPosition and TimingA.
It now waits at actual CheckDamage8010a78c with failed first-hit flags100 or
successful first-hit flags20000. AttackerU2a8 retains the live original attack
root; field story11/result0 and all HP remain intact at this pending boundary.
The [used lifecycle follow-up](../behavior/battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#original-used-hit-death-and-variant-lifecycle-2026-10-13-follow-up)
answers source generic reaction/death/variant scripts. Next bind full damage
orchestration/calculators/direct application and these actual reaction owners,
including lethal400→script status27/deferred deletion and counter warning
admission. Keep rewards/effects/audio/barriers/transitive service and original
execution requests open. Native fresh precheck/RNG/AC/display adapters are
explicitly bounded in [runtime scope](../behavior/battle/NATIVE_ORIGINAL_JUMP_RUNTIME_BINDING.md);
do not turn this partial caller binding into complete battle fidelity.
