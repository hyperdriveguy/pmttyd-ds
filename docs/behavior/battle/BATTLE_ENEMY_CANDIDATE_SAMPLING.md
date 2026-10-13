# Enemy candidate sampling: buffer, geometry, admission and ordering

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Static complete bodies: BattleSamplingEnemy800ff5c0/c4,
BattleSamplingEnemyUpdate800ff5a0/20, _btlSamplingEnemy800ff684/bd4 and
BtlCompForwardLv801254a0/54. See [choice](BATTLE_ENEMY_CANDIDATE_CHOICE.md)
and [takeoff caller](BATTLE_JUMP_TAKEOFF_POSITION.md). No original trace claim.
Verify with `python3 tools/verify_enemy_candidate_sampling.py`.
All bit masks here use ordinary hexadecimal least-significant-bit numbering.

## Entry and return ownership

BattleSamplingEnemy(B,weapon,actorID,belong,A,C,direction) first stores:

| Buffer offset | Store/read role |
| --- | --- |
| +0 | Weapon pointer |
| +4+0x24*i | Candidate row i |
| +0xa6c | Signed byte candidate count |
| +0xa6d+i | Signed byte permutation index i |
| +0xab7 | Signed byte current selection index |
| +0xab8 | Actor ID word |
| +0xabc | Low8 belong, interpreted signed |
| +0xac0 | Admission mask A word |
| +0xac4 | Attribute/occlusion mask C word |
| +0xac8 | Low8 direction, interpreted signed |

Fetch actor through shared BattleWork. If actor+0x27c has0x10 and exactly one
of A's0x100/0x200 bits is set, swap those bits and retain Q=1. Otherwise Q=0.
Call _btlSamplingEnemy(B), then return Q. This return is NOT the count,
nonempty predicate or chosen target. BattleSamplingEnemyUpdate(B) simply calls
the core using retained metadata; it does not repeat the swap or produce Q.
The core has no useful scalar result contract.

If A has0x80000000, write count byte0xff (-1) and return immediately. No
selection reset, row/permutation clear, actor lookup or further admission occurs.
Normal sampling reconstructs rows, sets current selection0, and publishes final
count/permutation. It does not clear all unused buffer bytes.

## Row construction before admission

Scan registry IDs0..63 ascending. Skip null units, units whose GetStatus(U,27)
returns nonzero, and units with word U+0x104 bit0x40. For each remaining unit,
read U+0x10 kind pointer, its U8(+0xb4) part count, and linked part U+0x14.
Walk exactly that count, following P+0; skip part word P+0x1ac bit0x10000.
No local linked-pointer or capacity validation occurs.

For each retained part request GetPartsWorldPos(P,&X,&Y,&Z). Let world results
be x,y,z and scale=F32(U+0x114). If U.flags104 lacks0x2,
replace y with singleFMA(P.float28,scale,y). Save this resulting y as baseY.
Let F=S8(P+0xbd). Compute hit coordinates in this exact order, each add/multiply
single-rounded and each FMA fused:

- hx=FMA(float(F), single(scale*single(P.float188+P.float194)), x).
- hy=FMA(scale, single(P.float18c+P.float198), baseY).
- hz=FMA(scale, single(P.float190+P.float19c), z).

Store unit registry ID and part kind word(P+4)+0 as low16 row+0/+2;
store low16(fctiwz(hx/hy/hz)) at+4/+6/+8. Initialize row+0x18=0 and
row+0x20=-1. If U.flags104 has0x1 and P.flags1ac lacks0x20000,
add300*S8(direction) to row+0x18. Then add S16(P+0x1a0)*F with word arithmetic.
Call that accumulated word bias. The independent sorting coordinates are:

| Row field | Value |
| --- | --- |
| +0xc | Sign-extended low16(fctiwz(single(x+single(float(bias))))) |
| +0x10 | Sign-extended low16(fctiwz(baseY)) |
| +0x14 | Sign-extended low16(fctiwz(single(z+5.0))) |
| +0x18 | bias word |
| +0x1c | Low8 BtlCompForwardLv(float(row+0xc),S8(direction)) |
| +0x1d | 0 if signed row+0x14<-30, else1 |
| +0x20 | -1 |

Here float(bias) is converted through signed integer bias subtraction with an
fsubs single-rounded result before the x add. The hit X field+4 and sorting
X field+c therefore need not match. Bytes+a..b,+1e..1f are not initialized.
The row+1c forward helper starts q=0; if X>-800, q=trunc(single(single(800+X)/40)),
then caps q>48 to48. Return48-q for negative direction, else q. No lower clamp.

There are74 physical row slots before+a6c, but no bounds check limits construction
to74. After construction, store low8(total), reload it as signed count N and
use that N for subsequent passes. Oversized/invalid source states can overwrite
metadata and produce negative counts; do not describe capacity enforcement.
Valid unit/kind/linked-part counts and numeric domains are implementation invariants.

## First admission pass: mask A

For each row in current physical order, fetch its unit and part anew using
signed16 IDs. Evaluate the following enabled checks in listed order. A reject
removes the entire36-byte row by shifting following rows left; repeat at the
same index, preserving order. Kind means signed word U+8; ID means U.word0;
belong means S8(U+c). Local N decreases; the published count remains the initial
byte until the final store.

| A mask | Reject condition |
| --- | --- |
| 0x80000000 | Always (normally handled at core entry) |
| 0x10000 | kind!=222 |
| 0x20000 | kind outside216..217 |
| 0x1 | kind222 or223 |
| 0x2 | kind224..230 |
| 0x10 | kind1..215 |
| 0x40 | kind218..221 |
| 0x20 | kind216..217 |
| 0x1000 | ID==actorID |
| 0x100 | belong==buffer belong |
| 0x200 | belong differs from buffer belong AND belong!=2 |
| 0x2000 | ID differs from actorID AND kind equals fetched actor kind |
| 0x4000 | ID differs from actorID |
| 0x100000 | !(P.flags1ac&3) OR (P.flags1ac&0x40000) |
| 0x200000 | !(P.flags1ac&7) OR (P.flags1ac&0x10) OR (P.flags1ac&0x40000) |

Unlisted A bits have no direct check in this body. Tests are conjunctive admission:
a rejection ends this pass's tests for that row. No direct HP query occurs.
Status27 is a called service, not proof of a particular HP threshold.

## Second admission pass: mask C

Again fetch unit and part anew; remove rejected rows with the same ordered shift.

| C mask | Reject condition |
| --- | --- |
| 0x1 | Part flag0x80000, or battleGetUnitMonosiriPtr(kind)==null, or its word+4==0; the nonnull branch calls that lookup a second time |
| 0x2 | U.flags104&4 |
| 0x4 | U.flags104&2 |
| 0x8 | U.flags104&4 |
| 0x10 | !(U.flags104&6) |
| 0x1000 | P.flags1ac&0x100000 |
| 0x2000 | P.flags1ac&0x200000 AND NOT (C&0x100000 AND ID==actorID) |
| 0x4000 | P.flags1ac&0x400000 |
| 0x8000 | !(P.flags1ac&0x400000) |
| 0x2000000 | U.flags104&0x20 |
| 0x4000000 | P.flags1ac&0x2000000 |

Do not name unknown attribute bits after guessed UI labels. C0x100000 alone
has no separate admission action; it provides the explicit self exception above.

## Same-unit part and occlusion passes

When A&0x100000, a row whose part has flag0x2 is removed if another retained
row of the same signed unit ID has part flag0x1. Self row is excluded by row
pointer comparison. The other part is fetched through registry and part helpers;
this operates after both admission passes, so removed parts do not block peers.

Next, for each row R, fetch its part even when C's occlusion bit is absent.
When C&0x1000000 and R.part lacks0x40, scan other retained rows S in current
order. S blocks R only when all these conditions hold:

- Distinct row pointers and equal signed byte row+0x1d lanes.
- R.signedWord+c > S.signedWord+c after multiplying both by-1 when direction<0.
- S.part lacks0x40 and0x400000 (fetched with BattleGetUnitPartsPtr).
- Unless actor.flags27c has0x10 AND fetched actor pointer equals S's unit pointer.

The first blocker removes R with the same shift/recheck. Rows already removed
are absent from later scans. This is ordering over retained sort coordinates,
not nearest visible target or target-hit X. Equal sort X does not block.

## Final publication and permutation ordering

Store low8(N) to count. Initialize permutation[i]=low8(i) for signed count>0.
Sort ONLY permutation by nested i=0..N-2,j=i+1..N-1 comparisons. Compute each
signed word key=wrap32(10*row.bias + S16(row.hitX)). For direction>0 swap
when key(i)>key(j); for direction<=0 swap when key(i)<=key(j). Thus equal keys
retain order on positive direction but swap on zero/negative direction. This
is the concrete nested exchange algorithm, not a stable descending library sort.
Physical rows remain in their filtered construction order. Selection index is0.

## Actual takeoff specialization and implementation checks

Takeoff passes null weapon,A0x01001240,C0x01000000 and signed(-sideByte).
Relevant A tests are kind218..221 exclusion, actor ID exclusion and same/neutral
belong inclusion via0x200. Actor.flags27c&0x10 swaps0x200 to0x100 and returns1;
that enables opposite-belong admission. C enables the occlusion pass. There is
no weapon dereference in the sampler, so the null weapon is valid here.
The [chooser](BATTLE_ENEMY_CANDIDATE_CHOICE.md) mode4 then selects deterministically.

Acceptance cases should exercise swap/no-swap Q independent of count, high-bit
count-1 preserving prior selection, signed byte count, each reject bit and
self exception, ordered compaction, base/hit/sort coordinate separation, lane
and equal-X blocking, actor pointer exception, positive versus nonpositive equal
key ordering, and unused padding. Source writers, full status/numerical/helper
contracts, valid capacity/domain proofs, selection UI and original traces remain
open. This closes local sampling semantics without establishing encounter timing.

## Monosiri source clarification (2026-10-13)

The [corrected Tattle table/consumer contract](MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md)
confirms the actual pointer-null test and fresh unit-kind reload/second lookup
in C-mask1. Its field4 is a battle-help message-key pointer, not a spatial-sound
resource. The original admission table above remains correct; earlier lookup
prose dismissing the pointer check is superseded. Table/leaf/event-key bodies
are now delivered, while source mutation/kind domains and runtime stay open.
