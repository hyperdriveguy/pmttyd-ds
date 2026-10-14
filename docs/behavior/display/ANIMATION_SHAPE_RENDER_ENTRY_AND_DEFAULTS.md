# Shape render/material entry and default material requests

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
renderProc0x8003c884,size0xd88 and materialProc0x8003d60c,size0x6f0.
No original execution. These local request graphs are split across five files;
called texture/map/GX/SDK/resource services remain separate.

Inputs shape ID r3,second integer argument r4. BOTH bodies retain shape ID but
do not consume/preserve the second argument as an animation frame. The
[group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md) passes its captured
current-key index there;that caller value does not become a used operand in
these bodies. Shape==−1 returns BEFORE camera/resource/GX work. Other negative/
out-of-range IDs have no local guard.

## Matrix requests precede shape admission

Request camGetCurPtr0x8000f2e4;Concat(camera+11c,current modeling matrix through
0x8041e8a0,local ViewModel). GXLoadPosMtxImm(ViewModel,0).
Request PSMTXInvXpose(ViewModel,localNormal)0x8029df28,ignore returned status;
GXLoadNrmMtxImm(localNormal,0),GXSetCurrentMtx(0). No local singular/identity
fallback. The [normal-matrix local graph](ANIMATION_SDK_NORMAL_AND_POINT_MATRICES.md) is
recovered; its reciprocal-estimate/numeric provider remains open. This uses CURRENT
camera,not an unconditional camGetPtr(4) or projection matrix+15c.

W through0x80415dc0;capture D=word[W+20]. Shape H=word[D+14c]+168*ID;
first draw B=word[D+19c]+108*word[H+98];capture signed drawCount H+9c.
Read shape pass M=word[H+a0]. Admission succeeds if:

- M==0 AND current W+e0==2;OR
- M==current W+e0;OR
- current pose alpha byte+f3 !=255;OR
- current pose alpha byte+f7 !=255.

Otherwise return after the earlier matrix/GX requests. W+e0 is the effective
GX pass from [inner draw](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md),not the
original traversal pass. No validity/type bounds for M. On admission read
cull table word[0x80415dc8+4*word[H+a4]],request GXSetCullMode. Table words in
index order are2,1,3,0;no index clamp. Original Mario shapes all have index3.

Each draw iterates while signed i<captured drawCount,advancing B by108.
Nonpositive count skips draw-specific material/geometry work but prior GX setup
still happened. Renderer additionally captures shape base offsets and eight UV
array bases before this loop;details in
[arrays/FIFO](ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md).

## Per-draw default guard and origin/light

Read CURRENT pose P=word[CURRENT W+18]. If word[P+e4]!=0,skip ALL default
material requests below and continue override scans. This is a raw zero/nonzero
guard;neither body calls it as a function pointer. No host shader interpretation
is inferred from the field name.

If P+e4==0:

1. Copy original seed vector(0,0,0) from802c103c(Render)/802c1030(Material),
   request PSMTXMultVec(currentModelMatrix,&seed,&seed)0x8029e6a4 with input/
   output alias. Copy returned words to another local vector.
2. Reload CURRENT global pose;mapSetMaterialLight(current P+ec,&copiedVector)
   0x800208c0. No direct lighting calculation occurs here.
3. Request animSetMaterial_Texture0x8003dcfc using exact ABI:
   r3=word[B+0] (texture slots),r4=B+10 (slot IDs),
   r5=word[current D+194] (base texture records),
   r6=word[current P+6c] (evaluated texture state),
   r7=word[current D+134] (texture-data count),
   r8=word[current D+198] (texture data),
   r9=word[current W+8]+12*word[currentResourceRecord+0c].
   ResourceRecord=word[W+0]+16*word[current P+10]. No local pointer/count guard.
4. Copy RAW current P+f4 to local C2,then current P+f0 to C1;request
   mapSetMaterialLastStageBlend(current P+e8,&C1,&C2)0x80021860.
   No local color conversion/clamp;global pose fields are reloaded separately.
5. Request camGetPtr(4),retain pointer;request camGetCurPtr AGAIN. Only pointer
   equality requests mapSetMaterialFog0x800217c8. Do not reuse the entry camera.
6. Request mapSetMaterialTev(slotsCapturedAtDrawEntry,word[B+8],CURRENT P+e8,
   CURRENT modeling matrix)0x80021b80.

These helpers may publish GX state;their internal texture/index/lighting/fog/
TEV semantics and source/lifetime domains are not completed here. Continue
[override selection](ANIMATION_RENDER_TEXTURE_OVERRIDE_SELECTION.md),
[last-stage requests](ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md),then renderer
arrays/emission or material-only next draw. There is no state snapshot/restore
on normal return. Matrix/default/override steps preserve their original order.

## Concrete bindings and verification

renderProc actual direct DOL callers:animPoseDrawShape80039768 and ordinary
Disp8003c804. materialProc actual direct caller:special Disp8003c6ec.
The latter special path prepares material before vivihimo,then skips ordinary
render/child work. Calling materialProc does not emit a vertex stream locally.
`verify_animation_render_material.py` pins both complete bodies,1310 words,
five spans,two US lecture imports,three direct sites and an original Mario
shape/draw resource example. Static evidence does not prove executed images,
all US source domains or full resource/GX/backend closure.
