# Rogueport ordinary road registration, contact and story owners

US G8ME01 disc0 revision0, read-only ISO DOL and gor.rel plus m/gor_00–04/d.
Static source reconstruction, not original gameplay observation or host numeric
 equivalence. `tools/verify_overworld_roads.py` checks the dedicated evidence
manifest. Addresses are virtual DOL addresses except explicit REL file offsets
and map-relative source-node identifiers. This does not implement a general EVT
VM or give permission to replace original story/text with invented transitions.

Existing lifecycle leaf evidence is in docs/MECHANICS.md under ordinary road
scripted travel. The [lecture entry](../story/LECTURE_FIELD_ENTRY_AND_RETURN.md) is a
separate story owner: these road callbacks do not universally invoke it.
This contract adds actual registration/contact identity, used map callers and
road-specific arrival/departure branches rather than duplicating those leaves.

## Shipped ordinary road records and map identities

Each record below is ROAD type0, case_type=-1, directionAUTO20000,
centerX sentinel100000, centerY/Z0, length=-1, animation tags1/1. The sentinel
means source hit-object position, not native marker-vertex average. Case_type-1
resolves to case10 at evt_bero_get_info_num80079fcc–8007a01c. Names are exact
ASCII hit-object identities. Source-node offsets are relative to map base32,
the same identifier retained as GroundContact.sourceNode by map conversion.
The listed attributes are ORIGINAL immutable map values, before event mutation.
All these mesh-owner nodes have source cull1.

| Room/source table REL offset | Hit name | Source node hex (decimal) | Original attrs | Target room/hit | Custom entry/out event |
| --- | --- | --- | --- | --- | --- |
| gor_00 0x20cd4 | n_bero | 0xd03a8 (852904) | 4 | gor_01/s_bero | none / no00_custom |
| gor_01 0x354f8 | e_bero | 0x1517c0 (1382336) | 5 | gor_02/w_bero | none / none |
| gor_01 0x35534 | w_bero | 0x1515b0 (1381808) | 5 | gor_03/e_bero | none / none |
| gor_01 0x35570 | s_bero | 0x1516f0 (1382128) | 5 | gor_00/n_bero | christine_nakama_before / none |
| gor_02 0x3ee24 | w_bero | 0x128d40 (1215808) | 5 | gor_01/e_bero | gor_02_event_00 / none |
| gor_03 0x4de80 | n_bero | 0x134340 (1262400) | 5 | gor_04/s_bero | debug_event / none |
| gor_03 0x4debc | e_bero | 0x1342d8 (1262296) | 5 | gor_01/w_bero | debug_event / none |
| gor_04 0x5d53c | s_bero | 0x120138 (1179960) | 5 | gor_03/n_bero | none / none |

Do not treat every named *_bero node as a road: gor_03/s_bero is NONE0xf00
with custom gor_03_down_evt, and gor_04/n_bero at source node0x1200d0 (1179856)
is DOOR type1, sfx8, target rsh_01_a/s_bero. Its default case is9 and includes
A/input/talk admission rather than the road case10. Pipes are distinct records
with type2/case6; not interchangeable road portals.

The five init callers set LW0 to their BeroInfo table, call evt_bero_get_info,
then RUN_CHILD_EVENT evt_bero_info_run unconditionally at the following sites:

| Room | SET / get_info / RUN_CHILD REL sites |
| --- | --- |
| gor_00 | 0x2f234 / 0x2f240 / 0x2f248 |
| gor_01 | 0x3c3b0 / 0x3c3bc / 0x3c3c4 |
| gor_02 | 0x4d5ec / 0x4d5f8 / 0x4d600 |
| gor_03 | 0x5a3e4 / 0x5a3f0 / 0x5a3f8 |
| gor_04 | 0x603f0 / 0x603fc / 0x60404 |

Unconditional registration does not remove callbacks/story or geometry-object
state. evt_bero_get_info requires each hit-object name to resolve before it
loads the array, resets 16 Bero switch entries to-1 and resets BeroEXEC0.

## Runtime collision mutation precedes case registration

bero_case_entry8030e608 iterates the table. For ROAD type0 it requests
`evt_hitobj_attr_onoff(0,1,hitName,0x00800000)` at8030e690 BEFORE the
`evt_run_case_evt_bero(caseType,1,hitName,0,bero_case_exec_road,LW15)` call
at8030e6a8. evt_hitobj_attr_onoff80074324 maps arguments0/1 to
hitObjAttrOn80017810, not group-off or removal. Thus the above nodes need a
runtime attribute OR0x00800000 overlay before the collision owner filters them.
Original4/5 have attr4; chkfilterVec normally rejects those unless00800000 is
set. The overlay also makes the ordinary front-wall wrapper pass through them
while writing contact+1f0/+1f4. See [wall policy](OVERWORLD_WALL_MOVEMENT.md).

Do not mutate packed vertices or replace source winding/cull with a circle.
Preserve source node identity through the full wall-ray provider and feed the
wrapper's actual contact token. Per-object attribute changes are runtime state.
bero_case_entry saves the case ID then enables BeroSW for the registration.
Arrival choreography registers these cases at its finish, not an arbitrary
60-tick cooldown after placement. Switch_on/off events recreate/delete saved
case registrations through the Bero switch owner; those are distinct from
BeroEXEC busy permission bits.

## Actual ordinary trigger: named front contact, not geometric proximity

ROAD default case10 dispatches to caseMain8006e940 via8030c1e0[10]. caseMain
snapshots Mario+1f0 at8006e61c. The case10 body requires this token nonnull,
resolves hitGetName(token), and strcmp-compares it with registered hitName at
8006e948–8006e95c. Equality sets case flag2 at8006e960–8006e968. There is no
radius, endpoint-distance, toward-marker dot, stick threshold, A-trigger or
road-specific cooldown in this body. Direction arises from the directional
wall/front contact geometry, not an additional road-input test.

caseMain global gates must still admit case processing: system level0,
winCheck==0, gp+14==0, N_fbatPreventMarioEventChk==0, and
marioCaseEventValidChk!=0 (8006e62c–8006e668). The last service rejects motion24
and requires vivianGetStatus==0. The ground motion0/1/2 or mounted-state check
only controls a cached A-trigger bit for OTHER case bodies; case10 doesn't use
it. A retained airborne contact can trigger road registration, after which the
road departure event explicitly waits for landing.

After marking, caseMain checks whether the case's running event ID remains
alive. It creates a new event only when that ID is0, copies saved local-work
operands, uses event type20, and requests npcStopForEvent (8006eb70–8006ebd0).
Then it clears transient case bit2. Event ID lifetime and BeroEXEC prevent
concurrent departure owners; no invented cooldown belongs in this local graph.

## Directions and position ownership

AUTO applies to ordinary road/door names using the ordered prefixes at802c2c28:
n_,ne_,e_,se_,s_,sw_,w_,nw_ →indices0..7. evt_bero_get_info_num compares
prefix strings, not mesh normals. Thus roads n/e/s/w resolve0/2/4/6 and force
direction uses index*45 degrees. Other naming/direction modes remain explicit.

bero_get_position_normal80079b74 reads source hitObjGetPos when centerX==100000.
For mode0 let outward heading H=index*45, u(H)=(sin(H),-cos(H)). The start XZ
is center-30*u; endpoint is start+(30+length)*u=center+length*u, with the source
single/FMA/trig operation order. Mode1 reverses H by180 before constructing
both. It also runs a downward source ray at the START point to set shared Y
(80079d30–80079d88); endpoint Y inherits it. This is not reciprocal endpoint
extrapolation or unconditional destination-marker Y. Native camera/source world
position transforms and the support-ray provider remain adapters.

Road length-1 resolves30 original units at evt_bero_get_info_length8007a76c;
normal entry calls mode1 with that length (8030f11c–8030f12c) and positions
Mario and both party slots at BeroS, then entry moves toward BeroE. Therefore
ordinary entry crosses60 original units from outside to inside. The movement
service uses FLOAT150 original units/s, gp frame rate, and trunc((distance+1)/
perTickRate) with final XZ assignment. At frame rate60 and unobstructed axial
60-unit travel this derives24 simulation updates; do not promote24 to a
universal delay or use 3 native units of displacement. Existing lifecycle
service evidence remains in the mechanics ledger.

Generic road departure starts from actual Mario position, waits for landing,
calls mode0 with FLOAT150 LENGTH, and moves to BeroE with FLOAT150 SPEED
(8030db30–8030db60). It then waits150ms, waits2 frames, checks whether target
map is exactly sys_01, adds1000ms only for that case, and invokes mapchange
(8030db98–8030dbdc). The eight roads above don't target sys_01.

## Used custom story callbacks must survive generic travel

Story operand is raw0xf5de0180. Keep it encoded until original persistent
namespace/save mapping is implemented. These callbacks have no new paraphrased
messages; they invoke original scene owners whose exact US text and control
tokens must remain original. A source callback returning LW0=1 handles the
transition presentation itself and suppresses its corresponding generic route.

| Callback REL offset | Used binding | Exact branch and request |
| --- | --- | --- |
| no00_custom 0x20c14 | gor_00/n_bero out | Story==5: wait_landon; set Mario flag0x100; key off; read position; Z-=80; mov_pos2(X,Z,FLOAT150); hide Mario and party1 display; shared operand0xfaa2b58a=0; LW0=1. Otherwise LW0=0. |
| christine_nakama_before 0x354b0 | gor_01/s_bero entry | Story!=6: LW0=0. Exactly6: key off, RUN_EVENT christine_nakama, LW0=1. |
| gor_02_event_00 0x3ed64 | gor_02/w_bero entry | Story!=7: LW0=0. Exactly7: key off, RUN_EVENT steel_coin, LW0=1. |
| debug_event 0x4de20 | gor_03/n_bero and e_bero entry | If system flag0x40 set, clear it; LW0=0 in all local paths. |

Arrival evt_bero_mario_enter invokes custom entry at8030f2f8; LW0==1 enables
input and jumps to common label100, bypassing generic road movement. Generic
path chooses type-specific entry, then common tail resumes NPC event state,
registers cases, clears BeroEXEC bit1 and sets bit0x10000.
Departure bero_case_init_check invokes custom out at8030d9b0; LW0==1 completes
its handled route and returns1. bero_case_exec_road temporarily enables input
then returns on that result at8030dae8–8030daf4, skipping generic departure and
mapchange. Generic inline completion sets BeroEXEC0x10000 and clears bit1;
the parent separately sets bit8 after its150ms wait. The wrapper uses argument0
to OR and nonzero to AND-clear; see the
[landing/concurrency addendum](OVERWORLD_ROAD_LANDING_WAIT.md).
Do not treat no00_custom story5 as a simple permission to jump to gor_01.

The road-local callbacks skip story6/7 custom arrival at story11/12, but this
is not a claim excluding other map initialization/cutscene owners. In particular
LECTURE_FIELD_ENTRY_AND_RETURN has gor_02 init story10 lecture and other guards.

## Adjacent train door gate and bounded native replacement

The station's n_bero is a door to rsh_01_a, not another ordinary road. gor_04
init: entering n_bero writes raw flag0xf840697a=1 at0x6054c (plus other train
flags); another entry writes it0, except resha_return writes it1 and starts train
return events. Init enables n_bero case when resha_return, or when that raw flag
is1, and disables it otherwise at0x605c0–0x60630. Preserve this train-story gate;
ordinary gor_03↔gor_04 road movement doesn't authorize train-door travel.

Native can replace radius/dot/cooldown for the eight plain road records with
an enabled registered case consuming the original wall-front contact identity,
runtime attribute overlay, explicit source route table, and BeroEXEC/event
lifetime ownership. Entry placement uses the actual destination source hit-object
center/direction and initial-position floor ray, not reciprocal extrapolation.
The normal scene slice can bind generic travel only after excluding the exact
custom callbacks above using persistent source story state.

Still required: persistent flag namespaces/save writers; map hit-object pose,
center and attribute mutation services; exact contact cache lifetime; EVT local
work, RUN_CHILD/RUN_EVENT/brother and event-ID scheduling; control/system/window/
NPC prevention service composition; custom scene actors/dialogue/event execution;
party restore/travel modes; camera/fade/sequencer/mapchange teardown. These are
named dependencies, not permission to substitute fabricated gates or dialogue.

## Reproduction and source-derived acceptance targets

Run `python3 tools/verify_overworld_roads.py`. The current manifest pins25 DOL
body/event spans,40 instruction/data anchors,14 gor REL event/table spans,
83 scoped self/DOL ADDR32 bindings, five whole original map files and10 named
map-node headers/draw records. No source executable, map asset or dialogue payload
is committed by this evidence package. It only reads the original ISO.

Source-derived native cases: contact to the correct enabled road node selects
its exact route even without A; a different node/no contact does not; original
attribute4/5 becomes00800004/00800005 only through registration; arrival case
registration remains deferred until the entry owner completes; generic travel
is excluded for harbor-out story5, plaza-south-arrival story6 and east-west-
arrival story7; station north remains the separate gated train door. These are
acceptance targets for a native binding, not original runtime traces or proof
that missing script/player/object adapters have been implemented.
