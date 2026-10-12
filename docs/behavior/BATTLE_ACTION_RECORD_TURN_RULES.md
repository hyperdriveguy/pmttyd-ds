# Complete local action-record turn-rule selector and two-phase status writes

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete BtlActRec_JudgeTurnRuleKeep8018f77c/1f0 and
_check_turn_count_0_turn8018ee94/88. Read
[record updates](BATTLE_ACTION_RECORD_BYTE_UPDATES.md),
[first-process order](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md).
Verify:`python3 tools/verify_battle_action_records.py`.
Static original DOL behavior;no interpreted rule-name/success-label/gameplay trace.
The separate [general JudgeRuleKeep](BATTLE_GENERAL_RULE_JUDGMENT.md) is now
complete locally;its threshold/end helper and unit-query contracts are in
[rule thresholds](BATTLE_RULE_THRESHOLD_AND_UNIT_QUERIES.md).

## Selector and captured sum source

Read global battle pointer B=word8041eb90 (r13+1c70). Read P=word(B+2738),then
UNSIGNED rule byte P+18. No B/P null check. Only exact60..65 are handled;
every other byte returns without rule helper/state write. The wrapper has no
specified scalar result contract for ignored rules;the current caller ignores
its return. Do not convert this into a blanket false/failure return.

For handled rule,read the following UNSIGNED counter bytes from captured B,
in EXACT listed load order,add using word arithmetic,then call
_check_turn_count_0_turn(sum). Offsets are relative to B,not B+10000 in this table.
Each addition/next-byte load follows original order;no cached record struct or
narrowing of the completed sum. Maximum18*255 fits the ordinary word domain.

| Rule | Byte load order / sum |
| --- | --- |
| 60 | 16f3a,16f39,16f3b,16f41,16f42,16f45,16f4f,16f51,16f53,16f38 |
| 61 | 16f43,16f56,16f44,16f46,16f50,16f52,16f54,16f55 |
| 62 | 16f3a,16f39,16f3b,16f41,16f42,16f45,16f4f,16f51,16f53,16f55,16f56,16f43,16f44,16f46,16f50,16f52,16f54,16f38 |
| 63 | 16f3a,16f39,16f41,16f38 |
| 64 | 16f55,16f43 |
| 65 | 16f3a,16f39,16f41,16f55,16f43,16f38 |

Rules60/61 sums are disjoint and62 combines their fields,but its actual load
order differs from concatenating two cached sums. Rules63/64 combine into65
with another actual load order. No action-record clear/increment occurs here;
these are reads. Full semantic rule labels and counter source ownership remain open.
The helper decides only exact sum0 versus nonzero,not an action-count threshold.

## Helper reselects global battle and rule descriptor

_check_turn_count_0_turn takes sum X in r3. Independently RELOAD global battle
B2,read its P2=B2.2738 and retain base B2+10000. It does not receive captured
B/P from the wrapper. No stability/equality/type check between the two captures.
Native service context must preserve this ownership distinction;source domain
may make them equal but this local code does not assert it.

Perform phase1:

1. Read current turn-count byte T=U8(B2+16f57).
2. Read threshold L1=U8(P2+1b).
3. If T<=L1 OR L1==255,write P2.status19=2 when X==0,otherwise1.
4. Otherwise leave status unchanged.

Then perform phase2,including NEW reads:

1. Reload T from same retained B2+16f57;read L2=U8(P2+1a).
2. If (T<=L2 OR L2==255) AND X==0,store status19=6.
3. If that admission holds and X!=0,read CURRENT status19. If (status19 & 0x02)!=0,
   leave it untouched;otherwise write1.
4. Outside admission leave status unchanged.

Finally return U8(P2.status19),reading it AFTER all writes. Helper does not
increment T,reset thresholds,interpret status as a bool or normalize high bits
unless a listed overwrite happens. Both phases execute;phase2 can overwrite
phase1's2 with6 on a zero sum.255 bypass applies independently per threshold.
T=threshold is admitted,not strict less. No signed counter/threshold compare.

For X!=0,phase1's write1 clears mask0x02 before phase2 tests it. Thus preserving an
incoming mask0x02 on phase2 is possible only when phase1 did NOT overwrite it (or
external source/alias change);do not snapshot the incoming status once and
apply both phases privately. Incoming other bits survive only on no-write paths.
No scalar clamp,save,unit mutation,rule notification/event creation/RNG here.

## Caller order and remaining rule ownership

Complete btlseqTurnFirstProcess calls judgment BEFORE its explicit battle turn
halfword increment and BEFORE AddCount(B+16f57). It reads prior record count,
not the incremented one from the same first-process call. Explicit B argument
and global B2 can differ;no pointer equality gate. Original controller/scene/
rule-descriptor/threshold/counter writers and call frequency remain separate.

Acceptance:ignored rule0/59/66/255 versus six handled selections;all exact load
orders/sums/source capture;helper global recapture;threshold equality/255,
independent phase rereads,X0/nonzero,write2→6,phase1 write1 before mask0x02 test,
phase2 preserve incoming mask0x02 only on relevant path,raw status return;prior-turn
counter ordering. General/end/selection local bodies are linked separately;full reset/save/
scene/source and caller lifecycle domains and original challenge/turn/damage traces remain open.


Mask clarification (ID2):numeric0x02,zero-based bit index1,comes from
rlwinm0,0,0,30,30. Prior phrasing “bit2” meant this mask;it must not be
implemented as0x04. Nonzero X with phase1 excluded/phase2 admitted preserves
status2/6 but overwrites status4 with1. General rule judgment is linked above;
full source/caller/lifecycle and original traces remain open.
