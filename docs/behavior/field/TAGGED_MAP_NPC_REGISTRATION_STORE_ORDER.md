# Tagged-map NPC registration store order

US G8ME01 disc0 revision0, static read-only DOL and m/gor_02/s evidence for
[EVT-01](../../requests/EVT-01.md). This refines the
[map constructor contract](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md)
and [NPC setup service](NPC_SETUP_ORDERED_INSTALLATION_AND_RETAINED_PROVIDER_FRONTIER.md).
The source map producer and later npcEnt installation are separate phases.

Complete fresh disassembly: `artifacts/tagged-map-npc-setupDataBase.txt`.
setupDataBase80084f24/668 SHA256 is
02b8acf7fce8e711405179cfb88e87e8a7cb8cde7906ec119977ce62be3c8594.
Symbols: `artifacts/reference/us_symbols.csv`. Original tagged data inspection
copy: `local-assets/field-actors/gor_02.s` (Shift-JIS).

## Parser inputs and upstream gate

setupDataBase captures function arg0 as the area-name pointer in r30 and arg1
as the map-name pointer. Its source frame is allocated by stwu SP,-1d0.
The following are offsets from that actual frame SP, not fields of an
original allocated input structure:

| Tag | SP output | parseTagGet1 mode | Call address |
|---|---|---|---|
| name | 160 | 0 | 8008523c |
| tribe | 120 | 0 | 8008524c |
| position | 40/44/48 | 3 | 8008525c |
| dir | 18 | 2 | 8008526c |
| aitype | e0 | 0 | 8008527c |
| territory_type | a0 | 0 | 8008528c |
| territory_base | 34/38/3c | 3 | 8008529c |
| territory_loiter | 28/2c/30 | 3 | 800852ac |
| territory_homing | 1c/20/24 | 3 | 800852bc |
| search_range | 14 | 2 | 800852cc |
| search_angle | 10 | 2 | 800852dc |
| homing_range | c | 2 | 800852ec |
| homing_angle | 8 | 2 | 800852fc |
| btl_setup_no | 60 | 0 | 8008530c |

Each call supplies tag pointer in r3, mode in r4 and SP destination in r5.
parsePopNext80084cec is called at85310 before the registration gate. Source
NPC ordinal r26 starts0 at85204 and increments at85560 even when the gate
skips a parsed row. The gate's enabled boolean comes from an earlier map
definition flag; when enabled, it reads current map-history record+10 and
tests source slw(1,low8(ordinal)). A set bit skips construction to85560.
This is saved field/map ownership, not an invented visibility mask. Native
code must supply its actual provenance or keep the encompassing flow unbound.

After each ordinal increment, parsePush80084d2c("<Npc>") is called at85568.
Nonzero repeats parsing at85230; final zero calls parsePop80084d14 at85574.
All gor02 tags listed here are present; missing-tag parser semantics and
uninitialized frame bytes must not become assumed zero defaults for new maps.

## Constructor, roots and dimensions

At85334 pass SP120 to npcGetTribe8004b828, capture returned descriptor T.
Read T+4 freshly at85344; call npcEntry8004b894(SP160,modelName) at85348.
Ignore its returned index, perform npcNameToPtr800499e0(SP160) at85350,
and retain that actor N. Duplicate-name behavior therefore follows original
first active named lookup, not necessarily the newly allocated slot.

1. Store low8 ordinal to N+314 at85358.
2. Read N+0, OR4 and store at85370.
3. Read SP40 X and SP44 Y before either store; write N+8c then N+90.
   Freshly read SP48 Z, write N+94.
4. Read single SP18, store N+144; store captured T to N+28.
5. Read single T+28, store N+14c; read single T+2c, store N+150.
6. Freshly read N+0, clear bit2 and store at853b0.

There is no previous-root copy in this span. npcEntry previously initialized
current and previous from the original zero vector. Later source owners may
update history. Map construction leaves active1/pending4, with query2 clear;
do not acknowledge later query readiness here.

## AI and territory

**Address correction:** the AI table is npc_ai_type_table80338270, not
80348270. Source lis8034 plus signed addi8270 yields80338270. This corrects
the earlier high-level registration prose's arithmetic transcription.

At853b4 read AI row+0; null ends the scan. Otherwise strcmp(rowName,SPe0)
at853c4; mismatch advances0x24 and repeats. First match performs:

| Ordered read | Store |
|---|---|
| N+0, then AI+4 | N+0 = captured flags OR captured mask |
| AI+8 | N+120 |
| AI+c | N+124 |
| AI+10 | N+12c |
| AI+14 | N+130 |
| AI+18 | N+134 |
| AI+1c | N+138 |
| AI+20 | N+13c |

Every metadata load/store is sequential. No match leaves those fields as
they currently stand. A matching all-zero metadata row still performs its
actual writes; it is not equivalent to skipping the AI owner.

Call dbGetDefData80043864(table80416580,SPa0) at8542c and store returned
value to N+1f8. This helper scans8-byte name/value entries, returns the first
exact match, and returns0 on missing name. Original territory entries are:
NOTHING0 at80416580, CIRCLE1 at80416588, SQUARE2 at80416590, null/zero at
80416598; names include the full NPC_TERRITORY_TYPE_ prefix.

At85434 capture integer1; at85438 load single0 from804207b4. These remain
captured through the following stores. In each triple, X and Y are loaded
before either store, then Z is freshly loaded/stored:

| Ordered source triple | Destination triple |
|---|---|
| SP34,SP38; SP3c | N+1fc,N+200; N+204 |
| SP28,SP2c; SP30 | N+208,N+20c; N+210 |
| SP1c,SP20; SP24 | N+214,N+218; N+21c |

Then separately read/store singles SP14→N+220, SP10→N+224,
SPc→N+228, SP8→N+22c. These are parsed source values, not native defaults.

## Sound, battle and initial pose

The captured tribe T remains the owner even if an injected service changes
N+28. Sequentially read/store T+40→N+1e0, T+44→N+1e4,
T+4c→N+1ec, T+50→N+1f0; read unsigned half T+48→N+1e8.
Store captured integer1 as half N+1d8; store captured float0→N+1dc.

areaDataPtr800391e0 receives the original captured area argument and is
called unconditionally at854dc. Retain its result A. At854ec compare
SP60 battle name with original BTLNO_INVALID802c3b10. Equal preserves
captured battle index-1; unequal freshly loads A+c and calls
dbGetDefData(A.numbers,SP60) at85500, retaining its result (missing→0).
Then npcSetBattleInfo80043d80(N,index) at85510. Sentinel handling does not
skip the earlier area lookup. Numeric setup itself may freshly resolve GP;
do not collapse its owner with this captured A lookup.

After that service, freshly read T+10; if nonnull strcpy8026d5fc(N+4c,string).
Freshly read T+14; conditionally strcpy(N+6c,string). Freshly read T+8;
if nonnull strcpy(N+2c,string), then freshly read N+104 and call
animPoseSetAnim80041170(handle,N+2c,1), with those arguments in r3/r4/r5.
The descriptor and actor pointers are retained, but late fields/handle are
not snapshots from before battle/pose services.

## Actual gor02 records

All fifteen source records use AI「なし」, territory
NPC_TERRITORY_TYPE_NOTHING, all three territory vectors(0,0,0), all four
range/angle values0 and battle name BTLNO_INVALID. AI「なし」is the first
row80338270 with name80421d34 and eight zero metadata words. Therefore
map construction calls the actual battle-1 clearing service, with no RNG
draws from that branch; subsequent npcEnt battle0 installation is separate.

| Ordinal | Actor | Tribe | XYZ | Direction |
|---|---|---|---|---|
| 0 | パワーアップ屋 | パワーアップ屋 | -320,0,-205 | 270 |
| 1 | クリハカセ | クリハカセ | 0,-1000,0 | 270 |
| 2 | 情報屋完璧 | 情報屋完璧 | -445,175,-180 | 90 |
| 3 | 盗賊団１ | 門番 | 0,-1000,0 | 270 |
| 4 | 盗賊団２ | 門番 | 0,-1000,0 | 270 |
| 5 | 盗賊団親分 | 盗賊団親分 | -125,0,-540 | 90 |
| 6 | サンダース | サンダース | 0,-1000,0 | 270 |
| 7 | クリボー４ | NPCクリボー | -375,0,150 | 270 |
| 8 | チューさん３ | チューさん | 150,0,-400 | 270 |
| 9 | ロテン３ | ロテン緑 | 200,0,200 | 270 |
| 10 | ボロドー１ | 盗賊 | 110,0,160 | 270 |
| 11 | ボロドー２ | 盗賊 | 0,-1000,0 | 270 |
| 12 | クラガリさん | クラガリさん | -335,0,-335 | 270 |
| 13 | ルイージ | ルイージ | 0,-1000,0 | 90 |
| 14 | 盗賊団３ | 門番 | 0,-1000,0 | 90 |

This tagged order differs from npcEnt table order. Name binding, byte314
and skip-bit consumption must preserve the tagged order. Initial below-map
coordinates are source values, not evidence that registration may omit an
actor. Their source init scripts subsequently own conditional placement.

## Implementation boundary

A portable native input view may retain these parsed words/string addresses
at explicit guest identities; it must label that layout as an adapter rather
than an original allocated structure. Keep source ordered reads for provider
aliases, captured T/N/A identity, actual battle clear and pose services.
Retain partial effects when a provider is unavailable, or admit the full
synchronous domain before construction. Original parser state, prior map
history/gate, allocation/resource readiness, stable registered actor lifetime,
later Main/query flags, NPC setup and GPU publication remain separate owners.
This source contract does not claim their execution or native completion.
