# Complete first-attack paired selection and called getter graph

US G8ME01 revision0; ID2,2026-10-12. Prioritized native EVT-01/JUMP-01 request.
Complete BattleGetFirstAttackUnit800feaf8/d0,BattleGetPartnerPtr800f86f4/34,
BattleGetMarioPtr800f87a8/6c,BattleGetPartyPtr800f8728/80 and
BattleGetUnitPtr800f88cc/3c. Verify `python3 tools/verify_first_attack_target.py`;
read [event/body-part output](FIRST_ATTACK_TARGET_EVENT_AND_BODY_PART.md) and
[existing target interfaces](BATTLE_TARGET_EVENT_INTERFACES.md).
Static source evidence;no original first-attack/Game execution trace.

## Scan retains captured battle and last lookup,including misses

Input B(r3),ATTACKER_OUT(r4),TARGET_OUT(r5) are retained. For index0..63 in order,
call BattleGetUnitPtr(retained B,index),assign CANDIDATE to EVERY return,then
if nonnull read UNSIGNED byte candidate0c. Exact1 stops. Otherwise advance.
No HP/status/kind/selected-target/part filtering or RNG call occurs.

Exhaustion leaves CANDIDATE equal to slot63's lookup,not a synthesized0/-1.
It can be null or nonnull with side byte other than1. Setup is read only AFTER
this scan:read WORD retained B2738 as S,then SIGNED WORD S08 MODE. No null
setup guard and no recapture of global battle. Getter/provider mutations during
the scan can affect these later live setup reads;B's identity remains retained.

## Exact signed mode selects two distinct output roles

| MODE | Calls after candidate scan | Attacker word | Target word | Raw r3 on return |
| --- | --- | --- | --- | --- |
| 1..6 | BattleGetMarioPtr(B)→M | M | retained CANDIDATE | M |
| 7..8 | BattleGetMarioPtr(B)→M;BattleGetPartnerPtr(B,M)→P | P | retained CANDIDATE | P |
| All other signed words | BattleGetMarioPtr(B)→M | retained CANDIDATE | M | M |

Default includes negatives,0 and9..signed maximum. No mode fallback/candidate
rescan exists. Store attacker to ATTACKER_OUT FIRST,then target to TARGET_OUT.
Aliases retain the target write. Neither pointer is null-checked. Raw r3 is the
last getter's return,not a generic scanned-enemy return or an independent miss
status. Event caller ignores r3 and reads the two supplied stack output slots.
Do not port this helper as a single first-enemy pointer return.

## Registry and Mario/party getters use the passed battle

BattleGetUnitPtr(B,I) returns0 for any signed I<0 or I>=64;otherwise returns
WORD(B+20+4*I) with word address arithmetic. It does not reload global battle,
read unit validity fields or require returned unit.id==I. First-attack scan uses
only valid0..63 indices;unit identity word0 remains a later event output field.

BattleGetMarioPtr(B) retains B,scans all64 slots through that getter. First
nonnull unit with WORD kind08==222(dehex) and SIGNED byte0c==0 wins. Kind is
read before side. Exhaustion explicitly returns0 (unlike first-attack candidate
retention). No HP/activity/body-part/source-generation filter exists.

BattleGetPartyPtr(B) retains B,scans slots0..63. For nonnull unit,read WORD1c
flags first and skip when mask0008 set;then read SIGNED kind08 and require
224<=kind<231(e0..e6hex);then read SIGNED byte0c and require0. First admitted
unit wins;exhaustion returns0. It does not infer the menu-selected partner.

## 800f86f4 is a kind predicate with unchecked input unit

BattleGetPartnerPtr(B,U) reads WORD U08 BEFORE any null guard. Exact222 calls
BattleGetPartyPtr(B). Every other kind calls BattleGetMarioPtr(B). B remains
passed through;there is no current-global recapture or partner-key argument.

Mode7/8 feeds it the Mario getter return. Under stable valid memory that getter
returns kind222,so Party is selected;missing Mario still dereferences U0+8.
Mutation of returned kind can instead select a fresh Mario lookup. These are
local source/provider consequences,not proof all original source scenes admit
them. Do not replace this helper with an unconditional Party getter or a null→0
shortcut. Source guarantees required by the caller remain explicit below.

## Native implementation acceptance and unresolved source invariants

Port paired outputs and raw getter return,captured B versus live setup/slot reads,
all signed mode boundaries,first side1 candidate versus last slot63 miss,registry
ID bounds,Mario kind/side order,Party flag/kind/side order and unchecked kind
predicate. Authored fixtures should exercise pointer/output aliases,manager
swaps,slot63 nonenemy,missing Mario/Party and kind mutation without calling them
original observations. Preserve original32-bit address/memory coherence.

Source/Game must establish setup S validity,which modes are produced,registry
population and absence of cycles/invalid part/kind pointers. Modes1..8 need the
selected candidate to be nonnull before event target.id read;default needs Mario
nonnull;mode7/8 additionally needs Mario nonnull for predicate. No such guarantee
is inferred solely from this helper. Unit/setup writers,scene selection and
original traces remain native/source requests;the complete local graph is now
supplied for implementation without inventing safe target fallback.
