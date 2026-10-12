# Move-count reset and status action eligibility

US G8ME01 revision0, recovered 2026-10-12. B evidence is original US instructions;
friendly status names are annotations. See [baseline](../ORIGINAL_BEHAVIOR.md),
[status storage](BATTLE_STATUS_STORAGE.md), [recovery](BATTLE_TURN_RECOVERY.md)
and [damage guard gates](BATTLE_DAMAGE_ORCHESTRATION.md).
No original execution or Switch equivalence was measured. Original disassembly
remains ignored. This covers local helpers and selected callers; the complete
action/turn lifecycle still needs recovery.

## Reset storage and order (B)

BtlUnit_ResetMoveStatus at0x801266d4,size0xf8 assumes valid unit U.
Relevant fields are bytes U+0x20..0x24. All stores narrow to8 bits. Except for
the initial base copy and Fast shift, comparisons interpret the bytes as signed.

1. Write0 to+0x20. Copy unsigned byte+0x21 into+0x22.
2. If CheckStatus(U,19)≠0 (Fast), shift the unsigned current+0x22 left1,
   store its low byte back. No cap to127 or255.
3. If CheckStatus(U,20)≠0 (Slow), apply the branch below.
4. Copy+0x22 into+0x23.
5. If signed+0x23 is exactly0, write+0x22=0 and+0x20=10.

The final test is equality0, not≤0. Negative narrowed counts remain nonzero
and do not receive that fallback. The helper does not write+0x21 or+0x24,
consume moves, test HP/status27, or return a separately computed count.
Both Fast and Slow checks execute, in that order. Their ordinary setters use
opposing-status cancellation, but this helper still specifies behavior when
both presence checks are nonzero through another state writer.

## Slow branch (B)

Let c=S8(U+0x22) after Fast handling.

- If c≥2, set+0x22 to signed trunc(c/2), stored as a byte. The original uses
  arithmetic shift plus add-with-carry; no read of+0x24 in this branch.
- Otherwise set threshold=2; if S8(U+0x21)<1, double threshold to4.
  If S8(U+0x24)≥threshold, write+0x22=1 and+0x20=0.
  Otherwise write+0x22=0 and+0x20=10.

Examples derived from explicit inputs: base1/no Fast/Slow active gives0 when
counter byte+24=1 and1 when+24=2. Base0 uses threshold4. Base3 with Slow gives1;
base3 with Fast then Slow gives3. Base64 with Fast gives stored128 (signed−128),
so Slow takes its low-count threshold branch rather than dividing128 by2.
These are instruction-derived cases, not observed gameplay or a claim of an
alternating-turn schedule. Other writers of+24 must be recovered before
assigning that cadence.

## Fast/Slow mutation-triggered reset (B)

_CheckMoveCount at0x801280f8,size0x4c runs after Fast/Slow setter mutations.
If U+0x27c has0x1000, skip. Otherwise require S8(U+0x23)>0 and equality of
signed bytes+0x23 and+0x22, then call ResetMoveStatus(U).
Thus a setter does not invariably reset an already-partially-consumed count.
Both cancellation and successful storage paths can invoke this check; local
status-setter return0 does not establish absence of move-count side effects.

The source recovery phase for status20 Slow explicitly calls ResetMoveStatus
when expiry is reported at0x8011d680. Fast expiry has no matching explicit call
in the turn caller. Setter/helper side effects are separately specified in
status storage; do not add an extra symmetric reset on the basis of names.

## Turn-first reset caller and inventory (B)

A scan of all DOL text sections for direct opcode18 branches to ResetMoveStatus
found three link sites:0x8011b260 in btlseqTurnFirstProcess,0x8011d680 in
btlseqTurn,0x80128130 in _CheckMoveCount. This inventory excludes indirect calls,
REL relocation targets and event-script function pointers.

btlseqTurnFirstProcess at0x8011b1ec,size0xf0 scans64 unit slots. For every
nonnull unit, increment unsigned byte+0x24 and store8, **then** call reset.
There is no HP/allegiance/status27 gate on that reset. A value255 becomes0.
The outer turn first-process invocation schedule and other+24 writers remain
open, so this local increment is not sufficient to infer all Slow cadence.

Following reset, the same caller draws irand with signed kind-data byte+0x89
and writes U+0x28 as signed byte kind+0x88 plus the draw minus
trunc(S8(kind+0x89)/2). A zero bound consumes no generator draw under the
[RNG contract](RANDOMNESS.md). Then it checks status flag0x01000000 and clears
that flag when set. Caller initialization/effect services and downstream
priority consumers remain separate contracts. These operations occur after
move reset; preserve their order and conditional draw count.

## Script max-count setter (B)

btlevtcmd_SetMaxMoveCount at0x8010fce0,size0x98 resolves/translates unit operand.
If translated ID is−1, return2 without resolving the count operand or fetching
unit. Otherwise fetch unit, resolve count through evtGetValue, store low8 bits
to+0x21. If S8(current+22)>S8(newBase+21), copy newBase to+0x22. Return2.

It does not call ResetMoveStatus, write+0x20/+0x23/+0x24, or apply Fast/Slow
multipliers. Raising the base leaves the current count unchanged. Lowering
caps it using signed comparison, after narrowing the new base. No unit-null
fallback exists after a non−1 lookup. Do not implement this callback by calling
the reset helper: that would change partially consumed moves and status timing.

## Action and guard status predicates (B)

BtlUnit_CanActStatus at0x8012724c,size0x90 short-circuits CheckStatus(U,1),2,9,27
in that order. Any nonzero result returns0. Otherwise return1 if status-flags
word U+0x138 lacks1, else0, using CheckStatusFlag and normalization.
The status helper itself tolerates null; CheckStatusFlag returns0 for null,
so the composed predicate returns1 for null. This is the local arithmetic
outcome, not a statement that a missing actor can execute an action in battle.
There is no HP, remaining-count, group-membership or kind test here.

BtlUnit_CanGuardStatus at0x801272dc,size0x20 simply calls CanActStatus and
propagates its result. It adds no guard-specific condition. GetACPossibility,
used by damage orchestration, adds membership/kind gates before similar status
checks; these are distinct predicates and must not be collapsed.

btlevtcmd_CheckActStatus at0x80105cb4,size0x84 resolves/translates/fetches unit,
calls CanActStatus, writes normalized0/1 to the raw second output token through
evtSetValue and returns2. Count or HP checks are not added by this wrapper.

## Differential obligations and open dependencies (P)

Compare no status/Fast/Slow/both, odd counts, narrowed counts≥128, base0/1,
counter below/equal threshold, counter255 wrap, equality0 fallback versus negative
counts; setter reset when untouched versus partially consumed; max-count raise
versus lower without reset; dead units still reset in the first-process caller;
null action predicate and separate AC possibility; zero random priority bound.
These are derived original/native comparison cases, not execution traces.

Remaining: count consumption and+20/+23/+24 producers/consumers, outer turn
cadence, kind priority bindings, all indirect callers, first-process effect
services and actor/menu scheduling. Local move predicates do not establish
complete turn/action equivalence or complete-game requirements.
