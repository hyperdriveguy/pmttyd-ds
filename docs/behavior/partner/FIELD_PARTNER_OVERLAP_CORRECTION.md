# Partner/Mario overlap correction and signed counter

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyMarioHosei at0x800cab04,size0x218. This is the called correction service
in [direction/speed composition](FIELD_PARTNER_DIRECTION_AND_SPEED.md),not a
history target-count getter. It writes correction components and returns a
signed halfword counter;no original trajectory or timing trace is claimed.

Let P be the party object,M=P+0x160,R each single-precision arithmetic result.
P+0xa0 and P+0xa8 are cleared to positive zero before any overlap rejection.
P+0xa4 is not cleared or used by this body.

## Axis-aligned overlap admission

Choose coefficients from P+8 mask0x40000;owner M+0 mask0x8000 overrides
the choice to the narrow row:

| Coefficient set | X | Z | Y |
| --- | --- | --- | --- |
| Mask clear,owner override clear |0x3ecccccd (~0.4)|0x3e99999a (~0.3)|Same as X |
| Mask set or owner override set |0x3e4ccccd (~0.2)|0x3e19999a (~0.15)|Same as X |

Compute width=R(P+0xf4 + M+0x1b8),height=R(M+0x1bc + P+0xf0).
Thresholds are R(coefficientX*width),R(coefficientZ*width),
R(coefficientY*height). Compare abs(R(P.x-M.x)),then abs(R(P.z-M.z)),
then abs(R(P.y-M.y)) against these thresholds,in that order. All three must
be ordered<=. Endpoint equality admits;unordered fails. There is no radial
distance replacement or epsilon. Coordinates are P+0x58/+0x5c/+0x60 and
M+0x8c/+0x90/+0x94.

If any axis rejects,read signed P+0x52 and decrement/store only if positive.
Zero and negative counters remain unchanged. Return the resulting signed
halfword;the X/Z correction remains zero on this route.

## Bearing,magnitude and counter policy inside the box

Request angleABf(M.x,M.z,P.x,P.z),retaining bearing a. If float M+0x180
is not equal to zero (including unordered):

1. Compute d=R(a-M+0x1a4),without local angle wrapping.
2. If abs(d) is ordered<=4,add90 to a when d is ordered>=0;otherwise
   subtract90. An unordered d fails the outer<= test and skips adjustment.
3. Read signed halfword P+0x52,add1,store its low16 bits and sign-extend
   those low bits. Clamp to100 only if that signed result>100.
4. Set initial correction magnitude m=1.

Thus32767 increments to signed-32768 and bypasses the>100 clamp. The
counter is not a saturating unsigned timer and does not count successful
history appends. There is no original elapsed-time interpretation here.

If M+0x180 equals zero,request angleABf(M.x,M.z,P.x,P.z) again,set m=4,
and leave the counter unchanged. The duplicate angle call remains in the
original body. Both routes then,if M+0x180 is ordered>1,override
m=R(0x3f4ccccd*M+0x180),where the coefficient is approximately0.8.

## Correction outputs

Compute radians=R(R(0x40490fdb*a)/180),using the binary32 pi constant.
Request the original double sin service at0x80272410,round its result to
single,then write **P+0xa0=R(m*roundedSin)**. Request the double cos service
at0x80271ea8,round to single,negate it and write
**P+0xa8=R(m*negativeRoundedCos)**. The sin/cos implementations,FPSCR and
angleABf numeric domain are separate dependencies;these are ordered calls,
not host-libm equivalence evidence.

Finally return signed **P+0x52**. Inside the box with stationary owner speed,
the returned counter may remain nonzero even though it did not increment on
this call. Outside the box a negative counter also remains nonzero. The caller
tests nonzero,not positive,for its history reset policy.

## Verification and remaining work

tools/verify_field_partner_direction.py pins this body,direction/speed and
the system sincosf wrapper,64 instruction anchors,19 scalar constants and two
actual lecture imports. Reports and corrected Gekko listings remain ignored.
No original executable/dialogue payload is included in source history.

Next:angle/trig and collision numeric services,owner dimension/velocity/flag
writers,counter and history state domains,original overlap/following trajectories
and field/battle teardown. This local correction does not complete collision
response,partner locomotion or game equivalence.
