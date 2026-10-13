# Effect pan/surround controls, volume read and emitter lookup

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundEfxSetPan800e09dc/88,
SoundEfxSetSrndPan800e0904/d8, SoundEfxGetVolume800e0adc/20,
sndEmitterVoiceID8028c5a0/5c and sndCheckEmitter8028c4d8/20hex.
Read [other record controls](SOUND_EFFECT_RECORD_CONTROL_LEAVES.md),
[live SFX requests](POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md),
[MusyX public APIs](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md) and
[synth controller core](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md).
Offsets/addresses hexadecimal; explicit decimal arithmetic literals labeled.

## Record addressing and store BEFORE voice resolution

The three game leaves accept raw effect RECORD ORDINAL I, not positional handle
or MusyX voice ID. Compute wrap32(I*88hex), read effect-pool pointer from
803e0138.wordf4. No ordinal-1/range/pool-null/record-generation guard.
Pan/surround retain raw caller value X; store zero-extended U8(X) to a halfword:
Pan+6, Surround+8. No active/flags/voice check precedes this store.

Then RELOAD the global pool pointer and form the record for voice resolution.
The published halfword and checked record may differ if pool state changes.
No stable-pointer shortcut or rollback on voice miss. This differs from volume,
which publishes its halfword only after a valid voice lookup in its own body.
No added field reset/allocation/free/status code or software clamp.

## Common voice resolution and pan request

Read current record flagsHalf0. Only mask10 matters; flags0 does not prevent
lookup. Mask10 set requests sndEmitterVoiceID(record+14), then sndFXCheck on
its returned word. Mask clear loads record.wordc, then requests sndFXCheck.
No second Check request, no pointer revalidation after child calls. Check return
exact-1 skips controllers, preserving the halfword already stored.

Pan valid return: request sndFXCtrl(returnedVoice,10 decimal,RAW retained X).
The public wrapper/core width rules apply separately: the game passes raw X;
its record stores only U8(X). No zero-volume gate, surround request or return
classification is added here. Controller return is not locally tested.

## Surround two-controller sequence

Surround retains the sndFXCheck result across the first controller request.
Valid result requests sndFXCtrl(voice,131 decimal,RAW retained X), ignoring return.
Then derive x=U8(X), compute word C:

- x0: C8192 decimal.
- x1..63: C=8192-3*(64-x).
- x64..255: C=8192-3*(x-64).

Request sndFXCtrl14(RETAINED voice,132 decimal,U16(C)), ignoring return. This
is a second public API request with its own fresh mapping, not reuse of a cached
internal synth identity. First request mutation/failure does not suppress it.
No clamp at127: all256 byte values reach the mapping. x0 is discontinuous from
x1; x64 also yields8192. Range over admitted byte x is7619..8192; this is a
source controller value, not a measured sound/surround/pitch quantity.

## Complete volume read

GetVolume computes the same ordinal/pool address, reads unsignedHalf4, returns
only its low8. No active flag, voice/emitter resolution, backend query, hardware
volume observation or field mutation. It can return a retained record value
for inactive records. [Fade caller](POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md)
uses that returned byte; actual controller acceptance is independent.

## Complete emitter flag query

sndCheckEmitter(E) first reads sndActive8041ee60. Zero returns0 without touching
E. Nonzero reads E.wordc, returns bit10000 as0/1. No IRQ, voice validity query,
list membership/source/null guard, mutation or generation check. It reports this
stored flag under the active gate, not independently observed audible playback.
A null E is not universally permitted just because the inactive branch avoids it.

## Complete emitter voice lookup

sndEmitterVoiceID(E) initializes retained result-1, reads sndActive ONCE. Zero
returns-1 without touching E or IRQ services. Nonzero requests hwDisableIrq;
reads E.wordc. If bit10000 set, capture E.word38 as returned external voice;
otherwise retain-1. Request hwEnableIrq; return retained result.

No fresh sndActive read after disable, voice-map/check request, null/member/
generation validation or allocation. Flags and stored voice are read under the
local MusyX IRQ pair; actual concurrency/list lifetime is still a source domain.
The game setters subsequently check this returned identity separately. Record
mask10 selects emitter storage, while emitter.wordc mask10000 controls whether
its voice field is read; these are distinct fields/masks.

## Verification and remaining owners

[Manifest](../../../tools/game_effect_spatial_control_leaves_evidence.json) pins
five complete bodies, five linked public/IRQ services,75 original anchors and11
resolved direct calls. All256 surround mappings, nine raw-width vectors and15
active/flag vectors are derived checks, not original execution. Full fresh ELFs/
listings stay `/tmp/inbox-efx*.elf` and `/tmp/inbox-emitter*.elf`.

Remaining: effect-pool/record/emitter active/voice writers and valid ordinal/
pointer/lifetime/concurrency domains, emitter construction/update/removal and
controller/MIDI/voice/backend execution, original pan/surround/fade/audio traces.
Five complete local leaves are answered; full audio/native/runtime remains OPEN.

[Complete vector/volume update and 3D root/config reset](MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md) now closes those exact local state producers. Update does not require emitter membership; reset does not clear detached flags/voices. Construction/consumers/provider/runtime remain open.
