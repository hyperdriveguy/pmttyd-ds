# Reference Files Inventory (`artifacts/reference/`)

Verified inventory of every file and directory under `artifacts/reference/`,
identifying the binary files, the reference text that explains them, and the
provenance recorded for each tree. All facts below were re-checked against the
files on 2026-10-12 with `find`, `file(1)`, `stat`, `readelf` (headers only),
`cmp` and `grep`. **No binary was executed.** The `artifacts/` directory is
git-ignored (`.gitignore` line 3: `artifacts/`), matching the project rule that
extracted/inspection material is never committed or distributed.

Companion documents (all in this `binary/` folder):

- [DOL function slices](REFERENCE_DOL_FUNCTION_SLICES.md) — the `.elf`/`.disasm`
  corpus, pairing table and symbol cross-checks.
- [Tool and asset trees](REFERENCE_TOOL_TREES.md) — vendored tool repositories,
  host builds, Windows binaries and the Switch extraction metadata.
- [Binary artifacts](BINARY_ARTIFACTS.md) — per-binary `file(1)` identification
  notes maintained alongside this inventory.
- [Unaddressed binary functions](UNADDRESSED_BINARY_FUNCTIONS.md) — register of
  slice names not yet covered by behavior contracts.

## Grand totals

Counts use the filter `-not -path '*/.git/*'` unless stated otherwise. A
parallel research session was actively adding DOL slices to `behavior/` and
curating other trees while this inventory was verified, so all counts are a
timestamped snapshot and some fluctuated between passes (`behavior/` grew from
527 to 539 `.elf` slices within the hour; `.git` internals moved between 146
and 160; `mbedtls` test vectors were pruned mid-run). Refresh with
`find`/`stat` before relying on any count.

| Measure | Value (snapshot 2026-10-12 16:22 MDT) |
| --- | ---: |
| Top-level regular files | 186 *(stable across every pass)* |
| Top-level directories | 8 |
| All files, excluding `*/.git/*` | 6,288 |
| All files, including `.git` internals | 6,448 |
| Total file bytes, excluding `.git` | 503,447,467 |
| Top-level file bytes | 12,394,064 |
| Empty files (excl. `.git`) | 35 |
| `.git` internals (five shallow clones) | 160 |
| Apparent tree size (`du -sh`, block-rounded) | 697M |

## Directory census

Per-directory file counts exclude each tree’s `.git/` internals; all values
come from the snapshot named above.

| Directory | Files (excl. `.git`) | Bytes | What it holds |
| --- | ---: | ---: | --- |
| `behavior/` | 1,177 *(grows)* | 54,963,032 | Per-function DOL slices/listings (`.elf`/`.disasm`/`.annotated`), derived evidence JSON, decomp source copies, symbol extracts |
| `citric-composer/` | 821 | 341,424,645 | Citric Composer / Koopa Harmony / Retsuko Sound Tool repository snapshot with .NET build outputs, NuGet packages, MSI installer, bundled SDK tool DLLs, format specs |
| `decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c/` | 343 | 9,691,833 | Snapshot of `doldecomp/ttyd` at commit `62131fc…` (JP `G8MJ01` config; MIT) |
| `hactool/` | 1,018 | 28,522,219 | SciresM hactool source plus a host build (24 `.o`, `hactool` executable) and vendored `mbedtls/` (incl. test key material) |
| `switch/` | 18 | 8,312,218 | hactool extraction metadata, integrity logs, AArch64 listings and derived battle-table JSON for the TTYD Remake (Switch, US v0) |
| `switch-libpulsar/` | 68 | 131,874 | `p-sam/switch-libpulsar` source snapshot (BFSAR sound library, MIT); no build products |
| `Unsimplifier/` | 92 | 691,626 | Darxoon “OrigamiWand/Unsimplifier” Svelte/Vite source snapshot (TTYD Remake data editor, Unlicense) |
| `vgmstream/` | 2,564 | 47,299,931 | vgmstream git clone (commit `7c520e8`, r2117) including a host CMake build tree and prebuilt Windows DLLs |
| *(top-level files)* | 186 | 12,394,064 | DOL slices/listings, symbol tables, provenance manifests, reference source files, build log, licenses |

Five trees are shallow git clones (29–30 `.git` files each; 146–160 files
total observed across passes): `vgmstream`, `citric-composer`, `hactool`,
`Unsimplifier`, `switch-libpulsar`. Pinned commits and remotes are listed in
the [tool-trees document](REFERENCE_TOOL_TREES.md).

## Top-level files by category (186 files)

| Category | Count | Files |
| --- | ---: | --- |
| DOL function slices (`.elf`, ELF32 MSB PowerPC relocatable, stripped) | 88 | see [slice inventory](REFERENCE_DOL_FUNCTION_SLICES.md) |
| llvm-objdump listings (`.disasm`) | 70 | 68 paired with a same-stem `.elf`; 2 are zero-byte placeholders (`marioChkLand.disasm`, `mot_step.disasm`) |
| Listings kept with `.txt` extension | 6 | `map-display.txt`, `map-display-group.txt`, `map-display-object.txt`, `map-main.txt`, `map-play.txt`, `mario-init.txt` (each is the listing of the same-stem `.elf`) |
| Event-script extraction (not a listing) | 1 | `map-init-calls.txt` — per-map lines of `map offset evt_command [args]`, e.g. `gor_00_init_evt 0x2f31c evt_map_playanim ['nami', '0x1', '0x0']` |
| Symbol table | 1 | `us_symbols.csv` (10,660,932 B; 98,403 lines incl. header) |
| GitHub provenance manifests (JSON) | 4 | `files.json`, `musyx-files.json`, `decomp-tree.json`, `tot-tree.json` |
| Reference source files (C/C++/headers) | 9 | `mapdrv.c/.h`, `mariost.c/.h` (byte-identical to `doldecomp/ttyd`), `evt_cmd.h`, `evt_bero.h` (from `jdaster64/ttyd-tot` `ttyd-tools/rel/include`, differ from decomp headers), `dsp.c`, `dsp_decoder.c` (byte-identical to vgmstream sources), `citro2d-base.c` (citro2d library source) |
| Reference TypeScript (noclip.website) | 3 | `world.ts`, `AnimGroup.ts`, `ttyd-render.ts` |
| Python tool source | 1 | `MusyXExtract.py` (Nisto/musyx-extract, CRLF) |
| Build log | 1 | `hactool-build.log` (UTF-8; documents the hactool/mbedtls host build) |
| License text | 2 | `noclip-LICENSE`, `vgmstream-COPYING` |

### `us_symbols.csv`

Header: `area,sec_id,sec_offset,sec_name,sec_type,ram_addr,file_addr,name,namespace,size,align,type,value`.
Rows cover the DOL (`_main`, 36,907 rows) and REL areas named after maps
(`las` 5,472, `gor` 5,291, `jon` 5,007, `tou2` 4,490, `mri` 4,208, `aji` 3,800,
`muj` 3,592, …). Provenance is recorded in `docs/ASSETS.md`: downloaded from
`https://github.com/jdaster64/ttyd-utils/blob/master/resources/us_symbols.csv`
(GPL-3.0 upstream; not redistributed in source history). This is the `--symbols`
input named in `docs/GEKKO_DISASSEMBLY.md` for creating the `.elf` slices, and
listing entry addresses match its `ram_addr` column (examples verified in the
[slice inventory](REFERENCE_DOL_FUNCTION_SLICES.md)).

### Provenance manifests

| File | Bytes | Contents (verified) |
| --- | ---: | --- |
| `files.json` | 7,961 | 8 GitHub API entries for `magcius/noclip.website` `src/PaperMarioTTYD/` (`AnimGroup.ts`, `REL.ts`, `Scenes_PaperMarioTTYD.ts`, `Scenes_SuperPaperMario.ts`, `evt.ts`, `render.ts`, `tpl.ts`, `world.ts`) with sha/size/URLs |
| `musyx-files.json` | 1,742 | 2 GitHub API entries for `Nisto/musyx-extract` (`MusyXExtract.py`, `README.md`) |
| `decomp-tree.json` | 268,007 | Git tree of `doldecomp/ttyd` at `62131fc31866f85a12e4817a8dc37f0c25eb5f8c`; the snapshot directory name embeds this commit and `behavior/upstream-revision.txt` contains the same hash |
| `tot-tree.json` | 132,481 | Git tree of `jdaster64/ttyd-tot` at `245473f7d4574c75f51b5fd1b7ea0af1ea9a7465`; contains `ttyd-tools/rel/include/evt_cmd.h` and `ttyd-tools/rel/include/ttyd/evt_bero.h`, the provenance of the top-level headers |

Note: an earlier draft of this inventory claimed `files.json` listed “1001 .c
files”; the file actually contains 8 entries. The corrected census above is from
`grep -c '"name"' files.json`.

## Binary census (magic-verified, `file(1)`)

| Class | Count | Where |
| --- | ---: | --- |
| ELF 32-bit MSB relocatable, PowerPC, stripped (game DOL slices) | 627 *(grows)* | 88 top-level + 539 in `behavior/` |
| ELF 64-bit LSB relocatable, x86-64 (host object files) | 768 | `vgmstream/build/` (672), `hactool/mbedtls/library/` (72), `hactool/` root (24 `.o`) |
| ELF 64-bit LSB pie executable, x86-64 | 6 | `hactool/hactool`, `vgmstream/build/cli/vgmstream-cli`, 4 CMake compiler-probe binaries under `vgmstream/build/CMakeFiles/4.4.4/` |
| PE32 / PE32+ Windows DLLs and executables | 101 | 81 in `citric-composer/` (NuGet libs, .NET build outputs, bundled SDK tool DLLs), 20 in `vgmstream/ext_libs/` (+`dll-x64/`) |
| Microsoft Installer (Composite Document) | 1 | `citric-composer/Citric Composer/Citric Installer.msi` (title “Citric Composer Installer”, author Gota7) |
| MSVC program databases | 19 | `citric-composer/` build outputs |
| PNG images | 156 | 154 in `citric-composer/` (UI/docs assets), 2 in `Unsimplifier/static/` (icon, favicon) |
| Cryptographic test vectors | 394 files at latest count | `hactool/mbedtls/tests/data_files/`: 185 PEM certificates, 22 certificate requests, 34 PEM RSA private keys, 23 OpenSSH private keys and 27 unclassified “data” blobs (DER/formats) at the latest pass; earlier passes saw 34 OpenSSH keys and 78 data blobs before concurrent curation pruned the directory. Upstream mbedtls test material, not project secrets |
| Git internals (packs, index, refs, hooks) | 146–160 (snapshot: 160) | 29–30 files in each of the five shallow clones listed above |
| Other “data” blobs (DER/keys/formats) | 78 | `hactool/mbedtls/tests/data_files/` |

No game payload (ISO, DOL, NSP, NCA, RomFS) is present anywhere under
`artifacts/reference/`; the game binaries live in the user’s read-only ISO and
local `local-assets/` extraction, per `docs/ASSETS.md`.

## Which reference file explains which binary

| Binary | Explaining reference files (verified relationship) |
| --- | --- |
| GameCube DOL slices (`*.elf`, ELF32 PowerPC) | Same-stem `.disasm`/`.txt` llvm-objdump listing (byte words + addresses); `.annotated` variants add resolved call-target names; `us_symbols.csv` supplies name/section/RAM/file addresses; `behavior/evidence.json`, `us-usage.json` and topic `*-evidence.json` carry address/size/sha256 fingerprints that match listing entry addresses; `tools/inspect_dol_function.py` + `tools/gekko_disasm.py` (repo `tools/`) document how slices and corrected listings are produced from the read-only ISO |
| `hactool/hactool` + `hactool/*.o` | `hactool/*.c/.h` sources, `hactool/Makefile`, `hactool/config.mk` (`CC = gcc`, mbedtls link flags), `hactool-build.log` (full build transcript ending in the `gcc -o hactool …` link line), `hactool/LICENSE` (ISC), vendored `hactool/mbedtls/` |
| `vgmstream/build/` object files + `vgmstream-cli` | `vgmstream/src/**` sources, `vgmstream/build/CMakeCache.txt` (compilers `/usr/bin/cc`, `/usr/bin/c++`; `CMAKE_HOME_DIRECTORY` = this vgmstream tree), `vgmstream/CMakeLists.txt`, `vgmstream/README.md`, `vgmstream/COPYING` (ISC-style; also `vgmstream-COPYING` at top level) |
| `vgmstream/ext_libs/*.dll` (+`dll-x64/`) | Matching `*.def` export lists, `vgmstream/ext_libs/Makefile`, `vgmstream/ext_libs/licenses/*` (celt, ogg, vorbis, mpg123, speex, opus, ffmpeg LGPL), `vgmstream/ext_libs/extra/` build scripts (`ffmpeg_options.txt`, `ffmpeg_extra.sh`, patch) |
| `citric-composer/**` Windows binaries (`.exe`, `.dll`, `.msi`, `.pdb`) | Corresponding `.sln`/`.csproj` project trees, `packages/` NuGet directories (NAudio, CSCore, VGAudio, Fody/Costura, Syroot), `citric-composer/README.md`, `citric-composer/docs/specs/*.md` (BFSAR-family format specs: seq/war/wav/bnk/grp/common), MSI metadata fields |
| `citric-composer/Koopa Harmony/.../Data/Tools/*` (Nintendo SDK sound tool DLLs/EXEs: `dspadpcm.exe`, `SoundFoundation*.dll`, `WaveCodecCafe.dll`, …) | Bundled under the Koopa Harmony application tree; no source in this snapshot — treat as third-party binaries, documented format facts only (project rule in `docs/ASSETS.md`: “no implementation copied”) |
| Switch remake `switch/*.asm` AArch64 listings | llvm-objdump listings of `local-assets/switch/decoded/main.elf` (path embedded in the listing header); `switch/main-nso.log` (NSO0 build id `78F37BB5…`, section ranges) and `switch/native.json` (same build id, segment table, labeled range sha256s) anchor them |
| `switch/tables.json` | Battle data tables extracted from `data/battle/data_Parameter.elf.zst` (packed/elf sha256, symbol `_ZN3wld3btl4data6s_DataE`, per-row strings with section/offsets) |
| `switch/*.log`, `switch/package.json`, `switch/content.json` | hactool integrity/extraction reports for the US v0 NSP (PFS0 entries, NCA/IVFC verification); `switch/package.json` records the source NSP path/size/sha256 (metadata only, no payload) |
| `switch/config/switch/title.keys` | 68-byte isolated title-key mapping for the extraction; **contents are a secret — never reproduced in documentation, commits or tests** (`docs/ASSETS.md`: “Keys are never published”) |
| `decomp-62131…/` object files (none — source only) | `configure.py`, `config/G8MJ01/`, `src/`, `include/`, `rels/`, `README.md`, `LICENSE` (MIT, NWPlayer123); byte-identical copies of `mapdrv.c/.h`, `mariost.c/.h` and `behavior/src_*.c` were diff-verified against this tree |
| `vgmstream` slices `dsp.c`, `dsp_decoder.c` (top level) | Byte-identical to `vgmstream/src/meta/ngc_dsp_std.c` and `vgmstream/src/coding/ngc_dsp_decoder.c` (`cmp` verified) |
| noclip TypeScript (`world.ts`, `AnimGroup.ts`, `ttyd-render.ts`) | `files.json` entries (name/path/sha/size/URLs into `magcius/noclip.website`), `noclip-LICENSE` (reverse-engineering notice + MIT) |

## Licensing / provenance ledger (as recorded in-tree and in `docs/ASSETS.md`)

| Tree / file | License evidence in tree | Notes |
| --- | --- | --- |
| `noclip-LICENSE` + `world.ts`/`AnimGroup.ts`/`ttyd-render.ts` | Notice file (MIT for re-interpretations) | `files.json` pins GitHub paths/SHAs |
| `us_symbols.csv` | Upstream GPL-3.0 (`docs/ASSETS.md`) | Read-only reference; not redistributed |
| `decomp-62131…/`, `behavior/LICENSE`, `behavior/README.md` | MIT, © 2019-2025 NWPlayer123 | `behavior/LICENSE` is byte-identical to the decomp `LICENSE`; `behavior/README.md` is the decomp README (JP `G8MJ01` support note) |
| `evt_cmd.h`, `evt_bero.h` | From `jdaster64/ttyd-tot` (per `docs/ASSETS.md`, `docs/PROGRESS.md`) | Differ from decomp headers (585/226 diff lines); `tot-tree.json` contains the source paths |
| `vgmstream/`, `vgmstream-COPYING`, `dsp.c`, `dsp_decoder.c` | ISC-style copyright notice | Commit `7c520e8` = r2117 per `docs/ASSETS.md` |
| `hactool/` | ISC (`hactool/LICENSE`) | Host build artifacts are local products of this snapshot |
| `citric-composer/` | Upstream project of Gota7 (README link) | Format facts only; SDK tool DLLs are third-party |
| `Unsimplifier/` | Unlicense (`Unsimplifier/LICENSE`) | Public-domain notice |
| `switch-libpulsar/` | MIT, © 2020 p-sam | Source snapshot; no binaries |
| `MusyXExtract.py` | Unlicensed upstream (Nisto); “read for format facts, not incorporated” per `docs/ASSETS.md` | |

## Handling rules (enforced by this inventory)

1. **Never execute** anything in `artifacts/reference/` — that includes the
   hactool and vgmstream-cli host executables, all `.dll`/`.exe`, and the
   GameCube `.elf` slices (which are not runnable host objects anyway).
2. Inspect with read-only tools (`file`, `readelf`, `objdump`-style listings
   already stored as text, `grep`, `cmp`).
3. Never copy key material (`switch/config/switch/title.keys`,
   `hactool/mbedtls/tests/data_files/*key*`) into documentation, tests or
   source history.
4. Cite file paths and exact values; provisional interpretation belongs in the
   behavior documents, not in this inventory.

## Verification performed for this document

- `find`/`stat` for every count and byte size quoted (re-run 2026-10-12).
- `file(1)` magic classification for every binary class in the census, run
  over all non-`.git` files in several passes (6,276 / 6,287 / 6,288 files);
  stable classes (ELF64, PE, MSI, pdb, PNG) were identical in every pass,
  while the DOL-slice and mbedtls-test-vector counts moved with the live
  corpus as documented above.
- `readelf -h`/`readelf -S` on sampled slices (`walkSpeed.elf`,
  `bero_normal.elf`) — header/section inspection only.
- `cmp` for all claimed byte-identical source copies (decomp and vgmstream
  pairs listed above).
- `grep` cross-checks of listing entry addresses against `us_symbols.csv`
  (`BattleCheckDamage` → `800fb01c`, `marioMain` → `8005a7e0`) and against
  `behavior/evidence.json`-family fingerprints (see the
  [slice inventory](REFERENCE_DOL_FUNCTION_SLICES.md) and
  [unaddressed-functions register](UNADDRESSED_BINARY_FUNCTIONS.md)).
- Redaction check: no key bytes from `title.keys` appear in this document.
