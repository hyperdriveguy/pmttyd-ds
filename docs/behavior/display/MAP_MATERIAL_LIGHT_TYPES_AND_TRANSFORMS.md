# Map material light type priority, transforms and object publication

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis. `tools/verify_animation_texture_light.py` checks fourteen
complete local bodies,1189 instruction words,eleven binary32 constants,eight
original spans,two US lecture imports and41 direct DOL reference sites. These
are evidence counts,not full-game completion or execution coverage. No original
instructions or hardware images were executed for this recovery.

The per-light loop in mapSetMaterialLight800208c0/0x57c processes selected list
index i while signed i<L. For each slot copy direction seed802bfaa8=(0,0,-1),
capture selected pointer E and U16[E+0],then choose type in priority
0x1000 point,else0x2000 directional,else0x4000 spot. It does not combine types.
Light ID is word[802bfa58+4*i];the pinned eight entries are1,2,4,8,16,32,64,128.
No local bounds check beyond selection/query count behavior.

## Point branch, flag0x1000

camGetCurPtr;request PSMTXMultVec(camera+11c,captured E+24,viewPosition).
GXInitLightPos(local object,viewPosition XYZ)802b6cb8. Reread list[i];copy raw
record+48 color and GXInitLightColor802b6ce4. Reread list[i]/U16 flags again.
If flag0x40 set,GXInitLightAttn802b6a3c(obj,E50,0,0,1,0,0),leaving attenuation
selector A unchanged. If clear,request Attn(obj,E50,0,0,E54,E58,E5c),set A=1.
Then GXLoadLightObjImm802b6cf0(obj,lightID). No separate direction/spot/distAttn
request in this branch. Those called object initializers own their field writes.

## Directional branch, flag0x2000

Request RotRad X from captured E+30*0x3c8efa35. Reread list[i] for Y at+34,
then reread again for Z at+38. Each degree multiplication uses fmuls.
Concat(Z,Y,Y) then Concat(Y,X,X):effective ordered matrix (Z*Y)*X.
Both concatenations alias their SECOND input. MultVec(matrix,seed,seed),then
PSVECScale8029ebc8(seed,-1048576,seed),then PSVECAdd8029eb80(fresh E+24,seed,
worldPoint). The scale constant8041f950=0xc9800000 is negative2^20.
camGetCurPtr,MultVec(camera+11c,worldPoint,worldPoint),InitLightPos,copy fresh
E+48 color,InitLightColor,LoadLightObjImm. No local InitLightDir/Attn request or
A change on this branch. The large offset is a literal positional request;
original SDK/hardware directional lighting equivalence remains a called boundary.

## Spot branch, flag0x4000

camGetCurPtr FIRST;MultVec(camera+11c,captured E+24,viewPosition). Build X/Y/Z
rotations from fresh list entries with the same degree factor and SECOND-input
alias concatenations as above. MultVec(rotation,seed,seed). Call camGetCurPtr
AGAIN;PSMTXMultVecSR8029e6f8(camera+11c,seed,seed),without translation.
The earlier view position remains captured across these calls.

InitLightPos(obj,earlier viewPosition);GXInitLightDir802b6cc8(obj,seed);
GXInitLightSpot802b6a58(obj,4,fmuls(fresh E+4c,.5));
GXInitLightDistAttn802b6be8(obj,0,1000,.5);fresh E+48 color→InitLightColor;
LoadLightObjImm(obj,lightID);set A=1. Original .5 constant8041f934=3f000000,
1000 constant8041f954=447a0000. Do not interpret GX function arguments as an
already measured portable spotlight curve.

## Shared consequences and numeric boundaries

If none of the three type flags is set,no local object initialization/loading
occurs. Regardless of which branch ran,OR lightID into the accumulated channel
mask;then advance selected pointer slot and light-ID table entry. This can enable
a retained hardware light slot for an unrecognized record. The local light object
is not cleared at the top of each iteration. Original field initialization is
owned by the called SDK functions and source lifecycle,not assumed memset.

[Channel setup](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md) uses the final sticky
A and complete accumulated mask. [Matrix primitives](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md)
and [axis rotation](ANIMATION_SDK_AXIS_ROTATION.md) supply concat/rotation local
contracts. MultVec/MultVecSR/Scale/Add paired numerics,full light initializer
packing,GQR0/FPSCR/libm,source pointer mutation and original visual traces are
still separate gaps. Keep camera getter order and fresh-versus-captured record
reads when implementing an equivalent request graph.
