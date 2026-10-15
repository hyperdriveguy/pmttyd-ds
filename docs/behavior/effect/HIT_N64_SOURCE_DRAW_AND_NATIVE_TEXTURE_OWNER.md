# Original HitN64 display, texture lookup and native texture ownership

This follows the frozen [Entry/Main contract](HIT_N64_ENTRY_MAIN_AND_DISPLAY_FRONTIER.md).
The new source draw module implements whole effHitDisp801c5d6c/450, its main_dl
801c5b58/214 child, whole effSetVtxDescN64801d771c/c8 and tri2801d76a0/50. A
separate retained texture-object helper implements whole effGetTexObjN64
801d78b0/60. Native texture import is a genuine GPU prerequisite; complete native
GX material/geometry replay, original effect-pool scheduling and normal audience
interruption remain separate owners. No shared renderer or battle activation is
changed by this batch.

## Source draw and retained child ownership

The first actual camGetPtr call remains even though its result is unused.
Display captures the actual effect payload/age/kind and multiplies progress by
original5. fctiwz uses the supplied real conversion provider: only frame>4 exits;
negative frames retain original signed behavior and address calculation. No
clamp or guessed animation rectangle is added. Actual source position, captured
scale and camera4 float114 feed translation, rotation and scale SDK requests in
original concat order. The source animation-fraction alpha uses original signed
conversion bias,256 multiplier and low8 result; root alpha is read from actual
payload2c, which Entry **does not initialize**. Do not fill it with255.

Kinds1/2/default retain original fire-table/color branches, including kind1's
unscaled first red channel and kind2's even-age swap. Original scratch BSS words
804296e0/e4 are read even though all their local color lanes are overwritten.
They require the actual mutable owner, rather than an immutable asset fallback.
Both original TEV stages, color/alpha inputs/ops/orders and packed RGBA writes
are emitted as exact source-function requests. Texture74's same actual constructed
object is loaded into both slots0/1. Its token denotes retained native object
ownership for the original private output storage; it is not a guest stack
pointer or the original void constructor's incidental return register.

Original TexGen matrices30/33 and three axis-vector rotations0/120/240 degrees
remain ordered. Each pass calls the complete retained main_dl child with stable
borrowed base storage. main_dl reads that base only at the reached concat,
after prior SDK work; a hold does not copy it prematurely. It constructs original
texture matrices, including the frame>=4 zero second matrix, walks particles
1..signed count-1 with fresh count reads, captures their original direction,
random angle and XYZ, and multiplies by the **actual** camera4 view at11c.
No identity projection/camera is substituted by the module.

The complete vertex helper emits original array/format setup: index16 position9,
color11 and UV13, position S16x3 at+0, UV S16x2/fraction5 at+6, RGBA8 at+10,
all stride14. Source vertex address is803a0458+frame*56. Source GXBegin is
triangles144/count6. Whole tri2 emits18 halfwords, six index triples:
0/0/0,1/1/1,2/2/2,0/0/0,2/2/2,3/3/3. Atomic FIFO acknowledgment means a false
consumer consumes no half primitive. Native owners must retain the GX request
stream and preflight the whole ordered publication before actual GPU effects;
this producer does not authorize acknowledging a missing shader or array owner.

All SDK/GX/conversion/FIFO requests latch before holding. Completed setters,
matrices, texture construction and geometry do not replay. Every provider table,
input identity and borrowed base lifetime stays fixed through a continuation.
Recursive admission refuses progress. Native owners must retain/cancel actual
resource leases and queued publications; resetting a source cursor does not
retire a GPU object or effect payload.

## Actual N64 texture-object state

The texture helper reads actual global80419a40, captures its state pointer and
reads state+4. If nonzero it reads actual palette word0 and requests original
UnpackTexObj80037a80(palette,index,privateOutput). If zero it requests original
GXInitTexObj802b72b0(privateOutput,pixels80419a60,1,1,I4,clamp,clamp,noMipmap).
That fallback is a **real source branch** and needs actual source pixels/state;
missing bank ownership does not permit fabricated zero state. The request/output
object is retained through waits; changing globals afterward cannot redirect
this already reached original child. No magic palette token is manufactured.

Additional complete-body hashes, independently verified read-only during export:

| Source helper | SHA256 |
| --- | --- |
| effGetTexObjN64801d78b0/60 | 3532b321a207e74d818bdabd00a2a0415261ead29507030b0eac166bd6f5d08f |
| effSetVtxDescN64801d771c/c8 | e35872f0f5164edadfb6f11b4ade065b49e9f9c35160402cd47c720bac881768 |
| tri2801d76a0/50 | af63fdb3a5065d4602fed504c2983eb16046e124164fee375a0637f28463965d |

## Original private products and immutable source reader

Run `python3 tools/hit_n64_texture_assets.py --output
local-assets/hit-n64/pack`. The ordinary converter output is ignored romfs/.
Original e/us/effect_n64.tpl provenance and texture74's source dimensions,
sampler, header/data offsets and original rights are in the Entry/Main contract.
This converter verifies complete parent/helper hashes and exports:

- hit-n64-texture74.t3x: exact32x128 I4 decoded into RGBA8 intensity in all four
  channels, original dimensions, no resizing/padding/atlas/mips; native repeat
  and linear filtering are mandatory.
- hit-n64-bank.tpl: unchanged original235776byte151-image bank for a future
  genuine source file/palette owner. Export alone is not that owner.
- hit-n64-texture74.bin:476-byte PMHITX01 native companion, described below.
- hit-n64-texture74.json: complete source/provenance/helper/table/product hashes,
  decoded-pixel hash and explicit readiness limits.

PMHITX01:8-byte magic, LE words(index74,width32,height128), exact36-byte original
TPL image descriptor, original280-byte vertex bank, original48-byte rodata at
802fb0a0, original52-byte count/animation/radius/fire table span80419888, and
original40-byte scalar span80425458. All original spans remain BE bytes, including
padding. The reader validates exact source fingerprints and EOF before admission;
its native FNV guards are trusted-pack integrity checks, not cryptographic
security. The export manifest retains authoritative SHA256 source/product hashes.

`HitN64TextureResources::byte/word` expose **only** these admitted immutable
original spans and vertex bank at their source addresses. They return unavailable
for actual cameras, effect payloads/pool, mutable State64 globals/fallback storage,
palette pointers and BSS804296e0/e4. Prefer the actual existing VM owner where
it already maps a source span. Do not overlay an immutable copy over mutable
source state or use this resource to invent any global pointer.

The full raw bank's identity is not interchangeable with image74 or another
same-named effect pack. Palette/resource binding requires the actual original
file owner and type4 relocation. The native image metadata cannot stand in for
that source pointer chain.

## Native GPU texture prerequisite and remaining replay

`NativeHitN64Texture` imports the actual Tex3DS texture only on3DS. Successful
import requires admitted source metadata, RGBA8 and actual32x128 dimensions;
then it applies repeat S/T and linear min/mag. Host load stays unavailable.
Generation is assigned only after successful import. Generation exhaustion
refuses new imports instead of reusing identities. `current(resource,generation)`
requires the exact resident immutable resource pointer and live generation;
`bind(slot0/1,resource,generation)` refuses stale/missing leases. Import failure
retains the prior valid owner. Release invalidates leases before C3D_FrameSync,
then frees real imported texture storage while C3D remains alive. Call load/
release outside FrameBegin. The caller owns the NativeHitN64Texture object's
strong lifetime through queued work and must release before C3D finalization.

This is texture ownership, **not** complete GPU replay. A real native consumer
still must retain ordered GX state, both texture-coordinate matrices/slots,
original TEV material and index16 arrays, and admit the whole batch with a live
source allocation/camera/resource lease before any GPU effect. It must discard
stale partial publications. No renderer hooks, fake native texture-object handle,
normal callback success or camera switch is supplied here. The genuine1x1
source fallback also needs a separate actual pixel/object owner; this image74
owner deliberately cannot acknowledge it as if it were image74.

Root must integrate actual EffectPool init/context/Main/release and real entry
heap allocation, read-only source spans plus mutable BSS/State64, callback queue,
source palette/file lifetime, SDK/camera and whole GPU material replay before
normal _attack_aud mode0 can finish. Original physical/touch selection and later
flag40 interruption ownership are unchanged by these portable prerequisites.

Original Nintendo/Intelligent Systems rights remain retained. These local
original-pixel conversions and source records grant no redistribution license.
No ISO, original binary payload, decoded image or compiled pack is tracked.

## Focused validation

C++ tests execute complete source display/main_dl with existing finite SDK math
adapters, explicit recorded camera/object/GX/FIFO fixtures and every reached
GX/matrix/fctiwz/FIFO boundary held twice. They cover all color branches, the
frame>4 exit, frame>=4 texture collapse, live borrowed-base timing, service/input
replacement and recursive admission. Those fixtures are protocol/math evidence,
not native pixels or a normal projection claim. Separate tests verify both real
texture-state branches, captured globals under repeated holds, exact original
source metadata/immutable reads and malformed/trailing/corrupted resource
rejection. Host native load remains unavailable. Python checks exact original
bank/descriptor/vertex/source-span/product hashes, unscaled I4 pixels/sampler and
signed vertex rows. Full host/native/emulator gates remain root's responsibility.
