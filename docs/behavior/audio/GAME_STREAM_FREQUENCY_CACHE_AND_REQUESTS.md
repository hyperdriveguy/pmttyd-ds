# Game stream frequency cache and immediate requests

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSSetPlayFreqCh800dd980/d4hex,
53 instructions. Offsets/addresses/masks hexadecimal; arithmetic/counts decimal.
Static source deductions, not original frequency/audio execution. Read
[game stream flags](GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md),
[shared counter/stop](GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md) and
[MusyX frequency child](MUSYX_STREAM_FREQUENCY_AND_COMPANION_CHAIN.md).

## Captured row, halfword write and fresh immediate gate

Inputs raw channel I and frequency F. Capture sound state S803e0138 and its
pointer word100, form V=base+wrap32(I*138). No channel/count/index/pointer/
generation/resource/IRQ check before this row access. Read U16 flags0. Exactly0
returns without frequency store or any request.

Nonzero: store low16 F at V.half18, THEN reload flags0. Only mask8 nonzero admits
immediate request processing. Clear returns with the cached halfword changed,
without IRQ/counter/MusyX work. No old-frequency equality/zero/range clamp or
signed interpretation. F65536 stores0; Fffffffff stores65535. This write occurs
before the critical section and its gate is independent of the initial nonzero
admission. Flag/state consumers and valid provider domains remain separate.

## Shared counter admission

Read U16 S.half20c at803e0344. Initial0 requests OSDisableInterrupts80299ad4 at
800dd9d0 and stores returned state at S.word210/803e0348. Initial nonzero skips
that request. Then freshly read counter, increment with low16 store. No overflow
cap:65535 can wrap0. No prior IRQ token retained on stack or counter invariant
checked. The manifest's alias label at this target is the same OS entry, not a
different service.

Row V remains captured; neither pool-pointer changes nor child effects recapture
it. Shared sound-state counter fields are addressed through retained S. Valid
address/alias/lifetime/IRQ/concurrency domains are required for execution.

## Primary then fresh conditional secondary frequency

Read raw V.word4c, THEN unsigned V.half18. Request
sndStreamFrq(primaryID,freshCachedFrequency)8027bd78 at800dd9f4. No ID sentinel,
voice/state/status gate beyond earlier flags. Ignore return.

AFTER primary child read V.half1a. Exactly2: read fresh wordd4 then reread
half18, request sndStreamFrq(secondaryID,secondFreshFrequency) at800dda0c.
Other type values skip. No type normalization, stable two-ID snapshot, common
retained frequency or primary-success condition. For example a primary child's
change of half18 from48000 to44100 makes the second request consume44100 under
this source ordering; actual mutation/ownership requires provider evidence.

The MusyX child owns its fullword frequency/state2/FP/companion/DSP behavior.
This game wrapper supplies freshly loaded UNSIGNED16 frequencies. It does not
calculate a hardware ratio, inspect physical playback or update frequency again
after either child. Flags and other row fields have no additional local writes.

## Fresh counter exit and return

After requests read counter FRESH. Zero skips decrement AND restore. Nonzero:
store low16(counter-1); low16 result0 reads saved S.word210 FRESH and requests
OSRestoreInterrupts80299afc at800dda38. Other results skip restore. No replacement
saved state, forced counter normalization or global-marker invalidation here.

Initial65535 with stable wrap0 therefore issues no local disable/decrement/restore,
although it can still issue stream-frequency requests. This is an unguarded
source domain, not a recommended synchronization rule. Children have independent
nested calls; source request counts do not prove IRQ restoration in all domains.
No independent success/failure return assignment. Suffix restores LR/GPR30/31
and16 decimal stack bytes; valid ABI/stack/child preservation remains separate.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_frequency_request_evidence.json) pins
four spans (complete body/three linked children),all53 anchors,four calls.
Six halfword narrowing/seven gate/four post-primary reread/six counter vectors
checked. Generic verifier checks hashes/anchors/entry targets, not semantic
provider deductions or original execution. Fresh full ELF/listing/generator
remain `/tmp`; no bodies/binary assets committed. Local whole wrapper answered.

Remaining: actual channel/base/flag/frequency/type/two-ID/counter/saved-state
providers and valid value/extent/alias/lifetime/IRQ/concurrency domains, game
stream engine/play/pan/volume/callbacks, transitive ARAM/AI/DSP and original
frequency/dual-stream/IRQ/audio traces. EVT-01/native/full-game binding remain OPEN.
