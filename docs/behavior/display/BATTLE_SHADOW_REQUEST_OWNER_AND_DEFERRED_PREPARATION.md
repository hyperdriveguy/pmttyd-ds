# Battle shadow request owner and deferred preparation

2026-10-14, original US G8ME01 disc0 revision0, read-only DOL evidence.
This closes EVT-01's shadowEntry frontier and identifies deferred preparation
and the next ordinary part service. It does not prove original execution,
setup4 rendered shadows or an implemented shadow collision/GX backend.
Offsets/addresses are hexadecimal. Source facts below refine
[generic shadow admission](ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md) and
[field shadow geometry](../overworld/OVERWORLD_MARIO_SHADOW_REQUEST_AND_GROUND_POLYGON.md).

## Owner, initialization and admitted default type

The primary owner pointer is SDA word804150e0, originally803c9e38; the two
auxiliary projection/image owners are slots804150e4/e8, originally803c9d34/
803c9c30. These are original address identities, not native pointers. A native
retained primary owner plus typed auxiliary state is a declared address adapter.
The original shadowInit8002dcec requests a1700-byte record pool from
__memAlloc(0,1700), retains it at owner108 and clears the full pool. It writes:

| Primary owner offset | Source initialized value |
| --- | --- |
| 0 /4 | 0 /0 |
| 8 | 1 |
| 100 | RGBA0000006e, source constant8041f9c0 |
| 104 | 0, active request count |
| 108 | newly allocated64 records of5c bytes |
| 10c | 1, camera2 class5 registration gate |
| 110 | 2, default shadow type |
| 114 | 20.0, source8041fa08 |
| 118 | 1, collision-enable request |

Additional init code allocates/initializes GPU image objects and auxiliary
buffers; the table is not a substitute for complete Init. Type2 is the original
fresh default, not a inferred ellipse or texture44. Later type/height/color/
collision/gate writers must remain retained; fresh initialization does not
establish they never change during setup4.

## Actual shadowEntry8002be98

This body has no external calls. Input f1/f2/f3=XYZ, f4=radius. Capture primary
count as slot index in r3, resolve owner108 + count*5c. Clear record HALF0.
Reload owner118 and set record flag1 if nonzero. Then write:

| Record offset | Written value |
| --- | --- |
| 2 | low byte of freshly reloaded owner110 |
| 3 | 0 |
| 4 /8 /c | requested X/Y/Z, copied float bits |
| 10 | requested radius |
| 14 | freshly reloaded owner114 |
| 18 | signed HALF−1 |
| 1a | signed HALF4, camera |
| 1c | 1.0 (8041f9e4) |
| 20 | binary32.2,3e4ccccd (8041f9ec) |
| 24 | binary32.4,3ecccccd (8041f9f0) |
| 28 | BYTE255 |

Record29..5b retain prior bytes. In particular Entry does not clear/rebuild the
matrix at2c; prepared flag2 was cleared by its HALF0 store. Reload primary
count at8002bf70, increment and publish owner104. Return with r3 retaining the
ORIGINAL slot index. No radius/finite/zero/capacity/camera checks occur. A native
64-slot guard can return unavailable outside this admitted domain, but cannot
drop a valid source request while reporting completion. Unlike display records,
this entry copies its four float inputs; it does not retain a part pointer.

Source record subtype variant3 and flags/payload setters may subsequently use
returned slot. Shared borrowed source identities need explicit typed resolution;
a fabricated ARM address is not the original return value.

## Main registration precedes request collection

shadowMain8002db8c first registers callback identity8002d3d8 (shadowDisp),
userdata0, priority+0, in this exact order:

1. camera4,class9;
2. camera1,class9;
3. camera5,class9;
4. camera7,class9;
5. camera2,class5, only if primary10c!=0;
6. camera2,class0;
7. camera2,class2;
8. camera2,class7.

The primary10c conditional is initialized1. It then resets primary104=0,
clears only flag1 in primary0 and resets primary4=0; performs the same flag1
clear and word4 reset on auxiliary owners804150e4 and804150e8. It does not
clear records, prepare matrices, retain a snapshot count, cancel its queued
callbacks or reset type/height/collision/color defaults.

The actual [outer update order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md)
calls shadowMain at8006f938 BEFORE battleMain8006f940 and later icon/field
requests. These callbacks are registered for **new live requests collected
AFTER the count reset**, including the battle part shadowEntry at80102ec8.
Do not copy the old pool at Main and draw that snapshot, prepare before the
battle records exist, or reset count again after battle registration.

The next outer display calls camDraw8000f454. It sorts display pointers once
at8000f470, visits camera IDs0..12 in ascending order, skips camera HALF0 flag1,
publishes current camera ID before each admitted camera draw and calls
dispDraw(cameraID) at8000f50c. Only after all13 camera opportunities does
8000f528 call dispReInit to clear DISPLAY count. This does not clear shadow
count. Thus when camera2 is admitted, its preparation pass precedes camera4's
visible shadow pass. Actual camera flags and those services still need owners;
registration alone does not prove a preparation callback ran.

## Preparation and real type2 rendering dependency

shadowDisp first obtains dispGetCurWork80010864. For camera2 it uses the
current record CLASS byte1 to dispatch, not a shadow type or copied priority.
Class5 target8002d658 calls shadowCharShadowDisp_Collision8002d16c FIRST.
The other camera2 class targets are0→8002d868,2→8002d9e0,7→8002db6c;
these own further projection/image passes, not no-op placeholders.

Collision obtains primary pool and current camera, then scans index<fresh
primary104 on each loop test. Radius==0 or record flag2 set skips. Type!=3
and flag1 set performs its independent downward ray query: origin XYZ+(0,1,0),
direction(0,−1,0), length150, source filter8002d380. Query miss skips WITHOUT
setting prepared2. Query hit builds record2c..5b matrix from actual point and
normal using radius/height; exact geometry/filter are in the field contract.
Type3 OR flag1clear instead initializes a translation to(X,Y+1.5,Z), then
sets matrix00/28=radius and14=height. Both successful branches OR flag2.
There is no packed battle floor substitution for this independent collision
ray. The battle caller's prior floor/radius calculation owns different inputs.

For camera4 (also1/5/7), shadowDisp calls Polygon8002c818, configures GX,
then cylinderShadowDraw8002bf84, then Texture8002c484. Type0/2/1 admission
selects these separate backends. Cylinder requires nonzero radius, type2,
prepared2 and record signed camera1a==draw camera. It concatenates current
camera view with prepared record matrix before original cylinder/GX drawing.
There is no synthetic ellipse producer in this source chain. A native retained
request owner can close Entry while these actual deferred providers remain
explicitly unavailable; marking a record prepared without a ray/matrix provider
or skipping its required backend is not source completion.

## Immediate ordinary part continuation

After battle shadowEntry returns, ordinary Mario kinddf skips the paper-kind
filter and calls801261f8 at80102f74. Complete source body length2bc has these
early admissions: null P→return; WORD(P4d0)==0→return; P1c0 exactly−1→return;
otherwise request animPoseGetAnimPosePtr80039cf8 and return if null. It is not
a generic unconditional no-op. The nonempty-table body consumes pose24/84,
retained timing fields P4d4..4e4, table entries and sound requests.

Original Entry clears the whole count*500 part block and does not initialize
P4d0 afterward; fresh no-table parts take the no-service return. The actual
AnimeSetPoseSoundTable event writer801063f4 writes table pointerP4d0 and timing
reset fields. Therefore later enabled-table parts must reopen the real sound
consumer; do not infer a permanent no-table gate from initial zero allocation.
Following a completed tail, btlDispMain loads live next-part pointer P0 and
continues its per-part loop before its remaining unit-tail callbacks.

## Evidence and limits

Fresh read-only ignored artifacts: artifacts/battle-shadow-evidence. Body hashes:

| Original range | SHA256 |
| --- | --- |
| shadowEntry8002be98:ec | 06cfea58a6edbb4341716d99601d96c02f7c40ac181db95ae8d5363b5d418386 |
| shadowMain8002db8c:160 | bd4e5dd15762e6adab10883b3941053f1e5e6f827e320071788cd6b4530d86a8 |
| shadowInit8002dcec:4f8 | 9c5b1d7ab733458feb069916b13f768dc01d290206addc7bda1a67952114d1f6 |
| shadowDisp8002d3d8:7b4 | ade549cabd0dcc3d9d69acec14ed8f09c77ff4b4c2e6e063a1c7ac1ce36819ad |
| camDraw8000f454:f4 | 846397680c48a140dbaed42a58605cf8ee6cf7f8ee648f52003f97455b36c783 |
| part tail801261f8:2bc | 3f230e6dde7ec35f112d5902ab9e8cb8c0ef39745cd17a67c3579a5fd952076b |
| Collision8002d16c:214 | 66a1090b8706be85e643ce99cff7a9bf75429f0d0c90c31ddc09e6088d20ef0d |

No native code, original setup4 execution or shadow raster fidelity is delivered
by this documentation. Full Init/image allocation, mutable globals, independent
ray owner, camera2 passes, actual GX/cylinder provider and nonempty pose-sound
consumer remain separate obligations. EVT-01 and full prologue remain open.
