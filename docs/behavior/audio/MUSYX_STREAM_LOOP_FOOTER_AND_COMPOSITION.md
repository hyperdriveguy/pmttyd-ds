# MusyX stream loop footer, ABI and whole-body composition

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete streamHandle footer REGION
8027af10..8027af50 and hwSetStreamLoopPS80290c34/14hex; ABI prefix
8027a698..8027a6ac and suffix8027af64..8027af7c, ends exclusive. Addresses/
offsets/masks hexadecimal; shifts/counts decimal. Static source deductions,
not original stream/DSP/audio execution. Read [nonzero wrap](MUSYX_STREAM_NONZERO_POSITION_WRAPPED_REFILL.md),
[zero-position wrap](MUSYX_STREAM_ZERO_POSITION_WRAPPED_REFILL.md) and
[hardware sample schema](MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md).

## Fresh common-footer admission

At8027af10 retain stream row S. Read state+c; anything other than EXACT2 skips
footer work to row tail8027af50. Then read flags8; mask00020000 set skips.
Then read format byte0d; anything other than EXACT1 skips. These are independent
fresh reads after refill callbacks/uploads, not the captured dispatch state/format.
No local state/flags/format normalization or callback-success condition here.

This footer can execute even after a producer returned0, an unsupported captured
format skipped refill, or a callback changed state and later left current state2.
Admission follows only the fresh gates. Conversely the earlier equal-position
route jumps directly to row tail, bypassing footer; state1 startup also bypasses
it. No assertion that every state2 row updates its loop byte every scan.

## Raw translated source read and ordered loop-byte request

After admission read source word14, then voice index word4c. Compute
A=wrap32(source+40000000), read full word at A, logical>>24 to obtain byte P.
Store P at stream row.byte0f, then request hwSetStreamLoopPS(I,P) at8027af4c.
No null/alignment/extent/residency/encoding/cache/coherency check or address-map
query. This pins raw add/load order, not a validated physical/uncached alias or
proof of source data availability. The word read is not a callback invocation.

Complete hwSetStreamLoopPS(I,P): compute wrap32(I*f8), read dspVoice8041ef88,
form record pointer, store low8(P) at record.bytea0, return. No active/index/
generation/value validation, old-byte comparison, dirty flag, cache/DSP event,
IRQ or hardware call. The local byte store does not prove DSP consumption or
immediate playback behavior. Source here uses a raw fullword voice index.

## ABI saves/restores and no status contract

streamHandle prefix allocates112 decimal stack bytes, saves LR at old stack+4,
sets r11 to old stack and calls _savegpr_27 at80264988. Complete helper fallthrough
stores GPR27..31 at r11-20,-16,-12,-8,-4, then blr. Suffix sets r11 to old stack,
calls _restgpr_27 at802649d4 (five matching loads then blr), reloads LR, releases
stack and returns. Those helpers' CSV entry symbols have4-byte boundaries;
full24-byte execution spans are independently pinned here and in the
[existing helper contract](MUSYX_LOW_PRECISION_FINAL_PITCH_AND_COMPOSITION.md).
No independent success/status return assignment. Valid ABI/stack/register/
child-preservation and exception domains remain runtime prerequisites.

## Complete local streamHandle composition

| Contract | Regions | Semantic words |
| --- | --- | --- |
| [Throttle/startup/row tail](MUSYX_STREAM_THROTTLE_DISPATCH_AND_STARTUP.md) | Two | 151 |
| [Position/forward refill](MUSYX_STREAM_POSITION_AND_FORWARD_REFILL.md) | 8027a8f4..8027aaec | 126 |
| [Zero-position wrap](MUSYX_STREAM_ZERO_POSITION_WRAPPED_REFILL.md) | 8027aaec..8027acb0 | 113 |
| [Nonzero-position wrap](MUSYX_STREAM_NONZERO_POSITION_WRAPPED_REFILL.md) | 8027acb0..8027af10 | 152 |
| This footer | 8027af10..8027af50 | 16 |

Six disjoint regions cover8027a6ac..8027af64 without gaps:558 semantic words.
Eleven individually anchored ABI words complete569 instructions/8e4hex at
8027a698. All four prerequisite region manifests and the ABI helper prerequisite
reverified against the read-only ISO. Every local region now has a contract;
this is source composition, not transitive execution/audio fidelity.

[New manifest](../../../tools/musyx_stream_finish_composition_evidence.json)
pins seven spans,44 anchors (16 footer/11 ABI/12 helper/5 loop-PS leaf),three
call sites. Seven gate/five translated-address/high-byte/four loop-PS vectors and
exact region/ABI composition checked. Address vectors are integer deductions,
not claims that those example addresses are valid loads. Generic verifier checks
hashes/anchors/direct targets, not source domain or runtime behavior. Fresh full
handler/leaf ELFs/listings/generator remain `/tmp`; no bodies/assets committed.

Remaining: stream allocation/activation/deactivation/free/frequency/mix lifecycle,
actual producer/delay/count/state/format/descriptor/position/cursor/length/flags/
source/selector/voice/loop-PS providers and valid value/divisor/extent/alignment/
encoding/address-map/cache/alias/lifetime/IRQ/concurrency/ABI/FP domains, transitive
ARAM/DSP consumers and original startup/refill/stream/audio traces. EVT-01 and
full native/full-game binding remain OPEN.
