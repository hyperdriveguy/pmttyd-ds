# Weapon-table ordinary Jump/Hammer and badge rows

Companion for complete [builder](WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md),
US G8ME01 revision0. Retained B/T/P/M,current stack count N,and Q=M+28*N
use that contract's layout. [Source bindings](WEAPON_TABLE_SOURCE_BINDINGS_AND_ICON_LEAVES.md)
pin the original arrays and data;[verifier](../../../tools/verify_weapon_table_builder.py)
checks every instruction. Static evidence only,not a runtime inventory snapshot.

## Ordinary level-admitted rows

T0 checks signed byte P+0x98 independently before thresholds1,2,3. T1 checks
signed byte P+0x99 likewise. Each admitted row emits in threshold order:

| T | Threshold | Original weapon address | Icon request |
| --- | --- | --- | --- |
| 0 | >=1 | 803538a8 | _getJumpIconId() |
| 0 | >=2 | 80353968 | _getJumpIconId() |
| 0 | >=3 | 80353ae8 | _getJumpIconId() |
| 1 | >=1 | 80353de8 | _getHammerIconId() |
| 1 | >=2 | 80353ea8 | _getHammerIconId() |
| 1 | >=3 | 80354028 | _getHammerIconId() |

There is no >=4 additional row. Later admission reads can differ after icon/
message services. The icon helper queries a FRESH pouch pointer,not retained
P;thus admission level and icon level need not be the same service-visible read.
Do not snapshot one level for the whole routine or derive icon from row ordinal.

For each admitted row,store source ordinal=-1,itemId0,weapon,enabled0,
reason0;request the relevant icon helper,store low16 return as icon;resolve
name through the common weapon-key/ItemData route,store name. Then increment
CURRENT N. The first >=1 row stores source ordinal at fixed M90 (N starts0),
while all other fields derive row from reloaded N. The remaining ordinary rows
use reloaded N for all stores. Partial-row/padding rules still apply.

## First-row weapon replacement occurs after name/icon

Before incrementing N for the first >=1 row ONLY:

- Jump reads WORD(B+0x0ef4),tests mask0x09000000.
- Hammer reads that word at its own corresponding point,tests0x0a000000.

If mask is nonzero,read WORD(B+0x424). Exactly-1 skips replacement.
Otherwise request BattleGetUnitPtr(B,thatCurrentId),then
BtlUnit_GetData(returnedUnit,68) for Jump or key69 for Hammer. Store returned
word as the row's weapon pointer,derived from CURRENT N. No returned-unit/
weapon null check;do not reset count or reject the row. Crucially,there is no
new icon/name lookup after replacement:the previously established presentation
remains,while later eligibility/selection uses the new pointer. Levels2/3 do
not perform this override. Replacement must not be moved ahead of presentation.
Unit-data lookup lifecycle and original override source bindings remain separate.

## Badge scan ordering and row admission

After ordinary rows,T0 scans fixed eight words from80416d80;T1 scans fixed
nine words from80416da0. Each iteration reads its current source word as badge
ID,requests pouchEquipCheckBadge(id),and admits ONE row iff SIGNED return>0.
A larger positive equipped count does not create duplicate rows. Zero/negative
omits the row. Arrays are read per iteration,not copied as one immutable host
snapshot;original initial values are pinned in the source companion.

For an admitted badge,derive retained ItemData address
I=wrap32(0x803108a8+40*id). Store source-array ordinal at Q90,retained badge
ID at Q94,read WORD(I+0x24) and store it as weapon Q80,then enabled0/reason0.
Reload row weapon and read WORD(weapon+8):nonzero chooses icon U16(I+0x20);
zero chooses U16(weapon+4). Store halfword Q8c. There is no null weapon guard
before reading+8. A badge weapon's itemId can differ from the badge ID stored
in the descriptor;the two fields serve different name/icon selection roles.

Resolve name through the common weapon-key/fallback-ItemData route AFTER
icon storage,store name,then increment CURRENT N. Increment source ordinal
and source-byte offset by1 and4,respectively,regardless of admission. T0 always
executes eight badge checks,T1 nine,unless a service faults;no early row-count
limit applies to these loops. With stable source inputs and positive badge
checks,local ordinary+badge maxima are11 Jump and12 Hammer rows. These are
local conditional counts,not global runtime menu bounds or party callback limits.

## Implementation checks

Use signed level/check returns,three independent admission reads,fresh pouch
icon calls,first-row late override,one-row-per-positive badge result,source
ordinal versus itemId distinction,icon branching on weapon+8,partial padding,
and the precise store/request/increment order. Reuse original keys and source
weapon definitions;do not generate labels or infer complete weapon behavior
from the pinned leading fields. Query reachability and original execution are
not closed by these local table paths.
