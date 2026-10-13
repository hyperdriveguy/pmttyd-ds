# EVT-01 service contract and remaining bindings

US GameCube G8ME01 disc0 revision0; consolidated2026-10-13 for
[EVT-01](../../requests/EVT-01.md), priority0 in the
[inbox](../../DOCUMENTATION_REQUESTS.md#implementation-priorities).
This answers the requested local scheduler/constructor/wait service contract
by composing the already recovered bodies. Read-only ISO verifiers were rerun;
this is static evidence, with no new original execution observation.

## Scheduler, interpreter and continuation requirements

| Requested portion | Established behavior | Owning contract / verifier |
| --- | --- | --- |
| Context and eligibility | Capture field/battle context from GP14; check active bit, scheduled/live ID equality, then flags0x92. Skipped entries receive neither lifetime nor credit. | [Manager](EVENT_MANAGER_COMPLETE_LOOP.md); `tools/verify_event_manager_loop.py` |
| Lifetime source and rounding | Capture GP38/3c raw timestamp; wrapping signed64 delta, negative clamp, integer clock-rate division and >500ms replacement. Store captured timestamp even after substitution. Lifetime receives retained raw tick delta once per eligible visit. | [Manager](EVENT_MANAGER_COMPLETE_LOOP.md), [timestamp producer](../platform/EVENT_RETRACE_TIME_PUBLICATION.md) |
| Credit and return order | Binary32 add speed to credit; subtract binary32 one BEFORE each interpreter call. Dispatcher1 aborts whole pass; -1 ends this entry; other results retest credit without rechecking ID/flags. | [Manager](EVENT_MANAGER_COMPLETE_LOOP.md) |
| Current-table additions | Rebuild priority table at pass start; reload live count at each outer iteration. Constructor additions append unsorted slot/ID pairs; do not freeze count or resort midway. | [Root append](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md), [descendant append](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) |
| Dispatch and first-call state | Fetch clears byte+d; USER_FUNC resolves/caches callback once, advances arguments once, calls with1 then0 on retained polls. Handler0 retains,1 completes/yields,2 continues immediately, negative aborts,255 terminates. | [Dispatch/waits](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| Frame wait | First invocation resolves/caches count and decrements it immediately. Exact0 advances immediately; decrement-to0 completes/yields. No <=0 clamp; later polls do not resolve again. | [Dispatch/waits](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| Millisecond wait | Capture lifetime before resolving first operand; retain start high/low but compute elapsed from wrapping LOW-word subtraction, sign extension and unsigned64 division. Zero duration bypasses clock/division with handler2. | [Dispatch/waits](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| Remaining interpreter cases | All119 dispatch targets have linked scoped local contracts; this supersedes the generic missing-opcode claim. Callback implementations remain separate services. | [Opcode map](EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md); `tools/verify_event_dispatch_tail.py` and opcode-family verifiers |

Interpreter invocations are not physical input samples. Several credits can
poll the same cached callback against one prior pad update; lifetime does not
advance again between those invocations. The documented
[outer loop](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) and
[battle sampling order](../input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) supply
local service ordering, not a measured attack impact or universal60Hz cadence.

## Initial state and constructor policy

All constructors clear the new1b0-byte entry and initialize credit/lifetime to0.
Each copies CURRENT global speed, rather than an immutable1 or parent speed.

| Route | Priority / type / owner | Append during active manager |
| --- | --- | --- |
| Root evtEntry | Low8 supplied priority; typeef; actor-1; no relationships | Only when supplied flags include0x20 |
| Root evtEntryType | Low8 supplied priority/type; actor-1; no relationships | Same0x20 gate |
| Child | Low8(parent priority+1); inherited type/actor; parent waits on child | Unconditional on supplied0x20 |
| Brother | Owner priority/type/actor; brother link, no parent wait | Unconditional on supplied0x20 |
| Inline107 | Typed root at owner priority/type with flags60; then post-constructor owner/local/metadata copies | Root0x20 gate passes for flags60 |
| Attached brother110 | Brother flags60; then specified owner-field recopy including150 | Descendant append policy |

Constructor run-check reads REQUESTED system-level enum8041e940, not the GP18
mask. Levels1/2/3/4 request StopAll masks1/2/10/ef after append; other levels do
nothing. It acts on the selected context's events, not solely its argument.
See [labels/run-check](EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md).
Root/child perform zero-ID correction after run-check; brother corrects before
run-check. Preserve those differences and the separate
[ID-output wrappers](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md).

Root/descendant allocation has no local full-capacity/null fallback. Pointer,
capacity, relationship, label and stream domains remain requirements. Native
checked failures must stay labelled as adapter policy.

## Other EVT-01 deliveries now reflected in the checklist

[HP/unit-work summary](../battle/BATTLE_UNIT_QUERY_EVENT_LEAVES.md) now reflects
the independently reinspected [capture refinement](../battle/BATTLE_UNIT_HP_WORK_CAPTURE_ORDER.md).
HP retains battle before resolution and skips lookup/output on ID-1. Work
resolves operands and captures its raw output before fresh battle lookup.
The two neighboring status/data leaves have their distinct orders recorded too.

[NormalJump root registration](../battle/NORMAL_JUMP_ROOT_REGISTRATION.md) and
[callback census](../battle/NORMAL_JUMP_CALLBACK_REGISTRY.md) answer the conditional
default-weapon→actor event→state6 root path, post-constructor actor/ID binding,
and156 direct USER_FUNC sites across three streams (72 distinct addresses).
They do not prove all menu/weapon/global writers or transitive callback sources.

The [cursor/type contract](../battle/WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md),
[ActMenu publication](../battle/ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md),
[audience11](../battle/AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md) and
[gift12](../battle/GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md) already establish
local unsupported-type writers and concrete same-invocation reuse in Input
state11. BattleGetSelectWeapon's separate stack slot is uninitialized; that
reuse cannot be imported as its fallback. Query-specific reachability remains open.

## Validation and remaining acceptance

Rerun `verify_event_manager_loop.py`, `verify_event_root_constructors.py`,
`verify_event_descendant_constructors.py`, `verify_event_dispatch_tail.py`,
`verify_event_run_and_controls.py`, `verify_weapon_menu_domains.py`,
`verify_act_menu_control.py`, `verify_menu_interruptions.py` and
`verify_jump_source_registry.py` under `tools/`. Each reads the original ISO;
inspection outputs stay outside history. Fingerprint checks establish source
identity, while instruction inspection establishes the scoped deductions above.

EVT-01 remains OPEN for query-specific menu invariants/stack residue, dynamic
and transitive source provisioning, callback/platform/numeric/alias domains,
global writer ownership and original pause/multicredit/creation/deletion/wait
traces. Compare an original ordinary Jump and tutorial encounter before claiming
executed equivalence. No fixed impact tick, replacement dialogue or native
harness timing is part of this service contract.
