# Tool and Asset Reference Trees (`artifacts/reference/` subdirectories)

Evidence-based documentation of the vendored tool repositories, host builds,
Windows binaries and Switch-extraction metadata under `artifacts/reference/`.
Verified 2026-10-12 with `find`, `file(1)`, `stat`, `git log`/`rev-parse`
(read-only), `cmp` and `grep`. **No binary was executed.** Companion
documents: [master inventory](REFERENCE_FILES_INVENTORY.md),
[DOL slices](REFERENCE_DOL_FUNCTION_SLICES.md),
[binary artifacts](BINARY_ARTIFACTS.md).

## Pinned clones (shallow, with remotes)

Five trees are shallow git clones; each has 29–30 `.git` files (packed clone).
`git rev-parse HEAD` / `git remote get-url origin` (read-only):

| Tree | Commit | Remote | License file |
| --- | --- | --- | --- |
| `vgmstream/` | `7c520e8f9b078dfde2f84f1b5bcbad0c6c415806` (“Merge pull request #2040 from bnnm/atrac9”) | `https://github.com/vgmstream/vgmstream.git` | `vgmstream/COPYING` (ISC-style; top-level copy `vgmstream-COPYING`) |
| `citric-composer/` | `73439816c9f2cd6f43ab9aa46f89247be8e32942` (“Merge pull request #6 from clarith-re/fix_crash_with_opus_stream”) | `https://github.com/Gota7/Citric-Composer.git` | none in snapshot; README credits Gota7 |
| `hactool/` | `1d64a83450e025622f3468c28fc4164dad2c5ef6` (“kernel .crt0 in .rodata reacts only…”) | `https://github.com/SciresM/hactool.git` | `hactool/LICENSE` (ISC) |
| `Unsimplifier/` | `e2bf56c22ae527560602a9b063bccb1bf03dac16` (“Compute sortedIDs in data_param_field…”) | `https://github.com/Darxoon/Unsimplifier.git` | `Unsimplifier/LICENSE` (Unlicense) |
| `switch-libpulsar/` | `3d3a057f4aa2578cdb89cab3596967c47de73b73` (“Loop support”) | `https://github.com/p-sam/switch-libpulsar.git` | `switch-libpulsar/LICENSE.md` (MIT, © 2020 p-sam) |

`docs/ASSETS.md` records vgmstream as “r2117, commit `7c520e8…`”, matching the
clone above.

## `vgmstream/` (2,564 files excl. `.git`, 47,299,931 B at snapshot; 2,573 in an earlier pass)

Game-audio decoding library used for format research and the project’s WAV/STM
conversion work.

| Part | Files | Contents |
| --- | ---: | --- |
| `src/` | 846 | codec/meta/layout/base/util C sources (`formats.c`, `meta/`, `coding/`, `layout/`, `base/`) |
| `build/` | 1,401 | host CMake build tree:677 ELF64 objects/executables, `CMakeCache.txt`, Makefiles |
| `ext_libs/` | 54 | prebuilt Windows DLLs + `.def` export lists + `licenses/` + `extra/` build scripts |
| `ext_includes/` | 141 | third-party headers (incl. ffmpeg) |
| `doc/` | 9 | `FORMATS.md`, `TXTH.md`, `TXTP.md`, `USAGE.md`, `BUILD*.md`, `DEV.md` |
| `cli/`, `fb2k/`, `winamp/`, `xmplay/`, `audacious/`, `cmake/` | 87 | player front-ends and build plumbing |
| `.github/` | 9 | issue templates and CI workflows of the upstream repo |
| top-level files | 26 | `CMakeLists.txt`, `Makefile`, `configure.ac`, `README.md`, `SECURITY.md`, `COPYING`, `version.h`, MSVC scripts/solution, `bootstrap`/`unbootstrap` helper scripts |

Binaries and their explaining text:

- `build/cli/vgmstream-cli` — `ELF 64-bit LSB pie executable, x86-64 … not
  stripped`; built by this tree’s CMake (`build/CMakeCache.txt`:
  `CMAKE_C_COMPILER=/usr/bin/cc`, `CMAKE_CXX_COMPILER=/usr/bin/c++`,
  `CMAKE_HOME_DIRECTORY=…/artifacts/reference/vgmstream`). Explained by
  `cli/` sources, `src/`, and `doc/BUILD.md`.
- `build/src/CMakeFiles/libvgmstream.dir/**.o` (672) and CMake probes —
  object files mirroring `src/` one-to-one.
- `ext_libs/*.dll` and `ext_libs/dll-x64/*.dll` (20 PE32/PE32+ DLLs: celt
  0.6.1/0.11.0, g719, mpg123, speex, vorbis, swresample, avcodec/avformat/avutil
  vgmstream builds) — explained by matching `*.def` export lists,
  `ext_libs/Makefile`, `ext_libs/licenses/*` (celt, libogg, libvorbis, mpg123,
  speex, opus, ffmpeg LGPLv2.1/LGPLv3 texts) and `ext_libs/extra/`
  (`ffmpeg_options.txt`, `ffmpeg_extra.sh`, `ffmpeg-revert-bcrypt-random.patch`,
  `media-autobuild_suite.ini`, `libg719_decode/`).

## `hactool/` (1,018 files excl. `.git`, 28,522,219 B at snapshot; 1,021 in an earlier pass)

Nintendo Switch content-decryption tool, vendored with its crypto dependency
and built on this host (build only; the binary is never run by tooling here).

- Sources: `main.c`, `nca.c`, `nso.c`, `pfs0.c`, `hfs0.c`, `xci.c`, `romfs.c`,
  `aes.c`, `rsa.c`, `extkeys.c`, … plus `cJSON.c`, `ConvertUTF.c`, `lz4.c`.
- Build evidence: `config.mk` (`CC = gcc`, `-O2 … -std=gnu11 -fPIC`,
  `-lmbedtls -lmbedx509 -lmbedcrypto`); `hactool-build.log` (22,338 B UTF-8)
  records the full transcript, ending in
  `gcc -o hactool save.o sha.o aes.o … -L ./mbedtls/library -lmbedtls …`.
- Build products:24 ELF64 x86-64 relocatable `.o` files and `hactool`
  (`ELF 64-bit LSB pie executable … with debug_info, not stripped`).
- `mbedtls/` vendored copy (938 files): `library/` (72 ELF64 objects),
  headers, and `tests/data_files/` with353 cryptographic test vectors
  (185 PEM certificates,22 certificate requests,34 PEM RSA private keys,
  34 OpenSSH keys,78 other DER/format blobs). These are upstream test files,
  not project secrets — but the `*key*` files must still never be copied into
  docs, tests or source history.
- `KEYS.md` documents the key *names* hactool recognizes (no key values).

## `citric-composer/` (821 files excl. `.git`, 341,424,645 B)

Gota7’s Citric-Composer repository snapshot (“Editor for 3ds, Wii U, and
Switch sound files”, per `citric-composer/README.md`). Largest tree; almost
all bytes are .NET build/package payloads.

| Part | Contents |
| --- | --- |
| `Citric Composer/` | `.sln` + `Citric Composer/` sources, `packages/` NuGet (NAudio1.8.4, CSCore1.2.1.2, VGAudio2.2.1, Fody3.3.5/Costura3.3.2, Syroot.IO.BinaryData), `bin/Release/` (`Citric Composer.exe`, `Citric Updater.exe`, `Solar Audio Slayer.exe`, `Citric Composer.zip`, `version.bin`), `Citric Installer.msi` (Composite Document; metadata title “Citric Composer Installer”, author Gota7), `SequenceDataLib/`, `SolarFileLoader/` debug builds |
| `Koopa Harmony/` | BFSAR player app (`Koopa Harmony.exe`, Mono/.NET PE32) plus `Data/Tools/` and `bin/Debug/Data/Tools/` copies of Nintendo SDK sound tools (`dspadpcm.exe`, `dsptool.dll`, `WavConvCafe.exe`, `SoundFoundation*.dll`, `SoundRuntimeCafe.dll`, `WaveCodecCafe.dll`, `ToolDevelopmentKit.dll`, `msvcp110.dll`/`msvcr110.dll`) — third-party binaries with no source here; use for format facts only |
| `Retsuko Sound Tool/` | `.sln` + `bin/Debug` build |
| `docs/` | `_config.yml`, `index.md`, `specs/`:8 format specifications (`seq.md`, `seqData.md`, `war.md`, `wav.md`, `binaryWav.md`, `bnk.md`, `grp.md`, `common.md`) describing BFSAR-family containers (e.g. `seq.md`: “Sequences (.bfseq, .bcseq) … FileHeader (Magic: FSEQ or CSEQ)”) |

Binary census:81 PE32/PE32+ files,19 MSVC program databases, plus PNG
icons/docs images. Explaining text: project sources, `packages/` metadata, MSI
fields and `docs/specs/`.

## `Unsimplifier/` (93 files excl. `.git`, 692,635 B)

Darxoon’s OrigamiWand/Unsimplifier — “A Data Editor for the Paper Mario: The
Thousand Year Door Remake” (README). SvelteKit/Vite/TypeScript application:

- `src/elf/` — TypeScript ELF parsing/serialization modules
  (`elfBinary.ts`, `parser.ts`, `serializer.ts`, `strip.ts`, …).
- `src/routes/`, `src/lib/` — application UI.
- `static/` — `Unsimplifier_icon.png`, `favicon.png` (the2 non-citric PNGs in
  the whole reference tree).
- `.env` — only `PUBLIC_OW_VERSION`, `PUBLIC_VERSION_TIMESTAMP`,
  `PUBLIC_IS_DEV_VERSION`, `PUBLIC_DEBUG`, `PUBLIC_TRACE` flags (values
  redacted here; no secrets).
- No build products and no game data in the snapshot.

## `switch-libpulsar/` (69 files excl. `.git`, 133,082 B)

`p-sam/switch-libpulsar` — “Switch homebrew library to load, parse, and play
sounds from BFSAR files and related file formats” (README). Pure source:
`include/pulsar/`, `src/` (`archive`, `bfgrp`, `bfsar`, `bfstm`, `bfwar`,
`bfwav`, `bfwsd`, `player`), `docs/` (Doxygen), `examples/`, `Makefile`,
`deps.mk`. No binaries; `docs/ASSETS.md` records it supplied format facts only.

## `decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c/` (343 files, 9,691,833 B)

Snapshot of the `doldecomp/ttyd` decompilation (“Paper Mario: The Thousand-Year
Door”, README) at the commit embedded in the directory name; the same hash is
in `decomp-tree.json` (the GitHub tree JSON for that commit) and
`behavior/upstream-revision.txt`. Not a git clone (no `.git`).

- `configure.py`, `config/G8MJ01/` (JP rev-0 configuration), `src/`,
  `include/`, `rels/`, `LICENSE` (MIT, © 2019-2025 NWPlayer123), `README.md`.
- Used as the provenance reference for byte-identical copies elsewhere:
  `mapdrv.c/.h`, `mariost.c/.h` (top level) and
  `behavior/src_driver_seqdrv.c`, `behavior/src_manager_evtmgr.c`,
  `behavior/src_manager_evtmgr_cmd.c`, `behavior/src_mario_mariost.c`,
  `behavior/include_manager_evtmgr.h` — all `cmp`-verified identical to their
  `decomp-62131…` sources. `behavior/LICENSE` is byte-identical to the decomp
  `LICENSE`; `behavior/README.md` is the decomp README.
- The top-level `evt_cmd.h`/`evt_bero.h` are **not** from this tree (they differ
  by585/226 diff lines); their provenance is `jdaster64/ttyd-tot`
  `ttyd-tools/rel/include/…`, recorded in `tot-tree.json` and `docs/ASSETS.md`.

## `switch/` (18 files, 8,312,218 B)

Metadata and inspection outputs for the TTYD Remake (Switch, US v0,
title `0100ecd018ebe000`). The NSP/NCA payload itself is **not** here — it
stays in the user’s local extraction (`local-assets/switch/…`, per
`docs/ASSETS.md`).

| File(s) | Bytes | Role (verified content) |
| --- | ---: | --- |
| `package.json` | 1,462 | PFS0 metadata for the source NSP (path, size5,456,787,792, sha256, per-entry offsets/hashes: `.cert`, `.tik`, three `.nca`) |
| `content.json` | 1,689,224 | CNMT content list (`title_id` `0100ecd018ebe000`, per-NCA sha256/size/type) |
| `pfs-extract.log` | 1,439 | hactool PFS0 extraction report (6 files listed with ranges; notes “prod.keys does not exist” for that run) |
| `program-info.log`, `program-verify.log`, `program-extract.log` | 5,143 / 15,107 / 730,501 | NCA/program section verification and extraction reports (IVFC level checks) |
| `meta-info.log`, `control-info.log`, `other-info.log` | 3,117 / 5,034 / 5,248 | further NCA verification logs |
| `main-nso.log` | 343 | NSO0 header: build id `78F37BB55D015BE3B368EC22AF595455F1544DC1…`, sections `.text` `00000000-00ac2000`, `.rodata` `00ac2000-00e92000`, `.rwdata` `00e92000-00f1f388`, `.bss` `00f1f388-02618000` |
| `native.json`, `native-repeat.json` | 1,131 each (byte-identical, `cmp`-verified) | decoded main segment table (same build id, `.text`/`.rodata`/`.data` offsets and sizes) plus labeled range fingerprints (`defence_parameter_query`, `counter_parameter_query`, `defence_update_prefix_partial`) with sha256 |
| `tables.json`, `tables-repeat.json` | 2,904,665 each (byte-identical, `cmp`-verified) | battle data tables decoded from `data/battle/data_Parameter.elf.zst` (packed/ELF sha256, symbol `_ZN3wld3btl4data6s_DataE`, stride24, per-row string tables with section/offsets, e.g. `BINGO_SLOT_1ST_INIT_VAL`) |
| `defence-check.asm`, `defence-consumer.asm`, `guard-runtime.asm` | 19,506 / 12,029 / 12,405 | AArch64 listings in llvm-objdump format; header path `local-assets/switch/decoded/main.elf:	file format elf64-littleaarch64` |
| `config/switch/title.keys` | 68 | isolated hactool title-key mapping for the extraction. **Secret: never reproduce, print, diff or commit its contents** (`docs/ASSETS.md`: “Keys are never published”) |

## Handling rules

1. Never execute any binary in these trees (host executables, `.dll`, `.exe`,
   `.msi`, or the GameCube slices documented separately).
2. Cite pinned commit hashes when recording format facts learned from these
   trees; `docs/ASSETS.md` is the licensing ledger.
3. Key material stays local: `switch/config/switch/title.keys` and
   `hactool/mbedtls/tests/data_files/*key*` are referenced by path only.
4. Third-party SDK tool DLLs under Koopa Harmony are read-only reference;
   no implementation may be copied (project rule, `docs/ASSETS.md`).

## Verification performed for this document

- `git rev-parse HEAD`, `git remote get-url origin`, `git log --oneline -1`
  for each clone (read-only).
- `file(1)` on every binary class cited (ELF64 executables/objects, PE32/PE32+
  DLLs, MSI, PNG).
- `stat`/`find` for every count and byte size quoted.
- `cmp` for all byte-identical claims (`native.json`/`native-repeat.json`,
  `tables.json`/`tables-repeat.json`, decomp source copies, `behavior/LICENSE`).
- `readelf` header/section inspection only, never execution.
- Redaction check: no bytes from `title.keys` or mbedtls key files appear in
  this document.
