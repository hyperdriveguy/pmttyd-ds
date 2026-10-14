# Native class-menu atlas, quad and GPU backend

2026-10-14 native presentation increment. This binds a finite request backend
for [original circle/bubble consumers](../battle/CLASS_CIRCLE_AND_SPEECH_BUBBLE_DRAW.md),
not the tutorial scheduler or original complete GX/font/camera implementation.
Normal Game class-menu activation and once-per-tick ring publication remain
open. No visible menu or hardware/Dolphin raster parity is claimed from builds.

BattleClassAssets atomically reads the private PMBCLS01 surface catalog and
PMICON1 dedicated class catalog. It requires the original two surface sizes,
IDs334..343 in order, static one-frame48x48 icon descriptors, finite bounded UVs
and EOF. Reload failure preserves the prior valid catalogs. Loading this sheet
does not replace the global message icon registry. TEX object indices resolve
to the admitted original surfaces; caller retains actual source palette ownership.
The GPU owner loads both original atlas t3x files, reuses existing message/font
textures and releases its own textures/linear arena at Renderer shutdown.

BattleClassBackend synchronously serves only the recovered fixed TEV setup,
texture0, four-point quad primitives, source quad helper, ordinary flag16 icons
and finite plain US label queries/draws. It retains ordered quads and transforms
XYZ through the supplied source matrices before native projection. Loaded matrix
and texture state are required; overlapping/incomplete primitives reject. The
native catalog indices and caller's source labels must resolve rather than
falling back to fabricated icons or text. Background material RGB/alpha modulates
source primary color; imported surface pixels are the original white/alpha art.
The quad helper uses original corner order/UVs, including reversed X geometry
for bubble mirroring. No ellipse/panel replacement or render-frame timing is added.

Icon flags16 keep source full dimensions, centered X/bottom-anchored Y and frame
UVs. Dedicated IDs use class atlas; other actually admitted frames, including403,
use the existing message atlas provider. Source flag48 currently rejects explicitly:
iconGX800197f8 bit32 enters a separate three-stage TEV path at80019a08, not the
ordinary modulation backend. Disabled appearance must be bound from that program
before allowing the complete live class menu; do not silently draw enabled color
or invent a gray multiply substitute. Broader icon flags also require their own
source mirroring/clipping semantics.

FontWidth resolves the retained source label and uses the existing plain US
WID1 measurement with spacing2. FontString centers by minus half that width,
uses the recovered StringMatrix glyph consumer and the supplied source matrix,
color and glyph atlas. Tagged/multibyte/outlined consumers remain outside this
finite domain; no controls are stripped and no dialogue is rewritten. Each glyph
retains the font consumer's two RNG requests through an explicit provider; absence
of that provider rejects. This is not blanket completion of FontDrawStart,
FontDrawStringCenterMtx, camera-current ownership or original global font/RNG state.

Renderer::drawClassMenu submits source-ordered triangles through the existing
textured/color shader. Native reflow maps source centered UI coordinates to the
400x240 upper screen with half-size XY, inverted Y and flattened UI depth.
Projection and raster restore use the actual upper target and existing Citro2D
alpha/blend/depth conventions. The bounded1024-quad arena is a native capacity,
reset per rendered frame; overflow rejects without silent truncation. UI clocks
are not advanced by this draw. These are native backend choices, not recovered
original camera framing or a claim of original SDK float bit equality.

Host checks load actual private catalogs, assert transformed mirrored surface
quads, incomplete primitive gates, source material alpha, original icon anchors,
disabled-path rejection, actual US glyph UV/color/centering and required RNG
requests. Native compilation checks the actual atlas import/submission/release
code. An emulator-visible class-menu fixture still requires connecting this
backend to actual packed source memory/camera services and ring producer.

Next: implement original disabled-icon TEV behavior, bind source center/offset
and actual actor projection, synchronous palette/font queries and renderer-ready
publication, then drive ring once per simulation tick. Keep physical D-pad/Circle
Pad/A/B command control and the
[upper-stage/lower-audience layout](NATIVE_BATTLE_HUD_AND_SUBMENU.md).
