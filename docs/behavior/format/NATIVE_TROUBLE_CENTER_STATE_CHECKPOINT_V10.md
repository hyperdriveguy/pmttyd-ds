# Native Trouble Center state and checkpoint v10

Commit10cb277f implements the native finite domain retaining GSF5374 lecture, all30 completion flags
5375..5404, all17 working flags5405..5421, GSW1420 active quest and six
working bytes1469..1474. It supports [original acceptance/reset](../field/TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md)
and the [original board producer](../field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md).
This is a native knowledge/persistence adapter, not the complete original GP
bank, memory-card record or event-variable namespace implementation.

FieldActorTalkState retains canonical named optionals5376/5405/5406/5407 and
1420 for existing live NPC owners. troubleQuestFlag/Byte route those fields
without duplication; all other Trouble flags use known/value48-bit masks.
Bit(flag-5374) maps IDs; bits2 and31..33 are forbidden in the additional
store. Mask covers44 additional flags. Six bytes have independent knowledge
bits. Reads outside the finite domain return unknown; writes reject the domain.
Nonzero byte writes retain low8 of the supplied word, including negative values.
Storage is inline and snapshots copy its state by value.

Explicit native new-file handoff calls initializeTroubleQuestState. Original
[swInit](../memory/SW_SET.md) zeroes GP178..577 global flags and GP578..d77
bytes, establishing all these values zero/false. This does not authorize clearing
on load, room change, swReInit or menu creation. Legacy checkpoints leave newly
unrecorded fields unknown. The reset service factory binds original
startTroubleQuest writers: clear5405..5421 then set1469..1474 zero, retaining
lecture/completions/1420. Pouch cancellation and source selection/acceptance/
quest start child event still require their actual owners.

## Native file layout

Minimum emission remainsv6/v7/v8/v9 unless additional fields are known. v10
files have exactly1568 bytes (0x620). The largest in-memory SaveBytes buffer is
0x620; old v8/v9 files remain exactly1536 bytes and v3..7 remain40. Old versions
1..2 retain32-byte load support. All integer payload words are little-endian.
Original pouch occupies unchanged40..1531, with unchanged FNV checksum1532..1535.
Header/checksum remain the [v9 layout](NATIVE_REWARD5376_CHECKPOINT_V9.md).

| Offset, decimal | Bytes | Meaning |
| --- | --- | --- |
| 1536 | 6 | Additional flag knowledge, low48bits |
| 1542 | 6 | Additional flag values, low48bits |
| 1548 | 1 | Working byte knowledge, bits0..5 |
| 1549 | 6 | GSW1469..1474 raw values |
| 1555 | 9 | Reserved zero padding |
| 1564 | 4 | FNV-1a/32 checksum of bytes0..1563 |

All three checksums validate before final Progress publication. Reject duplicate
alias bits, out-of-domain knowledge, values without knowledge, high byte-known
bits, nonzero unknown byte payloads, reserved padding and incorrect file lengths.
Old-version in-memory extension bytes must be zero. An absent pouch still requires
its old payload region zero. Failure preserves caller Progress and existing
backup-recovery/immutable queued-write semantics. Isolated smoke accepts1568-byte
fixtures in addition to the old32/40/1536 sizes.

## Validation and scope

Final isolated make test passes269/269 C++ and159/159 Python checks;
make assets-ready and make all pass. Review tree610293eb060f3d5c7695436f52bebb8661907e56
in artifacts/source-checks/quest13-field. All3110 tracked files match that tree.
Logs: artifacts/trouble-state-test-checked.log, trouble-state-assets.log,
trouble-state-build-final.log and trouble-state-audit.log. Normal3DSX SHA256
95921ca88dcf466d4ae14410d4dc85bea4b338aea73a2f6cfb435239948d4152.

New tests check all48 flags individually through unknown/false/true and save
round trips, each independent working byte including low8(-1/256), canonical
legacy aliases, fresh explicit zeros, reset preserving lecture/completions/1420,
snapshot isolation, exact1568-byte disk files, corrupted and rechecksummed
invalid payloads, truncation/trailing-byte backup recovery, and actual v9 disk
migration leaving additional fields unknown. Existing native_save checks retain
versions1..9 compatibility and original pouch snapshot/recovery coverage.

First host configuration failed because the review staging captured concurrent
CMake append blocks twice. Scoped staging now extracts only this step's block.
Next host compile exposed the separately owned obsolete fieldSoundFlags reset
on committed base; the validation snapshot includes its existing one-line removal,
now separately committed as c632bfec. Initial failure logs remain
artifacts/trouble-state-test.log and trouble-state-test-final.log; they are not
counted as passing. No foreign changes are part of this state commit.

No Azahar/New2DSXL execution is claimed for this persistence step. Native main
fresh admission compiles; host tests prove persistence/service mechanics, not
physical case/door/EVT/audio execution or rendering/hardware parity. Read-only
ISO recovery touched no user emulator profile/save or original ISO.

Remaining: actual gor_02 board/counter case9 and door/cutaway owner, lecture and
help/confirmation/acceptance message/event flow, native repeat/input and retained
iconMain ordering, original menu sample playback, complete saved-switch bank and
original card format. The [door/contact recovery](../field/TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md)
records story23 access, collision identity and priority0/typeef root ownership.
No interaction radius, professor interior mask or story condition is invented.
No new dialogue, geometry/art payload or PCM is included by the state adapter.
