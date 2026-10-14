# Native shadow collision cursor and texture import

2026-10-14. Implements the portable preparation suffix in
[collision captures/store order](SHADOW_COLLISION_PREPARATION_CAPTURE_AND_STORE_ORDER.md)
and packages the original [N64 texture44](SHADOW_TEXTURE44_RESOURCE_AND_SAMPLER.md).
This does not acknowledge shadowDisp or bind an original collision query.

`shadowPrepareCollision` captures the fixed primary pool before its required
current-camera query. A held camera query keeps that capture. Record admission
uses radius, prepared bit, type and captured flags; a held ray retains its
original XYZ+(0,1,0), direction,150-unit length and filter8002d380 identity.
Completed hits use fresh radius/height/flags, the raw point/normal, separate
binary32 arithmetic and original matrix store order. Misses preserve matrix
and flags. Count is read on every new loop test; completed earlier records do
not replay during a held query. Type3 or collision-disabled records execute the
recovered service-free PSMTXTrans leaf locally before fresh diagonal overrides.

The native owner has a fixed64-record pool and stable typed identity. Changing
the supplied owner while a cursor is active is unavailable; original mutable
global pointer replacement is outside this admitted domain. There is no
allocation during the scan. Explicit volatile binary32 boundaries inhibit FMA
contraction; exceptional floating-point flags and NaN payload parity are not
certified. Tests cover holds, mutations during holds, fresh appended records,
hit/miss/gates, signed zero, NaN admission and missing providers.

`tools/shadow_texture_assets.py` verifies the complete original US bank hash,
descriptor44/header/sampler and decoded pixel hash before converting the exact
16x16 I4 image to native RGBA8 tiling. No scaling, atlas padding or mipmaps.
Make's base asset pass packages the private t3x and provenance/sampler JSON.
The sampler metadata requires repeat S/T and linear min/mag at native binding;
importing this file does not establish the original N64 palette loaded flag.
The source's actual unloaded 1x1 dummy branch remains a separate obligation.

Production finishing still holds at shadowDisp8002d3d8 on camera1. Complete
[ordered setup](SHADOW_DISPLAY_SETUP_AND_EMPTY_RECORD_CONTINUATIONS.md), retained
GX state and actual texture lookup/bind must precede return even in a rejected
record domain. Camera2/class5 then requires this preparation owner with a real
independent filtered collision provider and its remaining image/projection
services. The portable helper is ready for that binding; no synthetic floor,
prepared flag, zero-work callback success or original runtime/raster claim is
introduced. Full prologue, field return and hardware performance remain open.
