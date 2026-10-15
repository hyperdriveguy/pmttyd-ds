# Original camera8 window primitive packets

The complete retained `windowDispGX_Waku_col8007b44c/68c` feeds
`BattleWindowPacketProvider` through its real GX requests and180 float FIFO
lanes. It captures one immutable packet containing all36 original vertices,
nine ordered quads, loaded source matrix and late-loaded TEV color1. It does
not call `Renderer::sourceWindowFrame`, tessellate a replacement panel or apply
that older consumer's independent message-window ortho/reflow.

## Source ownership and admission

The provider accepts the actual finite original attribute/material protocol:
XYZ/ST float attributes, zero channels, one texgen/stage, color=C0*TEXC,
alpha=A0*TEXA, no fog/culling/alpha cutoff, ALWAYS depth compare without depth
writes and source-alpha blending. Unsupported calls/tuples remain unavailable.
Each source CurrentCamera uses source/camera currentness without requiring an
unused atlas or the preceding window texture to remain resident. Surface capture
is deferred until actual LoadTexObj; older packets keep their original proofs.
Each source CurrentCamera begins fresh material coverage, so an omitted setter
cannot inherit success from the preceding window. Its actual query/camera proof
must be supplied by the real source context; no fallback guest camera exists.
Original completion0 can return after CurrentCamera without native import or
geometry. No completion flag or private source output is initialized here.

`GXLoadTexObj` requires actual texture object address map0, complete original
SDK words, descriptor0 metadata and an actual strong current source owner. The
finite expected source is72x72 CMPR, clamp/clamp, linear/linear, zero bias/LOD.
Byte comparisons validate a supplied genuine object; they do not construct an
object or prove its producer. A true `windowTexSetup`/palette/TEX SDK owner must
supply this witness. A copied descriptor, index-as-handle, fake BSS pointer or
forced `8041ea1c=1` cannot replace it. Missing source allocation, camera, numeric
or texture providers hold at the real original child. Source globals/palette
initialization and original SDK matrix services remain separately owned.

Successful FIFO lanes are captured once. The last lane produces the immutable
whole window only after all180 floats exist and passes its exact retained
packet to the ordered sink. A held sink retries that same final request/packet;
no earlier lane/quad is replayed. Reentry is rejected. Source/native generation
loss poisons partial work. The outer source frame must discard the mixed batch
if any allocation/resource proof is lost after an append.

A packet append is preparation, not acknowledgement of a queued source display.
The whole weapon callback must seal, preflight and actually submit one mixed
font/submenu/icon/window sequence in original source order before camera finish.
The packet's actual Camera8 projection/viewport/scissor/depth and source object
owner remain current through admission. The existing camera proof type is reused;
no opaque replacement window child is admitted by an independent redraw.

## Actual native art and clamp padding

Original `f/msgWindow.tpl` descriptor0 is the panel, distinct from the composed
ordinary-dialogue atlas named `message-window.t3x`. The older native `messagePanel`
import uses `message-window-panel.t3x`, a72x72 subimage in128x128 with transparent
padding. Original outer right/bottom edges contain opaque pixels. Its adjacent
padding differs in36/72 rows/columns; linear sampling at the source clamp edge
would fade them. It therefore fails this resource's strict pixel admission.
Existing panel imports/converters remain unchanged.

New `battle_window_panel_assets.py` reads the original321984-byte TPL from the
read-only ISO, SHA256
`2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67`.
It preserves all72x72 source pixels at0,0 in128x128 RGBA8. Every right, bottom
and corner padding pixel repeats the corresponding last source edge. No source
pixel is resized or rewritten. Native UV is `(u*72/128,1-v*72/128)`; full edge
replication plus native clamp preserves original linear clamp for source UV
outside0..1 as well. The output is a separate `battle-window-panel.t3x`, JSON
provenance and private PNG. Empty output is required; no existing editable asset
or extraction cache is overwritten. Source/interior and full atlas SHA hashes
are recorded in the JSON; full row-major RGBA FNV64 is `caadb76327b7d2be`.

`BattleWindowSurfaceResource` validates actual128x128 RGBA8, one unrotated full
subtexture with exact0/1/1/0 UVs, and ALL actual tiled ABGR allocation bytes,
decoded to original row-major RGBA for the full pixel guard. Metadata alone
cannot admit a texture or transparent padding. Every adoption creates an opaque
generation; retirement invalidates all copied leases before native release.
Address reuse cannot revive an old packet. Native load uses actual Tex3DS/C3D
storage outside FrameBegin, linear/clamp sampling, no lazy callback import.
Release synchronizes while C3D remains alive before freeing the paired import
and texture allocation. The caller owns overall frame/target/shader lifetime.

## Native replay and integration

`NativeBattleWindowSurfaceGpu::canDraw` preflights the entire window resource,
source owner, camera/raster, vertices, matrix and final finite transform. Replay
uses actual source projection times the loaded matrix, with a single native
clip-axis tilt. The source matrix already contains camera view. It then submits
all nine GX quads as ordered triangle fans with their exact XYZ/ST/color.
No default view, selector ortho, second camera multiplication or UI rescaling is
applied. Finite packet admission checks every binary32 composition product/sum and all36
shader clip products/sums. Finite offscreen, transparent, zero-area and singular
geometry remains admitted. Fixed ordinary material resets include fog and early
depth state. The genuine source/GX context must additionally prove the ordinary
global table0/indirect state used by this finite mapping; source Window itself
does not initialize those global GX tables, and no normal provider is fabricated.
Whole combined mixed-batch admission must precede any native GPU state change.

The Renderer methods in `render_battle_window.cpp` require the unapplied exact
header/shutdown journal before compilation. Preparation is explicit outside a
frame; ready-only access never imports. The new owner is optional and no normal
camera/queued callback is activated. The mixed typed publication owner can add
`BattleWindowPacketPtr` and call `validateBattleWindowPacket` during its whole
preflight, then native replay at that exact source append position.

True source window archive/DVD/palette/object setup remains the normal binding
frontier. Original source completion flags and actual camera8 readiness are
required. No GPU/vector alone acknowledges a source callback. Physical controls,
original menu placement/style and lower-screen scene/audience continuation are
unchanged. This module introduces no touch-only controls.

## Validation, provenance and limits

Host tests decode the actual original CMPR0 pixels from the ISO, admit their
padded native representation, compare every captured source vertex to the
existing original type0 reference, hold final publication, check reentry,
missing true object queries, resource loss, no-draw completion without import,
metadata/pixel/padding rejection, stale leases and native tilted transform.
Python checks every source/padding pixel, independent bilinear samples including
out-of-range coordinates, source identity and empty-destination preservation.
Strict owned-file host, ASan/UBSan, GNU ARM and the existing whole source/FIFO
verifier are scoped separately from root's combined gates. Unchanged dependency
warnings remain. No actual native import/render run or original framebuffer
parity is claimed by these host/ARM checks. Such a probe remains separate.

Original Nintendo/Intelligent Systems rights remain; source tracking grants no
new license. No ISO, original extracted asset or compiled pack is committed.
The exact asset/build/Renderer registration journals are unapplied artifacts.
