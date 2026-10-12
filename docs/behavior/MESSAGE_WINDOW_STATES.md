# General and clear message window state machines

US G8ME01 revision 0, recovered 2026-10-12. B evidence means static original
instructions/data, not executed timing. msgWindow_Main (0x80080c20,size 0x804)
and msgWindow_Clear_Main (0x8008093c,size 0x2e4) are separate contracts.
See [lifecycle](MESSAGE_WINDOW_LIFECYCLE.md), [playback](MESSAGE_PLAYBACK.md),
[selection](MESSAGE_SELECTION.md) and [parser](MESSAGE_PARSER_RECORDS.md).
Story text/order/conditions remain original data; no dialogue is invented here.

## Shared fields and clock arithmetic (B)

Both receive W pointer, use W.handle[0x28] to obtain D and assume valid work.
W.u16[0] is state, W.u16[2] flags, W.s16[4] display layer, W.s16[6] opacity-like
value, W.word[8] type and W.words[0x20/0x24] high/low animation timestamp.
D has playback wait at halfword a, history cursor at word f03c, history count
f040, history words f044 onward, current scroll at word 28, and talk countdown/
latch at words f250/f254. These offsets have different roles from parser fields.

Clock conversion subtracts animation timestamp from current global clock +38/
3c and uses the same bus-clock-derived denominator and __div2i route as
[playback](MESSAGE_PLAYBACK.md). Call the resulting signed 64-bit quotient m.
Fade quotient is signed divide of the modulo-64-bit product m*255 by 200.
No floating fade interpolation occurs. Opening stores the low halfword of this
quotient, then signed-clamps only when that stored value>255. Closing stores
low halfword of 255-quotientLowWord and signed-clamps to zero when <=0.
Overflow/narrowing and helper corner behavior are not arbitrary-precision math.
The divisor 200 is literal; no fade duration was measured in original execution.

Drawing paths register dispEntry(8,0,400.0-float(W.s16[4]),
msgWindow_Disp at 0x80080260,W). No new display allocation/ownership guarantee
is inferred. Physical fade appearance and manager invocation cadence remain open.

## General pre-dispatch talk countdown (B)

Before every state dispatch, cached D.f250 is checked. If nonzero and D.f254=0,
set talking pose: if D.flags bit 8 call BtlUnit_ChangeTalkAnim(D.word[f224]) at
0x801275ac, else npcSetTalkPose(D+f204) at 0x8004979c. Set latch f254=1.
Decrement f250 by one word. If it becomes zero, restore stay pose using freshly
loaded handle data: BtlUnit_ChangeStayAnim(D.f224) for flags bit 8, otherwise
npcSetStayPose(D+f204). Clear cached D.f254=0.

This countdown advances once per general-main invocation, including opening,
closing and history states. Any nonzero word enters it; no positive-only guard.
Generic playback records can set f250=15; that update occurs later than this
pre-dispatch decrement, so its first talk transition is on a subsequent call.
Clear and selection main callbacks do not run this pre-dispatch countdown.
Pose helper effects, handle relocation and talk-state producers remain open.

## General dispatch and window sounds (B)

The 12-entry table at 0x8030f5d0 selects:

| W.state | Target | Local phase |
| --- | --- | --- |
| 0,2,4 | 800813c0 | Draw only after countdown |
| 1 | 80080ef4 | Playback and result transition |
| 3 | 80080fac | Key wait / history entry |
| 5 | 80080d04 | Opening sound/timestamp then fall through to fade |
| 6 | 80080d50 | Opening fade |
| 7 | 80080e08 | Closing sound/timestamp then fall through to fade |
| 8 | 80080e58 | Closing fade / deletion |
| 9 | 800810a8 | History backward scroll |
| 10 | 80081178 | History forward scroll |
| 11 | 80081278 | History input |

States>11 also draw only after countdown. No universal state-4 hidden/completed
rule applies: selection treats state 4 differently.

Opening/closing sounds are skipped if W.flags mask 8 is set. Otherwise use
type-indexed 8-byte sound pairs at 0x8030f540, without a local type bound. If
selected signed word is nonnegative, psndSFXOn(word OR 0x20000).

| Type | Open low word | Close low word |
| --- | --- | --- |
| 0 | 22 | 23 |
| 1 | 26 | 27 |
| 2,4 | 32 | 33 |
| 3,5 | 28 | 29 |
| 6 | 24 | 25 |
| 7 | 34 | 35 |
| 8 | 36 | 37 |
| 9 | 38 | 39 |
| 10 | 30 | 31 |
| 11 | -1 | -1 |

State 5 plays open sound, stores current animation timestamp, writes state 6,
then falls through. State 6 ORs poll flag 2, computes opening value from elapsed
time since that fixed timestamp and draws. Only stored value>255 clamps to255
and changes state to1; exactly255 leaves state6. On transition refresh D's
start timestamp(+18/+1c) and playback timestamp(+30/+34), then draw. No msgMain
call occurs on that transition invocation.

State 7 plays close sound, refreshes animation timestamp, writes state8, then
falls through. State8 with opacity already zero calls windowDelete(W) and returns
without display. Otherwise calculate closing value/clamp and draw, including
the call that newly clamps to zero. Delete is deferred until the next state8 call.

## General playback outcomes and stay restoration (B)

Only state1 calls msgMain(handle). Map return codes:

- 0: state7, restore stay pose as battle/NPC branch above, draw. Closing sound/
  timestamp/fade starts on the next invocation, not in this result branch.
- 1: state2, clear W.flags mask2, restore stay pose, draw. No handle deletion.
  Event Continue/Repeat/Add can reactivate this retained window.
- 2: state3, draw. Wait input is handled on the next invocation.
- 3 or other: retain state and draw.

Result callbacks in msgMain can run before these transitions. General state2
does not continue playback; it simply draws after the countdown.

## General key wait and entry into history (B)

State3 uses mask A=0x100, expanded to0x500 if freshly loaded D.flags has0x100.
If keyGetButtonTrg(0) intersects A OR0x200, set W.state1 and cached D.wait=2.
Then if trigger has0x200 and cached D.flags lacks0x80, set D.flags bit4 for
acceleration. This local wait admission itself does not test0x80; it affects
only that additional acceleration assignment. Draw; playback resume occurs
on a later state1 call.

Only if resume mask is absent, test trigger mask0x10. Set cached history cursor
f03c=count-1, then reload handle and decrement its cursor again. If signed result
is negative, clamp it to0 and retain state3 without sound/flag changes. Otherwise
set W.state9, refresh animation timestamp, play psndSFXOn(0x20041), OR cached
D.flags with0x40 and draw. Ordinary history entry therefore begins at count-2;
it is not a generic decrement from a previously stored cursor.

## History scrolling and ordered input (B)

Use signed history target `D.words[f044+4*historyCursor]`, with unchecked index.
History animation changes current scroll D.word[28], not target D.word[24].

State9: if signed current scroll<target, draw without arithmetic/timestamp update.
Otherwise convert elapsed animation quotient to float and set current to
`trunc(fma(-binary32(0x3f19999a),elapsedFloat,float(current)))` (approximately
0.6 per quotient unit). Refresh animation timestamp. If new current<=target,
clamp to target and state11; otherwise remain9. No msgMain call here.

State10: if current>target, draw without arithmetic/timestamp update. Otherwise
add the same binary32 factor through fused arithmetic, truncate and refresh
timestamp. Once current>=target, clamp. If history cursor==count-1, write state3
and clear D.flags0x40; otherwise write state11. Skip playback on the arrival call.

State11 tests trigger inputs in this order, taking only the first admitted branch:

1. Mask A (0x100 or0x500 as above): state10, timestamp=current, increment freshly
   loaded history cursor and clamp it down to count-1 if greater; play0x20041.
2. Mask0x200: state10, timestamp=current, cached history cursor=count-1; play0x20041.
3. Mask0x10: state9 and timestamp=current, decrement freshly loaded history
   cursor. Negative result clamps to0 without sound; otherwise play0x20041.

The last branch sets state9 even when decrement clamps at the first entry.
These paths preserve wait state, E, parser records and target scroll. History
is navigation through recorded positions, not a reconstructed dialogue replay.

## Clear callback: separate entry, fade and completion (B)

msgWindow_Clear_Main caches D but does not run the talk countdown. It dispatches
directly: state5 initializes; states1/6 share fade/playback; state7 closes;
all other states draw only. It has no local input-trigger reader/history branch.

State5 writes state1, refreshes animation timestamp and D start/playback timestamps,
then falls into the common branch in this same invocation. That branch ORs poll
flag2. If signed opacity<255, calculate opening value using the integer fade
route and clamp only stored value>255. A reread state2 guard skips playback;
otherwise call msgMain even during opening. No opening sound or separate state6
transition is installed here.

Playback result0 writes state7, refreshes animation timestamp, restores stay
pose and draws. Result1 writes state2, clears poll flag2, restores stay and draws.
Result2 writes state3 and draws. Other returns retain state and draw. State3 on
the next clear call only draws: this callback has no local key-resume path.
External lifecycle/callback producers must establish whether/how it resumes.

Clear state7 with opacity already zero deletes W and returns without display.
Otherwise compute closing value from its existing timestamp, clamp <=0 tozero
and draw. It has no general closing sound or state8 staging. ForceClose writes
state7 without refreshing timestamp, whereas this callback's terminal-result
branch refreshes it; do not assume identical elapsed fade starts for both paths.

## Differential obligations and remaining work (P)

Compare countdown-before-state order, exact table targets/sound suppression,
elapsed integer fade versus selection's fixed invocation increments, exactly255
opening, zero-opacity extra deletion call, result1 retained-window poll completion,
wait admission versus acceleration input gate, history count-2 entry, signed
history comparisons/cursor clamps, timestamp updates, ordered history controls,
clear playback during opening, clear wait state without local input and forced
versus terminal close timestamps. These are derived obligations, not traces.

Pool update traversal is now covered in [window manager](MESSAGE_WINDOW_MANAGER.md),
selection draw geometry/cursor mutation in [selection drawing](MESSAGE_SELECTION_DRAWING.md).
Remaining: outer/display manager cadence, renderer and key-wait indicator,
event print/callback/identity/substitution producers, pose services and clock/
input snapshots/numeric helper details; original story attribution/order/branches/
conditions and all other systems. Local general/clear main branches are now
documented; full dialogue and complete-game equivalence remain unfinished.
