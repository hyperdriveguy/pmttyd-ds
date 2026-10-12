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


Battle movement follow-up: [jump event callbacks](behavior/battle/BATTLE_JUMP_EVENT_CALLBACKS.md)
provides nine complete local bodies and the concrete encoded-float route,with
separate ten-body/55-instruction verification. These fingerprints are additional
to the general report;neither set establishes measured hit/input cadence.


Input follow-up by agent ID1: [physical producer](behavior/input/PHYSICAL_INPUT_PRODUCER.md)
and [battle cadence/snapshot map](behavior/input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md)
connect makeKey to all four general pads,the sequence controller,the private
channel0 pad,selected AC main and later event processing. US strict48 global
directions,scaled24/6 repeats,error-3 retention and the input-clear chord are
separate from inclusive30 battle directions/16/8 repeats. A dedicated ten-body/
64-anchor verifier includes prior fingerprints;counts must not be summed as ten
newly completed gameplay systems. Main/pad/physical execution traces remain open.


[Input reset ownership](behavior/input/BATTLE_INPUT_RESET_OWNERS.md) identifies the
known direct update/reset routes and a script-selected raw trigger-history
injection. [System-level/speed controls](behavior/event/EVENT_SYSTEM_LEVEL_AND_SPEED.md)
closes the local level/type-mask table,getter and per-entry speed multiplier/
creation policy. Neither converts the static census into an executed pause graph
or establishes full input writer/restart coverage.


[Crump NPC versus lecture roles](behavior/story/CHRISTINE_HELPER_ROLES.md) and
[lecture coordinator](behavior/story/LECTURE_COORDINATOR_MODES.md) now distinguish
setup indices2/3,spawned kind/ID ownership and the professor's full local phase
request graph. Exact US messages/speakers/choice branches are pinned to gor_02
for the lecture;missing player/counter/field/services and original execution
remain open. These local closures do not complete either the full lecture or game.


[Lecture player bindings](behavior/story/LECTURE_PLAYER_ATTACK_FLOW.md) now supply
1→2/5→6 cleanup and first-hit/output feedback writers,default weapon table
substitution and a proven producer of TimingA's01000000 forced-success route.
[Status-window arbitration](behavior/battle/BATTLE_STATUS_WINDOW_ARBITRATION.md)
establishes the lecture2740 suppression consumer separately from selection.
Full menu/teardown/field and original lecture execution remain incomplete.

[Lecture field entry and return](behavior/story/LECTURE_FIELD_ENTRY_AND_RETURN.md)
now supplies the wall-event and story10 map-reload callers, crossing-poll
operands, exact choice branches, setup3 launch and common story11 write. Skipping
the lesson also advances to11. Field party services, intersection semantics,
end-controller cleanup and original execution remain incomplete.


[Field intersection](behavior/field/FIELD_SEGMENT_INTERSECTION.md) now supplies the
closed-endpoint and rounded fused cross-product contract,live Mario position
fields and used encoded LV quantization. [Field party callbacks](behavior/partner/FIELD_PARTY_EVENT_CALLBACKS.md)
close raw slot dispatch,first-call previous-kind constructor requests and
conditional run-mode writes. Party engine lifetimes,full variable namespaces,
battle teardown and original execution remain incomplete.


[Field party destruction](behavior/partner/FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md)
and [position constructors](behavior/partner/FIELD_PARTY_POSITION_CONSTRUCTORS.md)
now establish linked-kind preservation versus clearing,ordered pointer removal/
release/free,first-free versus secondary-only allocation,pose/init requests and
primary/secondary coordinate tails. Resource/driver/all-kind/mode/index and
original-transition composition remain incomplete.


[Field party mode initialization](behavior/partner/FIELD_PARTY_MODE_INITIALIZATION.md)
and [driver/index ownership](behavior/partner/FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md)
now specify old-mode dispatch,local cleanup and angle-loop behavior,two-slot
init,count/reread/update order and the Mario bit gate. BSS placement and zero
matching direct integer index stores are a scoped census,not complete writer
or live-state proof. Mode engine/services/teardown and original traces remain open.


[Field party driver](behavior/partner/FIELD_PARTY_DRIVER_DISPATCH.md) supplies all15
local mode branches,pending setup,mode14 destruction and effect/screen/presentation
tail. [Global countdown/action](behavior/partner/FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md)
connects signed byte expiry to primary mode0/1/2→3 requests. Called ability/mode/
collision/animation engines,writers,teardown and original execution remain open.


[Ordinary partner walk](behavior/partner/FIELD_PARTNER_WALK_CALLBACK.md),
[follower history](behavior/partner/FIELD_PARTNER_HISTORY_WRITER.md) and
[mode1 substates](behavior/partner/FIELD_PARTNER_MODE1_SUBSTATES.md) now specify local
initialization/speed arithmetic,two-bank history append/read policy and hazard/
effect requests. The ring's pre-wrap equality test is preserved. Final executor,
terrain/ability/source/index/writer services and original trajectories remain open.


[Switch stack/scans](behavior/event/EVENT_SWITCH_STACK_AND_CASE_SCANS.md) and
[group/mask/range](behavior/event/EVENT_GROUPED_MASK_AND_RANGE_CASES.md) specify
complete local34..49/eleven CASE helpers,actual opcode table,signed state and
stack writes,OR/AND latches,raw any-bit masks,inclusive ranges and lexical
header continuations. Original script/domain composition and runtime traces
remain incomplete;remaining VM opcodes are separate EVT-01 requests.


[Partner walkMain dispatch](behavior/partner/FIELD_PARTNER_WALK_DISPATCH.md) and
[stay motion](behavior/partner/FIELD_PARTNER_STAY_MOTION.md) now specify motion/substate
selection,threshold/map and distance branches,mode0 admission,ordered collision
X/Z stores and kind7 pose requests. The mode0 guard does not skip all correction
or terminal work. Called terrain/motion/geometry/ability bodies,domain/state
writers and original trajectories remain open.


[Integer/mask IF and scans](behavior/event/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md)
and [float/string IF](behavior/event/EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) specify
complete12..33 and twenty local predicates plus strcmp. Ordered queries,raw
masks,null replacement,CR-based unordered acceptance and scan-consumed markers
are explicit. Other VM opcodes,numeric/pointer/source domains and original
branch traces remain open.


[Partner overlap correction](behavior/partner/FIELD_PARTNER_OVERLAP_CORRECTION.md),
[direction/speed query](behavior/partner/FIELD_PARTNER_DIRECTION_AND_SPEED.md) and
[speed numerics](behavior/partner/FIELD_PARTNER_SPEED_NUMERICS.md) now supply signed
counter/state/history side effects,first-direction/second-speed output order,
fused vector/norm composition and three double estimate refinements. The stay
queries cannot be coalesced as pure reads. Numeric/source/terrain/domain and
original trajectory evidence remain open.


[Multiword/float data](behavior/event/EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md),
[bitwise/time/memory](behavior/event/EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) and
[user-buffer allocation](behavior/event/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md)
specify complete local65..90,including indexed signed-integer float conversion,
raw versus repeated operand queries,word-wrap time products and best-fit/split/
zero/cache/publication order. Heap/pointer/numeric/source/capacity domains and
original runtime traces remain open.


[Partner footmark clear/ID](behavior/partner/FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md),
[live target distances](behavior/partner/FIELD_PARTNER_LIVE_TARGET_DISTANCES.md) and
[SDK distance](behavior/partner/FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md) now distinguish
flag-word ID from other bank predicates,record reset from target selection,and
repeated Mario/partner pointer reads. Paired accumulation and one single norm
refinement differ from the direction query. Source/numeric environment/domain
and original trajectory/output evidence remain open.


[Event tail/debug](behavior/event/EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) and
[all119 opcode coverage](behavior/event/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md) close
local dispatcher entry coverage,including WAIT_WHILE/no-ops/debug formatting/
metadata/unchecked slot probe. Source/address/callback/context/capacity/time/
heap/numeric/platform domains and original execution remain incomplete;this
is not a complete executed Jump/story VM or complete-game requirements claim.


[Partner front-wall query](behavior/partner/FIELD_PARTNER_FRONT_WALL_QUERY.md) and
[filters/status](behavior/partner/FIELD_PARTNER_FRONT_WALL_FILTERS.md) now specify
ordered rays,quantized distance,half-tangent correction and asymmetric hit/null
returns. Backend result anchors correct the earlier swapped ABI labels:point+24,
normal+30. Full backend/traversal/mesh/domain and original collision traces remain open.


[Event context init/reset](behavior/event/EVENT_CONTEXT_INITIALIZATION_AND_RESET.md)
and [bootstrap SDK heap/direct callers](behavior/event/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md)
now establish two initial256-slot arrays,selected-array bulk reset with bank/
schedule/ID/speed preservation,distinct SDK allocation and four direct DOL
bindings. Full context/capacity/source/heap/scene/platform/clock domains and
original reset/transition traces remain open.


[Retrace clock](behavior/platform/EVENT_RETRACE_TIME_PUBLICATION.md) and
[outer/update order](behavior/platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) now
bind event timestamp accumulation to raw time-base deltas with GP10/window
freeze and unconditional baseline updates. Complete main/StMain requests pin
input-before-battle-before-event ordering and separate render/profile intervals.
Hardware/gate/scene/numeric domains and original measured cadence remain open.


[Partner surrounding-wall binding](behavior/partner/FIELD_PARTNER_SURROUNDING_WALL.md)
closes the used mode1 wrapper,global view/filter ownership,zero-speed correction,
fixed pass origins and local SDK vector numerics. Corrected the older shared
wall contract:save the UNCLAMPED first vector before cap;ordered dot>=0 skips
second correction,negative/unordered takes half-add under nontrapping continuation.
Native policy/fixtures were notified via inbox;documentation validation does not
claim native parity. Full scene/filter/domain/math and original trajectories
remain open.


[Map release/battle pool](behavior/memory/MAP_BUFFER_FREE_AND_BATTLE_POOL.md),
[five-pool bootstrap/reset](behavior/memory/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md) and
[SDK free/caller census](behavior/memory/SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md)
now specify complete local memory construction/release/coalescing/reset and50
DOL sites. Map and SDK size/null/merge/ownership policies remain distinct;
scene/REL/alias/platform/domain and original runtime ownership remain open.


[Partner fly height postpass](behavior/partner/FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md)
and [ground postpass](behavior/partner/FIELD_PARTNER_GROUND_POSTPASS.md) now close used
stay-tail query order,owner-gated height smoothing and floor snap/fall/named-
support motion requests. Selected floor-query entry proves early mask return
can preserve cached outputs. Complete local motion-change/name-classifier leaves
are specified,including exact25-name resource table;full queries,appearance/
motion/terrain/domain and original trajectories remain dependencies.


[Partner ceiling query](behavior/partner/FIELD_PARTNER_CEILING_QUERY.md) and
[scalar ray wrapper](behavior/field/FIELD_SCALAR_RAY_WRAPPER.md) now close local
four-probe first-success order,raw height cap,success/failure output writes,
callback/origin/direction/length ABI and ordered point/normal publication. Query
V.Y is captured input Y,not hit-point Y;fly ignores returned height,and all-null
still writes fourth sampled X/Z plus height0. Original caller behavior is explicit,
not replaced with a conventional ceiling solver. Full backend/numeric/source/
trajectory and remaining floor/motion engines remain open.


[Disc-thread readiness gate](behavior/platform/EVENT_DISC_THREAD_READINESS_GATE.md),
[system-message presentation](behavior/platform/DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md)
and [audio cover-state requests](behavior/audio/AUDIO_DISC_COVER_STATE_REQUESTS.md)
now bind GP10 freeze to exact status/retrace/card/thread behavior and an alternate
input/render request path. Original notice bytes/pointers and local audio state
are pinned;SDK/ROM-font/render/playback/source/domain and original traces remain open.


[Partner floor query](behavior/partner/FIELD_PARTNER_FLOOR_QUERY.md),
[slope pipeline](behavior/partner/FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md) and
[heading adjustment](behavior/partner/FIELD_PARTNER_FLOOR_HEADING.md) close the used
five-sample/global-filter/order,pointY quantization,50-degree partial-angle
rejection,duplicated admitted-slope requests,ordered drop/near-height admission
and highest-support cache graph. Slope fields may describe later rejected rays
rather than retained support;null can preserve outputs or initialize height/
last-drop depending on entry flag. Full backend/math/domain/field consumers and
original trajectories remain open.


[Complete battle unit entry](behavior/battle/BATTLE_UNIT_ENTRY_COMPLETE.md),
[registry/scalar/geometry](behavior/battle/BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md)
and [pose/display initialization](behavior/battle/BATTLE_UNIT_POSE_AND_DISPLAY_INITIALIZATION.md)
now specify full local construction/publication/stat/resource/event order and
listed getters/setters/pose helpers. Borrowed source/kind/content/lifetime and
resource/render/turn/platform domains and original execution remain open.


[Partner support appearance](behavior/partner/FIELD_PARTNER_SUPPORT_APPEARANCE.md),
[vector ray wrapper](behavior/partner/FIELD_PARTNER_VECTOR_RAY_WRAPPER.md) and
[screen recovery](behavior/partner/FIELD_PARTNER_SCREEN_RECOVERY.md) close complete
local12-angle sampling/return,asymmetric five-value quantization and control/
projection-gated bounded candidate recovery. Output is a sampled origin,caller
ignores failure and overwrites preparedY;screen recovery clears bank0 on both
flag branches and commits root only at explicit projection acceptance. Full
placement3/SDK projection/traversal/math/domain and original trajectories remain open.


[Placement3](behavior/partner/FIELD_PARTNER_PLACEMENT3.md),
[local math](behavior/partner/FIELD_PARTNER_PLACEMENT3_MATH.md) and
[NPC scan](behavior/partner/FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md) close complete local
nine-angle/raw-ray/name/normal/NPC/fallback/output behavior. Output is initialized
before search and can remain changed on return0;screen reset still copies it.
NPC scan returns last strict horizontal match,not nearest,and placement maximizes
its recorded3D fallback distances. NPC-free success snaps floorY and runs around;
fallback preserves ownerY and skips around. Full scene/NPC/source/numeric/runtime
and remaining partner motion engines remain open.


[Action-record byte updates](behavior/battle/BATTLE_ACTION_RECORD_BYTE_UPDATES.md),
[turn-rule judgment](behavior/battle/BATTLE_ACTION_RECORD_TURN_RULES.md) and
[first-process preparation](behavior/battle/BATTLE_TURN_FIRST_PROCESS_COMPLETE.md)
now specify exact byte-store/cap,rule sum/status-write,judge/counter/RNG/unit/fog
ordering. General challenge/action/reset/save/turn/source/backend ownership
and original executed traces remain open.


[Jump destinations](behavior/partner/FIELD_PARTNER_JUMP_DESTINATION.md),
[initialization](behavior/partner/FIELD_PARTNER_JUMP_INITIALIZATION.md) and
[flight/landing](behavior/partner/FIELD_PARTNER_JUMP_FLIGHT_AND_LANDING.md) close both
used motion2 handler graphs,main target/history priorities,distinct numeric
preparation and floor/horizontal/exception ordering. Counts at+24 are stored
preparation parameters,not local flight countdowns. Main flagged scratch movement
has no published X/Z;main landing0/marker versus jump2 distance-selected0/1
remain distinct. Full target/front-floor/pose/audio/source/numeric and original
trajectories remain open.


[Attack-command event selection](behavior/battle/BATTLE_ATTACK_EVENT_SELECTION.md) and
[weapon classification/data leaves](behavior/battle/BATTLE_WEAPON_ATTACK_CLASSIFICATION.md)
close the complete local selector and called predicate/audience/data graph.
Counters update at selection before execution;fallback event does not replace
retained weapon classification. Complete menu/event/audience/source/record reset
ownership,general rule judgment and original executed traces remain open.


[General action-record judgment](behavior/battle/BATTLE_GENERAL_RULE_JUDGMENT.md) and
[threshold/end/unit-query contracts](behavior/battle/BATTLE_RULE_THRESHOLD_AND_UNIT_QUERIES.md)
close all68 local rules and helpers,including signed scalar thresholds without
255 bypass,captured-versus-current output owners,duplicate54/55 inputs and
end-helper numeric mask0x02. Descriptor/raw record/reset/save/status consumer
ownership,full end/turn/scene scheduling and original traces remain open.


[Live direction/activity](behavior/partner/FIELD_PARTNER_LIVE_DIRECTION_AND_ACTIVITY.md),
[front-floor](behavior/partner/FIELD_PARTNER_FRONT_FLOOR.md) and
[status/clearance writer](behavior/partner/FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md)
close local direction q0 partner admission,repeated pointer roles,TargetDistY
activity scalar,guarded status codes,upward-clearance/downward-output sequence,
heading input and attribute-mask return. Front-floor miss writes current rootXYZ;
first-ray normal magnitude<45 is not an inclination-degree test. Original
float60 setter and bounded DOL access/call census are pinned;full source/alias/
REL/backend/math/domain and original trajectories remain open.


[Consolidated packed EventEntry](behavior/event/EVENT_ENTRY_PACKED_LAYOUT.md) and
[address/alias requirements](behavior/event/EVENT_ENTRY_ADDRESS_AND_ALIAS_REQUIREMENTS.md)
answer the native translator map request:all432 bytes,typed accesses/pointer
words,44 unknown raw bytes,context/global banks,and local/stack/label/message/
USER_FUNC alias requirements. Machine-readable layout and ten supporting ISO
suites are linked;actual native packed translation,source/owner/callback/backend
binding and original execution remain EVT-01 work.


[SDK/primary-map live ordering](behavior/memory/SDK_HEAP_BOOTSTRAP_LIVE_READ_ORDER.md)
clarifies retained metadata size versus live descriptor globals,post-header
descriptor size reread and start→end→handle capture before requested-length
publication. Bootstrap policy/verifier now include these exact reloads and
alias acceptance cases;native arena/cache/source/scene/runtime remains open.


[Ordinary Jump source/root binding](behavior/battle/NORMAL_JUMP_ROOT_REGISTRATION.md)
and [callback registry census](behavior/battle/NORMAL_JUMP_CALLBACK_REGISTRY.md) pin
conditional root10/flags0/typeef/context/ID/speed/post-construction actor order,
three child wrappers and156 direct sites/72 callbacks for the three packed
streams. Constructor run-check reads requested8041e940,distinct from GP18 getter
mask;native source correction is queued. Menu/global/raw/dynamic/domain/backend
closure and original execution remain EVT-01/JUMP-01 work.


[Fall air](behavior/partner/FIELD_PARTNER_FALL_AIR.md),
[landing](behavior/partner/FIELD_PARTNER_FALL_LANDING.md) and
[facing leaves](behavior/partner/FIELD_PARTNER_FALL_FACING.md) close the used complete
local fall graph:history-before-marker,floor-before-integration using old velocity,
conditional default acceleration,damping/clamps,excluded-surface mobility gates,
placement4 request/facing/exception order,two landing cleanup paths and air2/
land3 returns. Full placement4/pose/terrain/source/numeric and original trajectories
remain open;native validation does not supply original-game execution evidence.


[Unit-data child continuation](behavior/battle/BATTLE_UNIT_DATA_CHILD_CONTINUATION.md) and
[actor selector translation](behavior/battle/BATTLE_ACTOR_SELECTOR_TRANSLATION.md)
close full local wrapper/all sentinels/system query and actual Mario key7
source→tiny80416bd0 child. No USER_FUNC in that child does not remove its
ID/append/wait/copy-back lifecycle. Native source pack needs its span/trailing
word;other dynamic writers/providers/scenes and original execution remain open.


[Complete battle-pad reset](behavior/battle/BATTLE_PAD_RESET_AND_RAW_PRESERVATION.md)
clarifies snapshot0..a clear versus byteb preservation,all124 preserved bytes
and interleaved history stores. Prior whole-snapshot-zero wording is corrected;
update's full prior copy and current trailing preservation remain distinct.
SDK/physical/caller/pause/source and original sampling traces remain open.

ID1 placement4 recovery addendum (2026-10-12):the used fall caller now links
[complete placement4 local search/output](behavior/partner/FIELD_PARTNER_PLACEMENT4.md)
and [copied angle/adjacent-stack layout](behavior/partner/FIELD_PARTNER_PLACEMENT4_ANGLE_LAYOUT.md).
Local output accumulates across rejected moved attempts;final offsets can read
adjacent arrays or the reused query header. Exhaustion requests general
partyGetAppearPos,ignores its return and returns0 after optional owner-height
adjustment. The general fallback engine remains open;this is not complete
end-to-end placement or original-runtime equivalence.


[Jump Majinai power child](behavior/battle/JUMP_MAJINAI_POWER_CHECK_CHILD.md) and
[effect/nested source contracts](behavior/battle/JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md)
close14-command query/consumption graph,global name-based effect polling,
complete12-command jump child and17-command independent five-key sound root.
Additional source spans/callbacks,fortune producer,render/audio/message/backend
ownership and original execution remain EVT-01/JUMP-01/DMG-02 work.


[Battle fortune producer](behavior/battle/BATTLE_FORTUNE_PRODUCER.md) and
[pouch initializer/caller/RNG accounting](behavior/battle/BATTLE_FORTUNE_INIT_AND_CALL_BINDINGS.md)
close full local countdown/mode policy,upgrade-only signed request compare and
two direct DOL bindings. Remaining/countdown/mode use distinct byte domains;
modulo RNG thresholds and intermediate stores stay exact. Field assignment/
setup/pouch/save/turn/other consumers and original execution remain open.

ID1 general partner placement delivery (2026-10-12):
[entry/primary/admission](behavior/partner/FIELD_PARTNER_GENERAL_PLACEMENT.md),
[fallback/output](behavior/partner/FIELD_PARTNER_GENERAL_PLACEMENT_FALLBACK.md) and
[used trig/plane leaves](behavior/field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md)
close the local partyGetAppearPos writer previously left open by placement4.
Fresh signed-slot dispatch,owner reseeds,plane shortcut,asymmetric probe/move
angles,positive single-slot third displacement,raw floor/length gate and ordered
failure outputs are specified. Eight actual DOL direct caller sites are pinned;
other full caller bindings,backend/libm/source/alias/original traces remain open.
Earlier addendum's open general-local-writer status is superseded by this delivery;
full-system closure and final complete-game requirements remain incomplete.


[Majinai effect construction](behavior/effect/MAJINAI_EFFECT_CONSTRUCTION.md) and
[pool acquisition/map publication](behavior/effect/EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md)
close full local constructor/slot/helper order,payload fields and exact map+1e
four-byte request. Pool exhaustion has no local null fallback and acquisition
clears only the first instance-name byte. Update/draw/delete/resources/map/
heap/platform/source and original effect/frame behavior remain open.


[Effect soft/hard deletion](behavior/effect/EFFECT_SOFT_AND_HARD_DELETION.md) and
[caller/name/payload lifetime](behavior/effect/EFFECT_DELETE_CALLERS_AND_LIFETIME.md)
close full release leaves and271 scoped DOL sites (30 soft/241 hard). Corrected
fog helper name:8005e9c0 is soft OR04 for mask02 records,not unconditional
hard release. Update/driver/scene/backend/raw/REL and original traces remain open.

ID1 flying-partner delivery (2026-10-12):
[executor](behavior/partner/FIELD_PARTNER_FLY_DISPATCH.md),
[behind-fly](behavior/partner/FIELD_PARTNER_BEHIND_FLY.md),
[homing entry](behavior/partner/FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md),
[history loop](behavior/partner/FIELD_PARTNER_HOMING_FLY_HISTORY_LOOP.md),
[collision tail](behavior/partner/FIELD_PARTNER_HOMING_FLY_COLLISION_TAIL.md) and
[speed writers](behavior/partner/FIELD_PARTNER_FLY_SPEED_WRITERS.md) specify complete
local flying dispatch/two handlers/two scalar writers. Exact selector/tail
ownership,entry placement,countdown versus history,live versus record distance,
repeated speed effects,inherited-FPR domains,contact and publication order are
closed locally. partyFlyMain has only selected caller-tail evidence here;its
full history writer,called engines/source/math/pose and original trajectories
remain open. No full-system or complete-game equivalence claim follows.

ID1 flying record/source delivery (2026-10-12):
[history writer](behavior/partner/FIELD_PARTNER_FLY_HISTORY_WRITER.md),
[initializer](behavior/partner/FIELD_PARTNER_FLY_HISTORY_INITIALIZATION.md),
[callback bindings](behavior/partner/FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md) and
[source leaves](behavior/partner/FIELD_PARTNER_FLY_SOURCE_LEAVES.md) close seven
complete local bodies and concrete kind5/6 +8 records. Earlier selected-tail-only
partyFlyMain coverage is superseded by this local writer recovery. Slot versus
flag bank selection,unused/repeated pointer requests,unwrapped capacity guards,
flag preservation,transient selector order,post-motion offsets/trailing and live
speed/absolute-Y source choices are explicit. Full lifecycle/source/alias/math/
pose/ability/terrain/original trajectories remain open;full-game documentation
and final complete-game requirements remain incomplete.


[Majinai update/cleanup](behavior/effect/MAJINAI_EFFECT_UPDATE_STATE_MACHINE.md) and
[draw/pose scalar/material requests](behavior/effect/MAJINAI_EFFECT_DRAW_AND_POSE_REQUESTS.md)
close complete local Main/Disp,four phases/soft04 cleanup,map/material state,
exact progress thresholds/fused particle request and three draw passes. Resource/
pose/matrix/display/driver/scene/platform and original visual/frame traces remain open.

ID1 paper-form gate delivery (2026-10-12):
[slit state machine](behavior/partner/FIELD_PARTNER_SLIT_GATE.md),
[roll state machine](behavior/partner/FIELD_PARTNER_ROLL_GATE.md),
[slit-floor/forced exit](behavior/partner/FIELD_PARTNER_SLIT_FLOOR_AND_FORCED_EXIT.md)
and [paper wrappers](behavior/partner/FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md) close ten
complete local bodies and22 direct used bindings. Count/phase/return ownership,
ordered headings,sign-sensitive roll exit snap,rootY slit rise,0x3c0 cleanup,
global availability/output and guarded paper handle/name release are explicit.
Collision helper,underlying animation/resource/matrix and source/alias/numeric/
original-runtime domains remain open. Full-game requirements remain premature.


[Effect driver/texture cache](behavior/effect/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md)
and [pool/init/context release](behavior/effect/EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md)
close full local traversal/cache/init/auto-release,512-record source capacity,
four exact direct bindings and effect→animation→event teardown requests. US
effect texture file identity is recorded;file/TPL/pose/display/platform/scene/
source and original update/resource/frame traces remain open.


[Formatted file forwarding](behavior/file/FILE_FORMATTED_REQUEST_FORWARDING.md) and
[handle release/cache ownership](behavior/file/FILE_HANDLE_RELEASE_AND_EFFECT_CACHE_OWNERSHIP.md)
close complete Asyncf/Allocf/direct allocation wrappers and signed-halfword
reference release. Shared1024-byte filename,original varargs/forwarding order
and state2 without deallocation are explicit. Underlying resource/I/O/cache/
collection/TPL/platform/scene and original traces remain open.

ID1 slit collision/math delivery (2026-10-12):
[slit collision](behavior/partner/FIELD_PARTNER_SLIT_COLLISION.md),
[simple camera direction](behavior/camera/FIELD_SIMPLE_CAMERA_DIRECTION.md) and
[system angle table](behavior/field/FIELD_SYSTEM_ANGLE_TABLE.md) close three new local
algorithms. Six maximum ordered/filtered probes,first-hit side ownership,raw
normalXZ arithmetic-average gate,no output correction,camera quantization and
91-coefficient quadrant/zero-vector rules are explicit. Existing filter contracts
are reused;full backend/source/numeric/original-runtime domains remain open.

[US message inventory correction](behavior/meta/US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md)
rechecks260 resources/13186 actual key/text pairs. Newly generated filename-ID/
identifier-only/story-prefix claims do not supersede recovered loader/lookup
contracts or establish dialogue/story coverage. Final full-game requirements
remain premature.

ID2 shared-file acquisition delivery (2026-10-12):
[cache/reference/publication](behavior/file/FILE_ACQUISITION_AND_REFERENCE_REUSE.md)
and [archive/DVD requests](behavior/file/FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md)
close _fileAlloc,arcOpen and five narrow SDK/DVD leaves. Filename-only reuse,
pending yield,state2/3 count differences,retained available node,live manager
publication,identical archive retries,holder indirection and deferred close are
explicit. Async/collection/relocation/TPL/source/platform/original traces remain open.

ID2 async-file delivery (2026-10-12):
[request/cache publication](behavior/file/FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md)
and [completion/request state](behavior/file/FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md)
close fileAsync,dvdReadDoneCallBack and DVDMgrReadAsync. Raw-1 versus pending0,
state3/count0 preload,archive live-head versus retained DVD link,-3-only failure,
signed-byte relocation type,pending-clear/close/live user-callback order and flag
reread are explicit. Worker/collection/relocation/TPL/source/runtime remain open.

ID2 file collection/init delivery (2026-10-12):
[preload aging/collection](behavior/file/FILE_CACHE_COLLECTION_AND_PRELOAD_AGING.md)
and [pool/init/mode](behavior/file/FILE_CACHE_POOL_INITIALIZATION.md) close four local
bodies. Mode0 worker flags overwrite,mode1 initially-pending wait without aging,
signed count<=-2,state2 collection/holder routing/live unlink/retained traversal,
1024*176 heap0 clear/link and17 direct DOL sites are explicit. Smart heap/worker/
relocation/TPL/source/platform/scene/original traces remain open.

ID1 partner presentation delivery (2026-10-12):
[pre-display](behavior/partner/FIELD_PARTNER_PRE_DISPLAY.md),
[pose selection](behavior/partner/FIELD_PARTNER_POSE_REQUEST_SELECTION.md),
[facing](behavior/partner/FIELD_PARTNER_DISPLAY_FACING.md),
[matrix/draw requests](behavior/partner/FIELD_PARTNER_DISPLAY_MATRIX_AND_PASSES.md) and
[Vivian/shadows](behavior/partner/FIELD_PARTNER_VIVIAN_PRESENTATION_AND_SHADOWS.md)
close eight complete local bodies. Pending name/paper markers,frame setup,
animation-before-render gate,depth/registration,trailing rays/shadow source,
ordered aliased transforms,two draw calls and post-draw rate are specified.
Renderer/animation/SDK matrix/resource/lifetime/source/numeric/original visual
composition remains open. Partners/NPCs register now reflects partial local
coverage,not full-system closure. Unvalidated BTLNO name associations remain
superseded by the message-resource correction;final game requirements deferred.

ID2 smart release delivery (2026-10-12):
[header release/accounting](behavior/memory/SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md)
and [automatic tag release](behavior/memory/SMART_AUTOMATIC_TAG_RELEASE.md) close
smartFree/smartAutoFree. Tag4→3 deferral,no heap deallocation,live unlink/space/
free-list/count updates,stale payload and low16 first-pass versus full-word3
second-pass policy are explicit. Allocation/compaction/relocation/tag/source/
scene/platform/original traces remain open.

ID2 smart compaction delivery (2026-10-12):
[compactor/pending barriers](behavior/memory/SMART_HEAP_COMPACTION_AND_PENDING_FILES.md)
and [file payload/address requests](behavior/file/FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md)
close smartGarbage/fileGarbageMoveMem. Draw-sync request,equal-address bypass,
nonworker pending barriers,worker retarget,live holders/next/gaps,tail/full-arena
flush and separate clear ENTRY/setup PAYLOAD arguments are explicit. Relocation/
allocator/worker/cache/platform/source/scene/original traces remain open.

ID1 animation setter closure (2026-10-12):
[selector/restart](behavior/display/ANIMATION_SELECTOR_AND_RESTART.md),
[paper-group reuse](behavior/display/ANIMATION_PAPER_GROUP_REUSE.md),
[local time/clock](behavior/display/ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md)
close five complete used local service bodies. Signed selector encoding,forced
restart,group reuse/failure,shared/linked timestamp publication and rate writes
are implementation contracts;clock/conversion/resource/Main/draw/lifecycle/
original visual domains remain open. 299 direct DOL sites are references,not
an original runtime census or complete animation system.

ID2 smart allocation delivery (2026-10-12):
[header/gap placement](behavior/memory/SMART_ALLOCATION_HEADER_AND_GAP_PLACEMENT.md)
and [retry/failure ownership](behavior/memory/SMART_ALLOCATION_RETRIES_AND_FAILURE_OWNERSHIP.md)
close smartAlloc. First flag/auto3,release-counter sync/reset,unguarded header
reservation,word-wrap alignment,leading/first-fit gaps,three tail-only retries
and failure without header restoration are explicit. e018 stock-count wording
is corrected. Arena/relocation/worker/platform/source/runtime remain open.

ID2 smart initialization delivery (2026-10-12):
[arena/header initializer](behavior/memory/SMART_ARENA_AND_HEADER_INITIALIZATION.md)
and [scoped provisioning requirements](behavior/memory/SMART_RESOURCE_PROVISIONING_REQUIREMENTS.md)
close smartInit. Heap-index4 request,conditional first/unconditional second
clear,2048*28 header links/live endpoints and flag/counter reset are explicit.
Full-game RAM/requirements and heap/platform/source/runtime remain open.

ID2 relocation dispatch/flat delivery (2026-10-12):
[dispatch/type map](behavior/file/FILE_RELOCATION_TYPE_DISPATCH.md) and
[flat pointer handlers](behavior/file/FILE_FLAT_POINTER_RELOCATION.md) supply
all11 routes and eight complete flat Set/Clear blocks. Clear resolves holder
before type rejection;unsigned first-field guards,retained first value,ordered
uniform arithmetic and no null preservation are explicit. Nested4/5 handlers,
source formats/consumers/platform/original runtime remain open.

ID1 animation Main/init local closure (2026-10-12):
[entry/time](behavior/display/ANIMATION_MAIN_ENTRY_AND_TIME.md),
[samples](behavior/display/ANIMATION_TIMELINE_SAMPLING.md),
[cache](behavior/display/ANIMATION_COMPRESSED_DELTA_CACHE.md),
[interpolation](behavior/display/ANIMATION_OUTPUT_INTERPOLATION.md),
[facing](behavior/display/ANIMATION_MAIN_CAMERA_FACING.md),
[init](behavior/display/ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md)
cover complete local graphs and an original M_Z_1 serialized example. These
services are referenced by original partner/Mario pre-display,draw and paper
selectors;65 direct DOL sites are static references,not runtime frequency.
Resource/pool/relocation/copy/clock/helper/libm/draw/source/lifetime/original
visual domains remain open. No final whole-game requirements justified yet.

ID2 type4 relocation delivery (2026-10-12):
[palette setup](behavior/file/FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_SETUP.md) and
[palette clear](behavior/file/FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_CLEAR.md) close
nested type4 handlers:signature/panic,root guard,low16/live count,descriptor/data/
flag ordering and root-last clear. US effect source82 image headers/zero palette
slots pinned read-only. Type5/consumers/platform/source/original runtime remain open.

ID2 type5 relocation delivery (2026-10-12):
[nested setup](behavior/file/FILE_TYPE5_NESTED_POINTER_SETUP.md) and
[nested clear](behavior/file/FILE_TYPE5_NESTED_POINTER_CLEAR.md) close type5:
root25 fields,captured signed count,64-byte records,root-relative parent links,
child-relative8 fields,retained child/fresh root clear ordering. With flat/type4
contracts,all local Set/Clear routes are documented. Formats/consumers/platform/
source/scene/original traces/full-game requirements remain open.

ID2 DVD request delivery (2026-10-12):
[slot open/global callback](behavior/file/DVD_MANAGER_SLOT_OPEN_AND_GLOBAL_CALLBACK.md)
and [synchronous wait](behavior/file/DVD_MANAGER_SYNCHRONOUS_READ_PROTOCOL.md)
close Open/Read/SetupCallback. First256-slot8000 admission,path-1 gate,ordered
clear/name/LOW8-LOW16 parameters/occupied-before-open,0002 yield and length-word
return/global callback separation are explicit. Worker/SDK/source/platform/
scene/original traces/full-game requirements remain open.

ID1 draw local closure (2026-10-12):
[wrapper](behavior/display/ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md),
[buffers/selector](behavior/display/ANIMATION_DRAW_SHARED_BUFFERS_AND_PAPER_SELECTION.md),
[paper matrices](behavior/display/ANIMATION_DRAW_PAPER_MATRICES.md),
[inner state](behavior/display/ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md),
[facing](behavior/display/ANIMATION_INNER_DRAW_FACING.md),
[groups](behavior/display/ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md)
close five complete used local bodies. Original Mario pass/root metadata and
binary-derived modeling/tangent addresses are checked;previous803e tangent
locations corrected to803d. Original SDK/push/render/material/GX/resource/source/
lifetime/numeric/visual composition remains open. Host movement failures are
recorded separately;neither static coverage nor host green would prove full game.

ID2 native-port first-attack delivery (2026-10-12):
[paired selector/getters](behavior/battle/FIRST_ATTACK_PAIR_SELECTION_AND_GETTERS.md)
and [event/body-part order](behavior/battle/FIRST_ATTACK_TARGET_EVENT_AND_BODY_PART.md)
close seven complete local bodies requested by native EVT-01. Captured battle/raw
output order,setup modes/paired outputs/last-slot miss,getter kind/null policies,
body-before-id/setters and initial part source witness are explicit. Deeper DVD
work deferred per user priority;source/Game/original runtime remain open.

ID2 native movement-wrapper delivery (2026-10-12):
[complete setter order](behavior/battle/BATTLE_MOVEMENT_SETTER_EVENT_ORDER.md)
and [original source tokens](behavior/battle/BATTLE_MOVEMENT_WRAPPER_SOURCE_TOKENS.md)
close SetMoveSpeed/SetFallAccel and leaves requested by native EVT-01. Retained
arguments,GetValue→translation→GetFloat→fresh battle/lookup,leaf-only stores,
unchecked-1/null and completion2 pinned. Eight source requests prove exact bits;
Majinai f24a7c80 sign corrected to+0.5. Source/Game/runtime remain open.

ID1 node-builder/SDK local closure (2026-10-12):
[joint](behavior/display/ANIMATION_JOINT_NODE_MATRICES.md),
[transform](behavior/display/ANIMATION_TRANSFORM_NODE_MATRICES.md),
[stack/affine](behavior/display/ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md),
[rotation](behavior/display/ANIMATION_SDK_AXIS_ROTATION.md),
[layout/used bindings](behavior/display/ANIMATION_NODE_LAYOUT_AND_USED_BINDINGS.md)
close both node builders and seven matrix/two trig-wrapper local bodies. Four
actual traversal call sites and original Mario13/29 node-kind source metadata
are checked. Current generic bake formula is not full original node behavior.
Underlying libm/GQR0/FPSCR/render/material/GX/resource/source/lifetime/original
visual domains remain open. Current74-host/78-Python/3DS build run passes and
supersedes prior movement/fixture validation failures;full-game requirements
are still unproven.

ID2 native menu-domain delivery (2026-10-12):
[complete setup/decision](behavior/battle/WEAPON_MENU_SETUP_AND_DECISION.md)
and [scoped type/cursor source](behavior/battle/WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md)
supply two complete functions,16 DOL direct calls,type12 writer/state0 restore
and same-frame initialized cursor reuse. A globally supported menu-type invariant
requires reachability proof;GetSelectWeapon's separate uninitialized slot is not made
safe by Input reuse. Full menu/query reachability/Game/source/runtime stay open.

ID1 shape renderer/material local closure (2026-10-12):
[entry/defaults](behavior/display/ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md),
[overrides](behavior/display/ANIMATION_RENDER_TEXTURE_OVERRIDE_SELECTION.md),
[last-stage](behavior/display/ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md),
[arrays/FIFO](behavior/display/ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md),
[source/boundaries](behavior/display/ANIMATION_RENDER_RESOURCE_EXAMPLE_AND_PORT_BOUNDARIES.md)
close two complete local request bodies with three original DOL bindings.
Original Mario ordinary single-texture/all-kind0 metadata is checked;raw157
all-call fan triangles are not a frame image or native102-triangle idle result.
Texture/map/GX/SDK/resource/source/lifetime/numeric/original visual composition
remains open. Current76-host/86-Python/3DS build passes;full-game requirements
are still unproven.

ID2 native action-class producer delivery (2026-10-12):
[Admission/publication](behavior/battle/ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md)
and [eligibility/keys](behavior/battle/ACTION_CLASS_TABLE_ELIGIBILITY_AND_MENU_KEYS.md)
close complete _btlcmd_MakeActClassTable:ordered types0/6/1/2/4/5/13,partial
20-byte records,disabled counted rows,independent Operations/Lecture flag reads,
retained cursor and signed correction,seven original keys/icons.4 hashes,
366 anchors/7 spans verified;76 CTest/86 Python and make all pass.ActMenuMain
publication/query reachability/Game/source/runtime remain open.

ID2 native ActMenuMain writer closure (2026-10-12):
[Selection/type](behavior/battle/ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md),
[navigation/order](behavior/battle/ACT_MENU_MAIN_NAVIGATION_AND_SERVICE_ORDER.md),
and [setup/end/position gate](behavior/battle/ACT_MENU_SETUP_END_AND_POSITION_GATE.md)
recover six complete local bodies:three type stores,type13→state29,14-way
switch,live class count,post-service cursor rereads,nine partial cursor resets,
ordered Mario/Party gate,and result independent of mutation.24 hashes/472
anchors/3 spans,77 CTest/86 Python/make all passed.Input/confusion/query
reachability/Game/services/runtime remain open.

ID2 complete command Input delivery (2026-10-12):
[Entry/class/re-entry](behavior/battle/COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md),
[target](behavior/battle/COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md),
[remaining/attack](behavior/battle/COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
close complete BattleCommandInput801229d8/838:30 states/default,retained
actor/menu/shared cursor,pending/progress loops,fall-through/waits,paired target
remap,independent directions,confirmation/audience order,and state29 live clears.
39 hashes/526 anchors/3 spans verified;77 CTest/90 Python/make all pass.
Callee/source/query reachability/Game/platform/original execution remain open.

ID2 native weapon-menu Main/eligibility closure (2026-10-12):
[Confirmation](behavior/battle/WEAPON_MENU_MAIN_CONFIRMATION_AND_LIGHT_REQUESTS.md),
[navigation/End](behavior/battle/WEAPON_MENU_MAIN_NAVIGATION_CANCELLATION_AND_END.md),
[count/update](behavior/battle/WEAPON_MENU_TARGET_COUNT_AND_ELIGIBILITY_UPDATE.md)
supply seven complete local bodies,typed private sampling,captured row count,
confirmation/boundary/request/fresh cursor/result ownership.20 hashes/344
anchors/2 keys verified;78 CTest/90 Python/make all pass after resolved initial
host link failure.Full builder/source/query reachability/Game/runtime remain open.

ID2 full weapon-table producer closure (2026-10-12):
[Entry/layout](behavior/battle/WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md),
[ordinary/badges](behavior/battle/WEAPON_TABLE_JUMP_HAMMER_AND_BADGE_ROWS.md),
[inventory/Special/party](behavior/battle/WEAPON_TABLE_ITEM_SPECIAL_AND_PARTY_ROUTES.md),
[sources/icons](behavior/battle/WEAPON_TABLE_SOURCE_BINDINGS_AND_ICON_LEAVES.md)
close full builder/two icon leaves:all local paths/partial rows/source arrays/
late overrides/empty count preservation/scroll/count-address delegation.
10 hashes/894 anchors/52 spans verified;78 CTest/90 Python/make all pass.
Party delegate and source/query reachability/Game/platform/runtime remain open.

ID1 used texture/light/color/fog recovery (2026-10-12):contracts start at
[texture objects](behavior/display/ANIMATION_TEXTURE_OBJECT_SETUP.md),
[coordinate matrices](behavior/display/ANIMATION_TEXTURE_COORDINATE_MATRICES.md),
[smart patch](behavior/display/ANIMATION_SMART_TEXTURE_DATA_PATCH.md),
[color/fog](behavior/display/MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md) and
[light selection/channels](behavior/display/MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md).
Fourteen complete local bodies now define the actual render/material helper
request graph,including texture/query/comparator leaves. Reproduce with
verify_animation_texture_light.py:1189 words/11 constants/eight spans/two imports/
41 direct sites and65536 bounded host integer cases. Called TEV/GX/numeric/source/
lifetime/runtime and broader whole-game systems remain incomplete.

ID2 interruption writer closure (2026-10-12):
[Audience11](behavior/battle/AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md),
[gift12/recovery](behavior/battle/GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md),
[restore/fields](behavior/battle/MENU_INTERRUPTION_RESTORE_AND_AUDIENCE_FIELDS.md)
close ten local bodies,captured/global owners,post-shutdown previous type,
recovery versus generic return,typed bounds/FMA,pending/saved Input ownership.
20 hashes/243 anchors/3 spans verified;79 CTest/90 Python/make all pass.
Audience core/present source/confusion/party/query reachability/Game/runtime
remain open;no global supported cursor or safe fallback invariant claimed.

ID2 complete confusion selector delivery (2026-10-12):
[Class/type](behavior/battle/CONFUSION_CLASS_SELECTION_AND_MENU_TYPE_WRITE.md),
[weapon/target](behavior/battle/CONFUSION_WEAPON_SELECTION_AND_TARGET_POLICY.md),
[operations/party](behavior/battle/CONFUSION_OPERATIONS_PARTY_AND_EVENT_OUTPUTS.md)
close full selector:four low-byte candidate lists/original RNG/fresh type and
weapon rechecks/nine operation cases/script-null results/type13 local flustered.
9 hashes/388 anchors/5 spans verified;80 CTest/90 Python/make all pass.
Producer/source/query/event/Game/platform/runtime remain open.

ID1 ordinary US TEV update (2026-10-12):
[base/source](behavior/display/MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md),
[constant/character shadow](behavior/display/MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md),
[final callback/gaps](behavior/display/MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md)
specify complete scoped request blocks and four shadow getters. Original Mario
24 draw records all mode0. Reproduce with verify_map_tev_ordinary.py:one partial
containing identity/four complete leaves,303 scoped words,three spans,two imports,
six direct sites/source histogram. Other modes/six middle branch internals/GX/
source/lifetime/images and whole-game requirements remain open.

ID2 operation/party producer closure (2026-10-12):
[Entry/rows](behavior/battle/OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md),
[charge/mandatory](behavior/battle/OPERATION_TABLE_CHARGE_DEFEND_ESCAPE_AND_KEYS.md),
[party production](behavior/battle/PARTY_SELECTION_TABLE_PRODUCTION_AND_CAPTURE.md),
[icon/status](behavior/battle/PARTY_ICON_SOURCE_AND_CAN_ACT_STATUS.md)
close four bodies:partial descriptors/disabled counted parties/optional+mandatory
operations/ordered setup-cost-current HP/cursor publication/different scan bounds.
16 hashes/472 anchors/32 spans verified;80 CTest/90 Python/make all pass.
Menu/delegate/source/query/Game/platform/original runtime remain open.

ID2 complete operation-menu closure (2026-10-12):
[Opcode/state](behavior/battle/OPERATION_MENU_MAIN_OPCODE_AND_STATE_PUBLICATION.md),
[navigation/order](behavior/battle/OPERATION_MENU_NAVIGATION_CANCEL_AND_AUDIENCE.md),
[setup/end](behavior/battle/OPERATION_MENU_SETUP_END_AND_FLAG_OWNERSHIP.md)
close four local bodies:pending/admission/fresh opcode/return ownership/all
state routes/edge controls/caller-specific End/live flag order.14 hashes/213
anchors/2 keys verified;81 CTest/90 Python/make all pass.Party/delegate/
source/query reachability/Game/platform/original runtime remain open.
