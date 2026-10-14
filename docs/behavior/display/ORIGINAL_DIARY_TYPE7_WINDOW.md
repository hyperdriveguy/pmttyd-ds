# Original diary/type7 message window

US G8ME01 disc0 revision0; static read-only DOL/ISO recovery2026-10-14.
Used by Trouble Center exposition irai_help_02/13 and the other original
irai_help messages. Supplements
[board provider boundaries](../field/TROUBLE_CENTER_NATIVE_BOARD_PROVIDER_BOUNDARIES.md),
[general drawing](../message/MESSAGE_WINDOW_DRAWING.md), and
[shared Kanban helper](ORIGINAL_KANBAN_TYPE4_WINDOW_AND_LOCK_NOTICE.md).
Source requests and texture sampling are recovered; native raster parity
requires backend implementation and matched rendering checks.

## Type, dispatch and placement

Original parser `<diary>` writes W.type7 only. It does not set zero text speed,
reset style/coordinates, change window placement, or replace message identity.
Do not inherit kanban's parser speed0 merely because both use the same border
helper. Trouble help02/13 also preserve their original keys and page controls.

msgWindow_Disp80080260/6dc dispatch table8030f5a0 entry7 targets80080700.
It loads W.s16alpha, x/y/width/height and calls
windowDispGX_Kanban(type7,low8(alpha),x,y,width,height). Unlike ordinary
anchored bubbles, this route ignores local placement flagF and draws text at
every opacity. No speech tail, anchor-growth interpolation or bubble shadow
is requested on this route. The caller still owns its actual toge/identity.

After the border, the exact request order at8008071c..778 is:

1. Load y, subtract36, subtract that result from240, then subtract8, each
   source fsubs boundary. Convert through __cvt_fp2unsigned to scissorY.
2. Read cached currentCamera.u16f4/f8, call
   GXSetScissor(cameraX,scissorY,cameraWidth,126).
3. Reload x, add62; reload y, subtract36; reload alpha and handle; call
   msgDisp(handle,low8(alpha),x+62,y-36), retaining its result.
4. Restore the cached camera's f4/f6/f8/fa scissor at common tail80080888.
   If retained msgDisp result is nonzero, execute the common source indicator
   provider at(x+width-52,y-height+31,0,180,5).

Unsigned clipping conversion is the linked general drawing contract's U,
not arbitrary rounding or an intersect with the window rectangle. Default
constructor geometry(-280,225,560,176) gives textorigin(-218,189), clipY43,
height126. Width/height scale the frame, but this text origin and clipheight
remain these source constants. Explicit source wpos may change geometry;
the used help02/13 messages contain no such token.

## Exactly one original texture and one quad

windowDispGX_Kanban8007e9f0/ffc has11-entry dispatch table8030f480.
Entry7 targets8007f838; the complete branch ends at8007f9ac and rejoins
common epilogue8007f9b0. All texture-object references on this branch are
original window texture array+c0, exactly index6*32. There are no cap pieces,
second texture, alternative diary page, nine-slice or decorative pass.

Shared helper first creates T(x,y,0), then S(width/560,height/176,1),
concatenates T*S followed by currentCamera.view(+11c)*(T*S), loads position
matrix0 and selects current matrix0. Thus the whole original diary is scaled
uniformly by those independent width/height ratios, including frame borders.

The type7 branch sets one texture generator,
GXSetTexCoordGen2(0,1,4,30,0,125), reads the actual index6 height then width,
and creates texture matrix S(abs(560)/textureWidth,abs(176)/textureHeight,1).
It loads this at matrix30 using2x4mode, binds index6 as TEXMAP0, and issues
GXBegin(0x80,vertexformat0,4). Actual texture dimensions make both scales1.
The emitted FIFO vertices, each XYZ followed by ST, are:

| Order | XYZ | ST |
| --- | --- | --- |
| 1 | 0,0,0 | 0,0 |
| 2 | 560,0,0 | 1,0 |
| 3 | 560,-176,0 | 1,1 |
| 4 | 0,-176,0 | 0,1 |

This is20 float FIFO values and one quad. A native triangulation may retain
these same corners/sampling. A lossless image extraction of index6 supplies
the entire canonical560x176 frame; unlike type4 upper-middle there is no
edge-extension composition or nonidentity texture scaling to bake.

## Original TPL descriptor and material

Original f/msgWindow.tpl ISOoffset947396944,size321984,
SHA2562ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67.
It has51 descriptors; diary index6 has headeroffset27c, no palette, dataoffset
6900, width560,height176, format14/CMPR. Header wrapS/T0(CLAMP), min/mag
filter1(linear), zeroLOD bias,minLOD/maxLOD0. TEXGetGXTexObjFromPalette forwards
these fields into GXInitTexObj/LOD, and the diary helper does not override
wrap/filter. Preserve the original RGBA including CMPR transparency, image
orientation, edge samples and linear-filter footprint when packing a native
texture. Generated art and ordinary-bubble assets are not substitutes.

Material state is the same shared helper setup established by the linked
type4 contract: cullnone; ZCompLoc1; alpha compareALWAYS/AND/ALWAYS; blend
ADD/SRCALPHA/INVSRCALPHA; depth enabledALWAYS, updatesfalse; fogtype0; direct
POSITIONXYZ/F32 and TEX0ST/F32; zero color channels; one TEV stage with
TEXMAP0/TEXCOORD0/nullcolorchannel. Colorregister1 isffffff with low8 caller
alpha. TEV output is original textureRGB and textureAlpha*callerAlpha.
No arbitrary paper tint, baked text, outline or shadow belongs in the frame.
Source font styling and the alpha-bearing frame remain independent.

Normal current glyph text does inherit the window opacity: outer80080758
narrows W.s16alpha into msgDisp's alphaWord, whose initial request is
FontDrawStart_alpha(alphaWord). That helper80077ac0/8c stores G=low8(alphaWord)
at8041e9a8 and composes palette0 effective alpha as trunc(A*G/255).
FontDrawColor800777e0/64 also composes supplied coloralphaA with that storedG;
see [font alpha state](../message/FONT_DRAW_STATE.md). Therefore native
current-message glyph alpha modulation by message.alpha has source support.
Direct GXSetTevColor requests in scroll-history emphasis/fade or other glyph
effect branches can bypassG, so this is not evidence for multiplying every
possible original glyph request by window opacity universally.

General opening/closing sound table8030f540 uses diary row7=(34,35), requested
as0x20022/0x20023 unless W.flags8 suppresses sounds. Existing original general
window time-based fade/deferred deletion and key/scroll/retainedstop ownership
apply; the art branch itself neither skips nor completes playback.

## Evidence and native boundary

Fresh msgWindow_Disp listing is ignored artifacts/trouble-diary-disp.txt;
helper/call/constants listing artifacts/trouble-kanban-annotated.txt and
original TPL artifacts/trouble-msgWindow.tpl are read-only inspection inputs.
Whole outer body SHA2565267e64acd9f2b465d6bbeb6a8547e074f2656c99311db3bba38dafabf6e8274;
complete type7 inner branch8007f838/178 SHA256
d0edb1be2b39e9a96047f3a585f59b56b5b18fa1c32f84ddd2fa68e562c19e74;
whole shared helper SHA256befbf6b1ea453907df42fad9b4c8bae5ce6e51c6ea039feca5eb43e69d5b55b3.
Table target, texture header and constants36/240/62/8 were independently read
from the original ISO. No original assets, dialogue prose, code or shared
documentation files were changed by this recovery.

Native implementation needs a validated original index6 texture, dedicated
type7 frame branch, the general source type7 clip/text/indicator plan, and
retained MessagePlayback with ordinary parser speed. At the same existing
screen adaptation as other messages, extract/pack the original texture and
transform the recovered source geometry rather than inventing new framing.
