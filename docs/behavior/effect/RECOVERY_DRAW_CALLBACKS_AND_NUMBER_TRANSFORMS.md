# Original recovery draw callbacks and number transforms

US G8ME01 revision0, static read-only source evidence. Whole
`effRecoveryDisp801422f4/684` and `effRecoveryDisp2 801420fc/1f8` are adapted
by `recovery_effect_draw.hpp/cpp`. Read the
[recovery lifecycle](RECOVERY_EFFECT_LIFECYCLE_AND_MAYA_OWNERSHIP.md),
[Maya evaluator and pool](MAYA_QUANTIZED_CURVES_AND_EFFECT_POOL_CONTINUATIONS.md)
and [original resource ledger](../../ASSETS.md).
`tools/verify_recovery_effect.py` pins both complete bodies and relevant embedded
spans. This is not a claim of measured timing, original raster or hardware parity.

## Camera, payload and material ownership

Disp captures E+c, requests cameraGetPtr(camera), reads signed root kind, builds
root translation, requests camera again and rotates about Y by negative camera
114 times original degree-to-radian constant. It retains this initial billboard
matrix separately. That calculation is not dead work: particles later use it.
The first camera pointer remains the root's later view owner.

The original calls two texcoord-generator setters, unlit channel/cull controls,
clears vertex descriptors and selects direct F32 XYZ/ST before texture selection.
Original table803726c0 selects base-bank18/20 for HP kinds0,2,4,7;19/21 for FP
1,3,5,8. Other kinds, including6, skip texture loading and retain existing source
bindings. Each original effGetTexObj request returns a retained opaque texture
object; GXLoadTexObj binds it to map0 then map1. No guest address is a host pointer.
Unavailable construction/binding retains the request without replaying earlier
setters or texture work.

After texture calls, E+c is read again. Fresh root translation and another fresh
camera yaw form T*Ry*S10*T(0,2,0). For kinds0,1,4,5,7,8, append the current
root38 Maya matrix at+14. Then prepend the FIRST camera's live view at+11c and
append S0.1. Submit the original GX position matrix/index0 and first TEV program:
texture0 RGB/alpha replacement. No extra blend/depth reset is invented for this
first program; state not explicitly set by the original remains its owner's duty.

The root FIFO quad is (-16,16,0), (16,16,0), (16,-16,0), (-16,-16,0), with
unit ST. Actual values are loaded from original constant addresses at emission,
not silently replaced by these explanatory numbers. Kinds2/3 return after this
quad. Other kinds set the original opaque/alpha/depth and texture1*C0 program.

## Live particles retain the initial billboard

Scan particle indices1..<LIVE signed E8 with44-hex stride. Each iteration reads
the live GP pointer then tick1c. Original source also reads its default RGBA word
at804295e8; all four bytes are subsequently overwritten. That word belongs to
the runtime source memory domain, not the embedded DOL resource pack. Read low
byte of particle word4 for alpha. Choose RGB from palette804179d8 using unsigned
wrapping (tick+index)%12. GXSetTevColor register1 receives the resulting RGBA.

Particle geometry uses its live XYZ and scale, prepended by the retained INITIAL
root billboard. It then requests another camera and prepends that camera's live
view. The particle quad is ±6, with ST width2 and height1. Do not normalize its
ST width to1 or replace the initial billboard with the refreshed root matrix.
These distinct captures are tested with changing E+c, four distinct camera
providers, a changed kind and original unsigned tick wrap.

## Number world matrix and source formatting

Disp2 captures E+c, requests camera once, reads signed amount/kind, calls original
sprintf identity with format80422df8 (%d), then strlen. Native adaptation uses an
opaque stack-string token between those calls. No rewritten story string is
created. It composes the following world matrix, in source call order:

T(root XYZ) * Ry(-camera yaw radians) * T(0,0,1) * S10 * T(0,2,0)
* Maya(root38+14) * S0.035 * T(-(length-1)*24*0.5,0,0).

Length centering preserves wrap32 negation, signed conversion through original
binary64 bias802f3b68, binary32 subtraction and two separately rounded multiplies.
Number style0 is selected only for kinds0,2,7;style1 for1,3,8. Other kinds still
perform source formatting/transforms but do not call effNumberGX. The number
request retains matrix, signed amount, alpha255 and mode1. Full effNumberGX and
its original base-bank image108 remain independently owned services.

## Retained execution and integration limits

Both APIs keep caller-owned cursors. Active cursors reject changed operation,
effect/camera, memory reader/owner or service identity. Pending SDK requests copy
their matrix operands; output written by a false callback is discarded. Pending
GX calls, texture tokens, geometry and number requests retain exact operands.
Original source setters and completed matrix operations do not repeat after a
later service holds. Direct FIFO quads are atomically acknowledged: false must
consume no partial geometry. This adapts immediate FIFO streaming to retained
native submission without silently completing an unavailable renderer.

The Maya math adapter explicitly reuses the existing rendering affine adapter,
including its finite-domain policy and host/libm rotation. Its binary32/fused
helpers preserve local operation boundaries; full Gekko FPSCR, NaN/subnormal,
SDK transcendental and native raster parity are not claimed.

These source draw callbacks select original textures and submit original GX
requests. They do not create a GPU program, admit a texture cache, play sounds,
run the source effect driver, render the linked number/terminal stardust or bind
normal gameplay recovery. Scene4 continues to belong to both top scene and lower
audience continuation through the existing view owner. Physical controls and
original menu/HUD art remain mandatory. Native recovery gifts remain held until
actual effect, resource, numeric, sound, display and lifetime owners are admitted.
