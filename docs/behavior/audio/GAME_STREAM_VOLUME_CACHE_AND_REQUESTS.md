# Game stream volume cache and immediate requests

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSSetVolCh800ddc1c/198hex,
102 instructions. Addresses/offsets/masks hexadecimal; arithmetic/counts decimal.
Static source deductions, not original volume/audio observations. Read
[volume query/flags](GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md),
[pan/surround setters](GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md) and
[MusyX mix child](MUSYX_STREAM_MIX_AND_LOCAL_MODULE_COVERAGE.md).

## Initial row and ordered volume writes

Inputs raw channel I and raw volume R. Capture S803e0138, load its pool pointer
word100/803e0238 and form OLD row V0=base+wrap32(I*138). No local channel bound,
null, resource, generation, IRQ or address-domain check precedes access.
Read U16 flags0; exactly0 returns without a cache write or request.

Otherwise narrow R to low8, convert that unsigned byte to float using the
unsigned double-conversion bias802e3cc0 and fsubs, and store float10 FIRST,
then floatc, both in OLD row. All0..255 are exactly representable under ordinary
FP execution. No clamp to127:128..255 remain possible; raw256 becomes0,
ffffffff becomes255. No equality gate, fade-step write or flag mutation here.
Floatc is the value read by SoundSSGetVolCh; float10 is read by immediate mix
routing. This setter writes both even when no immediate request follows.

Reload OLD flags0 AFTER both stores. Mask8 clear returns cache-only. The gate
is a fresh read, independent of initial nonzero admission. No game interrupt
counter/OS IRQ service is requested before or after these writes.

## Reloaded pool and inline pan gates

After old mask8 admission, RELOAD S.word100, read OLD row half6, retain its
low8 P, and form NEW row V from the reloaded pointer plus retained channel
offset. New flags0 exactly0 returns after the old volume writes. Otherwise
store P as NEW half6; reread NEW flags0 and require mask8. Clear returns after
old volume writes and new pan write. There is no explicit SoundSSSetPanCh call:
its pan logic is inlined with this additional recapture/gate sequence.

Subsequent type/float10/surround/ID reads address NEW row. If the pool changes,
this is not necessarily the row whose volume was just written. No retained
input volume is substituted for NEW float10, and no volume is copied between
rows. Source ordering alone does not prove a real producer changes the pointer
between these instructions; actual lifetime/concurrency/alias domains remain open.

## One request for exactly type1; two for every other type

Read fresh NEW half1a. Exactly1: load NEW float10, signed fctiwz and spill/read
its low word as volume; read NEW half8/low8 and primary word4c. Request
sndStreamMixParameter8027b814 at800ddce0 with
(primaryID, volumeWord, P, low8Surround, AuxA0). P is low8 OLD cached half6;
there is no raw pan argument in this setter. No ID sentinel/status/voice-state
gate exists locally beyond earlier flag reads.

EVERY other type, including0,2 and65535, routes two streams. Capture NEW
float10 as W and retain a copy in FPR31. For low8 P, the complete calculation
and conversion ordering matches the linked pan contract:

- P<64: primary gain=W; secondary gain=single(single(P*W)/63).
- P>=64: primary gain=single(single((127-P)*W)/63); secondary gain=W.

Here single means single-precision rounding under the execution FP domain.
The literal8042190c is bits427c0000, exactly63.0, not64. Signed-conversion
bias802e3cc8 serves the127-P branch. No127 clamp: P128 yields negative primary
gain, P255 uses multiplier-128. P63/P64 both pass full finite W to each stream.
These formulas do not establish physical loudness or normalization.

Convert primary gain with signed fctiwz; read fresh half8/low8 and primary
word4c; request (primaryID, primaryWord, Pan0, surround, AuxA0) at800ddd70.
AFTER primary child convert retained FPR31 secondary gain; reread half8/low8
and secondary wordd4; request
(secondaryID, secondaryWord, Pan127, secondFreshSurround, AuxA0) at800ddd94.
Gains were calculated before the first child; the second ID/surround are fetched
afterward. No type/flags/volume/pool recheck or child-success condition separates
requests. Child mutation/ABI/FP-state preservation requires separate evidence.

## Evidence, return and remaining work

No independent success/failure value is assigned. Epilogue restores paired and
single FPR31, GPR31, LR and64 decimal stack bytes. The MusyX child owns its
independent nested IRQ/mix/companion behavior. Valid stack/ABI/provider/FP domains
are required; NaN/infinity/out-of-range fctiwz results remain unresolved rather
than assigned guessed host integers.

[Manifest](../../../tools/game_stream_volume_cache_evidence.json) pins four
spans, all102 body-word anchors and three direct calls. Six byte/float cache,
eight fresh gate, seven finite split and four type vectors plus pool-binding
annotations checked. Finite vectors assume ordinary nearest single rounding
and in-range signed conversion. These are mathematical source deductions,
separate from original execution. Generic verifier checks pins/targets, not
semantic/provider/runtime claims. Fresh full ELF/listing/generator stay `/tmp`;
no original bodies/binary assets committed. Local whole setter answered.

Remaining: game stream engine/play/callbacks; actual channel/base/flags/type/
volume/pan/surround/ID producers and valid value/extent/alias/lifetime/IRQ/
concurrency/FP domains; transitive ARAM/AI/DSP and original volume/mono/dual/audio
traces. EVT-01/native/full-game binding remain OPEN.
