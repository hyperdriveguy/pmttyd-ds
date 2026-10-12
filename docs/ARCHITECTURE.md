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

Game.returnContact remembers the eligible floor before native horizontal
movement when return-enabled0x40000000 is set; unbound/water contacts leave it
unchanged. recoveryReturn queries that source ID in the active room. restore
resets map-local remembered contact, including across warp/load, avoiding stale
IDs in another room. This latch does not implement the damage state machine.
Floor sampling now excludes original bit4 triggers unless inclusion mask
0x00080080 or0x00800000 applies, matching current normal-Mario ground-filter
coverage. Roll/Vivian and name-dependent high-bit cases remain open; normal
wall filtering is now implemented as documented below.

WaterRecovery is portable fixed-tick motion using original mot_damage state
numbers/parameters and native engine-to-world0.01. Game starts it on grounded
water attribute0x200, resolving saved eligible owner or ordinary original map
fallback, then supporting floor height. During recovery, input/menus/warps,
camera and partner movement are suspended; field map animation clock continues.
Rendering uses recovery body Y. Transient positions cannot save, including START
exit. Landing applies one HP loss after the source hold counter; zero HP enters
FieldDefeat, preserving the last checkpoint. Load-save request stages collision,
returns and rendering before restoring progress, and never supplies artificial HP.

Presentation is incomplete: original water/character effects, SFX/game-over
music/art, NPC event behavior and observed timing remain unverified. Landing
uses the current static floor provider at the authored point; original full ray
queries/dynamic collision are still needed. Float scaling can shift exact source
threshold boundaries; compare complete motion in Dolphin before acceptance.

Road transition fade: portable TransitionFade uses original default300ms at
NTSC60Hz (18 ticks), black linear out/in. Simulation freezes input, camera and
follower updates during fade; queued airborne crossings first finish the current
prototype jump, matching the source road-event landing gate. Main waits until
an opaque frame has been submitted before staging collision/returns/rendering.
Commit starts destination fade-in; failure rolls back room resources and fades
the retained scene back in. Both handheld screens receive a final C2D overlay.
START exit during queued travel, fade or arrival preserves the existing checkpoint.
Arrival autosave waits until the validated destination endpoint is reached.

Portable RoadMotion implements the source ordinary-road150-units/second rate
(0.025 native/tick), integer tick count and final XZ snap. Departure walks to
1.5 native outside the marker, waits9 ticks, then fades out. Entry walks from
0.3 outside to0.3 inside during fade-in. finishWarp validates support/collision
and movement before restoring destination state. Failed staging restores the
recorded departure position while CollisionTransition restores old room data.
Original party choreography, sensor volumes, dynamic floor physics and camera
framing still require source/observation work.

Wall packs now use count bits31/30 for height/owner metadata respectively.
New records are32 bytes: six float32 values (XZ segment endpoints,minY,maxY)
and two uint32 values (original flags,relative source node). Legacy16-byte and
24-byte records still load with zero attributes. Deduplication retains distinct
owners of overlapping geometry. WallContacts participates in clear, successful
load replacement and CollisionTransition snapshot/rollback. Query/minimap
wallAtLevel applies the ordinary-Mario bit4 exclusion/inclusion overrides before
height tests. Malformed/truncated metadata preserves the previous valid pack.

Normal-Mario collision calibration supersedes prior0.28 radius/1.2 body-height
wall tests. BodyRadius=0.1; wallAtLevel tests the source two sample heights0.11
and0.2664 separately instead of treating the whole vertical interval as solid.
The spatial index expands segment bounds by BodyRadius. Mario movement uses
support height plus current jumpHeight for wall tests while querying floors
separately. Floor step allowance remains0.25. Segment distance/interval geometry
is still a proxy for original swept rays and triangles; transformed-Mario and
partner-specific collision remain outstanding.

Wall triangle geometry supersedes the interval/longest-segment collision proxy.
Count bit29 now indicates9 additional float32 original transformed triangle
coordinates after each32-byte record (68 bytes total; flags0xe0000000).
Legacy formats still load. New queries intersect each retained triangle with
both normal-Mario height planes, then test clearance against the resulting XZ
segment or vertex. Spatial bounds use all three vertices. Height intervals are
validated against triangle extrema and used only for early rejection. Actual
triangle arrays participate in collision rollback/clear. Minimap still draws the
summary segment. Original swept direction rays and contact sliding are deferred.

Ordinary movement now uses source WalkSpeed0.01 and DashSpeed0.0225 per fixed
60Hz tick, replacing0.065. Circle Pad magnitude chooses the source walk/dash
threshold55/70; direction is normalized independently. Follower path budget now
uses DashSpeed (partner-specific catch-up logic remains provisional). Scripted
road rate remains0.025/tick, with original entry/exit distances unchanged.
Traversal fixtures preserve established map distances by extending directional
holds after the speed correction. These fixture-time conversions are not game
rules. Original motion transition delay and airborne drift remain unfinished.

FieldJump replaces the fixed36-tick sine-wave field jump. Portable derivative
integration retains source ascent/fall tuples and A-release reduction. Game
uses airborne state for repeat-jump prevention, water entry and road landing
waits; restoration, battle start and water recovery clear the jump state.
Both ordinary movement and queued-road waits advance the same integrator.
Rendering continues to consume jumpHeight. Height remains relative to currently
queried support and clamps at zero; original world-Y floor/ceiling resolution
is the next required physics step. Battle animation remains a separate harness.

Floor queries now share groundWithin(lowY,highY). Existing groundAt preserves its
legacy support band and no-pack flat-ground behavior. New landingGround provides
only the source world-Y center probe using abs(vertical velocity) and native
0.05/0.0001 ray margins. Failed queries leave height/contact outputs intact.
The primitive uses existing indexed triangles and original attribute/return
metadata. It is not yet called by Game: world-Y state, five-position footprint
sampling and contact acceptance must be integrated together to avoid treating a
raw intersection as a landing. Current relative-support jump is still provisional.

World-height integration supersedes relative-support-only jumping. Game retains
jumpBase and unbounded FieldJump displacement, so groundHeight+jumpHeight renders
actual world Y through jumps/falls. FieldJump's default base clamp remains for
standalone tests; Game disables it and resolves landing through landingFootprint.
Footprint caches no contacts:5 indexed center/rotated ray queries select the
highest eligible generic candidate. Landing resets derivatives and updates support,
progress elevation and source contact. Airborne movement checks walls at actual Y
and no longer requires a supporting triangle at every XZ step. Loss of grounded
support starts beginFall; queued roads advance the same landing path.

Menu saves clear while airborne and START preserves the checkpoint; jumping and
saving in the same tick also cannot persist transient position. Partner still uses
legacy floor stepping; camera/shadow anchor is the last support until landing.
Original ceiling search, special contact/slope branches, wall depenetration and
no-floor void recovery remain required. Generated native listing is ignored.
