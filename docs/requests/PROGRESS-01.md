# PROGRESS-01 — Original character progression and its save persistence

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the full-game path ([milestones](../MILESTONES.md)); the demo's
first encounter explicitly has a no-level-up tail, so this is deferred past D1 but
gates every later chapter. The documentation slice is explicitly requested; no
independent research-agent ownership or completion claim is made.

## Open todos

- [ ] How the original awards battle-end experience (star points) — the
  producer, the per-unit values, the accumulation and the carry into
  level-up — beyond the opening trio's presentation/counting.
- [ ] The level-up threshold/curve and the level-up bonus selection (HP/FP/BP),
  including the selection screen ownership, default, and what writes each choice
  makes.
- [ ] Badge-point (BP) accounting, badge equipping/unequipping and the field
  badge menu's effect on BP, distinct from the numeric equip import already
  recovered.
- [ ] Partner progression (rank/upgrade) if it shares the same state, and the
  presentation trigger/order for a level-up or rank-up.
- [ ] How level, star points, BP and equipped badges persist in the save block
  and round-trip on load; the record format stays with [SAVE-01](SAVE-01.md).

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
full-game path: the native port has no character progression. `include/runtime/game.hpp`
declares `struct Progress` with only `int hp = 10, mushrooms = 1;` and no level,
star points, badge points or equipped badges, and it carries
`uint8_t jumpLevelFixture=1,hammerLevelFixture=1; // Pouch/loadout initialization
is not yet bound.` The opening trio's battle-end is documented as having a
no-level-up tail
([no-level-up tail](../behavior/battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md))
and its experience presentation/counting
([EXP presentation](../behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md)),
but the progression system itself — how star points accumulate, when a level-up
fires, the HP/FP/BP bonus choice and its screen, badge-point accounting and badge
equipping, and how all of it persists — is not recovered. Please recover the
original progression contract as one system: the battle-end experience producer,
the level-up threshold/curve and bonus-selection writes, BP accounting and badge
equipping, partner rank progression if it shares the state, and the
presentation order — so later chapters level the party the original way instead
of via native fixtures. Damage/stat bindings stay with
[DMG-01](DMG-01.md)/[DMG-02](DMG-02.md), status lifetime with
[STATUS-01](STATUS-01.md), the numeric badge/equip import stays where it is, the
item/field menu with [ITEM-01](ITEM-01.md)/[MENU-01](MENU-01.md), and the save
record format with [SAVE-01](SAVE-01.md). No lock reserved.

## Answers and delivery notes (append-only history)

No delivery yet.
