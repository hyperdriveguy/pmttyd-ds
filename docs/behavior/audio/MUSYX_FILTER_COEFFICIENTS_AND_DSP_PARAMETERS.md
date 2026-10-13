# MusyX filter coefficients and DSP parameter publication

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete hwSetFilter80290e74/80 and
hwLowPassFrqToCoef80290d7c/f8hex; complete salBuildCommandList REGION
8028a2c0..8028a354 exclusive. Whole builder80288e00/25f8 fingerprinted,
other regions remain open. Addresses/offsets/masks hexadecimal; shifts/counts
and scalar constants decimal. Read
[hardware sample/SRC/polyphase/ITD writers](MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md),
[volume ITD offsets](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md),
[preceding ramp consumers](MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md) and
[following envelope region](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md).
Static operations, not measured filter response/DSP/audio.

## Complete filter setter state machine

hwSetFilter(I,M,A,B) captures V=dspVoice8041ef88+wrap32(I*f8).
Read V.byted4. Mode admission uses low8(M)==1, not generic nonzero.

| Old byte d4 | Mode low byte | Writes in order |
| --- | --- | --- |
| Zero | Not1 | Return without writes |
| Zero | Exactly1 | byte d4=1; halfd6=low16(A); halfd8=low16(B); freshly word24 OR c00 |
| Any nonzero | Exactly1 | halfd6=low16(A); halfd8=low16(B); freshly word24 OR800 |
| Any nonzero | Not1 | byte d4=0; freshly word24 OR400 |

Enabling an already-nonzero raw d4 does not normalize it to1. Disabling retains
old coefficients; inactive invalid mode preserves all fields. No coefficient
comparison/duplicate suppression, active/system/ordinal/type/value check,
base recapture, time-indexed event slot, IRQ request or direct hardware call.
Dirty word is slot0 word24. No independent scalar success assignment.

## Complete unsigned input/coefficient wrapper

hwLowPassFrqToCoef(F,Oa,Ob) retains two halfword output pointers. F is raw
UNSIGNED32; its name labels source role rather than validated physical units.
Build double43300000/F, SINGLE fsubs with original unsigned bias
4503599627370496 at804292c0. SINGLE fmuls by6.2831854820251465 at804292ac,
then SINGLE fdivs by32000.0 at804292b0. Request sndCos8029047c with this result.
Do not widen/reassociate multiplication/division or substitute signed conversion.
No frequency range/finite clamp or errno/status test.

Set beta=SINGLE fsubs(original2.0 at804292a8,child return). Ordered beta>1.0:
SINGLE square beta*beta, SINGLE subtract1.0; request sndSqrt80290428; set raw
coefficient c=SINGLE fsubs(child return,beta). Otherwise set c to original-1.0
at804292b8, including unordered beta if execution continues past FP exceptions.
No sqrt request on the <=1/unordered branch. Math children are fingerprinted only;
standard-library cosine/square root cannot be substituted without their contracts.

Clamp with ordered comparisons: c>1.0 selects original1.0; otherwise c<-1.0
selects original-1.0; otherwise retain c, including unordered c. Original constants
are at804292b4/b8. This is not a finite/NaN repair or rounding-mode policy.

Negate selected c with fneg. SINGLE add1+c; separately SINGLE multiply both
(1+c) and (-c) by original32768.0 at804292bc; signed fctiwz on each. Store low16
of the first converted word to Oa, then low16 of the second to Ob. No unsigned
conversion, saturation, endian descriptor packing or fullword output store.
At selected c=1, first scaled value65536 stores half0; c=-1 gives0/8000.
These are stage deductions, not proven reachable frequency/response behavior.
Output overlap makes the second store win; null/alignment/alias and invalid
conversion/FPSCR domains remain caller/runtime requirements. No independent
status is assigned; output arithmetic must not be interpreted as a success code.

## Complete DSP parameter-copy region

Retained V=r29 and parameter block PB=r19 come from preceding builder work.
No base/pointer recapture. Gates execute in this order, each freshly reading
V.word24. Skipped gates leave destination fields unchanged.

| Gate bit | Additional test | Source/destination writes |
| --- | --- | --- |
| 200 | None | V.halfd0 to PB.half40; then V.halfd2 to PB.half42 |
| 100 | None | V.halfcc to PB.half08 |
| 80 | None | V.halfce to PB.half0a |
| 400 | None | PB.halfba=(V.byted4!=0); then PB.halfbc=0 |
| 800 | Fresh V.byted4!=0 | V.halfd6 to PB.halfbe; then V.halfd8 to PB.halfc0 |

Filter-enable boolean uses neg/or/logical sign extraction on an unsigned byte;
all nonzero bytes map to1. Filter coefficient gating rereads d4 independently
of the earlier enable gate. Bit800 with d4==0 leaves PB coefficients unchanged;
bit400 with d4==0 still publishes enable0 and clears PB.bc. There is no general
filter reset/copy of zero coefficients. SRC/polyphase/ITD values are unsigned
halfword copies, not converted or bounded by this consumer.

The region does not clear any gate bit, emit a DSP command, flush cache, call a
child, wait for hardware or assign status. Next code begins8028a354, the already
recovered envelope prefix. Pointer/value/flag overlap or shared writers can
change later gates/source reads; no atomic publication or alias protection.
Original audible behavior requires command/cache/DSP execution evidence.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_filter_coefficients_and_dsp_parameters_evidence.json)
pins two complete bodies, three linked fingerprints (math children/whole builder),
one complete consumer region/seven numeric literals:13 spans,80 anchors/two
calls. Nine setter-state/seven consumer-gate/five input-scaling/six final-clamp
vectors are source deductions; clamp vectors test synthetic selected coefficients,
not original sndCos/sndSqrt execution or reachable raw frequencies. Full fresh
ELFs/listings remain `/tmp`; no bodies/binary assets committed.

Remaining: full sndCos/sndSqrt/source math/FPSCR/transitive execution, actual
frequency/mode/coefficient/flag/record/PB/type/value/alias/lifetime/IRQ/concurrency
domains, surrounding builder setup/cache/command consumers and original filter/
parameter/DSP/audio traces. Two local bodies/one consumer region answered;
whole builder/full native binding and EVT-01 remain OPEN.

## 2026-10-13 — Math-helper follow-up

[Paired cosine/root helpers](MUSYX_PAIRED_COSINE_AND_SQRT_HELPERS.md) closes the local sndCos/sndSqrt
bodies. Original paired/fused/estimate instructions are retained; GQR/FPSCR/
seed/caller domains and original math/filter/DSP/audio execution remain open.
