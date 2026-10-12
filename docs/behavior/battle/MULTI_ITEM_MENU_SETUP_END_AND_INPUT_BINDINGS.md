# Multi-item menu lifecycle and Input bindings

Complete US G8ME01 MultiItemMenuSetup801215b8/0x48,
MultiItemMenuEnd801212f8/0x38,and UI Main80115880/4,static evidence only.
[Verifier](../../../tools/verify_multi_item_menu.py) covers these with
[Main](MULTI_ITEM_MENU_MAIN_SELECTION_AND_NAVIGATION.md),
[producer](MULTI_ITEM_TABLE_MODE_FIELDS_AND_SOURCE_BINDINGS.md),and
[cancel check](MULTI_ITEM_CANCEL_CHECK_CONTROL_AND_DISPLAY_REQUEST.md).
UI internals/source/event/query domains remain separate.

Setup retains B/F;request MakeMultiItemTable(B),ignore return;request
BattleMenuDisp_MultiItem_Init(B,retainedF);then read CURRENT WORD(B+0x1720),
OR0x20,and store it. No explicit cursor output/table pointer passed to UI Init.
No type/state/mode/pending/reset assignment. Rawr3 remains UI Init return.
This setup does NOT OR0x01000000 as several other menu wrappers do.

End retains B;request BattleMenuDisp_MultiItem_End(B),then read live flags
WORD(B+0x1720),clear ONLY0x20(mask0xffffffdf),store. Preserve all unrelated
live bits. No mode/cursor/type/state reset or explicit scalar status assignment;
rawr3 remains UI End return. UI-before-live-mask order must survive adaptation.

Complete UI Main80115880 is `blr`. UI Init80115884/0x60 and End80115820/0x40
are pinned service identities,not no-op assumptions or complete render/resource
recovery. Original DrawMultiItemWin and cancellation display are other bodies.

[Complete Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
states14/15 choose Setup F1/F0 from retained dispatch state,then store16,
restore helper,and immediately request Main in the same dispatch. State17
creates/waits for the reason event;completion routes back to15. Target confirm
reads retained type2/fresh actor mode/M550 and pays original mode2/3 weapons,
while its [complete wrapper](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md)
controls exact order. Neither Setup nor End implements item-use count,event
scheduling,inventory mutation,or cost payment itself.

Implement retained flags/arguments,partial producer outputs,exact UI ABIs,
caller-specific state/store/End order,and explicit Input progress policy.Reuse
original sound/key/icon/event data;dynamic capabilities and full multi-item
attack execution/query reachability remain open.Native Game/platform/original
runtime/full-game requirements are not completed by these local wrappers.
