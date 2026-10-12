# Party switch:spawn,ID-list remap,home coordinates,and snapshot export

Complete static US G8ME01 BattleChangeParty800f8538/0x17c plus home-position
leaves GetHomePos801289b0/0x1c and SetHomePos801289a0/0x10.
[Field export/caller census](PARTY_FIELD_EXPORT_AND_SNAPSHOT_CALL_DOMAINS.md)
completes this slice.Native request:[EVT-01](../../DOCUMENTATION_REQUESTS.md#evt-01--native-event-scheduler-and-dispatch-closure-open).
[Verifier](../../../tools/verify_party_switch_snapshot_callers.py) pins four
complete bodies,ten linked identities,127 instructions,two source spans,and
all7 aligned direct DOL calls to snapshot import/export.No original execution
or whole battle/save/query reachability claim.

## Admission occurs after unchecked current-party query

Retain passed B.Read WORD(B+0x1c68) (menu M54c) as desired kind K BEFORE
requesting BattleGetPartyPtr(B).Retain returned OLD and read WORD(OLD+0) as
oldId without a null guard.Only then validate signed224<=K<231;failure returns0.
No current-party lookup avoidance when desired kind is invalid.

For valid K,derive setup address80363790+48*(K-224),request
BtlUnit_Spawn(setup,1),retain NEW.No returned-unit/null/same-kind guard.
It can spawn even when requested kind matches OLD.Source setup first pointers:

| Desired kind | Original unit definition |
| --- | --- |
| 224 | 80393bec,Christine/Goombella |
| 225 | 8038ef34,Nokotarou/Koops |
| 226 | 8037c79c,Yoshi |
| 227 | 803791f4,Clauda/Flurrie |
| 228 | 8038b89c,Vivian |
| 229 | 8038780c,Sanders/Bobbery |
| 230 | 803818c0,Chuchurina/Ms. Mowz |

All seven48-byte setup records are pinned.This is a concrete selected-kind→
setup→spawn source chain.[Complete spawn/entry](../unit/UNIT_ENTRY_STATS_AND_SPAWN.md)
and [seven key1 bindings](PARTY_WEAPON_CALLBACK_KEY1_SOURCE_BINDINGS.md)
provide linked table publication/callback evidence.Unit/source/lifetime services
may still mutate records;do not assume native construction is complete merely
because this static pointer chain exists.

##64-word ID-list exchange reads newId per entry

Visit WORD(B+0x120+4*J) for J0..63 exactly.In order:

- Value-1 is preserved,with no NEW-ID read on that branch.
- Value==retained oldId:read CURRENT WORD(NEW+0),store it to this list slot.
- Otherwise read CURRENT WORD(NEW+0);if current slot value equals it,store
  retained oldId.Other values preserved.

NEW ID is not preloaded once for the loop.There is no count/bounds validation
of the stored IDs,registry deletion,or early break on first match.This exchanges
matching entries in the raw64-word battle list without claiming a complete turn
scheduler/ordering policy from these instructions alone.

## Mixed home coordinate capture and global floor request

Request GetHomePos(NEW,&SP10,&SP0c,&SP08),then
GetHomePos(OLD,&SAME SP10,&SP14,&SAME SP08).Complete leaf loads/stores
unit30→x,34→y,38→z,in order,without null/alias guards.Thus private x/z
become OLD home x/z,while SP0c retains NEW home y.OLD home y at SP14 is
written but not used by later local coordinate operations.

Request BtlUnit_EquipItem(NEW,5,0),ignore result.Then RELOAD CURRENT global
battle GB=WORD(8041eb90),load private x/newY/z,and request
BattleGetFloorHeight(GB,x,newY,z).This global owner need not equal passed B
without a source invariant.Retain returned binary32 f1 just long enough for
original `fadds`:y=single(newY+f1).Store y to privateSP0c;load x/z;request
SetHomePos(NEW,x,y,z).Complete setter performs scalar stores to30/34/38
in that order.No visible-position setter or trajectory update appears here.

The floor service is linked,not recovered by this slice;the added scalar is
not labelled a measured delta/absolute height or guaranteed terrain intersection.
Equip service may mutate NEW before the later requests.No extra move-to-home,
render-frame delay,sound,or event constructor is invented.

## Acting ID,mode copies,old export,and retirement flag

Read CURRENT WORD(B+0x420);if equal retained oldId,read CURRENT NEW ID,
store it to B420.Otherwise preserve acting ID.Then copy BYTE OLD20→NEW20,
OLD22→NEW22,OLD23→NEW23,OLD24→NEW24,in that order,with fresh load before
each store.No word-wide copy or status normalization.

Request BtlUnit_SetParamToPouch(retainedOLD) at800f867c AFTER NEW spawn,
list/home/acting/mode mutations.Ignore return.[Complete exporter](../unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md)
updates old party pouch HP and old-kind44-byte battle snapshot through its own
captured global/pouch owners.New entry import can therefore precede old export;
do not reorder to export first merely to simplify a native switch.

AFTER export,read CURRENT WORD(OLD+0x1c),OR0x8,store back;then copy
OLD311→NEW311 and OLD312→NEW312 as bytes.Return retained NEW pointer inr3.
The old unit is not freed/unregistered by this body.The
[party getter](FIRST_ATTACK_PAIR_SELECTION_AND_GETTERS.md) excludes units
with1c mask8,so this local flag affects later selection;full retirement/lifetime
and resource teardown remain separate.

Implement captured desired kind/OLD-ID,unchecked source order,fresh NEW-ID
reads,shared coordinate outputs,global floor recapture,single addition,export
before old-flag/counter copies,and literal pointer/0 result.Pouch/save/source/
query/Game/platform/original runtime/full-game requirements remain open.
