# Field animation array reads and complete-fan staging

[FieldAnimationGpu](../../source/display/field_animation_gpu.cpp) recognizes
selected original GX calls, reads indexed attributes through the actual VM and
retains one complete fan before submission. The
[native camera adapter](../../source/display/render_field_animation_gpu.cpp)
publishes field camera/cull state. Material, lighting and texture calls remain
with the explicit fallback owner; a missing callback remains a hold.

## Source identities and register state

The [header constants](../../include/display/field_animation_gpu.hpp) name source
entrypoint addresses and the `0xcc008000` FIFO identity. They are guest ABI
operands, never callable native pointers. Compact binding slots are POS/normal/
CLR0 followed by TEX0..7; source attribute 12 is deliberately absent. Formats
admit VAT 0, INDEX16, binary32 XYZ/normal/ST and RGBA8, with byte strides
12/12/8/4 respectively. Enabled UV arrays form a contiguous prefix. Clearing
descriptors preserves array and format registers.

Only matrix slot/current matrix 0 is admitted. Position upload captures the
matrix before a physical wait. A real camera projection can repair that wait;
a cull hold instead blocks camera requests. Pending setters pin request bits
and the locals object's address, so unrelated source calls cannot bypass them.
Normal registers retain exceptional floating point bits for the explicit lit
consumer. The stager does not invent an identity normal after a singular result.

`restore()` reapplies acknowledged camera/cull state, including while a fan's
final submission holds. It neither commits a pending setter nor submits a fan.
The validity predicate is sampled at work boundaries; it is not a sticky epoch
poison. Owners must maintain their synchronous lifetimes and prevent invalid
reuse rather than assuming this stager supplies that policy.

## An index may span several VM reads

Each FIFO halfword indexes one enabled attribute, rather than an entire vertex.
Its effective address uses source unsigned 32-bit arithmetic: bound guest base
plus unsigned16 index times stride. The render parent already selected arrays
and applied shape offsets. Adding a second offset or casting the base to a
native pointer changes the source operation.

RGBA reads four bytes; other components read guest-endian words interpreted
as binary32. The retained component cursor advances only after each successful
read. A sparse-memory hold therefore preserves earlier components while later
components remain live. A different halfword cannot replace the held index.
After the last attribute of the last vertex, a submit hold retains the complete
decoded buffer and matrix snapshot; backing VM changes cannot alter that retry.

## Counts, capacity and submission

Begin supports TRIANGLEFAN `0xa0`, VAT 0 and the count's low16. Zero consumes no
matrices, arrays or FIFO. Counts 1/2 consume every enabled index without emitting
triangles. Storage grows only when the current count exceeds capacity and is
reused afterward; allocation failure holds before starting the fan. The maximum
65,535 records are a source count domain, not measured original scene usage.

Submit receives a borrowed reusable buffer and per-fan matrix snapshot. It must
finish synchronous GPU submission or copy the data. False retains the same fan;
true may allow later fans to overwrite that storage. No immediate GPU Begin may
remain open across a hold. Cull ALL consumes source arrays but submits nothing;
only accepted real submissions increment the fan/vertex counters. Active fans
refuse source state/camera changes.

## Native publication details and limits

The camera packet already contains its composed transform. Four rows are copied
into `C3D_Mtx` without another tilt/view multiplication. Viewport publication
restores the acknowledged scissor because Citro3D viewport setup disables it.
Saved scissor conversion is checked before viewport mutation. Viewports take
dimensions; scissor takes endpoint coordinates.

Source FRONT/BACK maps to native BACK_CCW/FRONT_CCW. PICA has no all-face cull
token here; ALL is safe only because this stager suppresses complete fans.
Do not use this physical callback as a general cull service for other draws.
Dummy material-state primitives remain unavailable in the factory until their
actual field owner exists. This adapter does not establish lit/textured shader
coverage, normal Game scheduling, original raster parity or hardware timing.
See the [field staging contract](../behavior/display/NATIVE_FIELD_ANIMATION_GPU_STAGING.md)
for source identities, ownership and recorded validation boundaries.
