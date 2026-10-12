# Event scheduling, ownership and state lifetime

US G8ME01 revision 0; 2026-10-12. Evidence classes and binary identity are in
[the reference baseline](../ORIGINAL_BEHAVIOR.md). B is inspected US bytes,
R is a public reconstruction, P is a required implementation/test contract.

The baseline already specifies sequence/subsystem order, command encoding,
handler result codes, basic waits and USER_FUNC continuations. This file adds
the ownership and ordering behavior required to execute original story scripts.
It does not yet specify every opcode or every event service callback.

## Reconstruction coverage

Pinned [evtmgr.c](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/manager/evtmgr.c)
contains a substantial reconstruction, but its build declaration remains
NonMatching. The US binary is the authority for the facts below. A comment
claiming completion cannot substitute for a matching build or US comparison.

The region distinction also affects structures and pointers: do not reuse a
Japanese RAM address for a US callback or serialized event argument. Original
REL imports must first resolve to the correct module/section/address domain.

## Event identity and storage (B)

The current event context is selected from field or battle state. Each event
entry occupies `0x1b0` bytes. Important US fields observed in the inspected
functions are:

| Offset | Role |
| --- | --- |
| `0x000` | 64-bit accumulated lifetime |
| `0x008` | Active/stopped/waiting flags |
| `0x009`, `0x00a` | Argument count and current opcode bytes |
| `0x00b`, `0x00c`, `0x00d` | Priority, type mask, command-initialized marker |
| `0x014`, `0x018` | Next command and current argument pointers |
| `0x06c` | Parent waiting for this entry |
| `0x070` | Child this entry is waiting for |
| `0x074` | Associated event that owns this brother entry |
| `0x09c`..`0x0d8` | Sixteen local work words |
| `0x0dc`..`0x0e4` | Three local flag words |
| `0x154`, `0x158` | User-work and user-flag storage pointers |
| `0x15c` | Event ID |
| `0x164`, `0x168` | Timescale and execution credit |
| `0x1a0`, `0x1a8` | Restart script and current command pointers |

P: native event handles need generation/identity checking in addition to an
array slot. The manager compares stored scheduled IDs with live IDs before
execution, so simply recycling an index is not equivalent.

## Priority ordering, including ties (B)

`make_pri_table`, `0x80037934`, first appends active entries in ascending slot
order, storing both slot and event ID. It then runs an exchange sort:

```text
for i = 0 .. count-2:
    for j = i+1 .. count-1:
        if priority(slot[i]) < priority(slot[j]):
            swap slot[i], slot[j]
            swap eventID[i], eventID[j]
```

Comparison is unsigned byte priority. The comparison/swap site is
`0x80037a00`–`0x80037a2c`. Higher priority is earlier. Equal-priority elements
are not directly swapped, **but this algorithm is not a stable sort**: initial
slots A(1), B(1), C(2) become C(2), B(1), A(1), reversing A/B. A stable native
sort would change their execution order. This example is a derived algorithm
prediction, not an observed game scene.

The manager rebuilds this table at the start of its pass, then reads the live
table count while iterating. Child creation during a pass appends an entry; it
does not rerun the sorting loop. Therefore initial priority sorting and same-pass
creation are distinct rules. Do not model all events as one immutable per-tick
sorted list if scripts depend on children running in the creation pass.

## Child construction (B)

`evtChildEntry`, `0x80036eec`, searches for an inactive entry and links it to
the parent. At `0x80036f78`, the parent gains wait bit `0x10`; the manager skips
that bit. The new child records its parent at `+0x6c` and clears its own waiting
child and brother association. The parent records the waiting child at `+0x70`.

Child priority is parent byte priority plus 1, stored back as a byte
(`0x80036fbc`–`0x80036fc4`). At 255 the stored value wraps to 0; do not replace
this with saturation if that state is reachable. Type, unit/owner-related state,
user-work pointers and the local work/flag values are copied; the child receives
its own ID, lifetime, command state and execution credit. A copied user-work
pointer is shared storage, while copied local words are separate values.

When `runMainF` is nonzero, `0x800371bc`–`0x800371e8` appends the new slot and
its ID to the current schedule and increments the table count. The construction
then applies entry-run policy via `evtEntryRunCheck` (`0x800371f0`). A new child
can therefore be appended but still be ineligible to run due to stop/type policy.
Do not infer unconditional execution merely from schedule insertion.

## Brother construction and lifetime (B)

`evtBrotherEntry`, `0x80036c04`, establishes a different relationship:
`+0x6c` is null, `+0x74` points to the associated event, and the priority byte
is copied unchanged (`0x80036cb8`–`0x80036cc8`). It copies local words/flags
and shared user-work pointers, but does not mark the associated event waiting.

This is attached concurrent work rather than a child call returning locals.
The deletion procedure finds entries by their brother-owner pointer. Treating
brothers as detached tasks that outlive the owner would leave original scene
work running after cancellation or map changes.

## Deletion and copy-back (B)

`evtDelete`, `0x80036700`, acts only on an active entry. Its order is:

1. If the entry has a waiting child at `+0x70`, recursively delete that child
   (`0x80036748`–`0x80036754`).
2. Scan the active context's entries in slot order; recursively delete each
   active entry whose brother-owner pointer `+0x74` equals this entry
   (`0x80036758`–`0x80036794`).
3. If this entry has a parent at `+0x6c`, clear the parent's wait bit and child
   pointer. Copy all 16 local work words and three local flag words back to the
   parent (`0x800367a4`–`0x8003684c`). Also copy the inspected message/UI-related
   fields at `+0x178`, `+0x17c`, `+0x180`, `+0x184/+0x188`, `+0x18c`, `+0x190`.
   Their full interpretation still requires the message system.
4. Clear active state, zero all `0x1b0` bytes and decrement the active-count
   global (`0x80036888`–`0x800368ac`).

Copy-back happens during this deletion path, not only a high-level successful
script return. Recursive parent deletion can cause child state to be copied
back just before the parent itself is cleared. A port must not invent a
transactional rollback of event locals on cancellation without documenting it
as a behavior change. Brother entries have null waiting-parent links, so their
local changes are not copied back via step 3.

Clearing a parent's wait state does not itself execute the parent. The manager's
remaining schedule position, execution credit and flags govern resumption.
This avoids an incorrect immediate recursive call into the parent from deletion.

## Stop and restart policy (partly B)

`evtStopAll` (`0x80035c68`) and `evtStartAll` (`0x80035bd4`) iterate active
entries in the selected context and pass the supplied type mask to the respective
per-entry function. They do not delete the events or reset command state.

`evtStop` (`0x80036210`) traverses waiting-child and associated-brother
relationships, then tests each entry's type byte against the mask. For a nonzero
intersection, it sets stopped flag `0x02`; the root's final test/write is
`0x800365b8`–`0x800365cc`. `evtStart` uses the corresponding mask intersection
and clears that flag (`0x800361e4`–`0x800361f8`). This is a bit flag, not a nested
pause reference count. A start call cannot be interpreted as balancing one of
several independent pause owners without additional policy above this layer.

Stopped events are excluded before lifetime and execution credit are incremented
in the manager. Basic WAIT_MSEC uses event lifetime; pausing through this path
does not let that timer simply continue on an unrelated host wall clock.

`evtRestart` has been fingerprinted and disassembled but its reset/preservation
contract is not yet fully documented. Do not conflate start/unpause with restart.
The [system-level/speed follow-up](EVENT_SYSTEM_LEVEL_AND_SPEED.md) now documents
the full local level transition table,getter mask and speed initialization policy.
Caller request/type ownership and complete UI/pause sequences remain open;those
are still required to reproduce menus,cutscenes and battle interruption.

## Variable namespaces and persistence boundary

R `evtmgr_cmd.c` identifies local/global work, local/global flags, saved-work
bytes/flags, user-work/user-flags and encoded numeric/pointer values. US
`evtGetValue`/`evtSetValue` are fingerprinted, but the full numeric tag ranges,
float coercions and every opcode's access mode have not yet been independently
specified. Do not replace all arguments with plain signed integers or assume
SET, SETI and SETF are equivalent.

The actual saved-work storage access is documented separately in
[STATE_AND_INVENTORY.md](STATE_AND_INVENTORY.md). It is distinct from the
per-event local words copied during child return and from the field/battle
context-global work arrays. This distinction is necessary for scene branches,
area reloads and eventual saves.

## Required conformance cases (P)

Use original traces or direct execution of the original routines to validate:

- The A/B/C priority example above, plus equal priorities without a higher
  element, deleted/reused slots, and priority changes during a pass.
- A parent creating a child that completes immediately; a child waiting one
  invocation; child creation when stopped; child priority wrapping at 255.
- Child local-word copy-back versus shared user-work pointer mutations.
- Owner deletion with child, brother, nested child and nested brother; exactly
  which message fields survive before final parent cleanup.
- Two stop masks followed by one start mask, field/battle context changes, and
  WAIT_MSEC across stop/unpause.
- Zero and one frame waits; same-invocation USER_FUNC completion versus blocking;
  loop/branch nesting, labels, switch cases and every opcode used by shipped data.

The static verifier checks selected sort, wait-link, copy-back and scheduling
anchors, not these behavioral traces. Until the missing opcode and service
contracts are closed, original event inventory is not a full story interpreter.
