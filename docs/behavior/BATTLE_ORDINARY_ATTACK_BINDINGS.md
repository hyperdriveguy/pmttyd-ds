# US ordinary attack event bindings

US G8ME01 disc0/revision0,recovered2026-10-12. Evidence B:original DOL data
streams marioAttackEvent_NormalJump0x80355cf0,size0xf0c and
marioAttackEvent_NormalHammer_Core0x8035d6d4,size0x14d4,and complete
btlevtcmd_SetupAC0x801077ec,size0x98. This resolves concrete retail script
bindings,not merely callback names or direct-call counts. No original execution,
full attack control-flow recovery or tutorial equivalence is claimed.

The subsequent [ordinary Jump request graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md)
now specifies all268 commands and per-hit/count/result/cleanup branches with
[local result/stop services](BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md). Mario-
specific motion/callback composition and original execution remain incomplete;
the scope limitation above describes this initial binding-only document.

## Command stream evidence

`tools/dol_event_inventory.py` validates symbol VA/file mapping against original
DOL sections and scans annotated big-endian event command/operand spans. It
retains raw operands and static address annotations in an ignored report;it
does not evaluate conditions,variables,child ownership or function behavior.
Original dialogue contents are not emitted. These two spans contain268 and387
commands respectively. Their independently checked hashes are:

| Stream | SHA-256 |
| --- | --- |
| NormalJump | 2858f3f63cfb95dc84eb6ed3b363c530e24200063044f37afd275d3847f9720c |
| NormalHammer_Core | a1db1fb43cdc5b0891c083bffb4506211972f85d4444879c77ff94b5b4007aef |

Event USER_FUNC opcode91 includes the callback as its first operand. Four exact
five-operand records establish the following bindings:

| Command address | Stream | Callback operands after function pointer |
| --- | --- | --- |
| 0x80355ebc | NormalJump | (-2,1,1,0) |
| 0x80356038 | NormalJump | (-2,1,1,0) |
| 0x8035d994 | NormalHammer_Core | (-2,3,1,0) |
| 0x8035dc28 | NormalHammer_Core | (-2,3,1,0) |

Each record begins word0x0005005b,callback0x801077ec. Thus ordinary Jump's
identified setup sites select ID1,while ordinary Hammer core's select ID3.
These are branch-local sites,not proof that each executes on every attack.
The original registration table routes ID1 to battleAcMain_TimingA and ID3
to battleAcMain_StickKeepLeft. See [TimingA](BATTLE_TIMING_A_COMMAND.md),[Hammer ID3](BATTLE_HAMMER_STICK_KEEP_LEFT.md) and
[registration/priorities](US_COMPLETION_AND_PRIORITIES.md).

## Setup callback: resolved and raw operands

The wrapper loads battle work from shared pointer0x8041eb90. In order:

1. Resolve operand0 through evtGetValue,then BattleTransID(event,value),then
   BattleGetUnitPtr(battle,translatedID).
2. Resolve operand1 through evtGetValue;this is command ID.
3. Load operand2 **raw**,without evtGetValue.
4. Resolve operand3 through evtGetValue.
5. Call BattleActionCommandSetup(battle,ID,unit,rawOperand2,resolvedOperand3).
   Return event completion code2.

There is no local first-call gate,null-unit validation or yield. RawOperand2
becomes battle+0x1c94;resolvedOperand3 becomes battle+0x1c98. Do not resolve all
four inputs uniformly. For the four literal sites above,this preserves the
raw1 option word. Meaning of actor sentinel-2 is supplied by BattleTransID and
event context;the literal alone must not be generalized to every actor.
The setup reset/registration path is already in
[input and defense](BATTLE_INPUT_AND_DEFENCE.md).

## Attack progression references and unresolved gates

The Jump stream contains StartAC at0x80356204,ResultAC at0x803565e0,
GetResultAC at0x803565e8,and several StopAC/damage/audience/stylish calls.
Hammer core contains StartAC at0x8035e0e0,ResultAC at0x8035e0ec and GetResultAC
at0x8035e264. These addresses are static anchors for upcoming branch/timing
work. Script address order does not prove wall-clock order:conditional branches,
spawned inline events,waits and child joins must be interpreted first.
No impact tick or early/late window relative to animation is measured here.

Remaining:weapon→event selection and unit data,all surrounding condition/flag
writes,first-strike variants,AC parameters/difficulty and activation/result
callbacks,damage and stylish consumers,control/camera/audio ownership. Establish
which original Crump/tutorial paths reach these bindings and their overrides;
do not substitute these normal attack scripts for the tutorial without evidence.
Original US story IDs,tokens,speaker/order/branch conditions remain mandatory.
Full attack/story/game documentation remains incomplete.
