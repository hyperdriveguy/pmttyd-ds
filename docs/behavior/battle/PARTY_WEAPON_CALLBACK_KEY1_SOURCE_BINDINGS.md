# Seven original party key1 weapon-table bindings

Static US G8ME01 revision0 DOL source census supporting
[complete Goombella callback](GOOMBELLA_WEAPON_TABLE_CALLBACK_AND_TECH_LEVEL.md).
[Verifier](../../../tools/verify_party_weapon_callback.py) pins every listed
unit-definition+0xc0 pointer,FULL source key/value table,all seven callback
body identities,and four Goombella leading16-byte weapon heads/exact keys.
Only Goombella callback and technique-level query are recovered COMPLETE
in this slice. Similar sizes/names of the other callbacks do not prove equal
control flow or source choices. Original data remains reused outside history.

## Unit-definition→data table→key1 chains

Original unit-kind definition field+0xc0 stores the data-table address. Each
source table consists of8-byte `(wordKey,wordValue)` entries ending `(0,0)`.
These static original tables contain exactly one key1 entry:

| Original namespace / party | Unit definition | Data table / size | Key1 callback / body size |
| --- | --- | --- | --- |
| unit_party_christine.o / Goombella | 80393bec | 80393b60 /40hex | 80189634 /2dchex |
| unit_party_nokotarou.o / Koops | 8038ef34 | 8038ee88 /60hex | 801890a0 /2d4hex |
| unit_party_clauda.o / Flurrie | 803791f4 | 80379168 /40hex | 8017fc68 /2dchex |
| unit_party_yoshi.o / Yoshi | 8037c79c | 8037c548 /40hex | 80180f34 /2dchex |
| unit_party_vivian.o / Vivian | 8038b89c | 8038b7b0 /a0hex | 8018303c /2dchex |
| unit_party_sanders.o / Bobbery | 8038780c | 80387778 /48hex | 80182600 /2dchex |
| unit_party_chuchurina.o / Ms. Mowz | 803818c0 | 80381750 /40hex | 80181db0 /2dchex |

The symbol name `__makeTechMenuFunc` is reused in all seven namespaces.
Tools/implementations must disambiguate by original namespace or address;
selecting a CSV row by name alone can silently pick another party. The verifier
uses qualified names for hashes. These addresses are DOL bodies,not unresolved
REL imports or absent-decompilation evidence based merely on a failed name search.

## Caller chain and required native address binding

[Generic type6 weapon producer](WEAPON_TABLE_ITEM_SPECIAL_AND_PARTY_ROUTES.md)
requests BtlUnit_GetData(retainedActor,1),calls the returned address directly
with `(passedMenu,&mutableCount)`,ignores scalar return,and handles count/
cursor/eligibility publication afterward. It adds no callability/null guard.
[Unit-data lookup](BATTLE_UNIT_DATA_CHILD_CONTINUATION.md) separately documents
live unit+0x18 data-table ownership/linear key lookup. A definition's+0xc0
source pointer alone does not prove a runtime unit currently retains that table:
unit initialization/copy/lifetime/dynamic writes still require source evidence.

Native port needs callable-address registrations for each reached callback
address,original table contents,unit-table provisioning,and callback ABI over
address-backed mutable count. Do not treat key1 as an event-script stream or
invoke it using a unit/event argument pair. The returned value is a function
address here;other data keys have other consumers and meanings.

## What the pinned source proves

All seven original key1 pointer chains exist;all source tables terminate and
have one key1 at this initial image. Goombella's fixed weapon pointers have
exact original names `btl_wn_pkr_normal`,`btl_wn_pkr_lv1`,`btl_wn_pkr_lv2`,
`btl_wn_pkr_lv3`;their heads pin key/icon-leading fields,not complete weapon,
action timing,target,cost/effect/event behavior. Names go through original
msg/US data;do not rewrite dialogue or invent missing plot/menu content.

This census is not all runtime data-table mutations,all actors,all indirect
calls,or source-event/query reachability. The remaining six complete callback
contracts are still needed before claiming the whole party weapon producer
implemented. Current-party kind translation/pouch provisioning/tech levels,
full returned weapon scripts,Game/platform/original execution/full-game system
requirements remain open. Next port documentation should recover the other
six callback bodies and compare their source/ordering differences explicitly.

## Delivered remaining-callback continuation

[Complete six-body protocol and Koops exception](PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md)
and [exact source order](PARTY_WEAPON_CALLBACKS_WEAPON_SOURCE_MAP.md) now
close the six local callbacks previously left open.All seven callback bodies
and technique-level query are complete;unit provisioning/full events/query
reachability/Game/original execution remain separate.
