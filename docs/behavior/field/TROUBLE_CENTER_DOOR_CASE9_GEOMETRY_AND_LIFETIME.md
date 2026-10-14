# Original Trouble Center door, case9 geometry and lifetime

US G8ME01 disc0 revision0; read-only ISO/DOL/REL recovery2026-10-14 for
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[ITEM-01](../../requests/ITEM-01.md). Supplements
[board event/controller](TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md),
[original same-map door services](../overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md)
and [wall/contact ownership](../overworld/OVERWORLD_ROAD_POSITION_AND_CONTACT.md).
Static original requests and fresh geometry inspection, not measured original
playback or bit-identical hit/matrix execution. Addresses/offsets/masks hex;
coordinates/counts/IDs decimal.

## Same-map original front door and story lock

Trouble Center remains in gor_02. Original door_data_04 at gor.rel469b8/48
has header words0,3,1 and these relocated fields:

| Offset | Original value |
| --- | --- |
| 0c | S_ie_04_doa |
| 10 | A_ie_04_doa02, inside/close trigger |
| 14 | A_ie_04_doa01, outside/open trigger |
| 18 | S_ie_04_soto, exterior display |
| 1c | null exterior collision |
| 20 | S_ie_04_naka, interior display |
| 24 | A_ie_04C, interior collision subtree |
| 28 | evt_door_04_00,file46808 |
| 2c | evt_door_04_01,file46838 |
| 30 | evt_wall_04,file46868 |
| 34 | door_data_04_npc,file469a0 |
| 38,3c | null map-object/item lists |
| 40 | evt_door_04_open,file46900 |
| 44 | evt_door_04_close,file4694c |

NPC list469a0 points to source name bytes at196c8, annotated盗賊団３,
then null. It is a source actor identity, not permission to invent an NPC.
The separate rear door_data_04_ura46b04 shares interior/collision/NPC groups
but uses S_ie_04_doa_02 and triggersA_ie_04_doa_02_01/_02_02, reverse front/
back naming; its item list469a8 contains item_star4,item_star5,item_star6,null.
Native front-door binding must not silently import rear-door item ownership.

`gor_02_init_evt` at4d874 tests original signed story GSW0<23 (opcode26).
True: hide S_ie_04_naka with mapobj_flag_onoff(1,1,name,1),disable
A_ie_04C with hitobj_onoff(name,1,0),register
case(9,1,A_ie_04_doa01,0,evt_door_04_junbi,0).
Otherwise setLW0=door_data_04 and RUN_CHILD evt_door_entry.
The lock notice event46a00/50 is KeyOff→toge(1,0,0,0)→
print(0,gor_02_035,0,0)→KeyOn→return/end. This guard is door access,
not the quest-list story producer or a reason to invent a board interaction
outside the building. Original notice US text remains unchanged.

Later init calls gor_irai_init at4dc40 outside this closed story branch.
That registers board then counter cases by authored collision names:
(9,1,A_i04_keizi,0,gor_irai_main,0),
(9,1,A_i04_kunter,0,gor_irai_tenshu_main,0).
They are map-object cases, not NPC proximity talk or a new indoor map transition.
No native interior mask value or saved-source switch is assigned by this graph.

## Front-door animation, collision and cutaway callbacks

Use generic type3 door choreography linked above, retaining its actual callback
slots instead of substituting a simple hinge. Local callbacks:

- evt_door_04_00: multiplyLW0 by tagged0.599609375,then
  mapobj_rotate(1,S_ie_04_doa,0,LW0,0).
- evt_door_04_01: multiplyLW0 by tagged.5,then
  mapobj_rotate(1,S_ie_04_doa,LW0,0,0).
- evt_wall_04: whenLW6==0 clear mapflag1000 onS_i04_msk;
  copyLW0 toLW2/LW3,multiplyLW2 by.5 andLW3 by-.5,then
  mapobj_rotate(1,S_ie_04_B,LW2,0,0),followed by
  mapobj_rotate(1,S_ie_04_ura_B,LW3,0,0).

Source tagged multipliersf24a7ce6/f24a7c80/f24a7880 decode via original event
float representation; .599609375 must not be described as measured. Reverse
wall callback46ab0 performs the same copies/multiplies but rotates only
S_ie_04_ura_B and has no initial mask request.

Front open callback RUN_EVT3ec20,disable hitpolySurface1317, BGMOff4800,
EnvOff4800,EnvLPF(0,800),return. That independent child disables collision
A_kabe,A_saku,A_kmn,A_gomi,A_l_ura_gomi in that order.
Close RUN_EVT3ec8c,enablespolySurface1317,BGMOn(a0,null),EnvOn(a0,null),
EnvLPF(0,0),return. Its child enables those five groups in the same order,
then story>=10 disablesA_saku_04; GSF445==1 disablesA_center_kabe_atr;
localLWflag10==1 independently disables that center group again.
Child and parent requests are concurrent source events, not flattened instant
flag mutations with assumed ordering across them.

Generic entry initially hides interior display,disables interior hit subtree
and hides its scoped NPC list,then registers inside/outside case9 events.
Generic open clears interior hidden1,enables collision/shows NPCs, manipulates
source exterior/interior/door800/1000 flags and invokes these callbacks.
Generic close restores source display/collision/NPC state and scoped outer
collision callbacks. Complete dark/light/material/rotation/camera/party/audio
execution remains required. A binary native cutaway can be a declared adapter;
it is not proof of these actual source visibility/material operations.

## Exact case9 contact/A/NPC order

Complete caseMain8006e5ec/620 FIRST captures Mario pointer,requests party IDs/
party pointer,then snapshots contact words1e8,1fc,1ec,1e4,1f0,200,1f8 and
unsigned trigger half24c. Only afterward test global gates in order:
SystemLevel==0,winCheck==0,GP14==0,N_fbatPreventMarioEventChk==0,
marioCaseEventValidChk!=0. Its documented eligibility excludes motion24 and
requires Vivian status0. Window gate is exactwinCheck, not any visible UI.

Retained A halfword is zeroed unless current motion0/1/2 or mounted state1;
retain onlybit100. Scan active case records in pool order, dispatch type9 to
8006e8fc. Require SNAPSHOTTED Mariofrontcontact1e4 nonnull; fresh
fbatNpcTalkCheck()==0; hitGetName(capturedcontact) equals registeredname;
retainedA!=0. Then OR transientcaseflag2. There is no new door/board distance,
center-radius, current rendered billboard bounds or camera-facing shortcut.

fbatNpcTalkCheck8004662c/60 selects field/battle NPC work through liveGP14,
reads candidate+10;if nonnull and marioChkTalkable()!=0 returns that candidate;
else0. Thus an eligible NPC talk candidate blocks the board even when board
geometry also matches. This check is fresh per matching case and follows
current NPC owner scheduling, not a permanently cached visual-nearest NPC.

Source outer order is EVT→Mario→mobj→NPC→caseMain→icon/audio/window/effect.
Mario's current motion/contact/input and NPC candidacy therefore precede case
admission; new case events are allocated after that outer pass's EVT visit.
The event body ordinarily begins on a later EVT visit. Preserve trigger24c,
forward probe1e4 and current NPC talk owner rather than repolling A in drawing.

## Case registration and retained event lifetime

`evt_run_case_evt`80070110/d8 resolves type,arg1,name via evtGetValue,
arg3 via evtGetFloat,arg4 via evtGetValue. Nonzeroarg1 ORs8000 into U16type;
work descriptor stores name,arg3 callback/float bits and copies16 callerLW
words. It stores eventscript pointer atdescriptor54 and event priority0 at58.
caseEntry8006ec28/100 finds first inactive160-byte slot,clears it,stores its
index48,type4 without8000,flag4 when8000 was present,active1, copiedname8,
arg3 bits4c,callback50,script98,priority9c,LWcopy54..93,eventID94=0.
It returns slot index; nonzeroarg5 publishes it through evtSetValue. Board/
counterarg5 is0,so no source output write. Constructor has no full-pool refusal
and no name deduplication; native capacity admission remains its own finite
boundary. caseInit requests64 records, not a dynamic per-board list.

After FIRST mark scan, SECOND pool-order scan visits active+marked records.
evtCheckID(case.eventID94);dead→zero94. Only zero94 requests
`evtEntry(script98,priority9c,0x20)`,writes case index48 to event16c,captures new
eventID15c to case94,copies16savedLW to event9c. Only priority20 calls
npcStopForEvent. **Board/counter wrapper sets priority0, so they do not perform
that priority20 NPC stop.** Root evtEntry supplies actual event typeef; see
[complete constructor ABI](../event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md).
Earlier roads/interior prose calling this case field eventtype20 is superseded
by that ABI and the fresh call-site inspection here. Clear transient2 even when a live owner prevented
restarting. Cases remain active after acceptance/cancel/menu event completion;
no automatic one-shot deletion or invented cooldown appears here.

caseDelete8006ec0c/1c clears active1 in the indexedslot only. caseReInit
8006ed28/38 clears active1 in every configuredrecord. They do not themselves
kill the saved event ID,zero all fields or remove unrelated records.
Map/scene reset and EVT teardown callers remain separate lifetime owners.

## Fresh original geometry and source units

Read `m/gor_02/d` from ISOoffset991830984,size1513698,SHA256
 d738bff28f92ed2a2d0658fb836b0a6e91b915a32553897b47032febe2f62353.
Fresh static conversion uses existing original node hierarchy and source-order
HEAD triangles,not previously saved native summaries. Node identity is file
node offset minus header32. Board/counter are children ofA_ie_04C/A_i04_kmnC,
respectivelyA_i04_keiziban andA_i04_asen. Each has one collision mesh part.

| Collision | Source node | Approximate engine bbox midpoint | First normal |
| --- | --- | --- | --- |
| A_i04_keizi | 12d518 | 241.26309,84.00012,-91.36853 | 0,0,+1 |
| A_i04_kunter | 12d3e0 | 160.00000,79.98331,-81.36851 | 0,0,+1 |
| A_ie_04_doa01 | 12d8c8 | 250.43804,29.45613,44.16446 | 0,0,+1 |
| A_ie_04_doa02 | 12d930 | 250.43804,29.45613,44.16446 | 0,0,-1 |

These are composed static transform inspections using hostnumeric conversion,
not bit-identical runtimePS matrix assertions. Authored map unit→engine10;
existing native conversion→.1map units=.01engine units. Interior collision
rootA_ie_04C node12d108 owns the board/counter subtree and must be enabled/
disabled with the original door state, covering wall/ground/head queries.
Do not use the professor's separateA_ie_02C or its native mask2.

Source type3 door destination is hitObjGetPos+40*firstnormal inXZ,
Y unchanged; truncate to signed EVT integers before native scaling.
Finite computed front destinations are outside(250,29,84),inside(250,29,4).
These points are door choreography targets. Board candidacy remains a source
forward-contact query, not proximity to its bbox midpoint. Animation/live-hit
updates, exact probe geometry and first-triangle source transforms remain
separate runtime services.

## Validation and remaining binding

All temporary assets/reports remain ignored under project artifacts:
trouble-gor.rel,trouble-door-events.json,trouble-door-callbacks.json,
trouble-door-cutaway.json,trouble-caseMain/Entry/Delete/ReInit/Init and wrapper
inspectionELFs/listings,trouble-fbatNpcTalkCheck,trouble-map-geometry and
trouble-board-geometry.json. Relative links verified; no shared/code/index/
progress/inbox files or user emulator profiles/saves changed.
REL sourceSHA d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.

Remaining: native actual door/case installers and finite collision/name registry;
source rotation/material/cutaway/dark/party/camera/audio services; exact Mario
forward/input/NPC eligibility bindings and map teardown; full Trouble Center
lecture/confirmation/save banks; matched original event/raster/hardware/performance
validation. This is evidence sufficient to define finite native admission and
requested ownership, not an assertion that gameplay binding is complete.
