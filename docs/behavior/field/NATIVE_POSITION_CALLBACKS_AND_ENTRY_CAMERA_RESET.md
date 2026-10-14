# Native position callbacks and entry camera reset

Source evidence: [Bero actor/camera writes](../overworld/BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md)
and [camera map/reset lifetime](../camera/OVERWORLD_CAMERA_MAP_LIFETIME_AND_DUPLICATES.md).
Original pointer/operand traffic is retained by the portable callbacks;
native typed storage and engine-unit scaling are explicit adaptations.

fieldMarioSetPositionEvent captures args before obtaining Mario once. For each
X/Y/Z lane it freshly reads the raw operand, calls the float resolver and stores
immediately at captured Mario+8c/90/94. Later operands are not pre-resolved.
Changing the caller's args pointer or live Mario lookup during a resolver does
not replace those captured pointers. No locals, heading, floor, pose or previous
snapshots are written by this setter.

fieldPartySetPositionEvent captures args and reads raw selector0. Literal0 reads
the actual current index; every nonzero raw word selects slot1 without variable
dereferencing. The actual member lookup happens before any coordinate read.
NULL returns2 with no coordinate operands, resolver or stores. A present member
gets sequential X/Y/Z reads, float resolution and stores at+58/5c/60. Unknown
native index/registry state must not be treated as actual NULL.

fieldCameraRoadResetEvent obtains Mario then camera4. Its source read/store order
publishes camera+94/98/9c=(rootX,single(rootY+.01),rootZ), reloads those three
words individually into+ac/b0/b4, then copies fresh Mario root words to+ec/f0/f4
and again+f8/fc/100. Each root triple's X/Y reads precede its first two stores;
Z is read afterward. Road reset precedes shift reset. No caller local writes,
view/lens reset or harness rebase are supplied by this callback.

Game's live native adapters map current Mario root to progress.position and
absolute groundHeight+jumpHeight; retained Mario+ec is marioCamera.retained;
Mario+f8 is marioCameraCollision. Camera4+94 is fieldCamera.point and+ac is
fieldCameraCollisionTarget. Current-Y stores adjust jumpHeight while preserving
the actual floor, progress.elevation and ground metadata. Actual party records
use their source-sized big-endian+58/5c/60 storage and current-index owner.
Native actor/camera tokens are typed aliases, not packed original heap addresses.

Mario's native root is stored at engine*.01 scale; stores/reads round through
that conversion, with Y also using floor subtraction/addition. Consequently
camera retained values can differ by rounding from decoded source coordinates
and the party's directly stored floats. For example5/1024 round-trips one ULP
lower through *.01f then*100f. Portable callbacks preserve raw float words in a
memory-backed provider; this native representation does not certify source-bit
identity. SDK/FPSCR and original-runtime numerical comparison remain open.

The native road reset reconstructs CameraRoadWork and clears cached candidate
active/rate, retaining the externally owned CameraRoadAsset. Source rates are
already constants in the native road owner. Raw source reset preserves container,
version and opaque+204; the latter has no current native consumer/owner and must
remain opaque if raw work storage is introduced. Native shift.reset restores
semantic fields but preserves previousMilliseconds. Neither reset changes
FieldCameraEvents/view/target state, blended follow/look, frame/lens, camera
flags, global simulation clocks or harness initialization. In particular,
rebaseFieldCamera is not used as a substitute for this callback.

beginRoadArrival now calls applyFieldBeroStart. With a known start and admitted
native current-party index it invokes the real callbacks in source order:
Mario setter, party raw0, party raw1, camera reset. Coordinate resolution delegates
to the existing getEventFloat LW service over the exact encoded start words.
No synthetic party is created for a NULL branch. All operand/context workspace
is stack-local; no new per-frame allocation or geometry upload is introduced.
Completion is retained once per restored arrival so later camera writers are
not reset by repeat checks. A missing start/current index or unsupported index
holds this prefix before writes; the existing native movement adapter is still
separate from a full original EVT scheduler. Checkpoint restore clears the
unsaved completion flag with the other arrival views.

Tests compare exact raw read/resolve/store order, pointer capture, null-party
short circuit and raw nonzero selector behavior. Native tests use actual Mario,
party, camera, road and shift owners, checking retained flags/floor/blends/time,
unknown-state admission and nonreplay. Actual Game road integration checks that
beginRoadArrival applies the prefix and restore invalidates it. Build/capture
results are recorded in [progress](../../PROGRESS.md).

Remaining: source Bero global RAM/recovery/motion25 publication and the genuine
original initial_pos EVT execution, full shared field case lifetime, complete
house05 source snapshots/door events and matched original/hardware validation.
This prefix does not claim that the whole initial_pos child or area init returns.
