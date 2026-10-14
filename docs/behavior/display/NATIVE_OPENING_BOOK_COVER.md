# Native opening book cover

Source: original US `rel/dmo.rel` `demo_book` at REL0x938..0x1238;
`camInit` camera8 setup at DOL80010514..800105a4. The read-only verifier
`tools/verify_opening_book_cover.py` verifies the complete source book body,
existing narrative/capture requests, and camera initialization/constant hashes.
No original executable or extracted texture bytes are retained in history.

The book uses procedural GX quads, rather than an animation/model bank. Original
`demo.tpl` indices1 and2 are the 288x352 front and inner cover. Local points are
(0,+h/2,0), (w,+h/2,0), (w,-h/2,0), (0,-h/2,0). Source matrix order is camera8
view × translation(work+18,work+1c,0) × scale(work+14) ×
translation(-w/2+8,0,0). Rotating faces additionally multiply a negative Y
rotation from work+4, using source degrees-to-radians constant0x3c8efa35.
The native adapter consumes the existing simulation-clock scale and angle;
it does not advance the narration or introduce rendering-frame timers.

Three cover passes preserve source winding/material intent:

| Quad | TPL | UV | Source cull | Blend/alpha |
| --- | --- | --- | --- | --- |
| Stationary inner cover | 2 | mirrored U | 2/back | source alpha; trunc255×(1-work+10) |
| Rotating front | 1 | normal | 2/back | opaque |
| Rotating inner cover | 2 | mirrored U | 1/front | opaque |

Source disables depth testing and uses white vertex color. Native CPU admission
uses projected winding to select these faces, then disables hardware culling;
this adapts the source winding to the tilted 3DS target without claiming GX
state or SDK bit parity. Native cover vertices retain XYZ and hardware
perspective texture interpolation. Cover and captured-paper draws append
separate vertex ranges to the same buffer; resetting between queued draws would
allow later paper vertices to overwrite cover data before the GPU reads it.

Camera8 initially uses a 25-degree perspective, aspect608/480, near100/far10000,
and eyeZ240/tan(double0.21816612780094147), looking at the origin. Native uses
that initial lens/distance with the full400x240 top-screen aspect and scissor.
The capture composition retains half-size source geometry. The visible screen
then fills the original settled cinematic interval Y49..436, as described below;
this retains uniform geometry scale instead of treating the hidden bars as page. The desktop raster fills the screen at
its original aspect, cropping above/below. Later camera mutations and original
SDK/Gekko numeric parity remain open.
The native framing now pans horizontally as the cover opens: half-size
`(288/2-8)*scale*clamp(rotation/180,0,1)` pixels. At scale1.5/rotation180 this
is102 pixels, placing the source hinge at screen x200 rather than x98. Before the cinematic-crop stage this gives each inside cover200 visible pixels
instead of clipping the green left cover to98. This is an explicit native viewport adaptation, not a recovered
original camera mutation or a change to source cover dimensions/UVs. The closed cover retains its pan-free position within the cinematic viewport. Final adjustment scales this same pan with
the book and multiplies it by1-fade, returning focus to the illustrated right
page while the cover and captured paper stay registered.
Final `adjust` snapshot ownership and native binding limits are described below.

The native page-base consumer retains source .95×coverwidth and .95×coverheight,
selecting original TPL3..7. A bounded local-only exporter retains the original
480x608 parchment/rune backgrounds at240x304 with256x512 padding. `demo_img`
selects original TPL8+page and draws (-101.3333359,+115,0),
(+101.3333359,+115,0), (+101.3333359,-45,0), (-101.3333359,-45,0), normalUV.
Its matrix is camera8 view × translation(work+18,work+1c,0) × scale(work+14),
without the cover hinge. It uses source alpha blending, cull2 and white color
with trunc255×(1-work+10) alpha. Draw order is stationary cover2 → page3..7 →
illustration8..12 → rotating front1 → rotating inner2. The stationary base is
always retained. The desktop TPL0 remains the ordinary background.

At narration24 completion, source makes marketplace actors/map visible and
requests BGMoff with6000ms fade, then linearly interpolates integer0..1000 over
3000ms, divides1000 and calls `adjust` each simulation tick. The portable book
retains fade=q, scale=1.5+1.5×sin(float(pi/2)×q), offsetY=-105×sin(float(pi/2)×q).
It exposes img4 matrix T(offsetX/10,offsetY/10,0) × S(scale/1.5) separately,
retains the final paper through this adjustment, and releases it at180ticks.
The existing marketplace request program owns the subsequent1000ms wait; no
second delay is introduced. Completion of this source clock does not acknowledge
framebuffer capture or a page animation. Source hashes and exact call operands
are covered by the read-only verifier.

The native renderer captures the old composition directly into a256x256 VRAM
render texture. It repeats desktop, original cover/page/illustration and retained
older paper draws with an untilted projection cropped to the same native
rectangle212/0/193/240 after the102-pixel opening pan (source124/0/386/480).
The capture can include the five pixels beyond the visible right edge; the
render target still contains the full source crop for subsequent deformation.
There is no CPU framebuffer
readback, rotation or Morton repack. The temporary render target owns its depth
buffer; deleting it after FrameEnd waits for GPU completion, then publishes
captureComplete. The page retains its texture independently until book cleanup.
Original rune backs remain separate. Source clip-space adaptation is
`x'=400/256*x-280/256*w`, `y'=240/256*y+16/256*w`, applied after the pan.
Host tests compare this crop with the ordinary screen projection, including
perspective cover rotations and homogeneous w. See
[citro3d render-target lifecycle](https://github.com/devkitPro/citro3d/blob/master/source/renderqueue.c).

The turning paper consumes the original capture-rectangle placement before the
ImageBaseMatrix's global T(offsetX/10,offsetY/10)*S(scale/1.5). Final scaling is
about the virtual image origin, not the crop's local hinge; offsets are outside
the crop scale. The native opening pan is applied to both cover and paper.
Before this framing change, an isolated frozen-frame0 native test compared the live page and
its captured replacement: every crop pixel differs by at most2 RGB levels,
mean absolute channel error0.0003742. This is an Azahar comparison of native
paths, not an original-runtime or hardware observation.

Source camLetterBox8000e388 caps the settled top/bottom bars at49/44; its draw
submits the documented source quads. The visible source interval is consequently
49..436, height387. Original Dolphin reference-t110 confirms the picture's dark
border at full-frame Y67, inside the visible region beginning49. The native
screen fills this cinematic crop with uniform zoom480/387, mapping the previous
half-size coordinates through X=200+(x-200)*zoom and
Y=(y-49/2)*zoom. The tilt projection uses the equivalent clip-row scale plus
Y offset5/387*w. The settled source picture border maps to11.1628 native pixels;
Azahar now renders it at11, versus34 before this correction. Quantitative
inspection stays in artifacts/book-vertical-comparison.json. Original artwork
and its runes receive this same transform, with no per-texture vertical offset.

Filling this crop would overflow the right illustration edge after the opening
pan. The native fit subtracts the source illustration-edge overflow, weighted
by opening angle, scale and1-fade; this keeps the picture in view while retaining
about179 visible pixels of green cover. Capture targets keep their193x240
half-size composition, including older paper, and the visible cover/paper apply
the same outer crop transform afterward. This avoids extra VRAM and prevents
capture insertion from shifting the picture/runes. The independently documented
left-side width adapter below is also inside this uniform outer transform.
The bar bounds are source facts; viewport filling, opening pan and picture fit
are explicit 3DS adaptations. Later camera/TEV/Gekko parity is not established.
See [source letterbox producer/draw](../camera/CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md).

The native left-side width adapter treats the capture hinge x=-32 as a
continuous boundary. On its right, the captured front retains193/64 pixels per
authored X unit, preserving the flat capture's registration. On its left, X
uses the same240/48=5 pixels per unit as Y, preserving the turned back's portrait
proportions instead of applying the front crop compression again. Source A_1
frame180 has X bounds[-71.9395,-32] and Y[-24.0625,25.8125]; the native turned
width consequently becomes199.6975 pixels, versus the former120.4426, with the
same249.375-pixel height. Placement stays continuous at the hinge, including
final image scaling, pan and fade. Original mesh samples, UVs, textures and
180-tick completion are unchanged. This piecewise native layout is an explicit
adapter, not recovered original camera/image projection or original raster parity.

During final adjustment, the market renders beneath desktop/cover/page layers;
source fade alpha and final img4 scale/translation consume the portable snapshot.
The original BGM6000ms request and subsequent marketplace start are consumed by
two fixed native PCM stream owners; see the source-verified portable BGM clock
and tests. These are native adapters, not proof of original GX/SDK numeric or
raster parity. Later camera mutations, original paper outer projection,
original audio scheduling and physical hardware comparison remain open.
