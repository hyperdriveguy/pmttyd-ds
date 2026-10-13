# Positional SFX pending startup: complete local request region

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete psndSFXMain pending/startup region
800da010..800da398 within body800d9e74/b94hex. This closes the source requests
in that region, not full psndSFXMain spatial/live-volume/fade or engine internals.
Read [outer update/prefix/tail](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md),
[provisioning](POSITIONAL_SFX_SLOT_PROVISIONING.md) and
[effect backend](SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md).

## Admission, delay and common volume expression

Retain P=current40-byte slot, R=80316b28+20*(P.key&1fff), G=803df6e8.
The second40-slot scan skips key-1, increments occupied count, and reads flags6.
Flag1 clear skips startup and enters live processing. Flag1 set clears flag100,
then reads unsignedHalf P0e. If nonzero, decrement/store it and skip this slot
for the rest of THIS update. Decrement-to0 does not start immediately; startup
requires delay0 on entry to this region.

For each startup backend, derive V from ordered slot reads:
U8(P8), U8(P9), U16(P10). Compute trunc(product/127)-P10 using signed-word
magic-multiply division, then clamp0..127. In the actual byte-product domain,
trunc equals floor. Low8 V is passed to volume requests. These local clamps
are real, unlike provisioning's equality-based255 defaults. Mutating service
boundaries may cause separate branches or later processing to reload fields;
there is no universal cached V for the whole main routine.

## Stream descriptor branch (R.word4 mask80000000)

Scan channel ordinals0..2, calling SoundSSCheck at each, accepting the first0.
If all three are occupied, scan0..2 again: for each Check whose mask20 is set,
request SoundSSStopCh. Then scan0..2 a THIRD time for first exact0; all occupied
supplies channel-1. This is not a single priority-based steal or unconditional
channel0 fallback. Keep all repeated checks and their fresh results.

Store chosen channel in P18. If-1, converge on failure cleanup. Otherwise:
SoundSSPlayCh(P18,R.word8), SoundSSSetPanCh(P18,U8(Pa)), compute V, then
SoundSSSetVolCh(P18,U8(V)). This region has no extra return check after PlayCh
and no surround/pitch request for the stream branch. R.word8 is a pointer/word
source passed unchanged, not low16 of the descriptor.

## Song descriptor branch (80000000 clear,40000000 set)

Scan channel ordinals0..3 through SoundSongCheck, accepting first exact0.
No local stealing pass. Store channel or-1 in P18. Nonnegative selected ordinal
requests SoundSongPlayCh(P18,R.word4&0fffffff), then derives V and requests
SoundSongSetVolCh(P18,U8(V)). The resource mask keeps LOW28 bits; it is not
low16 or a table ordinal. No separate pan/pitch call in this startup branch.

## Effect descriptor branch (both stream/song bits clear)

If P.flags6 mask2 is set, derive V then request
`calc3D(P.key,&P.position1c,U8(V),U16(P12))` at800da264. Store the returned
word in a private stack word. Its BIG-ENDIAN byte0 is the volume, byte1 pan,
byte2 surround input. This scoped delivery does not guess calc3D's internals.
Then, in exact order:

1. SoundEfxPlayEx(R.word4&ffff,0,stack.byte0,stack.byte1), store returned
   backend RECORD INDEX at P18.
2. If returned index!=-1, SoundEfxSetVolume(P18,stack.byte0),
   SoundEfxSetSrndPan(P18,stack.byte2), SoundEfxSetPitch(P18,S16(P14)).
3. If initial result==-1, skip those three controls, but still enter common
   effect aux gating and final startup cleanup.

If flag2 is clear, derive V and request
SoundEfxPlayEx(R.word4&ffff,0,U8(V),U8(Pa)); store result P18. A result other than-1
requests SoundEfxSetPitch(P18,S16(P14)); there is no initial SetVolume or
SetSrndPan in this nonspatial branch. Original Hammer PI1/PONE1 use the latter
route after [the public nonpositional wrapper](HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md).

Both effect routes then reread P18. If-1, omit aux. Otherwise read U8(R.byteF).
For a positive byte, add U16(G52); for byte0 do not add G52. Upper-clamp result
127 (no additional lower clamp), then request SoundEfxSetAux1(P18,low8(value)).
This uses a fresh P18, which may have changed during earlier controls.

## Final cleanup and same-update live processing

All three backends converge at800da36c and reread P18. Exact-1 stores P.key=-1,
then P18=-1 and skips the slot. This does not undo all provisional field stores.
Otherwise clear ONLY pending bit1 in unsigned flags6, preserve other low16
flags, and immediately enter the live-update region800da398 in THIS same pass.
A startup can therefore be followed by another spatial/volume/control request
before the outer loop advances. Do not defer all live processing a tick or mark
the slot complete solely because a backend call initially returned a value.

## Validation and boundaries

[Source manifest](../../../tools/sfx_startup_backend_evidence.json) pins containing
bodies and selected request/branch/mask/store anchors. Re-run the generic DOL
verifier with this manifest. Fresh current full listing is
`/tmp/inbox-sfxmain-current.disasm`; ISO is opened read-only.
Independent arithmetic check covers every byte-product0..65025, verifying the
original signed magic sequence against division127; these are derived cases,
not original audio observations. Caller masks/fields distinguish public slot,
backend record index and underlying MusyX voice identity.

Remaining: full calc3D/listener/spatial controls, live/fade/global policy paths,
stream/song services, effect control and MusyX resource/hardware execution,
source table/slot writers and original audio/runtime traces. No WAV-only cue or
native guessed startup cadence is established. The parent audio chain stays OPEN.

[Complete check/volume/pitch/aux/stop leaves](SOUND_EFFECT_RECORD_CONTROL_LEAVES.md) now supply the used local ordinal/voice/controller and KeyOff request rules; underlying MusyX/emitter/runtime semantics remain open.

[Complete calc3D/angle-table source](GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md) now closes that exact local child, including flag precedence, 196 pan threshold, byte-first volume clamp and retained result byte3. Listener/shared writers/PPC/libm domains/live updates/original audio remain open.
