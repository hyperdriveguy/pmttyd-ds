# Retained Act sequence

[Act](../../source/battle/battle_act_continuation.cpp) owns one invocation of the
original action-sequence parent. Its source PC, actor/event pointers, selected
script, wait ID and announcement aggregate survive unavailable children. A hold
does not redispatch from a newly written channel-6 word. Callback identities/B
stay fixed; actual guest allocation and child lifetimes remain caller-owned.
Reset abandons metadata without rollback. See the
[complete source contract](../behavior/battle/RETAINED_ACTION_SEQUENCE.md).

The fifteen-entry table subtracts 0x06000000 unsigned; indices outside 0..14
complete locally. `true` means this source invocation completed, not a complete
attack, battle or requested render delay. An engaged child result zero is real
work, including Input zero and event allocation failure. Only `nullopt` holds.
Requests capture exact function/site/operands once. Children retain their partial
work, while completed RNG, event, message and reserve effects never replay.

| State | Important local boundary |
| --- | --- |
| 0 | Unit, Nice, AudienceInit; ordered status 1/2/9/27 and kind/attribute routing; select 1/3/4/5 and return |
| 1 | Camera speed > original 2.0 returns; equality or unordered NaN selects 2 and falls through |
| 2 | Actual CommandInput zero returns; nonzero clears battle flag 0x80, selects 6 and returns |
| 3 | Publish U+0x2ac=1 and U+0x1c flag 0x04000000, select 6 and return |
| 4,5 | Actual UnitData keys 3/2, U+0x2ac=0x1b/1, select 6 and return |
| 6 | Independent event selection/creation, owner/ID writes and wait-event deletion; increment and return |
| 7 | Barrier, increment, message adjustment, then state-8 barrier |
| 8 | Barrier, both message pumps, then state 9 only when full-word OR is zero |
| 9 | Actor bookkeeping and reserve consumption, flag clear, AudiencePerAct, then state 10 |
| 10 | Low-byte audience reaction, conclusion or Break Slot reaction, then state 11 |
| 11 | Separate message/announcement drain, then state 12 |
| 12 | Barrier and independent post-event creation/deletion, then state 13 |
| 13 | Fresh actor/post-event ID query; existing event returns, otherwise select 14 |
| 14 | Actual final barrier; increment Move channel 5 only when ready |

State 0's first actor serves initial gates. Kinds 222 and 224..230 get status
queries; other kinds select state 3. Unit attribute 0x10 selects state 4; kind
228 additionally obtains fresh Mario for that attribute. Signed byte +0x12b
selects state 5. State 6 deliberately looks up the actor again after optional
WindowOff. Its zero-gravity branch has priority, followed by action flag 0x10,
flag 2 and default status/random logic. Only the flag-0x10 script branch checks
its script pointer for zero. Electric status 6 still calls Random(100) and tests
the signed result <0; do not remove that RNG effect using an assumed range.
Confuse status 5 independently tests <50, possibly setting unit flag 0x10 and
choosing its alternate script.

A created event uses actual `(script,10,0)`. Nonzero event E publishes unit
+0x2a8 from E+0x15c, then E+0x160 from fresh unit ID. Only afterward is unit
+0x290 captured for deletion; clearing follows acknowledgment. A null allocation
skips these writes and advances locally. State 12 has its own analogous post-
event pointer/ID at +0x2c8/+0x2cc; state 13 obtains the actor again before polling.
There is no synthetic root or readiness inferred from a missing handle.

State 9 may have a null actor and still runs reserve consumption and PerAct.
For a real actor it clears the source actor flag, optionally sets status
`(16,1,-99)` before a fresh flag clear, and normalizes signed mode 5->6, 6->7,
7/9->0, 8->9. Fresh mode 3/4 is cleared but skips count consumption on this pass.
The otherwise unused battle-flags read on the zero-mode route remains observable.
Status flag 0x02000000 bypasses decrement through its real Clear child; otherwise
the move byte wraps. Battle flag 0x2000 forces zero. Exhaustion selects mode 10,
preserves the turn byte once through unit flag 0x2000 or clears it, and may publish
marker 96 after the exact belong/phase gate. No clamp or whole legacy consumption
helper replaces these stores.

Messages and Announcements always both execute before their full-word OR test.
Reaction helpers instead consume only the low byte. Those distinct return domains
and the separate state-7/state-8 barriers are part of the source cadence. Original
Main/Turn scheduling, real event execution, effect/audio/GPU owners and normal
battle completion require their own bindings.
