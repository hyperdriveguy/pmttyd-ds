# Native handheld save record

This is the port's format, not the original GameCube card format. Original
save and switch persistence remains requested in [SAVE-01](../../requests/SAVE-01.md).
[Implementation](../../../source/save.cpp) writes little-endian40-byte PM2D
version4 records, using the existing temporary-file/backup replacement path.

| Offset | Value |
| --- | --- |
| 0 | PM2D magic, four bytes |
| 4 | Version word,4 |
| 8/12 | Native X/Z float32 |
| 16/20 | HP/mushroom count words |
| 24/25 | Native harness victory/sound bytes |
| 26/27 | Stable native room ID / signed elevation in tenths |
| 28 | Original story signed word, when present |
| 32 | Story presence byte,0 or1 |
| 33 | Native interior mask: inn1 in room0, professor2 in room2, otherwise0 |
| 34/35 | Reserved zero bytes |
| 36 | FNV-1a checksum of bytes0..35 |

Unknown masks, a mask for another room, invalid reserved/presence bytes, missing
story presence with nonzero story data and invalid checksum reject atomically.
Version3/40-byte saves retain their stricter zero bytes33..35 and load doors
closed. Versions1/2 retain32-byte records and checksum over the first28 bytes;
they load with no original story and doors closed. Version1 retains its original
plaza/elevation-zero restriction. No story value is inferred from harness state.

The completed door mask enters Progress after each simulation tick. Door
movement/waits continue to suppress saves. A stable interior may be saved
through the existing menu; restoring configures its display/collision state
before room floor/wall validation. Room changes clear the old room's door mask.
This restores the native playable interior, not the missing source NPC/item/
story event state. Original persistent namespaces and full card timing/errors
remain unimplemented.

[Save regression tests](../../../tests/native_save_tests.cpp) cover both room
masks, invalid masks/reserved fields, story presence, all three old versions,
actual file loading and backup recovery. [Original door integration tests](../../../tests/field_door_tests.cpp)
save/reload both original interiors and then exit through their close contacts.
