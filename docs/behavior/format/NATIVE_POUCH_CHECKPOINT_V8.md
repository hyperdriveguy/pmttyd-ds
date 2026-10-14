# Native v8 pouch checkpoint

This local homebrew format preserves native field checkpoint data and the
complete original0x5d4 packed pouch. It is not the GameCube memory-card format
or a claim that original save/card drivers have been ported. Original pouch
[initialization](../battle/BATTLE_FRESH_PACKED_POUCH.md) and
[parameter revision](../unit/POUCH_PARAMETER_REVISION.md) are separate owners.

Files are exactly1536 bytes for version8. Versions1/2 remain exactly32 bytes;
versions3..7 remain exactly40 bytes. Reads reject any truncated or extra bytes,
validate before publishing a new Progress, and retain the existing atomic
temporary-file/backup replacement and recovery behavior. Encoding uses the
larger in-memory container, but legacy-only writes remain40 bytes.

| Offset | Native meaning |
| --- | --- |
| 0..35 | Existing PM2D header; version at4 and adapted field checkpoint fields |
| 36..39 | Existing FNV-1a checksum over bytes0..35 |
| 40..1531 | Original0x5d4 pouch bytes, unchanged, or all zero when absent |
| 1532..1535 | FNV-1a checksum over bytes0..1531 |

The word at20 retains legacy mushroom quantity and original quest/flag
knowledge. Version8 adds bit21=known5407, bit22=value5407, bit23=pouch present.
Bits24..31 remain reserved. A value bit without its known bit is rejected;
an absent pouch must have a zero payload. Versions<=7 never manufacture5407
knowledge or a pouch, including when decoding over an existing populated
Progress. Explicit source fresh swInit establishes knownfalse5407 in the
native new-file handoff. Loaded5407 remains independent of story/quest/item
counts. Source5407 location/control semantics are in the
[talk control contract](../field/GOOMBA4_QUEST_TALK_CONTROL_AND_POSE_ADMISSION.md).

Present pouch payloads are copied byte-for-byte into independent owned storage.
Queued writes and checkpoint imports also deep-copy that payload on the
simulation thread. Worker encoding cannot race live item/HP writes, and loading
a cached file slot cannot turn its stored pouch into mutable live game storage.
No fresh-game normalization, inventory sorting, duplicate removal, HP healing,
badge validation or partner-rank rewriting occurs during persistence. Signed
resources, gaps, all partner records and unobserved fields survive. The finite
item/rank checks belong at revision service admission, not save decoding.
The legacy native HUD HP header remains a separate adapted field; with a pouch
present its admitted range grows from0..10 to0..32767. This does not clamp or
rewrite the original signed pouch HP halfword. Field item-menu/HUD ownership
still needs the original inventory/progression services.

Tests preserve every payload byte, evolved HP and signed BP, knowntrue/knownfalse/
unknown5407, independent loaded storage, older-import unknown migration,
header/payload/checksum corruption, missing/extra bytes and backup recovery.
Goomba13 tests also persist source flags and post-hand-in inventory through this
format. Full original switch banks, source card checksums/layout, global
save-event ownership and original acquisition/progression remain open.
