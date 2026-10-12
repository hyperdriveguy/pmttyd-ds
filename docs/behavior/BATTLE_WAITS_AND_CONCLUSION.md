# Battle event barriers, queued reactions and conclusion checks

US G8ME01 revision0, recovered 2026-10-12. B evidence is original instructions;
names are public symbol annotations. See [baseline](../ORIGINAL_BEHAVIOR.md),
[event ownership](EVENT_RUNTIME.md), [dispatch](BATTLE_EVENT_DISPATCH.md) and
[turn recovery](BATTLE_TURN_RECOVERY.md). No original execution or Switch
comparison was performed. Original disassembly remains ignored.

## Unit-event barrier (B)

BattleWaitAllActiveEvtEnd_NoBgSetEndWait at0x8011abb4,size0x134 receives battle
pointer B. Start result=1 and scan unit slots0..63 via BattleGetUnitPtr(B,slot).
Skip null units; there are no HP, allegiance or status gates.

For each unit, examine these event-ID words in order:
unit+0x29c,+0x2a8,+0x2b4,+0x2bc. For every nonzero word, call evtCheckID(id).
If nonzero, set result=0 and retain the ID. If zero, clear the stored field.
Continue checking the remaining fields and units even after a live ID was found.
This barrier therefore performs cleanup as well as a readiness test. Zero IDs
do not call evtCheckID. Fields not in this list, including wait-event+0x290,
are not checked by this helper.

If any live event was found, return0 after the complete scan. Otherwise call
BattleAfterReactionMain(), using that helper's global battle state rather than
passing B. A nonzero helper result changes this barrier's result to0; a zero
helper result leaves it1. The named barrier's return1 can therefore follow
queue processing, reserve-item consumption and group-result writes; it is not
necessarily side-effect-free.

The outer BattleWaitAllActiveEvtEnd(0x8011ace8,size0x104) first calls this unit
barrier. If0, it returns immediately. Otherwise it checks five battle event IDs
in order at B+0x182b4,+0x182c4,+0x180fc,+0x182c8,+0x182c0, clearing finished IDs.
Unlike the unit barrier, it returns0 immediately on the first live battle ID.
This exact outer protocol is also recorded in the turn recovery contract.

## After-reaction queue storage and insertion (B)

The global battle object contains64 pairs at+0x16f5c. Each pair is two words:
unit ID and data-table key. Unit ID−1 marks an empty slot. QueueInit at
0x800f7af0,size0x38 writes(−1,0) into all64 pairs, without deleting any events.

BattleAfterReactionEntry at0x800f7a78,size0x78 receives(unitID,key). First scan
all64 slots: if any nonempty slot has the same unitID, return immediately,
regardless of its key. A second entry for the same unit with another key is
ignored rather than replacing the old key or adding another reaction.
Otherwise find the first empty slot and write the pair; a full queue silently
returns without a write. There is no unit existence or data-key validation.
No boolean success return is computed for callers to consume.

The event wrapper btlevtcmd_AfterReactionEntry at0x80105a78,size0x68 resolves
both arguments with evtGetValue and translates only unit ID via BattleTransID.
Then it calls queue insertion and returns2 (command completes). It does not
block until the queued reaction runs or report queue saturation.

## Removal and compaction (B)

BattleAfterReactionRelease at0x800f79e4,size0x94 receives(unitID,key), scanning
all64 pairs. Match requires a nonempty ID and equality of **both** words.
For a match at index i, move each following pair one slot left, fill final
slot63 with(−1,0), decrement the loop index, then continue scanning.
Thus it removes all matching pairs, including adjacent duplicates if present
through another writer; it does not stop after the first match. Remaining
pairs preserve their relative order. An unmatched request leaves storage intact.
Insertion normally prevents duplicate unit IDs, but removal's exact behavior
still matters when loading or constructing state through other code.

## Reaction dispatch at an idle barrier (B)

BattleAfterReactionMain at0x800f78fc,size0xe8 uses global battle state. Scan the
64 queue slots in order until the first nonempty pair(unitID,key):

1. Resolve its unit with BattleGetUnitPtr(globalBattle,unitID).
2. If absent, release the pair and recursively call BattleAfterReactionMain;
   propagate that call's return. Several missing units can be removed during
   one barrier check. Reserve-item consumption is skipped for missing pairs.
3. If present, call BattleConsumeReserveItem() **before** checking conclusion.
4. Call BattleCheckConcluded(globalBattle). If nonzero, return0 while retaining
   this queued pair; no data-event creation or release occurs on that path.
5. Otherwise release the pair before resolving its data value.
6. Call BtlUnit_GetData(unit,key), then evtEntry(returnedValue,10,0). There is
   no local null-script guard between those calls. Event allocator/null-script
   behavior remains the event service's contract.
7. If event allocation succeeded, store event+0x15c at unit+0x2a8 and unit+0
   at event+0x160. Mark return1 regardless of allocation success.

An empty queue returns0. At most one present-unit reaction is attempted per
Main call; missing-unit recursion may consume earlier entries first. Returning1
makes the unit barrier return0, so it will poll the newly stored event on a
subsequent check. Allocation failure still delays readiness for that call.
There is no local cancellation of a pre-existing unit+0x2a8 on assignment;
the barrier had already checked unit handles before this dispatch.

## Reserved inventory consumption (B)

BattleConsumeReserveItem at0x800f74b4,size0x60 reads global battle's four words
at+0x19060,+0x19064,+0x19068,+0x1906c in order. For each nonzero value call
pouchRemoveItem(value), then clear that slot to0 regardless of the callee's
return. Zero slots are cleared without a call. Multiple identical values in
separate slots cause separate calls; there is no deduplication.
The pouch removal routine's exact inventory search/count policy remains a
separate dependency. Do not infer that every request successfully removed an
item merely because the reservation slot was cleared.

## Conclusion group configuration and writes (B)

BattleCheckConcluded at0x8011aa34,size0x180 receives B. It evaluates three group
indices g=0,1,2. Each uses U16 configuration at B+8+8g and writes a word result
at B+12+8g. Each result is cleared to0 before evaluating that group.
Group membership is signed byte unit+0x0c equal to g.

For each group, in order:

1. Configuration exactly0: result=1; increment concluded-group count; skip
   unit scans for this group.
2. If configuration has1, scan all64 slots for the first qualifying group unit:
   nonnull, matching signed membership, CheckStatus(unit,27)=0, attributes
   unit+0x104 lacking0x20000 and0x10000000. If none exists, result=1 and count++;
   skip the configuration-bit2 scan for this group. Otherwise proceed.
3. If configuration has2, scan all64 slots for the first qualifying group unit:
   nonnull, matching membership, attributes having0x40000, CheckStatus(unit,27)=0,
   attributes lacking0x10000000. If none exists, result=1 and count++.

The bit1 scan precedes bit2; with both bits set, each required scan must find
an eligible unit to leave the group result0. They need not find the same unit.
The bit2 test reads attributes before the status call, then reloads attributes
for the exclusion test. The bit1 scan checks status before attributes. Preserve
that call/read order when implementing services that can change state.

Neither scan explicitly tests HP or the AC-eligibility predicate. Do not replace
these masks/status gates with an assumed count of units having positive HP.
Configurations with unrelated nonzero bits and neither1 nor2 leave result0.
Group identity/gameplay meaning and configuration producers remain open.

After all three groups:

- If concluded-group count≥2, return1.
- Otherwise return normalized0/1 according to whether B+0xef4 has either mask
  0x10 or0x20. No writes to that flags word occur in this routine.

The group result words are updated even if the final flags force return1.
This is a conclusion predicate plus state refresh, not the victory/loss event
implementation. Existing callers set their top-level sequence after a nonzero
result; scripts deciding victory, defeat, escape or rewards still require
recovery.

## Differential cases and remaining work (P)

Compare live early unit IDs with later stale IDs: cleanup must still visit all
later fields/units; contrast the outer barrier's early exit. Test queued missing
units, one present dispatch per call, allocation failure readiness, full/duplicate
queue insertion, all-match compaction, same unit/different key, reserved items
consumed before a conclusion retaining the pair, and null data pointer dispatch.
For conclusion test masks1/2/both/neither, separate units satisfying each scan,
status27 versus positive/zero HP, three group result writes and forced flags.
These are derived comparison obligations, not executed-original tests.

Remaining: queue producers and data-key bindings, reservation writers and pouch
removal semantics, evtCheckID/allocator invariants, group configuration producers,
conclusion sequence/rewards, outer turn/action cadence and original execution
comparisons. The local barrier/queue/conclusion contracts are specified; complete
battle and complete-game equivalence remains unfinished.
