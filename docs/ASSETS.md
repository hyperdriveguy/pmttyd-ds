# Original asset pipeline

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
now fingerprints 92 selected US functions and includes two seven-byte defense
window tables. Independent verifier checks 212 instruction anchors plus those
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

## Complete static head collision mesh

Offline conversion emits .head for map/gor_00/gor_02/gor_03/gor_04 from original
m/<room>/d. Format HEAD magic+uint32count; each48-byte little-endian record is
9float32 vertices, uint32attributes, uint32sourceNode(relative to map base32),
uint32drawmodeByte1. Source hitCalcVtxPosition strip winding is preserved
separately from render triangles; every collision strip triple is retained,
including steep/vertical surfaces and degenerates. Conversion does not filter
by floor slope. Same original-ISO provenance, no replacement artwork/code.
Packs stay ignored in RomFS/local-assets; no ISO/runtime original extraction.
verify_head_mesh.py validates every record owner/attribute/cull against original
read-only node bytes; it does not independently validate converted positions.
Room triangle counts3153/1604/3450/3504/548 (12,259 total). Largest geometry
vector168,192 bytes plus spatial-index overhead; only current room is active
except temporary staging/rollback. Hardware memory/performance unverified.

## Floor slope selection moved to runtime

Floor converter keeps all nondegenerate triangles with nonzero XZ projection
(|normalY|/length>1e-6), replacing.65 cutoff. Native nearest-hit query then
rejects ordinary slopes>=50degrees. This preserves valid near-limit slopes
and steep nearest blockers. Pack remains tagged44-byte attributes/source-node
format; source cull/winding metadata is not added yet. New floor counts
1337/792/1285/1332/200 (4,946 total), verified original attributes/source IDs.
Ground vector payload217,624 bytes across all packs, plus runtime struct/index
overhead; room-specific loading still applies. No art/music conversion change.
Original-derived packs remain ignored; hardware performance unverified.

## Source-facing floor pack (2026-10-12)

map/gor_00/gor_02/gor_03/gor_04.ground now use header bits31+30 followed by
48-byte little-endian records:9float32 vertices, uint32attributes, uint32source
node relative to map base32, uint32drawmode byte1. Geometry comes from the
same source hit_topology strip order as the complete head mesh, not GX render
triangles. Preserve all nondegenerate projected surfaces; source slope/cull
selection happens at runtime. Legacy untagged36-byte and bit31-tagged44-byte
packs remain readable as two-sided, with their prior winding unchanged.

Source is locally converted m/<room>/d from read-only US G8ME01 ISO; no external
code/art/license dependency introduced. ISO/originals/packs remain ignored and
are not distributed or required at runtime. Node attributes and drawmode are
checked against original raw node bytes by verify_ground_attributes.py; source
strip order follows hitCalcVtxPosition evidence. This verifier does not prove
independent original-coordinate fidelity. Counts remain4,946 records,237,408
bytes total record payload;1,867 are one-sided downward backfaces now rejected
by floor queries. Only current room is active except transactional staging.
Hardware memory/performance unverified.
