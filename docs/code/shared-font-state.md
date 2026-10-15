# Shared original font state

[SourceFontState](../../source/display/source_font_state.cpp) owns fourteen
original scale/color/edge/start/noise/rainbow helpers over actual shared process
memory. See [text drawing](battle-ui-text-cost.md), [font GX](font-gx-pipeline.md)
and [texture objects](source-texture-objects.md). It creates no per-label default
style or successful missing GX child.

Offsets below are hexadecimal. Shared globals are scale XYZ `0x8041e9b0/b4/b8`,
raw RGBA `0x8041e9ac`, alpha byte `0x8041e9a8` and edge word `0x8041e9a4`.
ScaleVec reads all caller words before any store; Scale repeats original float
bits without host conversion. GetDrawColor returns the raw word. Void helpers
return native completion zero only; that does not describe an unused PPC return
register. Missing children retain the invocation and successful prefix.

## Raw and effective color

Effective alpha is `floor(rawAlpha*globalAlpha/255)` in a local color. The pointer
color helper stores raw global color, mutates the caller's alpha byte and rereads
the caller word before requesting TEV color. Guest aliases are real: a pointer
into the raw color can change that global again. Reusing a mutated caller cell
scales alpha again. An explicitly borrowed private color stays at the same host
cell through a wait; it is not assigned a fabricated guest stack address.

Palette index shift/address addition wrap 32 bits into `0x80416010`, with no
invented three-color bounds check. Edge setters only write the shared CPU word.
Alpha-start and default-start preserve different XYZ read/store order; both use
original scale literals `0x802c2b6c/70/74`, palette zero and edge reset before
whole JUT DrawStart. Alpha-start narrows its argument to a byte; default uses 255.
A request alone cannot acknowledge missing texture, matrix or font setup.

## Exact GX scopes

NoiseOff requests one texgen, then one TEV stage. RainbowOff changes only the TEV
stage count. Neither operation restores a saved configuration or defines a CPU
noise/rainbow boolean.

Rainbow enables two stages and configures stage 1 with null texcoord/map enums
255 and color channel 4. Color/alpha ops use ADD, zero bias, 1× scale, clamp and
PREV output; their raw input enums remain the original `15/0/10/15` and
`7/0/5/7`. These are GX selectors, not byte colors or alpha values. Whole linked
requests stay in source order. No final visual interpolation is inferred merely
from this helper's name.

Noise enables two texgens/stages, then stage 1 uses texcoord 1, map 2, null color
255 and KAlpha selector zero, with original color/alpha input enums. It freshly
reads the actual Window object-array global and adds `0x640`: original object
index 50 at 32-byte stride. There is no source readiness/count guard or invented
procedural noise asset. The matrix child captures original `(1/64,1/64,1)` once,
then its real output feeds texture matrix 33/type 1 and texgen arguments
`(1,1,5,33,0,125)`. Later setup or native residency must be proven by actual owners.

The cursor pins helper/input/private-cell address and genuine memory/GX owners.
Retained request objects preserve partially completed children without replaying
CPU writes. Changed identities refuse; observed context loss stales until reset.
Reset does not pop style, restore source globals or undo native GX effects.
See [shared-state contract](../behavior/display/RETAINED_SHARED_FONT_STATE.md).
