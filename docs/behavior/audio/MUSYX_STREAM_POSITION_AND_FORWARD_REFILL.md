# MusyX stream position normalization and forward refill

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete streamHandle REGION
8027a8f4..8027aaec, end exclusive,126 instructions. Whole handler8e4hex is
fingerprinted; wrapped-refill/footer/ABI remainder remains open. Offsets/
addresses/masks hexadecimal; shifts/divisors decimal. Static source deductions,
not original callback/stream/audio execution. Read
[startup/dispatch](MUSYX_STREAM_THROTTLE_DISPATCH_AND_STARTUP.md),
[position child](MUSYX_VIRTUAL_SAMPLE_INIT_UPDATES_AND_POSITION.md) and
[flush bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).

## Captured position, format and route

State2 dispatch retains stream row S. Read voice index word4c; request hwGetPos
80291238 at8027a8f8. AFTER child read U8 format byte0d. Format1 normalizes raw
unsigned return H to14*floor(H/14); every other format retains H. The exact
mulhwu by24924925, subtract/half/add/right3 sequence computes floor(H/14) for
all unsigned32 H. No position/format/range validation or status check.

Read cursor C=word20. Unsigned C==P (normalized position) skips directly to row
tail8027af50, bypassing common footer. C<P enters forward refill below. C>P
branches8027aaec to the remaining wrapped-refill path. Forward format0/1 are
handled;2..255 skip to common footer8027af10 with no callback/cursor/flush here.
The captured format determines the route even if a callback changes byte0d.

## Producer callbacks and post-child state admission

Define B(x)=8*floor(unsigned32(x)/14). Source computes this as the quotient's
pre-right3 value maskedfffffff8. On forward refill:

| Format | Callback word10 arguments |
| --- | --- |
| 0 | (source14+wrap32(2*C), P-C, 0, 0, user50) at8027a974 |
| 1 | (source14+B(C), P-C, 0, 0, user50) at8027aa40 |

Source pointer/length additions wrap32. Source14, callback10 and user50 are read
before invocation. Format1 retains offset O=B(C) in r30 across callback. No
callback-null/signature/return-capacity/source/lifetime check here; no IRQ request.
Callback returns raw word R. R0 skips to common footer WITHOUT a fresh state read.
Nonzero: read state+c; only exactly2 admits cursor/flush work. Other states skip
with no local cursor publication. This does not prove a callback filled R units,
that R<=requested length, or that source/provider fields remained unchanged.

## Fresh modulo cursor and optional flush

After admitted callback read cursor word20 fresh=F, then buffer length word18=L.
Compute A=wrap32(F+R); unsigned divwu A/L, low32 quotient*L, subtract to get new
cursor N. L0 is not guarded; no mathematical output is invented for invalid
division domains. Read flags8; mask00020000 set skips flush, not cursor store.

Format0 flush when flag clear:

| N | Offset and size |
| --- | --- |
| Nonzero | Offset=wrap32(2*F); size=wrap32(2*wrap32(N-F)) |
| Zero | Offset=wrap32(2*F); size=wrap32(2*wrap32(L-F)) |

Source14 and selector byte0e are reread for the request; F/L are retained from
post-callback reads. No implicit split if nonzero N<F: the subtraction wraps.

Format1 flag-clear flush uses retained PRE-callback offset O=B(C): nonzero N
ends at B(wrap32(N+13)), size=wrap32(thatEnd-O); N0 ends at freshly read byte
length word1c, size=wrap32(word1c-O). Source14/selector byte0e are reread. The
N+13 addition wraps BEFORE the block mapping; it is not an unbounded ceil.
Do not replace retained O with B(F) after callback mutation.

All four branch-specific sites request
hwFlushStream(fresh source14,offset,size,fresh byte0e,0,0). No size/alignment/
capacity/encoding/callback-state validation or completion wait. Hardware child
owns upload/cache/IRQ behavior. AFTER the child, or immediately on flag bypass,
store retained N at cursor20 and branch common footer8027af10. No post-flush
state recheck/modulo recapture; a flush child's cursor mutation is overwritten.
No state2 store or format change in this region.

## Evidence and remaining work

[Manifest](../../../tools/musyx_stream_forward_refill_evidence.json) pins four
spans (region/whole handler/two linked children),126 whole-region anchors,five
direct calls and two explicitly recorded indirect callback sites. Nine block/
seven route/six callback/nine refill vectors checked;65540 unsigned division
cases (65536 small inputs/four extrema) match /14 and block mapping. Invalid
length0 retains null model output, not a runtime no-op claim. Generic verifier
checks hashes/anchors/direct calls; callback ABI/source semantics are deductions.
Fresh full ELF/listing/generator remain `/tmp`; no bodies/binary assets committed.

Remaining: wrapped-refill/common footer/ABI, actual producer callback/position/
format/cursor/length/flags/source/selector/voice/count providers and valid value/
divisor/extent/alignment/encoding/alias/lifetime/IRQ/concurrency domains, transitive
ARAM/DSP consumers and original callback/refill/stream/audio traces. EVT-01 OPEN.
