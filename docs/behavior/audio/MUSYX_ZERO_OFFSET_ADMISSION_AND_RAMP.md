# MusyX zero-offset admission, timestamp and ramp

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete region80277ea4..80277fe0,
end exclusive, within ZeroOffsetHandler80277e68/7b0hex. Offsets/addresses/masks
hexadecimal; shifts decimal. Whole handler and child fingerprints do not close
remaining semantics. Static source deductions, not original execution or measured
cadence. Read [job drain/caller](MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md)
and [low-precision counterpart](MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md).

## Admission and captured timestamp

Retain raw input I in r29. Capture synthVoice8041ee90 and compute
V=base+wrap32(I*458), retained in r30 BEFORE hwIsActive(I)802907d8 at80277eb4.
If child returns0 AND V.word34==0, branch802785bc, skipping this timestamp/ramp
and the remaining admitted body. Nonzero child or nonzero word34 admits work.
No truncation/index/address/generation/lifetime check here; no global-base
recapture after child. Epilogue and child semantics remain separate.

Read synthRealTime8041eea0.high then low. Read previous V.low30 then high2c.
Compute low E=wrap32(newLow-oldLow) with subc, then high difference with subfe;
only low E is retained in r31. Store new low to V.word30 then new high to
V.word2c. The high subtraction result is discarded. No clamp, time conversion,
atomic timestamp-pair publication or OS clock request. These are different
previous-time fields from LowPrecisionHandler's24/28 pair.

## Wrapped ramp and signed target clamp

Capture V.flags114 and flags118. Only flags118&00008000 gates the ramp;
flags114 contributes a literal-zero masked term to that test. If set:

1. Read raw rate R=V.word198; K=E logical-right-shift8. Write
   V.word1a0=wrap32(oldAccumulator+wrap32(R*K)).
2. Freshly reread R. If signed32(R)<0, clamp when signed32(target word19c)
   >=signed32(updated accumulator). Otherwise clamp when target<=accumulator.
   Equality clamps in both branches; rate0 takes the nonnegative branch.
3. A clamp writes raw target to word1a0, freshly loads flags118 then flags114,
   clears only flags118 bit8000 and stores118 then114 unchanged. It does not
   clear low-word bit1000. Without a clamp, no flag writes in this region.
4. Freshly read word1a0 and copy to word158; set carried update indicator r0=1.
   This publication occurs on every admitted ramp call, even K0/rate0 and even
   when the clamp has just cleared the gate. It does not compare old word158.

The multiplication/addition wrap BEFORE signed comparison. For example,
R=1,K=1,old=7fffffff gives80000000: target0 does not clamp in the nonnegative
branch. R=ffffffff,K=1,old=80000000 gives7fffffff: target0 does not clamp in the
negative branch. Do not replace the body with unbounded arithmetic, saturating
addition or a monotonic target-crossing rule. No rate/target/accumulator domain
checks justify interpreting the raw fields as a physical unit here.

## Inactive ramp and region boundary

If flags118 bit8000 is clear, leave accumulator and word158 untouched. From the
previously captured flags114 compute r0=1 exactly when bit1000 is set, otherwise0.
The source addic/subfe pair converts the masked value to that Boolean; it does
not preserve1000 as the result. Other flags118 bits do not enable this path.

Both branches meet80277fe0 with their carried update indicator. That next region
freshly reads/clears flags114 bit1000 and performs table/gain/controller/output
work; it is outside this delivery. No claim that r0 immediately starts hardware,
that bit1000 owns every update, or that the whole handler is now closed.

## Evidence and remaining work

[Manifest](../../../tools/musyx_zero_offset_admission_ramp_evidence.json) pins
three spans (complete region plus linked whole handler/child), every79 region
instruction word and one direct call. Four admission/five low-time/fourteen ramp
vectors record the source predicates, wrap/clamp/flag/publication order. They are
mathematical deductions; the generic verifier checks source spans/anchors/call,
not the semantics or original runtime. Fresh full handler ELF/listing and generator
stay `/tmp`; no original bodies/binary assets committed.

Remaining: ZeroOffsetHandler's table/gain/controller/auxiliary/hardware/output/
requeue/epilogue regions, hwIsActive semantics, actual voice/time/rate/target/flag/
accumulator providers, source-valid extent/type/index/alias/lifetime/IRQ/concurrency
and timing domains, original voice/audio execution and native binding. EVT-01
remains OPEN.

## 2026-10-13 — Gain/attenuation follow-up

[Gain/attenuation](MUSYX_ZERO_OFFSET_GAIN_AND_ATTENUATION.md) closes the next flag-clear/live-fader/track/
base-gain/modulation/tremolo/attenuation region. Pan/aux/output/requeue and
provider/domain/original audio/native binding remain open.
