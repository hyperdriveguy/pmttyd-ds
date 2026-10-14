# Retained original icon geometry and two-screen replay

The original [iconGX](ORIGINAL_ICON_GX_DRAW_SERVICE.md) quad provider now owns
actual copied geometry and native state after its real Begin. The auxiliary
item/AP callbacks can acknowledge that primitive without drawing before the
independent world adapter and having its output overpainted. The original
[material programs](NATIVE_ICON_MATERIAL_PROGRAMS.md) and
[resource bank](NATIVE_ORIGINAL_ICON_RESOURCES.md) remain required.

## Admission and publication

Each Trio runtime owns a distinct shared identity token and a monotonically
increasing draw-pass sequence. These are native admission metadata, not the
original GP frame counter, simulation ticks or rendering frames. The sequence
starts when a fresh original CameraManager draw cursor is entered; waits on that
cursor keep the same sequence. Only its actual whole-pass successful return
sets complete. Rendering never fabricates completion to expose partial draws.

`RetainedIconFrames` keeps a building pass and the last completed published
pass. Accepted quads copy original XYZ/ST, compiled material constants, raster
pipeline, actual PICA position transform, viewport/scissor/depth mapping,
original texture index and camera index. Later source setters, grayscale cleanup,
texture binds and resource waits cannot alter an accepted packet. Unsupported
camera indices, unavailable state, nonfinite geometry or inadmissible pipeline
return false without accepting geometry. The original cursor owns exactly-once
advancement after acceptance. Empty completed passes clear previous icons;
owner changes clear both passes. Stale sequences cannot append or publish over
a newer pass. There is no fixed packet truncation.

Both passes pin their texture indices during cache replacement. The native cache
has202 slots, derived from two working sets of100 item images plus the one
single-frame AP442 image. It imports only on demand, after FrameSync and outside
FrameBegin. The number is an adapter capacity from the original banks, not a
measured performance claim. The currently source-bound texture is also pinned.
No original image bytes or conversion change in this increment.

## Composition adapter

Scene camera4 packets replay after the world and audience, using the same
published source projection/view and handheld aspect adapter as the scene.
Unlike rebuilding a LookAt from eye/target, the published view retains roll and
translation. Native world vertices are sourceXYZ*.01, so the camera's spatial
columns multiply by100 for that independent world representation. Icon vertices
retain source units. Tests compare both paths numerically, including partial
viewports and a rolled/translated view.

The top screen preserves each packet's original adapted viewport/scissor and
depth mapping. Native tilted row1 is display horizontal; its scale is source
aspect divided by adapted viewport aspect. This affects renderer copies only,
not the original camera or gameplay screen cache. The lower screen embeds that
viewport into full top clip space and uses the existing centered320x240 scene
continuation. Its raster/scissor is the full lower screen, deliberately exposing
the audience below the original top-screen crop. This is a handheld composition
choice, not an observation of a second original GameCube viewport.

AP camera8 packets replay only on top, after the independent HUD adapter and
before fades. This does not establish original whole-HUD/source queue ordering.
Both screen replays publish GPU state directly; they do not rewrite canonical
GX registers, source bindings/cursors, clocks, animation, RNG or item simulation.

Unused native samplers bind the same live2D image as TEX0; the compiled TEV
program still reads TEX0 only. The installed Citro3D1.7.1 C3D_TexBind dereferences
its texture argument, so nullptr is not a supported disable operation. The native
probe exposed the previous null binds as unmapped reads at address0xc; the safe
bind probe must show no such accesses before emulator termination. Other native
render paths with null binds remain a separate repair frontier.

## Limits

Normal Main/camera cadence, all queued source providers and source frame
completion remain unfinished. The published packet set stays empty until the
real full draw pass completes; a fixture's explicit completion is not production
battle proof. The independent world still owns actor/pose snapshot admission,
so this increment does not certify whole original-frame coherence or raster
parity. Enabled fog, lit material programs and the original1x1 dummy texture
remain separate unavailable providers. No input, command menu, original story,
throw receipt, damage, tutorial completion or audio success is invented here.

The portable tests exercise pass holds, copied state,200 retained quads, image
pinning across two passes, stale/completed rejection, owner retirement, empty
publication, invalid raster/matrix data, aspect and world/source/lower clip
agreement. Native probe evidence and exact full validations are recorded in
[PROGRESS](../../PROGRESS.md).
