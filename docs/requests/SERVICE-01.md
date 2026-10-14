# SERVICE-01 — Shops, inns and NPC item/coin services

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the full-game path ([milestones](../MILESTONES.md)); no shop
appears in the demo sequence, so this is deferred past D1 but gates every town
and hub from the prologue's Rogueport onward. The documentation slice is
explicitly requested; no independent research-agent ownership or completion
claim is made.

## Open todos

- [ ] The original shop/menu service contract: how a shop is opened from an NPC
  or a placed object, the item/service stock and prices, the buy flow
  (quantity, confirmation, capacity and coin guards), and any sell flow.
- [ ] The coin transaction rules: the coin field the transaction reads/writes
  (native sets `pouch.half(0x78,100)` at new-file), deduction/addition order,
  the insufficient-coin path, and how a purchase writes the pouch
  ([ITEM-01](ITEM-01.md) owns the pouch layout).
- [ ] The inn/heal services and any other coin-for-effect services, including
  the message prompts, choices and the state they change (HP/FP, story).
- [ ] NPC services that are not shops (tutors, movers, the Trouble Center and
  similar), and the event functions that back them, so native services use the
  original event entry points rather than hand-written menus.
- [ ] Story gating and stock changes per chapter, and the service
  presentation/sound ownership.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
full-game path: the native port has no shop, inn or service system. Coins exist
only as a raw pouch field (new-file sets `pouch.half(0x78,100)` in
`source/prologue_pouch.cpp`), and the only "item" interaction is the field
`Menu::Items` action that directly consumes a mushroom when `hp<10`
(`source/game.cpp`), with no price, stock, buy/sell or NPC service. No behavior
contract covers shops, inns or NPC services (the incidental "shop" mentions are
in unrelated storage/card/map documents). The original towns and hubs are built
around these services, so the port cannot be played past a shop without a native
stub. Please recover the original shop/inn/NPC-service contract as one system —
how a service opens, the stock/prices and buy/sell flow, the coin transaction
and its guards, the inn/heal and other coin services, the non-shop NPC services
and their original event entry points, per-chapter story gating and stock
changes, and the presentation — so later chapters offer original services. The
pouch/inventory layout stays with [ITEM-01](ITEM-01.md), the field menus with
[MENU-01](MENU-01.md), message playback with the message contracts, event
scheduling with [EVT-01](EVT-01.md), and story flags with [GSF-01](GSF-01.md).
No lock reserved.

## Answers and delivery notes (append-only history)

No delivery yet.
