# Animation output interpolation:linear triples and cubic node channels

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete animPoseMain;
local80040084..80040540. Notation from
[entry/time](ANIMATION_MAIN_ENTRY_AND_TIME.md),
[sampling](ANIMATION_TIMELINE_SAMPLING.md) and
[cache replay](ANIMATION_COMPRESSED_DELTA_CACHE.md). Round32 R() and single fused
F(a,b,c) denotes original fmadds;N(a,b,c) denotes original fnmsubs
(negated fused multiply-subtract). Retain instruction-specific signed-zero
semantics and original arithmetic/store rounding. No original execution.

## Current versus successor admission

Use retained signed current c,next n and fraction t. If c==n:copy position cache
R+48 to captured Opos (12*positionCount),aux cache R+50 to captured Oaux
(12*auxCount),node cache R+60 to CURRENT R+64 (4*nodeCount);each iff retained
count!=0,in that order. Byte/texture caches are not copied to new fractional
buffers. Then go to final publication.

If c>n,signed,skip these copies AND interpolation and go to final publication.
This includes a last-to-first loop successor0 while current ispositive. Existing
output contents survive locally;do not invent interpolation over the end seam.
If c<n,perform next-key fractional work below.

## Position and auxiliary channels

Next key B=word[T+28]+44*n. Compute scaled fraction g=R((1/16)*t).
For position:resolve its next-key stream offset/count B+4/+8 and T+2c;
copy current cache into captured Opos if retained positionCount!=0. Then walk
next-key stream using unsigned start<end,4-byte entries,four-entry unrolling
and remainder. Destination cursor begins Opos and advances12*unsigned byte0
cumulatively;read signed bytes1..3 through scalar GQR4. For XYZ separately:
out=F(signedByte,g,currentOutput);store immediately.

Auxiliary does the same after position using B+c/+10,T+30,R+50 and captured
Oaux. It starts a fresh destination cursor and copies the auxiliary cache first.
These are additive NEXT-key deltas,not linear interpolation between two complete
stored snapshots. Copies/load/store aliases must preserve original call order;
no buffer-distinctness or source index validation is local.

## Node cubic channel

Let hc=float[currentKey+0],hn=float[nextKey+0],h=R(hn−hc).
Resolve next-key node stream B+24/+28,T+3c,4-byte entries. Capture output
pointer O=word[R+64];copy current node cache R+60 to O iff count!=0.
Set t2=R(t*t),then for each entry:

1. Read byte3 and sign-extend b;advance destination cursor by4*UNSIGNED byte0.
2. If b>=89 OR b<=−89,skip ALL cubic update for this entry. Keep copied output.
3. Otherwise load signed byte1 d through GQR4;sign-extend byte2 as tangent a.
   There is NO analogous range guard for a.
4. Read Ta,Tb from runtime table at0x803d96e4+4*a/b.
   [Initialization](ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md) fills signed
   angle range−89..88;out-of-range a can read adjacent data,not a clamped tangent.

Literal operation order for admitted entry:

```
t3 = R(t*t2)
h10 = R(t + N(2,t2,t3))
h01 = F(-2,t3,R(3*t2))
m0 = R(h*Ta)
x = R(h10*m0)
delta = R((1/16)*d)
x = F(h01,delta,x)
h11 = R(t3-t2)
m1 = R(h*Tb)
x = F(h11,m1,x)
out = R(currentOutput+x)
```

At800404d8 `fnmsubs(2,t2,t3)` yields t3−2*t2 in one single fused subtraction;
the notation above preserves that instruction via N(2,...). Original contraction,
FPSCR,signed-zero/exception and denormal behavior remains a numeric dependency.
This is an additive cubic delta with authored angular tangents. No local modulo
angle,normalization,epsilon,coefficient clamp or linear fallback exists.
Texture-record/byte-state streams do not interpolate here;their cumulative
values remain current-key values.

## Final publication and optional facing

For ALL nonnull-T paths reaching80040538 (including c>n and failed cubic
admission),store retained c to R+3c,then retained t to R+40. These writes happen
AFTER copy/interpolation. The earlier same-key fraction-decrease clear does not
cause a replay or a new clock reset. Finally evaluate
[optional camera-facing tail](ANIMATION_MAIN_CAMERA_FACING.md).

Review cases:current==next copies base without deltas;loop next0 with c>0 skips
fractional output;same current key still recomputes fractional output;negative
next delta has signed1/16 value;relative byte indices accumulate;node b±89
skips but a±89 is unguarded;texture/byte channels remain discrete. These are
source-derived review requirements,not executed original comparisons.
`verify_animation_main.py` pins complete local instructions/constants and the
initializer,including tangent table's writer. Renderer/group matrix/material
consumers,loaded-resource/buffer lifetime and original animated visuals remain open.
