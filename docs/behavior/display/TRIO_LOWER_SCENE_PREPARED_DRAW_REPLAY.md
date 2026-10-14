# Trio battle lower scene replay

The independent native trio world now supplies both screens. Its top scene
prepares geometry once and captures the map ranges actually submitted after
source stage visibility filtering, including the texture descriptor after wrap
selection, cull mode and depth write mask. The transient actor stream records its
exact source-prepared ranges. Descriptors borrow the existing GPU texture data;
no extra atlas or actor animation sample is allocated for the lower view.

After top overlays, the lower target clears color and depth and replays sky,
opaque map, prepared actors, translucent map and prepared original audience
passes. The existing handheld clip continuation maps vertical X to X+2W and
horizontal Y to 1.25Y, retaining depth and W. This places the lower320x240 view
below the top400x240 view with a centered40-pixel crop at the same pixel scale.
The replay writes no geometry and reads no source actor, stage or audience state.
It does not call source event, animation, camera, control or rendering producers
again. Frame preparation occurs after the synchronized frame boundary; borrowed
resources remain resident through both submissions. Lists reset on each top
world preparation, and an unavailable top world cannot replay a stale scene.

Field proxy characters, field NPC sampling/drawing, field shadows, departure
presentation and field item-get effects are suspended while the source trio
battle owns the scene. Actual trio stage and actor owners continue to draw.
The existing command/target rows retain original type0 window art, original US
font, icons and selection hand. Harness action notices receive original type0
frames to keep them readable over the continued scene. This is presentation
reflow; physical command/action input and simulation timing are unchanged.

Until the original camera4 producer publishes a view, trio framing uses the
existing native tutorial camera adapter: eye(0,2.8,5.5), at(0,1,-.4), FOV42,
near.1 and far80. These are provisional native presentation constants, not
measured original values. A suspended field camera no longer frames the battle;
original published battle eye/at/up still take precedence. Audience GetFront uses
the same fallback/live eye and at as the visible world. Complete original lens,
framing and producer cadence remain open.

This replays the independent native world adapter, not the separately queued
original camera/display/GX traversal. Original camera4 framing, queued effects
and shadows, class-ring choreography, normal-story battle admission and complete
turn/tutorial flow remain separate unfinished work. Windowed harness notices
are development presentation, not replacement story dialogue. Original dialogue
IDs, tokens, order and conditions are unchanged. No audience count is granted by
this renderer and no production path creates a development fixture pouch.
