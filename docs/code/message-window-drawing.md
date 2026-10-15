# Message geometry and plain-font draw requests

Implementation: [window helpers](../../include/message/message_window.hpp),
[font source](../../source/message/window_font.cpp),
[font API](../../include/message/window_font.hpp),
[frame source](../../source/message/window_frame.cpp),
[frame API](../../include/message/window_frame.hpp).
Evidence: [native bubbles](../behavior/message/NATIVE_MESSAGE_BUBBLES.md),
[original font consumers](../behavior/message/WINDOW_FONT_STRING_AND_MESSAGE_CONSUMERS.md),
[type0 frame](../behavior/display/NATIVE_TYPE0_WINDOW_FRAME.md).

Window planning uses centered 640x480, Y-up coordinates. The backend supplies
projection and GPU state; half-scale artwork occupies centered 320x240 within
the 400-wide handheld display. Ordinary box is `(-280,225,560,176)`. Text insets
are `(62,-36)`, selection `(40,-25)` and small `(28,-6)`. Nonzero mode interpolates
from a 10x10 box at the anchor using alpha/255, and text waits for full alpha.
Mode 0 exposes text immediately.

Mode bits select tail (bit 0), alternate right-side tail X (bit 2), and upper-tail/
small-placement branch (bit 3). Speaker mode uses absolute X >=300 for mode 2,
otherwise mode 3 left of Mario or 7 right. Tail factors .7/.38, .175/.8 and
.225/-1.2 retain the original geometric branches. These helpers compute requests;
they do not prove current camera/speaker allocation lifetime.

`stretchMessageTail()` is the explicit handheld adaptation: stop four screen
pixels before the talk anchor, or hide if too close. White/shadow 48x64 masks
have different final opaque texel centers; mask correction compensates them
separately. This is not a newly measured original tail rule. Glyph cropping uses
a 400-wide screen band and adjusts UVs for partial glyphs; text scissor bands are
separate type-dependent source heights halved into screen coordinates.

Plain-font helpers accept only their documented finite single-byte text domain.
They reject `<` rather than strip controls; scalar pitch also rejects LF/outline.
Whole-stream preflight precedes emissions/RNG. Mapping `0xffff` skips glyph and
advance. Measurement sums with U16 wrapping and subtracts one trailing spacing
from a nonzero maximum. Drawing does not subtract that spacing.

| Consumer | Pen / transform domain |
| --- | --- |
| StringMatrix | Float local pen, LF Y-=28, signed-half conversion at glyph request |
| MessageMatrix | Signed-word pen; glyph local coordinates zero plus per-glyph origin |
| MessageDirect | Supplied signed-word XY, narrowing at direct request |
| StringPitch | Physical float pen outside glyph scaling, fused scaleX*(width+spacing) advance |

Glyph top is pen Y-2; cropped atlas height and LF pitch 28 are distinct metrics.
Atlas X already contains the original crop/bearing: do not add another bearing
or use padding as advance. Default raw RGBA/palette is `1e1414ff`; effective alpha
is rawAlpha*drawAlpha/255. Outline emits left/right/down/up at offsets 2, then
center. **Each quad** consumes two RNG requests with bound 64, even when noise UVs
are unused. Visibility optimization must not remove those source side effects.
Strict overlap rejects edge-touching quads and handles flipped scales; physical
pitch is outside scaling, per-glyph origin inside it. Actual camera/matrices,
texture/scissor, blend and raster execution remain backend responsibilities.

Type0 frame geometry keeps nine quads (36 FIFO vertices), including center, using
exact binary32 thirds and original arithmetic for absolute grid edges. Narrow
rectangles are not clamped or replaced. Native triangles expand each quad as
TL/TR/BR then TL/BR/BL, 54 vertices in original cell order. Transformed frames keep
local centered geometry and a separate center/rotation/scale for `T*Rz*S`.

Supported selector decorations 0/2 use a base frame with border 20, definition RGB
and low8 alpha. Decoration 1 adds the 150x28 blue `2ba4ff` tab, border 8, at centered
X and Y+14. Wrapping word subtraction and signed division by two preserve odd-width
centering. The tab rotates around its own rectangle center. This helper does not
implement other decorations, selector manager lifetime or contents.

See [resources](message-resources.md) and [playback](message-playback.md). Source
request fidelity is separate from matched PICA pixels, full story playback,
hardware performance or original GPU/runtime equivalence.
