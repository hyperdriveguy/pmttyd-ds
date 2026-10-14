# Original area parent in the shared native owner

Source: [gor02 parent and map copy](GOR02_ORIGINAL_PARENT_PREFIX_CLOSURE_AND_MAP_COPY.md),
[Bero prefix providers](../overworld/BERO_INFO_PREFIX_ASYNC_GROUPS_SYSTEM_FLAGS_AND_ACTOR_PREPARATION.md)
and [door continuations](NATIVE_SHARED_FIELD_DOOR_INSTALLERS.md).

FieldAreaEvent now admits the complete original gor_02_init_evt through a separate
PMFAIN01 room/root manifest. appendAreaInit requires a pristine owner and explicit
FieldAreaGpState: system-flags word0 plus GP bytes40..13d7. This is caller-owned
state, not reconstructed checkpoint defaults. Scheduler/context/clock fields
below40 remain adapter-owned. The root uses the real cleared constructor bank,
priority0/flags0; its actual allocated ID is published to GP+164. Full-parent and
supplied-bank door continuation modes cannot be mixed in one owner.

The converter retains970 source REL data objects and61 DOL EVT objects (23436
body bytes), plus real adjacent scanner context. door-data.pmevt is100520 bytes;
PMFAIN01 is20 bytes. Existing PMDRDT01/PMFAEV02 remain36/60 bytes. The DOL selection
follows known EVT address references from imported original objects. It does not
establish every dynamic target, immutable constant, native callback, resource,
or mutable world dependency. Original PPC is not executed.

Actual original get_info reads LW0 without user operands. Each real call
registers that table again, resets SW/EXEC, and preserves currentNumber. The
standalone native initialize adapter retains its once-only policy. into_info
reloads live VM GP word0 and GP+11c, writes the actual inline GP string address,
and cannot use stale external Bero information as authoritative state. The
63-character incoming-name admission limit is native policy. set_now_number uses
its actual operand and publishes through the shared Bero owner.

Unknown calls hold. FieldAreaNativeCall exposes the current event, operand,
memory, scheduling and creation services to future exact providers; it accepts
only source callback results0/1/2. No broad success fallback exists. pendingEventId,
eventAlive and liveEventCount retain diagnostics for independent roots even after
the tracked area parent is gone. The optional native retirement observation
captures a bank after actual recursive child deletion/copy-back, before clearing;
returned distinguishes source RETURN from forced cancellation. See the
[event observation contract](../event/NATIVE_EVENT_RETIREMENT_OBSERVATION.md).
The full parent's late monosiri child can publish locals after its last direct
callback, so a callback-time snapshot cannot substitute for retirement.

Host tests execute all seven grouped story sound branches, ordinary23 and402,
firstCall holding, actual attached-child linkage, GP root ID, stale external
cache rejection, cancellation copy-back and independent-root diagnostics.
Sound trace providers and zeroed GP images are explicit synthetic test fixtures,
not verified Game/checkpoint state or native audio implementation.

The isolated native probe uses story402 to follow the real sound-skipping branch.
It executes the unchanged original parent and attached Bero child, then holds at
8007a154 with parent1/child2 and two live entries. Actual event deletion copies
child locals to the parent: LW5=6 and LW3=8100011c; the retirement observation
reports returned=false and zero live entries. This demonstrates prefix execution
and cancellation, not successful completion of the full map initializer.

The originally unbound callback8007a154 now has a bounded
[original-resource prefetch provider](../display/NATIVE_ANIMATION_GROUP_PREFETCH.md).
The original flags/EXEC requests are also bound. Subsequent
[key/background preparation](NATIVE_MARIO_KEY_AND_BACKGROUND_PREPARATION.md)
now reaches previous-party with fully admitted actor storage, or background-off
with Game's key-only adapter. Game still runs separate Bero and Trouble runtimes.
Real GP/checkpoint ownership, native sound, subsequent
Mario/party services, NPC setup/gazigazi, original house05 inline installer,
shared map heap/GW lifetime and complete case activation remain required before
using the full parent for live area loading. No story or dialogue is fabricated.

This change adds no draw calls. Existing cached Citro3D field ranges remain in
use. Normal isolated east/plaza travel remains visibly intact at60 App FPS and
100% Azahar speed in inspected captures; this is emulator regression evidence,
not New2DSXL hardware timing or original raster parity.
