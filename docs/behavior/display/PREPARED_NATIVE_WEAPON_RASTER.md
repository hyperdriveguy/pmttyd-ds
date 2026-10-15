# Prepared native mixed weapon raster submission

This adapter lowers an already immutable `BattleWeaponOrderedBatch` into one
prepared native plan. It implements no original source function or menu state.
It neither completes the source weapon frame nor sets readiness, owns a source
queue, supplies a camera, invokes source RNG or imports a texture. The actual
[ordered publication](ORDERED_WEAPON_MENU_PUBLICATION.md) and
[weapon frame](NATIVE_WEAPON_MENU_FRAME.md) remain separate owners.

## Preparation and exact raster operands

`prepareBattleWeaponRasterGeometry` pins the exact shared immutable source batch,
its allocation/resource owners and an actual current font lease. It allocates
the complete native primitive vector before returning. Missing resources,
unknown source/object/camera proofs and nonfinite native geometry reject the
whole plan. The source batch must have no surviving mutable aliases. This
portable lowering validates the existing actual font/submenu/window resources;
actual resident icon proof additionally belongs to Renderer.

Each source packet becomes one typed primitive with copied transform, physical
viewport/scissor/depth, exact native texture identity, color/material/pipeline,
and either four vertices or all36 source Window vertices. It preserves packet
order and the nine Window quads' internal order. No texture/material regrouping,
transparent/zero-area/offscreen removal or synthetic vertices are introduced.
Every transformed clip lane is conservatively checked for finite products and
intermediate sums, including the last primitive. CPU preparation has no GPU
prefix to undo.

The leaf transformations remain distinct:

| Leaf | Prepared geometry/material |
| --- | --- |
| Font | Original local signed glyph rectangle; captured loaded source3x4 appended to already tilted actual projection;512x256 texel UV normalization and one T flip; RGB=C0,alpha=A0*TEXA |
| Submenu | Existing `battleSubmenuVertices` applies the actual source affine matrix; append actual projection with identity view, tilted once; copied atlas UV/color |
| Window | All36 raw source XYZ/ST; `battleWindowTransform` composes actual projection and source loaded matrix, tilted once; original72x72 area normalized into actual128x128 padded atlas, one T flip |
| Icon | Exact already prepared source `RetainedIconPacket.transform`, copied NativeIconPlan/NativeDisplayPipeline; source0..1 UV mapped through actual two-pixel image gutter and resident power-of-two dimensions |

The ordinary leaves reset all six TEV stages, attribute loaders, cull/fog,
late depth ALWAYS with color-only writes, alpha ALWAYS and source-alpha blend.
Font's RGB is primary color replacement; its alpha multiplies primary alpha by
texture alpha, with no GT8/128 cutoff. Submenu/Window multiply texture and copied
vertex color. Icons reuse the existing actual Renderer
`publishSourceIconProgram` and `finishingDisplayPipeline` setters, preserving
Normal/Grayscale/Blended constants, culling, write masks, blend/logic operation,
source depth compare and alpha rejection. Those two concrete setters perform
no allocation/query/source work and always return true on their admitted plan
inputs; they are not a replaceable fallible callback. Native sampler1/2 park on
the genuine imported font, which is separately required and preflighted.

## Real Renderer admission and no partial replay

The new `render_weapon_ordered_raster.cpp` implements actual Renderer members.
`prepareWeaponOrderedRaster(sharedBatch)` checks actual device/program/target,
original imported font generation, each actual native submenu/window handle,
and every actual icon cell generation before allocating geometry. It then
performs the same whole native preflight used for submission.

`canDrawWeaponOrderedRaster(plan)` checks the actual Renderer address and the
plan's captured opaque font generation, every copied resource/geometry proof,
actual native texture handles and icon residency. An equal Renderer/texture
address after release/reimport cannot revive the old opaque font lease. This
also covers plans with no glyphs: the real font is the required native parking
texture. Whole native checks need no original source execution or RNG.

`submitWeaponOrderedRaster(plan)` repeats full preflight **before** its first
`C2D_Flush`. Any rejection returns false with no mixed GPU prefix. Empty valid
plans return true without a flush. After that boundary the direct loop performs
only copied state setters, actual texture/program binds, matrix upload and
immediate primitives. It allocates nothing, queries no provider/resource,
constructs no leaf vectors and never calls the allocating
`Renderer::drawSubmenuPackets`. The Window texture getter journal only checks
its real resident lease/import and returns its actual C3D texture; it imports
nothing. All fallible geometry/handle discovery preceded the loop.

A narrow Renderer guard refuses reentrant prepare/preflight/submit calls.
Portable same-plan current checks also reject recursion. Source proof callbacks
must be pure, non-destroying and non-retiring; native resource retirement,
Renderer shutdown and unrelated GPU mutation are forbidden during admission
and synchronous submission. This serialization is mandatory, not supplied by
a copied bool or by the geometry plan. After the first GPU effect there is no
failure or retry path. The outer ordered owner records the one accepted
submission; this renderer intentionally does not invent per-frame/source
completion or suppress deliberate later rendering of a valid publication.

## Actual ordered owner binding

The real source allocation/bank/current proof remains externally mandatory.
Renderer validates its actual leaf resources directly; a caller bool is not a
substitute. Renderer does not interpret a fabricated batch as whole source
completion. Callers must retain the real ordered owner and use its actual
`validate` outside that owner's callback/reentry guard.

In particular `ordered.ready` runs while the ordered owner is invoking and
cannot recursively call `ordered.validate`. The real seal wrapper must first
obtain `sharedBatch=ordered.seal(pass)` and let that call return, then prepare
and retain its exact native plan. An unavailable preparation leaves the source
batch sealed and held; the already completed source parent/RNG is not rerun.
The wrapper returns the actual source batch only after preparation succeeds.

The actual `ready(user,batchRef)` requires exact identity with the retained
plan's source batch and calls Renderer whole native preflight. The required
nonfailing void `draw` wrapper must assert/abort an unexpected pre-GPU submit
rejection under that serialized ready/draw contract; it must never ignore false
and acknowledge a dropped batch. Native frame/current publication validation
combines the real ordered owner with actual resource checks outside owner
callbacks. Packet camera/source-current closures must not recurse into that
same ordered owner. They retain their real W/pass/resource proofs independently.
After legitimate camera.finish, copied raster geometry and native resource
leases stay valid only while the actual long allocation/bank proof remains
current. The prepared plan itself supplies no readiness or allocation predicate.

Exact declaration/getter replacements are unapplied in the artifact journal.
Root must rebuild a consistent Renderer ABI after its new guard field is added.
No existing shared source, build registration or original assets are changed
by this isolated batch. Native Make discovers the new Renderer file; host core
registers only the portable geometry module/test.

## Validation and remaining frontier

Host geometry tests preserve Window/font/submenu/icon/font/Window order, all
Window vertices, each distinct transform/UV/color/material/pipeline domain,
missing final resources, final-leaf overflow/nonfinite coordinates, mismatched
camera pass, retirement and actual font generation replacement at the same
texture address. A same-plan current callback reentry is rejected. Original
Window atlas test bytes are independently decoded from the read-only original
TPL; host import identities/object/camera guards are explicitly fixture owners.
Iterating the prepared primitives performs no additional provider calls.

Strict host, sanitizer and actual ARM syntax cover the new portable/native
sources and exact privately applied declarations/getter. An unchanged camera
dependency retains pre-existing misleading-indentation warnings; only that
separate dependency TU relaxes that warning's error status. New sources/tests
remain strict `-Werror`. No old archive or inconsistent header graph is used.

These tests prove geometry/preflight policy, not actual native mixed GPU pixels
or end-to-end source binding. The separate original leaf probes can provide a
future isolated native mixed test. Real process window Setup/DVD delivery,
queued source graph/GPU ownership, normal tutorial/battle activation and
original PPC parity remain separate unfulfilled integration work.
