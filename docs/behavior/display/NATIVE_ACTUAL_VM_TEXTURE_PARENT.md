# Original raw animation texture parent

`AnimationTextureBinding` connects the raw render parent's seven independent
arguments to complete `animSetMaterial_Texture`8003dcfc. It does not reconstruct
a BattlePose or substitute converted file offsets for current guest pointers.
The existing [typed adapter](NATIVE_ANIMATION_TEXTURE_BINDING.md) remains a
separate consumer. Source evidence is the complete original 0x5ac-byte body,
[texture object contract](ANIMATION_TEXTURE_OBJECT_SETUP.md) and
[coordinate matrix contract](ANIMATION_TEXTURE_COORDINATE_MATRICES.md).
`tools/verify_animation_texture_light.py` pins the original body/getter/data.
This is static source recovery and native execution, not a Gekko pixel oracle.

## Captures, children and lifetime

The input retains signed slot count, selector array, base records, mutable state
records, signed descriptor count, descriptors and texture resource separately.
All address arithmetic wraps 32 bits. Signed nonpositive counts skip both loops
but still read the second pass's original zero/one gate literals. There is no
source eight-slot guard, index/selector bounds validation or finite test.

First-pass selector order is reversed. Base index precedes the state byte;
invalid indices execute the real diagnostic path before descriptor 0 and wrap
capture. Descriptor kind is read before TPL index, including skipped kinds.
Both original TEXGet calls remain separate. Each resolves the current resource
handle chain; the first palette pointer determines the branch, while the second
pair supplies later header reads. Actual-VM TEXGet implements only its original
word[payload+8]+8*index calculation, without TPL loader validation.

Wrap/default fields remain captured across TLUT initialization. Image pointers
and dimensions/data are reread after TLUT initialization; LOD headers are reread
again after object initialization. The original lfd conversion-bias load is one
atomic 8-byte primitive: unavailable backing retains no high-word prefix. Native
admission currently requires one complete raw span; a pair of semantic providers
is not silently combined. Both byte conversions reuse that captured bias.

All first-pass slots finish before second-pass selection/state reads. State
capture order is angle,sx,sy,tx,ty. Gate zero/one retain their earlier values;
operation pivot/degrees/zero/one constants are read only when reached, after
previous helpers return. Translation preserves the original sy!=zero gate and
separately rounded (one-ty)-sy. The native scalar provider admits finite
binary32 multiply/subtract and the original unsigned-byte bias; unsupported
numerics hold explicitly. This is native policy, not original validation.

The original dispatch table is read after component construction. Captured
branch targets select the original R, M, S and ordered products, including
(R*M)*S with its actual scratch aliases. An unsupported captured target stays
held; changing the table cannot replace an already captured branch. Generator
setup precedes selected matrix upload. Four local matrix identities preserve
source scratch/scale/translation/rotation; the shared SDK engine implements
actual pair captures and math. Availability flags prevent consuming an
uninitialized native matrix as a fabricated source value.

Object/TLUT/matrix references are tagged native local identities. They never
become invented guest stack addresses. GX children own any partial local stores
while returning false. Object init, LOD, palette/texture loads, generator setup,
matrix upload and diagnostics remain explicit backend services. Missing GX
services never acknowledge the helper. The binding, VM, caller locals and
resource owners must outlive all pending calls. Request tuples, locals identity
and source-service identity are pinned across holds; unrelated calls and FIFO
cannot bypass a retained texture operation. A pure current predicate guards
all VM/helper work. The native visit budget is scheduling, not source timing.

## Original boot data and native boundary

The existing compiled boot image now includes unchanged original texture
dispatch 32 bytes, unsigned-byte bias 8, diagnostic format plus alignment 220,
and positive/negative pivot 4 each. Existing spans already provide gate ONE/ZERO,
degrees factor and SDK literals/tail. Source paths/provenance and licensing are
in [ASSETS](../../ASSETS.md). Runtime imports preserve live mutable bytes.
No image/geometry/music/dialogue conversion or editable-asset replacement occurs.

Tests inject a hold at every recorded ordinary/CI primitive, verify live helper
mutations and first-branch retention, diagnostic wrap capture, fresh second-pass
IDs/state, negative/nine-slot counts, atomic bias repair, all eight dispatch
routes with independent slot tuple goldens, and native shared-SDK identity and
nontrivial rotated/scaled/translated matrices. Actual original boot data supplies
the native test literals. GX acknowledgements and synthetic metadata in these
tests are explicit fixtures; no native texture residency or raster proof follows.

This supplies a prerequisite for the
[Crump encounter requirements](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).
It does not install normal field/battle scheduling, a live TPL resource loader,
GX object/TLUT decoding/upload/residency, material/TEV state or complete native
fan submission. Those owners must compose with retained field/battle leases
before ordinary gameplay can claim the complete original rendering path.
