# MusyX zero-offset filter, priority, requeue and whole-body composition

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete semantic region802784ac..802785d8;
ABI prefix80277e68..80277ea4 and suffix802785d8..80278618, ends exclusive.
Addresses/offsets/masks hexadecimal; shifts decimal. Static source deductions,
not original execution or measured scheduling/audio.

## Independent filter admission and exact parameter request

After the [volume/aux block](MUSYX_ZERO_OFFSET_VOLUME_AND_AUX_OUTPUT.md), read
V.word220 fresh. Only mask6000 nonzero admits filter work; otherwise skip directly
to priority. No local dirty-bit clear. Request inpGetFilterSwitch(V)8028fad0 at
802784bc. Low16 return<=8191 requests hwSetFilter(I,0,0,0) at8027855c; greater
requests inpGetFilterParameter(V)8028fb18 at802784d0 and proceeds enabled.
I is retained raw voice index; V is retained synth voice pointer.

After Parameter returns, capture A=V.word1e0 then B=V.word1e4. Convert unsigned
low16 parameter P and wrap32(B-A) through the unsigned double bias80429148.
With each operation a separate single result:

```
weight = S(1 - S(C * P))
conversionArgument = S(weight * S(unsigned32(wrap32(B-A))))
```

C=8042913c bits38800200;1=80429130 bits3f800000. Request
__cvt_fp2unsigned80264860 at80278520, then add returned raw word to A with wrap32.
The subtraction is wrapped and its conversion UNSIGNED, even B<A. Do not replace
this with an unbounded signed interpolation or add A before conversion. No
parameter/frequency/domain clamp or conversion-status test here. Read the
[conversion helper](MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md) for its local behavior;
exceptional conversion and valid caller domains remain separate.

Request hwLowPassFrqToCoef(result,&stackHalf0a,&stackHalf08)80290d7c at80278530.
Read half0a then half08, request hwSetFilter(I,1,first,second) at80278544. Outputs
are distinct stack halfwords under ordinary valid-stack execution. Ignore child
returns; no local output initialization/status rollback. Read the
[coefficient/setter contracts](MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md).

## Priority countdown and packed request

Freshly read V.word110. Exactly0 skips this whole priority update. Otherwise
read U16 half10e; multiply by retained low elapsed E from
[admission](MUSYX_ZERO_OFFSET_ADMISSION_AND_RAMP.md), retaining low32 product.
Subtract from old110 with wrap32 and STORE raw result at110 before the signed
check. Signed-negative result then stores0; nonnegative retains raw result.
No unbounded/saturating intermediate math, elapsed clamp or counter validation.

Reload word110, then U8 byte10c. Pack word=(byte10c<<24)|(word110 logical>>15)
and request hwSetPriority(I,word)80290800 at8027859c. No equality/change test:
an initially nonzero counter always requests, including when it reaches0.
A zero initial counter does not normalize a malformed rate/priority/counter.
Read [priority consumer](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md).

## Requeue, dirty tail and ABI

Every admitted handler path, after priority, requests hwGetTimeOffset802907d0
at802785a0. Use low8 return T; compute D=wrap32((5-T)<<8); request
synthAddJob(V,1,D)80278618 at802785b8. T>5 produces wrapped negative arithmetic;
there is no delay clamp or rejection here. Child return ignored. Read
[job insertion](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md) and
[offset leaf](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).

Both this ordinary finish and inactive admission shortcut reach802785bc.
Read V.bytea8;0 skips writes. Nonzero clears bytea8 then REPLACES word220 with
00007fff, not OR. This publication is AFTER requeue on admitted paths. It does
not reset the envelope/voice or rerun skipped filter/volume work immediately.

The prefix allocates160 decimal stack bytes and saves LR, GPR28..31 and FPR28..31
(including paired-single save slots). The suffix restores those registers/LR,
releases the stack and returns. No independent success/status return assignment;
caller treats this as a work handler. ABI, stack/GQR/FP and child-preservation
conditions remain part of the valid execution domain.

## Complete local composition and validation

The following five adjacent semantic regions cover every instruction outside
ABI saves/restores of ZeroOffsetHandler80277e68/7b0hex:

| Contract | Half-open range | Words |
| --- | --- | --- |
| [Admission/ramp](MUSYX_ZERO_OFFSET_ADMISSION_AND_RAMP.md) | 80277ea4..80277fe0 | 79 |
| [Gain/attenuation](MUSYX_ZERO_OFFSET_GAIN_AND_ATTENUATION.md) | 80277fe0..802781e0 | 128 |
| [Pan/surround](MUSYX_ZERO_OFFSET_PAN_AND_SURROUND.md) | 802781e0..80278328 | 82 |
| [Volume/aux](MUSYX_ZERO_OFFSET_VOLUME_AND_AUX_OUTPUT.md) | 80278328..802784ac | 97 |
| This finish | 802784ac..802785d8 | 75 |

Union audit proves461 semantic words without gaps/overlap, plus31 individually
anchored ABI words, total492 instructions/7b0hex. All four prerequisite manifests
reverified against the read-only ISO; the whole local body is now answered.
This is source composition, not proof of transitive or original runtime fidelity.

[New manifest](../../../tools/musyx_zero_offset_finish_composition_evidence.json)
pins13 spans,106 anchors (75 finish/31 ABI),nine call sites/eight distinct children.
Six filter-gate/six filter-math/six priority/six requeue/three dirty vectors checked.
Finite FP vectors model separate host single rounding; exceptional conversion
vectors retain null model output rather than invent a return. Generic verifier
checks source hashes/anchors/calls, not semantic/source-domain deductions. Fresh
full handler ELF/listing/generator remain `/tmp`; no bodies/binary assets committed.

Remaining: LowPrecisionHandler remainder, actual voice/time/gain/controller/
filter/priority/dirty/index/source writers, valid value/address/extent/alias/
lifetime/IRQ/concurrency/ABI/FP/conversion domains, transitive hardware/DSP/job
consumers and original filter/requeue/voice/audio traces. EVT-01 and full native
binding remain OPEN.
