# Original retained window frame and submenu surfaces

`window_primitive_continuation` owns the complete original US revision0
`windowDispGX_Waku_col8007b44c/68c`. This is the actual window child called by
weapon DrawHelp80117c1c at80117c64. No replacement panel rectangle, text, source
readiness byte or source camera is introduced. Existing `type0WindowFrame`
remains a useful synchronous geometry reference. The existing TexturePlane
Init/PlainGX children have different material/attribute state and cannot replace
this function's GX body.

## Complete source sequence

Capture type, borrowed color pointer, X/Y/width/height/border. Call actual
current camera8000f2e4 before reading completion word8041ea1c. Completion0
returns without drawing, even if other providers are absent. Low16 nonzero
type normalizes to0; the later original shifted/masked texture offset is
preserved, including the high16-only argument domain.

Original calls then set cull0, depth-location1, alpha compare7/0/0/7/0,
blend1/4/5/7, depth1/7/0, fog0 with actual source float80420538 and copied
source color80420528, clear descriptors, direct POS9/UV13 and original
attribute formats, texgen0/1/4/60/0/125, channels0, one texgen/TEV stage,
order0/0/0/255, add/clamped color/alpha ops, color inputs15/2/8/15, alpha
inputs7/1/4/7, swap0/0/0 and current matrix0.

Only at8007b64c is borrowed caller color loaded; the subsequent TEV color1
request retains that private copy. The controller calls actual SDK identity,
then concat with the captured source camera's matrix+11c, loads position
matrix0, sets current matrix0 again, freshly reads the texture-object array
pointer8041ea24 and loads its original type-selected entry into map0. No source
texture object or readiness flag is manufactured from native GPU residency.

Begin is QUADS128, format0,36 vertices. The complete retained FIFO has180
individual float stores with explicit source PCs. Original binary32 additions
and subtractions execute once at their actual source locations. Original zero,
thirds and one constants are read freshly at every original lfs, interleaved
with stores; they are not preloaded into a convenient mesh. The original UV
thirds and all nine source cells, including center, are retained. No finite
clamp, border adjustment or alternate tessellation is added by this owner.
SDK/GX providers must retain their own internal work when unavailable. Successful
requests and source reads never replay because a later operation holds.

The typed matrix provider receives a private retained identity/result and the
fresh original camera view. This portable provider boundary is not a claim to
original Gekko arithmetic/SDK internal instruction parity. Actual native camera
mapping, owner epochs, projection and numeric services must be admitted by the
caller. No fallback camera/address domain is supplied.

## Admission and tests

Service/user pointers, borrowed color identity and input float bit patterns are
fixed through completion/reset. Reentry/reset during a callback is rejected.
An explicit `current` predicate guards each provider boundary. Observed owner
loss poisons the invocation and prevents successful child replay after apparent
revival. The real caller and color object must remain alive through holds.
Missing source reads/GX/matrix providers remain unavailable. A source zero
completion value is a valid no-draw success; a missing completion read is not.

Focused tests compare every FIFO value with the existing original type0 mesh,
then hold each successful request/read/matrix twice and compare the entire
successful trace. They cover late borrowed-color capture, retention after its
load, fresh post-wait constant reads, partial matrix output on false, high16
and nonzero type arguments, unavailable camera data, completion0 short circuit,
service replacement, reentry/reset and post-call lifetime loss. Source verifier
pins all1,676 original bytes, all linked children and immediate GX operands,
and every retained FIFO source instruction/constant load; it counts180 floats.
Static/source geometry checks do not prove native GPU raster equivalence.

## Original submenu resource owner

`battle_submenu_surface_assets.py` reads original
`battle/common/battle_common.tpl` (217920 bytes,
SHA2568eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1)
read-only. Original index2 is16×24 CMPR at header35c; index3 is8×24 CMPR at
header380. It decodes unchanged pixels into a32×32 RGBA8 atlas with two-pixel
clamp-edge extrusion: index2 at2,2; index3 at22,2. Source wrapS/T0 and
min/mag1 require edge replication under linear filtering; transparent gutters
would incorrectly fade opaque side edges. All source pixels and catalog UVs
remain unchanged. It writes original-index catalog
`battle-submenu-surfaces.bin`, native `battle-submenu-surfaces.t3x` and
provenance JSON into an empty destination. No editable checked-in art is
replaced. New extraction also requires an empty private cache. Original rights
remain; no new distribution license is granted.

`BattleSubmenuSurfaceGpuResource` validates both catalog records/EOF and the
actual one-image native import:32×32 RGBA8, one unrotated32×32 subimage, full
UV0/1/1/0. It publishes one opaque generation only after admission. Every lease
retains the immutable exact original-index catalog; retirement invalidates all
leases before calling native texture release. Texture-address reuse cannot
revive an older generation. Native import/release is a real Tex3DS/Citro3D
owner, synchronized while C3D remains live. Native clamp-to-edge/linear
sampling is an explicit atlas adapter choice, not measured original GX filter
parity. Prepare outside FrameBegin; queued
callbacks require a current lease. No lazy import occurs in ready-only access.

Tests admit the actual converted ISO catalog with explicit host GPU descriptors,
reject each wrong dimension/format/count/UV/rotation/null handle, reject corrupt
catalog before import, test recursive load and retirement during import, and
prove stale leases remain rejected after reload. Python checks exact source
pixels survive atlas placement and refuses nonempty output/cache. Strict host,
ARM and sanitizer evidence is recorded in the frozen handoff manifest.

## Integration frontier

The exact Renderer journal adds explicit preparation, ready-only lease/resource
access and shutdown retirement, implemented in the new native renderer source.
It adds no normal scheduling/activation and acknowledges no missing draw child.
The producer must add these three compiled products after ISO-backed base
conversion and before the checked-in editable manifest compiler; use fresh
private staging for this converter, then copy compiled products into RomFS.
The existing circle/bubble0/1 and footer4..16 atlases remain distinct.

A complete weapon80121ce8 callback still needs its own genuine current-frame
Camera8 admission/allocation lease, actual source window palette readiness and
texture objects, and an ordered submenu GPU packet sink with these leases.
Existing `BattleClassBackend::appendFooter` rejects indices2/3; do not relabel
submenu surfaces as footer packets or spoof class8012210c. The help window's
frame texture is original `f/msgWindow.tpl` descriptor0 (72×72 CMPR), not either
common-bank submenu image. Existing `Renderer::sourceWindowFrame` has a declared
half-size viewport adaptation; it is not an actual Camera8 numeric backend for
this child. Missing font/message/source-window/numeric/texture providers remain
real holds. Original physical input and lower-screen audience continuation are
unchanged; nothing here introduces touch-only controls or fabricated readiness.
