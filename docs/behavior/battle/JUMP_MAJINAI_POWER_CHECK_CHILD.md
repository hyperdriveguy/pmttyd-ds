# Complete Jump Majinai power-check child and state consumption

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete14-command marioAttackEvent_MajinaiPowerUpCheck80363388/8c,
_majinai_powerup_check801146c4/80,BattleMajinaiDone800f7718/5c,
BattleMajinaiEndCheck800f76d4/44 and pouchGetPtr800d5eb0/8.
Verify:`python3 tools/verify_jump_majinai_child.py`:15 complete/linked body
identities,240 instructions,nine data spans and three complete source spans
including original trailing scanner words. Static evidence;no original fortune
activation/scheduler/execution trace. Read
[presentation/nested sources](JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md),
[ordinary Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md),
[prior data7 child](BATTLE_UNIT_DATA_CHILD_CONTINUATION.md) and
[local opcode coverage](../event/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md).

## Original14-command request graph

The ordinary non-first-attack Jump branch invokes this as a literal opcode94
child AFTER RunDataEventChild(-2,7). Its input locals/flags/metadata are constructor
copies;user pointers are shared. This is separate from Mario's tiny key7 child.
All addresses below are actual command headers;L0 is raw LW token-30000000.

| Header | Operation / operands |
| --- | --- |
| 80363388 | USER_FUNC _majinai_powerup_check(raw output L0) |
| 80363394 | IF_EQUAL(L0,0) |
| 803633a0 | DELETE current event when equality passes |
| 803633a4 | END_IF |
| 803633a8 | SET(L0,0) |
| 803633b4 | SET(L1,0) |
| 803633c0 | SET(L2,0) |
| 803633cc | RUN_EVT(80306874,evt_majinai_effect_se_btl) |
| 803633d4 | USER_FUNC _battle_majinai_effect(raw operand-3) |
| 803633e0 | CHILD_EVT(80363414,marioAttackEvent_MajinaiJump) |
| 803633e8 | USER_FUNC AnnounceMessage(2,0,0,802ef5e4,180) |
| 80363404 | WAIT_FRAME(60) |
| 8036340c | DELETE current event |
| 80363410 | END_SCRIPT terminal |

At query0,the explicit DELETE happens before any SET/fork/effect/jump/announcement/
wait. Child deletion still copies modified local/flag/message fields back and
clears parent waiting state. For nonzero query,the three SETs precede the
independent sound root,whose copied coordinates therefore begin0/0/0. The effect
callback blocks according to global name presence;only after it completes can
the jump child begin. The announcement request follows that child completion.
WAIT_FRAME counts dispatcher invocations using the existing wait protocol,not
60 rendering frames or a measured second.

The exact original message key at802ef5e4 is `btl_msg_mj_power_up`,including
its null terminator. Preserve that ID and the original announcement arguments;
no rewritten message or speaker/plot attribution is supplied here. Original
message lookup/text/control tokens,queue arbitration and renderer remain their
own required contracts. The graph is not a direct “display native success text”.

## Power-query callback: output first,flag/state consumption later

_majinai_powerup_check does NOT read callback firstCall. Retain E;read its
argument pointer E18,capture global B at801146dc,then retain RAW output token
A0 at801146e0. Call pouchGetPtr,which reads pointer8041eb00 (r13+1be0).
Read U8(pouch+5ba) and compare exactly1. No event-actor/unit/status lookup,
RNG draw,threshold,HP/damage calculation or first-call/poll marker participates.

For every value OTHER than1,call evtSetValue(E,raw output,0),ignore its return
and return handler2. This branch performs NO battle-flag clear and no Done/
EndCheck call. An existing B18ff9 value survives;query0 is not a global flag
normalization. Capture output raw,not evtGetValue(output) as an input.

For exact1,call evtSetValue(E,raw output,1) FIRST. After that call,write byte1
to CAPTURED B+18ff9 at80114720. Then call BattleMajinaiDone at80114724,then
BattleMajinaiEndCheck at80114728;ignore returns and return2. Output setters can
alias storage or mutate current globals;the flag still uses captured B while
Done and EndCheck obtain their own current owners. Do not snapshot all pouch/
battle state once or publish output after consumption. No event allocation occurs
inside this query;the containing script supplies its later event requests.

B18ff9 is the one-byte action state previously reset only when exact1 in the
[action controller](BATTLE_ACTION_CONTROLLER.md). Producer/consumer meanings
beyond these local writes remain source requirements. Pouch mode5ba==1 is an
activation predicate,not evidence of runtime activation probability or a permanent
bonus. Repeated external calls can differ after the following state consumption.

## BattleMajinaiDone: byte decrement/wrap and unconditional reset

Call pouchGetPtr independently;retain returned P. Read U8(P5b8),subtract1,
store LOW8 to P5b8;RELOAD that byte and compare with0. Both resulting branches
store P5b9=ff then P5ba=0,in that order. They have distinct instruction paths
but identical storage effects. There is no lower-bound/positive guard:
prior0→255,prior1→0,prior2→1. Do not clamp exhausted count0 or skip the reset
when the decremented count is nonzero. There is no scalar result contract.

The two branches are800f7740..50 and800f7754..60;the decrement/store/reread
are800f7728..38. No source pointer recapture between those stores,no RNG,
message request,save/disk request or battle-flag change in this body. These are
pouch RAM bytes;the local name does not establish persistent save ownership.

## BattleMajinaiEndCheck: conditional captured battle flag

Capture CURRENT battle B2 at800f76e4 BEFORE calling pouchGetPtr. From its
returned P2,read U8(P2+5b8). If nonzero,do nothing further. If exact0,read
word B2+ef4,OR numeric00400000 and store that word. Never clear it on the
nonzero path. Return has no specified scalar result;callers here ignore it.
No count decrement or mode reset occurs in this body. After ordinary Done
prior count1 causes0 and therefore the OR;prior0 wraps255 and does not.

Pouch pointers used by query,Done and EndCheck are three separate reads;query
captured B can differ from EndCheck B2. Preserve original ordering rather than
forcing one shared transaction. Count/mode producers,fortune trigger conditions,
turn reset,global pointer ownership and further battle flag consumers remain open.

## Acceptance and remaining implementation sources

Check all query modes0/1/2/255,raw/aliased output ordering and no flag clear on
false;captured versus current battle/pouch owners;count0/1/2/255 byte wrap,reread,
identical branch store order5b9→5ba;end flag OR only exact0;both script paths,
independent sound-root copies,parent/child copy-back and effect polling before
jump/announcement/wait. Preserve root speed/priority/context/ID and child timing
from the delivered constructors;do not replace the child with immediate success.

This closes local query/consumption and complete selected graph. Full fortune
producer BattleMajinaiCheck,pouch save/reset/writer lifetime,effect constructor/
update/deletion backend,announcement/message services,nested callback domains,
script address provider and original execution remain EVT-01/JUMP-01/DMG-02.


[Complete BattleMajinaiCheck](BATTLE_FORTUNE_PRODUCER.md) now supplies countdown/
mode activation,and [pouch initializer/caller/RNG budget](BATTLE_FORTUNE_INIT_AND_CALL_BINDINGS.md)
closes its used local initialization/source leaves. Full field assignment/
setup/pouch/save/turn/backend and original activation traces remain open.
