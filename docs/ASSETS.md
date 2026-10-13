# Original asset pipeline

## 2026-10-13 — Original save event arrays and string operands

`tools/save_block_event_assets.py`, called by the field actor asset build, opens
US G8ME01 disc0 revision0 read-only. It reads selected DOL data spans at
8030b4ec/164 bytes,803bac3c/52 bytes and803ba444/2040 bytes, plus the actual
source label-scanner lookahead words and direct message operand strings.
Ignored `romfs/save-block-events.pmevt` retains original addresses/221 commands;
`romfs/save-block-program.bin` records14 source callback identities/89 sites.
Ignored `local-assets/save-block/event-program.json` records exact disc offsets,
original event/compiled segment hashes, conversion fingerprint and string paths.
The exporter excludes DOL text sections and never exports executable bodies.
Original message IDs resolve unchanged `msg/US/global.txt`; no dialogue rewrite
or replacement event array is introduced. Native source interpreter and typed
window/card request providers are platform adaptations. Original Nintendo/
Intelligent Systems rights apply; no redistribution license. See
[native core](behavior/field/NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md).

## 2026-10-13 — Original object model-name filters and full cube queries

PMNPC004 records original model names `c_tyusan`, `c_kuri_h` and
`MOBJ_SaveBlock` alongside the existing ignored bbox catalog. Those names
come from the same original asset paths already extracted read-only. The source
name-dependent floor/wall filter is preserved rather than treating every
high-bit owner as unresolved or exempt. Original SaveBlock top/side surfaces
use the unchanged original12-triangle primitive; no new geometry conversion,
textures or rights are introduced. Cached static collision invalidates on
catalog replacement and admission changes. Original Nintendo/Intelligent
Systems rights/local-only restrictions remain. See
[native collision binding](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md).

## 2026-10-13 — Original map-object collision metadata

`tools/field_actor_assets.py` reads minXYZ/maxXYZ directly from original
`local-assets/map-objects/MOBJ_SaveBlock` at0xd0..0xe4, extracted by the existing
read-only ISO pipeline. Ignored PMNPC003 `romfs/field-actors.pmnpc` and `.json`
retain those source floats, original object name and explicit hit mode. No
rendered mesh bounds or fabricated radius are substituted. Runtime cube uses
original unit vertices/strip order documented in the source contract, keeping
head collision independent of S_1/A_1. Conversion *.01 and native float matrix
requests remain adapters. Original Nintendo/Intelligent Systems rights and
local-only/no-redistribution restrictions remain unchanged. See
[native head binding](behavior/field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md).

## 2026-10-13 — Original save-block surface conversion

`tools/map_object_assets.py` extracts original `a/MOBJ_SaveBlock` and its
header-selected `a/MOBJ_SaveBlock-` companion from the read-only US ISO into
ignored `local-assets/map-objects/MOBJ_SaveBlock` and `MOBJ_SaveBlock.tpl`.
Ignored `romfs/field-saveblock.pmanim`, `.t3x` and `.json` preserve original
S_1/A_1, hierarchy, geometry, RGBA, pass/cull and texture transforms. The
manifest records disc/path hashes and converter fingerprint. Source material
binding override3 sets repeat on both axes despite clamp in the TPL header.
Triangle subdivision preserves interpolation across wrap boundaries;124px
texture cells use2px opposite-edge repeat gutters and RGBA8 Tex3DS conversion.
A neutral white cell multiplies original untextured vertex colors; no generated
replacement artwork is used. PMNPC002 records the original plaza placement,
scalar10, story341 exclusion and constructor mask0x20. See
[native surface format](behavior/display/NATIVE_ACTOR_SURFACE_PACK.md).

Nintendo/Intelligent Systems original resources have no redistribution license.
Extracted banks, atlases, compiled assets, fixtures and captures remain outside
source history. Geometry/texture downscaling, native floating-point matrix
arithmetic, lighting and global translucent ordering remain adaptations.
This supersedes the earlier entry's deferred save-block conversion status.

## 2026-10-13 — Area NPC catalog and original east-side Mouser

`tools/field_actor_assets.py` reads `m/gor_02/s` from the local read-only US ISO
into ignored `local-assets/field-actors/gor_02.s`. The selected original
`チューさん３` row retains its tribe, XYZ and direction. Source resources
`a/c_tyusan` and its header-selected texture companion `a/c_tyusan-` stay under
the same ignored folder. `romfs/field-chu.pmanim` preserves original `CHU_S_1`
sparse animation and hierarchy/visibility. Textures use the established124px
cells with2px edge borders; atlas height trims to the next128px power of two,
and explicit UV scale preserves atlas addressing. Tex3DS converts RGBA8 for
citro3d. Original scalar2 and engine*.01 conversion remain separate from art.

Ignored `romfs/field-actors.pmnpc` and `.json` hold model namespaces, source
placements, narrow native story envelopes and door visibility admission.
Frankly reuses original `a/c_kuri_h` / `a/c_kuri_h-` resources compiled by the
existing professor converter. Per-model/source provenance JSON records source
disc/path/hash and conversion fingerprint. Assets and compiled packs carry
original Nintendo/Intelligent Systems rights with no redistribution license;
none are committed. See [native catalog](behavior/field/NATIVE_AREA_ACTOR_CATALOG.md).

Save-block resource `a/MOBJ_SaveBlock` and its texture companion were read into
the ignored workspace for conversion checks. Its original S_1 uses outside0..1
UVs and scrolling texture keys; ordinary NPC conversion rejects this domain.
No incomplete save-block pack is included in the catalog. Its source placement,
scalar10 and hit/save services are documented in
[the source contract](behavior/field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md).

Original US GameCube G8ME01 assets are read from the local ISO at build time;
extracted files, PCM libraries and compiled packs stay in ignored
`local-assets/`, `romfs/` and `artifacts/` paths and remain outside source
history. Nintendo/Intelligent Systems game data carries no redistribution
rights: no ISO, raw banks, PCM, dialogue, keys or compiled packs are
committed. Behavior-research provenance and the conversion pipeline reference
stay in this file; dated per-topic provenance, conversion choices and
licensing notes live in the chapters below.

## Asset chapters

Professor Frankly: [converter](../tools/professor_assets.py) reads `a/c_kuri_h`
and its `a/c_kuri_h-` texture bank from the read-only US ISO. It exports source
Z_1,S_1,T_1,W_1,R_1,D_1 as PMANIM3, retaining source geometry/hierarchy/visibility/
animation streams. Textures follow the established124-pixel cells with2-pixel
edge padding. The12 used texture cells occupy1024x256, so unused rows are
cropped without resizing or color quantization. Native V adaptation uses the
pack's4x scale, reducing RGBA8 texture allocation from4MiB to1MiB. Source
animations/UVs stay in their standard full-atlas domain for independent tests.
Ignored `romfs/professor.pmanim`, `.pmmesh`, `.t3x`, `.bin`, `.json` and
`local-assets/professor/` retain generated data/provenance, with no redistribution
rights for Nintendo/Intelligent Systems assets. The DOL tribe's literal `d_1`
differs from bank `D_1`; no invented case alias is introduced. Full NPC pose
selection/yaw/scalar/backend binding remains requested in the inbox.

Same-map inn/professor doors: [source contract](behavior/overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md)
and [converter](../tools/door_assets.py) use `m/gor_01/d`, `m/gor_02/d` and the
verified `rel/gor.rel` names/layout from the read-only US ISO. Existing original
geometry and128-pixel texture conversion is retained. The two interior display
subtrees are tagged, preserving their geometry for event visibility. No new
art or dialogue is generated. Ignored `local-assets/doors.json` records hit
identities, derived EVT destinations and collision IDs; ignored
`romfs/doors.pmdoor` contains the native bindings. Floating normal/coordinate
math uses the converter's host arithmetic, followed by EVT integer truncation;
SDK/FPSCR bit parity is not claimed. Nintendo/Intelligent Systems data has no
redistribution permission and these packs/assets are excluded from commits.

| Chapter | Contents |
| --- | --- |
| [Audio](assets/AUDIO.md) | DSP validation firmware, sound mapping and GameCube fallbacks, Switch audio extraction, complete SFX sample extraction, full loopable audio library |
| [Scripts, messages and effects](assets/SCRIPTS_MESSAGES_EFFECTS.md) | Original event script assets, US-default disc-thread system-message resources, shared effect texture source |
| [Field meshes and roads](assets/FIELD_MESHES.md) | Static head collision mesh, floor slope runtime selection, source-facing floor pack, collision mesh reused for wall movement, ROAD metadata conversion |
| [Camera](assets/CAMERA.md) | Camera lifecycle evidence, original camera container pack, dated camera native-binding entries |
| [Mario models, animation and poses](assets/MARIO_AND_POSES.md) | Mario data-child pack, ordinary head/airborne/support bindings, model mesh pack, sparse animation pack, low-HP/idle pose sources, grounded shadow geometry |
| [Switch static research](switch/STATIC_RESEARCH.md) | Switch 1.0.0 extraction, licensing and static-analysis research record |

## Behavior research provenance (2026-10-12)

`tools/behavior_evidence.py` inventories the read-only ISO and hashes its DOL
span, `rel/gor.rel`, and all 260 `msg/US/` files without exporting their contents.
Output is ignored `artifacts/reference/behavior/evidence.json`; function ELFs
and disassembly also stay in that directory. There is no conversion or native
linking of original executable code in this research step. Binary identity,
source URLs and inference boundaries are in [ORIGINAL_BEHAVIOR.md](ORIGINAL_BEHAVIOR.md).

Public doldecomp/ttyd reference source and its MIT LICENSE were downloaded at
revision `62131fc31866f85a12e4817a8dc37f0c25eb5f8c` into that ignored directory.
Its declared baseline is Japanese G8MJ01 revision 0; selected findings were
checked against US original bytes before documenting US behavior. No upstream
implementation was copied into the native game. The existing external US symbol
CSV remains research metadata from GPL-3.0 ttyd-utils. Extracted original assets
remain subject to their original rights; neither reference license grants rights
to distribute the ISO, messages, executable or converted asset packs.

## Existing conversion pipeline

Additional 2026-10-12 behavior research: the pinned public source snapshot now
includes src/include/config and rels under ignored
`artifacts/reference/decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c/`.
`decomp_coverage.py` parses its configuration without executing it; report is
ignored `artifacts/reference/behavior/decomp-coverage.json`. The binary report
now fingerprints 359 selected US functions and includes two seven-byte defense
window tables. Independent verifier checks 1916 instruction anchors plus those
tables, the binary32 accuracy multiplier and all twelve counter jump-table
targets directly in the read-only ISO. Direct-damage research additionally
verifies nine presentation float constants and seven sound-key bindings.
Status storage research checks all81 getter/setter/recovery jump-table targets.
Infliction/presentation research fingerprints ten additional inspected case
tables,254 entries total, and checks five status effect position constants.
The application tail documents sound-key bindings, payload writes and completion
semantics; effect and announcement service implementations remain open.
Orchestration research adds five float and three sound bindings plus the
Pikkyoro zero candidate initializer; its guard/selection helpers are fingerprinted.
Turn recovery research checks the78-entry sequence-table hash,23 recovery
calls, two sound keys and Freeze scale. The report inventories direct branches
to the recovery helper across DOL text; indirect/REL/script callers are excluded.
Periodic damage research fingerprints Burn effect scheduling and shadow guard,
with four binary32 effect constants checked directly against the ISO.
Regeneration research adds HP recovery helper identity and the small original
regeneration wait-event encoding check; no event-script extracts are committed.
Recovery-event research fingerprints three complete script spans and13 additional
US callbacks/helpers. Script semantics are documented as prose; original bytecode
remains ignored. Local-work operands and owner fields are preserved separately.
Battle barrier/queue/conclusion research adds eight helper fingerprints and
local contracts, including reservation consumption order and group-result writes.
Inventory removal/reservation research adds three routine fingerprints and two
refund constants, preserving parallel-array compaction and deferred consumption.
Move-count research adds six function fingerprints and a direct DOL reset-call
inventory, excluding indirect/REL/script callers. Signed-byte narrowing and
status predicate boundaries are documented explicitly.
Phase actor/event table research adds eight function fingerprints, the15-entry
move dispatch-table hash and two position-sort constants. The complete local move-controller state map is now specified; action-controller
local state map is now specified as well, with its callee/script dependencies open.
Action-controller research additionally checks the15-entry dispatch-table hash
and camera-speed threshold. Retained Electric RNG draws and the two announcement
drain phases are documented without a dynamic-equivalence claim.
Default action event research adds two script fingerprints, seven helper
identities, mode8 interpolation target/constants and original zero-gravity
message-ID binding. No original dialogue or bytecode extract is committed.
Camera request research adds ten callback/setter fingerprints and the mode-reset
zoom constant, preserving independent priority gates and homing vector sources.
Announcement research adds five service/draw fingerprints, four floats and
window color. Duplicate local symbols use an explicit namespace qualifier in
the metadata tool; original dialogue/text buffers remain uncommitted.
Message lookup/loading/tokenizer research adds five fingerprints. Read-only
message_resource_evidence.py inventories260 US files and13,186 pairs, with
byte offsets/text hashes and two duplicate keys; metadata output stays ignored.
No dialogue extracts or encoding normalization are committed.
Parser research adds msgAnalize/msgInit fingerprints, two command/format block
hashes, layout constants and original key-wait pose bindings. Prose records local
display record construction; original parser code and message text stay ignored.
Embedded-object research adds msgIconStr2ID, all18 US name/ID bindings and
geometry constants. Census schema2 adds selected control argument shapes without
exporting dialogue or annotation text; code-generated/non-US input remains open.
Playback adds msgMain/keyGetButtonTrg fingerprints, all14 dispatch targets,
original scroll binary32 coefficient and seven paper pose/group bindings.
Timing contracts are derived from clock arithmetic, not measured gameplay.
Window lifecycle adds12 service/event fingerprints and default/small geometry
constants; original pool/heap failure behavior and caller identity remain open.
Selection adds seven input/window/event fingerprints, display-order constant
and original direct-input identity. Input repeat cadence remains unmeasured.
General/clear window states add two fingerprints, all12 dispatch targets,
24 sound table values and the original history-scroll coefficient. Fade and
countdown arithmetic are static contracts, not observed durations.
Common print research adds three fingerprints, projection threshold and ten
original special NPC names. Speaker/identity contracts do not prove story order.
Print variants add eight fingerprints, all18 original partner suffix bindings
and the field speaker literal. Shared key buffers remain original-runtime lifetime
dependencies; no generated dialogue or extracted resource text is committed.
Insert formatting adds two fingerprints. Census schema3 records per-pair and
aggregate printf-like lexemes (93 pairs/119 matches), explicitly not execution
or a format-call census. Original format/dialogue bytes remain ignored assets.
Format output/ABI research adds five runtime fingerprints and the original narrow
null-string sentinel. No original library implementation is copied into source.
Directive parser adds one namespace-qualified printf fingerprint (distinct from
scanf), both dispatch-table hashes and byte-classification hash/mask checks.
Integer conversion adds two fingerprints and both33-entry dispatch-table hashes;
the documented prefix/capacity stages are static evidence, not executed outputs.
Decimal rounding/preparation adds two fingerprints, the original special-value
string-block hash, sign-comparison zero and case-classification checks.
Binary-to-decimal generation is covered by the subsequent entry/helper research.
Finite layout research extends the existing float2str fingerprint's prose and
anchors to local scientific/fixed/general paths. Actual call ranges and executed
layout/capacity behavior still need recovery/comparison.
Binary decimal entry adds two fingerprints and its separate input-zero constant;
numeric/ASCII phases and generator request boundaries are documented. Runtime
floating environment and actual caller ranges remain explicit dependencies.
Numeric decimal multiplication adds one fingerprint and local convolution,
scratch, 36-digit retention and rounding contracts. Actual caller domains and
executed numeric behavior remain unverified; no original helper body is
incorporated into the native runtime.
Decimal power research adds one fingerprint, the73-entry dispatch identity and
184-byte seed-only block identity; the21 seed bindings are independently checked.
Original numeric strings/instructions remain in the ignored read-only research
outputs. Contracts record the original ASCII comparison and recursive rounding;
no original numeric service is compiled into the prototype.
Binary numeric services add five fingerprints and six binary64 constants.
Contracts retain raw special-value/signed-zero branches, scaling arithmetic and
word-conversion saturation; derived finite generator power bounds are not
executed-original observations. No upstream numeric implementation is copied.
Unsigned64 quotient/remainder add two fingerprints and local restoring-loop
contracts. An authored recurrence check and zero-divisor derivations remain
ignored metadata, explicitly separate from original instruction execution.
Hexadecimal float research adds one fingerprint; special strings reuse the
already verified original block. Documentation preserves raw exponent/fraction,
truncation and extended-precision stack-address calculations. No original
formatter body is compiled into the prototype or distributed as source.
Window manager/selection drawing add seven fingerprints, source path bindings
font/msgWindow.tpl and %s/f/msgWindow.tpl, and selection color/geometry/smoothing
constants. Public Japanese windowdrv.c supplies structural corroboration; US
static instructions establish the contracts. No palette/GX objects are extracted
or converted in this step, and no original renderer is compiled into the prototype.
Display queue/dispatch adds eight fingerprints (namespace-qualified comparator),
all eleven offset floats despite the symbol's undersized annotation, eleven GX
dispatch targets and five depth/common-state constants. Japanese source provides
structural corroboration; original US instructions/tables establish the contracts.
Sorting internals are covered below; outer rendering cadence remains open.
Sort implementation adds two fingerprints. Pinned Japanese system.c provides a
complete structural guide checked against original US partition/cycle branches;
no upstream implementation is compiled into the prototype. Authored model reports
for3549 sorting cases,5914 cycle permutations and8072 bit-loop counts remain
ignored and are not original execution evidence.
General window drawing adds three fingerprints, twelve type targets, fifteen
geometry/indicator floats, mode0 interpolation target and unsigned32 conversion
constants. Cursor pose pointer binding is recorded; original renderer/animation
services and visual assets are not copied into the native runtime in this step.
Draw record admission adds three fingerprints and reserved-control dispatch,
pose orientation/icon/glyph constants and emphasis color/vector bindings.
Only the admitted/control/embedded paths of msgDisp are locally documented in
that step; its normal glyph effect tail is covered below. Original
glyphs/pose/icon data remain reused assets outside source history.
Glyph effects extend the existing msgDisp fingerprint's recovered local coverage
with priority/RNG/clock/matrix/font requests and draw-time record relaxation.
Effect literals and all five scale-vector templates are independently verified;
numeric/trig/font helper implementations and executed images remain open.
Font state adds fourteen fingerprints, original three-color palette/default-scale
and noise texture-matrix constants. Original window palette object50 is a request
binding; no descriptor/image conversion or replacement noise asset is generated.
Byte-alpha arithmetic checks are authored derivations, not original execution.
Font glyph/setup requests add three fingerprints and original texture-matrix
mode constants. Static font texture object0x8041e97c is a request binding;
its glyph/page/image ownership is still unrecovered. No font assets or original
raster implementation are added to native code/source history in this step.
Inner glyph quads add two fingerprints and original HSV/bias/two-unit constants.
Font block/page assets remain unextracted in this step; local FIFO/quad requests
and mandatory two-draw RNG behavior are documented independently of measured
GPU images. No original raster implementation is compiled into the prototype.
Hit-precheck and damage-calculator research adds prose contracts and metadata;
no executable conversion or third-party implementation is incorporated.
No full original routines, source extracts,
dialogue or assets were added to history. The detailed behavior documents explain
the scope and do not claim the original binary has been executed or recompiled.

The user's latest preference supersedes the initial hybrid-placeholder plan:
use assets very similar to the original game, preferably original geometry and
textures; scale textures/reduce vertices/adjust lighting for performance.
Preserve original sounds and music. Do not reintroduce the earlier vector redraws
or synthesized sound effects.

`tools/iso_assets.py` reads the FST from the legally acquired `G8ME01` ISO. It
validates bounds and filenames, inventories all files and extracts exact selected
paths. Extracted files receive SHA-256 provenance sidecars. The source ISO is
opened read-only. Original/converted content belongs under ignored local-assets,
romfs or artifacts directories, never source history.

`make` invokes `tools/build_assets.py`. Set TTYD_ISO to change the input path.
Selected original asset sources:

- `a/a_mario-`: complete Mario pose texture 0.
- `a/c_pkr` and `a/c_pkr-`: Goombella, animation PKR_Z_1 at frame 0.
- `a/c_kanbu` and `a/c_kanbu-`: Crump, animation Z_1 at frame 0.
- `a/c_kinopio` and `a/c_kinopio-`: Toad, animation KNP_Z_1 at frame 0.
- `m/gor_01/d` and `m/gor_01/t`: original plaza mesh, scene graph and textures.
- `b/gor_01.tpl`: room backdrop.
- `m/stg_01/d` and `m/stg_01/t`: original common battle stage.
- `sound/stream/stg_gor1_32k.stm`, `btl_zak1_32k.stm`, `btl_win1_32k.stm`:
  original Rogueport, normal-battle and victory music. Confirm encounter-specific
  track choice in Dolphin; do not assume normal battle music is the tutorial's.
- `sound/proj/pmario.{sdir,samp,pool,proj,etbl,slib}`: preserved original sound bank.

GX decoding covers I4/I8/IA4/IA8/RGB565/RGB5A3/RGBA8, indexed palettes and CMPR.
Only the base mip level is used. Map texture dimensions are capped at 128 pixels
and normalized to powers of two for native repeat sampling. Original character
poses are rasterized from source geometry to 128-pixel billboards. This is an
intermediate optimization; proper original animations and direction variants
must replace the idle-only display. No animation timing is inferred from the
atlas.

The map converter flattens original S subtree transforms and triangle topology
at scale 0.1, preserves texture coordinates, material colors, wrap modes and
alpha-layer grouping. The current plaza has 40,714 triangles and 79 batches; the stage has 10,801
triangles and 14 batches before static visibility overrides. The mask, closed
upper curtain and inactive switch props are suppressed for this playable view;
verify these inferred visibility rules against the original battle scene. Exact
current counts are in romfs/manifest.json. Near-ground original collision triangles are reduced
to 711 planar wall segments. The walkable subset still needs original floor,
slope, trigger and visibility semantics.
Geometry decimation, dynamic visibility and original map animations are pending.
Materials with multiple samplers are explicitly reported rather than assumed
correct. The selected room currently has none.

STM conversion preserves rate/channels and original loop points, clamped to
valid source sample bounds. Runtime streams PCM; it does not load the entire
music track into RAM. MusyX directory indexing exposes 1,247 original samples;
macro-to-game-event resolution is still needed before faithful effects play.
Unresolved effects remain silent, not replaced with generated tones.

Format research used noclip.website (MIT) and vgmstream (ISC); corresponding
license notices are in third_party. Nisto/musyx-extract was read for format facts;
its unlicensed source is not incorporated. No Paper Mario decompilation code is
currently incorporated. Record licenses before adding any external code.

Ground surfaces are exported from the original collision subtree into
map.ground (little-endian uint32 count, then nine float32 coordinates per
triangle). Slopes with absolute normalized Y normal >= .65 are retained. This
is a provisional traversal filter, not an observed original engine threshold.
Original rel/gor.rel is extracted read-only with provenance for exit/event
analysis; no original executable code is run by the remake.

Original overworld packs now include m/gor_00, m/gor_02 and m/gor_03.
rooms.bin contains a uint32 link count followed by records (source room uint32,
target room uint32, exit X/Z, spawn X/Z/Y float32), 28 bytes each. Road IDs
0/1/2/3 map to gor_01/gor_00/gor_02/gor_03. The manifest records original REL
offsets, marker-derived coordinates and target/arrival identifiers. Wall record
count high bit now indicates min/max Y following each XZ segment (six floats).
Unknown story-dependent exits are not activated by this road-only parser.

Wall deduplication includes the original height interval in addition to X/Z,
so stacked surfaces do not overwrite one another. Floor triangles also supply
per-room footprint bounds for touch-map scaling; runtime uses a spatial index
without modifying original geometry.

Map batches now reserve bits 8–23 of their layer word for an exterior-wall
cutaway group ID (0 means ordinary geometry). Original layer remains in the
low byte. Group owners are listed as cutaway_nodes in the manifest; S_gai_kabe
subtrees provide the classification, and drawable descendants inherit an
already selected owner. Geometry positions, UVs, colors and textures remain
original. Group-based batching increases draw calls without duplicating triangles.
This classification is a rendering adaptation, not recovered original visibility
event rules. The upstream viewer's runtime flag handling was inspected at
https://github.com/magcius/noclip.website/blob/master/src/PaperMarioTTYD/render.ts
for further visibility/event work; no code from that file was copied here.

Rogueport station (`m/gor_04/d`, `m/gor_04/t`) now converts through the same
original-map pipeline: 41,398 triangles, 50 batches, textures capped at 128.
No exterior-wall classification matches this room (zero cutaway groups).
Original `rel/gor.rel` records at 0x4de80 (west n_bero) and 0x5d53c
(station s_bero) establish the reciprocal road. Station's other geometric
n_bero marker is not an additional inferred road; train/event travel is unfinished.
Arrival elevation uses the marker base, not its volume midpoint.

Original per-node culling is read from the draw-mode pointer at node+0x58,
byte+1. Converter retains GX indices 0–3, rejects invalid drawable-node values,
and records cull_triangles counts. Hierarchy-only nodes can have a null pointer.
Format reference: noclip.website src/PaperMarioTTYD/world.ts (MIT), already
used by this converter. No visual guesswork is used to classify mesh sidedness.
Batches now split by sidedness as well as material/cutaway owner. Geometry
counts are unchanged; plaza/harbor/east/west/station batches 116/79/111/127/54.
This does not recover event-controlled visibility, train animation or multilayer
materials. Those remain necessary for original presentation.

Map mesh animation tracks are now preserved in ignored local conversion sidecars
`local-assets/<room>/map.animations.json` (plaza uses `map`, battle `stage`).
The original .d source remains authoritative, including the 12 unknown components
per mesh keyframe not interpreted here. Map summary/manifest lists names, duration,
mesh-track counts and material-table offsets. Mesh sampling uses original
translation/rotation offsets, scale divisors, step flags and Hermite tangents.
Format reference: existing MIT noclip.website/world.ts. Event activation and
loop modes are not inferred. Material animation tracks are not decoded yet.
Station has shadanki, n_1_door_open/close mesh tracks; train-body mesh animation
is absent from that table. Its visual defects still require material/scene/event
inspection. Harbor contains duplicate fune_b animation names; explicit name
selection rejects ambiguity rather than choosing an arbitrary duplicate.

Material animation tracks are now decoded into local map.animations.json: five
channels (S/T translation, S/T scale, rotation), centers, texgen index, key times,
Hermite tangents and step flags. Offline preview binds texgen0 by original
material names (including _v/_x-normalized aliases). Higher texgens are reported
as unsupported, not silently accepted. Shared texture_matrix supplies both bind
and animated texture transforms; no runtime animation is enabled automatically.

Downloaded read-only public US symbol metadata for event inspection from
https://github.com/jdaster64/ttyd-utils/blob/master/resources/us_symbols.csv
to ignored artifacts/reference/us_symbols.csv. Upstream repository is GPL-3.0;
no upstream code or table is copied into native runtime or source history.
Independently written map_event_inventory.py resolves original REL local/DOL
relocations, uses externally supplied symbol names/ranges, and inventories the
original init command stream to ignored local-assets/map-init-events.json.
It does not execute scripts or evaluate story conditions.

Station road-entry conversion now recursively suppresses gondora_2 and hako_1.
Original gor_04_init_evt has IF_STR_EQUAL(entry, hikousen_return) at0x604d0,
ELSE at0x604e4 and END_IF at0x60518; ELSE sets group hidden bit1 at0x604e8
and0x60500. The currently supported station entry is s_bero, so ELSE applies.
Both groups are hierarchy-only owners; their314 child triangles are omitted,
leaving41,084 triangles/54 batches. Original collision is unchanged.

inspect_dol_function.py creates inspection-only ELF files from named functions
in the read-only US ISO, using external US symbol ranges and llvm-objdump.
No original executable code is run or linked into the native game. Original
function bytes/disassemblies stay ignored in artifacts/reference. Primary
reference evt_cmd.h was read from jdaster64/ttyd-tot for string conditional
opcodes; doldecomp/ttyd mapdrv sources inspected but do not implement these
flag functions. Semantics below derive from the user's original DOL bytes.

East gor_02 now packages mizu/noname/ie_01_light texture-zero ambient tracks
identified at original init offsets0x4d694/0x4d6a8/0x4d6bc, outside enclosing
conditional blocks. The source starts each with mode1/clock0. Actual US DOL
mapPlayAnimationLv0x80029de4 sets loop flag0x4 for mode1 and speed1; mapMain
0x80026bb8 wraps against the full authored duration (not duration-1). It
calculates frames from elapsed milliseconds times gp->framerate/1000. US
marioStInit 0x8006fb3c (symbol range; see local disassembly) writes60 to offset4
for VIGetTvFormat()==0 and50 otherwise; NTSC US input selects60. Primary
field reference: doldecomp/ttyd/include/mario/mariost.h (framerate at0x4).
No original executable implementation is copied into the port.

UVAN tables bake original Hermite/step curves at integer60Hz frames. Binding UVs
come from inverse original bind texture matrices, preserving existing vertex
layout and material batching. gor_02.uvanim is41,856 bytes, updates369 vertices
(135/144/90) in tracks with durations999/159/400. All other room/stage files
contain valid zero-track headers. Manifest records clip IDs and bound counts.
East water texgen1 is still missing; do not claim complete water reproduction.

Original ambient mesh packs now include plaza m/gor_01/d hae (duration399,
2 owner tracks,12 vertices,38,524 bytes) and harbor m/gor_00/d nami
(duration979,3 owner tracks,4,581 vertices,214,308 bytes). Harbor UVAN enables
nami/nami_02/hiki_nami (4 bound tracks,5,451 vertices,143,144 bytes). Original
init playanim calls hae0x3c798 and harbor0x2f31c/0x2f330/0x2f344 are outside
conditional blocks; loop/clock/NTSC60 semantics follow the prior DOL inspection.
All rooms and stage package mandatory MSAN headers; unused rooms have zero
tracks. Original files remain read-only and generated packs remain ignored.

Harbor nami references fune_b_lambert83, absent from this map material table;
that channel has no visible geometry binding and is recorded in limitations.
Original two-sampler harbor materials still render texture zero only. Neither
missing channels nor conditional boat animations are claimed faithful.
Mesh verifier compares five native baked poses against explicit offline original
clip samples for both rooms, including ancestor transforms, with unchanged
collision/batch files. It does not prove original event or camera behavior.

Floor packs now preserve original drawmode+8 attributes and map-local source-node
IDs. Independent verify_ground_attributes.py checks all3,576 floor records in
the five original room files directly against raw extracted node bytes. Counts
are plaza1010/harbor463/east878/west1037/station188. Harbor A_mizu contributes
16 floors carrying0x200. Source-node names/attributes are indexed in room
summaries and runtime manifest; unknown bits are retained unchanged. Geometry
and normal filtering remain unchanged; tagged records add8 bytes per triangle.

Original US DOL _hitEnt0x8001896c:80018aa8 loads joint+0x58,80018ab4 loads
drawmode+8,80018ab8 stores hit-entry+4 (null drawmode stores0). hitGetAttr
0x80014310 reads that word or returns0 for null. These original read-only byte
inspections substantiate raw static attribute mapping; implementation is native
and does not link/copy original executable functions. Inspection artifacts stay
ignored. Effective event mutations remain separate unfinished work.

room_convert.damage_returns now reads original US evt_hit_damage_return_set
imports (8007458c), validates two-argument USER_FUNC event headers, follows
self-REL relocations and reads bounded16-byte name-pointer/XYZ tables. It rejects
missing relocations, nonfinite values and malformed termination. Source call and
table/entry offsets, ordering, names, coordinates and Y=-1000 auto-position
markers are preserved. No external symbol CSV is required to build these tables.

Build produces ignored local-assets/damage-return-tables.json and records both
tables in ignored RomFS manifest provenance. These are metadata for recovery
implementation; runtime does not yet consume them. Verify with
`python3 tools/verify_damage_returns.py`. Original function-address/layout evidence
is retained in mechanics notes; no original executable implementation imported.

Resolved original init return packs: gor_00 has5 node bindings (92 bytes),
gor_02 has16 inherited bindings (268 bytes); other room files are12-byte empty
RPOS headers. Original tables still retain raw positions/sentinels and call
offsets; manifest additionally records resolved positions and descendant IDs.
Harbor automatic A_yuka center is approximately(.85,.15,2.2). Literal positions
now correctly use engine-to-native0.01, substantiated by original _mapLoad's10x
model matrix. All original collision owners/parents/centers are inventoried,
not just mesh owners. Source floor geometry/raw attributes remain unchanged.

Original wall collision attributes are now preserved alongside geometry in
32-byte records (six floats,two uint32 words); tagged count uses0xc0000000.
Source-owner IDs and flags come from original node/drawmode records, with no
attribute edits. `python3 tools/verify_wall_attributes.py` independently compares
all5,264 records against source bytes across gor_01/00/02/03/04. No bit31
special-case owners occur in these packs. Collision mesh extraction remains
approximate planar wall segmentation; original flags now control solidity.

Wall packs now additionally retain all9 triangle coordinates, unrounded, after
the geometry/attribute prefix:68 bytes per record, count tag0xe0000000.
Distinct triangles belonging to the same owner are no longer collapsed merely
because their projected segment and min/max heights match. Five rooms export
7,830 retained triangles;92 records are non-solid under normal-Mario flags.
`verify_wall_attributes.py` validates the new layout and original owner flags.
The existing wall selection threshold(abs(normalY)/length<=0.2) is unchanged;
coverage of other angled surfaces and dynamic hit meshes remains unfinished.

## 2026-10-12 — Story-controlled outdoor geometry groups

Original `m/gor_00/d`, `m/gor_02/d`, `m/gor_03/d` geometry now retains
source subtree identities for ordinary-entry visibility recovered from
`rel/gor.rel`. The read-only ISO remains the source; local extraction and
compiled packs remain ignored. Conversion splits material batches by event
group and records it in bits26..31, preserving original vertex values, texture
scaling, collision and animation bindings. Geometry stays available for later
story conditions. The harbor ship groups/wakes, east interior/gate and west
alternate foliage use recovered initialization conditions; full event overrides
remain open. See [visibility contract](behavior/field/ROGUEPORT_STORY_OBJECT_VISIBILITY.md).
Nintendo original asset licensing/provenance is unchanged: local personal-use
conversion only, no ISO or extracted/compiled assets committed or distributed.

## Goombella follow jump pose (2026-10-12)

The existing original `a/c_pkr` model and `a/c_pkr-` texture resource now also
compile `PKR_J_1A` into ignored `romfs/partner.pmanim`, `partner.pmmesh` and
`partner-mesh.t3x` via `tools/build_assets.py`. Source selector4 is pinned by
the [Goombella pose contract](behavior/partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md).
The existing PMANIM3 visibility/scalar2 conversion is retained; no generated
art or substituted sound is introduced. Source extraction stays read-only and
all converted packs remain outside source history. Nintendo/Intelligent Systems
assets carry no redistribution permission; no original files are committed.

## Original dialogue bubble masks (2026-10-13)

`tools/message_window_assets.py` reads `f/msgWindow.tpl` from the US ISO opened
read-only (file offset947396944,size321984,SHA-256
`2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67`).
Original ordinary cap/edge indices9..16, tail indices1/2 and small masks3/4
are decoded from GX to RGBA8. The cap/edge masks are composed at their original
560x176 positions, filling only the flat interior with their matching source
color. Ignored `local-assets/fonts/msgWindow.tpl`, `window-*.png` and
`message-window.png` retain extraction/conversion inputs; ignored
`romfs/message-window.t3x` is a1024x512 native atlas and
`romfs/message-window.json` records source hash/offset/converter fingerprint.
There is no generated bubble art. Native drawing scales to400x240 and adapts
the original tail to a rotated mask; see the
[native dialogue contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md).
Nintendo/Intelligent Systems assets carry no redistribution permission. These
local assets/compiled packs and the ISO remain outside source history and are
not distributed.

## Speaker anchor metadata refinement (2026-10-13)

The same read-only original `a/c_kanbu_ibt`, `a/c_kanbu-`, `a/c_zako_n` and
`a/c_zako_n-` inputs now produce ignored `romfs/harbor-actors.bin` in PMHNPC2
format: existing billboard metrics followed by two three-float native talk
offsets. `tools/harbor_actor_assets.py` derives these offsets from the opaque
(alpha>8) bitmap top and horizontal center, excluding transparent128x128
margins. Ignored `romfs/harbor-actors.json` records the offsets and updated
converter fingerprint. Goombella's anchor uses the already imported original
`PKR_S_1`/`PKR_W_1` evaluated geometry; no new character art is created.
The bubble atlas is unchanged. Native border/glyph drawing now scales both
axes by0.5 to preserve original4:3 artwork on the wide display, and tail masks
use the source sheared quad rather than a rotated sprite. These are native
anchor/aspect adaptations, not recovered NPC tribe heights. Original rights
and local-only asset/cache exclusions remain unchanged. See the
[native bubble contract](behavior/message/NATIVE_MESSAGE_BUBBLES.md).

The2026-10-13 tail-stretch refinement reuses the same original48x64 tail masks
(indices1/2), without editing their pixels or rebuilding the atlas. Native
geometry compensates each mask's transparent margins so their last opaque
texel centers terminate together near the speaker. The four-screen-pixel gap
is a user-requested presentation adaptation, not measured original behavior.

## 2026-10-13 — Native opening assets

`tools/opening_assets.py` reads the US ISO with the existing read-only extractor.
Raw/conversion intermediates stay in ignored `local-assets/opening/`; runtime
resources and the provenance manifest stay in ignored `romfs/`. No asset, ISO,
original dialogue, executable or compiled pack is included in source commits.
All original assets retain Nintendo/Intelligent Systems rights; these conversions
grant no redistribution license. Existing tool-reference licenses still apply.

Title uses `mariost.tpl` indices5(background),21(US logo),16(English START mask),
9(US copyright). The letter uses palette4, requested by the original gor_10
`draw`/winTexSet call after `tegami` loads `%s/mariost.tpl`. Storybook uses `demo.tpl` desk0, cover1 and pictures8..12.
Large608x480 images become304x240 with Lanczos, smaller masks retain their pixels,
and transparent power-of-two padding is excluded by checked `.size` metadata.
The US `sys.rel` prologue heading table at0x2648 supplies19,20,21,22,21,23,24,25
and terminator255; the corresponding `e/us/effect_sys.tpl` letters are composed
without redrawing or replacing their text. Native heading layout is an adaptation.

Original `m/aaa_00/{d,t}`, `m/gor_10/{d,t}` and `m/muj_20/{d,t}` supply opening
room meshes/materials. Geometry remains source-derived; texture axes are capped
at128px using the established map converter. Original `a/c_luigi`, `a/c_paretta`,
`a/c_peach`, `a/c_nazo` and their `-` texture banks supply original L_5/S_1/P_S_1/S_1
clips; Luigi additionally retains L_7/L_8/L_9 for the original talk/read poses.
Mesh/animation hierarchy and model coordinates remain intact. Unused
1024x1024 atlas rows are cropped to the smallest128-based power-of-two height;
`.uvscale` compensates V coordinates while retaining all occupied cells.
Map presentation uses `mariost.tpl` palette3 requested by aaa `draw`/winTexSet.
The outside/interior house variants retain the corresponding original visibility
requests. Current scene snapshots do not reproduce the full source animation choreography.

The six original streams are `sound/stream/sys_tit1_32k.stm` (BGM_TITLE1),
`sys_demo1_32k.stm` (BGM_DEMO1), `sys_demo2_32k.stm` (BGM_DEMO2),
`evt_mri1_32k.stm` (BGM_EVT_MARIO_HOUSE1), `sys_opn1_32k.stm` (BGM_OPENING1),
`evt_kou1_32k.stm` (BGM_EVT_SAILING1). Decoding preserves original rates/channels
and loop intervals in PCM16 WAV/loop sidecars. Native environment sounds and
scene-specific effects remain unbound; no synthetic replacements were added.
Original `msg/US/{dmo_00,aaa_00,gor_10,gor_00}.txt` remain byte-for-byte message
resources with IDs/control tokens retained. See [opening flow contract](behavior/story/NATIVE_OPENING_FLOW.md).


## 2026-10-13 — Original professor first-talk resources

`tools/professor_event_assets.py` reads `rel/gor.rel` from the original US ISO
read-only, pins the REL/first-talk hashes and preserves the original180 commands.
Ignored `local-assets/professor/` records source section offsets, relocations and
callback bindings; ignored `romfs/professor-events.pmevt` and
`romfs/professor-program.bin` use authored native section addresses and actual
local/module0 ADDR32 relocations. Original executable bytes are data for the
existing portable event interpreter; GameCube machine code is not executed.

`tools/professor_scene_assets.py` imports seven actual Mario clips from
`a/a_mario` and `a/e_mario`, with `a/a_mario-` textures. Field M_C_3 uses the
original effect bank. Ignored `local-assets/professor-scene/` retains intermediates
and ignored `romfs/professor-mario.{pmmesh,pmanim,t3x,bin}` supplies the native
scene. The original normalized UV domain is retained; the cropped atlas is
resampled to512x512 RGBA8,62px cells with1px padding, to bound texture allocation
to1MiB. This pixel resampling is a handheld adaptation.

Item0x60 is the source treasure-map backside: icon314 resolves to `icon.tpl`
image319, imported into ignored `romfs/professor-map.{t3x,bin}`. The original
`sound/stream/evt_fkh1_32k.stm` supplies BGM_EVT_FRANKURI1, decoded with original
loop metadata into ignored `romfs/professor-music.wav` and sidecar. Message IDs
and control bytes come unchanged from the existing `msg/US/gor_02.txt` import.
The source manifest is ignored `romfs/professor-scene.json`. All originals remain
Nintendo/Intelligent Systems property; conversion grants no redistribution
license. No original assets or compiled packs are committed.


## 2026-10-13 — Original fence removal resources

`tools/professor_event_assets.py` now relocates the original first-talk,
kurihakase_wall_remove and kurihakase_lecture data:318 original commands with
pinned source hashes,63 scoped callbacks and three native entry addresses.
Ignored `romfs/professor-program.bin` uses PMPROG2; the original events, strings
and local/module0 relocation policy are preserved in local manifests.

`tools/professor_fence_assets.py` reads `a/OFF_d_saku` and `a/OFF_d_saku-` from
the read-only ISO. Ignored `romfs/professor-fence.{pmmesh,pmanim,json,bin}` retains
the actual A_1 nonloop deformation, seven keys and140 source animation frames,
plus source A_saku_04 collision subtree IDs from the imported gor_02 map. Raw
resources/intermediates remain ignored `local-assets/professor-fence/`. The
placeholder paper TPL is not used as a replacement for the fence appearance.

Native presentation retains the78 original S_saku04 map vertices and material
textures, then deforms that geometry using the actual paper quad in a frozen
camera plane. This handheld capture adapter avoids a second scene render; it
is not exact original GX offscreen capture, rectangle clipping, depth-plane or
shadow reproduction. Source capture-bound writes/aspect arithmetic and original
animation-completion wait remain in the event. Original fence open/fall SFX
requests are retained; complete original spatial/MusyX playback is unbound.
All originals retain Nintendo/Intelligent Systems rights; no redistribution
license or original assets/compiled packs enter history.

## 2026-10-13 — Original lecture message icons and Goombella talk

Read-only US ISO icon.bin/icon.tpl provide used STICK ID129 (TPL129,80x80,
static) and choice cursor ID504 (TPL533..542,40x40,source loop1; nine delays2,
last delay28). tools/message_icon_assets.py retains original pixels and packs
512x128 RGBA8 with2px transparent padding, no resampling (0.25MiB native texture).
Ignored local-assets/message-icons/ retains originals/PNGs/provenance; ignored
romfs/message-icons.bin/.t3x/.json retain descriptors/native atlas/manifest.
Source icon auxiliary fields are preserved; full original transform/depth remains
unbound. c_pkr/c_pkr- supplies original PKR_T_1 in ignored partner mesh/animation
packs through tools/build_assets.py, alongside ordinary partner poses. Expanded
PMPROG3 metadata retains original Frankly/Goombella identifier VAs for actual NPC
routing; original event words/relocations remain unchanged. Assets remain
Nintendo/Intelligent Systems resources with no redistribution grant; original
and compiled packs are excluded from commits.

## 2026-10-13 — Original first underground map and music

Read-only US ISO m/tik_01/d,t,c,s supply the first underground room. Ignored
local-assets/underground/ retains extracted originals/provenance, map products
and TPL PNGs. tools/underground_assets.py retains geometry/material UVs and
collision, scales map units through the existing converter, reduces textures
to max128 power-of-two RGBA8 via Lanczos and writes ignored romfs/tik_01.*
and tik_01-*.t3x. Prologue zero-flag init hides bom_kabe_2/bom_kabe_4; broader
wall hit/state owners remain pending. Original camera bytes are copied unchanged.
The original BGM_STG0_TIK1 mapping provides sound/stream/stg_tik2_32k.stm,
decoded PCM16 with source rate/channels/loop metadata as ignored
romfs/underground-music.wav/.loop. Source generic pipe SFX remain unbound.
Ignored field-pipes.bin/underground-assets.json retain reciprocal original REL
record/hit identities and conversion/source manifest. Original gor pipe event
extends PMPROG4 metadata; no original script instruction is rewritten apart
from supported relocation. Nintendo/Intelligent Systems rights remain;
original or compiled assets are not committed or distributed.

## 2026-10-13 — Original underground trio animations and event pack

Read-only rel/tik.rel provides gorotuki_sanninngumi161commands and
ashioto10commands. tools/underground_event_assets.py writes ignored
romfs/underground-events.pmevt/.program.bin and underground-trio.json metadata
using original words and supported relocations; no source code is compiled.
Actual original NPC keys remain pointer-resolved. a/c_kuribo and a/c_kuribo-
provide fifteen KUR/PTK/TGK S_1/T_1/W_1/R_1/D_1 poses in ignored
romfs/underground-goombas.pmmesh/.pmanim/.t3x/.bin/.json. Existing bounded model
converter retains original texture cells/pixels with atlas padding/cropping,
geometry and sparse keys. Idle Z_2 has non-endpoint key topology outside that
importer's domain and is unused by this scoped scene; no substituted idle pose
is claimed. Mario M_N_7 comes from actual e_mario/a_mario- (not a_mario), added
to the existing scaled scene atlas. Original US tik_01/tik_04 message packs
supply exact payloads and control tokens. Local originals/compiled packs stay
ignored and retain Nintendo/Intelligent Systems rights; no redistribution grant.

## 2026-10-13 — Original setup4 battle introduction and stage

Read-only rel/tik.rel group0x26460/unitarray0x263a0 supplies four source units;
kind and part pointers supply HP/FP, placement, flags/counter flags and five
zero defense/defenseattribute slots. tools/trio_battle_assets.py writes ignored
trio-battle-units.bin, original Frankly init15/entry7/phase39 commands under
trio-battle-events.pmevt/program.bin and provenance trio-battle.json. Supported
relocations preserve original words; no original machine code is compiled.
Actual m/bti_01/d,t and m/stg_00_1/d,t provide native stage maps, original
geometry/materials/UVs and textures reduced to max128 power-of-two RGBA8 via
Lanczos. Ignored local-assets/trio-battle/ retains originals/provenance/PNGs;
ignored romfs/bti_01*/stg_00_1* retain native products. Source selected A1/isi_1/isi_2/B_mati/C subtrees retain ancestor matrices and
additional translations; bti escape_wall/if_body/if_wire are hidden. Dynamic
intro scales/object lifecycle/depth still require original service binding.
Original message icon PAD_Y_ON=115/0x73 uses descriptor0x9de,TPL115,56x56,
static source pixels. Used icon atlas expands512x128→512x256 (0.5MiB) without
resampling. Original music uses existing BGM_ZAKO_BATTLE1/btl_zak1_32k stream.
No originals/compiled packs enter commits; Nintendo/Intelligent Systems rights
and lack of redistribution grant remain unchanged.

2026-10-13 setup4 command/target metadata: `tools/trio_battle_assets.py` now
extracts default weapon records0x803538a8/0x80353de8/0x80393ea8 from the read-only
US main.dol. Original admission(+64)/attribute(+68) words, name IDs and source
record hashes reside in ignored `romfs/trio-battle-commands.bin` and
`romfs/trio-battle.json`. PMBUNIT2 retains original tik.rel part local/hit/sample
coordinates, setup forward bias and unit name IDs. No synthetic target geometry
or rewritten labels. The native menu resolves original `msg/US/global.txt` labels.
These are locally extracted Nintendo/Intelligent Systems assets with no
redistribution grant; compiled packs remain outside history.

## 2026-10-13 — Native save System frame, selection panel and cue431

Read-only US ISO `f/msgWindow.tpl` (51 entries) now supplies separately packed
`romfs/message-window-panel.t3x` (index0,72x72),
`romfs/message-window-system-pattern.t3x` (index5,128x64) and
`romfs/message-window-42.t3x` through `-49.t3x` (32x32 source shape/ring masks).
`tools/message_window_assets.py` decodes original GX images to lossless RGBA8,
retains dimensions and records ISO offset/full source SHA/fingerprint in ignored
`romfs/message-window.json`. Pattern repeats; corner masks clamp; native PICA
stages adapt original System TEV algebra and four-quad geometry. Selection uses
source index0 nine-slice UV/destination caps. No generated panel art is used.
Converted PNG/TPL copies remain ignored under `local-assets/fonts/`; compiled
T3X/manifest remain ignored. Original Nintendo/Intelligent Systems rights remain;
local conversion does not grant redistribution rights. Native narrow opening
geometry/GX-PICA rounding parity remain adapters; see live save contract.

`config/audio_bindings.json` selects original SFX_MOBJ_BLOCK_SAVE1/cue431 for
`romfs/save-block.wav`: source pssfx row431 ->FX618/group5/macro745/sample550,
`local-assets/audio/pcm/samples/pmario-0226.wav`,22050Hz/24422 PCM16 frames/no loop.
`tools/audio_map.py` retains source row/parameters/macro/sample/conversion metadata
in ignored catalogs. No synthetic tone or replacement melody is generated. The
NDSP primary-sample fallback uses source volume100 with existing conservative
bus gain; MusyX modulation/3D attenuation/spatial pan/group residence are not
reproduced. WAVs/catalogs/extracted bank data remain outside source history.
ISO is never written or committed; original audio rights remain unchanged.

2026-10-13 original Jump runtime: `tools/trio_attack_assets.py` reads original
US DOL data sections7..12, retaining the pinned268-command NormalJump, Majinai,
Suitoru and empty attack-ready child streams at original addresses. SDK/mutable
tail and live event-manager/context/global address ranges are excluded to keep
native owners authoritative. Ignored `romfs/trio-attack-events.pmevt` is1,127,488
bytes; its JSON records exact source/segment/event hashes. No PowerPC text is
imported. Native source-address reads use the unchanged data/constant bytes.

Original `a/b_mario` with `a/a_mario-` texture owner supplies M_S_1, M_R_1,
M_W_1, M_J_1B, M_J_1D, M_D_2 and M_A_5. Ignored `trio-attack-mario.*` retain
original sparse streams/hierarchy with the existing512-wide resampled atlas.
M_A_5 has one source key at0 and duration2; no extra endpoint is generated.
The actual Mario battle atlas is bound for setup4 rendering. All original
assets/compiled packs stay outside history with Nintendo/Intelligent Systems
ownership and no redistribution grant.

2026-10-13 Jump hit/reaction dependencies: the local Jump event pack now also
imports relocated non-executable `rel/tik.rel` data sections at the existing
`0x93000000 + section*0x100000` banks. Original Goomba/Spike/Paragoomba kind
and part definitions, damage/wait wrappers and strings retain local relocations;
DOL imports retain original addresses. `romfs/trio-attack-units.bin` (PMTHIT1,
three rows) maps each source kind to its installed damage/wait scripts. No
PowerPC text is included. The converter reads the ISO only; extracted REL,
packs and source manifests stay ignored. Nintendo/Intelligent Systems ownership
and no redistribution grant remain unchanged. Native damage currently supports
fresh ordinary Goomba Jump only; packaging the other definitions does not imply
those callback paths are implemented.

## 2026-10-13 — Original NPC dimensions, facing constants and source surfaces

`tools/field_actor_assets.py` reads the read-only US DOL tribe rows80333554
(c_tyusan) and80333014(c_kuri_h), recording original30/30 dimension fields,
CHU_T_1/T_1 pose pointers and row hashes in ignored `romfs/field-actors.json`.
PMNPC005 adds source resource+c4 draw masks and per-actor dimensions/talk pose.
Source angleABf's91 float entries at803041e0 are endian-converted unchanged
into ignored `romfs/npc-angle.bin` (PMNANG01+364 bytes), SHA256
58c60c710ded98d199e5505805a56f956721d54cf4674bac65012bb431717432
for the original big-endian source table. No executable code is exported.

Original Chu now includes CHU_T_1 alongside CHU_S_1. Chu/Frankly PMANIM4
packs use `character_surface_mesh.py` to preserve original vertex RGBA, shape
passes/no-cull indices and sampler state. Original textures remain resized124px
with2px sampler-aware gutters in128px cells; dynamic UVs and source colors are
retained. All extracted/converted model/TPL/PNG files stay in ignored local-assets,
and packs/manifests/T3X stay in ignored RomFS. Nintendo/Intelligent Systems rights
remain; local conversion grants no redistribution rights. No generated NPC art
or rewritten dialogue is introduced. Numeric SDK/render and full NPC services
remain adapters as described by the native presentation contract.

## 2026-10-13 — Original NPC descriptor0 animation fallback

The local original NPC converters now retain the source animation bank's first
64-byte descriptor before other selected poses (directory offset at header+1ac).
Read-only `a/c_tyusan` supplies CHU_Z_1; `a/c_kuri_h` supplies Z_1.
Ignored per-model manifests record `original_index0`; compiled PMANIM4 retains
original sparse animation streams and existing RGBA/pass/cull/UV/sampler data.
The case-sensitive source tribe d_1 therefore falls back to Z_1 rather than
being rewritten to bank D_1. Canonical pose selection is documented in the
[native NPC contract](behavior/field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md).
Existing124px textures/gutters, hierarchy and scaling remain. Extracted banks,
textures, manifests and compiled packs stay ignored outside source history;
Nintendo/Intelligent Systems ownership and no redistribution rights remain.

2026-10-13 Jump death/reward metadata: ignored `trio-attack-units.bin` is now
PMTHIT2/52 bytes, retaining the three kind/damage/wait rows plus the relocated
original opponent group at `rel/tik.rel`0x26460. Reward setup follows original
FieldBattleInfo+c→NpcBattleInfo, NpcBattleInfo+18→group (count4), rather than
interpreting the stage record as the reward factor owner. Original DOL data
already include Mario wait script80417c38; the bridge installs that unchanged
script. No new art, animation keys or executable PowerPC data are generated.
The existing original Goomba D1 pose now uses recovered death rotations,
camera-dependent Z mirror and depth-5 over its original model scalar2. Packs/
extracted data remain ignored; original asset ownership and no redistribution
grant are unchanged. Particle and death sound presentation providers remain open.

## 2026-10-13 — Original harbor before-battle event and Mario poses

`tools/harbor_event_assets.py` extracts `rel/gor.rel` read-only and packages the
original before-battle event through source ADDR32 relocations in ignored
`romfs/harbor-events.pmevt` and `harbor-program.bin`. Raw data, provenance and
call/section manifests stay in ignored `local-assets/harbor-event/`. Native
section addresses are port policy; the original commands/branches/waits and
message/speaker pointers remain unchanged. Text comes from the existing original
`msg/US/gor_00.txt` resource. No extracted script or narrative text is committed.

Original `a/a_mario`, `a/e_mario`, `a/a_mario-` provide real M_S_1/M_W_1/M_J_1B/
M_J_1C/M_I_N/M_N_H clips. M_I_N and M_N_H are actual e_mario clips, not aliases.
The shared geometry/animation converter preserves model/hierarchy/key streams;
empty atlas rows are cropped, the runtime atlas is512px wide with62px cells and
1px borders, and V scaling compensates the crop. This bounds linear memory.
Existing original Goombella/Crump/X-Naut assets remain in use. Original game
rights remain Nintendo/Intelligent Systems; no redistribution license is granted.
Original environment/effect/SFX requests are retained but their native consumers
remain unbound; no generated replacement audio was added.


## 2026-10-13 — Original Goombella Headbonk battle assets

`tools/trio_attack_assets.py` reads the supplied US ISO read-only and imports
original `a/c_pkr` and `a/c_pkr-` into ignored
`romfs/trio-attack-goombella.pmanim`, `.t3x` and `.bin` (PMSUV1 UV scale).
The pack retains PKR_S_1/R_1/W_1/J_1A/A_2B/T_1 hierarchy and sparse animation
streams. Atlas width512, proportional height and texture resampling follow the
existing local battle conversion; original asset/model files and conversion
metrics stay under ignored `local-assets/trio-battle/goombella/`. Native rendering
batches the pack with citro3d and applies original battle pose clocks/rotation.
This is original Nintendo/Intelligent Systems copyrighted game data for local
port development; no redistribution license is granted or inferred.

The original read-only Goombella data span80418450..8041849b includes defense,
status, default attack and wait plus the scanner's original lookahead word.
The local event manifest inventories/pins Headbonk root803941a8, size0xce0,
SHA25677d2f5d30bea8bd4ff16b0d09ac2e942e705f4717e9fb233b1ccfb2166efc37c.
The native trajectory uses the existing original `npc-angle.bin` table rather
than a replacement atan implementation. Original ROM, extracted packs and
compiled assets remain outside source history.

## 2026-10-13 — PMNPC006 original NPC flag metadata

`tools/field_actor_assets.py` now emits ignored `romfs/field-actors.pmnpc`
with PMNPC006 actor rows200 bytes, retaining each original NPC Entry/setup/
constructor flag word after PMNPC005 dimensions/talk pose. Original read-only
`rel/gor.rel` rows0x452d4 (Chu3) and0x44ff4 (Frankly) supply setup0x80600;
DOL NPCEntry publishes3, Frankly init0x410c8 adds0x40000000. Local manifest
retains Chu0x80603/Frankly0x40080603 and source provenance. Objects retain0
NPCflags and their existing separate model/bounds metadata. Loader reads
versions1..6 atomically; missing legacy flags remain unbound0.

No raster, geometry, texture or animation conversion changed in this format
increment. Existing original c_tyusan/c_kuri_h art, source scales/dimensions
and original91-float angle table remain local and ignored.
[Native NPC side collision](behavior/field/NATIVE_NPC_SIDE_COLLISION.md) binds
these source masks independently of rendering, with explicit placement history
and scripted source flag ownership. Original numerical/runtime/backend and
hardware limits remain stated; static fixtures are not Dolphin measurements.
Nintendo/Intelligent Systems copyright remains; no distribution license is
granted or inferred. No original ISO, extracted asset or compiled pack is
committed.


## 2026-10-13 — Original enemy script/phase metadata

`tools/trio_attack_assets.py` now writes the ignored68-byte PMTHIT3 map:
original kind/damage/wait/attack pointers for all three trio foes, original
setup4 group and Frankly phase script. Original relocated tik data stays in
`romfs/trio-attack-events.pmevt`; no executable PPC section is imported or
executed. The native93105b2c callback maps tik section1+5b2c to its recovered
XY direction ABI, with original npc-angle.bin data. Provenance remains the
read-only local US ISO; original Nintendo/Intelligent Systems data has no
redistribution license. Compiled/original assets remain ignored.

## 2026-10-13 — Original Darkly and PMNPC007 talk-rule catalog

`tools/field_actor_assets.py` imports original read-only `a/c_f_mura` and
`a/c_f_mura-` for source Darklyクラガリさん from `m/gor_02/s`. Extracted
model/texture resources and provenance remain ignored under
`local-assets/field-actors/c_f_mura` and `c_f_mura.tpl`; converted/reference
images and `field-darkly-runtime.png` remain there too. Ignored
`romfs/field-darkly.pmanim`, `.pmmesh`, `.t3x` and `.json` retain original
descriptor0Z_1,idleS_1,talkT_1,hierarchy/sparse states,RGBA/pass/cull metadata.
Source hashes/offsets are in [Darkly source bindings](behavior/field/ROGUEPORT_DARKLY_STATIC_AND_TALK_BINDINGS.md).

Current manifest records13 texture cells,1024×256 runtime atlas,UV scale4,
124px decoded texture scaling with128px cells and original wrap-aware gutters.
tex3ds emitsRGBA8 with automatic compression. Source NPC draw scalar2 and
engine units*.01 are unchanged; no generated replacement art/music/text is
introduced. Width/height30/30,placement(-335,0,-335),direction270 and flags
0x40080603 come from the source tagged/tribe/Entry/setup/init owners.

Ignored `romfs/field-actors.pmnpc` is nowPMNPC007:200-byte fixed actor prefix,
then per-actor count and40-byte comparison/story/full-message-ID rules. Current
Chu retains original nine ordinary requests; Darkly retains onlyLESS56→
gor_02_034_00 and early0..55 coverage. Empty/unmatched lists do not invent
legacy fallback for unbound later175/name branches. Original US payloads stay
in existing ignored message packs byte-for-byte. See [native binding and limits](behavior/field/NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md).

Original Nintendo/Intelligent Systems copyright remains; no distribution
license is granted or inferred. ISO, extracted assets, compiled packs and
original dialogue remain outside source history. Static/native checks are not
Dolphin observations or New2DS hardware performance measurements.

## 2026-10-13 — Original wandering-Goomba resource preparation

The existing read-only `tools/field_actor_assets.py` model compiler prepared
`a/c_kuribo_n` and its header-selected `a/c_kuribo_n-` bank from the local US
ISO. Raw resources/provenance stay under ignored `local-assets/field-actors/`;
ignored `romfs/field-goomba.pmanim`, `.pmmesh`, `.t3x` and `.json` retain original
Z_1/S_1/T_1/W_1/R_1, hierarchy, sparse animation, material/cull/RGBA and source
hashes/offsets. The manifest records14 texture cells,1024x256RGBA8 atlas,UV4;
124px texture scaling and wrap-aware128px cells follow the existing converter.
No replacement artwork or tones are generated. This prepares the original bank
for native regular-event binding; it is not yet registered as a live field NPC.

[Original wandering driver](behavior/field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md)
retains source placement/tribe/dimensions/pose/sound metadata and quest guards.
[Portable core](behavior/field/NATIVE_NPC_WANDER_CORE.md) uses no exported DOL
text or executable body; native C++ implements recovered phase/clock/RNG/sound
requests. Shared clock/seed, event lifetimes and original audio providers remain
unbound. Nintendo/Intelligent Systems rights remain with no redistribution
license; ISO/raw assets/compiled packs/original dialogue stay outside history.

## 2026-10-13 — Trio enemy impacts and private defence poses

`tools/trio_attack_assets.py` additionally imports original `a/b_mario` poses
`M_B_1`, `M_D_1` and original `a/c_pkr` poses `PKR_D_1`, `PKR_G_1`, with their
original texture resources `a/a_mario-` and `a/c_pkr-`. They support the source
player damage/guard scripts. Existing mesh baking, atlas halving with Lanczos,
RGBA8 tex3ds packing and UV-scale metadata are retained. Original DOL/tik
scripts run unchanged through native callbacks; no new story text is authored.
The ISO is read-only. Extracted models, texture atlases, packed animations and
compiled binaries stay ignored. Nintendo/Intelligent Systems ownership and
absence of redistribution rights remain unchanged. Imported poses do not imply
complete original effects/audio, guard choreography or hardware parity.

## 2026-10-13 — Original ordinary Hammer animations

`tools/trio_attack_assets.py` adds original `M_H_1A`, `M_H_1B`, `M_H_1C` from
ISO `a/b_mario`, using original `a/a_mario-` textures. Unchanged wrapper/core
80416c40/8035d6d4 and weapon80353de8 were already in the data-only DOL pack.
Existing mesh baking, atlas halving with Lanczos, RGBA8 tex3ds conversion and
UV-scale metadata are retained; no replacement artwork or dialogue is authored.
The converter reads the ISO read-only, keeps extracted models/atlases/animations
and compiled packs outside source history, and fingerprints tool/source inputs.
Nintendo/Intelligent Systems ownership and no redistribution grant are unchanged.
Original charge cue keys are retained service requests; importing these poses
does not imply complete original gauge or MusyX voice/macro playback.

## 2026-10-13 — Live original Goomba4 and PMNPC008

The prepared original a/c_kuribo_n bank is now registered as live Goomba4 in
ignored romfs/field-actors.pmnpc PMNPC008. Original model source ISO offset
886803444,size18920,SHA256
2f62e39215aa2c355ea0c9edb4a3e40e2964d602186b6fd4016eac326fbbadb3;
texture a/c_kuribo_n- offset886822364,size24416,SHA256
2a9eed35878cb232d7ee8770e3cc76cbbfddba00d48cb7c95612b65ca9225cd1.
Raw model/header-selected texture bank and provenance stay ignored under
local-assets/field-actors/c_kuribo_n and c_kuribo_n.tpl. Ignored
romfs/field-goomba.pmanim/.pmmesh/.t3x/.json retain original Z_1/S_1/T_1/W_1/R_1,
hierarchy/sparse states,RGBA/pass/cull/sampler metadata. Manifest records14
texture cells,1024x256 runtime atlas,UV4;124px decoded textures and128px
wrap-aware gutter cells remain converter choices. Uncompressed RGBA8 payload
is1MiB(1024*256*4); this is not a hardware residency measurement. tex3ds usesRGBA8/auto
compression; source drawscalar2 and source engine units*.01 remain unchanged.

PMNPC008 retains100-byte model/200-byte actor prefix plus PMNPC007 rules and
adds188-byte original event-presence/wander/pose/sound metadata. Actor ordering
follows original tagged registry; Goomba4 retains source init(-370,0,130),
direction270,radius70/speed30,tribe dimensions30/30 and original movement sound
names. [Live binding](behavior/field/NATIVE_LIVE_NPC_WANDER.md) uses the existing
original-art citro3d renderer with simulation-owned live paper presentation.
Queued original SFX_FLD_NPC_KURI_MOVE1L/R requests now play original primary
samples993/994 through config/audio_bindings.json,tools/audio_map.py and ignored
romfs/audio-map.json. Original sound/pmario MusyX DSP ADPCM samples decode
to monoPCM16 at16000Hz: sample993 is540 samples at bank offset10014976;
sample994 is672 samples at10015296. Ignored romfs/goomba-step-left.wav and
goomba-step-right.wav are bounded one-shots with source rate/ID retained.
Native clips6/7 use libctru NDSP channels8/9 and source volume45/127 times the
existing master gain0.65. Source name/mask/root remain in the request queue;
macro modulation/child voices, material-mask effects and spatial attenuation
remain native adapters, not original audio parity. Quest playback
stays unbound; no fabricated state/dialogue or replacement art/tones added.

Original Nintendo/Intelligent Systems rights remain; no redistribution license
is granted or inferred. ISO/raw assets/compiled packs/original dialogue remain
outside source history. Source/native code/manifest evidence is not matched
Dolphin execution or a New2DSXL hardware performance measurement.

## 2026-10-13 — Original grounded Goomba pose and bank fallback

`tools/underground_event_assets.py` retains original descriptor0 `KUR_Z_2`
first and additionally imports grounded run `KUR_R_2` from read-only ISO
`a/c_kuribo`, using original `a/c_kuribo-` textures. Original flying/grounded
parts share this same resource bank. Authored `PTK_R_2` is absent from the bank;
the shared battle animation selector falls back to actual descriptor0 rather
than inventing or substituting a different run animation. `KUR_Z_2` has visible
static source geometry; existing original S/T/W/R/D poses remain imported.
The mesh/atlas/animation conversion pipeline and texture scaling are unchanged.
Original fall and grounded init/attack/wait/part/pose/data definitions were
already in the data-only tik pack; they retain original relocation addresses.
Extracted originals, compiled packs and binaries stay ignored. Nintendo/
Intelligent Systems ownership and no redistribution grant remain unchanged.

## Integrated native opening scene products

All resources below originate from read-only G8ME01.iso and remain ignored/local.
Original message catalogs retain msg/US IDs/payload bytes/control tokens. Native
house packs preserve m/aaa_00/{d,t}, original S_ie_door/S_posuto hierarchy/world
matrices and geometry bindings (PMHOBJ01). Original a/c_luigi/c_paretta poses
supply movement/carry/talking; native geometry/clock/trig adapters are documented.

Original a/c_peach/c_nazo and a/MOBJ_GoldenTreasureBox plus their trailing-minus
texture banks supply demo actors. Chest PMANIM4 preserves original wrapped UV
surfaces, untextured color and source opaque pass1. demo-chest.uvscale records
cropped atlas scaling; static selected samples are cached without changing art.
opening-demo-market.* preserves original m/gor_01 S hierarchy, excluding five
source interiors and tagging both source wall subtrees for demo-owned visibility.
Original texture dimensions are capped128px in map conversion.

Original a/OFF_d_meku_right and texture bank supply demo-page.pmanim A_1 and
PMDPG001 triangle capture-kind/pass/cull companion. demo-page-back.t3x uses
original rune TPL0, half-size240x304 with256x512 transparent padding. Original
old demo8..11 raster crops provide capture fronts; the native outer image-plane
projection is explicitly an adapter. No invented fold or generated texture.

Harbor setup2 uses original m/bti_01 and m/stg_00_0 selection/transforms and
sound/stream/btl_cbs1_32k.stm (source BGM126, original32k stereo PCM/loop).
Harbor tutorial actor packs retain original b_mario/c_kanbu/c_pkr clips/atlases;
Crump attack clips are added from that original bank. Ignored tutorial event
packs retain source scripts/selected DOL data/rodata, excluding executable PPC
text and live VM context storage. Packed source pad/state/services are native
code adaptations, not redistributed originals.

Original Nintendo/Intelligent Systems ownership/no-redistribution terms remain.
Source files, converted images/audio/geometry/event packs and compiled local
3DSX artifacts are never staged. No generated tones or replacement story art.

### Native prologue pouch name / original drop and EXP data (2026-10-13)

Prologue pouch initialization reads the unchanged packaged msg/US/global.txt
key name_party3 for the localized initial partner name; no replacement name or
story text is authored. The full message bank is indexed temporarily and only
that original name remains resident. Initial resource/item scalar mutations
and original drop/stockEXP semantics are native recovered logic. Heart/flower
profiles803702a0/80370f20 and Item Hog table802eea40 remain in the existing
original data-only trio pack, extracted read-only from G8ME01.iso. No new
original art or redistributed resources are committed. Nintendo/Intelligent
Systems ownership/no redistribution rights remain unchanged. EXP404/PointGet
art is documented next work, not claimed to be imported/rendered by this chunk.

## 2026-10-13 — Read-only trio EXP asset evidence

Original `icon.bin` ID404 uses `icon.tpl` frames418..421 (40x40,loop,delay6
perframe). Original `e/us/effect_us.tpl` indices6..13 are point-get "You got",
digits0..9 andsingular/plural Star Point(s) foreground/shadow art. Inspected
andconverted onlytoignored `/tmp/trio-exp-*`; this documentation does not
claim theyarealreadyimportedintothe nativepack. Proposednativeatlas preserves
originalpixels/frameIDs/delays/alpha with explicit3DS viewport adapter; exact
GXquad/window/iconclockparity remainsopen. Sourcevictorymodelbanks `a/b_mario`
M_V_1/M_V_2 and`a/c_pkr` PKR_Y_1/PKR_S_1 arelocallyavailable; originalsource
category69 handlesGoombella type43. Allare Nintendo/Intelligent Systems local
original-game resources; noreplacementlicenseorredistributionrights implied.
See [sourceEXP contract](behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md)
for exact dimensions, hashes, source coordinates and remainingrender dependencies.

## Original battle startup and harbor curtain resources

Read-only G8ME01.iso supplies a/p_b_st, a/p_jyabara (actual TPL owner a/p_slit-),
a/p_kaiten_h and original a/EFF_nice_us, EFF_good_us, EFF_great_us,
EFF_wonderful_us, EFF_excellent_us, EFF_lucky_us with their trailing-minus banks.
Ignored battle-startup-resources.bin keeps full original banks/25 descriptors;
23 supported original poses retain PMANIM4 wrapped UV/pass/cull/RGBA. Two dormant
accordion poses retain opaque original descriptors; no playback/capture is faked.
Original US language1/context1 keys and NULL grade-row slot remain source-owned.
Converted GPU atlases are capped512wide with original source gutter/wrap rules.
Explicit captured-texture export admission retains sourcekind1 in descriptors;
its fallback TPL does not substitute for an actual field capture.

Original a/OFF_d_maku_left_right and a/OFF_d_maku_left_right- supply ignored
harbor-curtain.pmanim/t3x/bin/json for source fade23/context2/A_2. Original sparse
geometry is216 triangles,14 keys,span0..45. One source atlas becomes512x64 with
UVscale8. Sourcepass0 normalization and native image-plane projection are recorded
in display/NATIVE_HARBOR_CURTAIN_FADE.md. Completion uses the admitted/evaluated
original pose, not the stored duration200. No generated image/audio/geometry.

All original banks, images, converted packs and compiled binaries remain local
and ignored. Nintendo/Intelligent Systems ownership applies; no redistribution
rights are claimed. Exporters honor configured ISO paths and open originals only
read-only. Source native adapters and source hashes may enter history; originals
and executable PPC bytes never do.

## 2026-10-13 — Guarded original Goomba4 ordinary message rules

The local PMNPC008 catalog now retains the11 source-order kuribo4_talk ordinary
story rules and gor_02_030_ prefix, directly matching the read-only US gor REL
event and existing msg/US/gor_02 bank. Original IDs/control tokens/attribution
remain unchanged. The native partial saved-switch guard excludes both earlier
quest arms before these rules can play; unknown legacy state does not imply0.
No additional texture, geometry, audio or text conversion was introduced. Existing
c_kuribo_n and message resource conversion/provenance/licensing applies; original
content remains Nintendo/Intelligent Systems copyrighted material, used locally
from the user's ISO without redistribution. Compiled catalog/messages/captures
and ISO remain outside source history. See
[known-state native binding](behavior/field/NATIVE_GOOMBA4_ORDINARY_TALK.md).

### 2026-10-13 EXP type0 window frame source evidence

Read-only ISO `f/msgWindow.tpl`,321984bytes; ignored local evidence
`artifacts/recovered-temp-evidence/msgWindow.tpl`. Original TPL descriptor0
is72x72CMPR (format14), source type0 nine-slice texture. See
[geometry/material and clock](behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md).
No generated substitute art or distribution of original assets; same user-owned
ISO/local-use licensing limitation applies. Native import/render remains next.

## 2026-10-13 — Native original EXP stars and victory poses

The read-only US ISO supplies `icon.bin` ID404 / `icon.tpl` frames418..421,
40x40 pixels, loop1 and six simulation updates per frame. The existing original
message-icon atlas now imports all four frames unchanged. Original `a/b_mario`
M_V_1/M_V_2 and `a/c_pkr` PKR_Y_1 are included in the trio animation packs.
Original `f/msgWindow.tpl` descriptor0 supplies the nine-slice EXP frame.
The declared400x240 viewport adapter halves source coordinates/dimensions,
preserves bottom anchoring, uses source icon scales.9/.45 and frame alpha128;
texture tint preserves RGB. Existing locally converted original victory PCM
is selected while this positive-EXP prefix owns the battle. Complete WIN2,
voice/crowd/camera and original raster routing remain open.

Extracted assets, local caches, RomFS packs and emulator captures remain ignored.
Nintendo/Intelligent Systems ownership remains unchanged; no redistribution
rights are granted. PointGet US texture strips remain next implementation work.

## Original demo chest crystal/radiation resources

Read-only tools/demo_effect_assets.py exports original DOL N64 quad/triangle
arrays, deterministic radiation rows, float/color constants to ignored
romfs/demo-effects.bin (PMDEFX01,700 bytes). Original e/us/effect_n64.tpl
textures109/110 become romfs/demo-crystal-109.t3x and demo-crystal-110.t3x;
64×32 I8 and64×64 I4 dimensions are unchanged. Shared GX intensity decoding
now retains original alpha by replicating intensity into all RGBA channels.
Exporter fingerprints include this decoder and regenerate affected packs.
Local source copies/PNGs/provenance remain local-assets/demo-effects; ignored
romfs/demo-effects.json records source addresses/hashes and conversion scope.
Only original-used subtypes2/6/3 are consumed, without invented geometry or RNG.
See [effect contract](behavior/effect/NATIVE_DEMO_CHEST_EFFECTS.md) for source
ownership and native camera/GX numeric adapters. Rights remain Nintendo/
Intelligent Systems; local use only, no redistribution rights. Original data,
compiled packs and source disassembly are never committed.

## Original opening foreground tree bindings

The aaa_00 exterior pack now retains original sai_ki2/sai_ki1 subtrees formerly
omitted by the opening exporter. Original187-command evt_prologue drives finite
4000ms opposite-X camera parallax; no ambient animation or generated art is
invented. The original map root/local transforms (including tree scale1.2),
geometry, texture indices/materials and atlas conversion remain intact.
Ignored romfs/opening-aaa_00.houseobj now uses PMHOBJ02 with four roots,6912
bindings/193776 bytes; exterior mesh31656 vertices includes6030 restored tree
vertices. Existing textures need no additional source resource. Converter
fingerprints force regeneration and native pack validation rejects obsolete
bindings. Rights remain Nintendo/Intelligent Systems, local use only with no
redistribution rights. Original payloads/compiled packs stay outside history.

## Original book composite capture and audio transition

Original demo.tpl cover1/2 remains288x352. A bounded read-only exporter keeps
page-base indices3..7 at240x304 in256x512 textures; original illustration8..12
and desktop0 retain their existing conversion. Native captured page fronts are
actual top-screen RGBA8 framebuffer copies, cropped193x240 and tiled256x256;
they contain no newly generated art. Original OFF_d_meku_right A_1 remains the
page deformation and the original rune texture remains its back.

Original BGM_DEMO1/DEMO2 PCM packs retain original decoding/loops. Two fixed
native streams consume source6000ms fade and750ms retiring-tail clocks; no
new music is generated. Native scaling/projection/readback/audio scheduling
remain documented adapters. All extracted/compiled resources and provenance
remain ignored local products, for local use without redistribution rights.
