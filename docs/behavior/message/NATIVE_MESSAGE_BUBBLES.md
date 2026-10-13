# Native ordinary dialogue bubbles

US GameCube G8ME01 revision0; implemented2026-10-13. This is the scoped
native harbor checkpoint renderer, not full message-service or raster parity.
Original IDs, tokens, dialogue bytes, speakers and event order are retained.

## Original request baseline

[Print events](MESSAGE_PRINT_EVENTS.md) provide NPC position plus NPC+150
height, snapshot at print entry, and selector0's projected speaker/Mario test:
abs(speakerX)>=300 selects mode2, speakerX<MarioX selects3, otherwise7.
The native harbor retains that actor root snapshot and caches placement for
each new message. Missing/unprojectable speakers take the direct-window adapter.

[Window creation](MESSAGE_WINDOW_LIFECYCLE.md) supplies default geometry
(-280,225,560,176). [Drawing](MESSAGE_WINDOW_DRAWING.md) supplies the
anchor-to-center alpha interpolation,10x10 opening size, full-opacity text gate,
ordinary text origin(x+62,y-36),100-high anchored text scissor and small-window
anchor offsets/origins. Coordinates are centered640x480,Y up. Native400x240
mapping is X=200+0.5*x,Y=120-0.5*y. Original640x480 art fits a
centered320x240 area, with40-pixel side margins on the400-wide display;
bubbles and glyphs keep their original aspect rather than stretching in X.

Additional read-only inspection: windowDispGX_Message US8007bad8,size2d0
calls _windowDispGX_Message US8007bda8,size1bec twice, r6=1 then0, for ordinary
types. The inner type0 arm at8007bec4 selects indices13/15 for r6=1 and9/11
for r6=0, with the other cap/edge in each pair. Original cap/edge placement is
(0,0),(488,0),(72,0),(72,-136), with widths72/416 and height176/40.
Constants at8042054c..80420578 are
560,176,72,-176,416,136,488,-136,40,-1,536,-24.
The native asset converter composes those four masks and a matching flat
interior into560x176 border images. Small masks are original indices3/4.

The shared tail arm8007d770..8007d944 checks mode&1. Base X is
x+width*(mode&4?0.7:0.38). Lower boundary Y=y-0.8*height;
endpoint Y=y-1.2*height. The inverted mode&8 arm instead uses boundary
Y=y-0.175*height and endpoint Y=y+0.225*height. Compare boundary against
speakerY; suppress a lower tail for boundary<=speakerY and an inverted tail
for boundary>=speakerY. Compute endpointX along the base-to-speaker line
and clamp against speakerX in its travel direction. Quad halfwidth=0.05*width.
These constants are binary32 at8042057c..80420594; tail indices1/2 selected
at8007d844..8007d868. The native plan reproduces these endpoint requests;
the native renderer submits two textured triangles for the sheared quad.
Texture rows and both end edges stay horizontal; no rotated-sprite approximation
or transparent-margin length correction is used. A dedicated12-vertex buffer
keeps shadow/normal submissions distinct until the GPU consumes them. The
renderer flushes citro2d before each tail and restores its state afterward.

## Controls and effects

The existing opening-domain parser handles key/page/wait/speed/scale/dynamic/
shake controls and suppresses a single LF following a control. Added pos,
scaleX, wave, col and their closing controls, small, clear and wpos based on
[parser records](MESSAGE_PARSER_RECORDS.md). Color records remain scheduled
and replay in exposed-prefix order, including controls above the clipping area.
Unsupported selection/icon/pose/paper/audio/noise services still reject the
message atomically rather than erasing controls or inventing their effects.
Malformed/missing numeric arguments reject atomically as a native safety adapter.

[Glyph effects](MESSAGE_GLYPH_EFFECTS.md) give dynamic relaxation
fma(0.5,dynamic-1,1), one-sided two-axis shake offsets0..2 and wave phase/motion.
Native shake uses a separate presentation xorshift adapter, not the original
shared RNG stream. Wave uses the unaccelerated fixed-tick elapsed millisecond
bridge, not character scheduling or rendering-frame counts. Dynamic takes
priority over wave; shake can accompany dynamic. ScaleX uses horizontal-only
scale except in dynamic's uniform branch. Original numeric/RNG/font-matrix
bit equivalence remains unverified.

Ordinary source vertical admission happens before effects. Admitted dynamic
records relax even when raster clipping hides their mask. Native UV/rectangle
cropping intersects glyphs with the window's vertical bounds and screen X;
partial glyphs are retained. This avoids altering shared GPU scissor state.
Full font matrix, history emphasis, material state and original indicator pose
(hoshi_2/HOS_C_1) remain unbound.

## Native ownership and limits

During ordinary dialogue and the entire locked harbor scene, top HUD/location/
interaction prompts and bottom menu/speaker captions/touch buttons/notices/FPS
are suppressed. The bottom screen remains clear; existing A/B/touch admission
continues. Overlays return when the scene ends. Development harness notices
also use the original ordinary border without inventing NPC attribution.

Projection reuses the actual world transform, including the handheld tilt and
aspect, then maps clip coordinates into the 2D screen space. Imported NPC tribe
height+150 is not yet available: Goombella uses the evaluated original pose
bounds; Crump/X-Naut use the opaque bitmap top/center. These native presentation
anchors are captured for each window relative to the original print-entry root.
Original getScreenPoint projection,
layer ownership and explicit placement-producing events need complete binding.
No matched Dolphin images or hardware measurements were obtained.

Host tests cover controls, malformed-input rollback, page/position layout,
small geometry, left/right/equality/offscreen/direct routing, opening interpolation,
tail suppression/clamping, glyph partial clipping and source speaker snapshots.
The isolated [capture script](../../../tests/azahar_textboxes.py) traverses the
original six-message scene and checks story4 persistence; images require review.

## 2026-10-13 — Tail-direction correction and aspect preservation

The prior rotated approximation visibly missed Goombella in
`artifacts/azahar-textboxes-final/bubble-page-08.png`. The current sheared
quad follows the original base/endpoint requests, while evaluated/art-derived
anchors replace the provisional height. Temporary isolated marker captures
verified the projected anchor on Goombella's visible head and the tail axis
toward that point. No marker remains in the production build. The original
shadow mask extends farther than the white mask; its tip is not the white
triangle's endpoint. Source tail support, alpha, event IDs/order and controls
are retained. Hardware and matched original-runtime raster parity remain open.
