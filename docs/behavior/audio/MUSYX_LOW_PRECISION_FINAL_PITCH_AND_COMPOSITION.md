# MusyX low-precision final pitch, requeue and whole-body composition

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete new semantic region
80277dbc..80277e34; ABI prefix80277724..80277738 and suffix80277e50..80277e68,
ends exclusive. Offsets/addresses/masks hexadecimal; shifts decimal. Static
source deductions, not original pitch/audio execution or measured cadence.
Read [portamento](MUSYX_LOW_PRECISION_PORTAMENTO_TRANSITIONS_AND_INTERPOLATION.md),
[ADSR contribution/dirty tail](MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md),
[pitch lookup helpers](MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md),
[controller wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md),
[hardware pitch](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md) and
[job insertion](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md).

## Note lookup and optional fraction

Retain V, raw input voice index I and prepared P=r30 after portamento/ADSR.
Read V.word124 as raw second argument; request
sndGetPitch((P logical>>16)&ff,word124)802862b8 at80277dc4. No note127 clamp,
status test or assumed sample-rate validity. The request uses eight bits of the
integral portion, not all16. Original helper owns its default/lookup behavior.

Take F=P&ffff and B=wrap32(return<<16). F0 skips the next lookup and retains B.
Otherwise Q=B logical>>16; request sndPitchUpOne(Q)8028627c at80277ddc. Set
D=low16(return)-Q; multiply F*D with low32 product and add to B with wrap32.
There is no float interpolation, saturation, signed-half Q or widened retained
base return. Even a raw return wider than16 bits loses its high bits in B.
The fraction and request inputs remain retained across children.

## Fresh periodic additions and doppler scaling

Read full word1a8 then word1a4, after the lookup children. Add first1a8 then1a4
to the retained pitch word with wrap32. Request inpGetDoppler(V)8028f890 at
80277e04. Its mutations do not cause these addends or pitch to be recaptured.
Use low16 returned word R. Q=summedPitch logical>>16; compute low32 product
W=Q*R. The rotate19/mask16..31 selects (W logical>>13)&ffff. Store that halfword
at V.half212, then request hwSetPitch(I,selectedHalf)80290cac at80277e20.

No rounding increment, arbitrary frequency-domain clamp or dirty equality test.
The raw product/shift/narrowing, halfword snapshot and hardware input are the
same selected bits. Child return ignored; no rollback or base recapture. Hardware
consumer owns its local DSP-field/dirty behavior, not this caller's provider
invariants or measured audio outcome.

## Requeue, tail and complete ABI helpers

After hardware request, request synthAddJob(V,0,00000f00)80278618 at80277e30;
ignore return. This constant delay is in the source job interface's units, not
a measured physical period. Then reach the existing dirty tail: nonzero bytea8
clears itself and replaces word220 with00007fff. Inactive admission goes directly
to that tail, skipping lookup/hardware/requeue entirely.

Prefix allocates48 decimal stack bytes, saves LR at old stack+4, sets r11 to old
stack and calls _savegpr_27 at80264988. The entry falls through five stores:
GPR27..31 at r11-20,-16,-12,-8,-4 respectively, then blr. Suffix sets r11 to old
stack and calls _restgpr_27 at802649d4: five corresponding loads then blr; reloads
saved LR, restores stack and returns. Symbol CSV gives each entry a4-byte boundary;
this delivery pins COMPLETE24-byte entry-through-31/return execution spans, not
only the first word. No independent success return assignment or return-status
promise. Valid stack/ABI/register preservation remains a runtime precondition.

## Whole local body composition and evidence

| Source contract | Regions | Semantic words |
| --- | --- | --- |
| [Admission/ADSR/contribution/tail](MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md) | Four regions | 67 |
| [Oscillators/periodic/pan](MUSYX_LOW_PRECISION_MODULATION_AND_PAN_RAMPS.md) | 80277788..802779d0 | 146 |
| [Pitch preparation](MUSYX_LOW_PRECISION_PITCH_PREPARATION.md) | 80277a30..80277bb0 | 96 |
| [Portamento](MUSYX_LOW_PRECISION_PORTAMENTO_TRANSITIONS_AND_INTERPOLATION.md) | 80277bb0..80277d7c | 115 |
| This output | 80277dbc..80277e34 | 30 |

These eight disjoint regions cover80277738..80277e50 without gaps:454 semantic
words. Eleven ABI words complete465 instructions/744hex at80277724. All four
prerequisite region manifests reverified against the read-only ISO. This closes
the whole local LowPrecisionHandler source contract; transitive source/runtime
fidelity is not implied by the region union or body fingerprint.

[New manifest](../../../tools/musyx_low_precision_finish_composition_evidence.json)
pins11 spans,53 anchors (30 output/11 ABI/12 helper),seven call sites. Six lookup/
six doppler vectors and bit-extraction/composition checks passed. Generic verifier
checks hashes/anchors/calls, not semantic deductions or provider assumptions.
Fresh full handler ELF/listing/generator remain `/tmp`; no bodies/assets committed.

Remaining: actual voice/note/fraction/sample/ADSR/periodic/controller/index/job
providers and valid value/extent/alias/lifetime/IRQ/concurrency/stack/ABI domains,
transitive hardware/DSP services and original pitch/voice/audio traces. EVT-01,
full native binding and full-game completion remain OPEN.
