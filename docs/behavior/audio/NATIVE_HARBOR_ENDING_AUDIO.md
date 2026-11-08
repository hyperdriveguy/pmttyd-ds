# Native Harbor ending audio

This contribution extends finite primary-sample PCM presentation and adds
explicit native music-stream ownership. It does not implement whole MusyX,
PSNDMain, BGMMain, source envelopes or the original ending coordinator.

## Actual unit sound

NativeBattleSound::unitSound(unit,key,rawVolume,belong,cursor) delegates to
the existing whole resumeSourceBattleUnitSound and actual retained State
services. Caller retains one cursor per invocation and supplies actual source
arguments. No EVT/guest stack is fabricated. Original position/status/pitch RNG,
PSND On birth, pitch and volume are retained. Completed cursors return their
handle without RNG/On replay. Changed invocation identity, stale lifetime,
retirement and recursive public entry cannot restart a successful prefix.

The ending literals802feafc/802feb14 are SFX_VOICE_MARIO_RELIEF1/RELIEF2,
not invented WIN aliases. Rows225/226, descriptors200002c1/200002c2,
objects831/832 and samples635/636 are original. Literal802feaa8 is
SFX_STAR_POINT_GET1: row2616, descriptor20000367, object1004, sample785.
The source raw-volume sentinel is f1194d80; actual unit/belong come from the
ending owner.

## PCM admission

The empty-destination importer verifies original name/row/object/single-sample
identity, four-bank hashes and per-asset encoded ranges. Three editable WAVs
retain original amplitude and rates: RELIEF1/2 at22050Hz, STAR_POINT_GET1
at16000Hz. No resampling, normalization, tones, macro/spatial parity or audible
native proof. Per-asset provenance/license accompanies the WAVs; normal recipes
compile edits and the importer refuses nonempty destinations.

The exact journals extend the same four NDSP voices10..13 and actual
NativeBattleSound/registry/birth ownership. Missing resources retain logical
slots. Cache grows15→18, with a native288Ki-frame (576KiB) budget; all original
eighteen total262587 frames. This budget is native policy, not measured
original behavior. Each clip remains bounded65536 frames. The star cue queues
at its actual16kHz. Muted known clips progress through real PCM at zero mix.
Only real backend completion Offs the same owned birth; external Off/reuse stops
old hardware without touching its replacement. No fake engine/PSNDMain receipt.

## Explicit music ownership

HarborBattleMusicBinding pins actual runtime plus independent proof/user/current.
Current must remain a pure ownership predicate checking weak actual incarnation.
Audio pins actual runtime storage. Calls are serialized on the application thread.

beginHarborBattleMusic(binding,enabled) starts existing original track12;
beginHarborVictoryMusic(binding,enabled) transitions to existing track2.
updateHarborVictoryMusic(runtime,enabled) services that owner. Muted starts queue
real PCM at zero mix; unavailable engine/resource returnsfalse. No main/Game
activation is included and no original BGM completion is claimed.

Every actual start/stop advances per-stream native generation; saturation
poisons0 permanently. Leases capture real slot/generation/track only from actual
successful start. Matching old/foreign track numbers cannot be adopted. Actual
binding and child identity stay fixed across holds. Successful battle stop,
victory start and mix prefixes are separately retained. Victory cannot downgrade
when legacy frontend12 visits continue. Ordinary/updateFrontend(int) defer
while explicitly owned. Other frontend overlays remain outside this policy;
the real coordinator must select the proper route.

Immutable RomFS header/length preflight precedes clearing ANY prior streams.
A violated second-open invariant poisons new admission rather than replaying
an unknown prefix. Resource mutation during the serialized handoff is unsupported.
This owner allocates no original source BGM slots.

retireHarborBattleAudio(runtime) rejects foreign12/2 leases before PCM retirement,
then stops only matching generations. Stale leases cannot stop replacement
streams. Teardown borrows independently pinned native backend, not stale source
memory. Successful retirement releases strong runtime/proof pins and retains only a weak
incarnation receipt, preventing leaks and address-reuse revival. Shutdown clears
queues before metadata release. None of this authorizes
Pouch rewards, whole original End or field return.

## Focused validation

Actual VM/source sound and portable PCM tests use complete coherent frozen
Star4775c988 headers/core. Audio layout changes receive separate ARM checks,
never old-core ABI execution. Tests hold eighty current boundaries, verify
one pitch RNG/On birth, original star-point name lookup, raw volume,
stale/reentry/retirement and allthree new PCM rows. Existing voice15,
NativeBattleSound and retirement tests run unchanged. Music tests cover retained
waits, mute, foreign control blocks, replaced generations and retirement.
Importer/source verifier read the ISO only. Fresh full root make test/make all
and actual audible native checks remain necessary.
