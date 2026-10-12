# Goombella weapon-table callback and current-party technique level

Complete static US G8ME01 revision0
`unit_party_christine.o::__makeTechMenuFunc`80189634/0x2dc and
`partyGetTechLv`80140e60/0x44. Native request:
[EVT-01](../../DOCUMENTATION_REQUESTS.md#evt-01--native-event-scheduler-and-dispatch-closure-open).
[Seven original source bindings](PARTY_WEAPON_CALLBACK_KEY1_SOURCE_BINDINGS.md)
identify how the caller obtains this address.
[Verifier](../../../tools/verify_party_weapon_callback.py) pins two complete
bodies/200 instructions,ten linked identities,and23 source spans. No original
execution,whole party action graph,or global query reachability claim.

## ABI and global-owner query before any row writes

Arguments are passed menu address M and mutable count-WORD address C.
Retain both. They are not a battle/unit pair. The
[complete generic builder](WEAPON_TABLE_ITEM_SPECIAL_AND_PARTY_ROUTES.md)
uses unit-data key1 as a callable address and passes `(M,&itsPrivateCount)`.
This callback appends;it does not initialize that count or acquire a cursor.

First load CURRENT global battle B=WORD(8041eb90),request BattleGetPartyPtr(B),
read returned unit's WORD(+8) kind without a null guard,request
BattleTransPartyId(kind),then partyGetTechLv(returnedId). Retain signed returned
level L once for all four row admissions. No check that current party is
Goombella,that B matches the generic builder's passed battle,or that the retained
menu belongs to that global B. Preserve this current-owner request sequence
rather than using an assumed Goombella rank from a native actor object.

Complete partyGetTechLv retains signed argument I. If I>=8,return0 WITHOUT
pouch request. Otherwise request pouchGetPtr(),read SIGNED halfword at
wrap32(P+14*I+12),return it. Negative I passes the upper-bound guard and is
not clamped;there is no pouch-null guard. Do not normalize negative level or
translate IDs a second time. The callback's source translation and level-query
pointer ownership are different services.

## Four fixed weapons and signed admission

Retained source base80393a58 provides these literal pointers:

| Order | Admission using captured L | Weapon | Original key |
| --- | --- | --- | --- |
| 0 | always | 80393ea8 | `btl_wn_pkr_normal` |
| 1 | L>=0 | 80393f68 | `btl_wn_pkr_lv1` |
| 2 | L>=1 | 80394028 | `btl_wn_pkr_lv2` |
| 3 | L>=2 | 803940e8 | `btl_wn_pkr_lv3` |

No fifth row for levels>2. Negative L still emits the initial weapon. With
unchanged count input,append1/2/3/4 rows for L<0/L0/L1/L>=2 respectively.
The default0 from level query's I>=8 branch therefore appends TWO rows.
These are local conditional counts,not a runtime party-level invariant.

## Repeated live count loads and partial28-byte records

For EACH admitted row,perform the following ordered actions. Before each
row-field address calculation,read CURRENT WORD(C) and compute
Q=wrap32(M+28*thatWord). Do not retain one Q across the whole sequence.

1. Store WORD(Q+0x90)=-1.
2. Reload count;store WORD(Q+0x94)=0.
3. Reload count;store fixed weapon address at WORD(Q+0x80).
4. Reload count;store WORD(Q+0x84)=0.
5. Reload count;reload CURRENT row weapon;read its U16(+4),store halfword
   icon at Q+0x8c.
6. Reload count;reload CURRENT row weapon;read WORD(weapon+0) as key;
   request msgSearch(key) without a key-null or success guard.
7. AFTER lookup,reload count again;store returned name address at Q+0x88.
8. Reload count,add1 with word wrapping,and store back to WORD(C).

Reason WORD(Q+0x98) is NEVER initialized here. Padding Q8e..8f also survives;
unused rows and leading menu bytes are untouched. Unlike the generic ordinary
Mario row path,this callback neither clears reason nor applies ItemData fallback
when a name-key word is0. Icon and name use current row pointers,which can
change through physical aliasing/shared services. Count C can alias other
address-backed fields;an implementation cannot replace it with an inaccessible
host integer or collapse the repeated loads.

The rawr3 at return is the final count value LOADED before its last increment,
not an explicit appended-count or success assignment. The generic builder
ignores this scalar and reads the actual count word afterward. No type/Input
state/cursor count/index/scroll publication or actor mode mutation occurs in
this callback's own stores.

## Integration and remaining work

After delegation,the generic builder publishes the current positive count,
corrects cursor/index/scroll,and runs
[eligibility update](WEAPON_MENU_TARGET_COUNT_AND_ELIGIBILITY_UPDATE.md),which
then clears visited reasons/enabled fields under its own actor/current cursor
ownership. Do not move that caller update into this callback or assume its
owner must equal the party used for L. Reuse original keys/icons/weapon data.

Implement exact `(M,countAddress)` ABI,current-global query,one captured L,
signed admissions,reloaded count/row pointers,reason preservation,name request,
count-write/raw-return distinctions. Other six callbacks,unit-kind provisioning,
full party weapon/event definitions,query-event domains,Game/platform/original
runtime and full-game requirements remain open.

## Delivered remaining-callback continuation

[Complete six-body protocol and Koops exception](PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md)
and [exact source order](PARTY_WEAPON_CALLBACKS_WEAPON_SOURCE_MAP.md) now
close the six local callbacks previously left open.All seven callback bodies
and technique-level query are complete;unit provisioning/full events/query
reachability/Game/original execution remain separate.
