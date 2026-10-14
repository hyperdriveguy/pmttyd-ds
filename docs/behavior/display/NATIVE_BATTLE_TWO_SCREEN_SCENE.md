# Native battle scene continuation across both screens

2026-10-14 native layout adaptation for the original harbor tutorial stage.
The user's original-game screenshots and rough two-screen mockup establish
[the layout requirement](NATIVE_BATTLE_HUD_AND_SUBMENU.md): upper stage/HUD and
lower audience scene, with physical controls and optional audience taps later.
This increment continues the actually loaded original scene; it does not create
fake audience members, throw events or touch-only controls.

The upper400x240 camera remains unchanged. The narrower320x240 lower view uses
the same scene, actor snapshot and pixel scale, cropped40 pixels from each side
of the upper width and showing the next240 pixels vertically. The physical
hinge is not represented as extra world distance in this adaptation. Native
camera framing remains the current prototype; this is not original camera
recovery or a measured GameCube view. Exact useful audience framing still needs
visual evaluation as original audience actors are bound.

Citro3D tilted clip X is display vertical and clip Y is horizontal. The lower
clip transform is X'=X+2W,Y'=1.25Y,Z'=Z,W'=W. Applying it to the combined
projection/view matrix adds twice row3 to row0 and scales row1 by1.25; depth and
homogeneous W are preserved. Homogeneous coordinates are transformed before
division. The crop compensates the lower width, so the same world edge does not
stretch when crossing between screens. Matrix and point tests assert the seam,
40px side crop,240px vertical continuation, equal pixel scale and unchanged Z/W.

Both native render targets now have actual depth buffers and screen-specific
output bindings. On tutorial frames, bottom is cleared with scene black and
renders the original background/stage and same actors through the continued
matrix; ordinary lower UI surfaces retain their existing path. It does not
repeat the upper HUD, message windows or screen-space startup curtain. The top
world matrix is restored after lower drawing for subsequent native consumers.
Both world passes consume the same immutable simulation snapshot; no additional
VM, action timing, animation-completion or gameplay tick is performed.

The lower pass reuses the exact top-pass actor vertex ranges without rewriting
the shared arena or reevaluating animation. Source poses/root transforms/colors/UVs
are unchanged, and top overlay/curtain ranges remain intact. Map buffers
are immutable cached original geometry. Existing top overlay and original
music/dialogue paths retain their own owners. Audience actor construction,
original audience display/animation, harmful-item telegraphs/targeting and
physical-equivalent optional tap controls remain incomplete. Continuing the
scene is not a claim that its audience simulation is finished.

## Isolated visual evidence

The direct-tutorial graphics harness under
artifacts/battle-dual-view-lvo3xlzi/artifacts/azahar-dual-view-graphics exits0
without a native exception. Inspected tutorial-two-screen-79.png shows the
original stage/actors/HUD above and original empty seating/floor continuing into
the lower view. Centered crop, downward orientation and visible seam are verified
in that harness. It temporarily injects beginFresh(0,7,10,4) into the isolated
snapshot entry only; the normal main source was restored and is not changed by
this feature. This is graphics evidence, not proof of normal story admission.
The two earlier checkpoint runs remained in the field and did not exercise this
view. The checkpoint was already room1; a mistaken field-offset adjustment only
changed HP in a separate generated fixture and is not part of the implementation.
Audience sprites/events and original camera/raster equivalence remain open.
