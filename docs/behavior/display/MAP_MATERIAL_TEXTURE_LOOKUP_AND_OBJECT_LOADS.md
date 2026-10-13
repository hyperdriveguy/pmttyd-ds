# Map material reverse texture slots, exact name lookup and object loads

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original instructions/GX execution or observed images.
`tools/verify_map_material_source.py` checks six complete local bodies,806
reviewed words,six binary32 constants,six original spans,two US lecture imports
and five direct DOL sites. Loader/source/GX/numeric/lifetime/runtime remain open.

Complete texture loop in mapSetMaterial80020e54/0x614. Capture initial
U8 material M+0b count N;iterate slot i=N-1 down0,loaded map j=0 up. N0 skips.
Slot D=word[M+0c+4*i];desired image name=word[word[D+0]+0]. Each slot resolves
CURRENT map group G and signed S=I16[object O+de]. Capture name table
Q=word[G+0x178*S+94]. Scan t ascending with signed t<word[Q+0],rereading count
at tests;strcmp(word[Q+4+4*t],desired name). First equality retains t;exhaustion
t=-1. No name/table/slot/count guard or diagnostic/fallback.

Reread signed O+de AFTER lookup;using retained G,load palette payload
word[G+0x178*S+84]. TEXGet(payload,t)80037b5c returns word[payload+8]+8*t without
index guard. Miss passes-1 DIRECTLY;this differs from animation helper's fallback0.
Capture returned pair,image header=word[pair+0]. The pair's palette-header+4
is not consulted;this caller always uses ordinary GXInitTexObj,not CI/TLUT path.
No local format normalization/CI error handling or asset lifetime is supplied.

Image fields:U16+2 width,U16+0 height,word+4 format,word+8 data,U8+21/+22 LODs,
float+1c bias,U8+20 edge LOD. Mipmap=(byte+21!=byte+22). WrapS=table[U8 D+8],
WrapT=table[U8 D+9],three-word table802bfa44=[0,1,2]. No wrap index bounds;
image header wrap fields+0c/+10 are not used here.

Exact requests:GXInitTexObj(local,data,width,height,format,wrapS,wrapT,mipmap);
fresh image-header reads from captured pair→GXInitTexObjLOD(local,minFilter1,
magFilter1,float(U8+21),float(U8+22),bias,clamp0,edgeLOD U8+20,aniso0);
GXLoadTexObj(local,j). Min/mag filters are FORCED1/1,not header+14/+18 as in
animation. Unsigned byte conversion bias802bfc20=4330000000000000. Then j++,
slot pointer-=4,i--;no resource/source writes or cache/retention/state restore.

Finally [TEV source](MAP_MATERIAL_ENTRY_CHANNELS_AND_TEV_INPUTS.md) rereads current
count M+0b instead of reusing N. Texture coordinate matrices are a SEPARATE
caller-requested helper;this loop does not request them. Original map loader/
relocation/signed slot domain/name-table/palette/format/wrap/count/sampler/source
scene/cache/GX lifetime/image comparisons remain open. Do not import animation
fallback/LOD/CI/UV behavior merely because both consume TEXGet image headers.
