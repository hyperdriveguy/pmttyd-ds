# Effect pool and HitN64 ownership chain

[Effect pool](../../source/effect/effect_pool.cpp) and its
[API](../../include/effect/effect_pool.hpp) own original record acquisition,
driver traversal, initialization and context release. The HitN64 modules separate
[Entry/Main](../../source/effect/hit_n64_effect.cpp),
[source draw](../../source/effect/hit_n64_effect_draw.cpp),
[texture-object construction](../../source/effect/hit_n64_texture_object.cpp),
[immutable/native texture resources](../../source/effect/hit_n64_texture_resources.cpp)
and [GPU capture/submission](../../source/effect/hit_n64_gpu.cpp).
Local completion does not activate normal effect scheduling or establish original
runtime, numeric, framebuffer or hardware-performance parity.

## Pool records and source predicates

Records stride40 decimal. Manager count/array are at0/4; cache handle/selector at
0x10/0x14. Record flags/context/payload/update callback are at0/4/0xc/0x10.
Entry searches exact flags0, while Main/context release require active mask1.
These predicates differ from nonzero occupied/name-visible flags.

Entry preserves the source unchecked domain: nonpositive signed count writes the
base, exhaustion writes one-past, and initialization clears only selected words
and one byte. Providers admit actual mapped capacity; no successful null/full
repair is inferred. Raw memory callbacks are synchronous. Retained external
services preserve acknowledged effects; cursor completion resets for another
source invocation. Pool identity checks cover used memory callbacks and service
user; keeping other service functions stable remains a caller obligation.

Main captures count/array once and freshly reads GP context per record. Later-slot
allocation can run in the same ascending scan, while earlier-slot allocation waits.
The texture tail runs afterward using fresh manager/GP. Free, asynchronous request,
synchronous allocation and selector publication have distinct capture/reload points.
Async nullopt is unavailable, executed0 finishes without a tag, and any nonzero,
including-1, proceeds. A synchronous handle0 is an executed result.

Init publishes512, allocates heap0, reloads manager/count/base for clear, resets
cache fields, then clears the independently selected N64 state. Its final state+4
store reloads that pointer. Context release compares the full supplied context,
frees heap3 payload and clears only record flags. It does not invoke effect-specific
cleanup, cancel queues or release pose/map/audio/file-cache owners.
See [pool lifecycle](../behavior/effect/EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md)
and [driver/cache ordering](../behavior/effect/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).

## HitN64 simulation and draw requests

[Entry/Main API](../../include/effect/hit_n64_effect.hpp) binds exact input bits and
providers, rejects recursive admission, and retains each held child. Reset drops a
continuation, not the allocated record/payload. Entry reserves a real pool record
before heap3 allocation. Wrapped signed arithmetic derives count, and each0x44-byte
row includes the root or a particle. Kind0 uses white colors,1 yellow/red,2 yellow/
green, and default cyan/blue. Original RNG runs once per particle1..count-1, with
signed remainder360 and source direction division. Root alpha0x2c remains
uninitialized; the later draw must not substitute255.

One Main invocation copies root XYZ, decrements life and increments age before
expiry. A held delete cannot repeat those writes. Otherwise original byte tables
update progress/radius before depth and camera4/class2 registration. Rendering
must not advance Main again. Registration requires actual queued callback ownership.

[Draw API](../../include/effect/hit_n64_effect_draw.hpp) keeps the unused initial
camera query, signed frame conversion, billboard transforms, color branches and
original TEV requests. Three0/120/240-degree passes each invoke the full particle
child. The borrowed base address stays fixed and is read only at the reached concat.
Frame>4 exits; negative frames retain source address arithmetic, without clamping.

Each particle uses fresh count and camera4 view. Original vertex blocks have four
stride14 rows: signed16 XYZ, signed16 UV with five fractional bits, RGBA8. GXBegin
triangles/count6 is followed by one atomic eighteen-halfword index stream. A false
consumer consumes no partial triangle. Matrices and GX requests retain their full
payload through holds, without replaying successful stages.

[Texture-object API](../../include/effect/hit_n64_texture_object.hpp) captures actual
N64 state. Loaded state uses its real palette; unloaded state uses the original1x1
I4 fallback pixels. Unavailable assets do not authorize a fabricated unloaded state.
The result denotes retained native output-object ownership, not an incidental
source return register or invented guest stack pointer.

## Immutable resources, capture and GPU submission

[Resource API](../../include/effect/hit_n64_texture_resources.hpp) admits exact
PMHITX01 metadata, original descriptor/vertex/table/constants and EOF. FNV guards
trusted-pack integrity; converter manifests separately pin original SHA256 hashes.
Only immutable spans are exposed. Effect allocations, cameras, BSS, palette and
fallback state require actual mutable owners.

Native import requires unpadded32x128 RGBA8 from original I4, LINEAR filtering and
REPEAT on both axes. It validates a new image before retiring the old one. Resource
identity plus nonwrapping generation invalidates old captures on reload/release.
Import/retirement occur outside FrameBegin with C3D alive; host GPU ownership is
unavailable.

[GPU API](../../include/effect/hit_n64_gpu.hpp) captures exact source setter order,
independent UV matrices and source vertex aliases at FIFO reads. Unknown requests,
unsupported third-coordinate coupling or changed aliases hold. Tex3DS T flips once;
UVs are not clamped. Packets preserve all particles of pass0, then1, then2.
Publication becomes available only after complete source preparation while actual
allocation/camera/frame/inherited-state/texture epochs remain valid. First lifetime
loss permanently retires the batch; a restored flag cannot revive it.

Preparation is not display completion. The outer display owner must retain its
callback/context, submit the whole batch, and only then acknowledge it. Native GPU
preflight checks capacity, finite data, texture generation and inherited raster
ownership before CPU conversion and GPU effects. Same-frame draws consume separate
arena ranges; synchronized beginFrame permits reuse. Shader inputs and TEV equations
are explicit. Original indexed RGBA is retained, but NumChans0 makes register0 the
native primary color; register1 supplies the second-stage constant. This maps the
source equations without claiming identical fixed-point rounding or pixels.

See [Entry/Main evidence](../behavior/effect/HIT_N64_ENTRY_MAIN_AND_DISPLAY_FRONTIER.md),
[source draw/texture evidence](../behavior/effect/HIT_N64_SOURCE_DRAW_AND_NATIVE_TEXTURE_OWNER.md)
and [ordered GPU evidence](../behavior/effect/HIT_N64_ORDERED_NATIVE_GPU.md).
