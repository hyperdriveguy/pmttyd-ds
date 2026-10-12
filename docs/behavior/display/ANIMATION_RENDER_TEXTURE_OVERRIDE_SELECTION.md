# Render/material texture override scans and coordinate requests

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete renderProc/
materialProc local bodies. Entry notation from
[entry/defaults](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md). No execution.
Both bodies implement these two scans with the same request structure.

## Capture and per-slot texture-data index

After default block,read CURRENT pose+4 as an ENTIRE word. Zero skips both
primary override scans;nonzero admits them. This is not a test of one named bit.
Capture resource D,current pose P,slot count N=word[B+0],base texture pointer
A=word[D+194],texture state pointer T=word[P+6c],texture count C=word[D+134],
texture data F=word[D+198]. Also capture primary override fields CURRENT W+30,
W+24,W+cc (kind1) or W+34,W+28,W+d0 (kind2).

For ascending coordinate slot j while signed j<N,read material slot ID
S=word[B+10+4*(N−j−1)] (REVERSED draw slot order). Compute32-bit
I=word[A+8*S]+UNSIGNED byte[T+24*S]. If signed I<0 OR I>=captured signed C,
replace I0 and request printf at8026a2a8 with original format802c1078.
No explicit frame argument is prepared locally. It continues using descriptor0,
without proving C>0;S/T/A dereferences happened BEFORE this bounds fallback.
There is no slot count/index/pointer validation. Texture kind=word[F+64*I+8].

## Kind1 priority and matching-with-null behavior

First scan looks for kind EXACTLY1. First match ALWAYS marks kind1Found=1 and
ends the scan,even if captured W+30 override pointer is null. Null skips its
texture/coordinate requests;it does not make the scan continue or try kind2.
For nonnull override:

- smartTexObj(capturedOverride,captured W+24)0x8002f33c;pass RETURNED pointer
  to GXLoadTexObj(returned,j).
- Captured W+cc nonnull:GXLoadTexMtxImm(matrix,30+3*j,1),then
  GXSetTexCoordGen2(j,1,j+4,30+3*j,0,125).
- Matrix null:GXSetTexCoordGen2(j,1,j+4,60,0,125).

No failure guard for returned texture object. The selected coordinate slot j
is before the original code replaces its loop variable with boolean1;retain that
slot for the requests. Subsequent state uses the BOOLEAN found result.

Only if kind1Found==0,perform a FRESH capture/scan for kind EXACTLY2 using
CURRENT W+34/28/d0 and CURRENT resource/pose inputs. Same reversed slot order,
index fallback,first-match/null semantics and request forms. Set kind2Found1 on
match. If kind1 found,no kind2 scan occurs at all. Helpers may change globals;
do not share stale captures across both scans as a substitute.

## Rendering-only alpha-input adjustment

renderProc alone additionally requests GXSetTevAlphaIn(0,7,7,7,5)0x802b89a4 iff:

- a kind was found;AND
- kind1Found AND CURRENT W+d8==0,OR kind2Found AND CURRENT W+dc==0;AND
- word[B+8]==0.

Flags are reread at that point. Matching a kind with NULL override can still
admit this adjustment. materialProc does NOT contain this GX request. Do not
factor the common scans into a helper that erases the difference.
Then perform [last-stage/rebind](ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md).

## Scope and source example

Original a/a_mario has150 texture-data descriptors,ALL kind0. Therefore its
serialized descriptor set does not exercise the kind1/2 overrides;these branch
contracts come from original code,not a fabricated Mario image observation.
Other original resource/pose/map/effect sources require binding before claiming
full US reachability or visuals. `verify_animation_render_material.py` checks
both full bodies and this original kind histogram. No original textures or
pixel data are exported. Texture object/coordinate/GX/helper/lifetime domains
remain open beyond these local request operands/order.
