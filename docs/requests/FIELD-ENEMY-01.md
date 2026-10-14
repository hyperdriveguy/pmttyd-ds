# FIELD-ENEMY-01 — Original roaming field enemies and contact battle entry

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, on the full-game path ([milestones](../MILESTONES.md)); the demo's
harbor encounter is scripted, so D1 does not depend on this, but D1 must not bake
in a shortcut that contradicts it. The documentation slice is explicitly
requested; no independent research-agent ownership or completion claim is made.

## Open todos

- [ ] The original field enemy actor kind and registration — how a roaming enemy
  is placed per room (the record fields beyond model/position/scale), how it
  differs from an ordinary NPC and a battle-carrying scripted NPC, and its
  lifetime (spawn/despawn/respawn) per area and per story state.
- [ ] The field enemy AI: idle/patrol/wander versus notice, the notice/aggro
  condition (radius, angle, elevation, line of sight, Mario state), the approach
  and retreat behavior, movement speeds, and any timers or cooldowns.
- [ ] The contact-with-Mario detection and exactly what starts the battle — the
  contact volume, which side initiates, what stops when the battle starts, and
  how Mario's first strike (ordinary Jump or Hammer from the field) changes the
  entry so it reaches the battle-entry protocol
  ([field battle entry](../behavior/field/FIELD_BATTLE_ENTRY_AND_RETURN.md))
  with the right side advantage.
- [ ] The field danger/entry presentation (the battle-entry cue, music switch and
  field effects) and the field return/respawn behavior after the battle.
- [ ] Whether the enemy's battle group/stage selection reuses the NPC
  battle-info path or a distinct enemy-spawn configuration.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
full-game path: the native port has no roaming field enemies. The field actor
record carries only `uint32_t key,room,model,interiorRequired,interiorForbidden,
storyGate,kind; // 0 NPC, 1 map object` (`include/field_actors.hpp`) with a
wander domain (`source/npc_wander.cpp`) for ordinary NPCs, and the only field
battle the port drives is the scripted harbor Crump encounter
(`source/harbor_encounter.cpp`). The original game's overworld is populated by
roaming enemies that notice, approach and chase Mario and start a battle on
contact (with an advantage when Mario strikes first), which the port does not
model. The existing
[field battle entry/return contract](../behavior/field/FIELD_BATTLE_ENTRY_AND_RETURN.md)
recovers the scripted NPC→battle setup/start/return protocol
(`evt_npc_set_battle_info`, `npcSetBattleInfo`, `evt_npc_battle_start`,
`evt_npc_wait_battle_end`) and explicitly leaves "NPC association and cleanup",
"actual result producers", and "tutorial unit scripts" open, but it is not the
field-enemy AI. Please recover the original roaming field enemy as one contract —
its kind/registration and per-area lifetime, its field AI (notice/aggro,
approach/retreat, speeds, timers), the Mario-contact and first-strike trigger that
hands off to the battle-entry protocol, the field danger/entry presentation, and
the post-battle return/respawn — so later areas spawn original enemies instead of
native fixtures, and the demo does not hard-code a scripted-only entry. The
battle-side enemy selection and attack-event choice stay in the battle contracts,
Mario's interaction/hammer side stays with [OW-INTERACT-01](OW-INTERACT-01.md),
and the scripted NPC battle protocol stays where it is. No lock reserved.

## Answers and delivery notes (append-only history)

No delivery yet.
