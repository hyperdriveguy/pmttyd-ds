# Action-command parameters, activation and result polling

US G8ME01 revision0,recovered2026-10-12. Evidence B:complete bodies
btlevtcmd_AcSetParamAll0x80107a64,size0x70;btlevtcmd_AcSetFlag0x80107ad4,
size0x18;btlevtcmd_StartAC0x80107788,size0x64;btlevtcmd_ResultAC0x80107708,
size0x50;BattleActionCommandStart0x800fa450,size0xc;
BattleActionCommandResult0x800fa574,size0x3c. These services occur in the
[ordinary attack streams](BATTLE_ORDINARY_ATTACK_BINDINGS.md). No execution or
complete event/cadence/tutorial equivalence is claimed.

## Parameters and raw flags

AcSetParamAll reads eight operands in order,calling evtGetValue on each.
Store resolved operand i at shared BattleWork+0x1cc8+4*i,**except** resolved
word0xf1194d80,which leaves that destination unchanged. Sentinel comparison is
on the resolved value,not the raw operand;the sentinel is not stored or converted
to zero. Return event completion2. No first-call gate,index/count validation or
whole-parameter reset occurs locally. Earlier stores remain visible while later
operands are resolved.

AcSetFlag reads the first operand **raw** and assigns the complete word at
BattleWork+0x1cc4;return2. It does not resolve a variable,OR bits with old state,
or synchronize the option block at+0x1f50. Those words are distinct.

The normal Hammer core sets raw flag0 immediately before both ID3 setup calls
at0x8035d988/0x8035dc1c. It has four parameter variants in each of its two
surrounding paths:

| Parameter call sites | Resolved literal prefix |
| --- | --- |
| 0x8035d8d0 / 0x8035db64 | 4,25,0,0,0 |
| 0x8035d900 / 0x8035db94 | 4,30,0,0,0 |
| 0x8035d930 / 0x8035dbc4 | 4,30,0,0,0 |
| 0x8035d95c / 0x8035dbf0 | 4,40,0,0,0 |

Each ends three sentinel operands0xf1194d80. Thus the first two stored fields
supply N4 and period25/30/40 to
[StickKeepLeft](BATTLE_HAMMER_STICK_KEEP_LEFT.md);the next three zero fields
include its suppression word. Which branch selects each period still needs
control-flow/weapon-level recovery. Data-address order alone does not establish
selection. Exact records and complete containing stream are independently checked.

## Start and blocking result

StartAC resolves one operand and calls BattleActionCommandStart,which stores
state100 at BattleWork+0x1c9c. If the resolved operand is nonzero and acting
unit+0x27c bit0x10 is clear,call BattleAcHelpSetDispType(2). Otherwise omit that
request. Return2;there is no local first-call guard. Ordinary Hammer supplies0
at0x8035e0e0;the identified Jump start supplies1 at0x80356204.

BattleActionCommandResult loads result callback+0x1ca4. If nonnull,invoke it
indirectly with BattleWork and propagate its integer return. If null,return
stored result+0x1cb8 masked with2. It does not change main state itself.

The event ResultAC wrapper calls that service but **discards its register return**.
Instead reread stored result word+0x1cb8. If bit0 remains set,return event0
(blocked). Otherwise request BattleAcHelpSetDispType(0),then return2. It does
not write an event output,force state1000 or clear callbacks. Correct event
resumption depends on the callback's stored-state effects,not on interpreting
its C return as event completion. Hammer's result callback merely reads the word;
its main must clear the pending bit for the script to advance.

Remaining:other indexed parameter/output/gauge setters,help display internals,
Jump/Hammer result state writers and complete branch/cadence/weapon/tutorial
bindings. These local wrappers do not define the whole battle command protocol.
