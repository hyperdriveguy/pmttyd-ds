# Original footer texture and ordered GPU composition

Native integration of the retained
[class frame](NATIVE_COLD_CLASS_FRAME_PIPELINE.md) and original common-bank
footer4..16 assets. This does not install ordinary battle scheduling or establish
hardware performance/original GX numerical parity.

`resumePackedBattleTexturePlane2` retains whole source8010323c/8c,
Init801036e4/e0 and PlainGX801033e4/300. Original Init requests precede matrix
copy; current-camera lookup precedes view concat/load; RGBA is read after that
load. Actual BattleWork/FileEntry/pointer-cell resolves the palette. Texture
format/width/height are retained queries; every original FIFO float and byte
write is separately acknowledged. Matrix/color object identities and provider
tables remain stable through waits. Missing services retain their boundary;
no guessed camera, descriptor, dimension, palette or successful leaf is supplied.

`BattleClassFooterGpuResource` requires the original footer catalog and actual
512x256 RGBA8 texture admission. The native owner imports the real t3x before
FrameBegin and releases it while C3D remains live. Each admission has a fresh
native generation, so old copied packets cannot acquire a reloaded texture at
the same native address. Host import callbacks are explicit test providers.

The source provider handles both paths: party/audience button Sub invokes the
retained TexturePlane2 helper; pinch/danger/critical invokes the original raster
TextureObject/LoadTexture/LoadMatrix/Quad stream directly. A combined router
selects the active texture owner after successful TextureObject; trying a
permissive old class backend before footer handling could otherwise draw an old
ring surface. Matrix/Init2D setters reach the appropriate owners. Original
CMPR14 format and dimensions are read from the actual executing VM's relocated
97-image bank via a bounded optional reader, not inferred from an atlas index.

Successful geometry appends a copied footer packet at its exact position in the
class backend's existing ordered quad vector. The immutable publication retains
that packet and resource generation. Native draw preflights the whole batch,
including every footer generation, before GPU/arena effects. Immediate footer
draw uses the same caller projection/shader, and later ordinary quads restore
their arena binding and material. There is no separate later footer overlay.
Class/menu and footer resources must be prepared before frame drawing; render
does not import unavailable textures inside FrameBegin.

The actual tutorial fixture installs this provider before its first source
pass, uses its explicitly declared native fallback camera, reads real source
descriptors and changes the actual Mario unit HP halfword. Its GPU admission is
host supplied unless the native Renderer supplies its real resource. Neither
this fixture nor these resource APIs prove normal BattleInit/End, btlDispMain,
camera-driver scheduling, source alert audio, complete partner menus, or live
audience targeting.
