# Native original US prologue heading

The used `sys` title0 display callback owns eight letters and the cached
`msg_stg_seq_prolouge` subtitle. The native provider consumes original
`e/us/effect_sys.tpl` backgrounds0..3 and glyphs19,20,21,22,21,23,24,25;
there is no static prologue card. Their original dimensions are160x160 and
64x64, with transparent power-of-two storage padding only.

Letter positions are `(32*i+16-128,50,0)`, initial Y angle90 and delay1+20*i.
An admitted display enters state1; source start request enters state2. On each
simulation tick, state1/2 advances background angle2.5; strictly greater than180
wraps to0 and advances the four-image palette. State2 decrements letter delays;
a strictly negative delay enters phase1 and records the original Stardust
mode4 request. Phase1 evaluates curve5 from90 to1800 over90 frames, including
both endpoints; after frame90 phase2 begins. The last letter's subsequent
phase2 ticks rotate the subtitle ring0.8 degrees and reveal48/256 alpha units
per tick, capped at0x4000. Sparse and dense tick delivery match.

The original background is one80-unit quad centered at(0,-40), rotating around
Y by90 plus the display angle. Glyphs use44-unit quads. The subtitle retains
its exact original global message ID/text; native original-font glyph masks
are captured once into a private512x32 VRAM target without CPU readback. Its
64-vertex strip uses source radii54/40, step11.612903594970703 degrees and
U step0.032258063554763794; positive source Z rotation subtracts from the
strip's sin/cos parameter. The final reveal includes the fractional vertex.
Camera eye(0,0,500), target(0,0,0), lens25 and centered304 crop adapt camera5.

Original curtain32 uses `a/OFF_d_maku_center` Z_1 (span0, one key).
Curtain35 uses A_1 (span60,121 keys); native evaluates the complete original
hierarchy/materials/vertex colors. Its1024x128 atlas alone is scaled to512x64
with Lanczos; geometry and pose keys remain intact. The renderer acknowledges
entry, A_1 completion, first state2 rendering and exit19/500 only after a
successful GPU frame and synchronization. Resource readiness never substitutes
for presentation. Missing resources stop the heading handshake. The producer's
3s+2s waits and post-start3s input gate remain in OpeningHeading; eligible
A/B/START mask0x1300 starts the real500ms black exit.

Limits: source glyph projective rainbow multi-TEV passes are not fully bound;
the native primary brightness fold is present. Existing original-font metrics
adapt the subtitle cache rather than reproducing SDKFontGetWidth/crop exactly.
The source Stardust mode4 requests now use the bounded original particle
consumer below; no CrystalN64 substitute is used. Original-runtime/Dolphin raster
comparison and physical hardware remain unverified. The isolated heading
fixture proves only this provider, not completion of the full opening.

Implementation: [display clock](../../../include/story/opening_heading_display.hpp),
[native provider](../../../source/display/render_opening_heading.cpp),
[asset exporter](../../../tools/opening_heading_assets.py).

## Recovered secondary Stardust mode4 evidence

Entry801eda2c, main801ed630 and display801ed208 confirm texture32 from the
original effect bank, not Crystal109/110. Four particles use random X/Y radii bounded by the source entry parameters. Entry owner lifetime32 increases by2 per particle, but particle lifetimes
use the separate r29=20/22/24/26. Initial velocityXY is0; accelerationY=-0.05.
The extra integer `(rand()%50)/1000` truncates to0. Each particle consumes four original rand calls, including the common entry
prologue radius draws. Mode4 main applies vx*=0.96, vy+=ay, position+=velocity,
lifetime--. Lifetime>20 or<0 hides the particle; visible ticks advance the
scale byte sequence60,90,100,104,106,108,109 multiplied by0.01, then relax
scale toward0.1 using `scale += 0.1*(0.1-scale)`.

Signed ADDI resolves the original eight N64 vertex arrays to803a91c8 (not
803b91c8), each56bytes with four signed-short XYZ/ST plusRGBA vertices. XYZ
uses±8 quads; fraction5 ST windows are22pixels wide across the176x22 strip.
Jump table803a9388 selects array indices3,4,5,6,7,0,1,2. Original palette
36bytes at80419c90 cycles through twelve source RGB colors, advancing3bytes
per drawn particle from `(ownerAge-1)*3`; scale bytes live at80419cbc. Startup
r2=80427440/r13=8041cf20 are established in
[original randomness](../meta/RANDOMNESS.md). Native presentation uses explicit seed1 with the original recurrence and
32-bit wrap. RadiusX=(rand%40000)/1000 and radiusY=(rand%8000)/1000; angular
position is `fma(90,i+1,rand%360)`, times source tau/360. The final rand draw
feeds the original truncated acceleration expression. Each display simulation
tick advances existing particles before spawning newly requested groups; the
first subsequent tick can draw. The owner-age color strip advances only for
visible particles, matching the source draw skip. Native TEV preserves the
source color equation `palette*(1-textureRGB)+textureRGB` and alpha textureA.
Original texture32 remains176x22 with transparent256x32 storage padding, no
image scaling, and the actual source signed-short quad geometry. Missing bank
or GPU texture admission prevents display admission; there is no unbound
request acknowledgment.

The explicit seed and effect-before-display60Hz cadence are provisional native
presentation bridges. Original global RNG continuity and display/effect-manager
queue ordering, numeric GPU and original/hardware raster parity remain unverified.
They do not alter story dialogue, input masks or event conditions.
