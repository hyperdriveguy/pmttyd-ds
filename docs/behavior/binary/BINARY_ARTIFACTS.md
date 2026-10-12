# Binary Artifacts

Read-only identification of binary files under `artifacts/reference/`.
Inspection used `file`, `read`, `head`, and `os.walk` only. **No binary was
executed.** For the full tree census and the text records that explain these
binaries, see [REFERENCE_FILES_INVENTORY.md](REFERENCE_FILES_INVENTORY.md).

## GameCube function objects (PowerPC, static inspection only)

- Location: 88 `*.elf` at `artifacts/reference/` top level plus 520 `*.elf`
  in `artifacts/reference/behavior/`.
- `file` identity (verified on `map-main.elf`, `adjustMove.elf`,
  `behavior/BattleCalculateDamage.elf`): `ELF 32-bit MSB relocatable,
  PowerPC or cisco 4500, version 1 (SYSV), stripped`.
- These are stripped relocatable objects, not runnable programs. Behavior
  claims about them must come from their paired text:
  - `.disasm` (objdump: `<name>.elf: file format elf32-powerpc` +
    `Disassembly of section .text:` + address-labeled mnemonics).
  - `.annotated` (16 in `behavior/`): identical body plus ` # SymbolName`
    on `bl` lines (proven by `diff` on the `BattleCalculateDamage` pair,
    429 lines each).
  - Top-level `map-display.txt`, `map-display-group.txt`,
    `map-display-object.txt`, `map-display.txt`, `map-play.txt`,
    `mario-init.txt` are objdump text under `.txt` names; `map-main.txt`
    likewise.
- Pairing gaps (verified by census script): top-level 68 both / 20 `.elf`-only
  (`check_width`, `evt-map-flag`, `evt_fade_in`, `hitCheckAttr`, `hitEntry`,
  `hitGetAttr`, `hitGetDamageReturnPos`, `init_registers`, `map-display`,
  `map-display-group`, `map-display-object`, `map-flag-off`, `map-grp-flag`,
  `map-main`, `map-play`, `map-play-event`, `mario-init`,
  `marioMakeJumpPara`, `marioResetGravity`, `marioSetFallPara`) / 2
  `.disasm`-only (`marioChkLand`, `mot_step`); `behavior/` 520 both / 7
  `.disasm`-only (`BattleGetFirstAttackTarget`, `PSVECdotProduct`,
  `_announceMessage`, `angleABf`, `flyMain`, `partyGetOtherPtr`,
  `partyGetTargetNum`).

## Host executables (identified, not run)

- `hactool/hactool`: `ELF 64-bit LSB pie executable, x86-64 ... dynamically
  linked`, built from the vendored C sources (`hactool-build.log` opens
  `make -C mbedtls lib`). `README.md`: views/decrypts/extracts Switch
  content archives.
- `vgmstream/build/cli/vgmstream-cli`: `ELF 64-bit LSB pie executable,
  x86-64`, built from `vgmstream/src|cli` (`build/src/libvgmstream.a`
  beside it).
- Windows-only vendored binaries (never executed on this host): 22 `.exe`,
  79 `.dll`, 21 `.pdb` tree-wide excluding `.git`, under
  `citric-composer/*/bin|obj` and `vgmstream/ext_libs/`.

## Compiled intermediates and other non-text artifacts

- `*.o` (768) and `*.d` (673) excluding `.git`: build outputs of
  `hactool/` (incl. `mbedtls/`) and `vgmstream/build/`.
- `switch/*.asm` are AArch64 objdump text quoting
  `local-assets/switch/decoded/main.elf: file format elf64-littleaarch64`;
  that image is not in the repo.
- Images (`*.png` 151, `*.ico` 39) are reference artwork in
  `citric-composer/**/Data/Image` and `Unsimplifier/static/`.
- `*/.git/` contents (116 files) are version-control data, excluded.

## Handling rules

- Do not execute, modify, or redistribute any binary or the ISO-derived
  objects; keep ISO assets out of source history per project policy.
- Cite the explaining text (`.disasm`/`.annotated`/`.json`/`.csv`/`.c`)
  for every claim; see the inventory document for exact paths.
