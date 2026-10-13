# Underground trio drop material and entry pouch ownership

2026-10-13 documentation-only read-only US source delivery. Full
BattleInformationSetDropMaterial801141bc/344 listing
`/tmp/trio-end-BattleInformationSetDropMaterial.dis`; used producer listings
`/tmp/trio-drop-*`. This extends the [ending controller](UNDERGROUND_TRIO_VICTORY_CONTROLLER_AND_FIELD_RETURN.md)
whose native initial00→05→06 now stops at this real callback. Native validation
for that prior increment is separate. This contract does not grant rewards,
initialize the pouch on every battle or replace the original field scene.

Source SHA256 witnesses: full DropMaterial
`8e0b50cf7c56ee01d24110571aec43794743a954df65b3e279a4079f0bbb624a`;
used first five heart rows
`5465b8204a38ce22717f4acab0bbdf8e73964c6226807713786677a41c9d7d03`;
flower rows `56a4067801fa86d01e37d30137b664a3acf0df024b3d1dee529043d900bda262`;
Item Hog25-word table
`e3620926928f2e7527617f5773d8a80dc596822cd502c62e492e39d7c760cd3f`;
stg0_00_init `37e80f53a3f78528f01fb98aca90b3f672f24892c5ac87c36eb036f379624bc3`.

## Distinct setup, unit and pouch inputs

Call ABI is ordinary C `DropMaterial(I)`, no event operands/output/readiness.
Capture S=WORD(I+c), current global B, G=WORD(S18), heart table=G14,
flower table=G18, then BattleGetMarioPtr(B)→U. It reads **current battle unit**
HP/FP, not entry pouch values: S16 U10c/U108 and U112/U10e respectively.
Compute low32(current*100), signed divw by max, cap only above100. No lower
clamp, max0 guard, pouch lookup or HP/FP heal. Current damage and FP spending
must remain reflected in the unit when this callback runs.

Source import is [SetParamFromPouch/ReviseHpFp](../unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md).
Mario(kind222) imports P70 currentHP and P74 currentFP; baseP8e→U10a,
maxU108=P8e+5*U2f1; baseP90→U110,maxU10e=P90+5*U2f2, then upper-clamps
currentU10c/U112. Goombella(kind224) imports actual party slot1 HP from
P+14+6, base from+14+4, plus U2f1 max adjustment; its partner import does
not write FP base/max/current. Original kind80393bec and Mario80372afc
halfwords at+8/+a are10HP/5FP; ordinary unit Entry supplies that FP before
the party-only HP import. Do not initialize either maxFP to0.

Original pouchInit gives P70/72=10,P74/76=5,P8e=10,P90=5,levelP8a=1,
BP92/94=3 and party slots1..7 halfwords2/4/6=10. Its initial boots/hammer
98/99=0 precede original stage/field item producers; they are not the final
prologue entry equipment. See [fresh pouch](BATTLE_FRESH_PACKED_POUCH.md).
A live supplied entry pouch must preserve field damage, item/equipment and
story mutations; a source-derived explicit initial snapshot is a bounded
entry adapter until the persistent original field pouch is actually owned.

## Complete HP then FP drop passes and actual trio tables

Clear S10 before HP pass. Table records are four signed32 words:
thresholdPercent, outerChancePercent, itemCount, perItemChancePercent,
stride10 hex. Scan forward until threshold0 terminator or ratio<=threshold.
Skipped rows make no RNG call. First admitted row calls irand(100).
If draw>=outerChance, finish the pass. Otherwise repeat exactly itemCount
times: irand(100), increment S10 when draw<perItemChance. Loop decrement
tests nonzero, not positive; source count0 with admitted outer branch is
not protected and lies outside valid used tables. FP pass repeats the same
algorithm after clearing S14. All HP draws finish before any FP draw.

Actual group93518a30 (REL26460) G14→803702a0 heart and G18→80370f20 flower.
Upper cap100 means the fresh valid ratio selects within these first five
records; later table profiles are not part of this used pointer's path.

| Ratio upper threshold | Heart outer / count / per-item | Flower outer / count / per-item |
| --- | --- | --- |
| 20 | 70 /2 /50 | 40 /2 /40 |
| 30 | 60 /2 /50 | 40 /2 /40 |
| 50 | 50 /2 /40 | 40 /2 /40 |
| 80 | 40 /2 /40 | 40 /2 /40 |
| 100 | 30 /2 /30 | 30 /2 /40 |

Heart record addresses803702a0/b0/c0/d0/e0; flower80370f20/30/40/50/60.
At HP10/10,FP5/5 both ratios100 and both outer draws still occur. Total
fresh drop draws are3,5 or7 when badge314 absent: outerHP +optional2innerHP,
outerFP +optional2innerFP, final item-category draw. Zero damage/full resources
do not eliminate drop RNG, and story11 does not eliminate heart/flower drops.

## Complete item pass, including story11 and Item Hog

Capture signed G8/Gc/G10 as item-category weights A/B/C. Query story through
evtGetValue(NULL,f5de0180). Story<=19 forces A=0,B=0,C=100; later story
retains original group weights. Trio original G8/Gc=0,G10=100 already,
so this source story gate preserves its actual values without changing RNG.

Call pouchEquipCheckBadge800d3300(ID314/hex13a) unconditionally. Positive
count k multiplies A and B by k+1 (low32), preserving C. Request
irand(A+B+C) even when A=B=0. Subtract B first: negative→category1; else
subtract A: negative→category2; else category0. Then clear S0c.

Category1: inspect ORIGINAL first group unit item-table pointer G4→record2c.
Nonnull table sums signed16 entry+6 weights, advancing8 bytes; continue
while weight>0 OR itemword0!=0, stop only when both false. Positive sum
draws irand(sum), subtract weights until negative and writes that itemword
to S0c. Null/nonpositive table leaves0. This is the first unit's drop table,
not a live random enemy and not weight+4 used at encounter setup.

Category2 counts nonzero original selected item words S1c..38, eight slots.
Positive count draws irand(count); scan all eight slots in order, decrement
the choice only at nonzero slots, stop when it becomes negative and write
that slot to S0c. Source loops retain their exact count/order, not a set.
Category0 makes neither table/slot draw.

Finally if S0c still0, **query badge314 again**. Positive result draws
irand(25) and indexes N_item_hog_table802eea40, writing its word to S0c.
The25 actual words are decimal
152,152,152,162,148,154,153,132,135,139 followed by15 zeros. This means
badge fallback can still produce no item; do not filter zeros or redraw.
Fresh no-equipped-badge/story11 path performs the first category irand(100),
leaves item0 and queries the badge a second time without fallback RNG.

Badge query ordinary C signedID→count scans exactly200 signed halfwords at
P38a, with no sentinel/early exit. Existing packedPouchEquippedBadgeCount
matches this producer. Passing a partial zero P preserves a zero badge count
but does not establish authentic resource/equipment ownership.

## Encounter producers and preserved drop consumers

Original numeric setup4 initializes NPC+230 S through npcSetBattleInfo/
npcSetupBattleInfo: C14 weighted list selects actual group/stage with its
original RNG; item selection uses each group unit's record2c and weight+4,
writing S1c item and S3c source table pointers. Goomba80417900 is terminator
only (no draw). Spike8036f0b8 and Para8036f108 each have weights+4
200,5,0,10,5,5,2,2,0 (sum229) and mandatory irand(229); item0 has weight200.
Original selected zero is a possible result, not an unconditional fresh rule.
Frankly table is null. Full selection order/RNG protocol remains the
[field setup contract](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).

Btl_UnitSetup imports each selected item into U308, with S5c preferred when
nonnull and setting U1c40000000; otherwise S1c and its original reveal/item
record service. Death GetBackItem8010da28 moves retained U308 into first
zero S7c..98 and clears U308 only when U1c40000000 and itemnonnull. Source
full eight slots can write slot8; this is not a bounds clamp. Actual held-item
consumption and this flag's writers must remain preserved. Source default
native no-item encounter admission is an explicit restricted adapter until
these real setup item draws/initial fields are bound.

DropMaterial itself writes only S10/S14/S0c; it does not zero S7c returned
items, S5c retained items, selected S1c/tableS3c, stock coins/EXP, Iresult,
unit HP/FP/status or pouch. Preserve S through battle exit and field return.
Death StoreCoin adds to **I14**, while StoreExp adds to Bf00; they are not
S14 (flower count) or direct pouch deposits. Preserve those distinct owners.
Field normal enemy_common_dead_event8033efc0 asks actual getters in order:
coin, heart, flower, random item, returned-item entries, fixed item. It
requests item IDs79(hex coin),7b(heart),7c(flower), with item-entry mode10
for counted drops, mode9 for random/fixed item; no direct pouch coin grant.
Source39-command inventory SHA256
`b583500de5cfe03b83f9c4f964964130c0b1233a5f7274d850eaa287a41c25d3`
is `/tmp/trio-drop-fielddead.json`. The custom161-command trio continuation
restores/departs its three field actors; do not replace it with this generic
enemy death stream merely because drop fields exist.

Returned-item consumer npcGetBackItemEntry80043440 scans exactly8 S7c slots,
nonzero item→itemEntry(NULL,item,XYZ,mode11,-1,0), nonnull returned object
gets itemSetFlag100, then clear source slot regardless of allocation success.
fbatBattleMode also contains this exact returned-item loop after its own
result/death-script admission. The normal field getters, item-object ownership,
drop visuals/physics/collection and scene-specific field admission remain
separate required providers, not proof that trio drops are already displayed.

Each field getter evaluates NPC name via evtGetValue→evtNpcNameToPtr8004f5d8,
uses RAW arg1 output token, returns2, and does not clear the stored counts.

| Getter | Source mask/field and later augmentation |
| --- | --- |
| Coin8004e924/90 | S4 mask1→0; otherwise recursive NPC328 owner plus four NPC330 associated nodes and own U8 NPC315 sum through8004e9b4/78+8004ea2c/14 |
| Heart8004eb00/c0 | S4 mask2→0 without badge query; otherwise S10 and badge311 count:1→irand3+1 extra,>1→irand(count+3)+1 extra |
| Flower8004ea40/c0 | S4 mask4→0 without badge query; otherwise S14 and badge312, same augmentation |
| Item8004ebc0/74 | S4 mask8→0; otherwise S0c |
| FixedItem8004ec34/58 | S8 directly, no mask/RNG |

These are a second field-side badge/RNG domain after battle DropMaterial,
not draws to pre-consume at the end callback. Generic coin getter sums the
actual NPC hierarchy's computed bytes, not the original encounter kind
coin table. fbatBattleMode result1 copies low8 I14→encountered NPC315 before
its own multiplier/admission logic (fortune F54c, badge313, S4 mask100).
Corresponding scope listings `/tmp/trio-drop-evt_npc_get_drop_*.dis` and
`trio-drop-coincount.dis` identify the exact fields/calls; full hierarchy,
dead-stream admission and collection remain separate lifecycle dependencies.

## Source equipment producers and scoped prologue entry snapshot

Original stg0_00_init800ee688/70 calls in order: pouchGetPtr (return unused),
pouchInit, pouchGetItem6, pouchGetItem32(hex), pouchGetItem9, pouchSetCoin100,
pouchReviseMarioParam, then separate source event/world initialization
801fad00,80229bcc,801fb068,GP20/24=0,marioSetCaseType0 and8013c110.
stage0_evt80338a78 is32-byte records; its first record story0 points to
this callback at+1c, strings STG0_00/Prologue/aaa_00/prologue. This establishes
the authored initial-stage provider; actual title/new-game invocation/lifetime
is a distinct caller binding. Do not invoke the full initializer during
ordinary battle entry or assume it runs on every story11 load.
The scoped aligned direct DOL census finds eventStgDtPtr800ebbc0 callers
only in seq_e3Main at80252bf0/2c64/2cb8; it does not prove this stage callback
is the ordinary title/new-game producer. marioInit directly calls pouchInit
at8005b620, then later repairs ordinary hammer item9. Actual ordinary boots6
grant/lifecycle beyond the authored initial-stage snapshot remains requested.

Used pouchGetItem800d50cc mutation branches:

- Item6 sets P98=1 before insertion; item9 sets P99=1 before insertion.
- Item0x32 has no additional scalar write.
- All three then use key-item IDs1..120, capacity121 S16 halfwords atPa0.
  Scan from slot120 down to0 for first zero, shift preceding halfwords right
  into that hole, put the new item at slot0, return1. No duplicate check or
  sort. Full array returns0; prior equipment-byte write remains in effect.
- Fresh sequential6,0x32,9 therefore yields keyitems[9,0x32,6,0...] and tiers1/1.

marioInit8005ba18 checks item9 and grants it if absent; marioReInit has the
same ordinary hammer repair. Getter pouchGetJumpLv800d3b20 recomputes P98
from highest present key8→3,7→2,6→1,else0; HammerLv800d3aa0 similarly
keys11→3,10→2,9→1,else0. They query original inventory, then store and return
signed8 tier; tier1 is not an invented battle start constant.

ReviseMarioParam800d35a8 uses actual global P; P8e→max72,P90→max76;
scans200 equipped S16 IDs38a, +5HP for ID278/+5FP for ID280. Upper-clamp
current70/74 only; no lower clamp/heal. P94→availableBP92, then for each
of200 slots subtract signed8 itemData803108a8+40*ID+1c (including ID0,
whose original cost byte0). Each store narrows16-bit. Fresh zero equipment
preserves original10HP/5FP/3BP. Original party kind resources and entry import
must be bound separately as described above.

An explicit supplied source prologue entry snapshot may apply these scoped
new-game field mutations once, then preserve its actual live HP/FP and field
state between encounters. Current native Progress does not by itself own
the original full packed pouch. Persistent Game/save/field construction,
real story11 inventory/equipment and encounter RNG selection remain explicit
requests. Do not hide a missing owner by resetting HP/FP, inventory or tiers.

Validation: complete read-only source body/disassemblies, actual source data
rows/item tables and linked import/export contracts; `git diff --check`.
No native code edits, original execution, build rerun or commit.

## Native supplied-pouch and drop implementation

source/battle_drop_material.cpp now implements the complete valid source
algorithm above. Native TrioAttack executes the actual callback at end06,
using current Mario HP/FP and the unchanged packed group/table records. The
fresh native source scenario reaches08 with actual heart/flower/item outputs
and original10EXP prepared before missing icon initialization802149ec.
The separate prologue_pouch module implements the explicit source initial-stage
pouch snapshot and native fresh-file ownership; supplied battle entry preserves
current HP/FP/partnerHP and imports original kind FP plus initialized snapshots.
This is an authored snapshot adapter, not recovered original title scheduling.
Legacy checkpoint serialization, later field/harbor/equipment ownership,
encounter-selected item/shared RNG producers, actual field drops and complete
reward/display/fade/export/field return remain open. See the dated PROGRESS
entry for exact141/103/build verification. No new visible victory capture.
