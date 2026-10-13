# Native portable item-table selection service core

The source baseline is [the original item selector](ITEM_TABLE_SELECTION_WINDOW.md)
and [Goomba4's quest/inventory graph](../field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md).
These components prepare the original native quest binding. They do not yet
activate an item-selection screen, positive-item quest event, original sound
macro or complete saved inventory backend.

`fillPackedExplicitItemRows` executes the original negative-address table branch
with original-address memory/allocator/clear services. The count pass includes
-1, allocation is(count+1)*4, and pointer publication/reload precedes explicit
clearing and row-count reset. The optional second argument prepends its low16.
The fill pass rechecks the terminator, rereads each admitted entry, preserves
duplicates/casts, and publishes incremented count before item halfword. Source
row flags remain zero from clearing. Finite terminated tables and valid arena/
context extents are caller domain requirements, not original bounds checks.

`packedItemSelectionResult` reads context flags as U16, returns0 before ready,
returns-1 on source cancel predicates, and otherwise returns U16 item identity.
It preserves duplicate index reads and original32-bit row-address arithmetic.
There is no injected cancel row, fabricated selected quantity or signed-byte
message-selection result. Native invalid-address policy belongs to the memory
provider, not this source leaf.

`itemSelectionEvent` resolves mode/table on each invocation. First call creates
with(table,0,1), retains the original handle in packed E+78 (the shared entry's
waitCount word), then locks keys. Nonzero poll gets index before deletion,
unlocks keys, and only then reads/stores output destinations. Nonzero mode
also stores selected index. The cached argument-stream pointer and late operand
reads survive callback mutations; E+78 remains unchanged after deletion. Full
manager allocation, physical key ownership and event callback registration are
still future providers; none is replaced with a no-op to enable quests.

`tickItemSelectionMain` owns source opening/closing requests, status/sound IDs,
transition gates, repeated/new direction distinction, eight-row scroll, disabled
row/A-before-B priority and source binary32 easing. Three source manager tokens
share one list owner; only that owner runs the controller. Active-slot gates
and unconditional description reset match the source. Positive addressable row
arrays are the current interactive domain; an empty explicit source table has
no invented safe-choice policy.

`tickItemSelectionSequence` provides source kinds0/2 with typed refresh and
interpolation services. It computes signed32(refresh*8)/60 with word wrap,
requests original curve12 for rotation then scale, rereads timing across both
calls and completion comparison, and retains transition4 until the next idle
invocation. Idle preserves visibility2; close completion clears2 and optionally
active1 under flag8. Kind2 uses immediate original transforms without timing
reads. The existing camera interpolation's finite curve12 may serve a native
numeric adapter; complete GP/clock/cadence/SDK math and GX presentation remain
unverified. No assumed fixed eight-tick delay is described as measured behavior.

The separate [native key-item services](../unit/NATIVE_KEY_ITEM_INVENTORY_SERVICES.md)
own original121-slot count/removal, event result/revision protocol and the live
Goomba count guard. Inventory observations do not become inferred checkpoint
state. Item-window creation does not remove items or write quest flags itself.

Validation and next work are recorded in [the handoff](../../PROGRESS.md).
Next provide complete persisted source pouch/switch ownership, original item
metadata/US names/icons and three-window drawing/input/help services, register
the original event callbacks, and execute both complete quest graphs. Original
control conditions, messages/speaker attribution and return/key restoration
must remain intact. Hardware performance and matched original frames are open.
