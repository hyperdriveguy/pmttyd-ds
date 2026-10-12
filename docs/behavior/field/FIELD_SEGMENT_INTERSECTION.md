# Field segment intersection and lecture coordinate sampling

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
evt_sub_check_intersect at DOL 0x80053c4c, size 0x2c4, and
evt_mario_get_pos at 0x800e71f8, size 0xc8. Original instructions were inspected
with Gekko paired-single correction. This is static numeric behavior, not an
original execution trace or a measured trigger location.

## Input and output ABI

evt_sub_check_intersect resolves operands 0..7, in order, with evtGetFloat.
Let them be A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y. Operand 8 is a raw destination
for evtSetValue; it is not resolved as an input. Every normal exit writes 0
or 1 and returns callback code 2. There is no first-call gate, hidden iteration
state, tolerance parameter or division. The routine compares the two closed
segments AB and CD through coordinate rejection and two signed-product tests.

Here .y means the second coordinate of this **2D service**, not necessarily
the game's height axis. The caller chooses the components.

## Ordered coordinate rejection

For each axis, in X then Y order, use a=A.axis,b=B.axis,c=C.axis,d=D.axis:

```
if ordered(a >= b):
    reject if (a < c and a < d) or (b > c and b > d)
else:
    reject if (b < c and b < d) or (a > c and a > d)
```

All rejection inequalities are strict. The first branch is formed by
fcmpo followed by CR OR of greater/equal, so unordered comparison takes its
else arm. Do not replace the actual branches with a sorting helper that has
different NaN behavior. Rejection writes destination 0 and returns 2
immediately; later arithmetic does not execute.

For finite ordinary coordinates this is the closed bounding-interval overlap
test. Both segments may have zero extent on either axis. Endpoint equality
does not reject. No segment endpoint is reordered in memory.

## Rounded cross-product tests

R denotes the instruction's single-precision result rounding; F(a,b,c)
denotes single-precision **fused multiply-add**. Preserve each fsubs, fmuls
and fmadds boundary, operand order and the configured Gekko floating-point
environment. First construct:

```
u = R(A.y - B.y)
v = R(A.x - B.x)
sC = F(v, R(C.y - A.y), R(u * R(A.x - C.x)))
sD = F(v, R(D.y - A.y), R(u * R(A.x - D.x)))
reject if ordered(R(sC * sD) > +0.0)
```

The exact threshold constant at 0x8041fe38 is binary32 positive zero.
If the first product does not reject, construct:

```
u = R(C.y - D.y)
v = R(C.x - D.x)
sA = F(v, R(A.y - C.y), R(u * R(C.x - A.x)))
sB = F(v, R(B.y - C.y), R(u * R(C.x - B.x)))
reject if ordered(R(sA * sB) > +0.0)
```

Otherwise write 1 and return 2. The first six subtractions/multiplications
and fmadds are at 0x80053e10..0x80053e38; the second set at
0x80053e60..0x80053e84. Product comparisons follow at 0x80053e40 and
0x80053e8c. A host double-precision determinant or two separate operations
in place of fmadds is not a proven equivalent.

Product zero, including signed zero, is accepted by these tests. Underflow
of a positive product to zero can therefore affect acceptance. Unordered
products do not take the greater-than rejection branch, assuming the
floating-point exception environment permits normal continuation. Full
FPSCR, denormal and trapping behavior remains a platform dependency. Do not
substitute an epsilon or claim these branches provide robust exact geometry
for every numeric input.

For finite small integral inputs, with no overflow/underflow/rounding ambiguity,
these static acceptance examples follow directly:

| AB | CD | Destination |
| --- | --- | --- |
| (0,0)→(2,2) | (0,2)→(2,0) | 1 |
| (0,0)→(1,0) | (1,0)→(2,0) | 1, shared endpoint |
| (0,0)→(1,0) | (2,0)→(3,0) | 0, coordinate rejection |
| (0,0)→(2,2) | (0,1)→(1,2) | 0, same-side product |
| (1,0)→(1,0) | (0,0)→(2,0) | 1, degenerate segment |

These are deductions from branches, not Dolphin observations.

## Mario position source and the used lecture call

evt_mario_get_pos captures marioGetPtr before resolving operand 0 with
evtGetValue. Its raw operands 1..3 are evtSetFloat destinations. With
resolved operand 0 equal to zero, it writes the live Mario work's
float fields **+0x8c,+0x90,+0x94**, in that order. A nonzero mode writes
the same X and Z but **R(Y-1.0)**, using constant 0x80421ad8. It returns
2 and does not check a null Mario pointer. The lecture uses mode 0.

The LV destinations are encoded local words at entry+0x9c+4*n, not native
float slots. The used evtSetFloat branch computes R(value*1024), converts
with fctiwz, adds encoded base 0xf24a7a80 and stores the resulting word.
The evtGetFloat local branch decodes an encoded word by adding 230000000,
converting to single precision and multiplying by 1/1024; an ordinary local
integer takes the unscaled signed-integer-to-float branch. Thus normal field
coordinates pass through **1/1024 quantization**, and the setter's return value
is the previous decoded local value, ignored by this position callback. The
used namespace branches, not every namespace of these general services, are
closed here. Overflow, sentinel collisions and out-of-domain inputs require
the original conversion/dispatch behavior rather than an invented clamp.

The [lecture field event](../story/LECTURE_FIELD_ENTRY_AND_RETURN.md) consequently
passes current (LV0,LV2) and saved (LV3,LV5) against the fixed segment
(-300,250)→(-200,250). Initially LV5 copies LV2, but after an unsuccessful
poll and its one-frame wait, **LV5 copies LV1**, the height float. The
second and later calls therefore compare a saved X/height pair against
current X/Z. This unusual original script operand is now tied to the actual
float-writing service, rather than merely an annotation.

For example, a stationary position (-250,0,300), unchanged across two
polls, first supplies a point segment at Z=300 and is rejected. After the
original update, it supplies (-250,300)→(-250,0), intersects the fixed
segment and writes 1. That is a static composed example, not a claim that
such a stationary original field state is reachable or observed. Do not
fix the Y copy or require real movement crossing in an equivalent script
without first resolving original state/scheduling evidence.

## Verification and remaining dependencies

tools/verify_field_lecture_callbacks.py pins these bodies, arithmetic/branch
anchors, constants and three actual lecture position imports plus the
intersection import. See [party callbacks](../partner/FIELD_PARTY_EVENT_CALLBACKS.md)
for the remainder of its 15-body/54-anchor/6-constant/10-import scope. Twelve
bodies are specified completely; the three general variable services are
fingerprinted but only their used local-coordinate branches are specified.
Original bytes/listings and reports remain ignored.

evtGetFloat/evtSetFloat/evtSetValue variable namespace composition, Mario
coordinate writers/collision, floating-point environment, event scheduling,
map-entry position and original lecture trigger traces remain open. The
callback itself is fully specified locally; the complete original trigger
is not verified by its body hash or these examples.
