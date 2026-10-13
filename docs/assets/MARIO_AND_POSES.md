# Mario model, animation and pose asset provenance

Mario data-child pack extension, the ordinary head/airborne/support bindings, the original ordinary Mario model mesh and sparse animation packs, low-HP/long-idle pose sources and the grounded shadow geometry. Split out of [ASSETS.md](../ASSETS.md) on 2026-10-12; sections are verbatim.

## Original Mario data-child pack extension (2026-10-12)

`make event-assets` now includes subset_attack_ready_jump_event@battle_event_subset.o
at original80416bd0,size8,SHA2561e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61.
Its DELETE2/terminal1 words and four ORIGINAL trailing bytes at80416bd8 remain
byte-exact. The tiny event has real constructor/deletion/ID/copy-back effects;
no zero padding or callback-completion replacement was generated.

Current four-stream ignored local-assets/events/jump.pmevt and romfs/events.pmevt
(with JSON sidecars) are4564 bytes,SHA256
82dd17bd66ff013fa3df32d25134b01d4077f11460bab222d9da0aad4d43790d.
Prior4540-byte three-stream fingerprint below is historical/superseded. All316
command words/raw operands,original addresses and scanner extents are retained.
No new USER_FUNC site is added by this child;direct original callback census
remains72 addresses for the original three main streams. Native reader and
manager integration verify the actual extracted child without claiming actual
Mario kind/table import or complete Jump runtime. Original Nintendo/Intelligent
Systems copyright/no redistribution rights and read-only ISO/source-history
exclusion remain unchanged.

## Ordinary head-query binding (2026-10-12)

Native31f6cb8 binds the read-only US DOL contract in
docs/behavior/overworld/OVERWORLD_AIRBORNE_AND_LANDING.md with ce29235/9869179/f305f1d
addenda. Its verifier checks26 bodies,185 anchors and19 data spans in
/home/carson/games/roms/G8ME01.iso without mutation. No asset conversion or
pack format changed: existing ignored romfs/*.head retains original triangle
winding/cull/attributes at scale.01. Raw ray normals feed head's projected angles
without support's Q1000 normal rounding. Host geometry/trig/scaled arithmetic
remain numerical adaptations. Mesh inspection at plaza(-6.7,.1) found rear roof
inclination114.93 degrees (rejected) versus center132.97 (admitted); the former
native peak fixture had accepted the rejected face. Updated native regression
uses these static cone/probe decisions;this is not original gameplay measurement.
Original copyrighted assets,ISO,compiled packs and emulator captures remain
outside source history;this work grants no distribution license.

## Ordinary airborne/landing binding (2026-10-12)

Native97fd570 and db71cca use the US DOL contract in
docs/behavior/overworld/OVERWORLD_AIRBORNE_AND_LANDING.md, recovered in35f82d8 with
a4df746/ce29235 addenda. tools/verify_overworld_airborne_and_landing.py reads
/home/carson/games/roms/G8ME01.iso read-only to verify original bodies,
instruction anchors and literals. Inspection does not mutate the ISO or link
original executable code. No asset extraction/conversion choice or pack ABI
changed in this native slice: existing ignored scale.01 original *.ground/
*.head provide winding/cull/attributes to the shared support service. Returned
normal/point quantization and host trig/intersection/scaled float arithmetic
remain adaptations. Original copyrighted assets and compiled/capture outputs
remain outside source history; this work grants no distribution license.

## Ordinary support/slopes/upstairs binding (2026-10-12)

Native commit78eb86b implements the bounded US support contract in
docs/behavior/overworld/OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md, with documentation
addendum b110e63. Its verifier reads /home/carson/games/roms/G8ME01.iso
read-only:13 DOL bodies,132 instruction anchors,24 data spans. No original
executable was linked or copied into the port, and no extracted or converted
asset format changed. Existing ignored romfs/*.ground and *.head retain
original source winding/cull/attributes at scale.01; native ray hits orient
back-facing normals toward their origin before support-angle interpretation.
Host trig, geometry intersections and scaled arithmetic are numerical
adaptations, not original SDK execution. ISO/converted packs retain original
copyright and remain outside source history; this work grants no distribution
license. Emulator captures and isolated saves remain ignored in artifacts/azahar.

## 2026-10-12 — Original ordinary Mario model mesh pack

Native53d573b replaces the ordinary field Mario billboard with ten original
frame0 meshes: M_S_1/M_W_1/M_R_1/M_J_1B/M_J_1C from a/a_mario, and their
R counterparts from a/a_mario_r. Their texture sources are a/a_mario- and
a/a_mario_r-. tools/build_assets.py extracts all four files from the US ISO
read-only into ignored local-assets; provenance JSON and romfs/manifest.json
record source identities and conversion paths. No ISO, extracted models,
textures or generated packs are committed or licensed for redistribution.
Original art remains copyrighted by its owners; existing MIT noclip format
reference attribution remains in third_party. New conversion code supplies no
license for Nintendo/Intelligent Systems assets.

Ignored local outputs are romfs/characters.pmmesh (PMCHAR1, ten poses,
306 front/186 rear vertices each), local-assets/characters-mesh.png and
romfs/characters-mesh.t3x. The atlas has31 source texture cells; original images
are resized to124px with a duplicated2px edge border in128px cells on1024px
RGBA. It retains authored UV orientation and rejects unimplemented wrapping or
untextured draws. Pack vertices retain unscaled local XYZ/depth/root rather than
cropping or fitting to a square billboard. Native source scalar2 and engine-unit
conversion0.01 give local-to-native0.02; no arbitrary .16 actor height applies
on this ordinary path. Rear models are separate original geometry/texture data,
not X-mirrored front UVs. Texture resolution and atlas filtering are handheld
adaptations, not original GX composition.

Converter998ec61 and its compatibility follow-up bind documented joint versus
transform builders, asymmetric joint Z gates, all-axis parent-zero fallback,
pivot-offset admission, ordered rotations and current-group sibling parent
selection. Generic helpers remain available to the separate map converter.
Host NumPy/trig/concat are numerical adapters, not measured Gekko/libm parity.
Geometry remains frame0; sparse animation updates are now documented but their
native clock/evaluator/geometry binding is unfinished. Original normals, colors,
material/GX/render callbacks, shadow geometry and complete display domains are
still separate requirements.

ID1 render/material source/build provenance (2026-10-12):US G8ME01 ISO opened
read-only;original a/a_mario identity/hash from earlier Main/draw contracts
rechecked. Serialized metadata:24 shapes(mode0,cull index3),24 single-texture/
coordinate0 draws,150 kind0 textures,70 raw call records/297 vertices/157 potential
fan triangles across ALL calls before visibility. No observed frame count or
original model/texture/pose/dialogue payload exported. Original cull table
80415dc8,zero lighting seeds802c1030/103c,diagnostic format802c1078 and initial
work pointer80415dc0 pinned as metadata;ELFs/listings/report ignored under
artifacts/reference/behavior. make all ran existing sound/proj/pmario.proj,
pool/sdir/samp pipeline:3218 GameCube identities,0 Switch,5 primary-PCM slice
fallbacks;local-assets/audio-map raw/catalog and RomFS WAV/audio-map.json outputs
unstaged. No ID1 art/audio/mesh conversion policy change. Original rights remain
with owners;local analysis grants no redistribution license. Exact log:
/tmp/id1-render-build.log. Native ordinary mesh subset remains distinct from
complete original materials/lighting/vertex-color/multi-coordinate/TEV behavior.

## 2026-10-12 — Original ordinary sparse animation pack

Native7102aaf loads ignored romfs/characters.pmanim (PMANIM1,293000 bytes)
for the ten documented ordinary front/rear poses. The same read-only ISO
sources a/a_mario,a/a_mario_r and their trailing-hyphen TPL files provide
original base positions,node floats,visibility,node-kind/sibling/child graph,
vertex/group/UV/texture bindings and sparse key records. No sampled-frame atlas,
flattened-vertex interpolation or generated art replaces those source streams.
The parser rejects unsupported normal/visibility updates,texture transforms,
coordinate selectors and UV wrap domains in this bounded conversion.

local-assets/characters-mesh.png and romfs/characters-mesh.t3x now contain37
original texture cells (124px plus2px duplicated border in128px cells). Added
cells cover actual stepped binding changes, including fractional rear-run keys.
The ignored manifest records each TPL source/index/cell and animation pack path.
The earlier PMCHAR1 frame0 pack remains a host fixture; native rendering uses
PMANIM1 evaluated geometry. The romfs/*.pmanim ignore rule was committed as dc1df68.

Independent test tooling writes ignored artifacts/character-animation-reference.bin
from the read-only extracted models using the documentation verifier's evaluator,
then the native host target compares143 poses/texture-boundary samples. Output
contains original derived geometry and remains local. Original asset copyright
and existing converter/reference attribution are unchanged; no original model,
texture,animation or compiled payload was committed or licensed for distribution.
Host/ARM floating-point affine/trig and handheld texture filtering remain adapters;
original SDK/material/normal/color/GX/lighting/shadow numerical/visual composition
is not established by these tests.

ID1 texture/light/color/fog provenance (2026-10-12):US G8ME01 disc0 revision0
ISO opened read-only. Original DOL texture-matrix dispatch80306678,light IDs
802bfa58,direction seed802bfaa8,fog modes804150c0,seed colors8041f8fc/8041f914,
conversion bias802c1060 and diagnostic802c10c4 are pinned metadata/hashes.
Runtime light presets803c9690/96f0/9750 and bank8041e650 are BSS pointers,not
serialized extracted assets. No image/model/pose/message/diagnostic payload
exported into tracked files. Local inspection ELFs/listings/report remain ignored
under artifacts/reference/behavior;14 local body hashes are reproducible against
the original. Original a/a_mario one-slot/kind0 witness remains as recorded in
prior delivery,not an observed scene. No ID1 texture scaling,geometry reduction,
lighting adaptation or music/effect conversion policy change. make all ran
existing sound/proj/pmario.proj,pool/sdir/samp pipeline:3218 GameCube identities,
0 Switch,5 primary-PCM slice fallbacks;ignored catalog/WAV/RomFS outputs remain
outside history. Log:/tmp/id1-texture-build.log. Rights remain with original
owners;local analysis and reusable code provide no asset redistribution license.

ID1 ordinary TEV/character-shadow metadata provenance (2026-10-12):US G8ME01
ISO read-only;original a/a_mario215124-byte identity rechecked,24 serialized
draw+8 mode values all0. No original model/texture/pose/message bytes exported
into source. DOL13-mode dispatch803049a0,ten projected matrix IDs802bf9f0 and
initial cswp pointer804150e0→803c9e38 are pinned words/hashes,not captured shadow
images. Runtime shadow texture/color/matrix/holder fields require lifecycle
recovery. Inspection ELFs/listings/report ignored under artifacts/reference/
behavior. No ID1 art/audio/converter adaptation change. make all ran existing
sound/proj/pmario.proj,pool/sdir/samp pipeline:3218 GameCube identities,0 Switch,
5 primary-PCM slice fallback WAVs;ignored catalog/RomFS outputs outside history.
Exact log:/tmp/id1-tev-build.log. Original rights remain with their owners;
metadata analysis grants no asset redistribution license.

## 2026-10-12 — Low-HP and long-idle original pose sources

Native778add7 extends the local pack to15 poses/425552 bytes and45 original
texture cells. Front a/a_mario adds M_S_3,M_I_S,M_I_Y; effect a/e_mario adds
M_I_5 and M_N_5A. The effect model's header+44 declares a_mario and uses the
original a/a_mario- TPL; there is no a/e_mario- file. The converter guards that
header declaration, records the effect model's read-only extraction provenance
and each pose's actual texture source. Source38e0298 verifies the original
shared binding. This corrects an intermediate attempted nonexistent asset path,
not an invented replacement texture. No new rear S3/I_S/Y model was fabricated.

Ignored local-assets/e_mario, original front/rear extracts, mesh PNGs, manifest,
romfs/characters.pmanim/characters.pmmesh/characters-mesh.t3x and native binaries
remain outside source history. Both effect poses have102 triangles after
source fan triangulation in the visible converted mesh; their ordinary single-coordinate/kind0
texture domain uses the existing source hierarchy/sparse evaluator. Atlas cells
retain the previous124px+2px border conversion. Additional models share TPL
IDs/cells through their declared texture source; no new generated artwork or
sampled-frame substitute was introduced. Original art rights and local-only
conversion policy are unchanged; no redistribution license is implied.

Independent reference samples now cover190 poses/texture boundaries across15
loaded animations with max model error6.79493e-06. Model/pose agreement does not
close original libm/Gekko/material/color/normal/lighting/GX/shadow composition.
Idle sound IDs are source service requests only; no arbitrary samples or tones
were assigned. Original macro/handle/envelope and cue371 alias ownership remain
open pending the requested audio evidence.

ID1 fixed TEV source/caller evidence (2026-10-12):US G8ME01 ISO read-only;
13-mode dispatch803049a0 and actual renderProc/materialProc mode12 preparation/
call words pinned. No original model/texture/pose/message payload exported.
Authored a/a_mario mode0 witness remains prior recorded metadata;mode12 is an
original procedural caller request,not an invented model asset or observed
frame. Fixed-program manifest contains decoded integer request metadata and
instruction anchors;inspection ELF/disassembly/report ignored under artifacts/
reference/behavior. No ID1 conversion,texture scaling,geometry reduction or
lighting adaptation policy change. make all ran existing sound/proj/pmario.proj,
pool/sdir/samp pipeline:3218 GameCube identities,0 Switch,5 primary-PCM slice
fallbacks;ignored catalog/WAV/RomFS outputs outside history. Exact log:
/tmp/id1-tev-fixed-build.log. Original rights remain with owners;local analysis
and new evidence code grant no redistribution license for original assets.

ID1 time-TEV source metadata (2026-10-12):US G8ME01 ISO read-only. DOL dispatch
803049a0,unsigned bias802bfc20,startup phases804150b0 and increments8041e0c0
pinned as words/hashes;phase startup all0,increment words00000000/bad1b718/
00000000/ba51b718. These are runtime state/source metadata,not observed scroll
speed or immutable material values. Mode8 raw color80429528 is SBSS2,not an
extracted serialized color asset;its initializer/writers remain open. No model/
texture/pose/image/message payload exported. Inspection ELF/listing/report
ignored under artifacts/reference/behavior. No ID1 asset adaptation/converter
change. make all ran existing sound/proj/pmario.proj,pool/sdir/samp pipeline:
3218 GameCube identities,0 Switch,5 primary-PCM slice fallbacks,then failed
concurrent renderer GPU_TEVSRC enum compilation. Log:/tmp/id1-tev-time-build.log;
no successful3DS build claimed by that run. Ignored catalog/WAV/RomFS outputs
outside history;original rights remain with owners and no redistribution license
is supplied by metadata analysis/new verifier code.

ID1 time-TEV build resolution:after native renderer owner's enum fix,make all
rerun succeeded/built3DSX;log /tmp/id1-tev-time-build-rerun.log. The failed run
above remains historical evidence,not current build status. Same3218 GameCube/
0 Switch/5 primary-PCM pipeline;no ID1 conversion or asset-policy change.

ID1 projected-TEV metadata provenance (2026-10-12):US G8ME01 ISO read-only.
Ten matrix IDs802bf9f0,register2 raw seed8041f8e0=000000ff and initial pswp
804150e4→803c9d34 pinned as words/hashes. Runtime global projection matrix
8041e680,holder slot8041e6b0 and embedded object8041e6b4 are SBSS source fields,
not extracted texture/matrix assets. pswp embedded data patch/matrix/color/config
initialization/capture/lifetime remain separate. No original model/texture/pose/
shadow image/message payload exported. ELF/listings/report ignored under
artifacts/reference/behavior. No ID1 converter/texture scale/geometry/lighting/
audio policy change. make all passes/log /tmp/id1-tev-proj-build.log;existing
sound/proj/pmario.proj,pool/sdir/samp pipeline3218 GameCube/0 Switch/5 primary-PCM
slice fallbacks ran. Ignored catalog/WAV/RomFS outputs outside history;original
rights remain with owners and local analysis grants no asset redistribution.

## 2026-10-12 — Original grounded shadow geometry

Native357a656 uses the six literal positions from testKagePosArray80304b00,
verified against the read-only US DOL by verify_overworld_mario_shadow.py.
The original indexed strip becomes four triangles in the native untextured
pass. The initialized source material is black/alpha110; no shadow bitmap or
new generated art is needed for ordinary type0. Size13.333333, query origin
rootY+1/length150, height20 and returned-hit-normal matrix follow the recovered
source contract. Existing .head original collision packs supply the fresh query.
No original ISO, geometry export or compiled assets were added to source history.
Small source numeric geometry/parameter metadata is represented in the portable
implementation; original art/source rights and local-only pack policy remain.

PICA maps source LEQUAL to GEQUAL under the existing reversed-depth projection,
uses source-alpha blending, color writes without depth writes and an untextured
primary-color pass after the existing map/actor passes. This is a backend/unit/
floating-point adaptation, not a measured GX image match. Partner/NPC/battle
shadows and Mario type2 remain provisional. Type2's original60 vertices/five
lists are now available through the documentation verifier's read-only exporter;
those original payloads must stay in ignored local-assets or /tmp when imported.
No air volume asset was silently replaced by the grounded strip.

Ground pass follow-up d8ec56f preserves framebuffer alpha: recovered dispDraw
80010948–78 disables alpha writes before every camera4 callback, and Polygon
inherits that state. Native writes only RGB with no depth writes. No asset
conversion changed. The air-shadow source alpha preparation remains separate.

ID1 terminal-TEV texture/seed provenance (2026-10-12):US G8ME01 ISO read-only.
Original DOL texture803048a0,256 bytes,SHA256
1b058d9cc9f2d6d3ee6fcb0252405672529f289f553ef90cd144cad5ca3eab5d,requested
as4x64/format14/wrap0,1/mipmap0. Its payload is hash-checked,not exported into
tracked source or replaced by generated gradient. Original eye/up/target vectors
802bfa78/84/90,raw KColor seeds8041f8f4/f8=000000ff,projected IDs/unsigned bias
pinned as metadata. Actual texture decoder/GX filtering/lifetime/capture/clock/
Ortho/LookAt remain separate from original asset reuse. ELF/listing/report ignored
under artifacts/reference/behavior;no ID1 art/audio/mesh adaptation policy change.
make all first concurrent unsupported RGB enum failure was resolved by native
owner;rerun3DSX passes/log /tmp/id1-tev-terminal-build-rerun.log. Existing
sound/proj/pmario.proj,pool/sdir/samp pipeline3218 GameCube/0 Switch/5 primary-PCM
fallbacks ran. Ignored packs/catalog/WAV/RomFS outside history;original rights
remain with owners and metadata/new verifier grants no asset redistribution.

ID1 depth-TEV source/config provenance (2026-10-12):US G8ME01 ISO read-only.
Original config80304a60,144 bytes,SHA256
b01f90b7b4e7213c09322f2c3319369f4f1c0a4b8b11dd2033163d94dc731c46;two72-byte
records both route3 at+3c. Initial dswp804150e8→803c9c30,debug colors8041f8e4..
f0=ff000000/ff000000/ffff0000/ffff0000 and projected matrix IDs pinned metadata.
Embedded ramp object addresses8041e738/e758 are queried,not exported initialized
ramp/texture images. Original shadow/ramp constructor/capture/color/matrix/holder/
resource/cache/lifetime still open. No model/texture/pose/message/shadow payload
exported. Inspection ELFs/listings/report ignored under artifacts/reference/
behavior;whole2927-word audit is metadata supporting manually reviewed requests.
No ID1 converter/art/audio/geometry/lighting policy change. make all passes/log
/tmp/id1-tev-depth-build.log;existing sound/proj/pmario.proj,pool/sdir/samp pipeline
3218 GameCube/0 Switch/5 primary-PCM slice fallbacks ran. Ignored catalogs/packs/
WAV/RomFS outside history;original rights remain with owners,without new asset
redistribution license from local analysis or verifier code.

Original airborne Mario shadow pack (2026-10-12): `tools/shadow_pack.py` reads
US G8ME01 revision0 DOL from `/home/carson/games/roms/G8ME01.iso` read-only.
Ignored `romfs/shadows.pmshadow` is1096 bytes:60 original XYZ binary32 positions,
two three-entry RGBA palettes and five indexed fans/strips. Positions80304ce0
(720 bytes), palettes80415140/80415160 (12 each), lists80304b60/4ba0/4be0/4c20/4c60
(64 each) retain original words/endpoints and position/color index order.
`romfs/manifest.json` records eight source span hashes and conversion metadata.
The pack converts big-endian floats to little-endian without geometry reduction,
ring rescaling or regenerated angles. Source bytes and compiled pack stay outside
Git; original game assets remain copyrighted by their owners, with no grant of
redistribution from this converter or documentation.

Native drawing expands original fans/strips into triangles with retained winding
and applies the original collision-derived3x4 matrix at engine scale0.01. It
uses three ordered passes per record: alpha-only OR/back cull, alpha-only zero/
front cull, RGB-only destination-alpha blend/back cull; sourceLEQUAL maps to
PICA GEQUAL. Original ordinary non-AA framebuffer is RGBA6/Z24; native uses
RGBA8/depth24, so six-bit quantization, TEV/raster/blend rounding and hardware
pixel equivalence remain unverified. Source alpha0 clear and RGB-only ordinary
scene writes are now bound, with opaque→shadow→translucent stage order.
This supersedes the provisional Mario type2 ellipse; partner/NPC/battle
shadows and nonordinary/cube/camera1 cache paths remain adapters.

ID1 map-material source metadata (2026-10-12):US G8ME01 ISO read-only. Wrap
IDs802bfa44=[0,1,2],eight light IDs802bfa58,seed802bfa9c=(0,0,-1),unsigned LOD
bias802bfc20,ambient8041f908=000000ff and projected IDs802bf9f0 pinned as words/
hashes. No map/model/texture/pose/light name/message payload exported. Actual
texture name/TPL/slot/format relocation/LOD/wrap/GX lifetime remains separate.
Projected setter copies embedded object metadata to8041e6b4 and holder8041e6b0;
no image data extraction/copy/license or runtime usage proven by that setter.
ELFs/listings/report ignored under artifacts/reference/behavior. No ID1 texture
scaling/geometry/lighting/audio adaptation/converter policy change. make all
passes/log /tmp/id1-map-material-build.log;existing sound/proj/pmario.proj,
pool/sdir/samp pipeline3218 GameCube/0 Switch/5 primary-PCM slice fallbacks ran.
Ignored catalogs/packs/WAV/RomFS remain outside source history;original rights
remain with owners and local analysis supplies no asset redistribution license.
# 2026-10-12 — Goombella animation and visibility assets

Read-only original `a/c_pkr` and `a/c_pkr-` supply the ignored
`romfs/partner.pmanim`, `partner.pmmesh` and `partner-mesh.t3x` packs.
The four original clips are `PKR_Z_1` (zero-duration setup), `PKR_S_1`
(ordinary idle), `PKR_W_1` and `PKR_R_1`; the native follower plays the latter
three. Source binding and original loop descriptors are in
[Goombella ordinary poses](../behavior/partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md).
`romfs/manifest.json` records model/texture paths, poses, texture cells and bounds
under `partner_mesh`; extracted source provenance remains in `local-assets/`.

PMANIM3 retains original visibility base bytes, per-group visibility indices,
and sparse full-key modulo256 deltas. Conversion retains geometry hidden at
frame0 when a stream animates visibility, so later original keys can reveal it.
Hidden ancestors suppress descendants; siblings keep their own visibility.
Visibility is stepped, with no interpolation or procedural blinking. The old
offline bake incorrectly assigned `delta==1`; it now adds the byte modulo256.
PMANIM1/2 retain their static boolean group interpretation for compatibility.
Zero-duration, one-key poses load and sample without division by zero.

Goombella uses the original scalar2 / native engine*.01 model conversion,
without billboard fitting or procedural bob. Her original texture pages are
resized to124px with2px borders in a separate1024px atlas (the Mario atlas is
already near its64-cell capacity). Original node/position/normal/index/visibility
streams remain present; native material/lighting and turning/homing services
still need binding. Original Nintendo/Intelligent Systems assets remain local,
ignored and without redistribution rights; no extracted data is committed.
