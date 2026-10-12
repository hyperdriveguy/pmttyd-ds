# Complete smart allocation:header reservation and gap placement

US G8ME01 revision0; ID2,2026-10-12. Complete smartAlloc8002f540/338.
Verify `python3 tools/verify_smart_allocation.py`;read
[retry/failure](SMART_ALLOCATION_RETRIES_AND_FAILURE_OWNERSHIP.md),
[release map](SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md),
[compaction](SMART_HEAP_COMPACTION_AND_PENDING_FILES.md).
Static evidence only;original allocation/source/hardware traces unmeasured.

## Retained request and first-allocation release before header selection

Retain raw requested size S(r3) and tag T(r4). Read WORD
g_bFirstSmartAlloc8041e810. Exact0 stores1 there THEN calls smartAutoFree(3),
ignoring its return. Any nonzero value skips both store and call. No local reset
or dependence on requested tag/size occurs. The auto-release's full3 two-pass
policy is separately documented;this allocator's first-flag admission is global.
Do not infer the flag's startup/reset values without its producer.

Reload wp804151e0 manager W;read word W.e018. Exact0 skips synchronization.
Any nonzero value calls sysWaitDrawSync(),then reloads CURRENT manager and stores
its e018=0. This is not a free-header inventory count:normal smartFree increments
it,while this allocator clears it wholesale after the draw-sync request.
Its role is an observed release/synchronization counter;the platform operation
and concurrency guarantees remain separate. No available-chain count is checked.

## Reserve and unlink the current available head with no null guard

Reload W,capture H=WORD W.e010. Read H18 with no null-H guard. If previous0,
read H14 and store to SAME W.e010;reload W/read its current available head;
if nonnull,store that header's18=0. If previous nonzero,read CURRENT H14,
store to retained previous14.

Reread H14. If next0,reload W/read CURRENT H18,store W.e014=previous;reload W/
read its current tail;if nonnull,store tail14=0. If next nonzero,read CURRENT
H18 and store next18=previous. H is now reserved outside the available chain;
this function does not decrement e018,clear its links immediately or validate
chain consistency. Keep H retained across all later service calls and retries.

## Size alignment and initial header writes precede placement

Compute N=S;when low5 bits nonzero,add32-remainder with32-bit wrap. No signed
positive/nonzero/overflow guard exists. Sizes0,negative words and round-up wrap
therefore retain their literal numeric behavior. Example ffffffff rounds to0.
Then store HALFWORD H0c=1,BYTE H0e=low8T,WORD H04=N,WORD H08=0,in that order.
Payload H00,gap H10 and links are not initialized by this sequence yet.
No requested payload memset or texture/model conversion occurs.

## Leading gap has unsigned first priority

Reload W and compare unsigned W.e004>=N. If true,read SAME W00 arena base,
store H00=base. Reload W/reread leading gap,store H10=currentGap-N with word
wrap. Reload W/read used head,store H14=head;store H18=0. Reload W,clear its
leading gap e004=0. Reload W/read current used head;if nonnull,store head18=H.
Reload W,store used head e008=H. Reread H14;when nonzero,return retained H.

If H14==0,also compute this first header's tail gap:reload W;read heapStart[4]
then heapEnd[4],read SAME W00 base,then H00/H04. Store
H10=(end-start+base-32)-payload-size with word wrap. Reload W/store used tail
e00c=H. Return H. The live head used for previous-link repair may differ from
the earlier head copied into H14 if providers/aliases change manager memory.

## Otherwise choose the first sufficient used-header trailing gap

Starting with the used head from the manager used for the leading-gap comparison,
walk live predecessor P14 until null. At each P,compare unsigned P10>=N. First
sufficient gap wins;there is no best-fit/size-sorted choice,tag/owner/pending check
or mandatory compaction on a successful immediate placement.

For admitted P,read P00 then P04;store H00=payload+size with word wrap. Reread
P10;store H10=gap-N. Read CURRENT P14;store H14=next,H18=P;store P10=0.
Reread CURRENT P14. If nonnull,store that node18=H. If null,reload W/store used
tail e00c=H. Finally store P14=H,return retained H. No new-manager used-head
store occurs on this gap path. Do not snapshot P.next or gap once for all uses.

Both paths split an existing reusable interval into new allocation plus its
trailing gap. They preserve header identity and stale payload bytes;file callers
later assign H08=their entry. No arena allocation or heap-free call is made here.
The [retry contract](SMART_ALLOCATION_RETRIES_AND_FAILURE_OWNERSHIP.md) supplies
what happens only after the entire immediate used-chain scan finds no gap.

## Implementation acceptance and remaining evidence

Check first-flag0/nonzero,flag store before auto-release,nonzero release-counter
sync/reset across manager swaps,available-head null boundary,all unlink cases,
size0/31/32/wrap,low8 tag,header store order,leading priority/first-fit order,
unsigned gap admission,live gap/next/head rereads and tail finalization arithmetic.
Native payload/header/manager addresses need explicit mapping and raw byte
coherence;an independent malloc request does not reproduce this moving arena.

Arena initialization/tag/source domains,type-dependent relocation,draw-sync/
cache/worker/platform execution,scene cadence and original traces remain
EVT-01/JUMP-01/DMG-02 requirements. This complete allocator is not proof of full
memory provisioning or sufficient RAM/performance for the complete game.
