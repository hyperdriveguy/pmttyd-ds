# DMG-02 — Direct application service internals and native bindings

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [ ] Complete `BodyPartsId`/`GetPartsPtr`, `GetHitPos`/`GetPos`/`GetHeight`/
  `GetBelong` plus the `field114`/`pointer144+10` schemas and lifetimes; the
  original `belong` getter needs closure.
- [ ] Concrete boss/Mario/partner part attributes, kind `9c/a0/a4/a8` sound
  pointers and the original effect/macro resources.
- [ ] Action-record `AddCount`/`AddPoint` saturation and storage, audience
  helper state/RNG effects, hit-event dispatch inputs and all reachable
  death/revival/recovery scripts.
- [ ] Shared FP owner lookup/update reconstruction and pouch/item151
  consumption (no invented revival).
- [ ] Exact numerical/rounding and position-query/service request order with
  original traces (suppression, zero/absorption, simultaneous HP/FP, Pity at
  full FP, wakeup on lethal/Instant Kill, pattern25 gates).
- [x] Unit/part position queries and movement sound core — delivered
  ([positions](../behavior/battle/BATTLE_UNIT_PART_POSITION_QUERIES.md),
  [movement sound](../behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md)).

## Request text (verbatim)

The local [direct application](../behavior/battle/BATTLE_DAMAGE_APPLICATION.md) is now
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

## Answers and delivery notes (append-only history)

Partial ID2 answer:[unit/part position queries](../behavior/battle/BATTLE_UNIT_PART_POSITION_QUERIES.md)
close home,part offset/world and scaled hit leaves,BattleGetUnitPartsPtr and the
event HitPos wrapper. Absolute part flag10000000 differs from owner-position
plus offset;relative X uses owner189,while scaled hit X uses captured side-table
factor. Two offset triplets scale through unit114 with ordered fused operations;
event outputs truncate to signed integer with raw destination sentinel checks.
No source writer/actual original scene coordinate is inferred from the leaf.
[Movement sound core](../behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md) closes retained
voice/periodic alternation/stop and identifies only record18 as retained handle;
keys0/4 are playback inputs. Original kind sounds,source writers,field144 schema,
height/belong/action-record/audience/revival/VM/resource helpers and execution
traces remain open. POS/MOVE-SE requirements provide scoped implementation cases.
