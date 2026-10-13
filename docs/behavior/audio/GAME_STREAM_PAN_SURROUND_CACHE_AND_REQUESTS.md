# Game stream pan and surround cache and immediate requests

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSSetPanCh800dddb4/160hex
(88 instructions) and SoundSSSetSrndPanCh800dda54/184hex (97).
Addresses/offsets/masks hexadecimal; arithmetic/counts decimal. Static source
deductions, not observed original panning/audio. Read
[game stream flags](GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md),
[frequency wrapper](GAME_STREAM_FREQUENCY_CACHE_AND_REQUESTS.md) and
[MusyX mix child](MUSYX_STREAM_MIX_AND_LOCAL_MODULE_COVERAGE.md).

## Pan write and fresh immediate gate

Inputs raw channel I and raw pan R. Capture sound state S803e0138, load its
pool pointer word100/803e0238 and form V=base+wrap32(I*138). Read U16 flags0;
exactly0 returns without cache write or request. No local channel bound, null,
resource, generation, IRQ or address-domain check precedes this access.

Nonzero flags: P=low8(R); store P as a HALFWORD at V+6, THEN reload flags0.
Mask8 clear returns cache-only. This is byte narrowing into a halfword, not
low16(R). No range/equality clamp, so raw256 caches0 and ffffffff caches255.
The row stays captured throughout this setter.

Fresh half1a exactly1 selects one request. Load float10, signed fctiwz and
spill/read its low word as volume; read half8, narrow surround to low8 and
load primary word4c. Request sndStreamMixParameter8027b814 at800dde30 with
(primaryID, volumeWord, RAW ORIGINAL R, low8Surround, AuxA0). The pan argument
is not the narrowed cached P. Raw256 therefore caches0 but requests pan256.
No sentinel, child-status or additional stream-state gate exists here.

## Surround write and pool recapture

Inputs raw channel I and surround T. Capture OLD row V0 using S.word100 and
wrap32(I*138). Old flags0 exactly0 returns. Otherwise store low8(T) as half8
in V0, reload OLD flags0 and require mask8 for further processing.

Then RELOAD S.word100, read OLD row half6, narrow it to P=low8(oldPan), and
form NEW row V from the reloaded pool plus the same retained channel offset.
New flags0 exactly0 returns, leaving only the old surround write. Otherwise
store P as NEW half6, reload NEW flags0 and require mask8. A cleared fresh
mask returns after both cache writes. No explicit SoundSSSetPanCh call occurs;
its pan-routing logic is inlined with this additional recapture/gate sequence.

Fresh NEW half1a exactly1 requests at800ddaf8:
(newPrimaryID, converted NEW float10, P, low8 NEW half8, AuxA0).
The surround argument comes from NEW half8, not a retained original T or old
surround write. With a changed pool these writes/reads address different rows.
This is source ordering, not evidence that a real producer changes the pool
between these instructions. Actual lifetime/concurrency/alias domains remain open.

## Every non-one type uses two streams

Both setters route EVERY half1a value other than1 through two requests,
including0,2 and65535. Capture V.float10 as W and retain a copy in FPR31.
P is already low8, in0..255. The literal divisor at8042190c has bits427c0000,
exactly63.0; unsigned/signed double-conversion biases live at802e3cc0/c8.

Let single(x) denote single-precision rounding under the execution FP domain:

- P<64: primary gain=W; secondary gain=single(single(P*W)/63).
- P>=64: primary gain=single(single((127-P)*W)/63); secondary gain=W.

The sign test of P-64 selects primary scaling. There is no clamp at127:
P128 makes the primary gain negative, and P255 uses multiplier-128. P63 and
P64 both give full W to both streams under finite representable inputs. This
source formula does not prove physical loudness, normalized balance or original
FP exception/rounding behavior.

Signed fctiwz converts primary gain; read fresh half8/low8 and primary word4c;
request (primaryID, primaryWord, Pan0, surround, AuxA0) at800dded0 for pan or
800ddb94 for surround. AFTER that child convert the retained FPR31 secondary
gain, reread half8/low8 and secondary wordd4; request
(secondaryID, secondaryWord, Pan127, secondFreshSurround, AuxA0) at800ddef4
or800ddbb8. Gains were computed before the first child; secondary ID/surround
are fetched afterward. No volume/type/flag/pool recheck or success dependency
between requests. Child mutation/ABI/FP-state behavior requires separate evidence.

## Return, evidence and remaining work

Neither setter requests the game shared interrupt counter or OS IRQ services;
the MusyX child owns its independent nested IRQ/mix/companion operations.
No independent success/failure value is assigned. Epilogues restore paired and
single FPR31, GPR31, LR and64 decimal stack bytes. Valid stack/ABI/provider and
FP domains are required; NaN/infinity/out-of-range fctiwz results are unresolved
here rather than filled with a guessed host integer.

[Manifest](../../../tools/game_stream_pan_surround_evidence.json) pins five
spans, all185 body-word anchors and six direct calls. Seven finite split,
four type and five cache vectors plus pool-binding annotations were checked.
The finite vectors assume ordinary nearest single rounding and in-range signed
conversion; these mathematical deductions are separate from original execution.
Generic verifier checks pins/targets, not prose/providers/runtime. Fresh full
ELFs/listings/generator remain `/tmp`; no original bodies/binary assets committed.

Remaining: game stream engine/play/volume/callbacks; actual channel/base/flags/
type/volume/pan/surround/ID providers and valid value/extent/alias/lifetime/IRQ/
concurrency/FP domains; transitive ARAM/AI/DSP and original mono/dual/pan/surround/
audio traces. EVT-01/native/full-game binding remain OPEN.
