# Finite native ordinary font GX state

`BattleFontGxState` translates actual linked GX children from
[whole draw-start](RETAINED_JUT_FONT_DRAW_START.md) and
[shared font state](RETAINED_SHARED_FONT_STATE.md) into the state used by the
real `BattleFontGlyphSink`. It records no request queue and acknowledges no
source parent or outer display batch. The original retained controllers still
own camera, SDK math, guest globals, private-color captures and every hold.

## Genuine bindings and actual effects

Bind one stable sink, its same actual resident `BattleFontLease`, strong source
resource context and separate source/publication current predicates. The source
predicate proves the held actual callback/camera/pass; the publication predicate
proves allocation/resource history. Both are pure and non-reentrant. The owner
pins these bindings; a failed current predicate or dead lease permanently
invalidates this instance. The resource context must retain the sink and all
callback storage. No raw temporary context is valid.

`staticTextureCurrent(8041e97c, lease)` must prove the real source Init/Setup and
GXInitTexObj/LOD object, its exact source font allocation, and the same actual
resident atlas generation. The original literal8041e97c is not dereferenced as
a pointer cell or replaced by an arbitrary atlas ID. Missing setup can hold
TexLoad and later become available without replaying prior source effects. Once
TexLoad was admitted, loss of its object proof invalidates the instance; a
later address reuse cannot resurrect its old profile.

Position-matrix slot0 and TEV color register1 requests invoke the real sink's
`loadMatrix` and `tevColor` before completing. The sink subsequently copies these
actual values into each accepted glyph packet and runs the real source glyph
consumer/RNG. The remaining supported setters update finite semantic GX state
that directly gates the sink's ordinary profile. This is actual backend state,
not success based on receiving or appending a request.

## Exact supported source state

All following states must have actually been supplied before the ordinary
profile is ready. Requests with extra or different operands do not complete.
Original call-site identities remain metadata; the whole source controllers
pin and execute their real call sites.

| State | Exact original operands |
| --- | --- |
| Indirect mask / cull / Z comparison location | `0` / `0` / `1` |
| AlphaCompare | `7,0,0,7,0` |
| BlendMode | `1,4,5,7` |
| ZMode | `1,7,0` |
| ClearVtxDesc | no operands |
| VtxDesc | attributes9,11,13,14 each direct1 |
| VtxAttrFmt | `0,9,1,3,0`; `0,11,1,5,0`; `0,13,1,2,0`; `0,14,1,0,0` |
| ChanCtrl / NumChans | `4,0,1,1,0,0,2` / `1` |
| NumTexGens / NumTevStages | `1` / `1` |
| TevOrder | `0,0,0,255` |
| TevColorOp / TevAlphaOp | each `0,0,0,0,1,0` |
| TevColorIn | `0,15,2,12,15` |
| TevAlphaIn | `0,7,1,4,7` |
| LoadTexObj | literal static8041e97c, slot0, genuine resource proof |
| LoadTexMtx | index30,type1; actual diagonal1/512,1/256,1 |
| TexCoordGen | `0,1,4,30,0,125` |
| LoadPosMtx / CurrentMtx | finite actual matrix,slot0 / `0` |
| TevColor | register1,actual effective RGBA word |
| Fog | actual type0,fourzero scalars; retain actual source color |

Thus the native shader must use constant C0 RGB and A0 multiplied by texture
alpha, with source-alpha/inverse-source-alpha blending. AlphaCompare is always
true: **there is no GT8 or other alpha cutoff**. Depth comparison is enabled
with ALWAYS and depth writes disabled; culling and fog are disabled. Source
viewport/scissor/depth still come from actual CameraDraw admission. The current
source position matrix already includes any source view multiplication; native
projection consumes it without a second camera view or projection tilt. Unused
HSV/noise coordinate lanes do not contribute in this admitted ordinary TEV
profile; they are not claimed to have arbitrary profile equivalence.

Original constants prove GP16c zero selects texture Y scale1/1024, while every
nonzero value selects1/256; X is1/512 and Z is1. This adapter accepts the latter
actual source matrix for the imported US512x256 font atlas. It does not rewrite
GP, rescale a zero-language matrix or replace a captured held matrix. Unsupported
language/resource profiles must remain unavailable.

## Unknown profiles and publication

Unknown/noise/rainbow tuples hold and mark the ordinary profile unavailable.
A fresh actual first DrawStart indirect-mask setter starts rebuilding complete
coverage; it does not modify guest state or force reentry. Ordinary noise-off /
rainbow-off setters with their original single-stage tuples can update already
supported state, but do not erase an unsupported profile's admission marker.
Only complete genuine ordinary setup can admit subsequent glyphs. This is an
explicit finite native policy, not a replacement implementation of unsupported
GX profiles. Nonfinite matrices, unsupported slots and malformed payloads hold.

Ordinary profile inspection never emits or consumes RNG. Long published glyph
packets depend on their copied matrix/color/raster and real font/allocation
leases, independently of this short live GX source admission. The external
mixed ordered batch and complete native submission remain the only whole queue
acknowledgment. This module supplies neither scheduling nor normal activation.

## Validation

Host tests compose original SourceFontState Start and ColorIDX through retained
DrawStart into this state and the real glyph sink, using the original event
constants, read-only US BFN and converted actual font metadata. The BFN fixture
emulates the original width0 setup writes7 then9 in its private copy; it does
not claim that untouched raw width was9. Real glyph packets retain distinct
matrix/colors and consume exactly two RNG64 draws per admitted glyph. Tests
hold every setup child twice, reject every malformed tuple, missing/retired
texture and allocation proofs, actual zero-language captured matrix, noise and
rainbow, recursion and retirement. Camera/static-object/RNG ownership callbacks
are declared host fixtures, not native producers. Source verification pins the
whole15 source bodies and exact texture/fog constants; ARM syntax and sanitizers
provide focused additional checks. No GPU pixel or executed-PPC parity is
claimed.
