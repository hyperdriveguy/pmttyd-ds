# Original radial class surface packets

US G8ME01 revision0. This is a finite native packet producer for the original
circle/bubble textures0/1, used by original MainIcon in both class and weapon
menus. It owns neither command scheduling nor source readiness. Physical
Circle Pad/D-pad/A/B/X behavior stays with the original controllers; this
module adds no command touch rectangles.

`battle_class_surface_packets.hpp` separates the original source GX object,
common-bank allocation, actual executing camera8, and resident native class
surface pair. It never treats a texture index as an SDK object address, uses
submenu texture2/3 storage, or supplies missing source fields.

## Source evidence

`python3 tools/verify_battle_class_surface_packets.py` opens the original ISO
read-only and composes the existing whole circle/bubble, InitSub/Rasta/Quad/
Vertex and TEX/GX verifiers. The common bank is
`battle/common/battle_common.tpl`, 217920 bytes, SHA256
`8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1`.
Original image0 header314(hex) is40×40 I4/GX format0, pixels10c0;
image1 header338 is128×80 I4, pixels13e0. Both use CLAMP S/T, linear min/mag,
zero bias/edge/minLOD/maxLOD. Original rights and the existing asset conversion
are unchanged. No new art/pixels/conversion are introduced.

Both source circle and bubble set ColorIn `{0,15,10,2,15}` and AlphaIn
`{0,7,5,4,7}`: RGB is raster RGB times C0 RGB; alpha is raster alpha times
texture alpha. C0 alpha is unused. The packet keeps C0 separate from every
source raster RGBA; it does not pre-multiply alpha, quantize CPU color products,
or accidentally multiply RGB by the I4 texture a second time. InitSub's original
texture-times-raster RGB is the other finite admitted profile.

The original retained circle/bubble owner is reused unchanged. Its Init2D
request composes whole retained InitSub80103a14/134 including Rasta80103c3c/94
and22 exact original GX calls. Quad801037c4/124 uses the existing retained
source helper. The new Vertex801038e8/30 bridge preserves its exact sequence:
X/Y FIFO, fresh Z word80422240, Z FIFO, four low-byte RGBA FIFO stores, U/V FIFO.
It retains a successful Z read and each acknowledged FIFO lane across waits.
Actual source XY/UV operands and the original eight circle quads/two bubble
quads come from the unchanged source parent, rather than native rectangles.

## Ownership and continuation

A genuine texture query must execute TEX80037a80 through the actual SDK object
owner and return its real private output address, all32 known output bytes,
original I4 header/image metadata and an actual current typed witness. That
witness compares the supplied output address/known bytes/metadata against the
constructor result at the original LoadTexObj boundary; unknown bytes hold.

At that actual load, the provider creates an independent opaque immutable
constructor-result snapshot. Packet validation mechanically compares its
handle/known32B/metadata and canonical owner against this snapshot, then checks
long common-bank/source-resource ownership. It never rereads the original
source temporary cells after the load. Legitimate later TEX constructors may
reuse/overwrite/retire those cells without invalidating earlier loaded packets.
The snapshot does not retain the temporary callback/owner or a Context/publication
back-reference, and does not manufacture an incarnation generation. Native
atlas and long source-bank retirement still invalidate old packets.

`readBattleClassSurfaceTexture` optionally validates the actual relocated
Battle→163f8→a0→palette chain and original97-entry common palette. Any missing
header/pointer/byte is unavailable. TEX's original void completion is not
source UI readiness or GPU submission.

The camera proof must originate from the actual WeaponNativeFrame camera8
bridge during the exact invoking80121ce8 queue callback, successful CameraMain
publication, actual W allocation and pass. Copied projection/viewport/scissor
remain immutable. Long publication current must validate allocation and source
resources independently after camera.finish; it must not require an expired
short invocation. No hardcoded selector projection is supplied here.

An explicit `ordinaryCurrent` proof covers the actual captured inherited source GX history (fog, write masks, swaps and indirect state) compatible with this finite path. It is separate from resource/current-camera ownership: InitSub does not write every global GX field. Missing real history holds before any finite GX acknowledgement and can resume only when that same real owner supplies the proof. Loss after the profile was proven poisons the invocation. No default global-ready flag is supplied. Its owner retains immutable source-profile evidence through publication, rather than requiring live GPU state to remain unchanged after camera.finish.

Query `sourceResourceOwner` is the actual common-bank identity. Query `lifetime` separately pins stable query/proof userdata; the packet retains it as `sourceProofOwner`. These control blocks need not match. Actual callbacks must use independently owned snapshots or weak outer-context references, avoiding a Context→publication→packet→Context cycle and raw userdata pinned only by an unrelated runtime token.

The source service context must outlive every retained parent/child. Its word
reader supplies actual source storage. Its low-level request callback routes
to the same `BattleClassSurfacePacketProvider`; this is not an accept-all GX
callback. The provider admits only its finite original state tuples and requires
complete descriptors/formats/source texture/matrix/material state before Begin.
It captures36 FIFO lanes for each real four-vertex quad. On final-lane sink
failure it retains the same immutable packet and scalar operands; it neither
restarts the primitive nor repeats a successful append. Changed held original
requests, recursive entry, observed owner loss, unsupported state and nonfinite
geometry hold. Owner loss is sticky for the invocation. Actual VM retirement,
not another render frame, abandons source work.

Validation checks binary32 transform products/sums, mapped vertices/UVs and all
four actual native clip products/sums before packet admission. Finite singular,
transparent and offscreen geometry remains valid; there is no visibility cull.
Native packet rejection is a finite port admission policy, not PPC/FPSCR or
hardware pixel equivalence.

## Ordered and native integration

The exact unapplied journal under `artifacts/class-surface-packets-source/`
extends frozen6458's existing ordered publication and prepared native plan.
It appends class alternative4, preserving Font0/Submenu1/Icon2/Window3. Class
append requires the actual matching W generation/pass and common-bank owner,
resident class pair and full original object/camera/geometry proof. Pending
font reservations block class final FIFO admission before any glyph RNG or
source replay. Whole mixed preflight validates every class packet before GPU
state mutation. Retirement of one class atlas/object invalidates the whole batch.

Prepared class primitives retain original source order, separate C0/RGBA,
actual atlas UV regions, true copied camera projection/raster and native texture
identity. The renderer journal binds source material RGB to a PICA constant
and uses texture only for alpha on the material profile. It does not use the
older class renderer's fixed selector transform. Geometry/resource work occurs
before the first GPU effect; prepared replay invokes no source/RNG/allocation.
Actual native owners must serialize retirement through the admitted draw.

## Validation and limits

Focused tests execute actual TEX/GX bodies on original ISO headers/constants,
then unchanged complete source circle/bubble up to the genuinely missing icon
child. Eight circle/two bubble packets are produced. The synchronous packed
circle's successful GX/FIFO trace matches the retained path. Every successful
source/FIFO boundary is held twice, including fresh Z and final append; retries
at the missing icon do not replay surfaces. Tests also cover sink reentry,
missing constructor objects, real alternating circle/bubble TEX writes into the same private32B output, captured snapshot survival after temporary retirement, independent long source/native retirement, finite overflow,
font-reservation barriers and exact mixed class/font/icon ordering/prepared
geometry. The typed publication parent in that last test is explicitly a TEST
owner, not original WeaponFrame sourceComplete or original font RNG execution.

This package does not bind normal Renderer weapon callbacks, initialize font/
window startup/global GX state, implement centered FontString80076a10 or its
width helper, acknowledge missing icons/fonts, or activate normal battle.
Actual WeaponMenuBinding, original cost/label/SDK/source state providers and
whole queue/source/GPU completion remain required. No source ready flag is
written by this adapter and no emulator/hardware pixel parity is claimed.

See [ordered publication](ORDERED_WEAPON_MENU_PUBLICATION.md),
[prepared native raster](PREPARED_NATIVE_WEAPON_RASTER.md),
[retained source circle/bubble](../battle/CLASS_CIRCLE_AND_SPEECH_BUBBLE_DRAW.md),
[class GPU ownership](ORIGINAL_CLASS_SURFACE_GPU_OWNERSHIP.md), and
[submenu packets](ORDERED_ORIGINAL_SUBMENU_PACKETS.md).
