# MusyX listener update, cross product and matrix inverse

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndUpdateListener8028c7b0/174,
salCrossProduct8028dd6c/64 and salInvertMatrix8028ddd0/1fchex.
Read [actual game caller](GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md)
and [3D reset](MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md).
Offsets/addresses hexadecimal; matrix equations below label element indices.
Static source/numeric operation order, not hardware/runtime observations.

## Complete listener admission and four-vector writes

Inputs r3..r8: listener L, vector pointers P/V/F/U, raw volume X. Input r9 is
not read. Capture sndActive8041ee60 once and retain inputs. Inactive returns0
without input dereferences/IRQ. Active requests hwDisableIrq, then copies:

| Source | Listener destination words |
| --- | --- |
| P | +c/+10/+14 |
| V | +1c/+20/+24 |
| F | +28/+2c/+30 |
| U | +40/+44/+48 |

For EACH vector: read X/Y words, store them in order, THEN read/store Z.
Vectors execute in listed order; aliasing can affect later reads. No field/flag/
list-membership/generation/null/finite guard, whole-vector snapshot, integration
or direction/up normalization. The caller supplies basis meanings; raw words
are copied here. Active does not establish a valid allocated listener pointer.

## Cross product, stack matrix and inverse request

Request salCrossProduct(L+34,L+28,L+40). Its output is F×U with the source
separate multiply/subtract rounding described below. No normalized result.
Then assemble stack matrix using fresh listener float reads in this order:

- Load cross XYZ to stack offsets0/c/18.
- Load U XYZ to offsets4/10/1c.
- Load F XYZ, fneg each, to offsets8/14/20.
- Load position XYZ from L+c/+10/+14 to offsets24/28/2c.

Thus row-major3x3 rows are [crossX,upX,-forwardX],
[crossY,upY,-forwardY], [crossZ,upZ,-forwardZ], followed by position triple.
Request salInvertMatrix(L+4c,&stackMatrix). Input/output are separate in this
caller, unlike unrestricted helper alias domains. Result is not checked and
there is no fallback identity/retry/rollback if singular or nonfinite.

After inverse request, derive volume=U8(X)/127 with pinned unsigned integer
bias/fsubs and fdivs; store float to L+88. No clamp at1 or before truncation;
low-byte128..255 exceeds1 in ordinary finite arithmetic. Enable MusyX IRQ;
return1. No listener list mutation, audio controller/voice call or success test
for children. Return1 is active-system admission, not valid/invertible geometry.

## Complete cross product helper and alias order

Inputs output O, A, B. For each component in X/Y/Z order, fresh load the four
needed source floats, compute TWO fmuls then fsubs, immediately store output:

`O.x=S(S(A.y*B.z)-S(A.z*B.y))`

`O.y=S(S(A.z*B.x)-S(A.x*B.z))`

`O.z=S(S(A.x*B.y)-S(A.y*B.x))`.

S denotes the indicated single instruction's rounding, not a host float oracle.
No fused multiply/subtract, source snapshots, length normalization, allocation,
status or pointer/alias check. If output aliases an input, subsequent components
can read earlier output stores. Return r3 remains output pointer unchanged.

## Complete matrix inverse helper

Input I has row-major3x3 [a,b,c;d,e,f;g,h,i] at offsets0..20, plus
translation tx/ty/tz at24/28/2c. Output O is the same12-float layout.
Every arithmetic operation below is a separate single fmuls/fsubs/fadds/fdivs;
no FMA replacement. Retain first-column cofactors and reciprocal:

`u=S(S(e*i)-S(h*f)); v=-S(S(d*i)-S(g*f)); w=S(S(d*h)-S(g*e))`

`det=S(S(c*w)+S(S(a*u)+S(b*v))); r=S(1/det); negR=-r`.

Write first three output values in order O0=r*u, O.c=r*v, O.18=r*w,
with fmuls rounding each. Then for EVERY remaining cofactor reload the four
input floats after preceding output writes, multiply twice, subtract, scale:

| Store order | Output offset | Computation (single operations) |
| --- | --- | --- |
| 4 | 4 | negR*(b*i-h*c) |
| 5 | 10 | r*(a*i-g*c) |
| 6 | 1c | negR*(a*h-g*b) |
| 7 | 8 | r*(b*f-e*c) |
| 8 | 14 | negR*(a*f-d*c) |
| 9 | 20 | r*(a*e-d*b) |

No determinant-zero/epsilon/finite/conditioning check. In-place O==I is not
safe by a source snapshot guarantee: later cofactor reads observe prior stores.
No helper rejection path, null/bounds guard or status assignment.

Finally output translations at24,28,2c, in order. For row offsets[0,4,8],
then[c,10,14], then[18,1c,20]: freshly load input tx/ty/tz and that output row.
Use fneg(tx), separate products (-tx)*row0, ty*row1, tz*row2, then two fsubs
((first-second)-third); store corresponding translation. Translation inputs
and output row are reread each time; source/output aliasing can affect later rows.
Return r3 unchanged as output pointer. No 4x4 fourth-row initialization occurs.

## Source checks and remaining numerical/runtime domains

[Manifest](../../../tools/musyx_listener_update_math_evidence.json) pins three
complete bodies, four linked IRQ/register-helper bodies and three literal spans:
10 spans/229 instruction anchors/six resolved direct calls.
Derived stack-field/store-order metadata, two exact admitted matrix examples and
one cross-product example remain static deductions. They do not establish PPC
rounding/exception or numerical equivalence for arbitrary bases. Full fresh ELFs/
listings stay `/tmp/inbox-listener{update,cross,matrix}.elf`.

Remaining: actual listener allocation/basis/volume providers and list ownership,
valid nonalias/null/finite/invertible domains, PPC single/exception semantics,
CalcEmitter/consumer transforms and original geometry/audio traces. Three complete
local bodies are answered; full source/native/hardware/runtime remains OPEN.
