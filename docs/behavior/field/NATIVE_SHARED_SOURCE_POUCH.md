# Shared original field pouch backing

`NativeMarioConstructor::sharedPouch()` returns the typed owner of the same
5d4-byte packed record selected by source global8041eb00. The record's guest
address is the actual original heap0 allocation. Before source publication, the
adapter extracts that allocation's payload, preserves its bytes in one stationary
`ProloguePouch::bytes` array and maps the array externally at the unchanged guest
address. Original SDK headers/list state stay in the real heap. Whole original
pouchInit then publishes, clears and initializes that same array in source order.
No second source allocation, fabricated pointer, later initialization or periodic
copy is used to share typed inventory edits with source VM reads.

Source constructor logic, message text, data pins and native finite-service
limits are unchanged; see [constructor ownership](MARIO_CONSTRUCTOR_NATIVE_OWNER.md).
Implementation: [native owner](../../../source/field/native_mario_constructor.cpp),
[Game admission](../../../source/runtime/game_field_animation.cpp) and
[restore](../../../source/runtime/game.cpp).

## Lifetime and publication

The public shared owner is available only after genuine constructor completion
and while the exact UI/process/cache/payload/global identities remain current.
Backing checks accept mutations of the live bytes. Equal bytes in a replacement
RAM mapping do not satisfy the receipt. The storage shared_ptr is never replaced
or moved after mapping. Retained typed holders keep the array alive after native
owner retirement; retirement removes only the VM external mapping with that exact
address AND expected host buffer. A foreign replacement is preserved. This is
native view retirement, not original pouch destruction, SDK free or field return.

The byte array keeps its original fixed packed format. Typed halfword/key/coin
helpers and VM byte/half/word operations immediately observe each other's writes.
`adoptPouchSnapshot` is still an explicit native packed-byte handoff into this
existing allocation; `capturePouchSnapshot` creates a detached value. Neither
helper executes original card load or a battle export/result/End parent.

## Normal Game ownership

Cold native startup copies an explicitly supplied packed inventory after the
original fresh constructor, then points `Progress.originalPouch` at the shared
source array before field animation starts. Ordinary room candidates retain that
canonical shared pointer. A non-checkpoint handoff rejects inventory pointer
substitution before any live pose or pouch mutation; it copies no pouch bytes.

An actual native restore captures the pouch through the admitted public Game
constructor accessor BEFORE resetting simulation state. A copied Game still
contains the private constructor shared_ptr but lacks the actual native field
animation ownership; that private pointer does not authorize a checkpoint write.
Restoring a private snapshot in such a candidate leaves the running pouch intact.
The successful checkpoint handoff copies its provided packed snapshot into the
existing canonical array only after every fallible candidate/pose admission check,
then publishes the same shared pointer. Guest address and buffer identity survive.
No constructor or pouchInit is replayed. These are native checkpoint policies,
not receipts for the original save/card driver or source sequence return.

Missing legacy inventory stays absent. The retained raw fresh constructor record
is not evidence that an inventory-less legacy checkpoint supplied packed bytes;
`Progress.originalPouch` remains null and no default inventory is inferred. A
consumer requiring canonical field inventory must admit that actual nonnull
Progress pointer and its shared source owner together.

Worker saves continue to use `snapshotSaveProgress`, which deep-copies the packed
array before a worker can write it. Later live edits cannot alter that worker
snapshot. The existing Trio runtime retains its supplied entryPouch and copies
its exact ending export into that same object; this now updates field source
bytes too when the canonical shared object was supplied. The Trio battle VM's
private projection and original export ordering are unchanged. This is not a
claim that every battle owner now uses one VM or that Harbor return is complete.

## Evidence and remaining work

[Native constructor tests](../../../tests/native_mario_constructor_tests.cpp)
exercise typed-to-VM and VM-to-typed mutation, initialized publication, later
heap extraction, exact foreign mapping replacement, source view retirement and
retained typed storage lifetime. [Game tests](../../../tests/native_ui_startup_game_tests.cpp)
exercise supplied snapshot admission, worker-copy independence, normal room
retention, private candidate nonmutation and rejection, admitted restore,
checkpoint commit into the same object and legacy absence.

Original Mario control/position/party state, whole field Main, Harbor battle
pouch/ending composition, source Prepare/Post and field start/wait/return remain
separate requirements. The [Crump integration contract](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md)
stays open; shared inventory backing alone is not a completed opening battle.

The native `Progress.hp` UI/legacy scalar remains a separate mirror. This packed
storage bridge does not route every legacy HP write or the existing staged
Harbor HP-minimum adapter; those consumers must use the actual canonical pouch
when the full field/battle owner is composed. No whole-resource-authority claim
is made for legacy harness scalars.
