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
   camGetPtr8000f2fc(4), at8002d688. Write row eye=(camera4.eyeX,
   single(camera4.eyeY+1000),camera4.eyeZ), up=(0,0,-1), at=camera4.eye.
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
stores retained request settings/pool; its word8 initializer is currently named
`enabled`, but has no bound mutable projection profile table/view-image owner
or offscreen capture allocation. Binding collision
alone cannot complete class5. A backend must provide actual state/readback,
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
