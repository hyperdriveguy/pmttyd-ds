# Native explicit item selector rendering

The retained [owner](NATIVE_ITEM_TABLE_SELECTION_OWNER.md) now has native
Game/Renderer services and a source-asset presentation binder. It accepts
original explicit item tables with zero row flags and second argument0;
story/quest/event/pouch integration is still separate. This does not replace
the original menu with message Yes/No selection or invented item strings.

## Text and glyph services

`window_font.cpp` binds the valid original US single-byte plain-text subset of
[window-font consumers](WINDOW_FONT_STRING_AND_MESSAGE_CONSUMERS.md). It uses
original byte-to-glyph mappings and patched WID1 widths retained in the existing
font pack. Measurements preserve low16 line widths/LF count, unsigned maximum
and final trailing-spacing subtraction. US startup's config1 supplies spacing2.
Strings containing tags or invalid mapped glyphs reject before emitting quads;
no tags are stripped and no bubble reveal/wait/parser state is substituted.

String-matrix pen is binary32; message-matrix pen is signed32 with a separate
per-glyph translation, and direct-message pen retains supplied signed XY.
LF pitch28 and source top offset-2/height22 remain distinct. Glyph geometry
retains signed16 local positions and original atlas UV crop. Outline requests
are source left/right/up/down offsets2, in that order, then center, with palette0
outline and retained text color. Effective alpha is floor(rawAlpha*drawAlpha/255).
Each inner quad invokes its supplied random64 service twice before submission.

Native font drawing uses one citro3d draw per text request, with source colors
in vertices. A1024-quad linear arena (221184bytes) allocates only at renderer
init; each request uses a unique slice until synchronous FrameBegin resets
the arena. Entire glyph quads outside the source scissor emit no native GPU
vertices while all original quad/RNG requests still execute. This supports
the original121-key-item table domain without exhausting the arena on invisible
rows. Only written vertices are cache-flushed. Shutdown synchronizes before
freeing. Capacity is native admission, not an original font limit. Source
wrapper T*S and half-size viewport reflow execute through GPU matrices;
per-glyph translation is assembled into local float vertices. Libcitro3d
matrix arithmetic, PICA rounding/sampling and native triangles remain backend
adaptations rather than original SDK/GX raster equivalence.

The native random callback uses separate presentation state. It preserves
request counts without claiming original shared rand ownership; noise UV/hue
are visually inactive in this plain font setup. Original global RNG/camera/
mutable font palette/width lifetime and tagged/icon streams remain required
for complete source service parity.

## Retained resources and source requests

`ItemSelectionPresentation` resolves original global message IDs through the
original340-record item catalog, preserving duplicate rows and original text.
Admission checks original item/cursor assets, long-list arrows and paged-help
icons when needed. Its immutable shared message bank survives subsequent asset
reloads; the supplied font must remain alive through owner release. Native
startup uses the stable original font owner. No resource read/allocation occurs
in a font draw; source sound requests are retained for later audio binding.

Game ticks this owner with physical direction repeat/trigger and A/B snapshots.
The engine's60-tick refresh and tick counter bridge sequence timing/GP.word1c;
original GP producer/global lifetime remain adapters. Icon animation/blink uses
the supplied counter rather than time since window entry. Deferred display
slots retain list/header/help order, frames precede contents, transition4
suppresses contents, and description publishes text before its own help gate.
Header resize occurs after that invocation's text draw. Fractional row easing
remains in font/icon wrapper positions.

The original frame texture, font atlas and icon textures are reused. Source row
icons use the [centered winIconSet service](../display/ITEM_SELECTOR_ICON_ANCHORS_AND_FLAGS.md),
alpha>=128 and blending disabled for opaque rows. Cursor and arrows use general
bottom anchoring; flags20 includes cursor X mirror, flags16 full dimensions.
No extra stripe is drawn for second argument0. Required draw failures propagate
instead of silently claiming complete art. Zero-flag authored quest rows do not
admit unimplemented gray/additional-icon row branches.

Native row/help scissor requests project all four original608x480 corners
through the actual tilted projection to240x400 framebuffer axes. Pending
citro2d work flushes before scissor/material changes. Native font restores
citro2d shader/buffers, explicit depth-disabled UI, alpha>8 and ordinary blending.
Opaque row icons flush around their alpha/logic-copy state then restore blending.
The selector disables its scissor before returning to the enclosing renderer.
Original filter/raster/edge pixels and scissor parity need matched captures.

## Integration and validation boundary

Original quest strings95/107 are plain; both descriptions have one LF, so their
actual help path has two lines and no page icons. The renderer is reusable for
other admitted original explicit tables, preserving source item IDs/control
semantics. Admission rejects missing/unsupported content without substituting
another item or rewriting dialogue. Shared manager allocation, Mario key
ownership/event destinations, status-window presentation and SFX macros remain
unbound; a retained UI alone does not prove a complete quest runs.

Host tests check glyph UV/position/outline/RNG order, consumer pen domains,
measurement/rejection, actual original text/metadata resources, duplicates,
deferred help publication, closing results and resource retention. Native
build/capture evidence and exact limitations are recorded in PROGRESS.
Next: source event/pouch/switch binding, original status/audio services, full
font streams and matched Dolphin/New2DS raster/performance validation.
