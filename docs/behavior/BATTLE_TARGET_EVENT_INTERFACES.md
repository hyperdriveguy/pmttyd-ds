# Battle target event interfaces and first-attack selection

US G8ME01 revision 0, recovered 2026-10-12. Evidence B: inspected original DOL
bodies and shipped harbor REL call bindings. Symbols are annotations, not a
substitute for instruction evidence. No original execution observation or Switch
behavior is asserted. The sampling cores are **dependencies**, not fully recovered
by documenting these interfaces.

| Complete local body | DOL address | Size |
| --- | --- | --- |
| btlevtcmd_GetFirstAttackTarget | 0x8010b418 | 0x78 |
| BattleGetFirstAttackUnit | 0x800feaf8 | 0xd0 |
| BtlUnit_GetBodyPartsId | 0x80128c98 | 0x40 |
| BtlUnit_GetEnemyBelong | 0x80127454 | 0x2c |
| btlevtcmd_GetEnemyBelong | 0x8010eed4 | 0x70 |
| btlevtcmd_SamplingEnemy | 0x8010b664 | 0xb0 |
| btlevtcmd_ChoiceSamplingEnemy | 0x8010b5e4 | 0x80 |

`W` denotes shared BattleWork. `S8`, `U8` and `U32` identify original load or
narrowing widths. Event completion value 2 is distinct from unit IDs, selection
results and hit-precheck codes. All four callbacks return 2 without a local
first-call gate or asynchronous poll. Their callees can have side effects.

## First-attack pair helper

BattleGetFirstAttackUnit(W,outAttacker,outTarget) scans registry IDs 0..63 via
BattleGetUnitPtr. Each lookup replaces the candidate pointer. Stop on the first
nonnull unit whose **unsigned** byte +0xc equals 1. Exhaustion does not assign a
special no-enemy result: the candidate remains the last lookup's value, which
may be null or a unit not satisfying that side test.

Read mode as the signed word at `U32(W+0x2738)+8`; that intermediate pointer is
not checked for null. The complete local mode map is:

| Mode | Pointer written to outAttacker | Pointer written to outTarget |
| --- | --- | --- |
| 1..6 | BattleGetMarioPtr(W), 0x800f87a8 | Scanned candidate |
| 7..8 | BattleGetPartnerPtr(W,BattleGetMarioPtr(W)), 0x800f86f4 | Scanned candidate |
| All other signed words | Scanned candidate | BattleGetMarioPtr(W) |

The two writes occur in attacker-then-target order. Aliased output pointers
therefore retain the target write. No local random draw, body-part lookup,
HP/status filter or fallback search occurs after the scan. The Mario/partner
getter implementations and the mode pointer's producer remain dependencies;
this body alone does not establish which mode the harbor encounter enters.
The helper at 0x800f86f4 is BattleGetPartnerPtr, distinct from the larger
BattleGetPartyPtr at 0x800f8728.

## Body-part ID helper and event output

BtlUnit_GetBodyPartsId(U) initializes result to -1. Null U returns -1. Otherwise
traverse the linked list starting at U+0x14. For each part P, test word P+0x1ac
bit 1 (mask value 1). The first match returns word 0 of the kind pointer at
P+4. If no match, follow P's word 0 next link until null and return -1. There
is no list-length/cycle guard, kind-pointer validation or numeric ID clamp.

GetFirstAttackTarget captures its two output operands **raw**, calls the pair
helper with two stack output slots, and ignores the attacker result. It calls
the body-part helper on the target, then writes target word 0 as the unit ID to
output operand 0 through evtSetValue, followed by the part ID to operand 1.

Crucially, the callback dereferences target word 0 **without a null check** even
though the part helper accepts null. It is not a safe no-target wrapper returning
(-1,-1). A nonnull target with no flagged body part yields (unitID,-1). Aliased
event destinations retain the second write; ordinary evtSetValue resolution and
side effects remain part of the ABI.

## Enemy side lookup

BtlUnit_GetEnemyBelong(U) reads **signed** byte U+0xc:

| Stored signed byte | Return |
| --- | --- |
| 0 | 1 |
| 1 | 0 |
| Every other value | Same signed byte |

This is not a boolean inversion for all values. The helper has no null check.
GetEnemyBelong(event) resolves operand 0 via evtGetValue, translates it through
BattleTransID, captures output operand 1 raw, fetches the corresponding unit from
shared W, invokes the helper and writes its return through evtSetValue. It does
not locally special-case translated ID -1, unlike the HP getters documented in
[unit entry and stats](UNIT_ENTRY_STATS_AND_SPAWN.md).

## Sampling interface and signed bias

SamplingEnemy captures W and the embedded candidate buffer address W+0x428.
Resolve operand 0, then BattleTransID to obtain actorID. Resolve operand 1 and
narrow it to S8, obtaining belong. Resolve operand 2 as the weapon pointer V.
The original order is actor resolution/translation, belong resolution, weapon
resolution, then weapon/work reads. Weapon and indexed work pointers are not
validated locally.

Read flags68 = U32(V+0x68). Read byte b = U8(W + 8*belong + 0xa). Compute
bias = S8(-b). If flags68 & 0x10000000, compute bias = S8(-bias). Both signed
byte narrowings matter, including the -128 case; replacing them with an arbitrary
width signed negation changes the callee argument. No belong range check precedes
the indexed byte load. Then read flags64 = U32(V+0x64) and call:

`BattleSamplingEnemy(W+0x428,V,actorID,belong,flags64,flags68,bias)`

The seven arguments are in r3..r9 order. The core is at 0x800ff5c0, size 0xc4,
with an additional inner selection/filter dependency. Callback return is 2;
there is no destination write by this wrapper. Candidate admission, sorting,
capacity, random draws, mutation lifetime and probabilities remain open.

ChoiceSamplingEnemy likewise captures W+0x428, resolves weapon operand 0, then
captures raw output operands 1 and 2. Read U32(V+0x7c), pass it and two stack
output slots to BattleChoiceSamplingEnemy at 0x800febc8, size 0x9d8. Ignore its
register return. Write the stack unit ID to output 1, then part ID to output 2
via evtSetValue; return 2. The wrapper does not initialize those stack results
before the core, so successful output initialization is a core obligation, not
a default supplied by the wrapper. Do not invent random target probabilities
or assume this always chooses Mario in the opening battle.

## Actual harbor caller and implementation boundary

The [boss attack stream](HARBOR_BOSS_NORMAL_ATTACK.md) calls GetFirstAttackTarget
when battle flag mask 2 is nonzero. Otherwise it gets the owner's opposite side,
samples using inherited weapon local 9, then chooses unit local 3 and part local
4. The script later checks **unit ID** -1, not part ID -1. Which route executes,
and whether missing-target cases are feasible, require mode/flag/target-core
producer closure and original traces.

Seven whole-body fingerprints and selected instructions pin these interfaces.
The same verifier also checks shipped REL imports; inspection listings now use
[correct Gekko decoding](../GEKKO_DISASSEMBLY.md). Preserve raw output operands,
write order, narrowing and shared candidate-buffer ownership in a port. Candidate
selection can be isolated behind a service while its core is recovered; label
that service incomplete rather than substituting a fabricated policy.

Next recover BattleSamplingEnemy/inner core, BattleChoiceSamplingEnemy, mode and
side producers, registry/part creation and target buffer resets. Record original
normal/first-attack/no-target and partner-target traces with seed state and exact
unit/part IDs before claiming complete target equivalence.
