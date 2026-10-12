# Party-selection table production and capture order

Complete `_btlcmd_MakePartyTable`80123360/0x1c4,static US G8ME01 revision0.
Called as a side effect by [operation producer](OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md).
[Verifier](../../../tools/verify_operation_party_tables.py) pins the body and
source rows.[Icon/admission leaves](PARTY_ICON_SOURCE_AND_CAN_ACT_STATUS.md)
cover presentation/service boundaries. This does not recover party key1 weapon
delegates or actual party switching/event execution.

## Retained actor/other unit,pouch,and guards

Retain passed B. Load WORD(B+0x420),request BattleGetUnitPtr(B,id),retain U.
Request BattleGetPartnerPtr(B,retainedU),retain OTHER. Compute M=B+0x171c;
initialize local count N0,byte offset0,selected ordinal K0;request pouchGetPtr,
retain P. Only afterward check wrapped M==0,then OTHER==0;either returns0
without table/cursor writes. No own U-null guard before the other-unit helper.

Request GetCursorPtr(M,10,&privateSP08),ignore return. Iterate EXACTLY8 source
records,16 bytes each,beginning80363f20:rows0..7. Read current source kind T.
T0 skips all further service/admission/emission. Otherwise request
BattleTransPartyId(T),derive R=wrap32(P+14*returnedId),read U16(R+0).
Bit1(value0x1) must be set to admit. No translated-ID bounds check or copied
pouch snapshot. Disabled admitted rows still increment N.

## Partial24-byte output and original presentation

Q=M+24*N;descriptor begins Q+0x390:
kind+0 word,enabled+4 word,name+8 word,icon+12 halfword,padding+14..15
untouched,help-key+16 word,HP+20 halfword,maxHP+22 halfword.
Store admitted T at Q390,enabled1. For exactlyT226 request pouchGetYoshiName;
otherwise read CURRENT source name-key word+4,request msgSearch. Store result
as name. Request _battleGetPartyIcon(retainedT),store low16 as icon. Read
CURRENT source word+0x0c,store direct help-key pointer. Read S16(R+6),store
HP;read S16(R+2),store maxHP. Do not clear padding or unused descriptors.

## Current actor versus retained/fresh other-unit ownership

Read CURRENT WORD(retainedU+8). If equalT,enter occupied-row branch directly.
Otherwise request BattleGetPartnerPtr(B,retainedU) AGAIN,read returned unit's
CURRENT kind without a null guard. Equality with T also enters occupied branch.
The fresh helper is omitted on actor-kind match.

Occupied branch:store enabled0;set selected ordinal K=currentN. Reread
CURRENT actor kind. If now equalT,replace HP with S16(U+0x10c). Otherwise
replace HP with S16(RETAINED entryOTHER+0x10c),not the fresh other-unit returned
by the comparison helper. Source maxHP remains unchanged. This can expose
retained/current owner differences;do not normalize to one actor snapshot.

Unoccupied branch:read fresh S16(R+6);<=0 disables the row. Occupied rows
skip that pouch-HP admission check. Both branches next read fresh U16(R+0);
bit0x2 set disables the row. No status-query service or actor104 mask here.
Increment N and output byte offset24 after all these operations;advance source
ordinal/byte offset1/16 regardless of admission.

## Final cursor writes and scalar count

After all8 source rows,load cursor from SP08,store count N at C+8;reload SP08,
store K to C+0. K starts0 and is LAST occupied admitted ordinal if several
match;it is not first enabled row,not randomized,and not clamped. Empty N0
still writes count0/index0 on this ordinary path. C+4 remains untouched.
Return N explicitly. Early M/OTHER guard exits preserve existing cursor fields.

This count includes current/other-unit disabled rows,dead/unavailable rows,
and flags2-disabled rows. The operation producer compares N>=2 without
searching for an enabled alternative. [Confusion party selection](CONFUSION_OPERATIONS_PARTY_AND_EVENT_OUTPUTS.md)
later filters enabled rows from this table,but does not itself build it.
Native implementation must preserve the producer side effect and that
caller distinction. Source/provisioning/menu/delegate/query/Game/runtime remain open.
