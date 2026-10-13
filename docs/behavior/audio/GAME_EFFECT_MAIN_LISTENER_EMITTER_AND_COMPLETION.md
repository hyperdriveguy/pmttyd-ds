# Game effect main: listener, emitter update and completion passes

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundEfxMain800e047c/344,
SoundMain800e22f4/a8 and SoundMainInt800e22d4/20hex. Read
[emitter update](MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md),
[emitter queries](SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md),
[removal](MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md),
[record controls](SOUND_EFFECT_RECORD_CONTROL_LEAVES.md) and
[outer psndMain](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md).
Offsets/addresses hexadecimal; loop counts explicitly decimal. Static source
order only; original cadence/voice/audio outcomes remain open.

## Complete listener-vector preparation

Shared sound state S=803e0138. SoundEfxMain has NO local init/active/pool gate.
Initialize retained sine/cosine accumulators to original float0. Read S.float1f4;
angle=fdivs(fmuls(originalTwoPi,heading),360). TwoPi is exact original float
6.2831854820251465. No heading normalization/finite check.

Read all six floats at S+1b8/+1bc/+1c0 and S+1c4/+1c8/+1cc. Retain the three
current and prior values; store current XYZ to previous slots1c4/1c8/1cc.
Using fsubs, write current-minus-capturedPrevious to S+1d0/+1d4/+1d8.
No elapsed-time division, velocity clamp or smoothing. This is displacement
per admitted invocation, not a measured velocity/rate.

Request sin(angle), frsp, fuse1*sin+0 to retained X; request cos(retained angle),
frsp, fuse0-1*cos to retained Z. Load S.word204 and retain low8. Publish direction
S+1dc/+1e0/+1e4=(computedSin,0,computedNegativeCos), and second basis vector
S+1e8/+1ec/+1f0=(0,1,0). Then request:

`sndUpdateListener(S+128,S+1b8,S+1d0,S+1dc,S+1e8,U8(S.word204),0)`

The final seven arguments are r3..r9; no listener-field/query guard or return
check. Listener transform/normalization/field semantics belong to that child.
The heading/position providers for this S are distinct from calc3D's psnd state
until actual writers establish a relationship. Do not substitute camera fields
from the other listener family without source evidence.

## First40-record pass: active emitter source publication

Capture effect pool S.wordf4 ONCE. Scan40 decimal records, stride88hex; each
record E remains based on that capture. flagsHalf0==0 skips. Nonzero without
mask10 skips. For admitted emitter record, compute/write using single fsubs:

- E.float7c=E.float64-E.float70.
- E.float80=E.float68-E.float74.
- E.float84=E.float6c-E.float78.

These read/store sequentially; no whole-vector snapshot. Then reread current
floats64/68/6c and copy them one by one to previous70/74/78. Read E.unsignedHalf4,
keep low8; request sndUpdateEmitter(E+14,E+64,E+7c,U8(volume)). Outgoing r7=0
is explicitly assigned, but the complete child reads only r3..r6. Ignore return.
No voice/controller check, emitter membership gate, allocated-pointer guard,
per-second scaling or follow-up stop in this pass.

## Second40-record pass: captured traversal versus fresh pool lookups

Capture S.wordf4 AGAIN as traversal pointer T; scan40 records at the same stride,
maintaining ordinal-byte offset O separately. Start each visit with T.flagsHalf0;
zero skips all remaining work for that visit. This traversal pointer is retained
across child calls, while query/cleanup paths below reload S.wordf4 for ordinal O.
An intervening pool mutation can select a different queried/cleared record.

If captured T.mask10 CLEAR: reload pool, select Q=pool+O, read Q.flags mask10.
Set chooses sndEmitterVoiceID(Q+14) then sndFXCheck(result); clear chooses
sndFXCheck(Q.wordc). In BOTH cases request sndFXCheck(firstCheckReturn) AGAIN.
Second result!=-1 preserves record; exact-1 follows cleanup below. No initial
Q.flags-nonzero check in the query path, no cached validity shortcut or keyOff.
Even this outer non-emitter branch contains a fresh emitter selector; different
pool pointers/flags can make it reachable. Do not eliminate it as contradictory.

Cleanup: reload pool, retain C=pool+O, read C.flags. Zero skips cleanup; nonzero
requests sndFXKeyOff(C.wordc), ignores return, rereads C.flags. Fresh mask10 set
requests sndRemoveEmitter(C+14). Finally store C.flagsHalf0=0. Retained voice,
volume/pan/surround/countdown/position and other bytes are not erased or freed.
Child mutations can change the post-keyOff emitter gate; pointer C is retained.

## Emitter branch: flag query then countdown continuation

If captured T.mask10 SET: request sndCheckEmitter(T+14). Nonzero skips initial
cleanup. Exact0 invokes the same fresh-pool cleanup sequence above. Regardless
of that outcome, continue using original traversal T and read T.unsignedHalf0a.
Nonzero decrements/stores it and ends this visit; reaching0 does NOT also query
in this invocation. Zero-on-entry repeats the fresh-pool voice-resolution/double
sndFXCheck and exact-1 cleanup sequence described above.

There is no fresh T.flags-nonzero gate between initial cleanup and countdown.
Cleanup may clear T.flags but this continuation still occurs. T and the fresh
pool's C/Q need not be the same address under unsupported mutation domains.
No counter underflow on the admitted nonzero decrement, timing units, automatic
countdown reset, generation validation or completed-record removed-count return.

## Complete outer SoundMain and interrupt wrapper

SoundMain captures S.wordf0 as four-record traversal, stride10hex. For each:
word8==0 skips; otherwise word0==-1 skips. Request sndSeqGetValid(word0)80279d6c;
EXACT return1 preserves it. Every other return: reload record.word8, OR its
pointed unsignedHalf0 with2, store; reload record.word0, request sndSeqStop80279d9c;
then store record.word0=-1. No pointer/generation guard, rollback or record free.
No assumption that every nonzero validity result is accepted.

After all four records, request SoundEfxMain, SoundDVDMain800e00b0, then
SoundSSMain800dd2e0, in that order; no child return checks or separate success.
SoundMainInt requests SoundSSMainInt800dce90 once, without a local gate/counter.
Actual calling cadence/SDK interrupt context and DVD/SS/sequence child semantics
remain separate. Outer psndMain admits this SoundMain only under its own init_f.

## Validation and remaining source/runtime

[Manifest](../../../tools/game_effect_main_listener_cleanup_evidence.json) pins
three complete bodies,14 linked children and four original literals:21 spans,
135 anchors and25 resolved direct calls. Derived40 pool offsets, four countdown
boundaries and double-query argument metadata preserve source distinctions;
original code is not executed. Full fresh ELFs/listings stay `/tmp/inbox-effectmain.elf`
and soundmain/soundmainint files; no body/bank asset exported to history.

Remaining: listener update/transform and S position/heading/basis providers,
record/pool/countdown/sequence metadata writers, pointer/flag/alias/concurrency
invariants, DVD/SS/sequence/controller/voice execution and original displacement/
completion/audio traces. Three complete local services are answered; full native/
source/runtime/hardware equivalence remains OPEN.

[Complete listener update/cross/inverse](MUSYX_LISTENER_UPDATE_CROSS_AND_MATRIX_INVERSE.md) now closes the exact listener child and its math helpers. Actual basis/allocation/alias/singular/PPC/consumer/source/runtime remains open.
