# Explicit item-table selector draw requests

US G8ME01 revision0; static read-only ISO recovery2026-10-13. Supplemental to
[item-table ownership](ITEM_TABLE_SELECTION_WINDOW.md) for negative-address
explicit tables with second argument0. These are original service requests,
not matched GX/PICA pixels or a complete native font implementation. Original
inspection-only disassemblies remain `/tmp/selector-recovery-*` and
`/tmp/item-win-disp*.txt`; no extracted assets are added to source history.

## List admission, rows and clipping

select_disp8023bc74 skips all contents when its owned list manager flags4
is set. It requests fog off, saves GX scissor, then requests scissor
(listX+304,274-listY,listWidth,listHeight-50). It visits all published rows,
not only eight; scissor limits visible pixels. Per-row Y uses index i and
retained displacement D. The original item table supplies message keys and
icon descriptors; see [item-table contract](ITEM_TABLE_SELECTION_WINDOW.md).

| Request | Original arguments |
| --- | --- |
| Item icon | XYZ=(listX+35,listY-44-24*i+D,0), scale=(.5,.5,.5), RGBAffffffff |
| Name | XYZ=(listX+60,listY-32-24*i+D,0), scale=(1,1,1), width limit185, RGBA000000ff |
| Disabled flag1 | Icon gray service; name RGBAa0a0a0ff |

Name calls winFontInit8017d7d0 (only FontDrawStart), msgSearch on item record+4,
then winFontSetWidth8017d52c. The row-scale vector is a fresh copy for each name.
Flag8's additional icon154 and numbered-selector price/count branches are
outside the zero-flag explicit quest rows. Source row order and duplicate items
must be retained.

## Stripe gate and heading

After rows, context+38 controls stripe requests: **if zero, no stripe447 is
drawn**. Both Goomba4 explicit-table calls pass0. Do not infer that the asset's
presence requires decorating these quest lists. In the nonzero branch, call
winIconInit, then for i=2..29 inclusive request icon447 at
(listX+trunc(i*listWidth/31),listY-56+D,0), scale=(.15,.15,.15), RGBA000000ff.
The .15 float has bits3e19999a. Integer multiply/divide use source signed32
arithmetic. Stripes are inside the saved row scissor; restore it afterward.

For negative selectors choose msg_window_title_1 through msgSearch. Measure
low16(FontGetMessageWidth(text)), convert to binary32, cap to120. Let
Q=trunc(binary32((float(listWidth)-cappedWidth)*.5)). The source then uses
**low16(Q)**, zero extended, in its signed32 addition to listX; a negative
centering result is not added as a negative value. Request heading at
(listX+low16(Q),listY+14,0), scale=(1,1,1), RGBAffffffff, width limit120,
through winFontInit then winFontSetEdgeWidth8017d10c. No row scissor applies
to heading. This wrapper has the same width/scale arithmetic as ordinary
winFontSetWidth, but forwards to winFontSetEdge8017d28c: build T*S, apply
FontDrawColor, set FontDrawEdge, call FontDrawStringMtx80076cdc, then
FontDrawEdgeOff. The heading is outlined and uses the string consumer rather
than FontDrawMessageMtx. FontDrawStringMtx skips markup without style/icon
requests; its post-command LF suppression still has the R-prefixed exception.

## Cursor and list arrows

Cursor request at8023c894..c8cc copies a vector with Z=1, replaces X/Y with
context cursor floats, and calls iconDispGx(vector,camera20,icon504,scale1).
This is not a window-manager transform applied to cursor coordinates.

Only when rowCount>8 and (GP.word[1c]&31)<20:

| Icon | Gate | XYZ | Scale / camera |
| --- | --- | --- | --- |
| 445 above | firstVisible!=0 | (fusedSingle(float(listWidth)*.5+float(listX)),listY-36,1) | .6 /16 |
| 446 below | firstVisible!=rowCount-8 | (sameX,listY-listHeight-12,1) | .6 /16 |

Scale .6 is binary32 bits3f19999a. These draws follow heading after scissor
restoration. The clock owner/cadence and icon animation producer remain separate.

## Header requests and rectangle mutation

select_disp2 at80239ae8 starts with msg_window_select_1. Numbered selectors3..18
can replace this key; negative explicit tables retain it. If its owned header
manager flags4 is set, no text or later geometry writes occur.

Call FontGetMessageWidthLine(text,&LFhalfword). This returns maximum width and
counts literal processed LF bytes, not visual wrap lines; see
[width scanner](FONT_MESSAGE_WIDTH.md). Let W=low16(return), L=LFhalfword.
Initialize scale=(1,1,1). If float(W)>float(headerWidth-30), set
scaleX=float(headerWidth-30)/float(W), and renderWidth=float(headerWidth-30).
Otherwise renderWidth=float(W). No narrow-width clamp is added.

Make T(X,Y,0)*S(scaleX,1,1), where:

- Q=trunc(binary32((float(headerWidth)-renderWidth)*.5));
  X=headerX+low16(Q), with the same zero-extension as heading.
- Y=headerY-trunc((headerHeight-24*(L+1))/2), using signed32 arithmetic.

Then FontDrawStart and FontDrawMessageMtx(matrix,text). Header has no explicit
FontDrawColor call: the draw-start palette applies. Unlike heading it does not
call winFontSetWidth.

After text draw, reload header manager through context+28. If active flag1,
write its rectangle for subsequent calls: X=current headerX,
Y=definitionY+trunc((22*L)/2), width=current headerWidth,
height=definitionHeight+22*L. These writes happen after matrix/text draw and
use the retained definition's baseline Y/height. Do not pre-adjust the current
text matrix using the new rectangle or apply these writes to the list/description.

## Help text and additional original icons

select_disp3 resolves selected item record+8 via msgSearch, stores text at
its manager+34, then winMgrHelpDraw8023e53c. This help helper independently
checks flags4. It sets scissor(0,384,608,56), calls FontDrawStart, and requests
FontDrawMessage(-210,trunc(fusedSingle(28*float(helpPage)-144)),text).
The lottery-description special case formats a pouch halfword first; it does
not apply to the two quest descriptions. Restore scissor(0,0,608,480), rather
than restoring the previous rectangle.

When manager lineCount+3c>2, help icons use X=helpX+helpWidth-12, Z=0,
uniform scale .6 and camera16. Define topY=helpY-helpHeight+44,
bottomY=helpY-helpHeight-4. Under (GP.word[1c]&31)<20:

- page0: draw446 at bottomY only.
- page+2==lineCount: draw445 at topY only.
- otherwise: draw445 at topY then446 at bottomY.

After those blink branches, always draw icon120 at Y=helpY-helpHeight+10
when lineCount>2; it has the same X/Z/scale/camera. Icon120 is a separate
original resource requirement, not an inferred controller substitute.

## Window-font wrapper and independent text services

winFontInit is exactly FontDrawStart: reset shared scale to(1,1,1), edge0,
draw-alpha255 and raw color to palette0 (initial1e1414ff). Delegated glyph/GX
setup remains part of that service; see [font state](FONT_DRAW_STATE.md).

winFontSetWidth measures low16 FontGetMessageWidth and computes binary32
currentWidth=scaleX*float(width). If greater than maxWidth, write
scaleX=scaleX*(maxWidth/currentWidth), preserving separate divide/multiply.
Only X scale changes. Then copy position/scale/color and call winFontSet.
winFontSet makes T(position)*S(scale), applies FontDrawColor to a mutable local
color, then FontDrawMessageMtx. Format-string expansion using original vsprintf
occurs only when the supplied text contains '%'; ordinary message keys/text
are not printf-format substitutes. The native immutable text adapter must
identify this service boundary rather than format untrusted story text blindly.

Window text does **not** call the message-bubble record/wait/parser owner.
Heading is the separate string/edge path above; the following paragraph covers
row/header/help message-font consumers.
FontDrawMessageMtx80075928 and FontDrawMessage80075fcc scan their byte streams
on every draw. Ordinary LF resets horizontal position and moves Y by-28.
Glyph advance uses the mutable original WID1 width plus the configuration's
spacing word; matrix text starts local XY=(0,0). Tags are interpreted by these
font consumers, not msgMain. The matrix variant has explicit red,/red,icon
handling; direct-coordinate variant additionally has col,/col. Other tag names
are skipped, with the same source R-prefixed-name/post-command-LF distinction
as the width scanner. There is no reveal wait, page-event or message-selection
state here. Numeric/named icon scanf, original mapping, mutable width patches,
GX glyph setup, edge paths and renderer RNG remain distinct dependencies.

Use [width scanner](FONT_MESSAGE_WIDTH.md),
[font mapping](FONT_CODE_MAPPING.md),
[width/resource patches](FONT_RESOURCE_SETUP.md),
[glyph quads](FONT_GLYPH_QUADS.md) and
[glyph draw requests](FONT_GLYPH_REQUESTS_AND_SETUP.md), not a width guessed
from decoded atlas rectangles. FontDrawMessage/Mtx complete token/style/icon
request parity is still an open follow-up; these observations establish the
selector's service selection and plain-text positioning only. No matched
Dolphin capture, hardware measurement or full selector rendering claim.
