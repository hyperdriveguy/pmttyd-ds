# SDK DVD queue pop, check, dequeue and local coverage

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete __DVDPopWaitingQueue802a2ea8/a0hex
(40 instructions), __DVDCheckWaitingQueue802a2f48/58hex (22),
__DVDDequeueWaitingQueue802a2fa0/60hex (24). Addresses/offsets/masks hexadecimal;
counts/arithmetic decimal. Static deductions, not original queue/IRQ/IO traces.
Read [queue clear/push and read submission](SDK_DVD_ASYNC_SUBMISSION_AND_QUEUE_APPEND.md).
These contracts cover all five functions tagged dvd.a dvdqueue.c locally;
actual command engine/list/provider/cancellation behavior remains separate.

## Pop: first nonempty priority is selected under initial IRQ token

Request OSDisableInterrupts80299ad4 at802a2eb8. Scan exactly four heads starting
WaitingQueue80413938, stride8, ascending ordinal0..3. For each H read next word0;
ONLY equality next==H means empty. No null/link reciprocity/state/type/extent/
generation/count validation. First unequal next selects priority ordinal and
skips later heads. Zero/null next is considered nonempty; it is not rejected.

All four empty: restore token via OSRestoreInterrupts80299afc at802a2f2c,
assign0 and return. No head/node writes. Token remains in r3 through scan local
operations; no child intervenes before that restore request.

## Pop restores, disables again, rereads selected head without emptiness check

Found: restore FIRST token at802a2edc, then request disable AGAIN at802a2ee0.
Retained priority ordinal reconstructs H. Read H.next FRESH as F at802a2ef4,
not the earlier observed pointer. No priority rescan or next!=H recheck.
This restore/disable boundary is not proof interrupts were actually enabled;
first token can represent an already-disabled caller domain. Actual intervening
mutation/scheduling must be established separately.

If selected queue became empty between these source reads, F can be H itself;
code still proceeds as below, returns H and clears its sentinel links. A newly
nonempty earlier priority is not rescanned. These mathematical source domains
are not observed original queue states or valid producer guarantees.

Read F.next N0; store H.next=N0. REREAD F.next as N1; store N1.prev=H.
No F.prev use or retained-next snapshot. Then restore SECOND token at802a2f08.
AFTER restore, store F.next=0, set raw return register to F, then F.prev=0.
Head unlink and removed-node clears straddle that restore; no third critical
section or ownership recheck. Child/alias effects can change fields before clears.

Ordinary one-node queue yields H.next=H and H.prev=H, then zeros removed F links.
For a longer valid queue, next node's prev becomes H and tail stays unchanged.
These invariants require initialized/coherent providers, not extra local checks.
Restore GPR31/LR and16 decimal stack bytes; no independent command-state/callback/
count/free/cache operation occurs.

## Check uses the same four head.next equality predicates

__DVDCheckWaitingQueue disables at802a2f54, scans heads0..3 as above, first
next!=H restores at802a2f74 then returns1. All equal restores at802a2f88 then
returns0. No queue mutation, link/state/null/count checks or stored priority.
Null next is considered nonempty. Status is not a stable promise for a later
pop; actual tokens/concurrency/command-engine callers remain open. LR/8 decimal
stack bytes restored; one disable/one selected restore on each completed path.

## Dequeue loads raw neighbors; removed node links survive

__DVDDequeueWaitingQueue(C) retains C; disables at802a2fb4. Load C.prev P and
C.next N BEFORE checking either. No C null/address/state/membership guard.
If P==0 OR N==0, restore at802a2fd0, assign0, return without writes.

Both nonzero: write P.next=N, then N.prev=P, using the retained neighbor values.
No fresh neighbor/reciprocity/priority/head check, predecessor traversal, count
adjustment or heap release. Restore at802a2fe4, assign1 and return.
C.next/C.prev are NOT cleared; no post-restore cleanup or command-state write.
Repeated dequeue can therefore return1 again and rewire through stale links.
A successful local return alone does not prove current queue membership.

Self/alias neighbors can change C fields through the two writes; ordinary
nonaliasing case leaves links intact. Preserve retained-neighbor semantics,
which differ from pop's fresh next reread and post-restore clears. GPR31/LR/
24 decimal stack bytes restored. No callback/ready service request here.

## All local queue functions verified; wider binding remains open

[Manifest](../../../tools/sdk_dvd_queue_consumers_evidence.json) pins five
spans/all86 body words/eleven direct calls and publication/IRQ sites. Five head-
selection/three selected-head reread/five dequeue/one stale-link vectors checked.
[Earlier queue manifest](../../../tools/sdk_dvd_submit_queue_evidence.json)
reverified. Namespace inventory maps all five original queue functions to their
complete-body evidence: clear14/push26/pop40/check22/dequeue24,126 words total.
Each word verified against read-only ISO; no function-name-only coverage claim.

Models are mathematical deductions, not original race/IRQ/IO execution; normal
list examples assume coherent initialized/nonaliasing providers. Generic verifier
checks pins/targets, not prose/providers/runtime. Fresh full ELFs/listings/
generator stay `/tmp`; no bodies/binary assets committed.

Remaining: stateReady/command engine/cancellation and actual head/node/link/
priority/state/token/init/caller/ownership providers and valid value/extent/
alias/lifetime/IRQ/concurrency/ABI domains, SDK thread/path/open/read services
and original order/pop/dequeue/pause/cancel/callback/IO/audio traces.
EVT-01/native/full-game remain OPEN.
