# Native class state and captured shadow frontier

[Original dispatch](ORDINARY_BATTLE_DISPLAY_QUEUE_ADMISSION.md),
[shadow pool/cadence](BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md)
and [native frame adapter](../camera/NATIVE_FINISHING_CAMERA_GPU_ADAPTER.md)
govern this bounded GPU binding.

## Real known state and leaf updates

`NativeDisplayState` has no guessed startup state. The acknowledged original
no-write dummy seeds RGB/alpha/depth writes off, depth disabled/ALWAYS,
blendZERO/ONE, alphaGEQUAL128 and late location. Only actual DummyDraw seeds
this state; backend DummyStateRestore cannot erase later acknowledged class
settings. Each common/class leaf changes only its corresponding fields and
commits after an available native submit. Missing providers preserve prior
state. Restore re-emits the cached pipeline without source requests/geometry.

ColorUpdate controls RGB bits; AlphaUpdate controls alpha independently;
ZMode retains both while controlling depth enable/compare/write. Source
LEQUAL3 maps to PICA GEQUAL under reversed depth. ZScaleOffset(1,0) leaves
retained viewport range intact; other scale/offset domains remain unavailable.
GX blendNONE0 maps to ONE/ZERO replacement (raw factor/logic operands are
inactive); recovered blend1(4,5) maps to SRC_ALPHA/ONE_MINUS_SRC_ALPHA for
RGB and alpha. ALWAYS alpha and GEQUAL128 OR NEVER are admitted; unsupported
blend/compare/input domains are holds. Disabled depth with requested depth
write is outside this native admitted domain.

Source ZCompLoc0/1 remains retained. C3D_EarlyDepthTest has a separate
constant-reference comparator and is not its GX counterpart. Native early
depth remains disabled; ordinary native depth plus alpha preserve output for
recovered dispatch pairings: location1 with alphaALWAYS, location0 with
rejecting alpha. Intermediate setters may temporarily change that pairing
before the next setter, with no intervening draw. `rasterAdmitted` forbids
before-alpha rejection with enabled depth writes; future callback backends
must honor it rather than claim general early-depth equivalence. This assumes
ordinary shader output/no additional side-effect or stencil domain; complete
GX/PICA raster equivalence and unrelated writers remain unverified.

## Frame restoration and source shadow wait

Renderer first restores camera/dummy backend state, then latest display
pipeline. Scalar simulation has no GPU provider. Actual queue dispatch emits
common/class state inside the admitted renderer frame and captures callback/
borrowed userdata once. Failed callback resumes that capture without repeating
state. Existing independent world/overlay drawing does not satisfy it.

The finishing first camera1 shadow record is class9, not class0. Its seven
requests produce RGB+alpha writes, depth enabled/LEQUAL with depth-write0,
blendSRCALPHA/INVSRCALPHA, before-alpha location and alphaALWAYS. Native then
holds at the real shadowDisp8002d3d8 callback;21 records remain. No shadow
raster, queue clear or teardown is acknowledged. Host dispatch tests cover
classes0..10, including inherited6/7/8, independent masks, invalid domains,
unavailable submits, retained restoration and captured callback resumes.

## Fresh original callback inspection

Read-only ISO/static check of shadowDisp8002d3d8/7b4:
first dispGetCurWork80010864; cameras1/4/5/7 call
shadowCharShadowDisp_Polygon8002c818 before shared cylinder/texture state.
Polygon captures primary pool GPshadow+108, calls camGetCurPtr8000f2e4, then
GXSetNumChans(1)802b6f3c and material/channel/TEV/fog/blend/depth/descriptor
setup **before checking eligible records**. Empty type0 domains do not permit
skipping this callback. After Polygon, shadowDisp freshly queries the current
camera and performs its own setup/cylinder/texture paths. Ignored inspection
ELFs: artifacts/reference/display-class-check/shadowDisp.elf and
shadowPolygon.elf. Existing shadow contracts own remaining exact branches.

Native source state/frontier evidence and explicit test-only Bero fixture
scope are recorded in the progress archive. All original shadow geometry,
images, collision matrices, cylinder/texture/raster callbacks, remaining
camera/material owners and full field return stay open. No new assets or
original payloads are committed; no hardware or original-timing claim.
