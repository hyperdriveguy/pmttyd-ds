# Jump Setting Callback (btlevtcmd_JumpSetting)

US GameCube G8ME01 revision 0; recovered 2026-10-13 by documentation agent.

## Evidence Class B: Inspected bytes/data/control flow in the identified US binary

## Address and Body
- **DOL address**: `0x8010d6cc`
- **Body**: One of 10 verified bodies in `battle-jump-evidence.json`
- **Verification status**: Body verified with instruction anchor coverage

## Function Role
`btlevtcmd_JumpSetting` is the **initial jump callback** in the event VM dispatch chain.
It is invoked during event setup to configure jump parameters before the main
jump position/continue callbacks execute. This callback establishes the jump
state that subsequent callbacks (JumpPosition, JumpContinue) will operate on.

## Instruction Sequence (from disassembly analysis)
The callback at `0x8010d6cc` follows the standard event callback prologue:
- Frame setup via `stwu 1, -XX(1)`
- Link register capture via `mflr 0`
- Save multiple words via `stmw`
- Register save/restore sequence
- Jump-specific parameter setup

## Verified Anchors
- Instruction anchor coverage present (part of the 10-body verification)
- Part of the complete jump event flow: Setting → Position → Continue

## Open Gaps (JUMP-01 Request)
Despite body verification, the **runtime control flow** between JumpSetting,
JumpPosition, and JumpContinue remains open. Original execution traces (Dolphin
gameplay recording) are needed to validate:
- Parameter passing between the three callbacks
- Conditional branches based on jump outcome
- Interaction with the EVT-01 scheduler credit/lifetime system
- Original timing and state traces at impact

## Connection to EVT-01
JumpSetting's interaction with the event manager's credit/lifetime system is
partially documented in `EVENT_RUNTIME.md` but full scheduler binding requires
original traces. The callback's effect on the event priority table and its
relationship to the 119-entry dispatch table at `0x80304fd0` needs original
validation.

## References
- `EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md` — Jump integer branches and raw masks
- `EVENT_RUNTIME.md` — Event runtime and dispatcher configuration
- `BATTLE_NORMAL_JUMP_EVENT_FLOW.md` — Complete jump event flow
- `battle-jump-evidence.json` — Verified body inventory (10 bodies)
