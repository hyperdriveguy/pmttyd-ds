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
