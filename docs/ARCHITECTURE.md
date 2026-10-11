# Architecture

Native C++17, devkitARM, libctru, citro3d, citro2d. `source/game.cpp` and
`source/save.cpp` compile on both the host and 3DS. Platform code owns input,
RomFS, GPU objects, NDSP, and SD access. Start with README and PROGRESS.

The simulation advances at 60 Hz with input edges consumed once. Rendering has
its own cadence. Catch-up is capped at five ticks; long interruptions reset the
accumulator. Dropped ticks are counted. Action timing is currently provisional.

Exploration, dialogue, battle, victory and defeat are explicit states. Battle
phases are select, windup, impact, enemy windup and enemy impact. Goombella is a
spectator in this first encounter. Only Mario acts; the broader partner combat
system is not implemented. Outdoor roads stream the destination room and commit validated arrivals; battle scenes reuse the resident stage.

The renderer loads the current original Rogueport room into a linear GPU
buffer, groups triangles by original material, and binds converted GX textures.
Destination textures and geometry are staged before GPU synchronization and
disposal of the previous room; failed loads keep the current rendering.
Character idle poses plus original Mario jump/hammer textures are baked from original layered geometry into a shared
transparent atlas. World geometry and paper billboards use a PICA vertex shader;
menus and text use citro2d. The battle stage also uses original geometry from stg_01. Encounter-specific
backdrops, props, audience and visibility rules still need comparison. Original animation playback
remain future work. Cameras follow player X/Z and floor height smoothly;
original event-script camera rails and foreground occlusion remain unfinished.

Music is losslessly decoded from DSP ADPCM to PCM16 offline, preserving source
sample rate and channels. Runtime triple buffers read PCM from RomFS and queue
NDSP channel 1. Loop metadata is separate. Original MusyX banks are preserved
locally; sound-event macro interpretation remains unfinished. No synthesized
sound effects are used.

Save v2 is 32 explicitly encoded little-endian bytes: magic PM2D, version,
position float32 x/z, HP, mushroom count, completion/sound flags, room ID, signed floor elevation at .1 resolution,
and FNV-1a checksum. Version 1 migrates to plaza at zero elevation. Decoding validates structural
content independently of active collision; runtime loading validates the saved
room floor and wall clearance. Writes close a temporary
file before replacement; a valid previous save becomes `.bak`. The 3DS backend
commits on close; host tests additionally use fsync. Battle state is transient.

Current settings are sound on/off. Saves are at
`sdmc:/3ds/ttyd-2ds/progress.bin`. Menu errors are shown on the bottom screen.

Collision retains original wall height intervals and walkable triangles.
Each successful load builds a two-unit XZ grid, indexing wall bounding boxes
inflated by player radius and triangle bounding boxes. Queries visit candidates
from one cell, then use exact capsule distance/barycentric checks. Original-data
host probes compare indexed answers to an exhaustive scan. Stacked wall
segments retain distinct height intervals. The minimap fits original collision
footprints to its panel and filters walls to the current elevation.

Room collision staging uses CollisionTransition: it retains the previous
walls, vertical levels, ground triangles, spatial indexes and footprint bounds
in memory. Its destructor restores the previous state unless the destination
is fully accepted and committed. Destination collision can therefore be rejected
for bad files, an invalid arrival or failed rendering without rereading old
files. Startup save restoration uses the same mechanism. GPU synchronization
also precedes disposal of partially loaded room rendering on failure.

Exterior-wall cutaways use original scene hierarchy under S_gai_kabe nodes.
The converter batches these meshes by wall group while keeping other geometry
batched by material. High bits of batch.layer carry the group; the low byte
remains the original layer. Nested drawable wall pieces inherit an ancestor
group. Renderer builds per-group bounds and tests camera-to-character segments
against actual original triangles. Obstructing groups are skipped; an eight-tick
hold prevents immediate toggling. Collision is unaffected. Battle geometry does
not use this adaptation. Original event-controlled visibility remains unfinished.
