# Trio runtime provider ownership

This guide covers selected sections of [TrioAttack](../../include/battle/trio_attack.hpp)
and [its runtime](../../source/battle/trio_attack.cpp): process assets, HP-provider
binding, pose/shadow registration, stage and audience composition. The [event/entry/damage](trio-events.md) and [ending](trio-ending.md) sections
have a separate audit. See [scheduling](trio-scheduling.md) and [display prefix](trio-display-prefix.md)
for the next runtime sections. [Public API](trio-api.md) and [draw bindings](trio-draw-bindings.md) cover the
recorded facade; newer source continuations still need complete review. See [commands](trio-commands.md),
[audience control](battle-audience.md), [audience sound](battle-audience-sound.md),
[part/HP presentation](battle-part-presentation.md), [stage](battle-stage.md) and
[retained owners](battle-retained-owners.md) for linked mechanics.

## Process resources and native storage

The loader validates original EVT signature, three reaction-unit rows, original
angle data, usable field Mario poses and original Mario/Goombella/Frankly pose
resources. Stage metadata supplies one source stage and seven descriptors;
audience config requires a null extra-setup pointer at `+0x1c`. Native metadata
uses little-endian words; guest EVT memory remains big-endian.

Loading updates some process globals before final success. It is not a rollback
transaction: failed reload cannot be assumed to restore the previous complete
catalog. Existing runtimes pin their own file/pose owners. Partner animation uses
a weak cache that can reload from its recorded path. The file-cache owner is
declared before the VM, so borrowed external VM views retire before that backing
owner under reverse C++ member destruction.

Disjoint native guest arenas use `0x84000000..0x84400000` for Battle, Units,
Parts, Kinds and Work. These tokens are not host pointers or original allocation
addresses. All packed offsets remain source layouts. Required VM reads/writes
abort on failed admission rather than return fake zero; floats travel by four-byte
bit copy. Native milliseconds and the source 243MHz timing convention are separate.
Host libm/FMA boundaries do not establish Gekko/FPSCR parity; cache-flush/system-call
hooks here are scoped no-ops.

Shared RNG/pose owners precede their borrowed references. Display cursors retain
pending operations independently of sample/publication clocks. Destruction retires
HP frame/camera before dropping its allocation token, balances an acquired menu
resource reference, publishes spotlight work and releases only pose handles whose
recorded native generation still matches.

## HP source graph and actual renderer

Allocation identity is a native token, separate from guest Battle address and
camera publication pass. Admission also checks real guest span `0x19100` and
source global `0x8041eb90==Battle`. A live token alone is insufficient. The
native draw entry pins its runtime across external callbacks, rejects reentry
and requires the currently invoking source HP callback `0x8011fde0` on camera 8
with Battle userdata and a nonzero camera-Main publication.

A held frame binds exact provider lifetime/user, begin/publish hooks, icon/raster
callbacks, borrowed GX-table address and copied GX members. Mutating a table in
place does not count as the same provider. The synthetic camera view at native
address `0x60000000` must be proved unmapped across actual VM/file-cache views;
it is an admitted read view, not copied original camera RAM.

Camera-owned word reads take precedence; other icon reads try external providers
then actual VM memory. Successful external results recheck camera availability.
The actor-memory view redirects only its read context and removes write/flush/
system hooks, preventing callbacks from receiving an incompatible user pointer.
Source camera getters route through the bridge; other GX calls/quads use the
retained actual renderer. Source status and saved-switch queries remain real.

For each increasing pass: admit camera epoch, acknowledge external begin once,
resume retained source HP frame, acknowledge publish and finish the current
camera epoch. Missing results or retired ownership remain unavailable. Publish
can retry after the source frame completed; external providers own any partial
begin/publish effects. Completion of an earlier pass is not a fresh display proof.

## Pose/shadow and stage services

Pose registration freezes computed camera depth across queue holds. Missing
snapshot/depth remains pending; no default priority is guessed. The actual part
callback is queued before its shadow/sound suffix. Unsupported part attributes
or special floor query suspend at their source function token.

Ordinary shadow projection captures world Y before replacing Y with floor plus
source offset. It preserves ordered factor/radius arithmetic using original
constant words and signed part radius, then requires shared `ShadowWork` admission
before sound-tail completion. Source automatic-pose sound work with a valid pose
handle requires the actual sound provider; recording a request does not play it.

Stage setup admits source rank 0, actual background/foreground hierarchy and hit
data, and primary-bank material/fog state. Name lookup prefers foreground when
both banks contain a group. Seven source stage-object rows retain original cache,
transform and visibility fields; row 5 imports the source special Y. Required
setup script fields must actually be null. Retained object/work cursors keep
acknowledged transforms/counters across unavailable map/spotlight work. Active
stage-entry provider remains unbound. Signed32 float truncation explicitly
requires a finite in-range value; arbitrary casts do not complete missing work.

## Audience service composition

Population uses shared bounded and raw RNG, actual original resource handles,
source arithmetic order and a narrowly admitted zero-target path. Break-slot
storage belongs to the shared source map arena; zero allocation is unavailable.
Animation scripts process once per admitted millisecond with retained child state;
a held visit is not marked complete by publishing its timestamp.

Audience Main dispatches original actor/item/AP/window/sound function domains.
Idle math admits only operands exactly representable as singles for its selected
host arithmetic paths; arbitrary inherited FPR29 inputs remain unavailable.
Item sounds require actual public handles and throw-lane ownership. Sound check
optional zero is a known false/stopped result; absent optional is unavailable.
Only inactive AP-source work is admitted by its adapter.

This audience parent is immediate. Its children being retained does not establish
safe restart of the whole parent after partial effects. Missing providers retain
diagnostics; this documentation does not add continuation behavior or claim that
the whole original Main integration is complete.
