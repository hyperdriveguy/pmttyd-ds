# Native unused sampler residency

The installed Citro3D1.7.1 `C3D_TexBind` reads `texture->param` at offset0xc
for texture units1 and2 before storing a binding. Disassembly of the local SDK
archive confirms the unconditional load for positive unit IDs. Passing nullptr
there produces native unmapped reads; it does not safely disable these units.
This is a native backend constraint, not original GX or battle behavior.

`Renderer::parkUnusedSamplers` binds units1/2 to the already imported original
font texture. Successful renderer initialization requires that image, and it
outlives transient menu, icon and item-get texture banks. The helper is used
only by an initialized renderer. It allocates nothing, loads no replacement
image, and does not alter source GX owners, game state, simulation or input.

The battle class renderer, ordinary field item/coin icon renderer and original
icon program read only TEX0 in their admitted TEV programs. They park the other
two units. The item-get effect parks first, then replaces units0/1 with original
pattern44 and opacity42 for its two-texture program; it parks again before
returning to Citro2D. The system message window still uses its three actual
textures while drawing each quadrant, then parks units1/2 before Citro2D.
No null bind remains in the project renderer.

The earlier [retained icon replay](RETAINED_ICON_SCENE_REPLAY.md) fix used its
same current source image for unused samplers. This increment uses the resident
font instead, so eviction/retirement of a transient source image cannot leave
an unused sampler pointing at that bank. The font is not sampled by these
single-texture TEV programs and does not change their displayed art.

Native fixture validation must exercise each repaired program, bank reload and
effect retirement, plus inspect a log captured before emulator termination for
unmapped accesses or native exceptions. Actual original icon/menu/window/effect
art must remain visible. Passing this fixture establishes the exercised native
sampler paths, not original story/menu command execution, whole-frame fidelity,
New2DSXL performance or hardware acceptance. Exact evidence and remaining battle
work live in [PROGRESS](../../PROGRESS.md).
