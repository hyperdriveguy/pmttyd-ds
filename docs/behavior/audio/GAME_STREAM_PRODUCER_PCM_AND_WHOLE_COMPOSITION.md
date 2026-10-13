# Game stream producer PCM refill and whole callback composition

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped _sscallback800df384..800df5a4 exclusive,
136 instructions. Combined with
[dispatch/silence and return ABI](GAME_STREAM_PRODUCER_DISPATCH_AND_SILENT_FILL_PREFIX.md)
and [live encoded/decode-call region](GAME_STREAM_PRODUCER_LIVE_ENCODED_COPY_AND_DECODE_CALLS.md),
this closes the whole local521-instruction callback800deda0/824hex. Addresses/
offsets/masks hexadecimal; counts/arithmetic decimal. Static deductions, not
original PCM/starvation/audio observations. Decoder helper remains separate.

## PCM entry and initial source alignment

Entry retains row V/GPR31 and subrecord Q/GPR30 selected by tag mask100, D1/r6,
N1/r8,D2/GPR28,N2/GPR29. Earlier admission required captured flags&8006==0,
latch0, captured format bits80/100 both clear. No fresh flags/latch/format/type/
channel/index/pointer/capacity guard here, pool recapture or child call.

Load U16 Q.half4c index and word44 offset; selected source=
wrap(Q.word[2c+index*4]+(offset&fffffffe)). Only initial source offset clears its
low bit. Q.word44 itself is not aligned or rewritten. Raw offset3 therefore
starts reading at selectedBuffer+2 but its first stored increment is5.
Alignment/extent/value/alias/lifetime/ABI domains require provider evidence.

## First span copies halfwords and counts actual iterations

N1 remaining count; D1 destination. N1=0 skips all copy/boundary/total processing
for first span. Each nonzero iteration:

1. Signed halfword load at retained source; advance source by2, decrement
   remaining, store low16 at destination, advance destination by2. Payload bits
   remain identical; no volume scaling, clamp or sample conversion.
2. Fresh V.word24 wrap-increment/store (one copied halfword).
3. Fresh Q.word44 wrap-increment by2/store.
4. Read index, offset44 and indexed available count Q.word[3c+index*4] FRESH;
   unsigned offset>=available selects boundary handling.

Count is published BEFORE offset/buffer/end handling. No range/busy/available
check precedes the actual halfword load/store. Source increments remain retained
unless boundary logic reloads a buffer. Word38 total position has NOT yet been
incremented when this buffer-boundary check executes, unlike ADPCM byte copy.

## Boundary rollback skips position increment and total check

At buffer end read signed Q.half54 busy. Exactly0 then reads V.flags0 FRESH;
mask4000 CLEAR: read Q.word38, subtract retained POST-increment offset, store
word38; set word44=0; reread half4c/current selected buffer as source.
Jump DIRECTLY to remaining-count test. No index/busy change, no position+2
increment or common total-end check on that iteration. The copied halfword was
already counted; no count rollback or waiting/new IO request. Example position8,
offset0,available2 gives stored position6 after one copy, not8.

Busy nonzero (including negative) OR fresh4000 set switches: clear half54,
clear word44, read index, store low16(1-index), reread index/select buffer as
source. No0/1 invariant enforced; index65535 becomes2. Then common position/
total processing runs. Offset<available also reaches that common path.

## Common position+2, total restart and stop

Fresh Q.word38 wrap-increment by2/store, THEN reread it and V.word1c total.
Unsigned position<total continues. Otherwise load V.word20 restart/store it
into Q.word38, read flags FRESH; mask200 set continues. Clear200 OR4/store and
breaks current span. No source-pointer/offset/index correction accompanies
that total restart store. No proof of correctly positioned original loop data
without actual producers/valid domains and execution evidence.

First-span stop still proceeds to second span. No flags8006/latch/format repeat
or first-success/copy-count gate. The second span uses D2,N2 and the source
pointer left by first span, without source/index/offset recomputation/alignment.
Repeat exact halfword/count/offset/boundary/position/total ordering, including
rollback skipping position/end and switch preceding position increment.

Second N2=0 skips its processing; an early first-span break does not reduce N2.
After both finish/break, common800df5a0 returns FRESH V.word24. Under stable
nonaliasing count fields this is copied iterations from both spans, not the
requested sum. Requested3+5 with each span stopping after one copy returns2;
ADPCM's corresponding early-stop return remains the requested sum. Buffer/row/
count aliases or external changes can alter fresh reads. No independent success
assignment, shared counter/OS IRQ or cache-service request in this region.

## Whole local composition, evidence and remaining work

| Manifest | Disjoint region / words |
| --- | --- |
| [Dispatch/silence](../../../tools/game_stream_producer_silence_prefix_evidence.json) | 800deda0..800df0d4:205;800df5a4..800df5c4 return ABI:8 |
| [Encoded](../../../tools/game_stream_producer_live_encoded_evidence.json) | 800df0d4..800df384:172 |
| [PCM/composition](../../../tools/game_stream_producer_pcm_composition_evidence.json) | 800df384..800df5a4:136 |

All521 instructions covered without gaps/overlap, independently checked against
read-only ISO; only the encoded region has the whole callback's two direct child
calls. PCM manifest pins two spans/136 new anchors/zero calls and whole-body hash.
Six alignment/seven boundary-position-end/four return models and publication sites
checked; earlier manifests reverified. Models assume stable/nonaliasing count/
provider fields and are mathematical deductions, not original execution. Generic
verifier checks pins, not semantic/provider/runtime claims. Fresh full callback/
helper ELFs/listings/scoped generator stay `/tmp`; no bodies/assets committed.
Prefix owns GPR28..31/LR restore and32 decimal stack bytes.

Whole local _sscallback answered. Remaining: complete ssDecodeDPCM, deferred DVD
manager and actual arguments/tag/channel/pool/flags/latch/index/buffer/offset/
available/busy/position/total/restart/count providers and valid value/extent/
alias/lifetime/IRQ/concurrency/ABI domains, transitive SDK/cache/ARAM/AI/DSP and
original short-span/starvation/buffer-switch/loop/early-stop/refill/audio traces.
EVT-01/native/full-game remain OPEN.
