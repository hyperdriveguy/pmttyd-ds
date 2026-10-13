# MusyX low-precision portamento transitions and interpolation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete LowPrecisionHandler region
80277bb0..80277d7c, end exclusive. Offsets/addresses/masks hexadecimal;
thresholds/shifts decimal. Static source deductions, not original glide/audio
execution or measured duration. Read [pitch preparation](MUSYX_LOW_PRECISION_PITCH_PREPARATION.md),
[macro/initializer](MUSYX_MACRO_VIBRATO_PORTAMENTO_AND_INITIALIZATION.md) and
[controller lookup](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md).

## Controller request and unchanged-value exception

Retain voice V, prepared target pitch P=r29 and low elapsed E=r31. Read U8
byte121=channel. Exactlyff skips the controller transition section; existing
portamento interpolation below can still run. Otherwise read U8 byte122=set,
request inpGetMidiCtrl(65,channel,set)8028ed48 at80277bc4. No voice pointer is an
argument to that request, and there is no return-status check.

Read cached U16 half134 AFTER child. Use low16 controller return R. Different
from cache admits transition processing immediately. Equal: freshly read
flags114 then118; process only when (flags118&21000)==20000. Every other masked
combination skips directly to interpolation, without rewriting cache/flags.
The flags114 term is masked0. This is not universal unchanged-value suppression.

## Disable, initialization and exact publications

Admitted R<=8064: freshly load flags118 then114, store118 with400 cleared,
then114 unchanged. No current/duration/elapsed reset. Proceed to final1000/cache
publication below. Strictly R>8064 enters enable processing:

1. Freshly read flags114 then118. Already-set118&400 skips initialization.
2. Otherwise118&20000 set also skips initialization. Regardless, enable flag
   publication still happens; a skipped initializer does not prevent enabling.
3. If both bits clear, read byte133. Exactly1 and captured118&1000 clear stores
   word140=0. Otherwise freshly read word138 and copy it to word140.
4. Read U8 byte132, store byte<<16 to word13c. This uses raw byte132, not the
   currently prepared target P or current hardware pitch.
5. Freshly read flags118, store OR400.

Both admitted disable/enable paths then freshly read flags118 and store OR1000,
then store low16 original controller return at half134. These stores happen even
when no effective flag change occurred. No initializer child call, child query
repeat, mode normalization, hardware pitch request or low flags114 mutation.
Post-child flags/providers may differ from pre-request state; prepared P remains
retained. byte133 values other than1 use the duration-copy branch.

## Wrapped interpolation and unsigned endpoint selection

Freshly read flags114 then118; set selected pitch r30=P. Only flags118&400 gates
interpolation. Clear skips directly80277d7c. Set: read elapsed word140 then
duration word138. D=wrap32(duration-elapsed) logical>>8. D0 skips interpolation
without current/elapsed writes; no duration clamp or completion flag clear.
D lies1..00ffffff on the admitted path, providing a nonzero positive divisor.

Read old current C=word13c. Compute signed delta=arithmeticSigned32(wrap32(P-C))
>>8. Numerator=arithmeticSigned32(wrap32(E*delta)). Signed divw by D gives Q,
truncated toward0. Store candidate N=wrap32(C+Q) at word13c BEFORE endpoint
checks. Do not replace this with unbounded interpolation, floating math or
multiply-before-shift reassociation.

Use UNSIGNED comparisons of captured C against P and fresh current N against P:

| Predicate | Selection and elapsed write |
| --- | --- |
| C<P AND N<P, or C>P AND N>P | Read fresh word140; select current word13c as r30; store wrap32(oldElapsed+E) at140 |
| Every other case, including equality | Keep r30=P; freshly read duration138 and copy it to elapsed140 |

Completion does not overwrite candidate word13c with P, does not clear flag400
and does not zero duration/current/elapsed. It only selects P for following pitch
work and snaps elapsed to duration. A later call can observe the retained
candidate. E addition occurs only on continuing paths; D0 does not advance time.
Unsigned endpoint direction and signed wrapped step are separate source rules.

## Evidence and remaining work

[Manifest](../../../tools/musyx_low_precision_portamento_evidence.json) pins
three spans (region/whole handler/controller child),115 whole-region instruction
anchors and one call. Twelve controller-transition/eight interpolation vectors
checked, including unchanged-value exception, ff skip, threshold, nonboolean mode,
zero divisor, overshoot and wrapped duration/numerator. These are mathematical
source vectors, not original portamento traces. Generic verifier checks hashes/
anchors/target, not semantic deductions. Fresh full handler ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed.

Remaining: final ADSR/pitch/scaling/hardware/requeue/ABI regions, actual channel/
set/controller/cache/mode/flags/duration/current/time/note/voice providers and
valid index/extent/value/alias/lifetime/IRQ/concurrency domains, original glide/
pitch/audio traces and native binding. EVT-01 remains OPEN.

## 2026-10-13 — Final pitch/composition follow-up

[Final pitch/composition](MUSYX_LOW_PRECISION_FINAL_PITCH_AND_COMPOSITION.md) closes final output/requeue/ABI
and composes the whole local handler. Providers/domains/transitive consumers/
original pitch/voice/audio/native binding remain open.
