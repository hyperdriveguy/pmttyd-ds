# Terminal animated projected texture, independent clock samples and KColor

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis,no original instructions/GX calls executed or images observed.
`tools/verify_map_tev_terminal.py` checks the containing TEV identity as PARTIAL
semantics,255 reviewed words,eleven binary32 constants,five original spans,
two US lecture imports,six direct DOL sites and65536 host byte-product cases.
Two earlier depth branches and backend/source/runtime remain open.

Complete block80024654..800248a8 exclusive is admitted iff F&0x800000.
Clear skips directly to the final callback block. Set appends one textured
stage with K/B/C each incremented once;I unchanged. No texture availability,
clock/finite,table-index or render tick/frame guard is provided locally.

## Source capture and first cosine clock

Copy nine RAW words from802bfa78..802bfa98 to three local vectors,BEFORE clock
and texture requests:eye=(0,0,1),up=(0,1,0),target=(0,0,0). These are original
immutable seed metadata;they are not an actor/scene camera query.
Read GP pointer80415fd0 and raw t=word[GP+1c]. Convert unsigned t to single q
using the same original double-bias/fsubs sequence as
[terminal color](MAP_TEV_TERMINAL_COLOR_AND_COSINE_ALPHA.md). This is a fresh
clock read,not reused from that earlier admitted block.

Compute theta=R32(R32(0x3fc90fdb*q)/30);double cos80271ea8 then FRSP s.
v=R32(127*R32(1-s));z=F32(v,.5,128). FCTIWZ z;retain the full32-bit integer
result D until later byte stores. The local body does not clamp D or interpret
it as a measured percentage/time curve. Original libm/FPSCR/numeric behavior
and GP clock cadence remain open;the three division constants in color/pulse/
matrix setup belong to different expressions.

## Original built-in texture object and second clock

Request GXInitTexObj802b72b0(local object,data address803048a0,width4,height64,
format14,wrapS0,wrapT1,mipmap0). No local GXInitTexObjLOD call. Then
GXLoadTexObj(local object,B). The source data is a256-byte original DOL texture
span,SHA2561b058d9cc9f2d6d3ee6fcb0252405672529f289f553ef90cd144cad5ca3eab5d.
The verifier checks its hash without exporting image/texture bytes into source.
Original GX format14 decoding/filter/initialization/cache behavior remains
backend work;do not replace the original payload with an invented gradient.

AFTER texture initialization/loading,reload GP and raw GP+1c AGAIN. Convert
unsigned word to single q2 via original bias. Request C_MTXLightOrtho8029e61c
with output local P and float-register arguments:

| Register | Exact requested value |
| --- | --- |
| f1 | -2 (c0000000) |
| f2 | +2 (40000000) |
| f3 | -2 |
| f4 | +2 |
| f5 | .5 (3f000000) |
| f6 | .5 |
| f7 | .5 |
| f8 | R32(q2/50),divisor42480000 |

These are eight raw ABI float arguments,not an assumed projection equivalent.
The called orthographic builder's complete scalar formula/rounding is still a
numeric dependency. Keep this second read independent of the pulse read across
called texture effects;no equality/once-per-frame snapshot is guaranteed locally.

C_MTXLookAt8029e330(local V,&captured eye,&captured up,&captured target).
PSMTXConcat(P,V,P) with output aliasing FIRST input,as specified in the
[concat contract](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md). This block does
NOT concatenate the caller's captured model matrix;its synthetic view/ortho
requests are its own coordinate construction. LookAt vector math/numeric backend
still requires recovery/validation before declaring exact projected pixels.

## Matrix, stage and KColor publication

1. matrixID=word[802bf9f0+4*B];GXLoadTexMtxImm(P,matrixID,0).
2. Reread indexed ID;GXSetTexCoordGen2(C,0,0,matrixID,0,125).
3. GXSetTevOrder(K,C,B,255).
4. GXSetTevColorOp(K,0,0,0,1,0);GXSetTevAlphaOp(K,0,0,0,1,0).
5. GXSetTevColorIn(K,8,0,14,15);GXSetTevAlphaIn(K,7,7,7,0).
6. GXSetTevSwapMode(K,0,0).
7. Initialize local raw KColor from8041f8f8=000000ff;store low8(D) to EACH of
   RGB bytes,retain seed alpha255. GXSetTevKColor(1,&local).
8. GXSetTevKColorSel(K,13);K++,B++,C++;I unchanged.

Original ID table has ten entries30,33,..57 and no local B bound. Matrix load
precedes generator request here,unlike projected-shadow flag0x20000. No object/
state restoration or lifetime retention is implemented by this local block.
When terminal color stage also ran,this block overwrites KColor1 again AFTER
that stage was configured. There is no local snapshot/material isolation of
KColor1. Its shared register/state semantics matter to earlier selected stages;
do not assign each stage an independent copy of the earlier color request.

Continue to [final callback/count publication](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md).
Two depth middle branches0x10000/0x40000 remain unrecovered;source/GX/pool/cache/
lifetime/clock/libm/Ortho/LookAt numeric and original images remain open. Reusing
original texture payload and seed vectors does not itself complete procedural
shader/camera behavior or final full-game requirements.
