# Switch 1.0.0 mechanics and behavior reference

Research baseline changed to the supplied **Nintendo Switch remake**, per the
2026-10-12 user request. **Static analysis only**, per the subsequent instruction.
This collection is incomplete. Existing [GameCube contracts](../behavior/README.md)
remain useful comparisons, but their addresses, constants, event VM and dialogue
must not be presented as verified Switch behavior.

## Read this collection

- [Binary identity and reproduction](BINARY_BASELINE.md): exact package/build,
  content verification, tool provenance and local evidence paths.
- [Combat parameters and defense selection](BATTLE_PARAMETERS.md): recovered
  data layout, seven defense groups, native selection and timing caveats.
- [Field parameters](FIELD_PARAMETERS.md): player/form and jump records with
  exact offsets; distinguishes stored numbers from untraced update equations.
- [Published behavior and version boundaries](VERSION_DIFFERENCES.md): Nintendo's
  feature descriptions and patch-specific changes, separate from binary proof.

## Evidence rules

**Binary data** establishes that a value or record exists at a specified offset.
**Native static contract** establishes control/data flow for a reviewed code
range, with module-relative addresses and build ID. **Published description**
establishes the publisher's feature statement, not the exact base-build algorithm.
**Hypothesis** identifies work to do; it is never an implementation requirement.
No gameplay timing, performance, or complete story behavior has been measured here.

Use ordered arrays rather than dictionaries keyed only by names: the battle
parameter file includes null IDs and repeated names. Retain relocation targets,
raw bit patterns and unknown fields. A script filename/string is not a recovered
script, and a field called FRAME is not sufficient to assign seconds. Follow the
clock producer, consumer and pause/speed branches first.

NSO addresses are relative to the loaded module, not absolute emulator addresses
and not GameCube RAM addresses. File offsets in a decompressed data ELF are also
not offsets in the NSP or compressed file. Reports keep these coordinate systems
separate. Never change the reference package or extracted reference data to make
an experiment easier; use separate derivative artifacts.

## Coverage and next work

| System | Current evidence | Required static work |
| --- | --- | --- |
| Version/platform | CNMT v0, NACP 1.0.0, main build ID, verified archive contents | Boot/update ordering, clocks and service boundaries |
| Combat input/defense | Original rows and native seven-group selection; timer decrement sites | Input production, eligibility, impact checker, reset/cooldown, timebase |
| Damage/status/turns | Named parameter values only | Full consumers, modifier order, callbacks, state lifetime, RNG |
| Field/player/forms | 18 player rows and 30 jump rows | Field meanings, integrator/order, collision, form transitions |
| Event/script engine | 1,246 files under script/; executable API declaration strings | Bytecode decoder and registration mapping; every reachable service |
| Story/messages | 47 US_English_Final files identified | MSBT labels/tokens, speakers, script bindings, branches and persistent conditions |
| Partners/NPC/AI | File/parameter inventory | All partner abilities and enemy state machines |
| Maps/travel | Data inventory; published quick-travel changes | Entrance/exit triggers, gates, return positions, loading lifetime |
| Audience/stage/special moves | Named battle parameter records | Selection, RNG consumption, counters, award formulas and timing |
| Inventory/economy/progression | Not recovered | Capacity, clamps, badges, shops, crafting, rewards and save writes |
| Save/load/UI/audio/graphics | Files present; no complete runtime contracts | Layouts, lifecycle, failures, token execution, playback and presentation |
| Chapters/interludes/optional content | File census only | Reachability, full content graph, minigames, bosses, postgame |

Priority: finish defense timer/input consumers and determine their clock; then
recover damage/status logic and field integrators. Decode scripts and messages
without substituting the GameCube event decoder or English text. A complete
behavior specification requires all rows above, not just parameter inventories.

## Local artifact policy

Original NSP, keys and firmware remain outside source history. Extracts are in
ignored `local-assets/switch/`; tools, reports and disassembly in ignored
`artifacts/reference/switch/`. Commit research prose, parsers and tests only.
See [asset provenance](../ASSETS.md). No original dialogue or full code listing
is included in these documentation files.
