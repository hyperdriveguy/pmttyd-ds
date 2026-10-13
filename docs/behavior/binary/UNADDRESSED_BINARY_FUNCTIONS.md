# Unaddressed binary functions

Static register of reference-binary functions named in `artifacts/reference/`
that no `docs/behavior/*.md` document mentions. Method: exact plus
case-insensitive substring search for each `.elf` basename over all 339
`docs/behavior/*.md` files, as of commit `725d65a` (before the first Group
A/B closers below were written). Result at that audit point: **43 names** — 22 in
`artifacts/reference/behavior/`, 21 at `artifacts/reference/` top level.
Every disassembly-only file (including the 7 zero-byte `behavior/*.disasm`
placeholders listed at the end) is already mentioned somewhere, so the gap is
exactly these 43.

All facts below are static: file presence/sizes, objdump entry addresses and
first instructions, branch-target census, and machine-evidence rows. No binary
was executed. Nothing here claims runtime behavior, call order semantics, or
gameplay effect.

## Address agreement (checked)

For every gap function that has a `us-usage.json`, `evidence.json`, or
`battle-jump-evidence.json` row, the `.disasm` entry address equals the row's
decimal address (21/21 `us-usage` rows, sampled `evidence.json` rows, and the
`BtlUnit_SetPos` jump-evidence row all match). Example: `fadeEntry`
`80013368` = 2147562344; `BtlUnit_SetPos` `80128c6c` = 2148699244.

## Group A — battle event/camera bodies with fingerprint rows (14)

`evidence.json` (359-function scope) carries address/size/sha256 plus
`direct_branch_link_sites` for each; `us-usage.json` adds object namespace,
link-site, data-word-candidate, and REL-import counts. `docs/behavior/*.md`
names none of them.

| Function | Entry | `.elf` / `.disasm` bytes (lines) | evidence sha12 (branch sites) | us-usage namespace / links |
| --- | --- | --- | --- | --- |
| `btl_camera_nomove_y_onoff` | 8012b2ac | 248 / 674 (20) | af51cb7c9d78 (0) | battle_camera.o / 2 link, 19 rel-imports: none, fingerprinted |
| `btl_camera_nomove_z_onoff` | 8012b21c | 248 / 675 (20) | 4b552f057b0a (0) | battle_camera.o / 2 link, fingerprinted |
| `btlevtcmd_AnimeChangePoseFromTable` | 801064d8 | 300 / 1105 (33) | a3df22081790 (5) | battle_event_cmd.o / 0 link, 24 data words, 373 rel imports, fingerprinted |
| `btlevtcmd_CheckDataOfDataTable` | 80109f88 | 328 / 1365 (40) | 767e8135bb94 (6) | battle_event_cmd.o / 0 link, 45 data words, 9 rel imports, fingerprinted |
| `btlevtcmd_CheckDefinePosetype` | 801062e0 | 468 / 2815 (75) | e2b430ba812b (7) | battle_event_cmd.o / 0 link, 3 data words, fingerprinted |
| `btlevtcmd_CheckStatus` | 80105f0c | 340 / 1499 (43) | acac4146c9d6 (6) | battle_event_cmd.o / 0 link, 37 data words, 426 rel imports, fingerprinted |
| `btlevtcmd_GetHp` | 801101c4 | 312 / 1236 (36) | ee4e1ca9732b (4) | battle_event_cmd.o / 0 link, 7 data words, 79 rel imports |
| `btlevtcmd_GetMaxHp` | 801100cc | 312 / 1239 (36) | ee15406de00c (4) | battle_event_cmd.o / 0 link, 5 data words, 55 rel imports |
| `btlevtcmd_GetUnitWork` | 8010f828 | 324 / 1319 (39) | a28f8e3bc83e (5) | battle_event_cmd.o / 0 link, 64 data words, 1106 rel imports |
| `btlevtcmd_SetUnitWork` | 8010f7a4 | 324 / 1319 (39) | 2ed0ef8d229c (5) | battle_event_cmd.o / 0 link, 136 data words, 1534 rel imports |
| `btlevtcmd_StopWaitEvent` | 8010a1fc | 304 / 1158 (34) | fc7b3d2b31bd (4) | battle_event_cmd.o / 0 link, 3 data words, 56 rel imports, fingerprinted |
| `btlevtcmd_CheckPhase` | 80113708 | 304 / 1149 (34) | 36f0a98938bf (3) | battle_event_cmd.o / 0 link, 3 data words, 79 rel imports |
| `btlevtcmd_get_turn` | 801137b8 | 244 / 590 (19) | 1c93dc2a7230 (1) | battle_event_cmd.o / 0 link, 37 rel imports |
| `btlevtcmd_onoff_battleflag` | 801137ec | 288 / 1044 (30) | 3b702cfef061 (1) | battle_event_cmd.o / 0 link, 1 data word, 19 rel imports |

Static branch notes (absolute `bl` targets; `<.text+..>` self-calls not
resolved): the ten `btlevtcmd_*` bodies all branch to `80031260`, and all but
`StopWaitEvent`/`get_turn`/`onoff_battleflag` also branch to `800f88cc`;
`CheckStatus`, `CheckDefinePosetype`, `CheckDataOfDataTable`, `GetHp`,
`GetMaxHp`, `CheckPhase` additionally branch to `80030cfc`. These shared
targets are unannotated (no `.annotated` file exists for any gap function),
so no callee identity is claimed. The two camera bodies branch nowhere (0
`bl`, 20 lines each, opening with `lwz 5, 7280(13)` / `cmpw 3, 0`).

These 14 also appear as bare names in `harbor-boss-events.json`,
`christine-helper-events.json`, `frankli-lecture-coordinator.json`,
`harbor-boss-init-event.json`, `lecture-counter-event.json`, and
`lecture-player-events.json` without any prose contract.

## Group B — usage-audited but uncontracted (3)

- `BtlUnit_SetPos` (`behavior/`, entry 80128c6c, 208-byte `.elf`, 10-line
  `.disasm`, 0 `bl`; body is three `stfs` stores). `battle-jump-evidence.json`
  pins address 2148699244, size 16, sha256
  `473beb07b076bbf52f19537606519838c337da49a6c284fb1761df1a7e936059`.
  `us-usage.json`: `battle_unit.o`, 44 direct link sites, 19 REL imports.
  `evidence.json` references it as a branch target (site 2148700468). No
  prose contract exists despite this linkage.
- `_btlcmd_MakeActClassTable` (`behavior/`, entry 80124dc8, 1656-byte `.elf`,
  372-line `.disasm`, 9 `bl` to `800f88cc`/`800d5eb0`/`80081b8c`).
  `us-usage.json`: `battle_seq_command.o`, size 1464, 2 link sites. No
  `evidence.json` row, no prose contract.
- `BattleStatusWindowEventOff` (`behavior/`, entry 800f75f8, 236-byte `.elf`,
  17-line `.disasm`, 1 `bl` to `800f7514`). `us-usage.json`: `battle.o`,
  size 44, 1 link site. No `evidence.json` row, no prose contract.

## Group C — symbol-table-anchored only (3)

- `battle_message_disp` (`behavior/`, entry 801abc70, 544-byte `.elf`,
  94-line `.disasm`, 4 `bl`). No JSON row, but
  `behavior/battle-message-symbol.csv` carries `_disp`, `battle_message.o`,
  `801abc70` — the same entry address. Named in
  `gekko-repair-report.json` only.
- `printf_parse_format` (`behavior/`, entry 8026bcb4, 1476-byte `.elf`,
  327-line `.disasm`, 2 `bl` both to `80264750`). No JSON row, but
  `behavior/printf-symbol.csv` carries `parse_format`, `printf.c`,
  `8026bcb4` — the same entry. TheFORMAT_* prose series never names it.
- `scanf_parse_format` (`behavior/`, entry 8026ce44, 1544-byte `.elf`,
  344-line `.disasm`, 0 `bl`). No JSON row, but
  `behavior/scanf-parser-symbol.csv` carries `parse_format`, `scanf.c`,
  `8026ce44` — the same entry. The SCAN_* prose series never names it.

### Group C alias resolutions (verified, no new contract needed)

All three listings name the file, not the function; each function is
contracted under its canonical name at the same entry address:

| Listing | Canonical contract |
| --- | --- |
| `battle_message_disp` = `_disp` @801abc70 | [Announcement messages](../battle/BATTLE_ANNOUNCEMENT_MESSAGES.md) documents the `dispEntry` callback use and the `_disp` symbol/size0x160 |
| `printf_parse_format` = printf's `parse_format` @8026bcb4 | [Format directive parser](../format/FORMAT_DIRECTIVE_PARSER.md) opens "This is printf's parse_format at 0x8026bcb4" |
| `scanf_parse_format` = scanf's `parse_format` @8026ce44 | [Scan directive parser](../format/SCAN_DIRECTIVE_PARSER.md) opens at 0x8026ce44 size 0x548; [consumers](../format/SCAN_FORMAT_CONSUMERS.md) call it per `%` |

## Group D — duplicate extractions under two names (2 pairs)

- `fallSpeed` and `marioGetFallSpd` (top level): both open `80094098
  <.text>:` with identical bodies — `diff` reports only the header filename
  line differs (43 lines each, 1 `bl` to `8005e0c0`). Only `marioGetFallSpd`
  has a `us-usage.json` row (`mario_motion.o`, size 148, 9 link sites);
  `fallSpeed` has none. Neither is mentioned in prose.
- `system_angleABf` (`behavior/`, entry 8000703c, 628-byte `.elf`, 115-line
  `.disasm`, 0 `bl`, opens with `fsubs 5, 3, 1`) and top-level `angleABf`
  (`.elf` + `.disasm`): the top-level disassembly opens with the identical
  entry and first instruction (`8000703c`, `fsubs 5, 3, 1`). `system_angleABf`
  has no JSON row; the ANGLE/system-angle prose never names it. Static
  cross-link: `checkLand2` (Group E) branches to `8000703c`.

## Group E — top-level field/hit/fade bodies, disassembly only (17 net of pairs)

No `.annotated`, no `.json` row (except where noted), no prose mention. Frame
sizes below are the first `stwu 1, -N(1)` immediates — static stack-frame
facts, not behavior claims.

| Function | Entry | `.elf` / `.disasm` (lines) | Frame | `bl` total (distinct absolute targets) |
| --- | --- | --- | --- | --- |
| `bero_length` | 8007a76c | 436 / 2562 (67) | -32 | 3 (800304bc, 80031260) |
| `bero_normal` | 80079b74 | 976 / 7754 (202) | -192 | 11 + 6 self (8001449c, 8001577c, 80030840, 80031260, 8005e0c0) |
| `bottomlessBero` | 80096de4 | 360 / 1736 (48) | -80 | 1 (8005e0c0) |
| `bottomlessEvt` | 8007ac7c | 372 / 1787 (51) | -48 | 4 + 1 self (80031260) |
| `bottomlessOn` | 80096d34 | 232 / 464 (16) | -16 | 1 (8005e0c0) |
| `bottomlessPos` | 80096d5c | 328 / 1365 (40) | -48 | 1 (8005e0c0) |
| `checkLand` | 8008dc70 | 340 / 1534 (43) | -32 | 4 + 2 self (8005e0c0, 8008d4a0) |
| `checkLand2` | 8008cda8 | 1976 / 18158 (452) | -416 | 13 + 3 self (8000703c, 800158a4, 8005e0c0) |
| `dashSpeed` | 80087ce4 | 352 / 1695 (46) | -32 | 2 (8005c164, 8005e0c0) |
| `walkSpeed` | 80087d84 | 348 / 1655 (45) | -32 | 2 (8005c164, 8005e0c0) |
| `movementSpeed` | 80095a8c | 300 / 1159 (33) | -16 | 1 (8005e0c0) |
| `mario_width` | 800916f0 | 368 / 1937 (50) | -16 | 1 (8005e0c0) |
| `evt_fade_mapchange` | 80135c38 | 452 / 2695 (71) | -32 | 5 (all 80031260) |
| `fadeEntry` | 80013368 | 2044 / 18950 (469) | -48 | 12 + 9 self (800050b4); us-usage: fadedrv.o, 55 link sites, 19 REL imports |
| `mapChangeInit` | 8000874c | 508 / 3451 (85) | -16 | 13 self only; opens `addi 4, 2, -32568` |
| `hitDamageReturnSet` | 80013fc4 | 1036 / 8612 (217) | -32 | 16 self only; us-usage: hitdrv.o, 17 link sites |
| `hitGrpDamageReturnSet` | 80013efc | 392 / 2135 (56) | -48 | 3 self only; us-usage: hitdrv.o, 1 link site |
| `hitVec` | 800158a4 | 3524 / 34524 (839) | -320 | 49 + 48 self (800153d0); named in gekko-repair-report.json |
| `hit_damage_return_set` | 8007458c | 408 / 2203 (60) | -32 | 5 (8000519c, 80013efc, 8001449c, 8002ff58, 80031260); named in harbor-opening/lecture JSONs |

Notes: `hit_damage_return_set` branches to `80013efc`, the entry of
`hitGrpDamageReturnSet` — a static cross-link, not a proven call contract.
`checkLand2` branches to `800158a4` (`hitVec` entry) and `8000703c`
(`angleABf`/`system_angleABf` entry). `dashSpeed`/`walkSpeed` share the same
two absolute targets. `gekko-repair-report.json` names `bero_normal`,
`bottomlessBero`, `checkLand`, `checkLand2`, `dashSpeed`, `hitVec`,
`hit_damage_return_set`, `walkSpeed` without prose contracts. (`fallSpeed`
is counted in Group D, not here.)

`party_flyMain` (`behavior/`, entry 800c7ff8, 1584-byte `.elf`, 354-line
`.disasm`, 43 `bl`) has no JSON row and no prose mention; the
FIELD_PARTNER_FLY_* series never names it. Its sibling `flyMain.disasm` is a
zero-byte placeholder (see below), so no content comparison is possible.

## Zero-byte placeholders (no content to document)

Seven `behavior/*.disasm` files are 0 bytes: `angleABf`, `flyMain`,
`partyGetOtherPtr`, `partyGetTargetNum`, `partyGetTargetNum` sibling
`BattleGetFirstAttackTarget`, `PSVECdotProduct`, `_announceMessage`. All seven
names already appear in prose, so they are not coverage gaps — but they carry
no instructions and cannot support contracts until re-extracted.

## What this register does not claim

- Name similarity (`CheckStatus` vs STATUS docs, `walkSpeed` vs movement
  docs, `fadeEntry` vs FADE docs) is not coverage; none of the 43 strings
  occurs in any prose file.
- `fingerprinted: true` on six rows (both cameras, `AnimeChangePoseFromTable`,
  `CheckDataOfDataTable`, `CheckDefinePosetype`, `CheckStatus`,
  `StopWaitEvent`) means byte identity only, not a behavior contract.
- Absolute `bl` targets (e.g. recurrent `8005e0c0`, `80031260`) are
  destination addresses, not identified callees; resolving them is future work.
- Duplicate bodies (`fallSpeed`/`marioGetFallSpd`, `system_angleABf`/`angleABf`)
  are reported as observed extraction facts; which name (if either) is
  canonical is not decided here.

## Next steps for closers

1. Group A first: each already has sha-pinned branch sites; recovering the
   shared `80031260`/`800f88cc`/`80030cfc` leaves unblocks ten contracts.
2. `BtlUnit_SetPos` (16 bytes, 44 link sites) and the `hit_damage_return_set`
   → `hitGrpDamageReturnSet` edge are small, high-linkage targets.
3. The 15 pure-disassembly top-level bodies need prose contracts from scratch;
   `hitVec` (839 lines) and `checkLand2` (452 lines) are the largest.
4. Re-extract the seven zero-byte placeholders before citing them as evidence.

## Closures recorded 2026-10-12 (late session)

Recorded by the documentation pass that produced
[stick angle/magnitude](../overworld/MARIO_STICK_ANGLE_AND_MAGNITUDE.md),
[jump parameter installation](../overworld/MARIO_SET_JUMP_PARAMETERS.md) and
[peak-Y publisher](../overworld/MARIO_PEAK_Y_PUBLISHER.md). The 43 rows above
are unchanged; these are additional rows found by re-running the
reference-versus-decompilation census, not a rewrite of the register.

Method note for later closers: the r2 small-data base **is** recoverable. The
US `__init_registers` at `0x8000539c` sets `r2 = 0x80427440` and
`r13 = 0x8041cf20`, and the DOL section map at `0x20300` translates those RAM
addresses to ISO byte ranges. Any `lfs f, disp(2)` therefore resolves to a
readable word in the read-only ISO, so "SDA values are not recoverable" is not
a justified limitation. Resolved examples: `r2-27100` = `70.0` (`0x80420A64`),
`r2-27096` = `360.0`, `r2-27516` = `1.0`, `r2-27360` = `18.0`,
`r2-27300` = `6.0`, `r2-27276` = `1.0`, `r2-27572` = `0.5`.
`rlwinm` masks with `MB == ME == k` select the bit whose LSB index is
`31 - k`; `MB > ME` wraps (for example `rlwinm 0,0,0,30,28` clears bit 2). With
that convention the walk/dash and `marioHitCheckWidth` gates test mask
`0x00100000`, i.e. `MARIO_FLAG_PAPER_MODE` (1<<20) from the pinned header, so
the "0x00100000 is distinct from the Paper-mode 0x00200000 bit" note in
[the speed/fall leaves](../overworld/OVERWORLD_SPEED_FALL_LANDING_LEAVES.md) is
a missed identity, and the `0x00200000` paper-mode value used in
[ordinary overworld movement](../overworld/OVERWORLD_MARIO_MOVEMENT.md) disagrees
with both the header enum and the binary. Use these conventions when closing the
remaining rows.

Closed by this pass (no decompiled definition in the pinned snapshot):

| Listing | Symbol | Entry | Contract |
| --- | --- | --- | --- |
| `getStick` | `marioGetStick` | 80095af8 | [Stick angle/magnitude](../overworld/MARIO_STICK_ANGLE_AND_MAGNITUDE.md) |
| `setJump` | `marioSetJumpPara` | 800941bc | [Jump parameter installation](../overworld/MARIO_SET_JUMP_PARAMETERS.md) |
| `peakY` | `marioSetPeakYpos` | 800968f0 | [Peak-Y publisher](../overworld/MARIO_PEAK_Y_PUBLISHER.md) |

`landOn` (`marioLandOn`, 80092c94) and `searchHead` (`marioSearchHead`,
8008c1a8) remain open: both are partially described inside the airborne and
bottomless contracts but have no dedicated body contract and no snapshot
definition.

### Second pass (same session)

Closed by the second pass, again with no snapshot definition:

| Listing | Symbol | Entry | Contract |
| --- | --- | --- | --- |
| `BattleAfterReactionQueueInit` | same | 800f7af0 | [After-reaction queue](battle/BATTLE_AFTER_REACTION_QUEUE.md) |
| `BattleAfterReactionEntry` | same | 800f7a78 | [After-reaction queue](battle/BATTLE_AFTER_REACTION_QUEUE.md) |
| `BattleAfterReactionRelease` | same | 800f79e4 | [After-reaction queue](battle/BATTLE_AFTER_REACTION_QUEUE.md) |
| `BattleStatusWindowCheck` | same | 800f7514 | [Status window/camera X](battle/BATTLE_STATUS_WINDOW_CHECK_AND_NOMOVE_X.md) |
| `btl_camera_nomove_x_onoff` | same | 8012b33c | [Status window/camera X](battle/BATTLE_STATUS_WINDOW_CHECK_AND_NOMOVE_X.md) |
| `btlevtcmd_SetEventDamage` | same | 8010a55c | [Damage event leaves](battle/BTLEVTCMD_DAMAGE_EVENT_LEAVES.md) |
| `btlevtcmd_CommandCheckDamage` | same | 8010a688 | [Damage event leaves](battle/BTLEVTCMD_DAMAGE_EVENT_LEAVES.md) |
| `msgWindow_ForceClose` | same | 80081424 | [Window close/repeat](message/MSG_WINDOW_FORCE_CLOSE_AND_REPEAT.md) |
| `msgWindow_Repeat` | same | 800814f4 | [Window close/repeat](message/MSG_WINDOW_FORCE_CLOSE_AND_REPEAT.md) |
| `evt_msg_close` | same | 800d2178 | [Window close/repeat](message/MSG_WINDOW_FORCE_CLOSE_AND_REPEAT.md) |
| `evt_msg_repeat` | same | 800d21a8 | [Window close/repeat](message/MSG_WINDOW_FORCE_CLOSE_AND_REPEAT.md) |
| `evt_sub_intpl_init` | same | 80056120 | [Interpolation/party leaves](event/EVENT_INTERPOLATION_AND_PARTY_RUN_STOP_LEAVES.md) |
| `evt_sub_intpl_get_value` | same | 80056064 | [Interpolation/party leaves](event/EVENT_INTERPOLATION_AND_PARTY_RUN_STOP_LEAVES.md) |
| `evt_party_run` | same | 800eae70 | [Interpolation/party leaves](event/EVENT_INTERPOLATION_AND_PARTY_RUN_STOP_LEAVES.md) |
| `evt_party_stop` | same | 800eaeb8 | [Interpolation/party leaves](event/EVENT_INTERPOLATION_AND_PARTY_RUN_STOP_LEAVES.md) |

Correction carried by the camera X row: the Y/Z contract in
[the wait/camera leaves](../overworld/OVERWORLD_SPEED_FALL_LANDING_LEAVES.md)
sibling `battle/BATTLE_WAIT_CAMERA_STATUS_EVENT_LEAVES.md` describes the guard
as "return immediately when incoming r3 < 0". The actual guard is
`cmpw r3, word[W+10076]` / `bclr 12,0`, which returns only when the first
argument equals that word, and the `word[W+10076]` load is therefore live.
The same contract's "unresolved `bl 0x800f7514`" is now
[resolved](battle/BATTLE_STATUS_WINDOW_CHECK_AND_NOMOVE_X.md) as
`BattleStatusWindowCheck`.
