# Game stream fade, stop and deferred DVD close

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSFadeoutCh800ddf54/a4,
SoundSSStopCh800ddff8/10c, SoundSetFadeTime800e22c0/14 and
DVDMgrClose80144dd4/10hex. Offsets/addresses/masks hexadecimal; arithmetic
constants decimal. Static source deductions, not original fade/close/audio
execution. Read [game stream leaves](GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md),
[MusyX free chain](MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md) and
[outer sound frame](GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md).

## Raw channel row and fade-time publication

Both channel services capture V=pointer at sound state S803e0138.word100 plus
wrap32(raw channel*138). Read U16 flags0; exactly0 skips all service work. No
channel-count/index/pointer/active-system/generation/IRQ check for fade.

SoundSetFadeTime(A,B) unconditionally stores low16 A to S.half208, then low16 B
to S.half20a. No unit/range clamp, timing query, pointer/user callback or status.
Only half20a is consumed by fade-out below; half208's other consumers remain open.

## Fade step: signed GP word and ordered minimum

Admitted SoundSSFadeoutCh reads GP pointer at80415fd0, then U16 S.half20a=D,
then GP.word4=G. G is SIGNED32: xoris conversion with original signed double
bias802e3cc8 (4330000080000000), single fsubs. Convert D unsigned through
bias802e3cc0 (4330000000000000), also single fsubs. Separate single operations:

```
interval = single(1000 / single(signed32(G)))
count = single(single(D) / interval)
denominator = 1 if ordered(count < 1) else count
step = single(fresh V.float10 / denominator)
```

Original1000/1 literals at80421910/14 are447a0000/3f800000. No GP-null/G0/
finite/type/range check; ordered minimum does not repair unordered values.
No reassociation to a single D*G/1000 expression. Read float10 AFTER denominator
selection, store step at float14, then freshly read flags0 and store OR20.
Current float+c, flags2 and other fields are not locally reset. There is no
volume decrement, stop/DSP/stream request, callback or child call here.

These are raw numeric relationships, not measured physical frame duration or
proof of the GP word's provider contract. FP rounding/trap/exception and actual
flag/step consumption remain open; finite vectors are mathematical examples.

## Stop's shared interrupt counter

Admitted SoundSSStopCh retains V and S. Read U16 S.half20c. Initial0 requests
OSDisableInterrupts80299ad4, stores returned state at S.word210. Nonzero skips
that request. Then RELOAD half20c, increment with low16 store, without overflow
cap. A counter65535 can wrap0; the caller does not repair this domain.

Read V.flags again. Mask400 set: read U16 V.half2, clear halfword at
S+226+2*thatIndex. No table-bound/index masking. This fresh flags gate can differ
from the initial admission. Then store V.flags0=0 BEFORE all release children.

## Ordered deferred close and stream-free requests

After flag clear:

1. Read V.word28; request DVDMgrClose(pointer).
2. Read V.wordb0 AFTER first child; request DVDMgrClose(pointer).
3. Read V.word4c AFTER both close children; request sndStreamFree(raw ID).
4. Read V.half1a AFTER primary free. Exactly2 reads fresh wordd4 and requests
   sndStreamFree(second raw ID); other values skip. No ID sentinel/result gate.

DVDMgrClose's complete leaf reads U16 pointer.half90, stores OR8 there, returns.
No null check, DVD SDK request, immediate close/free/list removal or completion
wait. Two pointers may alias; both source requests still happen. The owner which
consumes bit8 and actual handle residency/lifetime remain open.

All child returns ignored. The caller does not clear stored handle/ID pointers,
voice/source/format/volume fields or roll back earlier flags/table writes. MusyX
free owns its separate deactivation/buffer/companion/IRQ operations. This game
caller does not equate deferred DVD flags with completed IO or physical release.

## Fresh decrement/restore and global invalidation

After children read S.half20c FRESH. Zero skips decrement and restore. Nonzero:
store low16(counter-1); if that low16 result0, read saved S.word210 FRESH and
request OSRestoreInterrupts80299afc. Otherwise no restore. No assumed stable
counter or automatic pairing of arbitrary nested mutations. Initial65535,
unchanged wrap-to0 and final0 thus issues neither local disable nor restore.

Then unconditionally storeffffffff to S.word104 for every initially admitted
stop, even when child/type branches skipped some releases. Initial flags0 skips
this invalidation too. No matching-channel comparison or independent success
return. Actual shared-counter/marker/OS/provider domains require their own
source/runtime ownership; the row pointer stays captured throughout.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_fade_stop_close_evidence.json) pins nine
spans (four bodies/three linked children/two constant spans),all117 instruction
anchors,six calls. Seven finite fade/six counter/five stop/three time-set/four
close vectors checked, including signed/zero duration, counter wrap/mutation,
post-child optional second free and deferred-only close. Generic verifier checks
hashes/anchors/calls, not semantic/domain assumptions or original execution.
Fresh full ELFs/listings/generator stay `/tmp`; no bodies/assets committed.

Remaining: fade/stop consumers, GP/time/counter/marker/base/flag/table/handle/
stream-ID/type providers, deferred DVD close owner, valid value/extent/alias/
lifetime/IRQ/concurrency/FP domains, decoder/DVD/DMA/ARAM/DSP and original fade/
stop/IO/audio traces. Four local bodies answered; EVT-01/native/full game OPEN.

## 2026-10-13 — Game frequency follow-up

[Game frequency](GAME_STREAM_FREQUENCY_CACHE_AND_REQUESTS.md) closes the frequency wrapper using the same
shared counter, with fresh conditional dual-ID requests. Engine/providers/IRQ
domains/original frequency/audio/native binding remain open.
