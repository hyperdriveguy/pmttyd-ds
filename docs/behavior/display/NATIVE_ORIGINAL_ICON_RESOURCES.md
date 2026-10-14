# Native original icon resources

The renderer now connects the [matrix wrappers](ORIGINAL_MATRIX_ICON_SERVICES.md)
and [iconGX service](ORIGINAL_ICON_GX_DRAW_SERVICE.md) to original directory/TPL
metadata, the existing battle VM GP bank, published cameras and resident native
textures. This is not a completed item raster or normal battle-frame proof.

## Private source pack

`tools/source_icon_assets.py` opens the US G8ME01 disc0 rev0 ISO read-only.
Its normal assets-ready integration writes ignored `romfs/source-icons/`:
The native3DSX dependency includes nested cells so their changes repackage ROMFS.

| Original input | Output | SHA256 |
| --- | --- | --- |
| icon.bin | icon.bin | 0be6b95a7e45e5d932641f24f8240a289a852889f1906d32be839046a680532b |
| icon.tpl | icon.tpl | fb8e99398fd5ef3c36cc518077748405949c004fcc5c515cc8828a64340b74f9 |
| DOL spans below | constants.bin | 6500b95c8d8387a35d635530dd9a0084e406ff33e740e3bdfb3dcb022202b49d |

Constants are unchanged concatenated bytes from80415040/24 decimal,
80415080/32 and8041f864/60. These retain both six-entry alias tables, dummy
image storage and original color/numeric constants. No executable PPC text is
included. `sources.json` records spans and hashes, each original image's index,
size/format/wrap, decoded pixel hash, native dimensions and converted cell hash.
Nintendo/Intelligent Systems licensing remains inherited; private packs, source
images and the ISO stay outside source history/distribution.

Preserve all572 directory entries and613 texture images, including animation
cells. The existing GX decoder converts each cell to RGBA without resizing.
Add a two-pixel gutter using original clamp/repeat/mirror rules and place it in
a power-of-two image. `tex3ds -f rgba8 -z auto` stores a losslessly compressed
native cell `0.t3x` through `612.t3x`. Converted previews/provenance stay in
ignored `local-assets/source-icons/`. No replacement artwork is generated.

The native texture uses clamp at its physical boundary. Original wrap-aware
neighbor pixels in the gutter preserve the source icon quad's0..1 boundary
samples; this does not implement arbitrary repeated coordinates outside that
quad domain. The later emitter must map source ST to the inner rectangle:
U=(2+sourceS*originalWidth)/nativeWidth,
V=1-(2+sourceT*originalHeight)/nativeHeight. Do not sample padded blank space or
report native padded dimensions as source dimensions. Hardware filtering and
original compressed-texture raster parity are not established by conversion.

## Directory and object owner

`SourceIconResources` validates the retained directory, descriptor/frame extents,
texture references, positive animated duration totals and original TPL metadata
through `AnimationTextureResource`. Missing/corrupt resource files leave it
unavailable; they do not expose a fake source zero-buffer early return.

Directory identity83000000 and palette identity83100000 are native adapter
identities, not measured original addresses or cast ARM pointers. Original
loaded globals8041e630/634/638/63c resolve to these actual ready resource owners.
Directory halves and immutable constant words/halves are big-endian original
bytes. No invented descriptor dimensions, frame delays or aliases are supplied.

TPLGetGXTexObj resolves an original image index and creates its stable opaque
index+1 token. Retain the source-used linear LOD state on that object. Bind only
after real object creation and accepted LOD, and acknowledge GXLoadTexObj only
when the actual native cell is resident and bound. Width/height queries return
original TPL dimensions even if the GPU cell is larger. The tokens model the
used immutable-image/linear-LOD domain, not general independently mutable SDK
stack objects. Invalid tokens/arguments remain unavailable. The standalone
original1x1 dummy constructor is still unbound natively; normal loaded matrix
icons use their real TPL images, and never substitute a dummy on load failure.

## GP, camera and native residency

`TrioAttack` supplies actual current-camera identity and published view words,
plus reads from its existing `vm.gpAddresses`. Camera adapter identities start
at84000000 with stride400 hex; they are not original camera RAM addresses.
Current camera must come from CameraManager's real active index. A requested
camera8 is independently resolved. Missing index, absent publication and unknown
fields stay unavailable. Views are copied in source3x4 order without another
billboard, projection, field-camera fallback or guessed pose.

GP pointer80415fd0 and registered packed fields remain owned by the existing
battle VM. Reads do not increment GP.1c or synthesize a frame clock from render
calls. Full process-wide GP import and original outer Main counter cadence remain
upstream; binding this retained view does not prove them complete. GP.16c remains
the caller's raw alias selector, with no invented story/costume override.

Renderer preparation runs before C3D_FrameBegin. It loads metadata once and
services one pending native cell outside frame submission. The cache has101 slots, covering the original100 item records plus the
single source frame of AP icon442. Slots bound residency; eviction chooses the least recently used unbound cell and
pins the currently bound texture for restored held draw state. These are native
adaptation policies derived from the complete draw bank, not measured original
cache size/latency. C3D_FrameSync
precedes imports/eviction/retirement. Validate imported format, dimensions,
subtexture count/orientation, mip levels and bias. Apply source-used linear
filtering. Failures retain the actual request rather than acknowledging it.

An in-frame bind miss queues its real image index and holds at GXLoadTexObj.
After preparation imports it, the same retained source request resumes and binds
through the real `NativeShadowProgram::SourceIcon` texture owner. Restore uses
the pinned resident object. Leaving trio battle retires cells and clears pending/
bound state after synchronization; metadata stays owned until renderer teardown.
Shutdown also retires any remaining native cells. Source loaded-buffer state is
not fabricated from population count or an empty item bank.

The native callback now binds SDK matrix math, published position/current matrix,
resource queries/LOD and actual texture binding. Map lighting, channel/TEV/fog/
depth consumers and the primitive emitter still retain explicit waits. Ordinary
loaded icons stop next at mapSetMaterialLight800208c0; flags16 takes its own
channel path. No native draw or original item presentation/input is skipped.

## Validation scope

Host tests load the complete original pack, validate all613 object tokens and
original dimensions, reject bad ownership/LOD/indices, hold/retry binds, and
exercise actual packed GP/camera reads. A real matrix wrapper resolves the
original GP-selected alias and descriptor against published camera data.
Python tests check source-wrap gutters, original pixel preservation, wrong-disc
rejection before writes, all native dimensions and every generated file hash.

A temporary native entry point exercised the actual production preparation and
resource-call/binding methods in isolated Azahar. All613 native cells plus four
revisits passed, including101 simultaneous resident slots, real imports, validated dimensions/format, eviction,
bound-resource ownership and retirement. Inspected capture:
`artifacts/azahar-source-icon-bank/boot.png`, console617/617 PASS. The probe
used the actual renderer through private member-pointer access; no production
header/renderer was patched for access. Its main was restored before final
normal validation. Probe source/logs remain ignored artifacts. User emulator
profile and saves were not touched. This is resource/binding evidence, not icon
pixels, a playable throw, normal source frame cadence or a hardware benchmark.
