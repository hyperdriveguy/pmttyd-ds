# Prologue fence paper resource, capture rectangle and deformation

US G8ME01 disc0 revision0, read-only source ISO DOL/gor REL/resource inspection.
Scoped [OW-ROAD-01](../../requests/OW-ROAD-01.md) follow-up. The
[original fence command timeline](../story/PROFESSOR_FIRST_TALK_AND_FENCE.md)
retains source order; this document distinguishes serialized animation data,
screen capture services and native adapter boundaries. No original GPU capture,
hardware timing or full deformation/placement equivalence is claimed.

## Original asset and usable existing parsers

`a/OFF_d_saku` is3284bytes,SHA256
7ad577aed8faa957e972340db14e6221b604a7c241bb10286083b4b9a04c42bd.
It uses the same serialized character/model format as existing character tools:
animation-count at+148,directory relative offset at+1ac,64-byte descriptors,
animation header/key/update streams and24-channel transform nodes.
Original companion `a/OFF_d_saku-` exists, but this event overrides paper
texture with an offscreen capture of **S_saku04**. The companion alone does
not reproduce the photographed original fence appearance.

Bank order is Z_1,A_1,A_2. Exact source animation metadata:

| Name | Loop flag | Span | Key times | Updates |
| --- | --- | --- | --- | --- |
| Z_1 | 0 | 0..80 | 0 only | static pose |
| A_1 | 0 | 0..140 | 0,20,35,47,55,59,140 | 65 node updates; no position/normal/texture/visibility updates |
| A_2 | 0 | 0..140 | 0,20,35,47,55,59,140 | separate source resource; not selected by this scene |

Base positions are(-32,-24,0),(32,-24,0),(-32,24,0),(32,24,0).
There are3nodes/72channels. Left/right child pivots are(-32,24,0)/(32,24,0),
root initial transform zero translation/rotation, unit scale. A_1 animates the
transform-node hierarchy; inventing a flat quad rotation or gravity curve drops
the authored deformation. Preserve original vertices, shape draw binding,
node pivots, animation deltas and hierarchy.

Existing tools/character_animation_data.animation parses A_1 successfully;
evaluate reconstructs node samples and tools/character_bake.geometry composes
the original geometry hierarchy. These are host arithmetic adapters with
documented original rounding limitations. Existing animation pack/surface-mesh
formats can carry frame geometry and UVs; original capture texture substitution
is an additional service, not baked character atlas selection. Z_1 fails the
bounded parser's requirement that last key equal span end: it legitimately has
one static key despite end80. This needs an explicit static-resource adapter,
not forged endpoint keys or a claim that parser validation passed.

## Image/paper binding and completion clock

evt_paper_entry0x8006e3c4 requests animGroupBaseAsync(name,context,0),returns0
while unavailable, then animPaperPoseEntry. evt_img_set_paper0x80070ae8 binds
image+104 to animPaperPoseGetId(name,context),captures current animation clock
to image+110/+114, sets material flags0x1800. evt_img_set_paper_anim0x80070a54
stores original animation name at image+108 and recaptures that clock timestamp.

In imgDisp0x800721b4, after capture texture setup, if name nonnull request
animPoseSetAnim(pose,name,force0); restore saved start timestamp through
animPoseSetStartTime; call animPoseMain; request material flag2; read
animPoseGetLoopTimes into image+118. Texture setup requests animSetPaperTexObj
and animSetPaperTexMtx: image plane uses capture content, not a generated asset.

evt_img_wait_animend0x80070434 first call alwaysreturns0. Later it tests
image+118 against1.0,returns0 while<1 and2 otherwise. This is the actual
source completion publication, not a generic WAIT140frames event. The existing
[animation clock contract](ANIMATION_MAIN_ENTRY_AND_TIME.md) computes
frameTime=single(single(60*elapsedMilliseconds)/1000)*poseRate; nonloop ratio
is(frameTime-start)/(end-start). With ordinary rate1, A_1's140frame span
mathematically lasts2333.333ms and an integer-millisecond clock first reaches
it at2334ms. This derivation is conditional on clock/rate/draw admission,
not measured elapsed time. Image completion updates occur in imgDisp, so a
blocked/unadmitted draw can delay the waiting event. Do not fake completion
without producing the original animation state or explicit provisional status.

## Capture bounds and original aspect request

evt_offscreen_get_boundingbox0x800735cc invokes
offscreenGetBoundingBox_BeforeFrame0x80072bf8, not the similarly named current
frame getter. Unavailable box returns0 and retries. Available unsigned16 bounds
publish INTEGER event variables in order:left,top,(right-left),(bottom-top).
These are screen pixels, not world-space min/max or normalized model extents.

Fence caller receives LW0=left,LW1=top,LW2=width,LW3=height.
At REL0x49264 IF_LARGE(width,height), then:

```
if width > height: height = integer(width*48/64)
else:              height = integer(width*64/48)
```

Both branches derive HEIGHT from WIDTH. Preserve this literal original
arithmetic; do not repair it into a conventional fit-box algorithm. Capture
request is evt_img_alloc_capture(img,0,ofs,1,left,top,width,height).
The wrapper0x80070760 resolves eight operands and links the offscreen capture;
capture/texture allocation details require their actual GX services. Screen
bounds can become invalid after aspect adjustment; original admission must
retain its own branch checks, not force a guessed world rectangle.

Caller clears virtual points, sets Z_1, image on, shadow2, hides source
S_saku04 flag1, then selects A_1 and waits completion. Cleanup hides image,
clears source offscreen binding, releases image/offscreen and disables
collision A_saku_04. The prior shadow2/material flags remain real source
requests; native omissions must remain explicit.

## Bounded image placement matrix request

The ordinary capture-rectangle branch imgDisp0x80072650..0x80072828 reads
left/top/width/height and image offsetXe8/offsetYec. With framebuffer dimensions
W/H, depthD supplied by earlier camera/plane logic, source requests:

```
M = T(64*(left+offsetX)/W, -48*(top+offsetY)/H, D)
    * T(-32,24,0)
    * S(width/W,height/H,width/W)
    * T(32,-24,0)
M = ImageBaseMatrix(+120) * M
```

This is the left-concat request order, with source single-operation rounding.
Constants64/48/-32/+24/+32/-24 are binary32 at0x80420378..0x8042038c.
It maps the authored top-left(-32,24) into capture screen coordinates of
the virtual64×48 image plane. Source draw calls pass1,2,3 with incoming angle0
and the retained source scale argument; model vertices must remain3D while
the deformation turns/folds the plane. Actor model scalar2 and map*.01 world
adaptation are not automatic substitutes for this image plane mapping.

DepthD, cameraFOV-derived outer scale and the virtual-point/projection route
precede this matrix and are still dependencies, not fully closed by the bounded
rectangle block. Do not claim the formula establishes complete world placement,
shadow/material fidelity or source projection. For native adaptation, retain
captured camera/viewport/rectangle and original animated mesh, then document
the screen-to-native projection service separately.

## Reproduction and remaining requests

Source ISO asset read directly into/tmp/OFF_d_saku; animation descriptor/span,
keys/base vertices/nodes inspected using existing generic parser. DOL bodies
read with tools/inspect_dol_function.py and external US symbols:
evt_paper_entry,evt_img_set_paper,evt_img_set_paper_anim,evt_img_wait_animend,
evt_offscreen_get_boundingbox,evt_img_alloc_capture,imgMain,imgDisp.
Local reports stay ignored; no original payload/native source edits are added.

Next source owners needed:full offscreen capture/GX bounds generation,
animSetPaperTexObj/TexMtx texture substitution, image virtual-point/depth/FOV
matrix branches, paper rendering/material/shadow and original runtime scene
comparison. Existing host resource parser/native mesh infrastructure offers
an implementation route; it does not prove those omitted services are faithful.
