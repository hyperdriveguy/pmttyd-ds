# Camera 4 lifecycle, follow and field projection

US G8ME01 revision0; source-only contract recovered2026-10-12. Verify
`python3 tools/verify_overworld_camera.py`. The evidence manifest pins camera
owners, all 75 shipped gor_00–04 EVT owners containing camera requests (332
calls with relocations), and the five original camera containers. It records
caller operands; it does not assert those events are currently admitted.

This closes ordinary field camera lifecycle, player target/follow, input yaw and
screen-cache composition. **Authored camera-road zone/curve selection and limits
remain unclosed**, even though their complete owner bodies and input containers
are pinned. Native fixed view, .07 smoothing or invented room clamps must retain
provisional status until that service is reconstructed.

Read [movement heading/rate](OVERWORLD_DIRECTION_MOVEMENT_RATE.md),
[air/landing consumers](OVERWORLD_AIRBORNE_AND_LANDING.md),
[partner projection helper](FIELD_PARTNER_SCREEN_RECOVERY.md),
[harbor story bindings](STORY_HARBOR_OPENING_BINDINGS.md) and
[lecture events](LECTURE_FIELD_ENTRY_AND_RETURN.md). This does not duplicate
partner screen-recovery branches or claim original/native numerical equivalence.

## Initialization and room-authored ownership

`camInit`8000fd9c constructs camera4 with camEntryPersp8000fac8 and callback
cam3dMain8000ea68 at80010404. Default lens FOV25, aspect=0x3fa22222
(1.2666666508), near1, far32768, viewport depth0..1 are source literals.
The perspective entry gets framebuffer dimensions from DEMOGetRenderModeObj;
source NTSC interlaced/progressive MarioSt descriptors at8030c2a8/8030c2e4
both have framebuffer width608 and EFB height480. These are original viewport
inputs; they do not authorize using native 400x240 coordinates for source policy.

Room `_mapLoad`80028f18 formats `./m/%s/c` using room name and calls
camLoadRoad(cameraId4,path) at800296dc, then sets perspective type. The five
shipped camera containers are:

| Path | Bytes | 184-byte authored road records |
| --- | ---: | ---: |
| m/gor_00/c | 2048 | 3 |
| m/gor_01/c | 7700 | 11 |
| m/gor_02/c | 11316 | 14 |
| m/gor_03/c | 11392 | 14 |
| m/gor_04/c | 1304 | 2 |

They identify MarioSt_CameraRoadExport and carry record count at+c8, record
pointer at+ec and separate curve/vertex/index collections. Examples include
harbor C_minato_l/C_kakusi/C_umi_02, plaza C_mae/C_oku and building views,
station C_mae/C_hai. Exact names and file hashes are retained in the manifest;
these names are not a substitute for the original selection geometry.

camLoadRoad8000f36c calls camRoadSetup8000e104. Successful setup sets camera
flag4 and mode1 at8000f3c0–d4; failure clears that flag and sets mode3. It resets
FOV25, roll/view offsets, stateA+4=0, progressA+8=0, curveA+80=11,
curveB+e0=11, progressB+84=0 and target+94/98/9c=0. Do not infer an explicit
stateB+82 write where this body has none. camRoadSetup relocates the authored
container's pointer collections after loading through original asset services.
Asset parsing, allocation and original renderer/GX setup require portable adapters.

## Per-field-tick order and the retained target

marioStMain8006f904 calls **camMain first** at8006f944, EVT main at8006f954,
Mario main at8006f95c, and caseMain later at8006f97c. In the ordinary Mario
path marioMain calls marioCamMain at8005b0f4 after motion dispatch/movement.
Thus camera4 ordinarily consumes target fields retained from the preceding
Mario update; motion uses this tick's camera. EVT updates between these owners
can change camera/control state before current Mario dispatch. Rendering order
must not reverse this simulation dependency.

marioCamMain801a924c calls marioUpdateCam801a8f8c except motion29, then additional
zoom/request owners. In the bounded plain domain (no special zoom, display
flags10000/20000 clear, motion!=25), marioUpdateCam:

1. Approaches each retained shift toward its target: side current+13c/target+138/
   rate+150, vertical+144/+140/+154, depth+14c/+148/+158. If current>target,
   update current+rate*(target-1-current), clamping a crossed result to target;
   if current<target use current+rate*(target+1-current), also clamping a crossing.
   Equal values remain equal. Literal overshoot offset is **1**, not a tolerance
   inferred from native world scale. Reset rates are source .03/.06/.06 words
   at80424ae0/e4, set by marioResetCamShiftRate801a9544.
2. Uses camera4 viewYaw+114 minus90 and sincosf's (sin,-cos) outputs to set
   Mario.ec=rootX+sideShift*sin(yaw-90), Mario.f4=rootZ-sideShift*cos(yaw-90),
   Mario.f0=rootY+drawOffset9c+verticalShift. Mario+64 becomes depthShift.

The three shift approaches execute **before** testing display flag10000
(801a9100–108). Thus10000 suppresses target ec/f0/f4 and depth64 writes only;
it does not suppress current shifts13c/144/14c. With10000clear and20000set,
the alternate branch adds both side and depth along u(yaw-90) to rootXZ,
writes ec/f4 and depth64, and **preserves retained f0** (no vertical write).
With both flags clear the ordinary formula writes ec/f0/f4 and depth64. Ship motion25
has an extra vertical term. These are display flags at Mario+4, separate from
main Mario+0 flags used in airborne damping.

For ordinary mode1 camera3d, cam3dMain updates its player target as follows:

- If camera flag10 is clear, camera+98 :=
  oldY + (Mario.f0+1-oldY)*marioGetCamFollowRate(), with source fmadds
  at8000ed48. marioResetCamFollowRate801a9488 sets Mario+15c to source
  0x3e4ccccd (.20000000298). Other owners can set the rate; .2 is the reset
  value, not an unconditional per-frame constant.
- camera+94=Mario.ec+.01 (literal0x3c23d70a) and camera+9c=Mario.f4.
  There is no .07 exponential follow on these X/Z target writes.
- A separate collision/look target+ac/b0/b4 comes from Mario.f8/fc/100 with
  +1 on Y. Event blend stateB, camShiftMain and the authored camera-road
  service then act on these targets. They are not identical to final eye/at.

camRoadMain8000ab48 supplies final road-camera state after zone/curve and
transition owners. Event blend stateA can further replace/blend eye and at.
cam3dMain computes view distance at8000f148 and viewYaw from angleABf(eyeX,
eyeZ,atX,atZ) at8000f150–164. For perspective type0 it creates the projection
matrix+15c with current FOV, source aspect, near/far at8000f174–188. camMain
then builds camera+11c using C_MTXLookAt(eye,up,at) at8000f774–784; subsequent
roll/view-offset composition remains separate. A plain zero-offset view can
bind that standard view matrix, but cannot skip the unclosed road solver that
owns eye/at/FOV in real Rogueport.

## Heading snapshot and source screen cache

marioPreCamera801a938c reads camera4 viewYaw+114 and stores Q1000/revise360
in Mario+19c at801a9418. Its exact sign-aware rounding is already pinned in
movement-rate documentation. This field is a camera-produced heading offset;
ordinary Rogueport is not proved to have constant zero offset.

The system.o angleABf8000703c receives(eyeX,eyeZ,atX,atZ), computes
Delta=(atX-eyeX,atZ-eyeZ) and returns compass degrees0=-Z,90=+X,
180=+Z,270=-X. Therefore equal eyeX/atX and eyeZ>atZ gives exactly0,
consistent with a bounded fixed-view adapter pointing toward-Z. CoincidentXZ
also returns0 at80007124–2c. General headings use the original ratio/table
algorithm at803041e0 plus quadrant branches, not an unrestricted libmatan2
call; native vector/trig evaluation remains a numerical adaptation.

Motion29 in marioCamMain skips **only** marioUpdateCam at801a926c–274;
execution continues at801a9278 through zoom and remaining tail owners.
It is not a blanket disable of camera4's manager update or all player camera
services. A plain native domain can leave special29/zoom handling unsupported,
but must label that boundary rather than changing this source gate.

marioMove80059a78 invokes marioMoveMain at80059ab8, then projects the resulting
current rootXYZ. It obtains camera4, sets GX projection from camera+15c/type+19c,
reads GXGetProjectionv and GXGetViewportv, and calls GXProject with camera+11c
at80059b24. It converts projected SX/SY/SZ with **fctiwz without ±.5**, and
stores signed integers in Mario+168/+16c/+170 at80059b38/4c/60. In the finite,
representable domain this is truncation toward zero, not Q1000 or nearest rounding.
It does not use actor sprite center or previous root as the position argument.

For perspective GXProject802ba010, let view=matrix11c*root, projection vector
p[0]=0 and GX viewport=(vx,vy,width,height,nearDepth,farDepth):

```
invW = 1 / (-viewZ)
clipX = p[1]*viewX + p[2]*viewZ
clipY = p[3]*viewY + p[4]*viewZ
clipZ = p[5]*viewZ + p[6]
SX = vx + invW*clipX*width*.5 + width*.5
SY = vy - invW*clipY*height*.5 + height*.5
SZ = farDepth + invW*clipZ*(farDepth-nearDepth)
```

The leaf does not reject negative W or clipped coordinates; it computes them.
C_MTXPerspective8029e7e8 builds the source perspective matrix with
K=cot(FOVdegrees*pi/360), m00=K/aspect, m11=K,
`m22=-near/(far-near)`, `m23=-(far*near)/(far-near)`,
`m32=-1`, `m33=0`; all other entries are zero. Consequently the default GX
projection-vector p5/p6 coefficients have those same negative signs. This is
the source GX depth convention; default viewport depth0..1 yields near0/far1
through the GXProject expression above. The source computes an intermediate
single1/(far-near), then multiplies for m22/m23; matching algebra does not
establish identical instruction rounding.

Separate multiply/add single-precision instructions and the actual GX snapshots
matter to exact original numerics. Native matrix/trig/viewport services remain
numerical adaptations; finite-domain algebra alone is not bitwise equivalence.

marioChkInScreen80058928 tests cached SX inclusively0..600 and SY0..535.
It ignores SZ and does not recompute projection. marioFall consumes that cached
condition for its damping gate, in the movement/fall order pinned in the air
contract. Jump moves after jump integration; fall moves before fall integration,
so the timing of the cached root relative to vertical motion differs by owner.
A default true Boolean or a test against native screen bounds changes this policy.

## Authored event requests and remaining owner

The manifest records every camera request found in the 75 gor_00–04 EVT owners,
including door/interior, NPC and cutscene events. Operands remain source words;
full branches/admission are in their original event bodies, not inferred from
this census. Representative story requests (six integers eyeXYZ/atXYZ, duration
milliseconds, curve) are:

| Owner / REL call | Request |
| --- | --- |
| evententer_gorotsuki_town /25504 | set(0,700,1420,0,-10,0,0,11) |
| same /2581c | set(2,225,1015,2,50,243,3000,11) |
| same /25d98 | off(500,11) |
| christine_nakama /37478 | set(0,134,984,0,38,442,0,11) |
| same /37fe4 | off(500,11) |
| steel_coin /481e4 | set(-450,103,330,-525,43,-10,500,11) |
| same /4845c | off(500,11) |
| lecture /496b4 | set(-245,100,639,-245,40,294,500,11) |
| same /49944 | off(500,11) |

These are authentic requests, not permission to start those scenes in the harness.
evt_cam3d_evt_set800a89d4 captures current view, resolves the six coordinates
through the integer EVT getter (not arbitrary float geometry), records target,
GP+38 timestamp, duration converted to system ticks, stateA=3 and requested curve;
it returns2 immediately. evt_cam3d_evt_off800a7f48 captures current view, uses the
same timestamp/duration owner, stateA=1 and curve; it also returns2 immediately.
cam3dMain advances/clamps event progress and calls original intplGetValue with
that curve. Treating all curve11 requests as an invented generic interpolation
is not closed by this caller contract.

Next recovery owner: calcCamRoad8000b6cc, collisionCurve8000c484,
compareCamRoad8000b270, calcHokanCamRoad8000b038 and calcCurrentCamState8000def4,
composed by camRoadMain8000ab48 and camShiftMain800f3550. Their authored zone
selection, curve geometry, transitions and final limits must be reconstructed
before claiming ordinary room camera fidelity. Body/hash pins deliberately do
not label unresolved record fields as camera clamps or measured bounds.
