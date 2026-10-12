# Transform node matrices:pivot admission and ordered translation offsets

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
pushGXModelMtx_TransformNode__0x8003e7ec,size0x540. Called by
[group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md) when joint word+54
is zero. Input node N r3;no parent-scale inputs are consumed. No execution.

## Captured fields and initial components

Before any SDK request,capture translation N00/04/08,scale N0c/10/14,
primary angles N18/1c/20 SINGLE-multiplied by2,rotation pivot N30/34/38,
scale pivot N3c/40/44,rotation pivot offset N48/4c/50,
scale pivot offset N54/58/5c. Fields N24/28/2c are NOT read by this builder;
[the joint builder](ANIMATION_JOINT_NODE_MATRICES.md) uses those secondary angles.
Do not apply both angle triples to every node type.

Local mask F starts0:bit1 translation T,bit2 scale component S,bit4 rotation R.
If translation ANY component!=0 via fcmpu,request Trans(t) and set1.
If scale ANY component!=1 via fcmpu,request Scale(s) and set2;allones skips.
Unlike the joint builder,this own-scale gate is an ordinary three-axis equality
test. Unordered values admit;no finite/zero clamp is local.

## Scale pivot and offset are conditionally admitted

Let pivot p=N3c/40/44,offset d=N54/58/5c. If ALL pivot components equal0,
skip the ENTIRE pivot/offset block,including nonzero d. If p has any nonzero:

- Existing F bit2 SET:request Trans(−p),Trans(p),Trans(d) in that order.
  Then request C=Trans(d)*Trans(p),C=C*S,S=C*Trans(−p).
  Offset translation is requested even when d==0;each product has its own
  rounding/call/alias behavior. Bit2 already remains set.
- Existing F bit2 CLEAR:if d has any nonzero component,replace S with
  Trans(d) and set2;otherwise retain absent scale component.

Thus a zero scale pivot suppresses its offset even when nonzero scale exists.
Do not replace this gate with an unconditional T(p+d)*S*T(−p) formula or fold
adjacent translations:that changes valid input behavior/request rounding.

## Rotation accumulation and pivot

ALWAYS initialize R=Identity. For doubled primary Z,Y,X angles,in that order:
if angle!=0 via fcmpu,single multiply0x3c8efa35,RotRad(axis),
Concat(R,axisMatrix,R),set4. Secondary angles are absent from this body.
No angle wrapping or epsilon;zero angles skip requests.

Let rotation pivot p=N30/34/38,offset d=N48/4c/50. If all p==0,skip the
ENTIRE offset block. Otherwise:

- F bit4 SET:request Trans(−p),Trans(p),Trans(d) in that order.
  C=Trans(d)*Trans(p);R=C*R (output aliases SECOND input R);
  R=R*Trans(−p) (output aliases FIRST input R).
- F bit4 CLEAR:if any d!=0,request R=Trans(d),set4;else leave absent.

Translation-only scale/rotation components retain their corresponding mask bits;
these names identify stack dispatch slots,not guarantees of diagonal/orthogonal
contents. No inverse parent-scale compensation occurs locally.

## Eight dispatch combinations and stack publication

Increment/publish modeling level/pointer BEFORE dispatch. Original table
0x803066d8 supplies these combinations:

| F | Local result requests |
| --- | --- |
| 0 | fresh Identity |
| 1 | T |
| 2 | S |
| 3 | T*S |
| 4 | R |
| 5 | T*R |
| 6 | R*S |
| 7 | (T*R)*S |

Use [stack/affine contract](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md)
for final previous-world/local/current-world concat. The builder does not consume
work override cc/d0/d4 locally. `verify_animation_node_matrices.py` pins complete
local branches/tables/SDK bodies. Review pivot0/nonzero offset,pivotnonzero/
identity component/nonzero offset,nonidentity component/zero offset,secondary
angle-only input and mixed mask routes. Full loaded-node/source/alias/FPSCR/
libm/stack/render and original visuals remain open.
