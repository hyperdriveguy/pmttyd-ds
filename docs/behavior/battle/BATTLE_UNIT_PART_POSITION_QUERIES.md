# Unit home, part world and scaled hit-position queries

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete GetHomePos801289b0/1c,GetPartsOffsetPos801283bc/1c,
GetPartsWorldPos80128b10/e0,GetHitPos801289ec/124,
BattleGetUnitPartsPtr800f8874/40 and event GetHitPos801120a8/11c.
This closes used local position services for JUMP-01/DMG-02. Source/unit/part
lifetime,mutation and original executed positions remain separate obligations.

## Ordered unit/part leaf getters

GetHomePos(U,outX,outY,outZ) loads binary32 U30,writes outX;loads U34,writes
outY;loads U38,writes outZ. GetPartsOffsetPos(P,outputs) similarly uses part
24/28/2c. Both are ordered loads/stores,not an upfront vector snapshot;aliased
outputs or outputs into source objects can affect later reads. No local null/
alias validation or unit/part ID resolution occurs.

[Visible GetPos](BATTLE_JUMP_EVENT_CALLBACKS.md) uses distinct U3c/40/44.
Home and visible coordinates must not be merged just because an entry route
initializes both from the same values.

## Part world position

GetPartsWorldPos receives P and three output pointers. Read word P1ac and
owner pointer P4ec before dispatch. If part attributes have mask0x10000000,
copy P18/1c/20 directly to outputs in X,Y,Z order and return. The owner pointer
is read,but its unit position is not queried on this route.

Otherwise:

1. GetPos(owner,&unitX,&unitY,&unitZ) into separate stack temporaries.
2. GetPartsOffsetPos(P,&offX,&offY,&offZ) into separate temporaries.
3. Read signed byte owner189 and convert it to binary32 direction F.
4. Write outX=singleFMA(offX,F,unitX),outY=single(unitY+offY),
   outZ=single(unitZ+offZ),in that order.

There is no unit114 scale multiplication,rotation/heading transform or floor
query in this body. Owner189 is not necessarily the current movement-direction
byte188 or the side table factor used by the next hit-position layer. Owner
and part pointers are not null-checked. Keep absolute and relative branches
distinct;do not apply relative offsets again to a part already using absolute18.

## Hit-position layers,scale and two X signs

GetHitPos(U,P,outX,outY,outZ) first reads signed unit belong byte0xc and
captures S8(BattleWork+8*belong+0xa) as factor B. The work lookup has no local
index bounds check. Call GetPartsWorldPos(P,outputs),then apply two ordered
offset triplets:

| Triplet | Part X/Y/Z offsets | Scaling and addition |
| --- | --- | --- |
| First | 170/174/178 | X:round32(partX*U114),then singleFMA(B,product,currentOutX);Y:rounded product,then singleFMA(1,product,currentOutY);Z:singleFMA(partZ,U114,currentOutZ) |
| Second | 17c/180/184 | Same operations and X factor B,using outputs already updated by first triplet |

U114 is loaded again for every axis in each triplet. The Y coefficient is
original binary32 **1.0** at804226a8;it is not zero or a vertical side flip.
X's product is rounded before its B multiply/add;Z's multiplication and output
addition are fused directly. Do not reorder the two triplets or evaluate all
offsets from one cached scale/output vector when aliasing matters.

GetPartsWorldPos uses P's retained owner for base coordinates and owner189,
whereas this function uses its explicit U argument for belong/114. There is no
check that those units are equal. The two sign factors remain independent:
relative part offset X uses owner189;hit offset X uses captured side-table B.
The scale applies only in the hit-offset layer,not retrospectively to all world
coordinates. Numerical exceptional/FPSCR behavior and field114 writers remain
dependencies even though the local operations are complete.

## Registry part helper and event wrapper

BattleGetUnitPartsPtr(unitID,partID) loads shared BattleWork,fetches the unit,
then calls BtlUnit_GetPartsPtr(unit,partID). It returns that pointer with no extra
null/bounds/ID handling. The [part lookup contract](../unit/UNIT_PART_DEFENSE_AND_LIFETIME.md)
still applies:negative IDs select head,and a single part returns head for any
nonnegative requested ID;multi-part missing IDs return null.

The event GetHitPos wrapper resolves/translates unit operand0,then resolves part
operand1. Fetch unit using shared work,then separately request
BattleGetUnitPartsPtr(translatedID,resolvedPartID),which reloads shared work.
Capture output operands2/3/4 raw and call GetHitPos into separate float
temporaries. For each X,Y,Z in order:if that **raw output operand** equals
0xf1194d80,skip it;otherwise fctiwz the corresponding float and evtSetValue
the signed32 result. Return2,with no first-call or polling gate.

Outputs are **integer coordinates**,not evtSetFloat values. For a derived
finite coordinate1.9,the output is1;for-1.9 it is-1. Output destinations can
alias;the last written axis wins. Skipping an event output does not skip the
underlying full coordinate query. Unit/part translation and GetPartsPtr failures
are not locally repaired here.

This event wrapper accounts for the ordinary Jump's integer target coordinates
and ADD(L2,5) before Mario motion. The direct takeoff callback uses floating
GetHitPos itself and truncates only its final outputs;these are distinct routes.
See [takeoff selection](BATTLE_JUMP_TAKEOFF_POSITION.md).

## Implementation requirements and remaining evidence

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| POS-01 | Keep home/visible/part absolute/relative sources and read order | Home30 differs from visible3c;attribute10000000 uses P18 directly;relative X uses owner189 |
| POS-02 | Preserve both scaled offset layers and separate sign sources | Owner189=-1 and side B=1 produce different signs at different layers;U114 rereads and fused Z ordering retained |
| POS-03 | Preserve wrapper integer outputs/raw sentinels and part helper | Output-1.9 becomes-1;raw sentinel skips store only;single-part nonmatching ID still reaches head |

tools/verify_jump_preparation.py fingerprints all complete bodies and selected
instructions/constants. Original part construction copies and source records
are tracked separately;this file does not claim actual Crump/Mario hit positions
never change after initialization. Remaining DMG-02 services include height/
belong,specific damage/sound tables,field114/source/world-position writers,
event/data/pose lifetime and original executed suppression/position traces.
ISO is read-only;original data and inspection reports stay outside source history.
