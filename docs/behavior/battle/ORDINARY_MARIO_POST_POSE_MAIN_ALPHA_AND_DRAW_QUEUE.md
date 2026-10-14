# Ordinary Mario post-pose Main alpha and draw queue

2026-10-14 US G8ME01 disc0 revision0, read-only original DOL/model evidence.
Immediate btlDispMain continuation after actual animPoseMain8003ed2c for a
valid ordinary forced Mario bodypart, opaque modeP215=1, attributes9, unlinked
registered pose.

## First post-Main service and required scalar state

PartsWorldPos80128b10(P,&X,&Y,&Z) is called BEFORE animPoseMain at80102b90.
After Main returns, btlDispMain retains that pre-Main coordinate capture,
but reloads FLOAT P4f8 and builds depth vector{X,Y,single(Z+P4f8)}.
It captures part source pointer S=P4, loads SIGNED HALF S34 and stores LOW8
to BYTE P4f7. Then the first actual service at80102c00 is:

`BtlUnit_CheckStatus80127814(U,18)`, ordinary r3=retainedUnit,r4=18.

This is not another pose-owner lookup. Preserve the source return/predicate;
the native portable live status reader is reusable if it supplies actual packed
status/extra-byte owner inputs. Pending geometry/facing work inside Main must
complete or remain its specific boundary before this continuation executes.

## Alpha and RGB publication after the status query

If CheckStatus18!=0 OR unit U104 mask8 set, read U8(P4f7), compute signed
integer floor((alpha<<7)/255), store LOW8→P4f7. This is128/255, not float.5.
Then ALWAYS reread U8(P4f7) and colorBYTE P4f3; wrapping integer multiply,
signed divide255, LOW8 store→P4f7. Copy BYTE P4f0→P4f4, P4f1→P4f5,
P4f2→P4f6 in that order. Unit U311/U312 are separate from this part color lane.

Actual original Mario kind80372afc attributesWORDac=02040000, bodypart source
80372a18 SIGNED HALF34=255, attributes00000009, and original part color
P4f0=ffffffff. In the actual live status18false domain this yields alpha255,
RGB255/255/255. A later original color/status change admits the real product
or translucent branch; do not override these stores with opaque constant255.
P4f8 initially0 from fresh part memset but is a retained float owner, not a
constant offset declaration for every future pose/frame.

## Exact body draw admission and next concrete provider

Source gates in order at80102c5c:

1. U104 mask01000000 set skips this ordinary part draw region.
2. P1ac mask01000000 set also skips it.
3. If P1ac bit1 set and U104 mask10 set, skip the color-part callback region
   to80102cfc. Nonbody part bit1 clear does not apply that unitmask10 test.
4. If U8(P4f7)<255, select translucent-class candidate without TestXLU.
   Otherwise call **animPoseTestXLU80039c0c(P1c0)** at80102c94.
5. TestXLU nonzero selects class2, zero selects class1.
6. In either selected class, P204 mask100 set suppresses registration.
   This is mask100, distinct from the pending-name mask200 consumed earlier.
7. Otherwise **dispCalcZ8001086c(&depthVector)** then dispEntry80010bc4
   (camera4,chosen class,btlUnitPartsDisp80101774,parameterP,returned floatpriority).

For actual opaque ordinary Mario attributes9/Uattrs02040000 these gates
admit TestXLU then opaque-class1 when its source model metadata is clear.
No top-bank4, linked-empty or previous enemy-removal shortcut encloses the call.

TestXLU80039c0c/38 reads current animation work/pool, recordresource10,
current resource table/fileHandle→loadedData→MODEL WORDc4; returns its numeric
mask8 as scalar0/1. It does not inspect current animation descriptor, material
alpha or the pose active flags. Original read-only b_mario serialized header
WORDc4=00000001, therefore bit8 clear, return0 for that actual loaded model.
The loaded pointer layout and model flags must belong to the typed resource
owner; do not use ActorAnimationPack's first-pose alpha to answer this service.

dispCalcZ8001086c/94 first requests **camGetPtr8000f2fc(4)**. This is the next
concrete camera-owner frontier after the locally closed TestXLU/scalar work.
It applies PSMTXMultVec8029e6a4(camera+11c view matrix,depthVector,&temporary),
then PSMTX44MultVec8029eb08(camera+15c projection matrix,&temporary,sameout).
Read projected Z; single FMA(5000,Z,5000), lower-clamp0 then upper-clamp10000,
multiply single-1; return floatpriority. Source constants:
8041f760=459c4000/5000,8041f764=00000000/0,
8041f768=461c4000/10000,8041f76c=bf800000/-1.

Camera pointer/view/projection production and GPU/raster conventions remain
real providers. A declared native matrix/viewport adapter can project the
source priority graph; substituting priority0 for missing camera state does
not establish the original queue. Registered btlUnitPartsDisp callback and
later shadow/effect/pose-sound/footer services remain separate active frontiers.

## Main optional facing tail and actual fresh M_R_1 domain

After timeline sampling/cache/interpolation, animPoseMain's final local tail
at80040540 reads pose RECORD Rflags0 mask8. Clear preserves FLOAT R78 and
skips all facing requests. Set first calls **camGetPtr8000f2fc(4)**, reads
camera target-minus-positionX/Z (18-c,20-14), negates them, then
atan2_802725a0(f1=-deltaX,f2=-deltaZ), frsp and source factor42652ee4 to degrees.
It wraps source loops, subtracts fresh R74, wraps difference again and stores
R78=-1 for difference(90,270] or[-270,-90), otherwise+1. Exact formula and
edge/numeric domains are [the Main facing authority](../display/ANIMATION_MAIN_CAMERA_FACING.md).
Unlinked R90=-1 suppresses earlier recursive Main, not this flag8 gate.

Fresh successful mode1 battle Entry scans free flagsWORD0==0, ORs active1;
it imports no model/animation flag into recordmask8. AllocBuffer's complete
body assigns workspace/cache data without writing that bit. SetAnim force1
can conditionally OR4 when preexisting mask2 is set, but never OR8. LocalRate
only writesR44; Main itself does not writeRflags0. Ordinary _pose_def's actual
unlinked SetEffect(handle,0,0) clears materialLightECbit1 and keeps recordmask8.
Those used fresh providers therefore preserve recordflags8clear for b_mario
M_R_1; M_R_1 descriptor data does not independently admit this optional tail.

This closes a specific initializer/producer history, not a universal no-facing
claim for reused/field/paper/linked poses. A real later PeraOn/group/flag producer
must set the retained bit and enter camera/atan2 publication. Native must check
its actual typed record; it cannot set/clear8 to evade missing camera providers.
Even when Main facing skips, admitted post-Main dispCalcZ still needs camera4.

## Evidence and validation

New ignored full TestXLU/CalcZ and AllocBuffer listings post-main-* under
artifacts/recovered-temp-evidence/; original b_mario local payload read-only.

| Body/span/address/size(hex) | SHA256 |
| --- | --- |
| TestXLU80039c0c/38 | ce4582effd708ec862cfa109e96b913fff24fdb22d72bd466e1ff97d57d92b0f |
| CalcZ8001086c/94 | 427dfd35a380feb986c69fb690c6428ec2052a08060ae4d5fe4f4d18cbac4622 |
| PostMain80102b9c/160 | 9bbf159b1fa7b28eac14aece4841a5d616a04177e8d012db074532d245aeda02 |
| MainFacing80040540/108 | 478452539d429ed0b4dd7eb4e04d74ed1470cee47d98d977dd0f82848c32b19a |

Read-only source/model/mask/order tracing and scoped documentation diff check.
No native test/build/emulator or complete display/facing/backend equivalence.
