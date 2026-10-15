# Retained whole original command-input controller

US G8ME01 revision0, read-only DOL `BattleCommandInput801229d8/838`.
This is static source evidence and host contract validation, not executed-original
or native battle proof. Source behavior is detailed in [entry and reentry](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md),
[target selection](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md), and
[auxiliary states](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md).

The new `battle_command_input_continuation.hpp/.cpp` owns all30 state entries,
default, both common reentry paths and the whole state11 target handler.
`resumePackedBattleCommandInput` returns engaged0/1 only when that original
invocation has completed. Engaged0 is ordinary not-ready input and permits the
simulation owner to complete its pass; it is not a suspension until confirmation.
A missing original child returns `nullopt` while preserving every acknowledged
operation. The existing synchronous Input/Target helpers remain unchanged and
are never blindly retried by this owner.

## Source captures and native output ownership

Entry captures passed B, actual Unit lookup result U, and wrapped M=B171c;
M0 returns1 after lookup. Original GetCursorPtr(M,14,&SP08) precedes PresentItem,
optional help0, and Restore. Source SP08 is **uninitialized**: the prologue
contains no zero store before80122a14. The cursor owns a stable native optional
word, initially unknown, rather than a fabricated guest stack address or zero.
Only original GetCursorPtr requests expose its pointer; scalar arguments contain
M and type, while the output parameter is represented by that native pointer.
Unsupported cursor lookup preserves the previous output. A known written0
remains an actual original word with its original admitted-memory requirement.

If output remains unknown at a source consumption, this native adapter holds
at that exact boundary without replaying the acknowledged cursor query or
preceding remap. A source invocation that never consumes SP08 may finish while
it is unknown. Missing original stack contents are not inferred. A child may
borrow the request/output only for this retained invocation, never beyond its
completion/reset. B, all service/user/memory callbacks and lifecycle hook identity
are pinned across holds; replaced providers and recursive calls/reset are rejected
before source reads. Actual runtime allocation/lifetime admission remains the
caller's responsibility.

## Whole parent effects and physical controls

All setup state stores, reason-event creation/waits, paired setup flags and real
same-invocation fallthroughs remain parent-owned. Pending-byte reentry reloads
state without clearing progress; ordinary progress reentry clears it. Neither
path inserts a rendering-frame yield or arbitrary loop limit.

State11 captures type and the original target pair before actual sampling
refresh; it captures refreshed signed count and chooses the first matching pair.
Its acknowledged remap store survives a later unknown-output/provider hold.
Weapon mask01000000 permits repeat80000 followed independently by40000, with
original byte wraps, fresh counts and sound802efe98 requests. Cancel trigger200
precedes confirm100 and omits confirmation/audience entirely when accepted.
Confirmation stores29 before target End, retains entry type for cost literals,
freshly reads mode/operation, and still invokes the audience helper afterward.
The audience child is actual function8011f524;80122fb0 is its **callsite**.
The existing retained audience owner is reusable; it is not duplicated here.

State28 keeps cancel200 priority and source confirm100 ordering. State29 selects
the actual attack first, performs the two fresh flag clears and source OffLight
request, then returns1 directly. No synthetic root, turn consumption, damage,
menu choice, story completion or touch-only navigation is introduced. All child
requests identify exact original function and callsite; completed source0 and
unavailable remain distinct, including void functions whose scalar is ignored.

## Validation and integration frontier

The focused test compares new immediate execution against the existing
source-based synchronous Input plus complete Target handler. It checks all30
states/default, original read/write/call traces and final RAM. For every accepted
child in the branch matrix it holds that child twice and verifies exact prefix
retention, stable request/output identity, no extra reads/writes, reentry rejection,
and B/provider/lifecycle replacement rejection. It covers physical cancel/confirm,
simultaneous repeat directions, item costs and retained type despite target-End
mutation, signed refreshed count, pending reentry, wrapped M0, unknown/unconsumed
outputs and unknown-output holds after the target remap. This is source-contract
parity, not an independent PPC execution oracle.

Strict host, GNU++17 ARM and ASan/UBSan checks pass. The new read-only verifier
reuses the audited39 whole bodies,526 instruction anchors and3 data spans, then
requires a one-to-one match of all83 original Input callsites with retained
child boundaries. It introduces no new imported art, constants or dialogue.

The unapplied native proposal threads Act's CommandInput child through this
owner and binds actual Unit/Cursor/Restore/help/event/pad and original complete
light leaves. It composes the sibling retained ActMenu Main/Setup methods.
Missing PresentItem private position ownership, command pose, weapon/target UI,
sampling, payment, attack-selector and sound owners remain actual original
frontiers. Source PresentItem cannot be replaced with item0 or a guessed private
address. The proposal does not activate an active Main cadence or claim visible
normal menus are complete. Root owns runtime integration, build registration,
full validation and commits.

## Integrated native children

Root composes actual retained class and weapon menu controllers and UI work,
atomic original command-pose name copying, and typed PresentItem position/math
outputs. The latter still holds at the missing actual recovery effect. Original
physical-X uses the [typed backend](NATIVE_PHYSICAL_AUDIENCE_INTERRUPT.md).
Weapon tables/update/decision, target UI, sampling, payment, selector, sound,
actual queued class/weapon drawing and whole active Main remain unfinished.
No fabricated selection, timing, sound or visible-readiness acknowledgement
substitutes for those owners.
