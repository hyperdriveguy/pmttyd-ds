# Switch combat parameters and defense selection

Baseline: [Switch 1.0.0 main build and archive](BINARY_BASELINE.md). Addresses
below are relative to `main`; no GameCube address applies. This document contains
verified data and two bounded native query contracts. It does not yet establish
the complete guard/superguard acceptance algorithm, damage formula, input latency
or real-time windows.

## Relocatable tables

| RomFS file under data/ | Table symbol | Stride | Rows |
| --- | --- | ---: | ---: |
| `battle/data_Parameter.elf.zst` | `_ZN3wld3btl4data6s_DataE` | `0x18` | 360 |
| `battle/weapon/data_battle_weaponac_mario_ac.elf.zst` | `_ZN3wld3btl4data6s_dataE` | `0x108` | 197 |
| `battle/weapon/data_battle_weaponac_party_ac.elf.zst` | `_ZN3wld3btl4data6s_dataE` | `0x108` | 56 |

Symbol sizes are 8,640 / 52,008 / 14,784 bytes. The parameter table also has
`_ZN3wld3btl4data9s_DataNumE` after its array. Records use ABS64 relocations to
strings; the pointer storage bytes before relocation are not the string address.
`tools/switch_table_evidence.py` resolves those relocations and rejects unaccounted
pointer relocations, unresolved targets and incompatible strides. Reports retain
raw numeric bits plus signed/unsigned/float interpretations, not guessed types.

Parameter records contain ID pointer at `+0`, binary32 value at `+8`, unknown
32-bit word at `+0xc`, and optional pointer at `+0x10`. **26 rows have null IDs**;
for example rows 0–4 contain value 20, only row 0 naming BINGO_SLOT_1ST_INIT_VAL.
Repeated IDs also exist (RING_COMMAND_WIDTH at `.data+0x18a8` and `+0x18d8`).
Do not drop null rows, invent IDs, merge duplicates, or assume array lookup rules.

Action-command records have ID at `+0` and 64 words at `+8..+0x104`.
Unsimplifier calls the first word `frameWindow`; direct code confirms that defense
and counter consumers read more than the first word. Treating that label as the
entire row's semantics loses the seven-group behavior below.

## Native seven-group defense parameters

Mario action-command row 0 is MARIO_WEAPON_DEFENCE; row 1 is MARIO_WEAPON_COUNTER.
Each contains seven groups at row offsets `8 + group*0x20`. The first two words
of each group are:

| Group index | DEFENCE first | DEFENCE second | COUNTER first | COUNTER second |
| ---: | ---: | ---: | ---: | ---: |
| 0 | 28 | 12 | 6 | 15 |
| 1 | 24 | 16 | 5 | 15 |
| 2 | 20 | 20 | 4 | 15 |
| 3 | 16 | 24 | 3 | 15 |
| 4 | 12 | 28 | 2 | 15 |
| 5 | 8 | 32 | 1 | 15 |
| 6 | 4 | 36 | 1 | 15 |

The remaining six words in each of these groups are zero. The last eight words
outside the seven groups are separately preserved in the evidence report.
The table is a stored numerical contract, not a table of displayed frames.

### Query at main+0x779ff0 (research label: defence_parameter_query)

1. Form the string pointer MARIO_WEAPON_DEFENCE (`0xcdfb2d`), call `0x25150`
   with second argument zero, then call lookup helper `0x4e52f0` using the
   registry at `0x2573d10`. The helper names are not original debug symbols.
2. Read the returned wrapper through `[result]`, then pointer `+0x18`.
   Load seven signed-width words from row offsets `8, 0x28, 0x48, 0x68,
   0x88, 0xa8, 0xc8` (sites `0x77a044..0x77a074`).
3. Scan 64 pointer slots at an object-relative base `+0x38`, in increasing
   slot order. Skip null slots. The first object with word `+0x68 == 4` and
   word `+0x6c == 0` is selected (`0x77a080..0x77a0a8`). No matching object
   returns floating-point zero (`0x77a11c`). Object-type names remain unverified.
4. Compute a 32-bit subtraction of selected-object fields `+0x168c - +0x1688`,
   add 3, clear negative signed results to zero, and clamp to at most 6.
   For ordinary non-overflowing values: `index = clamp(3 + a - b, 0, 6)`.
   The instruction arithmetic wraps at 32 bits; do not silently substitute
   unbounded integers in an exact model. Sites `0x77a0ac..0x77a0d0`.
5. Select the first-word array at that index, perform the context/cleanup path
   below, convert the selected signed integer to binary32, and return it in s0
   (`0x77a16c`). Equal modifiers select 16; this does not prove that a newly
   started game always has equal modifiers.

The intervening context path reads objects through pointer slots associated with
`0xf0be98` and `0xf0c400`, calls virtual methods, clears selected-context `+0xa50`,
and disposes the old callback using different virtual slots depending on whether
it equals the inline object at `+0xa30`. Those effects are part of the query;
it is not proven pure. Null context branches later dereference their zero result,
so they are not evidence for a safe zero return. Missing registry data also lacks
a locally checked recovery path. These paths assume valid surrounding state.

### Query at main+0x77a180 (research label: counter_parameter_query)

Same seven offsets, scan predicates, clamped modifier calculation, cleanup and
float return, using MARIO_WEAPON_COUNTER (`0xc9e922`). Main sites:
`0x77a1d4..0x77a204` loads; `0x77a214..0x77a238` scan;
`0x77a23c..0x77a260` index; `0x77a2ac` missing-object zero;
`0x77a2fc` signed integer conversion. Equal modifiers select 3.

Both full query ranges and eleven selected instruction anchors are fingerprinted
by `tools/switch_nso_evidence.py`. Source field names, modifier writers, script
registration names and gameplay callers still require recovery.

## Defense update prefix: direct timing evidence and limits

The larger routine beginning `0x7225c4` looks up both rows. It loads the seven
first words of DEFENCE and the first two words of each COUNTER group, then uses
the same object predicates and modifier index (`0x722710..0x72277c`). This directly
corroborates the grouped layout; it is not inferred solely from the data editor.

A local timer structure is formed at entry object `+0x4a64`. In the reviewed
prefix, positive binary32 fields at timer `+0xc` and `+8` are decremented by a
binary32 field at another object's `+0x4a78` (`0x7227f8..0x72284c`). A third timer
at `+4` uses that decrement at `0x7228e8..0x722910`. The expiry branch can load a
selected COUNTER second word into timer `+0xc`, with a conditional 0.5 multiplier
at `0x722918..0x72292c`. The predicate's full gameplay meaning remains unresolved.

Consequently neither a fixed decrement of one nor conversion by a guessed 30/60
Hz follows from these bytes. Recover the decrement producer, input sampling and
pause/slow-motion behavior before stating an effective timing window in seconds.
Do not import the GameCube guard-history count or multiply/divide it to fit these
numbers. The larger routine and the impact-check candidate at `0x723bd8` have
not been completely reviewed; only the explicitly described prefix is a contract.

## Other stored action-command distinctions

The following sequences are the first word at `+8` of ordered named rows; suffix
0–10 is a source identifier, not yet proven a difficulty mapping:

| ID family (suffix 0–10) | Stored sequence |
| --- | --- |
| MARIO_WEAPON_FIRST_NORMAL_JUMP | 20, 20, 10, 10, 9, 9, 8, 8, 7, 7, 7 |
| MARIO_WEAPON_NORMAL_JUMP | 20, 20, 10, 10, 9, 9, 8, 8, 7, 7, 7 |
| PARTY_WEAPON_PKR_NORMAL_JUMP | 10, 10, 10, 10, 10, 10, 8, 6, 4, 2, 1 |

FIRST_NORMAL_JUMP starts at row 3 (`.data+0x318`); NORMAL_JUMP at row 36
(`+0x2520`). Their `+0xc` values differ for suffixes 2–10: first-strike rows use
4 while ordinary rows use 3. Thus even matching first-word sequences do not
establish identical command behavior. First and ordinary normal hammer rows
169/173 (`+0xae48`/`+0xb268`) start with words `4,25,62`. They require their own
consumers; the jump decoder must not be applied to them by analogy.

## Selected battle parameter values

Offsets are relative to `.data` of data_Parameter.elf; all values below are stored
binary32 numbers. Names indicate research leads, not completed formula semantics.

| Offset | Original ID | Value |
| --- | --- | ---: |
| `0x528` | FOG_HIT_RATE | 50 |
| `0x540` | FOG_TURN_COUNT | 2 |
| `0xcf0` | KIKEN_POWER | 5 |
| `0xd08` | PINCH_POWER | 2 |
| `0xd20` | AVOID_RATE_TAMANI_LUCKY | 10 |
| `0xd38` | AVOID_RATE_KEKKOU_LUCKY | 25 |
| `0xd50` | AVOID_RATE_PINCH_LUCKY | 33 |
| `0xe28` | STATUS_FIRE_DAMAGE | 1 |
| `0x17b8` | MAX_DAMAGE | 99 |
| `0x1800` | DEFEND_COMMAND_DF_VALUE | 1 |
| `0x1848` | BTLPAD_INPUT_LEVEL_STICK | 40 |
| `0x19c8` | KOBURON_DROP_SUPER_TANKOBU_DAMAGE | 4 |
| `0x19f8` | KOBURON_HEAL_MAX_COUNT | 1 |
| `0x1b60` | PRINCE_MASH_NORMAL_DEFENCE_POWER | 99 |

These entries do not prove universal damage caps, independently rolled badge
chances, status duration, secret-boss behavior or progression requirements.
Trace lookup selection, overrides, comparison boundaries, RNG draws and consumers.

## Fingerprints

Decompressed ELF SHA256:

- Parameters: `63de45ad7ae99760f60540af8e8b32ec9d8ba0c788ff30e9ebd162185f8dd228`.
- Mario AC: `b613999b500dcbd07678da4ced248d468a209ce7d6174f941c1e223f252592b8`.
- Partner AC: `22a1584eab08d38abfbc3f42ea812fb80366f9f0df91e79114cf350f8524a4e1`.

Compressed hashes, every row's ELF file offset and relocation target, and exact
binary32 bit patterns are in the reproducible `tables.json`. No original game
function was executed to obtain these findings.
