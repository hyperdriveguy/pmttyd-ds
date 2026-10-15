# Audience drawing, queued callbacks and lower-screen targeting

See [audience work](battle-audience.md) for source actors/resources,
[items/identity](battle-audience-items.md) for selected-holder admission and
[draw pipeline](draw-pipeline.md) for lower display ownership. The immediate draw,
retained callback prefix and native touch publication have different completion
contracts; none substitutes for another.

## Immediate actor draw

DispAudience captures audience owner, requires camera 4 and submits sixteen
original GX setup calls: culling, direct XYZ/ST attributes, channel/texgen and
TEV state. Numeric tuples are source SDK enums, not native GPU registers. Item/AP
auxiliary setup adds direct color and differs in its RGB tuple. Do not merge the
sequences merely because both draw audience-related work.

Each of 200 actors freshly queries map mode. Mode 2 admits alpha other than 255;
other modes admit alpha 255. Actor flags must contain both 1 and 4. Species 0..11
use the captured normal texture bank; other species match two live guest type
bytes. A texture supplies U16 dimensions; source species coefficients and actor
scale update size fields before downstream matrix/math/raster calls.

Local pivot multiplies size. Rotation reads source degree coefficients, promotes
angles to double trig, then assembles float/FMA terms. Default normal words are
written before actual vector transformation. World construction retains unit
position, offsets and species-2 hover, then concatenates the captured camera view.
The front helper separately reacquires camera 4 and tests eye-to-at dot stored
normal with two FMAs: nonnegative gives 0, otherwise 1, including unordered values.

Thirty-two copied UV floats represent two animation variants and two flips.
Front/back exceptions compare animation roots at table slots 7/8/9, not frame IDs.
Angle normalization uses source truncate/FMA. Exact negative full turns can land
on the full-turn boundary rather than zero. Loaded texture and the flip register
persist across actors within the invocation, but both start unwritten. A missing
guest type can therefore use a preceding texture; an angle that assigns neither
flip branch can retain a preceding flip. With no earlier value the draw remains
unavailable. There is no native zero/placeholder fallback.

Immediate drawing writes dimensions, normals and angles and may submit earlier
quads before returning false. It has no retained provider suffix. Run one admitted
source pass, then replay retained geometry for other views rather than advancing
those fields again.

## Retained queue callback and auxiliary draws

The actor callback retains owner, camera and sixteen GX acknowledgements. Its
actor-zero pointer independently captures current B afterward. It queries the
current display record per actor; class 2 admits nonopaque alpha, other classes
opaque alpha, then flags 1/4. The complete empty-bank domain returns success.
The first admitted actor reaches stage 5/pending `0x801a38f8`, an unimplemented
continuation. That hold is not evidence of an empty audience and cannot be
acknowledged as a successful actor draw.

Auxiliary callbacks are complete local parents: `0x801a37f0` draws 100 item
records at A+0xf25c, stride 0x48; `0x801a36c8` draws 100 AP records at A+0x10e7c,
stride 0x60. Item flags require 1/2; AP requires 1. AP captures A before an unused
camera-8 lookup; item records capture A after GX setup. Both retain GXInit's
discarded BaseGetPtr. One fresh flags load admits each record.

Item IDs index 40-byte definitions at `0x803108a8` for icon halfword +0x20. Six
matrix calls preserve lift, Y rotation, apply translation/scale and actual world
position. AP uses five calls, icon 442, flag 16, source color and a separately late
alpha byte +0x5c. Direct icon provider takes precedence over retained source-icon
fallback. Prepared requests survive holds; the next record advances only after
actual icon acknowledgement. Callback identity is checked, while fixed service
bindings and owners remain caller obligations.

The linked native math adapter rejects unsupported/nonfinite requests. ASCII
axes 120/121/122 mean x/y/z. TransApply adds translation; ScaleApply scales entire
rows including translation. Concat retains its explicit FMA order. This adapter
is not proof of original SDK/Gekko trig or matrix arithmetic parity.

## Native touch frame

Touch publication uses acknowledged world quads after local/world transformation,
without source view already applied. It applies the actual camera once and scales
source world units by 0.01. Rotated viewport bounds are 240x400; public lower-screen
hit coordinates are 320x240, with swapped/inverted clip axes and half-extents
160/120. These numeric domains must not be interchanged.

Begin rejects stale same-owner sequence numbers before reset. A new pass resets
old geometry before validating the camera. Append requires the same incomplete
building pass, nonzero lifecycle generations and valid actor/item bounds. Duplicate
actor slot or invalid target/clip poisons that pass until new begin/reset; an
unrelated-pass rejection does not. Positive clip W and finite projection are
required. This helper supplies no raster-depth/alpha occlusion policy.

Only finish with the completed pass makes it tappable. Hit requires that exact
currently displayed completed owner/sequence. Inclusive triangle edges admit
finite nondegenerate triangles. Every covering quad requires exactly one exact
live incarnation. Explicitly invisible matching identity permits tap-through;
unknown identity, a visible harmless/uninterruptible occupant or multiple eligible
overlaps selects none. No nearest-seat or guessed depth tie-break is introduced.
Physical X retains its independent original first-eligible item path.
