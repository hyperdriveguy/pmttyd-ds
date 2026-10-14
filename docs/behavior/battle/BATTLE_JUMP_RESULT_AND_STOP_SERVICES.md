# Jump result/count/grade readers, option masks and deferred stop

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete original local bodies identified below. This is a service
contract supporting [ordinary Jump flow](BATTLE_NORMAL_JUMP_EVENT_FLOW.md),not
measured input timing or closure of all called UI/unit/event services.

| Body | US DOL address | Size |
| --- | --- | --- |
| btlevtcmd_GetResultAC / GetResultCountAC | 0x80107660 / 0x80107548 | 0x34 each |
| btlevtcmd_ac_timing_flag_onoff | 0x80106f40 | 0x60 |
| btlevtcmd_ac_timing_get_success_frame | 0x80106ef0 | 0x50 |
| btlevtcmd_GetResultPrizeLv | 0x801075c4 | 0x9c |
| BattleActionCommandGetPrizeLv | 0x800fa0e8 | 0x44 |
| btlevtcmd_StopAC / BattleActionCommandStop | 0x80107758 / 0x800fa420 | 0x30 each |
| battleAcDelete_TimingA | 0x8012a7c0 | 0xc |

## Stored result and count are separate outputs

Both retain the argument pointer, capture the shared BattleWork pointer, then
load raw output operand0. GetResultAC reads retained BattleWork word1cb8;
GetResultCountAC reads word1cb4. Each writes through evtSetValue.
GetResultCountAC capture/load sites80107558/5c/60 were independently rechecked
on2026-10-13; see [count ownership](DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md).
Neither resolves the destination first,invokes the result evaluator,resamples
input,increments count or resets state. Both return event completion2 with no
first-call gate. Preserve evtSetValue side effects and raw destination policy.

The [ResultAC wrapper](BATTLE_ACTION_COMMAND_CALLBACKS.md) separately invokes
the command's evaluator and polls stored pending bit0 (mask1). The script calls it
before reading result. A result reader alone cannot replace that request.
The [TimingA evaluator](BATTLE_TIMING_A_COMMAND.md) can increment count on
success and can reevaluate the same retained pad history;these two readers
do not prevent repeated evaluation.

## Timing option update and success-window output

ac_timing_flag_onoff resolves operand0 through evtGetValue as on/off,and reads
operand1 **raw** as mask. With nonzero on,OR the mask into BattleWork1f50;
with zero on,AND with its complement. Return2. It neither updates whole
AcSetFlag word1cc4 nor evaluates success. The ordinary Jump sites use literal
mask1 on,then off;they do **not** set mask2,count-dependent window selection.

ac_timing_get_success_frame captures raw output operand0,calls
battleAcTimingA_GetSuccessFrame(work),writes the returned signed integer through
evtSetValue and returns2. The full difficulty/count-table behavior is in
[TimingA](BATTLE_TIMING_A_COMMAND.md). It does not return a measured animation
impact frame. Jump divides this output by2 and waits that result after turning
mask1 on. Shared work/option producers and command-main cadence remain separate.

## Prize wrapper actor policy and complete core

GetResultPrizeLv resolves actor operand0 first. Values-1 or-5 leave its local
unit pointer null;all others are translated through BattleTransID and looked
up from shared work. Then resolve operand1 through evtGetValue,capture raw
output operand2,and call BattleActionCommandGetPrizeLv(capturedWork,unit,
resolvedOperand1). Write the result through evtSetValue and return2.

The work pointer passed to the core was captured before operand resolution;
the wrapper reloads shared work separately for the unit lookup. Do not unify
those reads when describing service mutation/alias behavior. No unit-null check
is inserted by the wrapper;the core handles its null unit selectively.

The complete core:

1. If unit is nonnull and low8 bits of word unit278 equal30,return-1 immediately.
2. Otherwise read unsigned byte work1cbd. Add it to signed operand1 with32-bit
   word arithmetic,subtract1,then clamp the signed result to0..6 and return it.

The byte is not read on the early-1 route. There is no HP test,part lookup,
RNG,AC result evaluation,count read or tutorial condition in this core. Byte1cbd
is not the count word1cb4. Its producer and wider field schema still need
recovery;do not invent a combo-count grade formula. Jump supplies(targetID,0,
local6),so its local grade is clamp(U8(1cbd)-1,0,6),except the low-code30
early return. Unit278's last-hit code lifetime is another caller dependency.

Jump later passes the grade to success effects and AudienceDeclareACResult.
The [declaration writer](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md) treats exactly
-1 as bad and every other integer as good. Thus command success,damage code and
audience classification are separate layers;an early-1 grade must be retained.

## Stop requests delayed TimingA finish

BattleActionCommandStop reads the delete callback at work1cac;if nonnull,
invokes it with work. It does not itself clear callbacks,result,count or pads.
The event StopAC wrapper makes that request,then BattleAcHelpSetDispType(0),
and returns2. There is no first-call guard,blocking poll or implicit event delete.

TimingA's complete delete callback stores state1000 at work1c9c and returns.
The [main callback](BATTLE_TIMING_A_COMMAND.md) then starts/decrements the60-call
finish countdown,changes to1002 when it expires,and clears callbacks on a
later main invocation. Therefore StopAC is **not immediate callback removal**.
Multiple StopAC invocations while the delete pointer remains installed can
store1000 again. Work/result/count remain available until other owners change
them;do not fabricate per-hit setup/reset on the Jump loop.

## Scoped requirements and verification

| ID | Requirement | Derived acceptance case |
| --- | --- | --- |
| JAC-01 | Keep result/count readers and evaluator separate | Reader gets stored words unchanged;only ResultAC invokes evaluator/pending protocol |
| JAC-02 | Resolve on/off but retain raw mask and window-output policy | Mask1 on/off retains other1f50 bits and leaves1cc4 unchanged;window read does not count success |
| JAC-03 | Preserve prize early code,byte read and clamp | Nonnull code30 returns-1 without byte read;null unit bypasses code;byte0/1/7/255 with operand0 yield0/0/6/6 |
| JAC-04 | Preserve stop/delete/main ownership | Stop invokes delete then help0;TimingA stores1000;callbacks survive until documented main cleanup |

tools/verify_jump_event_flow.py pins complete bodies and instruction anchors,
including original concrete event sites. Tests are derived requirements,not
original executed observations. UI help,grade-byte/result-code producers,
unit lookup/translation/global state mutation,pause and actual command-main
credits remain open. Original code/resources/inspection artifacts are kept
outside source history;ISO reads are read-only.

## Shared wrappers and native Harbor TimingA evaluator (2026-10-14)

[`battle_action_events.cpp`](../../../source/battle/battle_action_events.cpp)
implements the complete ResultAC, StopAC and result/count reader wrappers with
explicit provider readiness. ResultAC captures B, loads its callback, invokes
it when present or reads the null-callback result&2 fallback, then independently
rereads retained B's pending bit. Scalar returns are ignored; help0 recaptures
its own current battle/window. Unbound callbacks return no completion after
preceding effects; they must not be blindly replayed as retry-safe transactions.
StopAC invokes the retained delete pointer before help0 without clearing fields.
Readers retain raw destinations, narrow signed words and never evaluate/reset.

The native Harbor adapter admits the actual TimingA result8012a8c4 using source
flags/count/result/classification, setup options, actor flags/automatic byte,
battle flags, general30-entry trigger history and retained RNG. Unsupported
callback/difficulty domains stay unbound. The ordinary history route requires
an admitted current physical producer; missing input is not treated as zero
presses. Source confusion, automatic and forced-result routes do not acquire a
pad dependency they did not have. The native delete8012a7c0 stores state1000;
other callbacks remain installed for the original deferred finish owner.

Tests cover ignored scalar return versus fresh pending word, null-result
fallback and separate reread, current help/window after global mutation,
unknown providers, deferred stop preserving callbacks/count, and raw signed
readers. Native fixtures age weapon-menu A out of the actual history, supply
one physical sample per simulation visit, and drive either no action press or
one press while the source option window is active. They establish result0/
count0/class2 versus result2/count1/class3 and help0. They are controlled native
fixtures, not measured original impact/window timing or a playable Game flow.

Without ordinary physical input the actual Jump remains at ResultAC. With
samples, its failure path advances to CheckDamage8010a78c and its success path
to original voice request80104508. Both retain HP and story result at these
frontiers. Complete TimingA main/private-pad/display-queue scheduling, voice,
hits, StopAC execution in the whole graph, post-action control and original
menu/HUD/second-screen integration remain required. No action result reader
alone substitutes for the actual callback manager or establishes visual fidelity.
