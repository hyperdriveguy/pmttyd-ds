# Opening Crump Encounter: REL Entry, Jump Selection, and Tutorial Boundaries

US G8ME01 revision 0, recovered 2026-10-12. Evidence B: original gor.rel, DOL battle
scripts, and event streams. This document consolidates the recovered local contracts
for the opening Crump encounter. No original execution trace or complete tutorial
equivalence is claimed. All addresses are US RAM virtual addresses unless marked
REL file offset.

## 1. REL Entry, Unit/Weapon/Event Selection, and Tutorial Exceptions

### 1.1 Harbor Scene Chain and Battle Entry

The harbor scene (`gor.rel`) selects its event flow through the story-state operand
`0xf5de0180` at REL `0x2f528`. The relevant case values and their immediate
requests are:

| Story Value | Event Spawned | REL Spawn Site | Note |
|-------------|---------------|----------------|------|
| 2 | `enter_gorotsuki_town` | `0x2f540` | Initial town entry |
| 3 | `christine_osoware` | `0x2f564` | Christine captured setup |
| 4 | `gundan1000tai_beforebattle` | `0x2f5bc` | **Tutorial battle entry** |
| 5 | `gundan1000tai_afterbattle` | `0x2f60c` | Post-battle cleanup |

The beforebattle event at REL `0x2639c` (size `0x830`, 132 commands) requests
`evt_npc_set_battle_info(actor 0x14b40, literal 2)` at `0x269dc`, then
`evt_npc_battle_start(actor 0x14b40)` at `0x269ec`, then waits via
`evt_npc_wait_battle_end`.

The NPC setup index 2 selects the area record at REL `0x60eb8` (stride `0x44`).
That record's weighted off-list at `0x60dd0` has one entry (weight 50) pointing
to group `0x60c90`, which has unit count 1 with setup `unit_boss_kanbu1`
(REL `0x645c8`). The unit's kind data table at REL `0x646b4` contains key48→DOL default
Dummy, key49→REL dead_event `0x64e58`, then a zero terminator. Its init event at REL `0x64808` installs hooks for Wait,
Phase, Attack, Damage, and default Confusion on owner -2.

### 1.2 Crump's NPC Helper vs. Lecture Goombella

Two distinct Goombella kinds exist in this module:

| Role | Kind Pointer (REL) | Raw Kind | Setup/Init |
|------|-------------------|----------|------------|
| Crump's spawned NPC helper | `unit_npc_christine` `0x66d78` | `0xdd` | `entry_npc_christine` `0x647d8` → init `0x66ed8` |
| Lecture attacker/helper | `unit_lecture_christine` `0x66fe8` | `0xb1` | `group unit setup` `0x60cb0` → init `0x67160` |
| Lecture coordinator (professor) | `unit_lecture_frankli` `0x67fb8` | — | `entry_lecture_frankli` `0x67130` → init `0x682b8` |

Crump's init (REL `0x64808`) calls `SpawnUnit(setup 0x647d8, flags 0)` and stores
the returned unit ID in Crump's work0. The NPC init `0x66ed8` (8 commands)
installs Wait/Attack/Damage hooks and `SetMaxMoveCount(-2, 0)`. It has **no**
lecture-mode setter, professor spawn, or selector-6 registration. The lecture
init `0x67160` (19 commands) installs `_set_lecture_mode`, spawns the professor,
cross-links work0 between Goombella and professor, writes battle flag `0x1000000`
to Mario's unit+0x104, and writes `1` to `W+0x2740`. These lecture-only writes
**do not occur** in Crump's opening encounter.

### 1.3 Ordinary Jump vs. Tutorial Weapon Selection

The ordinary Mario Jump event stream is `marioAttackEvent_NormalJump` at DOL
`0x80355cf0` (size `0xf0c`, 268 commands, SHA-256
`2858f3f63cfb95dc84eb6ed3b363c530e24200063044f37afd275d3847f9720c`). Its
SetupAC call sites at `0x80355ebc` and `0x80356038` pass callback operands
`(-2, 1, 1, 0)`, selecting **action command ID 1** (TimingA). The ordinary
Hammer core stream `marioAttackEvent_NormalHammer_Core` at `0x8035d6d4` uses
ID 3 (StickKeepLeft).

The Jump stream has an original **first-attack branch**:
at `0x80355d08`, battle mask 2 (bit 1) is tested. If nonzero, it calls
`GetFirstAttackTarget` and uses the ordinary weapon pointer `0x803538a8`
(Jump). If zero, it calls `GetSelectEnemy`, `CommandGetWeaponAddress`,
`AttackDeclare`, `WaitGuardMove`, `PayWeaponCost`, and runs the Majinai
power-check child. The actual harbor mask2 producer remains unbound. Crump init sets battle
flag0x1000; that separate bit does not establish mask2. The configuration
word+18 is not installed by the recovered npcSetupBattleInfo body, whose
load is overwritten. Do not force the first-attack path from configuration
or tutorial identity alone. The ordinary Jump stream's delayed SetupAC for the
first-attack path is created in an attached brother at `0x8035600c`; the parent
requests `MovePosition` at `0x80356054`. There is **one** `StartAC(1)` site at
`0x80356204`, outside the hit loop.

**Defense availability**: The defense checker `BattleActionCommandCheckDefence`
is invoked by `btlevtcmd_ResultACDefence` (DOL `0x80107694`) from the event
stream's `ResultACDefence` call. The Crump boss attack stream
`HARBOR_BOSS_NORMAL_ATTACK.md` documents that precheck result 1 alone requests
defense via `ResultACDefence` with `CheckDamage` flags `0x100`. The ordinary
Jump stream does not invoke `ResultACDefence`; defense is only available when
the enemy attack script explicitly requests it.

## 2. Jump SetParam/Flag/Difficulty/Count/State Writers and Call Order

### 2.1 Setup and Parameter Writers

The ordinary Jump stream **does not** call `AcSetParamAll` or `AcSetFlag`
anywhere in its 268 commands. The SetupAC wrapper (`btlevtcmd_SetupAC`,
`0x801077ec`) resolves operands as follows:

1. Operand 0 → `evtGetValue` → `BattleTransID` → `BattleGetUnitPtr` (actor)
2. Operand 1 → `evtGetValue` (command ID)
3. Operand 2 → **raw** (becomes `battle+0x1c94`, option word)
4. Operand 3 → `evtGetValue` (becomes `battle+0x1c98`)

For the four literal SetupAC sites, raw operand 2 is `1`. The first-attack
brother at `0x8035600c` and the ordinary branch at `0x80355e80` both call
`SetupAC(-2, 1, raw 1, 0)`.

### 2.2 Difficulty and Action Level

- `GetWeaponActionLv(L12, L0)` at `0x80355e80` (ordinary branch) and at
  `0x8035600c` (first-attack brother) reads the weapon's action level.
- `AcSetDifficulty(-2, L0)` stores the base difficulty at `battle+0x1cbc`,
  applies badge adjustment (Unsimplifier/Simplifier from unit+0x306/0x305),
  clamps to `0..6`, and stores final at `battle+0x1cbd`.
- The ordinary Jump stream does **not** write a difficulty parameter via
  `AcSetParamAll`; it uses the weapon action level directly.

### 2.3 Loop Count and State Writers

- `SET(L10, 0)` at `0x80356204` initializes the script's loop counter L10.
- On timing success with count ≤ 1: `ADD(L10, 1)` then `GOTO 10` (repeats
  precheck at `0x8035621c` with flags `0x100100`). The shared AC count at
  `battle+0x1cb4` is incremented by the TimingA callback; the script loop
  counter L10 is separate.
- On timing success with count > 1: `WAIT_FRAME(2)` then converges on label 90
  (final success cleanup).
- On timing failure: `CheckDamage` with flags `0x100`, then return movement
  via label 95.
- Avoidance results (2, 3, 6): request `StartAvoid` with codes 38/39/40, then
  `StopAC`, `JumpContinue`, landing pose, `WAIT_FRAME(40)`, `GOTO 95` —
  skipping `ResultAC`, damage, feedback, and aftereffect.

### 2.4 Result and Stop Services

- `ResultAC()` at `0x803565e0` calls `BattleActionCommandResult`, which
  invokes the stored result callback (if nonnull) and returns its value,
  otherwise returns stored `battle+0x1cb8 & 2`. The event wrapper **discards**
  this return and instead rereads `battle+0x1cb8`: if bit 0 remains set,
  returns event 0 (blocked); otherwise requests `BattleAcHelpSetDispType(0)`
  and returns 2.
- `GetResultAC(L6)` at `0x803565e8` stores the result callback's return in L6.
- `StopAC` requests are made at multiple sites: after avoidance, after final
  success (label 90), and after failure.
- No `AcSetParamAll` or `AcSetFlag` calls exist in the stream.

### 2.5 Conditional Paths and Repeat Result Calls

- The precheck at label 10 branches on `L10 >= 1`: first pass uses flags
  `0x100`; subsequent passes use `0x100100`.
- Timing failure on first pass (`L10 == 0`) requests
  `AudienceDeclareACResult(L12, -1)`; later failures omit this.
- Timing success with count ≤ 1 loops back to label 10 **without** a new
  `SetupAC` or `StartAC` call.
- Timing success with count > 1 proceeds to label 90 (final success).
- Label 99 (shared by missing target, avoidance, failure, and final success)
  requests `ResetFaceDirection`, the Suitoru badge effect child, `StartWaitEvent`,
  explicit event delete (opcode 2), then terminal opcode 1.

### 2.6 Finish-State Ownership

The event stream does not write a victory/loss/escape result. The field-return
wait (`evt_npc_wait_battle_end`) accepts **any nonzero** `BattleInformation`
result at `I+0x10` as admission to cleanup. Actual result producers remain
open (see `FIELD_BATTLE_ENTRY_AND_RETURN.md`).

## 3. Battle Pad Update Order, Private AC Pad, and Impact Sampling

### 3.1 General → Sequence → Private → AC Main Order

The recovered cadence (`BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md`) establishes:

1. Four general battle pads updated (channels 0..3) before the sequence
   controller.
2. Private action-command pad (channel 0, at `battle+0x1d1c`) updated **after**
   the sequence adapter, before the selected AC main callback.
3. Private sampling runs **even when no AC main callback is active**.
4. Later main-chain event dispatch occurs after private pad update.

The 12-byte snapshot write map for each pad update:

| Byte | Content |
|------|---------|
| 0..5 | Current snapshot (buttons + stick) |
| 6..7 | Trigger bytes (pressed/released edges) |
| 8..9 | Explicit zero bytes |
| 10..11 | **Untouched/retained** (not cleared by update) |

`BtlPad_WorkInit` (`0x8011a38c`) clears bytes 0..a of each snapshot (preserving
byte b), all 30 entries of the three shifted history arrays (pressed/repeat/
held), and the repeat phase/counter. It does **not** clear the release array.

### 3.2 Defense Checker Input Sampling

`BattleActionCommandCheckDefence` (`0x800fa1b8`) uses the **private AC pad**
at `battle+0x1d1c`. Its history reader
`BattleACPadCheckRecordTrigger` (`0x800fa1a0`) reads
`word(battle+0x1d34 + 4*historyIndex) & buttonMask` with masks `0x100` (A)
and `0x200` (B). The anti-mash scan covers indices 0..14 for both buttons.
A and B at the same index count as **two** occurrences.

**Impact sampling**: The native harness currently samples the pad **before**
impact evaluation. The original game's exact input sample included at impact
has not been isolated; an original input/state trace is still needed to
validate actual timing. The defense checker is invoked from the event stream
(`ResultACDefence`), not from a fixed impact tick.

### 3.3 Pause and Animation/Impact Interaction

`EVENT_SYSTEM_LEVEL_AND_SPEED.md` documents the event-level mask table
(levels 0..4), the getter returning the whole GP mask, and the speed setter/
creation regions. Nonzero lower levels do not automatically resume older stops;
mask `0xef` excludes `0x10`. Caller UI/type/speed ownership remains open.
No fixed impact tick or frame-relative schedule is established by the local
contracts.

## 4. Automatic/Token/Battle-Force Fields, Seed Ownership, and RNG

### 4.1 Automatic and Override Fields

- **Automatic command** (unit byte `+0x307`, called AutoCommand by R): if
  nonzero, selects result 5 (superguard) in the eligible branch (step 3 of
  `BATTLE_INPUT_AND_DEFENCE.md`), or result 4 (guard) when the eligibility
  gate (byte `+0x13 != 0` and HP > 0) fails (step 5). Its legitimate game
  writers are not yet established.
- **Override counter** (signed halfword at `battle+0x1d18`): positive value
  selects result 5 (decrement once) in eligible branch, or result 4
  (decrement once) when eligibility gate fails. This is separate from the
  Automatic byte.
- **Weapon gate byte** (argument `+0x13` to `ResultACDefence`): must be
  nonzero for the B/result-5 branch to be attempted. The Crump boss weapon
  `weapon_kanbu1_attack` at REL `0x64718` has byte `+0x13 = 1`. This is a
  weapon-data gate, not battle flag `0x1000` or global tutorial input.

### 4.2 RNG Consumption

- `irand` (DOL `0x80005da0`): bound 0 returns 0 **without** calling `rand` or
  changing its state. Positive bound calls `rand` once, then uses signed
  division/multiplication for modulo reduction (no rejection sampling).
- `rand` (DOL `0x8026c324`): LCG `state = state * 1103515245 + 12345 (mod 2^32)`,
  returns bits 16..30 (range 0..32767). State at `0x8041e3c0` (r13+0x14a0).
- Setup chain RNG: the harbor tutorial record 2 consumes **17 irand calls**
  (16 audience draws + 1 group lottery draw). The zero item table consumes
  none. This is local setup-chain consumption only; not full encounter RNG.
- Event stream RNG: the Jump stream does not directly call `irand`/`rand`.
  The Majinai power-check child (DOL `0x80363388`) calls `irand(5)` on pouch
  query, and the counter script calls `irand(100)` once. Seed writers/
  interleaving and original traces remain open.

### 4.3 Seed Ownership

The `rand` state at `0x8041e3c0` is shared across battle, audience, drops,
encounters, NPC/effects, shops, minigames, scripts, and **message rendering**
(each admitted glyph calls `irand(10000)` twice). Isolating visual randomness
into an independent generator would change behavior. Original save record
random state preservation remains open.

## 5. Original US Message IDs, Speakers, Control Tokens, and Persistent Writes

### 5.1 Message Call Sites (Command Address Order)

All calls are `evt_msg_print(0, key, 0, speaker)`. Speaker 0 remains literal
0; nonzero speakers are local actor-key relocations. The full original message
bytes and control tokens must be played from `msg/US/gor_00.txt` (328 key/text
pairs, SHA-256 `bd39efb45105781a403ca5e889ba8e9fb8dbaaaf0b2bd60af6955e38073acdcc`).

| Event | Call Offset (REL) | Message ID | Speaker Key Offset |
|-------|-------------------|------------|-------------------|
| enter | `0x258a8` | `mac_0_014` | 0 |
| enter | `0x258e0` | `mac_0_015` | 0 |
| christine | `0x25f60` | `mac_0_016` | `0x14b28` (Christine) |
| christine | `0x2610c` | `mac_0_017` | `0x14b40` (Kanbu) |
| christine | `0x26124` | `mac_0_018` | `0x14b28` |
| christine | `0x2616c` | `mac_0_019` | `0x14b40` |
| christine | `0x26184` | `mac_0_020` | `0x14b5c` (Zako A) |
| christine | `0x26314` | `mac_0_021` | `0x14b28` |
| beforebattle | `0x2673c` | `mac_0_022` | `0x14b28` |
| beforebattle | `0x26784` | `mac_0_023` | `0x14b40` |
| beforebattle | `0x26888` | `mac_0_024` | `0x14b40` |
| afterbattle | `0x27090` | `mac_0_025` | `0x14b40` |
| afterbattle | `0x270d8` | `mac_0_026` | `0x14b40` |
| afterbattle | `0x27390` | `mac_0_027` | `0x14b40` |
| afterbattle | `0x274b4` | `mac_0_028` | `0x14b28` |
| afterbattle | `0x27514` | `mac_0_029` | `0x14b28` |
| afterbattle | `0x276d4` | `mac_0_030` | `0x14b40` |
| afterbattle | `0x276f4` | `mac_0_031` | `0x14b40` |
| afterbattle | `0x27868` | `mac_0_032` | `0x14b40` |
| afterbattle | `0x280d8` | `mac_0_033` | `0x14b40` |

### 5.2 Story-State Persistent Writes

The scene chain writes the same story operand (`0xf5de0180`) in its main stream:

| Write Site (REL) | Value | Following Binding |
|------------------|-------|-------------------|
| `0x25dbc` | 3 | Spawn `christine_osoware` at `0x25df8` |
| `0x2633c` | 4 | Spawn `beforebattle` at `0x2638c` |
| `0x26b24` | 5 | After reset/NPC movement, child `afterbattle` at `0x26bbc` |
| `0x280f8` | 6 | Later scene cleanup/continuation (needs recovery) |

These writes and re-entry cases show distinct resume points. Several resume
paths pass `local0=1`; `christine_osoware`, `beforebattle`, and `afterbattle`
have leading `IF_EQUAL(local0,1)` setup blocks. Their exact restored
control/camera/actor state must be retained rather than restarting every
narrative action on re-entry.

### 5.3 Control Tokens and Branch Conditions

The message resource uses original control tokens (wait, choice, formatting,
embedded objects) as documented in `MESSAGE_PARSER_RECORDS.md`,
`MESSAGE_INSERT_FORMATTING.md`, `MESSAGE_EMBEDDED_OBJECTS.md`. The event
streams use `IF` predicates (integer/mask/float/string) and `SWITCH`/`CASE`
groups documented in `EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md`,
`EVENT_IF_FLOAT_AND_STRING_PREDICATES.md`, `EVENT_SWITCH_STACK_AND_CASE_SCANS.md`,
`EVENT_GROUPED_MASK_AND_RANGE_CASES.md`. No dialogue is paraphrased or
invented; the original msg/US payloads and control tokens are the authoritative
source.

## Implementation Boundaries and Open Requirements

| Area | Status |
|------|--------|
| Crump REL entry / unit/weapon/event selection | **Recovered** (this doc + linked contracts) |
| Jump ID1 selection and defense availability | **Recovered** (first-attack path uses ID1; defense only on enemy request) |
| Setup/Start/Result/Stop call order | **Recovered** (no AcSetParamAll/AcSetFlag; one StartAC; loop counter L10 separate from shared AC count) |
| Pad update order and private AC sampling | **Recovered** (general→sequence→private→AC main; 12-byte snapshot map) |
| Impact input sample / original timing trace | **OPEN** — isolated original trace still needed |
| Automatic/override/weapon gate writers | **Partially recovered** (weapon gate byte +0x13 documented; Automatic/override writers open) |
| RNG seed ownership and full consumption | **OPEN** — seeding, save preservation, cross-domain interleaving |
| Original US message IDs / speakers / tokens / persistent writes | **Recovered** (20 call sites + 4 story-state writes documented) |
| Complete tutorial event/command gates, stat overrides, win/loss/reload | **OPEN** — requires EVT-01 scheduler closure and original traces |

## Linked Contracts (Recovery Evidence)

- `CHRISTINE_HELPER_ROLES.md` — Crump NPC vs. lecture Goombella kind/init separation
- `STORY_HARBOR_OPENING_BINDINGS.md` — gor.rel event spans, actor placement, 20 message calls, story-state writes
- `FIELD_BATTLE_ENTRY_AND_RETURN.md` — NPC setup index 2, unit configuration, battle start/wait protocol
- `HARBOR_TUTORIAL_CONFIGURATION.md` — Record 2 data chain, unit_boss_kanbu1, kind HP5/FP0
- `UNIT_ENTRY_STATS_AND_SPAWN.md` — Entry publishes unit before init; copies signed kind HP/FP to three fields
- `BATTLE_ORDINARY_ATTACK_BINDINGS.md` — Jump/Hammer stream hashes, SetupAC call sites, ID1 vs ID3
- `BATTLE_ACTION_COMMAND_CALLBACKS.md` — AcSetParamAll/AcSetFlag/StartAC/ResultAC local bodies
- `BATTLE_INPUT_AND_DEFENCE.md` — Defense checker, private AC pad, difficulty, windows, decision order
- `BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md` — General/private pad order, snapshot write map
- `BATTLE_PAD_RESET_AND_RAW_PRESERVATION.md` — WorkInit exact byte preservation map
- `BATTLE_NORMAL_JUMP_EVENT_FLOW.md` — Complete 268-command request graph, per-hit branches
- `BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md` — ResultAC/GetResultAC/StopAC local services
- `HARBOR_BOSS_NORMAL_ATTACK.md` — Boss 95 commands, precheck result 1 requests defense
- `BATTLE_TARGET_EVENT_INTERFACES.md` — Seven target helpers, mode pair selection
- `HARBOR_BOSS_TUTORIAL_EVENTS.md` — Boss init/phase/data hooks, ResultACDefence at `0x656bc`
- `RANDOMNESS.md` — rand/irand algorithms, state address, message rendering consumers
- `MESSAGE_PRINT_EVENTS.md` — Speaker/window ownership consumption
- `EVENT_SYSTEM_LEVEL_AND_SPEED.md` — Event-level masks, speed setter, GP mask getter

## Verification

```sh
python3 tools/verify_lecture_helper_roles.py
python3 tools/rel_event_inventory.py --module gor --symbols artifacts/reference/us_symbols.csv
python3 tools/dol_event_inventory.py marioAttackEvent_NormalJump --symbols artifacts/reference/us_symbols.csv
python3 tools/verify_jump_event_flow.py
python3 tools/verify_behavior_contracts.py
```

ISO reads are read-only. Extracted assets and compiled packs remain outside
source history. Record asset paths, conversion choices and licensing in
`docs/ASSETS.md`.

---
**Documentation Agent ID 1** — lock claimed for AC-01 subtask. This document
answers the five explicit recovery requests in DOCUMENTATION_REQUESTS.md rows
174-231. Remaining open items are marked above; they require EVT-01 scheduler
closure and original input/state/RNG traces, not additional local contract
recovery.