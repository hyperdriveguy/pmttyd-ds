# MusyX low-precision oscillators, periodic accumulators and pan ramps

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete LowPrecisionHandler REGION
80277788..802779d0, end exclusive,146 instructions. Whole handler80277724/744hex
is fingerprinted; pitch/controller/portamento/requeue remainder remains open.
Addresses/offsets/masks hexadecimal; shifts and row ordinals decimal.
Static source deductions, not original audio execution or measured cadence.
Read [admission/clock/ADSR regions](MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md)
and [pan producers](MUSYX_MACRO_PAN_VOLUME_AND_ENVELOPE_COMMANDS.md).

## Two ordered oscillator rows and dirty comparison

Retain voice V in r28 and wrapped low elapsed E in r31 from admission. Initialize
row ordinal0 and row pointer V; visit exactly two rows, advancing pointer byc.

| Ordinal | Phase word | Period word | Output half | Comparison half | Dirty byte |
| --- | --- | --- | --- | --- | --- |
| 0 | 1c0 | 1c4 | 1c8 | 1ca | 1d8 |
| 1 | 1cc | 1d0 | 1d4 | 1d6 | 1d9 |

Read period:0 skips the whole row without a phase addition/query/output write.
Otherwise add E to phase with wrap32 and store. Freshly reread period N then
phase Q. Compute unsigned quotient Q/N, multiply by N with low32 product,
subtract from Q, shift result left4 with wrap32, then divide UNSIGNED by N>>8.
Send low16 of this final quotient to sndSin802902f8 at802777c4. N>>8==0 is not
guarded, so the initial nonzero check alone does not prove valid division.
No numeric result is assigned here for invalid divisor/processor domains.

Store low16 return to output half. Reload output SIGNED16, then comparison
SIGNED16; equality skips remaining row work. Otherwise copy output to comparison,
then freshly read indexed V.bytea(1d8+ordinal). If0, no dirty write; nonzero clears
that byte then freshly reads V.word220 and stores OR00007fff. This is an OR,
not the later handler dirty tail's replacement. A changed output alone does not
always publish dirty bits. Row1 reads follow row0's child/writes, with retained
V and row ordinal; no base/generation/lifetime recapture.

sndSin is pinned as a linked child, not semantically closed by its hash. Source
here does not justify a host sine, amplitude/unit interpretation or bounded
phase table index. No child status or output-domain check.

## Separately flagged oscillator

After both rows read flags114 then118. Only flags118&2000 gates this oscillator;
flags114 contributes a literal-zero masked term. Set: add E to V.word14c and
store, then freshly read N=word148 and Q=word14c. Use the same unsigned
remainder/wrapped shift/divisor formula; low16 quotient goes to sndSin at8027786c.
Sign-extend low16 returned word and store FULL word150. Clear: no oscillator
writes/query. There is no initial period-nonzero check on this path, no comparison
half, dirty-byte reset, flags clear or word220 publication in this block.

## Two periodic accumulators with distinct elapsed masks

Read byte1bc then1bd. Both0 skip this section. Otherwise capture the two elapsed
forms once: A=(E&00ffffff)<<4 and B=E&0fffffff. The original rotate4/mask4..27
produces A; it is not the unrestricted E<<4. Process first then second:

| Enable/period byte | Countdown | Accumulator | Rate word |
| --- | --- | --- | --- |
| 1bc | 1b4 | 1a4 | 1ac |
| 1bd | 1b8 | 1a8 | 1b0 |

The first row uses the captured byte1bc for admission; the second rereads
byte1bd. A checked byte0 skips. Nonzero: subtract A from countdown
with wrap32 and STORE, then reload and compare SIGNED32 with0. Result<=0:
reload period byte, store byte<<16 at countdown, then store0 at accumulator.
This performs one reset, without residual time, repeated-cycle catch-up or a
clamp of arbitrary countdown values.

Positive result: read rate then accumulator; arithmetic signed rate>>12,
multiply by retained B with low32 product, add to accumulator with wrap32 and
store. No saturating sum, phase/period unit validation or output/dirty flag here.
Second row's enable byte is reread after first-row processing. The initial byte
OR only admits the section; it is not a stable snapshot of both row enables.

## Pan then surround countdown ramps

Next initialize exactly two iterations, pointer V then V+4. Their current/target/
countdown/increment fields are174/184/18c/17c and178/188/190/180 respectively.
Read current then target. Equality skips all remaining work for that row,
including countdown update and dirty publication.

Otherwise decrement countdown by FULL E with wrap32, STORE and reload. Signed
countdown<=0: freshly read raw target, copy to current, then clear countdown0.
This expiry path does NOT clamp the target to the normal range.

Positive countdown: read increment, logical countdown>>8, then target. Compute
rawCurrent=wrap32(target-wrap32((countdown>>8)*increment)) and STORE before the
clamp. Reload current: signed-negative selects0; otherwise unsigned current
>007f0000 selects007f0000; otherwise retains current. Store selected result again.
No unbounded/saturating intermediate subtraction or equality recheck.

Both processed paths freshly read V.flags114, OR2000 and store. Flags118 stays
untouched. The flag publication happens for each admitted row even when its new
current equals the old value. Row1 follows row0, then both meet the existing
ADSR request region802779d0. No hardware request or requeue in this delivery.

## Evidence and remaining work

[Manifest](../../../tools/musyx_low_precision_modulation_pan_evidence.json) pins
three spans (region/whole handler/sndSin),146 whole-region instruction anchors
and two calls. Seven isolated phase-formula/five dirty/six periodic/eight pan-ramp
vectors checked, including shift wrap/invalid divisor, signed expiry/overflow,
raw target copy and distinct elapsed masks. Phase vectors are arithmetic inputs;
period0's two-row early gate skips the formula, while flagged oscillator has no
such gate. Invalid quotient outputs are left null in the mathematical model.
Generic verifier checks source hashes/anchors/calls, not semantic deductions.
Fresh full handler ELF/listing/generator remain `/tmp`; no bodies/assets committed.

Remaining: sndSin internals, handler pitch/controller/portamento/requeue/ABI,
actual phase/period/rate/countdown/target/dirty/flag/time/voice writers and valid
index/extent/value/divisor/alias/lifetime/IRQ/concurrency domains, original
oscillator/pan/audio traces and native binding. EVT-01 remains OPEN.
