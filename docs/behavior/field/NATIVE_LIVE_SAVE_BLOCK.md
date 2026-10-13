# Native live source save-block binding

2026-10-13; early Rogueport slice. Implements the bounded source domains in
[head/callback lifecycle](SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md),
[original event core](NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md),
[retained windows](NATIVE_SAVE_BLOCK_RETAINED_PRESENTATION.md) and
[animation clocks](../display/SAVE_BLOCK_RESTART_CLOCK_AND_EVENT_IDENTITIES.md).
Original execution/hardware timing and complete field ownership remain unproven.

## Instances, update order and control ownership

FieldSaveObjects creates room-owned instances by original model identity
MOBJ_SaveBlock, independent of room number. Catalog source key/name, placement,
visibility predicate and initial flags0x400085 are retained. S_1/A_1 metadata is
loaded/validated outside simulation; each instance has its own restart timestamp,
raw animation X and published ratio. Objects share resident model GPU resources.
Future source save-block catalog rows in other rooms use this path without a
room-specific branch. Other object constructors still need their own handlers.

After physical input publication, the save event/window adapter runs before Mario.
Mario's actual head-hit owner is reset each tick and published through the shared
collision service; proximity alone cannot trigger. After Mario, new HOBJ_sourceName
contact publication feeds the original head producer and one callback invocation.
No contact clears the owned name cache. Static head names use a provisional node
identity adapter because the collision pack lacks all original names. Animation
Main publishes after callback, so stage1 reads the previous ratio. Native elapsed
clock is simulation milliseconds converted separately to60*elapsed/1000, not a
render frame counter. Raw X reaches the sampler; S_1 clamps its initial X0 to
source start1, and A_1 clamps geometry at15 while ratio may continue beyond1.

Stage0 requests original431, restarts A_1, owns the outer key leaf and system1,
and records camera8 bit200 separately from native field camera4. Stage1 restores
S_1 after prior ratio>=1. Stage3 enters source system event8030b4ec at priority30/
type26; it first runs next simulation invocation. The source nested key leaves
use the same stable object-owned service. Stage4 releases outer key/system/camera
after event completion. Mario continues neutral-input falling/landing while the
save owns controls. A/B/directional/repeat input still reaches original windows.
No jump/menu/door/pipe request is admitted during that ownership. Failed native
providers release adapter ownership and publish a development notice; this is
native recovery, not fabricated original event behavior.

Camera8's [automatic letterbox decision lock](../camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md)
is recorded; full letterbox decision/fade/draw services remain unbound. Field
camera4 does not receive bit200 or a guessed follow freeze. Optional custom object
events, original field sequence/category/pause processing, full manager lifetime
and shared VM state across other field events remain outside this binding.

## Original rendering and native services

Save renderer cache includes per-instance raw time as well as model/pose/tick.
It keeps original PMANIM4 vertex colors/texture states/material passes, existing
source bbox collision and citro3d batching. Rendering never determines callback
completion. Window dynamic-effect caches are separate per retained serial and
pruned against live field windows rather than reset when auxiliary choices draw.

System type3 uses original msgWindow.tpl pattern5, shape masks42..45 and gray ring
masks46..49. Four quads split at width-32/height-32, shape/ring clamp while pattern
repeats with original+.005-per-draw phase/wrap10. A dedicated PICA vertex shader
publishes pattern and mask coordinates; two texture stages compute ring/pattern
RGB interpolation and pattern*shape*window opacity. Source GX/PICA fixed-point
rounding/pixel parity is unmeasured. Opening sizes below32 use a bounded minimum
32 adapter; the original narrow-quad branches need further source integration.
Normal box/frame data and textures are source-backed. System text scissor starts
at240-(y-36)-8 and uses height-50, scaled to handheld coordinates. Type3 has no
speech tail. Selection uses original index0 as nine slices,24/48 source breaks
and30-unit destination caps; source cursor icon1f8 and independent window opacity
remain. Selection has no extra message scissor. See source
[drawing](../message/MESSAGE_WINDOW_DRAWING.md) and
[selection drawing](../message/MESSAGE_SELECTION_DRAWING.md).

The original [cue431](../audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md) now has a
resident native one-shot: original bank FX618/macro745 primary sample550, decoded
unchanged to PCM16/22050Hz/24422 frames, scheduled once on NDSP channel7 at stage0.
It uses original table volume100 with the existing conservative native bus gain.
MusyX macro/pitch/envelope and original3D pan/attenuation/group residency are
adapters, not proven original audible equality. Selection sound IDs are retained
but their native playback remains unbound; no generated cue replaces them.

Native main consumes one-use write tickets outside fixed simulation. A libctru
worker uses its owned checkpoint/path,16KiB stack, priority31 and default permitted
core. Release/acquire completion publication precedes next source event polling.
Accepted writes finish before shutdown. Existing atomic40-byte SD/temp/backup
serialization stores source flags233/236 and mode4 from the request snapshot.
No starts no worker. Harness writes are suppressed while source save owns controls;
exit autosave remains only for legacy harness state without originalStory. Other
harness auto-save triggers elsewhere remain adapters. Header and generic SD failure
classification retain the limits documented by the portable provider.

## Verification and remaining work

Original-data Game tests hit the actual block through jump physics, wait A_1,
run original first-hit pages/choices, exercise delayed Yes/No I/O, preserve source
nested keys/camera separation/No unchanged files, restore S_1 and reach exploration
after landing. Direct instance tests pin previous ratio publication at250ms and
forced idle restart. Host test clocks are adapter inputs, not measured source time.
Isolated Azahar captures check original repeat-hit System/selection/No/Yes and
actual checkpoint/backup bytes; exact paths/results are in [progress](../../PROGRESS.md).
Emulator performance is not New2DS XL hardware performance. Remaining: original
runtime comparison, narrow System frame cases, shared category/pause/VM/resource
services, original spatial/macro sound, other constructors/areas and hardware
profiling. The overall remake goal is not complete.
