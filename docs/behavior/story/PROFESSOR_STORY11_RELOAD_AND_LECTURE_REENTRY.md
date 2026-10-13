# Professor story11 reload placement and lecture reentry

US G8ME01 disc0 revision0, read-only static gor.rel/DOL evidence. Scoped
[OW-ROAD-01](../../requests/OW-ROAD-01.md) follow-up to the
[lecture entry/return](LECTURE_FIELD_ENTRY_AND_RETURN.md) and
[temporary party/fence](../partner/FIELD_PROFESSOR_TEMPORARY_PARTY_AND_FENCE_ADMISSION.md).
Source state predicates are not original runtime/save observations.

## NPC entry is unconditional; init supplies actual position

gor_02_init_evt REL0x4d554,size0x87c calls evt_npc_setup(npcEnt) unconditionally
at0x4d630, after road/pipe registration and before door/lecture setup. Its
npcEnt array starts0x44f98,size0x5c0; Professor record at0x44ff4 is92bytes,
name originalShift-Jisクリハカセ,main flags0x80600,secondary flags0,
init+0c→0x410c8,talk+14→0x412e0. Other record hooks/AI parameters are zero.
These record fields contain **no XYZ placement tuple**. The source map loader supplies the actor before setup; the init event
determines story position.

evt_npc_setup0x8005323c finds each previously loaded actor by exact name,
skips missing actors, and on first invocation ORs record
flags and schedules nonnull init events with NPC pointer as event owner. Its
later polling branch checks whether each init event is still alive, returning0
until they are finished. Do not catalog an actor only for story8/10 merely
because those init cases happen to be currently implemented. Professor's
record still exists at story11; scene visibility is a separate predicate.

Original tagged-map **m/gor_02/s** supplies the Professor NPC block with name
andtribeクリハカセ,position(0,-1000,0),direction270,aitypeなし,territory
NPC_TERRITORY_TYPE_NOTHING,zero territoryvectors/search/homingrange/angles,
andbtl_setup_noBTLNO_INVALID. This is the actual loaded map default.
Generic npcEntry0x8004b894 earlier initializes XYZ fromDOL0x802c11f8=(0,0,0)
andunit scale; mapplacement theninit override it. The92-byte setuprecord
contains neithermapdefaultXYZ nor yaw. Sourceinit changesposition but does
notissue a yaw request; story11 reload retains originalmapyaw270.
Original Professor tribe binds c_kuri_h/S_1, not a fabricated character asset.

## Exact init cases, including the flag-dependent story11 branch

kurihakase_init REL0x410c8,size0x218 first requests NPC main flag0x40000000.
It switches raw story0xf5de0180; exact CASE_EQUAL matches do not fall through
to later ranges. Relevant source positions in original engine units:

| Story predicate | Source position | Source site |
| --- | --- | --- |
| exactly8 | (-135,0,-240) | 0x410ec |
| exactly9 | (0,-1000,0) | 0x4110c |
| exactly10 | (-220,0,305) | 0x4112c |
| exactly11 AND rawflag0xf8406037==0 | (-220,0,305) | condition0x4114c,position0x41158 |
| exactly11 AND thatflag!=0 | (0,-1000,0) | 0x41174 |
| subsequent range11..17 | (0,-1000,0) | 0x4119c; exact11 already matched |
| range376..402 | (0,-1000,0) | 0x411c0 |
| exactly18/64/120/174/221/271/347 | (-140,0,-225) | individual exactcase requests |
| default | (-140,0,-250) | 0x412bc |

Thus after original lecture decline/return writesstory11, a reload with flag0
must render Professor outside at nativeengine*.01(-2.20,0,3.05). Keeping a
live scripted actor until roomexit while omitting it on load is not equivalent.
Do not apply the following range11..17 to exact11 or hide Professor solely
because lesson mode is finished. PositionY=-1000 is the source hiding request,
not a reason to delete the unconditional actor record or lose its story owner.

## Flag1207 is set by original pipe departure

Raw0xf8406037 decodes to GSFindex1207 under the recovered variable namespace
(signedoperand+130000000). It is a distinct flag, not the story word or an
inferred successful-lecture marker. Among parsed annotated gor EVT spans, its
only sites are Professor init's read and **gor_02_event_01**:

| Source site | Request |
| --- | --- |
| 0x3edac | IF_EQUAL(flag1207,1) |
| 0x3edb8 /0x3edc4 | LW0=0;return when already1 |
| 0x3edcc | evt_party_stop(1) |
| 0x3edd8 | evt_party_cont_onoff(1,0) |
| 0x3ede8 | evt_mario_set_party_pos(1,11,0,-1000,0) |
| 0x3ee04 | flag1207=1 |
| 0x3ee10 /0x3ee1c | LW0=0;return |

This event is0x3edac,size0x78,11commands,SHA256
8e27b0b76820b43299165bec6d5b734031fa368d76f72d83a779fecc1507d8a2.
It is the customout callback at+24 of BEROrecord0x3ee60 for **dokan**,
target **tik_01/dokan_2**. LW0=0 leaves the generic pipe route active.
The callback itself has no story11 test. Preserve its actual flag/party
requests and external pipe admission, not an invented lecture flag write.

Consequently fresh prologue story11/flag0 keeps Professor visible; after that
pipedeparture makesflag1, story11 reload hides him. A source-correct native
save/load adapter must retain this flag independently of story. The annotated
gor span census is scoped: othermodules/computed/directmachinecode writers
are not excluded. Persistent namespace/save serialization and all pipe owners
remain their own contracts.

## Room unload and fresh story10 poll

_unload DOL0x80007ab4,size0x178 calls NPC teardown service0x8004bafc with0
at0x80007b0c. Later at0x80007b3c it calls evtmgrReInit0x800377b0,then
case-system reset0x8006ed28. The existing
[context reset](../event/EVENT_CONTEXT_INITIALIZATION_AND_RESET.md) and
[reset caller ordering](../event/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md)
recover the current-context selection and bulk entry reset. Ordinary field
GP+14=0 selects field context; do not generalize _unload into a field-only
reset without that selector domain. Bulk reset is not individual event-delete
copyback/callback execution, and full NPC teardown/resource bodies remain
dependencies. Native must not leave old-room lecture polls alive across unload.

On fresh gor_02 init, after actor setup and door setup, exactlystory10 at
0x4db94 RUN_EVENTs kurihakase_lecture at0x4dba0. Story11 does not take this
branch. Lecture entry captures currentMarioXYZ as initial previousposition and
starts its original crossingpoll; it does not restore old eventprogramcounter
or start the intro solely because story10 is present. Keep the documented
original LV1→LV5 update and segment predicate rather than rewriting the poll.
Crossing detection/camera/input/battle owners remain required. Fence event's
story10 RUN_EVENT and roominit story10 RUN_EVENT are separate admissions;
unload/reentry must avoid retaining the old event and spawning a duplicate.

## Reproduction and bounded validation

Inspected original gor_02_init_evt,kurihakase_init,gor_02_event_01 via
tools/rel_event_inventory.py; source NPC record through selfrelocations;
evt_npc_setup,npcEntry,_unload through tools/inspect_dol_function.py.
The Professor taggedNpc block was read directly from originalm/gor_02/s.
Parsed annotatedgorEVT spans were scanned for rawflag0xf8406037 only;
that census is not a whole-game writer proof. DOL defaultXYZ read directly.
No native source edits or original payloads added. Original save/transition
traces, full NPC teardown and source persistentflag adaptation remain open.
