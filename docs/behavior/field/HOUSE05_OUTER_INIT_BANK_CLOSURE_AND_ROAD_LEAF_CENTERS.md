# House05 outer-init bank closure and road leaf centers

US G8ME01 disc0 revision0; read-only static recovery2026-10-14 for
[EVT-01](../../requests/EVT-01.md). Extends the previously open
[incoming-bank frontier](HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md),
[Bero position publication](../overworld/BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md)
and [case lifetime](../overworld/BERO_CASE_POOL_IDS_AND_SWITCH_WRAPPER_LIFETIME.md).
Offsets/addresses/masks are hex;counts/indices decimal. No actual runtime
coordinate,case number or full native world completion is inferred here.

## Root bank zero has an actual source producer

The original gor REL _prolog,file5e4/108, registers each area's event through
relSetEvtAddr80039088. Atfile644/648/64c/650 it builds r3=gor_02 string
file13d38 and r4=gor_02_init_evt file4d554 using actual ADDR16_HA/LO
relocations. The imported REL24 call atfile654 targets80039088.
relSetEvtAddr retains name/event arguments,looks up the matching map record
and stores the event pointer at record+4 at800391c8.

seq_mapChangeMain80007c2c/af8 requests mapDataPtr(_next_map8041e5c8)
at800081bc,loads its event+4 at800081c0,testsnonnull,then sets priority0
at800081cc,flags0 at800081d0 and calls evtEntry800374ac at800081d4.
It saves actual returned eventID toGP+164. There is no caller-local copy
between this constructor and source gor_02_init execution.

[The complete root constructor](../event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md)
clears the selected1b0-byte event record and initializes metadata without
inheriting any parent's locals. Consequently the actual ordinary map-init
root's16LW words start zero. This is a source-proven initial condition for
this construction path,not a guessed native placeholder bank. Other ways
of creating/reentering an event must establish their own provenance.

## Outer prefix's explicit and callback write channels

gor_02_init_evt4d554/87c before house05 first SETs LW0=bero_entry_data
at4d5ec and executes get_info then attached info_run at4d5f8/4d600.
Later direct local SETs before4d8e8 targetLW0 for preceding door records.
Other direct SETs target source GSF/LSF,not highLW. Independent RUN_EVT
dearu_event at4d614 does not copy its local bank back.

Fresh complete DOL bodies for all18 distinct USER_FUNCTION callbacks in
this outer span were inspected. In the well-formed source memory domain,
their caller-local output channels are:

| Callback family | Caller-bank effect |
| --- | --- |
| evt_snd_bgmon,envoff,envon | input resolution then sound owner;no caller-local output |
| evt_bero_get_info | INFOARR/SW/NUM/EXEC state;no caller-local output |
| evt_npc_setup | resolves input and owns NPC/root-event setup/polling;no parent-local output |
| evt_npc_entry,set_tribe,set_position,set_ry | input resolution then actor/art state;no caller-local output |
| evt_map_playanim,mapobj_flag_onoff,hitobj_onoff | input resolution then actual named world state;no caller-local output |
| evt_hit_damage_return_set | actual table allocation/binding;no caller-local output |
| evt_mobj_kururing_floor,itembox,powerupblk | actual object/art/matrix/hit work;no caller-local output |
| evt_item_entry | resolved fields passed to itemEntry;no caller-local output |
| evt_run_case_evt | optional raw operand5 output;actual earlier outer call uses literal0,so no bank store |

No evtSetFloat/evtSetValue destinations are hidden in the inspected bodies
except optional case output above. Coordinate writes at NPC+98/+9c/+a0
are NPC previous-root fields,not eventLW0..;do not mistake matching offsets
on a different owner for local stores. Source bodies invoke actual world
helpers,so unchanged locals do not make those requests completed no-ops.
This is a normal source-owner/argument output audit,not permission for
arbitrary aliased pointers or raw event-memory writes to be ignored.

evt_npc_setup8005323c/288 makes its initialization events through
**evtEntry**,priority0/flags0 at800532cc,assigns NPC owner+170 and saves
their IDs on NPC+118. It polls those actual IDs at80053428 before starting
regular roots at80053488. These are not evtChildEntry records linked to
the gor_02 init parent,so their local changes do not copy back. Parent setup
still must wait for the real init events;this proof concerns bank provenance,
not acknowledgment of unbound NPC/world services.

Attached evt_gazigazi_entry803646e8/64 at4d6dc has seven commands:
entry,position,pera,yaw,flag,return/end. Its five inspected callbacks have
no caller-local output. It therefore returns the inherited local words
without resetting or altering highLW through those channels.

## Ordinary Bero child's high locals now have a complete initial source

Complete info_run8030ec7c/3e4 and initial_pos8030f060/278 words plus
their28 distinct callbacks were inspected for local-output channels:

- into_info explicitly writesLW1..8.
- evt_sub_get_system_flag and evt_bero_exec_get publish to actualLW0 operands.
- length publishes toLW10;initial_pos's READF then writesLW10/11/12.
- Direct script arithmetic/switch work usesLW0 and source global state.
- Other ordinary-path callbacks resolve inputs and perform Mario/party/NPC/
  camera/fade/Bero-world work,without new high-local output destinations.
- Attached initial_pos copies its actual bank back on return. Independently
  launched evt_bero_mario_enter at8030f050 does not copy its later locals back.

Thus on the actual ordinary root path,LW9/LW13/LW14 remain their
**source-constructor zeros**,not merely unknown words assigned zero by
a native adapter. LW4..8 are the latest into_info outputs (including that
inside initial_pos),andLW10..12 are actual encoded starting-tuple values.
Do not reuse an earlier cached selection if actual source input changed
before the later into_info call. The actual selected GP/Bero providers and
source event execution determine those values.

This closes the previous open high-local provenance for a completed ordinary
path. It does not claim the current native route has executed all required
Mario/party/camera/recovery side effects or that a native calculation alone
constitutes a finished original attached child.

## Preceding attached door children and final house05 words

The three original attached entry children before house05 are:
door02/file45d10 at4d844;door03/file462ac at4d858;door03_roll/file467b8
at4d86c. Their inspected bodies use selector3 and/or selector1 to overwrite
LW1/2/3 and explicitly assignLW15;they do not writeLW4..14. For story>=23,
generic door04 entry8033156c at4d8dc additionally uses selector9→LW1,
again without high-local output. Story<23's locked-door case registration
has literal0 as its output operand and does not write the bank. Their attached returns
copy preserved high words back rather than creating fresh banks.

House05 itself overwritesLW0 with its copied GW11 address,LW1..3 via
selector1,andLW15 separately for each case. Consequently these are exact
source dependencies for each final16-word case snapshot:

| LW | Completed ordinary source path | Completed save-load path |
| --- | --- | --- |
| 0 | actual mutable house05 copy/GW11 | same |
| 1 | S_ie_05_doa pointer | same |
| 2 | A_ie_05_doa02 pointer | same |
| 3 | A_ie_05_doa01 pointer | same |
| 4 | latest incoming Bero U16type | last indexed entry's U16type |
| 5 | latest incoming direction | last indexed entry's direction |
| 6 | latest incoming entry+0c pointer | last indexed entry+0c pointer |
| 7 | latest entry/default event pointer | last indexed entry/default event pointer |
| 8 | ffff0000 | ffff0000 |
| 9 | constructor0,preserved | actual last indexed BeroSW observation |
| 10..12 | encoded original Bero starting P.X/P.Y/P.Z | last indexed case type,exit-event pointer,ffff0000 |
| 13 | constructor0,preserved | last indexed target-map pointer |
| 14 | constructor0,preserved | last indexed target-entry pointer |
| 15 | close0 /open4,explicit house05 assignments | same |

For the gor_02 table's last entry(count2,index1,dokan),save-load source
fields include type2,direction9,explicit case6,exit scriptfile3edac,and
targets stringsfile184c4(tik_01)/184cc(dokan_2). LW9 still comes from
the actual observation,not a guessed global reset. Actual case allocations,
NPC init/other preceding world requests and attached-child completion remain
necessary before treating this as a fully executed original installer.
No runtime slot numbers or coordinate values are asserted by the table.

For the currently imported ordinary gor_02/w_bero source branch,the generic
ordinary column specializes to LW4=0,LW5=6,LW6=sourcefile3ee30 (the selected
row3ee24+0c),LW7=sourcefile3ed64 gor_02_event_00,LW8=ffff0000,
LW9=0,LW13=0,LW14=0. The existing explicit REL section-address policy maps
LW6 to9051eda8 andLW7 to9051ecdc;these are guest-address bindings,not original
runtime heap locations. LW10..12 still require actual source starting P and
event-float encoding. House05 replacesLW0..3/15 as above. These results
require the actual later into_info to select that branch;they do not authorize
assuming w_bero on arbitrary checkpoints,pipe entries or save-load paths.

## Imported ROAD markers are actual source leaves

Fresh raw-node checks for all8 currently imported ROADv1 endpoints found
child pointer+0c==0 and geometry-part count+5c==1:

| Area/name | Original node offset |
| --- | --- |
| gor_01/e_bero | 1517c0 |
| gor_01/w_bero | 1515b0 |
| gor_01/s_bero | 1516f0 |
| gor_00/n_bero | d03a8 |
| gor_02/w_bero | 128d40 |
| gor_03/n_bero | 134340 |
| gor_03/e_bero | 1342d8 |
| gor_04/s_bero | 120138 |

Each room-links.json center exactly equals its map-summary.json collision
node center,and all five summaries have empty scene_offsets. Converter code
uses bbox midpoint for geometry owners and transforms it by the authored
parent/local matrix;its .1 scale corresponds to source model→engine10 then
native engine→world.01. Because these are leaves,the original
hitObjGetPosSub descendant-leaf average reduces to this single transformed
center. This justifies the bounded static leaf-center adaptation for these
specific nodes;it does not justify aggregate hierarchy bounds or arbitrary
named points. Their sibling pointer is irrelevant to the top-level original
query,whose sibling traversal flag is0.

NumPy double matrix construction/summary values,ROADv1 float32 packing,
native math and original Gekko/SDK operations are not bit-equivalence proof.
Event-driven transforms and original runtime80 lookup can invalidate a static
center assumption. Actual source ray/floor/visibility state remains a
different provider. New rooms/nonleaf records must validate their own
geometry instead of inheriting this8-marker coverage claim.

## Evidence

Original REL SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Ignored listings artifacts/outer-bank-seq_mapChangeMain.txt,
outer-bank-mapDataPtr.txt,outer-bank-relSetEvtAddr.txt and
outer-bank-evt_<callback>.txt contain complete inspected callback bodies.
Original event reports trouble-child-installer-events.json,
house05-preceding-door-entries.json,house05-preceding-dol-events.json,
house05-bero-subchildren.json. Fresh geometry report
artifacts/outer-bank-road-leaf-centers.json retains source hashes,raw child/
part fields and exact compared centers. Original raw files are the read-only
ISO-derived local-assets/gor_00..04.d;converter summaries are local artifacts.
No source code/shared documentation/index/assets/build/git changes made here.
