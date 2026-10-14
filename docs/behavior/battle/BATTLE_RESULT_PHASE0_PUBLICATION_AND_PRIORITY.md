# Battle result phase0 publication and priority

US G8ME01 revision0; recovered2026-10-13 for
[RESULT-01](../../requests/RESULT-01.md). Complete result set/get/enemy-belong
leaves, scoped btlseqEnd prologue/initial phase through8021534c and direct DOL
setter census. Addresses/masks/fields hexadecimal; counts/codes decimal.
Static source, not original tutorial timing/outcome/field-return observation.
Read [conclusion service](BATTLE_WAITS_AND_CONCLUSION.md),
[existing result/ending composition](UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md),
[field protocol](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).

## Three complete leaves and exact result record

BattleInformationSetResult80114508/8hex stores RAW supplied r4 to I.word10,
returns without a guard, callback or independent scalar result.
BattleInformationGetResult80114500/8hex loads I.word10 into r3, returns.
No validation/enum narrowing/sequence/story/flag/death/save/IO occurs locally.

BtlUnit_GetEnemyBelong80127454/2chex reads unit BYTEc and sign-extends to r3:
0→1,1→0, every other signed value unchanged. Source conditional LR returns
have no calls or stack. This is not a generic modulo/binary-negation mapping,
and no0..2/belong/unit-pointer check exists. Negative/other returned indices
can form wrapping addresses in the caller; legitimate unit domains remain
providers, not a new native clamp.

## Ending entry capture precedes initial-phase result selection

btlseqEnd80215234/1470hex prologue captures supplied battle B/GPR26,
B.wordf28 into GPR28 and B.word2738 result-info pointer I/GPR27. It requests
pouchGetPtr800d5eb0, retaining P/GPR31, then _audience_Whistle_control80212890,
then BattleGetSeq800f89a0(B,7). Subtract07000000 from returned raw word and
compare unsigned<=31; otherwise skip table to existing ending tail.
Table803b0c88 first entry is8021529c, the phase covered here. Other31 ending
entries are NOT interpreted by this delivery merely because their table hash
and containing function are pinned.

I is captured BEFORE these children. Initial phase requests, in order:
btlsubResetMoveColorLvAll80125398(B), BattleGetMarioPtr800f87a8(B), read returned
Mario signed bytec into ownBelong/GPR28, then BtlUnit_GetEnemyBelong(Mario).
Retain its raw returned enemyBelong in r3 until the applicable indexed read.
Read B.wordef4 ONCE into r5 afterward; this captured flags word owns all three
flag tests below. No death/HP/sequence validity recomputation here.

## Five ordered result alternatives, including no-write path

| Priority / source condition | Result / setter site |
| --- | --- |
| Captured flags&40 nonzero | 4 at802152cc |
| Otherwise flags&20 nonzero | 5 at802152e4 |
| Otherwise B.word(wrap32(c+8*ownBelong))!=0 | 3 at80215308 |
| Otherwise B.word(wrap32(c+8*enemyBelong))!=0 | 1 at8021532c |
| Otherwise flags&10 nonzero | 4 at80215344 |
| None above | No result store; retain prior I10 |

Every setter uses captured I, not a freshly reloaded B2738. Own/opponent words
are each read only if reached, AFTER the preparation children. No null/extent/
index/atomic/IRQ/generation/status guard. Result4 can arise from two distinct
flag branches. Result5 precedes side conclusion; simultaneous side conclusions
choose3, never1. A preexisting result can survive the no-write path, so do not
implicitly clear I10 at phase entry.

After the selected setter returns (or no-write path), request
BattleInformationGetResult(captured I) at8021534c. Later ending decisions own
that reread; it is not a locally retained result constant if callbacks/other
owners mutate I. This scope stops before that return value's downstream routing.
No story/GSF/save-bank write or field-ready flag in this publication region.

In the recovered valid conclusion/team domain, code1 is opponent-side victory
with own-side-loss priority; code3 selects own-side conclusion. It is not
“any nonzero result means victory,” or an enemy HP counter check. Exact physical
death/end ordering requires the conclusion producer and end-bank admission.
Do not assign flee/timeout/forced-abort labels to4/5 solely from their numbers:
flags10/20/40 writer domains must establish those meanings. Result0 alone is
not a victory producer, and no-write is distinct from publishing0.

## Complete direct setter census and field handoff boundary

All seven DOL text sections contain nine direct branch-with-link requests to
SetResult: fbatBattleMode80046c6c/7c, evt_npc_battle_start8004efd0,
BattleInformationInit80114554, and the five btlseqEnd sites above. Every call
word is pinned. This is a direct-call census, not exclusion of inline stores,
REL/indirect pointers or aliased record writers. Non-ending caller conditions/
values remain their own contracts; names alone do not prove initialization.

The field waiter requires sequence2 AND a nonzero result, then requests control/
music/field postprocessing. It does not locally test exact result1. Ending
completion (Bef4 mask80000000), sequence-driver field return, and I10 publication
are separate services. The ending region here cannot be replaced by immediate
result1/scene5/save-unblock when enemyHP becomes0.

The original harbor beforebattle stream later writes story5 after its field
wait; the [harbor graph](../story/STORY_HARBOR_OPENING_BINDINGS.md) records that
source ordering. This document does not close all tutorial joins/rewards/save
admission or claim the phase itself advances story. Original scene/message keys
and predicates remain intact; no fabricated battle outcome or plot event.

## Verification and remaining RESULT-01 work

[Manifest](../../../tools/battle_result_phase0_evidence.json):ten spans/91
anchors:15 complete leaf words/71 ending-prefix words/one first table word/four
additional setter-census words. Twelve scoped direct calls; nine census sites;
no indirect calls, one CTR table dispatch in the prefix. Six enemy-belong/ten
priority/no-write mathematical vectors, masks/captures/indices/set/get sites,
original table target and exhaustive direct census checked independently.
Containing ending hash is not full semantic coverage: original full size
1470hex is1308 instructions, of which only71 are interpreted here. Raw ELF/
listings/generator stay `/tmp`; no assets/raw bodies committed.

Remaining: ending admission and all conclusion/death/override flag writers,
full tutorial-context pointer/value/lifetime/alias/IRQ domains;4/5 semantic
classification; reward/ending/sequence/field/story/save-unblock composition;
original input/outcome/story/flag/save traces and native real-result binding.
RESULT-01/DEMO-01 and full-game remain OPEN.
