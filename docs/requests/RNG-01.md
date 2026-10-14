# RNG-01 — Original seed lifecycle and random-consumer census

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the demo path with full-game intent ([milestones](../MILESTONES.md)).
The D1 tutorial battle already draws from `irand` (candidate choice, packed
weights, turn order, damage/status rolls), and the same generator drives the
whole game, so its seed lifecycle and draw order gate both D1 and G1. The
documentation slice is explicitly requested; no independent research-agent
ownership or completion claim is made.

[Randomness](../behavior/meta/RANDOMNESS.md) has already recovered the integer
generator (`rand` at `0x8026c324`) and the bounded helper (`irand` at
`0x80005da0`), including the no-draw `bound==0` case and the modulo bias. What is
still open is everything around them: the seed's origin and writers, the other
helpers, the consumer census with conditional draw order, and save persistence.

## Open todos

- [ ] Seed lifecycle: the origin and initial value of the generator state
  (`0x8041e3c0`), any reseeding or direct writers at new game, title/demo, and
  save load, and the emulator/launch initialization value — so the native port
  seeds the state the original does rather than an assumed constant.
- [ ] `randf` (`0x80137a10`) and the other random helpers: whether they share
  this state or hold independent state, and how many generator draws each call
  consumes (including any zero-draw paths).
- [ ] Consumer census with conditional draw order and counting by actual
  generator invocations, not `irand` calls: battle (candidate choice, packed
  weighted choice, turn-first span, damage/status application, attack
  registration, majinai queries), the audience/stage, drops/XP, encounters,
  NPC/field effects, shops, minigames, and the message glyph/font effects that
  already call `irand(10000)`/`irand64`. Preserve conditional order and keep the
  draw cadence independent of a new render frame or optional visual effect.
- [ ] Save persistence: whether and how an original save record preserves the
  random state, or whether the state is reinitialized on load — recorded as
  observed behavior, not an assumed seed field.
- [ ] Matched original/native traces pairing seed-before, call site, arguments,
  result, seed-after and simulation tick, and a rule for distinguishing a changed
  draw order from a changed formula when results diverge.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
demo path with full-game intent: the native port has no recovered random-state
lifecycle or consumer order. [Randomness](../behavior/meta/RANDOMNESS.md) fixes
the `rand` recurrence (`state*1103515245+12345 mod 2^32`, return bits 16..30) and
`irand` (single `rand` draw, modulo, no rejection, no draw when the bound is 0),
but explicitly leaves open seeding/reseeding (its item 1), `randf`/other helpers
(item 2), the consumer census with conditional draw order (item 3) and save
persistence (item 4).

The native battle code already routes many consumers through `platform.irand` /
`BattleRandom` (`source/battle_timing_a.cpp`, `source/battle_candidate_choice.cpp`,
`source/battle_packed_choice.cpp`, `source/battle_turn_first.cpp`,
`source/battle_damage_application.cpp`, `source/battle_attack_registration.cpp`,
`source/battle_majinai_query.cpp`, `source/trio_attack.cpp`), and the message
renderer consumes two `irand(10000)` draws per admitted dynamic/shake glyph and
two `irand64` draws per inner font quad. None of these sites has a recovered
seed-before/draw-order contract, so the port cannot reproduce the original draw
sequence. For the D1 tutorial battle this already matters: the recovered command
path draws through `irand`/weighted choice, and a different seed or draw order
changes the observable result.

Please recover the original random-state contract — the seed origin and all
writers across new game/title/demo/load, `randf` and the other helpers with their
draw counts, the full consumer census with each call's arguments, conditional
order and simulation-tick placement, and the save-record persistence question —
so the native port reproduces the original draws rather than an assumed seed.
Keep the generator arithmetic with [randomness](../behavior/meta/RANDOMNESS.md),
battle command timing with [AC-01](AC-01.md)/[DMG-01](DMG-01.md), status turns
with [STATUS-01](STATUS-01.md), message glyph effects with the message
contracts, and save storage with [SAVE-01](SAVE-01.md). No lock reserved.

## Answers and delivery notes (append-only history)

No delivery yet.
