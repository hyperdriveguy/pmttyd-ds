# Battle unit pointer getter

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes the open `BattleGetUnitPtr` callee flagged across the unit-query,
table-builder, and event-dispatch leaves.

## Anchors

- `us-usage.json`: `BattleGetUnitPtr`, battle.o, 60 bytes, 487 direct link
  sites — the most-called unresolved helper in those leaves.
- Pinned JP snapshot `src/battle/battle.c:333` defines it (JP/US caveat, but
  the body is five lines with no version-sensitive logic):

```c
BattleUnit* BattleGetUnitPtr(BattleWork* wp, s32 index) {
    if (index == -1) {
        return NULL;
    }
    if (index < -1) {
        return NULL;
    }
    if (index < 64) {
        return wp->units[index];
    }
    return NULL;
}
```

- No reference `.elf` exists, so no instruction-level verification was
  possible; every behavioral claim below is the snapshot definition plus
  US-side corroboration.

## Contract

- Returns NULL for index −1, for anything below −1, and for 64+.
- Otherwise returns word `units[index]` of the battle work — no null check
  on `wp`, no activity/kind validation of the slot (slots may hold NULL or
  stale pointers; that is the caller's problem).
- The explicit −1-first shape explains the leaves' `-1` early-outs
  (`btlevtcmd_GetHp/GetMaxHp` return 2 without writing when `BattleTransID`
  yields −1): calling through would only produce NULL. Leaves without the
  gate (`GetUnitWork`, `SetUnitWork`, `CheckStatus`, `CheckDataOfDataTable`)
  pass through unconditionally — reported, not repaired.
- Asymmetric twin: `BattleSetUnitPtr(B,I,U)` ([contracted](../battle/BATTLE_UNIT_REGISTRY_RESET_AND_INIT_EVENT_SCAN.md))
  guards only `I < 64`, admitting wrapped negative indices — exactly the
  setter behavior pinned in [actor
  translation](BATTLE_ACTOR_SELECTOR_TRANSLATION.md). Getter and setter
  deliberately differ; do not unify them.

## Not established

Slot lifetimes/owners, `units[]` base offset, call cadence beyond the
487-site count, and any gameplay effect.
