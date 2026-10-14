# Native stage dark and light display continuations

Source baseline: [original callbacks](BATTLE_STAGE_DARK_AND_LIGHT_CALLBACKS.md)
and [SDK numeric graph](STAGE_LIGHT_AXIS_ROTATION_AND_NORMALIZE.md).
Implementation: battle_stage_display.hpp/.cpp, render_stage_display.cpp and
actual TrioAttack finishing-camera callback bridge.

Each callback captures actual packed BattleWork once and retains W=B+163fc,
then calls cameraGetPtr(4). Stage alpha and twelve entry flags come from actual
mapped storage; initialization is not used as a substitute for live reads.
Camera pointer is retained and its published view is captured at the original
position-matrix setter. All completed GPU setters are retained across waits.
Missing services hold the exact source request, with no callback completion.

Dark always performs its nineteen prefix setters and three direct colored quads,
including alpha0. Each quad captures one source alpha argument; its held packet
retains that alpha. Later quads independently reread W+b08 or W+ae8. Original
opposite winding and depth compares6/1/1 are preserved. Current native reverse
window-depth convention maps these to PICA LEQUAL/GREATER/GREATER.

Light always performs its distinct twenty-setter prefix. It scans all12 actual
entry flags; inactive entries produce no triangles. Active entries capture axis,
width and vector, then each of18segments rereads start/end/center and recomputes
the previous vertex from the current unrotated vector and that NEW center before
rotation. RotAxis, MultVec and Normalize are separate retained requests. RGB is
read after math and captured into one complete triangle packet. A held triangle
never replays prior setters/math or recaptures its color. All18 real triangles
are required for an active entry; missing/degenerate finite math does not make
that entry inactive. Counter/writer ownership remains with original stage work.

Native numerical admission uses the source single-product/FMA graph with native
sinf/cosf and float1/sqrt in place of original libm/frsqrte estimates. Zero axes
and exceptional normalization results hold rather than inventing identity or a
safe direction. Exact estimate/FPSCR/subnormal/raster behavior is unverified.
This is a finite platform adapter, not a complete Gekko numeric implementation.

The renderer admits known unlit direct XYZ/RGBA descriptors, no texgens, one
raster-color stage and raster/zero alpha. It restores the acknowledged shared
program before each complete primitive and emits actual Citro3D immediate
triangle fans for four-vertex quads or triangles for the three-vertex beams.
Unused ST is supplied only to the existing native shader layout; source TEV
consumes none. No GPU Begin survives a frame or provider hold. Positions are
source units under projection*camera view, without actor/world scaling twice.
Blend/depth/channel/TEV/descriptor changes update shared source owners and stay
applied after the callback; no invented source restore is issued. Light uses
source-color/one RGB blend and source-alpha/one alpha blend, with source alpha0.
C3D immediate float24 conversion, source quad diagonal/culling and resulting
original pixels remain separate validation boundaries.

Tests use live packed-owner services and actual native logical shader/display
providers. They cover held source setters/quad/math/triangle, captured matrix
and alpha versus later live reads, retained owner after globalB changes,
three quad windings/depth states, full inactive light prefix, active18triangle
math and live center/RGB changes. Synthetic active fixtures are not original
execution evidence. Native actual command telemetry, remaining queue callbacks
and field restoration are recorded in PROGRESS and EVT-01. No new art is needed.
