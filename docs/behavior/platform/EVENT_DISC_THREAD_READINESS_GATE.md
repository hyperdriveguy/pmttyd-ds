# Complete disc-thread GP10 readiness and event-time freeze producer

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete gcDvdCheckThread8006f1e0/224,cardIsExec800b2b8c/50 and
VIGetRetraceCount802a4e40/8;scoped thread creation/resume in
marioStInit8006fb3c/348. Read
[clock consumer](EVENT_RETRACE_TIME_PUBLICATION.md),
[outer-loop consumer](GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md),
[error presentation](DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md) and
[audio cover state](../audio/AUDIO_DISC_COVER_STATE_REQUESTS.md).
Verify:`python3 tools/verify_event_readiness_gate.py`:eleven full/containing
hashes,184 anchors,13 data spans and four original system-message hashes.
Static original DOL evidence;no original disc/OS/thread/interruption observation.
DVDGetDriveStatus/hardware,OS thread scheduler and renderer remain platform
service boundaries,not complete implementations in this contract.

## Thread startup and retained state

At8006fd5c..9c marioStInit requests OSCreateThread with object803da900,
entry8006f1e0,argument0,stack top803da900,stack length1000 (4096 bytes),
priority16,attribute word1. It ignores the creation result,stores global
8041e938(r13+1a18)=1,then requests OSResumeThread(same object). No local failure
branch or retry;attribute interpretation/thread runtime is SDK/platform policy.
Registration occurs after retrace callback and ROM-font initialization requests,
before most subsystem/event initialization calls later in marioStInit.

The complete thread has no normal return. Initialize retained previous status
S=0,retrace-change count N=0,previous retrace R=0,and message/state word V=0.
At EACH iteration perform the ordered phases below. V is deliberately called a
WORD:rendering replaces it with a pointer,not just an enum. No per-iteration
reset of these retained registers occurs.

## Phase1:first status query and exact dispatch

Query DVDGetDriveStatus atf200 to A. Compute wrap32(A+1),unsigned-compare<=12.
In-range dispatch through13 words8030c320 indexed by that value. Out of range
uses the clear-gate branch. All GP writes reload GP pointer80415fd0.

| A / table target | Explicit GP10 write | V write |
| --- | --- | --- |
| -1 /8006f278 | 1 | 13 |
| 0 /8006f28c | 0 | Preserve |
| 1 /8006f298 | NO WRITE | Preserve |
| 2 /8006f28c | 0 | Preserve |
| 3 /8006f28c | 0 | Preserve |
| 4 /8006f23c | 1 | 11 |
| 5 /8006f228 | 1 | 10 |
| 6 /8006f250 | 1 | 11 |
| 7 /8006f28c | 0 | Preserve |
| 8 /8006f28c | 0 | Preserve |
| 9 /8006f28c | 0 | Preserve |
| 10 /8006f28c | 0 | Preserve |
| 11 /8006f264 | 1 | 12 |
| Every other signed word | 0 | Preserve |

Numeric SDK status is authoritative here;do not infer a one-to-one boolean
“disc present” mapping. In particular status1 preserves prior GP10 rather than
clearing or setting it. The clear branches also preserve V. Message selector
may therefore outlive a status transition. Naming/classification of platform
errors and supported native media-status adapter remain separate requirements.

## Phase2:second/third status queries and retrace count

Query DVDGetDriveStatus AGAIN to B and compare signed equality to retained S.
If DIFFERENT,query a THIRD time to C,set N=0,set S=C,then skip retrace counting.
V/GP10 still reflect phase1 A;no recomputation from B or C. A,B,C may differ
within one iteration. This is not a single snapshotted drive status.

If B==S,query VIGetRetraceCount to R1,unsigned-compare equality to retained R.
If unequal,query it AGAIN to R2,increment N with word wrap,store R=R2;then if
signed N>100,set N=100. If equal,leave N/R untouched. VIGetRetraceCount merely
loads global8041f1dc=r13+22bc,with no clock conversion or hardware polling in
this leaf. The count increments ONCE per observed change,not by R2-R. Missed
retraces are not retroactively counted. Status-change branch does not refresh R.
No measured rate,wall-clock duration or exact number of actual retraces follows
from N alone. For ordinary retained N,it saturates at100;malformed word-wrap
states are not locally clamped below0.

## Phase3:audio request,card override and display admission

Reload GP10. Nonzero→SoundOpenCover;zero→SoundCloseCover. This occurs BEFORE
card override and uses live GP10,not A's status categorization. Audio helpers
have their own state/service guards documented in companion contract.

Reload GP10 again. If nonzero,call cardIsExec. A nonzero return reloads GP and
stores GP10=0. This iteration does not issue SoundCloseCover again after that
clear. If original GP10 was0,do not query cardIsExec. The helper checks pointer
8041eab8=r13+1b98:null→0;otherwise U16 flags+0 bit2→1;else bit1→1;else
word+ec nonzero→1,zero→0. It reads flags again for its second bit check. No
other card state,disc query,message handler or clock test in this leaf.

Reload GP10 AGAIN. Admit error-render path only when nonzero AND signed N>10.
Exact N10 does not render;N11 does. Otherwise go directly to end-of-iteration
yield. No branch erases V when not rendering or resets N merely because card
cleared the gate. The presentation contract specifies admitted rendering and
its extra input/reset requests. At EVERY iteration end request OSYieldThread,
then loop to phase1. No sleep duration/deadline/normal exit/timeout.

## Event/input/clock implications

marioStMain reloads and waits/yields while GP10!=0 BEFORE makeKey and all33 game
subsystem requests. Retrace callback still runs independently;GP10 skips its
admitted clocks,including GP38/3c event time,while ALWAYS updating raw baseline.
The manager consequently neither receives a new normal update request nor
accumulated catch-up ticks from that frozen interval. OS/VI concurrency and
actual thread ordering/clock samples must be validated in original traces.

The admitted error-render path calls makeKey after DEMODoneRender,then
L_gcResetCheck. Thus physical input polling can occur through this thread while
normal game update remains gated. Do not infer “no pads sampled during freeze,”
or connect one key poll to every event credit. Only local request paths are
proved;OS scheduler/drive/VI/pad callback execution is still an external binding.

V is replaced by romFontGetMessage's returned pointer during rendering. A later
status1 preserves that word and gate;the next admitted render can pass retained
pointer bits back as a selector. The code has NO selector restoration or range
check. Source/platform status-transition validity and original long-status1
traces remain requirements;do not silently invent resetting V to its last enum.

Acceptance:all13 table entries/out-of-range wrap,status1 gate preservation,
retained V,A/B/C query ordering,status change skips/retrace two queries/count
once/cap100;audio before card override,card null/flags/live word,gate reloads,
N10/11 display boundary,yield every path,barrier/frozen baseline/input alternate
path,thread creation parameters/no failure handling. Other GP10 writers/aliases,
OS/VI/DVD/card/audio/render/source domains and original interruption/recovery/
pause/input traces remain EVT-01 work. This is a disc readiness producer,not a
complete policy for every story/menu pause.
