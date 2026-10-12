# TEV flag0x20000 projected shadow and live getter capture

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis,no original instructions/GX calls executed or pixels observed.
`tools/verify_map_tev_projected.py` checks one PARTIAL containing TEV identity,
four complete projected-shadow getter bodies,233 reviewed words,three original
spans,two US lecture imports and six direct DOL sites. Full TEV still has four
unrecovered middle branch internals and backend/source/runtime gaps.

Complete block80023d58..80023e9c exclusive runs after flag0x10000's branch
and before flag0x40000. It uses retained F,model and running K/B/C/I. Clear
F&0x20000 skips. Set calls shadowGetProjShadowTexObj8002bafc;NULL skips. If the
first object nonNULL,call that getter AGAIN and capture its return as texture T,
without checking T. Then get/capture projection-matrix pointer H through
shadowGetProjShadowProjTexMtx8002baf0,raw color through
shadowGetProjShadowColor8002bae4,and call shadowGetProjShadowConfig8002bac8.
The CONFIG RETURN IS IGNORED locally. Preserve its source reads/call even though
its returned pointer is not consumed. Captured T/H/color can come from distinct
live work-pointer states;do not collapse all getters into one work snapshot.

## Exact ordered shader/matrix publication

1. GXSetTevOrder(K,C,B,255).
2. GXSetTevColorIn(K,0,14,9,15).
3. GXSetTevColorOp(K,0,0,0,1,0).
4. GXSetTevAlphaIn(K,7,6,4,0).
5. GXSetTevAlphaOp(K,0,0,0,1,0).
6. GXSetTevSwapMode(K,0,0).
7. Read matrixID=word[802bf9f0+4*B];GXSetTexCoordGen2(C,0,0,matrixID,0,125).
8. GXLoadTexObj(captured T,B). No new NULL check.
9. PSMTXConcat(captured H,captured model,local).
10. Reread indexed matrixID;GXLoadTexMtxImm(local,matrixID,0).
11. GXSetTevKColor(1,&captured raw color).
12. GXSetTevKColorSel(K,13);GXSetTevKAlphaSel(K,29).
13. K++,B++,C++;I unchanged. Fall through to80023e9c flag0x40000 test.

The order is ColorIn BEFORE ColorOp,AlphaIn BEFORE AlphaOp,and texture load
BEFORE matrix concatenation/load;the earlier
[character-shadow branch](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md) follows a
different request sequence. Do not reorder stages into a common convenience
helper without proving effects equivalent. Original matrix-ID table has ten
entries;B is unchecked locally. GX enums/depth/raster/cache/images and original
shader arithmetic remain called backend work,not an already faithful handheld
lighting adaptation.

## Four complete projected-shadow source leaves

Each independently loads pswp=word[804150e4],initial DOL pointer803c9d34.
There is no separate work-null guard:

| Getter | Address/size | Complete local result |
| --- | --- | --- |
| Config | 8002bac8/0x1c | 80304a60+72*word[pswp+8],low32 arithmetic,no index bound |
| Color | 8002bae4/0x0c | Raw word[pswp+100] |
| Projection matrix | 8002baf0/0x0c | pswp+7c address |
| Texture | 8002bafc/0x40 | If word[pswp]&1 clear,NULL;else smartTexObj(pswp+dc,word[pswp+fc]) |

Texture flag1 is the getter's only admission. On the admitted branch the smart
object is embedded at+dc;the slot+fc contains the HOLDER pointer consumed by
[smartTexObj](ANIMATION_SMART_TEXTURE_DATA_PATCH.md),which can mutate packed
image-address bits and always returns the same object. Thus this consumer calls
that potential data patch twice before retaining T. First success does not prove
second success if source work/flag changed between calls;the local code makes no
retry or post-capture validation. Initial pointer identity is metadata,not an
initialized or captured shadow texture image.

Config's source table annotation has0x90 bytes (two72-byte records);this getter
only forms a pointer and does not read table contents or enforce that extent.
Its scalar return is discarded here,but loading live pswp+8 still occurs.
Shadow constructor/config/scene selection/render capture/holder/matrix/color
writers and lifetime remain open dependencies. pswp is distinct from cswp used
by the earlier character-shadow getter path;do not share state solely because
embedded offsets look alike.

[Final callback/counts](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md) remain later
behavior. This delivery closes local projected-shadow REQUEST/source-query
semantics,not complete shadow capture/GX equivalence or actual US scene execution.
All source/pool/resource/cache/alias/numeric/images and four remaining middle
branches need recovery before final shader/full-game requirements closure.
