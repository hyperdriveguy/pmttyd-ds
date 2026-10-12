# Complete-game behavior documentation work register

**Version scope:** This directory documents the GameCube US binary. Current
Switch 1.0.0 research lives in [the separate Switch collection](../switch/README.md).
Do not import these contracts as established Switch behavior.

Objective: thoroughly document behavior not established by available
decompilation, then produce a system requirements document for the **complete
game**, assuming original models, textures, messages, sound and other data are
reused. No requirement is satisfied merely by listing a subsystem or its symbols.
This collection is in progress as of 2026-10-12.

## Detailed documents and evidence boundaries

| Document | Concrete coverage | Still incomplete |
| --- | --- | --- |
| [Original baseline](../ORIGINAL_BEHAVIOR.md) | Disc/DOL identity, evidence rules, subsystem order, basic waits/callback continuations, initial complete-game scope | Most systems beyond the initial recovered contracts |
| [Event runtime](EVENT_RUNTIME.md) | Priority/tie order, child/brother relationships, deletion/copy-back, selected stop behavior | Every opcode/variable type, service callbacks, restart/pause lifecycle |
| [Battle input and defense](BATTLE_INPUT_AND_DEFENCE.md) | Original history buffers, virtual directions/repeat, complete local guard decision routine and tables | Caller gates, impact sampling, attacks, full damage/status/turn rules |
| [Hit precheck](BATTLE_HIT_PRECHECK.md) | Ordered eligibility, per-copy evasion draws, accuracy reductions and return codes | Caller reactions, status lifetimes, stage-effect producer |
| [Damage calculation](BATTLE_DAMAGE_CALCULATION.md) | Ordered HP/FP callback gates, badge/status/defense arithmetic, result codes, absorption and revival marker | Weapon callbacks, death/revival execution and turn lifecycle |
| [Damage orchestration](BATTLE_DAMAGE_ORCHESTRATION.md) | Complete local request order, superguard variants, event/result selection, retained RNG draws and two gating/selection helpers | Global flag producers, audience/effects, weapon bindings and battle turn callers |
| [Counters](BATTLE_COUNTERS.md) | Complete local selection order, work record, jump-table returns, consumer accumulation/status/direct-damage calls | Other callers, counter event bodies, effect internals and weapon bindings |
| [Damage application](BATTLE_DAMAGE_APPLICATION.md) | Ordered effects/audio, HP/FP mutation, shared FP owner, Pity Flower/Sleep draws, bookkeeping and lethal event-code selection | Helper internals, recovery scheduling and death/revival event execution |
| [Battle event dispatch](BATTLE_EVENT_DISPATCH.md) | Hit cancellation/replacement, data-key gate, wait/phase scheduling, code getters and raw/resolved operands | Installed event graphs, death/revival bodies, controller lifetime and remaining services |
| [Action controller](BATTLE_ACTION_CONTROLLER.md) | All15 local states, command/script routing, retained draws, count/cleanup order and post-event handoff | Command/event/service bodies, flag writers, outer cadence and content |
| [Move controller](BATTLE_MOVE_CONTROLLER.md) | All15 local states, immediate/fallthrough transitions, phase event ordering, actor reset/handoff and announcement drain | Action controller, services/scripts, external channel writers and outer cadence |
| [Phase tables](BATTLE_PHASE_TABLES.md) | Actor/event table membership and sorts, phase wrapper, selected actor/count-consumption regions and phase-end predicate | Full move/action controllers, flag/state writers and outer cadence |
| [Move counts](BATTLE_MOVE_COUNTS.md) | Fast/Slow reset arithmetic, max-count setter, action/guard predicates and turn-first reset caller | Count consumption, outer cadence, field consumers and actor scheduling |
| [Removal and reservations](INVENTORY_REMOVAL_AND_RESERVATIONS.md) | ID/index removal, ordered parallel compaction, reserve insertion/refund and deferred consumption | Item table bindings, all lifecycle callers and badge array consumers |
| [Waits and conclusion](BATTLE_WAITS_AND_CONCLUSION.md) | Unit/battle barriers, queued reactions, reservation consumption and three-group conclusion predicate | Queue/configuration producers, pouch removal, conclusion events/rewards and outer cadence |
| [Recovery events](BATTLE_RECOVERY_EVENTS.md) | Status fallback/override, Happy pose gates and waits, identity/data/pose wrappers | Motion, float coercion, pose/sound data and key8 bindings |
| [Regeneration](BATTLE_REGENERATION.md) | Status HP/FP additions, shared FP ownership, Happy per-copy trials, presentation and wait differences | Happy event bodies, display fields, effects and outer cadence |
| [Periodic damage](BATTLE_PERIODIC_DAMAGE.md) | Poison requests, Burn registration/polling/delayed damage and shadow guard | HP/FP regeneration, numeric sine/effect internals and outer cadence |
| [Turn recovery](BATTLE_TURN_RECOVERY.md) | Ordered status phases, slot scans, event registration/waits, Freeze expiry and direct caller inventory | Periodic effects, outer controller cadence, event bodies and inner wait helper |
| [Status storage and recovery](BATTLE_STATUS_STORAGE.md) | All local getter/setter cases, shared signed pairs, presence/badge override, explicit recovery steps | Infliction/immunity, recovery caller timing, periodic effects and lifecycle |
| [Status presentation](BATTLE_STATUS_PRESENTATION.md) | Complete application tail: effect selection/payloads, positioning, announcements, sound and return semantics | Effect internals, announcement queues and original rendering comparisons |
| [Status infliction](BATTLE_STATUS_INFLICTION.md) | Weapon fields/stacking, ordered wrapper, immunity gates, susceptibility/RNG and mutation prefix | Weapon bindings, effect services and lifecycle |
| [State and inventory](STATE_AND_INVENTORY.md) | Saved-work sizes/accessors, resource setters, capacity and generic item insertion | Full progression, equipment, special items and original save records |
| [Randomness](RANDOMNESS.md) | rand recurrence and irand draw consumption | Seeding, randf, consumer call order and persistence |
| [Mechanics ledger](../MECHANICS.md) | Existing US ordinary field movement/collision/recovery/road evidence | Full physics, partners, dynamic events, matched original gameplay |
| [Asset provenance](../ASSETS.md) | Original sources, conversion choices and reuse boundaries | Complete animation, sound-event and material behavior |

The final system requirements document is **not yet ready to be derived**:
many original behaviors below remain unverified. Its eventual acceptance
requirements must reference recovered contracts and test evidence, rather than
presenting this research backlog as if it were a complete game specification.

## Public decompilation audit

Pinned source: [doldecomp/ttyd](https://github.com/doldecomp/ttyd/tree/62131fc31866f85a12e4817a8dc37f0c25eb5f8c),
revision `62131fc31866f85a12e4817a8dc37f0c25eb5f8c`, declared Japanese G8MJ01
revision 0 support. Source existence, build matching and US behavior coverage
are three separate properties.

`tools/decomp_coverage.py` parses `configure.py` with Python AST, without
executing upstream code. It selects explicit `progress_category="game"`
libraries and the configured Rel helper calls; SDK/middleware helpers are
excluded. It records each object declaration, path, source presence/size/hash
and config line. This is an object census, not a percentage of decompiled
functions, instructions, bytes or game behavior.

Results for the pinned snapshot: **427 game-category object declarations**,
421 NonMatching and 6 Matching. **134 have source files; 293 do not**. A present
file may be partial or a stub. The six Matching declarations were not rebuilt
or independently verified against the Japanese game, and say nothing by
themselves about US equivalence.

| Config library/group | Declared objects | Source files present |
| --- | --- | --- |
| battle | 40 | 30 |
| battle_ac | 21 | 2 |
| battle_sac | 8 | 1 |
| data | 4 | 3 |
| driver | 24 | 24 |
| effect / effect_n64 | 76 / 103 | 6 / 2 |
| event | 45 | 15 |
| manager | 8 | 8 |
| mario / motion / party | 10 / 13 / 11 | 6 / 2 / 3 |
| sequence | 10 | 7 |
| unit / unit_party | 6 / 7 | 0 / 1 |
| window | 7 | 1 |
| library named sdk, categorized game by upstream | 4 | 3 |
| MarioSt | 25 | 15 |
| Explicit REL objects: dig / mri / pik / tou2 | 2 / 1 / 1 / 1 | 2 / 1 / 1 / 1 |

The five configured REL source objects are not a complete census of the 29
US ISO modules. The Japanese config even names a module not present in the
US REL inventory (`dig`); do not assign US content by matching names alone.
Cross-region REL coverage and shipped event streams need their own census.

Concrete reviewed incompleteness: the public defense checker is a constant
return stub; `BattleCheckDamage` contains only an initial fragment; most
action-command objects have no source. Conversely, substantial source exists
for the event manager and saved-work accessors despite NonMatching labels.
Research priority should be informed by those distinctions, not by treating
all 421 declarations as equally unknown.

Reproduction (snapshot and reports stay ignored):

```sh
python3 tools/decomp_coverage.py --snapshot artifacts/reference/decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c --output artifacts/reference/behavior/decomp-coverage.json
python3 tools/behavior_evidence.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evidence.json
python3 tools/verify_behavior_contracts.py
```

## Full-scope completion register

All rows remain open. "Partial" refers to the concrete documents above, not
completion. Reused data removes the need to author assets, but their parsers,
runtime semantics, resource lifetime and event bindings still require recovery.

| System | Current state | Evidence needed to close documentation |
| --- | --- | --- |
| Boot/platform/main loop | Partial order only | Retrace/input timing, initialization/shutdown, timebases, errors and region behavior |
| Event VM and services | Partial | All shipped opcodes, tagged operands, scheduling/lifecycle and every reachable service's effects |
| Story and quests | Not recovered | All original event graphs, prerequisites, branches, message/speaker/control-token bindings and persistent writes |
| Field movement/forms | Partial | Every motion/form, slopes/walls/ceilings, hazards, collision flags, interactions and moving geometry |
| Partners/NPCs | Not recovered | Following/path/AI rules, field abilities, scripts, recruitment/upgrades and partner-specific collision |
| Maps/transitions | Partial | All entrance types, dynamic objects, loading/REL lifetime, story gates, interior and scripted travel |
| Combat/turn engine | Partial defense, precheck, HP/FP calculators, counter selection and direct application | Unit/part/weapon lifecycle, target/turn ordering, callbacks, status application, hit-event execution, loss/escape/win |
| Action commands | Partial input/defense | Every attack/special/stylish command's timing, difficulty, input state, scoring and effects |
| Audience/stage/rewards | Not recovered | Audience AI, hazards, bingo/SP, XP/drop/progression formulas and conditions |
| Inventory/equipment/economy | Partial | All acquire/remove/sort/use/equip paths, shops/crafting/storage, mail and collectible effects |
| Save/load/progression | Partial in-memory state | Record format/validation/failure flows, complete state lifetimes and load reconstruction |
| RNG | Partial | All generators/seeds, persistence and consumption sites/order |
| UI/messages/pause | Not recovered | Token interpreter, windows/options, input repeat/focus, scene pause/advance and accessibility adaptations |
| Camera/graphics/animation | Format/field evidence only | Every track/type, scene/layer/cull/lighting behavior, paper transformations, camera modes, effects |
| Audio/music | Formats/streams only | Game event→macro→voice behavior, track transitions, mix/loops/envelopes/synchronization |
| Chapters/interludes/minigames | Not recovered | Reachable content inventory, per-mode mechanics, optional content and ending/postgame conditions |

## Gate for the eventual system requirements document

For each system, documentation must identify inputs, state and ownership, ordered
updates, outputs, numerical/data rules, exceptional paths, persistence and
cross-system dependencies. Each nontrivial claim needs source revision plus
binary address/data reference or a reproducible original observation. Unknown
branches remain explicit and cannot be hidden by selecting the prototype's rules.

Then derive requirements with stable IDs, traceability to these contracts,
dependencies, reused-data interfaces, numerical/timing constraints, platform
service boundaries and acceptance cases. Include complete playable content,
original story fidelity, asset conversion/runtime limits, save robustness,
input/audio/render integration and New 2DS XL performance verification.

Completion requires covering every shipped reachable system and content mode,
not merely booting the early Rogueport slice or passing its host tests. The
current output satisfies neither that full documentation gate nor the final
requirements gate. Continue the recovery work before marking the goal achieved.
