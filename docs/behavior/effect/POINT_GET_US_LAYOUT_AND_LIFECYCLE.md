# US PointGet original layout and lifecycle

2026-10-13 US read-only complete used bodies: effPointGetDisp8021ac94/8d0,
effPointGetMain8021b564/1a8, effPointGetEntry8021b70c/10c. Listings are ignored
`artifacts/recovered-temp-evidence/pointget-{disp,main,entry}.dis`.
The [EXP caller/assets](../battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md)
supplies type0, amount10, input XYZ(0,90,0), duration0. This closes native quad
and update math; it does not prove original GPU/camera/audio execution.

## Handles, constructor and exact update scheduling

Entry obtains an actual effEntry8005ec68 record H; H14=name802fed08 `PointGet`,
H8=count1. Heap3 allocation count*36 returns payload D storedHc;
H10=Main8021b564, Hflags OR2. Source effEntry establishes active/context;
the callback does not invent a separate battle-owned model/pose resource.

| Payload offset/type | Original entry value |
| --- | --- |
| 0/s32 | Passed type0 |
| 4,8,c/float | inputX-2000,inputY-30,inputZ (fresh -2000,60,0) |
| 10/s32 | Original amount10; it remains10 during separate pouch counting |
| 14/float | Scale1 |
| 18/s32 | Passed duration0 |
| 1c/s32 | Timer0 |
| 20/s32 | Phase0 |

Main snapshots pre-update XYZ for dispCalcZ8001086c, increments timer once,
then dispatches phase. Phase0 updates X with intplGetValue(11,-1000,0,t,45);
t>=45 resets timer0 and phase1. Phase1 duration0 waits Hflags&4; admitted
clear4, timer0,phase2, without same-call phase2 motion. Nonzero duration compares
incremented timer to duration instead. Phase2 updates X via(11,0,1000,t,60);
t>=60 calls effDelete8005ea30 and returns before draw registration. Other
phase values still register draw without movement. Otherwise use pre-update
depth, dispEntry(camera8,order2,Disp8021ac94,H). Disp reads the live payload,
so depth snapshot and visible X deliberately have different capture times.

Original [effect driver](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md) invokes
admitted active callbacks once per scan, context-matched to GP14. Outer update
effMain runs after battleMain and iconMain. Effects created earlier that pass
are eligible in this later scan; creation does not itself call Main. Thus used
first updated visible X is mode11 t1, not entry X-2000. Rendering/GPU redraws
do not advance timer. Mode11 cosine arithmetic is in the EXP contract; a host
cos implementation remains an explicit numeric provider.

effSoftDelete(H) flags2→flags4 request is consumed only by phase1. A request
arriving during phase0 persists until phase1. Battle end0c requests it;
end0d clears E29c after5calls independently of H's60-call phase2 leave. Actual
delete frees heap3 payload and clears effect flags. Local texture lookups do
not create per-effect texture ownership: shared effect-language resource/cache
and constructor/driver context lifetimes remain their existing contracts.

## Original records and width calculation

Disp uses passed cameraID via camGetPtr8000f2fc, Hc, GP16c language selector,
texid_tbl803b1070. Fresh US selector1/amount>=2 yields five12-byte records
at803b0f8c: `(type,foregroundID,shadowID)`:
(1,124,125),(-1,-1,-1),(0,126,127),(-1,-1,-1),(1,130,131).
Amount formats into a local decimal buffer using literal `%d`; no translated
or generated wording. Type1 is whole phrase, type0 decimal glyphs, -1 spacing.

Compute L from FOREGROUND texture widths only, starting0:
type1 adds width*0.5; type0 adds `(decimalLength*atlasWidth/10)*0.5`;
type-1 adds5. Unknown types contribute0. BaseScale starts D14; if L>200,
multiply BaseScale by200/L. No scale cap when L<=200.
US10: L=100+5+48+5+164=322, BaseScale=200/322 when D14=1.
Original constants:10=80427418,0.5=8042741c,5=80427420,200=80427424,
digit uniform scale1.4285714626312256=80427428,Uscale0.10000000149011612
=8042742c,one=80427430,zero=80427414.

Base matrix: camera+11c * T(D4,D8,Dc) * S(BaseScale,BaseScale,BaseScale).
Keep original multiplication order; native 2D camera/viewport can be a declared
adapter. D4 animates while D8 remains60 on this used entry.

## Two passes, cursor and source quads

Draw SHADOW first, then FOREGROUND. Shadow material RGBA00000080,
passoffset(+4,-4); foreground FFFFFFFF,offset(0,0). Both passes reset cursor0
and use the same foreground dimensions. Bind shadow texture for the shadow
pass, then obtain foreground object solely for dimensions; do not use the
half-resolution shadow width/height to size its quad.

Type-1 adds10 to cursor without drawing (half-width calculation above adds5).
Type1: cursor+=foregroundWidth*0.5; centerX=passX+cursor-L;
local matrix Base*T(centerX,passY,0); draw whole phrase then
cursor+=foregroundWidth*0.5.
Type0 repeats this for every decimal byte: cellWidth=foregroundAtlasWidth/10;
cursor+=cellWidth*0.5; centerX=passX+cursor-L;
matrix Base*T(centerX,passY,0)*S(1.42857146 all axes); draw glyph then
cursor+=cellWidth*0.5. Glyph scale does not change cursor advance.

Each quad uses original foreground dimensions W,H (digit W=atlasWidth/10):
vertices `(-W/2,H,0),(W/2,H,0),(W/2,0,0),(-W/2,0,0)`;
UV `(0,0),(1,0),(1,1),(0,1)`. Phrase texture matrix is identity.
Digit texture matrix is S(0.1,1,1)*T(decimalByte-'0',0,0), loaded at TEXMTX0;
equivalent digit UV range0.1*d..0.1*(d+1). Matrix arithmetic uses source
single constants; do not normalize to independently rounded rational values.

Fresh10 foreground centers before BaseScale are -222 (`You got`), -88
(digit1), -40 (digit0),158 (`Star Points!`). Phrase heights48, widths200/328;
digit cell48x48 before uniform glyph scale. Shadow centers add4 and baseline
-4. Source glyph scaling can slightly overlap adjacent phrase bounds; do not
invent additional spacing to remove it. Entire line's phrase bounds are
-322..322 before BaseScale and source camera projection.

Disp configures one channel, lighting disabled/material register, one texgen,
one TEV stage/texture modulation, direct XYZ/UVfloat vertices and quad primitive.
It does not locally configure blend mode/culling/depth; those states are retained
from its original display dispatcher. Citro3D should preserve source alpha art
and explicit dispatcher policy; do not claim arbitrary local blend state as
recovered from this function. Full global camera/dispatcher/GX texture sampling
and original numeric/raster comparison remain bounded backend dependencies.

## Assets and validation

Shared original `e/us/effect_us.tpl` IDs124..131 map TPL6..13; full asset hashes
and dimensions are in EXP contract. No generated text/font replacement, extra
model, effect-owned atlas allocation or implicit effect completion barrier.
Use imported original phrase/digit/shadow art plus these layout requests.

Validation: complete original local listings/record/constants, both quad FIFO
paths and matrix calls inspected, fresh10 width/centers resolved. Scoped
`git diff --check`; documentation-only/no native edits, tests, builds or commit.
