# Native area-owned door data and GW11

The native field now retains a mutable copy of the original house05 door record.
See [source installer and consumers](HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md)
for the original US addresses and exact distinctions between copying, installing
cases, changing parameters and consuming the lists.

## Original data and finite native initialization

`tools/door_data_assets.py` reads `rel/gor.rel` from the read-only ISO and verifies
its SHA256. The ordinary REL data packer follows original relocations from
`door_data_05` and `door_data_05_npc_01`: 27 objects, 1048 packed bytes.
`PMDRDT01` separately records room2, original record/list addresses, GW11 and
quest1. This is native metadata for gor_02_init_evt4d8e8..4d948, not a claim to
execute the entire original area installer or arbitrary PPC.

After retained NPC initialization succeeds, `Game` copies all 18 record words
without rebasing embedded pointers. It selects the normal list for known
GSW1420!=1, or the alternate list for1; unknown quest state holds initialization.
Initialization runs once per area/resource generation. Later quest changes do
not replay it or overwrite the live parameter selected by an event.

## Actual storage and event binding

`FieldDoorDataWorld` owns the copied record and its original read-only data bank.
The native guest arena84000000..840fffff maps actual owned72-byte records; these
addresses are native storage policy, not original mapAlloc addresses. GW11 is
imported into the running Trouble Center VM work context before execution. It
is neither an interior mask nor a door-case identifier. Other future room
records can use the same descriptor/owner; original global EVT/shared guest RAM
and arbitrary area installers remain separate work.

The native DoorParameter callback resolves the original script arguments and
executes the original small service: selector0 writes record+34; other selectors
complete without touching memory. A missing/uninitialized copied record remains
pending for selector0. The real original Q1 acceptance child now changes the
list to the two-name alternate after moving Garf outside; its ending child
restores the three-name normal list after returning Garf. The immutable source
record is unchanged. This acknowledges the completed parameter mutation only.

Room change, actor-catalog replacement or data-resource generation change clears
the copies. Area synchronization occurs before collision-hold early returns;
the event adapter also synchronizes before publishing GW11. Event death alone
does not free this map-owned record. The monotonic native address cursor survives
invalidation, preventing stale-address reuse; exhausted1MiB capacity holds new
initialization. That bounded policy is not the original heap implementation.
Failed resource replacement leaves the last valid data bank live.

## Boundaries and checks

No case registrations, exact LW15 snapshots, door transforms, collision toggles,
key49 unlock sequence or material1800 consumers are acknowledged here. In
particular the parameter write does not set NPC main800 or hide Garf. House05
has a false generic800 guard, and the original unguarded list consumers change
animPose material flags. Those distinct owners must be bound before claiming
original house05 door behavior.

Host tests compare all72 source bytes/relocations and both exact SJIS name lists;
test once-only init, original-bank immutability, actual mutable writes, unused
selector semantics, malformed replacement, stale addresses and held-simulation
room/catalog changes. Native-board integration executes the original Q1 start
and end children against the actual Game-owned copy. Native render smoke is
recorded separately in [progress](../../PROGRESS.md); a host mutation test is not
native door-animation evidence. Full-game and additional-area fidelity remain
incomplete.
