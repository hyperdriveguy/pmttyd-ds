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
that initial lens/distance, with the 400x240 target aspect and a content scissor
x48..352. Source 608x480 image dimensions therefore fit the native304x240
content region at half size. Later camera mutations and original SDK/Gekko numeric parity remain open.
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

The native renderer now captures the completed top-screen composition after
FrameEnd/FrameSync. A same-format RGBA8 display transfer detiles the sideways
240x400 target; a bounded CPU adapter rotates and crops the original386x480
rectangle124/0 to193x240 at native110/0, pads edges and writes a256x256 Morton
texture. The actual copy/flush acknowledges capture; failed old-composition
drawing cannot acknowledge it. Four independent textures retain the source
page owners. Original rune backs remain separate. Host tests independently
cover rotation, crop, every tile address, padding and malformed buffers.

During final adjustment, the market renders beneath desktop/cover/page layers;
source fade alpha and final img4 scale/translation consume the portable snapshot.
The original BGM6000ms request and subsequent marketplace start are consumed by
two fixed native PCM stream owners; see the source-verified portable BGM clock
and tests. These are native adapters, not proof of original GX/SDK numeric or
raster parity. Later camera mutations, original paper outer projection,
original audio scheduling and physical hardware comparison remain open.
