# Flying partner stay: ceiling requests and ordered height smoothing

US G8ME01 disc0 revision0, ID1, recovered2026-10-12. Evidence B:complete
unk_80150fac at0x80150fac,size0x334. This is the actual
[stay](FIELD_PARTNER_STAY_MOTION.md) tail selected ONLY when unsigned mobility
P+32==1,after front/around X/Z publication. Other mobility values select the
[ground postpass](FIELD_PARTNER_GROUND_POSTPASS.md). No elapsed-time smoothing
rate or original flight trajectory is measured by this static contract.

P position is +58/+5c/+60. Capture owner M0=P+160 at entry. If P+0 host
mask0x04000000 is set,return before map,ceiling or floor work. The body has
no defined boolean result;callers use its side effects.

## Ceiling before floor

Compare the current global map string GP+12c against exact resource key las_10
at0x802f47c8. On equality,copy P position to local V and height P+f0 to local
h;call partySearchHead0x800c27f8 with(P,&V,&h,f1=P+10c). f1 is the partner
heading argument,not vertical speed. On any other map,skip this ceiling stage.
The [ceiling query](FIELD_PARTNER_CEILING_QUERY.md) now specifies its complete
local probe/output graph;full scene-ray backend remains a dependency.

If that query returns nonnull H,store H at P+140,then compute
d=R(R(V.y-P+f0)-P.y). If ordered -5>d OR ordered d>=0,write
P.y=F(d,original binary32 .1,P.y). Otherwise retain Y. Thus the ordinary finite
dead interval is [-5,0);exact -5 does not adjust,exact0 takes a zero correction.
Unordered comparisons do not admit an update,assuming FP exceptions allow
continuation. The local h output and local X/Z are never published. A null
head result does not clear P+140. The new Y,if written,feeds the later floor
query in this same call.

Always next request partySearchGround0x800c3bcc with(P,f1=10.0,f2=0.0),retaining
its returned hit G. These are exact float arguments;their internal ray meaning
is left to the full query contract. If G is nonnull,call hitGetAttr(G). Attribute
0x00000800 requests partyChgMoveMode(P,13) and RETURNS immediately,skipping
all owner gating and height smoothing. Otherwise attribute0x00000100 sets
P+0 mask0x80;absence clears it. Null G preserves that flag.
The selected query entry0x800c3c54–6c can return null immediately when
P+0&0x02000000 is set,before refreshing P+e4/P+e8. Cached output fields may
therefore participate in the admitted null-G route below;do not initialize them
in the caller merely because G is null.

## Owner admission and branch selection

For the following admission tests,load a fresh M1=P+160 rather than reusing
M0. Suppress the ordinary owner-height route when signed halfword M1+50!=0
and M1+180 compares equal to0,or unsigned motion M1+2e is31,32,16 or17.
Otherwise admit it. NaN speed does not compare equal to0. Height selection
still uses captured M0+90,even if the owner pointer changed during callbacks.
There is no local owner-null guard.

Let E be current float P+e4 and Y current P+5c. Ordered comparison chooses
B=M0.y if M0.y>E;otherwise B=E. Do not replace that with an unspecified
host max operation for NaN inputs. Choose offset O=1.0 if P+4&0x8000 is set,
otherwise5.0. Corrections below use the same ordered dead-interval gate as
the ceiling stage:apply only for -5>d OR d>=0;perform a single-result fused
Y=F(d,k,Y),with original binary32 k.

| Owner route | Floor/query state | Base and factor before offset | Outcome |
| --- | --- | --- | --- |
| Admitted | G nonnull | Start B as above,k=.1;if ordered Y<E,set B=E,k=.2 | d=R(R(B+O)-Y);gated fused Y update |
| Admitted | G null and ordered P+e8>=0 | Same B/k rules | Same update |
| Admitted | G null and P+e8<0 or unordered | None | Return without further Y update |
| Suppressed | G null | None | Return without further Y update |
| Suppressed | G nonnull | Start B as above,k=.05;if ordered Y<E,set B=R(18.5+E),k=.2 | d=R(R(B+O)-Y);gated fused Y update |

The suppressed+below-floor branch adds18.5 BEFORE the ordinary1/5 offset;
it does not merely substitute an owner height or use the admitted branch's
target. The ordered comparisons and two rounded additions/subtractions are
part of the contract. The no-floor admitted route's P+e8 test does not by
itself establish a successful floor hit;do not coalesce it with G nonnull.

## Observable ownership and verification

This body writes only selected P flags,Y and nonnull ceiling contact plus
whatever its called query/mode services write. It does not store G into P+138,
snap directly to E,change motion with partyChgMot,request out-of-screen reset,
or publish local ceiling X/Z. Stay returns after this tail,so its ground-only
out-of-screen reset does not follow it. Query side effects may still alter
other fields;do not infer their absence from the caller's direct stores.

tools/verify_field_partner_postpass.py pins this complete body,actual stay
binding,ordered compare/branch/fused stores and exact constants. Finite
acceptance cases include ceiling differences -6,-5,-1,0;hazard-before-owner
priority;owner31 vs normal;null floor with e8=-1/0/NaN;and suppressed Y<E
using E+18.5 before offset. These are deduced fixtures,not original traces.

Remaining:complete head/floor queries and scene geometry,field/owner/height/
flag producers and supported numeric domain,all flight motion handlers and
original trajectory/ceiling/hazard observations. This postpass does not define
the complete flying-partner engine.
