# Trio runtime menu bindings

The menu-provider section of [TrioAttackRuntime](../../source/battle/trio_attack.cpp)
connects retained source controllers to the actual VM, packed actors and native
resource owners. This guide covers `originalActMenuUiServices` through
`resumeOriginalCommandInput`, plus their allocation metadata. It supplements
[Trio scheduling](trio-scheduling.md) and [the public facade](trio-api.md).
Newer Turn/Phase/Act, pinch, sequence, pose and public draw bindings require their
own review; this is a partial audit of the large runtime file.

## Completion and pending symbols

Every optional result has three relevant outcomes: absent means wait; engaged
zero is a completed source value; engaged nonzero is a completed source value.
Audience checks map completed false/true to 0/1 while preserving absence.
A private cursor output can remain unknown even when GetCursorPtr completes.
Never replace an unwritten private cell with cursor zero.

Main, Setup and End have distinct parent cursors. The UI, class-table, audience
and PresentItem children keep separate progress, so a parent wait cannot replay
completed allocation, query, recovery or type-publication prefixes. Wrappers
preserve a deeper pending symbol when present, then use a child symbol or their
explicit original entry fallback. Pending addresses are source identities for
diagnostics, not invented success values. See [menu controllers](battle-menu-continuations.md)
and [command input](battle-command-input-continuation.md).

## Actual map and message owners

Class UI allocation loads the current heap pointer from global `0x8041e80c`.
It admits both real flush/system-call providers before the VM allocator can
mutate storage. An engaged allocation address zero remains the source result.
Every nonzero allocation gets a shared identity token in `actMenuUiAllocations`;
the byte inside that token carries no guest data. Class release removes that
identity before calling actual map free, and refuses unknown addresses.

Weapon UI additionally requires the real battle-allocation lease. Its MapFree
request (`0x8002fdc4`) captures the exact cursor-owned request object, pointer and
allocation token once, even when the first lookup has no owner. A hold cannot
adopt a later allocation at the same address. Free retires metadata before the
actual VM free, then clears the captured borrow. MapAlloc (`0x8002ff58`) checks
the same flush admission and records nonzero results in the shared registry.
The predicate checks captured identity between synchronous groups; callers must
keep actual maps fixed throughout each group. Retirement resets cursor/borrow
metadata only after reset succeeds; it does not perform End, free guest storage
or erase the allocation registry. See [weapon UI](battle-weapon-menus.md).

Class-table services use packed registry lookup, actual pouch pointer global
`0x8041eb00` and the immutable original US message binding. The table captures
that binding before entry and holds its owner across waits. msgSearch returns
a mapped guest text address, preserving original bytes and control tokens.
Missing messages stay unavailable. See [native message ownership](native-menu-support.md).

## Input, camera and event domains

Menu input leaves reload current battle B and mask channel-zero words at offsets
`0xf44` (trigger), `0xfbc` (repeat) and `0x1034` (held). These reads neither
consume edges nor latch a new private input history. Camera mode/speed requests
use the packed part-position provider; original priority rejection completes
rather than becoming a provider wait. Only explicitly recovered return-only
original symbols acknowledge immediately; that says nothing about GPU readiness.

Input's `_commandRestoreRec` (`0x8011f5f0`) keeps three separate fields: pending
byte B+`0x1c74`, saved Input-state word B+`0x1c70` and current Input state
B+`0x171c`. With no pending byte, snapshot current state. With pending set,
clear saved then pending only at exact equality; unequal states preserve both.
Previous menu type B+`0x1c60` has a different purpose. See the
[restore contract](../behavior/battle/MENU_INTERRUPTION_RESTORE_AND_AUDIENCE_FIELDS.md).

The event constructor returns a mapped guest entry address, whereas checkEventId
consumes a scheduler event ID. Registry addresses, event IDs, source function
addresses and host object pointers are different domains despite all fitting
some integer representations. CommandPose uses the atomic packed-name adapter;
it supplies no animation/GPU completion. See [native support](native-menu-support.md).

## Audience and recovery barriers

The allocation token reused from HP represents the owned VM battle span, not HP
render/camera readiness. Physical X still queries actual current trigger input;
completed false means no interruption. Audience End is a retained child before
its source target/type/Input-state publication completes.

PresentItem keeps double subtraction and single rounding separate, then requires
actual fused multiply-add and addition providers. Missing math waits at that
child without host algebraic substitution. Recovery has already happened before
the missing effect owner can hold; the present item stays uncleared until actual
effect completion. The runtime deliberately leaves that effect callback absent.
Whole weapon table/update/decision/target and retained sound providers also
remain unavailable in these dispatch tables. Wiring UI Init/End or class messages
does not prove the whole command path is available. See
[PresentItem ownership](native-menu-support.md) and [ActMenu composition](battle-act-menu-continuation.md).
