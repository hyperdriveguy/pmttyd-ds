# Packed counter precheck and hit event ownership

Read-only US source identities:

| Body/data | Address | Size |
| --- | --- | --- |
| BattleCheckCounter | 800fa678 | 5a4 |
| BattleInitCounterPreCheckWork | 800fac1c | 3c |
| Counter result jump table | 8034c720 | 30 |
| BattleRunHitEvent | 8012984c | 84 |
| BattleRunHitEventDirect | 801298d0 | c4 |

The separate portable services operate on caller-owned source storage and native
memory providers; they do not assume the ProfessorStory roster or an enemy ID
range. Source bytes remain ignored. `verify_harbor_counter_services.py` pins all
five spans, including the actual jump table after signed address construction.

## Counter precheck

`BattleCounterWork::init` zeroes twelve words and sets word0 to1. CheckCounter
returns0 immediately for a NULL attacker, before touching the work or providers.
Otherwise it clears only count word10. The first matching part1b0 reaction in
bits1,2,4,30,c0,c00 terminates the branch chain, subject to source attacker immunity
bytes302/301, weapon78 bypass bits and attacker status6. These blocking counters
write code43..48, code|100, count1, pattern31..36 and word0=0. Source secondary
flags set scratch words2/5/4 for relevant doubled part bits.

The remaining branch chain can accumulate several gates while overwriting its
final code. It checks part1000 against weapon element and attacker status8;
part2000; defender status6 or partc00 against attacker status6; defender statuses21
and22; part300 and defender badge byte2f0. Short-circuit status calls retain source
order. A hit sets incoming flags|100. Later chains retain earlier count/gates and
do not restore word0 or clear other fields. The return table for code43..54 is
`8,7,9,10,11,0,12,13,15,14,16,17`. In particular code48 returns0 while its scratch
record still blocks ordinary damage. A caller must inspect the record rather
than infer permission to calculate HP from this return value.

## Hit event lifecycle

`packedBattleRunHitEvent` checks low hit code40 against actual unit data-table key46
before reading its installed damage script. Presence of key46 suffices even if its
data value is0. Absent installed damage script returns0 without cancellation.
The Direct entry cancels nonzero IDs in order2b4,290,29c,2a8 and clears each only
after its deletion callback. This occurs even for a NULL supplied script. With
script nonzero, it creates priority10/flags20 root, assigns unit word0 as owner,
then writes full hit code278 and new root ID2b4, returning that ID. There is no
HP check, assumed death root or battle result publication in these services.

## Native harbor integration boundary

These prerequisites are executable and tested but are not yet a full CheckDamage
callback. Original800fb01c first initializes counter scratch, records last part258,
checks superguard, executes CheckCounter, then uses its complete scratch record
before calculators, status application, DamageDirect and possible retaliation.
The existing TrioAttack narrow shortcut must not be broadened by an assumed
Crump kind gate: it does not yet own this full original control flow.

Original Mario kind+bc selects init80372ec4. The explicit fresh entry now runs
its twenty commands with source-owned fresh pouch and originalStory4, installing
wait80417c38, attack80417c30, damage80417c08 and confusion8034f1e8, original sound/
pose/parts/emblem state, entry80372bc0 and start-wait. Legacy begin retains the
older init/phase diagnostic boundary; parent native opening must select the new
fresh entry. A complete player command, common damage child and reaction
lifecycle still require their original scheduler/services. Current source
ResultACDefence may complete with actual physical input, but CheckDamage remains
pending and neither HP nor battle result is fabricated.
See [Mario init](../story/NATIVE_HARBOR_MARIO_INIT.md).
See [physical defense](../story/NATIVE_HARBOR_PHYSICAL_DEFENSE.md) and
[packed tutorial owner](../story/NATIVE_HARBOR_TUTORIAL_PACKED_TARGET_SERVICES.md).
