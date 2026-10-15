# Retained original Phase and Move controllers

US G8ME01 revision0 read-only DOL evidence; source inspection, not an original
execution observation. This implements complete `btlseqPhase8011c2b4/9c` and
`btlseqMove8011bc24/690`. Related source maps are [phase tables](BATTLE_PHASE_TABLES.md),
[Move](BATTLE_MOVE_CONTROLLER.md) and [Act](RETAINED_ACTION_SEQUENCE.md).

`battle_phase_move_continuation.hpp/.cpp` owns every parent state, scan, write,
immediate transition and real Phase→Move composition. `BattleMoveServices.call`
requests exact original children using their function address and retained scalar
arguments. An engaged0 is a completed original child result, including a barrier
not ready on this simulation invocation. `nullopt` is an unavailable provider:
the same child request and all acknowledged parent effects remain retained.
No generic whole-function retry, inferred handler, fabricated event or legacy
handoff counts as a completed original child.

## Source invocation boundaries

Phase channel4=04000000 increments4 and resets5, then returns. Values04000001
through04000005 call actual Move;04000006 increments3 and resets4, then returns.
Other values do nothing. Move's15-entry table80363c8c dispatches these states:

| State | Parent-owned work and source return boundary |
| --- | --- |
| 0 | PhaseFirstProcess; increment5 and immediate redispatch |
| 1 | increment5 and immediate redispatch |
| 2 | original pouch/Mario/Majinai gate and event writes; direct state3 barrier, or skip by two increments and redispatch |
| 3 | event barrier; source0 returns, nonzero increments5 and redispatches |
| 4 | scan all64 registry slots; launch every nonnull selector1 phase script; increment5 and redispatch |
| 5 | barrier as state3 |
| 6 | scan64 phase-event pairs; launch one selector0 script, mark only its ID−1, increment5 and return; none selects10 and redispatches |
| 7 | barrier then conclusion; end battle or set5=6 and redispatch |
| 8 | clear exact action bookkeeping; select actor, set5=9/S6=06000000; deferred-state cleanup and Mario command flags; return |
| 9 | actual Act8011b2dc; return |
| 10 | conclusion before cleanup; reset unit total damage and selection bits, phase-end predicate/forced flag; set5=8 and return, or audience phase child then increment5 and return |
| 11 | audience reaction lowbyte test then barrier; increment5 only if ready and return |
| 12 | MsgMain then AnnounceMain unconditionally; both words0 increments4/resets5, then return |
| 13 | increment5 and return |
| 14 | increment4/reset5 and return |

State8 does **not** fall into Act on the same invocation. Instruction8011bc3c
clears the immediate flag. State8's nonMario8011c058 and Mario8011c10c/11c
branches all reach8011c298 without setting it. State9 calls Act at8011c120/124
on the next invocation. The prior Move contract's contrary statement requires
the accompanying exact documentation correction journal.

## Captures, ownership and effects

Each scan performs original fresh registry lookups. Actor selection captures
signed state before status27; later helper mutation cannot change that captured
comparison. Committing the selected table ID performs another lookup. The
second deferred scan and kind222 branch use their own fresh reads. Mario item
availability uses two independent signed-halfword scans and signed pouch bytes.
No HP/status/count filter is added to selector1 phase events.

The all64-skipped actor-selection path retains the source's unguarded final ID
store/lookup/dereference. A real binding must establish the original valid-actor
invariant; this module does not fabricate a no-actor return. Majinai event pointer0
is likewise an actual scalar result, not unavailable. The original subsequent
memory accesses retain their original admitted-memory requirement.

A held cursor pins B, actor memory/user callbacks, child callback/user and the
request object. Replacement or recursive resume is rejected before source
reads; reset during invocation is rejected. Phase pins its original gate and
owns its real Move child across holds. Caller lifetime and actual allocation
admission remain external. Only the simulation owner starts the next completed
source invocation; render frames do not advance these controllers.

## Validation and native boundary

The focused test covers all15 states/out-of-range dispatch, actual source table
and Majinai helpers, full64 scans, exact write widths, state captures and fresh
lookup mutation, event owner/ID order,0-versus-unavailable, lowbyte reaction,
Msg/Announce ordering, state8/11 return boundaries, every representative child
holding twice without replay, provider/B replacement and reentry rejection.
Strict host, ARM GNU++17 and ASan/UBSan runs pass. These are host source-contract
tests, not executed-PPC differential observations.

Ignored evidence `artifacts/battle-phase-move-source-proof.json` pins both whole
bodies, the complete table, source call census and boundary instructions. The
unapplied native journal binds actual source table/Majinai/memory/unit/event
owners and composes the retained Act owner. Remaining audience/announcement
children hold at their actual provider boundary. It does not create an active
Main cadence or claim complete native battle scheduling. The root integrator
owns shared Trio/build changes and whole-project validation.
