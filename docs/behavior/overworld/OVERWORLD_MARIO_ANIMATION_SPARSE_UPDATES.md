# Original ordinary Mario animation sparse updates and interpolation

US G8ME01 revision0, animPoseMain8003ed2c, animInit80042854 and the ten
front/rear animations in [pose selection](OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md).
This closes their finite keyframe evaluation domain, following
[clock/loop selection](OVERWORLD_MARIO_ANIMATION_CLOCK_AND_LOOPS.md).
Source reconstruction plus host-derived checks, not original execution or
SDK/Gekko numerical equivalence. Native renderer binding remains separate.

## Bounded shipped domain and raw parsing

All ten animations have one loop descriptor, strictly increasing key times,
first time0 and last time equal descriptor end. Their position/node streams
are sparse, cumulative updates; normal/visibility update counts are0. Texture
updates have zero S/T shifts. Static texture matrices are(0,0,1,1,0) for
transS,transT,scaleS,scaleT,rotation. No interpolation of an unknown collection
or fabricated per-pose duration is required for this domain.

The raw original model header supplies absolute **file offsets**:

| Count | Raw pointer field | Array/stride |
| --- | --- | --- |
| f0 | 154 | Position XYZ floats,12 bytes |
| f8 | 15c | Normal XYZ floats,12 bytes |
| 12c | 190 | Texture matrix,24 bytes |
| 13c | 1a0 | Visibility bytes |
| 140 | 1a4 | Node scalar floats,4 bytes (24 scalars per node transform) |
| 148 | 1ac | Animation directory,64 bytes (name60,pointer4) |

Directory+3c is the raw animation's file offset. Within that animation A,
all table-pointer words below are **relative to A**, not file start. Runtime
consumers read relocated pointers; this raw parsing contract is for authentic
asset import/evaluation. +0 is animation byte size, +4 loop-descriptor count,
+8 keyframe count; +20 is auxiliary8-byte record count.

| Animation total count | Relative table pointer | Record |
| --- | --- | --- |
| c | 2c | Position updates,4 bytes |
| 10 | 30 | Normal updates,4 bytes |
| 14 | 34 | Texture updates,12 bytes |
| 18 | 38 | Visibility updates,2 bytes |
| 1c | 3c | Node scalar updates,4 bytes |
| 4 | 24 | Loop descriptor12 bytes:word flag,float start/end |
| 8 | 28 | Keyframes44 bytes |
| 20 | 40 | Auxiliary time/word records8 bytes |

Each44-byte key contains float time at0, then(startIndex,count) pairs for
position+4/8,normal+c/10,texture+14/18,visibility+1c/20,node+24/28. Table indices
count **records**, not bytes. Stream starts/counts must fit their own total;
these source ranges are checked by the verifier.

Every stream's first byte is an **unsigned index delta**. Reset the target
index to0 at the start of each key's stream, then add successive deltas. Vertex
updates address vector indices; node updates address scalar indices. A node
update index does not select a whole24-float transform. Placeholder index skips
with zero value remain meaningful cumulative addressing.

## Full passed keys and source cache ownership

Let k be last key with keyTime<=selectedTime, j be next key after source loop/
nonloop adjustment, and q be the fraction. For these ten assets, all ordinary
selected times have k>=0. Nonloop final j=k; loop end wraps to first key0 before
scanning, so there is no native end−.01 fudge.

Main computes duration=nextKeyTime−lastKeyTime and q=(selectedTime−lastKeyTime)
/duration at8003f064–f0. Zero duration sets q0. The loop-wrap next0 branch uses
loopEnd−lastKeyTime for q at8003f090–b4; the bounded shipped domain selects
before end or wraps selectedTime to0, avoiding a last=end/next0 partial span.
Node interpolation later obtains the actual next-minus-last key times at
80040440–54. Do not collapse those distinct exceptional expressions outside
this domain.

Source retains full-key buffers and last key3c. Fresh last=-1 or key rewind
resets position/normal/visibility/node/texture buffers from original model bases
(8003f120–1bc), then applies keys0..k in order. Forward movement from an older
key applies only new full keys (8003f910 onward); same k retains full cache.
Full-key updates change those buffers. Fractional output is constructed from
that cache; cache does not accumulate fractional updates. Main stores current
k and q to3c/40 at80040538/3c. Same-key fraction rewind resets the bookkeeping
at80040070–80, not an invented smoothing of geometry.

A portable/stateless evaluator may rebuild original bases and apply full keys
0..k on every evaluation, then the one fractional keyj. For this finite
ordinary domain that reconstructs the same buffer operation order used when
source resets; incremental cache is an optimization/lifetime choice, not a
new interpolation curve. Host precision/trig and cross-callback original f32
rounding remain explicit numerical boundaries. External buffer mutations,
linked paper/effect poses, camera-facing poseflag8 tail and special shared
buffer services are outside this domain.

## Full update decoding

- Position and normal4-byte record:unsigned index delta, then signed8 XYZ
  deltas. Add each delta/16 to the cumulative vector buffer. Source GQR4 is
  initialized type6/signed8 with zero quantization scale at8003eda4–ac;
  psq_l loads followed by factor.0625/fmadds are8003f224–250, etc.
- Node4-byte record:unsigned scalar index delta,signed8 value delta,signed8
  tangentIN angle,signed8 tangentOUT angle. A **full passed key** adds value/16
  unconditionally; it ignores tangents (8003f61c–774).
- Visibility2-byte record:unsigned index delta and byte delta. Add to current
  visibility byte and retain low8 bits (8003f528–53c), rather than assign a
  boolean from the sign. This generic full-key branch is pinned, though the
  ten target streams are empty.
- Texture12-byte record:unsigned matrix index delta, byte index delta at1,
  padding2/3, float transS delta+4,float transT delta+8. Add index byte modulo256
  and add S/T to the corresponding24-byte matrix (8003f7b8–900). These ten
  poses only change index bytes; they are stepped at passed key times.

The texture index byte addresses original texture-binding data, not a flat TPL
page ID. Keep each original draw's binding and current matrix index. For
example rear walk M_W_1R steps matrix11 at times8/15/20 by−1/−4/+5, and
matrix12 at25/27/37 by+2/+1/−3. Rear dash uses the same changes at
4/7.5/10 and12.5/13.5/18.5. Idle changes eye/other layers at later source keys.
Static triangle TPL IDs alone cannot express those swaps.

## Fractional next-key vertex and node output

When k<j, Main first copies full cached position/normal/node state to output.
It applies **only** keyj's position/normal updates linearly with factorq/16
(800400d8–4042c). No position interpolation curve inferred from node tangents.
Visibility/texture remain full-key state; no fractional next texture/visibility
update is added. When k==j, it copies full buffers directly and skips partial
updates (80040084–d4).

For each keyj node record, advance scalar index, read signed OUT:

    if OUT <= -89 or OUT >= 89:
        skip this partial scalar store entirely
    else:
        q2 = q*q
        q3 = q*q2
        delta = (q3-2*q2+q)*duration*tanTable[IN]
              + (3*q2-2*q3)*(valueDelta/16)
              + (q3-q2)*duration*tanTable[OUT]
        outputNode[index] = fullCachedNode[index] + delta

Branch800404a0–b4 skips directly to next-record80040530 for OUT outside the
strict range. In particular **OUT90 holds the full cached value until that key
actually passes**. It does not immediately add the next key's complete delta.
Each idle front/rear pose has24 such records. This differs from the provided
Noclip reference's `tangentOut===Infinity` immediate-add path; that reference
must not override the shipped US instruction order.

With both tangent angles0, node interpolation is smoothstep3q²−2q³, not linear.
Only valueDelta is divided by16; tangents are multiplied by source segment
**duration in animation frames**, without an extra inferred/16 factor.

Source arithmetic order800404bc–4052c is q2, q3; IN basis=q3−2q2+q;
value basis=3q2−2q3; OUT basis=q3−q2; compute duration*IN tangent, multiply by
IN basis, then fmadds(value basis,valueDelta/16,firstTerm), then
fmadds(OUT basis,duration*OUT tangent,sum), finally add full cache scalar.
The source uses mixed separate/fused f32 operations. Host double or standard
Hermite helpers are labeled arithmetic adaptations, not bit equivalence.

## Tangent lookup producer and safe ordinary range

animInit initializes the178-float BSS table tanfTbl803d9580, size2c8:

    for i in 0..177:
        angle = i-89   # signed degrees, -89..88
        tanfTbl[i] = float(original_tan(single(angle*RAD)))

RAD is **word3c8efa33=.017453288659453392** at8041fbb4; it differs from the
actor draw wrapper's radians coefficient3c8efa35. Producer800429d0–42a20
uses integer-to-float conversion, multiplication, double tan and frsp.
Consumers access table base+(angle+89)*4 at800404dc/e4 and40500/04.

The ten assets' admitted partial IN angles lie within−87..87, so every lookup
is inside the initialized array. OUT90 records skip before the lookup; they do
not require an infinite tangent entry. Arbitrary IN89/out-of-table custom
streams are not repaired by a synthesized table entry. Generic special assets
need their own domain audit.

## Derived evaluator and native import boundary

`tools/verify_overworld_animation_updates.py` exposes `animation(raw,name)` and
`evaluate(parsed,frameTime)` as a **host-derived reference evaluator**. It reads
original bases/streams and returns local position/normal/node buffers,
visibility, texture matrices/index bytes and selected key/fraction. It does not
change assets or the native converter. All original files are read-only.

Feed evaluated local buffers through the real node hierarchy/shape/draw
bindings and original actor root/scale/turn matrices. Interpolating already
flattened frame0 triangle coordinates cannot recover Hermite node transforms,
hierarchy/pivots or texture swaps by itself. An exporter can retain source
arrays/node bindings and update streams, or evaluate complete poses at the
needed frame times with an explicitly described sampling adaptation.

Current character_bake.py still applies only full passed keys; interpolation
remains unimplemented in that converter. The source contract is now closed
for these ten ordinary poses, while native integration and source-runtime
numeric/capture comparison remain unfinished.

## Validation

Run `python3 tools/verify_overworld_animation_updates.py`. It pins8 body/data
spans and63 instruction anchors against the ISO, validates both original model
hashes, all10 sparse stream domains and stepped texture restrictions, and
evaluates70 finite pose samples plus9 synthetic linear/Hermite/instant-byte
cases. It also checks admitted idle OUT90 scalar holds directly from shipped
records. These are reconstruction checks, not original runtime emulation.
