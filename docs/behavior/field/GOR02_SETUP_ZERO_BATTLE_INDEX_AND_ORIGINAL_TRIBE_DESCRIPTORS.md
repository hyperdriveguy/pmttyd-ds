# Gor02 setup zero battle index and original tribe descriptors

US G8ME01 revision0, read-only static REL/DOL and tagged-map evidence for
[EVT-01](../../requests/EVT-01.md). This supplements the
[ordered setup contract](NPC_SETUP_ORDERED_INSTALLATION_AND_RETAINED_PROVIDER_FRONTIER.md)
and [battle helper contract](FIELD_BATTLE_ENTRY_AND_RETURN.md). No source
runtime/raster observation or complete native actor registration is claimed.

## Index zero is populated

All15 nonterminal `npcEnt@gor_02.o` records (REL44f98, size5c0) have battle
index+58=0. The -1 clearing branch is therefore not their actual call path.
areaDataPtr800391e0 finds gor through the original registry: area_gor80305364
word0 points to8041fa70 (`gor`); relSetBtlAddr80038fd8 installs the loaded
module's table pointer at area record+8. Source npcSetBattleInfo80043d80
derives `gor` from current GP map prefix+12c..12e; index0 uses the first
0x44-byte table record. A registered table/current-map owner remains required.

Original `btlsetup_gor_tbl@battle_database_gor.o` begins REL60e30,
native relocation guest90540da8. Its first word is a relocation to nonnull
Shift-JIS string `マップstg_00_0` at REL1dbc8. Consequently the helper's
null/first-word-null shortcut does not apply. Relevant first-record fields:

| Field | Source value / destination |
|---|---|
| +08 | 0 |
| +0c | ffffffff, outside saved-switch encoded domain |
| +10 | null |
| +14 | weighted list REL60da0 / guest90540d18 |
| +18 | 0 |
| +1c | 0, audience initialization enabled |
| +20..3f | sixteen byte pairs (1,1) |
| +40 | string BGM_ZAKO_BATTLE1 at REL1dbd8 |

The audience loop requests irand(1) sixteen times; do not optimize these
requests away. The weighted list contains one positive entry weight50,
group REL60d80 / guest90540cf8 and stage REL608f8 / guest90540870,
then a zero-weight terminator. It requests irand(50) once, despite the unique
group. That group has count1, unit-array REL60d50 / guest90540cc8.
The unit kind is unit_monban at REL61c30 / guest90541ba8. Its item-table
pointer is DOL80417880 (`battle_item_monban`), whose eight original bytes
are zero, so that table makes no item RNG request.

The populated branch clears actor+316, reloads the selected group/unit/kind
and publishes kind byte+12 to actor+316, then ORs main flag10000000.
The outer helper still copies the200-byte block to four nullable associated
actors in its original order. This is17 RNG requests per actual setup actor;
source RNG ownership/request effects cannot be replaced with a null setup.

Evidence inputs: `artifacts/trouble-gor.rel`, ADDR32 bindings decoded with
`tools/rel_event_inventory.py::read_bindings`, original symbols in
`artifacts/reference/us_symbols.csv`, DOL file offsets302364 (area_gor) and
3c4cc0 (battle_item_monban). Raw REL words are zero at relocation sites;
their binding must be applied before testing null. A trial general data
closure of btlsetup_gor_tbl encounters an unbounded/ambiguous dependency at
REL60aac: the current EVT closure alone is not proof that battle/resource
tables are registered in a runtime address space.

## Actual initial descriptor identities

The original tagged map `m/gor_02/s` (local inspection copy
`local-assets/field-actors/gor_02.s`) assigns these tribes. DOL npcGetTribe
scans84-byte rows from803316d0. Actor+28 must come from the actual registered
actor's current descriptor; the table below does not authorize substituting
one generic model or ignoring later descriptor changes.

| Actor(s) | Tribe row | Original model |
|---|---|---|
| パワーアップ屋 | 80331c10 | c_deale |
| クリハカセ | 80333014 | c_kuri_h |
| 情報屋完璧 | 80331e08 | c_joho_b |
| 盗賊団親分 | 80332dc8 | c_binbo |
| 盗賊団１ / 盗賊団２ / 盗賊団３ | 80333404 | c_monban |
| サンダース | 80334b50 | c_bomt_n |
| クリボー４ | 8033335c | c_kuribo_n |
| チューさん３ | 80333554 | c_tyusan |
| ロテン３ | 803336a4 | c_roten_g |
| ボロドー１ / ボロドー２ | 80332d20 | c_borodo_n |
| クラガリさん | 80333110 | c_f_mura |
| ルイージ | 803328dc | c_luigi |

For every listed row except Chu, descriptor+08 and+10 strings are S_1,
+14 is T_1; Chu uses CHU_S_1, CHU_S_1 and CHU_T_1. These descriptor
fields are the source of setup's late pose-buffer copies and initial pose
request. Real animation handle/readiness, pose mutation, named actor record,
and original map registration lifetime remain separate required owners.

The finite field-reader graph is now exported in the shared area pack; see
[bounded setup0 views](BOUNDED_ORIGINAL_GOR_SETUP_ZERO_DATA.md). This avoids the
unbounded whole-table closure without admitting other numeric rows or the stage
body. The [actual registration producer](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md)
has a separate earlier map battle request and real pose/resource/lifetime owners.
