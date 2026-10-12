# Event timestamp producer: retrace time-base accumulation and freeze gates

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete clock-publication prefix8006f404..8006f60c within
viPostCallback8006f404/3b8;complete OSGetTime8029d84c/18 and
winCheck8017eb24/4c;scoped clock/reset/registration regions of
marioStInit8006fb3c/348. Callback's later reset-button/rumble/audio handlers and
platform VI/hardware bodies are separate requirements. Containing fingerprints
do not claim those unreviewed regions are complete. Verify:
`python3 tools/verify_event_time_publication.py`.
Read [manager](EVENT_MANAGER_COMPLETE_LOOP.md),
[init/reset](EVENT_CONTEXT_INITIALIZATION_AND_RESET.md),
[system-level masks](EVENT_SYSTEM_LEVEL_AND_SPEED.md) and
[outer loop](GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md).
Static original DOL evidence;no measured retrace frequency or execution trace.

## Raw time-base reader and bootstrap publication

OSGetTime reads upper time-base into r3,lower time-base into r4,upper again
into r5. If the two upper words differ,retry from the first read;otherwise
return r3/r4. This supplies a rollover-consistent raw64-bit time-base value.
It adds no seconds/milliseconds conversion,offset,pause adjustment or clock-
frequency query. Hardware/time-base behavior and original frequency are
platform requirements,not established by this reader alone.

marioStInit bulk-clears GP's0x13d8 bytes at8006fbdc..fbe8. Scoped clock setup
then clears64-bit banks20,28,40,38,60,58,50,48. It calls OSGetTime once and
stores raw LOW to GP34 then HIGH to GP30. Each bank stores high at the smaller
offset,low at+4. GP38 begins0,not the absolute sampled time. Further setup calls
VIGetTvFormat at8006fce4;exact return0 stores GP4=60,EVERY nonzero stores50.
This is a configured refresh WORD assignment,not measured frame/retrace/input
cadence. It clears GP18=0 atfd20. Atfd44..54 it constructs callback address
8006f404 and calls VISetPostRetraceCallback with it. VI registration/interrupt
implementation is external to these scoped instructions.

The event init called later in marioStInit snapshots GP38/3c,as the separate
context contract specifies. Clock callback registration precedes that call;
no local claim of identical field/battle snapshots or disabled interrupts follows.
The bus-clock word800000f8 used below is ORIGINAL low-memory hardware metadata,
not GP.f8 (which holds other initialization data). Frequency/domain/bootstrap
writers for800000f8 remain external requirements.

## Raw delta and total20 bank

Capture callback argument R in a retained register. Call OSGetTime to retained
sample T=(high,low). Load CURRENT GP pointer80415fd0;read previous raw LOW34
then HIGH30;calculate wrapping64-bit delta D=T-previous. Store GP.delta LOW2c
then HIGH28. No signed-negative clamp or large-gap substitution in this prefix.
Reload GP;add freshly read delta28/2c into GP.total20/24 with carry,storing
LOW24 then HIGH20. Reload GP and read clock800000f8;compute U=clock>>2 and
full unsigned64 product L=U*0x157529ff. Compare total as SIGNED64 against L;
if total>L,replace total LOW24 then HIGH20 with L. Equality stays unchanged.
This cap applies only to total20;it does not clamp D or other banks.

GP is reread between groups. No snapshot/stability/equality test,context select,
callback count increment or event-array access occurs. Retained T and callback
argument R survive calls;later bank increments use LIVE GP.delta fields rather
than an immutable private D. Word arithmetic/carry and low-before-high stores
are part of the original contract. Original GP alias/interrupt/publication
atomicity domains must be respected rather than assuming every64-bit read is
an atomic host snapshot.

## Freeze gates and bank additions

After total20 handling,reload GP and read word10. If nonzero,skip ALL conditional
bank increments and jump to raw-baseline publication. Otherwise call winCheck.
If its result!=0,likewise skip all conditional increments. Only when both gates
permit,perform the following in listed order. For EACH row,reload GP;read mask
where applicable;on admission read existing bank and live delta,add64 with
carry,store LOW then HIGH. No saturation or negative clamp for these additions.

| Bank high/low | Admission,after outer gates pass |
| --- | --- |
| 60/64 | GP18 bit8 CLEAR |
| 58/5c | GP18 bit4 CLEAR |
| 50/54 | GP18 bit2 CLEAR |
| 48/4c | GP18 bit1 CLEAR |
| 38/3c (event manager source) | ALWAYS;no GP18/type/system-mask test |
| 40/44 | Current GP14==0 (field context) |

Thus system-level low bits gate four separate banks but do NOT directly freeze
GP38 event time. Manager eligibility/Stop masks separately control which event
lifetimes actually receive that time. Battle context suppresses40/44 only;it
still advances38/3c when outer gates pass. Names for other bank consumers and
all mask/context writers remain separate source obligations.

## Baseline publication is unconditional

Both normal and frozen paths converge at8006f5fc. Reload GP;store retained raw
sample T LOW to34 then HIGH30. Reload GP AGAIN;store retained callback argument
R to GP1c. Callback then enters its separate reset/platform tail. That tail's
calls/effects are not inferred by this clock-prefix contract.

Important consequence:GP10/window freeze does not let raw delta accumulate for
later event-clock catch-up. Every callback updates the raw baseline even when
GP38 is unchanged. On the next admitted callback,GP38 receives only that
callback's new raw delta. Raw total20 still advances while gated. The event
manager therefore consumes accumulated admitted raw ticks,not absolute
OSGetTime or a counter of rendered frames. Its own per-entry eligibility and
large-gap rules continue to apply independently.

## Complete winCheck admission helper

Load window pointer from8041ec30=r13+1d10. Null→return0. Otherwise read its
UNSIGNED16 flags at+0;if bit0x1000 set→return0. Otherwise read SIGNED word
state+20:exact0→return0;state>=200→return0;every other value (including negative)
returns1. No other window scan,clock/pad read or pointer validation.

It is a specific pointer/flag/state predicate,not “any visible UI freezes time”
or “all nonzero window states freeze time.” Window-pointer/state/flag writers
and the complete window manager are separate integration sources. The current
callback uses nonzero result as its outer freeze gate;source naming alone does
not establish its plot/menu/display semantics.

Acceptance:upper rollover retry;bootstrap zero banks/raw sample/refresh0 versus
nonzero/registration order;raw delta wrap and total signed cap;GP reread/live
bank loads/LOW-before-HIGH publication;GP10 short circuit before winCheck;window
null/flag/state0/negative/199/200;four independent low-mask banks,event38 ungated
by mask,field40 versus battle;unconditional baseline/R publication while frozen,
no later catch-up. Clock/GP/interrupt/scene/pause/window/numeric/source domains,
callback reset/audio/rumble tail and original cadence/freeze traces remain open.


## GP10 producer follow-up (2026-10-12)

[Disc-thread readiness](EVENT_DISC_THREAD_READINESS_GATE.md),
[system-message presentation](DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md) and
[audio cover-state requests](AUDIO_DISC_COVER_STATE_REQUESTS.md) now close this
GP10 writer path,13-status routing,retrace counts/card override,scoped thread
startup and alternate error-mode input requests. DVD/VI/thread/ROM-font/render/
audio/source/other-writer domains and original pause/recovery traces remain open.
