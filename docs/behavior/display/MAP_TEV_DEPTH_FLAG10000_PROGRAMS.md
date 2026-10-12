# Depth flag0x10000 admission and six exact stage programs

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original CPU/GX execution or observed shadow images.
`tools/verify_map_tev_depth.py` checks1115 newly reviewed depth/source words,
eight identities(one containing TEV/seven complete source leaves),six original
spans,two US lecture imports,six direct DOL sites and65536 host byte products.
It also rechecks six contributing manifests covering all2927 TEV words. This
supports composed local REQUEST contracts,not SDK/GX/source/runtime equivalence.

Complete local block80023314..80023d58 exclusive. F=retained flags,
model=retained input matrix,K=running TEV stages,B=texture maps,C=coordinate
generators,I=indirect stages. Clear F&0x10000 skips. Set:first depth texture getter;
NULL skips. Otherwise SECOND getter→captured T (unchecked),depth matrix getter
→captured D,projection matrix getter→captured P,depth color getter→captured L,
config getter→pointer,then signed route=config word+3c. This call/read order occurs
before route selection,including invalid routes. See
[source leaves](MAP_TEV_DEPTH_SOURCE_QUERIES_AND_CONFIG.md).

| Signed route | Complete entry | Program / increments |
| --- | --- | --- |
| 0 | 80023384 | Three stages,Ramp8+D then T+P;K+3,B+2,C+2 |
| 1 | 80023640 | One debug stage,Ramp8+D;K+1,B+1,C+1 |
| 2 | 80023740 | One debug stage,T+P;K+1,B+1,C+1 |
| 3 | 80023840 | Three stages,Ramp16+D then T+P;K+3,B+2,C+2 |
| 4 | 80023b2c | One debug stage,Ramp16+D;K+1,B+1,C+1 |
| 5 | 80023c44 | One debug stage,T+P;K+1,B+1,C+1 |
| Negative or>=6 | — | No stage/coordinate/texture requests or counter increments after the earlier getters |

All routes retain I. Initial US config records BOTH route3;other routes are
complete executable branches,not claimed actually witnessed US scenes.

## Shared coordinate/texture/matrix sequence

For each textured stage with map b,coordinate c and source H (D or P):read
id=word[802bf9f0+4*b];GXSetTexCoordGen2(c,0,0,id,0,125);obtain Ramp getter result
or use captured T;GXLoadTexObj(texture,b);PSMTXConcat(H,captured model,local);
reread id;GXLoadTexMtxImm(local,id,0). This occurs AFTER that stage's shader
requests below. No ten-ID-table bound,second T NULL check,source/matrix guard or
state restore. For three-stage routes,first textured stage uses original B/C,
second uses B+1/C+1;B increments before second-stage Order. Both textured setups
complete before the untextured resolve stage and final publication.

## Three-stage route0/3 program

FIRST rewrite preceding stage:ColorOp(K-1,0,0,0,1,1),then
AlphaOp(K-1,0,0,0,1,1). Route3 additionally sets shared SwapModeTable1=(0,3,2,3),
then SwapModeTable2=(3,1,2,3),before any appended-stage requests. No restoration.

Each stage follows exact request order Order,ColorIn,ColorOp,AlphaIn,AlphaOp,
SwapMode,except resolve's KColor operations occur between ColorOp and AlphaIn.
Tuple Op below is(op,bias,scale,clamp,out);input tuples are A,B,C,D;swap tuple is
(raster,texture). All numbers are original raw request IDs,not physical register
ordinals or inferred GPU equations.

| Stage | Order(coord,map,channel) | ColorIn | ColorOp | AlphaIn | AlphaOp | Swap route0 / route3 |
| --- | --- | --- | --- | --- | --- | --- |
| K | C,B,255 | 15,15,15,8 | 0,0,0,1,0 | 7,7,7,7 | 0,0,0,1,0 | 0,0 /0,1 |
| K+1 | C+1,B+1,255 | 0,8,12,15 | 8,0,0,0,0 (route0);10,0,0,0,0 (route3) | 7,7,7,7 | 0,0,0,1,0 | 0,0 /0,2 |
| K+2 | 255,255,255 (route0);255,255,4 (route3) | 15,14,0,2 | 1,0,0,1,0 | 7,7,7,1 | 0,0,0,1,0 | 0,0 /0,0 |

At stageK use Ramp8/D for route0,Ramp16/D for route3 and the shared matrix
sequence. At stageK+1 use captured T/P and that same sequence. Before resolve
Order,increment B again and C by2. Resolve ColorOp is followed by
GXSetTevKColor(1,&captured L),KColorSel(K+2,13),then AlphaIn/AlphaOp/Swap.
No KAlphaSel request on these routes. Finally K+=3,exit to80023d58.
Do not collapse operation8/10,clamp0,route0's channel255 or route3's swaps into
one interchangeable depth compare. SDK/GPU comparison/rounding remains backend.

## One-stage debug routes1/2/4/5

First GXSetTevColor(2,&original seed):route1/2 seedff000000 from8041f8e4/e8;
route4/5 seedffff0000 from8041f8ec/f0. This is raw Color request ID2,not KColor2.
Route4 THEN SwapModeTable1=(3,1,2,3);route5 THEN SwapModeTable1=(0,3,2,3).
No such table rewrite in1/2. At stageK:
Order(K,C,B,255),ColorIn(K,15,8,4,15),ColorOp(K,0,0,0,1,0),
AlphaIn(K,7,7,7,0),AlphaOp(K,0,0,0,1,0),SwapMode(K,0,route>=4?1:0).
Then shared texture/matrix sequence as selected in route table;K/B/C each++;
exit80023d58. Captured L is not published by these routes,but its getter ran.

Both this and later [alternate depth branch](MAP_TEV_DEPTH_FLAG40000_PROGRAMS.md)
can run in one call,with live source getters anew and accumulated counters.
Caller scheduling/flags/config writers/capture/lifetime/SDK/GX/numeric/images
remain separate full-system dependencies. No original frame/tick gate exists.
