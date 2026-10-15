# Packed battle state and candidate records

These modules operate on guest fields through injected services. Hexadecimal
numbers below are byte offsets/masks unless a decimal count or source ID is
stated. [Battle entry](battle-entry.md) describes allocation, [battle events](battle-events-rules.md)
describes dispatch, and [packed motion](packed-battle-motion.md) describes vector
and movement fields. Original recovery evidence remains in the
[behavior contracts](../behavior/README.md). Native admission is not original
runtime/hardware fidelity.

## Status, move counts, recovery and pad history

[battle_packed_status.cpp](../../source/battle/battle_packed_status.cpp) narrows
status ID through signed low byte before selecting its exact field table.
IDs 25/26 and out-of-domain narrowed IDs return empty. IDs 10–15 share duration/
strength pairs: even IDs need positive signed strength, odd IDs negative; a sign
mismatch skips duration read. IDs 4/23/24 read two bytes; ordinary IDs return
(duration,duration). Optional status outputs both capture inputs before writing
duration then strength, so aliased destinations retain the latter. `check`
handles a null unit and additionally admits status 6 from badge byte `2f7`;
`get` does not synthesize that duration or guard a null unit.

[battle_packed_move.cpp](../../source/battle/battle_packed_move.cpp) resets state
`20`, copies base `21` to remaining `22`, applies status-19 doubling with byte
wrap, then status-20 signed halving/turn-threshold logic. For remaining <2, the
threshold is four when signed base <1, otherwise two. Failure sets state 10 and
zero remaining. Snapshot `23` is written last; only signed snapshot ==0 forces
that final exhaustion branch, not every negative byte. Turn byte `24` belongs
to source move accounting, not rendering.

[battle_packed_recovery.cpp](../../source/battle/battle_packed_recovery.cpp)
adds with word wrap, narrows to signed halfword, then upper-clamps HP/FP. It
has no lower clamp or HP-gauge bookkeeping. HP current/max are `10c/108`;
FP current/max `112/10e`. Partner kinds 224–230 share Mario's FP. Max-FP lookup
still reads own maximum before redirecting. Recovery captures battle, obtains
Mario for maximum, then obtains Mario again for current-FP write ownership;
helpers can independently recapture globals. Do not collapse those reads into
one cached pointer across service-visible mutation.

[battle_packed_pad.cpp](../../source/battle/battle_packed_pad.cpp) copies the
private battle history, not physical polling. Two decimal-12-byte snapshots at
`0/c` hold buttons, signed main/sub axes, triggers and analog/retained A/B.
Four 30-word histories at `18/90/108/180` hold trigger/repeat/held/released;
repeat bytes are `1f8/1f9`. Each index accesses all four banks before advancing.
It does not tick history or serialize a native struct wholesale.

## Menu selection and hit-event routing

[battle_packed_selection.cpp](../../source/battle/battle_packed_selection.cpp)
uses signed count `B+e94`, equivalent to candidate buffer `B+428+a6c`.
Nonpositive count writes raw outputs (-1,-1). Positive count uses signed cursor
`ab7`, ranked byte index `a6d+cursor` and a 36-byte row's signed unit/part halves.
The second half loads after the first output callback; outputs are not an atomic
snapshot.

Command weapon selection captures one battle for menu `171c` / type `1c5c`,
while the cursor helper independently reads current global acting unit `420`
before validating type. Source types 0–8/10/14 have exact cursor offsets;
5/14 distinguish Mario kind 222. Others return unavailable, not weapon zero.
Rows have decimal stride 28 and weapon pointer `80`. Nonplayer held item `308`
uses original item table `803108a8`, decimal stride 40, pointer `24`. Engaged
weapon zero remains a real stored value; `nullopt` is the unsupported domain.

[battle_packed_hit_event.cpp](../../source/battle/battle_packed_hit_event.cpp)
separates ordinary admission from direct replacement. Ordinary low-byte code
40 requires data key 46; missing script `2b0` also returns without touching old
events. Direct always removes/clears IDs `2b4/290/29c/2a8`, even with script zero.
Nonzero script creates priority-10 / flags-`20` root owned by unit ID, storing
code `278` and new ID `2b4`. No damage, death or battle conclusion is inferred.

[battle_packed_hit_precheck.cpp](../../source/battle/battle_packed_hit_precheck.cpp)
returns routing codes 1–6, not damage amounts or a Boolean. Ordered early
part/status/weapon checks can return before random work. Caller flag `100000`
bypasses only the probabilistic suffix. Per-badge 100-roll loops use thresholds
90/75/67, with the last gated by signed HP versus kind byte `c`. Status 7
without 1/2 has a 50 threshold. Attacker status 3 halves weapon accuracy;
active fog handle `182bc` applies loaded source coefficient and truncation.
The final 100-roll uses strict `<accuracy`. No HP is applied.

[battle_packed_counter.cpp](../../source/battle/battle_packed_counter.cpp) has
12 caller-owned words. `init` clears and sets word zero to 1; CheckCounter with
a nonnull attacker resets only count at `28`. Null attacker leaves it untouched.
Blocking conditions form a first-match chain; later nonblocking effects can
replace the final code while retaining prior gates/count. Fields `20/24` retain
code / code|`100`, `2c` pattern; caller flags OR `100`. Codes 43–54 map through
the exact return table, including code 48 returning zero. Count >0 is not a
substitute for that result table.

## Candidate memory layout and construction

[battle_packed_sampling.cpp](../../source/battle/battle_packed_sampling.cpp)
and [choice](../../source/battle/battle_packed_choice.cpp) share a guest/private
big-endian record view. Private storage has room for 74 rows; caller must admit
valid row indices, complete part chains and resources. Neither helper supplies
a capacity clamp, corruption repair or rollback.

| Offset | Content |
| --- | --- |
| `0` | Weapon pointer |
| `4 + 36*i` | Row `i` (decimal stride 36) |
| `a6c / a6d` | Signed count / ranked row bytes |
| `ab7 / ab8 / abc` | Cursor / actor ID word / side byte |
| `ac0 / ac4 / ac8` | Admission mask / attribute mask / direction byte |

| Row offset | Content |
| --- | --- |
| `0/2` | Signed unit/part halfwords |
| `4/6/8` | Hit XYZ signed halfwords |
| `c/10/14` | Sort XYZ words, derived through signed-16 narrowing |
| `18` | Forward/directional bias word |
| `1c/1d` | Forward bucket / lane byte |
| `20` | Source word initialized -1 |

Sampling publishes metadata before capturing actor/global state. Actor `27c`
mask `10` swaps exclusive admission `100/200`; scalar result 1 reports that
swap, not a candidate count. Admission `80000000` immediately writes count
byte `ff` (-1), preserving old cursor/rows/ranks.

Ordinary construction scans 64 unit slots and each kind's declared part count,
skipping status 27, unit attribute `40`, part mask `10000`. Geometry uses part
`188/194` layers, not hit helper `170/17c`. It can add part offset-Y `28` times
unit scale `114` to the world Y when unit attribute 2 is clear. Part facing `bd`,
forward `1a0` and optional `300*direction` feed sort bias. Preserve this formula
rather than replacing it with another seemingly equivalent hit-position helper.
Forward buckets use source minimum/offset/divisor, truncation and upper cap 48,
without lower clamp; negative direction maps `48-q`. Lane uses narrowed sort Z
strictly below -30, not hit Z.

Filtering compacts rows in separate passes with exact pairwise loads/stores.
Admission examples: `10000` Mario only, `20000` kinds 216–217 only; masks
`1/2/10/40/20` exclude source kind groups; `1000` excludes actor, `4000` requires
actor, `100/200` apply side/neutral rules and `2000` excludes matching actor kind.
Part masks under `100000/200000` select exact eligibility branches. Attribute
filters are a separate namespace: source Monosiri, unit/part state, same-unit
part preference and directional occlusion. Successful Monosiri admission queries
again with fresh kind; it does not reuse the first pointer.

Admission `100000` removes a mask-2 part when a same-unit mask-1 peer remains.
Attribute `1000000` checks same-lane directional blockers, with part `40/400000`
and swapped-actor exemptions. Blocker part lookup recaptures global battle even
though earlier unit lookups use captured battle. No guessed farther target
replaces an unavailable source provider.

Finally publish narrowed count and identity ranks, then execute the source pair
swaps on wrapped `10*bias + signed hitX`. Positive direction swaps `>`; zero/
negative swaps `<=`, including equality. This is not stable sorting, even when
positive direction never directly swaps equal keys. Do not use `stable_sort`.

## Weighted choice

Chooser captures battle and publishes (-1,0), then, for positive signed count,
publishes the first ranked row **before** computing weights. Output callbacks
may mutate live rows/ranks/count; unit write precedes part read. First row gets
bonus 10, each ranked occurrence adds 100, and a 74-entry array retains weights.

Flags select formulas using loaded original coefficients at `804221f0` and
explicit single-result arithmetic/truncation: Mario/partner/side preference,
rank bias, HP ratio/absolute HP and danger threshold (kind byte `d`, distinct
from precheck byte `c`). Weapon zero skips item overrides. Unit attribute
`20000` sets weight 1 before exact item-ID/status branches. FP-restoring metadata
can query max/current twice, or reload weapon/item metadata if already full.
No zero-maximum guard or inferred host heuristic is added.

Deterministic choice scans all 74 weights with initial best zero and strict `>`,
so ties favor first **row index**, not necessarily first rank. Default remains
row zero if no weight wins. Flag `80000000` instead sums all 74 with signed-word
wrap, draws from the provided bound, and subtracts until `<=0`. Draw zero can
select zero-weight row zero. Do not skip zero weights, change to `<0`, invent a
zero/negative-bound fallback, or collapse final live output reads.
