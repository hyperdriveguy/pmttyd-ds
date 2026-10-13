# SoundEfxPlayEx: complete allocation, stealing and start contract

US G8ME01 disc0 revision0; SoundEfxPlayEx800e0ca0/2f4hex, recovered2026-10-13
for [EVT-01](../../requests/EVT-01.md). Complete local body; called MusyX,
emitter, interrupt and resource services remain dependencies. Read
[pending caller](POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md).
No original playback or all-source reachability claim.

## Captures and two distinct identities

Retain four raw arguments: FX ID F=r3, reclaim priority A=r4, controller volume
V=r5 and pan N=r6. Retain S=803e0138 and initial pool base=word(S+f4).
There are40 records with stride88hex=136 bytes. A record is free when its
unsignedHalf0 is0. The public positional slot array at803df988 is separate.
The returned value is a pool RECORD ORDINAL, not the sndFXStartEx voice word
stored at record+c. Future controls resolve that ordinal through their own
record rules; do not collapse these handles into one channel ID.

## Free scan and first eligible victim

Scan ordinals0..39 ascending, implemented as four unrolled iterations of ten
records. First free flags0 record wins; do not retain the last free record as
the separate __psndSFXOn slot search does. If full, rescan0..39 ascending and
accept first record with U16(priorityHalf2)<=U8(A). Equality admits stealing.
If none, return-1 without a start request.

For an admitted occupied victim, call sndFXKeyOff(record.wordc). Reread its
flags0; if mask10 is set, call sndRemoveEmitter(record+14). Then clear flags0.
The reloaded flag follows KeyOff, not a pre-call snapshot. No sorting, oldest
selection, action of the project FX entry's priority, or atomic replacement is
specified by this body. This caller supplies its own byte priority.

## Interrupt nesting and ordered MusyX requests

Clear chosen record.flags0 again. Read U16(S+20c) nesting count. If0,
OSDisableInterrupts80299ad4 and store returned interrupt state to S210.
Increment low16 nesting count, then request:

`voice=sndFXStartEx(F,127,N,0)` at800e0e4c;
store voice to retained record+c;
`sndFXCtrl(record.voice,7,V)` at800e0e60.

Ctrl is requested even when the stored voice is-1. Raw F,N,V reach these
services before later byte/halfword record narrowing. Reread count; nonzero
subtracts1/stores low16 and restores saved S210 only when the new low16 is0.
Exact count0 skips decrement/restore. This source behavior supplies no overflow
or reentrancy repair; capacity/interrupt/source invariants remain required.
After restoration reread voice. Voice-1 returns-1, leaving the chosen flags0
and stored-1 voice; no additional undo/emitter request occurs on this path.

## Published record and global zero-volume gate

For a voice other than-1, store in order:
flags0=1; priorityHalf2=U8(A); volumeHalf4=U8(V); panHalf6=U8(N);
half8=0; FXHalf10=low16(F). No whole-record memset or remaining-field reset.
Then read U16(S214); nonzero requests sndFXCtrl(record.voice,7,0) while
retaining the earlier record volume field. A zero-volume command is not a
failed start or a change to the stored request value.

## Fresh final record/liveness gate

Reload word(S+f4), resolve the SAME ordinal against that fresh pool base, and
read fresh flags0. Flag0 returns failure status. Otherwise:

- Mask10 set: sndCheckEmitter(record+14); nonzero means live.
- Mask10 clear: read fresh voice and call sndFXCheck(voice), then call
  sndFXCheck on THAT returned word AGAIN. The SECOND result!=-1 means live.

The branch pair at800e0f1c/20 is complementary on one unchanged condition.
Consequently800e0f24..30's sndEmitterVoiceID path is locally unreachable;
record that dead region rather than inventing a third reachable liveness case.
The ordinary path's duplicate sndFXCheck is real and must not be silently
simplified to one query based on a presumed identity return.

Live returns ordinal0..39; failed liveness changes the local ordinal to-1 and
returns it. This final failure does not clear the already-published record or
key off the new voice. Later backend maintenance owns that lifetime. The fresh
pool may differ from the initially retained pointer under mutating source
services; this function performs no pointer-equality check or snapshot restore.

## Validation and remaining sources

[Manifest](../../../tools/sfx_startup_backend_evidence.json) records complete
body identity and original anchors, alongside the SFXMain caller region and
linked service identities. Fresh listing `/tmp/inbox-fxplay.disasm` comes from
read-only ISO inspection. Generic DOL metadata verifier pins the source, not
runtime liveness or macro/hardware fidelity.

Derived acceptance cases: first free, full pool/no priority candidate, equality
victim, post-KeyOff emitter flag, nested count0/1/wrap, voice-1 Ctrl request,
cover/global gate versus stored volume, fresh pool selection, duplicated Check
and final failure without cleanup. Remaining: S.f4 pool provisioning/other
writers, full sndFXStartEx/Ctrl/Check/KeyOff/emitter/interrupt semantics,
resource loading and original execution traces. Parent request remains OPEN.

[Complete check/volume/pitch/aux/stop leaves](SOUND_EFFECT_RECORD_CONTROL_LEAVES.md) now supply the used local ordinal/voice/controller and KeyOff request rules; underlying MusyX/emitter/runtime semantics remain open.

[Complete MusyX API/voice/IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md) now establish the used public calls, external-versus-internal voice identity and separate IRQ nesting. Core synth/resource/map lifetime and original playback remain open.
