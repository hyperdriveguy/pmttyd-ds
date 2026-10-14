# Native checkpoint byte layout

[save.cpp](../../source/runtime/save.cpp) and [save.hpp](../../include/runtime/save.hpp)
implement PM2D checkpoints. This guide describes current v1..9 code; the older
[v4 behavior note](../behavior/format/NATIVE_SAVE_FORMAT.md) describes that
historical extension. These records are the port's format, not original card
records. Original persistence remains incomplete.

All words are explicitly little-endian. X/Z use copied IEEE binary32 bits,
not decimal text or host struct serialization. FNV-1a/32 uses basis2166136261
and prime16777619 with unsigned wrap. `SaveBytes` is always0x600 bytes; file
size is selected by version.

| Offset (decimal) | Size | Current meaning |
| --- | --- | --- |
| 0 | 4 | `PM2D` magic |
| 4 | 4 | Version |
| 8,12 | 4 each | Native world X/Z |
| 16 | 4 | HP |
| 20 | 4 | Mushrooms and optional quest-state bits |
| 24,25 | 1 each | Harness completion, sound enabled |
| 26 | 1 | Stable native room registry ID |
| 27 | 1 | Signed int8 elevation in tenths |
| 28 | 4 | Signed original story word if present; legacy checksum in v1/2 |
| 32 | 1 | Story presence |
| 33 | 1 | Interior mask: plaza(room0)/1 or east(room2)/2 |
| 34 | 1 | Bit0=flag1207, bit1=flag233, bit2=flag236 |
| 35 | 1 | Original save mode |
| 36 | 4 | Checksum of bytes0..35 |
| 40 | 1492 | Original packed prologue pouch when present |
| 1532 | 4 | Checksum of bytes0..1531 |

## Versions and optional knowledge

| Version | File size | New state / constraints |
| --- | --- | --- |
| 1 | 32 | Native checkpoint; room/elevation must be zero |
| 2 | 32 | Room/elevation; checksum at28 covers bytes0..27 |
| 3 | 40 | Optional story; bytes33..35 reserved zero |
| 4 | 40 | Interior mask |
| 5 | 40 | flag1207; save mode reserved zero |
| 6 | 40 | Save-block flag233/236 and mode |
| 7 | 40 | quest1420, flag5405/5406 knowledge/value bits |
| 8 | 1536 | Original pouch, flag5407 knowledge/value |
| 9 | 1536 | flag5376 knowledge/value |

The encoder currently emits6/7/8/9 depending on populated optional state;
the decoder accepts1..9. The word at20 has these bit assignments:

| Bits | Meaning |
| --- | --- |
| 0..7 | Mushroom count |
| 8..15 | quest1420 value |
| 16 | quest1420 known |
| 17,18 | flag5405/5406 known |
| 19,20 | flag5405/5406 value |
| 21,22 | flag5407 known/value (v8+) |
| 23 | Pouch present (v8+) |
| 24,25 | flag5376 known/value (v9) |

A presence bit means the state is known even when its value is zero/false.
Absent legacy state remains unknown. Values without knowledge bits, reserved
bits and nonzero absent-pouch bytes are rejected. This prevents a checkpoint
from selecting story branches using invented switch values.

Validation admits mushrooms0..99, HP0..10 without pouch or0..32767 with pouch,
finite X/Z within +/-100, legal room/interior combinations and boolean header
bytes. Encoding bounds elevation to +/-12.7 before rounding to tenths. Decoder
structural checks do not sample room geometry; `main.cpp` validates floor and
wall clearance separately. Failed decoding leaves the caller unchanged.

## File replacement

`writeSave` encodes and decodes the candidate before writing `.tmp`. It checks
write/flush/close (hosts additionally fsync), moves a valid existing main file
to `.bak`, then renames `.tmp` to main. Invalid main files do not overwrite a
valid backup. If the final rename fails, `.bak` may be the only valid file.
`loadSave` tries main then backup and distinguishes missing files from invalid
files. This is recovery logic, not a guarantee against every filesystem or
power-loss failure. `snapshotSaveProgress` copies shared pouch bytes so queued
writes cannot see later live inventory mutations.
