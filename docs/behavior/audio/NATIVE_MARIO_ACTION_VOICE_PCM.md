# Native Mario action-voice primary PCM

This finite native presentation consumes fifteen original Mario action-voice
primary samples. It does not implement MusyX macro execution, source psndMain,
pitch/envelope/spatial parity, or an original started-engine handle.

## Original assets and identity

`tools/import_mario_action_voice_assets.py` reads the US revision-zero ISO
read-only and refuses a nonempty destination. Original pointer arrays80416ae4,
80416af8 and80416ad0 identify AC1/AC2/AC3 variants. Actual sound rows165..169,
171..175 and177..181 resolve respectively to effects654..658,659..663 and
664..668, then single primary samples590..604. Each original name, row,
descriptor, project object and sample indirection is checked. Four complete
sound-bank hashes and individual encoded-sample offsets/hashes are recorded in
per-asset provenance. No bank or ISO payload is added to history.

The fifteen editable WAVs in `assets/original/audio/mario-action-voice/` are
mono PCM16 at the original22050Hz:209739 frames,419478 sample bytes total.
Normal `kind:sfx` recipes copy these edits; builds never rerun the importer over
them. Original rights remain reserved; tracking grants no redistribution license.

## Actual native ownership

`MarioActionVoicePcm` holds four native voices and pins the actual public bank,
actual Harbor runtime, actual source Sound owner and nonwrapping slot birth.
The native adapter rechecks actual VM name and full descriptor even for a cached
sample. A shared bank row alone cannot authorize playback. Each actual birth
queues at most once. Source Off, replacement, stale runtime or unsupported
profile stops a native channel without completing another occupant.

The source slot remains pending with engine-1. Real NDSP DONE first clears the
actual channel, then calls source registry Off only for the same owned birth.
Unknown names, unavailable files and unsupported delayed/volume profiles retain
logical pending slots. False atomic backend operations have no effect; retries
retain successful queue/stop prefixes. Self-reentry is rejected.

Channels10..13 are a bounded native policy, handed off only after the actual
OpeningHousePcm owner clears its channels. A new nonnull house owner first stops
battle voices. Later null house visits cannot clear battle channels because its
own queued mask is empty. Cached linear samples remain pinned until after
ndspExit; maximum resident storage is512KiB. Muting queues/continues real PCM
with zero mix and never manufactures a completion timer or replays on unmute.

`Audio::retireHarborBattleAudio(actualRuntime)` stops actual owned voice
channels and actual track12 before returning true. A foreign/unproven owner
returns false. The completion caller must stop selecting track12 before this
hook and retire source pending slots afterward. This is native cleanup, not
whole original BGMOff/BattleEnd completion. Source Nice/success sounds outside
these fifteen remain pending and have no new audible consumer.

## Validation and integration limits

Focused strict host tests and ASan/UBSan use actual read-only ISO rows and the
real public registry/On implementation; backend status/queue fixtures are
explicitly test-only. They cover all fifteen names/descriptors, source birth
reuse, foreign same-bank ownership, capacity, missing resources, atomic holds,
muting, stale/reentry and no replay. Importer tests use fresh original extraction
and reject overwriting an editor change. Strict ARM checks cover new modules and
all journaled callers; pre-existing indentation warnings alone are demoted in
old callers. No native audible playback or normal tutorial reachability is
claimed by these focused tests. Root must rebuild the complete fresh ABI graph
and run normal make test/make all plus isolated emulator checks.

Actual completion retirement retains the runtime incarnation as a weak marker.
Later service visits cannot reacquire that incarnation's channels, and the main
Harbor music branch cannot restart its track12 while field return is held.
A different actual runtime is admitted separately; no numeric scene/track token
certifies another owner's retirement.
