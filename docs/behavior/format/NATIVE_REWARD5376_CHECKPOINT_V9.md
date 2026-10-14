# Native completion5376 checkpoint v9

Extends the [native v8 pouch checkpoint](NATIVE_POUCH_CHECKPOINT_V8.md) with
independent knowledge of original completion GSF5376. Original
[Goomba3 reward](../field/TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md)
clears GSW1420 then sets5376 after the blocking coin20 callback returns2,
before sys_request_finish. Admission reads quest1420 and5406, not5376.
Acceptance/cancellation clear5405..5421 and exclude5376. No new story condition
or inferred completion is introduced by persistence.

Source [swSet](../memory/SW_SET.md) places5376 at GP+418 bit0, in the same source
word as5405/5406/5407 bits29/30/31. Original
[card write assembly](CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md) includes that
switch bank. The native PM2D encoding below is an adaptation, not that original
card layout or checksum implementation.

FieldActorTalkState appends optional<bool> flag5376 after existing fields so old
aggregate initializers retain their meanings. Explicit fresh swInit initializes
it false. Unknown, knownfalse and knowntrue remain distinct. Neither story,
quest, delivery state nor inventory implies a value.

| Native word20 bit | v9 meaning |
| --- | --- |
| 0..23 | Existing v8 fields, unchanged |
| 24 | Known5376 |
| 25 | Value5376 |
| 26..31 | Reserved, rejected |

Encoding selects9 whenever5376 is known, including knownfalse with no pouch.
Otherwise existing v6/v7/v8 version selection remains. v9 files are exactly1536
bytes and share v8's header/pouch offsets and both FNV checksums. Value without
knowledge is rejected; absent-pouch payload must be allzero. v8 still rejects
bits24..31, v7 rejects21..31. Versions1..8 import5376 as unknown even when the
destination previously knew it. Decode publishes only after complete validation.

Atomic replacement/backup recovery remain unchanged. Queued snapshots copy5376
by value and deep-copy original pouch storage. The new scalar adds no rendering
work or native texture/audio allocation. Original full switch-bank/card services
remain unbound. Native reward setFlag/Game services are the next integration;
persistence alone does not admit the reward event.

Tests cover optional state with/without pouch, exact native write lengths,
versions1..8 clearing stale completion knowledge, version-specific reserved
masks, value-only rejection, absent-pouch payload, both checksum corruptions,
truncation/extra bytes and v9 backup recovery. Reward tests bind the new field,
check acknowledged coin completion precedes its write and confirm knowntrue
completion does not become an invented admission guard.

## Exact validation (2026-10-14)

Code commit b9310091; reviewed isolated tree d810dfe0b19a7d7bab864417b809df6069b8d93d.
Sequential make assets-ready passed; make test -o assets-ready passed250/250 C++
and144/144 Python, and normal make all -o assets-ready passed. Logs ignored:
artifacts/reward-save-assets.log, reward-save-test.log and reward-save-build.log.
Reviewed normal artifacts/ttyd-reward-save-reviewed.3dsx SHA256
53ce6e34aa6fe0d65214bef9c9b054c7313a5c48181f86afafa6f7a429467842.
Read-only parallel persistence review found no blockers; source audit matches.
No new assets/conversion or Azahar/hardware run in this persistence increment.
Previous audio-tail checks and complete native quest-chain validation remain open.
