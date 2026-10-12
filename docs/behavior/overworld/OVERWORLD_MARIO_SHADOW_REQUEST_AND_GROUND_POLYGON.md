# Original Mario shadow request and grounded polygon

US G8ME01 revision0. Source reconstruction of marioPreDisp, shadowEntryMode,
shadowCharShadowDisp_Collision/Polygon and their shipped display scheduling.
This closes the ordinary supported type0 shadow geometry and its fresh ray
query. Airborne type2 uses a separate GX volume backend, whose geometry/data
identity and admission are pinned here but whose complete raster result is
not claimed reproduced. No native code or original runtime observation.

## Mario caller

After pose Main and actor-display registration, MarioPreDisp checks display1:
if set, exits before these registrations. At80057b08–1c, **display1000** or
**main02000000** suppresses all shadow requests. Ordinary character3c=0
sets X/Z offset0 and size **13.333333015441895** (word41555555,
80420044;80057c08–10). These are original engine units. Native world scaling
is an adapter; do not identify size with Mario's physical radius or actor height.

At80057c14–64, if retained support pointer1e8 is nonnull, call
`shadowEntryMode(0,root8c,root90,root94,size)`. This is a pointer test, not
an airborne motion test: a motion3 ascent retaining its node can still take
this branch. It does not use the rendered pose pivot/offset98/a4 or contact
point as the root argument. The extra main02000000 size branch here is
unreachable after the preceding suppressing guard in this bounded callback.

With null support, h=root90−retainedGround1c4. At80057c68–d4:

    if h < -2: no request
    s = size - h / 13.333333015441895
    if s < float(word3dcccccd): s = 0
    if s == 0: no request
    else shadowEntryMode(2,root8c,root90,root94,s)

Threshold equality−2 is admitted; no arbitrary jump-height fade or ellipse
radius floor. Motion24 has an earlier special type2/rootY=1c4 request;
character1/2 use authored offsets/sizes and are outside ordinary Mario0.

## Queued record and lifetime

shadowEntryMode8002bdb0 indexes its92-byte array by current count104,
clears record flags, sets bit1 if global118!=0, stores requested type byte2,
rootXYZ at4/8/c, size10, retained global height114 at14, variant byte3=0,
image handle18=−1, camera1a=4, scalar1c=1, scalar20=.2, scalar24=.4,
alpha28=255; increments global count104. It has no local capacity guard.
shadowInit allocates64 records (5888bytes), sets global118=1,height114=20,
color100=**RGBA(0,0,0,110)** at8002dd34–60.

shadowMain queues shadowDisp for cameras4/1/5/7 at display stage9;
camera2 at stages5/0/2/7 (stage5 conditional on global10c!=0, initialized1).
It resets count104 at8002dc88–90 for subsequent entries. Display callbacks
consume the current shared array: this is deferred queue/display ownership,
not drawing at EntryMode. dispEntry stores camera byte0 and **stage byte1**
at80010bdc/e4; do not mistake shadowDisp's stage dispatch for a camera mode.
Original frame manager/display sort order remains an external service contract.

For camera2/stage5, shadowDisp calls Collision at8002d658–60 before its
projection pass. For ordinary visible camera4/stage9 it calls Polygon at
8002d420, later cylinderShadowDraw and Texture. Their filters select distinct
record types: Polygon0, cylinder2, Texture1. A type0 ground shadow does not
use Texture's icon44 or a cylinder display list.

## Fresh shadow collision query

Collision8002d16c skips size0 or already-prepared flag2. With type!=3 and
record bit1 set, construct HitQuery at8002d1d4–230:

- Origin=requestedRoot+(0,**1**,0).
- Direction=(0,−1,0), pinned vector802bfd10.
- Length=**150**, word43160000 at8041fa00.
- Filter=shadowdrv `_filter`8002d380, passed to hitCheckVecFilter800158a4.

Do not reuse Mario's winning support normal/point or an attr4-filtered ground
probe. This query independently chooses the ray hit. `_filter` calls hitGetAttr:
if attr80000000 is set, obtains mobjHitObjPtrToPtr(node) and rejects only if
that object's flags bit800 is set; otherwise returns1. Ordinary map nodes
without this tag pass even with attr4. Node enabled/culling/intersection rules
remain those of [the source ray provider](OVERWORLD_ROAD_POSITION_AND_CONTACT.md).
No new cull argument is supplied by this leaf: runtime node face cull applies.
Returned point/normal are raw query outputs; this owner performs no Q1000,
normal normalization or winding flip. Tagged object/runtime availability must
be an explicit dependency when the native provider contains only map geometry.

On miss it skips preparation/drawing for this record. On hit, let P be returned
point, N=(nx,ny,nz) returned source normal, s=record10,h=record14. Store this
literal3x4 matrix at record2c (8002d23c–300), preserving the signed normal:

    [-s*(ny*ny+nz*nz), s*nx*ny, s*nx*nz, Px+nx]
    [s*nx,             h*ny,    s*nz,    Py+ny]
    [0,                s*nz,   -s*ny,    Pz+nz]

The source uses mixed f32 operations, not a normalized general tangent basis.
For N=(0,1,0), this reduces to translate(Px,Py+1,Pz)*diag(−s,h,−s).
The plane geometry below has localY0, so h does not alter a horizontal shadow.
Do not invent a .01-native lift independently of the original unit mapping.

If type3 or bit1 clear, the bypass uses translate(rootX,rootY+**1.5**,rootZ)
and diagonal(size,height,size) (8002d308–334), then sets prepared flag2.
The ordinary initialized bit1 route uses the fresh ray. The flag2 write occurs
only after a hit or this explicit bypass, at8002d338–40.

## Actual grounded shape and visible draw

Polygon8002c818 admits size!=0, type0, prepared flag2, recordcamera1a equal
currentcamera (8002c9dc–a08). It loads current-camera view *record2c and issues
**GX_TRIANGLESTRIP0x98, six vertices** at8002ca38–44, indexed0,1,2,3,4,5
at8002ca48–7c. Original `testKagePosArray`80304b00 contains:

| Index | Local XYZ |
| --- | --- |
| 0 | (0,0,−1) |
| 1 | (.8999999761581421,0,−.5) |
| 2 | (−.8999999761581421,0,−.5) |
| 3 | (.8999999761581421,0,.5) |
| 4 | (−.8999999761581421,0,.5) |
| 5 | (0,0,1) |

This is a six-point polygon with four strip triangles, not an authored bitmap,
texture-circle approximation, triangle fan or smooth ellipse. The primitive's
constant channel material color is global100, default black alpha110
(8002c880–8c). Polygon requests source-alpha/inverse-source-alpha blending,
Z test enabled/LEQUAL/write disabled, cull none and no texture generators
(8002c890–960/984–98). It emits indexed positions without vertex color/UV.
Ordinary current opacity comes from global color; actor pose alpha and airborne
record attenuation are separate paths. Source shader/GX adaptation and render
ordering must be preserved or labeled when drawing this on Citro3D.

## Air volume / other backend boundary

Type2 is admitted by cylinderShadowDraw only with prepared flag2, camera match
and size!=0 (8002bfc8–ff4). It uses the same Collision matrix then three GX
passes with stencil-like color/blend/depth/cull requests. Source `vertex_tbl`
80304ce0 has60 vertices: top/bottom14-sided rings (with repeated endpoint),
inner top ring radius.6, and box vertices. Five64-byte cylinder display lists
80304b60–c60 encode top/bottom/side/inner/outer draws; cylinder8002c2ec applies
per-record/global opacity and the .4 decay field. No external Mario shadow
texture is selected in this path. Exact lists, vertex table and complete two
containing bodies are hashed, **not** a closed portable GX volume/raster solver.

Grounded type0 geometry/query can be bound independently. Type2 multi-pass GX
semantics, live color/height writers, full scheduler and special type1/3, cube
variant/paper/characters and image-cache domains need separate recovery/adapters.
Do not claim the native air ellipse is original because its center/size was
replaced by this caller formula, or feed the grounded strip into all type2 cases.

Verify `python3 tools/verify_overworld_mario_shadow.py`:14 containing bodies,
10 data spans,200 anchors, six literal polygon vertices and13 bounded request,
horizontal matrix/winding/filter checks. Host arithmetic is adapted; no original
execution, complete backend raster equivalence or image comparison is claimed.
