# Save-block original event VM and native presentation

Implementation: [event](../../source/save/save_block_event.cpp),
[event API](../../include/save/save_block_event.hpp),
[presentation](../../source/save/save_block_presentation.cpp),
[presentation API](../../include/save/save_block_presentation.hpp).
Evidence: [event core](../behavior/field/NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md),
[retained presentation](../behavior/field/NATIVE_SAVE_BLOCK_RETAINED_PRESENTATION.md),
[live field binding](../behavior/field/NATIVE_LIVE_SAVE_BLOCK.md).

The event wrapper imports original packed US event data and executes it through
the native EVT interpreter. It does not replace the script with authored dialogue
or a native yes/no plot branch. `PMSAVE01` metadata pins system root `0x8030b4ec`,
wrapper `0x803bac3c`, common event `0x803ba444` and 14 ordinal/function rows in
`SaveEventCall` order. Header/rows/EOF and mapped source words validate before
publishing the script catalog. CPU executable bodies are not executed here.

Native VM provisioning uses GP `0x81000000`, SDK heap
`[0x82000000,0x82100000)`, small imported source globals, refresh configuration 60
and the 243 MHz clock input passed through `eventClockRate`. Caller milliseconds
supply timestamps. These are explicit native contexts, not recovered heap
placement or complete GameCube startup. Numeric adapters have finite asserted
domains; flush/syscall hooks are no-ops.

Saved namespaces remain distinct: GSW0 story, GSW4 mode, GSF233 first hit,
GSF236 post-story hit. Root creation uses priority 30, flags 0, type 26 and puts
opaque object owner at EventEntry `+0x174`, never a host pointer. Each manager
invocation publishes the saved switches afterward. Allocation count zero marks
event finish; it does not prove field ownership restoration or disk persistence.
Runtime owner is rebound after moves/before ticks. Copying `SaveBlockEvent` shares
its underlying runtime, so an active copy is not an independent VM.

Fourteen typed USER_FUNC callbacks preserve original function identities, raw
operands, resolved values and first-call state. Source 0 means busy, 2 complete.
Only completion writes selection LW0 (`-30000000`) or Guard/Code operand-0 output.
Invalid function/argument/text access fails the event. Text reads are bounded 256
bytes; flag 1 means literal original controls, otherwise an unchanged US message
ID. Missing/unsupported text faults and yields, without substituted dialogue.

Presentation retains separate main message and selection windows. First callback
opens/appends/resumes, later callbacks poll window flags. Continue cannot clear a
key wait. Main and selection updates happen before event polling, so newly opened
or resumed windows advance on the next invocation. This adapter ordering does
not establish complete original field scheduling. Its key-control service must
outlive presentation; scripts call the same nested on/off owner. Active
presentation is movable, not copyable; moved-from I/O completion is refused by
active-event admission. Failure recovery/key release remain the field owner's job.
See [message playback](message-playback.md).

Header yields once because native SD serialization combines header/data in one
write. First Write enqueues a monotonic ticket and yields. Snapshot is refreshed
**after that manager invocation**, including switch changes reached after the
request. Platform takes queued ticket/switches once, snapshots other progress and
performs actual I/O outside simulation. `takeWriteRequest` consumes queue admission
but keeps waiting live. `completeWrite` accepts only an active taken request with
exact ticket and no prior completion. Stale, duplicate, untaken and moved-from
acks reject. A later callback consumes completion; Code reports native success 0
or generic -1. That -1 is an adapter classification, not proof of card damage.

Guard returns 0, Cleanup/Recovery acknowledge no-ops in this bounded SD backend.
IPL/Create/Format fault until supplied real implementations; they do not fabricate
card success. Original script controls No/error/success dialogue and source branch
conditions. No actual filesystem write occurs in tick or these portable wrappers.

Resource loading is **not one transaction across process catalogs**: local
messages/font publish at the end, while icon load can already have successfully
replaced the process icon catalog before a later event-asset failure. Individual
catalog loaders have their own publication boundaries. Field startup owns this
resource lifetime, not per-frame code. Original art/text/licensing are unchanged.
