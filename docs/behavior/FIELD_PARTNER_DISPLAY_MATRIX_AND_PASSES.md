# Partner draw callback: enable census,ordered matrix requests and two passes

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyDisp0x800b7690,size0x408. [Pre-display](FIELD_PARTNER_PRE_DISPLAY.md)
registers this callback with P as SECOND integer argument(r4). Incoming first
argument is unused. This closes the local matrix/draw request graph;SDK matrix
arithmetic,animation renderer/GX/material/resource and queue lifetime remain open.

## Enable census and mutable pivot

Read pointer table entries0/1 at SDA+1bb8/+4(0x8041ead8/+dc). Count nonnull
objects whose word+0 bit0 is set. Zero count returns before touching supplied P.
There is NO independent supplied-P enable-bit or null guard in this body.
A nonzero count enters drawing even if this P's own bit is clear;normal queue/
object-lifetime domains must justify any stronger admission invariant.

If current P+0 mask0x200 set,store P+88=-6.8(wordc0d9999a) and
P+8c=+6.8(word40d9999a). P+90 is preserved. These pivot writes precede all
matrix requests and can replace roll/slit-produced values. They do not move root.
Choose scale factor1.2 if P+0 mask0x00800000 set,otherwise2.0.

## Exact left-operand concatenation sequence

Create main matrix B at frame+68 with PSMTXScale0x8029e2b0 using separately
rounded factor*P+70,+74,+78. Every concatenation below requests
PSMTXConcat0x8029dd64(A,B,B):second input and output ALIAS the same B.
It is not permitted to reorder transforms or substitute a commutative product.
The wrapper preserves that call/alias order rather than claiming SDK bit parity.

| Order | A construction and admission |
| --- | --- |
| 1,optional reflection | If P+4 mask8 clear,wrap P+10c via revise360;create scale(1,1,-1) only when ordered90<=wrapped<=270,otherwise scale(1,1,1);concat in either case. Mask8 set skips this whole request pair |
| 2 | Translation(-P+88,-P+8c,-P+90) via PSMTXTrans8029e230 |
| 3 | PSMTXRotRad8029dff0,axis ASCII z(122),angle=single0x3c8efa35*P+6c |
| 4 | Same rotation service,axis x(120),angle=single0x3c8efa35*P+64 |
| 5 | axis y(121),angle=single0x3c8efa35*revise360(round(P+10c-P+f8)) |
| 6,optional extra Y | If P+4 mask0xc0 nonzero,wrap P+10c AGAIN. Ordered90..270 plus mask0x40 set selects180,mask clear selects0;outside/unordered reverses those0/180 choices. Convert with same factor,request Y rotation,concat |
| 7 | Translation(+P+88,+P+8c,+P+90) |
| 8 | Final root+offset+paper lift translation below |

Every row's local transform is constructed then concatenated before the next.
P+68 is not read as a rotation by this body. Rotations use the original binary32
factor approximately0.01745329238474369,not host double pi/180. revise360's
ordered iterative wrap remains [its own contract](FIELD_PARTNER_FALL_FACING.md).
Pivot/angle/source fields are reread at their actual rows;no immutable pose
snapshot is manufactured across matrix/backend calls.

## Final translation and resource-dependent Y lift

Default extra lift L=0. Only P+1c NONNULL triggers signed-kind-specific lift:

| Kind | Lift |
| --- | --- |
| 4 | 1 |
| 3,7,1 | 2 |
| 11 | 3 |
| 8 | 3.5 |
| 10 | 4 |
| 9 | -1 |
| Other | 0 |

Construct translation X=round(P+58+P+7c),Y=round(L+round(P+5c+P+80)),
Z=round(P+60+P+84). Preserve nested Y add order. This is where
[Cloud's +80 offset](FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md) and roll/slit
translation fields affect requested drawing;rootXYZ itself is unchanged here.
Concat that translation onto B.

Then request animPoseDrawMtx(current P+c,&B,2,f1=0,f2=1)0x8003b4d8,
followed by a FRESH pose-ID load and animPoseDrawMtx(P+c,&B,1,f1=0,f2=1).
These are two ordered integer-mode calls,not a single interchangeable draw or
verified opacity/pass interpretation. Full material/renderer internals remain open.

AFTER BOTH calls,reload P+0. Mask0x80 set requests
animPoseSetLocalTimeRate(current P+c,0.75)0x8004142c;clear requests1.0.
Do not apply that rate locally before animation update/drawing or invent elapsed-
render-time scaling. Subsequent rate/frame ownership remains a backend/cadence
contract. Static checks do not establish original frame order or visible parity.
