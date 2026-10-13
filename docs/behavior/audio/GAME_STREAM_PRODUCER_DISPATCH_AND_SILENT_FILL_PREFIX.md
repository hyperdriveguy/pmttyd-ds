# Game stream producer dispatch and silent-fill prefix

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped _sscallback800deda0..800df0d4 exclusive,
205 instructions, plus return ABI800df5a4..800df5c4 (8). Whole callback824hex/
521 instructions remains in progress; live refill/helper continuation is open.
Addresses/offsets/masks hexadecimal; counts/arithmetic decimal. Static deductions,
not original mute/refill/audio traces. Read
[header callback binding](GAME_STREAM_HEADER_ALLOCATION_CACHE_AND_COMPOSITION.md),
[MusyX invocation](MUSYX_STREAM_POSITION_AND_FORWARD_REFILL.md) and
[game mute/continue](GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md).

## Five-argument callback and low-byte row selection

Inputs (D1,N1,D2,N2,T)=r3,r4,r5,r6,r7, matching MusyX's two-span producer ABI.
Retain N1 in r8, N2 in GPR29; keep first pointer and second pointer for fill.
T low8 selects V=poolWord100+low8(T)*138hex, S803e0138. No channel<3, type/
ID/generation/index/null/extent/pool/lifetime check before the pool load and row
address calculation. High tag bits, including100, do not affect row selection.
Tag258decimal selects row2; tag3 selects outside the three-row scan domain with
no local guard. Later live route interprets mask100 separately.

AFTER loading pool/calculating V, if ALL four D1,N1,D2,N2 are exactly0, return0
without row count/flags/latch reads or writes. A zero pointer with nonzero count
is not this special case. No null/length pairing validation is inserted.

Other calls store0 at V.word24 BEFORE reading U16 V.flags0. Captured flags
mask8006 nonzero selects silent fill. Otherwise read shared S.half214/803e034c;
exactly0 selects live refill at800df0d4, EVERY other value selects silence.
No mask8/nonzero-flags admission, shared counter/OS IRQ request or latch write.
Mask8006 includes2,4,8000; clearing one bit does not establish live admission.
Captured flags, not a fresh read, choose silence format via mask100.

## Mask100 clear: PCM zero halfwords and publish count after each store

N1 nonzero: zero N1 halfwords sequentially at D1, eight-store groups (16-byte
pointer advance) then remainder. Each halfword store is followed by fresh load
of V.word24, wrap32 increment and store. Then do the same for N2 at D2.
N0 skips that span's stores, regardless of pointer value. No child request,
copying, pointer bound, capacity clamp or cursor/record/index mutation here.

Return FRESH V.word24 after both fills at800defcc. Under nonaliasing/stable
providers its value=wrap32(N1+N2), with2*N1 and2*N2 bytes written respectively.
Do not replace the per-store publication with a retained count when describing
original ordering. Buffer/row/count aliasing or external mutation can change
subsequent writes and returned count; actual valid domains remain separate.

## Mask100 set: encoded block zeroing; return requested counts

Convert EACH unsigned count N using the exact original24924925 sequence:

    H = high32(N * 24924925hex)
    Q = wrap32(((wrap32(N-H)) >> 1) + H)
    W = wrap32((Q & fffffff8hex) + (Q >> 31))
    B = raw32(arithmetic_shift_right(signed32(W), 1))

B is the halfword-loop count. In the finite ordinary domain before the sign
adjustment becomes relevant, B=4*floor(N/14), i.e.8 bytes per complete14-count
block. No +13 ceiling adjustment appears here. N13 writes0, N14 writes8 bytes,
N15 also writes8. Raw large/signed-loop domains are not silently normalized.

For each span, B0 skips stores; otherwise zero B halfwords at D1 orD2, with
eight-store groups then remainder. NO V.word24 increment in this branch; it
stays0 under stable nonaliasing input, although fill aliases can change it.
No coefficient/header write or actual audio-mode interpretation is inferred
from zero halfwords alone. Valid encoded buffer/data/cadence domains remain open.

Return wrap32(ORIGINAL N1+ORIGINAL N2) at800df0cc, independent of B and stored
word24. Thus the source can return13 after writing no encoded bytes. This raw
return does not prove all requested samples/bytes were produced. MusyX's later
cursor/flush consumers remain governed by their linked contracts and providers.

## ABI, evidence and continuation

Entry allocates32 decimal stack bytes, saves LR/GPR28..31; common epilogue
restores those and returns the explicitly assigned count. No direct child call,
FP instruction or cache service in this prefix. Live branch800df0d4 starts tag
mask100 subrecord selection and copy/refill processing; that remains open here.

[Manifest](../../../tools/game_stream_producer_silence_prefix_evidence.json) pins
two spans/all213 body-word anchors,zero calls. Ten encoded-count/seven tag/ten
dispatch/six PCM/five all-zero vectors checked. Count models assume stable
nonaliasing buffers; mathematical source deductions separate from original
execution. Large encoded vectors record raw loop words, not actual giant fills.
Generic verifier checks pins, not prose/providers/runtime. Fresh full ELF/listing/
scoped generator stay `/tmp`; no bodies/binary assets committed.

Remaining: whole _sscallback live refill/copy helpers and actual callback args/
tag/channel/pool/flags/latch/buffer/count providers, valid value/extent/alias/
lifetime/IRQ/concurrency/ABI domains, deferred manager/transitive SDK/cache/ARAM/
AI/DSP and original mute/short-span/block-boundary/refill/callback/audio traces.
EVT-01/native/full-game remain OPEN.
