# Field mesh, floor and road asset provenance

Static head collision mesh, floor slope runtime selection, the source-facing floor pack, the collision mesh reused for directional wall movement and the original ROAD metadata conversion. Split out of [ASSETS.md](../ASSETS.md) on 2026-10-12; sections are verbatim.

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

## Complete collision mesh reused for directional wall movement (2026-10-12)

Player wall rays now reuse the complete source collision-strip geometry already
converted into ignored romfs/map.head and romfs/gor_00.head, gor_02.head,
gor_03.head, gor_04.head. These are local derivatives of the read-only US
G8ME01 ISO room maps documented above; no new extraction, conversion, asset
publication or original data commit was made. Original node attributes, source
owner IDs, source collision winding and cull bytes feed a native finite 3D ray
adapter. Unlike legacy near-vertical .walls rows, this mesh retains oblique wall
surfaces. The native policy operates in original engine units and converts to/
from geometry scale.01; triangle math/traversal remain an adaptation requiring
original comparisons. Verified current mesh counts3153/1604/3450/3504/548;
none contains high-bit NPC-owner records. Dynamic named NPC filters require their
own provider; admission of such unsupported records asserts instead of inventing
owner names. Existing original asset ownership/licensing and private-use/no-
redistribution restrictions still apply. All binary packs/captures stay ignored.

Ordinary partner movement research by ID1 (2026-10-12):verify_field_party_follow.py
and field_party_follow_evidence.json pin seven whole US DOL bodies,66 selected
instructions,ten numeric constants,two kind-record data hashes and two lecture
imports. Authored documents specify callbacks/history/read helpers and hit
attribute query,not final terrain/movement engines. Reports and corrected Gekko
ELF/listings remain ignored under artifacts/reference/behavior. No asset
conversion,original execution,ISO/profile/save mutation,original binary/dialogue
distribution or rewritten story.

Partner walk executor research by ID1 (2026-10-12):verify_field_partner_executor.py
with field_partner_executor_evidence.json pins two complete US local bodies,66
instructions,nine numeric constants,five data-span hashes and two actual lecture
imports. Eight substate dispatch words are explicit address metadata. Map keys
aji_02/aji_05/aji_12 and pose keyPCH_A2_1 identify original resource spans;
no story payload is rewritten or copied. Corrected Gekko ELF/listings and
field-partner-executor-evidence.json remain ignored under artifacts/reference/
behavior. No asset conversion,original execution,ISO/profile/save mutation or
original binary/dialogue distribution occurred.

Partner direction/speed research by ID1 (2026-10-12):verify_field_partner_direction.py
with field_partner_direction_evidence.json pins three whole US DOL bodies,64
instructions,19 exact scalar constants and two actual lecture imports. The
correction helper is partyMarioHosei,not a target-count getter;artifacts use the
original annotation. Corrected Gekko ELF/listings and field-partner-direction-
evidence.json remain ignored under artifacts/reference/behavior. No original
execution,numeric seed measurement,asset conversion,ISO/profile/save mutation,
original binary/dialogue distribution or rewritten story occurred.

Partner target/footmark research by ID1 (2026-10-12):verify_field_partner_targets.py
with field_partner_target_evidence.json pins six whole US DOL bodies,55
instructions,three constants and two lecture imports. Live-target wrappers,
clear/ID and SDK paired distance are local contracts;source/domain and original
execution remain separate. Original ELF/listings and field-partner-target-
evidence.json remain ignored under artifacts/reference/behavior. Dolphin paired
interpreter source was read as a public operation-semantic cross-check (link in
SDK distance doc);GPL-2.0-or-later implementation was not copied into native code.
No asset conversion,original execution,ISO/profile/save mutation or original
binary/dialogue distribution occurred.

Partner front-wall research by ID1 (2026-10-12):verify_field_partner_front_wall.py
with field_partner_front_wall_evidence.json pins eight containing DOL hashes;
seven local bodies complete,the backend only selected layout/filter regions.
It checks62 instructions,16 constants,four data-span hashes and two lecture
imports. Original normal/point ABI was corrected from concrete backend stores;
hidden-block keys remain exact identifiers and hashed spans. Original ELF/listings
and field-partner-front-wall-evidence.json stay ignored under artifacts/reference/
behavior. No asset conversion,original execution,ISO/profile/save mutation or
original binary/dialogue distribution occurred.


Partner surrounding-wall research by ID1 (2026-10-12):
verify_field_partner_around.py and field_partner_around_evidence.json pin seven
complete local US DOL bodies,83 instructions,13 constants,five data-span hashes
and two lecture imports. Bodies are the actual partner wrapper,shared surrounding
core,global view leaf and four SDK vector leaves. Original listings/ELFs and
field-partner-around-evidence.json remain ignored under artifacts/reference/
behavior. Shared dot admission and pre-cap saved-vector claims corrected from
original bytes. The supplied ISO stayed read-only;no asset conversion,original
execution,profile/save mutation or original binary/dialogue distribution occurred.
Metadata contains identities/selected anchors,not complete original bodies.


Partner stay postpass research by ID1 (2026-10-12):
verify_field_partner_postpass.py with field_partner_postpass_evidence.json checks
six containing DOL identities (four complete local bodies,two query bodies only
selected entry/output regions),78 instruction anchors,12 constants,three data
spans and two lecture imports. Resource evidence pins las_10 and the exact26-word
named-support pointer table plus25-name containing span;these are resource keys,
not story dialogue. Original ELF/listings and field-partner-postpass-evidence.json
remain ignored under artifacts/reference/behavior. ISO read-only;no asset
conversion,original execution,profile/save mutation or original binary/dialogue
distribution occurred. Full ceiling/floor geometry and runtime domain remain open.


Partner ceiling/scalar ray research by ID1 (2026-10-12):
verify_field_partner_head.py and field_partner_head_evidence.json pin four DOL
identities (head query,scalar wrapper and sincosf complete;shared backend selected
callback/result-layout regions only),89 instruction anchors,six constants,two
zero-template spans and two lecture imports. Actual ground/fly call sites are
pinned. Inspection ELF/listings and field-partner-head-evidence.json stay ignored
under artifacts/reference/behavior. ISO remained read-only;no asset conversion,
original execution,profile/save mutation or original binary/dialogue distribution.
Full backend geometry/domain/runtime behavior remains separate from identities.

## Original ROAD metadata conversion (2026-10-12)

The existing read-only G8ME01 ISO map/REL conversion now resolves the eight
reciprocal gor_00..gor_04 ROAD names against original collision mesh owners,
using their transformed local-bounding-box centers (hitObjGetPos), source-node
identities and AUTO cardinal directions. Vertex-average marker positions are
retired for this table. local-assets/room-links.json and the asset manifest retain
source/arrival node IDs, directions, REL offsets and center/spawn coordinates.
romfs/rooms.bin is a versioned ROAD1 pack:12-byte header plus eight48-byte
records (396bytes). Target placement performs its actual source-height downward
query at runtime; manifest spawnY remains marker provenance, not an assumed floor.
Original triangle winding/cull/draw attributes remain packed unchanged. The
ROAD registration OR00800000 is a runtime overlay, included in transactional
collision rollback. No disc bytes, converted assets or compiled packs were
staged; local-assets/ and romfs/ remain ignored. Original Nintendo/Intelligent
Systems assets retain their copyright; local conversion conveys no right to
redistribute the original or derivative pack. No music/art substitutions were
introduced by this road change.


Partner fall/facing research by ID1 (2026-10-12):
verify_field_partner_fall.py with field_partner_fall_evidence.json pins six
complete local DOL hashes,101 instructions,21 constants,one conversion-bias
span,two lecture imports and five actual walkMain dispatches. Original ELF/
listings and field-partner-fall-evidence.json remain ignored under artifacts/
reference/behavior. make all invoked existing3218-identity/five-fallback PCM
packing using the same read-only ISO sound/proj and local-assets/audio-map/
RomFS paths recorded above;asset/config/audio conversion choices were not edited
by ID1. Local raw/catalog/WAV packs stayed unstaged;original assets retain
copyright and no redistribution grant. No original execution,ISO/profile/save
mutation or original binary/dialogue distribution occurred. Initial native link
failures were unrelated to the verified original instruction evidence.
