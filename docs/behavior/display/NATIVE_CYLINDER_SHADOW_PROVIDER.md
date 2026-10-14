# Native cylinder shadow provider

Implements [the original admitted-record graph](CYLINDER_SHADOW_PER_RECORD_GEOMETRY_AND_BOUNDS_CADENCE.md)
with [verified native cull/logic/blend mapping](CYLINDER_NATIVE_CULL_LOGIC_BLEND_AND_LATE_DEPTH.md).
The deferred shadow scan selects this driver only when explicitly enabled;
whole-geometry external providers remain separate. Request/radius/type/prepared/
signed-camera admission stays in the outer shadow helper.

A per-record cursor captures the borrowed current view and prepared record
matrix at native FMA concat, uploads source position slot0 through the retained
projection/position cache, then captures record alpha into GX_TEVREG0. It does
not rescale authored vertices, run another ray, replace the prepared transform,
or change a shadow counter. Complete source calls and lists advance once.
Held matrix/material/palette/list/bounds requests retain their copied arguments.
Each pass freshly reads variant and each cylinder helper freshly reads flags/
opacity settings. Its palette/list plan remains pinned during later holds.
Pass3 freshly captures primary material color. Signed bounding slot and outputs
survive an unavailable final offscreen registration.

Pass1 performs the actual additive request then OR replacement, alpha-only
writes, back-face cull and vertex material. Pass2 switches to front-face cull,
overwrite alpha and zero register material. Pass3 uses back-face cull,
destination-alpha/inverse-destination-alpha RGB blending, disables alpha writes
and publishes fresh global material. Independent masks and disabled depth
writes survive each state change. The native shader retains vertex/register
material selection and A0 alpha across frame-state restoration and resets A0
only at an actual source dummy; position cache restoration uploads the last
acknowledged matrix without repeating source prefix or primitive calls.

Native immediate fan/strip draws decode the existing ignored
romfs/shadows.pmshadow pack: all original60 float positions, ordinary/no-decay
palettes and five lists. Every INDEX8 pair and winding is preserved, including
asymmetric endpoint floats. GPU loader uses unscaled authored XYZ, unused UV0,
and original palette RGBA. No triangle fan/strip is regenerated as an ellipse.
The default plan is inside,outside,bottom,side on each of three passes:
92 pairs/84 triangles per pass; no-decay has62 pairs/56 triangles. Finite a<=0
skips palette/list emission only; all preceding matrix/TEV/pass states still
run. Alpha0 does not suppress no-decay or cube dispatch.

Source cube variant1 retains genuine palette/list requests; native cube palette
and lists are unavailable, with no substitution by cylinder geometry. Camera1
bounds are required once around all three passes, including geometry-empty
helpers: sysWaitDrawSync80005ce4,ClearBoundingBox802b69d0,wait,ReadBoundingBox
802b6a08,offscreenAddBoundingBox80072f38. Native bounds service is unavailable.
Camera4/5/7 have no bounds calls. Nonfinite opacity inputs stay unavailable in
the finite native numeric adapter. Other source global/palette writers and
original PS/SDK arithmetic and raster comparison remain open.

C3D_ColorLogicOp OR genuinely writes alpha through the separate alpha mask;
AlphaBlend explicitly disables it. Source GX front1/back2 map to native
BACK_CCW2/FRONT_CCW1 under the retained clip tilt, not equal numeric enum values.
Destination-alpha factors6/7 map to PICA8/9. Existing native RGBA8/depth24 target,
PICA uniform/TEV quantization and late-depth no-write implementation are declared
adaptations; original six-bit EFB/blend rounding is not reproduced bit for bit.
This provider consumes actual incoming target color/alpha/depth; it does not
invent a blank image or set an image completion flag.

Host tests verify three-pass state/list ordering, fresh versus held captures,
empty/no-decay paths, source bounds cadence, native matrix restoration and
original imported list triangle counts. A standalone native provider probe can
exercise copied live prepared records while leaving the actual camera4 audience
callback pending. That probe does not establish ordered battle draw completion,
source framebuffer depth history, visible original shadows or field return.
Native loaded camera4 helpers run synchronously. The renderer clears/reuses its
target each frame; pixel contents across an unavailable provider AFTER emitted
geometry are not retained by the command cursor. Persistent framebuffer/replay
ownership is still needed before claiming those cross-frame render holds.
