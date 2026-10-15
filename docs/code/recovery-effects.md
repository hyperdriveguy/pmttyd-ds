# Recovery effect lifecycle, drawing and resources

[Lifecycle code](../../source/effect/recovery_effect.cpp) and
[API](../../include/effect/recovery_effect.hpp) retain Entry/Main for kinds0..5,7,8.
Kind6 requires distinct Main2 and is refused before allocation. Original-data
loading and local callback completion do not provide gameplay recovery, a complete
effect driver, terminal stardust, sound or GPU ownership.

Entry obtains a real effect before heap3 payload/Maya allocation and Play.
Kinds2/3 normalize amount0;4/5 normalize20. Count and allocation use full wrapped
words, while root kind/amount truncate to bytes. Original requests remain unchecked
for null allocation in this local source domain; native providers admit real storage.
Initialization writes selected fields only. Immediate SFX uses copied stored XYZ
and freshly read signed kind/amount bytes, with original tiered sound identities.
Particles begin after root at stride0x44; kind4/5's upper group has a different Y.

Cursors retain original request arguments and completed operations. Raw memory
callbacks are synchronous and require mapped domains. External children can hold;
used memory/user/byte-reader identities are checked, while other lifecycle callbacks
remain stable by caller obligation. A held allocation, sound or delete cannot replay
earlier source stores. Successful calls reset for a new invocation.

## Main's two clocks and particle states

Maya Calc runs once before source GP1c catch-up. Elapsed is unsigned now-previous
only when now>previous, otherwise1. Each elapsed iteration reloads payload, signed
amount and XYZ, then advances particle state once. Launch latch and completed
count initialize once per invocation, not each iteration. Repeated visits to
state6 contribute repeatedly to completed; it is not a unique-particle recount.
Rendering must not tick Main again.

| State | Reached action |
| --- | --- |
| 0 | After root counter>10, initialize circular outward velocity |
| 1 | Damp then integrate while counter<10; then move to2 |
| 2 | Count down to0, then3 |
| 3 | Mode11 orbit over40; then consume signed random remainder20 and enter4 |
| 4 | Count down to0, then5 |
| 5 | Converge X/Y/scale over16, Z over20; then6 |
| 6 | Increment invocation-local completed |

State transitions do not fall through in one visit. Counters retain signed-half
wrapping and fresh source reads. Explicit numeric providers preserve SP operations
and source trigonometric/interpolation boundaries. Unsupported exceptional domains
do not gain invented finite values or guessed movement.

Completion checks apply only to0/1,4/5,7/8;4/5 use logical amount10 despite their
larger allocation. Kinds7/8 skip terminal stardust and SFX. Other completing kinds
request original stardust at rootY-20, then source shine sound. Every completing
branch deletes Maya before the effect. Kinds2/3 do not acquire a fabricated terminal
branch merely because their normalized amount is0.

After catch-up, Main writes a fresh tick, recopies XYZ, calculates depth and registers
camera4/class1 Disp. Kinds0/1/7/8 independently recopy/calculate/register class2
Disp2;4/5 instead register Disp3. Each child has its own completion boundary.
Registering a callback is separate from rendering it.

## Original draw callbacks

[Draw code](../../source/effect/recovery_effect_draw.cpp) and
[API](../../include/effect/recovery_effect_draw.hpp) implement Disp and number
Disp2. Their cursors pin the full used draw service table, camera and effect.
SDK/GX/texture/number payloads survive holds; false output is ignored. Actual
queued callback, texture, raster and borrowed-resource lifetimes remain external.

Disp retains the first camera pointer for the root's later view, and the initial
T*Ry billboard for particles. Texture work then precedes a refreshed payload/root/
camera transform. These are different source captures. HP kinds0/2/4/7 select
textures18/20, FP1/3/5/8 select19/21. Other kinds retain inherited bindings.
No fake texture object or camera is supplied for unavailable ownership.

Root model is T*Ry*S10*Ty2, optionally Maya, then first camera's live view and S0.1.
Its quad uses original constant reads for ±16 and unit ST. Kinds2/3 finish there.
Other particles use live XYZ/scale beneath the INITIAL billboard, then a fresh
camera/view. Their quad is ±6, ST width2 and height1. Per-particle palette color
uses unsigned wrapping tick+index modulo12 and low-byte alpha. The default BSS color
read remains required even though its lanes are overwritten.

Every quad captures twenty constant loads before atomic FIFO submission. Particle
index/row advances before a possible FIFO hold, so the retained geometry must be
retried without advancing again or rereading its constants. No false consumer may
accept half a quad. Original blend/depth state not set by a callback remains its
raster owner's duty.

Number Disp2 uses original sprintf(%d)/strlen identities and a retained opaque
stack-string owner. It composes T*Ry*Tz1*S10*Ty2*Maya*S0.035*T(centerX).
Centering retains wrap32 negation, signed-bias conversion and two SP multiplies.
HP style0 applies to0/2/7, FP style1 to1/3/8, with alpha255/mode1. Other kinds still
format/transform but skip NumberGX. The number renderer remains a real provider.

## Immutable resource view

[Resource reader](../../source/effect/recovery_effect_resources.cpp) and
[API](../../include/effect/recovery_effect_resources.hpp) admit PMREC001 with native
LE directory words and original BE payload. Limits64 spans/65536 bytes are native
pack bounds. Ordered nonempty, nonoverlapping spans must fit the32-bit address
space. Complete payload/EOF validates before swapping; failed read preserves the
old bank. A half/word must fit one span even when adjacent spans exist. No mutable
GP/effect/BSS/texture state is manufactured by this immutable reader.

See [lifecycle/Maya evidence](../behavior/effect/RECOVERY_EFFECT_LIFECYCLE_AND_MAYA_OWNERSHIP.md)
and [draw/number evidence](../behavior/effect/RECOVERY_DRAW_CALLBACKS_AND_NUMBER_TRANSFORMS.md).
Local source ordering remains separate from production wiring and original
numeric, pixel, audio or hardware-performance parity.
