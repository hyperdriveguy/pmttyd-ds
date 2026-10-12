# Switch binary baseline and reproduction

## Identity verified from supplied content

Source opened read-only:
`/home/carson/games/roms/Paper Mario The Thousand-Year Door [BASE][0100ECD018EBE000][US][v0].nsp`.

| Property | Value |
| --- | --- |
| Container | PFS0, 6 entries, 5,456,787,792 bytes |
| Whole-package SHA256 | `04fba23ab27de88276d2a04eaf69619cd09044acb41de62c1cb2f89a8de110e4` |
| CNMT application ID | `0100ecd018ebe000` |
| CNMT type/version | Application `0x80`, version `0` |
| NACP displayed version | `1.0.0` |
| NACP American English title | Paper Mario: The Thousand-Year Door |
| main NSO build ID | `78f37bb55d015be3b368ec22af595455f1544dc1000000000000000000000000` |
| Program NCA SDK version | `15.3.2.0` |

The filename is not the evidence for these identity claims. The CNMT and NACP
were decrypted from the archive. Its three content-record hashes and sizes match
the program/control/manual members in the independently hashed PFS0 inventory.
The package has no update content; research refers to base 1.0.0.

| Member | Type | Size (bytes) |
| --- | --- | ---: |
| `64e96ba1b0cb227919df497c0a206af3.nca` | Program | 5,455,298,560 |
| `303fd1bc6b03a2d09b0dd2d00cee0e03.nca` | Control | 1,272,320 |
| `f8bca7ac7e4b7759cd861fb4e09b2655.nca` | Manual | 210,432 |
| `727d05c31de821221e841efb0062a7ce.cnmt.nca` | Meta | 3,584 |

Other members are the certificate and ticket. Their payloads are not emitted by
the inventory tool. The supplied `prod.keys` could decrypt the NCA headers;
`title.keys` did not contain this rights ID. A private, isolated title-key mapping
was obtained from this package's common ticket. Original key files were unchanged.

`hactool -y` reported GOOD for the program fixed-key/NPDM signatures, both PFS0
hash tables, and all RomFS integrity levels, and for the corresponding metadata,
control and manual signatures/hashes. Logs are preserved locally. Hactool can
exit zero after reporting an error: check log contents and actual extracted
files rather than accepting its exit code alone. The initial attempt without
the matching ticket mapping reported corrupted sections; the corrected verified
extraction supersedes it.

## Files available for static recovery

RomFS contains **9,959 files**: 1,246 under `script/`, 1,582 files ending
`.elf.zst` across the tree, and 611 under `msg/`, including 47 in
`msg/US_English_Final/`. This is a file census, not a reachable-content census.
The Switch English files are MSBT resources, not the GameCube `msg/US/*.txt`
format. ExeFS has `main`, `main.npdm`, `rtld`, `sdk`, and `subsdk0`.

Selected data ELFs are little-endian ELF64 AArch64 relocatable objects. Their
`.text` sections can be empty: they contain runtime data, not necessarily
executable routines. Their symbol sizes and ABS64 relocations provide independent
checks of table boundaries and string associations.

Main decompressed NSO segments (module-relative): text starts `0`, rodata
`0xac2000`, data `0xe92000`. Do not subtract a GameCube load address. Exact segment
sizes, file offsets and reviewed function-range hashes are in `native.json`.
The generated `main.elf` is a section wrapper for disassembly, without a runnable
loader reconstruction or dynamic relocation application.

## Reproduction

Pinned public tools, downloaded as source and built locally where applicable:

- [hactool](https://github.com/SciresM/hactool/tree/1d64a83450e025622f3468c28fc4164dad2c5ef6),
  ISC, commit `1d64a83450e025622f3468c28fc4164dad2c5ef6`.
- [Unsimplifier](https://github.com/Darxoon/Unsimplifier/tree/e2bf56c22ae527560602a9b063bccb1bf03dac16),
  Unlicense, commit `e2bf56c22ae527560602a9b063bccb1bf03dac16`.
  `src/elf/fileTypes.ts` supplied layout leads; its field names remain annotations
  until a consumer is traced. The editor itself was not run against the originals.

```sh
python3 tools/switch_package_inventory.py --output artifacts/reference/switch/package.json
python3 tools/switch_table_evidence.py --output artifacts/reference/switch/tables.json
python3 tools/switch_nso_evidence.py --output artifacts/reference/switch/native.json --elf local-assets/switch/decoded/main.elf
llvm-objdump -d --start-address=0x779ff0 --stop-address=0x77a310 local-assets/switch/decoded/main.elf
```

The table reader requires `pyelftools` and `zstandard`. Input is extracted RomFS;
it does not decrypt the NSP. To recreate the extracts, use pinned hactool with
`-t pfs0 --outdir=local-assets/switch/nca`, then process the program member with
`--exefsdir=local-assets/switch/exefs --romfsdir=local-assets/switch/romfs`.
Supply the user's local keys privately via `-k` and an isolated title-key file;
use `--disablekeywarns --suppresskeys`. Do not put key values in command arguments,
logs, reports or version control. Use `-t nso --uncompressed=local-assets/switch/decoded/main.nso`
on the extracted `exefs/main` before the native-evidence tool.

`content.json` is the session census of every extracted file with SHA256 plus the
CNMT content cross-check. `package.json`, `tables.json`, and `native.json` have
committed generators. Original bytes, decompressed ELFs, public source snapshots
and disassembly stay ignored.

## Runtime boundary

Before the static-only instruction, an isolated Ryujinx 1.3.3 attempt stopped at
RYU-0002 (firmware missing), and the firmware installer later displayed RYU-0001
(keys not found). No game execution or gameplay observation was obtained.
The user supplied `/home/carson/games/roms/Firmware.23.0.1.zip`, but a successful
installation was not established. Static analysis does not require firmware.
No further emulator work is authorized by the current static-only instruction;
the user's normal profile and saves were not modified.
