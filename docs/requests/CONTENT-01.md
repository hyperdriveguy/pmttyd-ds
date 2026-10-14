# CONTENT-01 — Reachable chapter/interlude/minigame/optional-content census

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the full-game path ([milestones](../MILESTONES.md)). This is the
G1 end-goal row the coverage register still lists as **not recovered**; it is
deferred past D1 but is the whole remaining game after the Rogueport prologue.
The documentation slice is explicitly requested; no independent research-agent
ownership or completion claim is made.

The native port has no chapter, interlude, minigame or optional-content model.
`struct Progress` in `include/game.hpp` tracks only the prologue slice (`room`,
`hp`, `mushrooms`, `crumpDefeated`, `originalStory`=GSW0, `originalFlag1207`,
`originalFlag233`/`236`, `originalSaveMode`=GSW4, `interiorMask`). It stores the
chapter gate but has no content per chapter value, and the register's
"Chapters/interludes/minigames" row has no request.

## Open todos

- [ ] The reachable-content census: the original's map/area list and ordering,
  the chapter and interlude sequence, the minigames, the Trouble Center and any
  optional/side content, and which saved gate value (GSW0 and any other) admits
  each — building on the [GSW0 chapter value table](../behavior/story/OPENING_GSW0_CHAPTER_VALUE_TABLE.md)
  and [area-init state map](../behavior/story/OPENING_AREA_INIT_STORY_STATE_MAP.md).
- [ ] Per-mode mechanics for each non-field/non-battle mode (minigames and
  interludes) as their own systems: entry/exit conditions, inputs, rules,
  scoring/win-lose, timers, rewards and persistence — not folded into field or
  battle contracts.
- [ ] Optional and side content: entry conditions, rewards, effect on the
  chapter gate and on collectibles, and what is genuinely optional versus
  story-required.
- [ ] The ending/postgame conditions and how they gate after the finale,
  including any postgame-only content.
- [ ] A story-required versus optional split, so the native port can tell which
  content the D1 slice may omit without contradicting the recovered graph and
  which the G1 path must implement.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
full-game path: the native port implements only the Rogueport opening slice and
has no model for anything after it. `struct Progress` in `include/game.hpp`
stores `originalStory` (GSW0, the chapter gate) and a few prologue flags, but
nothing consumes a chapter value to load chapter content; there is no minigame,
interlude, Trouble Center or optional-content code (the only "stage" code is the
battle stage objects). The coverage register's "Chapters/interludes/minigames"
row is **not recovered**, its evidence needed being "reachable content inventory,
per-mode mechanics, optional content and ending/postgame conditions", and no
request covers it.

The opening deliveries already show GSW0 is a monotonic chapter counter with 34
distinct tested values and that every area init reads it, so the gate exists but
the content behind each value does not. Please recover the reachable-content
census and the per-mode mechanics: the map/chapter/interlude/minigame/optional
inventory with the gate values that admit each, the rules and rewards of each
minigame/interlude as its own system, the optional-versus-required split, and the
ending/postgame conditions — so later chapters can be built from original
content instead of invented structure. Keep the chapter gate value work with
[DEMO-01](DEMO-01.md)/[PROGRESS-01](PROGRESS-01.md), story flags with
[GSF-01](GSF-01.md), shops/services with [SERVICE-01](SERVICE-01.md), roaming
enemies with [FIELD-ENEMY-01](FIELD-ENEMY-01.md) and maps/transitions with the
overworld contracts. No lock reserved.

## Answers and delivery notes (append-only history)

No delivery yet.
