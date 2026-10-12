# Direct damage application and reaction dispatch

US G8ME01 revision 0, recovered 2026-10-12 from `BattleDamageDirect`,
`0x800fdfd4`, length `0xb24`. B evidence throughout is original instruction/
data inspection; friendly names are public symbol/header annotations (R).
Identity and source boundaries are in [ORIGINAL_BEHAVIOR.md](../../ORIGINAL_BEHAVIOR.md).
This specifies the function's ordered local behavior and explicit helper
interfaces. Helpers' internal behavior is not assumed from their names.
No original gameplay execution or Switch behavior was measured.

The pinned [public damage source](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/battle/battle_damage.c)
does not implement this US routine. Ignored local artifacts are
`artifacts/reference/behavior/BattleDamageDirect.{elf,disasm,annotated}`.

## Arguments and order

Register arguments are `(attackerID,D,P,H,Q,C,M,R)`:
defender unit, optional part, signed HP amount, signed FP amount, combined
damage/event code, reaction-pattern number, and a sleep-recovery enable value.
Negative amounts are not rejected. There is no null-defender guard.
The local code C can change; it is passed by value, not returned through a
pointer. The final action is `BattleRunHitEvent(D,C)`; do not interpret this
function's register return as a separately computed damage result.

The execution order is:

1. Resolve attacker; clamp HP amount and update attacker accumulator.
2. Resolve part, store/normalize pattern, obtain presentation positions.
3. Spawn hit/miss/recovery effects.
4. Apply Pity Flower recovery and its effects.
5. Select damage sound.
6. Store amount bookkeeping, mutate HP, mutate FP.
7. Update action records and possibly request recovery from Sleep.
8. Notify audience, modify lethal-event flags/pattern, dispatch hit event.

Presentation, RNG, and accounting are not all conditional on actual HP loss.
Changing this order changes observable behavior and resource outcomes.

## Attacker, amount and target preparation (`0x800fdfe4`–`0x800fe0d8`)

If attackerID is −5, use a null attacker. Otherwise call
`BattleGetUnitPtr(globalBattle,attackerID)`. Clamp H to [−99,99]; Q has no
equivalent clamp. If the resolved attacker exists, add clamped H to its word
+0x284, before checking any defender damage suppression or current HP.

If P exists, store it at D+0x258. Otherwise resolve
`BtlUnit_GetPartsPtr(D,BtlUnit_GetBodyPartsId(D))`, use that as P and store it.
Write M to D+0x274. If `U32(D+0x104)&0x00200000` and the stored pattern is
21,22,23 or24, clear D+0x274. Later presentation tests use the original
argument M, not this normalized field. Fetch `BtlUnit_GetHitPos(D,P)` into
an initial position used later for sound.

## Hit, miss and absorption presentation (`0x800fe0dc`–`0x800fe4d4`)

If C has `0x2000`, fetch a new hit position and call
`effRecoveryEntry(type=0,amount=abs(H),x,y,z)`, then skip ordinary hit/miss
effects. This flag alone does not change the sign of H or prevent later HP
subtraction. Normal absorption depends on the calculator supplying negative H.

Otherwise, if either H or Q is nonzero, fetch another hit position and select
effects by `C&0xff`. Below, the call parameters after x/y/z are the original
numeric values, not inferred effect meanings:

| Low code | Effects in order |
| --- | --- |
| 24 or25 | `effKemuri12N64Entry(0,x,y,z,1.0,30)`, then `effHitEntry(1,x,y,z,1.0,3)` |
| 26 | `effIceN64Entry(0,x,y,z)`, then `effHitEntry(3,x,y,z,1.0,3)` |
| 27 | `effHitEntry(2,x,y,z,1.0,3)` |
| Other | `effHitEntry(0,x,y,z,1.0,3)` |

If H is nonzero and D+0x104 lacks `0x00400000`, emit HP damage-star type0
with amount H. If Q is nonzero, emit FP damage-star type1 with amount Q.
There is no corresponding HP-star suppression test for the FP star. Position
and remaining float parameters depend on `S8(BtlUnit_GetBelong(D))` (the getter returns U8;this caller explicitly sign-extends):

| Belong / star | Position | Fourth / fifth float |
| --- | --- | --- |
| 0, HP | (x−8,y+5,z) | 10 / 30 |
| 0, FP with H≠0 | (x−8,y+5,z) | 10 / 60 |
| 0, FP with H=0 | (x−8,y+5,z) | 10 / 30 |
| Other, HP | (x,y+10,z) | 10 / −30 |
| Other, FP with H≠0 | (x,y+10,z) | 10 / −60 |
| Other, FP with H=0 | (x,y+10,z) | 10 / −30 |

Then, if attackerID is not −5, attacker exists, attacker byte+0x2e7 is nonzero
(R: Mega Rush), and signed attacker HP ≤ its unsigned kind-data peril byte
+0x0d, fetch another hit position and call `effFukidashiEntry` with integer
arguments `(0,0,0,60)` and float arguments `(x,y,z,10,0)`. This does not test
whether Mega Rush modified this attack's power.

If instead H=Q=0, fetch another hit position. Suppress the miss star if original
M is any of {2,3,7,19,20,21,22,27}, or if `C&0xff` is22. Otherwise call
`effMissStarEntry` with type1 when C has `0x20000`, type0 otherwise; its other
integer arguments are 1 and direction (−1 for belong0, +1 otherwise), with
the hit position as float arguments.

Referenced binary32 constants are at RAM `0x804221b0`=60,
`0x804221b8`=10, `0x804221c0`=1, `0x804221cc`=30,
`0x804221dc`=8, `0x804221e0`=5, `0x804221e4`=−30,
`0x804221e8`=−60, `0x804221ec`=0. These are application-call parameters;
effect spawning, animation and lifetime still need their own contracts.

## Pity Flower before incoming FP subtraction (`0x800fe4d8`–`0x800fe58c`)

If C lacks `0x2000` and H>0, read unsigned byte D+0x2f8 (R: Pity Flower
count). Draw `irand(100)` once per copy, without stopping after success.
Count results strictly below30. When successes>0:

1. Call `BtlUnit_RecoverFp(D,successes)`.
2. Obtain unit position and integer height. Set displayed y to
   `positionY + (float(D+0x114)*height + 5)`. The multiply-add uses the
   original single-precision fused instruction.
3. Call `effRecoveryEntry(type=1,amount=successes,x,y,z)`.

The displayed amount is successes, not necessarily actual recovered FP at its
cap. HP-suppression flags do not skip these rolls; lethal incoming damage does
not skip them either. FP subtraction occurs later, so full FP can discard
this recovery before incoming FP damage creates space.

## Sound priority (`0x800fe590`–`0x800fe718`)

Original M19 or27 suppresses the entire sound block. Otherwise the first
matching override below plays one `psndSFXOn_3D` sound at the initial hit
position and skips the remaining sound logic:

| C mask, in priority order | US string address / original sound key |
| --- | --- |
| `0x01000000` | `0x802ee2e0` / SFX_MARIO_HAMMER_PIKKYO_R1 |
| `0x02000000` | `0x802ee2fc` / SFX_MARIO_HAMMER_PIKKYO_Y1 |
| `0x04000000` | `0x802ee318` / SFX_MARIO_HAMMER_PIKKYO_B1 |
| `0x08000000` | `0x802ee334` / SFX_MARIO_HAMMER_PIKKYO_G1 |
| `0x10000000` | `0x802ee350` / SFX_MARIO_HAMMER_PIKKYO_P1 |
| `0x40000`, only with belong0 | `0x802ee36c` / SFX_MARIO_GUARD1 |

Absent overrides: if H>0, choose sound pointers from D's kind data. Low code24
plays +0xa0 then +0x9c; code26 plays +0xa4 then +0x9c; code27 plays +0xa8
then +0x9c; all others play +0x9c only. Code25 therefore differs from24 here,
despite sharing the earlier smoke/hit effects. Afterwards, if Q>0 and belong0,
play `SFX_MARIO_DAMAGE_FP1` at `0x802ee380`. That FP sound is skipped by any
earlier override, not layered on top of it. Full sound-macro playback remains
an audio-system dependency.

## Amount records and HP mutation (`0x800fe71c`–`0x800fe7f4`)

Always perform these writes first:

- Store H's low byte at D+0x270; add its signed-byte interpretation to word
  D+0x264.
- Store Q's low byte at both D+0x271 and D+0x272; add its signed-byte
  interpretation to word D+0x268.

Skip the following HP block if D+0x104 has `0x40000000` or P+0x1ac has
`0x00800000`. Skipping it does not suppress the preceding effects/accounting,
subsequent FP block, audience logic, or hit-event dispatch.

Otherwise:

1. Save signed current HP (halfword D+0x10c) to word D+0x208.
2. Subtract signed byte D+0x270 from current HP and store a halfword.
3. Reload as signed16; if above signed maximum HP at D+0x108, store maximum.
   Then reload and clamp negative values to zero. The halfword store happens
   before these comparisons; preserve narrowing for out-of-range states.
4. Save final signed HP to word D+0x20c.
5. If signed D+0x270 is nonzero, copy halfword D+0x202 to D+0x200 and
   halfword D+0x206 to D+0x204.
6. Set bit1-value `0x1` in halfword D+0x1fe when C has `0x100`; clear it
   otherwise. The other halfword bits survive.

H is bounded, so the stored signed byte retains its clamped value. Q is not
bounded here and cannot be assumed to share that property.

## FP mutation and shared resource helpers (`0x800fe7f8`–`0x800fe844`)

Only enter this block if signed byte D+0x271 is nonzero. Get current FP via
`BtlUnit_GetFp(D)`. If current FP < original Q, store current FP's low byte at
D+0x272; otherwise store Q's low byte there. Compute `newFP=currentFP−Q`
using the original Q, floor negative results at zero, and call
`BtlUnit_SetFp(D,newFP)`. Add signed byte D+0x272 to word D+0x26c.

This preserves the distinction between requested FP (word accumulation at
+0x268) and the byte-sized consumed amount (+0x272, accumulated at +0x26c).
For example, Q=256 leaves the low byte zero and skips the FP mutation block;
Q=128 enters it, while its requested accumulator contribution is −128. These
are consequences for unrestricted arguments, not evidence shipped attacks use
those values. Negative Q can increase FP; this path has no upper-bound clamp.

The helpers were checked in the US binary:

| Helper | US address / size | Local behavior |
| --- | --- | --- |
| BtlUnit_GetFp | 0x80126c64 /0x44 | Return signed halfword +0x112 from resource owner |
| BtlUnit_SetFp | 0x80126c10 /0x54 | Store value's low16 bits to owner's +0x112; no clamp |
| BtlUnit_GetMaxFp | 0x80126b14 /0x44 | Return signed halfword +0x10e from owner |
| BtlUnit_RecoverFp | 0x80126b58 /0x94 | Add amount to owner's +0x112, store16, reload signed16, cap only if above signed maxFP; no lower clamp |

For kind IDs224–230 inclusive, the owner is `BattleGetMarioPtr(globalBattle)`;
otherwise it is the input unit. `BattleGetMarioPtr` at `0x800f87a8`, size0x6c,
scans indices0–63 through BattleGetUnitPtr and returns the first nonnull unit
with kind222 and signed belong byte+0x0c equal zero, or null if absent.
FP helpers dereference this owner without a fallback; preserve the valid-battle
invariant rather than inventing an independent partner FP pool.

## Action records and Sleep recovery (`0x800fe848`–`0x800fe96c`)

For H>0, kind222 calls `BtlActRec_AddCount(battle+0x16f3e)` then
`BtlActRec_AddPoint(battle+0x16f3c,U8(H))`. Kinds224–230 instead use +0x16f3f
and +0x16f3d. These helper calls use requested H even if HP was suppressed or
lower than H. [Complete byte helpers](BATTLE_ACTION_RECORD_BYTE_UPDATES.md)
now close storage/order:AddPoint stores low8 BEFORE its100 cap;AddCount leaves
an existing byte>=100 unchanged. Original record reset/source/persistence and
turn/general-rule ownership remain separate.

If R is nonzero, either H>0 or Q>0, and `BtlUnit_CheckStatus(D,1)` succeeds,
draw `irand(100)`. For draw<50, write byte1 to D+0x118 and byte100 to D+0x119,
then call `BtlUnit_CheckRecoveryStatus(D,1)`. Afterwards obtain unit position/
height and spawn `effFukidashiEntry` with the same parameters as above, but
y=`positionY+trunc(height/2)` (signed division toward zero). The status helper's
outcome is not tested before that effect. This is the recovery request path;
the [status recovery contract](BATTLE_STATUS_STORAGE.md) now specifies local
clearing and the HP/Instant Kill gates; its turn callers remain unresolved.

Draw order is all eligible Pity Flower copies first, then this optional Sleep
draw. Neither is gated on measured HP/FP loss. See [RANDOMNESS.md](../meta/RANDOMNESS.md)
for generator and modulo behavior.

## Audience, lethal flags and reaction (`0x800fe970`–`0x800feae0`)

For kind222, call `BattleAudience_Case_MarioBigDamage(S8(D+0x270))`, including
zero or negative amounts. Then read signed current HP.

If HP>0, clear stored pattern D+0x274 only when it equals25, then proceed to
final dispatch. If HP≤0, set HP=0. If C lacks `0x100`, proceed directly to
final dispatch without the lethal flag/pattern processing below. Otherwise:

1. OR `0x200` into C.
2. If D+0x104 has `0x10000`, OR `0x400` only for low codes
   {24,25,46,49,50,54}; do not call the revival helper on this branch.
   If that unit flag is absent, unconditionally OR `0x400` and call
   `_checkDamageCode_EmergencyRevival(D,&C)` (may OR `0x800`).
3. If stored pattern is25, retain it only if C has `0x400`, lacks `0x800`,
   has `0x100`, and the byte at `(pointer D+0x144)+0x10` is nonzero.
   Otherwise clear stored pattern. The pointer's complete schema/lifetime
   remains unresolved.
4. If C has `0x400`: for kinds224–230 call
   `BattleAudience_Case_PartyDown`; for kinds1–215 call
   `BattleAudience_Case_KillEnemy(U8(D+0x0d))` then
   `BattleAudience_Case_FinalAttack`. These are distinct kind ranges.

On every path reaching the end, OR1 into word D+0x27c, then call
`BattleRunHitEvent(D,C)`. Original C may already contain lethal-related bits;
this routine generally adds bits, rather than sanitizing all caller flags.
The [dispatch contract](BATTLE_EVENT_DISPATCH.md) specifies replacement,
cancellation, scheduling and script access to the stored code.
The [revival marker](BATTLE_DAMAGE_CALCULATION.md) does not itself consume
items or restore HP. Those actions and actual death/removal belong to the hit
event graph and later services, still to be recovered.

## Verification obligations and remaining dependencies (P)

Derived cases: H outside ±99; absorption with negative/zero/positive H; HP
suppression with live effects/accounting/FP loss; simultaneous HP/FP damage at
full FP with Pity Flower; exact 29/30 and49/50 random thresholds; independent
partner FP access; negative and byte-boundary Q; overkill accounting; code25
sound vs visual differences; original vs normalized pattern; all lethal-kind
range endpoints, special unit flag, and preexisting event bits.

These are proposed differential tests, not runs against the original program.
The local instruction/data verifier checks selected anchors and constants,
not semantic equivalence of the entire function. Still open: status recovery,
unit/part resolution invariants, hit-position semantics, effect/audio internals,
action-record helpers, audience state, all hit-event handlers and complete
damage orchestration. Do not report the full battle system complete from this
application contract alone.


## Complete action-record/first-process follow-up (2026-10-12)

[Byte accounting](BATTLE_ACTION_RECORD_BYTE_UPDATES.md),
[turn-rule sums/status writes](BATTLE_ACTION_RECORD_TURN_RULES.md) and
[first-process/fog requests](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md) now close
local record helpers,judgment-before-counter order and64-slot preparation.
AddPoint narrows before its cap;count/halfword/unit-byte rules differ. General
rule/action/reset/persistence/source/turn/backend and original traces remain open.
