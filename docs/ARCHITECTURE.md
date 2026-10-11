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
system is not implemented. Scene transitions reuse the currently loaded room.

The renderer loads original static Rogueport geometry once into a linear GPU
buffer, groups triangles by original material, and binds converted GX textures.
Character idle poses plus original Mario jump/hammer textures are baked from original layered geometry into a shared
transparent atlas. World geometry and paper billboards use a PICA vertex shader;
menus and text use citro2d. The battle stage also uses original geometry from stg_01. Encounter-specific
backdrops, props, audience and visibility rules still need comparison. Original animation playback
and room streaming remain future work.

Music is losslessly decoded from DSP ADPCM to PCM16 offline, preserving source
sample rate and channels. Runtime triple buffers read PCM from RomFS and queue
NDSP channel 1. Loop metadata is separate. Original MusyX banks are preserved
locally; sound-event macro interpretation remains unfinished. No synthesized
sound effects are used.

Save v1 is 32 explicitly encoded little-endian bytes: magic PM2D, version,
position float32 x/z, HP, mushroom count, completion/sound flags, reserved bytes,
and FNV-1a checksum. Loads validate content and positions. Writes close a temporary
file before replacement; a valid previous save becomes `.bak`. The 3DS backend
commits on close; host tests additionally use fsync. Battle state is transient.

Current settings are sound on/off. Saves are at
`sdmc:/3ds/ttyd-2ds/progress.bin`. Menu errors are shown on the bottom screen.
