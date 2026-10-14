# Native ordered shadow setup and camera2 ray frontier

2026-10-14. Follows the recovered
[complete setup](SHADOW_DISPLAY_SETUP_AND_EMPTY_RECORD_CONTINUATIONS.md),
[register inheritance](DUMMY_DRAW_TEV_REGISTER0_AND_SHADOW_INHERITANCE.md),
[collision cursor](SHADOW_COLLISION_PREPARATION_CAPTURE_AND_STORE_ORDER.md)
and [camera2 suffix](SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md).

`shadowDisplay` retains the original current display record admission, passed
camera/userdata, fixed typed pool captures and current-camera view pointers.
Polygon, outer cylinder and Texture independently issue their original setup
before any record admission. Each GX request retains its copied arguments
across an unavailable backend, including material/fog colors. Per-helper count
tests stay fresh. Geometry admission retains the record token across a hold;
unavailable geometry/bounding providers cannot produce a successful callback.
Changing typed owner/camera/userdata during a hold is unavailable.

Native finishing binds this continuation inside the renderer's admitted frame.
The stable runtime owns source current-record/current-camera queries, shadow
globals and cursors; callers cannot invoke its shadow API outside the captured
dispatcher call. Camera1 submits73 source GX requests (21 Polygon,28 outer
cylinder,24 Texture), plus mandatory N64 texture lookup and four camera reads.
In its live type2/unprepared/camera4 record domain all geometry branches reject
after full setup. A completed callback advances the original camera dispatcher,
including camera1's real dummy; it never clears/recollects the queue or pool.

`NativeShadowProgram` retains source TEVREG0 alpha independently of PICA TexEnv
constants. Only the acknowledged real dummy publishes129; restoration never
reseeds or draws vertices. Dummy updates only its own fields and retains
material, bound texture, arrays, fog and cull. Shadow prefixes establish actual
material RGBA, register material source, disabled lighting and raster RGB.
The native two-stage alpha program represents inherited A0*raster alpha or
texture alpha*raster alpha with their separate operands; other PICA stages
pass through. Fixed-point GX/PICA raster rounding remains unverified.

Independent blend/depth/alpha masks are updated through the existing source
class-state owner. Cull/fog and shader/texture state restore across frames after
the camera and mask restoration. Source indexed array identities/descriptors
are retained as logical input declarations, never cast to ARM pointers. The
native world shader uses XYZ/UV/color streams; an eventual geometry decoder
must translate those declarations and source matrices/UVs before admitting a
primitive. This patch does not supply geometry, bounding-box or image services.

N64 texture lookup uses a declared native eager resource adapter. It publishes
the supported loaded44 branch only after an actual Tex3DS import validates
16x16 RGBA8,no mip levels,bias0 and applies repeat S/T,linear min/mag. Exact
source/pixel hashes were verified by the private converter. The sampler and
texture bind execute even if every type1 record rejects. Source archive/DVD
language selection, full N64 palette/global load history and the actual 1x1
unloaded dummy are still separate owners; this adapter does not infer their
state from request types. Native partial failure retains the actual wait.

Camera2/class5 invokes the independent collision continuation with the actual
pool/current camera. Missing ray service holds at hitCheckVecFilter800158a4
with original captured origin/direction/length/filter. Source profile selection
and remaining camera2 image/projection work remain unavailable after completed
collision; their first query is conditional on a mutable profile. No success
or unconditional camGetPtr4 claim is made without that owner.

Portable tests verify ordered source calls, replaced arrays/descriptors,
captured material color, held texture lookup, geometry admission, source-state
seed/inheritance, camera2 miss/frontier and finishing dispatcher integration.
The explicit host services and isolated emulator's Bero-resolution fixture do
not establish production room-history provenance, original execution timing,
shadow raster/audio parity, source outer lifecycle or hardware performance.
