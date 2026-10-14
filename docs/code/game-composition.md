# Game composition, room geometry and field ownership

[game.cpp](../../source/runtime/game.cpp) composes portable mechanics and native
adapters; [game.hpp](../../include/runtime/game.hpp) declares their retained
state. GPU, NDSP and SD work stays in the [native runtime](runtime.md). Original
contracts remain indexed under [behavior](../behavior/README.md). A recovered
helper does not establish complete original scheduling or native integration.

## Units and identifiers

| Value / owner | Representation |
| --- | --- |
| `Progress.position`, ground/jump heights, room triangles, follower trail | Native world units: .01 original engine units |
| Source NPC/event actor roots, wall services and `fieldCamera.point` | Original engine units; multiply native coordinates by100 at boundaries |
| `fieldCameraFrame.eye/at`, projected native scene | Native world units; source lens near/far adapt by the same scale |
| Camera/NPC yaw, source movement headings | Degrees; host sin/cos use radians |
| `ticks` | Simulation visits, converted to integer milliseconds with `ticks*1000/60` |
| `fieldCameraSimulationTicks` | Separate process clock retained across restore |
| Static collision `sourceNode` | Converted original map-node identity, room-local |
| Dynamic `objectKey` | Game-owned object namespace, separate from map-node IDs |
| Ray `token` | Zero for miss; static index+1, then dynamic indices after static range |
| Native room IDs | 0 plaza,1 harbor,2 east,3 west,4 station,5 underground |

A `CameraPoint` or vector type alone does not specify the scale: inspect its
owner and provider boundary. Angular constants57.295779513 and .01745329238474369
approximate180/pi and pi/180 respectively. Some recovered routines intentionally
use distinct binary32 constants and FMA ordering. Do not consolidate them into
a supposedly cleaner expression without proving original numeric behavior.

`UINT32_MAX`, `UINT64_MAX` and `~0u` mark invalid cache identities/epochs; room0
is a valid room and cannot substitute for them. Zero static node IDs mean no
source owner. Shape, map node, actor key, original address and native slot are
different namespaces even when all are integers.

## Collision pack and query numbers

Collision geometry is process-global. A candidate Game queries the room staged
by `CollisionTransition`, not a private copy. Snapshot swaps carry geometry,
height spans, contacts, head geometry, spatial indexes, return mappings, road
attribute overlays and room bounds together. Door masks are separately derived
from active Game state. An uncommitted transaction restores the saved geometry
cache; commit follows destination validation and renderer admission.

These private converter packs use native little-endian fields and finite
geometry. Import bounds are port policy, not original room limits.

| Pack | Layout / discriminator |
| --- | --- |
| `.walls` | Count low29 bits; high bits0x80000000 heights,0x40000000 contacts,0x20000000 triangles. Contacts require heights; triangles require contacts. |
| `.ground` | Count low30 bits;0x80000000 contacts,0x40000000 cull. Legacy XYZ geometry36 bytes, tagged44, faced48. Cull requires contacts. |
| `.head` | `HEAD` plus count, then48-byte position/contact/cull prefixes. Native cached bounds follow in memory only. |
| `.returns` | `RPOS`,version1,count (12-byte header), then16-byte nonzero unique node/XYZ rows. |
| Roads | `ROAD`,version1,count (12-byte header), then48-byte records with reciprocal source/arrival markers. |

Geometry caps100000 records, roads128, grid references2,000,000 and coordinates
within +/-4096 are native admission limits. Broad-phase cells span2 world units.
Grid keys retain signed cell coordinates as unsigned32 high/low halves, with
flooring for negative cells. Walls index bounds expanded by BodyRadius; exact
segment/triangle tests decide collision afterwards. Multi-cell rays sort and
deduplicate indices so repeated cell visits do not change equal-depth ordering.
Threshold1e-8 rejects degenerate projected areas/segments;1e-6 rejects unusable
headings. Legacy unconverted fixtures retain placeholder rectangles/bounds.
They are separate from original Rogueport geometry.

| Attribute / mask | Current owner / significance |
| --- | --- |
| 4 | Trigger filtering: ordinary solid-query paths may exclude it |
| 0x00080080 | Original special inclusion overrides (do not flatten to one flag) |
| 0x00800000 | Inclusion/pass-through override; enabled road nodes get a runtime overlay |
| 0x200 | Water contact; ordinary partner support excludes it |
| 0x40000000 | Return-enabled ground overlay when a source return binding exists |
| 0x80000000 | Source named/dynamic owner requiring additional admission service |

Filtering differs between ordinary Mario, head probes, partners and unfiltered
appearance rays. A nearer unresolved named owner cannot become a miss that
reveals a farther floor. Imported raw attributes and runtime overrides stay
distinct. Static source-node metadata is not a dynamic object key.

Ground queries choose the highest hit in `(low,high]` before rejecting steep
support. Rejecting that floor does not retry a lower triangle. Normal/tilt
stores may precede the50-degree support-cone test; prior inclination is restored
on rejection. Ground barycentric edge tolerance is +/-1e-4; head barycentric
checks use exact0..1. Five probes are center/forward/left/right/back. Head
queries return at the first admitted probe even for a near miss; floor support
keeps the highest reachable admitted probe.

Q1000 quantization acts in original engine units, then converts back to world
scale. `.00001` combines division1000 and conversion.01. Head response uses
separate truncation biases and a one-engine-unit root clearance, explained in
[head_collision.hpp](../../include/runtime/head_collision.hpp). A floor miss
may retain a drop `delta`, and `objectKey` clears at query entry; do not promise
all outputs remain untouched. `roadStartGround` preserves marker Y on a miss
and has a broad native10000-world-unit downward reach, distinct from ordinary
support depth.

## Tick order and early-return ownership

`Game::tick` installs a scope-exit owner named `PresentationOnExit`. It is
simulation publication work, despite its name. Every return executes that tail.
Moving it into only ordinary exploration changes interactions and animation.

| Phase | Current composition |
| --- | --- |
| Entry | Apply fence/door collision state, capture prior follower position for presentation |
| Clock/camera | Advance ticks and opening departure; camera consumes the prior Mario publication |
| Early events | Acquisition/reward event owners can consume retained input before new PAD publication |
| Input/NPC events | Publish physical PAD sample; wander event providers use previous pose/contact state |
| Save events | Configure/tick save events and synchronize saved notice/mode switches |
| Priority branches | Save lock, arrival/fade/pipe/road, defeat/recovery, door, selection/talk/item quests, trio/story/tutorial, legacy battle, ordinary field |
| Scope-exit tail | Save objects, temporary professor, NPC collision, Mario idle/presentation, candidate/talk/pose publication, harbor presentation, itemMain, road cases, follower presentation |

This is the implemented native chronology, not certification of the complete
original outer loop. Neutral `move(Input{})` during key/control locks still
advances support and airborne mechanics. Fade and explicit transition owners
can return without ordinary movement. A universal freeze for every dialogue
or lock would be incorrect. `ticks` advances even when a priority branch returns.

Normal ascent composes jump integration, overhead query, horizontal movement,
projection and landing. Falling moves/projects before vertical integration.
A just-launched call skips a second vertical update. Retained travel, current
stick drift, wall probe direction and paper display facing are distinct state.
Ground motion classes0/1/2 mean stay/walk/dash. NPC side response follows Mario
movement; wander then consumes its retained previous publication. Prior talk
candidate suppresses A-jump before this invocation scans new candidates. Talk
confirmation records pending work; the next NPC invocation starts the event.
Catalog generation guards prevent stale keys after resource replacement.

## Persistent versus reconstructed owners

`restore` copies supplied Progress before rebuilding the Game because callers
may pass its own `progress`. Reset work lives on the heap to avoid the native
stack limit. Session palette, cameras/shadows, RNG/SFX owners, Mario pose handles
and presentation facing are explicitly retained. Field camera clock and view
history survive separately from the reset Game tick. Per-room objects, NPC
instances, contacts, follower trail, transient dialogue/battle/air state and save
presentation are reconstructed. The original pouch is shared live storage;
queued save snapshots deep-copy it as described in [save format](save-format.md).

Original room/story gates select native adapters: harbor story3 begins arrival
story,4 resumes encounter; east8/9/10/11 selects professor/door/fence/lecture
paths; underground setup4 admits trio while harbor setup2 admits tutorial.
These gates do not infer story from the old harness completion boolean. Item95
and107 talk branches require explicit quest2/13 and known saved guard switches.
Original message IDs/control tokens stay in their separate story owners.

## Remaining native adaptations

Legacy Crump harness selection0/1/2 is Jump/Hammer/Mushroom. Fixed impact48/54,
postimpact42/36, fixture HP10 and mushroom heal5 remain harness inputs. Harness
request traces record effect/sound/notice/hit-event calls; they are not executed
or automatically drained queues. The actual harbor tutorial and trio use their
own services and must not borrow legacy victory or defeat completion.

Room fades use18 simulation ticks for300ms at60Hz; the opaque phase must be
presented once before blocking room replacement. Road choreography's11-tick
wait is a native bridge for150ms plus2 original waits, not measured event
scheduling. Arrival offsets.3 and outgoing target1.5 world units correspond
to30/150 engine units. Generic road callbacks can hold when an original scene
owner is unavailable. Native bottom-screen tab/button hitboxes and billboard
turn parameters are documented beside their uses, not original art constants.

[PartnerFollow](../../include/runtime/partner_follow.hpp) retains source easing,
width/height, jump/fall, projection recovery and support terms while composing
native swept terrain services. Its vector capacity512, substep.025, path lookahead
320 steps and blocked-path repair after8 visits are explicit native policy;
the original ring capacity80 has a separate contract. Jump marks stop shortcut
repair. Counter/easing work is in engine units, scaled at boundaries; close/far
thresholds45/50 are speed-control thresholds, not a required resting gap.
[Native follow scope](../behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md).
