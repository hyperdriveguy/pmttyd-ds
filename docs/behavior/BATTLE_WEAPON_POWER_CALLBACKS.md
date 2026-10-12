# Default and ordinary Mario weapon power callbacks

US G8ME01 revision 0; recovered 2026-10-12 by documentation agent ID 2.
Evidence B: complete original DOL callback bodies and original DOL/harbor REL
weapon records. No original execution, measured starting loadout or complete
tutorial damage trace is claimed. Names are symbol annotations; instructions,
data bytes and relocations establish the contracts below.

## Call ABI and dependencies

The [HP calculator](BATTLE_DAMAGE_CALCULATION.md) invokes weapon HP callback
`(attacker, weapon, defender, part)` in r3..r6, using pointer weapon+0x1c.
Its signed 32-bit return is **power before ordered modifiers**, not resource
damage. FP uses a separate callback at weapon+0x40. These callbacks do not
subtract resources, schedule impacts, advance action-command state or consume
Charge. Their caller gates and later modifications remain in the calculator.

| Complete body | DOL address | Size | Inputs actually used |
| --- | --- | --- | --- |
| weaponGetPowerDefault | 0x80189d30 | 8 | Weapon only |
| weaponGetPowerFromMarioJumpLv | 0x80189cb0 | 0x78 | Attacker, weapon, shared battle work, shared pouch |
| weaponGetPowerFromMarioHammerLv | 0x80189c38 | 0x78 | Attacker, weapon, shared battle work, shared pouch |
| pouchGetPtr | 0x800d5eb0 | 8 | Shared pointer at r13+0x1be0 |

Defender and part are ignored by all three power callbacks. Default also ignores
attacker, so a null attacker need not prevent this local callback from working;
the surrounding calculator has its own selective null handling. Mario callbacks
dereference attacker, battle and pouch without local null checks. Their valid
object/level domain must be supplied by runtime initialization and loadout owners.

## Default callback and concrete boss operand

The entire default body loads one 32-bit word from weapon+0x20 and returns it.
No byte/halfword narrowing, saturation, RNG, status query, fallback, action-command
test or mutation occurs. Retain its two's-complement word result, including zero
and negative values; do not replace the callback with a nonnegative byte field.

Opening Crump's weapon record `weapon_kanbu1_attack` is at **gor.rel file offset
0x64718**, size 0xc0. Its ADDR32 relocation at +0x1c binds DOL 0x80189d30.
Its signed word +0x20 is **1**, so this unchanged record supplies base HP power1.
The FP callback word +0x40 is zero and has no relocation there. The ordinary
HP-then-FP calculator contract therefore supplies no FP callback damage for
this record. The weapon element byte +0x6c is0; special flags word +0x74 is
0x1087. These inputs feed the documented HP modifier pipeline.

This resolves the actual weapon's base operand, not every hit's final damage.
Unit/part defenses, statuses, badges, guard dispatch and mode/flag state can
change the calculated result; runtime weapon mutation has not been exhaustively
excluded. The [boss attack](HARBOR_BOSS_NORMAL_ATTACK.md) passes this weapon
through inherited local9 and CheckDamage flags0x100. Its defense byte +0x13=1
is an eligibility input, distinct from power and battle flag0x1000.

## Mario callbacks: ordered reads and signed index

Both bodies execute the same algorithm with distinct pouch/unit byte offsets.
All offsets below are hexadecimal. Word arithmetic wraps at32 bits; comparison
of index with6 is signed. `S8` and `U8` mark original signed/unsigned byte reads.

1. Retain attacker A and weapon V. Load shared BattleWork pointer from
   r13+0x1c70, then read word BattleWork+0x1cb8. Capture
   `successBit = (word >> 1) & 1` **before** requesting the pouch pointer.
2. Call pouchGetPtr. It returns the word pointer stored at r13+0x1be0, with no
   null check or mutation. Read level as S8(pouch+0x98) for Jump, or
   S8(pouch+0x99) for Hammer.
3. Compute `index = 2*(level-1) + successBit` with32-bit operations. If signed
   index >=6, set index=5. There is **no lower bound clamp**. Negative index is
   retained and its scaled displacement wraps as a32-bit address calculation.
4. Read the bonus byte as U8(A+0x2ff) for Jump, or U8(A+0x300) for Hammer.
5. Load the 32-bit power word at V+0x20+4*index. When bonus !=0, add bonus
   with32-bit wrap; otherwise return the loaded word unchanged.

No callback checks the numerical AC result, command ID, equipment name, defender,
part, hit count or battle0x1000. Only the captured state bit controls which half
of a level's pair is selected. The callback does not write any of these fields.
Do not move the bonus load after the power-word read or reread successBit after
pouch access when implementing an ordered service interface.

For level0, index is -2 or -1; the body reads before the six-word table instead
of upgrading/clamping to level1. For a stored byte0xff, level is -1 rather than
255. Levels4..127 select index5 through the signed upper clamp. The valid normal
level range is an initialization/progression obligation; these local edge cases
are not evidence that invalid levels occur in ordinary gameplay.

## Concrete ordinary weapon tables

Each shipped ordinary weapon is0xc0 bytes. The HP callback pointer and six
signed power words are at +0x1c and +0x20..+0x34 respectively.

| Original record | DOL address | HP callback | Six words, index0..5 | Event pointer at +0xb0 |
| --- | --- | --- | --- | --- |
| marioDefaultWeapon_Jump | 0x803538a8 | 0x80189cb0 | 1,1,2,2,3,3 | 0x80355cf0, ordinary Jump stream |
| marioDefaultWeapon_Hammer | 0x80353de8 | 0x80189c38 | 1,2,2,4,3,6 | 0x80416c40, ordinary Hammer wrapper |

Both records have zero FP callback and element byte0. Their special flags are
Jump0x35807 and Hammer0x30887. The [ordinary attack bindings](BATTLE_ORDINARY_ATTACK_BINDINGS.md)
and [Hammer branches](BATTLE_HAMMER_WEAPON_BRANCHES.md) document those event
selections separately from power calculation.

At zero bonus, the concrete callback results are:

| Level | Jump, successBit=0 / 1 | Hammer, successBit=0 / 1 |
| --- | --- | --- |
| 1 | 1 / 1 | 1 / 2 |
| 2 | 2 / 2 | 2 / 4 |
| 3 | 3 / 3 | 3 / 6 |

These are derived local cases, not executed hit totals. In particular, ordinary
Jump's callback does not double a single hit's power when bit1 is set. Its attack
stream's subsequent-hit/AC/stop/damage paths must be recovered to establish
total attack behavior. A caller must use the actual weapon instance's table,
since this callback is not restricted to these two ordinary pointers.

## Implementation requirements and acceptance cases

These requirements close the scoped DMG-01 callback request; they do not form
the final complete-game requirements document.

| ID | Requirement | Derived acceptance case |
| --- | --- | --- |
| PWR-01 | Preserve callback ABI and signed32 power independently of resource application | Default ignores attacker/defender/part and returns weapon word0,-1 or150 unchanged |
| PWR-02 | Preserve captured battle bit, signed level, upper-only index clamp and byte bonus | For each ordinary level1..3, test both bits and bonus0/255; level4 selects word5; retained invalid level0 addresses table-8/table-4 in a bounded model |
| PWR-03 | Use each concrete weapon's callback/table and separate FP callback | Boss yields base1; ordinary table pairs match above; zero FP callback follows FP calculator's zero-return/code-preservation path |
| PWR-04 | Keep initialization and attack lifecycle as explicit dependencies | Do not supply invented level1,bonus0,hit count or tutorial AC bit as established original state |

Host tests can verify arithmetic/order and selected data, but original execution
must establish actual starting level/bonus/state, weapon lifetime and multi-hit
transitions. Remaining owners: pouch level initialization/upgrades/save reload;
unit+0x2ff/+0x300 initialization/badge writers; AC state-bit producers at each
hit; unit/part defense construction; damage flags, counter/precheck/application,
death/revival/reward/field return. DMG-01 remains partially open.

## Reproduction and evidence retention

Use tools/inspect_dol_function.py with the four named symbols above,
--symbols artifacts/reference/us_symbols.csv, and an ignored artifact or /tmp
output. tools/behavior_evidence.py records their whole-body fingerprints;
tools/verify_behavior_contracts.py independently pins32 instructions, full body
and weapon-record hashes, concrete power tables, FP/event pointers and the boss
base-power word plus its previously checked callback relocation.

ISO reads use binary read-only mode. No original asset, executable, extracted
body or dialogue is committed. This documentation adds no new asset conversion,
redistribution permission or runtime/gameplay observation.
