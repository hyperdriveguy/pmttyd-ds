# Map material entry, lighting channels and actual TEV source inputs

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original instructions/GX execution or observed images.
`tools/verify_map_material_source.py` checks six complete local bodies,806
reviewed words,six binary32 constants,six original spans,two US lecture imports
and five direct DOL sites. Loader/source/GX/numeric/lifetime/runtime remain open.

Complete mapSetMaterial80020e54/0x614 takes object O r3,material M r4.
Capture both pointers;fallback ambient raw8041f908=000000ff. Call
[mapSetLight](MAP_MATERIAL_LIGHT_SELECTION_AND_NAMES.md)(O,&local eight-pointer
list);retain returned signed count L. Initialize attenuation selector A=2,
mask B=0. Iterate selected entries in original order;no independent null/array/
valid-count guard. [Per-light construction](MAP_MATERIAL_LIGHT_OBJECT_REQUESTS.md)
specifies exact type and numeric requests;B ORs slot ID even on unrecognized type.

After loop,lightGetAmbient8001ba60:nonnull→raw returned+48;NULL→fallback.
GXSetNumChans(1);GXSetChanAmbColor(4,ambient);GXSetChanMatColor(4,raw word[M+4]).
No conditional material-color omission. Material U8+8 selects channel policy:

| U8 M+8 | Exact remaining channel requests |
| --- | --- |
| 0 | ChanCtrl(0,e,0,0,B,2,A);ChanCtrl(2,0,0,0,0,0,2) |
| 1 | ChanCtrl(0,e,0,1,B,2,A);ChanCtrl(2,0,0,1,0,0,2) |
| Other | No local ChanCtrl request,retain previous controls |

Here e=unsigned32(-L & ~L)>>31,which equals L!=0 on valid nonnegative count
range. ChanCtrl arguments are channel,enable,ambient source,material source,
light mask,diffuse function,attenuation function. A starts2 and becomes sticky1
on point/spot branches;directional preserves it. Alpha-channel diffuse argument
is0 here,unlike the previously recovered character-light helper's2.

Then [texture objects](MAP_MATERIAL_TEXTURE_LOOKUP_AND_OBJECT_LOADS.md) load in
reverse material slot order. Finally mode=U8[word[M+110]] if that pointer nonNULL,
else0. Call mapSetMaterialTev(U8[M+0b],mode,word[O+0],O+1c) at80021448.
This is a concrete ORIGINAL source binding:slot count from material byte0b,
mode from descriptor's first byte,flags from RAW object word0,model MATRIX
ADDRESS object+1c. No generated default flag/mode replacement or enum bounds.
[Whole TEV request graph](MAP_TEV_LOCAL_REQUEST_GRAPH_AND_BACKEND_BOUNDARIES.md)
specifies downstream local decisions;actual material/object writers and scene
invariants still need recovery.

This body does NOT call mapSetMaterialLight,mapSetMaterialLastStageBlend,
mapSetMaterialFog or mapSetTextureMatrix. It does not clear fog or load model/
normal GX matrices. Those services/state are caller-owned;do not mechanically
reuse the animation default-material sequence. Returned GX state is retained.

Actual direct DOL callers:_mapDispMapObj80026b20,makeDisplayList8002a370.
The separate texture-matrix helper is called by _mapDispMapObj80026460/800267fc.
These sites show shipped map/display-list dependencies,not original scene/
frequency/image or caller admission completion. Display-list/live-state capture,
map loader/relocation/slots/bounds,source flags/callback/cache/pool/lifetime,
GX/SDK/numeric and original comparisons remain open implementation requirements.
