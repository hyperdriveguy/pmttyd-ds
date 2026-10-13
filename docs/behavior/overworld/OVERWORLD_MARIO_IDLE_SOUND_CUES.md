# Original ordinary Mario idle sound cues and cancellation

US G8ME01 revision0; source-only recovery, linked to
[idle pose and phase ownership](OVERWORLD_MARIO_LOW_HP_IDLE.md).
This closes request/handle ownership and exact bank dependencies. It does not
synthesize the MusyX macros, infer unheard cue371 playback or select sample
fallbacks. Original audio payloads remain in the read-only ISO/ignored library.

## Requests and positional arguments

Every listed call uses `psndSFXOn_3D(cueIndex, &Mario.root8c)` with current
rootXYZ, not render pivot/root offsets or camera follow target:

| Source call | Admission in mot_stay | Cue index | Returned handle |
| --- | --- | --- | --- |
| 800a6df0 | Ordinary idle entry, HP==0, after M_I_S request | 371 | Discarded; no27c/280 write |
| 800a7034 | Exact name M_S_3, ChkKey!=0; increment58, reset on58>=rate*4; then58==0 | 225 | Store27c at800a7038 |
| 800a705c | Same block,58==rate*2 | 226 | Store27c at800a7060 |
| 800a72a0 | Healthy long-idle phase1, after evtpose M_I_5 and AdjustMoveDir | 239 | Store27c at800a72a4 |
| 800a7328 | Phase3 countdown<0, after evtpose M_N_5A and phase4 preparation | 228 | Store27c at800a732c |
| 800a7394 | Phase4 increment48 reachesrate*2, reset48=0 | 228 | Store27c at800a7398 |

These calls precede their subsequent source branches exactly as documented in
the idle phase owner. Counter58 resetting start-frame28 does not itself queue
an animation restart. Subsequent27c stores **overwrite the old handle without
calling Off first**; do not add a fabricated stop-before-each-breath rule.
mot_stay itself has no280 stores or separate280 request. The retained280 is
cancelled by its post owner; its producer is outside this bounded idle body.

psndSFXOn_3D800d9848 forwards to __psndSFXOn800d9934 with r3=cue index,
r4=255,r5=255,r6=0,r7=position pointer,r8=0,r9=16,r10=0
(800d9850–70). __On resolves a nonnegative argument as table index; masks the
index to13bits for pssfxlist stride20. It queues a psSFX slot, not an immediate
PCM start: r4=255 becomes127; r5=255 becomes table byte13; it copies position
XYZ to slot1c/20/24 and sets positional slot flag2 (800d9c60–80,9df0–e1c).
It stores r9=16 to slot12 and returns a handle with slot index in low8bits
and incremented generation counter shifted8 (800d9e2c–5c). Failures return−1.
Capacity/priority/deduplication/global gate admission belongs to __On, not a
native boolean indicating that NDSP accepted a sample.

psndSFXMain800d9e74 later starts queued effects, clears pending slot flag1,
updates/checks retained backend voices and positional controls. Its positional
path calls calc3D using the **copied position** (800da254–280); the call does
not retain the input Vec pointer as a live Mario-follow emitter. Explicit
psndSFX_pos/other services can update retained position separately.
For these literal flag20000000 table entries, Main passes low16 FX ID to
SoundEfxPlayEx, retains its returned backend handle, and applies pitch/pan/
surround/volume services (800da270–2b4). Source scene sound service/global
gates, allocation and spatial attenuation remain required native dependencies.

## Post cancellation

marioChgMot writes the **new** current motion before old mot_stay_post runs
([dispatcher ordering](OVERWORLD_MOTION_ARBITRATION.md)). At800a6b24–ba4:

- New motion19 or20: if27c!=−1, call psndSFXOff(27c), then store27c=−1;
  retain280 unchanged.
- Any other new motion: do that for27c, then if280!=−1, call Off(280) and
  store280=−1. Calls occur in this order; sentinel−1 makes no call.

No current cue index is substituted for these opaque handles. There is no
matching post-owned handle for discarded cue371. No pose counters/phase fields
are reset in the sound post.

psndSFXOff800d95f8 (now a [complete body](../audio/BATTLE_POSITIONAL_SFX_OFF.md))
skips input−1, selects slot from **low8 handle bits**, and
skips an inactive slot. Pending flag1 clears slot identity/backendhandle to−1
without backend stop (800d9638–50). A started slot selects stop service by its
retained cue table flags; flag20000000 uses SoundEfxStop800e0c38, then clears
slot identity/backendhandle (800d9654–6a8). This leaf does not validate the
input generation against the slot generation. Native handle services should
state their adapter semantics if they implement additional protection.
SoundEfxStop itself routes the retained voice/emitter through source MusyX
key-off/emitter-removal; stopping a WAV by cue name is not this contract.

## Exact original voice bindings

The four voice rows have flag20000000, empty alias, table pan64. They resolve
uniquely in **sound/proj/pmario.proj group5**:

| Cue / original name | FX ID | Object/macro ID | Sample ID | Table volume | Sample frames |
| --- | --- | --- | --- | --- | --- |
| 225 SFX_VOICE_MARIO_RELIEF1 | 705 / 0x2c1 | 831 / 0x33f | 635 / 0x27b | 77 | 19838 |
| 226 SFX_VOICE_MARIO_RELIEF2 | 706 / 0x2c2 | 832 / 0x340 | 636 / 0x27c | 82 | 25923 |
| 228 SFX_VOICE_MARIO_SLEEP1 | 707 / 0x2c3 | 833 / 0x341 | 637 / 0x27d | 69 | 21095 |
| 239 SFX_VOICE_MARIO_YAWN1 | 715 / 0x2cb | 841 / 0x349 | 645 / 0x285 | 87 | 33485 |

Each project FX record specifies priority1, max voices10, velocity127, pan64,
key60. These FX IDs are neither the mot_stay cue indices nor sample IDs.
`pmario.pool` macro records are64bytes at offsets61016/61080/61144/61656.
Each has the same ordered seven commands except its sample operand:

    op56: firstWord00000038, secondWord00107ac0
    op51: firstWord00181833, secondWord00000000
    op16: STARTSAMPLE (the specific sample ID above), secondWord0
    op7:  firstWord01000107, secondWordffff0000
    op17: firstWord00000011, secondWord0
    op49: firstWord00000031, secondWord0
    op0:  firstWord0, secondWord0

The dispatcher is literal **low7bits** opcode at80282be4–c04/table803bbc98.
STARTSAMPLE routes mcmdStartSample8027fc84→dataGetSample→hardware start and
source key-state/update handlers. op51 writes voice bytes1da/1db=24/24 at
802834d4–e4; do not call these envelope indices without the downstream field
consumer recovery. op56 computes voice halfword10e from retained priority110
and nonzero second word00107ac0 at80283568–84; this is another timing/control
dependency, not an ignorable padding record. op7 calls mcmdWait8027f50c:
ffff is the indefinite-wait sentinel, with key/hardware activity admission
branches, not a65535ms delay. Its body calls hardware-active check802907d8
and retains wake/key/end flags/timers. op17 calls hwBreak802909c8. op49 updates
priority110 and calls hwSetPriority80290800. op0 removes voice references and
frees the voice (80282c08–18).

There is no LOOP/GOSUB/explicit SetADSR or envelope opcode in this linear
seven-command stream; this does **not** make the voice envelope/control
runtime an identity. Initial voice/ADSR/control state, key-off/release, volume
control7, pitch, scheduler/sample-completion and DSP backend still determine
the result. SoundEfxPlayEx calls sndFXStartEx and sndFXCtrl(controller7)
(800e0e34–60); source priorities, group availability and voice limits remain
part of actual admission.

All four sample-directory entries are22050Hz DSP ADPCM, loop start/end[0,0]
(no sample loop). Resolve them through `sound/proj/pmario.sdir` and `.samp`;
existing local PCM paths are `local-assets/audio/pcm/samples/pmario-027b.wav`,
`-027c.wav`, `-027d.wav`, `-0285.wav`. These files are sample dependencies,
not approved substitutes for executing the macros/control owners. Counter
repetition of an idle cue is independent of sample looping.

## Cue371 is a literal placeholder dependency

pssfxlist row371 at80318824 is named **SFX_MARIO_FURAFURA1** with flags
20000000 (literal FX0), alias **SE4_FLD_M_FURAFURA1**, volume0, pan64.
The original catalog's FX0 lookup finds project group0/object62 and its
sample56 (22050Hz,1344frames,no loop), because it resolves the literal zero
operand. That macro contains op56,op25,STARTSAMPLE56,WAIT/op49/END. This is
not proof that sample56 is the intended named FURAFURA effect.

The alias string is absent from shipped pmario.etbl, and this bounded recovery
has no demonstrated writer replacing row371's FX ID/volume or alias route.
The actual idle call is retained, but named audible playback remains an open
source dependency. Do not silently map371 to sample56, another damage effect,
a fabricated tone or a Switch remake cue. A literal table consumer must retain
its zero-volume/FX0 state until a demonstrated original writer changes it.

## Validation and remaining services

Run `python3 tools/verify_overworld_mario_idle_sounds.py`. It pins13 containing
bodies, five20-byte cue rows, eight opcode dispatch entries and three handler
spans;102 call/post/argument anchors; exact six bank identities; five literal
FX→macro→sample dependencies and four finite post traces. Sample/command
metadata is re-read from the ISO, not trusted from the generated catalog.
The fifth literal dependency is explicitly the unconfirmed cue371 placeholder.
No payload/PCM is committed and no original playback equivalence is claimed.

Required remaining native services: psSFX queued allocation/deduplication/
priority/global gates and Main cadence; exact scene group5 loading; source
calc3D/listener/position/control updates; MusyX macro scheduler/wait/key/hardware
completion; op51 downstream consumers and op56 timing/age ownership; voice
initialization/ADSR/release, fx control7/pitch/mix; DSP ADPCM backend and stop/
emitter lifetime; any genuine cue371 runtime writer/alias owner. A full generic
MusyX interpreter is broader than this idle request contract. These explicit
dependencies permit queue/cancel binding while audible playback stays unbound.
