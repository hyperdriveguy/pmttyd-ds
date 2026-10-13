# MusyX macro scheduler and scoped command dispatch

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete macHandle802838b4/154hex and four
complete macHandleActive regions: fetch80282b94..80282c08, selector stubs
802835e8..802836d8 and80283798..802837b8, join/exit80283894..802838b4
(all ends exclusive). macHandleActive80282920/f94hex is NOT otherwise closed
by this document. Addresses/offsets/masks hexadecimal; counts/indices decimal.
Static source scheduling, not original callback/tick/audio measurements.
Read [macro lists/activation](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md),
[selector handlers](MUSYX_MACRO_INPUT_SELECTORS_FIRST_USE_AND_DIRTY.md) and
[sample-end event restoration](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md).

## Complete wait-queue admission and captured successor

macHandle(D) retains raw elapsed word D and loads timeQueueRoot8041ef18 once.
For each current nonnull V, read deadline LOW V.word9c, macro-clock LOW8041ef14,
deadline HIGH V.word98, clock HIGH8041ef10. The borrow/carry sequence compares
these as unsigned64-bit pairs. Deadline<=clock admits; a future deadline ends
this wait scan immediately. No traversal past a future head to find later due
nodes, sorted-list repair or atomic timestamp snapshot. Later heads reread clock.

For an admitted V: capture next=V.word44 BEFORE requesting macMakeActive(V).
After return store retained deadline LOW at V.worda4, then retained deadline
HIGH at V.worda0. These are the DEADLINE values read before the child, not a
fresh current clock. Advance to captured next regardless of list mutations.
No state/identity/generation/null-member/count/cycle/active/IRQ guard or child
success test. The compare's subfe using r30 against itself cancels that value;
it is not another uninitialized-register input to the timestamp comparison.

Activation can modify active/time lists as documented separately. Newly active
nodes are visible to the next phase's fresh active-root load. Wait root is not
reread to restart/reorder the scan; captured successors determine this pass.

## Complete active-list event check and interpreter request

After wait scan, load activeRoot8041ef1c fresh. For each current V:
read V.byte68 and capture active next=V.word3c BEFORE event/child work.
Initial event admission requires byte68!=0 and V.word54!=0. Then read word114
and word118; only word118 bit20hex suppresses the event path (word114 is masked
with0). If admitted, read V.wordf4.low8 and request hwIsActive(index).
Returned nonzero bypasses restoration; returned0 rereads V.byte68 and V.word54.
Only if both remain nonzero perform:

- V.word38=V.word60.
- V.word34=fresh V.word54.
- V.word54=0.
- Request macMakeActive(V); ignore result.

No post-hardware flag20 recheck, full voice identity check or restoration if the
fresh event fields were cleared. The first source gate and post-child gate are
not one captured snapshot. Regardless of event path, request macHandleActive(V),
ignore its result, then advance to captured active next. Newly inserted active
nodes are not independently enumerated; removed/changed captured successors
are not validated. No overall node-count limit or cycle repair.

This event restoration shares raw fields with sample-end handling but has a
hardware-state/flag20 gate of its own. Do not substitute that other handler or
invent an original elapsed wait for these local requests.

## Complete macro-clock tail

Only after BOTH scans/interpreter requests, read macro-clock LOW8041ef14 then
HIGH8041ef10. Add raw D to low32, carry into high32; store LOW then HIGH.
Equivalent valid arithmetic is wrap64(oldClock+U32(D)). Even empty roots reach
this tail. There is no per-rendering-frame increment, D unit normalization,
clock equality check or synthRealTime synchronization request here.
Children may alter clock before these fresh tail reads. No independent status
or atomic64-bit publication; original scheduler/audio tick relationship remains
an execution/provider requirement.

## Complete dispatch fetch region and command-attempt budget

After the still-open macHandleActive initialization phase reaches80282b94,
set DebugMacroSteps8041ef20 byte=0; retain synthInfo pointer803f8540 and shared
cstep8041ef08 (two words), not a per-voice command copy.
At EACH loop attempt: read DebugMacroSteps, increment, store low8, and test
that low8 unsigned>32. If greater, exit BEFORE reading the next command.

With no counter mutations, attempts1..32 fetch/dispatch; attempt33 leaves the
counter33 and exits. The voice PC remains at the next command. There is no
independent hidden instruction-count bound when other writers alter this global
byte:255 increments/stores0 and admits. Do not describe it as a rendering-frame
or whole-active-list cap.

For an admitted attempt, read PC=V.word38; initialize local stop marker0.
Load PC.word0/store cstep.word0, load PC.word4/store cstep.word4, THEN store
V.word38=PC+8. Fetch shared cstep.word0 fresh; opcode=word.low7. Opcode0..113
selects one of114 original targets at803bbc98, stride4. Opcode114..127 skips
handler dispatch and joins with stop marker0, so normally continues to the next
attempt. Bit7 and other high word bits do not change opcode selection.
No code-pointer/null/alignment/command-size/opcode validity/active/IRQ check,
PC rewind, decoder error or unknown-opcode stop is added.

The two-word shared command cell and PC advance precede handlers. Nested or
concurrent interpreter calls can mutate it; no isolated snapshot or ownership
policy is established in this region. Other targets remain open, even though
the table fingerprint/target list is pinned.

## Complete recovered-selector opcode stubs

Each listed stub requests selector(V,&shared cstep8041ef08), ignores any child
return and branches to the common join. It does not copy that return into the
stop marker or initialize evaluator r28. These are exact opcode registrations:

| Low7 opcode | Stub | Selector |
| --- | --- | --- |
| 64 | 802835e8 | mcmdVolumeSelect |
| 65 | 802835f8 | mcmdPanningSelect |
| 66 | 80283608 | mcmdPitchWheelSelect |
| 67 | 80283618 | mcmdModWheelSelect |
| 68 | 80283628 | mcmdPedalSelect |
| 69 | 80283638 | mcmdPortamentoSelect |
| 70 | 80283648 | mcmdReverbSelect |
| 71 | 80283658 | mcmdSurroundPanningSelect |
| 72 | 80283668 | mcmdDopplerSelect |
| 73 | 80283678 | mcmdTremoloSelect |
| 74 | 80283688 | mcmdPreAuxASelect |
| 75 | 80283698 | mcmdPreAuxBSelect |
| 76 | 802836a8 | mcmdPostAuxBSelect |
| 77 | 802836b8 | mcmdAuxAFXSelect |
| 78 | 802836c8 | mcmdAuxBFXSelect |
| 94 | 80283798 | mcmdFilterSwitchSelect |
| 95 | 802837a8 | mcmdFilterParameterSelect |

Join tests local stop marker:0 repeats the fetch attempt; any nonzero exits.
These17 stubs leave its initial0 unchanged. Other opcode bodies can update it
and/or the PC; their individual semantics are not inferred from this join.
The join/restore/return region adds no state/generation/PC check or status.

The selector contracts establish conditional first-use mode0 construction and
post-builder dirty publication. This dispatch contract establishes their opcode/
argument/request registration, not actual command assets/reachable first-use
flags or a universal first-row/r28 invariant.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_macro_scheduler_dispatch_evidence.json) pins one
complete outer body,four complete regions,23 linked fingerprints including whole
macHandleActive,and the114-entry dispatch table:29 spans,158 instruction anchors,
24 direct calls. It records17 opcode/stub registrations,six wait/six clock/
11 opcode/five budget/five stable event-gate vectors. These are deductions, not
original execution. Whole macHandleActive hash alone does not close its omitted
initialization/other command semantics. ABI entry hashes do not describe their
complete save/restore fallthrough chains. Fresh full ELFs/listings stay
`/tmp/inbox-macHandle.elf`, `/tmp/inbox-macHandleActive.elf` and
`/tmp/inbox-macHandleActive.listing`; no original bodies/binary assets committed
or original code executed.

Remaining: complete macHandleActive initialization/other opcode paths and their
children, actual command/code/flag/state/PC/debug-byte/list/time writers and
assets, valid pointer/member/cycle/sorted-queue/alias/reentrancy/IRQ/concurrency/
clock/first-mode/r28 domains and original wait/event/budget/selection/audio
traces. Outer body/scoped dispatch regions are answered; EVT-01 and full native/
source/runtime/hardware equivalence remain OPEN.

### Active initialization follow-up

[Complete active initialization region/last-started bodies](MUSYX_MACRO_ACTIVE_INITIALIZATION_AND_LAST_STARTED.md)
now closes the previously-open prefix through80282b94. Exact pending/default/
flags/variable/clock/job order is pinned; other opcodes and actual source/caller/
shared-domain/original execution requirements remain open.
