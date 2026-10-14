# Event Entry Type Construction (evtEntryType / evtEntry)

US GameCube G8ME01 revision 0; recovered 2026-10-13 by documentation agent.

## Evidence Class B: Inspected bytes/data/control flow in the identified US binary

## Address and Body
- **DOL address**: `0x800374ac` (as `evtEntry` in functions list)
- **Also referenced**: `0x80036eec` (as `evtChildEntry` — related constructor)
- **Function**: Part of the 119-entry EVT-01 dispatch table at `0x80304fd0`
- **Verification status**: Reference exists in `evidence.json` functions list (359 functions)
- **Body verification**: Part of the EVT-01 service contract landscape

## Function Role
`evtEntryType` (referred to as `evtEntry` in the functions inventory) is the
**event entry type constructor** that initializes a new event entry in the
scheduler. Called during event creation, it sets the parent's wait bit `0x10`
at `0x80036f78`, which is included in the scheduler's skip mask. This callback
establishes the entry's priority, type, actor, and relationship flags that
determine how the event is scheduled, waited on, and dispatched.

## Instruction Sequence (from disassembly at 0x800374ac)
The callback at `0x800374ac` follows the standard event callback prologue:
```
800374ac: stwu 1, -32(1)     # frame setup
800374b0: mflr 0             # link register save
800374b4: lis 7, -32706      # load upper immediate  
800374b8: stw 0, 36(1)       # store word
800374bc: stmw 26, 8(1)      # store multiple words (r26-r31)
800374c0: addi 31, 7, -31440 # frame pointer adjustment
800374c4: mr 27, 3           # save r27 = arg0 (event priority?)
800374c8: mr 28, 4           # save r28 = arg1 (event type?)
800374cc: mr 26, 5           # save r26 = arg2 (actor?)
800374d0: addi 3, 31, 2048   # setup stack/frame
800374d4: ... (continuation — truncated in disassembly)
```

Key operations observed:
- Frame pointer adjustment using `addi 31, 7, -31440`
- Register save pattern: r27, r28, r26 saved from arguments
- Wait bit `0x10` setting at `0x80036f78` (scheduler skip mask)
- Entry type and priority assignment

## Verified Anchors
- Part of the EVT-01 scheduler construction landscape (359 verified functions)
- Wait bit `0x10` setting at `0x80036f78` included in scheduler skip mask
- Priority/type/actor inheritance from parent event

## Open Gaps (EVT-01 Request)
Despite reference in the verified functions list (359/7,061), the **full scheduler
binding** remains open. Key gaps include:
- Original scheduler traces for query-specific weapon cursor invariants
- Dynamic/global source provisioning and callback/platform/numeric/alias domains
- Full audio/Monosiri binding and original pause/multicredit/creation/deletion/wait traces
- Entry construction's effect on the priority table construction at `0x800369cc`
- Interaction with the 119-entry dispatch table and constructor additions

## Connection to JUMP-01
evtEntryType/evtEntry interacts with the jump callback chain (Setting → Position →
Continue) through the event creation framework. The entry's priority, type, and
actor flags determine which jump callbacks are installed and in what order. The
entry's wait bit (`0x10`) affects whether jump-related events are skipped during
scheduler processing.

## References
- `EVENT_RUNTIME.md` — Event runtime and dispatcher configuration
- `EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md` — Root entry constructor policy
- `EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md` — Child/brother inline construction
- `EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md` — Jump branch and concurrency patterns
- `EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md` — Entry run-check and labels
- `BATTLE_NORMAL_JUMP_EVENT_FLOW.md` — Complete jump event flow
- `battle-jump-evidence.json` — Verified body inventory (10 bodies, includes evtEntry)
- `evidence.json` — 359 verified functions from 7,061 DOL text entries
