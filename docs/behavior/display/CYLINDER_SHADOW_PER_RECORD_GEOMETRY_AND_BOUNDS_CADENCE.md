# Cylinder shadow per-record geometry and bounds cadence

US G8ME01 disc0 revision0, read-only static ISO inspection 2026-10-14.
Complete cylinderShadowDraw8002bf84/368 and cylinder8002c2ec/198 verified
against original bodies, arrays and indexed lists. Extends
[setup/record gates](SHADOW_DISPLAY_SETUP_AND_EMPTY_RECORD_CONTINUATIONS.md),
[dummy register identity](DUMMY_DRAW_TEV_REGISTER0_AND_SHADOW_INHERITANCE.md),
and [full air-shadow requests/geometry](../overworld/OVERWORLD_MARIO_AIR_SHADOW_GX_GRAPH.md).
No source runtime/raster, emulator, hardware or numeric-parity result is claimed.
Ignored full listings: artifacts/reference/display-class-check/.

## Caller, admission and first actual geometry provider

shadowDisp's Polygon/shared cylinder setup completes before cylinderShadowDraw.
The helper captures primary pool before camGetCurPtr, retains that returned
camera's view pointer+11c, then scans source92-byte records with fresh signed
primary count each loop test. Admission reads radius10!=0, type BYTE2==2,
flags HALF0&2, signed camera HALF1a==passed draw camera, in that order.
Captured query camera supplies view; passed camera controls admission/bounds.
No geometry provider or per-record state requests run for rejected records.

For each admitted camera4 record, the actual ordered prefix is:

1. PSMTXConcat8029dd64(captured camera view,record.matrix2c,local3x4).
2. GXLoadPosMtxImm802ba2fc(local,0).
3. Copy retained startup-zero RGB word80429540, replace local alpha with
   freshly read record BYTE28, call GXSetTevColor802b8ab8(1,&localRGBA).
   Selector1 is **GX_TEVREG0**, not named GX_TEVREG1.
4. Execute pass1 state and its geometry; then pass2 state/geometry;
   then pass3 state/geometry. Only then advance record/index.

The current native admitted per-record geometry frontier is this matrix
concat/upload/register-color chain, followed by actual volume passes. It is
not merely an ellipse draw or an acknowledgement that a record exists.
An unavailable provider must retain its captured matrix destination/arguments
and completed earlier requests; later holds cannot replay prefix or triangles.

Prepared matrix already contains collision point/normal/radius/height. Do not
scale source vertices a second time, infer a flat floor, rerun the independent
ray, or use actor render position in place of the prepared transform. For
normal(0,1,0), matrix is translate(hitX,hitY+1,hitZ)*diag(-radius,height,-radius).
Original topY0 and bottomY-1 span the record height downward in engine units.

## Three passes, source arrays and fresh dispatches

All GX values below are original enums, not native constants. State setters
execute before each geometry helper even if that helper later emits no lists.

| Pass | Ordered source requests before geometry |
| --- | --- |
| 1 | ZMode(1,3,0); Cull(2); Blend(1,1,1,0); Blend(2,1,1,7); AlphaUpdate(1); ColorUpdate(0); ChanCtrl(4,0,0,1,0,2,2) |
| 2 | ZMode(1,3,0); Cull(1); Blend(0,4,5,0); AlphaUpdate(1); ColorUpdate(0); ChanMatColor(4,copied80429544=RGBA0); ChanCtrl(4,0,0,0,0,2,2) |
| 3 | Cull(2); Blend(1,6,7,0); AlphaUpdate(0); ColorUpdate(1); ChanMatColor(4,fresh primary.word100 RGBA) |

Pass1's first additive blend is replaced by LOGIC/OR before geometry. Pass2
modeNONE overwrites alpha; factor fields4/5 do not enable blending. Pass3
uses destination-alpha/inverse-destination-alpha RGB blending. Source depth
is LEQUAL with writes off; no depth reset is added in pass3. Channel material
source changes from vertex in pass1 to register in pass2, then stays register.
Inherited single-stage shader multiplies TEVREG0 alpha by selected raster
alpha; record alpha, palette alpha, material alpha and destination alpha are
distinct owners. No source restoration occurs between passes or records.

Freshly read record variant BYTE3 after each pass's setup. If exactly1,
GXSetArray(11,cube palette80415180,4), then GXCallDisplayList:
80304ca0/64 bytes,80415100/32 bytes,80415120/32 bytes in that order.
Otherwise cylinder(record)8002c2ec. The variant test is not captured once
for all three passes; mutations during an earlier held provider may change
later dispatch. Cube raw lists/palette are separate from the five-cylinder
import domain and must remain unavailable until their actual data/backend is
supplied. Parent fields/opacity do not authorize skipping cube primitives.

Every cylinder invocation separately captures flags HALF0 for its mask4 gate.
Mask4 set binds no-decay palette80415160 through GXSetArray(11,palette,4),
then top,bottom,side lists, regardless of global alpha. Otherwise it freshly
reads primary pointer/global color BYTE103, record.scale1c, outer24 and inner20
before its single-precision arithmetic. Captured plan values survive palette
or list holds; each new pass invokes the helper anew and can capture new values.

Ordinary arithmetic is source separate operations: product=R(U8alpha*scale),
innerRadius=R(1-outer), a=R(product/255),b=R(a*inner). Clamp innerRadius0..1.
a<=0 returns BEFORE palette bind or lists. b<=0 resets0; b>=1 also resets0.
alphaByte=trunc(R(255*b)) low8. Bind ordinary palette80415140; if captured
innerRadius!=0 draw inside; if !=1 and captured alphaByte!=0 draw outside;
always bottom then side. Exact numerical exceptional-input behavior remains
outside this finite contract. The source does not write palette colors or
rescale the authored inner ring using these admission values.

## Exact fixed cylinder data and list execution

Existing tools/verify_overworld_mario_air_shadow.py reads original data in
memory; tools/shadow_pack.py compiles that verified domain into a private pack.
All60 XYZ positions at80304ce0 occupy720 bytes, stride12, source F32BE.
Positions0..15 are top unit-ring/center/repeated endpoint atY0;16..31 bottom
atY-1;32..47 authored radius.6 top ring/center/repeated endpoint;48..59 box
positions unused by these five lists. Preserve actual float words, including
asymmetric least-significant bits; do not regenerate a regular polygon.

Each64-byte source list has primitiveBYTE,BE16 count, INDEX8 position/color
pairs, then zero padding. GXCallDisplayList802b9fa0 receives the entire64 bytes:

| List/VA | Primitive/count | Source pair order |
| --- | --- | --- |
| top80304b60 | FANa0/16 | (0,0), then (15,0)..(1,0) |
| bottom80304ba0 | FANa0/16 | (16,1)..(31,1) |
| side80304be0 | STRIP98/30 | i1..15: (16+i,1),(i,2) |
| inside80304c20 | FANa0/16 | (32,0), then (47,0)..(33,0) |
| outside80304c60 | STRIP98/30 | i1..15: (i,2),(32+i,0) |

Source descriptors are position INDEX8/XYZ/F32 stride12 and color
INDEX8/RGBA8 stride4; no UV field is emitted by these lists. Standard fan
expansion keeps center/order, standard strip expansion alternates winding.
The source palette vertices remain attached in register-material passes too.
Default record fields(scale1,inner.2,outer.4), global alpha110 choose
inside→outside→bottom→side in EACH pass:92 pairs,84 triangles if expanded.
No-decay selects top→bottom→side:62 pairs,56 triangles per pass.

Ordinary palette80415140 entries are RGBA(0,0,0,110),(0,0,0,0),(0,0,0,22);
no-decay80415160 contains three copies of black alpha110. Cube palette/list
identity is separate. No source alpha clear/framebuffer allocation is performed
in these helpers. Actual incoming framebuffer alpha/depth and destination-alpha
logic/raster behavior remain required owners for a genuine native volume draw.

## Camera1 bounding cadence and finite empty domains

Camera4/5/7 skip every bounding-box call below. Only admitted passed-camera1
records request sysWaitDrawSync then GXClearBoundingBox BEFORE matrix concat,
execute all three passes, then sysWaitDrawSync then GXReadBoundingBox
(&left,&top,&right,&bottom) and offscreenAddBoundingBox(record signed HALF18,
left,top,right,bottom)80072f38. This is once around all three passes per record,
not per list or per pass. Capture bounds outputs across the final provider hold.

Rejected/unprepared/camera-mismatched type2 records emit no per-record prefix;
the caller's earlier setup still occurs. For an admitted variant!=1 record
whose ordinary a<=0 in every helper invocation, no cylinder list emits, but
matrix upload, TEVREG0 write and all three pass setters still execute. They
leave pass3 state for the following record/helper. Global alpha0 does not
skip mask4/no-decay or variant1/cube domains. Passing camera4 does not remove
the source geometry requests merely because destination alpha happens to0.

## Evidence

All original reads were rb. Complete body/data hashes independently matched
the existing verifier manifest:

| Span | SHA256 |
| --- | --- |
| cylinderShadowDraw8002bf84/368 | 9997a7bc2fa9a50f2dcca8aa45b77b8dd43841e067d4cd4315093d608cab49b5 |
| cylinder8002c2ec/198 | d7d58e69e46ade187d4e237f59936d36c3427299b29b6952b6cd3f760c4e02b3 |
| vertices80304ce0/2d0 | bfb82d2b34d528cb2f77734dc9f219efe33b1ec9b892801ec530d3994af39341 |
| five lists80304b60/140 | f680b125fc83f6662015466b0361754eddee0d9831c30713643a527cec042d7a |

No code/shared index/progress changes. Whole native matrix/vertex/GX volume,
cube data, bounding/image services and original raster parity remain independent.
