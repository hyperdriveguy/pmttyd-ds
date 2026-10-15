# Native NPC setup world admission requirements

Read-only US G8ME01 revision0 source/data audit for the next real retained
registry adapter. This supplements [ordered callback mechanics](NPC_SETUP_ORDERED_INSTALLATION_AND_RETAINED_PROVIDER_FRONTIER.md),
[retained actor ownership](NATIVE_RETAINED_NPC_REGISTRY_OWNER.md),
[map registration](NATIVE_TAGGED_MAP_NPC_REGISTRATION.md),
[history admission](TAGGED_MAP_NPC_HISTORY_GATE_AND_DEFINITION_SOURCE.md), and
[room teardown](ORIGINAL_NPC_ROOM_RESET_AND_TEARDOWN.md). This source audit does not itself prove
native integration or all init/regular callbacks. The subsequent
[registered-world adapter](NATIVE_REGISTERED_NPC_SETUP_WORLD.md) implements the
scoped retained bridge; this document specifies its source provider boundaries.

## Field-battle mode is a fresh halfword owner

At evt_npc_setup8005323c, each genuine invocation resolves operand0/table and
calls fbatGetPointer8004829c before branching on captured first. The getter
loads GP through80415fd0 and then GP+168; its result is unused here but the
request must still occur. It is not an NPC actor field or the registry base.

After the first scan's terminating name read, fbatChangeMode8004828c(1)
independently loads fresh GP through80415fd0, loads fresh GP+168 and stores
**halfword**1 at F+0. It must preserve F+2 and the remainder of retained F,
including history. Do not implement a word store, cache the earlier getter's
F across all row effects, or confuse this mode with GP+14/battle-context
selection, Mario control depth, or query flags. Its freshness is separate
from native admission checking that the pointer aliases the real retained
FieldNpcRegistryOwner F allocation.

The first scan then returns0. A real subsequent invocation resolves the
table/getter again, polls init IDs, then creates regular roots. The mode
setter is not repeated in resume or regular completion.

## Lookup and history-skipped rows

First-phase names use npcNameToPtr_NoAssert80049934. A genuine absent actor
returns0 and skips that row's flags, init event, metadata, battle and pose
effects. This is distinct from an unavailable provider. Names resolve through
active1 and original exact bytes; main flag800/render hiding and query2 do
not remove a registered actor from lookup.

Resume poll and regular scans use release npcNameToPtr800499e0. Despite the
historical 'asserting' label in callback APIs, the original US release body
returns the exhausted scan pointer on missing names; it has no null/assert
fallback. These later source scans do not skip absent rows. A bounded native
world must admit actual valid matching registered actors for these reads;
when absent it must hold before effects rather than filter the table, reuse
an idle catalog actor, invent an actor, or treat the unchecked pointer as a
registered actor.

History suppression belongs to the earlier tagged-map producer, not to a new
npcSetup filter. Actual gor_02 has no named map-definition row in8030f668;
it uses the null terminator with flags0, so its ordinary producer's history
skip gate is disabled and all15 tagged names are registered. A future area
with enabled history needs source-backed consistency between its setup table,
actual history choices and later unchecked lookup domain. The first-phase
skip rule alone does not prove every such combination can safely finish.

## Synchronous world admission and retained deletion boundary

The native cursor captures actual source first/table and provider ownership
before world admission. Admission must establish infallible synchronous
lookup/read/store/battle/strcpy/pose/mode services for that invocation and
stable backing ownership across event-capacity holds. It is not permission
to acknowledge missing battle setup0, consume an unavailable RNG, or replace
real field-model poses with finite idle-render actors.

For every present first-phase actor, validate owned full actor340 storage,
actual descriptor captured from actor+28, descriptor strings and bounded
actor pose buffers, current actor+104 pose ownership, all source row fields
and float domains, and the exact battle service's complete data/RNG/switch
owners. The descriptor is captured before flag stores/init creation and
survives providers; replacing actor+28 later must not change that captured
owner. Pose+104 is read fresh only at its source request after battle/strings.
Zero/nullable source strings preserve their source skip behavior; they are
not missing-resource success.

The mode owner needs live GP/F reads and a writable F halfword. Full scalar
word readability is not equivalent to initialized actor/resource ownership.
The retained registry/FieldPosePool and packed battle adapter can provide
those owners. Actual source world callback support remains separate from
pre-admitting that their requested init event can be allocated.

Event allocation is the one supported mid-invocation hold: when an init or
regular script is nonzero, source first reads the existing actor event ID,
deletes it when nonzero, then rereads/captures the script and requests Entry.
If native Entry has no slot, the cursor retains that captured script and
after-deletion phase; retry must not repeat deletion or earlier row effects.
Source init roots use priority0/flags0, byte+10=0, context+170=actor; regular
roots use priority0/flags20, byte+10=1, context+170=actor. Both publish actual
created ID to actor+118/+11c respectively. They are independent roots, not
attached children of the setup caller. A source null init stores ID0 without
deleting an old ID; null regular skips without clearing/deleting old ID.

For a safe native synchronous erase service, exclude any actual deletion
closure containing the currently executing setup event. evtDeleteID80036664
scans all active matching IDs in the currently selected context; each target
recursively deletes its waiting child and active brotherOwner dependents,
then performs waiting-parent copyback and clears storage. A target can erase
the caller by being the caller itself, its waiting ancestor, or an owner
whose brother traversal reaches it. Checking only targetID!=callerID is
insufficient. Duplicate matching IDs and relationship cycles also need the
adapter's valid-domain admission. See [exact delete contract](../event/EVENT_DELETE_AND_COPYBACK.md).

This exclusion is native admission, not an original source no-op guard.
Never silently skip deletion to make an unsafe target appear successful.
Admission must guarantee safe graph/ID ownership through the actual erase
point; if callbacks can mutate IDs/links between admission and erase, they
must provide an equivalent complete synchronous guarantee or be excluded.
Deleting the executing event would also invalidate its retained cursor via
the area's retirement observer, so continuing with that C++ cursor cannot
faithfully model arbitrary source self-destruction.

## Actual gor02 npcEnt and event closure

Original npcEnt@gor_02.o is REL44f98/5c0, relocated into the shared90 guest
namespace at90524f10. It contains15 records of5chex plus a null-name terminal
record. Raw source SHA-256 is
565d2882a4e5627523750d3e72177d4e748bb6e02cad40f93bc4d5e51584b9b2.
All15 +58 battle values are actual0, not-1. This is later setup0 work,
separate from tagged registration's BTLNO_INVALID/-1 clear. See
[battle0 dependencies](GOR02_SETUP_ZERO_BATTLE_INDEX_AND_ORIGINAL_TRIBE_DESCRIPTORS.md).

| Row / original name | Init script | Regular script |
|---|---|---|
|0 パワーアップ屋|90520cb4 powerupya_init|0|
|1 クリハカセ|90521040 kurihakase_init|0|
|2 情報屋完璧|80415e18 npc_init_evt|0|
|3 盗賊団親分|905218ec oyabun_init|0|
|4 盗賊団１|90521eb8 tozokudan1_init|0|
|5 盗賊団２|905223bc tozokudan2_init|0|
|6 盗賊団３|90522d60 tozokudan3_init|0|
|7 サンダース|90522edc sanders_init|0|
|8 クリボー４|90523868 kuribo4_init|9052389c kuribo4_regl|
|9 チューさん３|90523d40 chusan3_init|0|
|10 ロテン３|90523e78 roten3_init|0|
|11 ボロドー１|90524138 borodo1_init|905241a0 borodo1_regl|
|12 ボロドー２|90524338 borodo2_init|0|
|13 クラガリさん|90524620 kuragarisan_init|0|
|14 ルイージ|90524eb8 luigi_init|0|

The table order differs from original tagged registration order; bind exact
names, never ordinals across those two producers. The terminal row begins
90525474. DOL npc_init_evt is1chex, actual hash
94b7007969ccdac1b7489cde7d8dc91a3001f17fb3927189744925fce6073326;
it calls evt_npc_flag_onoff, so its nonnull pointer is not an empty init stub.
chusan3_init and roten3_init genuinely contain RETURN/END only (8bytes),
which the actual event interpreter may execute normally.

Direct USER_FUNC dependencies in those15 init and2 regular bodies are:

| Original callback | Uses |
|---|---|
|80070110 evt_run_case_evt|powerupya_init|
|80053158 evt_npc_set_position|conditional placement in multiple init scripts|
|800e81f4 evt_door_param|powerupya_init|
|80038468 evt_mapobj_flag_onoff|powerupya_init|
|80074664 evt_hitobj_onoff|powerupya_init, tozokudan1_init|
|800500b0 evt_npc_flag_onoff|most nonempty init scripts|
|80050988 evt_npc_set_ry|tozokudan1_init, tozokudan3_init|
|8005088c evt_npc_set_ry_lr|luigi_init|
|80052e98 evt_npc_get_position|both regular scripts|
|80237878 urouro_init_func|both regular scripts|
|802372a8 urouro_main_func|both regular scripts|

This census includes conditional requests without claiming a particular
story/quest branch. Switch/operand services, case handler scripts installed
as data operands, talk scripts installed into actor metadata, and every
transitive callback's world/resource owners remain necessary at their actual
execution points. It is not an assertion that all11 primitives are bound.
A real blocked init stays live and prevents setup's source poll completion;
do not retire it simply because its visual pose already exists.

The current scoped area asset exporter follows the original full gor02 root,
retains the npcEnt REL data/script closure and linked DOL npc_init_evt with
actual adjacent prefetch context. Check source object metadata/segments in
romfs/door-data.json and door-data.pmevt, generated by tools/door_data_assets.py
and tools/dol_event_closure.py. These private artifacts are evidence of byte
availability, not execution/provider availability. Init/regular direct bodies
above were decoded from retained relocated data with dol_event_inventory's
strict command parser; original identities are from us_symbols.csv and REL
relocations. No original executable PPC is interpreted as EVT.

## Implementation frontier

Compose the actual FieldNpcRegistryBinding and its retained FieldPosePool,
source full actor bytes, shared area EventNativeRuntime and complete numeric
battle/RNG provider. Provide exact fresh GP/F halfword mode access and source
lookup domains. Keep these owners alive through independent init/regular
roots, capacity holds and actual room reset. Do not substitute npcEnt setup
for map registration, nor promote map registration to setup completion.
Subsequent query/Main/facing/GPU/regular-world functionality remains explicit.
No code changes, tests, builds or original runtime observations accompany
this contract.
