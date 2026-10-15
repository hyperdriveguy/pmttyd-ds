# Retained original class menu and UI allocation

`battle_act_menu_continuation` owns the complete original local menu bodies and
their source UI allocation children. Static evidence is US G8ME01 disc0 revision0,
read through the original ISO in read-only mode. It is not a Dolphin, hardware,
pixel-parity or complete native battle observation.

| Body | Address / size | Native entry |
| --- | --- | --- |
| ActMenuMain | `8012230c / 488` | `resumePackedBattleActMenuMain` |
| ActMenuSetup | `80122794 / cc` | `resumePackedBattleActMenuSetup` |
| ActMenuEnd | `801220d4 / 38` | `resumePackedBattleActMenuEnd` |
| ChangePositionEnable | `8012214c / 1c0` | `resumePackedBattleChangePosition` |
| `_cursor_init` | `80122860 / 10` | Exact two stores inline in Setup |
| ActSelect Main | `80116cd8 / 4` | Original `Display` child, verified `blr` |
| ActSelect Init | `80116cdc / 1a4` | `resumePackedBattleActUiInit` |
| ActSelect End | `80116c78 / 40` | `resumePackedBattleActUiEnd` |

The existing [read-only verifier](../../../tools/verify_act_menu_control.py)
pins all complete local bodies and linked services, the complete type dispatch
and original sound keys. An additional complete-body/callsite record lives in
`artifacts/battle-act-menu-source-evidence.json`. See the original
[selection](ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md),
[navigation](ACT_MENU_MAIN_NAVIGATION_AND_SERVICE_ORDER.md),
[Setup/End/position](ACT_MENU_SETUP_END_AND_POSITION_GATE.md), and
[UI allocation](ACT_SELECT_UI_ALLOCATION_AND_NATIVE_BINDING.md) contracts.

## Menu parent ownership

`BattleActMenuContinuationCursor` owns one Main, Setup, End or position invocation.
Setup and End return native completion of original routines with no defined
caller-consumed scalar. Main and position return `optional<int32_t>`: engaged
zero/one are completed original results; `nullopt` is a provider hold. Main
retains its original local result even when menu mutations occur on a zero
result path. It never infers completion from a changed menu state.

Every external child is a typed original `BattleActService` request, with the
actual callsite, captured scalar arguments and count. Main's complete local End
and position callees run inside this owner; they are not opaque callback handlers.
The nine Setup cursor-init leaves run their exact two word stores, preserving
their count words and skipped cursor slots. UI Init/End remain real nested
children, with the dedicated retained UI cursor described below. Display remains
the original four-byte no-op service and never certifies rendered presentation.

The battle pointer, entry kind, Setup flag, all memory callbacks, byte reader,
lifecycle observer and request user/function must remain fixed while held.
Recursive resume and reset during a callback are refused without source effects.
Reset otherwise abandons the cursor without undoing writes; it does not authorize
starting the source body again. Actual memory, native service context and child
owners must outlive the invocation. Identical callback addresses alone do not
establish guest allocation lifetime.

Each child returning `nullopt` must retain its own partial execution and resume
the same request. The parent stores argument reads once and never repeats a
successful UI close, sound, table build, camera request, audience check or pad
query. Native callbacks must not wrap a partially mutating synchronous helper
and simply restart it after a hold.

## Private cursor output and fresh reads

The original Main and Setup SP08 output is initially unwritten. The native
request supplies a stable cursor-owned `optional<uint32_t>* privateCursorOutput`
only for original GetCursorPtr. It does not fabricate a guest stack address.
The actual child writes its real selected cursor; a written zero remains zero.
An unwritten output stays unknown and holds at its first source consumption.
The getter is not repeated after it completed without writing. Source paths
which do not consume that output, including early successful position change or
key-admission failure, can complete without it.

The borrowed output pointer remains valid until this invocation completes or is
explicitly abandoned. Providers cannot retain it beyond that lifetime. Later
source SP08 loads use the current native cell, preserving post-sound cursor
reloads and source alias behavior; earlier child operands stay captured through
their waits. No previous weapon cursor or default index fills an unknown slot.

Class validation retains its row/type across weapon-table and target-count
children but reloads the loop count at each test. Timer stores precede retained
camera calls; fresh pending-confirmation gates follow them. Enabled confirmation
reloads the selected type after sound. Disabled confirmation rereads the reason
after End. In-range directional index stores precede sound; wrapping stores use
a fresh cursor after sound. Both announcement-independent menu output and
display order remain original.

## UI child allocation and release

`BattleActUiContinuationCursor` separately retains actual Init/End arguments,
old work pointer, provider identities and source allocator position. This fixes
the synchronous constructor's real replay hazard: after old release completes,
an unavailable allocation resumes at `__memAlloc(124)` rather than freeing the
old UI again. End clears the owner pointer only after actual release acknowledges.
Init leaves the prior pointer untouched between release and allocation, matching
the source. Its allocation publication precedes the fresh W capture.

The two actual external calls are `__memFree(800f8908)` and
`__memAlloc(800f8934)`. A false release or absent allocation result holds that
same child; each provider owns its partial work. An engaged allocation result
is the original word, including zero: the source contains no null-allocation
guard. Native allocation/memory admission must establish the original valid
work domain rather than fabricate a successful pointer or add a source guard.

After allocation there are no waiting external children. The owner performs the
complete original selected-first or next-first array initialization, fresh
count/selection/GP-rate reads and signed division/narrowing in order. It preserves
untouched work words. No blanket clear or guessed animation duration is added.

## Original physical controls and audience admission

The parent preserves forward repeat `80000` before reverse `40000`, confirmation
trigger `100`, both independent position trigger `800` queries, and original
sound keys `2520`, `802efe88` and `802efef4`. It introduces no touch-only control,
replacement menu layout or generated sound.

Only the original no-direction path reaches audience helper `8011f524` at
callsite `80122710`. This is the existing original X-triggered first-eligible
selection, with its original blockers and type11/Input29 publication. The new
`resumeAdmittedBattleActMenuAudience` wraps
`resumePackedBattleMenuAudienceCheck`; it does not duplicate its query/target/
publication body or change good/bad holder eligibility.

The older helper pins only a subset of its services and has no reentry guard.
The wrapper pins **all** actual inner callback identities, user, memory functions,
byte reader, lifecycle observer and mapped private-item output through ItemOn
and End holds. Replacement services or recursive entry/reset are refused before
effects. Its nonoptional child interfaces require the complete service set to
be admitted before initial entry; this is an explicit native admission policy,
not an extra original predicate. A persistent runtime context and real output
storage must outlive the wrapper. Harbor's stack Context remains synchronous and
is not made into a retained native owner by this change.

Optional lower-screen audience taps remain a separate displayed-frame/source
projection/incarnation admission path. This module publishes no rectangles,
audience population, visibility, throwing state or synthetic X. The original
physical alternative remains required.

## Tests and integration proposal

Focused tests compare full ordered guest read/store/provider traces and final
state against existing synchronous Main/Setup/End/position/UI reference bodies.
Only the reference's explicit private-output scratch is normalized to the native
cell, and the recovered position body is expanded through its actual children.
Each external child is held twice in turn before completing the trace.
Matrices cover all 14 type routes/defaults, type2 branches, target failure/reason
precedence, dynamic count changes, signed timers/counts/indices, directional
priority, repeated position queries, disabled reasons, fresh cursor replacement,
and all Setup reset gates. UI cases cover entry/resume arrays, allocator/free
holds, fresh W after publication and count/GP mutations during initialization.
These are explicit service fixtures, not story population or rendered gameplay.

Audience wrapper tests hold both ItemOn and End, swap user/functions/output/
lifecycle, attempt recursive entry/reset, and verify no repeated trigger, Mario
query, item selection or target publication. Strict host execution, strict ARM
production/test syntax, and Address/UndefinedBehaviorSanitizer checks pass (leak
detection disabled). Existing read-only original source verification also passes.

`artifacts/battle-act-menu-native-replacements.json` is an unapplied journal
proposal for actual Trio runtime cursors and stable services. It binds real
registry/status/cursor/pad/camera/key-admission/no-op Display leaves and retained
UI Init/End with actual map allocation/free ownership. Tokens are minted after
real allocation and retired before real free; unknown release owners hold.
The parent must still prove original allocation capacity/lifetime and integrate
source draw publication before claiming visible readiness.

ClassTable, WeaponTable, TargetCount and retained original sound remain explicit
missing children in the proposed native factory. Audience stays unavailable
until root binds actual mapped private-item output and immutable genuine query/
target/End providers; no scratch address is invented. The coordinated retained
CommandInput owner can delegate to `resumeOriginalActMenuSetup/Main/End` after
root applies and validates the journals. No normal battle activation or complete
Main scheduler is claimed by this isolated batch.

## Integrated native boundary

The root applies the exact journal and composes retained ActMenu, CommandInput,
ClassTable and UI allocation/free against the actual Trio VM. ClassTable now
uses the [original US message owner](NATIVE_CLASS_MENU_MESSAGES.md), acquired
once at fresh VM entry. All three original X callers use the
[typed physical-X backend](NATIVE_PHYSICAL_AUDIENCE_INTERRUPT.md), with a native
private index instead of mapped output scratch, actual GetItemOn/SetTarget and
retained End. The admission wrapper above remains available for callers that
actually own its guest output; Trio does not manufacture that storage.
WeaponTable, TargetCount and genuine sound remain missing. Actual source queued
drawing and whole active Main scheduling remain separate unfinished owners.
