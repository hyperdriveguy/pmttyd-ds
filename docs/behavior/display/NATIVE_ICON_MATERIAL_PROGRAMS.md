# Native original icon material programs

The native audience icon path now binds its actual map-light/map-work/blend/fog
services and compiles the original flat icon TEV programs for PICA. The later
[retained geometry adapter](RETAINED_ICON_SCENE_REPLAY.md) binds the primitive
provider and publishes only completed source draw passes. No audience throw,
tutorial or normal two-screen raster acceptance follows from material bindings.

## Source and state ownership

[Whole iconGX](ORIGINAL_ICON_GX_DRAW_SERVICE.md) supplies ordered setters through
its real retained service path. `NativeIconProgram` owns their canonical channel,
register, stage, swap, texture-coordinate, descriptor, cull and fog state.
Setters update that owner, including partial state across a wait. A complete
supported plan is published through actual Citro3D calls before GXBegin is
acknowledged. They are not no-op acknowledgements or inferred per-icon colors.
Unknown functions/lighting/fog domains remain unavailable. Incomplete or
unsupported stage combinations cannot produce a plan.

Normal source iconGX calls mapSetMaterialLight800208c0 with flags4. The recovered
[light helper](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md) uses its actual selected bank and
ambient query even though flags4 selects no lit channels. It sets material white
and constant source channels. Native binding runs that helper with the existing
battle light owner; an unknown bank does not become an empty-light success.
Flags16 instead supplies its original unlit channel setters. Both used paths
have disabled lighting and register material; dynamic lit channels need their
actual separate shader support.

mapGetWork resolves the actual primary material record through native identity
85000000, with its flags available at+4. This is an adapter identity, not original
RAM. mapGetBlend8001d968/34 hex was freshly inspected read-only: source SHA256
`eac895191334808cbd8a5388360bbcece852149c8a97440cbeea09983c96b0f9`.
It independently tests current group half4 bit2 and copies raw RGBA at group+16
hex (`MapMaterialGroup.colors[0]`); otherwise original DATA8041f920=ffffffff.
This differs from the separate last-stage blend helper's C2-red alpha arithmetic.
No invented white enabled group or secondary-stage fog is substituted.

Fog binding runs the existing actual current-camera/current-group helper. Its
real fog-off request is retained and published. Enabled source fog remains
unavailable pending the native equations/LUT. Display blend, alpha comparison,
Z-comparison location and depth updates use the existing actual display-state
owner. Icon shader compilation does not override its source write masks.

The declared cold graphics adapter retains identity swap table0. Verified
__GXInitGX802b2a24/938 source SHA256
`1fe7584e6f0ac1a2b97209bc38315a7da4ad4aec5e16acb98ff3624f97451e8d`;
802b30bc..30d0 installs table0=(R,G,B,A). Source writes to table0 are recorded;
a nonidentity inherited table prevents compiling the normal path until supported.
General cross-material shared-table writers still require their actual owners.
Grayscale explicitly installs its own tables1/2/3, independently of table0.

## Exact supported programs

Normal2 stages perform texture times actual constant channel material, then
PREV times source TEV color1 (the icon RGBA). An admitted source map blend appends
PREV times color2 as the third stage. PICA uses the same successive stage order,
with per-stage constants and independent RGBA handling. No inherited actor K0
program substitutes for these icon registers.

Grayscale3 stages replicate texture R/G/B separately, multiply each by original
TEV color1 RGB (normally555555), and accumulate through PREV with source clamp
and scale requests preserved. The final alpha is texture alpha times source
color2 alpha. Color2 RGB does not tint this grayscale path. This is the original
equal-component program, not a weighted luminance conversion. PICA uses three
RGB stages with multiply/add and the corresponding independent alpha stages.
Original GX internal precision/rounding equivalence remains unproven.

All six PICA stages are initialized at publication. Unused stages pass through;
unused texture units are cleared. Bind the actual resident source cell, actual
cull request and real fog-off color. No replacement bitmap or CPU grayscale
image is created. The source primitive has XYZ/ST only. The compatible native
vertex shader may receive a constant color attribute, which this source program
does not consume; it must not become an invented vertex material.

The complete plan is captured before the original grayscale swap-mode cleanup.
Cleanup changes current canonical state, not a captured draw plan. Captured plans
are cleared when source texture residency is retired. The future primitive owner
must retain/reapply that plan together with its own texture, transform and display
state. Accepting setters or Begin does not acknowledge a missing quad provider.

## Validation and remaining scene work

Host tests drive the whole original iconGX body through actual recovered portable
light/fog helpers into the new state owner. They cover all used flag/material
combinations, primary blend admission, ordinary/gray/alpha/cull paths, inherited
state across successive icons, linked waits, post-draw cleanup versus captured
plans, unavailable banks, nonidentity table0 and enabled-fog refusal. Test banks
and changed colors are explicit fixtures, not original execution observations.

A temporary heap-based native entry point renders original icon108 (the original
A-button artwork) in six orthographic fixture cases: ordinary, tinted, grayscale,
map blend, full-size alpha and full-size grayscale alpha. It runs original iconGX,
real original resource/object binding, recovered light/fog helpers and the actual
production PICA publication. The final quad/orthographic/depth-disabled submission
is an explicit fixture; production primitive binding remains absent. First probe
attempt exceeded the temporary main's stack allocation. Moving large Game/light
fixtures to heap storage, as the normal runtime does, resolved that harness issue.
No production stack limit was weakened.

Inspected isolated capture: `artifacts/azahar-icon-material-heap/boot.png`.
A scalar RGBA reference compared824 constant2x2 decoded original source patches
per case (4944 comparisons). Maximum channel error was0 for normal/tint/blend/
alpha,1 for grayscale cases. This verifies the native channel/alpha/dataflow on
that fixture; it is not a Dolphin/GX pixel-parity comparison or hardware benchmark.
Private check: `artifacts/icon-material-pixel-check.json`. The original user
emulator profile/saves were not touched; temporary main is excluded/restored.

The queued source pass currently runs before the independent world adapter in
Renderer::render. Production primitive work must retain complete packets and
replay them at the correct scene stage, including the lower-screen continuation,
while preserving accepted geometry across source waits. Directly submitting an
item at the current callback point would not establish correct scene composition.
Full GP/Main/camera cadence, enabled fog, complete presentation/input execution
and source-frame ownership remain open. Existing menu art/layout, simulation
clocks, original dialogue and physical/touch control behavior are unchanged.
