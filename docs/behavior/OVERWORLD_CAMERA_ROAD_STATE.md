# Camera road zone and state interface

US G8ME01 revision0, recovered2026-10-12; static source ownership only. Verify
`python3 tools/verify_overworld_camera_road_state.py`. Read
[camera lifecycle](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md) and
[authored-map root scale](OVERWORLD_ROAD_NULL_DOWNWARD_RAY.md).

This closes source input/output units, bounded zone selection and final eye/at
construction from an already valid road state. Full collisionCurve's curved
segment solver and compare/transition evolution remain open. A containing-body
hash is not completion of those algorithms, an original gameplay trace or native
numeric parity. Do not make a fixed/clamped view authoritative using this interface.

## Input and 96-byte road state

camRoadMain8000ab48 takes output buffer plus seven floats: blended player target
XYZ, blended look/collision targetXYZ and camera-shift secondary angle. It retains
the six incoming coordinates at work+1ec..200, then divides **all six by10**
(8000ab68–b0) before calcCamRoad. Thus the road container's geometric coordinates
are authored-map units, whereas camera/player fields are runtime engine units.
This matches the initial ×10 map/hit parent matrix.

The global camera-road work pointed to by r13+8088 owns current state+8,
candidate state+68 and previous/transition state+d8. Each state is0x60 bytes.
calcCamRoad candidate write tail8000c354–3dc establishes:

| Relative state field | Source role |
| --- | --- |
| +0 | Valid/active word, written1 on the normal retained/candidate state path |
| +4 | Whether current selection found a source zone, normalized0/1 |
| +8 | Retained collision-curve result flags |
| +c/+10/+14 | Current divided player targetXYZ |
| +18/+1c/+20 | Current divided look/collision targetXYZ |
| +24 | Selected/retained authored road record pointer |
| +28/+2c/+30 | Evaluated/retained camera anchorXYZ |
| +34/+38/+3c | Evaluated/retained directionXYZ |
| +40 | Camera distance, initially road record+58 |
| +44 | Primary pitch degrees, initially road record+64 |
| +48 | Secondary axis-rotation degrees, initially road record+70 |
| +4c | Retained float curve index/position token derived from collision output |
| +50/+54 | GP+40/+44 timestamp copied at update |
| +58 | Incoming seventh camera-shift argument |
| +5c | Authored road record+7c transition parameter |

Initial fields can later change through valid-state curve/transition composition.
The primary/secondary angle labels above follow their final consumer rather than
assuming a conventional yaw/pitch/roll authoring schema.

## Bounded zone selection before the path solver

calcCamRoad8000b6cc iterates the container's+c8 road records at+ec, stride184.
For version101 it skips records whose word+28 is nonzero (8000b7e4–fc). The
version producer is camRoadSetup; do not apply that skip to other source versions.
For each record, +a8 indexes the 104-byte zone collection at container+f0 and
+ac counts consecutive zones.

A record with zero zone count uses record+8c as its candidate plane height:
it is eligible only when height<=divided lookTargetY AND height>the current best
height. The initial best is literal-100000000. A tie does not replace this
fallback-height candidate (8000b808–824).

For nonzero zones, each zone's +58 indexes the vertex collection at+fc, +60
indexes 8-byte polygon descriptors at+f4, and +64 gives polygon count. A polygon
descriptor contains start index and index count into container+100. The source
uses a **triangle fan**: the first listed vertex stays fixed, and consecutive
remaining indices create triangles in authored order (8000b874–950).
It invokes collisionTri_simple8000d3b4 with divided lookTarget first, then the
three vertices, and a shared best-height pointer. The player target is not the
zone-testing point. Selection continues through all records/zones/polygons;
there is no first-zone-wins early exit for the entire search.

For ordinary finite nondegenerate horizontal zones with lookTarget strictly
above the plane and projected point inside/on the fan triangle, the helper's
ray is exactly(0,-1,0) and it admits the plane if its height>=the retained best
height. Its final comparison rejects height<best and writes/returns1 otherwise
(8000dec8–ee4). Thus these polygon ties **replace with the later visited zone**,
unlike the zero-zone fallback's strict-height tie rule. The source separately
has normal/plane/edge branches for other orientations and strict ray-plane
side admission; their full finite/degenerate boundary reconstruction remains
outside this bounded flat-zone rule. Do not broaden this to arbitrary volumes,
axis-aligned authoring bounds or a nearest-marker radius.

After selection, calcCamRoad calls collisionCurve(mode0,selectedRoad,playerTarget,
output) at8000b9d4 to validate/evaluate the path. A failure clears the selected
candidate pointer. If no current selection survives, a valid retained previous
state can supply its record pointer+24; without one, the source preserves the
input targets in global history and exits the ordinary state-building path.
Version101 with a surviving record+20!=0 overrides anchor Y with record+24;
otherwise it uses divided playerTargetY (8000bb0c–28). This fixed-Y owner is
separate from zone height selection and the ordinary cameraY follow owner.

Full collisionCurve solving, side caps/extension flags, look-target correction,
state inertia and transition blending must be closed before producing the valid
RoadState above for all actual room coordinates. The state interface does not
license replacing those calculations with a linear clamp.

## Final eye and at from a valid current state

calcCurrentCamState8000def4 copies container header triplet at+e8 into output
floats+0/+4/+8. Let P=currentState.anchor, D=currentState.direction,
R=currentState.distance, A=currentState.primaryPitch,
B=currentState.secondaryAxisAngle. Its finite ordinary construction is:

```
a = A * sourcePi / 180
O = (D.x*R*cos(a), R*sin(a), D.z*R*cos(a))
eye = P + O
axis = (-D.z, 0, D.x)
b = B * sourceRadiansPerDegree
at = eye + PSMTXRotAxisRad(axis,b) * (-O)
```

SourcePi is literal0x40490fd8 (3.1415920258), and sourceRadiansPerDegree
is0x3c8efa35 (.01745329238). The source recomputes cos separately for X and Z,
uses single multiplies/adds, and calls the SDK axis-rotation owner (which
normalizes its axis). DirectionY is not used in this final offset. B is an
axis rotation about the horizontal perpendicular to D, not a presumed camera
roll around its viewing direction. When B==0, at=P in real-number algebra;
original instruction rounding still matters. The source axis must be nonzero
for a valid ordinary direction; no invented substitute axis is established.

With inactive currentState, the leaf writes eye(0,100,100), at(0,0,0), retaining
header triplet metadata. camRoadMain then multiplies output eyeXYZ and atXYZ by10
(8000afd8–8000b020), leaving the first three metadata floats unchanged. This is
before camera3d's dolly and event-blend owners, not a final-screen projection.

The ordinary cam3dMain call at8000ef9c receives that output into a temporary
nine-float buffer. It uses eye and at for subsequent dolly and event blending;
it **does not store the returned header triplet into camera+38/+30/+34** in
this body. The actual perspective at8000f174–188 reads retained camera FOV+38,
near+30 and far+34. Therefore do not replace camera init's FOV25/near1/far32768
with the camera-file metadata25/.01/1000 merely because the road output contains
those values. Explicit event/lens owners remain separate source dependencies.

viewYaw is then computed from resulting eye/at with the system angleABf compass
convention already pinned in the lifecycle contract. A final matrix/GX backend
can consume recovered eye/at/lens, but an ordinary view claim still requires the
remaining authored curve and transition owners to produce the source-valid state.
