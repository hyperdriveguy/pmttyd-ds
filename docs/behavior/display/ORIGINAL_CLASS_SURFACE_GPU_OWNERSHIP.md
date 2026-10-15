# Original class surface/icon GPU pair ownership

`battle_class_surface_gpu.hpp/.cpp` owns the existing original surface/icon
catalog and two GPU imports as one atomic resource generation. It adds no art,
layout, animation, labels, source menu decisions, input mapping or camera.
Normal class callback activation remains a separate source queue/frame owner.

## Authoritative products

`tools/battle_class_surface_assets.py` imports original US
`battle/common/battle_common.tpl` (217920 bytes, SHA256
`8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1`).
Indices0/1 provide the original40×40 circle quadrant and128×80 speech bubble.
Existing `battle-class-surfaces.bin/.t3x` uses a256×128 RGBA8 atlas, two-pixel
clamp-edge extrusion and unscaled source pixels. Original descriptors0/1 use
wrapS/T0,min/mag1. Extruded edge texels preserve linear clamp sampling at the
circle quadrant seams; original source pixels/catalog UVs remain unchanged.
Circle mirroring and bubble
geometry remain source consumers, rather than artwork changes.

The unchanged `tools/battle_class_icon_assets.py` resolves original icon.bin
IDs334..343 to the original icon.tpl image IDs339..348, each a static48×48 image.
The read-only ISO source banks are icon.bin8174 bytes (SHA256
`0be6b95a7e45e5d932641f24f8240a289a852889f1906d32be839046a680532b`) and
icon.tpl677792 bytes (SHA256
`fb8e99398fd5ef3c36cc518077748405949c004fcc5c515cc8828a64340b74f9`).
Existing `battle-class-icons.bin/.t3x` uses a256×256 RGBA8 atlas,52-pixel cells
and two-pixel transparent padding. Native sampling remains linear,
clamp-to-edge. Original rights remain Nintendo/Intelligent Systems; tracking
or privately converting the assets grants no new redistribution rights.
Original extracts, PNGs and compiled packs remain ignored, outside source history.
This owner consumes the already-built products; it does not import or overwrite
editable assets. The build's source/asset authority still authenticates pixels;
checking native texture dimensions is not a cryptographic image-content check.

## Admission and immutable lifetime

The portable resource first validates both metadata files with the original
catalog loader, then checks every region against the exact existing packing
coordinates, dimensions and UVs. Valid but displaced metadata is rejected.
Only then does it import the two images. Each import must have:

| Property | Surfaces | Class icons |
| --- | --- | --- |
| Native dimensions | 256×128 | 256×256 |
| Native format | GPU_RGBA8=0 | GPU_RGBA8=0 |
| Subtexture count | 1 | 1 |
| Subtexture dimensions | 256×128 | 256×256 |
| Rotation | false | false |
| Full-image L,T,R,B | 0,1,1,0 | 0,1,1,0 |

No implicit transpose, cropping, extra subtexture, alternate storage format or
atlas growth is admitted. Null/failed or aliased imports are rejected. Each
successful import is an acquisition requiring one release, including a rejected
import. If the second import fails, the first is released too. No partial pair
or generation is published. Existing residency cannot be overwritten by load;
the caller explicitly retires it first.

A lease strongly retains the immutable catalog, exact two image identities and
an opaque generation. Generation construction/copying is private to the owner.
`current()` requires the live generation to match both the catalog and image
pair, so substituting a different ready catalog or handle is unavailable.
`accepts()` additionally proves this resource is the actual resident owner.
Retirement marks every copied lease unavailable **before** either release
callback, then drops the owner state. Fresh residency gets a distinct generation
even if native texture storage reuses the same addresses. An old strong lease
can safely retain metadata, but cannot revive its GPU residency.

Service callback contexts must outlive the resource. Imports/releases run on the
native rendering thread. Reentrant load is refused; retirement requested during
an import cancels publication and rolls back acquired imports. Release callbacks
can inspect the already-retired lease. Destroying the owner from inside one of
its callbacks is not permitted. The native wrapper imports through actual
Tex3DS/Citro3D owners. Import/retire happen outside FrameBegin while C3D is alive;
native destruction uses C3D_FrameSync before freeing referenced storage. Host
native import remains unavailable; portable tests use labeled import fixtures.

## Exact proposed renderer/publication integration

`artifacts/class-surface-gpu-source/renderer-journal.json` is an unapplied,
unique-anchor journal. It replaces the renderer's two unvalidated texture slots
with this owner, adds a ready-only `classMenuSurfaceLease()` accessor and adds
`BattleClassBackend::surfaceLease`. It explicitly copies the lease into
`BattleClassPublication`, rejects stale native publication/admission/consumption,
and preflights the actual resident pair before any C2D flush or native GPU effect.
Surface/icon replay uses the same imported owner. Existing source ordering,
footer placement, disabled-icon TEV, shader and top-screen half-scale placement
are unchanged. Footer textures4..16 retain their separate existing GPU owner.
Font/message-icon and actual renderer/frame lifetimes remain the caller's owners.

The actual source frame binder must set `backend.assets=lease.assets.get()` and
`backend.surfaceLease=lease`, strongly pin the context and include `lease.current()`
in every source allocation/frame predicate. This prevents a held source draw
from resuming after texture retirement. Preparation occurs before FrameBegin;
the ready-only accessor cannot import during a queued callback. Successful GPU
admission must occur in the real original callback position before its outer
queue owner acknowledges completion. No fixture camera, fake callback result or
readiness-byte write substitutes for that ownership.

## Focused evidence

Portable tests reject every import dimension/format/count/rotation/UV field,
null/failed/aliased imports, altered catalog layout and unavailable services.
They verify both rollback prefixes, cancellation during either import,
retirement-before-release and fresh generations under identical image addresses.
The publication integration test uses the exact journal in a private header/source
tree to verify immutable lease copying and stale admission/consumption rejection.
Strict host/ASan/UBSan and ARM syntax gates cover the new owner and journaled
renderer. Native import uses an isolated standalone Azahar profile and genuine
C3D/Tex3DS owners; it draws the raw original atlases as test images, not a normal
radial menu. Probe result/hashes are recorded separately under
`artifacts/class-surface-native-probe/`. No original framebuffer parity, normal
menu activation or physical-device performance is claimed.
