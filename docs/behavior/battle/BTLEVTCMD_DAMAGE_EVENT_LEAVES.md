# Damage event leaves: SetEventDamage and CommandCheckDamage

US G8ME01 disc0 revision0; static contracts, no execution, cadence or original
damage-timing claim. Closes `btlevtcmd_SetEventDamage` (`0x8010a55c`,
`battle_event_cmd.o`, size `0x5c`, 29 lines) and `btlevtcmd_CommandCheckDamage`
(`0x8010a688`, size `0x104`, 71 lines) from the
[unaddressed register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md). Neither has
a definition in the pinned snapshot (rechecked with
`tools/reference_source_gap.py`). `BattleCalculateDamage`/`BattleCheckDamage`
numerics belong to the existing damage-calculation contract.

Both bodies use the same operand pipeline as the other event leaves:
`args = word[entry+24]`, each operand word is resolved with `evtGetValue`
(`0x80031260`), the resolved id is translated with `BattleTransID`
(`0x801253ec`, `battle_sub.o`) and the unit is fetched with `BattleGetUnitPtr`
(`0x800f88cc`, `battle.o`) from the shared battle pointer `B =
word[0x8041eb90]` (`r13+7280`). Resolved values are re-read per operand; no
caching across operands occurs.

## btlevtcmd_SetEventDamage (0x8010a55c, 29 lines)

1. `id = evtGetValue(args[0])`; `unit = BattleGetUnitPtr(B, BattleTransID(entry, id))`.
2. `word[unit+688]` (`+0x2B0`) = `args[1]` — the **raw** second operand word is
   stored, not a resolved value.
3. Return 2.

There is no unit-null check, no range/sign test, and no first-call gate. The
body performs no arithmetic; the caller supplies the already-computed damage
word.

## btlevtcmd_CommandCheckDamage (0x8010a688, 71 lines)

Resolves five operands and drives the shared damage checker:

1. `A = BattleGetUnitPtr(B, BattleTransID(entry, evtGetValue(args[0])))` → `r28`.
2. `B2 = BattleGetUnitPtr(B, BattleTransID(entry, evtGetValue(args[4])))` → `r27`.
3. `parts = BtlUnit_GetPartsPtr(B2, evtGetValue(args[8]))` (`0x80128cd8`,
   `battle_unit.o`) → `r30`.
4. `w4 = args[12]` (raw word) → `r25`; `w5 = args[16]` (raw word) → `r26`.
   Both are kept as raw words and neither is resolved.
5. `weapon = BattleGetSelectWeapon(B)` (`0x8012021c`, `battle_seq_command.o`) → `r6`.
6. `result = BattleCheckDamage(r3=A, r4=B2, r5=parts, r6=weapon, r7=w4)`
   (`0x800fb01c`, `battle_damage.o`).
7. `cmpwi`-style sign test via `mr. 5,3` / `bf 0`: if the signed result is
   **negative**, return 255 immediately and skip the assignment.
8. Otherwise `evtSetValue(entry, w5)` (`0x80030cfc`) publishes the raw fifth
   operand word as the destination — the resolved damage value lives in the
   checker's own output, not in this body.
9. `live = evtCheckID(word[entry+348])` (`0x80036604`, `evtmgr.o`): if zero,
   return 255; otherwise return 2.

So the two failure exits are a negative checker result and a non-live event id;
success returns 2 only after the raw destination word has been published.

## Not established

- Caller census, operands' semantic identities (which slot is attacker, target,
  part index, payload and destination), and cadence.
- `BattleCheckDamage` internals, `BattleGetSelectWeapon`'s selection rules, and
  the actual damage arithmetic.
- Whether the raw fourth operand can alias the destination word in ways that
  change the store order.
