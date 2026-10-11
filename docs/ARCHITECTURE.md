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
batched by material. Bits 8–23 of batch.layer carry the group; the low byte
remains the original layer. Nested drawable wall pieces inherit an ancestor
group. Renderer builds per-group bounds and tests camera-to-character segments
against actual original triangles. Obstructing groups are skipped; an eight-tick
hold prevents immediate toggling. Collision is unaffected. Battle geometry does
not use this adaptation. Original event-controlled visibility remains unfinished.

Room ID 4 is Rogueport station (`gor_04`); existing IDs and save v2 layout remain
stable. Save decode now validates the room registry, avoiding a duplicated
four-room limit. The offline road-table assignment requires distinct REL tables
and reciprocal original destinations across all packaged rooms, rejecting zero
or multiple solutions. This resolves station's north/south marker ambiguity
without inventing a connection. Eight directed roads connect the five rooms.

Map batch layer bits24–25 preserve the original scene node GX cull index
(0 front, 1 back, 2/3 none). Material/group/cull form the batching key;
otherwise differently sided meshes sharing a material would render incorrectly.
For the current 3DS view transform, GX back maps to GPU_CULL_FRONT_CCW and
GX front to GPU_CULL_BACK_CCW; direct enum mapping removed the plaza floor
in the first emulator check. Character/backdrop rendering resets culling to none.
No original triangles or collision surfaces are dropped during conversion.

Offline map animation sampling is explicit (`map_convert.py --animation NAME
--frame FRAME`). Original local bind transforms compose with sampled delta
transforms and parent hierarchy before baking. Only visual scene nodes are
sampled; collision previews stay static. Runtime packs continue to use bind
poses until original event conditions and node-to-vertex bindings are recovered.
Original durations do not establish simulation frame rate or loop semantics.

Material preview samples replace the bind texture matrix before applying raw
GX UV coordinates and Tex3DS V inversion. S and T scales interpolate independently
(the upstream sampler text has a cross-channel scale typo; this implementation
uses each original channel). Original keyframes and centers are authoritative;
loop/rate/activation remain event work. map_event_inventory.py preserves command
opcodes/order and operand relocation provenance, without flattening branches into
a default scene state. It requires the separately obtained US symbol table.

Offline hidden-node selection now suppresses visual descendants recursively,
without leaking to owner siblings or changing separate collision geometry.
Station pack uses the original road-entry visibility branch (gondora_2/hako_1
hidden); future hikousen_return support must retain/restore those geometry
groups at runtime instead of treating this build-time subset as universal.
Original map flag2 is not a hidden flag: mapDisp branches around clipping work
when it is clear. Native renderer currently has no analogous per-node frustum
clipping, so clearing this bit requires no additional mesh suppression.

Runtime material animation: version1 UVAN files hold per-integer-frame affine
matrices and original pre-bind UV/vertex-index bindings. Portable loader validates
magic/version/counts/finite values/index bounds/unique bindings/exact EOF and
only replaces output on success. Original authored duration is the loop period;
frame is elapsed fixed60Hz ticks modulo duration. Room load resets animation
epoch; field animation currently follows the simulation clock through battle.
Original event/pause behavior beyond the identified init clips is unfinished.

Renderer updates only bound UV vertices after synchronized frame begin and
flushes their contiguous ranges from CPU cache before GPU drawing. Scene
positions/colors/collision stay unchanged. Room staging owns animation vectors
with the other resources; rejected imports retain previous scene state.
East room activates original init's three mesh-free material clips. Plaza and
harbor also load MSAN mesh tracks, as described below. Secondary-texture
animation remains unsupported at runtime.

Runtime mesh animation: MSAN v1 stores 3x4 world delta matrices per integer
frame and uint32 vertex indices with original bind XYZ. Conversion retains
node ancestry and bakes bind*delta composition through each affected owner's
ancestors, conjugating translations into native map units. Renderer applies
matrices to immutable bind positions (no accumulated drift) and updates cache
ranges after synchronized frame begin. UV and mesh tracks share the room epoch.
Loader shares bounded/finite/unique-index/EOF/atomic checks with UVAN.

Plaza activates hae; harbor activates nami mesh+material and nami_02/hiki_nami
material tracks. Mixed clips must explicitly enable both channels. Bindings
absent from visible geometry and unsupported texgens are recorded as
limitations. Animated collision, event-dependent boat clips, dynamic node
visibility and moving-wall cutaway bounds are not implemented.

Walking validates collision at both the old and sampled destination elevation.
This prevents a floor change from leaving an actor inside a wall that did not
overlap its previous height. Original entrance fixtures cover harbor retreat.

Stair movement uses a shared provisional StepHeight0.25 for ground ascent and
wall foot clearance; the earlier0.1 wall clearance blocked original harbor
0.1 risers through the projected actor footprint. Mario and partner validate
collision at sampled destination elevation before accepting a floor change.
Original solid-dock stair descent/ascent is regression-tested. Source collision
node attributes and original hazard recovery are not yet in runtime packs.

Tagged ground format: high bit of triangle count selects records of9 float32
coordinates followed by uint32 original attributes and uint32 source-node
offset (relative to map's data base32). Old36-byte triangle records still load
with zero metadata. Import validates counts, finite geometry and exact EOF,
replacing geometry/index/attributes together only on success. GroundContact
selects the highest supported triangle's attribute and node alongside height;
queries leave all outputs untouched on a miss. Room snapshots own metadata with
geometry, so failed staging restores both. Game carries Mario and partner
contacts through movement, restore and warp; saves reconstruct them from room
geometry instead of persisting stale node IDs.

These are source attributes, not all effective event-modified attributes. Wall
attributes, dynamic hitObjAttrOn/Off changes, special surface behavior, and
water/fall/recovery state remain unfinished. No guessed hazard action is attached
to the new contact fields.

Collision hierarchy metadata now retains every original hit owner, parent ID,
part count and transformed original hit center, including hierarchy-only nodes.
Return resolution averages leaf centers for the original sentinel, applies
ordered group overrides to descendants only, and converts explicit engine-unit
coordinates by model scale/10. RPOS v1 runtime files contain magic/version/count
and source-node uint32 plus3 float32 coordinates per binding. The5 current rooms
require files (empty where no verified init call applies). Import validates
finite/ranged positions, unique nonzero node IDs, version/count and exact EOF
atomically. Collision staging owns the return map with geometry/index data.

GroundContact reports raw source flags plus verified init return-enabled bit
0x40000000 when bound; groundReturn queries the active-room authored position
without changing output on a miss. Saves reconstruct these bindings through
room load. This provides return inputs, not the water-damage state machine:
saved-hit selection, animation, HP penalty/recovery and original effects remain
unfinished. Init centers are baked; dynamic collision/event mutations need a
subsequent runtime hierarchy implementation.
