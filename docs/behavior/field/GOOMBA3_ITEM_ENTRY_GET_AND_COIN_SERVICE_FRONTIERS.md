# Goomba3 item-entry, forced-get and coin-service frontiers

US G8ME01 disc0 revision0, read-only original DOL inspection2026-10-13.
Dependency detail for [Goomba3 quest acquisition](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).
This recovers wrappers and exact service boundaries; it does not substitute
inventory insertion for the complete item-get sequence.

## Item entry and forced get

`evt_item_entry`800aa400/fc resolves operands0..5 and7 through evtGetValue.
Operand6 is passed raw as the secondary entry argument. XYZ operands2..4
are signed integers converted to binary32; flags operand5 truncates to16bits.
Calls itemEntry(name,kind,flags,rawSecondary,resolvedFinal,X,Y,Z), returns2.
For irai_item the source arguments are kind95,XYZ(0,-1000,0),flags16,
secondary-1,final0. This callback does not itself call pouchGetItem.

`evt_item_flag_on`800aa30c/54 resolves name only, looks up itemNameToPtr;
missing entry returns2, present entry ORs raw operand1 truncated16bits through
itemFlagOn800aa56c/10, then returns2. Thus source flag2 is an entry flag,
not an immediate inventory mutation.

`evt_item_get_item`800a9f98/ac re-resolves name/entry each invocation.
On firstCall it calls itemForceGet800aa62c/34 and returns0. This first path
has no null-pointer guard before that call: native entry admission must prove
an actual created entry. itemForceGet clears entry words78/7c, sets half24=2,
half26=0,ORs mainhalf0 with1000 and clears statusword38 bit10000.

Subsequent calls return2 when entry missing, activehalf0 bit1 clear,
itemStatus(entry,2) nonzero, or itemStatus(entry,10000) nonzero; otherwise0.
itemStatus800aa530/c reads word38&mask. The event waits for item-service
state, rather than granting the item inside this wrapper. Source caller writes
GSF5405 only after this wrapper returns2.

`itemMain`800adfe0 reaches itemseq_GetItem800ab930/1324 for sequence2.
The latter dispatches substatehalf26 through table80313e68. In the ordinary
presentation branch, source800abc88..abccc installs camera8 flag200,
marioKeyOff, marioStSystemLevel(2),psndSetFlag(80), and clears Mario motion
floats174/17c/180/18c before later Mario get-item motion, effect/icon/window
work. Actual pouchGetItem(kind) occurs later at800ac134; a distinct shortcut
branch calls it at800ab9e4 when entryflag4 is set. Neither Goomba3's original
16|2 flags nor forced1000 establish that shortcut. Failed pouchGetItem and
callback/switch ownership, nine-way substate progression, discard UI and
exact get-item-window/icon/effect/audio timings remain to be recovered before
claiming a finite95 acquisition implementation. No inferred parameter-revision
wrapper is supplied here.

## Coin20 is an acknowledged presentation service

`evt_sub_get_coin`80054280/4f8 resolves amount each invocation. FirstCall
initializes event work78=0, allocates effItemGetEntry(7) into7c, zeros80/84,
allocates8bytes into88. Mario/camera-relative presentation placement is
computed separately; its numerical curve is not reduced to a guessed offset.

State0 creates original icon descriptor403, positions it and sets iconflags102,
sets effectwork+38=5,requests Mario get-item motion, opens original window
with parameter{amount,0}, calls source sound/BGM routing, records clock,
sets wait200ms/state1 and returns0. State1 advances only elapsed>200ms:
clears iconflag2,sets parameterword1=1,records wait500ms/state2,returns0.
State2 advances only elapsed>500ms tostate10,still returns0. State10 waits
keyGetButtonTrg(0)&0x100 (A), setsstate100 and returns0.

State100 calls **pouchAddCoin(amount) exactly here**, effSoftDelete(effect),
iconDelete, Mario stay motion and sound cleanup, closes window,
sets parameterword1=0,state101 and **returns2**. The ordinary caller may resume
its quest writes on this return; do not delay them waiting for state101.
State101, if subsequently invoked, waits winMgrAction()==0 before freeing
8byte work and returning2; otherwise0. This documents source behavior without
inventing a VM cleanup continuation after callback completion.

`pouchAddCoin`800d3e18/4c reads signed16 pouch+78, adds amount, stores low16,
then rereads signed16 and clamps negative to0 and greater999 to999. Returns
final signed16. Truncation happens before clamping; an unbounded mathematical
integer clamp does not preserve malformed/large-amount source semantics.
No Mario/party parameter revision call occurs in this leaf or coin callback.
Quest2 caller afterwards clears1420,sets5376,prints sys_request_finish;
those writes belong to the caller, not pouchAddCoin.

Evidence: tools/inspect_dol_function.py freshly read these named DOL spans
from the read-only ISO into ignored artifacts/reference/goomba3-services;
itemMain/itemseq_GetItem were also inspected to establish wrapper ownership
and the explicit remaining frontier. Documentation only, no native build,
original execution, presentation parity or hardware validation claim.
