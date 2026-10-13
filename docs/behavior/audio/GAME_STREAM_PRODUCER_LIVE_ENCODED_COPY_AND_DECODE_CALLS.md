# Game stream producer live encoded copy and decode calls

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped _sscallback800df0d4..800df384 exclusive,
172 instructions. Whole824hex/521-instruction callback remains in progress.
Addresses/offsets/masks hexadecimal; counts/arithmetic decimal. Static deductions,
not original refill/audio execution. Read
[dispatch/arguments/silence](GAME_STREAM_PRODUCER_DISPATCH_AND_SILENT_FILL_PREFIX.md),
[header binding](GAME_STREAM_HEADER_ALLOCATION_CACHE_AND_COMPOSITION.md) and
[completion/buffer counts](GAME_STREAM_COMPLETION_MATCH_CURSOR_AND_STARTUP.md).

## Retained row, tag subrecord and captured format

Entry live route retains V/GPR31, captured flags F/r4, input D1/r6,N1/r8,
D2/GPR28,N2/GPR29 and raw user tag T/r7 from prefix. Admission already required
F&8006==0 and shared latch0; no fresh repeat here. T mask100 SET selects
Q=V+b0, otherwise Q=V+28. No type2, channel/index/ID/generation/pointer/extent
check. Low8 tag chose row earlier; mask100 chooses subrecord independently.

Captured F mask80 selects ssDecodeDPCM calls FIRST; if clear, mask100 selects
encoded ADPCM byte copy; both clear jumps to PCM continuation800df384. Both
bits set take decode calls, not ADPCM copy. No flags reread selects format.
Valid field/alias/ABI/provider domains remain separate.

## Decode calls: first return feeds second source argument

Mask80: load U16 Q.half4c index and Q.word44 offset, read
Q.word[2c+index*4] and add raw offset with wrap32. No index bound/alignment mask.
Request ssDecodeDPCM800df5c4(V,Q,D1,N1,source) at800df118.
AFTER child retain RAW returned r3 as source for the second request
ssDecodeDPCM(V,Q,D2,N2,firstReturn) at800df130. No fresh source/index/offset
recalculation, pointer/status/sentinel guard or first-success condition.
Both requests occur even when their count is0. Q/V/D2/N2 stay retained.

Ignore second raw return; jump to common800df5a0 load of FRESH V.word24 as
callback return. Helper's writes/count/decoder/cursor semantics are linked,
not answered by this request protocol. No host decode algorithm is substituted.

## ADPCM counts round upward before forward byte copying

Captured80 clear and100 set: B1=8*floor(wrap32(N1+13)/14), implemented by
unsigned24924925 quotient sequence/maskfffffff8. No signed correction used in
this branch. Initial source=wrap(Q.word[2c+freshHalf4c*4]+freshWord44).
No source/index/null/alignment/capacity/available-count clamp.

Copy B1 bytes to D1, forward one byte at a time. For each iteration:

1. Read source byte, advance retained source pointer, decrement remaining count;
   store byte, advance destination.
2. Read Q.word44 FRESH, wrap-increment/store; read Q.word38 FRESH,
   wrap-increment/store.
3. Read index half4c, offset44 and indexed available count
   Q.word[3c+index*4] FRESH; unsigned offset>=available selects boundary handling.

There is no child/cache call, buffer snapshot or byte-count return publication
during this loop. Zero B skips copying and boundary/total checks for the span.
N1=1,13,14 all request8 encoded bytes; N1=15 requests16. This differs from
silent-fill's complete-block rounding down. Raw N+13 can wrap before division.

## Boundary rollback versus switch, then common total end

At offset>=available, read signed Q.half54 busy. Exactly0 reads V.flags0 FRESH;
mask4000 CLEAR takes rollback: fresh Q.word38 minus retained offset becomes
word38, set word44=0, reread half4c/selected CURRENT buffer as source. Jump
DIRECTLY to loop remaining-count test, skipping common total-end check for this
iteration. Index and busy are unchanged. More requested bytes can reread the
current buffer's beginning without a local availability wait or new IO request.

Busy nonzero (including negative) OR fresh4000 set: clear half54, clear word44,
read index and store low16(1-index), reread that halfword and select source buffer.
No assertion index0/1; index65535 becomes2. This switch then falls into common
total-end processing. Offset<available also reaches the common total check.

Read Q.word38 and V.word1c total FRESH. Unsigned position<total continues.
Otherwise load V.word20 restart and store it into Q.word38. Read flags FRESH;
mask200 set continues. Clear200 ORs4/store and breaks this span's copy loop.
No offset/index/source-pointer correction accompanies that total restart write.
Do not treat it as a complete reposition to authored restart data. Provider/
consumer domains are required to establish original valid looping behavior.

## Second span reuses first source; early stop does not suppress it

After first span completes OR breaks at total end, compute
B2=8*floor(wrap32(N2+13)/14); use D2 and the retained source pointer left by
first span. No source/index/offset reset/reload, flags8006/latch/format gate or
first-status check before the second span. Repeat the exact per-byte/boundary/
total processing, including rollback skipping total check and switch behavior.
A stop bit4 set by first span does not locally suppress second-span copying.

After second loop completes/breaks, store wrap32(ORIGINAL N1+ORIGINAL N2) at
V.word24 at800df37c; common800df5a0 reads it FRESH as return. Not actual copied
bytes/count, not B1+B2, and not conditioned on an early break. Buffer/row/count
aliases may affect a fresh load; ordinary model assumes nonaliasing/stable fields.
This raw return does not prove original physical playback or complete requested
production. MusyX cursor/flush services have independent source contracts.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_producer_live_encoded_evidence.json) pins
two spans/all172 region words/two direct helper calls. Ten rounded-size/seven
tag/five route/six boundary/four total/four returned-count vectors checked.
Prefix pins reverified. Generic verifier checks pins/targets, not semantic/
provider/runtime claims. Models are mathematical deductions, not original IO/
audio traces. Fresh full callback/helper ELFs/listings/scoped generator stay
`/tmp`; no bodies/binary assets committed. Prefix owns return ABI.

Remaining: _sscallback PCM continuation/whole composition and complete
ssDecodeDPCM helper; actual callbacks/flags/tag/pool/index/buffer/offset/count/
busy/position/total/restart providers and valid value/extent/alias/lifetime/IRQ/
concurrency/ABI domains, deferred manager/transitive SDK/cache/ARAM/AI/DSP and
original starvation/switch/loop/early-stop/short-span/refill/audio traces.
EVT-01/native/full-game remain OPEN.
