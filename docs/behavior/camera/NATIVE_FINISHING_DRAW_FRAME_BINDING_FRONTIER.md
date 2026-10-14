# Native finishing draw frame binding frontier

Current-source inspection 2026-10-14. Original request ordering remains governed
by [camera draw](CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md),
[letterbox/dummy](CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md) and
[display dispatch](../display/ORDINARY_BATTLE_DISPLAY_QUEUE_ADMISSION.md).
This is an implementation path, not a claim of native raster equivalence.

## Frame admission point

Renderer::render in source/display/render.cpp opens C3D_FrameBegin, clears
top, then calls C3D_FrameDrawOn(top) before world(g). GPU providers must be
called after successful FrameBegin/FrameDrawOn, before C2D_Prepare/SceneBegin
for overlay and before bottom target selection. TrioAttackRuntime currently
continues Draw from its simulation tick and supplies only letterbox; emitting
GPU commands there would be outside this admitted frame.

Add a renderer-facing continuation entry that accepts CameraDrawServices and
resumes the same retained CameraDrawCursor, DisplayDispatchCursor and queue.
Simulation can complete scalar letterbox and wait. Renderer can then resume
the pending pass with its frame-scoped provider; it must never reset the
cursor, append another letterbox record or re-sort on this handoff. The current
render argument is const Game&, so prefer an explicit mutable runtime/service
entry supplied by the owner over casting away const. Passing providers only
through the existing simulation tick is insufficient.

If a service holds between frames, the next frame must restore already
established backend camera state before resuming its cursor. FrameDrawOn and
world/overlay drawing can overwrite backend state while the source cursor
correctly declines to replay previous source requests. Maintain a retained
backend state cache with a frame epoch and reapply its state at frame admission;
this is backend restoration, not duplicate source callback execution. Until
real callbacks are bound, the existing world renderer can continue separately
but cannot certify queued drawing or permit the original queue to clear.

## Installed callable library mappings

Installed declarations are /opt/devkitpro/libctru/include/c3d/base.h,
effect.h, maths.h, uniforms.h and renderqueue.h. Top is physically240x400
(C3D_RenderTargetCreate(240,400,...)), displayed logically400x240.

| Original request | Native call/boundary |
| --- | --- |
| Viewport x,y,w,h,minZ,maxZ | C3D_SetViewport(u32 x,y,w,h) plus retained depth-range mapping through C3D_DepthMap; the four-argument viewport call alone loses minZ/maxZ |
| Scissor x,y,w,h | C3D_SetScissor(GPU_SCISSOR_NORMAL,left,top,right,bottom), with rotated target coordinates and extent-to-edge conversion |
| Projection | Adapt the retained original matrix to PICA clip/depth conventions and screen tilt, then C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,uniform,&matrix) for the bound shader |
| Ordinary finite lens numeric adapter | Mtx_PerspTilt/Mtx_OrthoTilt available, but rebuilding from lens scalars discards original extra-projection matrix changes |
| Queue blend/depth/alpha states | C3D_AlphaBlend, C3D_DepthTest, C3D_AlphaTest and C3D_DepthMap using explicit GX-to-PICA enum/depth conversion |
| Dummy triangle | Real shader/attribute/no-write state plus C3D_ImmDrawBegin/ImmSendAttrib/ImmDrawEnd or a bound vertex buffer; a success no-op is invalid |

The source bank is initialized608x480. The declared full-screen adapter scales
logical x/width by400/608 and y/height by240/480. Full-source viewport/scissor
therefore maps exactly to native physical(0,0,240,400). For subrectangles,
scale edges before quantization, rotate through the same tilt convention as
the shader, then form physical bounds; do not pass logical400x240 directly
to C3D_SetViewport. Rounding, fractional/out-of-range GX viewport values,
scissor edge inclusivity and rotation direction need explicit adapter tests.
No checked native subrectangle mapping or original fractional raster parity
is established here. Existing citro2d base source at
artifacts/reference/citro2d-base.c swaps scene dimensions when tilted;
installed Mtx_*Tilt declarations explicitly account for screen rotation.

## Actual next waits and callbacks

CameraManager::draw resumes stage3 viewport, stage4 scissor, captured stage5
near/far gate, optional stage6 recomputation, stage7 projection, stage8 queue
dispatch, stage9 DummyDraw. Only flag1 skips a camera. Current camera0 is
enabled and has no known finishing record, so after viewport/scissor/projection
its next real wait is DummyDraw: first GXLoadPosMtxImm(view,0)80005ad8 helper
request, before any later camera callback. Binding viewport alone cannot
reach battle part drawing. Dummy must complete its documented no-write
triangle/state before camera1 can be admitted.

Registered source callback identities retained by this finishing path include:

- Shadow8002d3d8 on cameras1/2/4/5/7 (classes0/2/5/7/9 by source tuple).
- Battle part80101774 on camera4, borrowed part address and class1/2.
- Stage80138da8/801389a0 on camera4,class2, priorities5000/6000.
- Audience801a38f8 twice and801a37f0 on camera4;801a36c8 and conditional
  801a35d0 on camera8.
- Command main8011fde0 plus flag-selected8012210c/80121ce8/80121638/
  80121074/80121330/80120a44/80120844 on camera8,class0, borrowed BattleWork.
- Stock EXP80100258 on camera8,class1 if original ef8 bit2 admitted;
  break-slot802036d4 on camera8,class0; letterbox8000e564 on its live drawCamera.

Actual order follows retained sorted keys and original tie permutation, not
this inventory. Dispatch emits three common states and up to four class states
before capturing callback/userdata. A held callback must resume that same
identity/borrowed address without replaying states or selecting another record.
Native world(g) independently drawing actors/maps does not satisfy these
callbacks. Preserve true holds until their material/pose/shadow owners exist;
queue reinitialization and outer Main advancement belong only after all thirteen
admitted cameras, callbacks and dummy draws complete.

Documentation only. No code/index/progress edit, build or runtime validation.
