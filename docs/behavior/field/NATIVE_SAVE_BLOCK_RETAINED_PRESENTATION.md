# Native save-block retained presentation and queued writes

2026-10-13; portable adapter for the original US save-event graph. This is a
service implementation, now connected through the bounded
[live binding](NATIVE_LIVE_SAVE_BLOCK.md).
See [event core](NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md),
[retained-window source contract](../message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md),
[window states](../message/MESSAGE_WINDOW_STATES.md) and
[parser records](../message/MESSAGE_PARSER_RECORDS.md).

`SaveBlockPresentation` executes the unchanged packed source events. Its main
MessagePlayback retains identity, serial, parser records, key ordinal, cursor,
elapsed schedule and scroll across original Add and Continue. Add reparses the
accumulated original bytes over the old terminal; Continue changes state to1,
sets the window poll bit and refreshes the playback timestamp only. It cannot
release a key wait or force-close the window. Flags1 Add uses the literal source
controls; flags0 resolves unchanged global message IDs. No dialogue is authored.
The original yes/no control owns a separate MessageSelection and its original
initial/cancel values, source input masks, selection sound requests and fade.

MessagePlayback now separates W.windowFlags from D.flags. Opening and retained
resume set W.poll2; explicit retained stops clear only that bit. Ordinary
printing/key waits and terminal/closing states retain it until deletion. D's
entry callback bit2 remains independent. State7 enters state8 on the same
invocation; deletion still requires opacity already0 at entry. `<system>` emits
original color record0xfff7/payload0xdcc8dcff without advancing T and sets type3.
Talk publication is suppressed for type3 by the existing consumer.

Window processing precedes the event polling within this adapter. Newly opened
or resumed windows progress on the next simulation update. This does not claim
the original whole field task ordering or pause/category ownership. The caller
owns the outer MapObjectKeyControl leaf; the script's nested KeyOff/On uses the
same supplied service. Active presentation is movable, not copyable. The owner
must keep its key service alive and must not replace a live presentation.

The SD adapter yields one invocation at Header; native serialization combines
header and data in its atomic file write. Write enqueues a monotonically
increasing ticket and the four source switches published after that manager
invocation, including same-invocation GSW4 changes. The platform takes it once,
snapshots other progress and performs I/O outside simulation. Matching completion
publishes success0 or a provisional generic source error-1. Duplicate/stale/
untaken/finished/moved-from completions reject. The source graph decides all
subsequent windows; No never enqueues Header or Write. Source GC card hardware
and its error classification are not implemented. Code-1 follows the source's
damage/error messages; it is an adapter classification, not proof of card damage.
Formatting, IPL and creation paths require a real provider and currently fault
instead of manufacturing success. Guard returns0; Cleanup/Recovery are adapter
no-ops for this bounded SD domain. Unsupported messages or providers mark
presentation failed and yield; the field owner must handle recovery/release.

Original payload/font/icon resources load once outside tick; invalid loads
preserve prior resident data. There are no filesystem reads or writes in tick.
Append reparses only at source Add boundaries; it does not run every frame.
This is not a hardware performance measurement or a complete window pool.

Tests run the actual original graph and global US payloads through all first-hit
pages, raw scroll/stop, yes/no/B cancel, normal successful and failed writes,
retained main versus auxiliary ownership, delayed/duplicate/stale acknowledgements,
same-invocation switch snapshot and live move. Focused playback tests check retained
resume, preserved key waits, independent poll flags and delayed close deletion.
Exact final build/test results are in [progress](../../PROGRESS.md).

Native save v6 keeps the existing40-byte envelope and checksum: byte34 bits0/1/2
carry GSF1207/233/236; byte35 carries the full GSW4 byte. Versions1..5 retain
their prior reserved-byte rules and migrate missing save switches to0. Decode
rejects unknown flags/version or corruption without changing caller progress.
This schema and live switch propagation are implemented by the linked binding.

Next: live object instance clocks/S_1/A_1, owner/name hit publication, source task
ordering, camera8's [letterbox decision lock](../camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md),
cue[431](../audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md), native window drawing,
asynchronous SD worker and live GSF233/236/GSW4 propagation, isolated emulator captures and
hardware profiling. No full-game completion claim.
