# MusyX stream nonzero-position wrapped refill

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete streamHandle region
8027acb0..8027af10, end exclusive,152 instructions. Addresses/offsets/masks
hexadecimal; divisors decimal. Static source deductions, not original stream/
callback/audio execution. Read [position/forward refill](MUSYX_STREAM_POSITION_AND_FORWARD_REFILL.md),
[zero-position wrap](MUSYX_STREAM_ZERO_POSITION_WRAPPED_REFILL.md) and
[flush bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).

## Two-span producer callback

Entry requires captured cursor C>normalized position P>0, unsigned, from prior
regions. Captured format0/1 admits;2..255 skips to common footer8027af10.
Read current logical length word18=L, source14, callback10 and user50. Let
B(x)=8*floor(unsigned32(x)/14), as established in the position contract.

| Format | Callback arguments |
| --- | --- |
| 0, site8027acec | (source+wrap32(2*C),wrap32(L-C),source,P,user) |
| 1, site8027ae04 | (source+B(C),wrap32(L-C),source,P,user) |

Unlike the forward/zero-position paths, third/fourth arguments are source/P,
not0/0. These are the tail and head span inputs; the actual producer's encoding/
fill behavior remains separate. Format1 retains O=B(C) across callback.
All pointer/count additions wrap32. No callback-null/signature/source/extent/
alignment/count/IRQ check. Format remains captured even if child changes byte0d.

R0 returns to footer without state read. Nonzero R reloads state+c; only exactly2
admits further work. No callback-count/request-capacity validation or rollback.
The callback's earlier writes remain even when this gate rejects publication.

## Fresh modulo, suppression and strict split decision

Admitted: read fresh cursor20=F then fresh logical length18=N. Compute
K=wrap32(F+R)%N through unsigned division/multiply/subtract when divisor valid.
N0 is unguarded; invalid divisor domains remain unresolved. Read flags8; mask
00020000 set skips uploads but still stores new cursor K afterward.

When uploads admitted, compare raw returned R UNSIGNED against wrap32(N-F).
Only STRICT R>wrap32(N-F) selects two uploads. Equality uses the single-upload
branch. This comparison uses post-callback F/N, not captured C/L/P or K. No
clamp guarantees F<=N or R within the callback's requested spans.

## Ordered upload sizes

Define O=wrap32(2*F) for format0; format1 uses retained PRE-callback B(C).
Source14 and selector byte0e are reread for EACH upload. Byte length word1c is
freshly read when forming a tail-to-end size. All arithmetic wraps32.

| Path | Ordered (offset,size) requests |
| --- | --- |
| Split,format0 | (O,word1c-O), then (0,wrap32(2*K)) |
| Split,format1 | (O,word1c-O), then (0,B(K)) |
| Single,K0,either format | (O,word1c-O) |
| Single,K nonzero,format0 | (O,wrap32(2*wrap32(K-F))) |
| Single,K nonzero,format1 | (O,wrap32(B(wrap32(K+13))-O)) |

Each requests hwFlushStream(source,offset,size,selector,0,0). Split first calls
at8027ad50/8027ae68; second calls at8027ad6c/8027ae9c. Single zero calls at
8027ad98/8027aec8; single nonzero at8027adbc/8027af08.

The split format1 HEAD size B(K) rounds down. Single-span END B(wrap32(K+13))
rounds up absent overflow. Do not replace both with a common rounded size.
Split K0 still posts its second zero-length request; there is no suppression.
There is no status/state recheck or source snapshot between the split calls:
first upload's child can alter second source/selector reads. K/O stay retained.
No further split for large R, size clamp, negative-wrap rejection, completion
wait, callback or capacity validation is added locally.

After all uploads, or on flag bypass, store retained K at cursor20; branch
common footer8027af10. No post-upload modulo/state recapture. Child cursor writes
can be overwritten; producer format/state mutations are not normalized here.

## Evidence and remaining work

[Manifest](../../../tools/musyx_stream_nonzero_wrapped_refill_evidence.json)
pins three spans (region/whole handler/flush child),152 whole-region instruction
anchors,eight direct call sites and two recorded indirect callback sites. Six
gate/fourteen ordered-refill vectors checked, including strict equality/overshoot,
zero-length second post, round-down versus round-up, callback cursor mutation,
byte-length distinction, bypass and invalid divisor unresolved. Generic verifier
checks source hashes/anchors/direct targets, not producer semantics or original
execution. Fresh ELF/listing/generator remain `/tmp`; no bodies/assets committed.

Remaining: common footer/ABI and whole-handler composition, actual callback/
position/format/cursor/length/flags/source/selector/voice/count providers and
valid divisor/value/extent/alignment/encoding/alias/lifetime/IRQ/concurrency
domains, transitive ARAM/DSP consumers and original wrapped-refill/stream/audio
traces. EVT-01 and native binding remain OPEN.

## 2026-10-13 — Footer/composition follow-up

[Footer/composition](MUSYX_STREAM_LOOP_FOOTER_AND_COMPOSITION.md) closes footer/loop-PS/ABI and composes the
whole local handler. Lifecycle/providers/domains/transitive ARAM/DSP/original
stream/audio/native binding remain open.
