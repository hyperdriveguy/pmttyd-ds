# Shadow camera2 class5 ordered suffix

US G8ME01 disc0 revision0, read-only recovery 2026-10-14. Extends
[request ownership and preparation](BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md).
Addresses/sizes are hexadecimal unless a decimal value is stated.

## Dispatch and complete caller suffix

shadowDisp8002d3d8/7b4 always calls dispGetCurWork80010864 first. For passed
camera2 it reads that retained record's **class byte1**, checks unsigned<=7,
and dispatches through80304fb0. Class5 enters8002d658. The passed userdata
is retained unchanged as r29. The exact ordered caller suffix is:

1. shadowCharShadowDisp_Collision8002d16c(camera2,userdata), at8002d660.
2. Fresh primary cswp804150e0 pointer and its word8 select mutable
   shadowConfig row at80304a60+72*word8. If selected row.word0==1, call
   camGetPtr8000f2fc(4), at8002d688. Write row eye=(camera4.atX,
   single(1000+camera4.atY),camera4.atZ), up=(0,0,-1), at=camera4.at.
   Otherwise keep row vectors. No range repair/clamp is present locally.
3. Freshly capture primary owner r30, request GXInvalidateTexAll802b7b54,
   at8002d6dc. Select row again through captured owner.word8 and read
   row.word40 as ShadowSizeTbl804150ec index; capture its unsignedhalf sizeN.
4. GXGetViewportv802ba5b4 into retained8041e720 at8002d704, then
   GXGetScissor802ba6bc into retained8041e710 at8002d724.
5. GXSetViewport802ba56c(0,0,float(N),float(N),0,1), at8002d764; then
   GXSetScissor802ba644(1,1,N-2,N-2), at8002d778. Preserve unsigned
   register/narrowing behavior outside ordinary finite N; no native clamp
   is an original producer.
6. Fresh captured owner.word8 selects row once more. C_MTXLookAt8029e330
   writes owner+0c from copied row eye, live row up at10 and at at1c.
7. If captured row.word38==0, C_MTXFrustum8029e74c; otherwise
   C_MTXOrtho8029e8b8. Destination owner+3c; both get arguments
   (top=row2c,bottom=-row2c,left=row28,right=-row28,near=row30,far=row34).
8. Reload captured owner.word8 and corresponding row.word38, then
   GXSetProjection802ba184(owner+3c,live row.word38), at8002d824.
9. camGetCurPtr8000f2e4 at8002d828; PSMTXCopy8029dd30 copies its view+11c
   into retained8041e6e0. Call camGetCurPtr **again** at8002d838; copy
   owner+0c into that fresh current camera's view+11c. These pointer reads
   cannot be collapsed when a provider may change current camera.
10. shadowCharShadowDisp_Projection8002caf4(camera2,userdata), at8002d854.
11. Fresh cswp pointer, then projShadowEnd8002b834(camera2,cswp), at8002d860.
12. Return. No shadow count reset or callback queue reset belongs here.

Source shadowConfig is exactly two72-byte rows, total90hex bytes. Fresh
shadowInit primary.word8=1 selects row1: word0=1, up initially(0,1,0),
row28=-240,row2c=240,near80,far10000,kind1,size-index0. After the camera
update its up is(0,0,-1). ShadowSizeTbl holds decimal400,256,128,64;
fresh row1 therefore uses400x400 and scissor(1,1,398,398). Other profile
writers remain independent owners; do not assume this row forever.

## First genuine providers after collision

In the fresh native profile1 domain the first further query is camGetPtr(4),
followed by the first GPU service GXInvalidateTexAll. Current native ShadowWork
stores retained request settings/pool and mutable projection profiles; word8
is named `profileIndex`. Conditional target tracking is now bound, but
view/image setup and offscreen capture remain unavailable. Binding collision
and tracking alone cannot complete class5. A backend must provide actual state/readback,
shadow profile, shader/matrix and image services before returning success.

Projection8002caf4 captures primary pool pointer, selected row, current-camera
view and source alpha. It emits unconditional channel/material/TEV/cull/
descriptor/blend/depth setup **before** iterating requests. Its color setup
also reads GP.word14 at8002ccec..cf8. The record loop draws only nonzero
radius,type3,preparedflag2; no record camera equality gate appears in this
loop. Each actual draw increments primary.word4. It reloads primary104 on
each loop test. Afterward captured primary.word4!=0 admits further image/GX
work. All-type2 requests skip those draws, but unconditional GPU setup remains.
Thus a type2 pool does not authorize Projection to become a no-op.

projShadowEnd8002b834 first queries current camera, restores its view from
8041e6e0, restores saved viewport then saved scissor, **before** testing the
passed owner's word4. A zero counter skips image capture/publication only.
Nonzero counter admits original sizing/smart allocation, texture copy/format,
GXCopyTex802b6844/GXPixModeSync802b5658 and texture-matrix publication,
then sets ownerflag1.
Those separate transitive services must be retained; no fabricated offscreen
texture or completion bit substitutes for them. Restoring viewport/scissor
without view restoration also omits an original effect.

## Live native finishing pool and camera predicates

shadowMain registers borrowed callbacks then resets primary104/count and
counterword4, clears flag1 and similarly resets two auxiliary owners. Battle
Main adds **new** requests after this reset. The subsequent Draw traverses
camera0..12, so camera2 class5 prepares this live pool before camera4 draws.
Display reset after the pass leaves shadow requests intact until next Main.
Retained continuations must not reset/recollect this pool on callback retry.

Native default requests are type2,camera4. Collision queries current camera
once on entry; it captures the primary pool pointer and reloads fresh primary
count each loop test. It skips radius0 or already-prepared2, then performs
its actual ray/matrix branch; the query camera does not supply a recordcamera
filter in this preparation scan. Collision completion alone does not establish
class5 suffix completion or a shadow image.

Visible camera1/4/5/7 shadowDisp invokes Polygon, unconditional GPU setup,
Cylinder, then Texture. Cylinder8002bf84 queries current camera once and
captures its view, then scans captured pool with fresh primary count. It
requires radius!=0,type2,preparedflag2 and **signed recordcamera==passed draw
camera**. Therefore default camera4 records draw in camera4, while1/5/7
skip them. Camera1's extra sysWaitDrawSync/GXInvalidate gate is inside an
admitted record branch; it is not called for default camera4-only records.
Passed camera ID controls admission while queried current camera controls
view matrix; conflating those two identities is unsafe. Type0 polygon and
type1 texture44 remain separate paths, not cylinder replacements.

## Evidence and limits

Fresh ignored complete listings/inspection ELFs:
artifacts/reference/shadow-class5/; original shadowDisp listing at
artifacts/reference/shadow-texture44/. Original table/constants reread through
DOL sections into rows.json. Mutable shadowConfig startup bytes SHA256
b01f90b7b4e7213c09322f2c3319369f4f1c0a4b8b11dd2033163d94dc731c46.

| Complete body | SHA256 |
| --- | --- |
| shadowDisp/7b4 | ade549cabd0dcc3d9d69acec14ed8f09c77ff4b4c2e6e063a1c7ac1ce36819ad |
| Collision/214 | 66a1090b8706be85e643ce99cff7a9bf75429f0d0c90c31ddc09e6088d20ef0d |
| Projection/678 | 45d3da8fc34ce3ec08cb7e01bf75d05b5f96cfbe6794bef0770490b3d34ce3e1 |
| projShadowEnd/294 | d5b52533c33a488880d068d80e68a2e658a770618bbd8a88a10d3328118735ca |
| Cylinder/368 | 9997a7bc2fa9a50f2dcca8aa45b77b8dd43841e067d4cd4315093d608cab49b5 |

Documentation only. No native code/shared index/progress edit, original runtime,
emulator or hardware validation. Exact nested image/GPU sequence implementation,
whole profile mutation history and raster correspondence remain integration work.

## 2026-10-14: precise captures, target tracking and native viewport boundary

Fresh US shadowDisp listing independently confirms the camera4 vector used
above is **at/target at18/1c/20**, not eye at0c/10/14. Exact row1 update:

| Order | Read/write |
| --- | --- |
| 1 | Fresh primary pointer, read word8, capture row token80304a60+72*index |
| 2 | Read captured row.word0; only exact1 calls camGetPtr(4) |
| 3 | On returned camera4 token, read atX/+18; write row.eyeX/+4 |
| 4 | Read atY/+1c; fadds(float1000,atY); write row.eyeY/+8 |
| 5 | Read atZ/+20; write row.eyeZ/+c |
| 6 | Store row.up/+10,+14,+18 as source +0,+0,-1 |
| 7 | Read camera4.atX/+18 AGAIN; write row.atX/+1c |
| 8 | Read camera4.atY/+1c AGAIN; write row.atY/+20 |
| 9 | Read camera4.atZ/+20 AGAIN; write row.atZ/+24 |

The row token/condition survives a held camera query; no completed update is
replayed on later GPU waits. After this conditional block, source captures
a FRESH primary pointer r30 before GXInvalidateTexAll. After that call returns,
it reads **captured r30.word8**, selects a row, captures row.word40 size-index
and its unsigned halfword table size. Do not reread global primary in place
of retained r30, and do not capture size before the invalidate service.
Saved viewport/scissor destinations are original independent buffers;
successful first capture must survive a held second capture without replay.
N is captured once for both setup calls. GX scissor arguments are x1,y1,
widthN-2,heightN-2, not right/bottom limits.

After scissor setup, fresh r30.word8 selects a NEW captured row tokenr26.
Copy eye raw words row+4/+8/+c into a local vector. C_MTXLookAt destination
is captured r30+0c, with that copied eye and borrowed up/at row+10/+1c.
Its eye copy and row addresses survive a held numeric helper. After LookAt,
read **captured r26.word38** for projection branch. Zero invokes Frustum,
all nonzero invokes Ortho; both retain their ordered float arguments and
destination r30+3c across a hold. Top=row2c, bottom=-row2c, left=row28,
right=-row28, near=row30,far=row34. Source reads row28 before row2c in each
argument setup. After helper, read fresh r30.word8 and that selected row.word38
for GXSetProjection; this kind can differ from the earlier helper branch.
Source does not rebuild the matrix just because that live kind changed.

After GXSetProjection, two separate camGetCurPtr calls surround the copies:
first borrowed camera view→saved8041e6e0; second captured r30 view→fresh
camera view. Do not collapse their camera tokens or replace original source
3x4 views with a rotated/composed PICA transform. Only then call Projection.
Each successful capture/write remains complete while a later provider holds.

### Retained startup table semantics

Primary word8 is a **profile index**, not boolean enabled. Index0 is a valid
frustum profile; index1 is the default tracking orthographic profile. Native
`ShadowWork.profileIndex` now names this field and binds its tracking consumer.
The previous `enabled` name was misleading. Full mutable table
rows must be retained, including fields not used by this caller:

| Field | Startup row0 | Startup row1 |
| --- | --- | --- |
| word0 tracking selector | 0 | 1 |
| eye4/8/c | (-100,256,335) | (135,40,41) |
| up10/14/18 | (0,1,0) | (0,1,0) |
| at1c/20/24 | (0,0,0) | (0,0,0) |
| float28/2c | (-20,20) | (-240,240) |
| near30/far34 | (356,741) | (80,10000) |
| word38 projection kind | 0 | 1 |
| word3c retained helper field | 3 | 3 |
| word40 size selector | 0 | 0 |
| float44 retained helper field | -4.46999979019165 | +0 |

Tables are source `.data`, not immutable constants once runtime writers act.
ShadowSizeTbl unsigned halfwords are400,256,128,64. Array-out-of-domain owners
must stay unavailable in a bounded native provider; original source has no
range clamp. Tracking constant1000 is float8041fa0c=447a0000; upZ-1 is
8041f9e8=bf800000. Full144-byte initial table hash is above.

### GP14 color owner at Projection

Projection first captures primary/row/pool, queries current camera, then
freshly reads primary color BYTE103. After unconditional shader/descriptor
setup, fresh GP pointer→WORD14 at8002ccec/f0 selects material color:
nonzero uses source8041f9c4=ffffffff (white RGBA255). Zero copies
8041f9c8=000000ff, overwrites RGB with that previously captured color byte,
retains alpha255, then calls GXSetChanMatColor(4,&localRGBA). This is separate
from earlier shadow global black-alpha110 material and inherited TEVREG0.

Current native TrioAttack explicitly sets vm.gp.battleContext=1 after its
context initialization. EventGpAddresses maps selected current GP+14 to that
live word and supports GP pointer replacement. A provider should read current
GP through that owner at the later color stage; Scene/old snapshots or an
assumed eternal battle context cannot substitute it. This native publication
does not certify full original sequence Init/Main history.

### Genuine400-square viewport adapter choices

Existing nativeCameraRect admits only exact integer physical endpoints under
source608x480→tilted240x400 mapping. Viewport(0,0,400,400) yields physical
(x40,y136.842105263,width200,height263.157894737); scissor(1,1,398,398)
also has half/noninteger endpoints. Both lie outside that exact adapter
domain. Source class5 type2-only/no-primitive domain still emits setup and
readback; returning success without C3D operations would be a fabricated
zero-domain provider.

A new explicit rounded-endpoint adapter can perform real main-target viewport
and scissor calls while retaining original logical values for source readback/
restoration; rounding and pixel coverage are native raster differences.
Alternatively a real unlinked400x400 C3D render target can admit viewport
(0,0,400,400) and scissor exclusive(1,1,399,399), then genuinely rebind and
restore the preceding target/state. That is a scratch-framebuffer adaptation:
future drawn/image-capture domains require their actual entering framebuffer
color/depth and copy owners, not a claim that a newly allocated target equals
the original EFB. Target/matrix/cache lifecycle and source projShadowEnd remain
required in either choice. No new implementation or native check is claimed
by this documentation appendix.
