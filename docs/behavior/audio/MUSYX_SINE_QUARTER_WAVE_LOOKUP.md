# MusyX integer sine quarter-wave lookup

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndSin802902f8/8chex and original
sndSintab803be710/800hex data identity. Addresses/masks hexadecimal; phase/index/
output values decimal. Static source deductions, not original audio execution.
Read [oscillator requests](MUSYX_LOW_PRECISION_MODULATION_AND_PAN_RAMPS.md).

## Complete phase folding and returned word

Input is raw r3 word X. P=X&00000fff, so every input reduces to phase0..4095.
No caller low16 normalization or signed-input interpretation is needed locally.
Let T[i] be signed16 loaded from sndSintab+2*i. The complete branch mapping is:

| Masked phase | Index | Returned signed word |
| --- | --- | --- |
| 0..1023 | P | Sign-extended T[index] |
| 1024..2047 | 1023-(P&1023) | Sign-extended T[index] |
| 2048..3071 | P&1023 | Sign-extend low16 of -T[index] |
| 3072..4095 | 1023-(P&1023) | Sign-extend low16 of -T[index] |

Positive branches use lhax and return directly. Negative branches use lhax,
32-bit neg, then extsh; the final narrowing matters if a runtime table value
were-32768. No interpolation, floating math, timer read, allocation, child call,
phase writeback, dirty publication or error/status branch. All table indices
are bounded0..1023 for every32-bit input by these local masks/branches.

Mirror endpoints use1023, not1024: phases1023/1024 both read T[1023]; phases
2047/2048 both read T[0], with the latter using the negative branch. Phases
3071/3072 both negate T[1023]. Phase4095 returns negative T[0];4096 wraps to0.
Do not replace this with a smooth host sine or a differently sampled table.

## Original numeric table and source/runtime boundary

The manifest records all1024 signed numeric coefficients decoded big-endian
from the read-only ISO and fingerprints their2048-byte source span. Source
T[0..2]=0,6,12; last three=4095,4095,4095. Minimum0,maximum4095. Therefore this
source table yields returned values-4095..4095; full-scale signed16 amplitude
is not the local contract. Mathematical phase checks do not establish physical
frequency, modulation depth, audible gain or original invocation cadence.

sndSintab belongs to initialized .data. These are the original image values,
not proof of immutable runtime storage or absence of writers/patches. The body
reads the selected halfword live; no table snapshot or initialization request.
The source body is integer-only; caller's divisor/phase/provider domains remain
separate even though this callee's index is locally bounded.

## Two pinned oscillator request bindings

LowPrecisionHandler calls at802777c4 and8027786c target this exact entry. Both
supply low16 of the preceding wrapped unsigned quotient, which this child
further reduces to low12. The two-row path stores returned low16 then compares
signed halves; flagged oscillator sign-extends low16 to a fullword. Existing
[caller contract](MUSYX_LOW_PRECISION_MODULATION_AND_PAN_RAMPS.md) owns those
publications and flags. These two bindings are not a complete indirect/REL/
runtime caller census, nor proof that the caller's divisions were valid.

## Validation and remaining work

[Manifest](../../../tools/musyx_sine_quarter_wave_lookup_evidence.json) pins
three spans (body/table/linked caller),37 anchors (all35 body instructions/two
caller sites),two caller bindings; the leaf itself has zero calls. Eighteen
boundary/high-bit vectors and all4096 admitted phases checked against independent
branch/index formulas, plus numeric-table big-endian reserialization/hash check.
Generic verifier checks hashes/anchors/call targets, not semantic deductions or
runtime table ownership. Fresh ELF/listing/generator stay `/tmp`; original binary
bodies/table bytes are not committed, only numeric coefficients/evidence metadata.

Remaining: table writers/residency/patch/alias/lifetime/IRQ/concurrency domains,
caller division/phase/time/depth providers and LowPrecisionHandler pitch/controller/
portamento/requeue remainder, indirect/dynamic callers, original oscillator/audio
traces and native binding. EVT-01 remains OPEN.
