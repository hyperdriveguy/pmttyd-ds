# Retained original battle event barriers

The native owner in `battle_event_barrier_continuation` implements the complete
local bodies of US G8ME01 revision0 functions below. Original source ordering is
described in [event waits and conclusion](BATTLE_WAITS_AND_CONCLUSION.md).
This is a source-derived implementation; no executed-original comparison is
claimed.

| Original body | Address | Size |
| --- | --- | --- |
| BattleWaitAllActiveEvtEnd_NoBgSetEndWait | `0x8011abb4` | `0x134` |
| BattleWaitAllActiveEvtEnd | `0x8011ace8` | `0x104` |
| BattleAfterReactionMain | `0x800f78fc` | `0xe8` |
| BattleAfterReactionRelease | `0x800f79e4` | `0x94` |
| BattleConsumeReserveItem | `0x800f74b4` | `0x60` |

The existing packed `BtlUnit_GetData` body at `0x8012733c`, size `0x40`, supplies
the data-table lookup. The owner does not replace conclusion checking or event
allocation with assumptions about units, scripts, or an empty event registry.

## Ownership and completion

`resumeBattleAllEventBarrier` and `resumeBattleUnitEventBarrier` receive the
actual battle pointer. Their engaged integer is the original completed result:
zero means the barrier is not ready, one means ready. `nullopt` means an actual
child provider has not completed. A source zero is never a native retry signal.
`resumeBattleAfterReaction` has the same optional convention for its original
zero/one reaction result. `resumeBattleConsumeReservedItems` instead reports
completion of the original void routine.

Each cursor holds a single invocation and entry kind. Battle pointer, child
callbacks, callback user, guest memory callbacks, actor byte callback and audience
lifecycle observer identities must remain unchanged while held. Recursive entry
and reset during a provider invocation are refused without guest effects.
Reset outside a provider abandons the native cursor without undoing committed
source writes; callers must not use it to restart a partially executed routine.
The actual memory allocation and provider-owned objects must outlive the cursor.
Function-pointer equality does not prove guest allocation lifetime.

The only external children are:

| Callback | Original call | Required meaning |
| --- | --- | --- |
| `eventExists(id)` | `evtCheckID`, `0x80036604` | Actual event-ID lookup; engaged zero means absent |
| `removeItem(item)` | `pouchRemoveItem`, `0x800d4c38` | Actual inventory removal; any engaged result completes |
| `concluded(B)` | `BattleCheckConcluded`, `0x8011aa34` | Complete original check, including its group result stores |
| `create(script,10,0)` | `evtEntry`, `0x800374ac` | Actual guest EventEntry address; engaged zero is actual allocation failure |

An absent callback holds only if source reaches that child. Empty queues and zero
event fields therefore complete with all child callbacks absent. A provider
returning `nullopt` owns its own partially completed work and must resume that
same invocation when called again with the captured operands. The barrier never
replays an earlier successful child or source mutation. In particular,
unsupported script execution or missing allocation support must hold rather than
returning an invented allocation failure.

## Source order retained through waits

The unit scan visits all 64 registry slots and all four event fields in order,
even after finding a live event. Finished IDs are cleared immediately after their
actual check completes. The outer five battle IDs instead stop at the first live
event. Neither scan adds status, HP, allegiance or animation gates.

AfterReaction captures its global battle independently of the pointer passed to
the barrier. The reserve consumer captures its own global once before its four
slots; Release captures another independent global. Missing-unit removal then
recaptures the global for the recursive reaction scan. These owners must not be
collapsed into the caller's battle pointer. A held reserve removal retains its
item and slot, then clears the slot after any completed source return, including
failure zero. Repeated item IDs in different slots cause separate removals.

A present reaction consumes reservations before checking conclusion. A concluded
result retains the pair. Otherwise Release completes before GetData; the resolved
script and exact allocation operands survive an allocation hold. Event creation
does not repeat Release or GetData. A successful actual event address causes a
fresh event-ID read and unit-ID read at the original post-allocation points.
Null data is still passed to the allocator; the caller does not invent a null
script guard. Allocation failure still returns reaction one and barrier zero.

Release removes all matching nonempty `(unit,key)` pairs, including adjacent
duplicates, shifting each pair's two words in original order and clearing the
tail. It has no external waiting child. No polling generation, fabricated queue
entry, synthesized physical input, or display state is introduced.

## Validation and integration frontier

The focused host test compares successful read/store/provider traces and final
guest state against the existing immediate barrier plus an independently written
synchronous reaction body. Every successful external call is held twice in turn.
It covers live-versus-stale scan cleanup, outer early exit, missing-unit drain,
concluded retention, repeated reserved items, actual allocation failure, null
script, duplicate compaction, callback identity rejection and reentry rejection.
Mutation tests prove independent consumer global capture and that allocation
holds preserve the resolved script while later event/unit IDs remain fresh.
Production and test translation units also pass strict ARM syntax checking.

The parent Turn runtime must bind actual VM event lookup and guest event-address
allocation, the real inventory owner and original conclusion implementation.
Reaction script execution remains owned by that VM. This module alone does not
admit unsupported queued scripts, establish allocation lifetime, or activate the
complete original Main scheduler. Native integration and full build registration
are owned by the parent change; this batch adds only the reusable source owner,
focused test and this contract.
