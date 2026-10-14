# Action-command feedback counters, initialization and stage-fall gate

US G8ME01 revision0,recovered2026-10-12. Evidence B:complete bodies
btlevtcmd_AudienceDeclareACResult0x8010749c,size0xac;
BattleActionCommandDeclareACResult0x800fa4f0,size0x84;
BattleStage_ObjectFallCheck0x80147644,size0x60;BattleInit0x800f8f04,size0x138.
These connect [tutorial feedback readers](BATTLE_TUTORIAL_STATE_SERVICES.md)
to actual writers and a consumer of the boss init flag. No original execution or
complete tutorial/command/audience equivalence claimed.

## Declaration, counts and last classification

The event declaration resolves weapon operand0,then result operand1 through
 evtGetValue. Use shared BattleWork W from0x8041eb90. Before calling the inner
declaration service,update these fields:

| Resolved result | Count field | Last-classification byte |
| --- | --- | --- |
| Exactly-1 | Failure W+0x16f59 | W+0x19074=0 |
| Every other32-bit value | Success W+0x16f58 | W+0x19074=1 |

Load the selected count as unsigned byte. If it is below200,increment and store
low8;otherwise leave it unchanged. A normal zero-initialized count reaches200
and stops growing. The routine does not normalize an already corrupt value
above200 down to200. The classification byte is written even when count cannot
increment. The other count is untouched by this local body.

Then call BattleActionCommandDeclareACResult(W,weapon,resolvedResult),return
handler2. There is no local first-call gate,unit/Mario check,positive-result
requirement or automatic reset. Counts reflect **declaration calls**,not every
pad press or raw action-command result evaluation. Other actors/commands that
call this same wrapper can contribute unless their callers exclude them.

The tutorial success/failure helpers read these count bytes. Their nonzero tests
therefore mean a prior declaration in the current count lifetime,not necessarily
that the immediately preceding move passed/failed. Unit-owned once-latches remain
separate and preserve [message priority](../story/HARBOR_BOSS_TUTORIAL_EVENTS.md).

## Concrete ordinary Jump/Hammer callers

The previously hashed original attack streams contain:

| Site | Original callback operands after function pointer |
| --- | --- |
| Jump0x8035662c | weapon local12,result-1 |
| Jump0x80356860 | weapon local12,result local6 |
| Hammer0x8035e29c | weapon local12,result-1 |
| Hammer0x8035e340 | weapon local12,result local6 |

Each uses USER_FUNC0x0003005b and callback0x8010749c. The failing branch declares
-1 rather than directly passing raw result0. Do not connect tutorial counters
straight to the port's raw success-bit field and bypass these script calls.
The precise conditions selecting each branch and all non-Mario callers remain
under recovery;see [ordinary bindings](BATTLE_ORDINARY_ATTACK_BINDINGS.md).

## Inner audience declaration request

Inner service checks result exactly-1. That path calls
BattleAudience_Case_ActionCommandBad(weapon);this body does not populate the
following success record. Other results perform the signed32→single conversion
using original double bias0x4330000080000000,then single fused
`multiplier=0.25*float(result)+0.5`. The quarter/half words at0x804221a4/1a0
are independently checked. No grade clamp is applied here.

Store weapon pointer at W+0x18ffc,multiplier at+0x19000,weapon unsigned byte
+0x19 at+0x19005 and byte+0x1a at+0x19006. Request
BattleAudience_Case_ActionCommandGood(weapon). Preserve conversion and fused
rounding boundaries;do not derive the multiplier from an already reduced
boolean. Bad/good helper internals can mutate additional state and are still
open;these local stores do not specify full SP,audience or bingo outcomes.
There is no RNG call directly in the declaration wrapper/inner body,while those
requested audience services may consume randomness.

## BattleInit reset and ordered requests

BattleInit captures its incoming pointer P. Request BattleAlloc(0x19098),publish
returned W at0x8041eb90,then memset(W,0,0x19098). Both feedback counts and the
last-classification byte lie within this initial clear. Null allocation has no
local fallback/check before memory access. Store-1 at W+0x19070 and W+0x424;
store P at W+0x2738.

The remaining original request/store order is:

1. BattlePadInit,BattleAcHelpInit,BattleIconInit,
   BattleActionCommandManagerInit(W),BattleAfterReactionQueueInit,
   BattleStatusChangeMsgWorkInit,btlDispInit.
2. BattleSetSeq(W,0,0),then BattleSetSeq(W,1,0x01000000).
3. Clear W+0xf28,+0x1720,+0x1c78;clear two words in each of ten stride12
   records beginning+0x1bb4 (last pair+0x1c20/+0x1c24);clear turn halfword W+0.
   Clear16 bytes at W+0x19060.
4. Store0 at W+0xef4,then BtlUnit_Init,BattlePartyInfoWorkInit(W),BattleStageInit,
   BattleAudience_Init,BattleBreakSlot_Init.

These calls are ordered requests,not recovered inner initializer contracts.
The bulk clear establishes the local reset before them;post-initializer state
still needs their effects. This routine alone does not prove counters can never
be reset by other bulk/indexed writes during a battle. No dedicated per-turn or
per-result reset has been established in this slice.

A direct DOL displacement scan found the explicit byte increments at the event
declaration wrapper. Such a scan misses indexed accesses,bulk clears,copies,
REL code and differently formed addresses. Do not turn its absence of further
matches into a proof of complete writer coverage or save persistence.

## Proven consumer of tutorial battle flag0x1000

Boss init explicitly sets W+0xef4 bit0x1000 through onoff_battleflag.
BattleStage_ObjectFallCheck reads that same word. If the bit is set,return0
immediately:do not register the stage-fall event and do not clear the old stored
handle word. If clear,request evtEntry(script0x803771bc,priority10,flags0),read
returned event word+0x15c,store it at W+0x182c8 and return that word. There is
no local null-event check after the registration request.

This demonstrates **stage-fall event registration suppression**,not a general
command-menu or defense disable. The script body/lifetime and all other tests
of this battle flag remain open. GP flags and audience-owned flags using the
same numeric mask are distinct memory domains;mask equality alone is not a
consumer link. The declaration-counter path has no local0x1000 gate.

## Validation and remaining requirements

Four new function fingerprints and48 instruction anchors pin declaration/cap/
classification,success-record conversion,initializer stores/order and stage gate.
Constants and four original attack declaration records are checked directly in
the read-only ISO. Original executable/listings/reports stay ignored.

Recover audience bad/good services,all counter reset/indexed/REL writers,
flag0x1000 producers/consumers and field-battle/unit initialization lifetime,
complete attack declaration conditions and original phase/input traces. Observe
counts at0/199/200,classification versus result0/-1,feedback ordering across
moves and actual stage-fall suppression. This closes local writer/reader links,
not full tutorial stats,restrictions,rewards,combat or complete-game requirements.

## Shared/native declaration implementation (2026-10-14)

`battle_action_feedback.cpp` implements the complete local wrapper and inner
record writer with explicit audience providers. Saturation preserves200/255,
classification is always written, exactly-1 is the bad path, other grades use
original single conversion/fused quarter-plus-half. Good-record weapon/bytes
remain source data. Unbound providers can follow prior writes; the API does not
promise retry-safe replay or imply audience/SP/UI completion.

Shared audience bad/good helpers retain captured inline base, query all200
waiting actors, invoke their required populated-case service sequence, then OR
20/10 into fresh flags at the retained base. Waiting uses original fresh getter
owners: GetExist requires flag1 and excludes100/2; GetSysCtrl additionally
excludes10; signed pending halfword must be-1 and unsigned state19 must be<=3.
Getters retain their record before nested existence/control requests. Actor
addresses/indices and field reads retain source wrap/order rather than a native
population-size shortcut. Native Harbor preflights actual waiting count0 before
publishing declaration counts, then runs the full local helper in that domain.
Populated-case services remain unbound; no count is replayed while they wait.
This is explicit native admission, not whole audience initialization/control.

Harbor capture now retains live declaration bytes so the next scene publication
does not erase them. The original failed Jump itself increments failure1 after
its HP impact; raw ResultAC alone does not. The actual AcrobatNotry wrapper
80139898 calls its20-byte base getter-only service8019f4d0. AP-recovery event
80139af4 is exactly li2/blr; those complete local bodies now admit their source
routes. Jump advances to InviteApInfoReport8010436c without inventing report
allocation, SP recovery or a reward. Its HP4/dealt1/failure1 remain stable.

Correction: older handoff/native-damage text misnamed pending8010749c as
AcrobatNotry. It is AudienceDeclareACResult; AcrobatNotry is80139898. The source
IDs, execution and assertions used the actual addresses; the label is corrected.
Tests cover count199/200/255, grade-2/2, original multiplier/bytes, waiting gates,
empty and populated helper requests, missing providers and original script
counter production. Native success still waits at voice80104508. Full audience
report/aftereffect/action completion, manager/display cadence, original menus/HUD
and second-screen crowd/item input remain open; no native visual/runtime or
Dolphin measurement claim follows from these controlled fixtures.
