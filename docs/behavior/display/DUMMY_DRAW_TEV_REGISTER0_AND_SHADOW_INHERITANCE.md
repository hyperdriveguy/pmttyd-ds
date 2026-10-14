# Dummy draw TEV register0 and shadow inheritance

US G8ME01 disc0 revision0, read-only static recovery 2026-10-14.
Extends the complete [dummy request list](../camera/CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md)
and [shadow setup order](SHADOW_DISPLAY_SETUP_AND_EMPTY_RECORD_CONTINUATIONS.md).
Ignored full listing: artifacts/reference/display-class-check/sysDummyDraw.disasm.
Named GX semantics cross-checked against sysDummyDraw in the pinned original
decomp source62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/system.c;
actual integer arguments and data below are freshly verified US ISO bytes.

## The selector1 names GX_TEVREG0

At80005c00 the source loads r3=1 and calls GXSetTevColor802b8ab8 at80005c08.
This selector is **GX_TEVREG0**, whose inputs are C0/A0. It is not the named
GX_TEVREG1 (selector2) or C1/A1. The setter's copied local RGBA is word8041f440,
whose initialized bytes are00000081: RGB0 and unsigned alpha129.

| Source use | Numeric argument | Named meaning |
| --- | --- | --- |
| GXSetTevColor register | 1 | GX_TEVREG0 |
| Dummy color D input | 2 | GX_CC_C0 |
| Dummy alpha D input | 1 | GX_CA_A0 |
| Color/alpha op output | 0 | GX_TEVPREV |
| Shadow alpha B input | 1 | GX_CA_A0 |
| Shadow alpha C input | 5 | GX_CA_RASA |

Dummy configures a single ADD/zero-bias/unit-scale/clamped stage with
color inputs(ZERO,ZERO,ZERO,C0) and alpha inputs(ZERO,ZERO,ZERO,A0).
Its output writes PREV, a distinct register; reading C0/A0 and writing PREV
does not overwrite TEVREG0. After the actual SetTevColor request completes,
TEVREG0 is a known retained source value. Completing the no-write triangle
does not reset it, even if geometry is clipped or culled.

## Shadow's inherited A0 and independent material owner

Polygon and outer cylinder setup use AlphaIn(7,1,5,7), named
(ZERO,A0,RASA,ZERO), ADD/zero-bias/unit-scale/clamp/PREV. Thus they multiply
inherited A0 by raster alpha. Following the admitted dummy and only the
common/class display leaves, A0 is129: those leaves change write masks,
depth mapping/blend/depth-location/alpha-test settings, not TEV register values.
This is a valid known-state seed for that specific completed prefix.
An intervening callback's register write must update/invalidate it.
Cylinder's admitted per-record passes explicitly replace TEVREG0 alpha from
record BYTE28; it cannot be treated as permanent129.

Dummy does **not** establish retained channel control or material color. It
calls GXSetNumChans(0) but never GXSetChanCtrl, GXSetChanMatColor or
GXSetChanAmbColor. No material/global alpha can be inferred from its constant
TEV output. Shadow setup itself establishes channel4=GX_COLOR0A0 with
GXSetChanCtrl(4,0,0,0,0,2,2): lighting disabled, register ambient/material
sources, no lights, diffuseCLAMP, attenuationNONE. It then freshly reads the
retained shadow owner's word100 and writes GXSetChanMatColor(4,RGBA).
That color is an independent owner; its initialized black alpha110 is
documented by the shadow owner, not provided by the dummy.

## Complete affected-state boundary

Dummy writes position matrix0/current matrix0, channel count0, texgen count1
and texgen0 descriptor, TEV stage count1/order/stage0 color+alpha operations
and inputs, TEVREG0 RGBA, depth disabled/ALWAYS/no-depth-write, RGB+alpha
writes off, blendZERO/ONE, depth locationafter-alpha, alphaGEQUAL128 OR NEVER,
vertex descriptors and position/UV formats. It emits original three XYZ/ST
vertices and restores no GX state. It does not bind a texture, reset fog/cull,
change channel control/material/ambient color, or initialize other TEV color
registers. Native state restoration may reapply acknowledged state but must
not replay its triangle or fabricate those untouched owners.

The PICA constant used for rendering the dummy is an implementation detail;
the retained source TEVREG0 owner must survive native TexEnv reconfiguration.
Shadow raster adaptation must read that owner, rather than infer A0 from a
current PICA TexEnv constant that unrelated rendering may have replaced.
Exact GX TEV fixed-point rounding versus native multiplication remains a
separate raster-parity question; this contract supplies source operands.

## Evidence

ISO opened rb only. sysDummyDraw80005ad8/20c SHA256
6773555033b684d8a4d4cad3453d3132620af908a37921aeb0a09a2ef8f90a4d and
constant8041f440=00000081 were checked from mapped DOL spans.
Named selector/input associations match
the source decomp's GX_TEVREG0/GX_CC_C0/GX_CA_A0 calls and the US disassembly
above. No runtime, emulator or hardware result is claimed.
