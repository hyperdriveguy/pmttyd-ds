# Generic door entry and preceding gor02 case installers

US G8ME01 disc0 revision0, read-only static recovery for
[EVT-01](../../requests/EVT-01.md). Complements
[house05 incoming/frontier](HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md),
[outer-bank provenance](HOUSE05_OUTER_INIT_BANK_CLOSURE_AND_ROAD_LEAF_CENTERS.md)
and [original mutable door lifetime](HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md).
Addresses/offsets/masks are hexadecimal; local indices/counts decimal.
This distinguishes genuine installation from activation of the installed cases.

## Complete generic entry stream

DOL evt_door_entry8033156c/134 ends at803316a0 exclusive, file32e56c;
SHA2566269747e7ff14d9effbd84761663f25e880e3ed3c191c842e974db0559e78281.
Its22 commands have only five distinct callbacks:
evt_door_param800e81f4, evt_mapobj_flag_onoff80038468,
evt_hitobj_onoff80074664, evt_run_case_evt80070110,
npc_hide_onoff800e8090. This child does not allocate/copy a door record.
LW0 is an incoming original record pointer; it is not a newly created case ID.

| Address | Original operation |
| --- | --- |
| 8033156c | door_param(LW0,3,LW1,LW2) |
| 80331584..5a8 | IF_NE25(LW1,0): mapobj_flag_onoff(1,1,LW1,1) |
| 803315ac..5cc | IF_NE25(LW2,0): hitobj_onoff(LW2,1,0) |
| 803315d0 | door_param(LW0,1,LW1,LW2,LW3) |
| 803315ec..624 | IF_NE25(LW2,0): SET50(LW15,0); run_case_evt(9,1,LW2,0,80330724,0) |
| 80331628..660 | IF_NE25(LW3,0): SET50(LW15,0); run_case_evt(9,1,LW3,0,8032f670,0) |
| 80331664 | door_param(LW0,9,LW1) |
| 80331678..694 | IF_FLAG30(LW1,1): npc_hide_onoff(LW0,1) |
| 80331698/69c | RETURN2 / END1 |

Selector3 publishes record+20 then+24; selector1 publishes+0c,+10,+14;
selector9 publishes record+0. All use raw output operands with evtSetValue,
in source order. Selector9 branch800e8474 reads raw operand2, then at847c
reads captured record+0, then calls evtSetValue80030cfc at8480. It returns2.
The tail tests bitmask1, not signed magnitude or record+8. npc_hide_onoff
walks record+34 names, skips actual absent registered actors and toggles main800;
unknown native registration is not evidence of absence.

Both generic cases capture LW15=0 and the entire actual16-word bank before
any output publication; final argument0 requests no returned case index.
At both snapshots LW1 remains door model name, LW2 close-hit name, LW3 open-hit
name. Only after registrations does selector9 overwrite final LW1 with word0.
LW4..14 are retained incoming words. LW15 changes only when a nonnull case
branch executes; if neither branch executes, incoming LW15 survives. Attached
child completion copies actual final locals back to the parent.

## Complete door-param selector ABI

The callback captures its argument pointer once, resolves raw operand0 with
evtGetValue80031260, retains that record address, then resolves operand1.
It loads two original zero XYZ templates802e41b8/802e41c4 into separate stack
triples before the unsigned selector<=11 dispatch gate. Every selector outside
0..11, including negative signed values, returns2 with no output writes and
no record-field dereference. It still resolves both input operands first.
All selector numbers in the following table are decimal.

| Selector | Target | Ordered record offsets / result |
| --- | --- | --- |
| 0 | 800e8288 | +4 |
| 1 | 800e829c | +0c,+10,+14 |
| 2 | 800e82d0 | +18,+1c |
| 3 | 800e82f4 | +20,+24 |
| 4 | 800e8318 | door_position_sub mode0,first XYZ |
| 5 | 800e8390 | door_position_sub mode0,second XYZ |
| 6 | 800e8408 | +28,+2c,+30 |
| 7 | 800e843c | +40,+44 |
| 8 | 800e8460 | +8 |
| 9 | 800e8474 | +0 |
| 10 | 800e8488 | door_position_sub mode1,first XYZ |
| 11 | 800e8500 | door_position_sub mode1,second XYZ |

For each direct-output lane, freshly load its raw destination operand
(starting operand2), then read the corresponding field from the captured
record, then evtSetValue80030cfc; finish that publication before reading the
next raw destination/field. Do not capture all destinations or all fields
before the first setter. Mutations made through a setter/provider can affect
later field/destination reads; changing event.arguments cannot replace the
captured argument base or captured record.

The four derived selectors read record+4,+0c,+10,+14 in that exact order,
then call door_position_sub800e884c with those four word arguments, pointers
to the first/second zero-initialized stack triples and mode0/1. They do not
resolve these record words as event operands. On return, for each selected
XYZ component in order: read that stack float, then raw destination operand,
truncate toward zero using source fctiwz, then evtSetValue with the signed
integer word. These are integer publications, not evtSetFloat outputs.
Store sites are8350/836c/8388(selector4),83c8/83e4/8400(selector5),
84c0/84dc/84f8(selector10),8538/8554/8570(selector11).
The actual geometry helper is a separate required provider; a scoped direct
selector implementation must hold selectors4/5/10/11 rather than invent
positions or silently return the initialized zeros. Complete helper listing
is artifacts/door-entry-position-sub.txt; this contract does not yet close
its hit/map geometry and source SDK math semantics.

The original dispatch table occupies803316a0/30. Its first word800e8288 is
also the actual immediately adjacent scanner-prefetch context after generic
entry END8033169c. A script-only pack can retain precisely one original word
at803316a0 as context without packaging the whole table or claiming a larger
event. A callback implementation using the table as source data may instead
retain all12 original dispatch identities; never execute those PPC addresses.

The hide helper captures its args, resolves record then hide flag, and reads
record+34 once. Its loop immediately dereferences that list pointer, so a
NULL list is not source empty-list success. A valid list terminates with a
NULL name word. Each nonnull name requests NoAssert lookup; actual missing
actors are skipped. Found actors have their current flags read and main800
set/cleared before the next list word is loaded. Original list storage and
registered actor lifetimes must remain available throughout the callback.

## Earlier original gor02 children

The contiguous outer sequence sets LW0 to each original record immediately
before RUN_CHILD94. These records are immutable original REL data, unlike the
later house05 copied GW11 record:

| Outer file | Record file/size | Attached child file/size |
| --- | --- | --- |
| 4d838 SET,4d844 child | door_data_02@ gor_02.o 45cc8/48 | evt_door_02_entry45d10/100 |
| 4d84c SET,4d858 child | door_data_03@ gor_02.o 4618c/48 | evt_door_03_entry462ac/100 |
| 4d860 SET,4d86c child | door_data_03_roll@ gor_02.o 46770/48 | evt_door_03_roll_entry467b8/50 |
| 4d8d0 SET,4d8dc child,story>=23 | door_data_04469b8/48 | generic DOL8033156c/134 |

Door02 and door03 perform the same selector3 hide/hit-disable and selector1
query pattern as generic entry, without its selector9/NPC-hide tail.
Door02 close case captures LW15=8; open captures0, targeting generic DOL
close/open. Door03 close/open both capture0, targeting local wrappers
file46240/6c(evt_door_sanders_close) and461d4/6c(evt_door_sanders_open).
The roll child performs selector1 only, SETs LW15=0 unconditionally, and
registers run_case_evt(0,1,LW3,0,evt_door_03_caseopen file463ac,0).
Its case type is0, not9; do not route it through door A-contact candidacy.
All cases use ordinary priority0 rather than Bero priority20.

Exact record names:

| Record | +0c model | +10 close-hit | +14 open-hit | +20 interior | +24 interior-hit |
| --- | --- | --- | --- | --- | --- |
| door02 | S_i02_doa | A_ie_02_doa02 | A_ie_02_doa01 | S_ie_02_naka | A_ie_02C |
| door03 | S_ie_03_doam | A_ie_03_doa_03 | A_ie_03_doa_01 | S_ie_03_naka | A_ie_03C |
| door03_roll | S_ie_03_doam | NULL | A_i03_rol | S_ie_03_naka | A_ie_03C |
| door04 | S_ie_04_doa | A_ie_04_doa02 | A_ie_04_doa01 | S_ie_04_naka | A_ie_04C |

All four original record first words are0,3,1. The generic door04 selector9
hide gate is therefore false. Retaining its real NPC-list pointer does not
authorize adding an initial hide call. Story<23 follows a different outer
branch: hide S_ie_04_naka, disable A_ie_04C and register the local locked
notice event evt_door_04_junbi; it does not call generic door04 entry.

## Pack closure, lifetime and smallest faithful frontier

Current tools/door_data_assets.py selects only house05 data roots. Its REL
closure does not supply these preceding original records/entry children, nor
DOL generic entry/open/close script words merely because imports name them.
Extend finite selected data by exact namespace-qualified roots: names such as
door_data_02 occur in several gor namespaces. Preserve actual ADDR32 REL
bindings and original strings, and exclude executable sections. DOL generic
entry needs no relocation; its callbacks and generic case-script operands
remain original DOL identities. Original creation label scanning prefetches a
word after END; retain actual adjacent original context separately from the
claimed event-object size, as with the Bero root.

Installing a case only requires storing its target address and saved bank;
claiming activation requires executable EVT data at that address plus every
requested service. Generic open8032f670/7f0 and close80330724/6fc further
launch DOL sound/light/dark helper events and record-provided callback scripts.
The source targets include house_open_se,house_close_se,door_open_se,
door_close_se,door_up_se,door_down_se,door_dark,door_dark2,door_light,
door_light2, plus indirect record callbacks. Their closure is not supplied by
a record-only pack. Camera, key locks, Mario/party movement, interpolation,
map-flag snapshots, actor material flags and original audio remain genuine
providers; do not acknowledge these effects through registration alone.

The smallest faithful next step is a retained area-owned field runtime with
one shared real case pool and original-address mutable record domain. Execute
the original finite entry children in source order with a proven supplied bank,
actual named map/hit overlays and real evt_run_case_evt snapshots. Persist the
pool and registered script identities beyond installer root death. Keep case
activation pending until its actual script/provider closure is present. Such an
isolated installer continuation is an explicit native scheduling adapter; it
does not establish that skipped NPC setup/gazigazi or every outer gor02 command
has executed. Generic entry cannot replace house05's manual installer, whose
open case captures LW15=4 and omits selector9 tail.

At source unload event ReInit precedes map heap reset; cases, events, mutable
copies, GW pointers and REL data references require coordinated area teardown.
Do not release copied records when the installer child returns, reset a shared
case pool on board interaction reentry, or carry pointers into another map.
A separate board-only manager is not automatically this shared field lifetime.

## Evidence

Fresh DOL inventory artifacts/door-entry-generic-next-closure.json. Original
REL child inventories artifacts/house05-preceding-door-entries.json and outer
artifacts/trouble-child-installer-events.json; full selector listing
artifacts/house05-doorparam.txt; open/close inventories
artifacts/house05-generic-door-events.json. REL bindings were reread from
artifacts/trouble-gor.rel with tools/rel_event_inventory.py read_bindings;
source digestd4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
No runtime/raster observation or complete area flow is claimed.
