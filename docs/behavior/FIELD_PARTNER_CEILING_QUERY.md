# Partner ceiling query: first successful probe and output ownership

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
partySearchHead0x800c27f8,size0x408. It is called by both recovered
[ground](FIELD_PARTNER_GROUND_POSTPASS.md) and
[fly](FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md) stay postpasses at0x80151328
and0x80151024. This documents the complete local four-probe graph,not the
full scene-ray backend or an observed ceiling trajectory.

Inputs are(P,V,&height,heading f1). V points to three engine-unit floats;
height is both an input maximum ray length and an output retained distance.
P is used ONLY for width P+f4 at entry. There is no null guard,height-domain
clamp or width6 override for P+0&0x1000 in this body. It does not query global
Mario,Yoshi status or partner-specific wall filters.

## Four probes,with short-circuit return

Load width W before the sincosf call. With R a single-result rounding and
u=sin(heading*pi/180),v=-cos(heading*pi/180),compute in this order:

```
r = R(original .3 * W)
b = R((-r) * v)
a = R(r * u)
```

The original .3 is binary32 hex3e99999a at0x80421524,not an arbitrary native
radius. After sincosf returns,capture V's X,Y,Z into local floats. Do not
instead use the root position from P;the pointer V owns this input. Load the
initial height after constructing the first probe. Sequential sampled positions
are:

| Probe | Sampled X | Sampled Y | Sampled Z | Call |
| --- | --- | --- | --- | --- |
| 0 | R(X+a) | Y | R(Z+b) |0x800c292c |
| 1 | R(X-a) | Y | R(Z-b) |0x800c29e8 |
| 2 | R(X+b) | Y | R(Z+a) |0x800c2aa4 |
| 3 | R(X-b) | Y | R(Z-a) |0x800c2b60 |

For each sample S,call [hitCheckFilter](FIELD_SCALAR_RAY_WRAPPER.md) with
null callback,origin(S.x,R(10.0+S.y),S.z),direction(0,1,0),and maximum-length
pointer initialized to the current retained height. The origin's Y addition
is performed separately for every probe. Point and normal outputs use separate
local float slots. Neither output vector feeds this routine's position result.

If the query returns null,continue to the next probe. If it returns nonnull H,
compare retainedHeight with the returned length using fcmpo plus greater-or-
equal CR OR. For ordered retainedHeight>=returnedLength,replace retainedHeight
with returnedLength. Otherwise keep it,including unordered comparison under
nontrapping continuation. Then exit the probe sequence immediately. Later
probes are NEVER requested after any nonnull result,even when the returned
length failed this comparison. There is no search for the minimum across
successful probes and no center probe. Unlike the front-wall query,there is
no local thousandth-unit distance quantization here.

The offset arithmetic uses separate multiply/add/subtract single-result
instructions. Do not fuse coordinate construction or substitute rotated ideal
axes if numerical equivalence matters. sincosf's full local degree conversion/
sin/cos request order is already recovered;its libm/Gekko environment remains
a numeric dependency.

## Exact success and no-hit outputs

On the first nonnull H,write V.X=that sampled X,V.Y=the captured ORIGINAL Y,
V.Z=that sampled Z,in that order;then write height=retainedHeight. Return H
unchanged. These are sampled coordinates,not the hit point or ray-origin Y.
No local root-Y correction occurs in this routine.

If all four queries return null,still write V.X/V.Z to the FOURTH sample,
V.Y to captured ORIGINAL Y;then write height=0 and return null. Thus failure
is not a no-write operation. With distinct V/height buffers and stable finite
inputs,the Y output preserves its input value while X/Z shift even on failure.
There are no output-pointer null checks. Stores occur XYZ then height;aliasing
height into V changes the final contents according to that store order.

Null-query success-output slots from hitCheckFilter are not read on null paths.
The original per-probe zero-vector templates are pinned for identity,but no
uninitialized hit-point output is used to fill V. A caller must bind the two
different output concepts explicitly.

## Actual stay consumers and fidelity consequence

Both postpasses seed V with P+58/+5c/+60 and height with P+f0;f1 is P+10c
heading. Ground records nonnull H at P+140,then ignores both V and height.
It does not move root X/Z to the sampled coordinates. Its head call precedes
the ground suppression flag test.

Fly makes the head request only in map las_10 and only after its entry
suppression flag admission. Nonnull H is recorded at P+140. It then computes
d=R(R(V.y-P+f0)-P.y),uses its ordered dead-interval test and applies the .1
Y correction described in the fly contract. It ignores returned height and
sampled X/Z. Since this query writes the captured input Y to V.y,this expression
does NOT derive a ceiling hit-point height. With stable P and ordinary finite
inputs,it is the separately rounded subtract-height/subtract-root expression
using the original root Y. Do not silently replace V.y with hit-point Y or
returned height to make a more conventional ceiling response. Query callbacks,
numeric state and later floor smoothing remain separate dependencies;no actual
in-game trajectory is claimed from this static deduction.

## Reproducible checks and remaining scope

tools/verify_field_partner_head.py pins the complete head/wrapper/sincosf bodies,
selected backend output/filter regions,independent branch/call/store anchors,
exact constants and original vector-template identities. It checks both actual
postpass call sites and original lecture imports reaching ordinary party work.
Fingerprints are binary identity,not proof of scene reachability or execution.

Deduced injected-ray cases:probe0 hit skips1..3;probe1 hit skips2..3;probe3
hit uses its sample;all-null uses fourth sample and height0;nonnull with equal,
larger or NaN returned length still short-circuits;outputs preserve original
sample Y rather than point-Y;ground consumes only H and fly ignores height.
These cases should constrain native tests without being reported as measured
original behavior.

Remaining:complete scene-ray/triangle/numerical backend,resource and hit lifetime,
width/heading/height/output-buffer domains,libm and GQR/FPSCR state,ceiling/floor
interaction and original las_10/ground trajectories. Full field/game
documentation and complete-game requirements remain incomplete.
