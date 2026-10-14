# Rogueport east Borodo alley camera and occlusion diagnosis

US G8ME01 disc0 revision0; read-only source2026-10-14 for the open
[EVT-01](../../requests/EVT-01.md) back-alley camera request. Complements
[world actor identities](TROUBLE_CENTER_QUEST1_4_WORLD_ACTORS_AND_DOOR_COPY.md),
[camera container](../camera/OVERWORLD_CAMERA_CONTAINER.md),
[source zone fan selection](../camera/OVERWORLD_CAMERA_ZONE_TRIANGLES.md), and
[camera state](../camera/OVERWORLD_CAMERA_ROAD_STATE.md). Exact asset geometry
and source requests below are not original runtime/raster observations.

## The initial capture fixture was inside the house camera area

Borodo2's source position is engine(-475,0,-180). Camera container geometry
uses engine/10 units, so its corresponding coordinate is(-47.5,0,-18).
The capture fixture instead placed Mario at engine(-380,0,-180), i.e.
authored(-38,0,-18). These select different original camera areas:

| Mario engineXYZ | AuthoredXYZ | Winning original road below queryY0 |
| --- | --- | --- |
| -380,0,-180 | -38,0,-18 | index4 C_ie01, planeY approximately-1 |
| -475,0,-180 | -47.5,0,-18 | index1 C_ground_ok, planeY-12 |
| -500,0,-180 | -50,0,-18 | index1 C_ground_ok, planeY-12 |

Original full names have prefix `|MarioSt_CameraRoad|`. Identity is file
record index, not a deduplicated name: index9 is also named C_ground_ok.
The house01 zone's authored bounds are approximately
X[-45.70116,-26.80116],Y[-1.00000,-0.99997],Z[-31.99999,-4.99998].
Mario at(-38,0,-18) is inside an actual zone triangle, not just that box.
It also covers ground roads0/1 atY-10/-12; nearest downward plane is the
house zone, so it wins. Keeping the exterior closed while directly placing
Mario inside this house camera area can make a front-view capture show walls.
That fixture is not evidence that the source alley hides a building.

The two alley points hit only road1 among below-Y0 road polygons.
Winning source polygon8, fan triangle1, is:

```
(-65,-12,-7)
(1.6561946868896484,-12,-7)
(1.6561946868896484,-12,-68)
```

Road2 C_ue also covers those XZ coordinates, but its planeY11 lies ABOVE
queryY0 and is rejected by the original downward-ray policy. Road0
C_ground_ma's overall bounds overlap the alley points, but its actual fans
do NOT cover them. Replacing zones by overall boxes would change selection.
Point coverage was independently calculated from original polygon indices
and fan geometry; these finite points are away from the relevant edges.
This is an inspection result, not bit-identical Gekko execution evidence.

## The alley has an authored side-facing camera road

Road1 start/count at+a0/a4 is a six-point/six-tangent slice. Its point rows2/3
are(0,0,-42.148258209228516) and(0,0,37.05173873901367).
All tangent rows have X0,Z1, with original parameter values
-1,0,.1700000018,.8299999833,1,2. Thus the straight-path normal is(-1,0,0),
as the source state computes direction=(-tangentZ,0,tangentX).
Its record+58 distance55,+64 pitch14,+70 secondaryangle-4,+7c shiftrate20;
fixed-Y flag+20 and disable+28 arezero. It is not the front-facing ground road.

The linked original camera state/transition and final view owners must produce
the final eye/at. Do not invent a new camera position, aim radius, always-on
cutaway or lens to make an inside-house fixture resemble the alley. Native
diagnostics should inspect actual candidate/current/destination road indices,
retained Mario look/collision target, active event overrides and transition
history at the stated alley coordinates before changing rendering.
The native camera already imports this same raw source container.

## Existing explicit cutaway events do not authorize a left-building hide

gor_02_init_evt launches front_wall_keshi(file4d428/6c) at4dd74 and
tobiori_check(4d494/c0) at4dd7c. Both are persistent event loops.

camera_front_check(file4ff4/140) reads camGetPtr(4).float+114, computes
reviseAngle(single(fma(100,heading,5)/100)), then MarioX. If MarioX<-300,
it publishes output0 immediately, independent of heading. Otherwise finite
heading bands[0,90) and[270,360) publish1;[90,270) publishes0. It returns2.
The loop invokes it with UF0, sets flag1 on S_gai_kabe_mae when UF0==1,
otherwise clearsflag1, waits one frame and repeats. Thus at the left-alley
X-475/-500 it explicitly clears that hide bit, not sets it. Do not infer a
left-building cutaway from this front-wall callback's name.

tobiori_check initializes a256-byte retained camera-name cache through
get_camcurve_init(file4fb0/44). get_camcurve(4f00/b0) observes actual roadwork
and name identity; it publishes1 only when current name is exactly
`|MarioSt_CameraRoad|C_ground_ok` and retained previous name is exactly
`|MarioSt_CameraRoad|C_ue`, then updates its cached current-name string.
The event waits for that result1 plus MarioX>370,
hides S_ie_05_soto, waits through camera_move_check(4e80/80), then shows it.
That is right-side house05 camera-transition ownership, not this left alley.
camera_move_check samples the same rounded camera4+114 until two samples
compare equal, returning0 when changed,2 when equal. No timer/radius is inferred.

Borodo2 init itself only sets its source flags/position; it has no camera,
map hide, door opening, pose turn or per-frame regular callback. Its setup
regular slot isNULL. No actor-specific cutaway follows from its presence.

## Provenance and remaining checks

Original m/gor_02/c ISOoffset991819668,size11316,SHA256
ccf70290b06daa8d965b03e3796df9cf846c8f88c71651ac61aa3831f7aae41f.
It has14 records/14 zones/101 polygon descriptors, preserved in ignored
artifacts/trouble-backalley-original.camera. Original event inventory is
artifacts/trouble-backalley-gor02-events.json. Fresh complete camera callback
listings are artifacts/trouble-backalley-camera_front_check.txt,
camera_move_check.txt,get_camcurve.txt,get_camcurve_init.txt (same prefix).
REL relocations identify actual camGetPtr/evtSetValue/roadwork/string helpers
and original constants; no original code executed.

Next verify native camera selection and a fresh capture from an alley point
such as engine(-500,0,-180), preserving original actor placement. Compare the
actual camera frame and source map visibility there. No original Dolphin
capture, native raster parity or universal occlusion fix is established here.
No source assets, code, shared indexes/progress or asset records were edited.
