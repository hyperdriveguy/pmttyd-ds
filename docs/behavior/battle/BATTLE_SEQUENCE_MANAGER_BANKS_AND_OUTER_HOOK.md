# Battle sequence manager:one captured bank and outer hook ownership

Complete US G8ME01 BattleSequenceManager8011f1ec/0xb0 and lowercase
battleMain800f904c/0x48,static evidence only.With
[uppercase BattleMain](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md),
[verifier](../../../tools/verify_battle_main_dispatch.py) covers three complete
bodies/117 instructions,17 service identities,one startup span,and two exact
aligned DOL direct sites.No full parent field/event scheduling or original
execution claim.Symbol name CASE matters:these are different functions.

## Manager recaptures global battle and dispatches one bank

Ignore incoming battle register state;load CURRENT B=WORD(8041eb90),retain it.
Request BattleGetSeq(B,0).Interpret returned WORD S signed using this exact
local route map.No null guard,parent pause gate,or repeated dispatch loop.

| Captured S | Ordered requests with retained B |
| --- | --- |
| 0 | btlseqInit(B),THEN BattleCheckAllPinchStatus(B,1) |
| 1 | BattleCheckAllPinchStatus(B,0),THEN btlseqFirstAct(B) |
| 2 | BattleCheckAllPinchStatus(B,0),THEN btlseqTurn(B) |
| 3 | btlseqEnd(B) |
| 4 | no handler request |
| negative or>=5 | no handler request |

There is no check of init/pinch/first-act/turn/end scalar returns.No handler
return suppresses a following listed request.Init can set bank0 from0 to1,
but this SAME manager invocation still executes pinch(B,1) and does not run
bank1's first-act.The dispatch bank is read only once;handlers can change fields
for subsequent invocations.Retained B is restored explicitly for each handler.

[Complete init](BATTLE_INIT_SEQUENCE_DISPATCH_AND_RESOURCE_READINESS.md)
now has its actual parent invocation bank.However pinch/source/first-act/turn/
end complete inner graphs are still separate where not already delivered.
The manager assigns no explicit portable success or VM completion scalar;
rawr3 remains the last service return,or original queried S on no-handler route.
Do not derive upper Main's explicit0/1 semantics from this incidental return.

## Outer lowercase hook:guard before uppercase Main

Lowercase battleMain captures CURRENT global B once BEFORE its null test.
B0 skips uppercase Main and all flag writes.No explicit scalar result assigned
on that skipped path;do not fabricate return0/false as original ABI evidence.

B nonzero requests uppercase BattleMain(),whose OWN entry recaptures global B.
Nonzero result performs no local write.Zero result reads CURRENT flags
WORD(retainedOuterB+0x0ef4),OR0x80000000,stores back.No battle free,unit/event
shutdown,resource close,global pointer clear,field scene switch,or init reset
request occurs in this wrapper.The retained outer owner can differ from Main's
recaptured owner;do not flatten them into one implicit pointer parameter.
Rawr3 remains Main's return when called,with no additional success assignment.

## Exact scoped source calls and integration

The verifier scans all7 original DOL text sections for aligned direct bl/bla
to uppercase Main/manager and checks these EXACT2 pairs:

| Site | Target | Containing body |
| --- | --- | --- |
| 800f9068 | BattleMain800f8e28 | lowercase battleMain,complete |
| 800f8e48 | BattleSequenceManager8011f1ec | uppercase BattleMain,complete |

This proves a local parent call chain.It does not census callers of the LOWERCASE
hook,REL/event/indirect/raw uses,or runtime invocation counts.The field/native
simulation cadence remains open;no one-call-per-render-frame assumption is
introduced.Two case-distinct symbols must have separate native callable bindings.

[Unit setup](BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md),
[registry/init readiness](BATTLE_UNIT_REGISTRY_RESET_AND_INIT_EVENT_SCAN.md),
[snapshot reset](PARTY_BATTLE_SNAPSHOT_INITIALIZATION_AND_RAW_MAP.md),
[party switch/export](PARTY_SWITCH_SPAWN_HOME_AND_SNAPSHOT_EXPORT.md),and
[all weapon callbacks](PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md)
provide linked local contracts.The parent dispatch now establishes when the
init handler is requested by bank0,while retaining the original subsequent
pinch call and late Main service/result checks.

Implement captured bank/no redispatch,handler result ignoring,explicit B restore,
independent global-owner captures,outer null guard/completion flag,and separate
raw versus explicit returns.Next needed evidence:outer hook scheduling and
end reimport/caller/source domains plus selected-weapon query reachability.
Source/resource/input/backend readiness,Game/platform/original runtime and
full-game requirements remain open.
