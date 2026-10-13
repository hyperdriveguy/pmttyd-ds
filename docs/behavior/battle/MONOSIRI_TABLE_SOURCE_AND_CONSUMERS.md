# Monosiri/Tattle table provisioning, field consumers and caller census

US GameCube G8ME01 disc0 revision0; recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete lookup and message-key event leaf;
scoped original sampling/log/model/pose consumers and direct-reference census.
The earlier spatial-sound interpretation is superseded. These are static
contracts, not complete Tattle gameplay or original execution observations.

## Initialized table and field-use evidence

Original DOL initializes monosiriMessageTable at8036cf40, size15a8hex,
231 records of24 decimal bytes. It is already present in the executable;
no per-unit allocation/population is performed by the lookup. Six word fields
contain string pointers or0 in this supplied image. Pointer null/non-null
counts by field are0:230,4:164,8:143,c:229,10:230,14:230 (nonnull counts).
These are source-data properties, not live mutation invariants.

| Hex field | Observed original use | Concrete consumer anchors |
| --- | --- | --- |
| 0 | Name message-key input to msgSearch80081b8c; comparisons/display can apply story overrides separately | compare_func3 reads80155aec/afc; monosiri_disp801716c4/c8; monoshiriGX80172b18/1c |
| 4 | Battle help/Tattle key; sampling rejects null and event leaf returns raw pointer | sampling800ffca0; event80189608 |
| 8 | Log eligibility and selected log message key | winLogInit80177794 and801778f0; winLogMain80176918→winMsgEntry80162ed0 |
| c | Animation group/model name | monosiriMain801724ac→animGroupBaseAsync80039d0c;80172578→animPoseEntry8004176c |
| 10 | Animation name/pose input | monosiriMain80172590→animPoseSetAnim80041170 with third argument1 |
| 14 | Additional message-key lookup for formatted log detail | monosiri_disp801717c0/c4→msgSearch |

The large consumers are only contracted in these used regions. Special kind107
has another model path, and kinds77/135 have story-dependent display overrides;
these do not replace every table field or establish a complete log state machine.
The last field's intended localized label is not inferred from a symbolic name.
Some records contain model/pose placeholder identifiers in message fields;
retain them exactly and recover callers instead of fabricating better keys.

Original examples (identifiers only, never authored replacement dialogue):

| Kind | Name/help/log key identifiers | Model / pose / additional key |
| --- | --- | --- |
| 0 | All three null | All three null |
| 1 | btl_un_kuriboo / btl_hlp_kuriboo / menu_enemy_001 | c_kuribo / KUR_Z_2 / menu_monosiri_nara |
| 13 | btl_un_kamec_bunsin / btl_hlp_kamec_bunshin / null | c_kamek / KMK_Z_1 / menu_monosiri_tik |
| 208 | btl_un_ufo / null / null | c_kuribo / KUR_Z_2 / menu_monosiri_gon |
| 222 | btl_un_mario / btl_hlp_mario / menu_enemy_160 | c_kuribo / KUR_Z_2 / KUR_Z_2 |
| 224 | btl_un_christine / null / menu_enemy_162 | c_kuribo / KUR_Z_2 / KUR_Z_2 |

Missing/null help and missing/null log keys have different gates. Kind223 has
a nonnull log key but no help key, for example. Do not infer Tattle target
eligibility or saved registration from only the table's total record extent.

## Complete event message-key callback

btlevtcmd_get_monosiri_msg_no801895b0/84hex:

1. Retain event E and argument pointer A. Resolve A0 through evtGetValue;
   retain that resolved UNIT ID, with no BattleTransID call.
2. Resolve A4 through evtGetValue too, but do not consume its returned value.
   Preserve this call and its memory/service effects; it is not a raw skip.
3. Capture raw output A8, then read CURRENT BattleWork and lookup the retained
   unit ID through BattleGetUnitPtr. Read unit.word8 kind.
4. Call battleGetUnitMonosiriPtr(kind), read its word4 and publish it through
   evtSetValue(E,rawOutput,keyPointer). Return event2.

No firstCall,unit/table/key-null/index guard, destination resolution or waiting
is supplied. A selector such as-2 is not translated by this leaf; callers must
supply its actual unit-ID domain. The output is a message-key POINTER, despite
“msg_no” in the symbol. No dialogue text or message playback occurs here.
Source anchors801895d4/e4/e8/f0/f4/f8/fc and80189608/610 pin the order.

## Original candidate sampling behavior

C mask1 first rejects part flag80000, then performs lookup(unit.kind), actual
null-pointer test, fresh kind reload/second lookup and record.word4 null test.
Only the retained part/candidate route is affected. This preserves the existing
[complete sampling contract](BATTLE_ENEMY_CANDIDATE_SAMPLING.md), whose pointer
check was correct. Earlier lookup prose incorrectly dismissed that check.
[Packed source-order requirements](BATTLE_PACKED_SAMPLING_SOURCE_ORDER.md)
remain valid and now link to a correct local leaf/source contract.

## Scoped complete direct-reference census

Aligned opcode18 branch scan over all original DOL text finds17 linked sites,
0 nonlink sites and0 initialized-data pointer candidates to8012ef20:

| Original containing body | Linked sites |
| --- | --- |
| _btlSamplingEnemy | 800ffc84,800ffc9c |
| compare_func3_r | 801559a0,801559ac |
| compare_func3 | 80155ad8,80155ae4 |
| compare_func2_r | 80155cbc,80155cc8 |
| compare_func2 | 80155dd0,80155ddc |
| monosiri_disp | 801715f4 |
| monosiriMain | 801724a0 |
| monoshiriGX | 80172aa0 |
| winLogMain | 8017690c |
| winLogInit | 80177790,801778ec |
| btlevtcmd_get_monosiri_msg_no | 801895fc |

Scanning all29 shipped REL relocation streams additionally finds module0
REL24/type10 references in gor.rel section1+508c (monosiri_on) and tou2.rel
section1+3bf8 (evt_tou_get_fighter_tribe). These are imported call bindings,
not executed scenes. ADDR32-only event inventories would miss both.

Reproduce with `python3 tools/us_usage_audit.py --symbols artifacts/reference/us_symbols.csv --output /tmp/monosiri-usage.json`.
This census does not resolve indirect calls, computed pointers, dead conditions,
raw aliases or dynamic source mutation. Zero data-word candidates is not a
proof that no dynamically registered callback ever uses this leaf.

## Validation and remaining owners

[Source metadata](../../../tools/monosiri_source_evidence.json) hashes complete
lookup/message callback/table and containing consumer bodies, with scoped
instruction anchors, table-null counts and census annotations. Generic DOL
verification uses `tools/verify_overworld_roads.py --manifest` with that file.
Fresh listings and independent usage report remain under `/tmp/inbox-monosiri-*`.
Original ISO is read-only; metadata contains no message payload or executable
body. Table identifiers are original source keys, not replacement dialogue.

Flag setters/checkers use separate persistent switch/work storage and shared
same-kind lists; they are not table-population writers. Remaining: those full
lifetime/save/source owners, table mutation/alias invariants, full REL/log/event/
Tattle action/AC/target/presentation composition and original execution traces.
EVT-01's table/used-consumer/direct-census portions are answered; its complete
source/runtime integration remains OPEN.
