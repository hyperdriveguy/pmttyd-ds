# Shadow frustum arithmetic and auxiliary profile owners

US G8ME01 disc0 revision0, read-only recovery2026-10-14 for EVT-01.
Companion to [class5 caller suffix](SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md)
and [projection prefix](SHADOW_PROJECTION_PREFIX_AND_ZERO_COUNTER_ADMISSION.md).

## Exact C_MTXFrustum8029e74c/9c numeric order

Arguments are output matrix r3; f1=top,f2=bottom,f3=left,f4=right,
f5=near,f6=far. S below denotes a binary32-rounded operation; multiplication
and addition are separate source instructions, never a fused expression.
The source SDK constants80429378..384 are exactly1,2,+0,-1.

The source performs these arithmetic instructions in order:

```
width = S(right - left)                 // e74c
height = S(top - bottom)                // e754
depth = S(far - near)                   // e75c
iw = S(1 / width)                      // e760
twiceNear = S(2 * near)                 // e764
ih = S(1 / height)                     // e768
xscale = S(twiceNear * iw)              // e76c
xsum = S(right + left)                  // e770
ysum = S(top + bottom)                  // e774
farNear = S(far * near)                 // e77c
id = S(1 / depth)                      // e780
xoffset = S(iw * xsum)                  // e78c
negativeNear = -near                    // e790, fneg
negativeFarNear = -farNear              // e794, fneg
yscale = S(twiceNear * ih)              // e79c
yoffset = S(ih * ysum)                  // e7a0
zscale = S(negativeNear * id)           // e7a8
zoffset = S(id * negativeFarNear)       // e7b0
```

Its row-major matrix is:

```
[ xscale, +0, xoffset, +0 ]
[ +0, yscale, yoffset, +0 ]
[ +0, +0, zscale, zoffset ]
[ +0, +0, -1, +0 ]
```

Store order offsetshex:00,04,08,0c,10,14,18,1c,20,24,28,2c,30,34,38,3c;
00 is published before farNear/id arithmetic,08 before yscale/yoffset,
then remaining stores as above. All arithmetic uses fsubs/fdivs/fmuls/fadds;
fneg changes the sign without a new arithmetic rounding. No FMA instruction
appears. In particular **zscale=-near/(far-near)**, not -far/(far-near)
or -(far+near)/(far-near). Use actual separately rounded intermediates;
an algebraically simplified/OpenGL matrix is not this source producer.
Zero denominators/nonfinite domains have no source repair guard. Numeric
exception or rounding adaptation must be declared separately on native ARM.

## Primary and auxiliary boot owners share mutable rows

shadowInit8002dcec/4f8 initializes these distinct retained owners:

| Owner | Source pointer/root | flags.word0 | counter.word4 | profile.word8 | Color.word100 |
| --- | --- | --- | --- | --- | --- |
| Character primary | cswp804150e0→803c9e38 | 0 | 0 | 1 | 0000006e |
| Projection auxiliary | pswp804150e4→803c9d34 | 2 | 0 | 0 | 00000040 |
| Depth auxiliary | dswp804150e8→803c9c30 | 2 | 0 | 0 | 00000040 |

Primary writes precede request pool allocation, then source smart image
allocation. Depth flags/counter/profile/color stores occur8002ddc8..dde8
before ramp allocation. Projection flags/counter/profile/color occur
8002e1ac..e1c8 after ramps are initialized/flushed. Thus completing just
the scalar prefix is not completing all allocation/bootstrap services.

shadowMain clears **bit0 mask1 only** and counterword4 of all three owners;
primary count104 also resets. It preserves auxiliary flag2, all profiles,
colors, matrices/images and shared rows. A zero-initialized auxiliary struct
does not reproduce fresh flags2. Do not infer profile/color from that reset.

shadowGetProjShadowConfig8002bac8/1c and
shadowGetDepthShadowConfig8002bb50/1c independently read their current owner
word8 and return80304a60+72*profile. They return pointers into the **same
mutable two-row shadowConfig table** used by primary. There is no profile
copy, row reinitialization or range clamp. External users can mutate rows
through those returned pointers; a getter query is not immutable metadata.
This local proof does not establish a complete direct/indirect writer census.

Camera2 class5 primary profile1 follows camera4's **at** at offsets18/1c/20:
row.eye=(atX,S(1000+atY),atZ),row.up=(0,0,-1),row.at=camera4.at.
Class0 selects pswp profile0; class2 selects dswp profile0. Neither auxiliary
setup has class5's camera4-follow call/mutation. If an external source owner
selects profile1, auxiliary setup observes its shared mutated row instead;
it must not manufacture a fresh copy. Fresh row0 keeps authored vectors
eye(-100,256,335),up(0,1,0),at(0,0,0), symmetric extents±20,
near356,far741,projectionKind0,size-index0. Fresh row1 starts
eye(135,40,41),up(0,1,0),at0,extents±240,near80,far10000,kind1,size0.

## Class5 setup and type3 geometry check

The existing complete caller suffix has no omitted call between Collision
and Projection: optional camGetPtr(4),GXInvalidateTexAll,viewport/scissor
readback,set square viewport/scissor,LookAt,Frustum/Ortho,SetProjection,
two separate current-camera queries/copies,Projection,then fresh primary
owner→projShadowEnd. Projection itself re-queries current camera at entry;
its view must therefore be the temporarily replaced camera2 view, not the
previous camera2 view or a captured camera4 view.

Unconditional GXSetArray(9,80304b00,12) selects original testKagePosArray,
exactly six floatXYZ vertices (48hex bytes):

```
0: (0,0,-1)        1: (0.9f,0,-0.5f)  2: (-0.9f,0,-0.5f)
3: (0.9f,0,0.5f)  4: (-0.9f,0,0.5f)  5: (0,0,1)
```

0.9f is original binary32 3f666666. GXSetVtxDesc(9,2) uses indexed8
positions, format(0,9,1,4,0) uses XYZ float. Each admitted type3 prepared
nonzero-radius record concatenates currentview*record.matrix, loads matrix0,
calls GXBegin(152,0,6), then submits index bytes0,1,2,3,4,5. No per-vertex
color/UV attribute is enabled by this prefix. Counter increments only after
all six indices. Native geometry must preserve the six-vertex triangle strip
and original matrix/material state; texture44 quads or type2 cylinders are
different source paths.

## Evidence and limits

Fresh ignored listings/ELFs/metadata: artifacts/reference/shadow-frustum/.
Complete C_MTXFrustum/9c SHA256
58f3033c5cbba465241364f450085ee57aa3a8ff887bbf2702a426fef65b28fc.
shadowInit/4f8
9c5b1d7ab733458feb069916b13f768dc01d290206addc7bda1a67952114d1f6.
Projection config getter/1c
86fdec9f328fd802b19405fab9ae78d7a80fe602fc453625b9e5d0b2ca3125c7;
depth config getter/1c
0411f0331fd836ca7e0daefe49f63b54ded9b2d836dc6ea5b789dbb911673fd2.
Original48hex-byte type3 vertex array SHA256
fd6a015a379ff7db983b2060be4790b4affb087dd0c68c064f02523161d7b435.

Documentation only; no native code/index/shared progress edits, runtime traces,
emulator/hardware proof or complete profile mutation history claimed.
