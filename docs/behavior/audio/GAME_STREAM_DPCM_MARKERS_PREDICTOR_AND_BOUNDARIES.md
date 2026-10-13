# Game stream DPCM markers, predictor and boundaries

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete ssDecodeDPCM800df5c4/210hex,
132 instructions, numeric dtbl80416548/20hex. Addresses/offsets/masks hexadecimal;
counts/arithmetic decimal. Static deductions, not original decoded/audio traces.
Read [two helper calls](GAME_STREAM_PRODUCER_LIVE_ENCODED_COPY_AND_DECODE_CALLS.md),
[PCM boundary order](GAME_STREAM_PRODUCER_PCM_AND_WHOLE_COMPOSITION.md) and
[header parameter mapping](GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md).

## ABI, zero request and raw state

Inputs (V,Q,D,N,P)=r3,r4,r5,r6,r7: parent row, chosen primary/secondary subrecord,
output pointer, requested sample count and retained source pointer.
Entry jumps directly to N comparison. N0 returns P without memory access;
no row/count/pointer check or write, even when P is0. Nonzero enters decode.
No channel/type/flags/generation/index/extent/null/IRQ check locally.

| Q field | Use |
| --- | --- |
| signed half50 | loop predictor seed |
| signed half52 | current predictor |
| half58 | packed-mode scale index |
| half5a | packed nibble position |
| half5c | packed nibble limit;0 selects header byte |
| word44,half4c | byte offset/current buffer index |
| indexed words2c /3c | buffer pointer/available bytes |
| signed half54 | buffer busy/completed value |
| word38 | total decoded position |

V.word24 holds copied-sample count; V.word1c/20 total/restart and flags200/4000/4
own loop/end handling. All provider/alias/value domains remain separate.

## Header byte: low nibble8 is marker, not negative sample

At each nonzero request iteration read Q.half5c and source byte at P BEFORE
selecting header/packed route. Limit0: read offset44, take low nibble L, advance
P by1 and offset44 by1/store. No buffer boundary check precedes that input read.

L8: take high nibble H; store half5c=H+4 (4..19), then half5a=0. Jump to N test
WITHOUT consuming requested sample or writing output/predictor/count/position.
Scale half58 is preserved. A sequence of marker bytes can consume source bytes
without producing a requested sample; no input-length/iteration bound is added.

Other low nibbles sign-extend four bits to delta d (0..7,-7..-1); store high
nibble H as half58, then compute sample. Header byte's delta-8 encoding is
reserved for marker here; packed data can encode-8. Original packet/schema/
termination constraints require separate evidence.

## Packed data: high then low nibble with fresh state reads

Limit nonzero: read half5a to choose high nibble when even, low when odd.
Read half5a AGAIN; load half58 scale index; store low16(position+1) at5a.
Sign-extend selected nibble to d=-8..7. Reread position5a and limit5c.

New position>=fresh limit clears half5c=0, advances P/offset44 by1.
Otherwise new position EVEN advances P/offset44 by1; odd retains both.
No nibble-position bound/monotonicity assumption, zeroing of position on packet
end or scale-index clamp. Wrapped position65535→0 can take the even-advance
path without ending a nonzero packet. Odd packet limits can discard the low
nibble of their final byte. Actual valid packet/value domains remain open.

## Original signed scale table and saturated predictor

Normal header H limits scale to0..15, but packed half58 is an unguarded raw U16
index into dtbl using scaleIndex*2. Signed16 values in source order:

    1,7,24,59,83,111,236,555,742,885,1052,2221,3285,4432,6962,9300

Load signed table halfword, sign-extended delta and fresh signed predictor52.
Low32 multiply/add forms X=wrap32(predictor+d*scale); signed compare clamps X
above32767 or below-32768. Store resulting low16 at Q.half52 BEFORE decrementing
N, storing output halfword at D and advancing D by2. Then fresh V.word24
wrap-increment/store. No rendering/timing conversion, interpolation or FP work.
The table, nibble arithmetic and saturation are source data, not measured audio.

## Buffer handling precedes decoded position increment

AFTER output/count publication, read fresh Q.index4c, offset44 and indexed
available count3c; unsigned offset>=available selects boundary handling.
Busy54 exactly0 and fresh V.flags4000 clear takes rollback: fresh position38
minus retained byte offset, store38; clear offset44; reload current indexed
buffer as P; jump to N test. Skip decoded position+2/end check for that sample.
Predictor/packed scale/position/limit are NOT reset by this rollback.

Busy nonzero OR4000 set: clear busy54/offset44, low16(1-index) store/reread
and load switched buffer as P. No0/1 index invariant or packed-state reset.
Offset<available and this switch both reach common position processing.

Fresh position38 +2/store, reread and compare to fresh V.total1c. Unsigned
position>=total loads restart20/store38, then signed Q.half50 loop seed loads/
stores into predictor52. Read flags FRESH:200 set continues; clear200 OR4/store
and breaks. Predictor resets after the already-output sample; no source/offset/
index/packed-state reposition accompanies restart. This does not establish valid
original loop data or complete source consumption without provider traces.

Continue until requested N decremented to0 or total stop breaks. Return P,
not copied count/status; parent passes this raw return to its second helper call
and later reads V.word24. No stack frame, direct child call or saved nonvolatile
register mutation; leaf uses volatile registers and returns with blr.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_dpcm_decode_evidence.json) pins two spans,
all132 body-word anchors,zero calls and all16 signed scale values. Six nibble/
six marker/seven predictor/six packed-state/seven boundary/three zero-
request vectors and publication sites checked; PCM/producer pins reverified.
Boundary vectors start with post-input offset and pre-position handling;
input-byte progress/markers are separately modeled above. Generic verifier
checks hashes/anchors, not semantic/provider/runtime claims. Mathematical source
deductions separate from original execution. Fresh full ELF/listing/generator
stay `/tmp`; no bodies/binary assets committed. Local whole decoder answered.

Remaining: actual marker/packet/table-index/predictor/seed/row/buffer/offset/index/
available/busy/count/position/total/restart/flags providers and valid value/extent/
alias/lifetime/IRQ/concurrency/ABI domains, deferred DVD manager/transitive SDK/
cache/ARAM/AI/DSP and original packet-boundary/saturation/starvation/loop/refill/
callback/audio traces. EVT-01/native/full-game remain OPEN.
