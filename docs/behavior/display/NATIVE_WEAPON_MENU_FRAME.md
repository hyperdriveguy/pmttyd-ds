# Native retained queued weapon frame

`BattleWeaponNativeFrame` composes the existing whole original
[queued weapon draw](../battle/RETAINED_QUEUED_WEAPON_DRAW.md) with actual camera
and external ordered submission ownership. It introduces no source scheduler,
queue record, menu readiness producer or synthetic private stack values.

## Original entry and separate admission

Original callback80121ce8/24 calls Disp8011604c/20, then
DrawWeaponWin80115dac/260 and its retained Common/Sub1 graph. The shared source
verifier pins all five complete bodies,29 calls and six constant spans. Native
camera admission requires exactly80121ce8, camera8, original passed B, the
actually invoking CameraDraw stage8/current8, actual queue current record and
reset epoch, plus the completed real CameraMain producer pass. A class callback
8012210c or HP callback8011fde0 never substitutes for it.

`BattleWeaponCameraBridge` is a fixed-callback companion to the existing
[audited camera bridge](../camera/NATIVE_CLASS_CAMERA_BRIDGE.md). A narrow
friend/allowed-callback integration journal enables its private bind entry;
the public class and HP entry points retain their identities. Original camera
words remain actual bank captures at explicitly reserved native addresses.
The reservation must be unmapped in the actual VM, including file-cache maps.
Unavailable words inside its owned range cannot fall through to guest memory.

In addition to exact view/projection/yaw validation in the common engine, the
weapon companion copies actual camera8 viewport, scissor and projection kind.
Any change through the held callback invalidates the lease. The copied view is
available only during the owner's live resume. It is not a guessed viewport or
selector projection. The renderer applies the actual projection to source
vertices/matrices according to their source space, without applying view twice.

## Whole source and external batch ownership

Binding requires a strong actual UI allocation token, actual W and B, strong
actual source-resource owner, separate pure current predicates, successful Main
producer predicate, actual bank, and strong linked-provider context. Token
address reuse cannot occur while this owner pins it. The native generation and
monotonic pass are metadata, not invented guest pointers or rendering frames.
The source table is copied once; its original current callback is retained and
combined with the actual allocation/resource/camera guards. Linked math and
raster providers remain real whole effect owners. The VM and camera bank must
remain frozen through synchronous source reads and actual provider callbacks.
No generic retry of a synchronous partially mutating child is introduced.

For each pass the owner completes these stages once: external begin, whole
retained source draw, external immutable seal, actual complete ordered submit,
then camera finish/queue acknowledgment. False begin or submit, or an empty
seal, requires that same external provider to retain its successful prefix.
Neither storing a GX request nor the early original W14 readiness write counts
as completion. The source cursor is never rerun after whole source completion,
including while seal or submission waits. Submit's successful return is latched
before subsequent guard checks and cannot be replayed after invalidation.

The opaque immutable shared batch must pin all original surfaces/icons/fonts,
actual effective GX/matrix/projection state and ordered packet/resource leases.
It becomes externally drawable only after whole submit and camera finish.
Its long current predicate proves allocation and resource history independently
of the finished short camera epoch. A failed batch proof permanently invalidates
this binding. No partial or dead batch is exposed. A later pass hides the old
batch while building its replacement; publication never starts another source
draw. Completed passes cannot be invoked a second time on this owner.

## Lifetime, retirement and native binding

Actual provider resources and source-resource contexts outlive every held
callback. Pure ownership predicates cannot reenter. Other callbacks may request
retirement: invalidation is immediate and destruction is deferred until the
active call returns. Reentrant resume/bind fails. Explicit retire clears only
native metadata; it invokes no guest End, free or readiness mutation. Rebind
requires explicit retirement and fresh actual ownership.

The unapplied Trio journal reads the actual UI allocation-token map and live
B1c78; it pins the runtime's actual menu-file owner and source initialized
FileEntry/pointer-cell/palette chain. It checks the real Main publication and
forwards the real Draw/queue operands. Its reserved native view range is
validated with actual `EventAddressSpace::unmappedRange`. The Trio facade must
outlive its closures. Provider word routing must query the bridge's owned range
first, then actual VM/GP memory only outside it. Original resources remain
unavailable if their actual producer has not completed.

## Validation and remaining frontier

Focused host tests run actual CameraManager Main/Draw and DisplayQueue dispatch
with declared fixture camera callback/SDK/submission providers. Source memory
comes from the original event constants pack and original Weapon UIInit.
Tests compare whole source traces and final memory to the existing direct
retained reference, hold every child twice, hold begin/seal/submit separately,
reject wrong class callback/no producer/reset-and-identical-repopulation,
allocation/producer/reservation/camera mutations, verify deferred retirement,
reentry, long publication invalidation and no source/submission replay.
These are source-backed host ownership tests, not real Trio activation,
executed PPC, GPU pixel parity or physical hardware evidence. ARM syntax and
ASan/UBSan provide additional checks. Real mixed ordered GPU translation,
resource residency and whole submit remain external requirements. No normal
weapon activation is claimed by this module or its integration journals.
