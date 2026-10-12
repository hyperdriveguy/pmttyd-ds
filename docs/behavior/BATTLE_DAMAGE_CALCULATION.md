# Ordered HP and FP damage calculation

Baseline: US G8ME01 revision 0; see [original identity/evidence](../ORIGINAL_BEHAVIOR.md).
Recovered 2026-10-12 by inspecting `BattleCalculateDamage` at `0x800fd790`
(length `0x69c`) and `BattleCalculateFpDamage` at `0x800fd680` (length `0x110`).
All numeric behavior below is B (US binary evidence). Friendly badge/status
names are R annotations from the pinned public headers. P cases at the end
are verification obligations, not observations. No Dolphin measurement or
Switch-version behavior is claimed.

The pinned [public damage source](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/battle/battle_damage.c)
does not provide these complete calculations. Read-only disassembly artifacts
remain under ignored `artifacts/reference/behavior/`; reproducible function
fingerprints are produced by `tools/behavior_evidence.py`.

## Interface, ownership and input data

Both functions receive `(A, D, P, W, outCode, F)`: attacker, defender, target
part, weapon, writable result-code pointer, and flags passed by value. They
return signed damage. HP calculation initializes `*outCode`; FP calculation
reads and may modify it. Neither calculator itself subtracts defender HP/FP.
Callbacks can have effects of their own and must be recovered separately.

The global battle pointer is loaded from r13+`0x1c70`. Offsets below are relative
to the stated object; all table indices and masks come from the instructions.
Byte badge counts are unsigned. Status strengths and defense-table entries
are signed bytes, current HP and D+0x260 signed 16-bit, other arithmetic is
32-bit. Integer division truncates toward zero. A replacement must preserve
word arithmetic without relying on host-language signed-overflow behavior.

| Field | Meaning used in these routines |
| --- | --- |
| W+`0x1c`, W+`0x40` | HP / FP callback pointers; invocation `(A,W,D,P)` |
| W+`0x6c`, W+`0x74` | Element byte / special-flags word S |
| P+`0x1ac` | Part attributes PA |
| P+`0x1b4`, P+`0x1b8` | Pointers to signed-byte defense / defense-attribute tables indexed by element |
| unit+`0x10` | Kind-data pointer; bytes +0x0c/+0x0d are danger/peril thresholds |
| unit+`0x10c` | Signed current HP |
| battle+`0x1cb8` | Action-command state; mask 2 is tested here |
| battle+`0x18ff9` | Mode byte; values 1 and 2 enable specific +3 adjustments |

Badge names used below map to these unit byte offsets (R labels, B accesses):
Power Plus `2e4`, P-Up D-Down `2e5`, All or Nothing `2e6`, Mega Rush `2e7`,
Power Rush `2e8`, P-Down D-Up `2e9`, Double Pain `2ea`, Last Stand `2eb`,
Defend Plus `2ec`, Damage Dodge `2ed`, HP Drain `2f9`, FP Drain `2fa`,
Ice Power `301`. Offsets are hexadecimal. Counts are not booleans unless
explicitly described as a gate.

## HP entry gates and callback (`0x800fd7b8`–`0x800fd88c`)

1. If `D+0x27c & 2`, add `0x1000` to local F by bitwise OR. This does not
   write back the caller's flags variable.
2. Read element from W+0x6c. If A is nonnull, S has `0x400`, and attacker
   status 8 is active, replace element with 1.
3. Save `defaultCode = F | 30` and write it to `*outCode`.
4. Return zero if HP callback is null, or if PA has `0x20000000`.
5. Return zero if A is nonnull, S has 1, attacker All or Nothing count is
   nonzero, battle+0x1cb8 lacks 2, and F lacks `0x20000`.
6. Call the HP callback `(A,W,D,P)` and use its return as power.

Although W+0x20 is loaded immediately before the indirect call, it is not
a fallback for a null callback: that case has already returned. Later paths
assume A exists when their flags need attacker data; the entry's selective
null checks are not general null-safety guarantees.

## Attacker modifiers, in execution order (`0x800fd890`–`0x800fdb18`)

1. If battle mode byte is 1 and S has 2, add 3.
2. If S has 1: add Power Plus and P-Up D-Down counts. If All or Nothing is
   nonzero, add its count when battle+0x1cb8 has 2 or F has `0x20000`;
   otherwise set power to zero and jump to defense-table processing. The
   repeated test remains in the code; callbacks may have changed state.
3. Still under S bit 1: if attacker HP ≤ peril threshold, add `5*MegaRush`;
   independently, if HP ≤ danger threshold, add `2*PowerRush`. If PA has
   `0x100`, add attacker Ice Power count.
4. If S has 4 and attacker status 16 exists, add its signed strength and
   clamp power to at least zero. Whenever S has 4, write `A+0x27c |= 8`,
   including when status 16 was absent.
5. If S has 2, process status 10 then status 12. For each active status,
   add signed strength and immediately clamp power to at least zero.
6. If S has 1, subtract P-Down D-Up, HP Drain and FP Drain counts, in that order.
7. If S has 2, process status 11 then status 13; add each active signed
   strength and clamp after each addition.
8. Clamp power to at least zero.

R labels: statuses 8 Burn, 10 Huge, 11 Tiny, 12 Attack Up, 13 Attack Down,
16 Charge. Negative strengths are added as signed values, not subtracted
again. These conditional intermediate clamps make reordering invalid.

## Defense and result-code selection (`0x800fdb1c`–`0x800fdca4`)

Read `defense = signed_table(P+0x1b4)[element]` and
`attribute = signed_table(P+0x1b8)[element]`. Set `*outCode` to F OR the
following code. This is bitwise OR, not replacement of F's low byte.

| Element | Code ORed with F |
| --- | --- |
| 0 or other | 23 |
| 1 | 24 |
| 2 | 26 |
| 3 | 25 |
| 4 | 27 |

For attribute 3, skip all the defense modifiers below. Otherwise:

1. Add defender Defend Plus count.
2. If F has `0x40000`, add defender Damage Dodge count.
3. If `BtlUnit_CheckStatusFlag(D,0x01000000)` succeeds, add 1.
4. If defender status 14 exists, add its signed strength without a clamp here.
5. If defender status 15 exists, add its signed strength and clamp defense ≥0.
6. If battle mode byte is 2 and defender kind ID at D+8 is 222, add 3.
7. If PA has `0x80`, add the signed word D+0x21c and clamp defense ≥0.
8. If S has `0x40` and defense is positive, set defense to zero. Negative
   defense is not removed by this test.

R labels: statuses 14/15 Defense Up/Down. The meaning and producers of the
mode byte, extra defense word and status flag need separate lifecycle tracing.
Do not replace the conditional clamps with an unconditional initial clamp.

## Damage modifiers and finalization (`0x800fdca8`–`0x800fde18`)

1. Start `damage = power-defense`, except attribute 3 starts with `power`.
2. Add defender P-Up D-Down count.
3. Only if damage is now positive: if S has `0x10`, subtract signed D+0x260
   and floor the result at 1; then, if S has `0x20`, subtract A+0x280 and
   floor the result at 1. These counters' reset lifetimes remain unresolved.
4. If F has `0x40000`, subtract 1 (guard flag).
5. If element is 1, subtract defender Ice Power count.
6. Subtract defender P-Down D-Up count.
7. If defender Double Pain count n is nonzero, multiply by `n+1`.
8. If defender Last Stand count n is nonzero and HP ≤ danger threshold,
   replace damage with signed `trunc((damage+n)/(n+1))`. For nonnegative
   damage this is ceiling division, but damage has not yet been clamped.
9. Apply the defense attribute: 1 adds 1; 2 sets zero; 3 leaves it unchanged;
   4 sets zero unless F has `0x80000000`; other values leave it unchanged.
10. Clamp damage ≥0.
11. Attribute 3: negate damage, OR `0x2000` into `*outCode`, and return.
    This branch also runs for zero magnitude; it keeps the element code and
    absorption flag instead of replacing them with defaultCode.
12. Otherwise, if damage is zero, write defaultCode and return zero.
13. For positive damage, if F has `0x100` and either S has `0x8000` or
    D+0x104 lacks `0x100`, OR `0x10000` into `*outCode`. Return damage.

There is no general minimum-one rule or 99 cap in this calculator. A later
consumer may impose other limits. Attribute 3 produces a negative result
with an absorption flag; actual healing/clamping belongs to damage application.
Double Pain stacking here is linear in count+1, not repeated doubling.

## FP calculation and its differences

`BattleCalculateFpDamage` consumes the HP result code produced earlier:

1. Return zero if `*outCode & 0x2000` (HP absorption), FP callback is null,
   or PA has `0x20000000` (`0x800fd6a8`–`0x800fd6e4`).
2. If A exists, S has 1, attacker All or Nothing count is nonzero and
   battle+0x1cb8 lacks 2, return zero. **F bit 0x20000 does not bypass
   this gate**, unlike the HP calculator (`0x800fd6e8`–`0x800fd718`).
3. Call FP callback `(A,W,D,P)`; subtract 1 if F has `0x40000`; clamp ≥0
   (`0x800fd71c`–`0x800fd750`).
4. If FP damage is positive and the low byte of `*outCode` is 30, replace
   that byte with 23 while preserving its high 24 bits. Return FP damage
   (`0x800fd754`–`0x800fd778`).

The HP power/defense/badge/element pipeline is not repeated for FP. A callback
can nevertheless implement attack-specific arithmetic, so this does not mean
badges can never influence an attack's FP outcome.

## Verification obligations and remaining dependencies (P)

Derived cases for a future implementation: stack Double Pain twice and expect
a factor of 3; use Last Stand count 2 with incoming damage 7 and expect 3;
exercise both danger/peril equality thresholds; preserve negative intermediate
values until each specified clamp. Check attribute-1 adding damage after prior
reductions, attribute-3 zero absorption suppressing FP, null callbacks, negative
base defense, and the HP/FP difference for F bit 0x20000. Check every modifier
gate and callback side effect rather than only typical attack examples.

These contracts cover calculators, not a complete battle. Still required:
weapon callback catalog and all original data bindings, status helper rules,
`BattleCheckDamage` orchestration, counters, direct HP/FP application and death/
revival, hit-counter lifetimes, action-result producers, audience/events, and
original execution traces. Connect [precheck](BATTLE_HIT_PRECHECK.md) and
[defense input](BATTLE_INPUT_AND_DEFENCE.md) through recovered callers before
claiming end-to-end equivalence. Switch behavior needs separately versioned
evidence and must not silently override this GameCube baseline.
