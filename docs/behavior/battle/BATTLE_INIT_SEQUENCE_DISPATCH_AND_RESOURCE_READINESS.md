# Battle init sequence:setup dispatch,event readiness,and resource polling

Complete static US G8ME01 btlseqInit8011ee40/0x198.
[Registry reset/readiness](BATTLE_UNIT_REGISTRY_RESET_AND_INIT_EVENT_SCAN.md)
and [sequence leaves](BATTLE_SEQUENCE_FIELDS_AND_INIT_SOURCE_BINDINGS.md)
complete six local bodies for the native request in
[EVT-01](../../requests/EVT-01.md).
[Verifier](../../../tools/verify_battle_initialization_dispatch.py) pins17 hashes
(6 complete/11 linked),209 instructions,five source spans,and two exact aligned
DOL init/setup calls.No original execution or end-to-end battle readiness claim.

## State is captured once per invocation

Retain passed B and source-key base802ef838.Request BattleGetSeq(B,1),retain
its returned sequence word S only through comparison dispatch.This caller has
NO loop redispatch after a sequence update;only state0 falls into state1's body.
The following are literal sequence values,not menu TYPE/Input-state values:

| S | Local behavior |
| --- | --- |
| 01000000 | request SetSeq(B,1,01000001),then immediately execute state1 below |
| 01000001 | field flags/unit setup,then SetSeq(B,1,01000002),return |
| 01000002 | check unit init events;nonzero→SetSeq(B,1,01000003),return |
| 01000003 | ordered effect resource checks;all nonzero→SetSeq(B,1,01000004),return |
| 01000004 | request SetSeq(B,1,01000005),return |
| 01000005 | status-window requests,SetSeq(B,0,1),SetSeq(B,2,02000000),return |
| all other signed words | return without further calls/stores |

State0 and4's SetSeq third argument is carried inr5 from prior comparison
setup (01000001/01000005),not explicitly reloaded beside those calls.The
complete setter actually consumes it;do not invent a two-argument increment
service or preserve a stale native r5.State transitions1→2 and2→3 do not
execute the next state's check in this invocation.

## State1 field services and unit setup

Request marioGetPtr();read returned address+4 flags,OR0x1,store it.No null
guard.Then request marioGetPartyId(),IGNORE its return;request partyGetPtr().
Returned0 omits party flag write;nonzero reads that address+4,OR0x1,stores.
There is no interpretation of the ignored party-ID value as a readiness gate.

Request Btl_UnitSetup(retainedB),IGNORE its scalar return;then SetSeq(B,1,
01000002).The [complete setup caller](BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md)
constructs/imports actors through its own global/resource/services.No wait,
status clear,event-ID repair,or cursor/menu publication is inserted here.
Bit0 writes are literal local field flags,not a claim of complete native field
visibility/collision teardown.Field getter/service bodies are pinned identities.

## States2 and3:ordered readiness checks

State2 requests BattleCheckEndUnitInitEvent(B).Zero leaves sequence unchanged;
nonzero writes01000003.[Complete readiness scan](BATTLE_UNIT_REGISTRY_RESET_AND_INIT_EVENT_SCAN.md)
checks/clears IDs through all64 unit slots without early break.This caller treats
ANY nonzero scalar as ready,not exactly1.

State3 requests these services in order,stopping on each returned0:

1. animEffectAsync(802efbe4,1),source key `p_b_st`.
2. animEffectAsync(802efbec,1),source key `p_jyabara`.
3. animEffectAsync(802efbf8,1),source key `p_kaiten_h`.
4. effNiceAsync(1).

All nonzero writes01000004.These are repeated readiness/service requests on
later invocations while state3 remains;there is no local sleep,tick countdown,
render-frame timing,or measured async duration.Negative nonzero returns still
advance this caller's gates.No rollback of earlier resource requests when a
later gate returns0.Original keys are pinned/reused;no generated effects used.
Complete resource-loader/cache/asset/platform lifetime behavior remains separate.

## State5 and result ownership

Request BattleStatusWindowEventOn(),then BattleStatusWindowSystemOn(),then
SetSeq(B,0,1),then SetSeq(B,2,02000000).There is no local request setting
sequence1 to6 or clearing that word in this case.No explicit success/count/
VM completion value is assigned at the epilogue.Rawr3 is whichever service/
query return the path last left there.Default leaves original query S;paths
ending in SetSeq leave passedB through that leaf.Do not drive progression
from an invented bool return;observe documented sequence-field writes.

Original source invocation of this sequence and its parent scheduler,field
ownership,resource/init event execution,end/query domains,Game/platform/runtime
and full-game requirements remain open.Recovery closes the local dispatch path
into Btl_UnitSetup,not complete battle start or tutorial equivalence.
