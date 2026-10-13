# Operation-table producer:entry,optional rows,and publication

Complete static US G8ME01 revision0 `_btlcmd_MakeOperationTable`
801238b8/0x448. Companions:[charge/mandatory rows](OPERATION_TABLE_CHARGE_DEFEND_ESCAPE_AND_KEYS.md),
[party producer](PARTY_SELECTION_TABLE_PRODUCTION_AND_CAPTURE.md),
[icon/action admission](PARTY_ICON_SOURCE_AND_CAN_ACT_STATUS.md).
Native request:[EVT-01](../../requests/EVT-01.md).
[Verifier](../../../tools/verify_operation_party_tables.py) pins four complete
bodies,12 linked services,472 instructions,and32 table/key/weapon-head spans.
Static only;menu consumers/delegates/source/query reachability remain separate.

## Ordered entry and current versus captured setup

Retain passed B. Load WORD(B+0x420),request BattleGetUnitPtr(B,id),retain U.
Load S=WORD(B+0x2738),retain D=WORD(S+0x0c). Compute M=B+0x171c;
request pouchGetPtr(),retain P. Only then honor M==0:exit with pouch return
still inr3,no local writes. Setup is dereferenced before this wrapped-address
guard;there is no prior U/P/S/D null guard.

Request GetCursorPtr(M,5,&privateSP08),ignore return. Initialize local N0.
Request `_btlcmd_MakePartyTable(B)`;this populates another table/cursor as a
side effect,not merely a count query. Signed return>=2 admits Change Party.
Its count includes disabled admitted rows. Ignore its enable flags for this
admission. The operation cursor and party helper's cursor use different private
outputs/current-global ownership;do not impose equality with retained U.

For current N,let Q=M+28*N. Operation descriptor begins Q+0x2cc:
code+0 word,enabled+4 word,reason+8 word,name+12 word,icon+16 halfword,
untouched padding+18..19,help-key address+20 word,cost+24 word. Neither all
rows nor padding/unused slots are cleared. Disabled admitted rows count.
No operation TYPE M540,selected opcodeM548,or Input-state write occurs here.

## Optional Change Party,code0

When party count>=2,write code0 at fixed M2cc. Read S8(U+0x12b):nonzero
initializes enabled0/reason3;zero initializes1/0. Lookup original key802eff40,
store returned name at M2d8. Choose icon source actor V:
read current WORD(U+8);signed224..230 retains V=U. Otherwise request
BattleGetPartnerPtr(B,retainedU),retain returned V. No null guard.

Request BtlUnit_CanActStatus(V) but DISCARD its result. Read current WORD(V+8),
request _battleGetPartyIcon(kind),store low16 icon at M2dc. Store help-key
address802eff58 at M2e0. Then read fresh WORD(U+0x104);mask0x10 denies
(enabled0,reason3,in that order). Read flags WORD(capturedD+4);mask0x04000000
also denies. These checks still run after an earlier denial;the discarded
CanAct result is not an extra enable gate. Finally store cost0 at M2e4,set N1.

The first row's fields are fixed addresses because N starts0. Do not infer
that all other operation rows necessarily exist or become enabled. Icon/name
services occur before the final restriction reads.

## Optional Appeal,code5

Read U16(retainedP+0x8c);nonzero emits at current N. Store code5,enabled1;
request msgSearch(802eff7c),store name;increment N BEFORE final stores,using
retained row address for them. Store icon364 halfword,reason0,cost0,then
help-key802eff90. Zero omits the row entirely.
There is no unit104,actor-kind,status,setup,or cost enable check in this Appeal
stage. Do not import Defend/Escape restrictions merely because all are operations.

Next optional charge rows and mandatory Defend/Escape are in the companion.

## Final cursor count and signed correction

After optional rows,N counts only those optional entries. Defend occupies N,
Escape N+1. Publish WORD(C+8)=N+2 to cursor loaded from SP08. Reload SP08,
current signed index WORD(C),then signed current count WORD(C+8). If index
>=count,store count-1;otherwise preserve it,including negative indices.
No cursor scroll(C+4) clear/correction or extra eligibility update is requested.
Ordinary local count is2..6,conditional on changing services/admissions;this is
not a full source/runtime invariant for downstream callers.

There is no explicit scalar success/count assignment at final epilogue:r3
holds the most recently loaded cursor address on the ordinary path. Do not
reuse the party helper's count as this function's return. Callers ignore it.

## Native requirements and source boundary

Implement party-table side effects,partial descriptors,optional versus disabled
rows,retained entry setup for Change Party versus fresh Escape setup,ignored
CanAct result,typed reads/store order,and signed cursor publication. Reuse
original keys/icons/pouch data. [Confusion selector](CONFUSION_OPERATIONS_PARTY_AND_EVENT_OUTPUTS.md)
uses the produced count/opcodes;its caller behavior is already complete.
OperationMenuMain/setup,party source delegates,query-event reachability,Game/
platform/original execution and full-game requirements remain open.
