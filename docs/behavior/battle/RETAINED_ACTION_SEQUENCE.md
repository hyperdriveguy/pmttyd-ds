# Retained original action sequence

`battle_act_continuation` implements the complete local US G8ME01 revision0
`btlseqAct` body, `0x8011b2dc`, size `0x8ac`. Its 555 instructions, all 84 direct
call sites, the 15-entry dispatch table at `0x80363c50`, and the binary32 camera
constant at `0x80422584` are pinned by
[`verify_battle_act_continuation.py`](../../../tools/verify_battle_act_continuation.py).
The original body SHA256 is
`91a2c4da610cea59151def6dcefe50b6886eed6a261a475edb2ec68207601de4`.
Inspection opens the original ISO read-only. No executed-original comparison,
Dolphin timing or hardware result is claimed.

The complete branch map is in [action controller](BATTLE_ACTION_CONTROLLER.md).
The new owner preserves every local branch and source read/store, including
the otherwise redundant battle-flags read in state9's always-true local test.
It uses the existing exact sequence word helpers at 20 callsites; the other 64
callsites each have an explicit retained child boundary, checked by the verifier.
It does not dispatch whole cases through a
legacy handler.

## Native invocation and retained child contract

`resumePackedBattleAct(B,services,cursor)` reports native completion of one
original invocation. `false` means a child is unavailable; it does not mean an
original scalar false or a requested render-frame delay. The cursor retains the
original battle pointer, source control position, unit/event pointers, selected
script, event-ID operands and announcement aggregate. It does not redispatch
from the currently stored channel6 after a hold.

`BattleActSequenceRequest` identifies one `BattleActChild`, its original function, exact
callsite and up to four original scalar arguments. There are no fabricated guest
stack addresses or output pointers. `BattleActOutput.value` carries the original
raw scalar, guest pointer or event address; `.real` carries the camera-speed
floating return. An engaged zero is a completed original child, including a null
event allocation or a command-input result of zero. `nullopt` alone holds.

The service callback must retain partial work inside a held child. Repeated
requests have identical captured operands. The parent never repeats successful
RNG, event creation/deletion, announcement, reserve consumption or other calls.
Guest reads before a call occur once; reads after that call stay fresh at the
actual source point. A provider must not re-run a partially mutating synchronous
callee from its entry when returning `nullopt`.

The same battle, service user/function, all guest memory callbacks, actor byte
reader and audience lifecycle observer must remain bound for the entire hold.
Reentry and reset during a child invocation are refused without effects.
Reset otherwise abandons the cursor without rollback; it does not authorize
replaying committed source mutations. Actual guest allocations and child owners
must outlive the invocation. Callback identity does not establish allocation
lifetime or permit recycling an address beneath a cursor.

## Exact source children

Argument order below matches the original source registers. Empty argument lists
are intentional: those children obtain their own globals.

| Child | Original function | Arguments / result |
| --- | --- | --- |
| Unit | `800f88cc` | `(B, captured actor-ID word B420)` → guest U |
| Mario | `800f87a8` | `(B)` → guest Mario U |
| NiceAsync | `80144d44` | `(1)`; source ignores return |
| AudienceInit | `801a82d4` | `()`; source ignores return |
| Status | `80127814` | `(U,id)` → original scalar |
| CommandInit | `80125380` | `(B)`; source ignores return |
| CameraSpeed | `8012c0e8` | `()` → `.real` |
| CommandInput | `801229d8` | `(B)` → original progress scalar |
| UnitData | `8012733c` | `(U,key)` → script word |
| WindowOff / WindowOn | `800f75cc` / `800f7624` | `()`; original system window children |
| CreateEvent | `800374ac` | `(script,10,0)` → actual guest EventEntry address |
| Random | `80005da0` | `(100)` → original signed comparison operand |
| DeleteEvent / EventExists | `80036664` / `80036604` | `(captured event ID)` |
| WaitEvents | `8011ace8` | `(B)` → original ready scalar |
| AdjustMessages | `801958bc` | `(B)`; source ignores return |
| Messages / Announcements | `801956f8` / `80195af8` | `(B)` → full word, ORed after both calls |
| SetStatus | `80127bac` | `(U,16,1,-99)`; source ignores return |
| CheckStatusFlag / ClearStatusFlag | `801277f8` / `801277d8` | `(U,02000000)` |
| Belong | `80128144` | `(U)` → low byte, sign-extended by caller |
| PhaseEnd | `8011bb88` | `()` → original global-battle predicate |
| ConsumeItems | `800f74b4` | `()`; complete original reserve consumer |
| AudiencePerAct | `801a7e94` | `()`; source ignores return |
| AudienceReaction | `801a7e30` | `()` → low byte only |
| Concluded | `8011aa34` | `(B)` → original predicate and group stores |
| BreakSlotReaction | `80203394` | `()` → low byte only |

The completed parent preserves state0's distinct status1/2/9/27 gates, kind228's
fresh Mario attribute query, the signed byte routing and state1's unordered float
comparison. State6 retains the second independent actor lookup after WindowOff,
the zero-gravity priority, null-guarded flag16 versus unguarded flag2/default
scripts, Electric's retained signed-negative draw branch, Confuse's draw and
flag publication, post-allocation owner/ID stores and wait-event deletion.

States7/8 preserve two separate barriers, Adjust between them, and both message
pumps even when the first result is nonzero. State9 performs exact source state
normalization, decrement/bypass/forced-zero/exhaustion gates, turn-byte preservation
and optional marker96; it does not call the older whole count-consumption helper.
Its reservation, flags and PerAct tail runs for a missing actor too. States10/11
preserve low-byte reaction checks, conclusion skipping Break Slot and the later
second announcement drain. States12..14 retain the separate post-event owner,
creation failure behavior, wait deletion, explicit `U2cc` event poll and final
move-channel5 increment. They never infer readiness from a missing root handle.

## Original physical audience input

Act has no direct X or touch branch. Its state2 reaches original CommandInput
through callsite `0x8011b500` → `0x801229d8`. The complete original caller bodies
are also pinned by the verifier. Three actual menu sites reach
`BattleCommandAttackAudienceCheck`, function `0x8011f524`, size `0xcc`:

| Original caller | Actual `bl` site | Source placement |
| --- | --- | --- |
| ActMenuMain `8012230c/488` | `80122710` | After no class-direction movement |
| WeaponSelectMenuMain `80121d0c/35c` | `80122018` | No-input path before caller's close/pending publication |
| CommandInput `801229d8/838`, target state11 | `80122fb0` | After confirm handling, including already-stored state29 |

`80122fb0` is the third callsite, not the audience helper's entry address.
The helper captures actual global B before `BattlePadCheckTrigger(0400)`, applies
the original battle/Mario blockers, then calls original first-eligible GetItemOn,
SetTarget and ActMenuEnd before publishing previous type, type11 and Input29.
See [interruption writer](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md) and
[native target/holder ownership](AUDIENCE_TARGET_SELECTION_AND_NATIVE_INTERRUPTION.md).
Physical X retains that exact original selection, including possible good-item
holders; it does not acquire a new harmful-only filter.

No optional tap site, audience population, eligibility, scene visibility or
rectangle is created by this owner. Optional targeted lower-screen taps still
require actual completed source projection, displayed frame/allocation ownership,
real actor/item incarnation and current harmful-holder admission. They must keep
the physical alternative. No synthetic X event substitutes for targeted source
selection.

## Validation and native integration frontier

Focused tests use an independent immediate source transcription to compare full
parent read/store/child traces and final guest memory. All 15 direct states and
invalid states are exercised. Matrices cover kind/status routing, priority flags,
camera equality/NaN, command false, all event-selection routes, null allocation,
Electric/Confuse thresholds, announcement/barrier/conclusion/reaction results,
signed count/state variants, absent actors and post-event failures. Every actual
child in each trace is held twice, then the complete suffix is compared; source
zero results stay complete. These are explicit guest fixtures, not fabricated
story audience or playable runtime observations.

Mutation cases distinguish fresh second actor lookup from an already-held actor
operand, capture script/wait ID through their respective waits, retain fresh
post-create event/unit IDs and retain fresh status flags after SetStatus. Null
provider, changed owner/lifecycle observer and recursive entry checks reject
without prefix replay. Strict host compile/execution and strict ARM production/
test syntax pass. Static original source verification passes independently.

`artifacts/battle-act-native-replacements.json` is a proposed exact journal,
not an applied shared change. It adds actual per-runtime Act/barrier cursors and
bindings for existing registry/status/data/RNG/event/conclusion/barrier/consumer/
phase-end leaves. Real nonzero reserve items hold at the missing inventory owner.
Nice, audience ActInit/PerAct/reaction, command init/input, camera speed, system
window, SetStatus, message adjustment/pumps and Break Slot remain explicit missing
children in that proposal. In particular, the current synchronous CommandInput
parent cannot be blindly retried after a partially completed child; its retained
owner is a separate coordinated task. No legacy Act or attack handoff is called
as an opaque substitute.

Native binding, source Main/Turn/Phase/Move cadence, actual VM scheduling while
held, source render ownership and normal battle activation remain parent work.
This module completes the local Act source parent without claiming those owners
or a complete battle/game.


## Native Trio composition

The native Trio provider composes actual VM unit/status/data/flag/lookup,
RNG, source event creation/deletion/readiness, conclusion, phase-end and retained
event/reservation barrier owners. CommandInit `80125380/18` is independently
pinned as the complete six-instruction leaf: write0 to supplied B171c, then
freshly read/OR80/store supplied Bef4. It performs no menu allocation or GPU
work. Native binding preserves those two source stores.

Complete Phase/Move calls this retained Act; the actual Turn controller calls
complete Phase. Missing Nice, audience ActInit/PerAct/reaction, camera-speed,
window/message, status-infliction, command input and BreakSlot providers hold
at their original calls. A nonzero reserved item still needs genuine inventory
removal. Active source Main scheduling and menu/effect rendering remain separate
admissions; these providers do not acknowledge the legacy action graph.
