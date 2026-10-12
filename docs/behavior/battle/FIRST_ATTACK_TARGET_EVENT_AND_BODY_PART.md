# Complete first-attack event capture and body-part output contract

US G8ME01 revision0; ID2,2026-10-12. Native EVT-01/JUMP-01 request.
Complete btlevtcmd_GetFirstAttackTarget8010b418/78 and
BtlUnit_GetBodyPartsId80128c98/40. Verify
`python3 tools/verify_first_attack_target.py`;read
[paired selection/getters](FIRST_ATTACK_PAIR_SELECTION_AND_GETTERS.md),
[event namespaces](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md) and
[ordinary Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md).
No native code changed here;static evidence is separate from Game/runtime traces.

## Capture battle before reading either raw output token

Retain event E(r3);incoming firstCall is ignored/overwritten. Read WORD E18 as
argument pointer A. Then capture WORD _battleWorkPointer8041eb90 as B. Only
AFTER that read,load RAW WORD A00 as UNIT_OUT and RAW WORD A04 as PART_OUT.
Neither output token is resolved through evtGetValue. No actor input/translation,
current unit lookup or event-variable read occurs before paired selection.

Call BattleGetFirstAttackUnit(captured B,&stackAttacker,&stackTarget),using
original frame slots newSP+0c for attacker and newSP+08 for target. Ignore its
raw r3 return;attacker slot is ignored by this event. These slots remain distinct
addresses,not one pointer result or a swapped role convention. The core uses
its passed B throughout;evtSetValue later has its own namespace/global behavior.

## Body-part traversal first,then fresh target slot/id,then two writes

Read stackTarget into U,call BtlUnit_GetBodyPartsId(U). Retain returned part ID.
Then reread stackTarget and read WORD currentTarget00 as UNIT_ID WITHOUT null
guard. Call evtSetValue(E,retained UNIT_OUT,UNIT_ID). Then call
 evtSetValue(E,retained PART_OUT,retained PART_ID). Ignore setter returns.
Finally return event completion2. No first-call gate,yield/wait/event creation,
attack scheduling,damage or selection mutation is supplied locally.
The ignored attacker slot is not an admission gate. A null Mario result in
mode1..6 or null Party result in mode7..8 does not itself reject a valid selected
target;mode7..8 still requires nonnull Mario before the kind predicate runs.
Do not add an attacker-null failure branch to this event.

The part ID is captured BEFORE the unit-ID read and first setter. The second
setter uses retained token/value even if the first setter changes E18,argument
words,units,parts or battle/global state. Output aliases retain the second write
under the setter's ordinary destination semantics. Do not resolve output tokens
as values,recapture B,read unit.id before body traversal or recompute part ID
between output writes. Stack-address aliases must preserve the displayed reads.

## Body-part helper is null-safe but flagged-kind pointer is unchecked

Initialize result=-1. Null U returns-1 immediately. Otherwise read WORD U14
as first part P. While P!=0,read WORD P1ac and test mask0001. If clear,follow
WORD P00 as next. If set,read WORD P04 as KIND,then WORD KIND00 as result and
return it. No kind-null guard,part numeric-ID field alternative,body geometry,
status,HP or first enemy policy occurs in this helper.

No flagged part returns-1. The list has no count/cycle guard. A nonnull target
without flagged part yields (its unit.id,-1). A null target first receives part
result-1 but THEN encounters the event's unchecked target00 read;this is not a
safe event(-1,-1) result. Host memory providers may surface invalid source state
explicitly,without claiming the original callback gracefully accepted it.

## Request reconciliation and acceptance

This contract and the paired getter graph supersede the short overview's generic
“find first enemy” interpretation. Setup mode can choose Mario as the target;
scan exhaustion retains slot63's lookup. Original caller source needs to prove
mode/registry/setup invariants before treating the complete event as safe for all
Game states. The existing target-interface contract already records paired roles;
this consolidation adds exact capture/read order and closes called getters.

Acceptance:argument pointer before global B capture before raw tokens,all mode
outputs,candidate scan miss with null/nonenemy last slot,ignored attacker/raw
core return,body-before-id,null helper versus event boundary,part mask0001,
kind pointer dereference,missing body part,first setter mutations/raw token aliases
and completion2. Use actual recovered getter/core/event composition rather than
a fixed-target fixture implementation. Existing event setter contracts remain
required providers;its body identity is pinned here but not reinterpreted.

The original Jump callback registry has a direct GetFirstAttackTarget site;
full ordinary root/menu/setup/unit/part source binding,field first-strike
producers,Game composition,platform/source domains and original traces remain
EVT-01/JUMP-01 requirements. Complete local bodies/getter graph are delivered;
this does not establish full original Jump or complete-game fidelity.

## Concrete initial body-part source witness

[Original part construction/source tables](../unit/UNIT_PART_DEFENSE_AND_LIFETIME.md)
pin Crump's single part ID1 with attributes9 and Mario's three parts IDs1/2/3
with attributes9/13010000/13010000hex. The constructor copies these attributes
to part1ac. Under that initial construction and intact links/kind pointers,
mask0001 selects part1 and this helper returns1 for either actor. Those sources
do not prove current setup mode,candidate side/registry population or absence
of later mutation. They support the body-part input invariant only at recovered
initialization;native Game bindings and original execution remain separate.
