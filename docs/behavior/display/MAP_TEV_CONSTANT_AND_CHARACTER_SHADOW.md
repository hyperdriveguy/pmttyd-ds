# TEV constant-color and character-shadow stages

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original execution or image parity. Evidence tool
`tools/verify_map_tev_ordinary.py` checks one containing TEV body identity,four
complete shadow getter bodies,303 reviewed words,three original spans,two US
lecture imports,six direct DOL sites and original Mario draw-mode metadata.
The containing body hash does NOT claim complete semantics for every mode.

These are complete local blocks80022f48..80023128 of mapSetMaterialTev.
Incoming running counters K=TEV stages,B=texture maps,C=coordinate generators,
I=indirect stages;for ordinary mode0,N>0 initially K1,B=C=N,I0.
No local original frame/tick gate or restored GX state.

## Constant stage, first

If F&0x10000000 SET,skip this stage. If CLEAR,append at stage K in this exact
order:GXSetTevKColorSel(K,12);GXSetTevKAlphaSel(K,28);
GXSetTevOrder(K,255,255,255);GXSetTevColorOp(K,0,0,0,1,0);
GXSetTevAlphaOp(K,0,0,0,1,0);GXSetTevColorIn(K,15,0,14,15);
GXSetTevAlphaIn(K,7,0,6,7);GXSetTevSwapMode(K,0,0);K++.
No B/C/I increments. K-color selectors read retained GX state;the preceding
[color helper](MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md) supplies KColor0 when
called by ordinary animation default setup. Do not erase this stage just because
base mode0 used one texture. Its raw inputs/selectors are necessary backend
requirements;integer GPU arithmetic remains unverified.

## Character shadow, second

Only F&4 admits probing. Call shadowGetCharShadowProjection8002bc7c;zero skips.
Then shadowGetCharShadowTexObj8002bca0;NULL skips. Otherwise:

1. Capture raw color word from shadowGetCharShadowColor8002bc88.
2. Get shadowGetCharShadowProjTexMtx8002bc94;Concat(shadow matrix,model,local).
3. Matrix ID=word[802bf9f0+4*B]. Pinned ten-word table is
   [30,33,36,39,42,45,48,51,54,57]. No B range check.
4. GXSetTexCoordGen2(C,0,0,matrixID,0,125);GXLoadTexMtxImm(local,matrixID,0).
5. Call shadowGetCharShadowTexObj AGAIN;GXLoadTexObj(returned pointer,B).
   This second result is not checked for NULL;do not reuse the first object.
6. GXSetTevOrder(K,C,B,255);ColorOp/AlphaOp(K,0,0,0,1,0);
   ColorIn(K,0,14,8,15);AlphaIn(K,7,7,7,0);SwapMode(K,0,0).
7. GXSetTevKColor(1,&captured color);GXSetTevKColorSel(K,13).
8. K++,B++,C++;I unchanged.

Model pointer remains captured at function entry;shadow source values/getter
results are live at their call sites. Color is captured before concat/texture
requests. No local counter saturation,texture retention or valid-table-index
proof is supplied. PSMTXConcat local arithmetic is documented separately in
[the matrix contract](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md);GX projection,
shadow capture/update and observed image behavior remain open.

## Four complete shadow source leaves

Global pointer cswp=word[804150e0],initial DOL pointer803c9e38. These leaves have
no separate null-work validation. Projection returns word[cswp+10c] (0x0c bytes).
Color returns word[cswp+100] (0x0c). Projection-matrix getter returns cswp+7c
(0x0c). Texture getter (0x40 bytes) tests word[cswp]&1:clear→NULL;
set→smartTexObj(cswp+dc,word[cswp+fc]),returning that same embedded object.
The second smart argument is a HOLDER pointer,as proven in
[the data-patch contract](ANIMATION_SMART_TEXTURE_DATA_PATCH.md);word[cswp+fc]
is not treated as the image address itself. Each getter rereads cswp.

Projection nonzero alone does not enable texture access when flag1 is clear.
The texture guard and later getter can observe different work/flag/holder state.
These local leaves do not initialize the buffer,projection mode,color,holder or
matrix;shadow constructor/capture/draw ownership and actor admission are separate.
The branch specifies requested projected texture composition,not a generic blob
shadow asset or a complete actor-shadow implementation.
