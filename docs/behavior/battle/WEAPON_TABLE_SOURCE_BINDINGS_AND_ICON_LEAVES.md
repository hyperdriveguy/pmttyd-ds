# Weapon-table original source bindings and complete icon leaves

Static US G8ME01 revision0 sources for complete
[builder](WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md).
[Verifier](../../../tools/verify_weapon_table_builder.py) pins three complete
bodies/894 instructions,seven linked service identities,and52 source spans:
SDA startup,two badge arrays,17 full40-byte ItemData records,16 weapon heads,
15 original NUL-terminated weapon-name keys,and the16-word Special table.
Weapon heads are their first16 bytes,NOT full weapon/action definitions.
These are original initial bindings,not mutable-runtime snapshots or observed
player inventory. Source assets stay outside source history;verifier stores
scoped evidence bytes without distributing the ISO or compiled asset packs.

## ItemData and badge source records

ItemData base is0x803108a8 with stride40. This builder reads+4 name-key word,
+0x10 unsigned flags halfword,+0x20 icon halfword,+0x24 weapon-pointer word.
Its fallback name route can index by a weapon's own+8 word rather than the
inventory/badge ID in the descriptor. Do not conflate those ID domains.

Startup r13=0x8041cf20 is pinned. Jump source array at r13-24992=80416d80
has eight words;Hammer at r13-24960=80416da0 has nine. Ordered bindings:

| Route / source ordinal | Badge ID | ItemData address | Initial weapon pointer |
| --- | --- | --- | --- |
| Jump0 | 240 | 80312e28 | 80354328 |
| Jump1 | 241 | 80312e50 | 80354268 |
| Jump2 | 244 | 80312ec8 | 803546e8 |
| Jump3 | 245 | 80312ef0 | 803547a8 |
| Jump4 | 246 | 80312f18 | 80354868 |
| Jump5 | 243 | 80312ea0 | 80354568 |
| Jump6 | 242 | 80312e78 | 803544a8 |
| Jump7 | 332 | 80313c88 | 803543e8 |
| Hammer0 | 247 | 80312f40 | 80354928 |
| Hammer1 | 250 | 80312fb8 | 80354ce8 |
| Hammer2 | 249 | 80312f90 | 80354c28 |
| Hammer3 | 251 | 80312fe0 | 80354da8 |
| Hammer4 | 253 | 80313030 | 80354fe8 |
| Hammer5 | 252 | 80313008 | 80354e68 |
| Hammer6 | 248 | 80312f68 | 80354aa8 |
| Hammer7 | 333 | 80313cb0 | 803549e8 |
| Hammer8 | 334 | 80313cd8 | 80354b68 |

Each row's FULL ItemData record is pinned,including its initial icon/name-key
pointer. Badge weapon bodies and pointed message resources are not claimed
complete by this source census. This table establishes original indirection,
not the equipped state returned by pouchEquipCheckBadge in any execution.

## Ordinary weapon name keys

These six weapon heads have original itemId word+8 equal0 and nonzero name key:

| Address | Original key |
| --- | --- |
| 803538a8 | `btl_wn_mario_normal_jump` |
| 80353968 | `btl_wn_mario_kururin_jump` |
| 80353ae8 | `btl_wn_mario_jyabara_jump` |
| 80353de8 | `btl_wn_mario_normal_hammer` |
| 80353ea8 | `btl_wn_mario_kaiten_hammer` |
| 80354028 | `btl_wn_mario_ultra_hammer` |

Preserve these literal keys and msg/US text;do not paraphrase them into invented
story/menu dialogue. The empty-inventory weapon80363e60 uses
`btl_wn_dummy_noitem`,also pinned. Explicit null-inventory substitute80365488
has zero name-key/itemId words in its pinned head;that branch gets its name
from the source inventory ItemData,+4,without applying the common fallback.

## Special pointer table and reusable keys

The16-entry table at80355ca8 has eight nonzero initial words at ordinals0..7;
ordinals8..15 are zero. Original bindings in source order:

| Bit ordinal | Weapon | Original key |
| --- | --- | --- |
| 0 | 803558e8 | `btl_wn_sac_genki0` |
| 1 | 80355a68 | `btl_wn_sac_dekkaku_dokkan` |
| 2 | 803556a8 | `btl_wn_sac_bakugame` |
| 3 | 80355b28 | `btl_wn_sac_mukimuki_body` |
| 4 | 80355768 | `btl_wn_sac_scissor` |
| 5 | 803559a8 | `btl_wn_sac_genki1` |
| 6 | 80355be8 | `btl_wn_sac_sukkari_sukkiri` |
| 7 | 80355828 | `btl_wn_sac_zubastar` |

The verifier pins all16 words,each nonzero weapon's head,and each exact key.
Type4 captures pouch mask once but rereads pointers during admission/pointer/
icon production;initial zeros are not a transitive runtime immutability proof.
Full Special action scripts/cost/effect/audio/source availability remain separate.

## Complete icon leaves: fresh pouch and signed level

`_getJumpIconId`80124c64/0x54 and `_getHammerIconId`80124c10/0x54 each
request pouchGetPtr(),then read its signed byte+0x98 or+0x99 respectively.
Exactly3 returns Jump169/Hammer166;exactly2 returns168/165;EVERY other
value returns167/164,including0,1,negative,and>3. There is no null guard or
clamping. These are WEAPON descriptor icons,distinct from class-table icons
334..343. Do not reuse class icon formulas here.

The builder's icon services acquire a fresh pouch pointer per emitted ordinary
row. Its row-admission checks use a previously retained P and separate level
reads. Preserve that ownership even if a portable implementation normally sees
the same pointer/level. Icon return is stored as a halfword;other descriptor
padding remains unchanged. Reuse original icon assets;no generated substitute
is part of this contract.

## Scope for native implementation

Use these source tables to resolve badge/Special/ordinary pointers and exact
message tokens. Implement complete producer decisions through its companion
contracts. Dynamic inventory/actor/unit-data provisioning,party callback key1,
full weapon/source graph and query reachability,Game/platform/original runtime,
and wider full-game system requirements remain open. Leading-field evidence
must not be promoted into proof that a complete native battle works.
