# Complete general action-record rule judgment

US G8ME01 revision 0; recovered 2026-10-12 by documentation agent ID 2.
Complete `BtlActRec_JudgeRuleKeep` at0x8018ef8c, size0x7f0; full 68-entry
jump table at0x803977b0, three called rule helpers and both initial unit queries.
Verify: `python3 tools/verify_battle_general_rules.py` pins eight complete or
linked body identities,390 instruction anchors,two spans and60 scoped direct
DOL calls. This is static binary evidence, not an original challenge/turn trace.
Read [threshold/end helper contracts](BATTLE_RULE_THRESHOLD_AND_UNIT_QUERIES.md),
[selection-time record producers](BATTLE_ATTACK_EVENT_SELECTION.md),
[byte arithmetic](BATTLE_ACTION_RECORD_BYTE_UPDATES.md) and
[turn-time judgment](BATTLE_ACTION_RECORD_TURN_RULES.md).

## Unconditional query prefix and ownership

Capture global battle pointer B from0x8041eb90. Read descriptor P=word(B+2738),
retain B and base B+10000, then call BattleGetMarioPtr(B) and retain its result
MARIO. Call BattleGetPartyPtr(B) next; its result is unused. Only AFTER both
queries read U8(P+18) as rule R. Thus rule0,ignored rules and counter-only rules
still perform both registry searches. Do not remove the party query or delay
Mario lookup to just the HP rules. No B/P null guard or descriptor validation.

R is an unsigned byte. R>67 returns without writing status; R0..67 dispatches
through the original table. The routine has no defined scalar return contract.
Direct-write rules update byte P+19. Scalar-helper rules call the helper with
input X and then store LOW8(returned word) to CAPTURED P+19, even though the
helper independently selects current global B2/P2 for its thresholds. End-helper
rules instead delegate writes to P2 and perform NO final write to captured P.
This distinction matters when the owner changes or storage providers alias.

All record reads below are UNSIGNED bytes at captured B+offset; all sums use
word addition, without narrowing the completed sum or clipping to100. Read in
listed order. A sum of two100 bytes is200, not100 or low8(200); ten255 bytes
sum2550. The cap in AddCount/AddPoint does not cap subsequent combined inputs.
There is no record clear,increment,persistence,message,RNG or event creation in
this general judge. The called queries/helpers also do not request those services.

## Exact rules0..57

N means `_check_no_use` at8018ef1c; U means `_check_use` at8018ef54.
Those are predicate function names, not inferred rule labels. For all N/U rows,
call with the listed input then write its returned byte to captured P19.
Hex offsets omit `0x` for compactness; arrows specify actual load order.

| R | Branch entry | Input / operation |
| --- | --- | --- |
| 0 | 8018efe0 | Write P19=6, no helper |
| 1 | 8018efec | N(B16f38) |
| 2 | 8018effc | U(B16f38) |
| 3 | 8018f00c | N(B16f39) |
| 4 | 8018f01c | U(B16f39) |
| 5 | 8018f02c | N(sum16f3a→16f3b) |
| 6 | 8018f044 | U(sum16f3a→16f3b) |
| 7 | 8018f05c | N(B16f3c) |
| 8 | 8018f06c | U(B16f3c) |
| 9 | 8018f07c | N(B16f3d) |
| 10 | 8018f08c | U(B16f3d) |
| 11 | 8018f09c | N(sum16f3c→16f3d) |
| 12 | 8018f0b4 | U(sum16f3c→16f3d) |
| 13 | 8018f0cc | N(B16f3e) |
| 14 | 8018f0dc | U(B16f3e) |
| 15 | 8018f0ec | N(B16f3f) |
| 16 | 8018f0fc | U(B16f3f) |
| 17 | 8018f10c | N(sum16f3e→16f3f) |
| 18 | 8018f124 | U(sum16f3e→16f3f) |
| 19 | 8018f13c | If MARIO null, write1; otherwise U(S16(MARIO+10c)) |
| 20 | 8018f160 | If MARIO null, write1; otherwise N(S16(MARIO+10c)) |
| 21 | 8018f184 | U(B16f40) |
| 22 | 8018f194 | N(sum16f41→16f42) |
| 23 | 8018f1ac | U(sum16f41→16f42) |
| 24 | 8018f1c4 | N(sum16f43→16f44) |
| 25 | 8018f1dc | U(sum16f43→16f44) |
| 26 | 8018f1f4 | N(sum16f43→16f42→16f44→16f41) |
| 27 | 8018f21c | U(sum16f43→16f42→16f44→16f41) |
| 28 | 8018f244 | N(B16f45) |
| 29 | 8018f254 | U(B16f45) |
| 30 | 8018f264 | N(B16f46) |
| 31 | 8018f274 | U(B16f46) |
| 32 | 8018f284 | N(sum16f45→16f46) |
| 33 | 8018f29c | U(sum16f45→16f46) |
| 34 | 8018f2b4 | N(B16f47) |
| 35 | 8018f2c4 | U(B16f47) |
| 36 | 8018f2d4 | N(B16f48) |
| 37 | 8018f2e4 | U(B16f48) |
| 38 | 8018f2f4 | N(B16f49) |
| 39 | 8018f304 | U(B16f49) |
| 40 | 8018f314 | N(B16f4a) |
| 41 | 8018f324 | U(B16f4a) |
| 42 | 8018f334 | N(sum16f49→16f4a) |
| 43 | 8018f34c | U(sum16f49→16f4a) |
| 44 | 8018f364 | N(B16f4b) |
| 45 | 8018f374 | U(B16f4b) |
| 46 | 8018f384 | N(B16f4d) |
| 47 | 8018f394 | U(B16f4d) |
| 48 | 8018f3a4 | N(sum16f4b→16f4d) |
| 49 | 8018f3bc | U(sum16f4b→16f4d) |
| 50 | 8018f3d4 | N(B16f4c) |
| 51 | 8018f3e4 | U(B16f4c) |
| 52 | 8018f3f4 | N(B16f4e) |
| 53 | 8018f404 | U(B16f4e) |
| 54 | 8018f414 | N(sum16f4b→16f4d), same input as48, different code branch |
| 55 | 8018f42c | U(sum16f4b→16f4d), same input as49, different code branch |
| 56 | 8018f444 | N(sum16f4c→16f4e) |
| 57 | 8018f45c | U(sum16f4c→16f4e) |

Do not “repair” rules54/55 into a count sum: actual loads are4b/4d again,
whereas56/57 load4c/4e. MARIO HP is SIGNED halfword, read directly rather than
through a resource getter; negative HP remains a signed helper input. Its pointer
is retained from the prefix search, not refreshed after rule-byte selection.
No partner HP rule uses the unused partner result in this body.

## Direct turn-count rules58/59

Rule58 branch8018f474 reads T=U8(B16f57), then A=U8(P1a). If T>A, write6.
Otherwise read L=U8(P1b): if T>L write2, else write1. The L read is skipped
when the first comparison passes. Rule59 branch8018f4b4 uses the same read order
but tests T<=A for6, then T<=L for2, otherwise1. Comparisons here are unsigned;
equality follows the <= path. There is NO255 bypass. At T=A=255,58 cannot take
its first6 branch,whereas59 does. Captured B/P supply both values and output;
no helper recapture or incoming-status-bit preservation participates.

## Zero-sum/end-helper rules60..65

Table entries60/61/62 all target8018f4f4;63/64/65 all target8018f628. The
shared branch tests retained R to select the following read order. Each computes
X=sum(bytes) and calls `_check_turn_count_0_end(X)` at8018ee0c. Its return is
ignored; all status writes belong to the helper's freshly captured P2, with no
copy-back to P. The end helper is byte-identical to the separate turn helper;
its complete two-phase contract is in the linked threshold document.

| R | Byte load order / sum |
| --- | --- |
| 60 | 16f3a,16f39,16f3b,16f41,16f42,16f45,16f4f,16f51,16f53,16f38 |
| 61 | 16f43,16f56,16f44,16f46,16f50,16f52,16f54,16f55 |
| 62 | 16f3a,16f39,16f3b,16f41,16f42,16f45,16f4f,16f51,16f53,16f55,16f56,16f43,16f44,16f46,16f50,16f52,16f54,16f38 |
| 63 | 16f3a,16f39,16f41,16f38 |
| 64 | 16f55,16f43 |
| 65 | 16f3a,16f39,16f41,16f55,16f43,16f38 |

These are the same selected read orders as the turn-time wrapper, but the
unconditional unit-query prefix and calling lifecycle differ. Do not infer that
all general rules should run at turn start or that end and turn are one callback.
No clear/count update appears before or after the helper call in this routine.

## Direct zero-sum rules66/67

Rule66 entry8018f69c reads unsigned bytes in order:
16f3b,16f3a,16f41,16f42,16f45,16f4f,16f51,16f49,16f53,16f39.
Rule67 entry8018f704 reads:
16f3b,16f3a,16f41,16f42,16f45,16f4f,16f51,16f49,16f53,16f38.
Each sums all ten using word arithmetic and writes P19=6 if X==0,otherwise1.
There is no threshold read,helper,turn-count gate or status preservation.
They differ only in their last input address,39 versus38,not the signedness
or a single-unit eligibility test. All loads execute even if an early byte is
already nonzero; do not turn the sums into a short-circuit boolean OR.

## Call evidence and implementation boundaries

The scoped DOL census finds60 direct linked calls to this judge and its three
helpers. Exactly one targets JudgeRuleKeep: btlseqEnd at80216678. The remaining
59 helper calls are inside this judge (57 scalar threshold calls,two end-helper
sites). This proves static local sites,not the full end-controller body,actual
battle result conditions,REL/indirect callers or execution frequency.

Acceptance should cover all68 table entries plus68/255 no-write paths;both
registry searches even for ignored/constant/counter rules;captured P versus
helper P2;all byte/HP widths and read orders;rule54/55 duplicate input;negative
HP/null Mario;word sums beyond100/255;threshold equality/reversed thresholds;
58/59 versus helper sentinel differences;60..65 no copy-back;66/67 last-byte
asymmetry and non-short-circuit reads;and preservation of status on ignored rules.
Do not rename raw status1/2/6 into success/failure grades without consumer evidence.

Rule descriptor/threshold/status consumers and source writers,full btlseqEnd,
record initialization/reset/raw writers/persistence,scene/turn ownership,original
challenge/menu/script/audio/presentation domains and execution traces remain
DMG-02/STATUS-01/EVT-01 work. This closes local general judgment,not its lifecycle.
