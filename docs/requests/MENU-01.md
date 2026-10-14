# MENU-01 — Original field pause menu entries and field-state contract

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the full-game path ([milestones](../MILESTONES.md)); the demo only
needs to open and leave it once, but every later session enters it, so it is
listed as the end-goal branch of the milestone map. The documentation slice is
explicitly requested; no independent research-agent ownership or completion
claim is made.

## Open todos

- [ ] The original field menu's entry table and order (the field pause menu
  opened from the field), the per-entry enable/disable conditions and which
  field/global state each entry reads.
- [ ] Navigation and cursor ownership: how the menu is opened and closed, what
  the field does while it is open (pause, camera, NPC/wander, timer and audio
  ownership), and how the open/close transition hands control back to the field.
- [ ] Per-entry content for the field menu — party/status, badges, items, the
  journal/mail, gear and options — and where each entry's own screen is owned.
- [ ] The input mapping of the original field menu (button/touch/stick
  ownership) versus the native `Menu` fixture.
- [ ] Whether the field menu can save directly and how that relates to the save
  block ([SAVE-01](SAVE-01.md) keeps the file format; the save block stays with
  the field save-block contracts).

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
full-game path: the native field menu is a provisional three-action fixture, not
the original pause menu. `include/runtime/game.hpp` declares `Menu menu = Menu::Status;`
over a `Menu` enum of Status/Settings/Items only, and `source/runtime/game.cpp` handles
only those three (`Menu::Settings` toggles sound, `Menu::Items` consumes a
mushroom when `hp<10`, `Menu::Status` sets `saveRequested`). `README.md` describes
the native mapping as "Y | Selected field menu action: save, item, sound" and the
port as having "touch menus and versioned saves", while the original field menu
uses the field pause/touch interface with its own entry list and its own pause
and resume behavior. Please recover the original field pause menu as one contract
— its entry table and order, per-entry enable conditions and the state each entry
reads, its navigation and open/close pause ownership, its input mapping, and its
relationship to saving — so the native menu is driven by the original entry table
instead of the three-action fixture. Other subsystems keep their own requests:
the item table draw and field item use are [ITEM-01](ITEM-01.md), the party
registry is [PARTNER-FULL-01](PARTNER-FULL-01.md), the save file format and
storage are [SAVE-01](SAVE-01.md), and character progression is
[PROGRESS-01](PROGRESS-01.md). No lock reserved.

## Answers and delivery notes (append-only history)

No delivery yet.
