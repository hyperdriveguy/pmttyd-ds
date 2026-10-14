# Audience nonzero target adjustment: chunks and rounding

2026-10-13 US G8ME01 disc0 revision0, read-only ISO/DOL. Source-used closure for
the actual initialized baseline8 audience owner and ordinary trio victory
requests; it replaces a native zero-baseline/zero-amount admission adapter with
the original nonzero helper. Existing
[ending target ABI](UNDERGROUND_TRIO_VICTORY_CONTROLLER_AND_FIELD_RETURN.md)
already identifies these operations; this delivery supplies implementation
pseudocode, current ignored listings, and verified numerical owners.

## ABI, owners, and outer operation order

BattleAudienceAddTargetNum801a1a74/180 takes ordinary C float f1=delta,
f2=bias increment. It returns no event readiness/output. It captures both
arguments, calls BaseGetPtr801a86a0, and retains A=B+2858. The fields are:
FLOAT A13778 baseline, FLOAT A1377c desired target, FLOAT A13780 bias,
signed WORD A13790 capacity. Nothing updates the current population here.

In this pseudocode `f(x)` means rounding exactly once to IEEE binary32 at each
original single-precision arithmetic operation. Packed stores and reloads occur
at the indicated field assignments. Ordinary finite representable source
inputs are assumed; exceptional processor conversions are discussed below.

```
d = delta
A.bias = f(A.bias + biasIncrement)
if A.bias > 0 and d < 0:
    A.bias = f(A.bias + d)
    if A.bias < 0:
        d = A.bias
        A.bias = +0
    else:
        d = +0
else if A.bias < 0 and d > 0:
    A.bias = f(A.bias + d)
    if A.bias > 0:
        d = A.bias
        A.bias = +0
    else:
        d = +0

AddTargetNumSub(d)  // always call, even d==0
if A.target < 0: A.target = +0
capFloat = f(signed32(A.capacity))
if A.target > capFloat: A.target = capFloat
if A.bias < -1000: A.bias = -1000
if A.bias > +1000: A.bias = +1000
```

Outer clamp order is lower target, upper target, lower bias, upper bias.
Malformed negative capacity can therefore leave a negative final target.
No fused multiply-add, RNG, NumToTarget, actor allocation, sound, or queue
service occurs. Even delta0 still writes incremented bias and applies clamps.

## Complete nonzero helper

BattleAudienceAddTargetNumSub801a18cc/1a8 takes f1=remaining delta and queries
BaseGetPtr once. At801a18f4 it compares remaining with0; equal exits before
any ratio calculation, so both signed zeros are no-division paths. Otherwise:

```
remaining = delta
A = BaseGetPtr()
while remaining != 0:
    if remaining > 0:
        chunk = min(remaining, +1)
        remaining = f(remaining - chunk)
        if remaining < 0: remaining = +0
    if remaining < 0:
        chunk = max(remaining, -1)
        remaining = f(remaining - chunk)
        if remaining > 0: remaining = +0

    // The source reloads target and baseline for each weighted chunk.
    if chunk > 0:
        ratio = f(A.target / A.baseline)
        q = truncateSigned32(f(f(ratio - 1) / .25))
        if q >= 1:
            denominator = f(wrappingSigned32(q + 1))
            A.target = f(A.target + f(chunk / denominator))
            continue
    if chunk < 0:
        ratio = f(A.target / A.baseline)
        q = truncateSigned32(f(f(ratio - 1) / .25))
        if q <= -1:
            denominator = f(wrappingSigned32(q - 1))
            A.target = f(A.target - f(chunk / denominator))
            continue
    A.target = f(A.target + chunk)
```

Source executes two consecutive remaining-sign branches rather than choosing a
sign again from the consumed chunk. For ordinary finite values the second
branch does not consume another chunk after a positive subtraction because
residual is clamped nonnegative. The last consumed chunk is applied even when
remaining became0; only the back-edge comparison ends the function.

Positive weighted update is801a1964..19c8; negative is801a19d8..1a3c;
unweighted addition is801a1a40..50. The negative weighted path uses **subtract**
f(chunk/negative denominator), not addition. This makes a negative adjustment
reduce a below-baseline target more slowly. q is recalculated after each target
store; replacing the loop with one coefficient computed at entry changes behavior.

The signed q+1 or q-1 conversion uses the xoris/43300000 double construction,
subtracts double constant8030a328=4330000080000000, and then rounds with fsubs
to single. This is a signed32 conversion, not unsigned q or a double-only path.
The fctiwz operation truncates the already rounded single quotient toward zero.

Examples of helper-only finite arithmetic (before outer cap clamp): baseline8,
target10, delta1 produces q1 and target10.5; baseline8,target4,delta-1 produces
q-2 and target f(4-f(1/3)). Target8, delta1 uses q0 and direct target9.
Actual initialized source target may be positive despite capacity0. The helper
must execute its real nonzero update **before** the outer capacity0 clamp.

## Constant words, verifier, and remaining numerical domains

Original r2=80427440. Read these original single words:

| Address | Raw word | Value | Source role |
| --- | --- | --- | --- |
| 80424988 | 00000000 | +0 | comparisons/residual clamps |
| 80424990 | 3f800000 | +1 | positive chunk and ratio subtraction |
| 804249ac | 3e800000 | .25 | ratio bucket divisor |
| 804249d4 | bf800000 | -1 | negative chunk |
| 804249d8 | c47a0000 | -1000 | lower bias clamp |
| 804249dc | 447a0000 | +1000 | upper bias clamp |

There is no source baseline0 guard for nonzero delta. Actual initialized
prologue baseline8 closes that fresh scope. Nonfinite inputs, out-of-range
fctiwz conversions, and enormous deltas whose binary32 subtraction of1 cannot
make progress are not ordinary terminating numerical domains. Do not silently
substitute success for them or add an unobserved source iteration cap. General
exceptional numerical-provider behavior remains explicit; it does not block
the source ordinary victory amounts with initialized baseline8.

`python3 tools/verify_audience_target_accumulator.py` passed against the
read-only original:5 bodies,205 instruction anchors,7 constant/data spans.
Body hashes:

- AddTargetNum801a1a74/180:
  530c171eec20b20be0ff921df69dbf1cf0ac0712ceb11b7046995fcf52f7130a.
- AddTargetNumSub801a18cc/1a8:
  4488e434e11973e184116de6977c4cea5a671c22e99d1ec20e2838db2230a422.
- BaseGetPtr801a86a0/0c:
  17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79.

New ignored listings are audience-target-sub.elf and audience-target-outer.elf
under artifacts/recovered-temp-evidence/. Complete originals remain on ISO.
Documentation-only; scoped diff check, no native edits/build claim or commit.
