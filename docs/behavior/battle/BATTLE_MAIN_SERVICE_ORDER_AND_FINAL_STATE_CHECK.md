# Complete BattleMain service order and final state check

Static US G8ME01 BattleMain800f8e28/0xdc.
[Manager/hook](BATTLE_SEQUENCE_MANAGER_BANKS_AND_OUTER_HOOK.md) completes the
parent dispatch chain.Native request:[EVT-01](../../requests/EVT-01.md).
[Verifier](../../../tools/verify_battle_main_dispatch.py) pins three complete
bodies,17 linked identities,117 instructions,startup r13,and two exact aligned
DOL direct callers.No original execution or full battle/field cadence proof.

## Retained battle and ordered services before completion check

Capture B=WORD(8041eb90),retain it;no local null guard.Call,in order:

| Site | Service | Explicit caller argument preparation |
| --- | --- | --- |
| 800f8e40 | BattleCheckUnitBroken | r3=initial captured B |
| 800f8e44 | BattlePadManager | no new argument assignment here |
| 800f8e48 | BattleSequenceManager | no new argument assignment here;manager recaptures global B |
| 800f8e50 | BattleActionCommandManager | r3 restored to retained B |
| 800f8e54 | BattleStageObjectMain | no new argument assignment here |
| 800f8e58 | BattleIconMain | same |
| 800f8e5c | BattleStageMain | same |
| 800f8e60 | BattleAcHelpMain | same |
| 800f8e64 | BattleAudience_Main | same |
| 800f8e68 | BattleBreakSlot_Main | same |
| 800f8e6c | btlDispMain | same |

Each request runs even if the sequence manager already changed a bank/state.
No returned service result is used as an early gate in this list.Do not move
completion testing before pad/action/stage/audience/display services or invent
a pause bypass;none appears locally.Callee formal ABI and resource/input/event
semantics remain their own contracts;absence of explicit argument preparation
alone is not proof that a helper consumes no register arguments.

After all services,request BattleGetSeq(retainedB,0).Exactly4 returns0 directly,
skipping the global-system-word path.Other words continue.This query uses
Main's retained B,which may differ from the manager's separately captured B
under global-owner changes.No registry/event/resource teardown occurs here.

## Global-word gate and forced end-bank writes

Load pointer P=WORD(80415fd0) (startup r13=8041cf20,signed displacement-28496),
then read WORD(P+0) once.No pointer-null guard or field-user action interpretation.
Test bit0x00001000 first;clear returns1.If set,test bit0x00002000 on SAME
captured word;clear returns1.Both set requests BattleGetSeq(retainedB,0)
AGAIN;only exactly2 takes the write path.All other values return1.

For exactly2:read CURRENT WORD(B+0x0ef4),OR0x60,store it;request
BattleSetSeq(B,0,3),then BattleSetSeq(B,7,07000000),then return1.
The second bank read is fresh after earlier services/checks;do not cache the
first bank query.Neither global-word mask is labelled as a physical button,
pause,debug cheat,or observed escape condition in this evidence.

[Complete sequence leaves](BATTLE_SEQUENCE_FIELDS_AND_INIT_SOURCE_BINDINGS.md)
map selector0→B04 and selector7→B0f24.Do not confuse07000000 with a menu
TYPE or Input state.The end handler is reached by future manager dispatch;
this Main does not redispatch it immediately after those writes.

## Native requirements and remaining work

Implement one retained Main owner,ordered services,independent manager owner,
late bank4 result0,typed/raw global-word gates,fresh second bank read,live flags,
exact end-bank writes,and explicit0/1 result.All other returns from this Main
are1 regardless of called handler returns or state mutation.

Reuse original source/input/resources through documented services.No tick or
render-frame cadence is measured by this local graph.Outer hook/field scheduler,
called service readiness,turn/first-act/end internals,source/query-event domains,
Game/platform/original runtime/full-game requirements remain open.
