# Sound effect record check, volume, pitch, aux and stop leaves

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Five complete local bodies below;
MusyX/emitter/provider execution remains a boundary. These accept effect-pool
RECORD ORDINALS from [PlayEx](SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md),
not positional slot allocation handles or raw MusyX voice IDs.

| Body | Address / hex byte size |
| --- | --- |
| SoundEfxCheck | 800e07c0/9c |
| SoundEfxSetAux1 | 800e0a64/78 |
| SoundEfxSetVolume | 800e0afc/c0 |
| SoundEfxSetPitch | 800e0bbc/7c |
| SoundEfxStop | 800e0c38/68 |

Each reads current word(803e0138+f4), then adds wrap32(136*ordinal), where
136 is decimal record stride. There is NO ordinal-1, range, pool-null or stale
generation check before record access. Valid caller/source domains are required;
native checked errors are adapter policy, not recovered graceful returns.

## Liveness checker

Read U16 flags0. Zero returns0 with no backend request. Nonzero mask10 set
requests sndCheckEmitter(record+14) and returns1 exactly for nonzero result.
Mask10 clear loads record.voiceC, requests sndFXCheck(voice), then requests
sndFXCheck on that returned value AGAIN; second result-1 returns0, else1.
The complementary branch pair800e07f8/7fc makes800e0800..80c's emitter-voice
conversion path locally unreachable. There is no record clear in this checker.

## Shared setter voice capture

Volume, pitch and aux do NOT check flags0 for liveness first. They test only
mask10: when set, request sndEmitterVoiceID(record+14), then sndFXCheck on its
returned value; when clear, sndFXCheck(record.voiceC). Unlike Check, these
setters make ONE sndFXCheck request. A returned-1 skips all stores/controllers.
No “same record therefore valid” shortcut or duplicate Check policy is added.

## Volume and global zero-volume gate

Retain raw caller volume V and resolved record across voice requests. If valid,
store ZERO-extended U8(V) to record.unsignedHalf4 BEFORE calling
sndFXCtrl(resolvedVoice,7,V), with raw V rather than the narrowed field.
Then freshly read U16(803e0138+214). Nonzero calls sndFXCtrl with the SAME
retained voice, controller7 and value0. The stored halfword still retains
U8(V). No min/max clamp, rollback or fresh voice lookup after the controller.
The record pointer can be retained while the final global gate is freshly read.

## Pitch and aux controllers

Pitch retains raw signed/word input Q, resolves/checks voice, then requests
sndFXCtrl14(voice,128,low16(wrap32(Q+8192))). It writes no record pitch field
and performs no signed clamp. For derived Q=-8192/0/8192, controller values
are0/8192/16384; values outside that range still narrow as encoded.

Aux retains raw value A, resolves/checks voice, then requests
sndFXCtrl(voice,91,A). No record-field store or narrowing before the call.
Controller128 and91 are decimal literals, not inferred UI percentages or
reverb labels beyond this local request. Underlying controller meanings and
hardware response require their own MusyX contracts.

## Stop and reload after KeyOff

Read flags0. Zero returns without writes. Nonzero reads record.voiceC and
calls sndFXKeyOff. Reread flags0 AFTER that call; mask10 set requests
sndRemoveEmitter(record+14). Finally clear only flags0 halfword. The voice,
priority, stored volume/pan/FX and remaining record bytes are preserved.
No voice-1 guard, whole-record zero, automatic unregister or backend-slot
allocation follows. This source ordering matches PlayEx's victim stop sequence.

Validation: [metadata](../../../tools/sfx_startup_backend_evidence.json) pins
all five bodies and selected load/call/branch/store anchors. Fresh full listings
were generated read-only under `/tmp/inbox-fx*.elf`. Existing generic DOL
verification checks source identity/instructions, not original audio execution.
Remaining: full sndFX*/emitter/controller semantics, pool allocation/other
writers/teardown, source valid ordinals and original mute/liveness/control traces.

[Complete MusyX API/voice/IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md) now establish the used public calls, external-versus-internal voice identity and separate IRQ nesting. Core synth/resource/map lifetime and original playback remain open.
