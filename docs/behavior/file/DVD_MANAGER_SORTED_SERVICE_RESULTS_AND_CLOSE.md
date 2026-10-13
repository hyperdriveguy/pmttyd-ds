# DVD manager sorted service, read results and deferred close

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete DVDMgrMain80145024/264hex,
153 instructions; DVD-specific compare80145288/44hex,17 instructions.
Addresses/offsets/masks hexadecimal; counts/arithmetic decimal. Static source
deductions, not original thread/disc/IO observations. Read
[slot/open/global callback](DVD_MANAGER_SLOT_OPEN_AND_GLOBAL_CALLBACK.md),
[synchronous wait](DVD_MANAGER_SYNCHRONOUS_READ_PROTOCOL.md),
[async publication](../audio/GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md) and
[qqsort algorithm](../display/DISPLAY_SORT_IMPLEMENTATION.md).
The compare namespace is dvdmgr.o, not another same-name function in the CSV.

## Snapshot occupied indices, then sort before servicing

Allocate288 decimal stack bytes; save LR/GPR26..31. Request memset800050b4
(localSP+8,0,256) at80145044. Load queue pointer dvdq8041ebf8 ONCE and scan
all256 records, stride98hex/152decimal,64 groups of four unrolled flags90 reads.
Append each raw ordinal low8 to the local byte list iff flags mask8000 is set;
count appended entries. No pending/close/type/SDK-state/priority filter or count
provider. No queue null/extent/generation/locking/IRQ check locally.

Request qqsort800064fc(list,occupiedCount,elementSize1,compare80145288)
at801450e8. Snapshot is indices only, not descriptors/flags/priorities. Preserve
the original game's qqsort protocol/algorithm from the linked contract.

Comparator loads both index bytes, then CURRENT dvdq ONCE for that comparison.
Read each indexed half92 and use LOW8 as key. Return1 if first>second, -1 if
first<second,0 if equal. No occupied recheck, secondary key or ordinal tie-break.
Sorting callback/key/base alias/concurrency domains require separate evidence;
do not claim a stable priority snapshot or independent stable ties.

## Service each listed index using fresh queue base and flags

For each sorted byte, reload dvdq and capture D=base+index*98. No occupied8000
recheck against this current row. Initialize local progress flag0; read flags90.
Mask80 SET skips read processing. Otherwise mask1 must be SET for a read.
Failed gates still reach deferred-close handling. No flags2/20/8 exclusion.

Load FULL U16 priority half92 and fullword remaining80 as retained N. Priority
nonzero and SIGNED32 N>262144 cap N=262144. Priority0 never caps. Negative raw
lengths, includingffffffff, are not capped/rejected;0 is not skipped.
Comparator low8 key and full-half cap predicate differ: half92=100hex sorts as0
but admits the cap. No bound/alignment/file-end guard precedes the SDK request.

Read offset84, progress88 and destination7c, then request
DVDReadPrio802a03c0(D+40,destination,N,wrap32(offset+progress),priority2)
at8014515c. D92 does not become SDK priority; final argument is literal2.
No source-field write before this call, callback-null gate or asynchronous
request form here. Actual SDK blocking/results/short-read/cache/error semantics
remain linked/transitive work; this contract pins the game service branches.

## Result exactly-3, other negative, or nonnegative

Exactly rawfffffffd (-3): fresh flags90 clear mask1/store, reread flags OR2/store.
Then read D.word8c; nonnull requests callback(rawSDK -3,D+40) at80145194.
No request count/destination/progress updates, no20/80 error OR and no local
progress flag set. Continue through close handling, then later list items.

Other signed negative: fresh flags OR20/store, reread flags OR80/store. No
callback, clear1/set2, count/destination/progress update or progress flag set.
Mask80 blocks later read servicing while1 remains set; an external clear/recovery
producer is required to resume it. Exact SDK error meaning and original error/
recovery cadence are not inferred here.

Nonnegative, INCLUDING0: fresh remaining80 minus RETAINED REQUESTED N/store;
fresh progress88 plus N/store; fresh destination7c plus N/store, all wrap32.
The SDK returned byte count is NOT used in any of these three updates.
Short/zero positive completion therefore does not locally shorten advancement.
Child/provider mutations can change the fresh bases before these operations.

Reread remaining80; exactly0 clears1 with fresh flags, rereads flags OR2/store,
reads callback8c and, when nonnull, requests callback(raw nonnegative SDK return,
D+40) at8014521c. No accumulated-count/status normalization: first argument
retains this last SDK return. Nonzero remaining skips completion callback/flags.
AFTER this branch/child, set local progress flag1 regardless of remaining count.
No returned-success equality, rollback, payload-validity or complete-byte proof.

## Fresh close then stop after first nonnegative read

After read/skips/result callback, reread D.flags90. Mask8 set requests
DVDClose802a0178(D+40) at80145234; ignore return, then memset(D,0,152)
at80145244. No pending/completed/blocked/SDK-state/null guard protects deferred
close beyond the list snapshot and this fresh bit. A callback can change8 before
this read. Clear includes name/request/callback/flags/SDK-info storage, not an
independent native descriptor release protocol.

Then local progress flag1 ends the ENTIRE sorted-list scan. Flag0 advances to
next index. Consequently at most one NONNEGATIVE read occurs per invocation;
multiple negative reads/close-only operations can precede it. Even an N0 read
returning0 ends scanning. No yield/retry-loop inside this body, measured period
or callback-success-dependent scan rule. Earlier pending slots can be skipped
or retried independently of list creation.

## Fresh global callback and ABI

After no entries, exhaustion or progress break, read global _callback8041ebf4
FRESH. Nonnull requests it at80145270 with NO argument register assignment or
formal argument adaptation. Do not invent a slot/count/result ABI from whatever
volatile registers preceding operations leave. Per-record word8c and this global
pointer have distinct ownership. Callback return not inspected locally.

Epilogue restores GPR26..31/LR and288 decimal stack bytes. No independent status
assignment or global queue reset. Valid callback/stack/SDK/queue/address/alias/
lifetime/concurrency/IRQ domains remain separate; initialization/worker cadence
are not proven by this complete main body.

## Evidence and remaining work

[Manifest](../../../tools/dvd_manager_main_sort_service_evidence.json) pins six
spans/all170 body-word anchors/five direct calls, three indirect call sites and
sort comparator binding. Seven cap/seven result/five advancement/six comparator/
five completion-flag/seven service-gate vectors and callback/publication sites
checked. Generic verifier checks pins/targets, not semantic/provider/runtime
claims. Mathematical source deductions separate from original execution. Fresh
full ELFs/listings/generator stay `/tmp`; no bodies/binary assets committed.

Remaining: DVDMgrInit/Delete/proc_main and actual queue/thread/callback/request/
priority/length/progress/destination/flag/name providers and valid value/extent/
alias/lifetime/IRQ/concurrency/ABI domains; SDK DVDReadPrio/Close/path/fileInfo/
thread/result and original sorted servicing/short-read/error/retry/close/callback/
IO/audio traces. EVT-01/native/full-game remain OPEN.
