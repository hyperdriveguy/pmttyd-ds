# Window font string and message consumers

US G8ME01 revision0; read-only original DOL/ISO recovery2026-10-13. Local
consumers: FontDrawStringMtx80076cdc/358, FontDrawMessageMtx80075928/6a4,
FontDrawMessage80075fcc/714. Hex spans denote bytes. Used by the explicit
[selector draw requests](ITEM_TABLE_SELECTION_DRAW_REQUESTS.md); these are
font draw services, not message-bubble playback, msgMain or record analysis.
Static recovery establishes requests, not executed GX/PICA pixel equivalence.

## Original used text and resource scope

Read original msg/US/global.txt directly. The two quest tables' six strings have
this census; text/control tokens remain in the original local resource rather
than being copied or rewritten here:

| Message ID | Bytes excluding NUL | LF | Control tokens |
| --- | --- | --- | --- |
| msg_window_title_1 | 5 | 0 | 0 |
| msg_window_select_1 | 11 | 0 | 0 |
| in_irai_butsu | 3 | 0 | 0 |
| msg_irai_butsu | 52 | 1 | 0 |
| in_fighters_pants | 13 | 0 | 0 |
| msg_fighters_pants | 54 | 1 | 0 |

All bytes are ordinary US single-byte glyphs or LF. These particular descriptions
have two text lines; the selector's lineCount producer adds1 to processed LF,
so lineCount2 requests no help paging icons445/446/120. This is a source text
census, not permission to remove token support from a reusable window-font
service. Required selector row icons307/308,cursor504 and optional long-list
arrows445/446 retain their separate original icon owners.

## Common byte scanning and advance inputs

Process a NUL-terminated byte stream until the current signed byte is0. Per
iteration initialize glyph0xffff. Call _ismbblead on signed byte: when true,
read/map a big-endian U16 and advance2; otherwise dispatch LF, '<', ordinary
unsigned byte. See [encoding/mapping](FONT_CODE_MAPPING.md) and
[lead predicate/command split](MESSAGE_RESOURCES_AND_LOOKUP.md).
The configuration word+16c selects the lead predicate and spacing; it is not
inferred from screen size. Ordinary CR is mapped as a glyph, not a newline.
No local invalid/truncated multibyte guard or Unicode decoding occurs.

A valid low16 glyph uses first matching WID1 inclusive range; missing entry
becomes null and is dereferenced, as in [width lookup](FONT_GLYPH_QUADS.md).
Advance uses its unsigned byte+1 W plus spacing S from original table8041601c
indexed by configWord*4: entries4,2,2,2,2,2,2. The source uses source32-bit
addition in integer consumers. FontDrawStringMtx instead computes integer W+S,
converts to binary32 and fadds to its float penX. No subtraction of trailing
spacing in a draw consumer; the separate [measurement scanner](FONT_MESSAGE_WIDTH.md)
subtracts one spacing from nonzero maximum on return. Do not confuse those APIs.

No wrap, wrap width, message reveal wait, event evaluation, page-close or
selection operation in any of these three font draw consumers. Layout clipping
is supplied by callers. Tags are parsed here on every draw; they are not
prebuilt bubble display records.

## US startup configuration and font setup caller

The producer is now source-bound, rather than inferred from BFN language:
marioStInit8006fb3c loads GP through word80415fd0 at8006fd0c,
executes li r4,1 at8006fd10, and stores WORD1 to GP+16c at8006fd18.
This write is unconditional in the inspected US startup path. The neighboring
PAL/NTSC refresh branch sets GP+4 to50/60 first; it does not change this1.
US startup therefore admits single-byte lead handling and spacing table index1
(value2); JUT glyph texture normalization takes its nonzero configuration branch
(1/512,1/256,1). No native constant needs to be guessed from atlas dimensions.

The original setup caller is fontmgrTexSetup80077bdc/2c: load fresh GP,
read GP.word16c, narrow low16 into argument r3, call JUTFontSetup80075004
at80077bf4. Its direct DOL caller is seq_logoExit8008746c/44,
calling fontmgrTexSetup first at80087478. Under the initialized value1,
[source font setup](FONT_RESOURCE_SETUP.md) selects original table index1,
resource basename papermarioset_US, with the documented first-load guard and
ordered glyph0 width patch7 then9. This binds startup config, spacing and font
resource as source requests; it does not establish actual original file I/O or
GPU raster execution.

[cardCopy2Main](../memory/CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md)
captures LIVE GP.word16c before bulk restore and writes that captured value to
fresh GP+16c at800b24f0 after the other restoration changes. It does not take
font configuration from the selected save record. Thus the documented ordinary
US initialized live value1 survives this restore path. Broader custom language
changes, reset callers, mutable font teardown/reload and runtime configuration
writers remain separate lifecycle work; this is not a global immutability claim.

## FontDrawStringMtx: outlined heading consumer

Input (matrixPointer,textPointer). Cache camGetCurPtr, compute
PSMTXConcat(camera+11c,inputMatrix,localMatrix), GXLoadPosMtxImm(localMatrix,0)
once before scanning. Pen X/Y start binary32 +0. No GXSetCurrentMtx locally.
Caller matrix is not modified. Ordinary LF sets X=+0 and fsubs Y by28
(bits41e00000). All '<' commands call msgGetCommand and skip their contents;
none changes color, scale, icon requests or spacing in this string consumer.
The parsed name beginning uppercase R skips post-command LF suppression.
Other names consume a following LF byte without moving the pen. The inspected
slash branch compares a byte to2f72 and cannot match; do not invent a '/r'
exception. Low16 glyph0xffff skips all drawing/advance.

For each valid glyph, if edge word is nonzero:

1. Save raw color. Set raw color to palette0; compose its effective alpha with
   current draw alpha G and GXSetTevColor(1,color).
2. Convert float penX/Y through fctiwz, narrow each to signed16, and request
   _JUTFont_DrawPos in order at (X-2,Y),(X+2,Y),(X,Y-2),(X,Y+2), narrowing the
   offset additions again to signed16.
3. Restore saved raw color, reread G and reissue its effective TEV color.

Then request center glyph at integer conversions of float penX/Y (the inner
primitive performs its own signed16 positions). Read width and spacing, compute
binary32 penX+=float(source32(W+S)). Ordinary heading wrapper deliberately sets
edge before this consumer and clears it afterward. Five source quads per glyph
are not a single baked-outline texture; transformed offsets scale with the
wrapper matrix. Inner quads each consume two RNG calls even when visually
unused; see [quad/RNG contract](FONT_GLYPH_QUADS.md).

## FontDrawMessageMtx: row names and header

Input (matrixPointer,textPointer). Cache current camera. Signed32 penX/penY
start0. Ordinary LF resets X=0 and source32 Y-=28. For each valid glyph:

1. PSMTXTrans(localTranslation,float(penX),float(penY),+0).
2. PSMTXConcat(cachedCamera+11c,inputMatrix,localMatrix).
3. PSMTXConcat(localMatrix,localTranslation,localMatrix).
4. GXLoadPosMtxImm(localMatrix,0). No local GXSetCurrentMtx.
5. Optional edge color/save/four draws/restore, then center at(0,0).

Edge offsets are (-2,0),(2,0),(0,-2),(0,2), unlike string consumer positions
which already include integer penXY. Matrix/glyph requests do not read or apply
shared font scale independently here; caller matrix owns scale. Then signed32
penX+=W and +=S through separate source additions. Caller matrix unchanged.
Default row/header draw starts edge0; ordinary valid glyph emits one quad.

Recognized tags, comparing complete parsed command names:

| Name | Source effect |
| --- | --- |
| red | Set raw RGBAe00000ff, compose effective alpha, GXSetTevColor(1,effective) |
| /red | Set raw palette0, compose effective alpha, GXSetTevColor(1,effective) |
| icon | Original icon path below |

Other commands have no effect except their pointer consumption and common
post-command LF suppression. Uppercase-R-prefixed names suppress that suppression;
no reachable '/r' exception. This function does not recognize col or /col.
Unknown style tags are not translated into bubble-record effects.

## FontDrawMessage: direct-coordinate help

Input signed32 X,Y and textPointer. Save originalX; signed32 currentX/currentY
start supplied values. LF resets currentX=originalX and currentY-=28. It does
not request a per-glyph matrix. JUTFont_DrawStart supplied by the help caller
owns the inherited camera/scale matrix. Each valid glyph uses direct signed16
penXY coordinates for the optional edge four draws then center. Advance is
currentX+=W then +=S as in the matrix message consumer.

Tags red,/red,icon match matrix-message semantics. Additionally:

- col: sscanf(argument,"%x",&localWORD), conversion count ignored; use the
  resulting whole WORD as raw RGBA, compose alpha, request TEV color.
- /col: restore palette0 and its effective TEV color.

There is no local fallback/default for a failed col conversion. Preserve style
service semantics for valid original streams; malformed scan behavior is not
a native user-facing feature. Numeric scanner behavior belongs to the separate
[scanf runtime](../format/SCAN_INPUT_RUNTIME.md).

## Message-consumer icon path and destructive font reset

Both message consumers recognize exact icon. If signed first argument byte is
ASCII0..9, scan "%d%f%d%d%d" into ID,Q,J,Z,N. Otherwise scan
"%s%f%d%d%d", resolve name via msgIconStr2ID, replace negative ID with0.
Conversion count ignored; missing fields have no local defaults. Leading sign
therefore takes the named route. Name stack buffer32 bytes has no scanf width
bound. Narrow ID to low16 for iconGetWidthHight, obtaining U16 I,H.

Form binary32 local position, preserving listed operation rounding:

```
a = SP(.5 * float_unsigned16(I))
b = fused_single(a * Q + float_signed32(J))
iconX = SP(float_signed32(penX) + b)
c = negated_fused_single(float_unsigned16(H) * Q - float_signed32(Z))
iconY = SP(float_signed32(penY) + c)
iconZ = +0
```

The matrix variant calls PSMTXMultVec(inputMatrix,&position,&position) before
iconDispGxAlpha. Direct-coordinate variant uses the formed position directly.
Request iconDispGxAlpha(position,camera16,low16(ID),Q,currentDrawAlpha). Q is
passed unchanged as icon scale, even for a scaled caller matrix; transformed
position is not proof of a uniformly scaled icon sprite. Source service/matrix
numerics and icon animation/material ownership are distinct dependencies.

After icon draw, BOTH consumers inline a font restart with the current G:
clear edge0, set scale(1,1,1), raw palette0, preserve G and invoke
JUTFont_DrawStart(effectivePaletteColor). Then reissue effective current raw
color through GXSetTevColor. Thus pre-icon red/custom color and edge are lost;
there is no saved style restoration. Matrix-message next glyph rebuilds its
matrix from cached camera/inputMatrix; direct help inherits the restart matrix.

Finally advance:

```
a = fused_single(float_unsigned16(I) * Q + float_signed32(N))
b = SP(float_signed32(penX) + a)
penX = original_fctiwz(b)
```

No extra S at this icon step. J and Z only affect draw position, N only advance.
Width scanner shares the advance shape but not all drawing/style/reset behavior.
After command handling the normal post-command LF suppression still applies.

## Source quads, native atlas metrics and hardware adaptations

All three consumers call _JUTFont_DrawPos directly, **not** the bubble glyph
wrapper FontDrawCode/Mtx. Its exact width, UV, top=-2, height=cellHeight-2,
vertex ordering, signed16 narrowing and two unconditional RNG calls are in
[glyph quads](FONT_GLYPH_QUADS.md). GX state/filter/TEV/current-camera ownership
comes from [draw-start setup](FONT_GLYPH_REQUESTS_AND_SETUP.md),
[color/reset state](FONT_DRAW_STATE.md) and [source font setup](FONT_RESOURCE_SETUP.md).

Current native tools/font_assets.py preserves original US code0..255 mapping,
patched WID1 widths, and cropped I4 intensity. It packs each glyph's X as
column*24+unsignedWID1offset, Y=row*24+2, width=patchedWID1width,height22.
The unsigned offset is already in the UV crop; it is **not** an extra pen
bearing. For this valid US glyph scope, packed FontGlyph.width equals source
WID1 W and may supply the advance service. Native padding fields are not
source advances. The US initialized path supplies S=2 as established above; preserve the
configuration service boundary for other callers. Place
quad top at source penY-2 and bottom at penY-24. Draw height22 and LF pitch28
are distinct. A generic atlas rectangle metric from another asset format would
not establish these widths/positions.

The mapping pack has only256 single-byte entries. Arbitrary multibyte original
mapping requires extending the source mapping service/resource domain. Live mutable-font state and source service failures
cannot be inferred from this pack. The six inspected quest texts fit its valid
single-byte domain, and glyph0 has the original ordered setup patch width9.

GPU batching is a native implementation choice. Preserve original glyph order,
scissor, transforms, source draw colors/edge offsets, texture alpha, and shared
RNG effect boundaries when claiming source behavior. Actual PICA sampling,
blend rounding, matrix numerics and unused second UV attributes remain native
adaptations until matched original/hardware evidence is obtained.

## Evidence and validation limits

Fresh tools/inspect_dol_function.py disassembled the three entire named original
spans from the read-only ISO; /tmp/selector-recovery-{stringmtx,msgmtx,msg}.elf
and corresponding .txt are inspection-only. Source constants/strings/palette
were mapped directly through original DOL sections. Census above reads exact
US resource byte pairs. Fresh marioStInit/fontmgrTexSetup/seq_logoExit evidence additionally establishes
the US initialization/setup chain; existing leaf contracts constrain inherited font setup,
quad, encoding and width services. No native code, test/build configuration,
shared index/PROGRESS, original ISO, emulator profile or save modified.

This covers all local valid plain-text behavior needed by the two explicit quest
tables and the local token service requests for future window streams. Full
original scanf, icon/backend/matrix raster parity, broader config/font lifecycle
and hardware speed remain open; no complete renderer/execution claim.
