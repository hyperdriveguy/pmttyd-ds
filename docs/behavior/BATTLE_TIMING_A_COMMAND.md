# US TimingA command: lifecycle, input result and difficulty windows

US G8ME01 revision0,recovered2026-10-12. Evidence B:complete local bodies
battleAcMain_TimingA0x8012aae0,size0x104;battleAcResult_TimingA0x8012a8c4,
size0x1a0;battleAcTimingA_GetSuccessFrame0x8012aa64,size0x7c;
BattlePadCheckRecordTrigger0x8011a028,size0x2c and
BattlePadMultiCheckRecordTrigger0x8011a008,size0x20. The registration entry at
0x80364348 binds ID1 to main/result/display/delete at0x8012aae0/8012a8c4/
8012a7cc/8012a7c0. [Original ordinary Jump sites](BATTLE_ORDINARY_ATTACK_BINDINGS.md)
select ID1;this is a concrete US gameplay dependency. Static recovery only.

## Work fields and main lifecycle

All offsets below are relative to the BattleWork argument:

| Offset | Local use |
| --- | --- |
| 0x1c90 | Acting unit pointer |
| 0x1c94 | Raw setup option word |
| 0x1c9c | Main state |
| 0x1ca0/1ca4/1ca8/1cac | Main/result/display/delete callbacks |
| 0x1cb4 | Success/result count |
| 0x1cb8 | Result word |
| 0x1ce8 | Result classification:1 pressed/mash failure,2 no press,3 success on ordinary route |
| 0x1f20..1f4b | 44-byte command scratch initialized locally |
| 0x1f50 | Shared option/flag word:bit0 cleared during initialization;bit1 selects count-dependent windows |
| 0x1f54 | Main finish countdown |

Main takes one BattleWork pointer. All unhandled states,including99 and100,
return1 and leave the main callback live. State0 clears44 bytes at+0x1f20,
stores original float-300 at+0x1f34 and positive zero at+0x1f38,stores20 at
+0x1f40,changes state to99,clears bit0 of+0x1f50 and stores classification0.
It does not clear the bit1 window selector or the whole shared option block.
No input sampling or success test occurs in this main body.

State1000 sets countdown60 and state1001,then **falls through on the same call**
to decrement the countdown. State1001 decrements the word;when its signed value
is nonpositive,changes state to1002 and still returns1. State1002 clears all
four callbacks and returns0. At normal cadence this means60 main calls including
the state1000 call reach the1002 state;cleanup occurs on the following call.
These are callback invocation counts,not measured rendering-frame durations.
The callback manager also clears main when it returns0;its existing pad/display
order is in [input/defense](BATTLE_INPUT_AND_DEFENCE.md). Producers of states1000
and the result/option fields must still be traced.

## Difficulty and count-dependent window

GetSuccessFrame calls BattleActionCommandGetDifficulty,selects unchecked pointer
array0x80416e18 by signed returned difficulty*4,and initially loads the first
entry's window. The documented difficulty setter normally clamps0..6;this helper
itself performs no range validation. Seven tables contain signed threshold/window
pairs;the full table words and pointer list are independently checked:

| Difficulty | Table | Ordered threshold:window pairs |
| --- | --- | --- |
| 0 | 0x803644f0 | 4:10,5:9,6:8,7:7,8:6,9:5,99999:1 |
| 1 | 0x80364528 | 0:10,1:9,2:8,3:7,4:6,5:5,6:4,7:4,8:3,9:2,99999:1 |
| 2 | 0x80364580 | 0:9,1:8,2:7,3:6,4:5,5:4,6:3,7:2,8:1,9:1,99999:1 |
| 3 | 0x803645d8 | 0:8,1:7,2:6,3:5,4:4,5:3,6:2,8:1,9:1,99999:1 |
| 4 | 0x80364628 | 0:7,1:6,2:5,3:4,4:3,5:2,8:1,9:1,99999:1 |
| 5 | 0x80364670 | 0:6,1:5,2:4,3:3,4:2,8:1,9:1,99999:1 |
| 6 | 0x803646b0 | 0:5,1:4,2:3,3:2,8:1,9:1,99999:1 |

If+0x1f50 bit1 is clear,return the **first window regardless of count**:
10,10,9,8,7,6,5 respectively. If set and signed count at+0x1cb4>=100000,
return0. Otherwise advance8 bytes while count>threshold,then return that entry's
window. Equal threshold stays on the current entry. Do not interpolate,fill
missing thresholds or use the last threshold<=count. Original negative counts
would select the first pair;normal count lifetime requires caller evidence.

## Result routing and input history

Always compute window W first. Apply these early routes in order:

1. If option+0x1c94 bit0 is set and actor+0x27c bit0x10 is set,draw irand(100).
   Nonnegative result stores result0;negative result stores2 and increments count.
   Return the result without changing classification. In the recovered normal
   [irand](RANDOMNESS.md) range,the negative branch is not produced:retain the
   RNG consumption and failure rather than inventing a percentage success rule.
2. Otherwise actor byte+0x307 nonzero stores result2,increments count and returns.
   This body does not identify that byte's producer or set classification.
3. Otherwise set result0. Battle+0xef4 bit0x01000000 forces classification3,
   result2 and count+1,then returns without input checks.
4. Otherwise perform the ordinary history checks below.

Do not assign story/tutorial or badge names to the raw forced-result fields until
writers and original context establish their meaning. Earlier success routes
preserve any old classification and do not clear history.

BattlePadCheckRecordTrigger(index,mask) delegates to the multi-channel helper
with channel0. That helper reads the shared battle pointer,then returns:

`word[battle + 0x0f44 + channel*0x1fc + index*4] & mask`.

No bounds check or history mutation occurs. This is the general battle pad
history,**not** the action-command private history at+0x1d34 used by
BattleACPadCheckRecordTrigger. Result checks mask0x100 (A) using the general
history. Treating those two getters as interchangeable would sample different
state. The existing pad updater establishes index0 as the newest sample.

Count N of nonzero A-trigger entries at indices0..19. If N>=2,store
classification1 and return failure0,even if a press also lies in the success
window. Otherwise search indices0..W-1 for a trigger. If found,store
classification3,result2 and increment count. If none,store classification1 when
N>0,or2 when N==0;return0. Search is skipped when W<=0. The helper does not
consume a trigger,so repeated result evaluation with unchanged history can
repeat success/count mutation. Caller cadence and result evaluation count matter.

For difficulty3 with count-dependent bit clear,the derived window is eight
history entries0..7. One press at index7 passes;at8 fails;two presses in0..19
fail. These are static deductions,not measured attack-impact timing or tests
against the original game. Older presses at20..29 are outside this local mash
check. Inputs remain simulation samples rather than rendering-frame events.

## Remaining mechanic closure

Six new whole-function fingerprints (including SetupAC),32 instruction anchors,
seven pointer/threshold tables,two initialization floats,registration and two
complete attack-stream hashes/four setup sites pin this slice. Reports and
original disassembly remain ignored. None of the tests executes original code.

Recover unit/weapon bindings and the active branches of the Jump event;trace
SetParam/flag/difficulty/state/result producers and general versus private pad
update cadence;recover display/delete and normal Hammer ID3. Compare original
single/early/late/multiple presses,success counts,difficulty changes,forced routes
and countdown boundaries at actual impact/result calls. The opening Crump
encounter may override these rules;its event/unit/tutorial path still needs
binding. Full combat/story/game documentation and requirements remain unfinished.
