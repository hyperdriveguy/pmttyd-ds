# String vector/pitch drawing and scalar wrappers

US G8ME01 revision0,recovered2026-10-12. Evidence B: FontDrawStringVecPitch
0x80077034,size0x334;FontDrawStringPitch0x80077368,size0x68;FontDrawString
0x800773d0,size0x68. See [font mapping](FONT_CODE_MAPPING.md),
[mutable widths](FONT_RESOURCE_SETUP.md),[quad output](FONT_GLYPH_QUADS.md),
[state/alpha](FONT_DRAW_STATE.md) and [message width](FONT_MESSAGE_WIDTH.md).
Static original code only;no executed glyph/layout/image equivalence is claimed.

## Interfaces and one-time scale matrix (B)

VecPitch takes vector pointer r3,string pointer r4,pitch P in f1. Capture the
current camera from camGetCurPtr and starting pen X from vector.x. Build a local
scale matrix once from shared fontScale x/y/z at0x8041e9b0..0x8041e9b8 using
PSMTXScale. No translation is applied to the caller vector,and it is not written.
Vector.y/z are reloaded per accepted glyph,while starting x is captured once and
then maintained as a floating pen. Global scale x is also reloaded for positioning
and advance;the initial scale matrix is not rebuilt inside the loop.

FontDrawStringPitch receives string r3,x/y/pitch in f1/f2/f3. Store x/y as floats,
construct a local vector with original zero z template,and forward VecPitch with
pitch copied from f3. FontDrawString receives string r3,x/y in f1/f2,constructs
the same float vector and forwards original binary32 pitch-1 at0x80420430.
Both source vector templates at0x802c2b78/0x802c2b84 contain three zero floats.
There is no integer coordinate input conversion or additional centering rule
in the scalar wrappers. No semantic scalar return is established.

## Byte stream and skipped commands (B)

Process until the current byte is NUL. Initialize glyph G0xffff each iteration,
and call _ismbblead on the signed current byte. A true result reads a big-endian
halfword,maps it through JUTFont_CodeToGlyph and advances two bytes,without
trail/NUL validation. Otherwise if byte is '<',call msgGetCommand on input+1
with local name/argument buffers and replace the cursor with its result.
All command names/arguments are ignored;there is no icon/style/scale action or
post-command LF suppression in this routine. Null parse result is unchecked
and later dereferenced by the loop condition.

Every other byte maps as an unsigned byte code,then advances one byte. There
is no LF/CR-specific branch,no line reset or vertical movement. A literal LF
therefore follows ordinary mapping/replacement rather than ending a line.
Do not implement this caller by using the [message-width scanner](FONT_MESSAGE_WIDTH.md)
or full [record parser](MESSAGE_PARSER_RECORDS.md):their token/LF behavior differs.
Original input bytes and story tokens must remain unchanged;this documents the
caller's consumption policy,not a rewritten dialogue format.

If low16(G)==0xffff,skip glyph drawing and pen advance. Otherwise perform the
first inclusive WID1 search exactly as the width/quad contracts:scan block count
from font+0x20,advance block size,select entry+0xc+2*(glyph-first),load its unsigned
width byte w. Missing entry produces null and is dereferenced,no width fallback.
The current in-memory width includes setup patches.

## Proportional versus fixed-pitch placement (B)

Compare P to original positive float0 using fcmpo. The proportional route is
selected only by ordered less-than;all other comparison outcomes take the
fixed-pitch route. Let SX be the current global scale x,SP an original single
arithmetic boundary,and FMA single fused multiply/add. The translation x is:

| Route | Glyph translation |
| --- | --- |
| Ordered P<0 | Current pen X |
| Other | FMA(SX,SP(0.5*SP(P-float_unsigned(w))),X) |

Width-to-float uses the unsigned integer double bias followed by fsubs. The
0.5 factor is original binary32 at0x8042041c. No clamp prevents a fixed cell
narrower than the glyph or a nonfinite pitch;executed FPSCR/conversion behavior
remains unverified. The formula centers the width within the pitch cell in the
ordinary finite domain,not a caller-specific text block or screen.

Use PSMTXTransApply(initialScale,localMatrix,translationX,vector.y,vector.z),
then PSMTXConcat(camera+0x11c,localMatrix,localMatrix),then GXLoadPosMtxImm(...,0).
These are helper requests and in-place output order;matrix service/raster
execution remains separate evidence. No GXSetCurrentMtx call appears locally.
The inner glyph requests use local offsets below,so the vector translation is
not narrowed to s16 before this caller's matrix path.

## Edge/color draw order and unconditional inner RNG (B)

If shared fontEdge word0x8041e9a4 is zero,issue one _JUTFont_DrawPos(G,0,0),
inheriting effective GX color. If nonzero:

1. Save raw global draw color0x8041e9ac. Select raw palette0 from0x80416010,
   store it globally,scale a local copy's alpha by global fontAlpha/255,and
   request GXSetTevColor(1,...).
2. Issue inner glyphs at(-2,0),(2,0),(0,-2),(0,2),in that order.
3. Restore the saved raw color globally,scale a local copy's alpha the same way,
   request GXSetTevColor(1,...),then issue center glyph(0,0).

The raw-versus-effective alpha rule is the recovered [font-state policy](FONT_DRAW_STATE.md).
This caller does not mutate a caller color buffer or toggle the edge flag itself.
Matrices are unchanged between its four outline requests and center request.
No rainbow/noise setup or local irand call appears in this wrapper.

Each inner [quad](FONT_GLYPH_QUADS.md) consumes two irand64 calls unconditionally,
so a mapped glyph here requests two draws of RNG for edge-off or ten for edge-on.
These are local call-chain counts,not full-game seeding/cadence observations.
Skipped tags/glyph0xffff make no local inner-quad request. Do not infer zero RNG
consumption just because this wrapper does not directly invoke irand.

## Pen advancement and remaining evidence (B)

After drawing,repeat ordered pitch comparison. Proportional advance reads the
shared configuration word+0x16c,selects the unchecked seven-entry US spacing
word table0x8041601c,and forms a signed32 word sum(w+spacing). Convert via signed
bias/fsubs,and set X=FMA(SX,float(sum),X). Fixed advance sets X=FMA(P,SX,X).
Both are single-precision fused operations. X is not halfword-wrapped and there
is no trailing-space subtraction or returned width in this drawing routine.
The pen moves only after a mapped/drawn glyph,not on skipped commands.

Three whole-function fingerprints and24 instruction anchors pin matrix/byte/
command/width/pitch routing,color/outline ordering and pen formulas. Direct
checks pin both zero-vector templates,half and default negative pitch. These
checks never execute original rendering;ELF/disassembly/reports remain ignored.
No original code linking,asset conversion or story rewrite occurs.

Validate vector/current scale/camera state,proportional/fixed pitch,LF/tag/high
bytes,outline color restoration,inner RNG and final positions against original
execution/images. Recover matrix,string-matrix/center/shake/message-draw callers
and resource/cadence conditions;closed local numeric contracts still require
composed execution validation. Full text/story/field/battle/audio/save/content
and complete-game requirements remain unfinished;no Switch behavior is inferred.
