# Alternate depth flag0x40000 and two exact three-stage programs

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original CPU/GX execution or observed shadow images.
`tools/verify_map_tev_depth.py` checks1115 newly reviewed depth/source words,
eight identities(one containing TEV/seven complete source leaves),six original
spans,two US lecture imports,six direct DOL sites and65536 host byte products.
It also rechecks six contributing manifests covering all2927 TEV words. This
supports composed local REQUEST contracts,not SDK/GX/source/runtime equivalence.

Complete block80023e9c..800244ac exclusive runs AFTER projected-shadow0x20000
and BEFORE terminal color. Clear F&0x40000 skips. Set:first depth texture getter;
NULL skips. Otherwise second getter→captured T (unchecked),depth matrix→D,
projection matrix→P,depth color→L,config getter→pointer,signed route word+3c.
Only route0 and3 proceed;all other signed values skip after these source reads.
These are FRESH getters,not reused from earlier0x10000 even when both flags set.
[source leaves](MAP_TEV_DEPTH_SOURCE_QUERIES_AND_CONFIG.md) specify their complete
local results and initial source records(route3 in BOTH records).

K/B/C are CURRENT accumulated counters;I retained. This branch appends three
stages,two maps and two generators,then K+=3. It does not force a fixed global
K/B/C or clear prior states. First rewrite ColorOp(K-1,0,0,0,1,1),then
AlphaOp(K-1,0,0,0,1,1). Route3 sets SwapModeTable1=(0,3,2,3),then
SwapModeTable2=(3,1,2,3). Route0 does not reset those retained shared tables.

## Exact per-stage requests

Order is Order,ColorIn,ColorOp,AlphaIn,AlphaOp,SwapMode for EACH row. Op tuple is
(op,bias,scale,clamp,out);inputs A,B,C,D;swaps(raster,texture),all raw IDs.

| Stage | Order(coord,map,channel) | ColorIn | ColorOp | AlphaIn | AlphaOp | Swap route0 /route3 |
| --- | --- | --- | --- | --- | --- | --- |
| K | C,B,255 | 15,15,15,8 | 0,0,0,1,0 | 7,7,7,7 | 0,0,0,1,0 | 0,0 /0,1 |
| K+1 | C+1,B+1,255 | 0,8,12,15 | 8,0,0,0,0 (route0);10,0,0,0,0 (route3) | 7,7,7,7 | 0,0,0,1,0 | 0,0 /0,2 |
| K+2 | 255,255,4 | 2,14,0,15 | 0,0,0,1,0 | 7,7,7,6 | 0,0,0,1,0 | 0,0 /0,0 |

After first row shader requests:
read id=table[B];GXSetTexCoordGen2(C,0,0,id,0,125);Ramp8 getter(route0) or
Ramp16 getter(route3);GXLoadTexObj(ramp,B);Concat(D,model,local);reread id;
GXLoadTexMtxImm(local,id,0). table=word entries at802bf9f0,ten IDs30,33,..57.

Before second-row Order set second coordinate=C+1 and B++ (first map consumed).
After its shader requests:read id=table[B];Gen(second coordinate,0,0,id,0,125);
GXLoadTexObj(captured T,B);Concat(P,model,local);reread id;LoadMtx(local,id,0).
Before third-row Order,B++ again and C+=2;Order triple is now untextured.
After ALL third-row shader requests,set GXSetTevKColor(1,&captured L),
GXSetTevKColorSel(K+2,13),GXSetTevKAlphaSel(K+2,29);finally K+=3.

The third row differs from
[flag0x10000 resolve](MAP_TEV_DEPTH_FLAG10000_PROGRAMS.md):ColorIn(2,14,0,15),
ColorOp0 instead of1,AlphaIn final6 instead of1,and explicit KAlphaSel29;
KColor publication occurs AFTER AlphaOp/Swap rather than between ColorOp and
AlphaIn. Do not unify these two flags solely because initial ramp/depth setup
looks identical. Each retains source captures and shared register/table writes.

No local table/texture/matrix/clock guard,state restore or frame gate. Final
terminal/callback/count stages still run. The synthetic/real source capture
configuration,original selected scenes,GX/SDK/cache/resource/lifetime/numeric/
image parity remain open despite complete local request graph recovery.
