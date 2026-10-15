# Retained event barriers and reaction queue

Implementation: [source](../../source/battle/battle_event_barrier_continuation.cpp),
[API](../../include/battle/battle_event_barrier_continuation.hpp).
Evidence: [retained barriers](../behavior/battle/RETAINED_EVENT_BARRIERS.md),
[reaction queue](../behavior/battle/BATTLE_AFTER_REACTION_QUEUE.md).

All and Unit entry both visit all 64 unit registry slots. Unit omits the five
outer battle event IDs; it is not a one-unit API. Engaged integer 0/1 is completed
source readiness, distinct from unavailable `nullopt`. Reaction's engaged 1 means
attempted reaction dispatch, including completed allocation failure; consequently
a barrier returns not-ready 0 after it. Consume wraps a source void routine: its
boolean means native completion, not successful inventory removal.

A cursor pins entry kind, passed battle, callback/users, memory hooks and audience
lifecycle observer identities. Providers and actual allocations must remain alive;
identity equality alone proves no lifetime. Reentry/reset during invocation is
refused. Reset elsewhere abandons continuation without undoing queue compaction,
item removal, cleared IDs or provider effects. `pending()` is a guest function
diagnostic and may survive acknowledged bookkeeping; `active()` owns native
continuation lifetime.

| Storage | Scan / interpretation |
| --- | --- |
| Unit `+0x29c/+0x2a8/+0x2b4/+0x2bc` | Four stored event IDs, all checked even after live result |
| Battle `+0x182b4/+0x182c4/+0x180fc/+0x182c8/+0x182c0` | Five outer IDs, stop at first live result in this exact order |
| Global battle `+0x16f5c` | 64 `(signed unit ID,key)` pairs, eight-byte stride; ID -1 empty |
| Consumer battle `+0x19060` | Four reservation words, each removed independently |

Event IDs are not guest entry pointers or generation handles. Capture ID and
field before `eventExists` can hold. Clear only acknowledged absent IDs. Unit
scan continues cleanup after live results and has no HP/status/allegiance gates.
If any unit ID remains live, return not-ready before reaction work. Outer scan
instead stops immediately at its first live ID.

Reaction captures global battle independently of passed battle. Consumer captures
another global once before four reservations. Release captures another global
each time. Do not merge these pointers. Missing-unit release then restarts
reaction with a fresh capture. A present unit is retained while reserved items
are consumed before conclusion. Each engaged removal return clears its captured
slot, even failure zero; repeated item IDs cause separate calls. An empty slot
still gets the original zero store.

Concluded true retains the pair. Otherwise Release compacts all matching pairs,
word by word in original order, writes tail `(-1,0)` at queue `+504/+508`, and
revisits the same index to catch adjacent duplicates. It has no waiting child.
The retained unit/key then feeds GetData, even if release observed another global.
Null script still goes to the real allocator.

Allocation uses `evtEntry(script,10,0)`. A hold preserves the resolved script and
does not repeat Release/GetData. Engaged zero is actual allocation failure and
still completes Reaction 1 / barrier 0. Nonzero is a guest entry pointer: read
its fresh `+0x15c` ID into unit `+0x2a8`, then fresh unit ID into entry `+0x160`.
An unavailable allocator must not masquerade as allocation failure.

When no reaction dispatch occurs, Unit returns ready; All checks its five outer
IDs first. The owning VM must supply real event lookup, conclusion, allocation
and inventory work. This module does not admit unsupported scripts or prove their
allocation lifetime. See [Turn caller](battle-turn.md).
