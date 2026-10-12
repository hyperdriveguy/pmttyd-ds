# Operation menu setup/end and live flag ownership

Complete static US G8ME01 revision0 local bodies:
BattleCommandDisplay_OperationMenuSetup80121928/0x4c,
BattleCommandDisplay_OperationMenuEnd80121600/0x38,and
BattleMenuDisp_Operation_Main80115d2c/4. With
[Main](OPERATION_MENU_MAIN_OPCODE_AND_STATE_PUBLICATION.md) and
[navigation](OPERATION_MENU_NAVIGATION_CANCEL_AND_AUDIENCE.md),the
[verifier](../../../tools/verify_operation_menu_control.py) covers four complete
bodies. UI Init/End identities are pinned service boundaries,not fully recovered
UI/resource/source graphs. No original execution claim.

## Setup:producer before UI,then live flag OR

Retain passed B and F. Request MakeOperationTable(B),ignore scalar return.
This [complete producer](OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md)
also builds the party table and performs icon/status/cost/message requests.
Then request BattleMenuDisp_Operation_Init(B,retainedF),ignore return.
Read CURRENT WORD(B+0x1720),OR0x01000100,store it. Read flags AFTER UI Init,
so its live bits are preserved. No cursor-query output argument or descriptor
pointer is passed explicitly to this UI Init;do not copy the Act/Weapon setup
ABI. There is no local index/scroll/opcode/pending/type/Input-state reset.

No explicit success/count assignment:rawr3 remains UI Init's return.
Input states19/20 choose F1/F0 from retained dispatch state,then store21 and
fall directly into OperationMenuMain after restore. Source execution is in
[complete Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md).

## End:UI service before one-bit clear

Retain B;request BattleMenuDisp_Operation_End(B) at80121614. Then read live
WORD(B+0x1720),clear ONLY0x00000100 with mask0xfffffeff,store it. Preserve
0x01000000 and unrelated bits. This mask is PPC rotate-mask24..22 wrapping,
not a whole-menu reset. No type/opcode/state/reason/count/scroll stores or
resource lifetime assumptions are supplied by this local wrapper.
Rawr3 remains UI End's return,without scalar status assignment.

## Exact UI symbols and caller boundary

Complete UI Main80115d2c is `blr` and has no draw/store/return assignment.
Called UI Init80115d30 and UI End80115ccc are larger pinned bodies;their
shared buffers/GX/render/resource behavior remains a separate request. Their
names do not authorize replacing them with no-ops merely because Main is empty.

Setup and End both preserve live flags AFTER their UI services. Main's enabled
confirmation writes Input state BEFORE End,cancel writes it AFTER End,and
audience stores pending BEFORE End. Preserve each order over address-backed
fields/services;do not centralize them into one reset action with different
observability. Reuse original keys/icons/audio/assets through documented
services. Remaining party/protect/multi-item consumers,delegate/source/query
reachability,Game/platform/original runtime/full-game requirements remain open.
