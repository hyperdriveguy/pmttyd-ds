# Inventory removal and deferred battle consumption

US G8ME01 revision0, inspected 2026-10-12. B evidence is original DOL code/data;
public symbols annotate names. See [baseline](../ORIGINAL_BEHAVIOR.md),
[inventory storage](STATE_AND_INVENTORY.md) and
[reaction barrier](BATTLE_WAITS_AND_CONCLUSION.md). No original execution or
Switch equivalence was measured. Original disassembly remains ignored.

## Removal array selection (B)

pouchRemoveItem at0x800d4c38,size0x29c receives signed item ID.
pouchRemoveItemIndex at0x800d4974,size0x2c4 receives(itemID,index).
Both choose the following arrays from the global pouch pointer:

| Item ID | Primary halfword array | Active capacity | Parallel halfword array |
| --- | --- | --- | --- |
| 121..235 | pouch+0x192 | 10 if pouchCheckItem(1)=0, otherwise20 | none |
| 1..120 | pouch+0xa0 | 121 | none |
| 240..338 | pouch+0x1fa | 200 | pouch+0x38a |
| Others | no array | return0 | none |

Range comparisons use signed words. Unlike acquisition, removal does not
special-case coins/hearts/flowers into negative resource deltas. For example,
ID121 selects the carried array; it does not decrement coin count. IDs236..239
are rejected. Stored-item pool+0x1ba is not selected by either routine.
Capacity is selected once before scanning; removing an item that changes later
capacity checks does not resize this in-progress compaction.

## Remove by ID and preserve order (B)

Scan the active primary array from index0 upward, comparing signed halfwords
to the supplied word ID. Missing ID returns0 without array writes.
At the first match i:

1. Clear primary[i] to0; clear parallel[i] if that array exists.
2. For j=i..capacity−2 ascending, copy primary[j+1] to primary[j], then clear
   primary[j+1]. When parallel exists, perform its corresponding copy/clear
   for each j as well.
3. Return1, without scanning for more copies.

This shifts **every remaining active slot**, including zeros, rather than packing
all nonzero entries or stopping at the first gap. Unused storage outside active
capacity remains untouched. In the badge case the parallel entry moves with its
primary ID; deletion clears that slot and shifts the rest. This function does
not otherwise recompute battle badge counters, BP usage, saved acquisition flags,
weapon statistics or UI selection. Those consumers remain separate contracts.

## Remove by index (B)

After the same array/capacity selection, compute pointer=primary+2*index using
word arithmetic, and read its signed halfword **before any index bound test**.
If it differs from itemID, return0. The original has no preliminary negative-index
or index≥capacity rejection.

If it matches, the later signed index<capacity test gates the removal loop;
index≥capacity returns0 after that earlier read. For a valid0≤index<capacity,
remove exactly that slot using the same clear/shift algorithm and return1.
It does not search an earlier copy when the requested slot differs.
Caller index invariants are required; the pointer read and negative-index path
are not a supported public interface for arbitrary untrusted indexes. A portable
adapter must distinguish game-equivalence behavior from boundary validation.

## Reserve consumption callback (B)

btlevtcmd_ConsumeItemReserve at0x8010adac,size0x1ac resolves unit and item
operands through evtGetValue, translates unit through BattleTransID, and fetches
its pointer. The unit is assumed valid. Return2 on every local path.

If kind is222 or224..230:

1. Index itemDataTable at0x803108a8 using40*itemID, without local ID bounds
   validation. Read U16 record+0x10.
2. If it has1, scan the four global battle reservation words+0x19060..0x1906c
   in order. Matching item ID ends the scan without a write. Otherwise the
   first zero slot receives itemID and ends the scan. Full array does nothing.
3. Independently of whether a reservation was added, perform the refund path
   below if unsigned unit byte+0x2fb is nonzero.

The duplicate check precedes the empty check **within each slot**. It does not
scan all slots for duplicates before insertion. In an array with holes, a new
entry can be written before encountering a duplicate in a later slot. Thus
ordinary dense reservations suppress repeated IDs, while arbitrary sparse
state need not. ItemID0 matches the first empty slot and leaves it unchanged.

For other unit kinds, skip the table/reservation/refund path. If word unit+0x308
is exactly151, clear it; otherwise leave it intact. This condition does not
compare the supplied item operand to151. Other held-item semantics remain open.

## Refund amount and presentation (B)

For unsigned byte n=unit+0x2fb>0, read unsigned item record halfword+0x1a as p.
Compute the original single-precision sequence:

`refund = truncToWord(float(float(70+5*n) * float(p)) / 100.0f)`.

Conversions, multiplication and division are binary32, followed by fctiwz;
this is not an unlimited-precision integer-percent calculation. If refund≤0,
skip the rest. Otherwise call pouchAddCoin(refund), get unit position, and call
`effCoinN64Entry(0,0,x,y,z,1.0)`.
The coin effect is emitted even if coin storage was capped and actual gain0.
There is no RNG gate or need for successful reservation insertion on
this path. Field+0x2fb, price-table meaning and refund producers still need data
bindings; do not infer them solely from the helper's name.
Constants at0x804222d8 and0x8042228c are100.0f and1.0f. Pouch coin narrowing/
clamping is already specified in the inventory storage contract.

## Consumption at the reaction barrier (B)

BattleConsumeReserveItem scans the four reservation words in order, calls
pouchRemoveItem on each nonzero value and clears every slot regardless of removal
success. In dense state, repeated calls for the same reserved ID may produce
only one deferred removal. If duplicate IDs occupy different slots, each slot
causes a separate removal call, so multiple copies can be removed.

The [reaction dispatcher](BATTLE_WAITS_AND_CONCLUSION.md) consumes reservations
when it encounters a present-unit queued reaction, before its conclusion check.
Missing-unit reactions skip consumption; an empty queue does not trigger it.
This call site alone does not establish the full inventory-consumption schedule:
other callers, item scripts, cancellation and battle teardown must be inventoried.
Refund timing is in the reservation callback, distinct from deferred removal.

## Differential obligations and remaining work (P)

Compare first/middle/last removal, duplicate IDs, internal gaps, parallel badge
entries,10/20 capacity, excluded ID ranges and ID121 versus coin storage.
For indexed removal compare a matching slot and a different duplicate elsewhere;
trace valid caller indexes before implementing the original unchecked accesses.
For reservations compare repeated dense entries, holes preceding a duplicate,
full queue with refund, zero price, coin cap, noneligible kinds and held-item151.
These are derived comparison cases, not observed original execution.

Remaining: all reservation/consumption callers and initialization/teardown,
item record bindings, pouchCheckItem details, badge parallel-array consumers,
refund/held-item producers, item execution/cancellation and coin effects.
Complete inventory and battle equivalence remains unfinished.
