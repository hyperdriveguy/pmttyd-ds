# Original opening house door and postbox geometry

The bounded `OpeningHouseObjects` adapter retains the original outside aaa map
vertex indices, object-local coordinates, original converted XYZ and original
local-to-world matrices for `S_ie_door` and `S_posuto`, including descendants.
It changes presentation geometry only. Collision and complete map-driver
ownership are separate. The renderer supplies `OpeningHouseScene.doorAngle`
and `postboxX` and applies the visitor's XYZ before drawing the outside house.
The interior map hides both objects and does not load this outside-only pack.
See [opening flow](NATIVE_OPENING_FLOW.md).

US revision0 `evt_door`, REL0xb74..0xbb0, SHA256
`fb757f201ef61a0482a7bc99a053b5c75daf0dcfb3b6ccb9daea3cee67b25b38`,
multiplies LW0 by Q1024(-568), then by -1, before
`evt_mapobj_rotate(1,"S_ie_door",0,LW0,0)`. Therefore interpolation input180
requests local Y rotation99.84375degrees. The controller operand is not itself
the door's final angle. Source door opening interpolation0→180 takes500ms;
closing180→0 also takes500ms. The native controller now uses the recovered
curve11 half-cosine, with source Q1024 parameter truncation before the door
multiplier; its60Hz clock bridge is an adaptation. Closing runs alongside
Luigi's movement as requested by the source inline event.

The two source postbox shake loops request
`evt_mapobj_trans(1,"S_posuto",2,0,0)` and reset0 at REL0x1084/0x10a8 and
0x132c/0x1350. Source `mapObjTranslate`0x8001e254 divides each translation by
10 (SDA8041f944,41200000). `mapObjRotate`0x8001e390 builds local rotations
using radians multiplier8041f948,3c8efa35. `mapReCalcMatrix`0x80028130
concatenates the original world matrix with the object's dynamic local matrix
(e.g.800283c4..800283d0). Consequently the adapter rotates around the original
object hinge, retains the door's base45degree orientation, and applies postbox
translation in object-local coordinates. Converted world scale is retained in
the companion matrix; the postbox2 request shifts rendered X by0.02.

Geometry conversion yields594 door vertices and288 postbox vertices in the
25626-vertex outside pack. The original door hinge in converted coordinates is
approximately(.482071,.3,-1.536127). These are static conversion results, not
Dolphin observations. Every update recomputes from retained source geometry;
reset0 publishes the original converted floats directly and avoids drift.
The loader checks the map's vertex count, fixed object tags, finite values,
binding indices, unique ownership, complete reads and trailing bytes. Failed
loads preserve the previously admitted pack.

Ignored `opening-aaa_00.houseobj` uses magicPMHOBJ01, vertexCount and objectCount
(two), then per-object tag/count/12-float row-major affine world matrix and
bindings(index,localXYZ,originalXYZ). All scalars are little-endian. Tags0/1
are door/postbox. Geometry remains outside source history.

`tools/verify_opening_house_objects.py` reads the original ISO only and checks
source event/three matrix-service hashes and15 scalar/object/axis/constant
anchors. Portable tests verify local hinge rotation, source99.84375 endpoint,
translation scaling, repeated-update stability, exact reset, transactional
rejection and duplicate ownership. An optional original-pack test checks all882
bindings. Host/native trig and affine arithmetic are adaptations; original
Gekko matrix bit equality and full map flag/frame ownership are not claimed.
