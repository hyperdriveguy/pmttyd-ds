# Shadow auxiliary setup and shared restore cursors

US G8ME01 disc0 revision0, read-only static recovery2026-10-14 for EVT-01.
Completes ordered finite counter0 camera2 class0/2/7 paths, alongside
[class5](SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md) and
[profiles/frustum](SHADOW_FRUSTUM_ARITHMETIC_AND_AUXILIARY_PROFILE_OWNERS.md).
Addresses/offsets are hexadecimal; ordinary sizes/indices are decimal.

## Shared saved buffers, separate image owners

All setup/End paths use the **same three retained saved buffers**:

| Buffer | Original address | Layout |
| --- | --- | --- |
| Saved current-camera view | 8041e6e0 | 12float3x4 matrix |
| Saved scissor | 8041e710 | four32-bit x,y,width,height |
| Saved viewport | 8041e720 | sixfloat x,y,width,height,minZ,maxZ |

Each setup overwrites these buffers in source order. They are not per-callback,
per-camera, per-owner or stack snapshots. The following End reads their current
contents at its actual restore requests; external mutation must remain visible.
Separate setup cursors can retain progress, but must share the saved-buffer
owner. Source never backs up/restores projection in these finite End prefixes.

Auxiliary pswp804150e4 and dswp804150e8 are distinct owners with boot
flags2,counter0,profile0,color00000040. Each has view at0c,projection at3c,
texture matrix at7c,embedded texture object atdc,smart image pointerfc,
color100. Their mutable profile selectors independently reference shared
shadowConfig80304a60 rows; class0 uses pswp,class2 setup uses dswp. Source
shadowMain clears only mask1 and counter4, preserving bit2/profile/colors.
Primary is a third owner, bootflags0/profile1/color0000006e, with its own
view/projection/image and the separate live request pool.

## Class0 complete setup cursor

shadowDisp first queries dispGetCurWork and dispatches class byte1 for passed
camera2. Class0 target8002d868 executes, without flag/counter predicates:

1. Capture fresh pswp ownerO into r30; GXInvalidateTexAll at8002d86c.
2. Read captured O.word8 **after** that call; select rowR, read R.word40
   size index, capture unsignedhalf N=ShadowSizeTbl[index] at8002d890.
3. GXGetViewportv(saved8041e720),8002d894.
4. GXGetScissor(saved8041e710),8002d8b4.
5. GXSetViewport(0,0,float(N),float(N),0,1),8002d8f4.
6. GXSetScissor(1,1,N-2,N-2),8002d908. N remains the captured size even
   if a provider changed profile/size table during preceding requests.
7. Fresh O.word8 selects row again. Copy row eye4/8/c into a localXYZ;
   C_MTXLookAt(O+0c,copiedEye,row+10 up,row+1c at),8002d948. The selected
   row pointer is retained across subsequent projection branch/arguments.
8. Read retained R.word38. Zero calls C_MTXFrustum(O+3c,...),8002d974;
   nonzero C_MTXOrtho(O+3c,...),8002d998. Exact arguments: top=R2c,
   bottom=-R2c,left=R28,right=-R28,near=R30,far=R34.
9. Reload O.word8 and select row **again**, then read its kind38 for
   GXSetProjection(O+3c,kind),8002d9b4. A provider may change this kind
   independently of the previously captured math branch.
10. camGetCurPtr at8002d9b8; copy its view11c into saved8041e6e0 at8002d9c4.
11. camGetCurPtr **again** at8002d9c8; copy captured O.view0c into that
    fresh current-camera view11c at8002d9d8.
12. Return. No immediate End,collision,request scan,counter increment,texture
    capture or profile follow-camera4 mutation occurs in this class0 setup.

Fresh pswp profile0 yields N400 and perspective kind0 with authored row0
eye/up/at,extents±20,near356/far741. This setup remains required for counter0.
Camera4 offsets18/1c/20 are **at**; auxiliary setup has no camera4 query.

## Class2 restore then depth setup cursor

Class2 target8002d9e0 first freshly fetches pswp and calls
projShadowEnd(camera2,thatOwner),8002d9e8. Its finite counter0 continuation:

1. Capture passed owner,query camGetCurPtr at8002b84c.
2. Copy saved8041e6e0→queried currentcamera.view11c,8002b85c.
3. GXSetViewport from live saved six floats,8002b87c.
4. GXSetScissor from live saved fourwords,8002b894.
5. Fresh passedOwner.word4 query at8002b898. If exactly0 return;
   otherwise retain the genuine image/copy/texture-publication suffix.

Only after this End completes, class2 captures fresh dswp ownerO at8002d9ec.
Then GXInvalidateTexAll at8002d9f0; read O.profile/row.size-index and capture
N at8002da14; **GXSetColorUpdate(0)** at8002da18 precedes viewport readback.
Continue with same setup algorithm as class0:

| Request | Source site |
| --- | --- |
| GetViewport→shared saved | 8002da20 |
| GetScissor→shared saved | 8002da40 |
| SetViewport square0..1 | 8002da80 |
| SetScissor1,1,N-2,N-2 | 8002da94 |
| Fresh profile select,copy eye,LookAt→O0c | 8002dad4 |
| Retained row kind branch Frustum/Ortho→O3c | 8002db00 /8002db24 |
| Fresh O.profile reselect/live kind,SetProjection | 8002db40 |
| Current query,backup view→shared saved | 8002db44 /8002db50 |
| Second current query,replace view←O0c | 8002db54 /8002db64 |

Then return; no request scan/triangle/counter mutation in this setup. The
shared buffers now hold the pre-depth current view/viewport/scissor and
replace class0's saved values. Fresh dswp profile0 likewise has N400,kind0.

## Class7 complete finite counter0 restore cursor

Class7 target8002db6c freshly reads dswp and calls
depthShadowEnd(camera2,thatOwner),8002db74. Ordered requests:

1. Capture passed owner; camGetCurPtr,8002b510.
2. Copy live saved8041e6e0→queried camera.view11c,8002b520.
3. GXSetColorUpdate(1),8002b528.
4. GXSetViewport(live shared saved sixfloats),8002b548.
5. GXSetScissor(live shared saved fourwords),8002b560.
6. Fresh passedOwner.word4,8002b564; exactly0 returns. Nonzero is an
   actual image/GX/depth-texture suffix and must hold until bound.
7. Return from shadowDisp after completed End.

Neither End calls a whole GPU reset nor restores projection/material/blend/
TEV/vertex states. Class7's color re-enable is its explicit extra state.
Auxiliary flags2/counter0 cannot replace any of the preceding requests with
success no-ops. These paths can complete faithfully in the finite0 domain
once shared buffers,mutable owner matrices/current camera queries and typed
GPU services are actually supplied.

## Continuation and evidence

Capture owner before a suspending invalidation/End call. Capture size only
at its original read; don't reread N on retry. Keep separate initial profile
read, math profile read and projection-publication profile read. Preserve
distinct current queries and copy direction. Publish each matrix/buffer mutation
once at its actual stage. End counter must be read after completed restoration,
so a provider mutation may legitimately reopen its nonzero suffix.

Source listings already freshly recovered under ignored
artifacts/reference/shadow-texture44/shadowDisp.txt,
shadow-class5/projShadowEnd.txt and
shadow-projection-prefix/depthShadowEnd.txt. Complete hashes:

| Body | SHA256 |
| --- | --- |
| shadowDisp/7b4 | ade549cabd0dcc3d9d69acec14ed8f09c77ff4b4c2e6e063a1c7ac1ce36819ad |
| projShadowEnd/294 | d5b52533c33a488880d068d80e68a2e658a770618bbd8a88a10d3328118735ca |
| depthShadowEnd/35c | 43ab8f516923e9765ddf4a9fd7c3aed3a00f84e0f21551f9b05b7b012eadde44 |

Documentation only; no code/shared index/progress changes or original/runtime/
hardware validation. Nonzero image suffixes and complete mutable-profile history
remain separate providers; no projection or saved-buffer snapshot isolation is
established by these cursors.
