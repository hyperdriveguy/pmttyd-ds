# Stylish timing: start, polling, input boundaries and results

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Static complete btlevtcmd_ACRStart80106dfc/b8, ACRGetResult80106d7c/80,
BattleAcrobatStart80192fbc/28, Main80192db4/208 and GetResult80192d04/18.
Read [presentation](BATTLE_STYLISH_TIMING_ICON.md),
[Jump request graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) and
[pad cadence](../input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md). Verification:
`python3 tools/verify_stylish_timing.py`. No measured timing/original trace claim.

## Shared record and start

The record belongs to BattleWork W, independently of TimingA's AC work:

| W offset | Role |
| --- | --- |
| +0x271c | Signed word invocation counter t |
| +0x2720 | Signed word lower boundary L |
| +0x2724 | Signed word upper boundary H |
| +0x2728 | Signed word no-input terminal boundary E |
| +0x272c | Translated actor registry ID |
| +0x2730 | Signed result word |
| +0x2734 | Signed history-scan count P |

BattleAcrobatStart(W,actor,L,H,E,P) stores t=0,L,H,E,actor,result=0,P,
in that order, and returns0. It does not validate/reorder boundaries, initialize
pad histories, install a manager callback, allocate per-actor records or invoke
Main. Concurrent starts overwrite this same shared work.

ACRStart event callback captures its operand pointer and shared W. On its first
invocation (nonzero first-call argument), resolve actor via evtGetValue then
BattleTransID, then resolve L,H,E,P in order through evtGetValue, and call
BattleAcrobatStart with captured W. It then calls BattleAcrobatMain(W) in the
SAME invocation and returns that result unchanged. Later invocations skip ALL
operand resolutions and start stores and call Main once with the then-captured
shared W. Thus t first becomes1 before the first callback return. Event locals
changing afterward do not change retained boundaries.

USER_FUNC opcode91 dispatch0x800326fc resolves/caches its function on first
entry and calls it with first-call1; retained calls use0. Return0 preserves the
current command and yields; return1 completes/clears current opcode and yields;
return2 completes and continues dispatch in the same invocation. Relevant
sites80032710..80032768 and80032a1c..80032a44 are pinned separately. This is
invocation cadence, not rendering frames. Original manager credit/pause/schedule
composition remains an EVT-01 dependency.

## Complete ordered polling algorithm

Each BattleAcrobatMain(W) first requests BattleGetUnitPtr(W,actor), then
increments t=wrap32(t+1), even if actor is null. There is no result-terminal
gate at entry: invoking it again after completion increments and reevaluates.

1. Null actor: write result1, return1. Otherwise actor.flags27c&0x10 does the
   same, before input or icon work. There is no HP/status query here.
2. If W.word163f4&4 AND signed L<t<=H, queue the two
   [icon presentation requests](BATTLE_STYLISH_TIMING_ICON.md). This happens
   BEFORE input rejection/outcome; it can also happen on a successful call.
3. Set local autoInput to1 exactly when U8(actor+0x307)!=0 AND L<t<=H;
   otherwise0. This byte is only an eligibility source here, not initialized by
   this body. No fabricated equipment label is established by this reader.
4. If signed t<=1 AND P>0, scan indices i=0..P-1 in ascending order. Call
   BattlePadCheckRecordTrigger(i,0x100); first nonzero writes result-1 and
   returns1. P is reread at every loop condition. No bound clamp/history reset.
5. Call BattlePadCheckTrigger(0x100) unconditionally on the remaining route.
   If return is nonzero OR autoInput!=0, evaluate signed boundaries in order:
   t<=L writes result-1; otherwise t<=H writes result2; otherwise result-2.
   Each returns1. AutoInput does not bypass the history rejection above.
6. With no current/automatic input: if signed t>=E, write result1 and return1.
   Otherwise return0, preserving result (normally0 after start).

Each boundary read occurs where shown, after intervening service callbacks.
The icon uses the first boundary reads; auto eligibility rereads them; actual
outcome rereads them again. Do not snapshot all fields up front or assume
callbacks cannot mutate shared work. History scanning precedes the current
trigger query, so a current edge already present at history index0 can cause
an initial rejection when P>0, including when current t is in the window.

## Input storage and result readers

The two pad helpers select GENERAL channel0, not private AC defense history.
BattlePadCheckRecordTrigger(i,mask) routes to MultiCheckRecordTrigger(0,i,mask),
which reloads shared W and returns word(W+0xf44+4*i)&mask. Current Trigger
routes to MultiCheckTrigger(0,mask), which reloads shared W and reads W+0xf44.
Neither normalizes to bool nor validates the index. Mask0x100 is the A virtual
button; ownership/update/physical publication follow the linked pad documents.
The Main record's captured W and pad helper's freshly loaded W need not be the
same pointer under mutating adapters; the original local bodies assume no
explicit identity check.

BattleAcrobatGetResult(W,&result,&counter) reads W2730, writes output0, THEN
reads W271c and writes output1; return2. It does not run Main, clear state or
stop/cancel anything. The event wrapper first captures both RAW destinations,
then calls this leaf into stack temporaries. Write result destination first,
counter destination second through evtSetValue, independently skipping exact
raw sentinel-250000000 (0xf1194d80). No evtGetValue resolves destinations.
Both values have been captured before either event output write; aliased event
outputs end with counter. The leaf's direct pointer aliases instead follow
its read/store/read/store sequence.

## Ordinary Jump bindings and implementation cases

Jump's later-hit brother0x8035632c and return brother0x8035696c both WAIT3,
calculate lower=count/2-10, upper=count/2+2, E=count-4 with signed VM division,
then call ACRStart(-2,lower,upper,E,0) at0x803563a4/0x803569e4,
followed by ACRGetResult(L6,L7) at0x803563c0/0x80356a00.
The later-hit count is MarioJumpParam's UPDATED parent value copied into the
brother;36 only gives8,20,32 when distance adjustment leaves36. The return
count32 gives6,18,28. Both P=0 skip history scan; first-call t=1 still queries
the current trigger. Result CASE>=2 selects the pose/audience/voice branch;
negative outcomes and1 follow the notry branch in those actual streams.
Only2 is a locally produced success. The brother's result local6 does not
replace the parent's count6. No audience service runs in Main itself.

Acceptance cases: first invocation t1; L and H themselves; no-input t=E;
input after E preceding timeout; lower>upper; t word wrap; null actor counter
advance; actor flag0x10 bypass; positive history P with first-match short circuit;
autoInput versus history rejection; icon request before rejection; result0
preservation while waiting; repeated Main after terminal; output sentinel and
alias ownership; current general pad versus private defense pad. Original
source writers, record collision/lifecycle policy, complete scheduler, audience
feedback and execution traces remain open. This contract supplies local services
without inventing a fixed attack impact or stylish rendering-frame count.
