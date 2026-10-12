# Damage request orchestration and caller-visible results

US G8ME01 revision0, recovered 2026-10-12 from `BattleCheckDamage` at
0x800fb01c, size0xa00. B evidence below is original US instructions/data;
names are public symbol annotations. The pinned public battle_damage.c only
contains this function's initial null-target/current-part handling. See
[baseline and provenance](../../ORIGINAL_BEHAVIOR.md). Local disassembly is ignored
at artifacts/reference/behavior/BattleCheckDamage.{disasm,annotated}.
No original execution or Switch-version equivalence is claimed.

This contract covers all local branches. It composes the separate
[counter](BATTLE_COUNTERS.md), [calculator](BATTLE_DAMAGE_CALCULATION.md),
[status](BATTLE_STATUS_INFLICTION.md), [direct application](BATTLE_DAMAGE_APPLICATION.md)
and [event dispatch](BATTLE_EVENT_DISPATCH.md) contracts. Called audience,
effect, action-command and weapon callbacks still need their own contracts.

## Interface and execution order (B)

Inputs `(A,D,P,W,F)` are attacker, defender, defender part, weapon and flags
by value. F below is a mutable local copy. K is a fresh48-byte counter record,
initialized by BattleInitCounterPreCheckWork. H=0, statusBits=0, result=−1,
partReaction=0 before the main paths. Q is assigned by the FP calculation.
Global battle pointer is captured for action-command stop.

If D is null return18, after initializing K. If P is nonnull store it at
D+0x258. There is no corresponding fallback part lookup in this function.
Later paths dereference P, W and A; the null-attacker tolerance of the counter
selector does not make this whole caller null-attacker-safe.

Order after this preparation:

1. Superguard branch, which exits before counter selection.
2. Counter selection; ignore its register return, inspect K+00.
3. If K+00≠0: part counter reactions, ordinary guard feedback, part flags,
   Pikkyoro helper, HP then FP calculations, status requests, event-code
   rewrites, special event exits, audience classification, reaction pattern,
   and direct defender damage.
4. Increment attack sequence bookkeeping, including on counter-only paths.
5. If K+24≠0, apply accumulated counter effects and return−1; otherwise return
   the result selected above.

There is **no BattlePreCheckDamage call here**. Callers must preserve the
separate precheck protocol; do not silently insert another accuracy/RNG check.
No loop repeats the ordinary path: the backward branch at0x800fb7a8 is its
single conditional entry after counter selection, not an iteration.

## Superguard branch (`0x800fb080`–`0x800fb29c`)

Enter only when W special flags+0x74 lack0x20000,
BtlUnit_GetACPossibility(D)≠0, and defence result equals5. Short-circuit in that
order. Spawn `effNiceEntry(2,hitX−60,hitY+45,hitZ−10)` using D/P hit position;
write float0.75 at returned effect payload+0x1c. OR0x80000 into F, play
SFX_SYSTEM_GREAT1, call BattleAudience_Case_GuardGood. Increment word A+0x280
and copy its low byte into D+0x273, before direct damage or attacker reaction.

Let m=U8(W+0x13), pattern=U8(W+0x6d). Every row first calls
`BattleDamageDirect(U32(A+0),D,P,0,0,C,pattern,0)`:

| m | Local F update / defender code C | Further action / return |
| --- | --- | --- |
| 2 | no extra F bit; C=F OR42 | return21 |
| 3 | F OR=0x100; C=F OR42 | return21 |
| 4 | F OR=0x100; C=F OR42 | If A+0x2d0 nonnull, BattleRunHitEventDirect(A,311,pointer); otherwise attacker direct call below; return−1 |
| All other byte values | F OR=0x100; C=F OR42 | attacker direct call below; return−1 |

Attacker direct call is `BattleDamageDirect(−5,A,null,1,0,311,0,1)`.
No normal K-based counter processing occurs in any of these rows. Do not
assume every superguard returns21 or always damages the attacker; m selects
these distinct responses. Weapon bindings giving m its gameplay meaning remain
an explicit data-inventory dependency.

## Counter selection and defender reaction exits (`0x800fb2a0`–`0x800fb340`)

Call BattleCheckCounter(K,A,D,P,W,&F). If K+00=0, skip the whole ordinary path
and join sequence/counter processing with H still0 and result still−1.

Otherwise, if F has0x100, inspect P+0x1b0 counter attributes C:

- C has0x2000: BattleRunHitEvent(D,1817); result=18; join sequence/counters.
- Else C has0x1000: same exit if W element+0x6c is3,4 or1, or if W+0x74
  has0x400 and CheckStatus(A,8) is true.
- Otherwise continue.

The0x2000 row precedes0x1000. These event exits happen before guard feedback,
calculators, D+0x260 increment and weapon status calls. A counter can still
run afterward because they join the common tail.

## Ordinary guard and part reactions (`0x800fb344`–`0x800fb458`)

If W+0x74 lacks0x20000 and GetACPossibility(D)≠0, read defence result again.
If it is signed greater than3, call effNiceEntry with type0 and the same
position/payload constants as superguard; F OR=0x40000, play SFX_SYSTEM_NICE1,
and call GuardGood. Otherwise call GuardBad. If either gate fails, neither
audience guard callback runs. The earlier equality5 test and this greater-than3
test are separate reads; preserve their ordering and exact comparisons.

Then evaluate all these part reaction pairs:

| W+74 mask | P+1ac mask | Action when both present |
| --- | --- | --- |
| 0x1000 | 0x1000 | partReaction=1; F OR=0x200000 |
| 0x2000 | 0x2000 | partReaction=1; F OR=0x200000 |
| 0x4000 | 0x0800 | partReaction=1; if F has0x100, F OR=0x400000 |

The third pair sets partReaction even if F lacks0x100. Numeric mask pairings
are authoritative; this contract does not infer a friendly physical reaction
name without the data bindings and downstream events.

## Calculation and status ordering (`0x800fb45c`–`0x800fb504`)

Call BattleCheckPikkyoro(W,&F), specified below.
Then `H=BattleCalculateDamage(A,D,P,W,&code,F)` and
`Q=BattleCalculateFpDamage(A,D,P,W,&code,F)`, sharing one code location.
HP initializes that code; FP may rewrite it. Increment signed halfword D+0x260
with16-bit storage narrowing **after both calculations**, before statuses.
Thus calculations see the preceding hit count, not the newly incremented one.

Call `statusBits=BattleSetStatusDamageFromWeapon(A,D,P,W,F)` exactly once if
F lacks0x40000 and either F has0x40000000 or F has0x100. Otherwise leave
statusBits=0. The0x40000 gate has precedence over both enabling bits.
Status application occurs before direct HP/FP mutation, but after the amounts
were calculated. Do not recalculate H/Q from the newly inflicted state.

## Ordered damage-code rewrites (`0x800fb504`–`0x800fb5b8`)

Each assignment below builds from **F**, not from the preceding code; OR
operations do not clear any low bits already present in F. Conditions testing
code low byte refer to its current value at that step:

1. If `(code&255)==30` and partReaction≠0, code=F OR23.
2. If `(code&255)==30` and W pattern+0x6d is19 or27, code=F OR23.
3. If `(code&255)==30` and statusBits≠0, code=F OR23 when statusBits has1,
   otherwise code=F OR22.
4. If statusBits has4, code=F OR28, irrespective of preceding code.
5. If statusBits has8, code=F OR29, overriding the previous step.

Do not replace this with a single exclusive classification or assume code's
low byte necessarily equals the literal OR operand for arbitrary caller flags.

## Special events and retained RNG draw (`0x800fb5bc`–`0x800fb660`)

If F has0x100 and statusBits has0x10: clear D+0x274, set local event=0x717,
pass its address to _checkDamageCode_EmergencyRevival(D,&event), then
BattleRunHitEvent(D,event). Set result18 and join sequence/counters. Skip
ordinary direct damage, audience classification and reaction-pattern selection.

Otherwise, if F has0x100, W+0x74 has0x80, and CheckStatus(D,9) is true,
call irand(100). Only a **signed negative result** enters an event branch:
event=0x8717, EmergencyRevival(D,&event), RunHitEvent(D,event), result18,
then common tail. This branch does not clear D+0x274 here.

The [recovered RNG](../meta/RANDOMNESS.md) returns0–99 for this bound, making that
negative-result event path unreachable under those helper semantics. The draw
still executes whenever its three gates pass. Removing the apparently inert
block would alter subsequent RNG outcomes. This is a static deduction, not an
observed Freeze shatter probability or a guessed intended percentage.

## Result classification and audience (`0x800fb664`–`0x800fb6dc`)

If statusBits=0, H=0, Q=0 and partReaction=0, result=19. For signed kind ID
D+8 in1..215, call EnemyNoDamage unless W pattern equals27.
Otherwise result=18. For kind1..215, call EnemyDamage(H) only if H>0 or
statusBits≠0. Thus nonzero FP damage or a part reaction can yield18 without
EnemyDamage; negative H with zero statusBits also does not satisfy that call.
Classification uses calculated amounts, before downstream clamping/suppression.

## Reaction pattern and defender application (`0x800fb6e0`–`0x800fb79c`)

Start M=U8(W+0x6d). If M=20 and K+24≠0, change M to0.
For M=14 or16, do the following in order:

1. If D+0x104 has0x4000, set M=0.
2. Always draw irand(100), even if step1 cleared M. Read unsigned byte
   susceptibility at `(U32(D+0x10))+0x8c`; if draw≥byte, set M=0.
3. If F lacks0x20000, set M=0.

Call `BattleDamageDirect(U32(A+0),D,P,H,Q,code,M,sleepRecovery)` where
sleepRecovery=0 if statusBits has0x20, otherwise1. Ignore its register return.
Even result19 reaches this direct application call: zero amounts can still
produce presentation, bookkeeping and hit events. Its own status/HP/event
semantics are specified separately. It does not determine this caller's result.

## Common sequence and counter tail (`0x800fb7ac`–`0x800fba18`)

Increment A+0x280 as a word; copy low byte to D+0x273. This occurs after the
ordinary direct defender call, unlike superguard's earlier increment. If
K+24=0, return result. Otherwise run the [counter consumer](BATTLE_COUNTERS.md):
three separately rounded half-H contributions, ordered attacker status requests,
zero-only amount fallback, audience notification, optional explosion, action
command stop, direct attacker damage and optional defender event; return−1.

Complete explosion bindings for K+18≠0: obtain D/P hit position, call
`effBombEntry(0,x,y,z,1.0)`, play SFX_BTL_THUNDERS_ATTACK4 at that position,
then BattleFogForceStop. Stop uses the global battle pointer captured at entry;
BattleAcHelpSetDispType(0) follows before direct attacker damage.

Original presentation constants: binary32 at0x804221b0=60,b4=45,b8=10,
bc=0.75,c0=1 (suffixes hexadecimal). Sound keys are at0x802ee064
SFX_SYSTEM_GREAT1,0x802ee078 SFX_SYSTEM_NICE1,0x802ee08c
SFX_BTL_THUNDERS_ATTACK4. Effect internals remain outside this contract.

## Guard eligibility helper (B)

BtlUnit_GetACPossibility at0x8012737c,size0xb8 returns0 on the first of:
signed byte unit+0x0c≠0; kind word unit+8=223; CheckStatus(unit,1), then2,
then9, then27 returning nonzero. Otherwise return1 iff word unit+0x138 lacks1.
There is no null-unit guard in this routine and no HP/weapon test here.

Its final call BtlUnit_CheckStatusFlag at0x801277f8,size0x1c returns0 for null
unit, otherwise `(U32(unit+0x138)&requestedMask)`, without boolean normalization.
GetACPossibility normalizes the mask1 result through a count-leading-zeros test.
These local checks do not establish every action-command eligibility rule in
other callers or the timing of defence result updates.

## Pikkyoro selection helper (B)

BattleCheckPikkyoro at0x800fde2c,size0x1a8 receives `(W,optionalFlagsPointer)`.
Start local flags from the pointed word, or0 if pointer is null; initialize
five candidate words by copying20 zero bytes from US0x802ee050. If W+0x74
lacks0x800, leave candidate count0. Otherwise inspect global battle word
+0x163f4, in ascending mask order0x100,0x200,0x400,0x800,0x1000. Each set bit
sets corresponding candidate[0..4]=1, increments count and remembers its index.

- Count0: preserve local flags; no random draw.
- Count1: use the remembered sole index; no random draw.
- Count≥2: draw irand(count). Scan indices4,3,2,1, subtracting that candidate's
  0/1 from the draw until negative, and select that index; otherwise select0.
  Thus enabled candidates are selected in descending order, one rank each.

OR the corresponding mask0x01000000,0x02000000,0x04000000,0x08000000 or
0x10000000 into local flags. Existing bits are never cleared. If the pointer
is nonnull, write the flags back; always return local flags. A null pointer
does not disable selection or its RNG draw. W/global state must still be valid.
The main caller uses the pointer write and ignores the register result.
Global flag producers, gameplay names and full downstream consumers remain
open; the Japanese helper name alone is not a behavioral explanation.

## Caller result summary and verification obligations (B/P)

| Path | Return |
| --- | --- |
| Null D | 18 |
| Superguard m2/m3 | 21 |
| Other superguard responses | −1 |
| Common tail with K+24≠0 | −1, regardless of prior result |
| Ordinary special event exits with no counter | 18 |
| Ordinary path with all amounts/status/reaction zero, no counter | 19 |
| Other ordinary path, no counter | 18 |

K+00=0 with K+24=0 would retain−1; the documented fresh selector's early-hazard
rows set a nonzero counter code, so this is not an extra ordinary miss category.

Derived differential cases: superguard m2/3/4/default; counter-only path skips
calculators; part reaction event before guard; both code override bits4/8;
status suppression overriding enable flags; hit count visible to callbacks;
zero-amount direct events; FP-only audience behavior; negative H; inert Freeze
branch consuming one draw; pattern immunity still consuming one draw; counter
result overriding defender18/19; sequence byte wrapping at256. These are
proposed original/native comparison cases, not executed-original observations.

Remaining dependencies: global selection-flag producers, all audience/effect
implementations, weapon bindings/callbacks, battle turn callers, death/revival
scripts and item consumption. Local orchestration is documented; this does not
establish complete battle or complete-game equivalence.
