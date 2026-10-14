# Camera manager boot, update and draw publication

US G8ME01 revision0, read-only DOL inspection2026-10-14. This contract recovers
camInit8000fd9c, camEntryPersp8000fac8, camMain8000f704, camDraw8000f454 and
source SDK LookAt/normalize/cross. It supplements
[matrix publication arithmetic](../display/BATTLE_CAMERA4_MATRIX_PUBLICATION_ARITHMETIC.md),
[camera4 inherited history](UNDERGROUND_BATTLE_CAMERA4_ENTRY_AND_INHERITED_LENS.md)
and [current-camera accessor](CAM_GET_CUR_PTR.md). Static evidence only; no
original execution, hardware trace or raster-equivalence assertion.

## Persistent boot table

camInit allocates thirteen independent 0x260-byte camera records through
__memAlloc800300f0(heap0,size0x260), clears each complete record, then publishes
pointers into camPtrTbl803d95f8, slots0..12. Original constructor vectors read
from camPtDefault80414fb0=(0,0,1000), target80414fbc=(0,0,0),
camUpDefault80414fc8=(0,1,0). These are explicit input vectors.

Common fields: flags halfword+0 starts0 through complete memset; +2/+4 start0;
eye+0c, target+18, up+24; near+30,far+34,FOV+38,aspect+3c; roll+14c and
view-offsets+150/+154/+158 zero; projection+15c is constructed; projection
kind+19c is0 for perspective/1 for ortho. View+11c and extra+1a0 are still
zero until their actual producer executes. Callback+1ec and name+254 are
retained; word+1e8=8 is a separate draw-camera field, **not flags0=8**.
The perspective constructor explicitly writes +2/+4/+8 zero. The inline ortho
constructors leave other cleared fields zero.

All scissorXY halfwords+f4/+f6 and viewportXY floats+fc/+100 are0.
Scissor width/height+f8/+fa and viewport width/height+104/+108 independently
read DEMOGetRenderModeObj8002eb30 fields+4/+6. Reviewed ordinary NTSC dimensions
are608x480. Source viewport depth+10c/+110 is distinct from lens near/far.
Distance+118 initializes from PSVECDistance(eye,target), before later eye
replacement described below; yaw+114 starts0. Lens metadata remains25 and
aspect0x3fa22222 in inline ortho entries too, independently of their matrix.

| Slot/name | Projection and source lens or Ortho arguments | Depth range | Callback |
| --- | --- | --- | --- |
| 0 `off` | Ortho(top240,bottom-240,left-304,right304,near1,far32768) | 0..1 | null |
| 1 `off2` | Ortho(0,480,0,608,1,32768) | 0..1 | null |
| 2 `bg` | Ortho(0,480,0,608,1,32768) | 0..1 | null |
| 3 `shadow` | Ortho(0,480,0,608,1,32768) | 0..1 | null |
| 4 `3d` | Perspective(FOV25,aspect0x3fa22222,near1,far32768) | 0..1 | cam3dMain8000ea68 |
| 5 `3deff_A` | Perspective(25,0x3fa22222,100,10000) | 0x3e4ccccd..0x3e99999a | camEffMain8000e974 |
| 6 `3dimg` | Perspective(25,0x3faaaaab,1,32768) | 0..1 | camImgMain8000ea30 |
| 7 `3deff_B` | Perspective(25,0x3fa22222,100,10000) | 0x3e4ccccd..0x3e99999a | camEffMain8000e974 |
| 8 `2d` | Perspective(25,0x3fa22222,100,10000) | 0x3dcccccd..0x3e4ccccd | null |
| 9 `fade` | Perspective(25,0x3faaaaab,1,32768) | 0..0x3dcccccd | null |
| 10 `fade2` | Perspective(25,0x3faaaaab,1,32768) | 0..0x3dcccccd | null |
| 11 `dbg` | Ortho(0,480,0,608,1,32768) | 0..1 | null |
| 12 `dbg3d` | Perspective(25,0x3fa22222,1,32768) | 0..1 | null |

Inline ortho source calls: slot0 at8000fdf0, slot1 at8000ff7c, slot2 at80010108,
slot3 at80010294, slot11 at800106d0. Perspective calls for4..10 at80010404,
80010438,8001046c,800104f8,80010528,800105ac,80010630; slot12 at8001083c.
Slot6 replaces eye with(0,0,400/frsp(tan(A))) at800104d4..e8; slot8 replaces
eye with(0,0,240/frsp(tan(A))) at80010594..a4. Slots9/10 replace eye with
(0,0,400/frsp(tan(A))). A is original double802bf658=0.21816612780094147,
read by camInit through802bf5d8+80, tan802724e8 returnsdouble, explicitly frsp
before fdivs. Do not substitute independently computed pi or FOV conversion
and claim original rounding. Slot7 keeps the default until camEffMain copies
camera4 vectors.

Common source constants freshly read: zero8041f6b4=00000000,
240 at8041f6bc=43700000, one at8041f6c0=3f800000,
25 at8041f6cc=41c80000,32768 at8041f6d0=47000000,
100 at8041f6d4=42c80000,aspect at8041f6d8=3fa22222,
400 at8041f700=43c80000,10000 at8041f720=461c4000,
4/3 at8041f728=3faaaaab,.1 at8041f738=3dcccccd.
Constructor zero/view and field-camera lifetime are separate evidence; native
boot defaults must not overwrite inherited camera4 lens/event state on battle
entry.

## camMain updates every slot; flags0 bit1 does not filter it

Loop8000f758..8000fa88 visits indices0..12 ascending. It assumes thirteen
published pointers, loads callback+1ec, invokes any nonnull callback with the
camera pointer, then constructs C_MTXLookAt(out=C+11c,eye=C+0c,up=C+24,
at=C+18). There is **no flags0 admission check** and no current-camera-index
publication in this loop. Flags disabled for drawing therefore do not suppress
callbacks, view rebuilding or extra-matrix consumption.

Then roll rotationRz and translationT concatenate into view in source order:
V=T*(Rz*LookAt). Both constructors and both concats execute for zero values.
Only index4 has Mario zoom/screen correction, and only when global GP+14==0.
That whole conditional block8000f7d8..8000fa50 is a separate provider,
not an unconditional identity. A native unavailable callback or matrix provider
must suspend at that exact service rather than mark all camera records updated.

Finally for every camera, flags0 mask8 causes PSMTX44Concat(C+1a0,C+15c,
C+15c), then clears exactly bit8 after the call returns. Bit8 is a one-use
projection-extra request; camMain still consumes it on draw-disabled cameras.
The exact multiply and alias ordering is in the linked arithmetic contract.
Camera5/7 copy camera4 eye/up/target then rebuild their own projection; camera6
callback solely rebuilds perspective from its retained lens. Source callback
camera4 may consume battle controller output, but normal field scheduling and
prior shake/lens history remain independent providers.

## Source SDK LookAt and normalization

C_MTXLookAt8029e330/18c computes:
back=normalize(eye-at), right=normalize(cross(up,back)), correctedUp=cross(back,right).
It does not normalize input up first and does not normalize correctedUp again.
Rows0/1/2 of the 3x4 output are right/correctedUp/back. Every translation term
separately multiplies eyeX*rowX and eyeY*rowY, adds those, then adds eyeZ*rowZ
in the explicit source order `-(pz+(px+py))`; each product/add is single.
No fallback for coincident eye/at, parallel up/back or other degeneracy is
present in the original leaves. Native finite/degenerate admission policy is
an adapter boundary, not recovered fallback behavior.

PSVECNormalize8029ebe4/44 uses paired singles. With x/y/z single inputs:
px=single(x*x), py=single(y*y),
length2=single(single_fma(z,z,px)+py).
Then seed=frsqrte(length2), q=single(seed*seed), h=single(seed*.5),
r=single_fnmsub(q,length2,3), factor=single(r*h), output=single(component*factor).
The .5 and3 literals are80429394=3f000000 and80429398=40400000.
Source has **one** Newton correction, no explicit zero branch. The seed is
Gekko reciprocal-square-root estimate; substituting exact1/sqrt or native
hardware rsqrt changes numerical behavior and needs a documented adapter.
The paired-single sequence follows8029ebec..8029ec20; it is not an ordinary
reassociated sum of three squared scalars.

PSVECCrossProduct8029eca4/3c uses prior rounded products and fused
multiply-subtracts, then negates two lanes before storing:
x=single_fmsub(aY,bZ,single(bY*aZ));
y=-single_fmsub(aX,bZ,single(bX*aZ));
z=-single_fmsub(aY,bX,single(bY*aX)).
Thus using mathematically equivalent swapped subtractions can alter signed zero,
rounding or NaN signs. Existing native upright camera math is a numerical adapter
until these exact source arithmetic requests/seed behavior are supplied.

## camDraw current camera and real request order

At8000f46c camDraw first calls camLetterBox8000e388, which has real predicates,
mutations and a dispEntry for camera8/its stored draw ID. It then calls
**dispSort80010b5c once**, before visiting thirteen slots. It is not permissible
to sort before letterbox registration and call that the same queue snapshot.
[Letterbox decision lock](SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md) documents
its source predicates separately; bit200 does not suppress the whole helper.

For each index0..12, load live camera pointer and halfword flags0; skip drawing
only if flags0&1. For an admitted camera:

1. Store current index to SDA+16e8 at8000f490. This is the publication consumed
   by camGetCurPtr, persists after the camera, and is not restored after callbacks.
2. GXSetViewport802ba56c(C+fc,C+100,C+104,C+108,C+10c,C+110).
3. GXSetScissor802ba644(unsignedhalf C+f4,+f6,+f8,+fa).
4. Reload flags0. If mask40 and projectionkind+19c==0, call
   camEvalNearFar8000f548(C), then C_MTXPerspective(C+15c,FOV,aspect,near,far).
   Constructorflags0 leave this branch unselected, but live external flags matter.
5. GXSetProjection802ba184(C+15c,projectionkind+19c).
6. dispDraw80010900(index), using the sorted shared pointer table and callback
   current-record publication already recovered by the native display queue.
7. sysDummyDraw80005ad8(C+11c). This is real GPU work after callbacks; source
   begins GXLoadPosMtxImm(view,0)802ba2fc, GXSetCurrentMtx(0)802ba39c,
   GXSetCullMode(0)802b6f3c, GXSetNumTexGens(1), GXSetNumTevStages(1),
   GXSetTexCoordGen2(0,1,4,60,0,125), and many TEV/channel/depth states. It
   disables color/alpha/depth writes and submits a real triangle. See
   [framebuffer-alpha owner](../overworld/OVERWORLD_FIELD_FRAMEBUFFER_ALPHA.md).
   Treating it as identity/no-op drops inherited GPU state for later cameras.

After all thirteen slots, camDraw calls dispReset80010c64 at8000f528.
It does not reset current-camera index. A suspended callback/provider must
retain traversal stage and index; replaying viewport/projection/letterbox or
resetting the queue before completion loses source ordering. Drawing disabled
all slots would leave prior current index intact, but still runs letterbox,
sort and final reset. camMain and camDraw are separate outer calls.

## Implementation order and evidence scope

First preserve thirteen boot records and current draw index independently from
battle camera-controller state. Implement ascending camMain with typed callback,
source LookAt/roll/offset/zoom/extra providers; publish its completion only after
all13 records. Reuse the recovered arithmetic contract for source matrices.
Then implement a resumable camDraw around shared queue: letterbox→sort→per-camera
publication/viewport/scissor/optional nearfar/projection→dispDraw→dummy→finalreset.
Bind actual native draw callbacks and PICA adaptation after these owners exist.
Camera2 stage/background and camera8 overlay are genuine independent cameras;
queue providers must not force their callbacks into camera4 matrices.

Open evidence: exact Gekko frsqrte seed implementation/hardware equivalence,
full live field-camera4 history/Mario zoom callback, camEvalNearFar geometry
providers, letterbox service lifetime, complete dummy draw typed request/raster
adapter, GX-to-PICA projection/depth/write masks and hardware image comparison.
The recovered ordering does not prove those services complete or justify
skipping them to release battle field-return.

Fresh read-only listings under ignored artifacts/reference/camera-manager/.
Body SHA256 (source bytes only):

| Span | SHA256 |
| --- | --- |
| camInit8000fd9c:ac8 | c63639c022820914ef06891f3f407fc38ba8ea681a48308fa1f061be9d6ca19d |
| camEntryPersp8000fac8:2d4 | 8124e9f8dda6fb8323a6b001a4d5cf9ea9b7669bfdf8ee1e666f0e28f253931b |
| camMain8000f704:3c4 | fd7074046f87e9b4806121364396793eff9fb0e43454df0d33fd46dd2d1e26c4 |
| camDraw8000f454:f4 | 846397680c48a140dbaed42a58605cf8ee6cf7f8ee648f52003f97455b36c783 |

No original code payloads/assets distributed or committed. Docs-only change,
no make/build/emulator claim. Original ISO remains read-only.
