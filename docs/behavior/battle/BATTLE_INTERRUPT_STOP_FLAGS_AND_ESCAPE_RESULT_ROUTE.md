# InterruptStop flags and the original Escape result route

Static US G8ME01 source for [RESULT-01](../../requests/RESULT-01.md).
[Evidence](../../../tools/battle_interrupt_stop_evidence.json) covers complete
btlevtcmd_InterruptStop80105368/78, scoped Escape script control/publication
words and an annotated DOL event-stream census. Complete callback recovery is
not complete escape mechanics, tutorial reachability or original execution.

## Complete callback: raw call gate and fresh battle owner

Retain incoming r4 as GPR31. Read operand0 from WORD(WORD(event+18)+0) and
always request evtGetValue(event,rawOperand). Even a zero call gate resolves
its operand; do not skip this service or fabricate a resolved value.

After evtGetValue returns, load current B=WORD(8041eb90), using startup
r13=8041cf20. This capture occurs after operand resolution, including on
no-write paths. There is no battle-null guard. The original source then uses:

| Retained raw r4 | Resolved signed operand | Local publication |
| --- | --- | --- |
| 0 | any | No flags read/store |
| any nonzero | exactly0 | Read current B+ef4, OR10, store B+ef4 |
| any nonzero | exactly1 | Read current B+ef4, OR20, store B+ef4 |
| any nonzero | other signed word | No flags read/store |

All routes restore saved LR/GPR31/stack and explicitly return2. No evtSetValue,
result-record publication, sequence change, event deletion, wait, reward or
story/save write occurs. A nonzero gate is not tested for exactly1; a later
invocation with nonzero r4 can OR the same bit again. This delivery keeps r4
raw; VM first-call/continuation policy remains the caller's contract.

Both writes preserve other flags. The battle owner is captured only after
resolution; do not replace it with the original event's unit owner or a pointer
captured before evtGetValue. Reads/writes are ordinary local operations, with
no explicit IRQ masking or atomic/lifetime guarantee.

## Escape script: result-bit split and terminal flag10 request

btldefaultevt_Escape8034e7a4/a44 parses into161 commands. Its full hash pins
identity; only the following control/publication regions are interpreted here.
Unchanged script words and child services own motion/audio/partner/AC behavior.

At8034eb9c request GetResultAC(LW6). At8034eba8 the original IF_FLAG tests
LW6 mask2, not equality with2 or a generic nonzero result.
[IF predicates](../event/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) own its
resolved-mask and false-arm scanning semantics. The mask-set arm
contains the failed-escape presentation and returns through:

- WaitEventEnd(LW15) at8034f038.
- END_EVENT at8034f044, before this IF's END_IF8034f048.

Consequently, that arm does not fall through to the terminal InterruptStop.
When mask2 is clear, the arm is skipped; subsequent original script handling
includes audience_code_escape_success at8034f100, original sound request
at8034f108 and movement requests. These names are public annotations; their
complete implementations/physical presentation are not proved by call words.

The final scoped tail is exact:

1. SET LW15=0 at8034f178.
2. check_battleflag(LW0,400000) at8034f184.
3. IF LW0!=0, BROTHER_EVT_ID(LW15), RUN_CHILD(_event_MajinaiEnd),
   END_BROTHER, END_IF.
4. IF LW15!=0, WaitEventEnd(LW15), END_IF.
5. InterruptStop(0) at8034f1d4.
6. END_EVENT8034f1e0; END_SCRIPT8034f1e4.

The child-ID wait can delay the request; it is not elided. Mask400000 in this
script is separate from result-priority mask40. In particular, ORIS0040 in
BattleMajinaiEndCheck means400000, not40; its
[existing contract](JUMP_MAJINAI_POWER_CHECK_CHILD.md) owns that producer.

When the actual VM invokes InterruptStop with a nonzero gate, this literal0
operand publishes flag10 to the post-resolution battle owner. Zero gate still
resolves but publishes nothing. Thus this is an original Escape-domain producer
of10, with an explicit AC failure-bit exclusion. Do not infer automatic success
from HP0, classify every nonzero AC result as success, or bypass the original
presentation/child waits.

## Bounded caller census and result composition

Among all736 symbol-annotated `_main` DOL event streams of type `evt`, parsing
candidate streams and requiring USER_FUNC opcode91/callback80105368 yields
exactly one site: Escape8034f1d4 with literal operand0. This census excludes REL
streams, unannotated data, runtime-built scripts and machine-code/indirect uses.
It does not prove there are no operand1 callers elsewhere or that the tutorial
admits the Escape menu/action.

[Conclusion](BATTLE_CONCLUSION_GROUP_PUBLICATION_AND_FRESH_READS.md) returns1
when flags10/20 are present. [Scoped admission](BATTLE_END_ADMISSION_GATES_AND_CONCLUSION_CALLERS.md)
then bridges selected caller states to ending. The
[result publisher](BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md) chooses4
for flag10 only after flag40, flag20 and both group-conclusion tests fail.
Accordingly, Escape flag10 is one verified source route to result4, subject to
those higher-priority outcomes. It is not proof that result4 universally means
escape, nor that4 cannot have another producer. Flag20's local producer accepts
operand1, but its gameplay meaning and callers remain open; do not label5
as timeout. Flag40's semantic producer also remains open.

Port the raw gate/operand distinction, mandatory resolution, fresh owner, OR
semantics, result-bit branch and child wait. Remaining work includes complete
Escape helper/event composition, VM admission, other producer domains, tutorial
restrictions, complete ending/rewards/field/story/save and original TRACE-01.
