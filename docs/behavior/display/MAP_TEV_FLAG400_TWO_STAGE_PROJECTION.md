# TEV flag0x400 projection and preceding-stage output rewrite

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis,no original instructions/GX calls executed or pixels observed.
`tools/verify_map_tev_projected.py` checks one PARTIAL containing TEV identity,
four complete projected-shadow getter bodies,233 reviewed words,three original
spans,two US lecture imports and six direct DOL sites. Full TEV still has four
unrecovered middle branch internals and backend/source/runtime gaps.

Complete local block80023128..80023314 exclusive belongs to mapSetMaterialTev
80021b80/0x2dbc. Let K=running TEV stage count,B=texture-map count,C=coordinate
generator count,I=indirect count,model=captured incoming matrix,F=captured flags.
It runs after [constant/character shadow](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md)
and before flag0x10000. F&0x400 clear skips all its requests/writes. Set admits
without texture/matrix/holder availability checks or a second frame/tick gate.
All recovered base paths initialize K>=1;no separate local K-1 guard exists.

## Rewrite and projected coordinate setup

1. GXSetTevColorOp(K-1,0,0,0,1,1),then
   GXSetTevAlphaOp(K-1,0,0,0,1,1). This rewrites preceding stage OUTPUT ID1;
   it does not append a duplicate of its previous requests or change its inputs.
2. PSMTXConcat(address8041e680,captured model,local). The first operand is a
   GLOBAL matrix address (`projMtx` annotation),not a pointer stored there.
3. Read matrixID=word[802bf9f0+4*B];GXSetTexCoordGen2(C,0,0,matrixID,0,125).
4. Reread the indexed ID;GXLoadTexMtxImm(local,matrixID,0).
5. smartTexObj(address8041e6b4,word[8041e6b0]);GXLoadTexObj(returned object,B).

Original ten-ID table is[30,33,36,39,42,45,48,51,54,57],with no local B bound.
8041e6b4 is the embedded global object ADDRESS;8041e6b0 stores a HOLDER pointer,
not the image address itself. [smart patch](ANIMATION_SMART_TEXTURE_DATA_PATCH.md)
preserves object identity and conditionally dereferences that holder to patch
object+0c. No allocation/null-result fallback is introduced here. Model/global
matrix numeric behavior is supplied by
[the concat contract](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md);resource
initialization/cache/lifetime and GX packing remain open.

## First appended stage and retained swap table

At stage K,in exact order:

1. GXSetTevOrder(K,C,B,255).
2. GXSetTevColorOp(K,0,0,0,1,0).
3. GXSetTevAlphaOp(K,0,0,1,1,0). Its scale argument is1,unlike color scale0.
4. GXSetTevColorIn(K,15,15,15,4).
5. GXSetTevAlphaIn(K,7,7,7,4).
6. GXSetTevSwapMode(K,3,3).
7. GXSetTevSwapModeTable(3,0,1,2,0)802b8ce0.
8. GXSetTevColor(2,&raw seed)802b8ab8,seed8041f8e0=000000ff.

The seed is raw RGBA bytes0,0,0,255;this is a TEV COLOR REGISTER2 request,
not KColor2. Swap table3 is shared retained GX state;there is no local restoration.
Literal input/output/swap IDs are original request enums,not a proved host
shader equation or measured blend/alpha effect. Other users of table3 can observe
its rewrite according to original backend state ownership.

## Second appended stage and counters

Set s=K+1. Prepare GXSetTevOrder(s,255,255,255);increment B and C BEFORE its
call (no subsequent coordinate request uses these increments locally). Then:
ColorOp(s,0,0,0,1,0);AlphaOp(s,0,0,0,1,0);
ColorIn(s,2,0,1,15);AlphaIn(s,7,7,7,1);SwapMode(s,0,0).
Finally K+=2;I unchanged;fall through to flag0x10000 test80023314.
The first appended stage plus second resolve the rewritten preceding output
through explicit original register routing. Do not replace this with one extra
texture multiplication or leave previous-stage output unchanged.

Initial global matrix/object/holder occupy SBSS source storage:8041e680 matrix
0x30 bytes,8041e6b0 holder slot4 bytes,8041e6b4 object0x20 bytes by annotations.
Their runtime setters/valid source domains are not recovered by this consumer.
No serialized texture/image/matrix payload is invented from zero-initialized
storage. Actual US scene/pose/material flags,backend numeric/images/capacity/
cache/lifetime and all remaining shader branches remain required.
