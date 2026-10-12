# Original ordinary field framebuffer alpha, depth and shadow ordering

US G8ME01 revision0. Source-only control/state reconstruction for the normal
non-AA field render path, not a captured EFB or PICA200 numerical comparison.
This supplies the entering-state dependency for
[the three-pass air-shadow graph](OVERWORLD_MARIO_AIR_SHADOW_GX_GRAPH.md).

## Bounded result

For the normal authored Mario non-AA render modes, ordinary field camera4,
completed normal frame-copy/clear, default camera2 shadow passes, background
camera3 and ordinary DOL map/actor callbacks: **entering camera4's shadow
stage9 retains alpha0**, scene depth contains preceding opaque scene draws,
and pixel format is **GX enum1=RGBA6/Z24**. Camera4 scene callbacks normally
write RGB with alpha writes disabled. Shadow cylinder changes that write mask
explicitly; its per-record changes persist until the next source callback reset.

This is an inference from the literal clear/write/copy/callback graph below,
not original-runtime EFB observation. It excludes title/DVD-error/debug/battle
rendering, custom native callbacks/alpha writers, AA mode overrides and
unrecovered external render overrides. It does not say alpha is0 before each
individual shadow record: earlier type2 records in the same callback can
change destination alpha, and the source does not clear it between records.

## Initialization, format and normal frame clear

marioStInit8006fb3c selects original GXNtsc480IntDfMarioSt8030c2a8 or
GXNtsc480ProgMarioSt8030c2e4. Both60-byte authored mode records have **AA
byte19=0**. DEMOInit8002efac–cc chooses PixelFmt(2,0) if AA!=0, otherwise
**PixelFmt(1,0)**. The latter is RGBA6/Z24 with linear depth compression.
Do not substitute the decomp's provisional RGB8 format comment for the
shipped literal1. RGBA8 is a native adaptation: original EFB RGB and alpha
storage have6-bit component precision, depth24. This contract does not derive
exact GX quantization/blend rounding from those format labels.

marioStInit calls GXSetCopyClear(localRGBA0,**0xFFFFFF**) at8006fbbc–d0.
The zero color word comes from cold-zero sbss2 address80429568 (r2+2128).
DEMOInit likewise uses cold-zero80429548 (r2+2108), sets the same depth clear,
then GXCopyDisp(buffer,**clear1**) at8002efd0–eff0. Startup __init_data's
BSS range80429500..8042990c covers both words. These are zero RGBA inputs;
background sky RGB/native clear RGB must not be mistaken for their alpha.
SDK initialization's only direct GXSetDstAlpha call at802b3208 has arguments
(0,0): destination-alpha override is disabled, rather than forcing alpha255.

Main800055e0 runs initialization, then each loop:
DEMOBeforeRender →marioStDisp/camDraw →marioStMain →DEMODoneRender
(80005600–1c). Thus the completed frame's copy-clear supplies the next frame's
initial EFB. DEMODoneRender enables color writes, depth writes and alpha writes,
then GXCopyDisp(buffer,clear1) at8002eb80–c0. marioStInit enables hang-workaround5;
its normal __NoHangDoneRender path likewise enables AlphaUpdate1 and calls
GXCopyDisp(clear1) at8002e578–5b8. Its recovery reinitialization selects the
same non-AA format1 and copy-clear0; recovery/error execution remains outside
this ordinary frame inference. These are actual source clear requests, not
CPU clearing a NativeVertex color or an invented per-render alpha reset.

## Cameras and offscreen shadow copy-clears

camDraw8000f454 sorts entries then visits camera IDs **0..12 ascending**;
flag1 suppresses a camera. It installs each admitted camera's viewport/scissor,
projection, calls dispDraw(cameraId), then sysDummyDraw(camera.view)
(8000f478–524). Camera2 therefore precedes background3 and field4.
Camera0/title and camera1/battle activation are outside ordinary field display.

In camera2's default shadow pass, dispDraw enables alpha writes at callback
entry. The source shadow offscreen stages prepare/draw their retained regions:
projShadowEnd8002b834 requests ZMode(1,3,1), then **GXCopyTex(destination,
clear1)** at8002b968–984; depthShadowEnd8002b4d8 does the same at8002b60c–628.
These copy-clears clear the rendered offscreen footprints with the retained
zero copy-clear RGBA/depth. Their ordinary containing paths do not issue
GXSetAlphaUpdate0; camera2 callback admission supplied AlphaUpdate1. Their
viewport/scissor/copy rectangles and projection state remain in the pinned
containing bodies, rather than a guessed full-screen texture copy. If a pass
produces no draw, its no-draw admission does not create an alpha footprint.

Combined with the preceding full-frame zero clear, the bounded normal
camera2→3→4 path preserves/restores zero alpha before the visible field.
This is not a license to retain arbitrary alpha left by a native offscreen
render target that does not execute the original copy-clears.

bgDisp80009d98 explicitly calls AlphaUpdate0 at80009de8 and disables depth
writes. sysDummyDraw80005ad8, called after each camera, disables both color
and alpha writes and depth writes (80005bb4–c0 and preceding ZMode).
Neither turns the background's visible RGB or dummy draw into alpha255.

## Per-callback scene ownership and stage order

Before **every matching callback**, dispDraw80010940–78 requests
ColorUpdate1. It requests AlphaUpdate0 for camera4/5/7 and AlphaUpdate1 for
other cameras (camera6 explicitly takes the true branch). This happens before
its stage-specific blend/depth/alpha-test defaults and before calling the
callback pointer. A source callback can override the mask; otherwise normal
map/Mario/NPC rendering inherits RGB-only scene writes. Actor texture alpha
still participates in alpha testing/blending; disabling destination-alpha
writes does not make its sampled/source alpha opaque.

A complete direct-BL census of executable DOL sections finds18 AlphaUpdate
sites: dispDraw's two branches; bgDisp/sysDummyDraw/zFill;
cylinderShadowDraw's three changes; image projected-plane handling; battle
spotlight handling; window depth clearing; DEMO render finish/SDK init and
system error paths. **No map/Mario/NPC actor material body directly calls
GXSetAlphaUpdate.** The emitted graph/body identities are source evidence,
not a claim that all arbitrary function-pointer targets or hardware-register
writes have been modeled. The shipped rel/gor.rel import census has no
GXSetAlphaUpdate/PixelFmt/DstAlpha/CopyClear/CopyDisp import of any relocation
kind, including direct REL24 calls and imported function pointers.

The original display stage offset table80414fd8 has:

| Stage | Sort base | Normal scene role |
| --- | --- | --- |
| 0 /1 | 400000 | Opaque/depth-writing scene requests |
| 9 | 500000 | Visible shadow callback (order0) |
| 2 | 600000 | Translucent scene requests |

Sorting is ascending. dispEntry subtracts supplied order for stages0/1, adds
it for stages9/2. Ordinary actor depth orders are bounded by their recovered
projection owner. Thus source stage9 sits **after opaque scene draws and
before translucent scene draws**. It is not the last overlay after all actor
blending. Stage9's default depth state is LEQUAL/no write, and its shadow
callback also uses no depth writes. Do not clear scene depth before the air
mask: the three-pass volume deliberately compares against preceding opaque
scene depth. Later stage2 resets the camera4 alpha mask to off again.

## Backend contract and open precision boundary

A justified native ordinary adapter can clear destination alpha0 at frame
start, preserve that alpha through background/map/actor RGB draws, retain
opaque scene depth, run the exact source stage9 ground/air requests, then
continue translucent RGB-only draws. It must preserve per-record shadow
ordering and restore/reset write masks at actual source callback boundaries.
The existing native clearRGBA(...,255) plus sceneRGBA writes is not equivalent
to this bounded source input state; source alpha0 is an explicit dependency.

RGBA8/PICA200 OR/destination-alpha blending remain adaptations unless original
RGBA6 quantization, shader/alpha-mask/logic precision, sampling and blend
rounding are established. This contract closes source entering-state and
request ownership, not those numerical details or original image fidelity.
Custom callbacks, AA/external mode writers, overlays, active camera1 battle,
special render targets and missing field scene services need their own audits.

Verify `python3 tools/verify_overworld_field_framebuffer_alpha.py`:17 bodies,
five data spans,110 selected anchors; both complete executable DOL section
identities/direct-call census; original gor REL identity/import audit;12 bounded
format/clear/write/order cases. Direct call totals: PixelFmt7,DstAlpha1,
CopyClear5,AlphaUpdate18,CopyDisp6. Missing indirect-register/hardware traces
are stated explicitly; no assets, scene renders or framebuffer data are committed.
