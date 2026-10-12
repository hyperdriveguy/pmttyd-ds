# Partner follower-history writer, source selection and read queries

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyWalkMain at0x800c9b9c,size0x598;anotherPartyGetPtr at0x800bf190,size0x24;
partyGetReadId2 at0x800cc8a0,size0x40;partyGetReadId at0x800cc8e0,size0x40.
This closes local history behavior,not the final walkMain movement engine or
all ring-index writers. See [callback initialization](FIELD_PARTNER_WALK_CALLBACK.md)
for80-record bank layout and addresses.

## Slot-based source selection differs from initialization

Read P's slot byte+0x2f,signed,and capture owner M=P+0x160. Slot exactly0
writes bank0. Every other signed slot takes the bank1 path. Initialization
instead selects its bank using P+0 mask8;these predicates are not merged here.
The valid constructor/state domain must establish their intended relationship.

Bank0 source coordinates are owner M+0x8c/+0x90/+0x94,captured before
history tests. Query global Mario+0 mask0x10000 to set source flag1,and
call marioChkSlitThrouh at0x800a6204;nonzero adds source flag4. These reads
are not replaced with P's own flags or rendered position. Some subsequent
marioGetPtr calls have unused results;they do not change the captured vector.

Bank1 first calls anotherPartyGetPtr(signed slot). A negative argument returns
null;nonnegative returns table[((argument+1)&1)],with32-bit add/shift behavior.
It is not an unbounded table lookup. Let that returned object be Q:

| Source condition | Captured vector | Source flag byte |
| --- | --- | --- |
| Q null |Fresh global Mario position|Global Mario+0 mask0x10000→flag1 |
| Q kind byte3 and mode byte3 |Fresh global Mario position|Same flag1 |
| Otherwise nokonokoGetStatus(Q)==3 |Fresh global Mario position|Same flag1 |
| Otherwise distance(owner M position,Q position) ordered>=150 |Fresh global Mario position|Same flag1 |
| Otherwise |Q+0x58/+0x5c/+0x60|Q+0 mask0x10→flag1 |

The kind3/mode3 shortcut skips the status/distance calls. The status3 shortcut
skips distance. A NaN distance does not take the ordered>=150 branch.
Bank1's local source flags begin at0 and can become1;they never acquire flag4
through these branches. Unlike bank0,no slit query supplies flag4 here.

## Original append guards and wrap order

For the selected bank,let w and r be its current write/read index words and
old=record[w]. The callback performs these guards in order:

1. If old.flag&1 and source.flag&1 are both set,skip append.
2. Compare old X,Y,Z to captured source X,Y,Z in order with fcmpu.
   If all three equal,skip append. Unordered comparison is not equality.
3. If **r == w+1**,using32-bit addition **without modulo80**,skip append.

Every skip still goes to the final walkMain call. Otherwise increment/store
the write word;if the new signed value is>=80,store0. Negative/corrupted
index values are not clamped before the record lookup or this update. No
upper-bound check protects the initial old-record address.

Use the updated write index to store the captured X,Y,Z words. Update only
record flag masks1 and4 to match source flags;preserve its other six bits.
The unwrapped full-buffer guard is literal original behavior:for w=79,r=0,
the equality test compares0 with80 and does not reject,then the write wraps
to0. Do not wrap w+1 before this guard or replace it with a conventional
ring-buffer capacity test. This example describes static branch composition,
not proven reachability of that index configuration during gameplay.

No read index advances in this body. Actual delay,spacing,path interpolation
and consumer cadence depend on other index writers and the movement engine.

## Read helpers and final movement request

partyGetReadId(bank,&x,&y,&z) and partyGetReadId2(bank,&vector) compute:

```
readIndex = word[0x8041eae8 + 4*bank]
record = 0x803ec2c8 + 0x500*bank + 16*readIndex
```

Load float record+4,+8,+0x0c and store to the three separate destinations or
vector+0,+4,+8,in X/Y/Z order. There is no bounds/null/flag check,no index
advancement and no coordinate resolution or quantization in these helpers.
Their names do not mean they return an integer record ID.

partyWalkMain always ends by requesting **walkMain(P)** at0x800c8568,
including when an append guard rejects. It does not itself integrate P's
position or call the full terrain/pose engines beyond those requests.
Those larger movement semantics remain open;an80-record history alone is
not a complete equivalent follower implementation.

## Verification and remaining work

tools/verify_field_party_follow.py pins these four bodies together with the
two movement callbacks and hitGetAttr,66 instruction anchors,ten constants,
kind1/8 record hashes and two lecture imports. Original listings and reports
remain ignored. No original execution or fixed following-distance observation.

Next:walkMain and ring read-index/flag producers;global/owner slot invariants,
source-query/status/distance bodies,collision/pose services and original follower
trajectory/stop/run/transition traces. These local closures do not complete
partner locomotion,all field mechanics or the full game.
