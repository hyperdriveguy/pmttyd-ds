# Native retained NPC setup and independent event scheduling

Source: [complete ordered callback](NPC_SETUP_ORDERED_INSTALLATION_AND_RETAINED_PROVIDER_FRONTIER.md),
[actual gor02 battle/tribe dependencies](GOR02_SETUP_ZERO_BATTLE_INDEX_AND_ORIGINAL_TRIBE_DESCRIPTORS.md)
and [shared parent frontier](SHARED_BERO_NORMAL_POSITION_AND_COMPLETE_COLLISION_OWNER.md).

fieldNpcSetupEvent implements8005323c with explicit synchronous world providers
and real event creation/deletion/ID checks. It performs source flag ORs,captures
actor+28 before deletion,installs every setup metadata lane in source order,
calls required battle setup,then uses the captured descriptor for ordered
strcpy and fresh animPoseSetAnim. Integer pair loads precede both stores;
row3c/40/44 is read again for the second destination triple. Null init stores
initID0 without deleting its old event; null regular leaves its prior ID alone.

FieldNpcSetupWorld::admit runs after actual argument resolution and the source
ignored fbat getter,but before row effects. It must guarantee all world callbacks
are synchronous,infallible and backed by real registered actors,readable rows/
descriptors,valid bounded pose strings,resource handles,battle/RNG services and
mode owners for this invocation. It may inspect the actual caller to reject
unadmitted self/ancestor deletion IDs. False has no effects. Initial nonasserting
lookup may return real null and skip a row; later asserting lookups must be
separately admitted and nonnull. An absent actor/resource provider is not a null
lookup or a constructed actor. Native primitive bindings are mandatory; there is
no Game adapter that equates finite idle catalog entries with full registration.

Event allocation is a separate boundary. The helper deletes the required old
ID,then freshly captures the script,then attempts creation. Null creation must
have no constructor effects. This retains row/actor/descriptor/script and resumes
at creation without repeating flag ORs,deletion,metadata,battle RNG or pose work.
A full pool cannot prevent the preceding required deletion which may free the
replacement slot. The source constructor is invoked only after actual native
capacity is available. Script pointers must already have admitted readable
original scanner closure; no arbitrary guest instruction stream is certified.

The cursor tracks world,memory,event-service and actual EventEntry/ID identity.
A provider/VM/caller swap cannot resume an old invocation. Resource backing must
remain valid across allocation holds; retire events before replacing Game/world
storage. New first=true resets the cursor,including source restart. A genuine
source return0 ends this invocation and resets native state. Its next first=false
invocation freshly resolves args/table,calls the getter and scans from row0;
first live init returns0 immediately. Only native suspension retains partial
progress. Event locals and waitCount are not used as invented scratch storage.

The resume second scan reads actor+124,not row+10. After actual old regular deletion
it reads actor+124 again,then creates a root with priority0/flags20. Init roots
use priority0/flags0. Actual root constructor,byte10=0/1,context170=actor and
ID publication are ordered. These are independent roots with source typeef,
not attached children of the setup caller. Actual null regular skips deletion.

FieldAreaEvent owns cursors per actual event ID and clears them on retirement,
new source first visits and completion. Absent typed providers retain first
admission before a generic pure hold; installing a typed owner later cannot
consume the original first phase. Partial generic world effects cannot be
handed to a new typed owner. Its create adapter checks the actual current VM
context for a free slot after deletion,then uses createRootEvent. Actual VM
retirement does not delete independent NPC init roots when only the setup
caller is retired.

Host fixtures verify source mutation/order distinctions,allocation suspension,
full-pool replacement,null paths,first retention,fresh polls/restarts,identity
refusal and regular metadata changed by init/deletion. The actual original gor02
parent test installs a deliberately test-owned actor memory model after a pure
hold and verifies fifteen original init roots in the same VM,with exact script
addresses/context/byte/type/flags. It supplies synthetic battle/pose world effects
only to test callback forwarding and event scheduling. It is not native Game
registration or a claim of populated battle setup correctness; it cannot be
shipped as a Game provider. Independent unrelated story events make a single
fixed total event count unsuitable for that continuation.

All fifteen original setup rows request populated battle index0. Per present
actor this consumes at least sixteen irand(1) and one irand(50),not a null-clear
shortcut. Actual area-table registration,shared RNG,battle data/kind/stage closure,
original tribe descriptors,pose owners and NPC registration remain required for
Game admission. The current event pack alone does not prove those data owners.
Existing render models/body/wander initialized flags do not fill that gap. Keep
full live area and independent Mario-entry service integration open.
