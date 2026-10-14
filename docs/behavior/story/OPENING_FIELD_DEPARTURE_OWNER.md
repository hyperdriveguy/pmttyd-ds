# Opening arrival parent and detached field departure

The used original harbor parent publishes story3 after the ship departure has
run180 simulation ticks (three seconds). The detached ship child continues to
480 ticks (eight seconds). See [original ship schedule](NATIVE_OPENING_HARBOR_SHIP.md)
and its pinned REL request/body evidence. The native parent now ends at180;
ordinary field simulation/input begins immediately, while Game owns the copied
child state and its original source-clock anchor. This replaces the former
extra five-second frontend hold.

`OpeningFieldDeparture::begin` requires actual story publication, completed
paper presentation and an unfinished departure. It preserves the original
harbor scene, including child wheel/rudder/chimney/screw clocks, root rocking,
fixed wake and departure position. The native field tick maps its own monotonic
simulation tick to the retained source tick. `Game::tick` advances this owner
before field input/control returns, so dialogue/control locks do not freeze the
ship. At original tick480 the owner retires; leaving room1 also retires it as a
bounded native map-unload adapter. Render frame rates cannot change the clock.

The native renderer waits for GPU frame completion, compacts only original
ship/wake groups1/13, remaps original vertex IDs to dense local IDs, retains
original cached world/local/inverse transforms, and transfers only used texture
handles. Whole Rogueport geometry/actors, capture targets and unrelated textures
are freed with the frontend before loading the field. No mutable converter
output is hardlinked and no geometry is substituted. Source material/mesh clip
phase is rebased to the field clock; original vertex colors/UV/material flags
remain. The field's actual view/projection/world transform draws this small
resource owner alongside ordinary Mario control. No queued pointer survives
resource transfer; retiring/replacing/shutdown resources also frame-sync first.

Native resource preparation and field restore must both succeed before the
frontend publishes its successful handoff or requests the checkpoint save.
The original title/new-file scene/message order and source IDs are unchanged.
General detached-event SDK behavior, map-unload timing beyond this used branch,
full native field-camera/control parity and hardware raster parity remain open.

Host tests cover the actual +179/+180 parent boundary, continuous copied clock,
sparse/dense tick agreement, movable Mario while the ship remains active,
independent child progress during a field dialogue input lock, +479/+480
retirement and room unload. Actual original ship binding compaction preserves
all mapped vertex positions exactly and rejects incomplete remaps. The isolated
fresh input fixture records field release, live Circle Pad Left with the boat,
continuing child, and final retirement; screenshots are actual evidence, not
pixel-parity assertions.
