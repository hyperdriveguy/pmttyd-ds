# Slit collision membership: two ordered three-probe sides and normal-angle gate

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
unk_800c2010 at0x800c2010,size0x7b0. Its sole direct DOL caller is
[slit phase10](FIELD_PARTNER_SLIT_GATE.md) at0x8014c134,passing(P,&local root
copy). Nonzero retains slit;zero permits that caller's further owner/contact
exit tests. This is a membership query,not a movement correction or published
contact. No original slit/collision trajectory has been measured.

## Captures and direction requests

Inputs are partner P and vector V. Capture width W=P+f4 BEFORE direction
services. Request toMovedirSimple(270),then sincosf for its returned angle,
yielding u=sin,v=-cos. Compute offsets with TWO single multiplications each:
DX=round(0.5*round(u*W)),DZ=round(0.5*round(v*W)). They are retained for both
sides. No minimum width/absolute value/normalization is imposed. Negative or
exceptional widths follow the actual arithmetic/domain behavior.

Initialize side angles A=B=0. Request toMovedirSimple(180),then sincosf to
prepare the first side's direction(dX,0,dZ). The Y component is copied from an
original +0 template. Later,after completing the first side,request
toMovedirSimple(0) and sincosf AGAIN for the second direction. Each helper
requests current GLOBAL Mario/camera state;the second direction is not obtained
by negating the first. The
[complete camera-direction leaf](FIELD_SIMPLE_CAMERA_DIRECTION.md) supplies
quantization/wrap rules;[sincosf](FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md)
supplies degree conversion,libm request and sine/negative-cosine output order.

## Exact probe order and request layout

For each side,attempt these origins IN ORDER until a nonnull result:

| Attempt | Origin |
| --- | --- |
| Center | Current V.x,round(current V.y+10),current V.z |
| Plus | round(current V.x+DX),round(current V.y+10),round(current V.z+DZ) |
| Minus | round(current V.x-DX),round(current V.y+10),round(current V.z-DZ) |

Every attempted origin rereads V. Plus/minus are independent offsets from
current V,not accumulated from the previous local ray. Each maximum length is20;
no distance minimum/closest-hit arbitration between the three probes occurs in
this body. First nonnull stops that SIDE. The second side still executes even
when all first-side probes missed. A completed nontrapping invocation therefore casts between2 and6 rays.

Each cast uses hitCheckVecFilter(&Q,callback),with originQ+0c,directionQ+18,
maximum lengthQ+3c. PointQ+24 and normalQ+30 are the backend layout. Frame
query bases:side1 center+1a4,plus+164,minus+124;side2 center+e4,plus+a4,
minus+64. Original72-byte zero templates at0x802cb830 fill the vector copies;
normal/point/query-header backend writes remain their own contract.

BEFORE EVERY cast,request yoshiGetStatus. Nonzero selects chkfilterYoshi.
Zero rereads unsigned P+31;kind2 AND P+0 mask0x100 set selects
chkfilterNokotaro. Every other case selects chkfilter. No filter selector is
cached across probes or sides. These are the exact
[previously recovered ordered filter rules](FIELD_PARTNER_FRONT_WALL_FILTERS.md),
including global Yoshi selection and named dynamic-owner exclusions. The
callback can reject a geometric hit;the body's nonnull result means backend
selection AFTER those filters. There is no local attribute exemption or partner
pose/height filter added after that result.

## Side angles and final membership predicate

For a nonnull side result H,read its RAW normal X and Z from Q+30/+38,
ignore normalY,point and clipped distance,and request
[system angleABf](FIELD_SYSTEM_ANGLE_TABLE.md)(0,0,normalX,normalZ).
Store that returned angle as A for side1 or B for side2. No normal normalization,
floor/slope angle,point quantization or repeated hit attribute test occurs here.
If all probes missed on a side,its pointer is null and initialized angle remains0.

Only when BOTH side pointers are nonnull,compute:

```
S = round(A+B)
T = fnmsubs(0.5,S,180)
E = fabs(T)
accept = ordered E<=10
```

The fnmsubs is the actual single negated fused multiply-subtract,algebraically
180-0.5*S. Preserve its fused/rounding/signed-zero semantics. Unordered E rejects.
Accepted returns integer1;every missing-side or failed predicate returns0.
There is no angle wrapping,shortest-arc averaging,dot-product/parallel-plane test
or requirement that the two results refer to different hit objects. Replacing
this arithmetic mean with a geometric normal-opposition test changes the local
contract. AngleABf's actual table/quadrant representation is part of admission.

## Output and implementation boundary

The body makes NO local writes to P or V,does not publish P+12c/+134/+138,
and never copies a ray intersection back to the input vector. Its stores are
stack/saved-register work;called backend/global/filter services remain separate.
Do not treat caller V as an output correction buffer merely because its address
is supplied. Source/alias/callback/global/normal/FP domains remain open.

Run `python3 tools/verify_field_partner_slit_collision.py`:nine containing hashes
(eight complete local bodies,one selected backend scope),408 anchors,19 constants,
seven spans,two US lecture imports and the exact single direct DOL caller site.
Only three local algorithms are newly recovered here;five filter/owner leaves
reuse their existing full contracts. Backend traversal remains open. Static
byte checks do not execute original PPC or establish movement/form equivalence.
