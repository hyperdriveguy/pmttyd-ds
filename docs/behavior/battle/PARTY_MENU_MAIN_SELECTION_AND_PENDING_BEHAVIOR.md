# Party menu Main:selection,pending confirmation,and live party word

Complete static US G8ME01 revision0
BattleCommandDisplay_ChangePartySelectMenuMain80121098/0x224.
[Navigation/cancel](PARTY_MENU_NAVIGATION_CANCEL_AND_AUDIENCE.md) completes
the same body;[party/protect lifecycle](PARTY_PROTECT_MENU_SETUP_END_AND_INPUT_BINDINGS.md)
closes five other local bodies. Native request:
[EVT-01](../../DOCUMENTATION_REQUESTS.md#evt-01--native-event-scheduler-and-dispatch-closure-open).
[Verifier](../../../tools/verify_party_menu_lifecycle.py) pins six complete
bodies,called service identities,all local instructions,and two original sound
keys. Static evidence only;UI internals/source/event/query reachability remain
separate. Reuse original data,dialogue,icons,and sounds.

## Entry retains pending state

Retain passed B,local R0,M=wrap32(B+0x171c). Request
BattleMenuKeyOKInACT(B). Zero skips cursor/input controls and goes to display/
return0. There is NO initial pending-byte or saved-Input-state clear,unlike
OperationMenuMain. No local actor lookup,table rebuild,or eligibility update.

Gate nonzero requests GetCursorPtr(M,10,&privateSP08),ignore return.
Request Trigger(0x0100);nonzero confirms. Zero still confirms when current
BYTE(B+0x1c74)!=0. Cursor helper has its own current-global actor ownership,
while B/M remain passed/retained. No index/count/actor guard is added here.

## Enabled confirmation selects a fresh row after services

Read cursor/index,derive Q=wrap32(M+24*index),read WORD(Q+0x394) enabled.
Nonzero executes:

1. Request psndSFXOn(2520).
2. Request BattleStageOffLightInCommand(0,1).
3. AFTER both services,reload SP08 cursor/index,derive current Q,and read
   WORD(Q+0x390) source party/kind word.
4. Store that word to WORD(M+0x54c),then store Input state29 at WORD(M+0).
5. Request ChangePartySelectMenuEnd(B),then set local R1.
6. Common display and return1.

The enabled-test row need not be the published row under service-visible cursor
mutation. Do not reuse the initial index or conflate the source kind word with
a translated pouch-slot index. There is no current/previous menu TYPE write,
operation-code write,party-class cursor reset,registry/pouch switch,event
constructor,or pending clear in this enabled branch's own instructions.
Its callbacks can have separate effects;this is not a transitive no-write claim.

## Disabled confirmation continues navigation

Enabled0:if current pending byte is0,request original key802efe88,
`SFX_NOT_CHOICE1`;nonzero pending omits sound. Always clear pending byte.
Then FALL THROUGH to navigation in this same invocation. No reason field read,
reason-event state,error panel,or immediate return is inserted here. This differs
from disabled operation/weapon confirmation,which can exit early or open a
reason panel. Simultaneous confirm and direction/cancel gates can therefore
perform both disabled-confirm handling and a later movement/cancel action.

## Complete producer/caller relationship

[Party table producer](PARTY_SELECTION_TABLE_PRODUCTION_AND_CAPTURE.md)
provides24-byte rows and type10 cursor count/index. The
[operation producer](OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md)
builds it as a side effect;party Setup itself has no direct producer request.
[Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md) routes
states24/25 through Setup then immediate state26 Main;nonzero Main return
sets its progress flag. State29 uses the
[attack-event selector](BATTLE_ATTACK_EVENT_SELECTION.md),which independently
interprets menu type/operation/selected-party fields and binds ChangeParty.
No weapon-query call occurs directly in this Main.

Static local routes are not proof that all downstream events avoid unsupported
weapon cursors. Remaining UI/producer provisioning,protect/multi-item/delegate
sources,query-event reachability,Game/platform/original runtime/full-game
requirements remain open. Never fabricate a default party/weapon to fill an
unproved source-domain gap.
