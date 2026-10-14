# STAR-01 — Crystal Stars, star power and special moves

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the full-game path ([milestones](../MILESTONES.md)); the prologue
grants no Crystal Star, so this is deferred past D1 but gates every battle from
chapter 1 on. The documentation slice is explicitly requested; no independent
research-agent ownership or completion claim is made.

## Open todos

- [ ] The star-power gauge: where the original stores it, what fills it (the
  audience, stylish/action-command performance, first strike, other sources) and
  by how much, the thresholds/levels, and what consumes or resets it.
- [ ] The battle Star menu: how it is offered and toggled, which stars/moves are
  available and in what order, the selection and cancellation controls, and the
  cost/consume applied on use.
- [ ] Each special move's resolution: its action-command timing/checks, target
  selection (single/all/enemy/party), damage, healing, status or
  audience/meter effects, and how it touches unit parts and the packed target
  services.
- [ ] Which special move each Crystal Star grants, and the acquisition and story
  gating (the star/flag that grants each star) and ordering across chapters.
- [ ] Presentation: the star-power meter display, move effects, camera and
  sound, and the field/battle availability rules.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
full-game path: the native port has no Crystal Star or special-move system. No
`source/` or `include/` symbol references star power or a special move, and the
documentation has no star-power contract — the audience pipeline that fills the
gauge is recovered
([battle fortune](../behavior/battle/BATTLE_FORTUNE_PRODUCER.md),
[audience requests](../behavior/battle/BATTLE_PINCH_AUDIENCE_REQUESTS_AND_SOURCE_THRESHOLDS.md)),
and the battle menus are recovered for the action-command, operation, party,
weapon and multi-item menus, but not for a Star menu. Each chapter awards a
Crystal Star whose special move is used in battle from a star-power gauge, which
is a full-game system the port does not model. Please recover the original
Crystal Star / special-move system as one contract — the star-power gauge and its
fill/consume rules, the Star battle menu (availability, selection, cost and
cancel), each special move's action-command resolution and effects, the
star-to-move mapping and per-chapter acquisition/story gating, and the
presentation — so later chapters offer original special moves instead of native
stubs. The audience generation that charges the gauge stays with the audience/
battle-fortune contracts, the action-command menu stays with the battle menu
contracts, damage application stays with [DMG-01](DMG-01.md)/[DMG-02](DMG-02.md),
status with [STATUS-01](STATUS-01.md), and star/story flags with
[GSF-01](GSF-01.md). No lock reserved.

## Answers and delivery notes (append-only history)

Delivered [star-power gauge storage](../behavior/battle/STAR_POWER_GAUGE_STORAGE.md)
(2026-10-13, no lock, static artifacts only) for todo 1 (storage half):
`pouchAddAP`/`pouchSetAP` complete add/floor/cap at pouch+122/+124 with US
rows and an explicit storage-vs-policy boundary. Fill callers (2 link
sites), thresholds/levels, max-SP writers, consume/reset, and save mapping
stay open. No native/test/CMake/asset change by this answer.
