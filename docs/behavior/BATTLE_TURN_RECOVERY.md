# Turn sequence status recovery phases

US G8ME01 revision0, inspected 2026-10-12. B evidence is original DOL
instructions/data. This specifies the recovery region of btlseqTurn
(0x8011c510,size0x1ed8), its dispatch ordering and wait boundary. It does not
claim the whole turn controller is recovered. Identity/provenance are in
[ORIGINAL_BEHAVIOR.md](../ORIGINAL_BEHAVIOR.md); original extracts remain ignored.
No original execution or Switch comparison was performed.

## Call-site inventory and timing boundary (B)

A scan of all seven DOL text-section slots for PowerPC opcode18 direct branches
to BtlUnit_CheckRecoveryStatus(0x80127890) found24 sites, all link branches:
23 in btlseqTurn and one at0x800fe8fc in BattleDamageDirect. This inventory
excludes REL relocation targets, indirect calls, event-script function operands
and runtime code generation. It is not proof no other caller exists.

The direct-damage call is the separately documented
[Sleep interruption](BATTLE_STATUS_STORAGE.md). The turn calls occur in discrete
sequence states, not every renderer frame. In particular, waiting on events
must not repeat the preceding recovery pass. Establishing when the outer battle
controller enters or re-enters this sequence remains a separate task.

## Dispatch and status order (B)

btlseqTurn reads BattleGetSeq(B,3), subtracts0x03000000 and accepts unsigned
indices0..0x4d. It dispatches through78 words at US0x80363cc8. Outside that
range it exits without executing a case. Table SHA256:
`42ae54671dbfed589235c6a229767eba9b48984a21a16ee5d10788bc9c85343f`.

Recovery occupies the following even-index cases. Full state values are
0x03000000 plus the hexadecimal index below; status IDs are decimal.

| Index | Entry address | Status ID | Helper call site |
| --- | --- | --- | --- |
| 12 | 8011c6f0 | 1 Sleep | 8011c710 |
| 14 | 8011c7a8 | 2 Stop | 8011c7c8 |
| 16 | 8011c868 | 3 Dizzy | 8011c888 |
| 18 | 8011c928 | 5 Confuse | 8011c948 |
| 1a | 8011c9e8 | 6 Electric | 8011ca08 |
| 1c | 8011caa8 | 7 Dodgy | 8011cac8 |
| 1e | 8011cb68 | 8 Burn | 8011cb88 |
| 20 | 8011cc28 | 9 Freeze | 8011cc50 |
| 22 | 8011cd50 | 10 | 8011cd70 |
| 24 | 8011ce10 | 11 | 8011ce30 |
| 26 | 8011ced0 | 4 Poison | 8011cef0 |
| 28 | 8011cf90 | 12 | 8011cfb0 |
| 2a | 8011d050 | 13 | 8011d070 |
| 2c | 8011d110 | 14 | 8011d130 |
| 2e | 8011d1d0 | 15 | 8011d1f0 |
| 30 | 8011d290 | 0 Allergy | 8011d2b0 |
| 32 | 8011d350 | 23 HP regeneration | 8011d36c |
| 34 | 8011d410 | 24 FP regeneration | 8011d42c |
| 36 | 8011d4d0 | 18 Invisible | 8011d4ec |
| 38 | 8011d590 | 19 Fast | 8011d5ac |
| 3a | 8011d650 | 20 Slow | 8011d66c |
| 3c | 8011d718 | 21 Payback | 8011d734 |
| 3e | 8011d7d8 | 22 Hold Fast | 8011d7f4 |

Friendly names are annotations; shared fields and sign filtering for10–15 are
in [status storage](BATTLE_STATUS_STORAGE.md). This sequence does not invoke
recovery for16,17,25,26 or27. Do not decrement all numeric status IDs in a
single generic loop or assume the order is numeric.

The immediately following odd indices13..3f all dispatch to the common wait
case0x8011c69c. Lower indices contain periodic damage/regeneration and other
turn preparation; their detailed contracts remain open. Their dispatch places
those phases before these recovery passes, so changing this order risks
changing the final tick of a finite-duration status.

## Ordinary recovery pass (B)

Each phase scans slots0..63 ascending via BattleGetUnitPtr(B,index). States
32..3e explicitly skip null units. Earlier phases pass null through to
CheckRecoveryStatus, whose own null guard returns0. The helper additionally
gates on status27 and positive HP and performs duration/strength mutation as
specified in the storage contract. Do not add a separate allegiance filter.

For every nonzero recovery result, except Freeze's special case:

1. For Slow only, call BtlUnit_ResetMoveStatus(unit) first (0x8011d680).
   Its internal behavior still needs recovery; Fast has no corresponding
   explicit extra call in this caller.
2. Request `evtEntry(0x80353704,10,0)`, symbol subsetevt_status_recovery.
3. If allocation succeeded, copy event+0x15c into unit+0x2a8, and unit+0 into
   event+0x160. No local cancellation/clearing of a prior unit+0x2a8 occurs.
4. Mark that the phase had a recovery even when event allocation failed.

The phase OR-accumulates helper results across slots. If nonzero, play
SFX_REST_CONDITION1 once after the complete scan (key US0x802efb80).
This is one sound per status pass, not one per unit. The event body, animation
selection, message behavior and eventual clearing of event handles remain open.

If no recovery occurred, increment sequence channel3 twice and immediately
call btlseqTurn(B) recursively, skipping the associated wait state. Otherwise
increment channel3 once. Sleep is an exception: it then exits without
recursing (0x8011c7a4), leaving the wait for the next outer invocation. Other
recovery phases recurse into their wait state and return after that call. These immediate transitions mean one outer invocation
can traverse many empty phases. Do not impose one-render-frame-per-state delays.
Each recursive invocation initializes its local aggregate/changed flags to0.

## Freeze expiry exception (`0x8011cc28`–`0x8011cd4c`)

For each slot where CheckRecoveryStatus(unit,9) returns nonzero:

- If BtlUnit_CheckShadowGuard(unit)=0, call
  `BattleDamageDirect(−5,unit,null,1,0,282,0,1)` before presentation.
- Regardless of that gate, get unit position and integer height, set y+=height,
  then call effFreezeN64Entry with integer arguments(0,1), floats(x,y,z,1.0).
  The scale constant is binary32 at0x8042256c.
- Play SFX_CONDITION_BREAK_ICE1 (US0x802efb94) in3D at the adjusted position.
- Mark that the phase had a recovery and continue through the remaining slots.

This branch does not create subsetevt_status_recovery or play the generic
recovery sound. It uses the same one-versus-two sequence increment rule.
The one-damage request goes through full direct-damage handling, so clamping,
bookkeeping, Sleep interruption and lethal event handling follow that routine;
it is not a raw HP decrement. The shadow-guard predicate is specified in
[periodic damage](BATTLE_PERIODIC_DAMAGE.md).

## Shared event wait boundary (B)

At0x8011c69c call BattleWaitAllActiveEvtEnd(B). If0, exit and leave sequence
unchanged. Otherwise call BattleCheckConcluded(B): if nonzero, set channel0=3
and channel7=0x07000000, then exit. If zero, increment channel3 once and exit,
without immediate recursive progression. BattleCheckConcluded internals and
conclusion sequence are not inferred merely from these names.

BattleWaitAllActiveEvtEnd at0x8011ace8,size0x104 first calls
BattleWaitAllActiveEvtEnd_NoBgSetEndWait(B). A zero result immediately returns0.
Otherwise inspect event-ID words at B+0x182b4,0x182c4,0x180fc,0x182c8,0x182c0
in that order. For each nonzero ID call evtCheckID(0x80036604): if nonzero,
return0 immediately; otherwise clear that stored word and continue. Return1
when all are absent/finished. Later fields are not examined or cleared after
an earlier live event. The initial helper's unit/background wait rules remain
unrecovered, so this is the exact outer protocol, not complete wait semantics.

BattleIncSeq(0x800f895c,size0x44) obtains the requested channel through
BattleGetSeq, adds1 as a word, and calls BattleSetSeq. The getter/setter
implementations are fingerprinted; other channel users and the outer driver
remain part of turn-controller recovery.

## Verification obligations and next recovery (P)

Compare a run with several statuses expiring across multiple slots: finish all
units for one status before proceeding to the next; play one generic sound per
changed pass; preserve event allocation failure handling and wait barriers.
Test no-recovery phases recurse immediately and changed Sleep defers its wait; dead/status27 units do not mutate;
Freeze expiry can produce direct-damage events and skips the generic recovery
event; Slow's explicit move reset precedes its recovery event. These are derived
cases, not original execution traces.

Still required: periodic Poison/Burn/HP/FP phases and their effects, outer turn
entry/reset cadence, generic recovery event body, move-reset and shadow-guard
helpers, initial unit-event wait helper, conclusion handling and phase/action
controller. A dispatch table or call inventory alone does not establish all
turn behavior. Complete-game requirements remain dependent on those contracts.

Poison and Burn phases are now specified in [periodic damage](BATTLE_PERIODIC_DAMAGE.md);
HP/FP regeneration and other remaining turn phases are still open.

Status HP/FP regeneration and Happy badge phases are now specified in
[regeneration](BATTLE_REGENERATION.md); their event/effect dependencies and
outer turn cadence remain open.

The recovery and Happy event bodies are now documented in
[recovery events](BATTLE_RECOVERY_EVENTS.md), with their remaining motion, pose
and data-binding dependencies kept explicit.
