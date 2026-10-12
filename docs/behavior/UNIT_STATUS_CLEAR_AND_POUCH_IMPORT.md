# Unit status clearing, pouch import and resource revision

US G8ME01 revision 0; recovered 2026-10-12 by documentation agent ID 2.
Evidence B: complete BtlUnit_ClearStatus0x80127b58/0x54,
BtlUnit_SetParamFromPouch0x80127134/0x118,BtlUnit_ReviseHpFp0x80126fe8/0x14c,
BattleTransPartyId0x800f8444/0xf4 and
BattleStatusChangeInfoWorkInit0x80195d34/0x48. Entry call sites are independently
pinned. No original execution or save-record reconstruction is asserted.

## Entry order and status clear

The [entry contract](UNIT_ENTRY_STATS_AND_SPAWN.md) establishes publication,
initial kind HP/FP stores and later asynchronous init registration. At
0x80129330, entry calls BtlUnit_ClearStatus(U), before part construction.
At0x80129634, after parts and display-registration request, entry calls
BtlUnit_SetParamFromPouch(U), before init-event registration. Therefore the
earlier kind-resource stores/status clear do not necessarily establish final
actor state. Other unit entry and display/pose service internals remain partial.

ClearStatus does the following in order:

1. Zero **30 bytes** at U+0x118..U+0x135 inclusive through memset.
2. Call BtlUnit_SetStatus(U,16,1,-99). The [Charge setter](BATTLE_STATUS_STORAGE.md)
   adds -99 to the just-cleared unsigned Charge byte, narrows to byte0x9d,
   interprets it as negative and clears it. Duration1 is unused on this route.
   The return is not consumed. This call does not leave Charge duration1 or
   strength-99 stored.
3. Call BattleStatusChangeInfoWorkInit(U).

The info initializer updates six records at U+0xae8+12*i, i=0..5. It sets
byte+0 to0xff, bytes+1,+2,+3 to0, and word+4 to0. **Word+8 of each record is
untouched.** This is a field initializer, not a72-byte memset; reused records
retain those last words. Entry previously invokes the same info initializer
at0x801290b4, and the initial whole-unit memset clears otherwise retained bytes
on this construction route. Other ClearStatus callers may have existing records.

ClearStatus does not locally zero badge bytes,move counters,unit flags at138/
27c,pose handles or status-effect objects. No turn decrement,RNG,immunity test
or persistence write occurs in these complete local bodies. Effect cleanup and
other lifecycle callers remain separate owners.

## Shared inputs and resource widths

SetParamFromPouch captures BattleWork from r13+0x1c70, calls pouchGetPtr and
retains its pointer, then reads signed unit kind word U+8. Even an unrelated
enemy kind requests the pouch pointer before its later no-op return.
Resource reads are signed halfwords;resource stores retain low16 bits. No local
null checks exist. ReviseHpFp makes its own pouch-pointer request and kind read,
so callers must not silently merge those service calls or snapshots.

| Runtime field | Distinct role in these bodies |
| --- | --- |
| U+0x10a / U+0x108 / U+0x10c | Base HP / revised max HP / current HP |
| U+0x110 / U+0x10e / U+0x112 | Base FP / revised max FP / current FP |
| U+0x2f1 / U+0x2f2 | Unsigned count bytes contributing5 per copy to HP/FP maximum |

The original kind stores fill these resource copies before this later import.
Keep the copies distinct when implementing a semantic state object. The
[pouch resource contract](STATE_AND_INVENTORY.md) establishes pouch offsets,
but counts' equipment aggregation and load/save producers are not closed here.

## Mario kind222 import

For kind222, SetParamFromPouch:

1. Read S16(pouch+0x70),store U+0x10c (current HP).
2. Read S16(pouch+0x74),store U+0x112 (current FP).
3. Call ReviseHpFp(U),then return without importing a battle status snapshot
   or testing current HP for status27.

ReviseHpFp's Mario branch then executes in this order:

1. Store S16(pouch+0x8e) at U+0x10a.
2. Read U8(U+0x2f1),**reread** S16(pouch+0x8e),add5*count and store at U+0x108.
3. Store S16(pouch+0x90) at U+0x110.
4. Read U8(U+0x2f2),reread S16(pouch+0x90),add5*count and store at U+0x10e.
5. Compare signed current HP with signed reloaded max HP;if current>max,
   store max into current HP.
6. Likewise upper-cap current FP to its reloaded signed maximum.

There is no lower clamp and no heal to the new maximum. Additions narrow to
halfword **before** signed comparisons. Neither function writes revised maximum
or current resources back to the pouch. Do not collapse current112 with base110
or assume a negative resource is automatically repaired here.

## Partner kinds224..230 import and battle snapshot

For partner kinds, translate kind through BattleTransPartyId and use pouch
record `R=pouch+14*translatedIndex`. The mapping is not simply kind-223:

| Returned index | Accepted input kind/alias words |
| --- | --- |
| 1 | 80,137,224 |
| 2 | 81,138,225 |
| 3 | 142,229 |
| 4 | 82,139,226 |
| 5 | 83,140,227 |
| 6 | 141,228 |
| 7 | 143,230 |
| 0 | 0 and every other signed32 input |

The leaf mapper has no mutation,RNG or narrowing. Only kinds224..230 reach
its use in these unit resource bodies;other aliases are documented as leaf
behavior,not evidence of additional partner actor bindings.

SetParamFromPouch first reads S16(R+6) into current HP and calls ReviseHpFp.
The latter translates the kind again,stores S16(R+4) into base HP,then computes
max HP from **reread R+4 +5*U8(U+0x2f1)**. It rereads R+6 into current HP and
upper-caps it against signed narrowed max HP. This partner branch does **not**
write any FP base/max/current field or apply count2f2.

After revision, SetParamFromPouch **rereads unit kind**, forms battle snapshot
`B=BattleWork+0x162a0+44*(kind-224)` and imports:

| Snapshot source | Runtime destination | Access |
| --- | --- | --- |
| B+0 | U+0x114 | One binary32 load/store |
| B+4..B+0x21 | U+0x118..U+0x135 | Exactly30 bytes,seven word copies then a halfword |
| B+0x24 | U+0x138 | Word,loaded/stored before status-byte copies |
| B+0x28 | U+0x13c | Unsigned halfword copy |
| B+0x2a | U+0x13e | Unsigned halfword copy |

Snapshot bytes+0x22/+0x23 are not copied by this body. Status restoration copies
bytes directly;it does not call setters for each imported effect. Thus byte119,
negative durations,shared sign pairs and other retained combinations are not
normalized by status setter rules on this route. The matching export leaf below
now identifies one writer;valid state and caller/battle/field/save lifetime
remain open. It is **BattleWork memory**,not
established original on-disk save data. Entry's earlier clear can be overwritten
by this partner restore.

Finally read S16(U+0x10c). If <=0,request SetStatus(U,27,1,1);if >0,do nothing.
ID27 stores duration1 at135;strength is ignored by that local setter. This
overwrites only that imported duration byte on the nonpositive-HP route. It
does not itself spawn a death/revival event,consume an item or clean up the unit.
The exact kind reread and source-copy order must survive implementation.

## Other-kind revision versus import no-op

For kinds outside222/224..230, SetParamFromPouch returns after its pointer/kind
reads,with no local resource/status writes and no ReviseHpFp call. Thus it does
not replace original boss HP5/FP0 from kind entry with Mario pouch resources.

However, **a direct ReviseHpFp call** on another kind has a distinct body:
compute max HP=base HP+5*U8(2f1),narrow and upper-cap current HP;then max
FP=base FP+5*U8(2f2),narrow and upper-cap current FP. It leaves base copies
unchanged and has no lower clamp. Do not make the import no-op imply revision
no-op or omit this callable enemy resource behavior.

## Matching export leaf and unresolved caller lifetime

BtlUnit_SetParamToPouch0x80126f00/0xe8 is now fully recovered alongside badge
setup research. Capture BattleWork,call pouchGetPtr,read kind U+8. Kind222 copies
signed current HP10c to pouch70,then signed current FP112 to pouch74. No maxima,
status,clamp,resource setter or snapshot write occurs in this Mario branch.

Kinds224..230 call BattleTransPartyId,write signed current HP10c to pouch+
14*translatedIndex+6,then **reread kind** and form the same44-byte BattleWork
snapshot B used by import. Copy binary32 U114 to B0,then word U138 to B24,
then exactly30 bytes U118..135 to B4..21 (seven words and a halfword),then
halfwords U13c/13e to B28/2a. Snapshot gap22/23 remains untouched. No partner
FP write or status normalization occurs. Other kinds only perform entry reads
and return with no local writes. Unit/pouch pointers are not null-checked.

This identifies **one concrete snapshot writer**,so the import producer is no
longer entirely unnamed. It does not establish which switching/turn/battle-end
callers invoke export,when BattleWork snapshots reset,or how disk saves capture
pouch/battle memory. Aliasing/service order must remain explicit. Add paired
host cases for Mario current-only export,partner direct status-byte roundtrip
with gap retention,and enemy no-op;original reentry/lifetime traces are still
required. Do not label this function an original disk-save implementation.

## Scoped implementation requirements

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| INIT-01 | Separate status clearing,info initialization and live effect teardown | Clear30 bytes,Charge0,six info records retain +8;badge/flags/counters remain untouched |
| INIT-02 | Preserve distinct base/max/current resources and upper-only narrowing | Mario imports current70/74,uses base8e/90,adds5 per unsigned count;negative current retained when <=max |
| INIT-03 | Preserve partner mapping,read order and direct snapshot restore | Kind229 maps index3;status119 and negative/shared-pair bytes restored verbatim;FP unchanged;HP<=0 overwrites only status27 duration |
| INIT-04 | Keep import and revision dispatch distinct | Other-kind import has no writes;direct other-kind revision applies both count bytes and upper caps |

These are derived host-verification obligations,not original executed traces.
The [badge/setup contract](UNIT_BADGE_COUNT_IMPORT.md) now closes one concrete
badge/count/bonus import path. Unclosed STATUS-01/DMG-01 work:other count writers,
move-count writers,snapshot caller and switch/reload lifetime,status infliction/recovery/periodic
controllers,all pointer/flag mutations and per-hit AC state. Original live/lethal
partner entry,swap/reentry,negative-status and multi-turn traces remain required.

tools/verify_behavior_contracts.py pins32 new instructions,five whole-body hashes
and the original entry call sites;tools/behavior_evidence.py fingerprints the
complete bodies. Inspect with tools/inspect_dol_function.py using the US symbol
index. ISO reads are read-only;code/data/inspection artifacts remain ignored or
under /tmp. No asset conversion,dialogue rewrite or completed-game claim is made.
