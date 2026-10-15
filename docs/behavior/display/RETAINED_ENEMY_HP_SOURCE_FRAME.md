# Retained whole enemy HP source composition

`BattleEnemyHpFrame` composes existing recovered source controllers. It does
not replace their behavior with native HP formatting, visibility, positions,
or rectangles:

- Queued BattleDrawEnemyHP8011fde0 owns the64-slot parent scan, status27,
  actual Tattle checker provider, source position offsets, projection, fade,
  live HP reads, number-before-bar order, and source matrix.
- Complete iconNumberDispGx80019120 owns digits, mode, color capture,
  transforms and width, then complete retained iconDispGxCol/iconGX.
- Complete BattleDrawEnemyHPBar8011fc3c owns its private copied point/color
  and live unit ratios. Its stable requests enter retained TexPlane801032c8,
  InitRasta80103b48/InitSub80103c3c, QuadRasta80103918/Point801039ec and
  LoadPosMtxImm. The exact typed Plane request stream reaches caller services.

Source recovery remains in those controllers and their existing contracts.
This module is a native ownership/composition policy, not a new original game
function, scheduler, projection formula, FPSCR implementation or GPU backend.

## Binding and queued invocation

`bind` requires a nonzero actual allocation token and a pure, non-reentrant
allocation-current predicate that verifies that exact live allocation and keeps
its actual memory/service owners alive. A manually retained integer or a
reused B address alone is insufficient proof. This predicate is allocation
lifetime, not temporary resource readiness; missing resources belong to their
optional service providers. For HP the owner is the actual
battle/VM allocation, not invented dependence on a class-menu W allocation.
Only explicit `retire` permits binding another owner.

The caller supplies actual source parent word/half/camera/status/Tattle/
projection/interpolation/truncation providers; actual bar actor memory and
camera lookup; actual number resource/camera/math/draw services; and actual
Plane memory/matrix/GX providers. Parent.number/bar and bar.request must be
null at bind because this module explicitly owns those child slots. Nonnull
values are rejected instead of silently replaced. Other missing providers
remain unavailable at their original source frontier.

`resume(pass,dispatch,queue)` requires the actual actively invoking camera8
callback8011fde0, its actual queue.current record, and matching source userdata.
The record's address and complete copied fields, queue/dispatch objects and
cursor index stay fixed through a held invocation. A different pass/context
cannot replace one that is held. New source frames require strictly increasing
nonzero pass keys from actual admitted simulation/Main passes. Replaying copied
native packets does not resume a source callback. Duplicate acknowledgement of a completed pass inside the
same actual invocation returns success without replaying source/backend work.

The allocation, invoking record and actual native DisplayQueue reset epoch are
checked before and after each optional source provider boundary. Every actual
queue.reinitialize advances its native epoch (saturation poisons0 permanently).
The frame captures a nonzero epoch; reset/repopulation with identical record
bytes cannot reuse a held invocation. This is native borrowing policy, not a
change to original dispatch: the underlying dispatcher can still resume its
captured invoking callback after count resets to0. No guest metadata is written.
These checks include number GX and linked bar raster providers.
Loss while held latches invalidity even if old scalar values are restored.
An already completed frame's `complete(pass)` uses actual allocation lifetime;
it does not depend on the short camera/dispatch epoch after Draw advances.

Actual camera admission stays external. In particular the class camera bridge
only admits8012210c; changing the source callback ID or faking invocation fields
to make it serve HP is not permitted. A real HP camera owner must supply the
actual8011fde0/camera8 draw context and published shared bank.

## Number and bar child lifetimes

The source HP parent's callback ABI passes RGBA by value. This module captures
that actual argument in stable owned storage once per number invocation, along
with borrowed matrix object identity and scalar number/mode. All are retained
through SourceIconNumber waits; a changed invocation is rejected. The retained
parent matrix is borrowed directly, avoiding a temporary flattened matrix.
The source number's computed width stays private because the original HP parent
discards it. Each next actual unit gets a fresh number invocation/context.

The queued bar controller owns its private source point/color copy and its
request object. The adapter forwards that same stable request directly to the
retained raster child. It never reconstructs a temporary request during a wait.
All callback contexts and nested provider tables are stable owned records. GX
function tables are copied at bind; their actual context/resource owners remain
borrowed and must outlive the binding.

Reentrant resume is rejected. `retire` from an actual provider defers metadata
deallocation until the outer resume unwinds, preventing child callback context
use-after-free. The containing frame object itself must outlive every active
resume/provider call; destroying it from inside a provider is not supported.
Retirement does not call guest End, modify guest fields, reset the camera/queue,
roll back acknowledged effects or discard an external GPU batch.

## Total-memory and ordered publication boundary

Actor memory used by source bar/Plane bodies has a total word/half ABI. There
is no unavailable return and the adapter does not invent zero as one. The
caller must admit and preserve a valid allocation and immutable camera epoch
through each synchronous total-memory loop and provider callback. No Main pass,
End/Init, mapping replacement or camera mutation may race those reads. Optional
provider guards can reject stale ownership; they cannot repair a violation of
this total-memory lifetime contract.

All source number/iconGX and bar operations execute inline in original per-unit
order. Feed them to one actual ordered presentation owner: number glyphs for a
unit, texture17, optional solid ratio overlay, texture18, then the next unit.
Collecting all numbers and then all bars loses source ordering and is invalid.

This module owns no GPU batch or publication. `complete(pass)` is only whole
source-callback completion. Actual sinks must retain acknowledged prefix effects
across waits and expose no partial drawable batch. The caller publishes only
after this callback and the required whole source queue/frame are complete,
with actual texture residency/generations/allocation still valid. Stale or
retired partial sink state must be explicitly discarded by its real owner.
The allocation predicate must remain about actual lifetime, not the short
camera epoch, so completed copied geometry survives camera advancement.

## Focused validation and remaining integration

Tests execute actual DisplayQueue dispatch and the whole recovered source graph
using explicit host providers, actual existing tutorial constant data and original
icon directory. They cover every number/matrix/GX/bar-raster effect frontier,
no replay after waits, copied RGBA, multiple units in original number/17/solid/18
order, increasing frames, duplicate completion inside actual invocation, replaced
contexts/keys, lost allocation, invalidation after an acknowledged provider,
reentry, deferred retirement/rebind and unavailable status/GX owners. Host
projection/texture/GPU providers are declared test services, not native proof.

Normal battle Main/sequence/registration scheduling, actual HP camera binding,
shared icon/HP publication/resource owner and final GPU draw remain integration
responsibilities. This module does not activate an ordinary tutorial path or
synthesize provider readiness.

Related:
[queued HP parent/bar](QUEUED_ENEMY_HP_PRESENTATION_AND_RETAINED_BAR.md),
[original number](SOURCE_ICON_NUMBER_CONTINUATION.md),
[HP parent/checker](../battle/ENEMY_HP_DRAW_AND_NATIVE_TATTLE_CHECK.md).
