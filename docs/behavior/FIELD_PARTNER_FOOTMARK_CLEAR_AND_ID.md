# Partner footmark clearing and ID selection

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyClearFootmark0x800cc7c4,size0xdc;partyClearFootmark2 at0x800cc64c,
size0x178;partyGetFootmarkId at0x800cd1c8,size0x0c. These are local record
and index contracts,not original following trajectories or timer observations.

## Two explicit clear functions

Both clear functions ignore their incoming argument and query the global Mario
pointer themselves. They do not interpret a P pointer or accept an arbitrary
bank index. Their names do not imply that they erase all record bytes.

partyClearFootmark captures global Mario X/Y/Z at+0x8c/+0x90/+0x94 and
resets **bank0** at0x803ec2c8. partyClearFootmark2 captures global Mario as G,
queries marioGetPartyId and passes its result to partyGetPtr as Q:

| Clear2 source condition | Seed vector |
| --- | --- |
| Q null |Captured G position |
| Q nonnull,nokonokoGetStatus(Q)==3 |Captured G position |
| Otherwise PSVECDistance(G position,Q position) ordered>=150 |Captured G position |
| Otherwise |Q+0x58/+0x5c/+0x60 |

Status3 skips the distance query. Unordered distance does not take>=150.
The selected coordinate words are captured before the record reset loop.
Clear2 resets **bank1**,base0x803ec7c8;it never resets bank0 as a fallback.

Each loop performs ten iterations of eight16-byte records,writing X/Y/Z
words at+4/+8/+0x0c and flag byte0 at+0. Record padding+1..+3 remains
unchanged. After all80 records,clear only that bank's write/read indices:

| Bank | First cleared word(write) | Second cleared word(read) |
| --- | --- | --- |
| 0 |0x8041eaf0|0x8041eae8|
| 1 |0x8041eaf4|0x8041eaec|

This is the same layout as the [history writer](FIELD_PARTNER_HISTORY_WRITER.md).
The functions do not move P,change a driver mode or clear every object flag.

## Footmark ID is a different predicate

partyGetFootmarkId(P) reads word **P+0** and returns **(word>>2)&1**.
There is no null check,slot-byte read,read-index lookup or state write.
In particular,the following selectors remain distinct:

| Selector | Actual local predicate |
| --- | --- |
| This footmark ID |P+0 mask4 |
| Callback/query history initialization bank |P+0 mask8 |
| partyWalkMain history write bank |Signed slot byte P+0x2f==0 versus otherwise |
| [partyFlyMain write bank](FIELD_PARTNER_FLY_HISTORY_WRITER.md) |Signed slot0→bank0;nonzero slot+nonnull fixed party0→bank1;null→bank0 |

The constructors establish these combinations for their normal roles,but
arbitrary object states or alternate allocation domains are not validated by
these helpers. Do not silently derive all three from one guessed slot enum.

## Actual stay bindings and verification

[Stay motion](FIELD_PARTNER_STAY_MOTION.md) queries this ID before its entry
marker processing. Its marker2 path chooses clear/clear2 using P+0 mask8;
its secondary-distance route can also call clear2 after a motion-change request.
These clear calls make no use of the P value left in the integer argument
register by earlier services.

tools/verify_field_partner_targets.py pins these three bodies,live target
distance helpers and SDK PSVECDistance,six whole hashes/55 instructions/three
constants/two lecture imports. No original code/resource payload is distributed;
corrected Gekko listings and metadata reports remain ignored.

Next:source-ID/status services,all read-index writers and history consumers,
flag/slot/owner invariants,original following/clear/stop/run trajectories and
field/battle teardown. This does not close the entire footmark/follower system.
