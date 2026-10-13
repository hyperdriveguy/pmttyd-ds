# Native Darkly and per-actor original talk-rule binding

2026-10-13; bounded early-Rogueport native adapter to
[Darkly source bindings](ROGUEPORT_DARKLY_STATIC_AND_TALK_BINDINGS.md),
[actor catalog](NATIVE_AREA_ACTOR_CATALOG.md),
[NPC presentation](NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md),
[talk arbitration](NATIVE_NPC_TALK_SELECTION.md) and
[side collision](NATIVE_NPC_SIDE_COLLISION.md). Static source deductions and
native implementation checks are separate from original runtime observations.
No Dolphin pixel/mechanics or New2DS hardware performance equivalence is claimed.

## Original actor admission and reusable model resources

Native gor_02 now includes Darkly after Chu in their original relative registry
order. The source row retains original position(-335,0,-335),direction270,
tribe width/height30/30 and initialized flags0x40080603. Early story0..55 is a
native coverage envelope; it is not a recovered source visibility condition.
Missing story remains unknown, not inferred. No native story write is added.

Ignored `field-darkly` model resources contain original c_f_mura descriptor0Z_1,
idleS_1 and talkT_1. Z_1 remains first for exact case-sensitive source fallback.
Drawmask1, source model scalar2, original sparse hierarchy/node/texture updates,
shape culling/RGBA/material passes and source dimensions remain separate owners.
The actor uses existing source matrix/camera-facing, pose/message clock, root+
height anchor, talk-ray and side-response services. It does not get a new
character-specific physics/rendering branch or copied dialogue.

## PMNPC007 variable rule tail

PMNPC007 retains PMNPC006's200-byte fixed actor prefix and100-byte model rows.
After **each** actor prefix, append a little-endian uint32 rule count, then
that many40-byte rules: uint32 comparison,int32 story,32-byte null-terminated
full original US message identifier. Supported comparison values are0 less,
1 equal,2 greater/equal,3 unconditional. Store/evaluate in source order; first
match returns its exact key. Do not sort thresholds, deduplicate cases or infer
an ID from row position. Full IDs allow suffixes/thresholds specific to each
original actor rather than forcing one shared family.

A PMNPC007 row marks talkRulesBound even with an empty list. No matching rule
returns empty; it does not use legacy prefix fallback. Object rows require
zero rule count. Native talk admission requires a supported rule for current
story, so empty/unmatched bindings do not suppress Mario's jump. This is a
native coverage gate; original eligibility still reads the source event pointer.
Room prefixes are validated generically for PMNPC007, allowing future area
message resources to use their own six-character names. Reader rejects
counts>64,unsupported comparisons,unsafe/
unterminated IDs, IDs whose source-room prefix differs from the actor's,
truncation/trailing bytes and prior malformed catalog conditions; failure
preserves the previous complete catalog. Versions1..6 remain readable and
retain the existing explicitly legacy prefix selector compatibility.
The variable rule tail is a native serialization format, not an original REL
record layout or complete original SWITCH/event interpreter.

Current Chu rules retain the complete existing nine ordinary source requests:
LESS56/112/164/215/261/339/373/401→gor_02_031_00..07, then unconditional08.
Current visibility still restricts him to the existing early envelope.
Darkly binds **only** LESS56→gor_02_034_00. Frankly and map objects have empty
ordinary rules; their scene/object event providers remain separate.

## Unsupported branches preserve missing behavior

Original Darkly talk first tests exact story175 with flag0xf84060bd and can
run the name-event child. Later ordinary thresholds differ from Chu, include
a duplicateLESS215 and terminalGREATER_EQUAL401. PMNPC007 supports useful
comparison forms for future bounded one-print actors, but this does **not**
implement Darkly's175 flag/name branch or all later talk behavior. His current
one-rule list intentionally returns empty outside story<56. Future addition
must bind the original state/event graph, without fabricated fallback dialogue.

The early original key gor_02_034_00 is opened from unchanged US room/global
message resources, preserving its111-byte payload and wait250×3/key controls,
actor attribution and source event turn/return relationship. No paraphrase,
branch repair, authored scene or progression write replaces original content.
Its source event has no selection or state side effect in this admitted arm.

## Actual current conversion choices and performance ownership

`tools/field_actor_assets.py` reads source model/texture/header/tribe/tagged
setup from the read-only local ISO. Current `romfs/field-darkly.json` records
13 texture cells,atlas height256,UV scale4 and the three original clips.
The surface converter retains geometry/sparse states/RGBA/pass/cull; decoded
textures resize to124px with clamp/repeat/mirror gutters in128px cells. Runtime
atlas is1024×256,encodedRGBA8 through tex3ds with automatic compression.
UV scaling compensates the cropped atlas; original geometry is not redrawn.
Native source model scalar2 and engine-unit*.01 mapping remain unchanged.

Original extracted c_f_mura and c_f_mura.tpl plus decoded/reference/runtime
atlases stay under ignored `local-assets/field-actors/`. Compiled
`romfs/field-darkly.pmanim`, `.pmmesh`, `.t3x`, `.json`, catalog and angle
packs stay ignored. Source provenance/hash/fingerprints allow cache invalidation.
Destination-area resources stage with geometry; shared model/sample caches,
source surfaces and citro3d batching retain the existing catalog performance
boundaries. Rule evaluation is bounded by imported per-actor count, with no
new live asset decode or talk-ray query in rendering.

## Validation and outstanding scope

Documentation was checked against current exporter/loader/runtime and generated
local manifest. Source resource paths/hashes/poses/initialization are documented
in the linked source contract. Native tests cover rule ordering/unmatched/
empty handling, old format compatibility, malformed/truncated rules and exact
Darkly key/story behavior. Exact implementation build/tests and isolated native
capture results belong in [progress](../../PROGRESS.md); no independent pass
claim is made here. Original assets remain Nintendo/Intelligent Systems data,
with no redistribution license inferred.

Remaining: Darkly's later original name/flag graph, all-story actor lifetime,
complete original event/registry/auto-turn/timer ownership, geometry/numeric/
material/shadow and runtime comparison, additional original areas and hardware
profiling. The per-actor rule format is extensible without implying the original
full-game event graph is already implemented.
