# Audience interruption: complete type11 writer

Complete static US G8ME01 revision0 BattleCommandAttackAudienceCheck
8011f524/0xcc. Native request:[EVT-01](../../requests/EVT-01.md).
[Gift handling](GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md) and
[restore/getter ownership](MENU_INTERRUPTION_RESTORE_AND_AUDIENCE_FIELDS.md)
complete this interruption slice. [Verifier](../../../tools/verify_menu_interruptions.py)
pins ten complete local bodies,ten linked services,243 instructions,and three
startup/numeric spans. No original execution or global query reachability.

## Ordered admission and captured global battle

No battle argument is consumed. Capture B=WORD(0x8041eb90) BEFORE requesting
BattlePadCheckTrigger(0x0400). Trigger0 returns0 with no later service or flag
read. Nonzero reads fresh WORD(B+0x0ef4);bit0x40000000 set returns0.
Otherwise request BattleGetMarioPtr(retainedB),then read WORD(returnedU+0x104)
without a null guard. Bit0x10 set returns0 immediately. No current-actor query,
partner substitution,or original physical button mapping is introduced here.

Request BattleAudience_GetItemOn(&privateSP08,0,0,0,0) at8011f584;ignore
its scalar return. SP08 is not explicitly initialized by this caller. Read the
output word;exactly-1 returns0. Any other word is passed to
BattleAudience_SetTarget(outputWord),whose return is ignored. The producer's
output initialization/entity admission remains a separate complete-core request;
do not manufacture a zero/-1 default or assume all non--1 values are valid.

## Previous type captured after menu shutdown

Request ActMenuEnd(retainedB). AFTER its UI/flag services,read CURRENT
WORD(B+0x1c5c) (M540) and retain it. Then store,in order:

1. WORD(B+0x1c60)=that previous type (M544).
2. WORD(B+0x1c5c)=11.
3. WORD(B+0x171c)=29 (Input state,not type).

Request BattleStageOffLightInCommand(0,1). Then read CURRENT battle flags
WORD(B+0x0ef4),OR0x4,and store them. Return1. Do not preload the previous
type or battle flags before the intervening services. This body does not write
pending byteB1c74 or saved Input-state wordB1c70;callers handle those fields.
Successful service-side changes are not rolled back on any local branch.

## Concrete caller relationship,not a global invariant

[ActMenuMain](ACT_MENU_MAIN_NAVIGATION_AND_SERVICE_ORDER.md) calls this only
after no class direction moved. [WeaponSelectMenuMain](WEAPON_MENU_MAIN_NAVIGATION_CANCELLATION_AND_END.md)
can close its own menu and set its passed battle's pending byte after this
helper returns nonzero,while returning0. [Input state11](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md)
can call it after target confirmation has already stored state29. Retained
passed B in those callers need not equal this helper's captured global B
without a source ownership invariant.

[Complete Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md) routes
state29 to the [attack-event selector](BATTLE_ATTACK_EVENT_SELECTION.md),whose
type11 path is AudienceAttack. This is a real local writer/dispatch chain.
It does not establish that all downstream AudienceAttack event callbacks avoid
selected-weapon queries,or prove unsupported cursor output is safe. Type11
has no output-writing cursor case;do not borrow Input's separate initialized
stack word for a different query wrapper. Audience entity/core,source event,
remaining writers,Game/platform/original runtime domains remain open.
