# Complete game outer loop and event update request order

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete main800055e0/54,marioStMain8006f904/238 and marioStDisp8006f8dc/28;
complete clock leaf OSGetTick8029d864/8 and seqGetSeq8002e210/8. The called
subsystems/hardware services below are requests,not recovered full implementations
merely because their caller is complete. Verify:
`python3 tools/verify_event_time_publication.py` (nine containing/complete hashes,
247 anchors and SDA setup). Read [retrace producer](EVENT_RETRACE_TIME_PUBLICATION.md),
[physical input](../input/PHYSICAL_INPUT_PRODUCER.md),
[battle cadence](../input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) and
[event manager](../event/EVENT_MANAGER_COMPLETE_LOOP.md).
Static original DOL evidence;no measured frame/pad/retrace rate or gameplay trace.

## main boot,first update and repeating request order

Call marioStInit,then marioStMain ONCE before the repeating loop. Then forever:

1. OSGetTick→retain starting LOW32 time-base value S in r31.
2. DEMOBeforeRender at8002ec1c.
3. marioStDisp:request camDraw8000f454,then set global8041e810(r13+18f0)=0.
   No scalar result. This local display body calls no event manager or input poll.
4. marioStMain,complete body below.
5. OSGetTick→T;reload GP;store GP1314=wrap32(T-S).
6. DEMODoneRender8002eb38.
7. OSGetTick→U;reload GP;store GP1318=wrap32(U-S).
8. Repeat from the first OSGetTick.

OSGetTick is a single lower-time-base read,returning r3. It has no upper-word
retry or conversion. These two GP fields are raw low-word profiling intervals,
not the event GP38/3c timestamp. The same retained S spans rendering/update and
end-render requests;the second measure includes the first interval. No overflow
clamp,frame counter,delay or event “one tick per render” assignment occurs here.
Rendering/VI service internals and actual callback/loop cadence remain external.

## marioStMain readiness barrier and complete request sequence

First test current GP.word10. While nonzero,call OSYieldThread8029ccf4,then
RELOAD GP and test again. No key/game/event subsystem request occurs before this
barrier permits it;zero initially avoids the yield call. This is a hardware/OS
scheduler service boundary,not a busy-spin with a native frame timeout.

Afterward execute ALL direct calls below,in exact order,without local per-call
GP14/system-level/paused tests. Each callee may have its own guards/producers.
Names are US symbol labels;requests do not prove those callee bodies complete.

| Order | Call site | Target / symbol |
| --- | --- | --- |
| 1 | 8006f928 | 80005ff8 / makeKey |
| 2 | 8006f92c | 8002e228 / seqMain |
| 3 | 8006f930 | 800b2508 / cardMain |
| 4 | 8006f934 | 8000a274 / bgMain |
| 5 | 8006f938 | 8002db8c / shadowMain |
| 6 | 8006f93c | 8004281c / animMain |
| 7 | 8006f940 | 800f904c / battleMain |
| 8 | 8006f944 | 8000f704 / camMain |
| 9 | 8006f948 | 8007f9ec / windowMain |
| 10 | 8006f94c | 80026bb8 / mapMain |
| 11 | 8006f950 | 80018264 / hitMain |
| 12 | 8006f954 | 800368d0 / evtmgrMain |
| 13 | 8006f958 | 80024b08 / mapDisp |
| 14 | 8006f95c | 8005a7e0 / marioMain |
| 15 | 8006f960 | 8005f968 / mobjMain |
| 16 | 8006f964 | 80049a7c / npcMain |
| 17 | 8006f968 | 80012058 / fadeMain |
| 18 | 8006f96c | 800714c0 / imgMain |
| 19 | 8006f970 | 8001c028 / lightMain |
| 20 | 8006f974 | 800730ac / offscreenMain |
| 21 | 8006f978 | 800adfe0 / itemMain |
| 22 | 8006f97c | 8006e5ec / caseMain |
| 23 | 8006f980 | 8001b2d8 / iconMain |
| 24 | 8006f984 | 800d67cc / extMain |
| 25 | 8006f988 | 800dc48c / psndMain |
| 26 | 8006f98c | 8017df24 / winMain |
| 27 | 8006f990 | 8013d654 / statusWinMain |
| 28 | 8006f994 | 8019dad0 / envMain |
| 29 | 8006f998 | 80200af0 / countDownMain |
| 30 | 8006f99c | 8020940c / nameEntMain |
| 31 | 8006f9a0 | 8023f1b0 / winMgrMain |
| 32 | 8006f9a4 | 8005ea7c / effMain |
| 33 | 8006f9a8 | 8006edbc / L_gcResetCheck |

makeKey precedes battleMain and evtmgrMain;seqMain also precedes both. Battle
update request occurs BEFORE the event manager;mapDisp/marioMain/mobjMain/npcMain
and later world/pose/effect/window requests follow it. Do not reorder these
requests to render phases or poll input inside every event-manager credit merely
because a host controller makes that convenient. Original callee scheduling,
input availability and asynchronous retrace barriers still require integration
and execution evidence. This complete body has no direct GP timestamp publication.

## Complete idle-input/time tail

After the33 requests,reload GP and test GP.word0 bit0x1000. If CLEAR,return
immediately,without idle baseline read/write or elapsed-time query. If SET,
scan four channels in ascending order,implemented as two unrolled pairs.
Channel i qualifies as inactive only if ALL are zero:

- GP.word(1378+4*i),then GP.word(1328+4*i).
- GP.bytes13b8+i,13bc+i,13c0+i,13c4+i (signed zero tests),then13c8+i,13cc+i
  (unsigned zero tests).

Stop the scan at the FIRST nonzero field. Increment qualifying-channel count
only after all eight tests for that channel pass. No deadzone or duration
tolerance in this tail. Names/producers for these fields are in physical input
contracts;raw conditions must not be replaced with “no pressed buttons” only.

If fewer than4 channels qualify,call OSGetTime;store returned LOW to global
8041e94c(r13+1a2c),then HIGH to8041e948(r13+1a28). Return. If all4 qualify,
call OSGetTime to T;load prior low/high from these globals,subtract with wrapping
64-bit arithmetic,read clock800000f8 and divide SIGNED64 delta by denominator
(clock>>2),high denominator0,through __div2i80264ad8. No negative clamp,
zero-rate guard or timestamp re-sampling. Compare returned signed64 quotient
STRICTLY>120. If not greater,return;exact120 does not request a flag change.

For quotient>120,call seqGetSeq and compare exact2. If unequal,call seqGetSeq
AGAIN and compare exact4. If either comparison matches,reload GP,OR bit0x2000
into word0,and store. Otherwise return without mutation. seqGetSeq merely loads
word80415190(r13-7d90);it does not capture GP or normalize sequence IDs. Two
queries can observe different values. No idle-baseline update on this all-
inactive path. The flag's display/audio/reset consumers remain separate sources;
this local request does not prove an observed screensaver or plot transition.

The120 threshold uses raw hardware-rate conversion here,not the event-clock
bank and not120 event simulation/render ticks. Platform clock/overflow/sequence/
flag validity and measured inactivity behavior remain open. The idle baseline
is untouched when the enabling bit is clear;initial-global/writer provenance
cannot be inferred reset from this body alone.

Acceptance:initial StMain before loop;retained profile start and two raw low
measurements;display camDraw/flag-clear order;GP10 barrier/GP reload/no timeout;
all33 calls and battle-before-event ordering;idle enable gate,first-nonzero scan,
all channel fields/count,fewer-than4 baseline refresh,all4 no baseline update,
signed quotient strict120 and sequence2 versus second-read4;OR-preserved flags.
Callee/VI/thread/pad/scene/context/clock/idle/numeric domains and original update/
pause/idle/multicredit traces remain EVT-01 and system integration requirements.


## GP10 producer follow-up (2026-10-12)

[Disc-thread readiness](EVENT_DISC_THREAD_READINESS_GATE.md),
[system-message presentation](DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md) and
[audio cover-state requests](../audio/AUDIO_DISC_COVER_STATE_REQUESTS.md) now close this
GP10 writer path,13-status routing,retrace counts/card override,scoped thread
startup and alternate error-mode input requests. DVD/VI/thread/ROM-font/render/
audio/source/other-writer domains and original pause/recovery traces remain open.
