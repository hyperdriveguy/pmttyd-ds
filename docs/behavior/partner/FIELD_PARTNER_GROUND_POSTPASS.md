# Ground partner stay: head contact, floor decisions and named-support motion

US G8ME01 disc0 revision0, ID1, recovered2026-10-12. Evidence B:complete
unk_801512e0 at0x801512e0,size0x18c;partyChgMot0x800cb584,size0x44;and
named-hit classifier unk_800cb9bc0x800cb9bc,size0xa4. This is the actual
[stay](FIELD_PARTNER_STAY_MOTION.md) tail for every unsigned mobility P+32
OTHER THAN1. Calling it ground describes the ordinary intended domain;the
byte test does not require mobility0 specifically. P position is+58/+5c/+60.

## Query and suppression order

Copy P position to local V and float P+f0 to local h;call [partySearchHead](FIELD_PARTNER_CEILING_QUERY.md)
0x800c27f8 with(P,&V,&h,f1=P+10c). Nonnull H overwrites P+140;null leaves
that field untouched. The local position/height outputs are not published.
This query happens even when P+0&0x04000000 is SET;only AFTER the query does
that mask suppress the remaining floor/motion path. This differs from the
[fly postpass](FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md),where that mask exits
before either query,and head probing is map-specific.

If not suppressed,clear P+0 mask0x80,then request
[partySearchGround](FIELD_PARTNER_FLOOR_QUERY.md)0x800c3bcc
with(P,f1=15.0,f2=-15.0). On nonnull G,call hitGetAttr(G),retaining attributes
A. If A&0x200 is set,discard G regardless of signed byte P+4c. When that
byte is exactly0,also OR0x01000000 into P+8 and0x02000000 into P+0. Nonzero
signed byte does not prevent discarding;it only suppresses those two latches.
There is no local0x800 hazard check or direct driver-mode13 request here.

The selected floor-query entry at0x800c3c54–6c tests P+0&0x02000000 and
returns null BEFORE resetting its output fields when set. Thus the latch above
can suppress a later floor query and preserve cached query outputs;the complete
writer/clearer/domain must establish how it is released. Do not add an automatic
clear at this postpass exit or assume every null query refreshed P+e4/P+e8.

Store the retained or null G at P+138. In the null case,clear P+0 mask2,
request partyChgMot(P,3),then store original -1.5 at float P+11c and return.
No floor-height snap or named-hit classifier follows that route. The old
P+14c contact is not cleared by this body. Exact floor-query side effects
remain the called query's responsibility.

## Retained floor and named support

For retained G,snap P+5c to current P+e4;set P+0 mask2. Set mask0x80 exactly
when A&0x100 is set;otherwise keep the earlier clear. If A&0x40000000 is set,
store G at P+14c. Absence of that attribute preserves the previous P+14c.
This is a direct snap rather than the fly postpass's smoothing.

Next call unk_800cb9bc(G). If it returns0,finish. If it returns1:

1. Request [partyGetAppearPos5](FIELD_PARTNER_SUPPORT_APPEARANCE.md)(P,G,&P+94)
   at0x800c5794. Its complete local search is now recovered;the caller ignores
   its return,so failure does not gate the following stores/motion request.
2. OR0x00400000 into P+8.
3. Copy the current P+5c into P+98 AFTER that helper returns.
4. Request partyChgMot(P,2),then finish.

The +94 destination points into a multiword local state region. Its later
consumer remains separate;the helper's sampled writes are recovered above. It is not the
partner root X/Z. The named-support list includes fixed blocks/chests as well
as stands and switches,so the classifier must not be described as a generic
moving-platform detector.

## Complete classifier and exact resource table

unk_800cb9bc returns0 for a null hit. Otherwise obtain hitGetAttr and require
its0x80000000 bit;absence returns0 without looking up an owner. For an admitted
hit,call mobjHitObjPtrToPtr0x8005f7c4. The already recovered
[owner leaf](FIELD_PARTNER_FRONT_WALL_FILTERS.md) reads H+d0 when that high
attribute bit is present. There is no owner-null guard before owner+15.

Starting at pointer table0x80314fe8,load one name pointer at a time and compare
owner+15 with strcmp0x8026d464. First equal comparison returns1;advance by4
after a mismatch;null pointer terminates with0. The original table contains
these25 exact resource identifiers followed by a null pointer,in this order:

| Index | Name | Index | Name |
| --- | --- | --- | --- |
| 0 | MOBJ_TreasureBox | 13 | MOBJ_eki_06_su1 |
| 1 | MOBJ_BigTreasureBox | 14 | MOBJ_eki_06_su2 |
| 2 | MOBJ_GrayTreasureBox | 15 | MOBJ_eki_06_su3 |
| 3 | MOBJ_BlackTreasureBox | 16 | MOBJ_RedSwitch |
| 4 | MOBJ_GoldenTreasureBox | 17 | MOBJ_BlueSwitch |
| 5 | MOBJ_BlueJumpStand | 18 | MOBJ_WhiteSwitch |
| 6 | MOBJ_BlueJumpStand2 | 19 | MOBJ_BlackSwitch |
| 7 | MOBJ_RedJumpStand | 20 | MOBJ_green_FlowBlockSwitch |
| 8 | MOBJ_RedJumpStand2 | 21 | MOBJ_purple_FlowBlockSwitch |
| 9 | MOBJ_MapJumpStand | 22 | MOBJ_orange_FlowBlockSwitch |
| 10 | MOBJ_SaveBlock | 23 | MOBJ_RedTimerSwitch |
| 11 | MOBJ_RecoveryBlock | 24 | MOBJ_BlueTimerSwitch |
| 12 | MOBJ_CoinTakenRecoveryBlock | 25 | Null terminator |

These identifiers are asset lookup keys,not story dialogue. Case and ordering
remain exact;do not replace the table with broad substring/name categories.
The tool pins all26 pointers and the contiguous containing name-data span.

## Complete motion-change leaf

partyChgMot(P,new) reads unsigned current substate P+36. If it is3,clear
P+0 masks0x70,0x03000000 and0x04000000,in that order with separate rereads/
stores. This cleanup depends on the OLD substate,including a3→3 request.
Then store the low byte of new to P+36 and OR2 into P+8 unconditionally.
No early same-state equality check exists. There is no driver P+34 update,
mode callback execution,pose call,velocity clear or immediate fall/jump step
inside this leaf. The entry marker is consumed by later motion handlers.

Null floor therefore sets substate3/entry marker BEFORE writing -1.5;named
support sets substate2 AFTER saving the prepared Y. The current stay handler
does not recursively run either new motion. After this ground tail returns,
stay calls party_force_reset_outofscreen and may continue its kind7 pose logic.

## Verification and remaining work

tools/verify_field_partner_postpass.py checks four complete local body hashes,
actual stay call sites,selected independent instruction anchors,exact numeric
constants,las_10 resource identity,named-support table words and name bytes.
It also pins containing head/floor query bodies with explicitly SELECTED-region
scope for entry/return field evidence;their whole algorithms are not claimed
complete here. Useful deduced cases include suppression with nonnull head,
null head preserving P+140,attr0x200 with P+4c zero/nonzero,null-floor fall,
floor snap/0x100 flag and0x40000000 stale-contact ownership,known/unknown
names,old-substate3 cleanup and motion-byte truncation. No original execution
or timing measurement follows from these checks.

Next:complete head/floor and appear-position queries,query/flag/height/source
domains,named-support jump/fall/landing and out-of-screen services,all object
lifetimes and original collision/transition trajectories. Full field/game
documentation and complete-game requirements remain incomplete.
