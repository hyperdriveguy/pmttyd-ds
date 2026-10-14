# Battle pose depth: camera4 matrices and dispCalcZ

2026-10-14. Read-only original US G8ME01 disc0 revision0. This closes the
depth consumer and identifies its actual matrix producers. It does not claim
native battle framing or the full battle camera controller has been ported.
Addresses and packed offsets below are hexadecimal.

## Exact consumer and helper order

`dispCalcZ8001086c`, length94, takes r3 pointing to three source-space floats
and returns floating queue priority in f1. It retains that input pointer, then:

1. At80010888, request `camGetPtr8000f2fc(4)` and capture returned C.
2. At8001089c, call `PSMTXMultVec8029e6a4(C+11c,input,stack+8)`.
3. At800108ac, call `PSMTX44MultVec8029eb08(C+15c,stack+8,stack+8)`.
   The same temporary is input/output, supported by this helper's load order.
4. Read projected temporary Z at800108b4. At800108bc use single fused
   `FMA(5000,Z,5000)`.
5. Ordered compare against0: if below, replace with0; otherwise ordered compare
   against10000: if above, replace with10000. At800108e8 multiply by-1 with
   single rounding and return. There is no integer conversion or viewport use.

Constants at8041f760/764/768/76c are exactly
459c4000 /00000000 /461c4000 /bf800000. Do not clamp Z first or turn unordered
comparisons into a finite-value guard. A positive0 clamp subsequently becomes
negative0 by the final multiply. NaN behavior is the source ordered-comparison
domain, not a fabricated fallback priority.

`camGetPtr8000f2fc`, length14, returns `WORD(803c95f8+4*id)` without ID,
nonnull or active validation. Camera4's pointer slot is803c9608. C+11c owns
the 3x4 view matrix, C+15c the 4x4 projection matrix. The getter does not build,
update, copy or select a matrix on demand.

## Matrix arithmetic and homogeneous divide

Both SDK helpers use paired-single GQR0 loads/stores, whose hardware state is
external. XYZ W1 load supplies the other lane as1. For finite ordinary float
inputs, let R round to binary32, F be single fused multiply/add, and define
the source row operation:

```
dot(row,x,y,z) = R(F(row[2],z,R(row[0]*x))
                 +F(row[3],1,R(row[1]*y)))
```

`PSMTXMultVec8029e6a4`, length54, evaluates this for view rows0,1,2 and
stores each output float in order. Its paired grouping differs from a naive
left-associated four-term dot product. It loads source XYZ before destination
stores, so in-place input/output is admitted by its own body.

`PSMTX44MultVec8029eb08`, length78, first computes the row3 denominator
`w=dot(P[3],x,y,z)`, then a rounded paired `1/w`. It evaluates numerator rows
0,1,2 with the same grouping, multiplies each by that retained reciprocal and
stores XYZ. Thus it returns divided coordinates, **not raw clip coordinates**.
Do not implement the third coordinate as raw projection row2 dot, forget row3,
or substitute one direct numerator/w division for the original reciprocal and
multiply rounding sequence. No zero-w guard exists. All matrix/input loads
needed by this routine occur before stores to its in-place destination.

An ARM float/FMA implementation can retain this finite ordinary operation
order as a numerical adapter. Original paired-single/FPSCR/GQR0 exceptional,
denormal and NaN payload behavior remains a platform dependency. C3D tilted
projection or PICA depth conventions do not supply these source queue matrices.

## Actual matrix writers and battle admission

The [camera lifecycle](../camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md)
recovers `camInit8000fd9c` creating camera4 with callback
`cam3dMain8000ea68`, default FOV25, source aspect3fa22222, near1 and far32768.
That initializer does not establish a current battle framing snapshot by itself.

`camMain8000f704` iterates the13 camera pointers. For each C it first invokes
nonnull callback C+1ec at8000f770, passing C in r3. Then it always constructs
view C+11c with `C_MTXLookAt8029e330(out=C+11c,eye=C+c,up=C+24,at=C+18)`
at8000f784. It builds Z roll from C14c, translation from C150/154/158,
then concatenates in this order:

```
V = LookAt(eye,up,at)
V = RollZ * V       // PSMTXConcat8029dd64 at8000f7bc
V = Translation * V // PSMTXConcat8029dd64 at8000f7cc
```

There is an additional camera4/Mario transform branch only when GP+14==0
at8000f7e0..8000fa50. A real nonzero battle context skips it; do not apply field
Mario shake/size transforms merely because the selected pointer is camera4.
Finally if camera HALF0 mask8 set, `PSMTX44Concat8029e994(C+1a0,C+15c,C+15c)`
at8000fa6c applies the extra projection on the left, then clears that flag.
Both roll/offset and projection-extra fields need retained producer history;
unbound fields must not be silently zeroed to simplify framing.

`cam3dMain` first returns immediately when camera HALF0 mask2 is set. Otherwise
camera HALF2 mode2 selects the battle branch8000f1d4; GP+14 must be nonzero
to call `battleCameraMain8012c8b0` at8000f1e4. Following that call it derives
distance C118 and yaw C114 from the updated eye/at. If projectiontype C19c==0,
the exact projection request is `C_MTXPerspective8029e7e8(C+15c,C38,
FLOAT8041f6d8,C30,C34)` at8000f230: FOV, fixed source aspect, near, far.
Other projectiontype uses the original orthographic branch, not a guessed
perspective fallback. GP+14==0 retains that branch's matrices instead of
inventing a battle camera.

`battleCameraMain8012c8b0` itself gets camera4 at8012c8dc and captures global
BattleWork. At8012c930..958 it first copies packed B2780..2794 to camera
eye/at C+c..20 before its mode/controller graph. Later axis-admitted source
updates at8012db10/18,db38/40,db60/68 write eye/at after controller work.
Existing mode/zoom/homing request fields B2754..2857 are therefore inputs to
a real framing/update owner, not final source view matrices. Complete mode
controller, its initialized vector history and cadence remain the next camera
provider; this consumer audit does not replace them with request setters.

## Reusable native components and truthful binding

Current `source/battle/battle_camera_requests.cpp` ports original request
admission, mode/axis bits, zoom, speed, homing and shake writes. Its header
explicitly leaves Main/framing/priority producers external. Reuse these packed
inputs for a new camera update owner; do not label them an implemented
`battleCameraMain` merely because native scripts return from their callbacks.

`include/display/camera_road_view.hpp::fieldCameraFrameFromView` supplies an
ordinary upright LookAt/perspective mathematical adapter. It can reuse an
**already source-produced** eye/at/lens snapshot in the zero-roll/offset domain,
with declared SDK normalization/rounding differences. It currently retains a
six-coefficient GX projection vector rather than a complete typed4x4 source
matrix; `projectCameraRoot` computes viewport coordinates/depth and is a
different consumer. Neither helper supplies missing battle framing history.

At inspection `Renderer::drawMap(battle=true)` uses native fixed eye(0,2.8,5.5),
at(0,1,-.4), FieldFov42, near.1/far80 and C3D tilted handheld projection.
`Game::fieldCameraFrame` is owned by field road/event camera services, and the
trio intro feeds its source event views into that field adapter. Neither is
the original mode2 battle camera4 writer described above. They may be passed
explicitly as a best-effort native matrix snapshot adapter for queue ordering,
but must not be reported as restored original battle-camera policy.

For a minimal reviewable binding, give `poseDrawRequest` an explicit camera4
snapshot provider owning view3x4 plus projection4x4 in the same units as its
sourceXYZ input. Apply the recovered helper operations and priority arithmetic;
retain actual source camera production as a named frontier if unavailable.
Source root XYZ is engine units; a native .01 coordinate snapshot requires
scaling the input and every eye/at/near/far/translation consistently. Do not mix
engine-unit part XYZ with an unscaled native-unit matrix or substitute the
queued sprite's screen Z. Subsequent `dispEntry80010bc4` and actual unit-part
draw callback remain independent queue/draw services.

## Read-only evidence hashes

Listings/ELFs are under ignored
`artifacts/recovered-temp-evidence/calcdepth-*.dis/.elf`.

| Original body/address:length | SHA256 |
| --- | --- |
| dispCalcZ8001086c:94 | 427dfd35a380feb986c69fb690c6428ec2052a08060ae4d5fe4f4d18cbac4622 |
| PSMTXMultVec8029e6a4:54 | 0e86b49668bd9172bfd1256aa2842a4326032efab48cb6b8df58523a63992e78 |
| PSMTX44MultVec8029eb08:78 | a9ff54d2a86e2d747391bd563743295604a932ccc380d95c320b983d85df395b |
| camGetPtr8000f2fc:14 | 726c91fbc0c5f3c0a1e728d6fd9cde3f5df96a49a66c878179347fd7ce4df247 |
| camMain8000f704:3c4 | fd7074046f87e9b4806121364396793eff9fb0e43454df0d33fd46dd2d1e26c4 |
| cam3dMain8000ea68:86c | 3f53cc9fb1cd329508cbd04b88663fb664e80ab1ed9df39d0415204d3dfe57f3 |
| battleCameraMain8012c8b0:1638 | 1bc682b107a1dac6a1134f51d34cd2482ecec868c805424840d35e584c6c0edd |

Hashes pin source bodies, not runtime pointer/state values or original/native
numerical equivalence. No native code, shared index/progress or source assets
were changed for this audit.
