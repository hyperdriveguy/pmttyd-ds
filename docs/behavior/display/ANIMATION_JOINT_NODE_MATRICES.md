# Joint node matrix construction and literal scale gates

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
pushGXModelMtx_JointNode__0x8003e2a8,size0x544. Called by
[group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md) when group's joint
word+54 is nonzero. Input node pointer N r3,parent scale triple P f1/f2/f3.
No original execution. No node/null/length guard;captured values precede all SDK calls.

## Captured fields and component requests

Capture N+00/+04/+08 as translation t;N+0c/+10/+14 as own scale s;
N+18/+1c/+20 multiplied SINGLE by exact2 as rotation triple r;
N+24/+28/+2c as secondary rotation q. Incoming parent scale captured unchanged.
All arithmetic below retains original single rounding/FPSCR behavior. Local
component presence mask F starts0;bit1=T,bit2=Qparent,bit4=Sown,bit8=Rrotation.

Translation:if all three components compare equal0 via fcmpu,skip;otherwise
request PSMTXTrans(t) and set bit1. Unordered/NaN admits;negative0 equals0.

Parent compensation has a literal asymmetric gate at8003e398..8003e404:

```
if px != 1: admit
elif py != 1: admit
elif pz == 1: admit
else: skip
```

Equality is fcmpu equality;!= includes unordered. Thus all ones ADMITS,while
(1,1,z!=1) skips,including a NaN Z in that last position. For admitted input,
if ANY parent component equals0,request Scale(1,1,1) for ALL axes. Otherwise
request Scale(single1/px,single1/py,single1/pz). Set bit2 in either case.
Do not apply per-axis zero fallback or assume a conventional nonidentity gate.

Own-scale gate at8003e404..8003e438 has the same asymmetry:
if sx!=1 OR sy!=1 OR sz==1,request Scale(sx,sy,sz) and set bit4;
otherwise skip. There is no zero fallback for own scale. All ones issues an
identity scale;Z-only nonidentity can be omitted by this local body.

## Six ordered rotation requests

ALWAYS initialize R using PSMTXIdentity. For each admitted angle (!=0 by fcmpu),
compute single angle*0x3c8efa35 (0.01745329238474369),request
PSMTXRotRad(axis,...),then Concat(R,axisMatrix,R),set bit8.
Request order is secondary qZ,qY,qX,then doubled rZ,rY,rX. Each loop is unrolled;
no angle wrapping,bounds or common Euler collapse. Original local rotation is
therefore accumulated with six RIGHT multiplications in that order. A zero
angle skips its rotation/concat requests,including the extra rounding they
would introduce. SDK trig/concat local graphs are documented separately.

This builder reads only N offsets00..2c;transform-node pivot fields30..5c are
not used by this body. Group traversal supplies parent scale according to its
own parent/joint predicates;the builder does not reread parent pose/groups.

## Sixteen dispatch combinations

After component generation,increment global modeling level and publish its
pointer BEFORE selecting/composing the local matrix. Complete jump table at
0x80306698 maps each presence mask to these requests (each product is an
explicit PSMTXConcat;grouping is intentional):

| F | Local matrix/result requests |
| --- | --- |
| 0 | fresh Identity |
| 1 | T |
| 2 | Qparent |
| 3 | T*Qparent |
| 4 | Sown |
| 5 | T*Sown |
| 6 | Qparent*Sown |
| 7 | (T*Qparent)*Sown |
| 8 | Rrotation |
| 9 | T*Rrotation |
| 10 | Qparent*Rrotation |
| 11 | (T*Qparent)*Rrotation |
| 12 | Rrotation*Sown |
| 13 | (T*Rrotation)*Sown |
| 14 | (Qparent*Rrotation)*Sown |
| 15 | ((T*Qparent)*Rrotation)*Sown |

A single-component route passes the already built matrix directly;it does not
concatenate identities. Triple/quad routes use distinct scratch slots;rotation
accumulation aliases first input/output. Final stack/world composition is in
[stack and SDK affine numerics](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md).
These functions do not read work+cc/d0/d4 override pointers locally.

`verify_animation_node_matrices.py` pins full bodies,instructions,dispatch words,
constants and address producers. Review parent(1,1,2),parent(2,1,1),parent zero,
own(1,1,2),own allones,NaN last axis and all six rotation orders. These are
static acceptance cases;resource/source/stack/FPSCR/libm/render/runtime remain open.
