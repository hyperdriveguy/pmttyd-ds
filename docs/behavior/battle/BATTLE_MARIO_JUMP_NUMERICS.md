# Mario jump numerical operations and angle normalization

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete MarioJumpPosition0x801093a8/0x8c0 and
reviseAngle0x80007328/0x60,with original constants. Read alongside
[motion order/state](BATTLE_MARIO_JUMP_MOTION.md). Numeric helper execution,
frsqrte/sin/fmod/FPSCR fidelity and actual caller domains remain dependencies;
the equations below preserve instruction operations rather than prove a host
math library equivalent to original Gekko execution.

## Notation and original constants

`sadd/ssub/smul/sdiv` mean the respective original single-precision instructions.
`r32` means original frsp or single-result conversion,retaining its floating
environment behavior. `fma32` and `fma64` are fused single/double operations;
fused subtract `fs32(a,b,c)` computes c-a*b in one original fnmsubs operation.
`i32(x)` is the original fctiwz conversion;`wrap32` is word addition/narrowing.
`F(n)` is the bias-based signed32->single conversion used by this body. Do not
replace every intermediate with unbounded double arithmetic or unfused products.

| Constant | Original address | Bytes |
| --- | --- | --- |
| Positive single0 | 0x80422280 | 00000000 |
| Single360 | 0x804222bc | 43b40000 |
| Single2pi,6.2831854820251465 | 0x804222c0 | 40c90fdb |
| Singlepi,3.1415927410125732 | 0x804222c4 | 40490fdb |
| Single90 | 0x804222c8 | 42b40000 |
| Single1.8,1.7999999523162842 | 0x804222cc | 3fe66666 |
| Signed conversion bias | 0x802ee608 | 4330000080000000 |
| Double20 / 6 / 47 | 0x802ee6f0 / 0x802ee6f8 / 0x802ee700 | 4034000000000000 / 4018000000000000 / 4047800000000000 |
| Double1.12 / 1.0 / 0.53 | 0x802ee708 / 0x802ee710 / 0x802ee718 | 3ff1eb851eb851ec / 3ff0000000000000 / 3fe0f5c28f5c28f6 |
| Double1.25 / 0.8 | 0x802ee720 / 0x802ee728 | 3ff4000000000000 / 3fe999999999999a |
| Double16.5 / 33 | 0x802ee730 / 0x802ee738 | 4030800000000000 / 4040800000000000 |

The decimal labels for0.53,0.8 and1.12 are descriptions of these binary64
patterns,not a license to replace them with decimal-exact rational arithmetic.
The independently recovered register base r2=0x80427440 locates these single
constants;the callback's retained base802ee458 locates its doubles.

## Count, speed and fused residual

After target/current initialization,let D=stored original distABf return,
S=unit170 before adjustment,N=resolved count. D and S are binary32.

- If N==0:store n=i32(sdiv(D,S));retain S;compute
  R=fs32(F(n),S,D). If n remains0,return2 before any divisions by F(n)
  in the curve setup. Earlier initialization stores remain visible.
- If N!=0:store n=N;store S=sdiv(D,F(n));compute R=fs32(F(n),S,D).
- With nonzero n,store each baseline delta as sdiv(ssub(T_i,P_i),F(n));
  Phi=positive0;G=sdiv(singlePi,F(n));S=sadd(S,sdiv(R,F(n))).

The explicit-count branch recomputes speed from distance/count before residual.
R is an FPR temporary,not a persistent unit residual-distance store. Baseline
integration uses stored deltas,not the adjusted speed or heading. Heading/D
are calculated from raw resolved input X/Z even when target sentinel rules
retained a different T_i. Do not derive them from stored target automatically.

No n<=0 check,min/max count clamp or division guard is added for nonzero N.
The fctiwz/divide/word behavior for exceptional inputs must remain separate
from any port-side malformed-data handling. Current ordinary callers use the
count output from [MarioJumpParam](BATTLE_MARIO_JUMP_PARAMETER.md).

## Mode-selected angle, height and first phase advance

Initialize theta=single90. Modes use signed M comparison:

| Mode region | Angle step omega | Phase coefficient C | Amplitude multiplier |
| --- | --- | --- | --- |
| M<3,except initialization-skipping M1 | sdiv(single360,F(n)) | Double0.53 | Double1.12 only for M2 |
| M>=3 | F(signed integer division360/n) | Double0.8 | Double1.25 only for M4 |

The second region uses integer divw before conversion. For n42,the first
region's step is approximately8.5714283 while the second's is exactly8. Negative
mode values belong to M<3;other modes within either region retain that region's
base shape. M1 skips normal setup and has its separate phase2/3 path.

Both regions form height H from stored D in this order:

1. H=r32(double(D)-double20).
2. H=r32(double(H)/double6).
3. H=r32(double(H)+double47).
4. For M2 or M4 only,H=r32(double(H)*the relevant double multiplier).

There is no amplitude sign/minimum clamp. Each listed rounding occurs before
the next double operation;rewriting `(D-20)/6+47` as one expression changes
rounding. Then form the first phase update on the initialization call:

`rad = sdiv(smul(single2pi,theta),single360)`

`q = r32(originalSin(double(rad)))`

`factor = fma64(C,double(q),double1)`

`Phi = r32(fma64(double(G),factor,double(Phi)))`

This updated Phi is used immediately by the first phase1 integration in that
same callback invocation. It is not a sample of theta directly. The same mode-
selected update runs after every phase1 visible-position calculation,including
the final n0 invocation before terminal Phi/G reset.

## Phase1 visible position and termination order

Add delta to each baseline axis using separate sadd operations. Evaluate
q=r32(originalSin(double(Phi))). Store visible position
`(P_x,fma32(H,q,P_y),P_z)` through SetPos.

The target-Y clamp requires ordered T_y>visibleY and **old n<3**. Then advance
Phi with current M/G/theta as above,theta=reviseAngle(sadd(theta,omega)),and
n=wrap32(n-1). Return0 unless n==0 exactly. At n0,SetPos using targetY and
current baseline X/Z,then Phi=-ssub(lastY,lastY),G=single1.8,return1.
The lastY field retains the pre-terminal step/clamp Y rather than being
overwritten by that final SetPos when it snaps down from an above-target Y.

Negative zero from finite self-subtraction/negation is retained. Infinity/NaN
self-subtraction must still execute;unconditionally assigning negative zero
would replace original exceptional values. Terminal return1 advances and yields
at the VM level;it is not another blocked call followed by return2.

## Phase2/3 exceptional mode1 route

After eligible phase2 setup,n=1,G=single1.8,Phi=-ssub(lastY,lastY),theta=90,
omega=F(360/n). H=r32(double(abs(ssub(lastY,lastY)))/double16.5).
Finite lastY therefore gives H0. GetPos copies current visible XYZ into
baseline154/158/15c. Phase2 then changes the event state to3 and returns0.

Phase3 computes rad=sdiv(smul(single2pi,theta),single360) twice through separate
sin requests. First q1=r32(originalSin(double(rad))),product=smul(H,q1),
then P_x=r32(double(P_x)+double(product)/double33).
For the second sin result q2,P_y=fs32(H,q2,P_y). Preserve the repeated call and
single multiply versus double divide/add path. Then advance/normalize theta,
SetPos from baseline XYZ,decrement n and return1 exactly at0,otherwise0.
This route does not use phase1's final target snap or Phi/G reset.

## Complete local reviseAngle wrapper

reviseAngle at0x80007328 calls original fmod(input,double360 from0x802bf240),
then rounds its return to single. If that rounded value is unordered with
itself (NaN),replace it with positive single0 at0x8041f448. If the resulting
value is ordered<0,add single360 at0x8041f45c;when that single sum is>=360,
replace it with positive0. Otherwise retain the value. Return the single result.

Negative zero does not satisfy<0 and is retained. NaN normalization is a local
wrapper behavior;underlying original fmod exceptional/FPSCR/errno behavior
remains a dependency. This is not a repeated integer angle-subtraction loop.

## Numerical implementation requirements

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| MJP-NUM-01 | Preserve mode-specific division and height rounding | M0/n42 float step differs from M3/n42 integer8;M2/4 amplitude multipliers apply after three explicit roundings |
| MJP-NUM-02 | Preserve fused phase/visible formulas and independent baseline | Sine offset does not feed back into baselineY;initial Phi advances before first position;phase1 performs two sin calls per step plus its setup call |
| MJP-NUM-03 | Preserve terminal and normalization exceptional/sign paths | Exact countdown0 only;finite lastY generates negative-zero Phi;reviseAngle preserves negative zero and normalizes NaN to positive0 |
| MJP-NUM-04 | Retain original math-service dependencies for equivalence | Differential capture records distance/angle/sin/fmod/FPSCR/intermediate bits as well as visible positions before claiming original accuracy |

tools/verify_mario_jump_motion.py pins the instruction sites/constant words and
full motion/reviseAngle bodies. Angle/distance/sin/fmod numeric execution is not
proved by those hashes or a native host curve plot. Derived cases are host-test
obligations,not measured original trajectories or rendering-frame durations.
Original inspection artifacts stay under /tmp or ignored paths;ISO reads are
read-only and original asset data is not copied into source history.
