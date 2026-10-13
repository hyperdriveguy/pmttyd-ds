# Original cache-range services and audio call corrections

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete five cache-range bodies below.
Addresses/offsets/masks hexadecimal; counts/shifts/strides decimal.
Static instruction/range/request behavior, not observed cache/DSP completion.

## Source identity and correction boundary

| Original entry | Size, hex | Loop instruction | Post-loop instruction |
| --- | --- | --- | --- |
| DCInvalidateRange80296a28 | 2c | dcbi | None |
| DCFlushRange80296a54 | 30 | dcbf | sc |
| DCStoreRange80296a84 | 30 | dcbst | sc |
| DCFlushRangeNoSync80296ab4 | 2c | dcbf | None |
| DCStoreRangeNoSync80296ae0 | 2c | dcbst | None |

The US symbol map and resolved direct targets identify these exact names.
Earlier audio contracts incorrectly attributed some ab4/e0 calls to each other,
and some a54/a28 calls to each other. Corrected contracts and linked body pins
are listed below. Original caller instruction/address/argument/order evidence
was unchanged; earlier hash/site verification did not validate prose service names.
This delivery supersedes those specific prior name/operation attributions.

## Complete shared range admission and count

Inputs P=r3,N=r4 are raw unsigned words. Compare N unsigned with0; zero returns
immediately without cache-loop/sc/argument mutation. Any nonzero admits loop.
Capture low5(P); compute:

`K = wrap32(wrap32(N+(P&1f))+31) logical>>5`

Load CTR=K, then execute the selected cache instruction with address operand P
WITHOUT clearing P's low bits. Increment P by32 with wrap32; bdnz back to the
cache instruction. No separate K==0 check before first operation.

For ordinary nonwrapping positive inputs, K equals ceiling((N+low5(P))/32).
P starts at the raw unaligned address and advances by32; the source does not
round the argument pointer down or modify it by low5 subtraction. Actual line/
virtual-physical/cache behavior belongs to processor/address/runtime evidence.
No alignment, pointer, memory region, write ownership, length cap, active-system,
IRQ or caller identity gate. No loads/stores of pointed memory in these bodies.

If the count additions wrap and yield K0 despite N!=0, the loop still runs its
first cache instruction, then decrements CTR0 toffffffff. Absent a trap/control
change, this entails2^32 iterations before CTR reaches0. Do not repair it into
an empty operation or use widened host ceiling arithmetic. Pointer adds can
also wrap while CTR advances. These are local control-flow deductions, not
original invalid-address/cache experiments.

## Five operations, syscall order and return

Each iteration executes exactly the opcode in the identity table, then pointer
increment and CTR branch. Flush/store variants are distinguished by dcbf versus
dcbst; invalidation uses dcbi. Instruction identity must not be inferred from
a generic cache-flush label. No direct or indirect call instruction exists.

For DCFlushRange/DCStoreRange only, AFTER the loop emits sc, then blr. The sc is
not a literal sync instruction in these bodies. Original syscall vector/handler,
ordering/completion/error/trap behavior remains open; do not infer its effects
from the API name. The other three contain no sc/sync/eieio/isync in their local
bodies. Zero length bypasses even sc in the two variants that otherwise emit it.
No independently assigned scalar status; under ordinary return r3 has advanced
with loop iterations. There is no callback or return-word success classification.

## Corrected audio caller contracts

- [DSP initialization/activation](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md):
  surround clear uses DCFlushRange(ptr,280); ITD allocation uses DCInvalidateRange
  (ptr,40*count); per-voice PB uses DCStoreRangeNoSync(PB,c4); studio/HRTF clear
  publications use DCFlushRangeNoSync. Earlier inverse labels corrected.
- [Startup format prefix](MUSYX_DSP_STARTUP_CALLBACK_ITD_AND_FORMAT_PREFIX.md):
  after clearing V.word08's40-byte region, original targeta54 is DCFlushRange.
- [Envelope/voice command loop](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md):
  the per-voice command range at8028a650 calls DCStoreRangeNoSync (e0).
- [Command rollover/finalization](MUSYX_DSP_COMMAND_ROLLOVER_AND_FINALIZATION.md):
  all twelve deferred previous-chunk publications and both final publications
  call DCStoreRangeNoSync. Their byte pointer/size order stays exact; linked
  child fingerprint changed from the previously mislabeled a54 body to e0.
- [Studio payloads/fades](MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md):
  last-PB chain termination and the36-byte packed fade block call
  DCFlushRangeNoSync (ab4), with corrected linked fingerprint.
- [Budget/voice chain](MUSYX_DSP_BUDGET_REJECTION_AND_VOICE_CHAIN.md):
  preceding per-voice command range uses DCStoreRangeNoSync; previous-PB link
  publication uses DCFlushRangeNoSync, with corrected linked fingerprint.

Four manually selected linked-cache pins are corrected; manifests whose original
resolved-call scan already selected the actual children need no pin change.
Caller manifests and the composed-builder coverage audit are reverified.
Historical inbox/progress entries remain append-only; this source correction is
recorded as a new delivery rather than rewriting past validation claims.

## Evidence and remaining source/runtime

[Manifest](../../../tools/gekko_cache_range_services_evidence.json) pins five
complete bodies: five spans,57 whole-body instruction anchors/zero direct calls.
Eleven zero/nonzero/alignment/threshold/wrap range vectors and8224 small-domain
checks (32 low-pointer offsets,257 lengths) are deductions, not original cache/
syscall/DSP execution. No huge wrapped loop was executed. Full fresh ELFs/listings
stay `/tmp`; no bodies/binary assets committed.

Remaining: original syscall vector/handler and cache/address/IRQ/coherency/
ordering/trap/ownership domains, actual caller ranges/alias/lifetime/partial-
publication and DSP command consumers, original cache/voice/audio traces and
native binding. Five local bodies and listed source-name corrections answered;
full audio/runtime and EVT-01 remain OPEN.
