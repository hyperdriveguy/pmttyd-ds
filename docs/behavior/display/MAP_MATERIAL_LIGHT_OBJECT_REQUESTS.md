# Map material light objects and differences from character lighting

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original instructions/GX execution or observed images.
`tools/verify_map_material_source.py` checks six complete local bodies,806
reviewed words,six binary32 constants,six original spans,two US lecture imports
and five direct DOL sites. Loader/source/GX/numeric/lifetime/runtime remain open.

This is the complete per-light loop within mapSetMaterial80020e54/0x614.
For each selected slot i,copy original seed802bfa9c=(0,0,-1). Capture selected E
and U16[E+0];priority0x1000 point,else0x2000 directional,else0x4000 spot.
LightID=word[802bfa58+4*i],eight pinned IDs1,2,4,8,16,32,64,128. No local i<8
check beyond [selector](MAP_MATERIAL_LIGHT_SELECTION_AND_NAMES.md) behavior.
The local GX light object is not memset/cleared at iteration start.

## Point

camGetCurPtr;MultVec(camera+11c,captured E+24,viewPosition);InitLightPos.
Fresh selected pointer/color raw+48→InitLightColor. Fresh E fields:
InitLightAttn(obj,E50,0,0,E54,E58,E5c);LoadLightObjImm(obj,lightID);A=1.
It does NOT test source flag0x40,unlike character mapSetMaterialLight. Numeric
requests/field capture order must not be replaced by its point shortcut.

## Directional

RotRad X from captured E30*k;fresh pointer Y fromE34*k;fresh pointer Z fromE38*k,
k=8041f948 bits3c8efa35. Concat(Z,Y,Y),Concat(Y,X,X),aliasing SECOND inputs:
rotation=(Z*Y)*X. MultVec(rotation,seed,seed);Scale(seed,-1048576,seed);
Add(fresh E+24,seed,worldPoint);camGetCurPtr;MultVec(camera+11c,worldPoint,worldPoint);
InitLightPos;fresh E48→InitLightColor;fresh E50→InitLightAttn(obj,E50,0,0,0,0,1);
LoadLightObjImm. A remains prior2/1. The explicit directional attenuation call
is present here and absent in the character-light helper. No local InitLightDir.

## Spot

camGetCurPtr first;MultVec(camera+11c,captured E24,viewPosition). Fresh selected
pointer rotations X30/Y34/Z38*k;same(Z*Y)*X aliases;MultVec(rotation,seed,seed).
camGetCurPtr AGAIN;MultVecSR(camera+11c,seed,seed). InitLightPos with earlier
captured viewPosition;InitLightDir(seed);fresh E4c*.5→InitLightSpot(obj,4,angle);
InitLightDistAttn(obj,0,1000,.5);fresh E48→InitLightColor;LoadLightObjImm;A=1.

No recognized type→no object initialization/loading. ALWAYS OR lightID into
mask B,including unknown types;advance selected slot/ID table. A initialized2
once,sticky1 after point/spot,not reset per light. Unknown-type mask can select
retained backend light state;actual source valid-type invariant is not proven
by this loop. Calls own SDK field packing/GPU behavior. Camera and pointer
reloads retain their order;model/world numeric/vector/libm/FPSCR/GQR0 remain
boundaries even with prior matrix/axis contracts. Final ambient/material/channel
and [TEV inputs](MAP_MATERIAL_ENTRY_CHANNELS_AND_TEV_INPUTS.md) follow after loop.
No global fog reset or original tick/frame gate occurs in this map body.
