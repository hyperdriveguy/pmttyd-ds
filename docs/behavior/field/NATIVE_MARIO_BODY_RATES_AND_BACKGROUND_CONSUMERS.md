# Native ordinary Mario body and background consumers

Source: [constructors and consumers](MARIO_BACKGROUND_PREPARATION_CONSTRUCTORS_AND_LIVE_CONSUMERS.md),
[preparation callbacks](NATIVE_MARIO_KEY_AND_BACKGROUND_PREPARATION.md) and
[speed leaves](../overworld/OVERWORLD_SPEED_FALL_LANDING_LEAVES.md).

FieldMarioBody owns source-unit width/height, raw walk/dash bases and form for
Game's supported ordinary character. Its defaults form0,width20,height37,
walk1,dash2.25 are original MarioInit stores, not guessed checkpoint values.
Multiplier1 comes from marioInit's marioResetGravity8009696c call at8005b624:
that helper loads source float1 at80420a74 and writes fresh Mario+228 at80096988.
This is a bounded native constructor adaptation, not all original MarioInit.
Native room restore still rebuilds the supported normal body through the existing
Game adapter; transformed form/ReInit/HitWidth transients are not claimed.

Game's preparation world now admits key callbacks plus background on/off and
camera-Y-off only for known form0,unit multiplier and no small04000000 or paper
00100000 flags. Unsupported domains hold before any source mutation. Source
main02000000 remains in the existing fieldMarioBattle owner. Dimension/base-speed
reads/writes map to FieldMarioBody, while form+3c reads that same native owner.
No extra float fields masquerade as rendering scale.

FieldMovement accepts explicit ground rates with its previous ordinary defaults
retained for standalone callers. Game supplies live rates for both float and
physical-stick movement and jump launch; it no longer classifies motion by exact
equality with a fixed DashSpeed. Three consumers deliberately use different
source quantities:

| Consumer | Native input |
| --- | --- |
| Ground walk/dash | `(raw base * background-half) * multiplier`, then native .01 conversion |
| Ceiling and move-motion threshold | raw walkBase, converted once; never background-half |
| Airborne combined-speed cap | raw dashBase * multiplier, converted once; never background-half |

Motion classification uses source strict travel>raw-walk comparison. In ordinary
background mode .5 walk remains walk and1.125 dash remains dash against raw1.
Adding drift to background takeoff is allowed above1.125 until raw2.25 cap; using
effective ground dash as that cap would incorrectly erase source air steering.
Special paper/scale/badge/character branches remain outside native admission.

Game wall actors, NPC talk dimensions, NPC sphere response dimensions and head
queries read the retained body width/height. Head APIs accept explicit dimensions
while legacy callers keep the ordinary defaults. Bounds, clearance and root
correction use live height; the five-ray radius derives from width. The source
contact-height quantization bias remains unchanged. Native broadphase expansion
and fallback rectangles remain calibrated for ordinary width20; generic changed
form collision is not claimed. NPC response collisionWidth is distinct source
+1c0 and retains its existing native20 policy; it is never aliased to width+1b8.
Source HitWidth/ReInit transient ownership remains work for a later binding.

NPC talk uses actual background mode and the existing shared key counter, with
the retained native hand-in adapter fallback when no original key counter is held.
This does not establish the missing full source hand-in/key lifecycle. Original
NPC side collision has no extra Mario background flag gate in its inspected
wrapper/caller; its own NPC gates and existing narrowphase remain in force.

Camera U16+0 maps to the existing fieldCameraFlags consumed by camera follow.
Stores preserve its native upper bits. Mario display+4's callback-specific view
maps to fieldCameraDisplayFlags, the actual MarioCameraTarget consumer of20000.
Other render/pose/fieldMarioBattle display domains are not replaced by a duplicate
full-word shadow. No view/target timer,shift clock,road reset,pose or GPU change
is performed by background preparation.

Tests verify actual Game on/off flag/rate changes, preserved unrelated camera
bits and timelines, background movement displacement, raw ceiling threshold,
unhalved air steering cap, source reset of modified bases, actual triangle head
contact with different heights, unsupported-state refusal and original parent/
attached-child progression to previous-party800e3058 using real Game owners.
The native prefix probe also seeds a genuine background-on request then lets the
unchanged original parent turn it off, retaining its real nested key hold while
the next party service is pending. Its GP/story image remains an explicit fixture.

Next bind actual previous-party allocation/art/setup, then the original attached
initial_pos length/geometry/actor providers in this shared scheduler. Live area
loading still needs real GP/checkpoint/NPC/world lifetime and coordination with
Game's separate Bero/Trouble runtimes. Original raster parity, full source init/
ReInit and New2DSXL hardware timing remain unproven.
