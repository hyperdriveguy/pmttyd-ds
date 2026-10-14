# Battle ending admission: wait gates and conclusion callers

Static US G8ME01 source for [RESULT-01](../../requests/RESULT-01.md).
[Evidence](../../../tools/battle_end_admission_evidence.json) pins three scoped
admission regions, entry/return ownership, dispatch tables and the complete
aligned direct DOL call census of BattleCheckConcluded. Controller hashes also
pin their enclosing bodies; this does not claim complete controller recovery.
No original execution, tick cadence or tutorial configuration is measured.

## Two turn wait gates

btlseqTurn8011c510/1ed8 captures its incoming B in GPR31 before querying
BattleGetSeq(B,3). Subtract03000000 as a word; unsigned index above77 exits.
Otherwise table80363cc8 selects one entry. The following gates retain that B
across all calls and use their returned scalars without Boolean truncation.

| Region | How the scoped route is admitted |
| --- | --- |
| 8011c69c..8011c6ec | Table indices7 and odd11..73 directly select8011c69c |
| 8011db0c..8011db5c | Index9 selects8011da2c. Its U16(B+2) value>=2 branches directly to8011db0c; values0/1 run separate unanalysed preparation/wait regions |

At either gate, request BattleWaitAllActiveEvtEnd(B). Zero goes straight to
return8011e3d4; no conclusion query or local sequence change. Any nonzero calls
BattleCheckConcluded(B). Its zero result requests BattleIncSeq(B,3) once,
then returns. Any nonzero result requests, in this exact order:

1. BattleSetSeq(B,0,3).
2. BattleSetSeq(B,7,07000000).
3. Return through8011e3d4.

The paired writes are sequential service calls, not an atomic publication.
There is no local end-handler call, recursive turn call, result-record write,
reward, field scene, story or save-unblock write on either scoped gate. The
zero-conclusion increment also returns without immediate recursive progression.
[Sequence leaves](BATTLE_SEQUENCE_FIELDS_AND_INIT_SOURCE_BINDINGS.md) own the
field mappings/increment mechanics; [event barriers](BATTLE_WAITS_AND_CONCLUSION.md)
own cleanup and reaction queues. Index9's unanalysed preparation must remain a
separate requirement; its complete table entry is not replaced by this tail.

## First-act conclusion route

btlseqFirstAct8011e5c0/7f8 retains incoming B in GPR30, captures
WORD(WORD(B+2738)+8) in GPR31, then queries BattleGetSeq(B,2). Subtract02000000
as a word; unsigned index above14 exits. Table80363e00 index3 selects8011e9d4.
The captured record child is not used by this scoped conclusion region.

Call BattleCheckConcluded(B) directly here: this region has no preceding local
BattleWaitAllActiveEvtEnd call. A nonzero result calls BattleAudience_PerAct
at8011e9e4, then restores retained B for the same ordered bank0=3/bank7=07000000
writes, then returns through8011eda4. Audience's return does not veto either
write. Its internal side effects and global-owner behavior remain its contract;
do not turn this request into a silent no-op.

A zero result instead requests BattleSetSeq(B,2,02000004) at8011ea1c and falls
through to later first-act handling outside this delivery. It does not take
the ending writes. This route therefore differs from the two turn gates both
in its barrier placement and its zero-conclusion continuation.

## Complete direct DOL conclusion-query census

Scanning all seven DOL text sections at aligned words for bl/bla targeting
8011aa34 yields exactly these ten sites. This is a call-site census, not proof
that every caller admits ending or that every site executes in the tutorial.

| Site | Containing function | Boundary |
| --- | --- | --- |
| 800f796c | BattleAfterReactionMain | Existing reaction/barrier contract; no ending inference from this query |
| 8011ba4c | btlseqAct | Query site only in this delivery |
| 8011be74 | btlseqMove | Query site only |
| 8011c130 | btlseqMove | Query site only |
| 8011c6b0 | btlseqTurn | First scoped wait gate above |
| 8011db20 | btlseqTurn | Second scoped wait gate above |
| 8011e9d8 | btlseqFirstAct | Scoped first-act ending route above |
| 8011eaa0 | btlseqFirstAct | Different query site; not covered by index3's ending rule |
| 80146854 | _object_fall_attack | Query site only |
| 80147d10 | _nozzle_work_effect | Query site only |

The census excludes REL imports, indirect calls, non-link branches, inlining
and unsymbolized aliases. It cannot prove the absence of another producer or
runtime reachability. In particular, do not apply the first-act index3 ending
rule to the later first-act query merely because it calls the same predicate.

## Composition and remaining requirements

[Conclusion publication](BATTLE_CONCLUSION_GROUP_PUBLICATION_AND_FRESH_READS.md)
updates group result words and returns count>=2 OR flags10/20. Its return alone
writes no battle result. These admission callers bridge selected routes to the
[manager](BATTLE_SEQUENCE_MANAGER_BANKS_AND_OUTER_HOOK.md): a later invocation
that captures bank0=3 calls btlseqEnd. Its
[initial result publisher](BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md)
uses captured record/group/override priority. Do not redispatch ending inside
these gates or synthesize victory from the conclusion Boolean.

[Upper Main](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md) has a separate
GP-word1000/2000 forced-end route, including flags OR60; this is not the normal
turn barrier above. Result values, completion/ready flags, sequence4, field wait,
story5 and save admission remain different publications with different owners.

Implement the local retained owners, call order, raw nonzero predicates, table
bindings and return/fallthrough distinctions. Actual tutorial groups/status27,
death/override writers, remaining controller paths, complete ending/rewards/
field/story/save composition and matched original TRACE-01 execution stay open.
