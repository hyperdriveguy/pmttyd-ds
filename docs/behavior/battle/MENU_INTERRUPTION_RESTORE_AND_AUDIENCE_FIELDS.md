# Interruption restore and audience present-field ownership

Complete local leaves supporting [audience type11](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md)
and [gift type12](GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md).
US G8ME01 revision0,static disassembly only.
[Verifier](../../../tools/verify_menu_interruptions.py) covers all ten recovered
bodies,243 instructions,ten service identities,and three startup/numeric spans.
No whole audience lifecycle/source/runtime claim follows from these leaves.

## Saved Input state is separate from previous menu type

`_commandRestoreRec`8011f5f0/0x3c reads current global battle B from8041eb90.
Read U8(B+0x1c74) pending byte. Zero loads current WORD(B+0x171c) (Input
state),then stores it to WORD(B+0x1c70) as saved state,and returns. No type
read,menu reset,or pending-byte write occurs in this branch.

Nonzero pending:load saved-state WORD(B1c70) first,then current Input-state
WORD(B171c). If unequal,return immediately,preserving both pending and saved.
If equal,store WORD(B1c70)=0 THEN BYTE(B1c74)=0,and return. Equality is a
word comparison;do not add a supported-state range or menu-type predicate.
There is no explicit portable success result. On the pending branch rawr3
is the loaded saved-state word;on the zero branch r3 is incoming register state.
Callers ignore it. The global B is recaptured on EACH helper invocation.

By contrast B1c60 is PREVIOUS menu TYPE,used by
[Input state0 restoration](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md).
Do not merge B1c70/B1c60 into one “previous menu” field. Input common pending
re-entry calls this helper before redispatch without resetting its local progress
flag. This helper can clear pending at equality,allowing the normal progress
path to resume. Unequal state leaves pending active;there is no local retry cap.
Neither caller nor leaf proves every source transition eventually resolves it.

## Current audience base getter

Complete BattleAudienceBaseGetPtr801a86a0/0x0c:
load CURRENT global battle B from8041eb90,return wrap32(B+0x2858).
No null guard,no read of a pointer stored at B2858. This is an embedded work
address. Every present getter/setter calls this leaf anew,so their owner can
differ from the captured battle in an interruption caller.

Complete getters each use this returned A,add0x10000,then read:

| Function / body | Return field |
| --- | --- |
| BattleAudience_GetPresentItemNo8019ebcc/0x28 | WORD(A+0x137e4),equivalently currentB+0x1603c |
| BattleAudience_GetPresentTargetUnitId8019eb70/0x28 | WORD(A+0x137ec),equivalently currentB+0x16044 |

These are raw word returns;caller-specific signed bounds happen later.
There is no ID translation,default value,or target-unit registry request inside
the getters. Do not substitute a cached audience owner without source proof.

Complete BattleAudience_SetPresentItemNo8019ebf4/0x34 retains its argument I
BEFORE calling BaseGetPtr,then stores I unchanged to WORD(A+0x137e4).
It does not clear the target field or modify present-valid flags. Its rawr3
remains the intermediate A+0x10000 address,without explicit status assignment.
The two recovery branches pass0;other callers are not censused in this slice.

## AllEnd is a flag mask leaf

BattleCommandDisplay_AllEnd8012081c/0x18 reads WORD(passedB+0x1720),
stores its AND with0xffeeeeee,then returns with passedB still inr3.
There are no UI,resource,actor,type/state,or cursor operations. Preserve live
flags and this exact mask. ActMenuEnd and WeaponSelectMenuEnd are distinct
larger wrappers that request their own UI services before clearing a single bit.
This leaf's name is insufficient evidence of those same requests.

## Concrete native requirements and remaining source proof

Provide distinct packed fields for current Input state,saved Input state,pending
byte,current/previous menu TYPE,and embedded audience present words. Retain
interruption-captured B while allowing helper queries to recapture global B;
implement byte/word store order and route-specific flags/returns. Reuse original
present data,source IDs,effects,and text through documented services.

The local type11/type12 writers and their restore dependencies are now complete.
Audience_GetItemOn/SetTarget core admission,original present producers,remaining
confusion/menu writers,party callback,query-specific source-event domains,
Game/platform/original runtime and full-game requirements remain open. Known
writer paths do not authorize fabricated cursor/weapon defaults when a query
wrapper leaves its own private stack output uninitialized.
