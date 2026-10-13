# Native save-block lifecycle and original event core

This portable layer follows
[head producer/callback](SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md),
[restart/publication](../display/SAVE_BLOCK_RESTART_CLOCK_AND_EVENT_IDENTITIES.md),
[common save graph](SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md) and
[retained window requests](../message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md).
It is compiled for host/ARM; Game/renderer/window/card binding remains the next
step. No user-facing save interaction is claimed by this library milestone.

## Object callback ownership

`MapObjectHeadCache` retains original hit names as native owned strings. Equal
names suppress retrigger despite owner/pointer changes; null contact clears.
`produceSaveBlockHead` preserves active/head-enable flags, normal-character
admission, cause0x400000 and the flag8 suppressions. This is the save
constructor's head domain, not a universal attack/partner object producer.

`advanceSaveBlock` executes exactly one source callback stage. It requests
nested KeyOff, system1, original source sound431, restartA_1 and camera8 flag200;
reads the provider's previous published ratio at stage1; restartsS_1; separates
optional child and system child stages; checks running==1 in waits; and restores
system/camera/key ownership in stage4 without clearing the retained eventID.
Ordered ratio<1 keeps the source NaN behavior. Event entry requests priority30,
type26 and owner identity. The entry helper checks any nonzero running result.
Valid event allocation/service domains are explicit; no original allocation
failure repair or global event-manager equivalence is invented.

`MapObjectKeyControl` keeps the original uint8 nesting/wrap and mainflag8
semantics. These leaves do not replace the Game's existing story/road owners.
Animation Main must publish after callback; its provider must use simulation
time/source clip metadata, not renderer frame counters.

## Original program data and interpreter

`tools/save_block_event_assets.py` packages only selected original DOL data
sections. Source system164 bytes/16 commands, wrapper52 bytes/8 commands and
common child2040 bytes/197 commands retain their addresses, hashes and all
221 original commands. The source label scanner reads one word past END, so
the actual adjacent data word is included, not a fabricated sentinel. Direct
message operands retain original ID/raw-control strings. Fourteen callback
identities cover89 encoded sites. Original CPU executable bodies are excluded.
Data/strings and compiled packs remain ignored/local; licensing/provenance is
recorded in [assets](../../ASSETS.md).

`SaveBlockEvent` imports that data into the existing source-derived interpreter,
creates a typed root at priority30/type26 and writes native opaque owner data
at source entry+174. Source VM opcodes execute conditions, child/copyback,
labels/switches, saved flag namespaces and original request order. GSW0/4 and
GSF233/236 remain distinct from event LW/LF variables. The original first-hit
flags survive No; No performs no header/slot write and no invented warning.
Success follows the source terminal append without an added Continue call.

All14 USER_FUNC callbacks pass through typed `SaveEventRequest` services using
original0/busy and2/complete return protocol. Selection, guard and completion
outputs write the original operands/LW0. Continue has no script operands. The
service receives unchanged message IDs or literal raw text with source flags1.
Window providers must retain/resume/append and poll the original window condition;
card providers must explicitly own yielding I/O and status codes. This layer does
not force-close windows, synthesize dialogue or claim GameCube-card/SD error
identity. The source arrays retain alternate/error branches; platform services
remain outside the interpreter. Runtime ownership is rebound after native moves.

## Verification and remaining integration

Source-derived lifecycle traces test stage ordering, suppression, optional
children, helper/wait predicates, previous ratio, NaN and nested key wrap.
Original-data interpreter tests check exact ID/raw-control request order,
No/success, initial flag/mode/story effects, asynchronous header/write polling,
all normal-mode error families, accepted format/create, failure repolls, guard
break and runtime moves. Test backends complete messages/choices; they are not
a display or filesystem fidelity test. Missing/malformed compiled assets and
full asynchronous allocation/card failure domains need broader coverage.

Next: register source actor instances and clocks, connect the actual head-name
publication to this callback after Mario, run event manager before Mario,
implement retained windows/original selection, persist source switches through
an explicit native card adapter, bind431 if its original audio is available,
and select per-instance S_1/A_1 in the streamed renderer. Source pause/category
and camera flag consumers, deletion/lifetime, full field ordering and original
execution/hardware comparisons remain separate unfinished work.
