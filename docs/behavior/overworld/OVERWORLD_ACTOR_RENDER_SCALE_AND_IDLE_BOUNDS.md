# Ordinary Mario/Goombella render scale and idle-pose framing

US G8ME01 revision0, read-only source owners plus explicitly converter-derived
frame0 geometry. This supplies a bounded art-scale/pose-transform contract for
original camera framing. Model vertices, physical collision dimensions and
transparent atlas quad dimensions are separate quantities. No Dolphin frame,
original GPU output, complete animation runtime or numeric parity is claimed.

## Original draw wrappers

2026-10-12 converter baseline correction: recomputed both model metadata rows
with the current host geometry/character-node arithmetic adapter after the
stepped visibility fix (original modulo256 byte addition). Goombella PKR_Z_1
metadata remained exactly equal; Mario's derived bounds/root values changed
by sub-micro-unit rounding amounts (maximum Z now1.000000954529515 rather
than1.0000004221423517). Updated only converter-derived manifest values;
all source-body/constants/model fingerprints remain unchanged. Rounded tables
below retain their displayed precision. This is host-adapter regression
maintenance, not a newly measured original-game bound or numeric-parity claim.

Goombella's ordinary stay resource is now separately recovered as selector1
PKR_S_1 in the [ordinary pose binding](../partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md).
The PKR_Z_1 frame0 envelope below continues to describe only that resource.

marioDisp80056be0 reads Mario+c8/+cc/+d0 scaleXYZ and multiplies by an inlined
marioGetScale at80056bf4–c28. Ordinary main flag0x02000000 clear gives scalar2
(word40000000 at80420004); set gives1.2 (3f99999a at80420000).
marioGetScale80058908–924 has the same branch. MarioInit initializes scaleXYZ
from literal(1,1,1) at802c1958 via8005b73c–750. MarioReInit restores its unit
scale vector via8005b474–488 from802c1970. These are ordinary initialization
values, not a claim no animation/event ever changes scale.

partyDisp800b7690 likewise multiplies partner+70/+74/+78 by scalar2 ordinarily,
or1.2 when main flag0x00800000 is set (800b772c–760; constants804211f8/fc).
partyEntry800bea68 copies literal(1,1,1) from802cb178 at800beab4–acc. This
render-scale initialization is separate from partner movement ownership.

Both wrappers pass their composed engine-space matrix to animPoseDrawMtx
8003b4d8 with an additional scale argument1: Mario80057088–a0 and partner
800b7a34–60. The paper draw leaf's ordinary scale1/rotY0 path does not add an
extra inferred factor; its optional extra-scale handling is8003b58c–5bc.
Normal/special pose flags select other draw paths and remain separate services.

Ordinary render model coordinates therefore receive **2 engine units per model
unit**, before the actual retained scaleXYZ and pose/rotation transforms.
With the port's engine*.01 coordinate adaptation, unit-scale model coordinates
receive*.02. The authored map root×10 conversion does not apply to actor models:
using map_convert's*.1 on actors would conflate two different source owners.

## Model matrix order and root anchor

For column-vector notation, ordinary Mario composition follows:

    M = T(drawRoot) * T(pivot) * Ry(yaw) * Rx(rotX) * Rz(rotZ)
        * T(-pivot) * FacingReflection * S(scaleXYZ * scalar)

Source pivot is Mario+b0/b4/b8. Negative-pivot concat is80056da0–dcc;
ordinary Z then X rotations are80056e30–e7c; Y uses
revise360(Mario+1ac-Mario+19c) at80056e80–eb0. Positive pivot is80056f64–f84.
The special motion24 changes X/Z order and is excluded here.

FacingReflection is the ordinary Z reflection selected from revised Mario+1ac
at80056cf0–d58: angles in inclusive[90,270] use scaleZ-1, otherwise+1.
Display flags can force/skip reflection or insert quarter-turn variants; this
contract is ordinary flags clear. It is not a source unconditional X mirroring
or camera-facing billboard rule. Rotation angle multiplication uses source
radians-per-degree word3c8efa35, and SDK matrices are numeric dependencies.

Draw-root translation at80056fb8–57008 is:

    X = root8c + offset98 + offseta4
    Y = root90 + offset9c + offseta8
    Z = root94 + offseta0 + offsetac

There is no source instruction that places a cropped image's bottom exactly on
rootY. Source model coordinates/root/pivot determine that position. Extra main
flag0x01000000 can insert Y scale+130; special motions/display flags and event
field writers are separate domains.

Partner composition is analogous: negative pivot+88/+8c/+90 at800b77d8–804,
Rz(+6c) then Rx(+64), Ry(revise360(+10c-+f8)), positive pivot at800b793c–95c,
then draw-root translation800b79f4–7a30. Root is+58/+5c/+60, offsets+7c/+80/+84.
The source can additionally add a member-dependent Y term when partner word+1c
is nonzero; Goombella memberID1 gets2, whereas+1c0 gets0. partyEntryMain clears
+1c at800be48c. Do not infer its later state solely from which member is active.
Rendered height is unchanged by a pure extra translation, but root alignment
is changed. Ordinary fields/flags are explicit adapter inputs.

For idle, unit scale and zero X/Z rotation, the wrapper's factor2 scales
local-pose height; yaw/reflection can change projected width and layer order.
Actual display is original pose geometry, not a universal square of one height.

## Source-derived frame0 geometry metadata

Existing tools/character_bake.py evaluates exported node hierarchy/visibility,
frame0 deltas and source indexed shape vertices. The numbers below are outputs
of that **current converter**, using NumPy host arithmetic. They are not measured
original animated actor dimensions. Source asset hashes and animation IDs are
pinned by the verifier. Bounds include visible draw geometry vertices, including
texture-transparent corners; opaque silhouette bounds can differ.

| Asset / animation | Local model X bounds | Local model Y bounds | Local height |
| --- | --- | --- | --- |
| a/a_mario / M_Z_1 frame0 | -10.860232..7.286044 | -1.658197..21.801798 | 23.459995 |
| a/c_pkr / PKR_Z_1 frame0 | -5.329821..8.856460 | -.135232..14.526864 | 14.662096 |

Under ordinary wrapper scalar2 and unit scale, engine*.01 adaptation gives:

| Actor | Local native X bounds before yaw | Local native Y bounds | Derived mesh height |
| --- | --- | --- | --- |
| Mario | -.217205...145721 | -.033164...436036 | .469200 |
| Goombella | -.106596...177129 | -.002705...290537 | .293242 |

The asymmetric X bounds and nonzero lower Y are meaningful. Neither actor's
geometric bbox is centered at rootX with lower corner rootY. Mario's physical
body height28 engine units is not a replacement for this pose vertex envelope.
The converter's animation interpolation/whole-pose runtime, source f32 matrix
order parity, shape transparency and dynamic frame bounds remain separate work.

## How to carry these units through the current bake

The current bake makes a128px square with a5px geometry margin. For raw local
bounds lo/hi, maxExtent=max(width,height), pixelsPerModelUnit=118/maxExtent,
center=(loXY+hiXY)/2. It maps model root(0,0) to pixel:

    rootPixelX = 64 - centerX * pixelsPerModelUnit
    rootPixelY = 64 + centerY * pixelsPerModelUnit

The full quad native side is128/pixelsPerModelUnit*.02, not simply mesh height.
For this bounded frame0 bake:

| Actor | Root pixel XY | Full square quad side in native units |
| --- | --- | --- |
| Mario (if baked through geometry) | 72.988796,114.659537 | .508963 |
| Goombella | 49.808870,121.911657 | .318093 |

Full quad corners relative to source root are:

    left = -rootPixelX / pixelsPerModelUnit * .02
    right = (128-rootPixelX) / pixelsPerModelUnit * .02
    bottom = (rootPixelY-128) / pixelsPerModelUnit * .02
    top = rootPixelY / pixelsPerModelUnit * .02

This preserves source units/root anchoring despite transparent padding. It is
a billboard approximation to the original geometry, with depth/layers/yaw still
adapted. Metadata should be per pose/frame or use a documented stable envelope;
independently cropping every frame without preserving root causes actor drift.

Current tools/build_assets.py bakes Goombella from geometry but makes Mario's
idle by cropping **TPL texture0 directly**, thumbnails its opaque bbox and pastes
it into an atlas. That pipeline discards geometry-to-image/root correspondence;
the geometry-baked Mario quad/root table above cannot be silently applied to
that differently cropped cell. Reusing source a/a_mario M_Z_1 geometry through
the same bake, or recovering the exact source texture0 model mapping and crop
transform, is necessary before binding these metadata to Mario's atlas. Native
scalar or physical body height alone cannot restore the discarded relation.
No assets or native code are changed by this documentation.

## Verification

Run `python3 tools/verify_overworld_actor_render_scale.py`. It pins source draw,
scale/init and matrix-adapter bodies/constants, reads both source model files
from the ISO into a temporary directory, and re-evaluates only the documented
frame0 converter geometry. It verifies hashes, animation IDs, triangle counts,
local bounds, derived native size and bake root/quad metadata. Extracted files
stay outside source history. The result validates current conversion metadata
and containing source owners, not original GPU output or full display fidelity.
