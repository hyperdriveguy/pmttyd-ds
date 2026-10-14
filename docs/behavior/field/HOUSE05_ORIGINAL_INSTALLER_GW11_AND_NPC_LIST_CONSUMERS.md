# House05 original installer, GW11 and NPC-list consumers

US G8ME01 disc0 revision0, read-only static recovery2026-10-14 for
[EVT-01](../../requests/EVT-01.md). Complements
[actor/door-copy contract](TROUBLE_CENTER_QUEST1_4_WORLD_ACTORS_AND_DOOR_COPY.md)
and [Garf registration versus drawing](GARF_ORIGINAL_TALK_AND_REGISTERED_HIDDEN_ACTOR_ADMISSION.md).
Addresses, offsets and masks are hexadecimal; coordinates and allocation sizes
are decimal. This contract corrects an overbroad interpretation of the earlier
NPC-list discussion: house05 does not request main800 hide/show merely because
the generic door implementation contains that helper.

## Contiguous original house05 installer

gor_02_init_evt file4d554/87c runs NPC setup earlier, at4d630. The following
span follows the closed Trouble Center story/door conditional. It has no new
story or quest admission surrounding the copy itself. LW and GW are original
event local/global integer banks, not native case identifiers.

| File | Original request/control |
| --- | --- |
| 4d8e8 | SET LW0=door_data_05(file46c9c) |
| 4d8f4 | evt_door_data_copy(LW0,GW11) |
| 4d904 | SET LW0=GW11 |
| 4d910 | IF GSW1420 !=1 |
| 4d91c | evt_door_set_param(GW11,0,door_data_05_npc,file46bec) |
| 4d930 | ELSE |
| 4d934 | evt_door_set_param(GW11,0,door_data_05_npc_01,file46bfc) |
| 4d948 | END_IF |
| 4d94c | evt_door_param(LW0,3,LW1,LW2) |
| 4d964..4d988 | if LW1!=0: evt_mapobj_flag_onoff(1,1,LW1,1) |
| 4d98c..4d9ac | if LW2!=0: evt_hitobj_onoff(LW2,1,0) |
| 4d9b0 | evt_door_param(LW0,1,LW1,LW2,LW3) |
| 4d9cc..4da04 | if LW2!=0: SET LW15=0; evt_run_case_evt(9,1,LW2,0,evt_door_close,0) |
| 4da08..4da40 | if LW3!=0: SET LW15=4; evt_run_case_evt(9,1,LW3,0,evt_door_open,0) |

Selector3 returns record+20/+24, here S_ie_05_naka and A_ie_05C.
Selector1 returns record+0c/+10/+14, here S_ie_05_doa,
A_ie_05_doa02 and A_ie_05_doa01. Thus the two case9 registrations have
actual authored collision names and retain the same mutable copied record
through LW0. Each case snapshots all16 callerLW words; the close case captures
LW15=0 and the open case captures LW15=4. Later mutation of record+34 is
observed through their shared copied-record address; it does not rewrite their
saved local banks. The last case argument0 requests no published case index.
See [case constructor/lifetime](TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md#case-registration-and-retained-event-lifetime).

House05 manually spells out this installation. It does **not** RUN_CHILD
evt_door_entry. Generic evt_door_entry8033156c/134 instead assigns openLW15=0
and has an additional conditional NPC-hide tail at80331664..80331694.
Substituting the generic installer would change the house05 request stream.

The very next command4da44 begins a separate GSF1219==0 branch creating
evt_mobj_lock(mobj_lock00,49,446,18,50,0,evt_chk,evt_unlock,GSF1219).
Its ELSE creates/configures house06 copy and calls evt_door_entry at4dae4.
That house06 branch must not be mistaken for house05 initialization.

## Copy output and area lifetime

evt_door_data_copy800e8004/8c resolves operand0 with evtGetValue, allocates
72 bytes from current mapalloc_base_ptr8041e808 using _mapAlloc8002ff58,
copies18 words, and publishes the new address using evtSetValue(event,
raw operand1,address). It returns2 without a local allocation-failure guard.
The exact output token at4d8f4 is fd050f8b, GW11. Embedded pointers are copied,
not rebased or deep-copied. GW11 is the mutable record address, not a case slot,
door ID, interior mask or address of the original immutable REL record.

evt_door_set_param800e7f8c/78 resolves all three operands every invocation;
selector0 writes replacement pointer to copied record+34 and returns2. It
does not enumerate actors or immediately update visibility/material flags.
The lists remain original REL strings/pointers with NULL terminators:
46bec:盗賊団親分,盗賊団１,盗賊団２,NULL;
46bfc:盗賊団親分,盗賊団１,NULL. Quest1 start/end select these lists after
their actual Garf position requests. A native owner must preserve the copy,
its selected list, case references and actual GW11 binding together.

No free appears in copy/set-param or this installer. Event completion is not
proof of allocation death. [_unload and heap reset](../memory/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md#memclearindex-complete-bulk-reset-contract)
establishes event ReInit at80007b3c before memClear(1)80007bf4 and
memClear(2)80007bfc; primary map-pool reconstruction invalidates old payloads.
mapUnLoad80028d78/1a0 individually frees its own map buffers, but this listing
does not identify a dedicated free of the copied house05 record. Native area
teardown must invalidate its record/cases/GW domain; complete indirect scene/
REL alias lifetime remains open. Do not retain old REL/list addresses across
the teardown or invent a per-root-event free.

## Exact NPC hide admission: absent for this record

Fresh evt_door_param dispatch table803316a0 has selector9 target800e8474,
which returns record+0. House05's original first three words at46c9c are
0,3,1. The generic hide sites all test opcode30: **bitmask(recordWord0,1)**,
not signed >=1 and not record+8:

| Owner | Query/test and conditional call |
| --- | --- |
| generic entry | 80331664 selector9;80331678 mask1;80331684 npc_hide_onoff(LW0,1) |
| generic open | 8032f8b4 selector9;8032f8c8 mask1;8032f8d4 npc_hide_onoff(LW0,0) |
| generic close | 80330c3c selector9;80330c50 mask1;80330c5c npc_hide_onoff(LW0,1) |

House05 manual initialization omits the entry tail, and its first word0 makes
the shown open/close hide tests false. Therefore these source paths do not
initially hide or later toggle house05 NPC main800. No direct ADDR32 import
of npc_hide_onoff800e8090 exists in the inspected gor.rel. This is a scoped
static caller census, not proof against every indirect/full-game flag writer.

The helper itself resolves record and hide, walks record+34's NULL-terminated
names, uses npcNameToPtr_NoAssert80049934, skips missing actors and sets/clears
main800 only. Retained mutable main flags are valid infrastructure, but their
implementation does not authorize adding an unrequested house05 hide call.

## Actual list consumer: material lighting flags

Generic open unconditionally requests door_dark_flag(LW0,1) at8032f8e8,
after the skipped hide block and before the LW15 mask4 house-animation branch.
Generic close requests door_dark_flag(LW0,0) at80330c2c, before its skipped
hide block and later interior/exterior map-flag restoration.

door_dark_flag800e8594/60 resolves record then flag, freshly loads record
+34/+38/+3c and passes them to door_dark_flag_sub800e85f4/258. The NPC list
walk uses NoAssert names, skips missing actors, takes pose handle NPC+104,
then requests animPoseSetMaterialFlagOn80040730(handle,1800) for nonzero flag
(call800e86b4), or animPoseSetMaterialFlagOff80040710(handle,1800) for zero
(800e87b4). These are animation material flags, distinct from main NPC800.
The helper also affects Mario/parties and any supplied map-object/item lists;
house05+38/+3c are NULL. Full material/raster equivalence is not established
by this call-site recovery. The selected list does therefore matter for Garf
even though house05 main800 hide admission is disabled.

## House05 callback and provider boundaries

Record callbacks are evt_door_05_00(file46b4c),NULL,evt_wall_05(46b7c),
open46c08 and close46c40. The first rotates S_ie_05_doa using its source
tagged multiplier; wall callback copies LW0 toLW2/3, applies its two tagged
multipliers, and rotates S_ie_05sB then S_ie_05uB. Open launches
unk_evt_gor_0003ec20, then bgmoff4800,envoff4800,env_lpf(0,800).
Close launches unk_evt_gor_0003ec8c, then bgmoff201,bgmon(120,0),
bgmon(a0,0),envon(a0,0),env_lpf(0,0). Sound IDs/masks here are hex.
These local callbacks contain no NPC800 writes.

Actionable native boundary: instantiate original copied door record and shared
global binding, publish exact savedLW case records, bind actual geometry,
camera/Mario/party/interpolation/key lifetime, map flags/collision and required
original sounds, and retain actor pose material state separately from main
flags. A list-update-only provider cannot claim that the house door gameplay
or quest1 unlock scene is complete. Missing actual door/GW11 providers must
remain pending, rather than acknowledge a zero/unrelated handle.

## Reproducible evidence

Original rel/gor.rel SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Ignored reports: artifacts/trouble-child-installer-events.json,
house05-local-callbacks.json,house05-generic-door-events.json;
listings house05-doorparam.txt,house05-door_dark_flag.txt,
house05-door_dark_flag_sub.txt,house05-mapunload.txt and earlier
trouble-child-npchide.txt/trouble-door-setparam.txt. Generic open SHA256
4875a988b82384dd441f162d538455220319571873c062a596aa8363d39da471;
close0bcae33e0c587456b1152662f2e91107b7d28c3673429fc09dc947e18917b9d0.
No native code, shared documentation, assets, builds or runtime tests changed.
