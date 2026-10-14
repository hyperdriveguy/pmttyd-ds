# ITEM-01 — Field inventory/pouch and field item use

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the first demo milestone ([milestones](../MILESTONES.md)) and the
full-game end goal. The slice is explicitly requested; no independent
research-agent ownership or completion claim is made.

## Open todos

- [ ] The original field inventory/pouch record layout, item slots and counts, as
  distinct from the battle pouch already documented.
- [ ] Field item use: which items are usable in the field, their effects and
  consumption, the full-HP/eligibility guards, and the menu/targeting flow.
- [ ] Where field and battle pouch state share or copy, and how the save block
  stores the field inventory.
- [ ] Item acquisition at the opening stage (pickup/chest/shop) and the key-item
  (key-item count) semantics the NPC gates already read.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13), first
demo milestone + full-game end goal: the native field item menu is a fixture.
`include/game.hpp` carries `int hp = 10, mushrooms = 1;` and
`enum class Menu { Status, Map, Items, Settings }`; `source/game.cpp` heals from a
counter (`else if (menu==Menu::Items && progress.mushrooms>0 && progress.hp<10) {
--progress.mushrooms; progress.hp=std::min(10,progress.hp+5); ... }`) and refuses
at full HP, matching the README's "a mushroom cannot be used at full HP" note.
The battle pouch is separately recovered (`source/battle_packed_pouch.cpp`:
0x5d4-byte pouch with 121/20/32/200/200 counts, seven 14-byte partner records;
`source/battle_damage.cpp` item151 revival check), and
`progress.originalPouch->keyItemCount(95/107)` is read by field NPC gates, but the
original field inventory — item definitions, slot/count layout, field-use effects
and guards, the menu flow and save storage — is unrecovered, so the demo's items
and every later chapter's inventory are limited to a mushroom counter.
[DMG-01](DMG-01.md) and the weapon-table deliveries cover battle item rows, not
field inventory state or use. Please recover the original field inventory/pouch
and field item-use contract. No lock reserved.

## Answers and delivery notes (append-only history)

Delivered [pouch item dispatch](../behavior/battle/POUCH_GET_ITEM_DISPATCH.md)
(2026-10-12, no lock, static artifacts only): central receive entry with
verified type-id routing (coins cap 999, capped global counter, HP-ish and
flag-byte leaves, badge subroutine for 240–338, key insert over 121 slots),
slot bases +0xA0/+0x192 and 10/20 capacity cross-checked against the snapshot
`PouchData` layout, single shared pouch pointer (no separate battle pouch at
this layer), and 1/0 store/miss returns with memmove compaction. Effects,
consumption, save format, shop/chest/pickup callers, and the 0x800858fc +
0x800d4ed4 + 0x801410e8 callees remain open. No native/test/CMake/asset
change by this answer.

### 2026-10-13 — Goomba quest13 native binding and upstream recovery

Delivered [native quest13 hand-in](../behavior/field/NATIVE_GOOMBA4_QUEST13_HAND_IN.md),
[native pouch checkpoint v8](../behavior/format/NATIVE_POUCH_CHECKPOINT_V8.md),
and [Trouble Center writers/acquisition](../behavior/field/TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).
First/continuing source dialogue, duplicate item selector, original Mario poses,
5407 write and checkpoint persistence are bound for explicit known state.
Native Trouble Center acceptance, quest2 item-entry/GetItem presentation and
quest13 acquisition remain pending; parent request stays open.
