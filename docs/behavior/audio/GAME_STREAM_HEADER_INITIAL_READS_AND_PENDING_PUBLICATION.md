# Game stream header initial reads and pending publication

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped SoundSSPlayChEx_main800de3cc..800de6c0
exclusive,189 instructions. Addresses/offsets/masks hexadecimal; counts/arithmetic
decimal. Static source deductions, not original IO/audio execution. Read
[header matching/format/reset prefix](GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md),
[interrupt chunk/request contract](GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md),
[cache initial reads](GAME_STREAM_CACHE_CALLBACK_TRANSFER_AND_INITIAL_READS.md) and
[completion callback](GAME_STREAM_COMPLETION_MATCH_CURSOR_AND_STARTUP.md).
Whole callback allocation/cache continuation remains open.

## Entry skips cached streams; no other initial admission gate

Retain row V/GPR31 and sound state S/GPR29 from the prefix. Read V.flags0 FRESH.
Mask400 set jumps directly to allocation continuation800de6c0, skipping both
initial reads/index/count/pending publications in this region. Mask400 clear
starts primary, without checking flags nonzero, masks8/2/1, count/busy/pending,
type, record/buffer pointers or callback-match success. Prefix has already reset
both cursor groups, but actual aliases/concurrency can alter those fields.
No shared IRQ counter or OS IRQ request in this region; V is not recaptured.

## Primary index and complete chunk calculation

Store V.half74=1 BEFORE selecting primary Q=V+28. Read cursor C=V.word5c and
end L=V.word88 once. The linked interrupt contract's COMPLETE unsigned32 chunk
calculation appears here too: B32768, short tail when wrap(C+B)>L, otherwise
first greater boundary for k2..5 and unsigned divide, then STRICT32 advance.
If none succeeds N=B. Request A=wrap(N+31)&ffffffe0.

Strict32 advance means wrap(N+32-(N&31)), so an already aligned N increases32.
For C0/L32768, N/A16416. For C0/L65535, N/A32800 can exceed B. No clamp to
buffer capacity, length or nonzero is inserted. Read/wrap/extent/address domains
require separate provider evidence. N and A are distinct retained operands.

Read V.flags0 AGAIN after chunk selection; mask400 independently selects the
storage path. With stable flags admitted by the initial clear400 gate, this
second read is also clear and DVD path follows; a changed flag can select ARAM.
No local child has occurred between these two reads. Source ordering alone
does not prove a real producer mutates them.

## Primary DVD or ARAM request precedes count/pending publication

Mask400 clear: read U16 Q.half4c and select
Q.word[2c+wrap((1-half4c)*4)] without bounding the halfword. Read primary
record Q.word0 and header U16 V.half4. Request DVDMgrReadAsync80144de4
(record,selectedBuffer,A,wrap(header+C),_ssDVDReadAsync_activeChk800dfc98)
at800de4a8. No status, ID, pointer, physical-completion or file-extent check.

Mask400 set: read V.half2, ARAM base S.word[21c+half2*4]/803e0354, header half4
and Q.half4c/selected buffer. Retain X=wrap(base+header+C). Request
DCInvalidateRange80296a28(buffer,32768) at800de4f0. AFTER child reread Q.half4c
and selected buffer; request ARQPostRequest802aa8ec
(Q+4,Q,type1,priority0,retainedX,secondFreshBuffer,A,cache_flush800dfc64)
at800de528. Fixed invalidation count is independent of N/A; source remains
retained while destination can change. No post-invalidation storage-path recheck.

After either child returns, store N at Q.word48/row70 at800de52c. Read V.flags0
FRESH, OR1000 and store low16 at800de538. Ignore child return values; no rollback
or busy half7c=1 write occurs in this region. This local pending publication
does not establish successful original IO or callback completion.

## Fresh post-primary type controls secondary read

AFTER primary request/count/pending, reread V.half1a. Exactly2 starts secondary;
every other value jumps to allocation continuation. No primary success condition,
initial type snapshot or additional flags/count/busy/pending admission.
Type can differ from the header's earlier narrowed value under child/alias effects.

Store V.half fc=1, choose Q=V+b0; fresh cursor C=V.worde4/end L=V.word110 feed
the same full chunk calculation. Compute retained plane offset
T=wrap(L+32-(L&31)) BEFORE its branch sequence. This strict advance adds32 to
aligned L too. Secondary storage path uses a fresh V.flags0 mask400 independently
of the initial admission or primary storage choice.

DVD request at800de628 uses secondary Q.word0, selected buffer from fresh
Q.half4c, A and wrap(header+C+T), same active-check callback. ARAM source
X=wrap(base+header+C+T), with fresh half2/header/index/buffer reads; invalidate
at800de674(buffer,32768), then second fresh index/buffer for ARQ at800de6ac,
same type1/priority0/Q owner/cache_flush callback. No ID/status/extent guards.

Store retained N at Q.word48/rowf8 at800de6b0, THEN fresh flags OR2000/store
at800de6bc. No busy half104 write or physical completion check. Primary/secondary
pending bits can coexist; callback terminal/nonterminal branches own clearing
semantics. No frame/tick cadence or readiness/activation claim follows here.

## Region exit, evidence and remaining work

All paths continue at800de6c0 into stream length/zeroing/allocation and caching
setup. No return/status assignment, resource rollback, voice allocation or mix
call in this scoped region. Prefix owns the function prologue/exit ABI and the
unmatched third-row/index3 behavior; that remains part of the composed context.

[Manifest](../../../tools/game_stream_header_initial_reads_evidence.json) pins
six spans, all189 body-word anchors, six calls/four callback bindings. Twelve
linked chunk/seven admission/four fresh-type/six pending mathematical vectors
and index/count/type/pending sites checked; prefix/chunk pins reverified.
Generic verifier checks pins/targets, not prose/providers/runtime. Fresh full
ELF/listing/scoped generator stay `/tmp`; no bodies/binary assets committed.

Remaining: whole callback allocation/zeroing/cache continuation, actual header/
record/index/type/flags/cursor/end/count/buffer/ARAM/header-offset producers and
valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains; transitive manager/
SDK/cache/ARAM/AI/DSP and original initial-read/callback/type-change/audio traces.
EVT-01/native/full-game remain OPEN.
