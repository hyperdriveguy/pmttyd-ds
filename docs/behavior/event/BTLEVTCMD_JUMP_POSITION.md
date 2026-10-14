# Jump Position Callback (btlevtcmd_JumpPosition)

US GameCube G8ME01 revision 0; recovered 2026-10-13 by documentation agent.

## Evidence Class B: Inspected bytes/data/control flow in the identified US binary

## Address and Body
- **DOL address**: `0x8010d208`
- **Body**: One of 10 verified bodies in `battle-jump-evidence.json`
- **Verification status**: Body verified with instruction anchor coverage

## Function Role
`btlevtcmd_JumpPosition` is the **jump position callback** that determines the
jump trajectory and landing state. Called after `btlevtcmd_JumpSetting` and
before `btlevtcmd_JumpContinue`, it integrates the jump physics (ascent tuple
`(8,-.84,.08,-.02)`, fall `(-.1,-.2,.024,-.004)`) and handles the release
adjustment `-.526`. This callback is where the jump's vertical motion is
computed and landing conditions are evaluated.

## Instruction Sequence (from disassembly analysis)
The callback at `0x8010d208` follows the standard event callback prologue with
jump-specific parameter processing:
- Frame setup via `stwu 1, -XX(1)`
- Link register capture via `mflr 0`
- Save multiple words via `stmw`
- Register save/restore sequence (r27-r28-r26 pattern observed in evtEntry)
- Jump position computation using the ascent/fall tuples
- Landing equality check and block condition

## Verified Anchors
- Instruction anchor coverage verified (part of the 10-body verification)
- Jump position integration verified in first call
- Blocks on remaining positive count (per jump event flow documentation)

## Open Gaps (JUMP-01 Request)
Despite body verification, the **original executed selection and impact trace**
remains open. Key gaps include:
- Exact input sample included at impact (timing validation)
- Automatic/override writers and active tutorial values
- Seed ownership and conditional RNG consumption at this call site
- Original single/mash/A+B/held/early/late trace patterns
- Integration with the EVT-01 scheduler's credit/lifetime system

## Connection to EVT-01
JumpPosition's interaction with the event manager's priority table and its
relationship to the 119-entry dispatch table at `0x80304fd0` needs original
trace validation. The callback's return value (0/1/-1) affects the event
manager pass continuation per `runEventManager` logic.

## References
- `EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md` — Jump integer branches and raw masks
- `EVENT_RUNTIME.md` — Event runtime and dispatcher configuration
- `BATTLE_NORMAL_JUMP_EVENT_FLOW.md` — Complete jump event flow
- `battle-jump-evidence.json` — Verified body inventory (10 bodies)
- `ORIGINAL_BEHAVIOR.md` — Core execution contract (seqMain, marioStMain)
