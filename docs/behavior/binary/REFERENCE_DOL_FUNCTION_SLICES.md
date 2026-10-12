# DOL Function Slices (`.elf` / `.disasm` corpus)

The GameCube (Gekko/PowerPC) inspection corpus under `artifacts/reference/`:
small ELF objects cut from the read-only US ISO DOL, each explained by a
text listing. This document inventories every slice, its listing, and the
cross-checks that tie listings to the symbol table. All facts were verified
2026-10-12 with `find`, `stat`, `cmp`, `diff`, `readelf` (headers/sections
only) and `grep`. **No binary was executed.**

Companion documents: [master inventory](REFERENCE_FILES_INVENTORY.md),
[tool trees](REFERENCE_TOOL_TREES.md),
[unaddressed-functions register](UNADDRESSED_BINARY_FUNCTIONS.md),
[binary artifacts](BINARY_ARTIFACTS.md).

## What the slices are

- `file(1)` identity (verified on `walkSpeed.elf`, `bero_normal.elf`):
  `ELF 32-bit MSB relocatable, PowerPC or cisco 4500, version 1 (SYSV),
  stripped`. `readelf -h` on `walkSpeed.elf`: type `REL`, machine `PowerPC`,
  0 program headers, 3 section headers; sections are `.text`, `.shstrtab` and
  a NULL entry.
- They are inspection objects, not runnable programs: no program headers, no
  entry point (`readelf`: entry `0x0`), stripped symbols.
- `docs/GEKKO_DISASSEMBLY.md` documents the pipeline: repo tool
  `tools/inspect_dol_function.py <name> --symbols artifacts/reference/us_symbols.csv
  --output <name>.elf` slices a named function from the read-only ISO DOL;
  `tools/gekko_disasm.py` then repairs LLVM’s misdecoding of GameCube
  paired-single instructions in the stored listings. Neither tool executes
  original code.

## Listing formats that explain the binaries

| Extension | Format (first lines, verbatim shape) | Count |
| --- | --- | ---: |
| `.disasm` | blank line, `artifacts/reference/<path>.elf:	file format elf32-powerpc`, blank, `Disassembly of section .text:`, blank, `<addr> <.text>:` then `addr: ww ww ww ww  	mnemonic operands` | 70 top-level + 546 in `behavior/` (snapshot2026-10-1216:22 MDT) |
| `.txt` (listings) | identical format to `.disasm`; only the filename extension differs | 6 (`map-display.txt`, `map-display-group.txt`, `map-display-object.txt`, `map-main.txt`, `map-play.txt`, `mario-init.txt`; the seventh top-level `.txt`, `map-init-calls.txt`, is not a listing) |
| `.annotated` | identical body to the paired `.disasm`, plus ` # SymbolName` comments on `bl` lines (verified: `BattleCheckDamage.annotated` has 54 such comments, its `.disasm` has 0) | 16 in `behavior/` |

The listing’s first content line embeds the path of the slice it disassembles,
which is how each listing is bound to its binary. `map-init-calls.txt` is **not**
a listing: it is a per-map event-script extraction
(`gor_00_init_evt 0x2f31c evt_map_playanim ['nami', '0x1', '0x0']` …).

## Top-level pairing table (88 slices)

Verified with `stat`; “no listing retained” means no same-stem `.disasm` or
`.txt` exists at top level (some of these names have copies under `behavior/`,
noted after the table).

| Slice (.elf) | Bytes | Listing file | Listing bytes | Notes |
| --- | ---: | --- | ---: | --- |
| `adjustMove.elf` | 948 | `adjustMove.disasm` | 8051 |  |
| `angleABf.elf` | 628 | `angleABf.disasm` | 4548 |  |
| `bero_length.elf` | 436 | `bero_length.disasm` | 2562 |  |
| `bero_normal.elf` | 976 | `bero_normal.disasm` | 7754 |  |
| `bottomlessBero.elf` | 360 | `bottomlessBero.disasm` | 1736 |  |
| `bottomless.elf` | 444 | `bottomless.disasm` | 2654 |  |
| `bottomlessEvt.elf` | 372 | `bottomlessEvt.disasm` | 1787 |  |
| `bottomlessOn.elf` | 232 | `bottomlessOn.disasm` | 464 |  |
| `bottomlessPos.elf` | 328 | `bottomlessPos.disasm` | 1365 |  |
| `checkLand2.elf` | 1976 | `checkLand2.disasm` | 18158 |  |
| `checkLand.elf` | 340 | `checkLand.disasm` | 1534 |  |
| `check_width.elf` | 368 | — | — | no listing retained |
| `chkFilterAttr2.elf` | 236 | `chkFilterAttr2.disasm` | 531 |  |
| `chkfilterVec.elf` | 416 | `chkfilterVec.disasm` | 2402 |  |
| `dashSpeed.elf` | 352 | `dashSpeed.disasm` | 1695 |  |
| `evt_fade_in.elf` | 300 | — | — | no listing retained |
| `evt_fade_mapchange.elf` | 452 | `evt_fade_mapchange.disasm` | 2695 |  |
| `evt-map-flag.elf` | 360 | — | — | no listing retained |
| `evt_mario_mov_pos2.elf` | 576 | `evt_mario_mov_pos2.disasm` | 3859 |  |
| `fadeEntry.elf` | 2044 | `fadeEntry.disasm` | 18950 |  |
| `fadeMain.elf` | 4744 | `fadeMain.disasm` | 46852 |  |
| `fallSpeed.elf` | 340 | `fallSpeed.disasm` | 1542 |  |
| `getStick.elf` | 632 | `getStick.disasm` | 4503 |  |
| `hitCalcVtxPosition.elf` | 1396 | `hitCalcVtxPosition.disasm` | 12068 |  |
| `hitCheckAttr.elf` | 500 | — | — | no listing retained |
| `hit_damage_return_set.elf` | 408 | `hit_damage_return_set.disasm` | 2203 |  |
| `hitDamageReturnSet.elf` | 1036 | `hitDamageReturnSet.disasm` | 8612 |  |
| `hitEnt.elf` | 1356 | `hitEnt.disasm` | 11660 |  |
| `hitEntry.elf` | 348 | — | — | no listing retained |
| `hitEntrySub.elf` | 824 | `hitEntrySub.disasm` | 6174 |  |
| `hitGetAttr.elf` | 216 | — | — | no listing retained at top level; byte-identical slice and listing exist as `behavior/hitGetAttr.elf`/`.disasm` |
| `hitGetDamageReturnPos.elf` | 204 | — | — | no listing retained |
| `hitGrpDamageReturnSet.elf` | 392 | `hitGrpDamageReturnSet.disasm` | 2135 |  |
| `hitObjGetPos.elf` | 528 | `hitObjGetPos.disasm` | 3431 |  |
| `hitObjGetPosSub.elf` | 1580 | `hitObjGetPosSub.disasm` | 14232 |  |
| `hitVec.elf` | 3524 | `hitVec.disasm` | 34524 |  |
| `init_registers.elf` | 336 | — | — | no listing retained |
| `landOn.elf` | 3388 | `landOn.disasm` | 33574 |  |
| `mapChangeInit.elf` | 508 | `mapChangeInit.disasm` | 3451 |  |
| `mapChangeMain.elf` | 3000 | `mapChangeMain.disasm` | 29496 |  |
| `map-display.elf` | 2276 | `map-display.txt` | 21900 | listing kept with `.txt` extension |
| `map-display-group.elf` | 1076 | `map-display-group.txt` | 9490 | listing kept with `.txt` extension |
| `map-display-object.elf` | 3308 | `map-display-object.txt` | 31111 | listing kept with `.txt` extension |
| `map-flag-off.elf` | 980 | — | — | no listing retained |
| `map-grp-flag.elf` | 472 | — | — | no listing retained |
| `mapLoad.elf` | 3232 | `mapLoad.disasm` | 30291 |  |
| `map-main.elf` | 5688 | `map-main.txt` | 55474 | listing kept with `.txt` extension |
| `map-play.elf` | 812 | `map-play.txt` | 6456 | listing kept with `.txt` extension |
| `map-play-event.elf` | 316 | — | — | no listing retained |
| `marioCheckWall.elf` | 876 | `marioCheckWall.disasm` | 7012 |  |
| `marioChkFrontStep.elf` | 1728 | `marioChkFrontStep.disasm` | 15520 |  |
| `marioChkGnd2.elf` | 548 | `marioChkGnd2.disasm` | 3836 |  |
| `marioChkGnd.elf` | 3636 | `marioChkGnd.disasm` | 35593 |  |
| `marioChkLandon.elf` | 340 | `marioChkLandon.disasm` | 1539 |  |
| `marioChkWallAround.elf` | 2652 | `marioChkWallAround.disasm` | 25132 | byte-identical slice/listing also in `behavior/` (bodies equal; header path differs by 9 characters) |
| `marioFall.elf` | 1256 | `marioFall.disasm` | 11162 |  |
| `marioGetFallSpd.elf` | 340 | `marioGetFallSpd.disasm` | 1548 |  |
| `marioHitCheck2.elf` | 2548 | `marioHitCheck2.disasm` | 23647 |  |
| `marioHitCheck.elf` | 996 | `marioHitCheck.disasm` | 8006 |  |
| `mario-init.elf` | 1032 | `mario-init.txt` | 8849 | listing kept with `.txt` extension |
| `marioInit.elf` | 1352 | `marioInit.disasm` | 11724 |  |
| `marioJump.elf` | 936 | `marioJump.disasm` | 7691 |  |
| `marioLandOn.elf` | 3388 | `marioLandOn.disasm` | 33579 | slice byte-identical to `landOn.elf`; listings differ only in the embedded header path |
| `marioMain.elf` | 2744 | `marioMain.disasm` | 27039 |  |
| `marioMakeJumpPara.elf` | 312 | — | — | no listing retained |
| `marioMove.elf` | 3196 | `marioMove.disasm` | 31359 |  |
| `marioMoveMain.elf` | 1680 | `marioMoveMain.disasm` | 15940 |  |
| `marioNoUpdateCamPos.elf` | 236 | `marioNoUpdateCamPos.disasm` | 510 |  |
| `marioResetGravity.elf` | 248 | — | — | no listing retained |
| `marioSearchGround.elf` | 2832 | `marioSearchGround.disasm` | 26869 |  |
| `marioSetFallPara.elf` | 336 | — | — | no listing retained |
| `mario_width.elf` | 368 | `mario_width.disasm` | 1937 |  |
| `mot_bottomless.elf` | 1504 | `mot_bottomless.disasm` | 13313 |  |
| `mot_damage.elf` | 2236 | `mot_damage.disasm` | 20449 |  |
| `mot_dash.elf` | 1048 | `mot_dash.disasm` | 9281 |  |
| `mot_fall.elf` | 764 | `mot_fall.disasm` | 6079 |  |
| `mot_jump.elf` | 712 | `mot_jump.disasm` | 5454 |  |
| `mot_upstairs.elf` | 488 | `mot_upstairs.disasm` | 3080 |  |
| `mot_walk.elf` | 1040 | `mot_walk.disasm` | 9172 |  |
| `movementSpeed.elf` | 300 | `movementSpeed.disasm` | 1159 |  |
| `moveRate.elf` | 292 | `moveRate.disasm` | 1080 |  |
| `overhead.elf` | 1380 | `overhead.disasm` | 12597 |  |
| `peakY.elf` | 232 | `peakY.disasm` | 464 |  |
| `pouchAddHP.elf` | 272 | `pouchAddHP.disasm` | 885 | byte-identical slice/listing also in `behavior/` (bodies equal; header path differs by 9 characters) |
| `searchFrontWall.elf` | 2596 | `searchFrontWall.disasm` | 24366 |  |
| `searchHead.elf` | 1708 | `searchHead.disasm` | 15543 |  |
| `setJump.elf` | 684 | `setJump.disasm` | 5040 |  |
| `walkSpeed.elf` | 348 | `walkSpeed.disasm` | 1655 |  |

Top-level summary: 88 slices; 68 paired with a same-stem `.disasm`; 7 more
paired via `.txt` listings; 13 have no listing at top level (of those,
`hitGetAttr` is covered in `behavior/`; `landOn`/`marioLandOn` are the same
bytes under two names); 2 zero-byte `.disasm` placeholders exist without any
slice (`marioChkLand.disasm`, `mot_step.disasm`).

## `behavior/` slice corpus

As of the snapshot (2026-10-12 16:22 MDT; the corpus grows as research adds
slices — it grew from 527 to 539 `.elf` files during this verification alone.
Re-run the commands below to refresh):

| Measure | Value |
| --- | ---: |
| `.elf` slices | 539 |
| `.disasm` listings | 546 (every slice has one; 7 are zero-byte placeholders listed below) |
| `.annotated` listings | 16 |
| Evidence JSON files | 64 |
| Symbol CSV extracts | 4 |
| Decomp source copies | 4 `.c` + 1 `.h` |
| Slice size range | 200 – 8,088 bytes |

Zero-byte `.disasm` placeholders (failed inspections; kept, never rewritten —
see `docs/GEKKO_DISASSEMBLY.md`): `angleABf.disasm`, `_announceMessage.disasm`,
`BattleGetFirstAttackTarget.disasm`, `flyMain.disasm`, `partyGetOtherPtr.disasm`,
`partyGetTargetNum.disasm`, `PSVECdotProduct.disasm`. Note the top-level
`angleABf.disasm` is a complete 4,548-byte listing; only the `behavior/`
placeholder is empty.

Companion text in `behavior/` that explains the slices:

- `evidence.json` (408,261 B) — 359 named bodies with address/size/sha256
  fingerprints (`grep -c '"name"'` = 359) and branch-site data;
  `evidence-repeat.json` (399,919 B) is a separate capture of the same family
  and is **not** byte-identical. The `-repeat` companions of
  `us-usage.json`, `message-resources.json`, `harbor-boss-events.json`,
  `harbor-opening-events.json`, `ordinary-attack-events.json` and
  `hammer-wrapper-events.json` were `cmp`-verified byte-identical to their
  primaries.
- `us-usage.json` (18,131,051 B, with `us-usage-repeat.json` identical) — 80
  annotated action-command/usage records tying DOL addresses to names.- Topic files `field-partner-*-evidence.json`, `harbor-*.json`,
  `lecture-*.json`, `input-cadence-evidence.json`, etc. — per-subsystem
  fingerprints.
- `battle-message-symbol.csv`, `disp-sort-symbol.csv`, `printf-symbol.csv`,
  `scanf-parser-symbol.csv` — one-row extracts in the `us_symbols.csv` column
  format for specific functions (e.g. `_disp` in `battle_message.o`).
- `gekko-repair-report.json` — the list of listings that `tools/gekko_disasm.py`
  corrected (includes top-level and `behavior/` paths).
- `src_driver_seqdrv.c`, `src_manager_evtmgr.c`, `src_manager_evtmgr_cmd.c`,
  `src_mario_mariost.c`, `include_manager_evtmgr.h` — byte-identical copies of
  `doldecomp/ttyd` sources (`cmp`-verified against
  `decomp-62131fc…/src/…` and `…/include/…`); `LICENSE`/`README.md` are the
  decomp MIT license and README (JP `G8MJ01`).
- `upstream-revision.txt` — `62131fc31866f85a12e4817a8dc37f0c25eb5f8c`,
  the decomp commit that `decomp-tree.json` and the snapshot directory name pin.

## Cross-checks (verified 2026-10-12)

| Check | Result |
| --- | --- |
| `walkSpeed.elf` `.text` section (`readelf -S`): addr `80087d84`, size `0x9c` | `walkSpeed.disasm` starts `80087d84 <.text>:` with `stwu 1, -32(1)` |
| `us_symbols.csv` row `_main,1,000f5a3c,.text,text,800fb01c,000f801c,BattleCheckDamage,battle_damage.o,00000a00` | `behavior/BattleCheckDamage.disasm`/`.annotated` start at `800fb01c` |
| `us_symbols.csv` row `_main,1,00055200,.text,text,8005a7e0,000577e0,marioMain,mario.o,000009f8` | `marioMain.disasm` starts at `8005a7e0` |
| `us_symbols.csv` has no `walkSpeed` row (0 matches) | slice names are not exclusively from `us_symbols.csv`; some are local research names (e.g. `walkSpeed` is also registered in [UNADDRESSED_BINARY_FUNCTIONS.md](UNADDRESSED_BINARY_FUNCTIONS.md)) |
| `us_symbols.csv` row `_main,…,jon.o` names `bero_normal` as a `.data` `BeroInfo` object at `80c8a68c` | `bero_normal.elf` nonetheless contains only a `.text` section at `80079b74` (`readelf -S`); treat the slice name as the inspection label, and do not infer object kind from the slice alone |
| `behavior/evidence.json` fingerprints | entry addresses in `.disasm` listings equal the JSON decimal addresses (method and results also recorded in [UNADDRESSED_BINARY_FUNCTIONS.md](UNADDRESSED_BINARY_FUNCTIONS.md)) |
| Annotated vs plain listings | `diff` of the `BattleCheckDamage` pair differs only by ` # SymbolName` call comments (54 in the annotated file, 0 in the plain listing) |
| Duplicate names | `landOn.elf` ≡ `marioLandOn.elf` (identical bytes); `hitGetAttr.elf`, `marioChkWallAround.elf`, `pouchAddHP.elf` exist identically in both locations |

## Handling rules

1. Never execute the `.elf` slices (not runnable on the host anyway) and never
   link them into builds.
2. Prefer the text listings for analysis; the listings were repaired for
   GameCube paired-single decoding and are the project’s authoritative
   inspection text.
3. Empty placeholders indicate a failed inspection, not an empty function —
   regenerate with the documented tools rather than inventing content.
