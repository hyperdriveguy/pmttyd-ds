# MusyX DSP initialization and studio/voice activation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salInitDspCtrl80288104/3b4,
salActivateStudio802884f0/150, salInitHRTFBuffer802884b8/38,
salActivateVoice8028b43c/a0, hwStart80290c48/3c and hwKeyOff80290c84/28hex.
Addresses/offsets/sizes/masks hexadecimal; counts/shifts decimal. Static local
allocation/publication/list order, not measured DSP initialization/audio.
Read [bootstrap](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md),
[DSP envelope/deactivation](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md),
[job start/key-off caller](MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md)
and [hardware break/query](MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md).

## Complete initializer allocation prefix and partial failures

salInitDspCtrl(N,T,F) retains raw F, stores low8(N) to salNumVoices8041efba,
then low8(T) to salMaxStudioNum8041efb9. Request aramGetZeroBuffer and store
return into dspARAMZeroBuffer8041ef50. Then, in order:

| Request | Published global | Null result |
| --- | --- | --- |
| salMallocPhysical(800) | dspCmdList8041ef70 | Return0 |
| salMallocPhysical(280) | dspSurround8041ef80 | Return0 |
| salMalloc(f8*fresh voice byte) | dspVoice8041ef88 | Return0 |
| salMallocPhysical(40*fresh voice byte) | dspITDBuffer8041ef84 | Return0 |

Publication precedes each null test. After surround allocation, request
memset(return,0,280), reload surround pointer and DCInvalidateRange(ptr,280).
After ITD allocation, request DCFlushRange(return,40*fresh voice byte).
Do not substitute a zero fill of ITD memory: none is requested here. Capture
ITD pointer into a local cursor after the flush. Count reads for allocation,
flush and later loop are independent. Zero count still requests zero-sized
voice/ITD allocations; null results follow the same failure paths.
All explicit failures return0 without local rollback/free/count restoration.
No singleton/reinit/null-global/IRQ/callback ownership check is present.

## Complete per-voice initialization loop

Initialize ordinal i=0 and byte offset off=0; before EVERY iteration compare
ordinal against fresh salNumVoices byte. Increment ordinal by1/off byf8 after
body. Under an unchanged count, exactly that count is initialized. Global
count/base mutation across children is not hidden by a captured bound/base.
For each record at fresh dspVoice+off, stores occur in this order:

1. bytef0=0, bytef1=0, bytef2=0, bytese8/e9/ea/eb=ff; each store reloads base.
2. Request salMallocPhysical(c4); publish return to record.word0; reload that
   pointer and request memset(ptr,0,c4). No null allocation test precedes use.
3. Request salMallocPhysical(80); publish to record.word4. No null test or
   zero-fill request for this command buffer.
4. Reload PB=record.word0 for each binding: PB.half4=high16(PB), half6=low16(PB);
   PB.half4e/50=high/low of fresh record.word4; PB.half38/3a=high/low of captured
   ITD cursor. Store cursor to record.word8; advance cursor by40; wordec=ffffffff.
5. Reload PB and request DCFlushRangeNoSync(PB,c4).
6. Store record.words24/28/2c/30/34=0 in that order, with fresh base loads.

There is no whole-record zero fill, envelope setup, link initialization or
bytef3/studio write in this loop. Unmentioned fields remain uninitialized by
this body. Index/base/allocator/address/alias/cache/partial-failure domains must
come from callers and children; a null unchecked per-voice allocation cannot
be reclassified as a clean return0 from this function.

## Complete studio allocation loop and final requests

Initialize studio ordinal0 and pointer at dspStudio80407c68. Compare ordinal
against fresh salMaxStudioNum byte on every loop test; advance pointer bybc.
Original static studio storage is5e0 bytes, eight bc-sized records; this local
loop does not enforce that extent. At each record S:

- byte50=0; request salMallocPhysical(36), publish S.word0; null returns0.
- Request salMallocPhysical(3c00), publish S.word28; null returns0.
- Reload S.word28; memset(ptr,0,3c00), then reload and DCStoreRangeNoSync(ptr,3c00).
- Set word2c=fresh28+780, word30=fresh2c+780, word34=fresh30+780, word38=fresh34+780,
  word3c=fresh38+780, word40=fresh3c+780, word44=fresh40+780, all wrapping32.
- Reload S.word0; memset(ptr,0,36). Clear words in order c,8,4,18,14,10,24,20,1c.
  Reload S.word0; DCStoreRangeNoSync(ptr,36).

Eight780-sized slices fit one3c00 block. Root words48/4c are not cleared by
this allocation loop; studio activation below initializes those for its selected
record. Failure can leave earlier studios/voices allocated and globals published.

After the loop, ALWAYS request salActivateStudio(0,1,F!=0), using the exact
neg/or/sign-bit nonzero conversion of raw32 F. No T>0 gate or child-result test.
Then salMallocPhysical(100), publish dspHrtfHistoryBuffer8041ef7c; null returns0. Nonnull:
request salInitHRTFBuffer, return1. Thus studio0 activation precedes even the
last possible explicit allocation failure; zero studios does not skip it.

## Complete studio activation and HRTF initialization

salActivateStudio(I,B,W) selects S=dspStudio+bc*low8(I). Retain B/W. In order:
memset(fresh S.word28,0,3c00); reload pointer, DCStoreRangeNoSync(ptr,3c00);
memset(fresh S.word0,0,36); clear S.words c,8,4,18,14,10,24,20,1c;
reload S.word0, DCStoreRangeNoSync(ptr,36); memset(fresh S.word30,0,780),
reload pointer/DCStoreRangeNoSync(ptr,780); repeat for fresh S.word3c.
Finally clear S.word48 and4c; byte50=1; byte51=low8(B); byte52=0;
word54=raw W; wordb0=0; wordac=0. No bounds/null/allocated/active guard,
voice traversal/deactivation/free, preservation of old roots or scalar status
assignment. Re-activation can replace live roots; actual admissible callers
and membership ownership remain open.

salInitHRTFBuffer: read dspHrtfHistoryBuffer8041ef7c, request memset(ptr,0,100), reload
pointer, request DCStoreRangeNoSync(ptr,100); return without independent status.
No allocation/null guard. A child mutation can change the second request's pointer.

## Complete voice activation and hardware start/key-off

salActivateVoice(V,studio) retains both inputs. If V.bytef0 nonzero, request
salDeactivateVoice(V), then freshly OR20 into V.word24. Initially inactive
records skip BOTH deactivation and this OR; an existing20 bit is not cleared.
Compute studio root address80407c68+bc*low8(studio)+48. Clear V.bytef1; capture
root into V.wordc. If captured root nonnull, RELOAD V.wordc and store V to its
word10. Then V.word10=0; root=V; V.bytef2=0; V.bytef0=1; bytef3=low8(studio).
Root/link writes precede active/studio publication. No envelope reset, memory
allocation, list-member/count/studio/null validation or explicit status result.
The linked deactivation child is closed in the DSP envelope contract.

hwStart(I,studio): compute raw wrap32(I*f8). Capture dspVoice, read
salTimeOffset8041efb8; store that byte to selected V.byteda. RELOAD dspVoice
and form record from the retained offset; call salActivateVoice(record,raw studio).
The activation record can differ from the time-byte record if base changes;
studio is not truncated by this wrapper before passing it. No ordinal/time-slot
bounds, system/active/admission check or direct DSP start command.

hwKeyOff(I): compute raw wrap32(I*f8), read dspVoice and salTimeOffset byte,
form A=base+offset+4*byte. Store A.word24 OR40. No active query, direct release
call, flags reset, IRQ control or hardware completion wait. Time byte is not
clamped to0..4 here. The [DSP envelope loop](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md)
consumes indexed40; setting this bit alone does not prove playback ended.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_dsp_initialization_and_activation_evidence.json)
pins six complete bodies and nine linked children:15 spans,305 anchors/33 calls.
Six count/size, six activation/root and four indexed-event formula vectors,
eight studio slice offsets and seven explicit failure-stage cases are source
deductions, not original execution or allocation-failure experiments. Fresh full
ELFs/listings remain `/tmp`; no original bodies/binary assets committed.

Remaining: allocator/ARAM/cache transitive behavior, full DSP builder/consumer,
count/base/time/studio/record/link/descriptor providers, valid static extent/
index/null/value/alias/partial-init/reinit/lifetime/IRQ/concurrency domains and
original init/allocation/start/key-off/DSP/audio traces. Six local bodies are
answered; full native/runtime binding and EVT-01 remain OPEN.

## 2026-10-13 — Pitch/volume/ramp follow-up

[Pitch/volume/DSP ramps](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md) closes hardware duplicate/dirty/event
publication and two local DSP ramp helpers. Actual mixer internals/ramp callers/
consumers/value/FP/alias domains and original audio remain open.

## 2026-10-13 — Studio depop prepass follow-up

[Depop/fade/prepass](MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md) closes two local depop helpers and the builder's
studio two-list prepass. Captured/fresh successor ordering and callback/root
publication retained; list providers/domains/original DSP/audio remain open.
