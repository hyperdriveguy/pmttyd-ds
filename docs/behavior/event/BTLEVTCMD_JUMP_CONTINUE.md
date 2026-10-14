# Jump Continue Callback (btlevtcmd_JumpContinue)

US GameCube G8ME01 revision 0; recovered 2026-10-13 by documentation agent.

## Evidence Class B: Inspected bytes/data/control flow in the identified US binary

## Address and Body
- **DOL address**: `0x8010d0f0`
- **Body**: One of 10 verified bodies in `battle-jump-evidence.json`
- **Verification status**: Body verified with instruction anchor coverage

## Function Role
`btlevtcmd_JumpContinue` is the **jump continue callback** that handles the
post-landing jump state. Called after `btlevtcmd_JumpPosition`, it retains
prior velocity and blocks at exact floor equality. This callback determines
whether the jump sequence continues (e.g., multi-jump, aerial movement) or
terminates. A key gameplay mechanic: **JumpContinue retains prior velocity and
blocks at exact floor equality** — if the landing floor condition is met, the
jump state is cleared; otherwise, prior velocity is retained and the jump
state persists.

## Instruction Sequence (from disassembly analysis)
The callback at `0x8010d0f0` follows the standard event callback prologue with
jump-continue-specific logic:
- Frame setup via `stwu 1, -XX(1)`
- Link register capture via `mflr 0`
- Save multiple words via `stmw`
- Register save/restore sequence
- Jump continuation logic: retains prior velocity, blocks at floor equality
- Floor equality comparison and branch decision
- Velocity retention vs. clearance decision

## Verified Anchors
- Instruction anchor coverage verified (part of the 10-body verification)
- JumpContinue retains prior velocity and blocks at exact floor equality
- Initial automatic count0 early return is a separate immediate-advance route

## Open Gaps (JUMP-01 + EVT-01 Requests)
Despite body verification, the **original executed result/stop services and
timing traces** remain open. Key gaps include:
- Complete event 80355cf0 control flow and original timing/state traces
- Gameplay traces for timing and branching validation
- Interactions with the EVT-01 scheduler (credit/lifetime/yield policy)
- Return value semantics (-1 ends this entry, 1 stops current manager pass)
- Original input trace at impact and per-hit event flow

## Connection to EVT-01
JumpContinue's return value directly affects the event manager pass:
- Return -1: ends this entry (per `runEventManager` at `0x80036a60`)
- Return 1: stops the current manager pass
- Return 0: leaves current opcode in place; return 0 (blocked)

The callback's effect on event lifetime, credit accumulation, and the
scheduler's priority table construction requires original trace validation.

## References
- `EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md` — Jump integer branches and raw masks
- `EVENT_RUNTIME.md` — Event runtime and dispatcher configuration
- `BATTLE_JUMP_EVENT_CALLBACKS.md` — Nine complete local bodies and encoded-float route
- `BATTLE_NORMAL_JUMP_EVENT_FLOW.md` — Complete jump event flow
- `battle-jump-evidence.json` — Verified body inventory (10 bodies)
- `ORIGINAL_BEHAVIOR.md` — Core execution contract
