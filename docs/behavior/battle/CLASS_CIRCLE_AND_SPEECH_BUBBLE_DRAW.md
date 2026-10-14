# Original class circle and speech-bubble draw consumers

2026-10-14 static US G8ME01 revision0. Complete local bodies implemented:
DrawMainIconCircle801191a8/0x4a0 and DrawMainIconHukidasi80119648/0x474.
[The verifier](../../../tools/verify_battle_class_raster.py) pins both bodies,
battleMenuDispInit80119da0/0x58, original templates/constants/path and r2 base.
This is static recovery, not Dolphin observation or native GX equivalence.
[Ring routing](ACT_SELECT_RING_DRAW_AND_ICON_ROUTING.md) supplies actual row,
matrix, position and animation parameter. These functions do not run battle
input, advance story or infer audience events.

Both capture current-global B before camera8, call btlDispGXInit2D, install
source material color, one TEV stage, color/alpha op and input configuration and
texture order. Math providers preserve all source rotation, scale, translation
and concatenation calls. The retained camera's live matrix at+11c is multiplied
only at its original use point. Texture lookup follows B+163f8, owner+a0 and
its first palette pointer, requests original index0(circle) or1(bubble), then
loads that object into texture unit0. SDK/GX helpers, TEX palette-to-object,
icon and font bodies remain linked providers, not implicitly complete backends.

## Circle

Source gray is BYTE(192-truncate(1.5*positionZ)), without a native clamp.
Circle X/Y scale is fused1.3-0.005*positionZ, Z scale1. It rotates about Y using
the supplied parameter in degrees converted to radians, composes with the
retained row matrix, then the live camera matrix and loads position matrix0.

Shadow X offset is `(4-8*parameter/180)/1.3` for0..180, or
`(-4+8*(parameter-180)/180)/1.3` for180..360. Both inclusive tests run at180.
Outside those ranges the original f30 is unwritten; the typed port reports a
provider frontier after preceding texture/state effects instead of inventing an
offset. Source XY802ef650 and UV802ef6d0 templates are each copied into32-float
local arrays before primitives can mutate source memory. Four quad primitives
emit16 shadow points (offsetX, Y-3.0769231319, black/alpha128), followed by four
quad primitives emitting16 foreground points (source XY, gray/alpha255).

The icon translates by(0,7.5,0), scales by0.66*(1.3-0.005*positionZ) on X/Y,
and calls Y rotation followed unconditionally by PSMTXIdentity. The latter
replaces that icon rotation; it is not elided as an apparently redundant call.
After the source concatenations, read U16(row+0c);0 substitutes original403.
WORD(row+4)==1 chooses icon flags16, otherwise48. Source color alpha is kept
while RGB takes the byte gray. The color icon request receives the completed
matrix, actual icon ID and source flags.

## Speech bubble

The background starts with unit scale, translation(0,-5,0), supplied Y rotation,
row composition and live camera matrix. Its shadow X offset uses the same two
angle branches without division by1.3. Fresh WORD(work+1c) determines whether
left/right64/-64 are swapped. Black alpha128 shadow draws with Y76/-4 and
shifted X; white alpha255 foreground draws Y80/0 without the shadow offset.
The original128x80 bubble texture has its authored tail; no generated ellipse
or rounded-panel replacement is used.

Icon translation(-67,20,0), unit scale and Y rotation use the original ordered
concatenations. Read row icon with0->403 fallback and fresh row+4==1 for flags
16/48, then issue the ordinary icon call. Capture label pointer row+8 before
translation(0,52.5,0). FontGetMessageWidth returns a scalar whose low16 bits are
converted unsigned: width>90 compresses X to0.9*(90/width), otherwise0.9;
Y/Z remain0.9. Compose the text matrix, then FontDrawStart. Fresh row+4 selects
source black000000ff or gray808080ff after the width/start providers, then draw
the retained label with FontDrawStringCenterMtx. A native row snapshot must not
hide a provider changing enable state between icon and text draws.

## Port boundaries and assets

Request records retain original function identities, integer/float arguments
and matrix values. The optional return signals provider readiness, not a
replacement original ABI result; a present scalar0 is success for void requests.
False can occur after prior effects and does not authorize a fresh retry.
Transcendental/matrix SDK and GPU restoration remain provider responsibilities.

[The asset converter](../../../tools/battle_class_surface_assets.py) verifies
the original battle/common/battle_common.tpl hash and imports only original
indices0(40x40) and1(128x80) into a256x128 RGBA8 atlas with2px transparent
padding and unchanged decoded pixels. Metadata retains native UVs and original
palette indices. This matches the source constructor's pathname and source
consumers' indices; battle/win/menuwin.tpl is a different resource and is not
used. Private payloads are ignored; provenance and licensing are in
[ASSETS](../../ASSETS.md).

Tests check request ordering, source primitive/point counts and shadow colors,
gray byte, icon fallback/disabled flags, texture indices, fresh text enable
state, low16 width, wide-label X compression, mirrored coordinates and the
unwritten-offset frontier. Asset checks compare every atlas crop to its original
decoded image, descriptor UVs and transparent padding. These tests do not prove
native raster fidelity. Actual renderer texture binding and providers are still
required before replacing the development class-readiness acknowledgment or
activating normal Game tutorial command scheduling. Preserve the original upper
menu and [lower audience continuation](../display/NATIVE_BATTLE_HUD_AND_SUBMENU.md).
