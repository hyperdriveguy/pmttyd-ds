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
The source Stardust mode4 requests are retained but their particle renderer is
unbound; no CrystalN64 substitute is used. Original-runtime/Dolphin raster
comparison and physical hardware remain unverified. The isolated heading
fixture proves only this provider, not completion of the full opening.

Implementation: [display clock](../../../include/opening_heading_display.hpp),
[native provider](../../../source/render_opening_heading.cpp),
[asset exporter](../../../tools/opening_heading_assets.py).
