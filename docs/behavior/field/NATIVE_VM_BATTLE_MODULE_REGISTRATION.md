# VM-owned original battle registry and module registration

US G8ME01 revision0. See [numeric setup](NATIVE_NUMERIC_NPC_BATTLE_SETUP_AND_AREA_REGISTRY.md),
[bounded setup0](BOUNDED_ORIGINAL_GOR_SETUP_ZERO_DATA.md),
[original registry protocol](FIELD_BATTLE_ENTRY_AND_RETURN.md) and
[real NPC registration owners](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md).
No complete original module loader, Game NPC factory or runtime equivalence is claimed.

## Original bytes and gor registration

`field_battle_registry_assets.py` exports these unchanged DOL spans, each pinned
to an original source hash:80305320/5cc,80415b30/270,8041fa60/a4. Hex sizes;
2272 decimal bytes. They contain all33 area records, their original pointer
order from world_data+8=803057ec, name strings and terminator80305870. Every
record originally has zero setup/number pointers+8/+c. No generated initialized
table pointer is written into the asset pack. Narrow spans avoid live EventNative
GP/runtime cells; broad sdata import is inappropriate.

Original gor _prolog REL5e4/108 calls relSetBtlAddr at REL6d4 with module-name
REL13d60→90400260 (`gor`), setup REL60e30→90540da8 and number table
REL6100c→90540f84. The prolog also registers map events; this phase adapts its
battle registration call only. Its constructor list is empty. The epilog does
not clear battle registry pointers; no invented unregister store is performed.

The number table has six8-byte name/index pairs followed by(null,0), source
size38. Its exact original strings occupy aligned source1dc80..1dce8. Names
BTLNO_GOR_TEST1, BTLNO_GOR_TEST2, BTLNO_GOR_00_01, BTLNO_GOR_02_ACLEC,
BTLNO_GOR_02_SACLEC, BTLNO_GOR_02_01 map to0..5. These are number mappings,
not admission of the corresponding battle graphs. Only setup0 is exported.
`packedBattleSetupNumber` follows dbGetDefData80043864: return the first exact
case-sensitive match's signed word; no match returns0. The source map parser's
invalid-name sentinel route is separate and can request-1. Do not conflate a
missing lookup with-1 or collapse map registration's earlier battle request
into npcEnt's later setup0.

## Native asset and VM lifetime

`door_data_assets.py` merges these original registry/module views into the
shared90 pack. New native descriptor field-area-battle.bin uses little-endian
PMFABT01,count and six-word rows(room,nameAddress,setupTable,numberTable,
firstNumericIndex,count). Its current room2 profile admits only0. These bounds
are native supported-data limits, not measured/source table-length constants.
Future area profiles need their own actual data views and registration evidence.

Candidate loading validates33 original-style registry records/zero pointers,
profile names/rows, complete finite number pairs/strings and terminator. Native
bounds are256 number rows and64 bytes per name. Rejection retains prior global
assets; existing runtimes retain their own shared asset snapshot. Runtime
initialization imports scripts transactionally as VM-owned writable RAM, then
runs the actual registration helper for its room profile. Source pack bytes
remain unchanged. Each retained VM owns its registry mutations and module bytes.
Replacing that VM destroys the copy; a future session/module loader must preserve
or transfer the appropriate lifetime across area changes. This per-VM adapter
does not claim the original global module loader's full lifecycle.

`battleSetupConfiguration(index)` reads fresh GP/prefix and registry pointers.
It returns an engaged null configuration for-1, row0 for zero/other negative
indices, and only admitted positive rows. Changed/malformed pointers or another
prefix reject availability safely before source callbacks. The registered setup
and number pointers must still match that VM's retained module profile. This
query proves module/row availability, not actor/descriptor/pose/RNG readiness.
Callers must still admit those owners and the actual supported data graph.

Host tests verify raw source hashes,33 zeroed records, original number mappings,
first exact duplicate semantics, independent mutable VM copies, unchanged source
assets, fresh GP selection, negative/unsupported indices, live pointer mutation,
malformed candidate tables and retention of prior valid assets. Isolated ARM
fixtures exercise original registration plus actual setup in the retained VM
with explicitly test-owned actors/poses/RNG. They do not acknowledge Game NPC
construction, pose success, original module-loader completion or story fidelity.
