# Harbor dialogue and encounter controllers

[HarborStory](../../source/story/harbor_story.cpp) is a bounded native adapter
for the crowd scene; [HarborEncounter](../../source/story/harbor_encounter.cpp)
runs the unchanged before-battle event in the native interpreter. Their
[public crowd state](../../include/story/harbor_story.hpp) and
[encounter state](../../include/story/harbor_encounter.hpp) expose different
ownership contracts. Neither completion means the full opening or battle is done.
See the [source bindings](../behavior/story/STORY_HARBOR_OPENING_BINDINGS.md),
[before-battle scope](../behavior/story/NATIVE_HARBOR_BEFORE_BATTLE.md) and
[encoded jump admission](../behavior/story/NATIVE_HARBOR_ENCOUNTER_EVENT_FLOAT_ADMISSION.md).

The crowd adapter validates original `mac_0_016` through `021` and retains the
speaker order Goombella, Crump, Goombella, Crump, X-Naut A, Goombella. Print
captures the actor root once. Stage numbers describe native sequencing, while
REL offsets identify speakers; neither is a host address. The source-derived
retreats use delayed requests per actor. A positive displacement subtracts from
X after the source Q1024-to-integer round trip. The existing linear speed bridge
uses engine units per second and milliseconds; its timing is provisional.

Every crowd tick advances actor movement before checking text, then visits just
one stage. A sparse tick cannot consume all waits at once. The final line waits
for Goombella's movement, with the followers independent. Pose presentation uses
`actualSpeed` supplied by the field owner, rather than assuming the movement
request speed is the observed displacement. Tribe run thresholds differ between
Goombella and the other actors. Talk requires original NPC bit `0x200` and the
matching speaker; camera-relative matrices retain the separate source yaw.

The encounter metadata maps source callback addresses to the ordered service
catalog in [the converter](../../tools/harbor_event_assets.py). Those small
indices are pack ABI values, not event opcodes. Original relocated script words,
branches and child events remain in the interpreter. First callback admission
records raw operands, including variable tags and guest pointer identities;
resolved values can change as execution continues. Unknown callbacks fail,
while unbound battle services retain the command without inventing a result.
Environment/reaction/SFX requests are accepted into the trace without a complete
backend. A camera request also does not acknowledge display completion.

Public encounter time is milliseconds. The platform reports the original
243 MHz clock rate, and GP timestamps use the event clock conversion. A private
1 MiB guest allocator and original SDK global addresses isolate this interpreter
from host pointers. GP+4 supplies 60 Hz, GSW0 starts at story4, and root LW0
selects fresh versus resumed execution. Root priority10 is scheduler setup.
The source event owns the spatial admission, HP changes and setup2 request.

Movement advances before text and VM dispatch. Native Mario X/Z interpolation
leaves Y with the jump/field support owner; NPC movement interpolates all axes.
The `(distance+1)/speed` duration and Mario run threshold are adapter policy.
The jump admits only its own exact Q1024 position round trip, supported roll
and height parameters; unsupported domains hold at the service boundary.
Bit2 suppresses automatic jump pose changes. Milliseconds convert to 60 Hz
simulation ticks, with support supplied by the native floor query.

`admitted` reflects nonzero current key-depth. `locked()` also checks source
control enable, and `finished` requires every VM event allocation to retire,
including children. A battle-start request remains running and pending; the
controller does not write a postbattle story value or supply a tutorial outcome.
