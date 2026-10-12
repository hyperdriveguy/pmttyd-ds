# Candidate choice: ordered weights, deterministic maximum and RNG

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Static complete BattleChoiceSamplingEnemy800febc8/9d8, irand80005da0/44
and rand8026c324/20. Read [sampler](BATTLE_ENEMY_CANDIDATE_SAMPLING.md) for
buffer layout/admission/permutation and [takeoff](BATTLE_JUMP_TAKEOFF_POSITION.md)
for the actual null-weapon mode4 binding. No original execution claim.
Verification: `python3 tools/verify_enemy_candidate_sampling.py`.

## Output initialization and weight ownership

Arguments (B,flags,unitOut,partOut). Capture shared BattleWork W, then write
unitOut=-1 followed by partOut=0. If S8(B+a6c)<=0, return without any unit
lookup, weight construction or RNG call. There is no specified scalar return;
consumers must use the count/outputs, not a guessed success value.

For positive count fetch actor through word(B+ab8), retain its pointer, then
write initial outputs from physical row j=S8(B+a6d): S16(B+4+36*j+0) to
unitOut, then S16(row+2) to partOut. Only afterward initialize74 signed word
weights to0 and add10 to weight[j]. No permutation/count/index/null validation.
Walk rank i from0 while i<S8(B+a6c), reloading count at each loop test.
Set j=S8(B+a6d+i), fetch that physical row's unit, add100 to weight[j], then
apply every enabled modifier below in order. Weight arithmetic/additions use
32-bit words. Each ordinary floating update converts signed current weight by
biased double subtraction with single-rounded fsubs, performs the shown single
operations, fctiwz to signed word, and stores before the next modifier. Do not
multiply all factors together and truncate once. Constants are binary32.

## Flags and ordered modifiers

Let H=S16(candidate+10c), M=S16(candidate+108), i=rank, N=S8(B+a6c).
`single` denotes round to binary32; `FMA` and `FNMA` use one fused rounding.
C0.1=0.10000000149011612, C0.4=0.4000000059604645,
C0.05=0.05000000074505806. Each table result undergoes fctiwz immediately.

| flags mask | Condition and new weight w |
| --- | --- |
| 0x1 | If signed kind222: single(1.5*float(w)) |
| 0x2 | If signed kind224..230: single(1.5*float(w)) |
| 0x4 | single(float(w)*single(C0.4+single(C0.1*float(N-i)))) |
| 0x8 | single(single(0.5+single(C0.1*float(i)))*float(w)) |
| 0x10 | If actor/candidate signed belong bytes equal: single(1.5*float(w)) |
| 0x20 | If belongs differ AND candidate belong!=2: single(1.5*float(w)) |
| 0x100 | single(single(2.0-single(float(H)/float(M)))*float(w)) |
| 0x200 | single(single(1.0+single(float(H)/float(M)))*float(w)) |
| 0x400 | single(FNMA(C0.05,float(H),1.5)*float(w)) |
| 0x800 | single(FMA(C0.05,float(H),1.0)*float(w)) |
| 0x1000 | If H<=U8(word(candidate+10)+0xd): single(1.5*float(w)) |

Unlisted flags have no direct weight modifier. 0x80000000 controls the final
choice path below. There is no denominator-zero, negative-weight or floating
conversion-domain check. Numerical extremes require original FPSCR/fctiwz
contracts; host rejection/clamps must be labeled port policy.

## Weapon-specific overwrite after all flags

If B.weapon==null, skip this entire section (valid for takeoff). Otherwise if
candidate.flags104&0x20000, overwrite weight1, then CONTINUE into weapon-item
dispatch; later branches can change it again. Item is signed word weapon+8.
Use GetStatus calls in listed order and skip later queries when the branch
condition is already established:

| Item | Override weight1 when |
| --- | --- |
| 0 | Never; jump directly to next candidate |
| 142 | CheckRecoveryStatus(candidate)==0 OR GetStatus(candidate,10)!=0 |
| 146 | GetStatus(candidate,11)!=0 |
| 147 | GetStatus(candidate,15)!=0 |
| 143 | GetStatus(candidate,14)!=0 |
| 134 | GetStatus(candidate,18)!=0 |
| 135 | GetStatus(candidate,6)!=0 |
| 136 | GetStatus(candidate,7)!=0 |
| 137 | GetStatus(candidate,5)!=0 |
| 153 | ALL GetStatus(candidate,s)==0 for s=1,2,3,4,5,8,9,11,15,20 |

For every other item, read U8(ItemData[item]+0x1e), table base0x803108a8,
stride40, with no index check. Zero leaves the existing weight. Nonzero:
request GetMaxFp(candidate), then GetFp(candidate). If FP<maximum, request
both again, calculate signed word deficit=wrap32(maximum-FP), and set
w=trunc(single(float(w)*FMA(C0.1,float(deficit),1.0))). This retains repeated
queries and fused addition. Otherwise reread B.weapon and its signed item,
look up the table again, and overwrite1 ONLY if both unsigned bytes+0x1d
and+0x1f are zero (the second is queried only when the first is zero).
Partner FP getters follow [shared ownership](BATTLE_WEAPON_COST_AND_PAYMENT.md).
Lookup/status/resource service internals are separate dependencies.

## Deterministic selection, maximum and ties

When flags lacks0x80000000, scan ALL74 physical weights ascending, starting
bestIndex=0,bestWeight=0. Replace both only when signed weight>bestWeight.
Thus positive ties choose the lower physical row index, not earlier permutation
rank. If none is positive, choose physical row0 even if its weight is zero or
negative. Read row+0/+2 as signed16 and write unitOut then partOut. Unused
weights remain0; there is no restriction of this scan to candidate count.
The buffer and permutation are not mutated by this chooser.

Actual takeoff uses flags4 and null weapon. It applies the rank factor only,
adds the initial first-permutation bonus, and follows this deterministic branch.
It never calls irand/rand. A nearest-screen-target substitution is unsupported.

## Weighted draw and shared random state

When flags&0x80000000, sum ALL74 weights with word arithmetic; call irand(sum).
Scan weights ascending physical indices with retained result R: at each index
replace R=wrap32(R-weight[index]); stop at the first signed R<=0. If all74
subtractions remain positive, the loop reaches index74 and reads that physical
row without a validity guard. Write the selected signed16 unit then part.

This is the original subtraction boundary, not a conventional R<0 chooser.
For draw0 it selects physical row0 even when that row has weight0. At a positive
prefix boundary it chooses the row ending that prefix. Preserve these cases.
No negative-weight/sum overflow validation or candidate-count clamp occurs.

irand(0) returns0 WITHOUT consuming random state. For nonzero signed word n it
calls rand once and returns signed remainder r-(r/n truncated toward zero)*n
using divw/mullw/sub. Negative n is not locally rejected; valid positive sums
produce0..n-1 with modulo bias. rand maintains shared word state at SDA13+14a0
(0x8041e3c0): state=wrap32(state*0x41c64e6d+0x3039), return
(state>>16)&0x7fff. Seed initialization/writers and call interleaving remain
open; neither a new per-choice seed nor a uniform rejection sampler is original.

## Implementation acceptance and limits

Cover empty outputs(-1,0), output alias order, initial bonus and physical versus
permutation tie ownership, every modifier with intermediate truncations, HP0/
invalid-domain policy, null weapon, item status short circuit, flag overwrite
followed by later weapon modification, repeated FP/table queries, deterministic
mode4 with zero RNG consumption, draw0/prefix boundary, zero-sum no consumption
and LCG word wrap. These complete local contracts do not establish selection
UI producers, valid target/domain limits, battle cadence or original traces.
