# Original-game behavior reference

Research baseline: 2026-10-12. Target: US GameCube `G8ME01`, disc revision 0.
Purpose: specify externally visible behavior and the state transitions needed
for a native port or strongly equivalent reimplementation. This is a partial
specification, not a claim that the whole game has been reverse engineered.
The current native slice is not an oracle for the original.

Continue with the [detailed system documents and coverage register](behavior/README.md).
They extend this baseline with US defense/input behavior, event ownership and
priority semantics, saved work/inventory mutations and RNG arithmetic.

## Evidence and scope

Use these evidence classes in subsequent additions:

| Class | Meaning | What it does not prove |
| --- | --- | --- |
| B | Inspected bytes/data/control flow in the identified US binary | That a branch occurs in the intended gameplay context |
| O | Recorded original-game execution with inputs and state | Unobserved branches or other revisions |
| R | Public reconstruction, format research or symbols | An exact match to this US binary |
| P | Proposed port behavior or validation criterion | Original behavior |

This document's new scheduler findings are B, assisted by R. Earlier B findings
are indexed below; their detailed addresses remain in [MECHANICS.md](MECHANICS.md).
There are still no original Dolphin gameplay measurements in the project ledger.
Native host tests and Azahar captures establish native behavior only.

Accessible private evidence used here is the user's local ISO, existing local
extracts and project research artifacts. No private repository, private Nintendo
documentation, authenticated community archive or Switch executable was available
or consulted. No inference is presented as having come from such a source.

## Binary identity and reproducibility

`tools/behavior_evidence.py` opens the ISO as `rb`, checks its FST, translates
virtual addresses through the DOL section table, and compares those translations
with the supplied symbol file offsets. Its output contains metadata/hashes and
direct branch-and-link sites, not original function bytes or dialogue text.

| Input/property | Observed value |
| --- | --- |
| ISO | `/home/carson/games/roms/G8ME01.iso` |
| Disc ID / number / revision | `G8ME01` / 0 / 0 |
| Image size | 1,459,978,240 bytes |
| FST regular files | 3,204 |
| REL modules | 29 |
| `msg/US/` files | 260 |
| DOL offset in ISO | 131,840 bytes |
| DOL span hashed | 4,020,864 bytes, header through last initialized section |
| DOL SHA-256 | `93c79aa312ca57c2785d0854a516c554e7683b525f8c3456dad568f4c440451a` |
| `rel/gor.rel` size | 642,564 bytes |
| `rel/gor.rel` SHA-256 | `d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a` |
| Supplied US symbol CSV SHA-256 | `6c3899e26338e098e50e688bb1a2afb9488180f6551f256418179a036f2fe1bd` |

The DOL hash is not a whole-disc hash. A disc ID alone cannot establish that a
dump is unmodified. Fingerprints identify the evidence used, not ownership or an
independent retail-dump certification. BSS is described by the DOL header but has
no initialized file bytes to hash. External symbol names remain annotations.

Reproduce locally:

```sh
python3 tools/behavior_evidence.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evidence.json
python3 tools/inspect_dol_function.py evtmgrCmd --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evtmgrCmd.elf
python3 tools/inspect_dol_function.py evtmgrMain --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evtmgrMain.elf
python3 tools/inspect_dol_function.py marioStMain --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/marioStMain.elf
python3 tools/verify_actor_collision.py
```

The report now fingerprints 359 functions, inventories all 119 dispatcher targets,
hashes `gor.rel` and all 260 US message files, and lists all REL paths. Direct
call-site lists are static instruction scans: indirect calls, conditional calls,
execution counts and branch feasibility are not reconstructed. Fingerprinting a
function does not mean every branch in it has been documented.

For correct GameCube paired-single mnemonics and existing listing repair,see
[Gekko disassembly tooling](GEKKO_DISASSEMBLY.md). The inspection command above
applies that correction automatically.

## Public reference register

1. [doldecomp/ttyd](https://github.com/doldecomp/ttyd): MIT; consulted revision
   `62131fc31866f85a12e4817a8dc37f0c25eb5f8c`. Its README declares support for
   **G8MJ01 revision 0 (Japan)**. Source comments claiming matching status do not
   prove US equivalence. Inspected
   [event manager](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/manager/evtmgr.c),
   [command interpreter](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/manager/evtmgr_cmd.c),
   [sequence driver](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/driver/seqdrv.c),
   and `src/mario/mariost.c`, `include/manager/evtmgr.h`.
2. [jdaster64/ttyd-utils](https://github.com/jdaster64/ttyd-utils): GPL-3.0;
   consulted US symbol CSV with the hash above. Provides region-specific symbols,
   event extraction tools and structure notes. Names aid discovery; original
   bytes decide semantics. No symbol table or upstream code is added to runtime.
3. [jdaster64/ttyd-tot](https://github.com/jdaster64/ttyd-tot): existing project
   research consulted `ttyd-tools/rel/include/ttyd/evt_bero.h` and `evt_cmd.h`.
   A mod is useful for ABI research but modified combat/story behavior is not
   evidence of retail behavior. Check individual licenses before code reuse.
4. [noclip.website TTYD](https://github.com/magcius/noclip.website/tree/master/src/PaperMarioTTYD):
   prior format research for map/animation/texture decoding, MIT. Rendering a room
   in a viewer does not establish game event visibility or collision behavior.
5. [vgmstream](https://github.com/vgmstream/vgmstream) and
   [musyx-extract](https://github.com/Nisto/musyx-extract): prior audio-format
   research; licensing/reuse boundaries are recorded in [ASSETS.md](ASSETS.md).

Downloaded source, LICENSE and revision marker for item 1 remain in ignored
`artifacts/reference/behavior/`. Only independent tooling and explanatory notes
are committed. Original game assets retain their original rights; a research
tool's license is not a license for extracted assets.

## Core execution contract

### Sequence changes and subsystem order (B)

US `seqMain` at `0x8002e228`, size `0xdc`, compares requested/current sequence.
When they differ, an existing sequence first waits for draw synchronization
(`0x8002e24c`) and invokes its exit callback (`0x8002e270`). It records the old
sequence, clears 32 bytes of sequence work, installs requested sequence and
map/entrance parameters, and invokes initialization (`0x8002e2cc`). It then
invokes the current main callback (`0x8002e2f0`), including on this transition
call. Callback slots are init/main/exit at offsets 0/4/8, table `0x80304400`.
P: releasing room GPU resources must respect synchronization and initialization
must precede that sequence's first main update. Do not defer every new sequence
by an extra tick without evidence.

US `marioStMain` at `0x8006f904` has the following consecutive direct calls at
`0x8006f92c` through `0x8006f9a4` (names from the US symbol map):

```text
seqMain → cardMain → bgMain → shadowMain → animMain → battleMain
→ camMain → windowMain → mapMain → hitMain → evtmgrMain → mapDisp
→ marioMain → mobjMain → npcMain → fadeMain → imgMain → lightMain
→ offscreenMain → itemMain → caseMain → iconMain → extMain
→ psndMain → winMain → statusWinMain → envMain → countDownMain
→ nameEntMain → winMgrMain → effMain
```

`makeKey` is called at `0x8006f928`. This is ordering evidence inside this
function, not proof of the outer retrace loop or each subsystem's internal
update/render policy. In particular, the name `mapDisp` does not prove that the
GPU completes rendering at that point. P: event changes may affect Mario/NPC
work later in the same pass; a replacement loop must preserve observable
dependencies rather than freely reorder all systems.

### Event contexts and scheduling (B)

`evtGetWork` (`0x80037a60`) reads `gp+0x14`: zero selects work at `0x803e8d30`,
nonzero selects the next work block, stride `0xa0`. R names this field
`inBattle`. US `evtmgrMain` independently uses the same selection. Field and
battle event state must not be flattened into one undifferentiated context.

`evtmgrMain` (`0x800368d0`, size `0x1ec`):

- Computes elapsed time from `gp+0x38/+0x3c` and work's prior timestamp. Negative
  elapsed time clamps to zero (`0x80036938`–`0x80036954`).
- If integer elapsed milliseconds exceed 500, substitutes OS ticks representing
  **integer `1000 / gp->framerate` milliseconds** (`0x8003698c`–`0x800369c0`).
  At 60 this is 16 ms, not an exact 1/60 second. The comparison occurs after
  millisecond conversion; do not silently replace it with a raw 500-ms comparison.
- Stores the new timestamp and calls priority-table construction at
  `0x800369cc`. The verified exchange-sort and nonstable tie behavior are now
  detailed in [EVENT_RUNTIME.md](behavior/event/EVENT_RUNTIME.md).
- Locates entries using stride `0x1b0`, requires active bit 1 and a matching
  recorded event ID, and skips flags masked by `0x92` (`0x800369e0`–`0x80036a10`).
  The ID comparison prevents a recycled slot automatically inheriting the old
  scheduled identity. Full creation/deletion scheduling is not yet specified.
- For eligible events, increments lifetime, adds floating-point timescale to
  an execution-credit accumulator, and invokes `evtmgrCmd` while credit is at
  least 1, subtracting 1 per invocation (`0x80036a14`–`0x80036a74`). A credit is
  an interpreter invocation, **not one bytecode instruction**.
- Interpreter return -1 ends this event's processing; return 1 stops the current
  manager pass (`0x80036a60`–`0x80036a7c`). Stopped/ineligible entries do not get
  the lifetime/credit increment on this path.

P: retain separate simulation tick, event lifetime, and presentation clocks.
Use fixed simulation ticks for action-command inputs. A fixed-tick port still
needs an explicit policy for the source's elapsed-time event waits; replacing
all waits with `ceil(ms*60/1000)` is an adaptation until compared at boundaries.

### Event encoding, continuations and waits (B)

US `evtmgrCmd` at `0x800316b8`, size `0x13b8`, dispatches using a 119-entry
table at `0x80304fd0`. Command words are big-endian 32-bit words with argument
count in the high half and opcode in the low half. Fetch (`0x80031704`–
`0x80031740`) stores opcode and count in byte fields, advances to the next
command by `4 + 4*count`, and clears the blocked marker. Valid known opcodes
fit those fields; malformed bytecode should not be treated as valid game data.

The handler result and interpreter return are **different layers**:

| Handler result | US dispatcher effect at `0x800329f4`–`0x80032a48` |
| --- | --- |
| 0 | Leave current opcode in place; return 0 (blocked) |
| 1 | Set opcode to fetch-next; return 0 (yield) |
| 2 | Set opcode to fetch-next and continue in the same invocation |
| 3 | Redispatch immediately |
| 255 | Return -1 to event manager |
| Negative | Return 1 to event manager, stopping this manager pass |

Opcode 2 invokes event deletion then supplies result 255. Opcode 1 reaches the
default status path; do not equate the script-data end marker with the explicit
event-deletion opcode. The latter distinction matters when validating streams.

Opcode 9, WAIT_FRAME, enters `0x800318e8`. On first evaluation it resolves the
argument through `evtGetValue`, stores a counter and marks itself initialized.
Zero completes immediately. Otherwise it decrements the counter; nonzero blocks,
zero yields. P: with timescale 1, `WAIT_FRAME(1)` yields on its first invocation
and the next command is reached on the next eligible invocation. Negative inputs
are not specified here as a supported timing API.

Opcode 10 calls `evt_wait_msec` at `0x80035884`. First evaluation records the
resolved duration and current event lifetime. Zero completes immediately;
otherwise elapsed time is converted to integer milliseconds and compared to the
requested duration, yielding on completion and blocking beforehand. The US code
uses a signed low-word difference for that conversion (`0x80035908`–
`0x80035924`), despite storing both lifetime words. Long-duration/wrap behavior
needs dedicated analysis; do not assume arbitrary 64-bit timer semantics.

Opcode 91, USER_FUNC (`0x800326fc`): resolve and cache the callback on first
evaluation, remove that first argument from the exposed argument span, set the
blocked marker, and invoke `(event, 1)`. Resumption invokes the cached function
as `(event, 0)` without reinitializing. Propagate its result using the table
above. P: portable wrappers need persistent per-command state and a first-call
indicator; restarting a movement/message callback each tick would be incorrect.

`evtChildEntry` (`0x80036eec`) sets the parent's wait bit `0x10` at
`0x80036f78`, which is included in the scheduler's skip mask. US local-variable
inheritance, copy-back on deletion, brother ownership, priority sorting and
selected stop semantics are now documented in
[EVENT_RUNTIME.md](behavior/event/EVENT_RUNTIME.md). Restart, complete pause ownership
and the remaining opcode/service contracts are still open. Do not implement
every child as detached fire-and-forget work.

## Existing recovered field behavior (B; no new O claims)

These summarize the detailed evidence and limitations in [MECHANICS.md](MECHANICS.md).
Later sections of that chronological ledger supersede earlier implementation
status notes; an old paragraph saying ceilings are missing is historical.

| Area | Recovered original rule | Remaining gap |
| --- | --- | --- |
| Coordinates | `_mapLoad` applies model scale 10. Native model scale .1 implies original engine literals × .01 | Preserve distinct coordinate spaces for dynamic transforms |
| Ordinary movement | Walk/dash rates 1/2.25 original units per tick; stick cap 70, dash threshold 55 | State hysteresis, drift, transformations, follower physics |
| Jump | Ascent tuple `(8,-.84,.08,-.02)`, fall `(-.1,-.2,.024,-.004)`; jerk→acceleration→velocity order; release adjustment -.526 | Special states, gravity variants and observed input timing |
| Jump startup | Launch phase0/counter1; next call phase10/counter2 integrates; release reduction from counter4; [verified phase gate](behavior/overworld/OVERWORLD_MARIO_MOVEMENT.md) supersedes earlier counter3 interpretation | Compare matched Dolphin inputs |
| Body/walls | Width 20, height 37; ordinary wall clearance 10; rays at 11 and about 26.64 | Push-out, sliding, dynamic walls |
| Floor queries | Five probes, footprint radius 7.5; velocity-dependent ray bounds | Full slope/contact acceptance, walking beneath ceilings |
| Head queries | Separate attribute filter and cull rule; five probes; original rounding and clearance | Special forms, block effects and dynamic geometry |
| Damage return | Latch eligible floor with attribute `0x40000000`; keep latch across other contacts | Full dynamic return-anchor lifetime |
| Water | Recovery state machine includes source sink/rise/travel/fall; landing counter 50; HP -1; surviving injury counter 12 | Exact effects, SFX, camera and all branch conditions |
| Void | Inclusive Y ≤ -2000 triggers bottomless policy; ordinary motion 34 | Reload/background variants and fallback selection |
| Road entry | Default length 30; start/end ±30 around marker; travel rate 150/s | Door/pipe/story-dependent entry and exact companion path |
| Road exit | Wait for landing, scripted movement, then 150-ms wait; default black fades 300 ms | Trigger semantics and observed boundary timing |
| Animation | Map loops use authored full duration; event-selected animation/visibility | Complete actor, material, vertex and event-controlled behavior |

Counter assignments can decrement on the assignment call: 50/12 do not mean
50/12 *additional* idle updates. Test this distinction explicitly. Published
native tap/held landing counts in the ledger are predictions from source-derived
integration, not original gameplay measurements.

## Story and content contract

The binary separates executable logic, relocatable area data and message assets.
The local census contains 29 REL files and 260 US message files. Preserve module
identity, section identity, relocation type and addend when resolving references;
a raw REL word is not necessarily a process address. Current
`map_event_inventory.py` resolves a limited Rogueport init subset; it is not a
general event interpreter or a proof of branch execution.

P: a scene specification must capture all of the following together:

- Original event entry/module/offset, prerequisite switches and entrance ID.
- Ordered commands, nested conditions, spawned/child events and join points.
- Message ID, original tokens and speaker supplied by the calling event.
- Choice results and cancellation, plus the consequent state mutations.
- Actor position/pose/facing, camera, controls, sounds, and completion conditions.
- Persistent writes and what occurs on re-entry, defeat, reload and interruption.

Message-file order alone cannot establish plot order or attribution. A string
reference behind a false condition is not spoken dialogue. US message bytes and
full dialogue remain local; commit identifiers, schemas, hashes and behavior
notes. Do not author replacement story text. Screen reflow and control prompts
may adapt presentation while retaining original meaning and event timing.

## Coverage and implementation gates

The following is a work inventory, not a list of recovered rules. Every row
requires concrete B/O evidence before it can become a fidelity claim.

| System | Present evidence | Required before an equivalent implementation claim |
| --- | --- | --- |
| Scheduler | US pass ordering, result protocol, basic waits | Priority/ties, child/brother lifecycle, pause classes, variable addressing, every used opcode |
| Rogueport story | Original message files and selected init streams available | Full opening/Crump control-flow graph, speaker attribution, conditions and switch writes |
| Battle | Public names/layout leads; prototype constants explicitly provisional | Command availability, unit/weapon data, target selection, turn order, damage pipeline and tutorial overrides |
| Action commands | No measured original timing windows | Per-move start/impact/input sampling, early/late/mash rules, guard/superguard and difficulty modifiers |
| Statuses/badges/items | Public research leads only | Stacking/order, immunities, duration decrement, equipment and inventory constraints |
| Rewards/progression | Prototype victory is not retail evidence | Star Points, coins/drops, level-up, partner/ability and story unlocks |
| RNG | rand/irand arithmetic and draw rules in [RANDOMNESS.md](behavior/meta/RANDOMNESS.md) | Seeding, other generators, seed persistence and consumption order per random action |
| Save/load | Native versioned checkpoint format only | Original persistent switches, inventory/party/stats, validation, slot operations and failure behavior |
| Input/pause/UI | Input construction precedes listed pass calls | Held/edge/repeat rules, lock masks, pause clocks, message/menu overlap |
| Field | Detailed ordinary static movement evidence | Grounded slopes, walls, dynamic collision, all transformations and partner abilities |
| Camera/rendering | Original map formats and some event flags | Camera zones/movies, layers/occlusion, actor animation, effects and visibility branches |
| Audio | Original music decoded; samples indexed | Sound-event→macro→sample mapping, envelopes/pitch/loops, music transitions and synchronization |
| Chapters/minigames | Module census only | Every reachable event, battle, map, minigame and persistent consequence |

### Acceptance method (P)

Define equivalence by the same initial state and timestamped logical inputs
producing the same story decisions, state changes, collision contacts, combat
results and completion conditions. Permit documented presentation adaptations
(two screens, texture budgets, physical controls). Declare tolerances for position
and audiovisual alignment; do not hide gameplay timing changes in them.

Each reference trace should record disc/DOL hash, emulator/version/configuration,
map/entrance, persistent switches, actor/party/battle state, RNG state if known,
input edges and held values by simulation tick, event PC/ID, message ID, sound
event ID, and output checkpoints. Pair recordings with boundary tests, including
zero/one-tick waits, stalls, pausing, simultaneous events, early/late inputs,
repeat entry, defeat and load. Use isolated Dolphin profiles; do not touch the
user's profile or saves. Native unit tests prove the native contract only until
compared to those traces.

## Nintendo Switch comparison policy

Switch may serve as a separate behavior/presentation reference, as requested.
No Switch-specific mechanics or timing claims were established in this pass.
Record release/update/language/platform and source for each future comparison.
Never silently transfer a remake's dialogue, frame counts, UI or combat rule to
the US GameCube baseline. If a Switch change is selected for the port, document
it as a deliberate variant with its own acceptance trace.

## Next research slices

The [US completion assessment](behavior/meta/US_COMPLETION_AND_PRIORITIES.md)
supersedes the ordering below. Start from shipped opening/Crump event bindings,
resolve actually selected attack commands and record gameplay traces;defer
unbound generic library edge cases. The US usage audit distinguishes direct
references,callback/data candidates and shipped REL imports from execution.

1. Close the event foundation: all variable namespaces, restart, full pause
   lifecycle and remaining opcode/service contracts, then a host interpreter
   verified with short isolated original traces.
2. Recover the opening harbor/Crump event graph from original REL data and its
   DOL callbacks; bind exact US message IDs and switches before enabling story.
3. Recover tutorial unit/weapon/action-command data and exceptions, then record
   matching Dolphin runs for timing and branching. Replace provisional battle
   constants only where evidence supports the replacement.
4. Extend the same procedure to persistent state, RNG, audio callbacks, camera,
   animations, partner abilities and the remaining module census. Track unresolved
   functions/branches rather than treating an available symbol as a specification.

## Follow-ups

Dated follow-up notes appended after this document (recovered-contract updates, verification counts and remaining boundaries) live in [ORIGINAL_BEHAVIOR_FOLLOWUPS.md](ORIGINAL_BEHAVIOR_FOLLOWUPS.md); append new ones there.
