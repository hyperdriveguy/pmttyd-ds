# Original Mario shape/draw source and renderer implementation boundaries

US G8ME01 disc0 revision0;ID1,2026-10-12. Static original binary/resource
analysis,no original execution or frame image oracle. Local renderer/material
contracts start at [entry/defaults](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md).

## Original resource witness

Read-only a/a_mario is215124 bytes,SHA256
61b585d6b2f1a36551d3b779551a12122f228bf1354ebc5bb44651c58f118293.
The verifier checks original resource identity/name/time metadata and:

| Source collection | Original metadata |
| --- | --- |
| Shapes | 24,all shape modes0,all cull indices3 (original table index3→0) |
| Draw records | 24,all texture-slot counts1,all single-coordinate selectors0 |
| Call records | 70 |
| Texture descriptors | 150,all kind0 |
| Low16 call-count histogram | 3:15;4:50;5:1;10:1;11:1;12:1;14:1 |
| Sum of those vertex counts | 297 |
| Fan triangles across ALL source call records | 157,using sum(max(0,count−2)) |

The last two values cover the raw source collection BEFORE group visibility,
shape admission,animation/resource selection or caller scheduling. They are
not observed frame triangle counts and do not supersede the current102-triangle
idle bake fixture. Different scope explains why a raw all-call total is not a
rendered idle result. This resource witnesses the ordinary single-texture/type0
path;it does not demonstrate kind1/2 overrides,extra stages or multi-texture
rendering. Those code paths require their own US source bindings/runtime evidence.

`verify_animation_render_material.py` rereads and hash-checks the ISO,then checks
these serialized header/table histograms/counts. No original model/texture/pose/
message payload is exported into tracked docs. The loaded D pointer layout is
separate from serialized root-relative offsets until loader/relocation/pose
ownership domains are established.

## Concrete original call bindings

renderProc direct DOL references:animPoseDrawShape at80039768 and ordinary
Disp at8003c804. materialProc direct reference:special Disp at8003c6ec.
Both bodies ignore their second incoming integer argument locally. This census
is complete for direct references in seven DOL text sections;indirect/REL calls,
US reachability/frequency and original visible outcomes are not implied.
The original [draw wrapper](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md),
[group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md),
[node builders](ANIMATION_NODE_LAYOUT_AND_USED_BINDINGS.md) and
[animation evaluator](ANIMATION_MAIN_ENTRY_AND_TIME.md) supply preceding local
graphs. These local closures still leave called helpers and source lifetime open.

## Native port requirements supplied by this recovery

A faithful backend needs original position/normal/color/UV arrays and index
streams,shape/draw offsets,low16 fan counts,reversed signed coordinate selectors,
original cull/pass/alpha gates and retained material/texture state. It must
preserve the timing/order of default and override requests,including kind1
precedence with null pointer,render-only alpha adjustment and extra-stage smart
object return nonuse. A one-texture/no-color bake can implement the witnessed
ordinary resource subset;it does not establish complete-game material behavior.

The current asset converter/native meshes must reconcile source visibility,
node matrix branches,animated indices/texture states,multiple coordinate sets,
vertex colors,normals/lighting/fog and additional texture/TEV stages before full
original geometry/material fidelity claims. Original assets remain reused;
platform texture scaling/geometry reduction/lighting adjustments are port
choices that need recorded validation,not substitutions for this source logic.
ID1 has not changed native code or asset conversion policy in this delivery.

Still open:animSetMaterial_Texture,map material Light/LastStageBlend/Fog/Tev,
smartTexObj ownership,PSMTXInvXpose/MultVec numeric and singular behavior,GX
backend/cache/memory,pool/pose/link/resource relocation/source invariants and
original frame comparisons. These are actionable remaining dependencies,not
proof that all systems are documented. Final complete-game requirements remain
premature until these and broader gameplay/story systems are closed.

ID1 dependency closure addendum (2026-10-12):the preceding open-helper list is
superseded locally by [texture objects](ANIMATION_TEXTURE_OBJECT_SETUP.md),
[coordinate matrices](ANIMATION_TEXTURE_COORDINATE_MATRICES.md),
[smart patch](ANIMATION_SMART_TEXTURE_DATA_PATCH.md),
[color/fog](MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md),
[light selection/channels](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md).
Complete helper/query/data-patch local request graphs now exist. Their called
GX/numeric backends,smart holder ownership,map TEV,normal matrix/resource/source/
lifetime and original frame comparisons remain open. The same-pointer smart
return clarifies extra-stage nonuse without erasing its object-data mutation.
