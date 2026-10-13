# Original demo book providers and captured page deformation

This bounded adapter owns the initial dmo book phase separately from the
[marketplace controller](NATIVE_OPENING_FLOW.md). Static US disc0 revision0
source inspection supplies the requests below; no Dolphin or hardware timing
observation is claimed. Book layout/projection, camera setup and GX image
material/capture services remain additional native bindings.

## Clock and message ownership

Original `demo_evt` starts a narration brother at REL0x2418. That brother waits
4000ms before op_book_01/02. The root calls `demo_scale` at0x24ac concurrently,
then waits1000ms before op_book_03. `demo_scale` (REL0x388,size0x114,SHA256
b6dbf358ff3734debf4de2343c33724ff85f24449c92d14fa165def8b20d7b4c)
uses integer elapsed milliseconds from the event clock, computes

```
q = elapsedMilliseconds / 12000
scale = (600*(1-q) + 1500*q) / 1000
```

and clamps at1.5. It returns0 before the clamp and2 after it. Native
`OpeningBookScene` adapts elapsed simulation ticks to integer milliseconds;
scale advances independently of message playback and op_book_03 is not admitted
before boot+13000ms. The adapter retains sequential message ownership rather
than claiming complete execution of competing original message events. Source
`demo_init` initially sets scale1; the immediately admitted scale service writes
0.6. A source-callback entry/first-render trace is still a separate observation.

After op_book_04, `next` selects the first story page and source calls
`evt_sub_intpl_msec_init(11,0,1800,2000)` at0x258c. The loop divides LW0 by10
at0x25b4 and passes it to `book_r`0x25c0, producing angle0→180. The adapter
uses the existing recovered curve11 half-cosine kernel over120 simulation ticks
and retains the following1000ms wait. It does not invent a linear book-opening
curve. Complete source book cover/matrix/GX draws still need their presentation
providers; angle/scale publication alone is not a rendered original book.

## Four source captured papers

After op_book_08/12/16/21, source requests img1/2/3/4, binds
`OFF_d_meku_right`, clears virtual points and captures `(124,0,386,480)`.
Each paper then runs `A_1` and waits `evt_img_wait_animend`, followed by1000ms.
These are four three-second authored deformations, not merely one-second waits.
Original image depths are6/5/4/3. Source `next` advances background8→9→10→11→12;
the respective old captures use8/9/10/11. The preceding image is released only
after the following paper completes. Last-image cleanup occurs before market
handoff; complete source fade/capture scheduling remains separate.

`OpeningDemo.paperServicesBound` explicitly registers a native paper renderer
only after original assets load. Registered mode retains the old background
until `captureComplete(index)` publishes capture admission; then it publishes
the next background. `pageFrame(index)` follows simulation time capped180.
`presented(index,frame)` requires admitted capture, elapsed180ticks and a finite
actually evaluated frame>=180. Only then does the dialogue continuation gain
its1000ms wait. Missing/blocked draws cannot silently satisfy the page wait.
The completed paper remains visible until the next paper's completion retires
it. Skipping A/START remains owned by the demo title transition. Unregistered
mode preserves the existing provisional static-page adapter for builds without
a paper binding; it does not establish source animation completion.

## Original resource, geometry and UV ownership

`a/OFF_d_meku_right` is245932bytes,SHA256
096736b5a0277b4334d8f2c32038562f9c8dcd2ed9d925cff0aa03f978fbbfd7.
A_1 is nonlooping,span0..180,181 keys,14748 position and11396 normal updates.
The retained original hierarchy produces372 vertices/124 triangles. Each source
triangle is pass2/cull1. There are62 ordinary-backtexture triangles and62
capture-kind1 triangles. The ordinary back owns original companion TPL index0;
capture-kind1 uses a framebuffer crop, not its fallback TPL image. The generic
PMANIM4 converter still rejects unsupported texturekind1; this adapter exports
PMANIM3 plus explicit source material metadata rather than weakening that guard.

PMANIM3 uses distinct cell0 for the original back and cell1 for capture. Undo
its atlas inset to recover original UV:

```
u = (packedU*1024 - cell*128 - 2) / 124
v = ((1-packedV)*1024 - 2) / 124
```

Both cells occupy row0. Original geometry, not a guessed flat page rotation,
produces the fold: frame90 has peakZ13.5; frame180 bounds are approximately
X[-71.939518,-32],Y[-24.0625,25.8125],Z[-2.4375,6.0625]. These are decoded
resource results under host math, not original renderer measurements.

The source viewport is608×480. `demo_book` obtains camera8 at REL0x96c,
passes camera+15c to GXSetProjection at0x97c, uses camera+11c in its matrix
composition and sets viewport608×480 at0x9a8. Paper image outer camera/FOV/depth
placement is not recovered by merely using the book's camera pointer.
The bounded original image capture matrix is documented in
[paper capture and animation](../display/PROLOGUE_FENCE_PAPER_CAPTURE_AND_ANIMATION.md).
A native orthographic image-plane projection may adapt original XYZ to the
304×240 content viewport; it must be labeled as an adapter, retaining source
geometry, capture rectangle, source UV/back art, visibility and completion.
It does not prove original GX outer perspective, source shadows/material state
or physical3DS performance.

## Packs and validation

Ignored `demo-page.pmanim` retains original A_1 sparse geometry/hierarchy.
`demo-page.bin` is magicPMDPG001,u32 triangleCount124,then124 records of
u32 capture-kind/pass/cull; record order matches consecutive triples in the
pose bindings/evaluated vertices. Original rune back texture is
`demo-page-back.t3x`, with `demo-page-back.size`240/304/256/512 after half-size
Lanczos conversion and transparent power-of-two padding. No geometry, original
script bytes, rune art or proprietary assets enter source history.

Portable tests cover independent scale/third-message timing, half-cosine angle,
capture admission, refusal of completion before simulation/frame admission,
retained previous image and retirement. Original-pack tests validate372 vertices,
181 keys, frame90 deformation, atlas UV inverse and source62/62 material ownership.
`verify_opening_book_requests.py` checks the392-command root/provider hashes and
53 source book/capture/constant anchors read-only. Native capture, rendering,
final-frame publication and full integrated build/smoke must be checked separately.

## Native renderer binding

The frontend now admits the original PMANIM3 page and material companion before
registering paper services. It admits each original old-background texture before
captureComplete, draws the new background underneath, and renders the source
A_1 deformation with captured-front and original rune-back textures. Source
pass2 and cull1 are retained. The simulation clock selects the sample; rendering
does not increment its frame. Only a submitted final180 sample publishes the
completion acknowledgement after C3D_FrameEnd. Static final samples are cached
per page; source earlier-page retirement remains owned by the book controller.

The native outer projection is explicitly orthographic, using source mesh XYZ
and capture rectangle124/0/386/480 on the source608x480 viewport:

```
X = 48 + (124 + (x + 32) * 386 / 64) / 2
Y = (24 - y) * 5
Z = depth - 16 - z * 386 / 128
```

The depth reference16 and inverted native depth axis place the paper layer above
the native2D underlay while retaining the source depth ordering and deformed3D
geometry. This is a renderer adapter, not the recovered outer camera/FOV/image
base projection. Captured-front UVs sample native old-background pixels62..255
from a512x256 texture; rune-back UVs use the240x304 content of256x512 padding.
The source book-cover opening rotation remains a request with unbound cover
geometry. Original fade, outer projection and GPU capture fidelity remain open.

Isolated Azahar first-turn captures in azahar-original-paper-depth show the
original front bending, the rune back, and subsequent narration after the
final draw acknowledgement. Earlier azahar-original-demo-paper completed the
unskipped37-message route but exposed a native depth-layering bug; it is retained
as regression evidence and is not proof of visible page rendering.
