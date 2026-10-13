# Game calc3D spatial result and table-driven angle

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete calc3D800dc6d4/4f4 and
angleABf800dcbc8/1b4hex (the pmario_sound.o symbol, not another same-named body).
Read [pending SFX caller](POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md).
All offsets/addresses hexadecimal; arithmetic literals below decimal unless hex
prefixed. Static request/body recovery, not original audible/numeric observation.

## Inputs, point precedence and captured listener heading

calc3D inputs: raw key K, position pointer P, raw volume V, raw distance D.
Copy P's three words to a local XYZ before testing flags; no null guard.
Listener state is psnd803df6e8: XYZ floats+30/+34/+38, heading float+3c.
If K&0x01000000, add listener XYZ to local XYZ using three fadds.
THEN if K&0x02000000, replace local XYZ with fresh listener words. Replacement
wins when both flags are set. No early skip of the initial P reads or arithmetic.
Remaining key bits do not alter this body. Volume uses low8, distance low16.

Read listener X/Z, capture heading H, read local X/Z and request
angleABf(listenerX,listenerZ,localX,localZ). Listener position is not captured
once for the whole function: later distance calculations reload each coordinate.
No coherent listener snapshot/concurrency invariant is supplied locally.

## Complete angleABf/table helper for admitted finite coordinates

Compute dx=fsubs(targetX,listenerX), dz=fsubs(targetZ,listenerZ), ax=abs(dx),
az=abs(dz). If ax>az, let t=fmuls(45,fdivs(az,ax)); otherwise, if az==0
return0, else t=fmuls(45,fdivs(ax,az)). These are single-precision operations.
Multiply t by2 with fmuls; derive table index by fctiwz(value+double0.5) for
ordered nonnegative, otherwise negate fctiwz(value-double0.5). There is no index
clamp/null/finite guard. Read float angleABTBL8032dbf4[index], then
A=fmuls(t,tableValue). Original table is91 floats, covering indices0..90 for
ordinary finite admitted ratios; this is not atan2 or interpolated table lookup.

Branch return, with single fadds/fsubs and ordered sign comparisons:

| Dominance | dx sign | dz sign | Returned angle |
| --- | --- | --- | --- |
| ax>az | >=0 | >=0 | 90+A |
| ax>az | >=0 | <0 | 90-A |
| ax>az | <0 | >=0 | 270-A |
| ax>az | <0 | <0 | 270+A |
| ax<=az | >=0 | >=0 | 180-A |
| ax<=az | <0 | >=0 | 180+A |
| ax<=az | >=0 | <0 | A |
| ax<=az | <0 | <0 | 360-A |

The zero-az return precedes the second branch table lookup. Unordered comparisons
follow literal PPC branch outcomes and are not equivalent to every finite table
row above; NaN/Inf/fctiwz/index domains need platform/source evidence.

## Heading difference, normalized direction and pan fold

Let returned angle A. Compute fabs(fsubs(A,H)); if ordered >=180, ordered A<H
adds360 to A, otherwise subtracts360. Equality at180 takes this adjustment.
Then u=fdivs(fsubs(A,H),180). No repeated wrapping or heading normalization.

Use fmadds(127,u,127), fctiwz to word W. Compare SIGNED low16(W): below64
replaces W with signed16(127-W); above196 replaces it with signed16(392-W).
Those thresholds are literal64/196, not an inferred symmetric192 endpoint.
Then pan=clamp(signed16(W-64),0,127). All compares/truncations occur before the
final byte write. Invalid floating conversions are not assigned host fallbacks.

## Ordered distance arithmetic and reciprocal-square-root refinement

Reload listener Y/X/Z as their respective local coordinates are read. Compute
dy=fsubs(localY,listenerY), dx=fsubs(localX,listenerX), dz=fsubs(localZ,listenerZ).
Squared distance Q uses fmuls(dy,dy), then fmadds(dx,dx,prior), then
fmadds(dz,dz,prior), retaining single instruction order/fused rounding.

If ordered Q>0, use frsqrte(Q), followed by THREE double-precision reciprocal
sqrt refinement steps. Each forms y*y, 0.5*y, fnmsub(Q,y*y,3), then multiplies
these last two; the third result is multiplied by Q and rounded with frsp.
This hardware-estimate/refinement sequence is not replaced by host sqrt.
If not Q>0: ordered Q<0 returns canonical __float_nan8041e3c8; otherwise classify
stored float bits. NaN class uses the same canonical constant; remaining values
retain Q. Positive infinity enters the Q>0 path; its result is not inferred from
this fallback classification. Let resulting distance R.

## Surround direction/distance transform

Request cos(frsp(double3.14*u))80271ea8, round return with frsp, then
T=fabs(fmuls(R,cosResult)); ordered T>2048 clamps T to2048.
Compute E=fdivs(fmuls(64,T),2048). If ordered abs(u)<0.5:
C=fctiwz(fsubs(64,E)), enforce signed16(C)>=1. Otherwise C=fctiwz(fadds(64,E)),
enforce signed16(C)<=127. These are one-sided intermediate clamps.

Sign-extend C. For C<=64, degrees=trunc_toward_zero((64-C)*90/64);
otherwise degrees=trunc_toward_zero((C-64)*90/64). Convert via source integer bias,
fmuls(6.2831854820251465,degrees), fdivs(prior,360); request sin80272410 and frsp.
For C<=64, use fnmsubs(64,sinResult,64), representing fused64-64*sinResult;
otherwise fmadds(64,sinResult,64). fctiwz to word, then signed16 clamp0..127.
This is surround byte S. Table/libm/PPC numeric behavior is separate from a
simple geometric surround assumption; these constants are static source values.

## Volume falloff and shared packed return

Dlow16==0 stores Vlow8 directly. Otherwise let range=(Dlow16*100), exactly
integer-converted by source bias/fsubs; delta=fsubs(R,200). Ordered delta>range
sets delta=range, ordered delta<0 sets0. With each step using its source single
operation, compute:

`remaining=fsubs(range,delta); scaled=fmuls(127,remaining); ratio=fdivs(scaled,range)`

`weighted=fmuls(float(Vlow8),ratio); out=fdivs(weighted,127); word=fctiwz(out)`.

Store word LOW BYTE first, THEN reload that byte and cap unsigned values>127
to127. This is not an integer-word clamp before narrowing; invalid/out-of-domain
conversion/rounding can interact with low-byte wrapping. D!=0 gives nonzero
finite range for ordinary source values, but other numeric domains remain open.

Shared PAN$4553=word8041eb18: byte0 volume, byte1 pan, byte2 surround. Store
volume as above, then pan byte1, then surround byte2; return fresh whole word.
Byte3 is NEVER written here and retains prior shared state. No local result
pointer, clear, IRQ, backend dispatch or audio playback. Caller uses returned
packed bytes; repeated/reentrant calls share this result storage.

## Validation and remaining source/runtime

[Manifest](../../../tools/game_calc3d_angle_evidence.json) pins both complete
bodies, two linked cos/sin bodies, original91-float table and four literal spans:
9 spans,161 original instruction anchors and4 resolved direct calls. Fifteen
pan-fold boundary vectors preserve signed-width/196-threshold behavior. Full
fresh ELFs/listings remain `/tmp/inbox-calc3d.elf` and `/tmp/inbox-angleab.elf`.
No original function is executed or numerical approximation measured.

Remaining: listener/shared-byte writers/caller domains, original heading/flag/
finite-coordinate invariants, PPC frsqrte/fused/fctiwz/NaN and libm equivalence,
live SFX updates/backend/audio observations. Two complete local bodies/table
are answered; full spatial/native/source/runtime equivalence remains OPEN.
