# MusyX sample identity and hardware field services

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Six complete local bodies:
seqGetInstanceForVoice80279f30/a4hex, vidGetPublicId802849b4/54hex,
hwSetVirtualSampleLoopBuffer80290bc4/20hex, hwGetSampleType80290bf8/14hex,
hwGetSampleID80290c0c/14hex, hwGetSampleExtraData80290c20/14hex.
Addresses/offsets/masks hexadecimal; counts/indices decimal.
Static source reads/stores, not original playback/loop or execution evidence.
These close children of [sample-start notifications](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md).
Read [prior ARAM stream-buffer lookup](MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md)
and [sequence ID lookup](MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md).

## Complete active-sequence instance lookup

seqGetInstanceForVoice(X) captures activeRoot8041ee5c ONCE, initializes CTR=8
and starts at that captured root. For every sequence node Q in the chain:

1. Traverse Q.worde64 note head, following each note.word0 successor.
2. If no match, traverse Q.worde68 identically.
3. If no match, traverse Q.worde6c identically.
4. If still no match, load Q.word0 sequence successor and repeat.

A match is note.word8==RAW full X. First match immediately returns fresh
Q.wordc, the external sequence word, not a pointer or byte9 private index.
No input tag removal, non-missing-ID check, sequence state-byte filter or note
voice-generation/validity recheck. Exactffffffff may match a resident note.word8.
The return is not re-tagged from X.

Only when the sequence chain reaches NULL does bdnz decrement CTR. If more
passes remain, reset Q to the SAME captured root and repeat the full traversal.
After eight complete misses, returnffffffff. This is NOT an eight-node cap:
there is no CTR decrement inside the sequence-node traversal. A stable chain
with ten nodes can find node9 in the first pass. A stable missing note yields
eight repeated traversals; a cyclic sequence or note chain can prevent even
the first traversal from reaching the bounded outer retry.

PausedRoot is never read. No list/root refresh between retries, array sweep,
lock/IRQ request, fallback to paused nodes, early single-pass miss or count/
cycle/member/alias guard. Nodes, note-heads, IDs and successors are read fresh
when visited; the captured root remains retained. Concurrent mutation contracts
are separate. A paused sequence's matching notes alone cannot satisfy this
local search. No source write or child call occurs in this body.

## Complete public-ID lookup through internal voice chains

vidGetPublicId(X) captures root word8041ef2c and synthVoice8041ee90 once.
For every outer record R in root's word0 chain:

- Read internal ID C=R.wordc.
- Exact C=ffffffff ends this inner chain before any equality test.
- Otherwise compare full C with raw X. Equal returns fresh R.word8.
- On mismatch, select voice at capturedSynthVoice+C.low8*458hex;
  load its wordec as the next internal ID and repeat.
- After sentinel, load fresh R.word0 and search the next outer record.

Total outer miss returnsffffffff. No state/private index/ID high-bit mask,
voice.wordf4 identity validation, count/cycle/array-bound gate or lock. The
low8 index belongs to the CURRENT chain ID C, not X. Two generation-distinct
IDs with the same byte index remain unequal at the full-word comparison.
Because sentinel is tested first, an inputffffffff cannot match a chain's
sentinel as an ordinary ID. Duplicates resolve by outer-record order, then
internal chain order. R.word8 is returned raw without validity normalization.

The captured synthVoice pointer is reused through the whole lookup; it is not
reloaded at each successor. Stale or cyclic wordec chains require actual source
invariants, not invented repair or identity validation. There are no writes or
child calls. This is distinct from the sequence lookup above: public-ID search
follows voice links and returns R.word8, while sequence lookup follows note lists
and returns Q.wordc.

## Complete hardware loop-buffer setter and getters

All four use the hardware voice base word8041ef88 and byte stridef8hex.
Their own raw index I is NOT truncated; displacement=wrap32(I*f8hex).
No sndActive/state/generation/null/count/bounds/IRQ/service request.
The sample-start parent supplies I=voice.low8; direct callers must establish
their own valid index and base invariant.

| Complete service | Exact local operation |
| --- | --- |
| hwSetVirtualSampleLoopBuffer(I,A,N) | Load base; store raw A at voice.word94; RELOAD base; store raw N at voice.word98 |
| hwGetSampleType(I) | Load base; return zero-extended voice.byte90 |
| hwGetSampleID(I) | Load base; return zero-extended voice.half70 |
| hwGetSampleExtraData(I) | Load base; return raw voice.word7c |

The setter retains the computed displacement across the two base loads, so a
base mutation between stores can select different records. It does not update
sample type, virtual-ID active byte or DSP event flags, flush a cache, configure
loop hardware directly, or validate address/length. Its result is incidental;
there is no status or independent failure return. Passing A=N=0 clears only
these two words. Getters do not prove that the returned type/ID/extra word
belongs to the currently requested full voice ID or to a resident sample.
Actual field producers/backend consumers remain separate source contracts.

## Sample-start binding and prior ARAM leaf

The parent uses vidGetPublicId(raw original V), then, for a non-missing public
ID, seqGetInstanceForVoice(fresh slot.word1c). These two searches do not establish
hardware voice identity, generation freshness or paused-sequence association.
A public-ID match with no active-note match legitimately produces sequence
wordffffffff from this local graph; do not turn that into a successful pointer.

aramGetStreamBufferAddress is already complete in the linked prior contract;
its body is re-fingerprinted here as context, not a new unanswered service.
It selects registry80410860+(I.low8*10hex), optionally stores row.word8 to a
non-null size pointer, then returns fresh row.word4. The parent passes size
pointer0, so that optional output write does not occur. No occupied-slot check
or stream-buffer allocation is part of this read. Preserve the distinction
between its low8 registry selection and the hardware services' raw index.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_sample_identity_and_hardware_fields_evidence.json)
pins six complete bodies and the prior ARAM lookup fingerprint: seven spans,
50 instruction anchors, zero child calls. Eight raw hardware/ARAM index, eight
field-width, seven stable active-sequence and seven stable public-ID vectors
are derived checks, not original execution. The ten-node example distinguishes
a full traversal from a supposed eight-node cap. Stable vectors do not establish
alias/concurrency/cycle guarantees or repair unchecked memory domains.
Fresh inspection ELFs/listings stay `/tmp/inbox-seqGetInstanceForVoice.elf`,
`/tmp/inbox-vidGetPublicId.elf`, `/tmp/inbox-hwSetVirtualSampleLoopBuffer.elf`,
`/tmp/inbox-hwGetSampleType.elf`, `/tmp/inbox-hwGetSampleID.elf`,
`/tmp/inbox-hwGetSampleExtraData.elf`, `/tmp/inbox-aramGetStreamBufferAddress.elf`.
No original bodies/assets committed or original code executed.

Remaining: actual outer/internal voice and active sequence/note/ID providers,
hardware/sample/loop-buffer field producers and backend consumers, valid
pointer/index/count/generation/member/cycle/alias/IRQ/concurrency domains and
original start/end/public-ID/sequence-association/playback traces. Six local
services are answered; EVT-01 and full native/source/runtime/hardware equivalence
remain OPEN.
