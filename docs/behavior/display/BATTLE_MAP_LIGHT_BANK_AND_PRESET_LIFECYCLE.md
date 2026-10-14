# Battle map light banks, presets and fog lifecycle

US G8ME01 disc0 revision0; read-only DOL and original map/model metadata,
2026-10-14. This supplies source owners for the current renderer frontier
mapSetMaterialLight800208c0. Static source requests and metadata are not original
execution or rendered-image validation. The existing [selection/channel](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md),
[query](MAP_CHARACTER_LIGHT_QUERIES.md), [light-object](MAP_MATERIAL_LIGHT_TYPES_AND_TRANSFORMS.md)
and [blend/fog](MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md) contracts remain the
consumers; their GX/PICA backends are separate implementation obligations.

## Two retained banks and actual zero-record initialization

lightInit8001c21c/234 sets count32 at8041e650 and8041e658, allocating each
record array through __memAlloc800300f0(0,32*96). Array pointers are the bank
words+4. __memAlloc/64 calls the selected heap allocator; on nonnull result,
memset(pointer,0,requestedSize) at80030128, then DCFlushRange(pointer,size)
at80030134. Thus fresh banks contain **32 complete zero records**, not count0
or an unknown allocation filled with presumed inactive records. Allocation
failure handling remains the original caller/provider domain.

mapGetActiveGroup8002b430 returns WORD8041e664. Light selection uses bank1
8041e658 only for exact selector1; other values select8041e650. It does not use
the map-record pointer8041e660 as the light-array base. Each light record has
stride96, flagsU16+0, embedded name+2, position+24, direction/rotation+30,
RGBAword+48, angleFLOAT+4c, intensityFLOAT+50, attenuationFLOATs+54/+58/+5c
(offsets hex). Active flag1, character flag80 and ambient flag8000 are distinct.

lightReInit8001c1d8/44 selects that current bank, captures its count and array,
then memset(array,0,count*96). It neither changes count nor clears the other
bank or static presets. Complete seven-text-section direct linked-call census
finds only bmapUnLoad80028b60 and mapUnLoad80028e28. bmapUnLoad calls it while
selector1 is still current; selector0 restoration occurs later at80028c34.
This proves a reset producer for repeated ordinary battles, independently of
fresh startup. bmapLoad itself does not call lightReInit.

lightEntry8001bd98/290 resolves the current bank and first free record in source
order, clears96 bytes, copies its selected name and input fields, then ORs
active1. Its type argument0/1/2/3 adds8000/1000/2000/4000 respectively;
input flags mask2 adds character80. Name collisions select a formatted alternate
name before allocation. No inferred empty-bank result supersedes actual entries.

## Selected underground battle map producers

The original ordering is seq_battleMain80073b24, rank0 bmapLoad at80073d48,
then battle_init800738ac at80073d94, BattleInit800f8f04 at80073b08, and
BattleStageInit80139780 at800f901c. Map loading precedes BattleStageInit; it
is not its child. Rank is signed HALF[pouchGetPtr()+88]. Rank0 chooses
bti_01; the retained selected stage supplies stg_00_1 as the second map.

bmapLoad8002892c selects map bank1, clears its entire2f4-byte map record,
then _mapLoad80028f18 loads slot0 and, for nonnull second name, slot1 at+178.
The two authored maps have no light-table records:

| Original source | Size | Light table offset/count | Fog offset / fields(enabled,type,start,end,RGBA) |
| --- | --- | --- | --- |
| m/bti_01/d | 181042 | 281fc /0 | 28200 /(0,0,0,1000,000000ff) |
| m/stg_00_1/d | 82534 | 1129c /0 | 112a0 /(1,0,300,1500,1a1e05ff) |

Offsets are file-relative hex; sizes and fog values are decimal. _mapLoad calls
lightEntry at800295f4 only inside the authored light-table loop. Zero counts
skip those requests. A real fresh lightInit allocation or preceding bank1
unload/reset, followed by these loads and no subsequent writer, therefore gives
32 inactive records. Do not derive that history solely from table count0.

_mapLoad copies the fog table into the corresponding per-map record+24..34,
setting/clearing flagsU16+4 bit1 according to enabled. Slot1's fog occupies
bank1+178+24..34. mapSetMaterialFog reads the **primary** current map record,
so bti_01 supplies disabled fog; foreground stg_00_1 fog is not substituted.
The disabled helper still requests the original camera getter and fog-off GX
call. Material blend flags2/4/8 stay clear from the map-record clear unless a
real later writer changes them. Stage subtree visibility flags are separate.

At80029618/1c every _mapLoad also writes fallback ambient globals8041e6d8 and
8041e6d4 from8041f90c/910, both raw707070ff. The second map repeats these same
initializations. These globals are distinct from ambient light records and from
the map blend RGBA fields. With live material flags1 or8 and exactly one
selected preset, the consumer can select this fallback; default ambient for the
other documented paths is000000ff until an actual character ambient overrides it.

## Static preset initialization and per-main camera update

lightInit additionally initializes four independent96-byte BSS preset records.
Their flags are1040, **without active1**; preset getters return them directly,
so absence from the active map bank does not disable their selection. It copies
names, six vector words at+24..38, RGBA+48, FLOAT+4c and FLOAT+50. Unlisted
fields are not individually reset by these writes.

| Address / name | Initial position+24 | Direction/rotation+30 | RGBA /4c /50 |
| --- | --- | --- | --- |
| 803c9750 / PAPER | (0,0,0) | (0,0,0) | ffffffff /0 /1 |
| 803c96f0 / PAPERI | (0,0,1048576) | (0,0,0) | ffffffff /0 /1 |
| 803c9690 / PAPERCRAFT | (-600,300,300) | (0,0,0) | ffffffff /0 /1 |
| 803c9630 / BooBoo | (0,0,0) | (0,0,0) | ffffffff /0 /10 |

PAPERCRAFT starts from(-200,100,100), then requests Scale(3,3,3) and aliased
PSMTXMultVec into its position. This is an original numeric request sequence,
not a constant position replacement for arbitrary numeric domains.
lightGetPaperCraft returns803c9690. lightGetPaper queries current camera number:
camera4 returns803c9750; every other number returns803c96f0.

lightMain8001c028/1b0 calls camGetPtr(4), captures its view3x3 rows from
camera+11c/+12c/+13c, and seeds coefficients(0,0,1) from802bf8d0. For each
output component it performs single(row1*coefficientY), then fused adds
row0*coefficientX and row2*coefficientZ in that order. It requests in-place
PSVECNormalize8029ebe4, multiplies components by source1048576 at8041f8c0,
and stores803c9750+24/+28/+2c. Even the zero coefficients participate in the
source arithmetic. It then copies raw vector(0,1000,0) from802bf8f4 to
803c9630+24/+28/+2c. Other preset fields and both map light arrays are untouched.
Normalization's numeric/zero-vector domain remains a called provider; do not
replace an unknown camera sample with a guessed directional light.

The scoped direct census finds lightInit at marioStInit8006fe04, and lightMain
at marioStMain8006f970. On the recovered Main branch, camMain8000f704 is called
at8006f944 and mapMain80026bb8 at8006f94c, preceding lightMain. These are source
call order facts, not measured timing or proof every outer-state branch runs.

## Used shape and material-flag provenance

Original c_pkr group20 at6228 has sibling19, child-1 and shape15. Shape15 atb88
has position/normal/color/UV0 bases107/0/0/107; drawfirst15,count1; pass0 and
cull index3(the original table produces GX cull0). Draw15 at5284 has one slot,
word+4=1, TEV modeword+8=0, word+c=0, slot ID+10=9 and signed selector+30=0.
Calls start66,count22; position/normal/color/UV0 stream offsets are235.
Texture base9 at40a8 starts at44, override0; initial slot state0. All46 c_pkr
texture descriptors have kind0. Rendering must still read the live evaluated
slot9 state; initial44 is not a lifetime invariant.

animPoseEntry writes material lightEC=0 at80041b50. Ordinary battle _pose_def
can OR8 when ownerU1c mask01000000 is set. Restored **field** Goombella separately
inherits live Mario lighting through GetMaterialLightFlag/On; see
[field material provenance](../partner/FIELD_MARIO_POSE_HANDLES_AND_MATERIAL_PROVENANCE.md).
Read the actual current pose owner. Neither freshEC0 nor empty map lights proves
that preset selections are absent later.

For actual flags0 or2, no preset bits19 and a genuinely inactive bank1, the
documented queries produce no selected lights and no character ambient. The
consumer still executes its fog reset, channel count, ambient and channel-control
requests. If flags1/8/10 admit presets, use the real preset initialization and,
for camera4 PAPER, the actual lightMain update. The full light-object/channel
backend and other live writers remain required. This contract does not
acknowledge geometry, history, queue completion or full prologue parity.

## Evidence identities and remaining bindings

Private corrected disassemblies and metadata are retained under
`artifacts/battle-display-queue-evidence/`: `lightInit.dis`, `lightMain.dis`,
`lightReInit.dis`, `lightEntry.dis`, `memoryAlloc.dis`, `lightMainCaller.dis`,
`lightInitCaller.dis`, and `battleBank1LightFogMetadata.json`. They are temporary
evidence, not tracked source assets or a substitute for rereading the ISO.

| Original body/span(hex) | SHA256 of original bytes |
| --- | --- |
| lightInit8001c21c/234 | 4a456de5a4e8ff8d8ac53d4b60301104b18539bcdda69090634c55d3262aa5a4 |
| lightMain8001c028/1b0 | d879a7380f09eeebb3f40260e52807115931b5ad0cff674ef7e309d6ff3d7a9e |
| lightReInit8001c1d8/44 | 3372cb17b802b8a6497e9753d7a075eadf0335fe87c4f393fa66f144ca3730c0 |
| lightEntry8001bd98/290 | c9985fdabcacea85ce2d9e269cd0f320b7fdd95d485427b7beeff9da0f1e0634 |
| __memAlloc800300f0/64 | 51d2010e5ae0c3265971a8dfaee60b24489d8bf8eb2ca386f77c1d0ceb98712b |
| lightMain caller8006f92c/94 | a6c1bb437cb6b706de0addcd8b7c338f30f2bb4600514049144f6a304399e140 |
| lightInit caller8006fddc/44 | 832a29cb3620e9647dddba8639fa78e715f4fdae1760e41244959a0740742c1e |

Original assets: m/bti_01/d SHA256
213c62e758be15bc0731b3fa58558d1b2c565ea1c96e91111ea07b52cf751ca8;
m/stg_00_1/d SHA256
8d749bd8caad7fced1596231b755dc4e372e938816e10add8a8e0d7a69f5a2e1;
a/c_pkr SHA256
d712df83d7b70b64a90d45551b39ecf7adf277b9572f6931daae7792b52d69e7.
No original payload or external emulator implementation/table is included here.

Next native binding: retain actual selector/bank reset and later mutations,
poseEC, fallback ambient and admitted presets; implement the existing ordered
material-light/channel requests. Exact SDK normalize/light-object math, PICA
lighting translation, mutable scene/preset writers and original frame comparisons
remain open under [EVT-01](../../requests/EVT-01.md).
