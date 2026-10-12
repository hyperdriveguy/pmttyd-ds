# Inner draw admission,GX requests and evaluated-buffer publication

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
_animPoseDrawMtx0x8003bc68,size0x7e8 and mapTestXLU0x8001ca2c,size0xa8.
Inner inputs RECORD POINTER P r3,M r4,original pass V r5,A f1,S f2.
[Wrapper](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md) translates ID to pointer.
No original instructions executed;GX calls are requests,not validated images.

## Initial capture and mode gates

Capture current resource D,animation descriptor T,current sample index c=P+28,
next n=P+2c and fraction t=P+30 before callbacks/cache-miss evaluation.
T==null returns immediately. Then CURRENT W+10c nonzero and P+c==0 returns;
CURRENT W+10c zero and P+c==1 returns. Other pose modes are not rejected by
these two exact tests. Apply ORIGINAL pass/resource admission exactly as in
wrapper,including V2/resource mask1 exception. Rejection skips all later work.

## Translucency classifier and effective GX pass

Copy RAW P+f4 to local color word C2,P+f0 to local C1. Call
mapTestXLU(P+e8,&C1,&C2) at8003bd5c. The leaf returns1 if first admitted
condition succeeds:

1. Flags mask0x40 SET AND unsigned byte C1+3 !=255.
2. Unsigned byte C2+3 !=255 (regardless flags).
3. Flags mask0x800 CLEAR AND current map-group halfword+4 mask2 SET AND
   unsigned byte group+19 !=255.
4. Flags mask0x1000 CLEAR AND freshly loaded current map-group halfword+4 mask4
   SET AND unsigned byte group+1d !=255.

Otherwise0. Map base through0x8041e660,index0x8041e664,record stride0x2f4.
No map/index/null validation,no color mutation,no float conversion or callback.
It reloads group state separately for the last two predicates. Return nonzero
forces effective GX pass E=3;zero retains original V. Resource admission is NOT
repeated with E3,so a resource without bit8 can reach effective translucent state.

If P+e0 callback nonnull,call it with E as its sole prepared integer argument.
Skip built-in GX requests AND W+e0 publication. Otherwise built-in sequence:

| E | GXSetZMode | GXSetBlendMode | GXSetZCompLoc | GXSetAlphaCompare |
| --- | --- | --- | --- | --- |
| 1 | (1,3,1) | (0,4,5,0) | (1) | (7,0,0,7,0) |
| 0 or2 | (1,3,1) | (0,4,5,0) | (0) | (6,128,1,0,0) |
| 3 | (1,3,0) | (1,4,5,0) | (1) | (7,0,0,7,0) |
| other | no local GX calls | no local GX calls | no local GX calls | no local GX calls |

After built-in branch ALWAYS publish CURRENT W+e0=E,including other values.
No original state snapshot/restoration;SDK hardware/emulator behavior is separate.
Then perform [inner facing/matrix](ANIMATION_INNER_DRAW_FACING.md) and late
[cache-miss/ring/Main](ANIMATION_DRAW_SHARED_BUFFERS_AND_PAPER_SELECTION.md).

## Global context,cache flush and traversal

Set modeling level0 and pointer0x803d8e70;Copy(effective M,modelingBase).
Publish CURRENT W+18=P,W+1c=captured T,W+20=captured D,separate W reloads.
If CURRENT W+e8!=0:flush W+f8 for12*CURRENT resource positionCount,
then W+fc for12*CURRENT resource auxiliaryCount. Otherwise flush P+4c then
P+54 for those CURRENT resource lengths. Calls DCFlushRange0x80296a54 are
unconditional with respect to count;there is no local zero/null/negative guard.
Re-resolve resource after first flush;do not assume captured D counts for both.
Call GXInvalidateVtxCache0x802b4df4 after both flushes.

Finally request dispProc(parent=−1,group=word[CAPTURED D+144]−1,
current=captured c,next=captured n,pass=ORIGINAL V,f1=captured t) at8003c420.
Effective GX pass E is not substituted for traversal's original V. A preceding
Main cache miss does not refresh these captured sample/resource values.
Group/node/render/material scheduling is specified in
[group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md);render/material
bodies and SDK matrix push bodies remain separate. No inner post-traversal
cleanup/restoration exists here beyond function epilogue.

`verify_animation_draw.py` pins five complete bodies and148 direct references;
mapTestXLU has exactly one direct DOL caller in this census. Call counts are not
original runtime frequency;global map/context/callback/alias/cache/GX/source
lifetime and original visual domains remain open.
