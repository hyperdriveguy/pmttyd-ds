# Trio consumes its actual camera4 lens

The independent native trio world previously used a provisional42-degree lens,
near.1/far80, even though its retained CameraManager already owned the recovered
source lens. It now reads camera4 through the actual TrioAttack camera owner.
`battleSceneLens` reads FOV unchanged and converts source near/far distances by
.01, matching the world-coordinate conversion. The recovered boot lens25 degrees,
near1/far32768 therefore becomes25 degrees, near.01/far327.68 native units.

The helper rereads the owner for every preparation; it neither copies the boot
constants into another owner nor writes camera state. Changed lens fields are
consumed without resetting the camera. Missing owners, non-perspective camera4,
nonfinite values, FOV outside(0,180), nonpositive near or far<=near yield no native
world preparation. Synchronized readiness reset prevents stale lower replay.

Handheld400x240 aspect remains the declared presentation adaptation; the lower
view replays its centered320x240 continuation from the same top projection.
This change consumes live lens fields, not the original cached GX projection
matrix or its flag0x40 refresh cadence. Original projection-extra publication,
roll/offset and view-producer cadence remain separate work. Published battle
eye/at/up still take precedence when available; the existing provisional native
eye(0,2.8,5.5)/at(0,1,-.4) fallback remains until that producer supplies a view.
Audience facing uses the same eye/at. This is not original camera/raster parity.

Host tests check the retained original boot values, live owner changes, unit
conversion, source-state preservation and unsupported/invalid inputs. Existing
clip continuation tests still verify seam continuity, crop and preserved depth/W.
Native original-resource graphics testing verifies the resulting scene framing
separately. No actor/model scale, asset, control, story or simulation-time value
is changed by lens consumption. Complete battle/camera ownership remains open.
