# Bounded original gor setup0 field data

US G8ME01 revision0, static original data/relocation evidence. Complements
[the recovered graph](GOR02_SETUP_ZERO_BATTLE_INDEX_AND_ORIGINAL_TRIBE_DESCRIPTORS.md),
[numeric native setup](NATIVE_NUMERIC_NPC_BATTLE_SETUP_AND_AREA_REGISTRY.md) and
[actual map registration](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md).
This is a field-side read domain, not a complete battle/resource export.

`tools/field_battle_setup_assets.py::gor_setup_zero_segments` checks the exact
original gor REL SHA256, symbol ownership and source graph before returning
aligned original data views. `door_data_assets.py` merges those views into its
existing shared90-address pack with byte-agreement overlap checks. No original
PPC executes. The original ISO is opened read-only; extracts/packs stay ignored.

| Original source | Shared address | Bytes / purpose |
|---|---|---|
| REL60e30 | 90540da8 | 44: table row0 only |
| REL60da0 | 90540d18 | 18: weighted choice and zero terminator |
| REL60d80 | 90540cf8 | 20: selected group |
| REL60d50 | 90540cc8 | 30: selected unit |
| REL61c40 | 90541bb8 | 4: kind+10 word containing kind byte+12 |
| REL1dbc8 | 9040a0c8 | 10: original setup name with alignment padding |
| REL1dbd8 | 9040a0d8 | 14: original music name with alignment padding |
| DOL80417880 | 80417880 | 8: original all-zero item table |

Sizes in the table are hexadecimal. Total payload220 decimal bytes. Metadata
records original/relocated span hashes, exact local/DOL ADDR32 bindings, source
symbol identities and the admitted numeric domain `[0]`. The kind record's base
pointer90541ba8 is retained; only its actually dereferenced field is exported.
The selected stage90540870 and group auxiliary pointers are retained identities;
their later consumers need independent data/resource admission. The next setup
row and the stage body are deliberately unavailable in this finite view.

The generic `rel_data_pack.bounded_segments` primitive exports explicit aligned
views with a unique nonexecuting section owner. It relocates whole ADDR32 sites
without following targets, rejects partial relocation coverage and unbound
foreign modules, and returns provenance per view. It supports later areas that
need finite field views without pretending they have a recursive data closure.
The existing symbol-closure `package` path remains unchanged. Bounds/schema and
consumer admission belong to each subsystem; pointer presence alone proves no
stage, music, unit initializer or native world readiness.

Host original-data tests load the shared pack and run the actual inner helper
for all fifteen source npcEnt rows, each of whose index+58 is0. An explicitly
shared test RNG consumes16 irand(1)+1 irand(50) per call,255 total. Tests verify
original stage/group/music, audience bytes, flags and no item draw. Bounded-view
tests reject executable/cross-section/partial/foreign/unaligned requests and
verify actual source/relocation hashes. The isolated ARM fixture exercises the
numeric outer service with these bytes, test-owned registry/actors and seed1;
it does not establish original module registration or Game NPC allocation.

Next import the genuinely zeroed original DOL area registry as VM-owned mutable
memory and register gor through its original helper/prolog arguments. Other
numeric rows and number-table consumers remain outside this export. Original
map registration also has its own preceding battle request; do not collapse it
into the later npcEnt setup0 request or infer initialized NPCs from a rendered
catalog model. Rendering, full story/encounters and New2DSXL performance remain
separate unfinished work.

The original zeroed registry and number table now have a [VM-owned registration adapter](NATIVE_VM_BATTLE_MODULE_REGISTRATION.md). Source pack bytes stay zero; the runtime calls the actual registration helper. Setup0 admission remains separate from the six original name-to-number mappings and from real actor/pose/RNG ownership.
