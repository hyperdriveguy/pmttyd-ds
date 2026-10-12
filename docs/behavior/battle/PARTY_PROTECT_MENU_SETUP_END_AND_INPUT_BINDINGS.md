# Party/protect setup,end,and original Input bindings

Complete static US G8ME01 local bodies:party Setup801212bc/0x3c,
party End8012103c/0x38,protect Setup80120990/0x58,
protect End80120834/0x10,and party UI Main80115acc/4.
With [party Main](PARTY_MENU_MAIN_SELECTION_AND_PENDING_BEHAVIOR.md) and
[navigation](PARTY_MENU_NAVIGATION_CANCEL_AND_AUDIENCE.md),the
[verifier](../../../tools/verify_party_menu_lifecycle.py) covers six complete
bodies. UI internals/delegates/source/query/runtime remain separate.

## Party Setup forwards F without a local table rebuild

Arguments B/F arrive inr3/r4. Retain B;request
BattleMenuDisp_ChangePartySelect_Init(B,incomingF) immediately. F is forwarded
without a separate saved local copy;no intervening service changes it before
this call. There is no direct MakePartyTable/cursor-query request in this wrapper.
AFTER UI Init,read WORD(B+0x1720),OR0x01001000,store it. No explicit state/
party ID/opcode/type/index/count/pending reset. Rawr3 remains UI Init return.
The complete producer is linked by
[operation setup/table](OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md);
UI Init's own provisioning remains a separate contract.

Party End retains B,requests BattleMenuDisp_ChangePartySelect_End(B),then
reads live WORD(B+0x1720),clears ONLY0x00001000 (mask0xffffefff),stores it.
Preserve0x01000000 and unrelated live bits. No local selection/count/index/
state reset. Rawr3 remains UI End return. Producer/selection source kinds must
not be translated or overwritten by this End wrapper.

## Protect Setup queries other unit but does not store it

Retain B. Load WORD(B+0x420),request BattleGetUnitPtr(B,id),then request
BattleGetPartnerPtr(B,thatReturnedU). No own returned-unit null guard;the
second helper has its recovered actor-kind/registry policy. Its returned OTHER
word is not stored in any selected-party or menu field by this wrapper.

AFTER queries,read flags WORD(B+0x1720),OR0x00100000,store them. Then
REREAD the live word,clear0x01000000 (mask0xfeffffff),store again. Preserve
two distinct read/store operations;do not collapse them to a single expression
when preserving address-backed writes. No UI Init,cursor/table lookup,input
state/type/opcode/party ID write,or display request occurs in this Setup.
Rawr3 remains the other-unit helper return,without explicit success assignment.

Protect End is a four-instruction leaf:read WORD(passedB+0x1720),clear ONLY
0x00100000 (mask0xffefffff),store it,return with passedB still inr3. No UI End
service or selected-field/state reset. Its protocol differs from party End's
UI-before-live-mask sequence;the names do not imply identical lifecycle calls.

## Input owns protect confirmation/cancellation

[Complete Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
provides these actual bindings rather than an invented protect Main wrapper:

- State27:ProtectSetup(B),store28,restore helper,then immediate state28 control.
- State28 cancel Trigger0x0200: sound2521,ProtectEnd(B),THEN store20.
- Otherwise confirm Trigger0x0100:store29 BEFORE ProtectEnd(B).
- Neither branch sets Input's local progress flag;pending re-entry or a later
  invocation drives the next state under its complete common-loop policy.

No confirm sound,help request,party ID assignment,or protect-target-table query
is present in this control sequence. [Operation Main](OPERATION_MENU_MAIN_OPCODE_AND_STATE_PUBLICATION.md)
opcode4 selects state27 while retaining menu type;state29's attack selector
uses its own ProtectPartner event/category/source policy. These wrappers do
not create that event themselves. Wider event/source/query domains remain open.

## Port requirements and boundaries

Implement exact UI/request ABIs,live flag timing/two-store protect setup,
helper-return ownership,caller-specific state-before/after-End,and original
0/1 Input progress semantics. UI Main is the separate four-byte stub;UI Init/
End and ProtectPartnerSelectMenuDisp are larger untouched bodies,not no-op
assumptions or evidence of complete original visuals. Reuse assets and dialogue.
Party key1 weapon delegate/source,dynamic provisioning,remaining multi-item/
query-event domains,Game/platform/original runtime/full-game requirements remain open.
