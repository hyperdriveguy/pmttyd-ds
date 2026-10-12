# Turn HP/FP regeneration and Happy badge recovery

US G8ME01 revision0, inspected 2026-10-12. B evidence is original instructions
and data in btlseqTurn, primarily0x8011db60–0x8011e1c4. Public names are
annotations, not proof of a rule. See [baseline](../ORIGINAL_BEHAVIOR.md) and
[turn recovery](BATTLE_TURN_RECOVERY.md). Original code/data extracts remain
ignored. No original execution, frame measurement or Switch equivalence claim.

## Phase order and common scan (B)

Channel3's verified dispatch maps these states, after Poison/Burn and before
status-duration recovery:

| State | Entry | Operation | Following wait |
| --- | --- | --- | --- |
| 0x0300000a | 8011db60 | status23 HP regeneration | 0b |
| 0x0300000c | 8011dd08 | status24 FP regeneration | 0d |
| 0x0300000e | 8011de64 | Happy Heart byte unit+0x2ee | 0f |
| 0x03000010 | 8011e01c | Happy Flower byte unit+0x2ef | 11 |

Each operation scans slots0..63 ascending. Skip null units, CheckStatus(unit,27)
nonzero, and signed current HP at unit+0x10c≤0, in that order. These are not
revival operations. Neither regeneration scan adds a shadow-guard check.
Each phase completes its slot scan before entering its event wait; it does not
wait for one unit's animation before processing the next.

The Happy phases have an additional gate **before the scan**: signed halfword
B+0 must be>1. Otherwise increment channel3 twice and recurse immediately into
btlseqTurn, skipping the associated wait. This is the literal gate; outer turn
counter initialization and progression still need recovery.

## Status23 HP regeneration (`0x8011db60`–`0x8011dd04`)

GetStatus(unit,23,&duration,&strength); require signed strength>0. Do not test
the returned duration or current-versus-maximum HP in this caller.

If attributes word unit+0x104 lacks0x40000000:

1. Store signed current HP as word unit+0x208.
2. BtlUnit_RecoverHp(unit,S8(strength)).
3. Store signed resulting HP as word unit+0x20c.
4. Copy halfword unit+0x202 to+0x200, then+0x206 to+0x204.
5. OR1 into halfword unit+0x1fe.

If that attribute bit is set, only the RecoverHp call occurs here; the listed
bookkeeping writes are skipped. Do not interpret this attribute as disabling
healing. Display-field consumers and producers remain separate dependencies.

Then run presentation and register the regeneration event described below.
Mark the phase changed even when HP was already full, the amount was capped,
or event allocation failed. Full HP does not suppress this path.

BtlUnit_RecoverHp at0x80126bec,size0x24 adds the supplied word amount to signed
halfword currentHP, stores the low16 bits, reloads signed16, and caps to signed
halfword maxHP(+0x108) only if the reloaded value is greater. There is no lower
clamp, null guard or amount validation. Narrowing precedes comparison; do not
replace it with an unlimited-precision sum clamped before storage. This helper's
other callers may pass amounts outside this periodic path's positive-byte range.

## Status24 FP regeneration (`0x8011dd08`–`0x8011de60`)

GetStatus(unit,24,&duration,&strength); require signed strength>0. Call
BtlUnit_RecoverFp(unit,S8(strength)), then presentation and the regeneration
event. No corresponding unit+0x208/+0x20c bookkeeping block occurs here.
No full-FP or attribute0x40000000 gate occurs in this caller.

The [FP ownership contract](BATTLE_DAMAGE_APPLICATION.md) applies: kind IDs
224..230 redirect recovery to Mario's FP storage; other kinds use their own.
The helper stores16 then compares signed16 to the resolved maximum and has no
lower clamp. Eligibility, status strength and effect position still belong to
the unit being scanned, even when the resource owner differs. Multiple eligible
units can therefore request additions to the same FP pool in slot order;
there is no pool-deduplication in this routine.

## Status regeneration presentation and event (B)

Resolve the unit's body part, obtain that part's world position(x,y,z), and
integer unit height h. Compute `y=fma(unitFloat(+0x114), float(h+5), y)` using
the original binary32 conversion/fmadds behavior. x/z are unchanged.
Call effRecoveryEntry(type,S8(strength),x,y,z), with type0 for HP,1 for FP.
The displayed amount is the original request, not the actual capped gain.

Call evtEntry(0x80416bc0,10,0), symbol subsetevt_regeneration. If nonnull,
copy event+0x15c into unit+0x2a8 and unit+0 into event+0x160. There is no local
check/cancellation of an older stored event before replacement. A null allocation
does not undo resource recovery or the effect, and still counts as changed.

The16-byte US event is a WAIT_FRAME with literal60, followed by END_EVENT and
the END_SCRIPT terminator. The [event wait semantics](../ORIGINAL_BEHAVIOR.md)
apply, including timescale; do not convert this directly to60 render frames.
There is no sound or additional resource-change command in this event body.
Effect internals may have their own behavior and are not recovered here.

## Happy Heart trials and HP update (`0x8011de64`–`0x8011e018`)

After the B+0 gate and common unit gates, load unsigned count=unit+0x2ee.
Set successes=0. For each copy, in countdown order:

1. Load signed currentHP(+0x10c) and signed maxHP(+0x108).
2. Compute signed `q=trunc(currentHP/maxHP)`; threshold=`33−3*q` with32-bit
   arithmetic. Division occurs **before** multiplication.
3. Draw irand(100) and increment successes if draw<threshold (signed compare).

No current/max changes are made between trials by this local loop. Each copy
consumes a draw, including at full HP. Count0 consumes none. The original has
no explicit zero-divisor guard or threshold clamp; a reimplementation needs the
original data invariants before assigning behavior to invalid maxima.
For ordinary positive maxima and0<HP<maxHP, q=0 so threshold33; at HP=maxHP,
q=1 so threshold30. These are comparison thresholds with the recovered modulo
RNG, not measured exact statistical probabilities or an interpolated rate.

If successes=0, continue to the next slot. Otherwise add successes directly to
unit's signed currentHP, store16, reload signed16 and cap above maxHP. This
inlined update does not call RecoverHp and does not perform the status23
bookkeeping writes. Narrowing/clamping order is the same as that helper.

Resolve body-part position and height; here use
`y=fma(unitFloat(+0x114),float(h),y)` with **no +5 height adjustment**.
Call effRecoveryEntry(0,successes,x,y,z), even if the capped actual gain is0.
Register evtEntry(0x80351a3c,10,0), symbol subsetevt_happy_heart, and assign
unit/event identity fields as above on success. Mark changed regardless of
allocation success. This event's body remains unrecovered.

## Happy Flower trials and FP update (`0x8011e01c`–`0x8011e1c4`)

Use count=U8(unit+0x2ef) and the same per-copy loop, but compute q from signed
**local** halfwords unit+0x112 and unit+0x10e. The probability inputs are raw
unit fields; this caller does not use BtlUnit_GetFp/GetMaxFp or resolve Mario
before the division. Preserve this distinction from the recovery destination.

If successes>0, call BtlUnit_RecoverFp(unit,successes). The helper may redirect
the addition to Mario's pool. Then compute body-part position with the same
height expression as Happy Heart and call effRecoveryEntry(1,successes,x,y,z).
Register evtEntry(0x80351b10,10,0), symbol subsetevt_happy_flower, bind identity
fields if allocation succeeded, and mark changed. Its event body is open.

All trials for one unit precede its recovery. Subsequent units run afterward;
if an earlier addition changed a later unit's raw FP field, later trial inputs
can consequently differ. Do not precompute every unit's thresholds at phase
entry or replace these per-copy draws with one aggregate random decision.

## Wait transition differences (B)

When no unit marks a phase changed, all four phases increment channel3 twice
and recurse immediately, skipping their wait. For changed phases:

- Status23 and24: increment once and recurse into the common wait immediately.
- Happy Heart: increment once and exit at0x8011e018, leaving the wait for the
  next outer invocation.
- Happy Flower: increment once and recurse into the wait immediately.

The common wait is the [turn recovery wait contract](BATTLE_TURN_RECOVERY.md):
wait for active events, check conclusion, otherwise advance one state and exit.
Allocation failure still takes the changed transition; actual resource gain is
not the deciding condition. Duration recovery runs later in the sequence.

## Verification obligations and open dependencies (P)

Compare full/near-full regeneration with its displayed request and event wait;
attribute0x40000000 toggling only HP display bookkeeping; multiple units sharing
FP; local Flower probability fields differing from owner fields; count0/1/many;
partial versus full-resource thresholds; each copy consuming a draw; cap after
narrowing; Happy Heart's deferred wait versus Flower's immediate recursion;
allocation failure after resource/effect changes. These are proposed comparison
cases, not original execution observations.

Remaining: Happy event bodies and sounds, display-field semantics, full effect
rendering, unit initialization of FP probability fields/maxima, outer turn
counter/cadence, unit-event wait/conclusion internals and content bindings.
Local recovery requests are specified; complete turn/game equivalence is not.

The recovery and Happy event bodies are now documented in
[recovery events](BATTLE_RECOVERY_EVENTS.md), with their remaining motion, pose
and data-binding dependencies kept explicit.
