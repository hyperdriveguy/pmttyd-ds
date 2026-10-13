# Original EXP icon clock and type0 window frame

2026-10-13 US read-only used-path delivery. Complete iconEntry2D8001aeb0/1e4,
iconMain8001b2d8/374, windowDispGX_Waku_col8007b44c/68c and
windowTexSetup8007fbd8/118 listings are in ignored
`artifacts/recovered-temp-evidence/`. No native edits or original execution.
See [EXP callers](../battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md)
and [window palette lifecycle](../message/MESSAGE_WINDOW_MANAGER.md).

## Per-record creation and clock

Icon pool owner starts r13+1720; GP14!=0 selects owner+8. Owner0 is record
count, owner4 the record pointer; records stride38 hex/56 bytes. Fresh entry
finds an inactive record, memset clears all56 bytes, copies source default XYZ,
scale at+10 and color at+14, copies name into18, stores original iconID atU16+28.
Both r13+1718 and+171c resource-ready values must be nonzero to resolve the
binary descriptor. Missing readiness sets flag2000; loaded looping descriptor
mask1 sets flag1000; final active/2D flags OR11. Used404 descriptor is loaded
loop1,count4, first image418/delay6, with no locale73/103 remap.

| Record offset / width | Entry producer / used meaning |
| --- | --- |
| 28/u16 | Original icon404 |
| 2a/u16,2c/u16 | Descriptor words2,4 (reserved metadata) |
| 2e/u16 | Descriptor6 = frame count4 |
| 30/u16 | Frame index0, explicitly written |
| 32/u16 | Descriptor8 = current image418 |
| 34/u16 | Descriptora = remaining delay6 |

iconMain first requires both resource-ready values, then visits active records
in pool order. Pending flag2000 clears and refreshes descriptor fields/frame0;
loop descriptor sets1000. For an animated record1000, decrement remaining34
if nonzero. If remaining is now0, increment frame30, wrap >=count2e to0,
then load image/delay from descriptor+8/+a+4*frame. No global frame counter,
modulo-total animation time or rendering-clock read exists in this branch.
First Main call changes6→5; sixth advances418→419 and reloads6. Hidden flag2
suppresses draw registration **after** clock update, so hidden icons still age.
Loaded active2D icons register ordinary icon callback8001abf0 via
dispEntry(8,1,callback,record,sourceLayerFloat).

[Outer update order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md)
pins iconMain call8006f980 after battleMain8006f940 and evtmgrMain8006f954.
An EXP icon created during that battleMain pass receives its first decrement
later in the same outer pass. All19 fresh icons begin frame0/delay6 separately;
simultaneous creation synchronizes them through identical local state, not a
shared clock. Native simulation/update scheduling must preserve this ordering;
GPU redraws must not independently advance the clock. Resource readiness,
alternate pool, reload/delete/name collision and other icon types keep their
original service/lifetime domains.

## Exact type0 frame geometry and material

windowDispGX_Waku_col takes integer type, color pointer, and float arguments
X,Y,width,height,border. It calls camGetCurPtr8000f2e4; completion global
r13+1afc=0 exits without drawing. Nonzero low16 type is normalized0 by this
local body. The used0 loads texture-object r13+1b04 plus0*32, TEXMAP0.
Source camera matrix+11c concatenates an identity local matrix and is loaded
at position-matrix0. This is a source camera provider, not proof of a specific
3DS viewport projection.

GXBegin draws quads primitive80 hex, vertexformat0,36vertices. Define grid:

* X columns: X, X+border, (X+width)-border, X+width.
* Y rows: Y, Y-border, border+(Y-height), Y-height.
* U and V: 0,0.3333333432674408,0.6666666865348816,1.

Emit nine quads in row-major order. For each row/column, vertices are top-left,
top-right,bottom-right,bottom-left with Z0 and corresponding grid UV. The
full original FIFO contains exactly180float writes (36*(XYZ+UV)); no omitted
center, arbitrary colored rectangle, tiled edge or generated rounded frame.
Single-precision grid arithmetic ordering above follows source8007b6b4..dc.
Constants zero80420538, one-third80420540, two-thirds80420544, one80420548.

Material: no culling; one texgen/TEV stage; color register0 receives supplied
RGBA; TEV color C0*TEXC and alpha A0*TEXA, add/clamp; alpha compare always;
blend enabled source-alpha/inverse-source-alpha. PositionXYZfloat and UVfloat
are direct vertex attributes. Supplied EXP RGBAFFFFFF80 therefore preserves
source texture RGB and multiplies its alpha by128/255. Full GX state equivalence,
camera/texture filtering and Citro3D numeric/raster parity remain separate
backend responsibilities.

Original frame texture binding: synchronous archive `font/msgWindow.tpl` or
DVD fallback `%s/f/msgWindow.tpl`; see palette contract for asynchronous
completion-word policy. Used original ISO file `f/msgWindow.tpl`, size321984,
read-only FST offset947396944. Its TPL has51descriptors; descriptor0 header1a4
is width72,height72,format14(CMPR). It supplies the actual nine-slice frame.
Extraction remains ignored evidence; import descriptor0 through the existing
original TPL converter rather than drawing substitute art.

EXP caller uses (-76.8999939,21,165.7999878,24.1999989,10). A declared half-size
viewport adapter scales positions, dimensions **and border** together; it does
not change the original UV grid or replace type0's original texture.

Validation: complete read-only local bodies, original icon404 descriptor and
TPL directory/header, symbolic FIFO census180floats, existing outer-call-site
contract, `git diff --check`. No native builds/tests/commit or original raster
comparison; icon inner draw, palette loading and source-camera scheduling are
still separate backend services.

## 2026-10-13 used404 draw anchor follow-up

Complete iconDisp8001abf0/b8 delegates iconGX800197f8/9a4. Record flags1011
(active1,2D10,animated1000) have flip masks4/8 clear; descriptor reserved
width2a/height2c are0, so GX reads current TPL width/height40. Original geometry
8001a018..124 uses half-width0.5 but full-height: XY vertices
(-20,40),(20,40),(20,0),(-20,0),Z0,UV(0,0),(1,0),(1,1),(0,1).
**The record position is centered in X and anchored at the bottom in Y.**
Flag10 admits this full-size branch, bypassing ordinary .6 geometry multiplier
and special403/405 adjustments. Do not apply that .6 modifier to404.

iconDisp matrix is camera+11c * T(recordXYZ) *
Ry(-camera114*0.01745329238474369) * S(record10 all axes). Conversion constant
is8041f8a0. iconGX adds local axis flips only if masks4/8; used1011 clearsboth.
The current-camera-versus-camera8 alternate inversion branch is a separate
dispatcher domain; ordinary registered camera8 uses the actual passed camera.

Big original scale0.899999976/Y25 gives bottom25,topapproximately61, width36;
small0.449999988/Y0 gives bottom0,topapproximately18,width18. Values are source
float products, not newly measured integer extents. A half-size viewport adapter
scales all coordinates/dimensions and preserves the bottom anchor. It must not
add half-height to the source bottom before drawing. The source frame has
top21/bottom-3.1999989, so small icons occupy0..18 inside it; big icons areabove.

Evidence ignored icon-disp/icon-draw listings; complete iconGX scope9a4 (the
initial convenience dump included neighboring bodies beyond that scope).
Read-only source/constants; git diff --check, no native edits/build/commit.
