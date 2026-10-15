# Retained battle Main, stage and field-sequence ownership

Read [battle controllers](battle-control.md) for immediate sequence logic,
[stage storage](battle-stage.md) for record layouts and
[damage order](battle-damage-order.md) for the synchronous hit boundary. A retained
parent records completed children, not their internal progress. A held child must
retain its own suffix; no parent acknowledgement supplies missing source work.

## Main and sequence dispatch

Main cursors require fixed provider, memory and audience-lifecycle bindings.
RAII invocation guards reject recursive run/reset, including reset from inside
an admitted child. Successful completion releases cursor state, so the next
simulation invocation starts afresh. Rendering or wall-clock time does not create
a second manager visit.

The lower wrapper captures outer B, then upper Main independently captures inner
B. Null outer B completes without upper children. Otherwise eleven children run
in source order using retained inner context: unit-broken, pad, sequence, AC,
stage objects, icons, stage work, AC help, audience, break-slot and display.
An unavailable suffix resumes at its index without replaying completed children.
The captured B is context, not an invented argument to every source ABI.

Only after all children does upper Main compute its result. Channel 0 equal to 4
returns engaged zero. The wrapper tests optional presence, so zero is completed
work; it ORs outer B's end bit 0x80000000, not inner B's. This is not field return.
GP flags 0x3000 plus manager channel 2 can request manager 3/end channel 7 while
still returning 1 until an actual later channel 4 is observed.

Sequence dispatch captures B and bank once before preparing at most two requests:

| Bank | Fixed local route |
| --- | --- |
| 0 | btlseqInit, then BattleCheckAllPinchStatus mode 1 |
| 1 | Pinch mode 0, then btlseqFirstAct |
| 2 | Pinch mode 0, then btlseqTurn |
| 3 | btlseqEnd |
| Other | Complete with no child |

A child's guest bank/global mutation cannot change this held route. Its Boolean
acknowledges a whole retained child, not the original callee's scalar result.

## Stage counterparts

Stage-object cursor captures B for 32 records at +0x1715c, stride 0x7c. Per-row
vector-cache copies and dirty-bit-2 clearing happen once before map calls. Shared
layer-2 rotation cache remains shared. Each pending translate/rotate pins name
and XYZ; after acknowledgement the next pivot/rotation/name reads remain fresh.
Positive pivot is read after rotate; visibility is read after transformations,
then actual group flags queried. A callback's mutation can therefore affect a
later operand without altering the already held request.

Stage-work cursor captures W=B+0x163fc before spotlight and retains it across
children. Twelve entries begin +4, stride 0xe8. An inactive scratch pointer +0xe4
is captured for free and cleared only after acknowledgement. Four byte-output
interpolators use the [stage offset table](battle-stage.md). Each positive count
predecrements once, then freezes unsigned endpoints, signed mode, duration and
elapsed. Interpolation result is retained separately before a possibly held
truncate. Only actual truncate permits low-byte output and index advancement.
Identity/reentry guards prevent invalid resume, not rollback of source stores.

## Field-sequence facade

`BattleSequenceBinding` borrows Game and the exact EventNativeRuntime. Game's
shared process owner retains saved-mode BSS 0x8041e968 and optional source storage.
Facades do not seed that BSS. Bind precedes the legacy action snapshot and requires
an established field NPC owner. If script import fails, only a complete byte-exact
original view can be reused; conflicting/partial data is not repaired implicitly.
Destructor unregisters its tracked aliases while retained process storage lives on.

Storage must share the original pouch and process owner. Five aliases cover heap
globals 0x8041e808 (20 bytes), pouch/battle pointer cells, actual arena and original
pouch. Failure rolls back only completed mappings; process storage publishes after
all succeed. Animation attachment requires the actual shared party/NPC pool before
entry/init and binds its established W/RAM. A synthetic empty pool is not a substitute.

Allocation requires exact VM facade, attached storage, actual flush/barrier hooks
and source allocator admission. Ownership checks walk 32-byte-aligned heap headers,
validating full header/payload span, used state and monotonic nonoverlapping links.
A numerical payload address alone proves neither current ownership nor size.
Entry admission independently verifies GP+0x168's actual NPC field work, its owned
active actor at F+8, optional shared-pool agreement and no legacy snapshot. This
is no-effects native admission, not completed NPC or battle initialization.

Only original Init 0x80073f20/Main 0x80073b24 run on Game's exact sequence work.
Entered publishes after complete Init. Per-call stack bridge objects are rebuilt;
retained Init/Main/sound/field/animation cursors belong to the facade. Source calls
for map-pool allocation require 0x64000 bytes plus platform hooks; the camera adapter
admits only (4,2), saved-switch adapter only (0,0xf5de0180). Others delegate to the
real backend. Main intercepts actual field initialization and animation battle-init
while preserving their child cursors. Its word/half-only outer view deliberately
has no byte callbacks. Missing backend stays held rather than enabling the legacy
snapshot renderer. Pending diagnostics prioritize sound, animation, field-init,
then Init/Main; zero pending alone is not a complete integration proof.
