# Game stream main ordered fades and inline stops

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSMain800dd2e0/638hex,
398 instructions. Addresses/offsets/masks hexadecimal; arithmetic/counts decimal.
Static deductions, not original fade/cadence/audio observations. Read
[outer caller](GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md),
[fade-step/stop producers](GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md),
[continue producer](GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md) and
[pan routing](GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md).

## Three channels; initial gates and immediate stop priority

Retain S803e0138, scan channel0,1,2 with offsets0,138,270hex. RELOAD pool pointer
S.word100/803e0238 for each iteration and capture OLD row V. Initial flags0
must have mask8 set and mask2 clear; otherwise skip the entire row. No local
pool/index/null/generation/extent/lifetime check or cadence/tick conversion.

Initial mask4 set: clear mask4 in OLD flags using that captured value, then
recapture pool+same channel offset as NEW row U for inline stop. New flags0
exactly0 skips to next channel without counter/children/marker update. Otherwise
perform the stop sequence below and skip all fade work. This is inlined code;
there is no call to SoundSSStopCh in this body.

Initial mask4 clear: process mask40, then fresh mask20, then fresh mask10.
These are sequential checks, not mutually exclusive modes. OLD V remains the
fade arithmetic/flags owner even when children change the pool pointer.

## Mask40: fade to muted, checking zero before subtraction

Using initial captured flags, mask40 set loads literal positive0 at80421908
(bits00000000) and OLD float10 volume. Unordered compare equality with0:
exactly zero (including signed zero under normal FP execution) ORs2 into the
initial flags, then rereads OLD flags and clears40. Skip ALL remaining row work;
no volume/step write or mix/stop request. No resource release occurs here.

Nonzero: load OLD float14 step; single fsubs volume-step, store float10, reread
it, ordered compare<=0 and clamp to positive0 when true. Unordered/exceptional
FP domains are not filled with guessed results. Then enter inline mix block40.
After it finishes/skips, proceed to fresh mask20 check, with no recheck of the
initial mask2/8 gates. Reaching0 by subtraction does not itself set mute or
clear40 this invocation.

## Mask20: fade to stop, also checking zero before subtraction

Read OLD flags FRESH after mask40 block. Mask20 clear proceeds to mask10.
Set: read OLD float10, compare equality with literal0. Exactly0 clears20 in
that fresh OLD flag value, recaptures pool+same offset for inline stop, then
skips the rest of the row regardless of stop admission. New flags0 exactly0
skips counter/children/marker. Otherwise use the stop sequence below.

Nonzero: load step14, single subtract/store/reread float10, ordered<=0 clamp
to positive0. Enter inline mix block20; then proceed to fresh mask10. The bit20
is not cleared on merely reaching0. With only20 set, stopping normally happens
on the NEXT invocation; with40 also set,40 can reach0 before this precheck and
cause an inline stop during the same invocation. Source ordering is not a
measured fade duration or recommended flag configuration.

## Mask10: rise toward cached target

Read OLD flags FRESH; mask10 clear skips to next channel. Set: load OLD float10
and step14; single fadds/store/reread float10, then read floatc target.
Ordered newVolume>=target clamps float10 to that fresh target, stores positive0
at step14, rereads flags and clears10. Otherwise retain step/bit10. No range,
step sign, equality, overflow or monotonicity guarantee is enforced.
Then enter inline mix block10 even if clamping cleared10. No fade arithmetic
or stop block follows this mix before the next channel iteration.

Stable example: flags68hex, volume5/step1 subtract twice, ending3 with two mix
blocks. Flags78hex also adds once afterward, ending4 with three mix blocks.
Flags28hex, volume1/step2 clamps0 but retains20; its next admitted invocation
can stop. These mathematical invocations do not establish physical frame/tick
frequency. Original caller/pause/credit/IRQ scheduling remains separate.

## Three inline mix blocks share recapture and child ordering

Each block independently RELOADS pool pointer, reads OLD half6/low8 P, forms
NEW U=pool+same channel offset. New flags0 exactly0 skips the block; otherwise
store P as NEW half6, reread NEW flags and require mask8. No mask2 check here.
The OLD arithmetic write does not copy float10 into a different NEW row.

Fresh NEW type half1a exactly1 requests sndStreamMixParameter8027b814 with
(newPrimaryID, signed fctiwz NEW float10 low word, P, low8 NEW half8, AuxA0).
Every other type follows the linked pan contract: capture NEW float10 as W;
P<64 retains primary W and scales secondary single(single(P*W)/63), otherwise
retains secondary W and scales primary single(single((127-P)*W)/63).
Literal63 is8042190c bits427c0000; double biases802e3cc0/c8. No127 clamp.

Dual primary request uses Pan0, secondary Pan127, both AuxA0. IDs/surround
are fresh reads before their respective calls; gains calculated before primary
remain retained across it. No status/type/flags/volume/pool recheck between
requests, sentinel guard or game counter request in mix blocks.

| Block | Mono call | Dual primary | Dual secondary |
| --- | --- | --- | --- |
| 40 | 800dd4a0 | 800dd53c | 800dd560 |
| 20 | 800dd6c0 | 800dd75c | 800dd780 |
| 10 | 800dd824 | 800dd8c0 | 800dd8e4 |

The linked child owns nested IRQ/mix/companion behavior. FP invalid-conversion,
ABI/alias/lifetime/concurrency domains remain separate. After a skipped or
completed block40/20, subsequent mode checks still read OLD V, not NEW U.

## Two inline stops: row clear precedes close/free

For admitted recaptured U, read U16 S.counter20c/803e0344. Initial0 requests
OSDisableInterrupts80299ad4 and stores returned state at S.word210/803e0348;
otherwise skip. Fresh counter increments with low16 store. No overflow guard.
Fresh U.flags mask400 set reads U.half2 and clears halfword
S[226+half2*2]/803e035e slot table without an index bound.

Store U.flags0=0 BEFORE requesting DVDMgrClose80144dd4(U.word28), then
close fresh wordb0, then sndStreamFree8027c3e0(fresh word4c). AFTER primary
free read fresh U.type half1a; exactly2 frees fresh wordd4. No ID/null/status
checks or child success dependency. Deferred close only publishes its close
bit; no physical file release is claimed here.

Read counter FRESH. Zero skips decrement/restore; nonzero decrements low16,
resulting0 requests OSRestoreInterrupts80299afc with fresh saved state.
Then store ffffffff at S.word104/803e023c marker, regardless of child results.
Initial65535 stable wrap0 therefore performs no local disable/restore but can
still issue children and marker write. IRQ invariant/providers remain open.

| Stop trigger | Disable | Close primary/secondary | Free primary/secondary | Restore |
| --- | --- | --- | --- | --- |
| 4 | 800dd358 | 800dd3a0 / 800dd3a8 | 800dd3b0 / 800dd3c4 | 800dd3e8 |
| 20 prezero | 800dd5a8 | 800dd5f0 / 800dd5f8 | 800dd600 / 800dd614 | 800dd638 |

No independent success assignment. Epilogue restores paired/single FPR31,
GPR26..31/LR and80 decimal stack bytes. No initial playback, callback invocation
or cursor/refill processing occurs in this body; those services are separate.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_main_fade_stop_evidence.json) pins eight
spans, all398 body-word anchors and21 direct calls. Thirteen finite stable-frame,
six recaptured stop-gate/five stable counter vectors plus three mix/two stop call
groups checked; linked pan/stop evidence reverified. Stable models assume same
pool/no child mutation, ordinary single rounding and admitted stop clears flags;
they do not replace fresh source reads or original execution. Generic verifier
checks pins/targets, not semantic/provider/runtime claims. Fresh full ELF/listing/
generator stay `/tmp`; no bodies/assets committed. Local whole main answered.

Remaining: game stream play/deferred DVD manager and actual flags/pool/fade/
volume/step/target/pan/type/ID/slot/counter/saved-state/marker providers and valid
value/extent/alias/lifetime/IRQ/concurrency/ABI/FP domains, transitive SDK/ARAM/
AI/DSP and original pause/cadence/fade/stop/mix/audio traces. EVT-01/native/full-
game binding remain OPEN.
