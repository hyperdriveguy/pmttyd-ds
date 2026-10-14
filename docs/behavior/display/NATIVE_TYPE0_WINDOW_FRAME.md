# Native original type0 window frame

Shared geometry and citro3d binding for the original
[untransformed EXP frame](EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md) and
[transformed selector frame/tab](ITEM_SELECTOR_WINDOW_FRAME.md).
`source/message/window_frame.cpp` is host-testable geometry; 3DS GPU services live in
`source/display/render_window_frame.cpp`. Original complete selector contents/lifetime
and matched hardware raster/performance remain unbound.

## Geometry and source requests

The ordinary helper retains the exact source absolute grid arithmetic. The
transformed helper starts at(-width*0.5,height*0.5), then retains the separate
rectangle center(x+width*0.5,y-height*0.5), rotation degrees and uniform scale.
All nine source quads, including the center, retain36 XYZUV vertices and source
one-third/two-thirds binary32 constants. Native triangulation uses54 vertices
in original cell order, each TL/TR/BR and TL/BR/BL. Finite geometry is caller
owned; no minimum width/height/border clamp or replacement shape is introduced.

The selector plan covers the documented decorations0/1/2. Its base frame uses
border20, definitionRGB and low8 manager alpha. Decoration1 adds original
RGB2ba4ff,150x28 tab with border8. Its signed-word centering computes
(width-150)/2 with truncation toward zero and wrapping add/subtract; it retains
the tab's own rectangle center instead of scaling/rotating around the base
center. This helper does not initialize manager lifecycle or draw text/icons.
Other original decoration branches require their own contracts/bindings.

## GPU binding and ownership

The shared original descriptor0,72x72 frame from f/msgWindow.tpl is already
resident as message-window-panel.t3x. Native UVs map the original grid through
Tex3DS's retained subtexture bounds; no texture/icon ID confusion, resampling,
baked tab recolor or extra GPU texture is added. Vertex RGBA multiplies original
texture RGBA. Source frame material uses no culling, always depth/alpha compare,
color-only writes, source-alpha/inverse-source-alpha blending and one texture
stage. GX enum/backend/raster equivalence remains an explicit future audit.

Citro3d's matrix chain is native viewport * T(rectangle center) * Rz * S, then
orthographic projection. The source degree conversion constant is retained;
native libcitro3d trigonometry/PSMTX arithmetic and PICA rounding are numeric
adaptations, not verified original SDK results. Half-size native screen reflow
occurs after the source pose, at center200/120 with X0.5,Y-0.5. Each frame is
submitted as one54-vertex triangle draw with a transform uniform; CPU does not
rotate every corner or issue nine rectangle requests.

An initialized native linear arena holds64 frame slices (124416bytes). This
is native capacity, not a recovered original manager limit. Each submission
uses a distinct slice and flushes only its1944bytes through libctru; deferred
GPU commands never alias a later frame's vertex rewrite. Counter resets after
synchronous FrameBegin in both native render paths. Shutdown synchronizes
before freeing. No heap/linear allocation occurs during a frame draw. The
method reports failure if resources/arena capacity are unavailable; future
callers must handle that native admission result explicitly. Current live EXP
owns one frame request within this budget.

Before drawing, flush pending citro2d content. After drawing, restore its shader,
attributes, buffers and TEV through Prepare/SceneBegin, then explicitly restore
depth-disabled/color-only UI and ordinary alpha/blend settings. Installed
citro2d Prepare re-enables GEQUAL/depth writes and does not restore alpha/blend;
relying on Prepare alone would let stage depth obscure following EXP/PointGet
content. Review caught this and the native binding now restores all used state.

## Live use and validation boundary

The original trio EXP frame now uses this shared geometry/GPU path with its
unchanged rectangle(-76.8999939,21,165.7999878,24.1999989),border10 andFFFFFF80.
EXP icon/PointGet order and their original animation clocks remain separate
owners. Full item-selector UI is not newly enabled here.

Host tests independently check all36 source grid vertices/UV bits,54 triangle
references, separate source base/tab centers, signed odd centering, low8 alpha,
definition RGB and narrow folded geometry without native clamping. Builds and
isolated native capture are recorded in PROGRESS. Captures establish native
admission/rendering evidence only; they do not prove original SDK/libsin or
GX/PICA pixel parity, original camera8 lifetime, or measured New2DS speed.

Next: bind selector manager display order/sequence ownership, all original
list/header/help text/icons/scissor and GP/key/status/audio services; verify
rotated/scaled three-window captures and compare source/runtime/hardware.
