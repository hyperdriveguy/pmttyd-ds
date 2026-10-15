# Native class-menu camera bridge

`BattleClassCameraBridge` binds the original menu projection providers to the
shared `CameraManager`. It does not run camera Main, construct a view, register
a display callback, alter actor coordinates, or advance the battle sequence.

## Source contracts and native policy

The recovered `btlGetScreenPoint801009b4/98` captures camera4 before copying
three actor-position words, calls view at camera+11c and projection at camera+15c,
then reads the original304/240/0 constants. `packedBattleScreenPoint` retains
that body. `GetRingCenter` and `GetRingOffset` remain the existing complete
`packedBattleActRingCenter` and `packedBattleActRingOffset` implementations.
`btlDispTexPlane2` calls actual current-camera lookup; menu circle/bubble and
pinch paths request camera8. The queued class callback is8012210c, with passed
battle userdata; its source camera argument is ignored by the queued wrapper.

This bridge supplies existing `cameraDepthView`, `cameraDepthProject`, native
libm sine/cosine, and `bindAudienceAuxMathAdapter` providers. Finite inputs and
finite outputs are the declared admission domain. This is native arithmetic
policy, not SDK/Gekko FPSCR, paired-single, libm, or pixel parity. Positions stay
in source units. No multiplication by100, fixture lens, identity camera8,
projection substitution, or screen clamping is introduced.

The bridge copies the actual bank's camera4/8 matrices at admission. Camera4
has already passed its Draw projection refresh when the camera8 callback runs;
camera8 has also reached its projection stage. The bank remains authoritative:
every query compares the copied float bits, yaw, and published-view state to
the bank. A change during a held source callback invalidates the binding.

## Required producer and dispatch ownership

`bind` requires a shared bank, nonzero allocation generation, actual guest actor
memory services, explicit native address policy, and a current-epoch predicate.
Both predicates must be pure and non-reentrant. The guest memory owner must
outlive the binding; the predicate should capture
that owner's lifetime. A second bind requires explicit `retire` first.

The caller admits `{generation,pass}` only after the actual camera Main owner
has returned success for that pass. The required predicate validates that exact
successful producer admission and the still-current allocation generation.
A default inactive Main cursor, manually set publication flags, or a retained
numeric key alone is insufficient. The bridge also requires the actual completed
Main cursor (`index13`, inactive).

`begin` runs inside the actual `CameraManager::draw` callback: Draw is active
at stage8/index8, current camera is8, its display cursor is actively invoking
8012210c with the passed battle userdata, and `DisplayQueue::current()` matches.
The actual cursors and queue must outlive `begin` through `finish`. The same
pass/context may resume across waits; a different pass or borrowed owner cannot
replace it. Each completed pass must have a strictly greater key than the last.
`finish` occurs after the entire retained class frame completes, before the
callback returns success and Draw advances to another camera. There must be no
remaining potentially unavailable provider between successful `finish` and
returning success from that callback.

The producer predicate, range predicate, live source dispatch, and matrix bits
are checked at every bridge provider boundary. Loss of any of them while held
poisons the binding even if old scalar values are later restored. Explicit
`retire` releases metadata; it never writes guest fields or calls original End.
Retire dependent retained source cursors before releasing/rebinding this service
context; borrowed old native view tokens must not escape into another owner.

## Native word-address reservation

The caller selects an aligned nonzero base for13 slots with stride400(hex).
These are native view tokens, not recovered original camera addresses. The
mandatory `unmapped(base,bytes)` predicate must prove the complete reservation
has no actual guest storage, including dynamic shared `NativeFileCache` regions;
it is checked at bind and on every provider call. There is no default base.
In particular84000000 aliases the Harbor battle allocation;87xxxxxx may alias
shared file data and is not presumed safe.

`owns` reserves the entire range while bound, even when unavailable. `word`
exposes only camera4/8 +114 yaw, +11c..14b view, and +15c..19b projection,
preserving original float bits. All other words inside the range are unavailable.
A composed reader must ask the bridge first and may fall back to actual guest
memory **only outside** its owned range. Never cast these tokens to native
pointers or let missing bridge words fall through to unrelated guest storage.

`EventMemoryServices::word` has no unavailable return. A product raw reader must
therefore admit and preserve an immutable valid camera epoch before entering a
non-suspending source camera-word loop; zero is not an unavailable substitute.
Keep real CameraManager Draw retained at this dispatch and prevent another Main
producer from changing the bank until it completes. Providers must not mutate
camera state or its admitted epoch inside synchronous source word loops or
provider callbacks. Gate source callback entry/resume on bridge availability
while the frame is held. Keep the pipeline's allocation-current predicate tied
to actual UI allocation lifetime: adding this short camera epoch would hide the
completed drawable batch after `finish`. Published copied geometry needs no
subsequent camera queries. Optional bridge methods check the epoch around their
guest/math calls.

## Integration boundary and validation

The bridge is an isolated provider, not normal tutorial activation. The product
owner must supply real command registration, battle/sequence scheduling, actual
successful camera Main publication, queued draw dispatch, guest range checks,
retained footer/resource owners, and final native draw admission. The ordinary
Harbor path must not borrow the development fixture's camera maps or readiness
state.

`battle_class_camera_bridge_tests` executes actual `CameraManager::main` and
`draw` with explicit host providers. It verifies raw source coordinate projection,
ring results, exact source word views, retained callback admission, monotonic
passes, no source reads after stale admission, mutation poisoning, unavailable
boot views, finite-domain rejection, mid-read epoch loss, shared bank lifetime,
and explicit retirement/rebinding. These host tests do not demonstrate normal
product scheduling, GPU admission, measured timing, or physical hardware output.

Related: [camera bank owner](NATIVE_CAMERA_BANK_AND_DEFERRED_DRAW_OWNER.md),
[camera Main/Draw](CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md).
