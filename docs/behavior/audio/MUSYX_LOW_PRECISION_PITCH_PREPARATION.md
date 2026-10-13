# MusyX low-precision note, bend and modulation pitch preparation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete LowPrecisionHandler REGION
80277a30..80277bb0, end exclusive. Offsets/addresses/masks hexadecimal;
shifts and divisors decimal. Static source deductions, not measured tuning/audio.
Read [clock/ADSR contribution](MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md),
[oscillator state](MUSYX_LOW_PRECISION_MODULATION_AND_PAN_RAMPS.md),
[controller wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md) and
[note producers](MUSYX_MACRO_SUBROUTINES_AND_NOTE_COMMANDS.md).

## Retained note and signed cents

Retain voice V in r28. Read U8 byte130 then U16 half12e. Sign-extend byte130
as C; compute signed X=C<<16. The original signed mulhw by51eb851f, arithmetic
shift5 and sign correction gives truncation toward0 of X/100 for every signed
byte C. Set retained r29=wrap32((half12e<<16)+trunc(X/100)). No note127 clamp,
cent normalization or physical-frequency conversion. The note/cents captures
precede controller children; later mutations do not recalculate this base.

The intervening flags114/118 reads determine bend admission; they do not change
the arithmetic. This region does not store the prepared base pitch to a field.

## Cached/query/skip bend selection

Only flags118&00010030 nonzero enables a fresh bend query; flags114 contributes
a literal-zero masked term. Query enabled: read byte121. Exactlyff skips ALL
bend work, including cached value. Otherwise request inpGetPitchBend(V)8028f848
at80277a9c, store low16 return at half1dc and consume low16 return.

Query mask clear: read cached U16 half1dc, regardless of byte121. No controller
query/cache write. Both consuming paths use B=unsigned16 bend. Exactly8192
skips contribution. Otherwise delta=B-8192: negative reads U8 byte1da, positive
reads U8 byte1db. Compute product low32 of selectedRange*delta, shift left3 with
wrap32, add to retained r29 with wrap32. Range bytes are read AFTER query when
it occurs. No signed16 interpretation of B, bend16383 cap, per-range validation,
flag clear or child-return status gate.

## Modulation factor and fresh scale flag

After bend processing, freshly read flags114 then118. Only flags118&2000 admits
modulation; low114 contributes a literal-zero term. Clear skips directly80277bb0.
Set requests inpGetModulation(V)8028f8d8 at80277b10, retaining its return R.

After child read U8 byte145, SIGNED half154, then U8 byte144. Start factor
G=wrap32((byte144<<8)+trunc((byte145<<8)/100)). byte145 is UNSIGNED here, unlike
base cents. The same mulhw/shift/sign-correction sequence gives its exact quotient.
If signed half154 H!=0, Q=low16(R) logical>>7; compute low32 H*Q, arithmetic
signed result>>7, then add to G with wrap32. H0 skips this term. No factor clamp.

Then read flags114/118 AGAIN. Fresh flags118&4000 set selects a scaled oscillator:
read full word150; multiply by Q with low32 product, interpret signed and shift
right7. Clear selects raw word150. This flag can differ from the pre-child gate;
no reread of modulation result or factor bytes. The two masks are independent.

Multiply G by selected oscillator value with low32 product; arithmetic signed
result>>4; add to retained r29 with wrap32. No unbounded multiplication, host
floating math, symmetric rounding or saturating addition. Signed right shifts
round negative intermediates downward; the earlier /100 truncates toward0.
No dirty/flag publication, hardware pitch request or requeue in this region.

## Evidence and remaining work

[Manifest](../../../tools/musyx_low_precision_pitch_preparation_evidence.json)
pins four spans (region/whole handler/two controller children),96 whole-region
instruction anchors and two calls. Seven base/six bend-gate/five bend/six modulation
vectors checked; all256 signed base-cent bytes and256 unsigned modulation-cent
bytes checked against the decoded division sequence. Generic verifier checks
source hashes/anchors/calls, not semantic deductions or runtime domains. Fresh
full handler ELF/listing/generator remain `/tmp`; no bodies/binary assets committed.

Remaining: portamento/controller transition and final pitch/ADSR/scaling/hardware/
requeue/ABI regions, actual note/cents/cache/range/factor/depth/oscillator/flag/
controller/voice providers and valid value/index/extent/alias/lifetime/IRQ/
concurrency domains, original pitch/audio traces and native binding. EVT-01 OPEN.

## 2026-10-13 — Portamento follow-up

[Portamento](MUSYX_LOW_PRECISION_PORTAMENTO_TRANSITIONS_AND_INTERPOLATION.md) closes the next controller-transition/interpolation
region80277bb0..80277d7c. Final pitch/requeue/providers/domains/original
glide/audio/native binding remain open.
