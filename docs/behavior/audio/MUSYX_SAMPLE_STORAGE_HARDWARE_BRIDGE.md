# MusyX sample storage hardware bridge and encoded byte counts

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete hwSaveSample8029145c/98,
hwRemoveSample80291514/88, hwTransAddr802913d8/4 and
hwSyncSampleMem8029159c/20hex. Read
[directory lifetime](MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md).
ARAM/DMA/cache/hardware internals and original playback remain separate.

## Encoded size computation shared by save/remove

Read metadata.word4 (sample entry+10), let format=its high8 and N=low24.
For format0/1/4/5, the source computes these unsigned-word operations:

`T=wrap32(N+13); H=high32(T*0x24924925); Q=wrap32(((T-H)>>1)+H); bytes=Q&fffffff8`.

In the admitted24-bit N domain this equals ceil(N/14)*8, including N0→0.
The multiplier is ceil(2^32/7);3*T<2^32 proves H=floor(T/7) over this domain.
Writing T=7*k+r shows Q=4*k+floor(r/2), whose low-three-bit clear equals
8*floor(T/14). This is an arithmetic deduction, not a measured transfer.
For formats2/6, bytes=wrap32(N*2). Format3 and7..255 keep N as the byte count.
There is no unsupported-format failure or zero-size bypass. These are literal
format routing/size rules; wider encoding labels require decoder/hardware sources.
All arithmetic masks/offsets in this document are hexadecimal unless stated.

## Complete save request and address rewrite

Inputs: pointer-to-metadata-pointer M, mutable address-word pointer A,
raw cache argument C. Retain A, load metadata pointer=*M, then metadata.word4
and compute byte count. Read CURRENT *A as source address, request
aramStoreData(sourceAddress,bytes,C)80291c58. Store its returned word to *A
UNCONDITIONALLY and return without a defined independent status code.
No address/null/format/allocation check, error classification, refcount store or
metadata write here. The [acquisition caller](MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md)
sets ref1 AFTER this request; do not infer successful residency from that flag.

## Complete remove request

Inputs: DIRECT metadata pointer M, raw address A, cache argument C. Compute size
from M.word4, then request aramRemoveData(A,bytes,C)8029207c. No pointer-to-pointer
load, address update, reference counter mutation or second hardware request.
It is not symmetric to save's returned-address publication and does not clear
the caller's retained address field. Callers cannot infer a freed CPU pointer
or deleted voice from this local request alone.

## Identity translation and synchronization request

hwTransAddr is ONLY blr: return r3 unchanged. No address conversion, copy,
cache admission or loading. The game loader's sample-address0 remains0.
A native host translation is a separate adapter responsibility.

hwSyncSampleMem requests aramSyncTransferQueue802918b8 once, then returns.
No local queue scan/timebase/pad/IRQ request or explicit success assignment;
waiting/progress behavior belongs to that child. This body establishes request
order, not a measured blocking duration or audio-start boundary.

## Original sample examples and validation

Original Hammer dependencies775/776 have format0 and N5276/1526 respectively;
source transfer sizes are3016/872 bytes BEFORE any child alignment/cache/DMA.
These are computed encoded-source sizes, not PCM16 lengths or audible duration.
The existing converter retains16000Hz and full original sample counts; this
contract adds no conversion or changed original bank data.

[Manifest](../../../tools/musyx_sample_lifetime_evidence.json) pins all four bodies
and size/request/store anchors. Independent arithmetic/source check covers all
1247 original directory records and boundary counts0/1/13/14/15/ffffff across
routing formats; no original code is executed. Inspection ELFs/listings stay
`/tmp/inbox-hw*sample.elf`. Re-run generic read-only DOL source verification
with that manifest. The original .sdir file hash is included as a resource span
with no geometry/map-node claim.

Remaining: aramStoreData/RemoveData/Sync queue, allocation/cache/alignment/DMA,
metadata/sample-bank pointers and actual voice/sample lifetime/decoding/output.
These bridge bodies and size rules are answered; original execution, hardware
and complete audio-chain equivalence remain OPEN.

[Complete local ARAM store/remove/configuration/address services](MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md) and [queue completion/synchronization](MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md) now answer those exact child bodies. Removal rewinds the cache cursor; sync busy-waits only the low queue. Initialization/source/SDK/DMA/voice and original runtime remain open.
