# Retained display state and icon continuations

This guide covers the portable helpers behind the [render adapters](render-adapters.md):
`native_*`, `source_icon_*`, `retained_icon_frame`, `camera_letterbox_draw`,
`field_camera_publication` and `battle_scene_views` in `source/display/` and
`include/display/`. These helpers retain source state without accessing 3DS
services. Recovered behavior/evidence remains in [display contracts](../behavior/README.md).

## Logical state and physical publication

`NativeCameraDraw` and `NativeDisplayState` validate a candidate, call a backend,
then publish their logical state only after acknowledgement. Failed publication
leaves the prior logical values intact; it cannot promise rollback of physical
GPU commands already queued by a backend. Restore replays acknowledged state.
The camera restore order is viewport, scissor, projection, dummy state; it never
replays the source dummy triangle. `afterDummy` is a source side effect, not an
alternative restore operation.

`NativeShadowProgram::request` returns a validated copy for the adapter to
publish. Its independent color/alpha channel controls and material sources must
not be collapsed. Eight second-stage setters contribute bits 1..128 to mask 255;
enabling the second stage requires all eight plus K0. Direct/indexed declaration
slots are position, color 0, texcoord 0. Guest array addresses remain resource
identities; these classes never cast them to host pointers.

`NativeIconProgram` accepts individual setters before recognizing a whole plan.
A successful setter does not prove the current combination drawable. `plan`
requires the exact normal, grayscale or blended program, initialized material,
channels, fog, cull, texgen and vertex declarations. An engaged optional containing
zero is an actual write; an absent optional means unknown state. Grayscale swap
tables replicate texture R/G/B into RGB while preserving component 3 alpha.
Ordinary programs require identity swap table 0, so stale grayscale swap history
cannot silently become an ordinary icon.

## Numeric domains

| Domain | Meaning |
| --- | --- |
| Source raster 608x480 | Original UI rectangle domain; center 304, 240 |
| Physical 240x400 | Rotated native top target; X corresponds to display vertical |
| Projection vector 7 | type, xx, x offset, yy, y offset, zz, zw; type 0 perspective/type 1 orthographic |
| View 3x4 | Affine matrix; implicit final row 0, 0, 0, 1 |
| Near/far 0..1 | Viewport depth range; native scale=near-far, offset=1-far |
| Exact endpoint epsilon 1e-7 | Native integer-edge admission policy; rounded helper is a separate domain |
| Channel 0/2/4 | Color/alpha/combined source channel |
| Attribute 9/11/13 | Source position/color 0/texcoord 0 |
| Descriptor 0/1/2 | NONE/DIRECT/INDEX8 in the admitted declarations |
| Format 3/4/5 | S16/F32/RGBA8 for the relevant declared attribute |
| Color inputs 0/2/4/8/10/12/14/15 | PREV/C0/C1/TEXC/RASC/ONE/KONST/ZERO |
| Alpha inputs 0/1/2/4/5/6/7 | APREV/A0/A1/TEXA/RASA/KONST/ZERO |
| Swap component0/1/2/3 | R/G/B/A |
| Blend mode 0/1/2 | Copy/blend/logic; admitted logic operation7 is OR |
| Blend factors0/1/2/4/5/6/7 | ZERO/ONE/SRCCLR/SRCALPHA/INVSRCALPHA/DSTALPHA/INVDSTALPHA |
| Compare0/1/3/6/7 | NEVER/LESS/LEQUAL/GEQUAL/ALWAYS; native depth reversal is a backend concern |
| Texture-coordinate tuple0, 1, 4, 60, 0, 125 | One 2x4 generator from TEX0, identity matrix, no normalization, original postmatrix selection |

These numbers belong to different APIs. For example, GX cull2 means front-face
culling; actor surface cull2 elsewhere suppresses that surface. Numeric phase160
in iconGX is a native continuation label inserted for overlay setters, not an
original opcode, function address or delay.

## Icon resource addressing

`SourceIconResources` reads original BE directory/TPL data into immutable owners.
`icon.bin` has a U16 descriptor count followed by U16 offsets. A descriptor's
width/height/frame count are at+2/+4/+6, then four-byte frame rows contain U16
texture index/delay at+8/+10. Each row must reference an actual TPL image.
Multiple frames require a positive total duration; one static frame may have zero
delay. Public `count()` is the number of TPL images, not icon descriptors.

The 188-byte constants record concatenates seven source DOL spans. The inline
span table stores original address, byte length and packed offset. `half`/`word`
provide BE reads without importing mutable guest RAM. Directory 0x83000000 and
Palette 0x83100000 are synthetic address identities. Original globals0x8041e630/
634/638/63c map the palette/directory/bin/TPL roots to those domains.

Texture object tokens are texture-index+1, leaving zero unavailable. State bytes
0/1/2 mean absent/constructed/LOD configured. `tplGetGXTexObj`, zero-LOD linear
configuration and `GXLoadTexObj` remain separate calls; a held actual bind retains
state 2 instead of reconstructing the object. Original dimensions come from TPL
queries0x802b76c4/76d4. Resource load publishes nothing until all validation passes.

Synthetic camera identities start 0x84000000 with 0x400 stride across 13 source
slots. GP reads have first refusal. Camera+0x114 yaw does not require a published
view; the twelve binary32 view words+0x11c..0x14b do. Bit-preserving memcpy is
intentional. Original function tokens in these helpers can be read alongside the
[adapter callback table](render-adapters.md#original-callback-and-sdk-identities).

## Resume behavior that must survive cleanup

`SourceIconGXCursor` latches complete call/math payloads while a provider holds.
Table steps increment only after acknowledgement. Optional0 is successful void
return; nullopt is a wait. A quad hold follows acknowledged GXBegin and must not
repeat Begin, texture, matrix or material calls. The geometry provider acknowledges
all four vertices atomically. After the quad, grayscale reset setters still have
their own retained acknowledgement boundaries.

Source flags 4/8 mirror X/Y. Flag 16 selects unlit full-size UI and depth-off, with
another Y flip outside camera 8. Flag 32 selects grayscale; flags 16|64|128 skip the
conditional map blend. Flags0x400/0x800 select front/back cull. Record alpha below
255 selects blending/no depth write; opaque alpha selects cutoff 128/depth write.
Default world geometry scales .6, with extra .8/.9 for icons 403/405; UI flag 16
bypasses those factors. Only width==0 AND height==0 asks for texture dimensions.
Actual unloaded bin/TPL pointers choose the source fallback path; an unavailable
native fallback provider must still hold.

The matrix wrappers narrow flags/icon to16 bits and alpha to8. Icons 73/103 use
GP+0x16c language tables; the matrix wrapper's local variable named `story` does
not refer to plot progress. Animation selects texture with signed GP+0x1c clock
remainder. Negative remainder behavior is preserved; do not normalize it into a
positive modulo. Zero/signed-overflow total duration domains are unadmitted.
The position wrapper composes translation/negative-yaw rotation/scale then view.
It samples live position after initial camera/buffer reads. Its late unload path
really reaches a descriptor read at address 2; it is not a second early empty-icon
return. Read order2, 4, 8, 10, 6 is retained across holds.

`RetainedIconFrames` has building and published histories. Owner identity changes
reset both; a newer pass clears only building state, preserving the last completed
image while the new pass holds. Whole source return publishes, including an empty
completed pass that retires old icons. Both histories pin texture residency.
Camera 4 packets require a retained scene camera, unlike camera 8 UI packets.
Viewport embedding uses homogeneous row 3; native-world replay multiplies matrix
spatial columns by 100 to undo the independently prepared vertices' source*.01.

Letterbox saves/restores viewport, compact projection and scissor. GP flag 0x8000
or zero camera 8 alpha+0x1e0 takes the original early return. Top/bottom extents
+0x1e2/+0x1e4 are reread by each vertex that uses them. Eight source S16 vertices
use X+/-304, Y+/-240; narrowing preserves low 16 bits without saturation. CPU staging
accepts sequential vertices 0..7; if whole submission holds at 7, only the identical
final vertex may retry it. This avoids leaving an immediate GPU primitive open.

Field camera publication validates a complete candidate before replacing camera 4;
battle mode 2 is excluded. World-to-engine conversion multiplies position/distances,
view translation and projection Z translation by 100; orientation/lens coefficients
stay unchanged. Lower battle view rewrites clip X+=2W, Y*=400/320 and retains Z/W.
These framing/raster choices are native adaptations, not claims of original
hardware, pixel, camera or floating-point parity.
