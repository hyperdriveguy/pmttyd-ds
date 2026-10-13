# MusyX emitter vector/volume update and 3D state reset

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndUpdateEmitter8028c400/d8
and s3dInit8028da94/38hex. Read
[emitter queries](SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md),
[removal/list lifetime](MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md) and
[bootstrap caller](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).
Offsets/addresses hexadecimal; arithmetic volume divisor127 is decimal.
Static source writes only; actual CalcEmitter/voice/audio execution remains open.

## Complete update admission and retained inputs

Inputs r3..r6: emitter E, first-vector pointer P, second-vector pointer V,
raw volume X. Retain all four; read sndActive8041ee60 ONCE. Zero returns0
without reading/writing E/P/V or requesting IRQ. Nonzero requests hwDisableIrq.
No emitter active flag, list membership, null/bounds/source-generation check;
a currently unlinked emitter can still have its fields updated under active1.
No fresh sndActive check after disable.

## Low-byte target and ordered vector copy

Take x=U8(X). Convert using the pinned unsigned integer bias double
0x4330000000000000 and fsubs, then fdivs by original float127. Let target T.
No clamp at127 or float1; x128..255 yields T>1 in ordinary finite arithmetic.
Full raw X is not an independently retained larger volume value for later writes.

Load P.word0 and P.word4; store them to E.word10/E.word14, in order.
The target division occurs before loading P.word8; then load P.word8 and store
E.word18. Load V.word0/V.word4, store to E.word1c/E.word20; only afterward load
V.word8 and store E.word24. Store target float T to E.float2c.
These are raw word copies; no normalization, velocity integration, geometric
transform or emitter flag/voice/controller request. The two vector field roles
beyond this ABI require consumers/caller evidence; do not invent units.

Each Z word is read AFTER its corresponding X/Y stores, and the whole second
vector is read after first-vector publication. Aliased input/target/vector regions
can therefore affect later reads. Do not replace with two atomic vector snapshots
without source-domain evidence. The heading/listener argument family is separate.

## Current-volume one-sided update and return

After storing target, read E.float30, then read E.float2c FRESH. Ordered comparison
current>target stores the freshly read target to current; otherwise no current
write. Ordinary finite values behave as min(current,newTarget), but unordered
values follow the literal PPC comparison and are not normalized to zero.
Increasing target can leave current unchanged; lowering target can immediately
lower current. No local smoothing/rate/timer increment is present.

Request hwEnableIrq, return1. Return1 means active-system admission/field-update
completion, not list membership, valid voice or audible acceptance. Links, flags,
external voice and FX identity remain untouched; no allocated resource is freed.
The IRQ pair uses existing MusyX nesting, separate from frame direct OS critical
calls. Actual concurrency/alias/null/vector/volume domains remain prerequisites.

## Complete 3D initializer and flag-width mapping

s3dInit(rawFlags F) has no sndActive gate, IRQ or child requests. Extract
(F>>1)&1 and (F>>2)&1, then publish in this exact order:

| Global | Publication |
| --- | --- |
| s3dEmitterRoot8041efa4 | word0 |
| s3dListenerRoot8041efa0 | word0 |
| snd_used_studios8041ef9c | word0 |
| snd_base_studio8041ef9a | byte1 |
| snd_max_studios8041ef99 | byte3 |
| s3dCallCnt8041efa8 | byte0 |
| s3dUseMaxVoices8041ef98 | byte((F>>1)&1) |
| s3dUseLegacyLogic8041ef97 | byte((F>>2)&1) |

Bits outside1/2 do not change these local configuration outputs. It does NOT
unlink/clear old nodes, send key-off, free listeners/emitters, unregister resources,
initialize caller vector records or synchronize callbacks. Root0 and detached
old active flags can coexist after reset; query leaves test stored flags, not
list membership. Safe initialization/reset ordering remains source ownership.
Return r3 holds literal3 used for the studio-byte store, without a separately
assigned success code. The complete sndInit caller ignores this return.

## Caller census and verification

Whole-DOL aligned direct-call census finds two sites: sndUpdateEmitter in
SoundEfxMain, s3dInit in sndInit. It does not establish complete SoundEfxMain
arguments/gates or indirect/REL/dynamic callers. No original per-frame cadence
or current-volume ramp duration is inferred from these local writes.

[Manifest](../../../tools/musyx_emitter_update_init_evidence.json):two complete
bodies/two linked IRQ bodies/two original literal spans,6 spans/47 anchors/two
resolved direct calls plus two caller-site anchors. Eight volume-boundary, seven
configuration-bit and five finite current/target vectors are derived checks,
not original execution or a hardware floating-point oracle. Full fresh ELFs/
listings stay `/tmp/inbox-emitterupdate.elf` and `/tmp/inbox-s3dinit.elf`.

Remaining: emitter construction/active/link/vector/volume-current consumers,
SoundEfxMain actual argument/provider/gate graph, listener/studio setup and valid
alias/null/finite/reset/concurrency domains; original update/reset/audio traces.
Two complete local bodies are answered; full native/source/runtime remains OPEN.
