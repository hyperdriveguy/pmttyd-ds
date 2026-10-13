# NPC talk ray callback policy and Mario display-direction getter

US G8ME01 disc0 revision0 read-only static DOL evidence, scoped addendum to
[ordinary NPC talk](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) and
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). Corrects the earlier
"scope0" terminology: the talk caller passes a **null callback pointer**,
not a collision-world scope selector.

## Null talk callback versus ordinary wall predicate

At0x80047bc8 the talk candidate sets r3=0 before hitCheckFilter0x8001577c.
The [complete scalar-ray ABI](FIELD_SCALAR_RAY_WRAPPER.md) is r3 predicate
callback,r4/r5/r6 hit-point outputs,r7 input/output length,r8/r9/r10 normal
outputs, floating originXYZ and directionXYZ. Wrapper constructs Q, copies
length from*r7, and calls hitCheckVecFilter(Q,callback) at0x80015844.

Therefore ordinary talk **does not invoke an ordinary Mario wall-attribute
callback**. Suppressing a polygon because native ordinaryWallAttributes would
reject it changes the talk caller's request. On backend null callback, generic
region/bounds/triangle/cull/nearest-length behavior still applies; null does
not mean every raw polygon must hit or disable geometric admission.

Fresh backend inspection pins all four callback invocation sites:
0x80015950..970,0x80015aa0..abc,0x80015e14..e30,0x80016178..194.
Each tests callback pointer and skips invocation when null; otherwise calls
callback(Q,record) and skips that record on returned0. Attributes are not
filtered by an invented default callback in the null branch. These sites
compose across original backend collections with their independent sphere/
axis/plane/cull tests. Complete generic traversal is a separate contract;
this local source policy only closes callback admission.

The native talk-ray adapter should retain source generic geometry admission
and use no additional ordinary wall predicate. If only a semantic native wall
mesh is available, its missing source polygons/material-attribute differences
remain explicit limitations; a native filtered mesh cannot be called a complete
original null-callback query.

## Complete Mario display-direction getter

`marioGetDispDir`0x80096598,size0x64 calls marioGetPtr, loads current display
turn+0x1ac, constant90, rounded camera heading snapshot+0x19c. It computes
single result A=R(R(displayTurn-90)+cameraHeadingSnapshot). Repeatedly add360
while A<0, then repeatedly subtract360 while A>=360. Return final A. Constants
r2=0x80427440:90 at0x80420a98,360 at0x80420a68,zero at0x80420a60.
These loops are not an fmod/reviseAngle request and have no finite-input guard.

This is a simulation-field getter, not animation renderer smoothing, incoming
animPoseDraw angle or mirrored mesh yaw. A native presentation's retained
`displayTurn` and `cameraOffset` may supply this getter only when bound to
source+1ac and+19c respectively. [Mario direction producers](../overworld/OVERWORLD_MARIO_DISPLAY_DIRECTION.md)
own those fields and display-target smoothing; [camera heading snapshot](../camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md)
owns the sign-aware quantized+19c publication. Do not substitute current-stick
movement heading+1a4, literal sprite left/right, or live camera+114 directly.

Talk scan then **separately** subtracts current camera4 viewYaw+114 from this
getter and normalizes via reviseAngle at0x80047920..928. The rounded snapshot
and current viewYaw are distinct source inputs; event/camera updates can make
them differ. Camera subtraction is not already absorbed by the getter.

## Reproduction and limits

Fresh read-only inspect_dol_function.py disassembly of marioGetDispDir,
hitCheckFilter,hitCheckVecFilter and the source talk call-site; direct constant
reads. Callback null branches and all four invocation boundaries were checked.
Inspection outputs stay /tmp; no native code/assets/commits changed. Existing
source geometry/traversal/triangle/FPSCR and simulation/event lifetimes remain
separate; no runtime/hardware/build claim is made.
