# Weapon-menu type domains and concrete original cursor reuse

US G8ME01 revision0; ID2,2026-10-12. Native request before Game binding.
Read [complete setup/decision](WEAPON_MENU_SETUP_AND_DECISION.md),
[cursor/query contract](BATTLE_SELECTED_TARGET_AND_WEAPON.md),
[attack-event selection](BATTLE_ATTACK_EVENT_SELECTION.md).
Verify `python3 tools/verify_weapon_menu_domains.py`. This document recovers
scoped original paths/census,not complete BattleCommandInput/gift/confusion graphs
or all source reachability. No original runtime trace.

## Menu type and input state are distinct words

M=B+171c. Cursor/query TYPE is WORD M540=B1c5c;previous type is M544=B1c60.
Input-state dispatch reads WORD M00 and uses30 targets at80364074 for states0..29.
Do not confuse an input state with a cursor type. Type9/11/12/13 and unsigned
out-of-range cursor queries leave their output word untouched;the helper still
queries the current acting unit before its type guard. Type14 writes a class
cursor selected by current acting kind222 versus other. These rules are complete
in the linked cursor contract.

## An explicit type12 writer prevents assuming a globally supported domain

Scoped generic audience-item branch in _check_present_item:
read-only instructions801201a8..80120204 require retained item ID>=0 and<339,
after earlier ID0/123/124 special branches. It sets BYTE captured B1c74=1,
or2 when WORD B171c (input state) is12/13. Then BattleCommandDisplay_AllEnd(B).
AFTER that call,read CURRENT B1c5c;store it to B1c60;store TYPE B1c5c=12;
store input state B171c=29;return1. Retained B is the global captured at80120034.

This proves a type12 write in the source. It does not prove every item/actor
input is reachable or that GetSelectWeapon is called while12. Gift branches,
item/target providers,full null policy and transition scheduling remain separate.
Do not infer the full function or source enum from this scoped path.

## State0 has a conditional previous-type restore

In BattleCommandInput state0 branch80122a6c..80122aa8,read BYTE B1c74. If nonzero,
read CURRENT M540;only signed types11 or12 copy CURRENT M544 to M540. Type13
is not restored by this numeric test. Then reread B1c74;exact2 clears that byte0.
Other types/byte values preserve their displayed fields. Broader state0 logic
is not recovered here;no global guarantee that every weapon query passes this
restore or that the previous type was supported is supplied.

## Same-frame cursor initialization/reuse is explicit in input state11

BattleCommandInput entry captures passed B,looks up acting unit from B420,
computes M=B+171c (wrap-to0 returns1),then requests _btlcmd_GetCursorPtr(M,14,&newSP08) at80122a14.
It next calls _check_present_item and other services before state dispatch.
State11 targets80122d20. That branch calls
_btlcmd_GetCursorPtr(M,currentType,&THE SAME newSP08) at80122d34. Later80122dc4
reads that stack word,then its cursor index to select a weapon-table row.

If currentType is unsupported and no intervening operation changed newSP08,
the helper leaves the type14 cursor intact from this SAME invocation. This is
concrete initialization and reuse,not a made-up fallback cursor. Providers,
physical stack aliases and full input control flow can change the word;source
execution/reachability remain open. An adapter for this caller must model the
existing output slot instead of rejecting every untouched-output helper call.

BattleGetSelectWeapon allocates a DIFFERENT private stack slot and does NOT
initialize it before querying current TYPE. WeaponSelectMenuSetup likewise uses
its own uninitialized slot. The input evidence does not prove their unsafe types
valid,provide a shared global cursor or justify initializing those slots to14.
Original stack residue/reachability at those specific query callers is still
needed. Native checked failure for those unresolved domains stays explicit policy.

## Scoped direct call and store evidence

[Call-site census](../../../tools/weapon_menu_call_sites.json) pins16 aligned
DOL direct bl/bla sites:BattleGetSelectWeapon6,table builder4,decision3,setup3,
CommandGetWeaponAddress0 (total16). CommandGetWeaponAddress is invoked through original event sources,
including ordinary Jump;absence of a direct DOL call is not absence of gameplay.
Census excludes REL,event operands,indirect/raw-pointer and dynamic call counts.

Seven selected battle-command store instructions use displacement540/1c5c:
AudienceCheck8011f5b8,ConfuseAct8011f788,present-item80120200,
ActMenuMain80122380/8012254c/80122754,Input80122a94. Only the gift and restore
paths above have their scoped value/order recovered here. Other type producers
and their class-table domains remain open;opcode displacement matching alone
is not an exhaustive semantic field-writer census. No all-writers proof claimed.

## Porting requirements and remaining blocker

Port the supplied setup/decision functions and unchanged-output cursor service.
Preserve initialized-versus-uninitialized caller slots;keep actual type,previous
type,input state,record index and captured/current battle owners distinct. No
default weapon is supplied. Validate real source class/type writers and the
six query callbacks' event/control paths before enabling their Game bindings.

Recover full ActMenuMain/MakeActClassTable,Input/table/gift/confusion/audience
branches and original event/menu/unit/source domains. A global valid-type
claim requires proving the explicit12 path unreachable or irrelevant to each
query;safe query-specific reachability remains unproven. Source/native fixtures/platform/Game/original
traces and complete-game requirements remain open under EVT-01/JUMP-01.

## Delivered class-table producer continuation

[Complete admission/publication](ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md)
and [eligibility/keys](ACTION_CLASS_TABLE_ELIGIBILITY_AND_MENU_KEYS.md) now
recover _btlcmd_MakeActClassTable80124dc8/5b8,including class type13 and
independent final flag reads.This closes local record production;ActMenuMain
type publication and query-specific reachability remain open.
