# ID1 map material, texture/light and TEV contract status

Delivery summaries for the display/map-material contracts, moved out of the [behavior register](../README.md) on 2026-10-12 so that index stays a register. Content is verbatim; append new ID1 delivery summaries here rather than to the register. Contracts are in this `display/` folder.

## ID1 used texture/light/color/fog local contracts (2026-10-12)

| Contract | Closed local behavior | Remaining boundary |
| --- | --- | --- |
| [Texture objects](ANIMATION_TEXTURE_OBJECT_SETUP.md) | Reverse slot/kind0/palette/wrap/LOD/load requests and TEXGet | Relocation/image lifetime/GX/filtering |
| [Texture coordinates](ANIMATION_TEXTURE_COORDINATE_MATRICES.md) | Component gates/grouping/two-pass request order | Libm/GQR0/FPSCR/GX/image parity |
| [Smart data patch](ANIMATION_SMART_TEXTURE_DATA_PATCH.md) | Holder dereference/packed data mutation/same pointer return | Holder/source/cache/object lifetime |
| [Color and fog](MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md) | C2+0 alpha factor/staged floor255/map/camera/fog requests | Group producers/TEV/GX/depth/images |
| [Light queries](MAP_CHARACTER_LIGHT_QUERIES.md) | Presets/banks/source-order threshold/sort comparator/filter/postwrite capacity | Initialization/sorter stability/source domain |
| [Selection/channels](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md) | Preset replacement/ambient/sticky attenuation/channel/fog requests | GX/channel retained state/source bindings |
| [Light types](MAP_MATERIAL_LIGHT_TYPES_AND_TRANSFORMS.md) | Type priority/ZYX/view transforms/SDK object requests/ID mask | Numeric/initializer/GX/original visual parity |

Verifier:tools/verify_animation_texture_light.py. Fourteen complete local bodies
are used dependencies of the existing render/material graph. No whole-system
completion or original execution is inferred from body/anchor/reference counts.
Actual mapSetMaterialTev modes,normal/GX/cache/resource/source/lifetime and broader
US gameplay/story reachability remain open;final complete-game requirements
still require whole-system closure.

## ID1 ordinary TEV and character-shadow paths (2026-10-12)

| Contract | Closed local scope | Open boundary |
| --- | --- | --- |
| [Base/source](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) | Mode0/generic/no-texture requests,13-target dispatch and actual24-draw Mario mode0 witness | Other base modes1..9/12,GXSetTevOp backend,flag producers |
| [Constant/character shadow](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md) | Exact request order/counters and four complete source getter bodies | Shadow initialization/capture/actor ownership,GX/cache/lifetime/images |
| [Final callback/gaps](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md) | Seven-word callback record/nil indirect reset/selective reload/count truncation;remaining masks/destinations | Six middle branch internals,callback body/source/valid counts |

Evidence verifier:tools/verify_map_tev_ordinary.py;303 SCOPED words,one partial
containing identity/four complete leaves. No full TEV body/system closure claim.
Ordinary Mario static source witness is prioritized over unbound library paths;
actual executed source flags and original image comparisons remain necessary.

## ID1 fixed TEV base programs (2026-10-12)

[Stage programs](MAP_TEV_FIXED_STAGE_PROGRAMS.md) specify eight complete
literal base blocks(modes1..7/12),25 rows/150 requests and fixed base stage counts.
[Mode12 caller/evidence](MAP_TEV_MODE12_CALLERS_AND_LITERAL_EVIDENCE.md)
binds actual N2/M12 renderer/material requests and explains static inspection.
Verifier:tools/verify_map_tev_fixed.py;903 scoped words,one dispatch span,two
imports,six direct sites. Modes8/9 and six middle branches,GX/GPU/source/scene/
lifetime and original images remain open;containing identity is not whole-body
semantic closure. Original Mario authored mode0 does not replace procedural12.

## ID1 time-driven TEV bases and base closure (2026-10-12)

[Mode8 pulse](MAP_TEV_MODE8_PULSE_AND_RAW_CLOCK.md) specifies raw GP1c,
low32 unsigned conversion/single+double-sin/FMA/truncation/byte1 color-register
request. [Mode9 shared scroll](MAP_TEV_MODE9_SHARED_SCROLL_AND_MATRICES.md)
specifies per-call state mutation/strict single-wrap/two translated matrices/
three stage rows. [Base closure/gaps](MAP_TEV_BASE_MODE_CLOSURE_AND_REMAINING_WORK.md)
indexes all locally recovered base dispatch paths and remaining dependencies.
Verifier:tools/verify_map_tev_time.py;266 scoped words/six constants/four spans/
two imports/six sites. All base REQUEST blocks close locally,not full TEV/system
semantics. Six middle branches,clock/source/reset/cadence/GX/libm/lifetime/
original images and actual mode8/9 US scene execution remain open.

## ID1 projected middle TEV blocks (2026-10-12)

[Flag0x400 projection](MAP_TEV_FLAG400_TWO_STAGE_PROJECTION.md) closes
preceding-stage output1 rewrite,global matrix/smart image patch/two stages,
shared swap table/register2 and counter publication.
[Flag0x20000 projected shadow](MAP_TEV_FLAG20000_PROJECTED_SHADOW.md)
closes double texture capture/live matrix/color/ignored config query/exact
stage/generator/texture/matrix/KColor order and four pswp source leaves.
Verifier:tools/verify_map_tev_projected.py;233 scoped words/three spans/two imports/
six sites. Four middle branch internals0x10000/0x40000/0x80100000/0x800000 remain
open,alongside source/GX/lifetime/US scene/original images. Full TEV body identity
is not complete semantic or whole-system evidence.

## ID1 terminal TEV color and projected pulse texture (2026-10-12)

[Color/context/cosine alpha](MAP_TEV_TERMINAL_COLOR_AND_COSINE_ALPHA.md)
specifies raw selector precedence/clock/rounding/byte products/retained alpha.
[Animated projected texture](MAP_TEV_ANIMATED_PROJECTED_TEXTURE.md)
specifies original built-in payload/two clock reads/synthetic view/ortho/matrix/
stage/KColor/counter order. Verifier:tools/verify_map_tev_terminal.py;255 scoped
words/11 constants/five spans/two imports/six sites/65536 host integer cases.
Two depth middle branches0x10000/0x40000,source/GX/libm/Ortho/LookAt/capture/cache/
lifetime/original images remain open. Hash/count/request closure does not prove
full original shader/system or whole-game completion.

## ID1 complete local TEV request documentation (2026-10-12)

[Depth source/config](MAP_TEV_DEPTH_SOURCE_QUERIES_AND_CONFIG.md),
[flag10000 routes](MAP_TEV_DEPTH_FLAG10000_PROGRAMS.md),
[flag40000 routes](MAP_TEV_DEPTH_FLAG40000_PROGRAMS.md) close final two
middle blocks/seven queries;original two config records both route3.
[Whole local graph/boundaries](MAP_TEV_LOCAL_REQUEST_GRAPH_AND_BACKEND_BOUNDARIES.md)
indexes ALL local branches/requests/captures/order/counters. Evidence verifier:
tools/verify_map_tev_depth.py,1115 new words/six spans/two imports/six sites/
65536 byte cases plus rechecked six-manifest2927-word coverage with zero holes/
conflicts. This is complete local REQUEST documentation,not SDK/GX/numeric/
source/capture/cache/lifetime/US execution/images or whole-system/game completion.

## ID1 map material source caller and texture matrices (2026-10-12)

[Entry/TEV](MAP_MATERIAL_ENTRY_CHANNELS_AND_TEV_INPUTS.md),
[light selection](MAP_MATERIAL_LIGHT_SELECTION_AND_NAMES.md),
[light objects](MAP_MATERIAL_LIGHT_OBJECT_REQUESTS.md),
[texture loads](MAP_MATERIAL_TEXTURE_LOOKUP_AND_OBJECT_LOADS.md),
[matrices/setter](MAP_MATERIAL_TEXTURE_MATRICES_AND_PROJECTED_SETTER.md)
close six local bodies with actual raw object/material source inputs and used
map caller sites. Verifier:tools/verify_map_material_source.py;806 words/six
constants/six spans/two imports/five direct sites. Setter has zero DIRECT DOL
sites,not claimed runtime use. Loader/source/caller admission/GX/numeric/cache/
lifetime/actual scene/images and whole-game requirements remain open.
