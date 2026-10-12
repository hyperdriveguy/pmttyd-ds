# Map material light selection and final channel requests

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis. `tools/verify_animation_texture_light.py` checks fourteen
complete local bodies,1189 instruction words,eleven binary32 constants,eight
original spans,two US lecture imports and41 direct DOL reference sites. These
are evidence counts,not full-game completion or execution coverage. No original
instructions or hardware images were executed for this recovery.

Complete mapSetMaterialLight800208c0/0x57c takes r3 flags F,r4 world query origin
V. It supplies animation/map caller lighting requests. The following selection,
[light construction](MAP_MATERIAL_LIGHT_TYPES_AND_TRANSFORMS.md) and final GX
channel sequence form one body;no original image equivalence is implied.

Capture F,V;initialize selected count L=0,attenuation function A=2,material
RGBA=8041f914=ffffffff,fallback ambient8041f918=000000ff.

## Ordered selection

1. F&1:append lightGetPaper;set L=1.
2. F&8:append lightGetPaperCraft.
3. F&0x10:append lightGetPaper AGAIN;no pointer deduplication.
4. F&2:call lightCheckCharaLight. If nonzero and signed L<8,call
   lightGetNearObj(V,list+L,8-L,0x80);add returned count to L.
5. If F&0x19 and L==1,choose fallback ambient priority F&1→word[8041e6d8],
   otherwise F&8→word[8041e6d4],otherwise8041f91c=000000ff.
6. If F&0x19 and L!=1,call lightGetNearObj(V,list,8,0x80) and REPLACE L/list.
   This discards previously appended presets/query results rather than appending.

When F&0x19 clear,there is no final replacement query. See
[query details](MAP_CHARACTER_LIGHT_QUERIES.md) for source order when at most8
active records,ambient exclusion,mask overlap and postwrite capacity checks.
Valid source/query capacity and preset lifetime are external invariants.

## Final ambient, fog and channel order

After per-light processing,call lightGetCharaAmbient. NonNULL overrides ambient
with raw returned record+48;NULL retains the selected fallback. Always request:
GXSetFog(0,materialRGBA,0,0,0,0);GXSetNumChans(1);GXSetChanAmbColor(4,ambient).
The fog reset is an actual effect even when no lights were selected. Later
[caller-admitted map fog](MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md) is separate.

Enable e is the literal word expression unsigned32(-L & ~L)>>31;for the valid
nonnegative count domain e=(L!=0). Accumulated mask B includes each selected
slot's original light ID,even if its type did not initialize/load a light.
A starts2 and becomes1 after a non-0x40 point attenuation branch or spot branch;
that change persists across later selected lights. A is the final attenuation
function argument,not the diffuse argument.

| F&4 | Ordered remaining requests |
| --- | --- |
| Set | GXSetChanMatColor(4,materialRGBA);GXSetChanCtrl(0,e,0,0,B,2,A);GXSetChanCtrl(2,0,0,0,0,2,2) |
| Clear | No material-color write;GXSetChanCtrl(0,e,0,1,B,2,A);GXSetChanCtrl(2,0,0,1,0,2,2) |

Channel argument order is channel,enable,ambient source,material source,light
mask,diffuse function,attenuation function. GX calls are respectively802b8ff4,
802b6f3c,802b6d6c,802b6e54,802b6f78. Numeric enums/request values are pinned;
SDK/GX register/cache/raster behavior,hardware capabilities and retained prior
material state when no write occurs remain open. No restore occurs on return.
No local tick/frame gate,source writes or selected-pointer null guard exists.
Original scene/group/preset producers and actual executed/image evidence are
needed before treating the native ordinary mesh lighting as this complete system.
