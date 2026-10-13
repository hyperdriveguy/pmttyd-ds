# Mario landing query 2 (five-probe search)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes `checkLand2` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Fingerprint: entry 8008cda8, `.elf` 1976 bytes, 452-line `.disasm`, 13 `bl`
to 6 absolute targets (no same-TU calls). No JSON row. Identity: the [movement
doc](OVERWORLD_MARIO_MOVEMENT.md) pins `marioChkLandon2` code at
0x8008ce7c–cea4, inside this body — adopted as the name with that citation.

Callees: `marioGetPtr` 0x8005e0c0, `sin` 0x80272410 / `cos` 0x80271ea8 (MSL
double, `frsp`-rounded), `hitVec` 0x800158a4 (hitCheckVecFilter; orientation/normalization paths in
[support/slopes](OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md)), `system_angleABf` SDA constants read from the ISO:
−27540 = 0.0, −27476 = 0.1, −27508 = 0.5, −27500 = 180.0, −27480 = π,
−27492 = 0.25, −27504 = −0.5, −27544 = 1000.0, −27456 = 100.0.

## Prologue and early-outs

Incoming r3 retained (r24, output-word owner), f1/f2 retained. Absolute
table 0x802C3B78 (+120..+140 words) held in r27. `marioGetPtr` → Mario.
[Mario+536/540/544/548] initialized to 0.0. Then two flag early-outs on
[Mario+0]: bit LSB9 (rlwinm 22,22) set, or bit LSB8 (23,23) set → store
float [Mario+144] to [arg+0], return 0. Both flag identities are open.

## Heading selection and angle setup

`f3 = |f1|`; f29 = 0.1 + f3; f28 = 0.5 · [Mario+444]; `fcmpu([Mario+384],
f5)` — f5 is the incoming float register, never written locally: equal
selects [Mario+428], else [Mario+420]. The movement doc's travel/facing
account (+0x1a4 vs +0x1ac on nonzero/zero base+0x180) matches this shape
exactly if f5 arrives 0.0; that precondition is not proven here. Then a
self-TU call (0x800963fc), sin/cos of the quotient lane, and the negated
pair (f30, f31) plus summed lanes (f28, f29) staged for the loop.

## Five-probe loop (r25 = 0..4)

Each iteration selects a direction pair (f22, f21) by counter value —
(0.0,0.0), (f27,f26), (f31,f26), (f26,f30), (f30,f31) — builds the `hitVec`
query struct on the stack (base f25 = [Mario+140], f24 = [Mario+148], lane
floats, int words), and calls `hitVec` with one of three .text tables
(0x80091434/0x80091504/0x800915E4) chosen by [Mario+46] == 28 versus the
[Mario+0] bit-7 routes (same selector shape as `marioChkLandon`'s
ground/roll split). Null `hitVec` result → next iteration; five nulls →
return 0.

On a hit, three fixed-point quantization idioms run (`fctiwz` → int→float
via the bias pair → `fdivs`, each followed by the NaN-safe `cror` ≤ test
selecting −0.5/0.5-class SDA lanes), then two `system_angleABf` calls
publish [Mario+540/544] and [Mario+548] (second call gated on a further
`fcmpo`/`cror` lane test; a `frsqrte` + triple Newton-Raphson refinement
precedes it). [Mario+536] was published mid-loop from the first angle call.
Loop counter increments; r25 == 5 exits with r3 = 0.

Success return: r3 = the `hitVec` result with [arg+0] = the first lane
float. One gate (`bt` on CR0 last set by `addic.`, whose sum r1+24 cannot be
zero in-domain) is effectively an unconditional copy — reported as observed.

## Not established

Flag/offset meanings, table contents at 0x802C3B78, `hitVec` variant-table
roles, f5's value, lane semantics, cadence, and any landing effect. The
[airborne doc](OVERWORLD_AIRBORNE_AND_LANDING.md) names this query's family;
the [support doc](OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md) owns the
five-foot contract this loop resembles but does not cite this address.
