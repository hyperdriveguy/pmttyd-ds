# Party snapshot reset,import/export,and native lifetime requirements

Companion to [complete initializer](PARTY_BATTLE_SNAPSHOT_INITIALIZATION_AND_RAW_MAP.md),
static US G8ME01 revision0. This consolidates recovered local contracts so a
native adapter can implement the request with correct storage/ownership.
It does not establish original on-disk save format or full battle reentry.
[Verifier](../../../tools/verify_party_snapshot_initialization.py) pins the
initializer/current source constant/scoped BattleInit binding.

## Same record layout across three local operations

Each partner kind224..230 addresses44 bytes at
`BattleWork+0x162a0+44*(kind-224)`. The complete existing
[status/pouch import and export](../unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md)
provides these consumer/producer bindings:

| Record field | Initializer | Import to unit | Export from unit |
| --- | --- | --- | --- |
| +0 binary32 | 1.0 | U114 | U114 |
| +4..21hex,30 bytes | fill0 | U118..135 | U118..135 |
| +22/+23 | preserve | not copied | not copied |
| +24 word | 0 | U138 | U138 |
| +28 halfword | 0 | U13c | U13c |
| +2a halfword | 0 | U13e | U13e |

Initializer order is float→word24→fill30→half28→half2a. Import loads/stores
word24 before the30 status bytes after float. Export writes float→word24→30
bytes→half28/2a. Do not merge all three operations into a generic whole-record
copy/reset that changes ordering,preserves the wrong fields,or invents state
normalization. Snapshot data is BattleWork memory,not a disk-save claim.

The default float1.0 is later imported as unit114. The
[complete gift-effect caller](GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md)
reads that field as a scalar multiplier after recovery/height queries. This
links a literal initializer to an established consumer;it does not prove a
measured effect height or that intervening status producers cannot change it.
30 imported bytes can contain signed durations/shared pairs and are copied raw,
not restored via individual status setters.

## Unit data-table provisioning already has local evidence

[Complete unit entry](BATTLE_UNIT_ENTRY_COMPLETE.md) and
[entry source/stat bindings](../unit/UNIT_ENTRY_STATS_AND_SPAWN.md) already
establish kind-definition+c0→unit18 data-table publication. They also publish
registry membership before later initialization. The
[seven original party key1 chains](PARTY_WEAPON_CALLBACK_KEY1_SOURCE_BINDINGS.md)
and [complete party callbacks](PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md)
therefore have a local static definition→runtime field→lookup/call path.

This linkage is narrower than complete runtime party provisioning:it does not
prove which field/battle source selects every party definition,when global
battle/current party pointers change,all later unit18/table mutations,or full
resource/init event readiness. Preserve that distinction instead of repeating
that the initial table-copy writer is wholly unknown. Remaining source/lifetime
work must target those actual missing domains.

## Native implementation requirements

Provide address-backed seven-record storage and exact kind-index arithmetic,
1.0 scalar initialization,ordered fill/typed stores,and retained gap bytes.
Import/export must use the same record addresses and their own fresh kind/
resource read order,as in the existing complete contracts. Keep unit data-table
provisioning separate from snapshot restoration;these are different fields.
No pouch calls or party HP copies occur in this initializer itself.

Meaningful native cases include nonzero seeded gaps surviving reset,all7 records
being visited in kind order,helper-visible word24 already cleared before fill,
retained record address after local K increment,and import/export copying raw
30-byte states without gap normalization. Use reused original data and explicit
host-to-original address mappings;tests of chosen native assumptions are not
original execution observations.

## Remaining request boundaries

Later snapshot producers/import/export invocation domains,party switch/turn/
battle-end/reset cadence,global owner lifetime,field/pouch/disk persistence,
original traces,query-event source reachability,Game/platform/full-game system
requirements remain open. A complete reset leaf does not make save/party/battle
lifecycle complete. Next port documentation should establish caller domains of
snapshot export and unit/table source changes or continue the native event/
selected-weapon query closure with explicit source evidence.
