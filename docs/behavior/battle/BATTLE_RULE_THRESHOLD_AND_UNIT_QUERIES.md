# General rule thresholds, end-helper writes and initial unit queries

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete `_check_no_use`8018ef1c/38,`_check_use`8018ef54/38,
`_check_turn_count_0_end`8018ee0c/88 and
BattleGetMarioPtr800f87a8/6c,BattleGetPartyPtr800f8728/80.
Verify:`python3 tools/verify_battle_general_rules.py`. Read
[complete general selector](BATTLE_GENERAL_RULE_JUDGMENT.md) and
[turn wrapper/helper](BATTLE_ACTION_RECORD_TURN_RULES.md). Static evidence only.

## Scalar threshold helpers return; the caller writes

Each helper takes signed word X, freshly reads global battle B2,then
P2=word(B2+2738). There is no null guard or owner-equality test. Both use unsigned
byte thresholds but SIGNED word comparisons with X. Neither writes status or
any other storage,reads incoming status,changes thresholds,consumes RNG or calls
another service. Returned1/2/6 are raw numeric values,not verified game labels.
The general judge writes their low byte to its separately retained P+19.

`_check_no_use` first reads L=U8(P2+1b). If X>=L,return1 without reading A.
Otherwise read A=U8(P2+1a): if X>=A return2;else return6. This is:
`X>=L ? 1 : (X>=A ? 2 : 6)`, retaining short-circuit load order.
Equality at L takes1; equality at A takes2 only if the first comparison failed.
Reversed/equal thresholds do not get sorted or normalized.

`_check_use` first reads A=U8(P2+1a). If X>=A,return6 without reading L.
Otherwise read L=U8(P2+1b) and return2 when X>=L,else1. This is:
`X>=A ? 6 : (X>=L ? 2 : 1)`. The second signed predicate is produced through
`srawi(X,31); srwi(L,31); subc; adde; addi1` at8018ef74..84,not a separate
conditional branch. Unsigned carry and sign correction reproduce the signed
comparison;do not drop sign correction when emulating this sequence.

Neither helper treats255 as a bypass. For X=-1,A=0,L=0,no_use returns6 and
use returns1. For X=255,A=255,L=255,no_use returns1 and use returns6. If A=5,
L=10,X=5,no_use returns2,use returns6;X=10 gives1/6. The domain includes signed
HP halfwords and full unsigned-byte sums,preserving their widening rules.

## End helper: independent two-phase writes

The complete136-byte body of `_check_turn_count_0_end` is IDENTICAL to the
136-byte `_check_turn_count_0_turn` at8018ee94. Both identities are pinned.
Do not infer identical call cadence from identical bodies.

Input is signed word X,used only for exact zero/nonzero tests. Capture fresh
B2 from global pointer,read P2=B2.2738 and retain B2+10000. Phase1 reads
T=U8(B2+16f57),then L=U8(P2+1b). If T<=L or L==255,write P2.status19=2
for X==0,otherwise1. Outside admission leave status unchanged.

Phase2 RELOADS T from the same retained B2 and reads A=U8(P2+1a). If T<=A
or A==255: for X==0,write6;for X!=0,read CURRENT status19 and preserve it
when `(status & 0x02)!=0`,otherwise write1. Outside admission leave untouched.
Finally reload and return U8(P2.status19). General judge ignores this result;
it does not write it to captured P. This is a separate write ownership rule
from the scalar threshold helpers above.

The mask is numeric0x02 (zero-based bit index1),from
`rlwinm. r0,r0,0,30,30` at8018ee7c. It is not numeric0x04. Phase1 write1
clears0x02 before phase2's test;do not test a snapshot of incoming status.
Phase1's zero-sum2 can become6 in phase2. With phase1 excluded and phase2
admitted for nonzero X,prior status2 or6 survives,but prior4 is replaced by1.
Other prior bits survive only on no-write branches. All status writes are bytes.

The 255 bypass occurs independently for EACH end-helper threshold,unlike the
scalar helpers and direct rules58/59. Counter/threshold compares are unsigned.
Equality admits a phase. Both phases execute even after a write;T and threshold
reads are live and ordered. No clear/increment,unit query,RNG,save or callback.
General rules60..65 read sums from captured B and write through freshly selected
B2/P2;the provider must preserve that distinction instead of passing captured P
into a generic evaluator. Rule source/threshold writers and status consumers
remain separate requirements.

## Mario and partner queries use explicit battle registry

BattleGetMarioPtr(B) scans indices0..63 in ascending order,calling
BattleGetUnitPtr(B,index) each time using retained EXPLICIT B. Skip null entries.
Read signed word kind at U+8;only222 continues to the byte check. Read S8(U+c)
and admit exactly0. Return FIRST admitted pointer;otherwise return null after64.
No HP,status,visibility,unit-flag or current actor test. Signed extension before
zero comparison does not change the byte0 condition,but is the actual sequence.

BattleGetPartyPtr(B) likewise scans64 explicit-B slots,skipping null entries.
First read word U+1c and reject when numeric bit0x08 is SET. Only after this
test read signed kind U8;require224<=kind<231. Then read S8(U+c) and require0.
Return FIRST admitted pointer or null after64. Other flag bits do not reject,
and no HP/alive/active-partner/current-actor test appears. Do not substitute a
selected partner index or require HP>0. The flag test precedes both kind and
belong-byte reads. Multiple admitted entries are resolved by slot order.

Both queries retain explicit B and use the existing
[registry getter](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md). They
perform no global battle recapture,write,allocation,pouch query or status reset.
GetMario is already a cost-service dependency;this general judge ALWAYS calls
Mario then party BEFORE reading rule,even though party result is discarded.
The cost caller's additional reads/policy remain in
[weapon payment](BATTLE_WEAPON_COST_AND_PAYMENT.md).

## Acceptance and remaining requirements

Check short-circuit threshold loads,all equality edges,255 ordinary scalar
threshold versus end bypass,reversed/equal thresholds,negative and large signed
inputs,raw1/2/6 returns,scalar no-write versus caller byte write and fresh owner.
For end helper exercise both threshold phases,T reload,zero/nonzero X,
2→6,nonzero prior2/4/6,phase1 clearing before0x02 preservation and final reread.
For queries cover first/multiple matches,64 misses,null slots,Mario kind222
versus partner224..230,boundary223/231,all belong bytes,HP0/negative admission,
partner flag08 rejection versus other flags and exact read order.

Descriptor/threshold/raw record producers,registry mutation/lifetime,full
end/turn/scene callers,persistence,status/UI consumers,source/script/domain
services and original execution remain open. Byte equality and static control
flow prove local contracts,not original challenge success or game timing.
