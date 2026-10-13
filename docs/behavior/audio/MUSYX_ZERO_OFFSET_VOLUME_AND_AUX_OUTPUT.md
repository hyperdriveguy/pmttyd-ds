# MusyX zero-offset main volume and auxiliary-output request

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete ZeroOffsetHandler region
80278328..802784ac, end exclusive. Addresses/offsets/masks hexadecimal.
Follows [pan/surround](MUSYX_ZERO_OFFSET_PAN_AND_SURROUND.md) and
[gain/attenuation](MUSYX_ZERO_OFFSET_GAIN_AND_ATTENUATION.md).
Static source deductions, not original execution or measured mixing.
Read [controller wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md)
and [hardware gain consumer](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md).

## Admission and six-request order

Retain V, raw voice index I, base/attenuated gain B=f30 and master/track gain
F=f31 from prior regions. Nonzero carried indicator r0 admits this block without
reading word220. Otherwise freshly load V.word220: mask0f01 nonzero admits;
zero skips directly802784ac. This block does not clear dirty word220 or source
flags. Its mask differs from the previous pan admission mask6.

Admitted requests occur in this order, with no child-return status gates:

| Site | Request |
| --- | --- |
| 80278340 | inpGetVolume(V)8028f770 |
| 80278374 | inpGetReverb(V)8028f9b0 |
| 802783c4 | inpGetPreAuxA(V)8028f968 |
| 80278418 | inpGetPostAuxB(V)8028fa40 |
| 80278448 | inpGetPreAuxB(V)8028f9f8 |
| 802784a8 | hwSetVolume(I,U8(V.byte196),main,auxA,auxB,V.word164,V.word168)80290f50 |

All five controller results use low16 only, converted with the original unsigned
double-bias/fsubs sequence. No controller/value/index/studio/extent/generation/
voice-lifetime validation here. V/I and B/F remain retained across children;
no reread of the global voice base or earlier gain fields. Each child can mutate
fields that subsequent source reads access.

## Separate single operations and fresh byte reads

Let S denote one single-precision instruction result. C=the exact float at
8042913c (bits38800200), D=80429120 (3c010204). Vc/R/PA/PB/QB below are the
unsigned low16 Volume/Reverb/PreAuxA/PostAuxB/PreAuxB results, respectively.

```
main = S(C * S(S(B * F) * Vc))
reverbTerm = S(D * S(U8(byte194) * S(C * S(main * R))))
preATerm = S(C * S(B * PA))
extraATerm = S(D * U8(byte195))
auxA = S(extraATerm + S(preATerm + reverbTerm))
postBTerm = S(C * S(main * PB))
preBTerm = S(C * S(B * QB))
auxB = S(preBTerm + postBTerm)
```

Read byte194 AFTER Reverb returns and before PreAuxA request. Read byte195
AFTER PreAuxA returns and before PostAuxB request. These are distinct captured
bytes; later mutations do not recapture them. The reverb term remains in f29
until AuxA composition, then f29 is reused for postBTerm. Main replaces f31;
B remains f30. Source interleaves conversions with arithmetic; the expressions
above preserve dependent rounding, not a license to fuse/reassociate operations.

The pre-aux terms use B rather than main; AuxA's extra byte195 term is added
without multiplication by B/main/controller. For finite mathematical inputs
B=F=0, all five controllers0 and byte195=127, main0 but auxA1 follows the exact
single literal D. This is a source vector, not proof that original providers
allow those inputs or that audible output has that value. No local clamping,
zero-main suppression, route normalization or gain-range check appears.

## Integer snapshot before hardware publication

After PreAuxB returns, compute single S(32767*main), using literal80429140
bits46fffe00. Apply fctiwz, spill its double-format result to stack, read its low
word and store low16 at V.half454. This is conversion/truncation followed by a
halfword store, not a clamp to signed16. Out-of-range/NaN/FPSCR/exception behavior
requires original processor-domain evidence; finite vector checks only cover
convertible signed32 results. No host cast is asserted for exceptional cases.

Then finish the remaining preBTerm arithmetic, read U8 byte196, then word164,
then word168 fresh, and call hwSetVolume with floating arguments f1=main,
f2=auxA,f3=auxB and integer arguments r3=I,r4=studio,r5=pan,r6=surround.
Half454 publication precedes those final field reads and the hardware request.
Ignore child return; no failure rollback or voice/gain recapture. Skipped block
performs no controller requests, half454 store or hardware-volume call.

## Evidence and remaining work

[Manifest](../../../tools/musyx_zero_offset_volume_aux_output_evidence.json)
pins11 spans (region/whole handler/six children/three constant spans),97 complete
region instruction anchors and six direct calls. Eight admission/seven finite
output vectors checked, including bypass, zero main/nonzero AuxA, unsigned
low16/high gains and negative converted snapshot. Float vectors use a finite
host single-rounding model; no original Gekko execution/audio claim. Generic
verifier checks source hashes/anchors/calls, not semantic deductions. Fresh full
handler ELF/listing/generator remain `/tmp`; no bodies/binary assets committed.

Remaining: filter/requeue/epilogue, actual gain/controller/byte/studio/dirty/
voice providers and valid value/extent/alias/lifetime/IRQ/concurrency/FP/conversion
domains, transitive hardware consumers and original output/audio traces. Existing
hardware contract closes its local body, not full runtime binding. EVT-01 OPEN.
