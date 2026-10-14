# Camera4 matrix publication: SDK arithmetic and mode2 projection

2026-10-14, original US G8ME01 disc0 revision0, read-only ISO/DOL evidence.
This refines EVT-01's [depth matrix producer](BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md).
It establishes instruction operation order, not original execution or bitwise
ARM/Gekko equivalence. Addresses/offsets are hexadecimal; matrix indices are
decimal row/column, with column-vector convention. R rounds to binary32;
F(a,b,c) means one binary32 fused multiply-add. Ordinary finite numeric formulas
below require the original paired-single GQR0/FPSCR domain or a declared native
numerical adapter. NaN payload/denormal/exception rules remain external.

## camMain roll, translation and final retained view

camMain8000f704 first invokes camera callback C1ec, then C_MTXLookAt8029e330
writes C11c from eye Cc, up C24, target C18. At8000f788..798 it reads roll
C14c, computes R(k*roll) with k at8041f6f0, bytes3c8efa35,
0.01745329238474369, then requests PSMTXRotRad8029dff0(out=stack13c,axis='z').
Roll is **degrees multiplied into radians**, with positive roll retained;
there is no local sign inversion, wrap or condition suppressing a zero roll.

PSMTXRotRad passes the input angle to sin80272888 first, retains its result,
then calls cos802728ac. It passes both FPR results to PSMTXRotTrig8029e060,
which frsp-rounds sine and cosine before its stores. Axis is case-folded with
OR20. For Z, s=R(sinResult),c=R(cosResult):

```
Z = [ c -s  0 0
      s  c  0 0
      0  0  1 0 ]
```

Constants1/+0 are80429358/35c. Other axis branches exist; invalid axis performs
no matrix stores. A native declared trig provider is necessary if the source
libm wrappers are not executed; don't present arbitrary float sincos as a
proved exact original service.

At8000f79c..7ac PSMTXTrans8029e230 creates a positive translation from
C150/154/158:

```
T = [1 0 0 C150
     0 1 0 C154
     0 0 1 C158]
```

This is initialization, not translation added to an existing matrix. Ordered
PSMTXConcat calls at8000f7bc and7cc publish `V=Z*V`, then `V=T*V` in C11c.
Do not combine into one matrix multiplication or skip identity-valued operations
when retaining instruction rounding and exceptional behavior. The field-only
Mario transform branch is skipped by nonzero GP14 in battle context.

At8000fa54 test HALF(C0) mask8. If set, PSMTX44Concat8029e994 at8000fa6c
publishes `P=C1a0*P` into C15c, then clears flag8. This consumes retained extra
projection only once per flag admission. A native owner must preserve C1a0 and
its flag/history; an identity assumption is unsupported without its producer.

## Exact concatenation order

PSMTXConcat8029dd64 receives r3=A3x4,r4=B3x4,r5=destination. For each row i
and j in0..3, first:

```
t = R(A[i,0] * B[0,j])
t = F(A[i,1], B[1,j], t)
t = F(A[i,2], B[2,j], t)
```

For j0/1, store t. For j2, store F(A[i,3],+0,t). For j3, store
F(A[i,3],1,t). The latter two come from paired constant vector[+0,1] at
8041e458. Do not replace j3 by an ordinary unfused t+Atranslation; do not omit
j2's zero multiply/add in exceptional/zero-sensitive domains. All source inputs
are loaded before the first destination store, so exact destination=A/B works.
Unproven shifted partial overlap is not implied by this in-place admission.

PSMTX44Concat8029e994 similarly evaluates each cell by:

```
t = R(A[i,0] * B[0,j])
t = F(A[i,1], B[1,j], t)
t = F(A[i,2], B[2,j], t)
out[i,j] = F(A[i,3], B[3,j], t)
```

Columns0/1 are held separately from2/3; the source load/store schedule supports
the actual destination=B request in camMain. It is not a grouped pair-sum dot,
double accumulator, or compiler freedom to fuse the initial multiply with a
later sum. This order differs from the SDK vector projection helper's paired
sum order documented in the depth consumer contract.

## Perspective projection from cam3dMain mode2

With camera HALF2 mode2, nonzero GP14 admits battleCameraMain8012c8b0 at
8000f1e4. After the actual controller, source PSVECDistance8029eeb4 stores
C118; angleABf8000703c stores C114. C19c==0 selects Perspective8029e7e8 at
8000f230: output C15c,FOV C38,aspect constant8041f6d8 bytes3fa22222
(1.2666666507720947),near C30,far C34.

C_MTXPerspective computes in this exact order, retaining the libm FPR result:

```
theta = R(.5 * FOVdegrees)
theta = R(radPerDegree * theta)
tangent = tan80272864(theta)
K = R(1 / tangent)   // no preceding frsp on tangent result
m00 = R(K/aspect)
m11 = K
D = R(far-near)
Q = R(1/D)
product = R(far*near)
m22 = R((-near)*Q)
m23 = R(Q*(-product))
m32 = -1
```

All other entries are source+0, including m33. Constants .5/radiansperdegree
are80429388/38c (3f000000/3c8efa35);1/0/−1 are80429378/380/384.
Negations are separate sign operations. No near/far/FOV/aspect validity guard
is present. In particular a source tangent FPR double followed by fdivs is not
necessarily equivalent to first narrowing tangent to float or using tanf.
Native libm substitution must be declared as a numerical provider.

## Mode2 orthographic branch uses a fixed half-angle

C19c!=0 selects8000f238..274. Load double at802bf638, bytes3fcbecde60000000,
0.21816615760326385, and call tan802724e8. Frsp-round the result. This is a
fixed authored half-angle; it does **not** read C38 on this branch. Then:

```
halfY = R(C118 * R(tanResult))
xNumerator = R(608 * halfY)
halfX = R(xNumerator / 480)
```

608/480 are8041f6dc/e0. Call C_MTXOrtho8029e8b8(C15c,top=halfY,
bottom=-halfY,left=-halfX,right=halfX,near=C30,far=C34). Its exact arithmetic:

```
width = R(right-left)
height = R(top-bottom)
xSum = R(right+left)
xInv = R(1/width)
yInv = R(1/height)
m00 = R(2*xInv)
m03 = R(xInv*(-xSum))
ySum = R(top+bottom)
zSpan = R(far-near)
zInv = R(1/zSpan)
m11 = R(2*yInv)
m13 = R(yInv*(-ySum))
m22 = R((-1)*zInv)
m23 = R((-far)*zInv)
m33 = 1
```

All other entries+0. The translation uses −far rather than −near; don't copy
perspective m23 into this projection. Zero/extreme extents are not guarded.
There is no use of a 3DS tilted projection/depth convention in these source
publication services. Renderer hardware projection and source sorting matrix
remain separate bindings.

## Evidence identities and remaining owners

Fresh ignored ELF/disassembly: artifacts/battle-camera-matrix-evidence.
Paired-single text independently corrected with tools/gekko_disasm.py; original
bytes read directly from ISO. No source public reconstruction is needed for
these operation-order findings.

| Original function range | SHA256 |
| --- | --- |
| PSMTXConcat8029dd64:cc | de50e27eaa9d4816ce7462637664e423cba8f7a85196f965a830331828e2ad37 |
| PSMTX44Concat8029e994:104 | 0d9ba046c7716fc7aeb7f32b95918b1e5cb23b69f727aae302d6b1a75d5474c5 |
| PSMTXRotRad8029dff0:70 | 76e5350aa14c58b81f57a50f132c5a9da14d7dd134db4bbe2b439327a756dec0 |
| PSMTXRotTrig8029e060:b0 | e0172ba54e8a396853cbf52188ff9ce1333037b83bdfa60837e02e4d9417beb7 |
| PSMTXTrans8029e230:34 | 617da755381d59f14f4d26dc4f016ce25b3005cbafdab53e7b89556303a695f9 |
| C_MTXPerspective8029e7e8:d0 | dc64426e64e14379313f2d35df8daf6f4fd47f5d476243e7436caf4ccbbaf7b5 |
| C_MTXOrtho8029e8b8:98 | 8a7c696bfe1df8825f9a8ed81b22e2410a03eaf226460acec844cd4b5bf1669b |
| camMain8000f704:3c4 | fd7074046f87e9b4806121364396793eff9fb0e43454df0d33fd46dd2d1e26c4 |

This delivery does not implement camera controller/history, exact LookAt
normalization, source trig/FPSCR/GQR services or native matrix publication.
Typed numerical adapters can implement the admitted finite domain while
retaining source order, but actual lifetime/cadence and exceptional results
remain separately verified requirements. EVT-01 and prologue parity stay open.
