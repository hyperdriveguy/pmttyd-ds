# Battle conclusion group publication and fresh reads

US G8ME01 revision0; recovered2026-10-13 for
[RESULT-01](../../requests/RESULT-01.md). Complete BattleCheckConcluded8011aa34/
180hex (96 instructions). Static original code, not tutorial death/outcome timing.
Addresses/fields/masks hexadecimal; indices/counts decimal. Strengthens the
[existing conclusion contract](BATTLE_WAITS_AND_CONCLUSION.md); read
[result publisher](BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md).

## Three groups publish independently

Capture supplied battle B/GPR28. Group g0,1,2 uses record B+8*g/GPR31:
configuration U16 at+8 and concluded word at+c. FIRST clear each concluded word
at8011aa5c, then read configuration. Exact0 stores1/count++ and skips both scans.
Other configuration tests are the two low bits; unrelated bits do not create
an empty-group fallback. There is no global battle/null/type/extent/IRQ guard.

Each scan visits slots0..63 in order through BattleGetUnitPtr800f88cc(B,slot),
skipping null. Each present unit must have signed bytec equal to current g.
A first qualifying unit ends that scan; scanning does not require every unit
eligible or use a summed HP/death count. All three groups are processed even
if the return condition was already achievable after an earlier group.

## Configuration bit1: status before one attribute snapshot

Test bit1 of the initially read config. If set, reset slot index0 and scan:

1. Resolve unit, reject null or signed membership mismatch.
2. Request BtlUnit_CheckStatus80127814(unit,27); returned nonzero rejects.
3. Read unit.word104 AFTER child into one retained word. MASK00020000 or
   MASK10000000 nonzero rejects. Both exclusions use that same snapshot.
4. First surviving match ends scan with slot<64, then proceed to bit2 gate.

Exhausting slots stores group conclusion1/count++, then skips the bit2 gate
entirely. It does not reread config to reconsider that conclusion.
No local direct HP query, unit removal or destruction. Status semantics/writers
and attribute/allegiance/lifetime domains remain the unit services' contracts.

## Bit2 gate rereads config; its scan straddles the status request

At8011aaf4 reread current group U16 config FRESH. Test MASK2. This read happens
when initial bit1 was clear OR its first scan found a match. A child can have
changed config; stable mathematical vectors do not freeze the second gate.
If bit2 clear, leave group word at the initially published0, then next group.

If set, reset slot0 and scan independently:

1. Resolve unit and test signed membership as above.
2. Read attributes at8011ab28 BEFORE status. Require MASK00040000 nonzero;
   absence rejects WITHOUT calling status.
3. Request status27; nonzero rejects.
4. REREAD attributes at8011ab44 AFTER status. Require MASK10000000 clear.
   Do NOT retest40000 or add the first scan's20000 exclusion here.
5. First match leaves conclusion0; exhausting64 stores1/count++.

A unit can therefore pass this scan with40000 before the child but cleared
afterward, as long as status result0 and new10000000 clear. New10000000 after
the child excludes it. Units qualifying in the two scans need not coincide;
config3 requires BOTH scans to find some match to keep group0. No combined
single frozen eligibility predicate replaces these fresh source reads.

## Return condition differs from result-publication flag priority

After all groups, concluded count>=2 returns1. Count<2 reads B.wordef4 FRESH,
extracts MASK00000030 (10|20) and normalizes nonzero to1/zero to0. MASK40 alone
does NOT force this helper's return1. Return is normalized0/1, not a group
index or final battle result code. Count includes configuration0 groups.

This function writes only the three concluded words via local stores; no
I10 result, ready80000000, sequence/story/GSF/save/reward publication. The
result producer later reads side words with its own captured flag40/20/10
priority. End-bank entry and override writers can create paths not implied by
this Boolean alone. Native enemyHP==0 must not replace this group service.

ABI48-byte frame/GPR25..31/LR restored. Four direct call sites (two lookup/two
status), no indirect calls/wait/clock; no measured event/death cadence inferred.

## Verification and remaining RESULT-01 work

[Manifest](../../../tools/battle_conclusion_publication_evidence.json):three
spans/all96 words/four calls. Nine stable config/seven return/four attribute-
snapshot mathematical vectors, exact group/slot limits/result writes/config/
attribute/status/return mask/call sites independently checked against the ISO.
These vectors are deductions, not physical unit/status mutation observations.
Raw ELF/listing/generator stay `/tmp`; no assets/raw bodies committed.

Local conclusion producer is now explicitly bound to RESULT-01. Remaining:
actual tutorial group configurations and unit/status/attribute/death producers,
end-bank admission/override semantics, full ending/rewards/field/story/save
composition, valid pointer/value/alias/lifetime/IRQ domains and original
execution/native result binding. RESULT-01/DEMO-01/full-game remain OPEN.
