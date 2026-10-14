# Trouble Center quest1/4 world actors and mutable door copy

US G8ME01 disc0 revision0; read-only source recovery2026-10-14. Complements
[board provider boundaries](TROUBLE_CENTER_NATIVE_BOARD_PROVIDER_BOUNDARIES.md)
and [door case9 ownership](TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md).
Static source identity/mutation evidence, not a native implementation claim.

## Actual named actors exist before the board request

Original m/gor_02/s has both actors at placeholder(0,-1000,0), direction270,
AIなし. These names are room-local authored actor identities, not enemy kinds
or generic substitute characters:

| Name | Raw Shift-JIS excluding NUL | Tribe | Original tribe row/resource |
| --- | --- | --- | --- |
| 盗賊団２ | 939091af92638251 | 門番 | 80333404 / c_monban |
| ボロドー２ | 837b838d8368815b8251 | 盗賊 | 80332d20 / c_borodo_n |

Both actual tribe rows have idleS_1,talkT_1,walkW_1,runR_1 and width/height30.
The second resource is **c_borodo_n**, not c_borodo or c_borodo_g. Original
model/texture/pose conversion and actual actor registration are required before
a name-based reposition provider can acknowledge its request.

The92-byte NpcSetupInfo rows under gor_02 npcEnt file44f98/5c0 are:

| Actor | Row | Main/secondary masks | Init | Regular | Talk |
| --- | --- | --- | --- | --- | --- |
| 盗賊団２ | 45164 | 80600 /0 | 42444 tozokudan2_init | NULL | 42c04 |
| ボロドー２ | 453e8 | 80600 /0 | 443c0 borodo2_init | NULL | 44518 |

Row+0 is name,+4/+8 main/secondary masks,+c init,+10 regular,+14 talk.
gor_02_init_evt requests evt_npc_setup(npcEnt) at4d630. The
[source setup owner](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md) installs flags,
creates actor-owned init events and waits for init completion before returning;
it saves talk callbacks and only then admits regular callbacks when nonnull.
These two rows have no ordinary regular event. The map setup resource actor
construction precedes this callback; evt_npc_setup does not invent the actor
from a missing name. Exact map-loader teardown scheduling remains external.

tozokudan2_init42444/60 ORs40000000 on self. IfGSW1420==1, setposition
(445,0,120),return; otherwise(10,0,-460),return. It does not change direction.
borodo2_init443c0/8c ORs40000000. If1420==4 andGSF5406==1, setposition
(0,-1000,0),return. If1420==4 and5406!=1, use(-475,0,-180),return.
Other quests use the same(-475,0,-180),return. The underground placeholder
is an explicit source position; it must not become an invented interior mask.

Q1 start65ddc/34 moves盗賊団２ to(445,0,120),then updates the door copy below.
Q1 end65eb0/34 moves that same actor to(10,0,-460),then restores its door list.
Q4 start65e10/20 and end65ee4/20 both moveボロドー２ to(-475,0,-180).
Their world effects are nonblocking RUN_EVT children of the board/counter.
Source child completion is independent of the parent's1420 write.

## Complete evt_npc_set_position body and mutation limits

evt_npc_set_position80053158/e4 ignores firstCall. It resolves operand0
through evtGetValue, then resolves X,Y,Z in order through evtGetFloat, retaining
all three floats before calling evtNpcNameToPtr(event,resolvedName). For these
literal names the resolver uses named actor lookup; it is not event me, party
or Mario. Source actor existence is an unchecked caller invariant here.

Build the XYZ vector in local storage. Write its words to NPCprevious98/9c/a0
at800531f4..200, then copy those fields to current8c/90/94 at80053204..218.
Return2. There are no other calls after lookup: no floor snap, actor creation,
direction/pose change, visibility flag mutation, collision reset, shadow
rebuild, task-item grant, save write, event delete or delayed interpolation.
Do not acknowledge missing actors or only move a rendered copy while leaving
collision/previous-root state stale. Coordinate units remain original engine
units; native renderer conversion is a separate*.01 adaptation.

Native FieldNpcInstance has body.root/body.previous and movement.root plus a
placement generation. Its actual scripted placement owner must be extended
so later static-catalog refresh does not overwrite this mutation. At recovery
the five-model actor catalog contains neither named actor/model. A future raw
source-name field such as PMNPC009 must preserve the exact Shift-JIS identity
independently of the existing UTF-8-derived native hash/key.

## GW11 is a mutable72-byte door-data copy, not a case handle

gor_02_init_evt4d8e8 setsLW0=door_data_05(file46c9c/48), then4d8f4 requests
evt_door_data_copy(LW0,GW11). At4d904 it setsLW0=GW11. If1420!=1 it sets
the copied record's NPC list to door_data_05_npc; else to_npc_01. This happens
after NPC setup completes. Subsequent manual door registration uses that copy.

evt_door_data_copy800e8004/8c ignores firstCall: resolve source operand0,
mapAlloc(mapalloc_base_ptr,72), copy18 words without rebasing the embedded
pointers, then evtSetValue(event,RAWoperand1,newCopyAddress),return2. There is
no source allocation-failure check. GW11 is therefore the actual allocated
record address. It must be retained in the same event/global-bank domain as
the board child; current carrier's unrelated default GW11=0 is not binding.
Map allocator reset/destruction owns lifetime; no per-request free appears.
Do not keep an arbitrary record pointer alive across a map teardown.

evt_door_set_param800e7f8c/78 resolves all three operands through evtGetValue
in order, every invocation. If resolved selector operand1==0, write resolved
operand2 to *(resolvedOperand0+34hex); other selectors do nothing. Return2.
No event creation, case registration, copy allocation, current NPC hide/show,
pointer validation or dereference of the replacement list occurs locally.

The source door_data_05 record begins words0,3,1 and contains:
S_ie_05_doa; closehitA_ie_05_doa02; openhitA_ie_05_doa01;
exteriorS_ie_05_soto; NULLexteriorcollision; interiorS_ie_05_naka;
interiorcollisionA_ie_05C; doorcallback46b4c; NULLsecondcallback;
wallcallback46b7c; NPClist46bec; NULLobject/item lists;
opencallback46c08; closecallback46c40. This is house05, not Trouble house04.

| Original list | File | Ordered names then NULL |
| --- | --- | --- |
| door_data_05_npc | 46bec/10 | 盗賊団親分,盗賊団１,盗賊団２,NULL |
| door_data_05_npc_01 | 46bfc/c | 盗賊団親分,盗賊団１,NULL |

Q1 start removes盗賊団２ from future door-scoped NPC operations by assigning
_npc_01; Q1 end restores _npc. It does not itself toggle actor visibility.
npc_hide_onoff800e8090/a4 resolves record/hide arguments, reads record+34,
iterates the NULL-terminated names, looks each up through
npcNameToPtr_NoAssert80049934, skips missing names, and sets mainflag800 for
nonzerohide or clears800 for zero. Thus changing the list controls later
open/close operations on actual named actors; replacing it with a generic
door-open boolean or setting it on a static unrelated field-door record does
not reproduce that ownership.

## Actionable provider boundary and evidence

Need original named model/pose assets and actual room actor registrations;
init flags/positions/talk ownership; persistent scripted current/previous-root
writes; a house05 mutable door-record owner created by the original installer;
the actual GW11 binding, source NPC-list addresses and source door visibility
consumers. These providers must exist before acknowledging Q1/Q4 effects.
Current Trouble board/counter installer is separate from the full gor_02
installer which creates GW11. No fabricated success for zero GW11/missing NPC.

Fresh ignored reports: artifacts/trouble-child-installer-events.json,
trouble-child-npc-events.json,trouble-child-doorentry-event.json;
complete helper listings trouble-npc-setposition.txt,trouble-door-setparam.txt,
trouble-child-doorcopy.elf,trouble-child-npchide.txt. Source rel/gor.rel SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
No source assets, native code, shared indexes or progress files changed.
