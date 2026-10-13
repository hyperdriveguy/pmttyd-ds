# MusyX synthesis job insertion and start timestamps

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete synthAddJob80278618/150 and
synthStartSynthJobHandling80278768/64hex. Read
[forced jobs/mac activation](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md).
Offsets/addresses hexadecimal; category/slot counts explicitly decimal.
Static queue operations, not physical callback or simulation timing evidence.

## Slot calculation and embedded node layout

Inputs synthAddJob: retained voice/base pointer V(r3), RAW category C(r4), raw
unsigned delay word D(r5). Read synthJobTableIndex8041ee61 as byte J; compute
slot=(J+(D>>8))&31. Low8 delay bits are discarded, high24 added before mask.
No rounding, wide time comparison, active/IRQ gate or delay/caller tick conversion.
Table synthJobTable803f8d94 has180hex bytes:32 slot records,12 decimal bytes each.
Each slot contains three independent node-pointer roots.

| Exact category | Embedded node | Root within slot |
| --- | --- | --- |
| 0 | V+0 | word0 |
| 1 | V+c | word8 |
| 2 | V+18 | word4 |

Node fields: next word0, previous word4, existing slot byte9 (ff means unqueued).
Only exact categories0/1/2 establish these pointers. Signed C<0 or C>=3 branches
fall through to common insertion WITHOUT initializing node/queue pointers: r4
still holds raw category and r7 is not assigned by this body. No guarded invalid-
category return, default category or host-safe fallback is recovered. These types
are a real source precondition; an unsupported API call is not a no-op.

## Category0/1 duplicate and unlink behavior

Read node.slotByte9. ff skips unlink. Equal to computed slot returns immediately,
without changing node/root fields. Every other byte unlinks from its OLD slot:

1. Read node.next; if nonnull, read node.previous and write it to next.previous.
2. Read node.previous FRESH; if nonnull, read node.next and store previous.next.
3. If previous null, read slot byte fresh, form old slot's corresponding root,
   read node.next fresh and store that root=node.next.

No whole-node clear or source membership/cycle/index validation. Old slot byte
32..254 can address beyond the supplied32-slot table; ff alone is exempt.
Category0 and1 use different embedded links/root fields; do not merge them into
one list or prevent one category because another is queued.

## Category2 and common prepend publication

Category2 reads its byte9. Any value OTHER THANff returns immediately, regardless
of target slot. It NEVER unlinks/reschedules an existing node in this body.
ff admits new insertion. Category0/1 also converge after their conditional unlink.

Common insertion, exact order:

1. Store computed slot to node.byte9.
2. Capture current target root, store node.next=capturedRoot.
3. If captured root nonnull, RELOAD target root and write its.previous=node.
4. Store node.previous=0; store target root=node.

Own old next/previous values are overwritten only in this phase. No root-last
membership proof, job execution, result/status assignment, allocation/free,
active-system/pointer/count/IRQ guard or queue consumer tick advancement occurs.
Unsupported pointer/alias/concurrency/cycle domains remain explicit prerequisites.

## Complete initial timestamps and two requests

StartSynthJobHandling retains V. Read synthRealTime8041eea0 high then low;
store low to V.word28, then high to V.word24. RELOAD both global words, high then
low; store low to V.word30, then high to V.word2c. The two pairs can differ under
unsupported concurrent mutation; no atomic64-bit capture or time increment.

Then request synthAddJob(V,0,0), followed by synthAddJob(V,1,0); no result checks
or separate success assignment. Current slot is recomputed independently by each
request. Existing same-slot nodes cause the category0/1 early returns above;
initializing timestamps does not guarantee a new queue insertion or execution.
No category2 request, node-marker init, old-job flush or emitter/voice activation.
The earlier ForceLowPrecisionUpdate wrapper requests the same pair without these
initial timestamp stores; preserve that distinction.

## Verification and remaining source/runtime

[Manifest](../../../tools/musyx_synth_job_insertion_evidence.json) pins both
complete bodies:2 spans/75 anchors/two resolved direct calls, preserving original
branches/width/root/publication/timestamp requests.
Derived21 slot cases, three category mappings, nine duplicate/unlink cases and
two time-field maps are static deductions; original code not executed. Full fresh
ELFs/listings stay `/tmp/inbox-synthaddjob.elf` and `/tmp/inbox-startjobs.elf`.

Remaining: table/index/node-marker/time initialization and consumer advancement,
valid category/member/index/null/alias/IRQ/concurrency domains, actual job handlers/
macro/voice scheduling and original queue/time/audio traces. Two complete local
services are answered; full source/native/runtime/hardware remains OPEN.
