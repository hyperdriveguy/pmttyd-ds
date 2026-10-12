# Partner direction-query norm,square-root refinement and speed output

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
numeric region0x800cb0fc..0x800cb1e0 inside partyGetMoveDirSpd.
See [composition](FIELD_PARTNER_DIRECTION_AND_SPEED.md) for X,Z,direction,
factor and prior state writes. This specifies local arithmetic,not original
execution or a verified host-math replacement.

## Squared norm and positive branch

Compute q=F(X,X,R(Z*Z)),where R/F are single-result rounding/fused
multiply-add. If **ordered q>0**,execute frsqrte(q),retaining its hardware
reciprocal-square-root estimate r. Its exact seed and floating-point environment
must be reproduced or independently verified;the body does not call sqrtf.

Using double constants0.5 and3.0 at0x802cc5c0/0x802cc5c8,execute three
refinements. D denotes each double arithmetic result,and N is the fused
negative multiply-subtract result corresponding to3-q*u:

```
repeat three times:
    u = D(r*r)
    v = D(0.5*r)
    w = N(q,u,3.0)
    r = D(v*w)
root = R(D(q*r))
```

Each step is separate fmul/fnmsub/fmul in the original;the third result uses
a different scratch FP register but the same arithmetic dependency. Preserve
the mixed precision and seed. A direct host sqrt result is not proven to match
all rounding,exception,underflow or NaN cases. Positive infinity takes this
estimate/refinement branch;it is not specially returned as infinity here.

## Nonpositive/unordered branch

If ordered q>0 is false,compare q against double0 at0x802cc5d0. An ordered
negative value loads binary32 **0x7fffffff** from0x8041e3c8 as the root.
Otherwise store q temporarily as binary32 and classify its exponent/fraction:

| Classification | Local root policy |
| --- | --- |
| Exponent0xff,fraction nonzero | Replace with0x7fffffff |
| Exponent0xff,fraction zero | Keep original q |
| Exponent0,fraction zero | Keep original q |
| Exponent0,fraction nonzero | Keep original q |
| Other exponent | Keep original q |

The temporary classification mask excludes the sign bit. Negative infinity
was already handled by the preceding ordered-negative branch;positive infinity
normally entered the ordered-positive branch. This table describes the original
branch structure,not a claim that every row is reachable from finite X/Z.
Zeros retain their input sign through this branch before the final multiply.

Finally compute **R(factor*root)** and store through the speed pointer.
Factor is0.75 or1.0. There is no epsilon,explicit finite-value clamp or
exception suppression. Direction was already written before this region;
later numeric failure does not roll back that output or prior state/history
changes.

## Verification and remaining numeric evidence

tools/verify_field_partner_direction.py pins the containing body,frsqrte,
three fnmsub sites,frsp and output store,plus exact single/double constants.
Whole-body hashes and repeatable metadata validate evidence identity;they
do not execute the original floating-point path or prove an emulator's seed.
Original inspection files remain ignored.

Next:exact Gekko frsqrte/FPSCR/denormal behavior,double trig/angle services,
supported numeric domains and original captured trajectories/output comparisons.
These gaps remain explicit requirements for strong behavioral equivalence.
