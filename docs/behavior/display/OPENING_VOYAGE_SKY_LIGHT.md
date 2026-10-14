# Used original Voyage sky light and normal binding

US G8ME01 disc0 revision0; read-only original DOL/map inspection. Verifier
[verify_opening_sky_light.py](../../../tools/verify_opening_sky_light.py) pins seven
function bodies,116 instruction words, attenuation table, original muj_20 map
hash, named sky selection, pointLight9 fields and first used normal. No original
instructions or hardware lighting were executed.

Original m/muj_20/d S_hai01 node0x87500 selects mask0x100 with control byte+2
flag8 clear. Named index8 resolves pointLight9. Its white vertex RGB and
untextured lambert1_v color-source1 are material inputs, not the lit result.

_mapLoad80029528..295f4 scales raw light position+8 by10 before lightEntry.
The caller passes raw+48 as f1, raw+52 as f2, raw+56 as integer r8. Raw+60 is
an optional r9 flags argument admitted only if raw+64==0x012d60d8. It is not an
attenuation float. lightEntry8001bf68..bf7c writes f1→runtimeE4c, f2→E50 and
three coefficients from table802bf900+12*r8→E54/E58/E5c. Table rows are:

| Index | Runtime E54/E58/E5c |
| --- | --- |
| 0 | 1,0,0 |
| 1 | 0,0.1,0 |
| 2,3 | 0,0,0.01 |

Static pre-animation pointLight9 record0x884ec: position(0,90.000061,-250.224274),
RGBA141e57ff, raw+48=0,+52=1.5,+56=0,+60=0,+64=0x012d60d8. Runtime flags
are active1|point0x1000. Consequently the existing point-material branch calls
GXInitLightAttn(a0=1.5,a1=0,a2=0,k0=1,k1=0,k2=0): no distance falloff.
The position and geometry must share a coordinate scale; uniform scaling does
not change this no-falloff light direction. The original map table has no ambient record. A bank populated by this table
alone therefore uses the material black fallback000000ff; retained light-bank
lifecycle and runtime modifications remain outside this bounded proof.

mapSetPolygonVtxDesc80020354..20384 requests GXSetVtxAttrFmt(0,NORMAL10,
XYZ0,S8-type1,frac6), then GXSetArray(NORMAL10,VCDnormalptr+4,stride3).
The normal array contains THREE SIGNED BYTES divided by64, not S16/16384.
For S_hai01 mesh0x70020, VCD0x880c8 and normal array0xe1e4, display list0x6fdc0
first indices(pos7278,normal213,color4) select bytes(0,0,-64)=(0,0,-1).
_mapDispMapObj800262fc..26338 computes cameraView*objectModel and, when
raw runtime object flags0x100 is clear, invokes
PSMTXInvXpose8029df28 and GXLoadNrmMtxImm0. The0x100 branch retains the prior normal matrix. Nonuniform source transforms need
this inverse transpose; using the position matrix for normals changes lighting.

For a portable native equation, Dolphin's primary
[vertex shader generator](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/VideoCommon/VertexShaderGen.cpp)
normalizes the transformed normal. Its
[lighting generator](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/VideoCommon/LightingShaderGen.cpp)
applies angular/distance polynomials, clamped diffuse dot, rounded integer light
accumulation, saturation and byte material multiplication. For the static initial color, the light term
reduces to `1.5*max(dot(normalizedN,normalized(lightPosition-vertex)),0)` times
RGB(20,30,87), with black ambient and white vertex material. This equation is
an emulator-source bridge, not a claim of measured original GX/hardware numeric
parity. Preserve original normals, current source pose transforms and named
selection; do not tint the entire sky uniformly or fabricate a background asset.

Existing [source channel setup](MAP_MATERIAL_ENTRY_CHANNELS_AND_TEV_INPUTS.md)
and [point requests](MAP_MATERIAL_LIGHT_OBJECT_REQUESTS.md) supply source
channel/material ownership. Other light types/table indices, source runtime
mutation, GX precision and original/hardware comparisons remain separate work.

## Original alpha byte producer and actual attachment targets

The muj_20.o symbol `_ap` is REL BSS section6+0x20, a pointer rather than the
alpha byte itself. Do not conflate it with muj_00.o's separate BSS+0x8 alias.
`alpha_init_func` file0x5318/0x44 requests _mapAlloc(current map allocator,1),
stores the returned pointer in `_ap`, then writes byte255. `alpha_set_func`
file0x52dc/0x3c resolves event argument0 through evtGetValue and writes its low
byte through `_ap`; it returns event status2. `alpha_tev` file0x535c/0xe4 loads
that pointer and byte, sets TEV registerA0 to the byte, passes prior-stage RGB,
and multiplies prior-stage alpha byA0. The scalar does not directly replace RGB.

A complete original self-relocation census identifies exactly six references
to section6+0x20: HA/LO pairs in these three functions. The only function/event
attachments and calls are the eight ADDR32 bindings below:

| Original event/binding | Concrete use |
| --- | --- |
| nami_alpha0x2ffcc | alpha_init_func |
| nami_alpha0x2ffdc | evt_map_set_tevcallback slot1→alpha_tev |
| nami_alpha0x3003c | alpha_set_func(LW0) |
| moonout_effect0x30070 | alpha_init_func |
| moonout_effect0x30080 | evt_map_set_tevcallback slot1→alpha_tev |
| moonout_effect0x30110 | alpha_set_func(LW0) |
| moya_effect0x30180 | child moonout_effect |
| onryo_event0x31270 | child nami_alpha |

nami_alpha marks S_umi_l_siro_b with runtime object flag0x01000000 and clears
0x100, then interpolates curve11 alpha255→0 over500ms, calling alpha_set_func
for each sample. moonout_effect marks S_hai02 and S_hai03_moon with flag
0x01000000 and clears0x100, then uses the same sampling loop over1500ms. Both
loops test completion LW1 and wait one frame before the next sample.

Neither the original muj_20_init_evt prologue entrance branch nor evt_prologue
references these producers or alpha attachments. The opening branch's actual
`yoru_hiru` mode2 request at0x345d4 and sun translation at0x345e8 precede its
ship/child setup; they do not initialize `_ap` or attach alpha_tev. Therefore
an opening-day sky fix must not invent `_ap=0` or attach this night-fade callback
because S_hai02 is white. Its original pose transforms, culling/winding,
visibility and light/material state remain the appropriate owners to inspect.

The verifier additionally pins the original complete muj REL identity, nine
function/event spans and all fourteen BSS/producer/attachment references.
This closes the local producer/reference question, not arbitrary foreign REL
imports, original runtime execution or global map callback teardown/lifetime.

## Current-bank ambient teardown

Original lightReInit8001c1d8/0x44 calls mapGetActiveGroup; group1 selects
bank8041e658, other values bank8041e650. It captures count and array, then
memset(array,0,count*96), clearing every complete runtime light record, not
just active flags. The complete DOL direct-branch census finds only two callers:
bmapUnLoad80028b60 and mapUnLoad80028e28. The verifier pins the reset body and
both original unload calls. Consequently standard same-bank map teardown
clears preceding House/book ambient entries before muj lightEntry repopulates
its15point-only records. Black ambient is justified under that lifecycle, not
merely because the new map table lacks ambient entries. This remains a source
request proof; active-group swaps, alternate/foreign lifecycle, runtime edits
and physical original execution are not established here.

## Original yoru_hiru light tracks and day endpoint

The opening's mode2 `yoru_hiru` does update light colors. Animation entry
0x887e4 has duration250 and table pointers: +12 mesh, +16 material (zero),
+20 another track class (zero), +24 light pose table0x936dc and +28 light color
table0x94c24. Both light tables contain nine named tracks for pointLight1..9.
The earlier static light9 analysis is not the final day light state.

mapMain80026c04..26c14 captures these separate tables. Each light track begins
with name pointer+0 and signed key count+4. Light pose keys begin+8, stride184
(time plus nine20-byte components); light color keys begin+8, stride104 (time
plus five20-byte components). Component records are value, incoming tangent,
outgoing tangent, unused word, nonzero step flag. Named tracks resolve through
lightNameToPtr. The used keys have zero step flags, and the source evaluator
uses cubic Hermite with left outgoing/right incoming slopes, clamps times
beyond the last key, and chooses the final segment at equality. Single-key
tracks do not enter this local sampling loop; the used tracks have3..5keys.

The light pose publisher80027e7c..27eb8 scales sampled XYZ by10 intoE24/E28/E2c,
then writes sampled rotationXYZ toE30/E34/E38; its final three sampled scale
components are not published. Light color publisher8002803c..2809c uses
original binary32 constant255 at8041f9a8: fmuls RGB by255, fctiwz truncates
toward zero, stb publishes lowbytes E48/E49/E4a, alphaE4b is forced255. There
is no invented gamma/exposure conversion. Component3 publishes toE50
(point intensity/angular constant); component4 publishes toE4c (spot angle,
unused for these point lights). All calculations occur before material lighting.

| Named color track | Keys | Original final RGB floats | Published day RGBA/intensity |
| --- | --- | --- | --- |
| pointLight8 at0x9550c | 0,200,250 | 1,1,1 | ffffffff /2 |
| pointLight9 at0x9564c | 0,150,200,230,250 | 0.21180300414562225,0.4081819951534271,0.7960000038146973 | 3668caff /1.5 |

The point9 position/rotation keys keep its original static pose. Its intensity
remains1.5, while RGB changes from static20/30/87 to54/104/202. This precisely
explains the observed original liveRAM3668caff and brighter sky without adding
a palette or exposure multiplier. The existing source mapPlayAnimationLv
mode2 branch80029fdc..2a038 backdates animation start by its duration, allowing
mapMain to publish the completed250 pose; mode1's loop flag is separate.
The verifier now pins the complete mapMain/mapPlayAnimationLv bodies, table
layout, original point8/9 key times and final RGB/intensity byte publication.

Original liveRAM/image evidence is held in the boat owner's ignored
artifacts/dolphin-original-voyage-ram/fresh-095-lights.json; this contract's
endpoint is independently derived from original asset keys and DOL stores.
Native sampling/bake and matching original/native pixel comparison remain
boat-owner validation; source intermediate single/fused arithmetic, clock
and hardware GX numerical parity remain separately bounded.
