# MusyX ARAM sample requests, callback and queue synchronization

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete aramQueueCallback8029163c/a0
and aramSyncTransferQueue802918b8/18hex. The complete
[sample store](MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md) supplies the
low-queue request producer below; aramUploadData/high-queue and initialization
remain separate. Addresses/offsets are hexadecimal; counts/strides labeled decimal.

## Queue layout and sample-store publication

aramQueueLo80410358 and aramQueueHi804105dc each occupy284hex bytes:
16 request slots of40 decimal bytes, followed by byte280 insertion index and
byte281 pending count. Bytes282..283 are not written by these bodies.
Store uses the LOW queue only. Under OSDisableInterrupts80299ad4, read count;
unsigned count>=16 restores saved interrupt state via OSRestoreInterrupts80299afc
and retries disable/check. No timeout/drop/overflow return. A count that cannot
fall below16 causes unbounded retry; progress depends on external completion.

At count<16, repeatedly read insertion index before EACH field address/store.
The sample request writes these slot words in order:

| Offset | Value |
| --- | --- |
| +4 | Owner literal42 decimal |
| +8 | Type0 |
| +c | Priority0 |
| +10 | CPU source retained after flush or upload callback |
| +14 | Retained fresh caller-cache cursor destination |
| +18 | Aligned byte count or chosen chunk length |
| +1c | aramQueueCallback8029163c |
| +20 | User callback0 |
| +24 | User callback argument0 |

Word0 is not initialized locally. Immediately before ARQPostRequest802aa8ec,
source reloads insertion index independently for request pointer and each of
seven arguments. It supplies slot pointer and words+4/+8/+c/+10/+14/+18/+1c
in that order (r3..r10). It does not use one immutable slot snapshot or inspect
a post return. AFTER posting, reload count and store U8(count+1); reload index
and store (index+1)%16. Restore saved OS state afterward. The emitted signed
remainder sequence agrees with modulo16 for all possible byte-index inputs;
normal slot bounds still require index0..15. Index16..255 can address beyond
this queue even though the subsequent index update folds back into0..15.

No complete local producer for high-queue requests is claimed here. OS masking
and actual SDK callback deferral are child-service/source prerequisites: the
post-before-count increment is preserved, not rewritten as count-first admission.

## Complete completion callback

Input request pointer Q. Read Q.wordc; value1 selects HIGH queue, every other
word selects LOW. Scan ALL16 slot pointers ascending, stride40 decimal. At a
pointer-equal slot, read word20; if nonzero invoke it with current slot.word24.
Do not break on match or callback return; continue the full scan. Then read the
selected queue's FRESH pending count and store U8(count-1), unconditionally.

An unknown pointer still decrements count; count0 wraps to255. No queue-range
validation, duplicate completion guard, request erasure, index update, IRQ call
or SDK cancel/free request. The selected queue base is retained across any user
callback, while final count is freshly read AFTER it. The sample-store producer
writes user callback0, but callback bodies from other producers may mutate state.
No assumption of one completion per outstanding request is proved locally.

## Complete synchronization leaf

aramSyncTransferQueue reads LOW queue.byte281 repeatedly until0, then returns.
No high-queue read, slot scan, cache flush, IRQ service, SDK wait, timeout,
timebase/pad polling or explicit status assignment. This is a busy-wait, not a
promise to drain all audio/DMA queues or a measured audio-start boundary.
Hardware bridge hwSyncSampleMem issues this exact local request once.

## Source validation and remaining work

[Manifest](../../../tools/musyx_aram_sample_storage_evidence.json) checks these
two complete bodies plus six store/config/address bodies:16 complete/linked
spans,207 anchors,16 independently resolved direct calls. Derived ring/counter
boundary metadata does not execute original code. Original ELF inspection and
listings stay `/tmp`; no source body or bank asset is exported to history.

Remaining: queue initialization and other producers, valid priority/request/
callback/index/count/concurrency invariants, SDK ARQ/interrupt/DMA and actual
original transfer/lifetime traces. Two complete local bodies and the sample
store's low-queue request order are answered; full backend/runtime remains OPEN.

[Complete generic upload and hardware stream bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md) now supplies the high/low producer, retained user callback/argument and full-word priority normalization. [Initialization](MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md) supplies queue reset and zero upload. Caller/source/SDK/concurrency/runtime invariants remain open.
