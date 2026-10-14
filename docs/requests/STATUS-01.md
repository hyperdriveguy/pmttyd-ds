# STATUS-01 — Status lifetime, event binding and turn ownership

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [x] Local unit-entry status/badge/move-count initialization — full
  [Entry](../behavior/battle/BATTLE_UNIT_ENTRY_COMPLETE.md),
  [pouch/status](../behavior/unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md) and
  [badge/count](../behavior/unit/UNIT_BADGE_COUNT_IMPORT.md) contracts.
- [ ] Concrete weapon/unit/event bindings reaching infliction, recovery and
  periodic turn controllers, later initialization mutations and original traces.
- [ ] Full main/battle event cadence, turn/phase state producers,
  remaining-count consumption and the `+20/+23/+24` writers.
- [ ] End-of-battle cleanup and status effect/message/sound/pose callback
  resource bindings.
- [ ] Original multi-turn traces (Slow/Fast, negative durations, Charge
  consumption, wakeup on lethal/live damage) and actor save/reload lifetime.
- [x] Unit status clear, pouch import/export and badge/count import —
  delivered ([initialization](../behavior/unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md),
  [badge import](../behavior/unit/UNIT_BADGE_COUNT_IMPORT.md)).

Portable storage/recovery, move-count helpers and guard/action predicates are
implemented natively; portable test fixtures and prototype turn timing are not
original evidence.

## Request text (verbatim)

Local [storage/recovery](../behavior/battle/BATTLE_STATUS_STORAGE.md),[move-count helpers](../behavior/battle/BATTLE_MOVE_COUNTS.md)
and guard/action predicates are implemented in `source/battle/battle_status.cpp`,with
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

## Answers and delivery notes (append-only history)

Native implementation breadcrumb (2026-10-12): commit7e43c5a implements
UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md in `source/battle/battle_unit_resources.cpp`,
with derived cases in `tests/battle_unit_resources_tests.cpp`. Native battle
start now invokes the recovered status/info clear. Pouch resource callbacks and
seven partner snapshots have explicit semantic service boundaries; they are not
yet native save/load, party-switch or turn owners. Existing requests for exact
export/import caller cadence, snapshot initialization and resource/equipment
ownership remain open. Preserve direct negative/shared status bytes on import;
only nonpositive partner HP overrides status27 duration. Do not infer original
disk-save layout or status lifetime from these portable helpers.

Further ID2 answer:[badge/count import](../behavior/unit/UNIT_BADGE_COUNT_IMPORT.md)
recovers full _EquipItem/BtlUnit_EquipItem,40-byte clear,all200 signed pouch
slots,exact numeric count maps/mode flags and wrapped multi-item threshold.
Setup uses clear+Mario scan flags3 and clear+partner scan5 before a later
pouch import;enemy saved/nonsaved held-item branches differ. Concrete Mario
setup/kind now proves count3 and original part array;items289/290 write the
power bonus bytes2ff/300. These are also partial DMG-01/AC-03 badge writer
answers,not initial player equipment or global guard eligibility proof.
[Snapshot export](../behavior/unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md) identifies
the matching SetParamToPouch body and exact current-resource/status write order;
swap/turn/battle-end export cadence and disk-save reconstruction remain open.
Use EQUIP-01..04 and INIT requirements rather than invented loadout/turn rules.

Partial answer from ID2:
[status clear and pouch import](../behavior/unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md)
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

### 2026-10-13 — Priority5 initialization portion checked

The priority1 [initialization delivery](../behavior/battle/DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md)
revalidates full local Entry/UnitSetup and existing status/badge/count imports.
Marked only that initialization portion complete. Entry/setup/general behavior
verifiers pass against the read-only ISO. Infliction/turn/end/presentation/save
binding and multi-turn execution remain OPEN. Priority4 AC-02's remaining full
lifecycle/display/macro bindings still need recovery; this shared prerequisite
check does not replace that work or establish turn cadence.
