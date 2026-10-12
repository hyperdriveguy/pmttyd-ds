# Battle hit, wait and phase event dispatch

US G8ME01 revision 0; recovered 2026-10-12. B evidence here is original US
instruction inspection; symbol names are research annotations. Baseline,
source revision and evidence classes: [ORIGINAL_BEHAVIOR.md](../ORIGINAL_BEHAVIOR.md).
This contract connects [damage application](BATTLE_DAMAGE_APPLICATION.md) to
the [event scheduler](EVENT_RUNTIME.md). It does not establish the contents
of every unit's reaction, death or revival script.

Local disassembly/ELF evidence stays in ignored `artifacts/reference/behavior/`.
The pinned [public unit header](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/include/battle/battle_unit.h)
is a naming aid, not a US layout specification: its wait/damage event offsets
are four bytes earlier than the US accesses recovered below. Use the numeric
US fields, not a direct copy of that incomplete cross-region structure.

## Unit/event fields established by these calls (B)

| US offset | Role |
| --- | --- |
| unit+00 | Unit ID written into a new event's +0x160 owner field |
| unit+18 | Pointer to eight-byte data records, zero-key terminated |
| unit+274 | Stored reaction pattern |
| unit+278 | Full combined damage/event code stored when direct hit dispatch creates an event |
| unit+28c /290 | Wait script pointer / tracked event ID |
| unit+294 /298 | Phase script pointers selected by nonzero/zero selector |
| unit+29c | Most recently created tracked phase event ID |
| unit+2a8 | Another tracked event ID cancelled by direct hit dispatch; role not fully traced |
| unit+2b0 /2b4 | Installed damage script pointer / tracked hit-event ID |
| event+15c /160 | Event ID / unit-owner word |

Offsets in this table are hexadecimal. Script pointers and event IDs are
different kinds of values; event ID zero is the wrappers' no-event marker.
The entry constructor's allocation and ID scheme must agree with that invariant.

## Normal hit dispatch (`BattleRunHitEvent`, 0x8012984c, size0x84)

Inputs are `(unit,code)`. Initialize result ID to zero. If `code&0xff` equals
40, call `BtlUnit_CheckData(unit,46)`. If the helper returns zero, return zero
immediately, without deleting/replacing an existing event or writing the code.
Other low codes do not perform that lookup.

If dispatch is allowed and unit+0x2b0 is nonnull, call
`BattleRunHitEventDirect(unit,code,installedScript)` and return its ID.
If the installed script is null, return zero with no direct-dispatch side
effects. High code bits do not change the low-code40 gate.

`BtlUnit_CheckData`, `0x801272fc`, size0x40, reads the pointer at unit+0x18.
A null pointer returns zero. Otherwise, inspect the first word of each
eight-byte record in order. A zero key ends the search and returns zero;
a key equal to the requested word returns one. Advance eight bytes after
each unmatched nonzero key. The second word is not examined, so a present
key46 whose associated value is zero still enables code40. No array-length
or missing-terminator check occurs here.

## Direct hit dispatch (`BattleRunHitEventDirect`, 0x801298d0, size0xc4)

Inputs are `(unit,code,script)`. Before checking whether script is null,
perform the following sequence for unit fields +0x2b4, +0x290, +0x29c,
+0x2a8, in exactly that order:

1. Read the field's event ID.
2. If nonzero, call `evtDeleteID(id)`.
3. After the call, clear that unit field to zero.

Deletion may recursively delete child/brother events and copy local values
back to their parent; it is not just a detached task cancellation. The ID
fields are read as the sequence progresses, not snapshotted together first.

If script is now null, return zero. In particular, direct null-script dispatch
still cancels tracked work, but leaves unit+0x278 unchanged. The normal
dispatcher with no installed script never reaches this path.

Otherwise call `evtEntry(script,10,0x20)`. Write the unit ID to the returned
entry's +0x160, write the full supplied code to unit+0x278, read the new event
ID from entry+0x15c, store it at unit+0x2b4 and return that ID. No child-parent
link or wait-for-completion operation is requested by this wrapper. It assumes
the event constructor returned a valid entry, with no null-result check.

Direct dispatch bypasses normal hit dispatch's low-code40/data-key46 gate.
It also does not itself compute damage, apply statuses, clear the reaction
pattern or execute the replacement script synchronously.

## Scheduling and deletion dependencies checked in US code (B)

`evtDeleteID`, `0x80036664`, size0x9c, selects the active field/battle context,
scans every slot at stride0x1b0, and calls `evtDelete(entry)` for active entries
whose +0x15c ID matches. It continues scanning after a match. A stale ID with
no matching active entry causes no deletion; the battle wrapper still clears
its tracked field. ID matching is in the current context, not a global scan of
both contexts. Recursive deletion/copy-back is documented in EVENT_RUNTIME.md.

`evtEntry`, `0x800374ac`, size0x28c, scans the current context for an inactive
slot, zeros the entry and stores low-byte `(entryFlags|1)` at +8, priority at
+0x0b, a new ID at +0x15c and initially −1 at +0x160. Hit dispatch later
overwrites that owner field. The routine initializes script state and scans
labels, then applies `evtEntryRunCheck`; it does not call the command executor
to run the script body in this constructor.

At `0x800376c8`–`0x80037700`, if an event-manager pass is active and entry
flag0x20 is set, append the slot and its ID to the current schedule and increment
the schedule count. Thus a hit event created during a pass can become eligible
later in that same pass, subject to stop/type policy and scheduler checks.
Creation order and current schedule position matter; do not implement this as
an immediate nested call or universally defer it to the next tick. Exact
capacity-exhaustion policy and all constructor initialization fields remain
outside this contract; valid resource capacity is an unresolved caller invariant.

## Wait and phase wrappers (B)

`BattleRunWaitEvent`, `0x801296e0`, size0x6c:

1. If unit+0x290 is nonzero, delete that ID then clear it.
2. If wait script at +0x28c exists, call `evtEntry(script,10,0)`; assign
   unit ID to event+0x160 and store event ID into unit+0x290.
3. Return zero regardless of whether an event was created.

The absent 0x20 flag means this wrapper does not use evtEntry's same-pass
schedule append. It only replaces the tracked wait event, not the four slots
cancelled by direct hit dispatch.

`BattleRunPhaseEvent`, `0x801297bc`, size0x90, receives `(unit,selector)`.
For nonzero selector use script+0x294; for zero use script+0x298. A nonnull
script is passed to `evtEntry(script,10,0x20)`. If an entry exists, set its
unit owner, store its ID at unit+0x29c and return that ID; otherwise return
zero. This function does **not** first delete the previous tracked phase ID.
If no entry is created it leaves the old field unchanged. Repeated calls can
therefore overwrite the tracked ID without this wrapper cancelling an earlier
event; caller sequencing/lifetime must be recovered rather than presumed.

## Event-service argument semantics (B)

All four services below return event completion code2. For their unit argument,
each performs `evtGetValue(event,arg0)` → `BattleTransID` →
`BattleGetUnitPtr(globalBattle,id)`. They assume a valid resolved unit.
Other arguments deliberately differ in whether they are resolved or raw:

| Service / US address | Arguments after unit and behavior |
| --- | --- |
| btlevtcmd_SetEventDamage /0x8010a55c, size0x5c | Copy **raw arg1** into unit+0x2b0. No evtGetValue on this script operand; no event launched or cancelled. |
| btlevtcmd_RunHitEventDirect /0x8010f174, size0x8c | Read **raw arg1** as code; resolve arg2 as script and arg3 as pattern through evtGetValue. Store pattern at unit+0x274, then call direct dispatcher with `code|0x100` and resolved script. Discard returned ID. |
| btlevtcmd_CheckDamageCode /0x8010f2e8, size0x8c | Read **raw arg1** as mask. Write 1 to output operand arg2 when `(unit+0x278 & mask)!=0`, otherwise0, via evtSetValue. This is any-bit overlap, not equality or an all-bits test. |
| btlevtcmd_GetDamageCode /0x8010f374, size0x70 | Write `unit+0x278 & 0xff` to output operand arg1 via evtSetValue. High flags are omitted. |

The full code and high flags remain in the unit record, not a private snapshot
in each reaction event. A later dispatch may change that shared value. Input
resolution must follow each service's instructions: resolving all operands
uniformly would misinterpret raw pointers/masks and encoded variable operands.

## Verification obligations and remaining recovery (P)

Derived cases: code40 with absent/present data key and zero associated value;
null installed script vs direct null script; stale IDs and recursive deletion;
four-slot cancellation order; full code preserved vs low-byte getter; zero,
single and multi-bit masks; resolved vs raw arguments; same-pass hit insertion
vs wait insertion; phase replacement that leaves the earlier event alive.
These are proposed differential cases, not original execution tests already run.

Next evidence must enumerate script installations and their relocated data
bindings for all units, decode reachable reaction/death/revival event graphs,
recover remaining event services, and verify turn-controller wait/lifetime
rules. No conclusion about actual item consumption, revival HP, enemy removal,
death animation completion or battle victory follows merely from launching a
script with a lethal code. Those remain required for the complete-game goal.
