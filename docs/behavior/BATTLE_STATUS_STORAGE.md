# Status storage, setters, presence and recovery steps

US G8ME01 revision0, recovered 2026-10-12. B evidence is original code/data;
R labels are from the pinned [public status enum](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/include/battle/battle_status_effect.h).
See [baseline identity](../ORIGINAL_BEHAVIOR.md). No original gameplay run or
Switch comparison is claimed. This establishes local helper semantics, not
status infliction probability, immunity or complete turn lifecycle.

| Routine | US RAM / size |
| --- | --- |
| BtlUnit_CheckStatus | 0x80127814 /0x7c |
| BtlUnit_CheckRecoveryStatus | 0x80127890 /0x2c8 |
| BtlUnit_SetStatus | 0x80127bac /0x388 |
| BtlUnit_GetStatus | 0x80127f34 /0x1c4 |
| _CheckMoveCount | 0x801280f8 /0x4c |

Original disassembly stays under ignored `artifacts/reference/behavior/`.
The recovered jump tables at `0x80364204` (25 recovery entries),
`0x80364268` (28 setter entries), and `0x803642d8` (28 getter entries)
establish the case-to-ID mappings below; instruction adjacency alone does not.

## Getter layout and presence (B)

`GetStatus(unit,id,durationOut,strengthOut)` initializes both output values to
zero. ID is narrowed/interpreted as signed8; values outside0–27 use the zero
default. Each nonnull output pointer receives one byte. There is no valid-case
null-unit guard. Outputs are bytes; signed interpretation belongs to callers.

Offsets below are hexadecimal unit offsets. `same` means both outputs receive
the same stored byte. Shared signed pairs only expose the requested side when
the **strength** has the indicated sign; otherwise both outputs remain zero.

| ID / R name | Duration offset | Strength offset / condition |
| --- | --- | --- |
| 0 Allergic | 12a | same |
| 1 Sleep | 118 | same; byte119 is not returned |
| 2 Stop | 11a | same |
| 3 Dizzy | 11b | same |
| 4 Poison | 11c | 11d |
| 5 Confuse | 11e | same |
| 6 Electric | 11f | same |
| 7 Dodgy | 120 | same |
| 8 Burn | 121 | same |
| 9 Freeze | 122 | same |
| 10 Huge | 123 | 124, only if signed strength>0 |
| 11 Tiny | 123 | 124, only if signed strength<0 |
| 12 Attack Up | 125 | 126, only if signed strength>0 |
| 13 Attack Down | 125 | 126, only if signed strength<0 |
| 14 Defense Up | 127 | 128, only if signed strength>0 |
| 15 Defense Down | 127 | 128, only if signed strength<0 |
| 16 Charge | 129 | same |
| 17 Flipped | 12b | same |
| 18 Invisible | 12c | same |
| 19 Fast | 12f | same |
| 20 Slow | 130 | same |
| 21 Payback | 12d | same |
| 22 Hold Fast | 12e | same |
| 23 HP Regen | 131 | 132 |
| 24 FP Regen | 133 | 134 |
| 25 Fright | none | both zero |
| 26 Gale Force | none | both zero |
| 27 Instant Kill | 135 | same |

`CheckStatus(unit,id)` returns0 for null unit. Otherwise call GetStatus; if its
duration byte is nonzero, return1, including negative signed durations. If
duration is zero, return1 only when signed8 id is6 and unsigned unit+0x2f7
(R: Zap Tap count) is nonzero; otherwise0. Thus badge-provided Electric is
present to CheckStatus even when GetStatus returns zero duration/strength.
Presence does not test HP or general immunity here.

## Setter defaults and simple cases (B)

`SetStatus(unit,id,T,S)` narrows ID to signed8 and writes byte-sized duration T
and/or strength S. Return1 is the common path, including unrecognized IDs and
IDs25/26, which perform no writes. Return0 occurs in specific branches below;
it can accompany a state mutation. This is not a uniform "no change" result.
There is no general HP, null-unit, immunity, duration cap or RNG test.

- IDs2,3,5,6,7,17,18,22,27 store T to the table's duration byte, return1.
- ID1 stores T at118 and clears byte119, return1.
- IDs4,23,24 store T and S to their two separate bytes, return1.
- IDs0 and21 store T and return1 only when signed8(T)>0. Otherwise clear
  their duration byte and return0.

## Burn/Freeze cancellation (`0x80127c24`–`0x80127c68`)

Setting8: if Freeze byte122 is nonzero, clear122 and return0, without writing
Burn121. Otherwise store T at121 and return1. Setting9 mirrors this: if
Burn121 is nonzero, clear121 and return0 without writing Freeze122; otherwise
store T at122 and return1. The opposite-status test is nonzero, not positive.
It runs even when the new requested T is zero; neither branch clears both
bytes unconditionally or installs the new status after cancelling the old one.

## Shared signed pairs (B)

For IDs10/11, nonzero signed8(S) replaces both123/124 with T/S and returns1.
If S is zero: ID10 clears both bytes only if old signed124≥0; ID11 clears
both only if old signed124≤0. Return0 in either case, even if the opposite
side was left intact. Nonzero S is not forced to match the requested ID's sign.

IDs12/13 behave the same way using125/126: nonzero S replaces both and returns1;
zero S clears only the requested sign side (12: old≥0, 13: old≤0), returns0.

IDs14/15 share127/128 but have **different replacement semantics**:

1. For positive signed S, if old signed128<0, clear both bytes and return0;
   otherwise store T/S and return1.
2. For negative S, if old signed128>0, clear both and return0; otherwise
   store T/S and return1.
3. For zero S, clear both only for matching sign side (ID15: old≤0;
   ID14: old≥0), then return0. Leave the opposite side unchanged.

Thus opposite Defense Up/Down cancel, whereas a nonzero opposite size/attack
strength overwrites. Do not apply a single generic stacking rule to all pairs.

## Charge and Fast/Slow (B)

ID16 at `0x80127dec`: add S to the old **unsigned byte**129, then store only
the result's low byte. Interpret that stored byte as signed. If negative,
clear129 and return0. Otherwise if it exceeds99, store99; return1. T is unused.
Narrowing occurs before saturation: old99 plus29 produces byte128, which is
negative and clears Charge rather than clamping to99. This is an edge-input
consequence, not proof that normal attacks supply such a value.

ID19 Fast at `0x80127e84`: if old signed Slow130>0 and signed T>0, clear
Slow130, invoke _CheckMoveCount and return0; do not store new Fast duration.
Otherwise store T at Fast12f, invoke _CheckMoveCount and return1. ID20 Slow
mirrors this using opposite Fast12f and destination Slow130. Negative opposite
durations do not trigger cancellation. Unlike Burn/Freeze, incoming T must also
be positive to select this cancellation branch.

`_CheckMoveCount`: if unit word+0x27c has0x1000, do nothing. Otherwise, if
signed byte+0x23 is positive and equals signed byte+0x22, call the helper at
`BtlUnit_ResetMoveStatus` at `0x801266d4` with the unit. The downstream move-count recalculation remains a
dependency; this document records its exact invocation gate rather than
assuming that Fast always grants an immediate action.

## Recovery step eligibility (B)

`CheckRecoveryStatus(unit,id)` returns0 without mutation for null unit,
CheckStatus(unit,27) true, signed HP at10c≤0, or GetStatus returning zero
duration. It then dispatches by signed8 id within0–24. IDs16/17 are explicit
no-op cases; IDs25–27 and out-of-range values return0. Badge-only Electric
has zero getter duration and therefore does not expire through this path.

The function is a **single explicit step**, not inherently once per turn.
Caller timing, repeated calls and turn-boundary ownership must be recovered
separately. No RNG call occurs in this helper.

## Recovery transitions (B)

Read getter outputs as signed-byte T/S for comparisons. Every adjusted value
is stored as a byte before later tests. Return1 when the locally updated T
becomes zero, otherwise0, for each mutating group below. Setter return values
are ignored where SetStatus is called.

| IDs | Case entry | Step |
| --- | --- | --- |
| 1 | 0x8012792c | If T>1, decrement; otherwise set T=0. Write unit118 directly; preserve byte119. |
| 0,2,3,5,6,7,8,9,18,19,20,21,22 | 0x80127970 | Move T one toward zero. Independently change S by −1 if S≥0, +1 if S<0; if new T=0 force S=0. Call SetStatus(unit,id,T,S). |
| 10,11 | 0x801279f4 | Move T one toward zero; retain S until T becomes0, then set S=0. Directly write123/124. |
| 12,13 | 0x80127a68 | Same duration rule, direct writes125/126. |
| 4,14,15,23,24 | 0x80127adc | Move T one toward zero; retain S until T becomes0, then set S=0. Call SetStatus(unit,id,T,S). |

For the large second group, getters normally return S=T, so both change in
parallel; preserve the actual call sequence rather than inferring a separate
strength decrement for every status. Sleep is exceptional: any negative T
clears immediately, not one unit toward zero. Poison and regeneration retain
strength while their duration approaches zero. Size/attack recovery writes
directly; defense recovery routes through its setter.

The helper can return0 after mutating a still-active duration. It can also
invoke setter side effects, including Fast/Slow move-count checks. Do not use
the boolean return as a generic changed/not-changed indicator.

## Consequence for damage-triggered wakeup (B)

[Direct damage](BATTLE_DAMAGE_APPLICATION.md) on a successful Sleep-recovery
roll first writes118=1 and119=100, then calls CheckRecoveryStatus(unit,1).
For a living unit without Instant Kill status, this helper clears118 and
returns1 while preserving119=100. For HP≤0 or Instant Kill status it returns0
before that clear, leaving those caller-written bytes in place. The caller
does not branch on that result before showing its recovery effect. This
establishes the local outcome; later turn/death cleanup remains separate.

## Verification obligations and remaining dependencies (P)

Derived cases: shared-pair sign filtering with zero duration; badge-only
Electric; negative durations; opposite Burn/Freeze with incoming zero; size/
attack overwrite vs defense cancellation; Charge wrap-before-clamp; Fast/Slow
positive-only cancellation; setter no-write success; recovery no-op returns vs
mutation; Sleep recovery on lethal damage. Verify all three jump tables, since
misassigning one case can produce plausible but wrong gameplay rules.

Still required: status infliction/resistance/probability, strength/duration
construction from weapons, complete recovery caller schedule, periodic HP/FP
effects, movement/action restrictions, animation/UI and end-of-battle cleanup.
These local contracts and selected static anchors are not original execution
tests or proof of a complete status system.
