# Counter selection, accumulation and dispatch

US G8ME01 revision 0, recovered 2026-10-12. Identity and B/R/P evidence classes
are in [ORIGINAL_BEHAVIOR.md](../../ORIGINAL_BEHAVIOR.md). B means the original US
instructions and data, R public naming annotations, P proposed verification.
This document establishes local counter selection plus its consumer tail, not
[complete local orchestration](BATTLE_DAMAGE_ORCHESTRATION.md). No original
gameplay capture was made.

The pinned [public damage source](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/battle/battle_damage.c)
contains only the start of `BattleCheckDamage`. The recovered routines are
`BattleCheckCounter` at `0x800fa678` (size `0x5a4`),
`BattleInitCounterPreCheckWork` at `0x800fac1c` (size `0x3c`), and the tail of
`BattleCheckDamage` at `0x800fb7a0`–`0x800fba18`. Ignored ELF/disassembly files
under `artifacts/reference/behavior/` retain the local inspection evidence.

## Work record and interface (B)

Initialization clears all 48 bytes with memset, then writes word 1 at offset 0
(`0x800fac24`–`0x800fac40`). Counter selection receives
`(work,A,D,P,W,flagsPointer)`: attacker, defender, defender part and weapon.
It returns an integer classification and mutates the work record and pointed
flags. A null attacker returns zero immediately without changing the record.
Otherwise only work+0x28 is reset on entry; callers must initialize the other
fields themselves. D/P/W are assumed valid on their respective access paths.

All fields in this record are 32-bit words:

| Offset | Meaning established by selector and consumer |
| --- | --- |
| `00` | Continue ordinary defender damage path; initially 1, early hazards set 0 |
| `04` | Add one half-damage contribution (shared by status 21 and part flags 0x100/0x200) |
| `08` | Request attacker status 8 |
| `0c` | Request attacker status 4 |
| `10` | Request attacker status 6 |
| `14` | Request attacker status 9 |
| `18` | Add another half-damage contribution and enable explosion presentation |
| `1c` | Add another half-damage contribution |
| `20` | Selected classification code, later mapped to function return |
| `24` | Selected counter damage/event code; consumer uses nonzero as counter gate |
| `28` | Accumulated counter HP amount |
| `2c` | Optional defender hit-event code |

Notation below: `C=U32(P+0x1b0)` part counter attributes,
`R=U32(W+0x78)` weapon counter resistance, `S=U32(W+0x74)` special flags.
Badge fields `A+0x302` and `A+0x301` are unsigned bytes, named Spike Shield
and Ice Power by public headers. Status IDs remain numeric in the contract.
Every selected row ORs `0x100` into `*flagsPointer`; no row clears it.

## Exclusive early contact hazards (B)

Evaluate the following rows in order. A selected row sets work+0x20/+0x24/
+0x2c to its three listed codes, adds 1 to work+0x28, sets work+0x00 to zero,
sets any extra request, then jumps directly to return-code mapping. It skips
all later rows, including the accumulation phase.

| Order / start | Trigger | Conditions allowing the hazard | Codes: +20 / +24 / +2c | Extra request |
| --- | --- | --- | --- | --- |
| 1 / `0x800fa6b4` | C has 0x1 | A+0x302 is zero; R lacks 0x2 | 43 / 299 / 31 | none |
| 2 / `0x800fa710` | C has 0x2 | A+0x302 is zero; R lacks 0x4 | 44 / 300 / 32 | none |
| 3 / `0x800fa76c` | C has 0x4 | A+0x302 is zero; R lacks 0x8 | 45 / 301 / 33 | none |
| 4 / `0x800fa7c8` | C has 0x10 or 0x20 | A+0x301 is zero; R lacks 0x10 | 46 / 302 / 34 | If C has 0x20, set work+08=1 |
| 5 / `0x800fa840` | C has 0x40 or 0x80 | R lacks 0x20 | 47 / 303 / 35 | If C has 0x80, set work+14=1 |
| 6 / `0x800fa8ac` | C has 0x400 or 0x800 | R lacks 0x1; A does not have status 6 | 48 / 304 / 36 | If C has 0x800, set work+10=1 |

There is no generic counter-resistance test before this ordered set. Each
row has its own mask, badge/status gate and precedence. In a freshly initialized
record, any selected early hazard has amount 1 and no half-damage requests.

## Accumulating effects (B)

If no early hazard selects, evaluate **all** of the following rows in order.
Each successful row overwrites work+0x20 and work+0x24 with its listed codes,
ORs 0x100 into the input flags, and performs its listed additions/requests.
These rows do not clear work+0x00 or set work+0x2c. Earlier amounts and request
flags persist even when later rows replace the classification codes.

| Order / start | Trigger and gates | Codes +20 / +24 | Additional effects |
| --- | --- | --- | --- |
| 1 / `0x800fa92c` | C has 0x1000, R lacks 0x80, and either W element is 3/4/1 or (S has 0x400 and A has status 8) | 49 / 305 | amount +=1 |
| 2 / `0x800fa9a8` | C has 0x2000 and R lacks 0x100 | 50 / 306 | amount +=1 |
| 3 / `0x800fa9e8` | D has status 6 or C has 0x400/0x800; R lacks 0x1; A lacks status 6 | 52 / 308 | amount +=1; if C has 0x800 set work+10=1 |
| 4 / `0x800faa6c` | D has status 21 and R lacks 0x200 | 53 / 309 | set work+04=1 |
| 5 / `0x800faab0` | C has 0x100 or 0x200 and R lacks 0x40 | 51 / 307 | set work+04=1; if C has 0x200 also set work+0c=1 |
| 6 / `0x800fab04` | D has status 22 and R lacks 0x400 | 54 / 310 | set work+18=1 |
| 7 / `0x800fab48` | U8(D+0x2f0) is nonzero and R lacks 0x200 | 53 / 309 | set work+1c=1 |

R labels: status 6 Electric, 8 Burn, 21 Payback, 22 Hold Fast; D+0x2f0
Return Postage. Return Postage count is a boolean gate here: multiple copies
do not multiply this field's contribution. Payback and the part's 0x100/0x200
effect share work+04, so selecting both does not create two contributions.
Hold Fast and Return Postage use separate flags and can each add another half.

The repeated electric test is retained as executed control flow; do not remove
it simply because some combinations would have selected the early electric
hazard. Status helpers and input data determine reachable combinations.

## Classification return mapping (B)

The final switch at `0x800fab84` uses a twelve-entry US jump table at
`0x8034c720`, indexed by unsigned `(work+0x20)-43`. Values outside 43–54
return zero. The table maps as follows:

| Work+20 | Target RAM address | Return |
| --- | --- | --- |
| 43 | 0x800fabac | 8 |
| 44 | 0x800fabb4 | 7 |
| 45 | 0x800fabbc | 9 |
| 46 | 0x800fabc4 | 10 |
| 47 | 0x800fabcc | 11 |
| 48 | 0x800fac04 | **0** |
| 49 | 0x800fabd4 | 12 |
| 50 | 0x800fabdc | 13 |
| 51 | 0x800fabec | 15 |
| 52 | 0x800fabe4 | 14 |
| 53 | 0x800fabf4 | 16 |
| 54 | 0x800fabfc | 17 |

Code 48 returns zero despite populating a counter record. Classification is
not a boolean success indication. `BattleCheckDamage` calls this selector at
`0x800fb2b8` and does not branch on its register return; it branches on work+00
and later work+24. Other callers still require inspection.

## Consumer tail in BattleCheckDamage (B)

The caller creates a fresh work record at stack+0x54. Before reaching the
selector it has a separate superguard path, specified in the orchestration
contract.
After selection, work+00 decides whether to enter the ordinary defender damage
path at `0x800fb2c0` or skip to `0x800fb7ac`. The HP-calculator return is held
in register 22, initialized to zero before these branches. The ordinary path's
complete control flow is specified in the orchestration contract.

At `0x800fb7ac`, increment attacker word +0x280 and copy its low byte to
defender +0x273. This happens even if work+24 is zero. If work+24 is zero,
return the caller's current result, without the counter work below. Otherwise:

1. In order work+04, +18, +1c, for each nonzero flag add
   `trunc(HP_calculator_result*50/100)` to work+28 (`0x800fb7cc`–`0x800fb834`).
   Each division occurs separately. Use the signed calculator result, not the
   defender's actual HP lost after clamping. Arithmetic is 32-bit, with signed
   division toward zero. Do not turn three half contributions into one
   `trunc(HP*150/100)` operation: odd amounts differ.
2. In order work+0c, +14, +08, +10, make a status application call for
   each nonzero flag (`0x800fb838`–`0x800fb934`). Status IDs are respectively
   4, 9, 8, 6. The exact argument tuple is
   `BattleSetStatusDamage(&localResult,A,P,0,statusID,100,0,3,1)`;
   each localResult starts at zero and is not subsequently consumed here.
   P is the original defender part pointer, even though A is the recipient.
   These are requests, not proof the status successfully applies: the status
   routine's [immunities/rate/stacking semantics](BATTLE_STATUS_INFLICTION.md) apply.
3. If work+28 equals zero, set it to 1 (`0x800fb938`). This is an equality
   check, **not** a general clamp to at least one; a negative value stays negative.
4. If signed byte A+0x0c equals zero, invoke
   `BattleAudience_Case_Countered` (`0x800fb94c`).
5. If work+18 is nonzero, obtain defender/part hit position, spawn
   `effBombEntry`, play the referenced 3D sound and call `BattleFogForceStop`
   (`0x800fb95c`–`0x800fb9a4`). Position/effect parameters and sound binding
   are specified in the orchestration contract; this flag also drives presentation.
6. Stop the action command, set action-command help display type to zero,
   then call `BattleDamageDirect(-5,A,null,work+28,0,work+24,0,1)`
   (`0x800fb9a8`–`0x800fb9d8`). The direct application routine's handling of
   negative damage, HP caps, death and event flags is specified in
   [direct application](BATTLE_DAMAGE_APPLICATION.md).
7. If work+2c is nonzero, clear D+0x274 and call
   `BattleRunHitEvent(D,work+2c)` (`0x800fb9dc`–`0x800fb9f8`). Return −1.

The selector itself has no direct RNG calls, HP changes, visual effects or
status writes; the consumer and called helpers provide those effects. This
does not prove the entire operation consumes no randomness.

## Derived verification cases and gaps (P)

Check precedence with several early C bits set and selectively enable weapon
resistance; verify only the first eligible row contributes. Verify code 48's
zero return still leads to counter processing through work+24. Combine later
effects to check accumulated amount/request flags and last-writer code priority.
Exercise Payback plus part 0x100, and add Hold Fast plus Return Postage: there
are three independently rounded half contributions, not four. For HP result 5
and no flat additions, these contribute 6; for result 1 their sum is zero,
then the equality fallback makes it 1. These examples are derived expectations,
not executed-original observations.

Test null attacker leaves the supplied record untouched and nonnull entry
does not initialize fields other than +28. Preserve this distinction when
binding original event services. Trace all record consumers/callers, full
damage orchestration, immunity/status helper behavior, countercode handlers,
attack data and presentation before claiming complete counterattack equivalence.
