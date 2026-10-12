# WeaponSelectMenuMain: admission, confirmation, reason panel, and light requests

Complete local `BattleCommandDisplay_WeaponSelectMenuMain`80121d0c/0x35c,
US G8ME01 disc0 revision0. With [navigation/end](WEAPON_MENU_MAIN_NAVIGATION_CANCELLATION_AND_END.md)
and [eligibility helpers](WEAPON_MENU_TARGET_COUNT_AND_ELIGIBILITY_UPDATE.md),
this answers the native request in [EVT-01](../../DOCUMENTATION_REQUESTS.md#evt-01--native-event-scheduler-and-dispatch-closure-open).
[Verifier](../../../tools/verify_weapon_menu_main.py) pins seven complete local
bodies,13 linked service identities,344 instructions,and two original sound
keys. Static evidence only;full builder/source/query reachability and original
execution remain separate. [Input](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md)
provides the actual caller;[setup/decision](WEAPON_MENU_SETUP_AND_DECISION.md)
provides the already delivered complete local services.

## Captured owners and key admission

Arguments B and T are retained;T is not reloaded from current menu type.
Retain M=wrap32(B+0x171c), load WORD(B+0x420), request
BattleGetUnitPtr(B,actor), retain U. Local result R=0. Request
BattleMenuKeyOKInACT(B). Zero skips cursor lookup,eligibility,confirmation,
navigation,cancel,and audience,and reaches common display/return0.

Nonzero requests `_btlcmd_GetCursorPtr(M,T,&privateSP08)`, ignoring return,
then `_btlcmd_UpdateSelectWeaponTable(B,T)`. These use DIFFERENT private
stack output slots;the updater's cursor query does not initialize Main's SP08.
Unsupported cursor types leave each output unchanged. This Main does not
initialize SP08 with a type14 cursor or inherit Input's different stack slot.
Current-global actor recapture in the cursor helper can also select different
owners on its two calls;do not impose equality without a source invariant.

After the updater, if current BYTE(B+0x1c74)!=0,clear that byte then
WORD(B+0x1c70). Request Trigger(0x0100). Nonzero confirms;zero still confirms
when a fresh pending byte is nonzero. The fresh reread occurs AFTER trigger,
so a service can republish pending state despite the preceding clear.
Confirmation precedes all directional/cancel/audience controls.

## Enabled confirmation: repeated cursor reads and retained T

Read cursor from SP08,index WORD(C),derive Q=wrap32(M+28*index),then read
WORD(Q+0x84) enabled. Nonzero performs:

1. Sound2520 via psndSFXOn;WeaponSelectMenuEnd(B).
2. Reload SP08/cursor index AFTER both services;request
   `_btlcmd_SelectWeaponDecide(B,retainedT,currentIndex)`.
3. Request TargetSelectMenuSetup(B);then store WORD(M+0)=10.
4. Reload cursor/index, read WORD(Q+0x80) weapon pointer,then
   WORD(weapon+0x64). If bit0x00000200 is CLEAR, request
   BattleStageOnLightFriendInCommand().
5. Reload cursor/index and weapon pointer AGAIN. Read flags+0x64 once.
   If bits0x00004000,0x00000100,0x00000010 are all CLEAR,request
   BattleStageOnLightEnemyInCommand().
6. Set R=1;reach common display and return1.

The two lighting checks can use different weapon rows after the intervening
service. Their first mask is0x200;the second combined mask is0x4110, tested
by short-circuit bit checks on one captured flags word. Do not reuse the first
weapon/flags read. T remains the original function argument even when sound,
End,decision,or target setup changes current M540.

The two complete light symbols at80138994/4 and8013898c/4 are each `blr`.
These specific requests are no-ops in this original DOL;other light services
may have larger bodies. Likewise complete WeaponSelect UI Main8011606c/4
is `blr`. Do not infer all original lighting or weapon rendering is absent.

## Disabled confirmation and reason ownership

Enabled word0:if current pending byte is zero,request original key802efe88,
`SFX_NOT_CHOICE1`;otherwise omit the sound. Always clear pending byte.
Reload cursor/index and read reason WORD(Q+0x98). Zero goes DIRECTLY to
common display with R0;it does not continue navigation in this invocation.

Nonzero requests WeaponSelectMenuEnd(B),stores state8,sets R1,then reloads
cursor/index AFTER End and reads reason AGAIN into WORD(M+0x564).
The displayed reason may differ from the one tested. End/UI services can
change cursor or row memory. Reach common display/return1. There is no
selected-weapon decision or target setup on this branch.

## Common display and return

Every local path requests BattleMenuDisp_WeaponSelect_Main(B) exactly once,
at80122048,then returns local R. This complete linked UI Main stub leaves
r3 unchanged;Main nevertheless explicitly sets its own return from retained R.
R is1 only for enabled confirmation,nonzero reason panel,and explicit cancel
branches. Audience handling may change memory while R remains0.
Neither Main nor End writes menu TYPE M540 directly. Their services can do so;
this is not a transitive no-write claim or a safe query-domain proof.
