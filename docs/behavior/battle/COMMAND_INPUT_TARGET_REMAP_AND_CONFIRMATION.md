# BattleCommandInput state11: target remap, navigation, and confirmation

Companion to [entry/re-entry](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md),
covering80122d20..80122fd4 of the complete US G8ME01 Input body.
[Verifier](../../../tools/verify_command_input.py) pins every instruction.
Static evidence only; sampling, status, audience, cost, audio and UI services
remain separate contracts. B/U/M/SP08/P have the retained meanings there.

## Pair capture before sampling refresh

Retain current type T=WORD(M540), then retain BUF=wrap32(B+0x428).
Request GetCursorPtr(M,T,&SP08), ignoring return. Before sampling refresh:

1. Read BYTE(BUF+0xab7), sign-extend it as selected permutation index I.
2. Read S8(BUF+0xa6d+I) as record ordinal J.
3. Derive Q=wrap32(BUF+4+36*J); retain S16(Q) as unitId and S16(Q+2)
   as partId. Neither index is bounds checked.
4. Request BattleSamplingEnemyUpdate(BUF). Ignore return.

After update, read S8(BUF+0xa6c) once as retained count N. Start local K=0.
If N>0, scan exactly that captured count:read each live permutation byte
S8(BUF+0xa6d+K), derive its live row, compare signed unitId first; only a
unit match reads/compares signed partId. Stop at the FIRST matching pair.
On no match, or nonpositive count, local K reaches/exceeds signed N and is
replaced by0. Store low8(K) to BUF+0xab7. Preserve the original unit/part
pair captured before update, not the old ordinal or a fresh lookup afterward.

Now read current cursor from SP08 and its index;derive M+0x80+28*index,
read weapon pointer, then current WORD(weapon+0x64). Bit0x01000000 gates
both directional controls. No weapon/target/count admission guard is added.

## Independent forward and reverse requests

If the weapon bit is set, request BattlePadCheckRepeat(0x00080000).
Nonzero reads current selected byte UNSIGNED, adds1 and stores low8;then
rereads selected and count as SIGNED bytes. If selected>=count, store0.
Reread signed count;if >=2, request psndSFXOn(802efe98), the retained
`SFX_BTL_CURSOR_MOVE2` key. There is no sound when that count is<2.

Next ALWAYS request BattlePadCheckRepeat(0x00040000), even after a forward
move. Nonzero reads current selected byte unsigned, subtracts1 and stores
low8;then rereads it signed. If negative, read count UNSIGNED, subtract1,
and store low8. Reread count signed;if >=2, request the same sound.
Simultaneous repeat gates can execute BOTH directions, unlike ActMenuMain's
forward-priority `else` route. Sound can mutate the bytes before reverse reads.
If the weapon bit is clear, skip both repeat requests entirely.

## Cancellation takes priority over confirmation

Always request BattlePadCheckTrigger(0x0200). Nonzero:

1. Request psndSFXOn(2521), then TargetSelectMenuEnd(B).
2. Store state6;read live WORD(B+0x0ef4), clear only bit0x4, store it.
3. Request BattleAcHelpSetDispType(0), then SetCommandAnimPose(retainedU).
4. Read current type. T0 with fresh flag0x200, or T1 with fresh flag0x400,
   stores state1 and requests OffLightEnemyInCommand. Other combinations
   leave state6. Go to common tail WITHOUT a P1 assignment.

Cancellation omits the confirmation trigger and audience query below.
For the type/flag test, the current type is reread after pose service; do not
reuse entry T for that check.

## Confirmation and the following audience query

When cancel trigger is zero, request BattlePadCheckTrigger(0x0100).
Nonzero executes in order:

1. Sound2520;BattleStageOffLightInCommand(1,1).
2. Store state29 BEFORE TargetSelectMenuEnd(B).
3. Read live WORD(B+0x0ef4), OR0x4, store it.
4. If RETAINED entryT==2 and fresh S8(U+0x20)==0, inspect U8(M+0x550).
   Exactly2 stores BYTE(U+0x20)=8, then requests
   BtlUnit_PayWeaponCost(U,0x803553a8). Exactly3 stores byte5, then
   requests PayWeaponCost(U,0x80355468). Other values skip cost entirely.
5. Request BattleAcHelpSetDispType(2).

The weapon addresses are literal original pointers, not synthesized menu rows
or default ItemData. This slice does not recover their entire definitions.
The type comparison uses retained T even if menu-end services change M540.
The actor mode comparison and multi-item byte are fresh. No P1 assignment.

After either this confirmation path OR a zero confirm trigger, request
BattleCommandAttackAudienceCheck() at80122fb0. If nonzero, store
BYTE(B+0x1c74)=1, request TargetSelectMenuEnd(B), then HelpSetDispType(0).
Thus an audience service can run after state29 was already stored, and can
cause a second menu-end request. Zero just reaches common tail.
Do not turn confirmation into an early function return or bypass this query.

## Port/source boundary

Implement signed permutation/row addressing, captured pair and count,
first-match remapping, fresh byte rereads, both directional gates, cancellation
priority, retained-versus-current type, cost pointer selection, and the
post-confirm audience call exactly. Empty or malformed counts are not repaired
by this body beyond the documented selected-byte store. Address safety policies
must remain explicit native policies rather than claims of original behavior.

State29 is processed only when common control re-enters or a future invocation
arrives;merely storing29 does not set P1. Pending BYTE(B1c74) can force same-call
re-entry. [Attack dispatch and other states](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
complete the tail. This local route does not prove every reachable original
menu/input/weapon/query domain or complete source-event scheduling.

## Shared implementation and admission boundary (2026-10-14)

[`battle_target_selection.cpp`](../../../source/battle/battle_target_selection.cpp)
implements the complete local state11 route against source-address memory and
explicit services. `processPackedBattleTargetSelection` returns provider
readiness, not Input's original progress value. Refresh, menu-end, command-pose,
cost and audience providers must own their complete invoked services. An
unbound provider stops at that frontier after preceding source effects; this is
not a resumable state machine and callers must not blindly replay the handler.

`finalizePackedBattleCommand` implements state29: the retained actor goes to
attack selection before the two fresh flag reads and light-off request; the
result is the original1. Missing attack-selection readiness returns no result.
The separate original attack selector can publish the Jump event80355cf0,
category0 and usage count without creating an attack root.

Host tests cover pair/part remapping, first matching duplicate, permutation
changes, independent repeat gates, fixed-target gating, cancel priority and
post-pose type reread, confirmation-before-End, fresh flags, retained type for
multi-item cost literals, existing actor-mode exclusion, post-confirm audience
interruption, unbound service readiness and selector-before-final-flags order.
An integration fixture uses the real shared attack selector for the Jump row.
These are deterministic source fixtures, not original-runtime observations.
No provisional timing constant or gameplay result is introduced.

The native Harbor tutorial does not yet invoke these handlers. Its original
class/target presentation, command-pose and complete UI/help owners, common
pending/progress re-entry and selected-player event registration remain the next
integration step. Confirming state29 alone must not synthesize same-call
progress, damage, a root handle, tutorial completion or a field-story advance.
