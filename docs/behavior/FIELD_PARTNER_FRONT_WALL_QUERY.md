# Partner front-wall query,ray order and movement correction

US G8ME01 disc0 revision0, ID1, recovered2026-10-12. Evidence B: complete
partySearchFrontWall0x800c2c00,size0x568,and seven supporting local bodies.
The ray backend is fingerprinted with selected filter/result-layout anchors;
its complete traversal/numeric contract is **not closed here**. Original
execution and measured collision timing remain open.

Inputs are(P,V,speed,direction). V is a writable position vector;speed and
direction arrive in floating registers in that order. The
[stay handler](FIELD_PARTNER_STAY_MOTION.md) passes the outputs of its first
stateful direction/speed query. This function returns a retained hit pointer
or null and writes V's X/Z;it does not write V.Y.

## Setup and original ray-result layout

Call [sincosf(direction)](FIELD_PARTNER_DIRECTION_AND_SPEED.md) to get u=sin
and v=negative cos. Radius starts from P+0xf4,but P+0 mask0x1000 overrides
it to6. Compute maximum ray length **L=F(0.5,radius,speed)**. F is a
single-result fused multiply-add. No absolute speed or radius clamp occurs.

Query origin is taken from **P+0x58/+0x5c/+0x60**,not V's incoming values.
Direction is(u,0,v);the middle zero is copied from the original constant
vector. The initial upper origin Y is R(P.Y+R(0.75*partyGetHeight(P))).
The height service returns P+0xf0 normally;unsigned kind4 with P+0
mask0x100 set returns R(P+0xf0+18.5). No null check occurs in this leaf.

Both local stack queries initialize origin+0x0c,direction+0x18 and
maximum length+0x3c. Other query/backend fields are not all explicitly
initialized by this caller. Full backend header/cull/context behavior remains
a dependency;do not invent a cull selector from this caller alone.

The actual output layout is **hit point+0x24,normal+0x30,distance+0x3c**.
This corrects the formerly reversed labels in
[overworld wall documentation](OVERWORLD_WALL_MOVEMENT.md). Evidence:
backend0x80016454/58 writes origin plus scaled direction into+0x24;
0x80016474..80 copies triangle normal into+0x30;0x80016530 normalizes
the retained normal,and0x80016544..68 publishes point then normal.
The partner caller uses+0x30 for projection,not the hit point.

## Ordered filter selection and upper query

Call hitCheckVecFilter(&upperQuery,filter). Choose the filter by requesting
yoshiGetStatus on each pass;it examines the globally selected party,not
necessarily input P:

| Selection order | Callback |
| --- | --- |
| Global yoshi status!=0 |chkfilterYoshi0x800c49b0 |
| Otherwise,unsigned P kind2 and P+0 mask0x100 set |chkfilterNokotaro0x800c48cc |
| Otherwise |chkfilter0x800c4a9c |

See [filter/status bodies](FIELD_PARTNER_FRONT_WALL_FILTERS.md) for the exact
ordered attribute/name tests. Nonzero filter return admits;zero rejects.

If the upper query returns H0 nonnull,quantize its rewritten distance d:

```
bias = +0.5 if ordered(d >= 0),otherwise -0.5
k = fctiwz(F(1000,d,bias))
Q = R(singleResultSignedInteger(k) / 1000)
```

The signed conversion uses original0x4330000080000000 bias. Preserve FMA,
conversion and rounding order;this is not a generic decimal round or clamp.
NaN/out-of-range conversion and FPSCR are separate domain requirements.
Retain normal N from query+0x30 and store **P+0xec=Q**.

Let a=R(speed*u),b=R(speed*v),p=F(a,N.x,R(b*N.z)). Construct tangent
components tx=single fused negative multiply-subtract(p,N.x,a),
tz=single fused negative multiply-subtract(p,N.z,b),i.e. a-p*N.x and
b-p*N.z with original fused boundaries. Write
**V.x=F(0.5,tx,V.x)**,then **V.z=F(0.5,tz,V.z)**. Return H0 immediately.
There is no lower query or slope-angle test after an upper hit. N's Y component
does not participate in this projection;do not renormalize N horizontally.

## Lower query and steepness admission

Only upper null builds the lower query,again from current P position with
the same(u,0,v) and original maximum L. Lower origin Y is R(P.Y+11),or
R(P.Y+1) when unsigned kind3 and P+0 mask0x100 is set. Requery global
yoshi status and reselect the filter;the earlier callback choice is not cached.

Lower null goes directly to unobstructed movement below. Lower nonnull H1
quantizes distance with the same policy and retains its normal. Compute
q=F(N.x,N.x,R(N.z*N.z)),then its square root through the same local
three-double-refinement/classification pattern as the
[direction-query norm](FIELD_PARTNER_SPEED_NUMERICS.md),using separately
pinned0.5/3.0/0.0 double constants. Call angleABf(0,0,root,-N.y).

Only an **ordered angle>=45** accepts this lower hit for correction/return.
Accepted stores the quantized lower distance at P+0xec,performs the same
half tangent X/Z update and returns H1. The backend normal and angle helper
define actual surface interpretation;this predicate alone is not a measured
maximum traversable slope.

Unordered or smaller angle rejects H1 locally:it does not return that pointer
or preserve the lower-hit distance in P+0xec. This is different from upper
non-null policy;do not merge the two passes into one uniform hit acceptance.

## Unobstructed result and called composition

Upper/lower null,or lower angle rejection,stores **P+0xec=-1**,sets integer
return null and writes **V.x=F(speed,u,V.x)**,then **V.z=F(speed,v,V.z)**.
Thus V changes even when the return pointer is null. There is no local Y write,
HP/FP change,mode publication or history reset.

The stay caller stores a returned hit at P+0x12c,and at+0x134 when P+0x104
is nonzero;it then publishes V.X/Z and performs a second stateful direction/
speed query before surrounding correction. Do not cache the two queries or
discard movement merely because this wall call returned null.

## Verification and remaining work

tools/verify_field_partner_front_wall.py checks eight containing bodies:
seven complete local front-wall/filter/height/status/owner bodies and the
backend **selected result-layout/filter regions only**. It pins62 instructions,
16 constants,four data spans and two actual lecture imports. The whole backend
hash is an identity check,not a full traversal specification.

Next:complete hitCheckVecFilter and mesh/GQR/FPSCR geometry;surrounding/floor/
fly/homing services,source/owner domains and original collision trajectories.
These dependencies prevent full partner-collision or game-equivalence closure.
