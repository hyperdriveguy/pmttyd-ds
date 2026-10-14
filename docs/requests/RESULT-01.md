# RESULT-01 — Tutorial battle result publication, victory and save-unblock

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the first demo milestone ([milestones](../MILESTONES.md)); full
game playability remains the end goal. The documentation slice is explicitly
requested; no independent research-agent ownership or completion claim is made.

## Open todos

- [ ] The original battle-end result record producer: which owner writes the
  result, in what order relative to unit death/end, and which values mean win,
  loss, flee and timeout.
- [ ] Victory/defeat determination and the story-advance write (the value or flag
  that admits the post-battle field scene).
- [ ] Reward publication (EXP/coins/items/drops) for the tutorial encounter, if
  any, and the field-return handoff.
- [ ] The exact condition that unblocks saving after the encounter, and the
  original save-point existence/placement at this stage.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13), first
demo milestone: the native battle result is a development token.
`Game::returnFromBattle` in `source/game.cpp` writes
`fieldBattleWork.result = harnessResult` with the comment "Development outcome
tokens only; original outcome producers are unrecovered"; victory is synthesized
from the harness HP check (`if (enemyHP==0) scene=Scene::Victory`); and
[the opening playtest chain](../behavior/story/NATIVE_OPENING_PLAYTEST_CHAIN.md)
records that "result0 is not victory, story5 is not synthesized and saving remains
blocked during this unfinished battle". An older deliverable already bound the
field-side wait condition (`include/field_battle.hpp`: "evt_npc_wait_battle_end
(8004eed8): sequence2 AND any nonzero result"), but the producing side — what
writes that result, how victory is decided, what story advance it publishes and
when saving is unblocked — is unrecovered. Please recover the original
battle-result publication, victory/defeat determination, story advance,
reward/field-return and save-unblock contract for the tutorial encounter. No lock
reserved; the cross-scene persistence contract is [DEMO-01](DEMO-01.md) and the
flag/save-block map is [GSF-01](GSF-01.md).

## Answers and delivery notes (append-only history)

No delivery yet.
