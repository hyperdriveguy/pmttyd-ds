# Rogueport same-map interior doors

US G8ME01 disc0 revision0, static read-only DOL and `rel/gor.rel` evidence.
This delivery answers the indoor portion of [OW-ROAD-01](../../requests/OW-ROAD-01.md).
It is not a runtime observation or proof of a complete story implementation.

## Scoped native adapter

[Door asset conversion](../../../tools/door_assets.py) reads the existing local
original map summary and source-ordered HEAD triangles. It resolves the two
ground-floor source hit names, preserves the first triangle normal, applies
the40-unit offset and truncates engine XYZ before scaling for the handheld.
The ignored `doors.pmdoor` pack retains source node IDs and the complete
professor interior collision subtree. Inn's null collision field stays null.
[Portable door state](../../../include/field_door.hpp) and
[native loading](../../../source/field_door.cpp) implement exact contact+A
admission and a fixed-tick movement/wait bridge at135 units/sec. Registration
currently covers the inn ground floor and professor house. Rendering groups9/10
hide their interiors until opened; collision enable/disable applies to all
ground, wall, overhead and shadow queries. Display groups11/12 suppress the
outer subtree while inside as a provisional cutaway for the source flag2/0x800
requests; their precise source fade/material consumer is requested in the inbox.
The collision mask cache updates
only when room/door state changes.

Movement/party placement and camera publication are native adapters, not
bit-identical execution of the complete original event graph. Door mesh
rotation, light/dark effects, indoor NPC/item activation, inn upstairs callbacks,
Professor story8 first-talk and story9 wall-removal callbacks, letterboxing,
original sound macros and nested key ownership remain unbound. This adapter
does not write story words or fabricate any dialogue. Native
[version4 saves](../format/NATIVE_SAVE_FORMAT.md) restore the completed interior
mask before collision validation; saves during door choreography stay suppressed.
These limitations prevent a claim of full
prologue or original indoor scene parity.

## Room identity and source records

The plaza inn/bar and Professor Frankly's house remain within `gor_01` and
`gor_02`, respectively. Their doors use `evt_door_entry` and related same-map
services, not BERO map transitions. The `gor_01` BERO table contains only the
three ordinary roads and a terminator; `gor_02` contains its west road and pipe.
Do not invent an indoor room ID or reciprocal BERO arrival for these doors.

Each scoped door record is 72 bytes. Fields +0/+4/+8 are 0/3/1; +0c is the
animated door object; +10 the close/interior trigger; +14 the open/exterior
trigger; +18/+1c the exterior display/collision groups; +20/+24 the interior
display/collision groups; +28/+2c/+30 the animation callbacks; +34/+38/+3c NPC,
map-object and item lists; +40/+44 the open/close callbacks. Null is a meaningful
absence, not permission to substitute a guessed mesh.

| Purpose | Room / REL record | Animated object | Close hit (+10) | Open hit (+14) | Exterior display (+18) | Interior display (+20) | Interior hit (+24) |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Inn/bar ground floor | gor_01 / 0x36644 | S_ie_03_f1_doa | A_ie_03_f1_doa02 | A_ie_03_f1_doa01 | S_ie_03_soto | S_ie_03_naka | null |
| Inn upper-floor door | gor_01 / 0x36924 | S_ie_03_f21_doa | S_ie_03_f2_doa02 | S_ie_03_f2_doa01 | S_ie_03_soto | S_ie_03_naka | null |
| Professor's house | gor_02 / 0x45cc8 | S_i02_doa | A_ie_02_doa02 | A_ie_02_doa01 | S_ie_02_soto | S_ie_02_naka | A_ie_02C |

The inn record NPC list at 0x36614 includes the original inn clerk, bar master
and other indoor actors; the professor list at 0x45994 contains the professor.
These are source bindings, not newly authored actor/story text.
All three exterior collision fields (+1c) are null.

Plaza init registers inn ground/upper doors unconditionally at 0x3c748–0x3c768.
Professor init selects its record at 0x4d838 then RUN_CHILD_EVENT
`evt_door_02_entry` at 0x4d844. That specialized entry at 0x45d10 has the same
initial interior hide/disable and case registrations as the generic entry, but
sets LW15=8 for the close event and 0 for open. Inn uses LW15=0 both ways.

## Admission and default visibility

`evt_door_entry` DOL 0x8033156c (0x134 bytes) loads parameter3 (+20/+24),
calls `evt_mapobj_flag_onoff(1,1,interiorDisplay,1)` when nonnull and
`evt_hitobj_onoff(interiorHit,1,0)` when nonnull. It loads parameter1
(+0c/+10/+14) and registers case9 against each nonnull trigger. The close hit
runs `evt_door_close`; open hit runs `evt_door_open`. It hides the NPC list
when parameter9 (+34) is nonnull. Professor's specialized entry lacks this
generic final NPC hide call; retain its actual actor setup owner.

Case9 jump-table entry 0x8030c1e0[9] is 0x8006e8fc. It requires nonnull Mario
forward contact +1e4, `fbatNpcTalkCheck()==0`, exact `hitGetName` equality with
the registered hit, and admitted A bit0x100. A admission is restricted to
motion0/1/2 or mounted state1 at 0x8006e66c–0x8006e69c. Common case gates are
the same as [ordinary roads](OVERWORLD_ROGUEPORT_ROADS.md): system level0,
no window, gp+14 zero, NPC event prevention zero, and Mario case eligibility.
There is no door-specific distance/radius test in case9. Geometric contact and
its cache lifetime remain responsibilities of the source collision owner.

## Type3 movement points

`evt_door_param` 0x800e81f4 (0x3a0) parameter4 and parameter5 invoke
`door_position_sub` 0x800e884c (0x4bc), passing record type(+4), animated object,
close hit(+10), open hit(+14), two output vectors and mode0. Type3 branch is
0x800e8bcc–0x800e8cdc. Parameter5 uses the exterior/open hit; parameter4 uses
the interior/close hit. Each starts at `hitObjGetPos(hit)` and updates X/Z as
single FMA `position.axis + hitObjGetNormal(hit).axis * 40.0`; Y retains the
hit position. Missing hits use Mario's current XYZ. Parameter4/5 publish XYZ
by truncating floats toward zero to integer event variables. Do not substitute
an average of rendered vertices for the source hit-object center or normal.

`hitObjGetNormal` 0x800143c4 (0xd8) searches active hit arrays by node name,
rejecting hit flag0x80, requires source node partCount(+5c)>0, reads the first
hit triangle through hitWork+ac and normalizes its vector at triangle+48 using
PSVECNormalize. `hitCalcVtxPosition` 0x800165a8 computes that vector at
0x800166d0–0x8001670c as cross(v2-v0,v0-v1), equivalent to the ordinary
cross(v1-v0,v2-v0). Preserve first-part/first-triangle ordering and original
winding; do not average triangle normals or reverse them for render culling.

Constant40 is binary32 at 0x80421b5c; startup r2=0x80427440 and the load is
r2-22756. Mode0 constants at 0x80421b58..6c are 10/40/30/50/0/0.5; type3
does not use the separate hinge offset math of types0/1. Opening first moves
Mario to parameter5 XZ, then later moves Mario/party to parameter4 XZ. Closing
moves to parameter5 XZ. Speed operand0xf24c9680 decodes to135.0 original units
per second via `(signedOperand+230000000)/1024`. These are computed movement
targets in the current room, not map-entry spawn positions.

## Open and close event ownership

Open 0x8032f670 (0x7f0) disables letterbox changes, disables player keys and
sets DoorFLAG=1. It RUN_EVENTs the original open callback before visibility and
travel. It saves exterior display mapflags, clears exterior flag2, requests
camera parameter4 XZ with MarioY+2 using150ms blend/type11, then sets exterior
flag0x800 and door flags0x800/0x1000. It clears interior flag1, sets interior
0x800/0x1000, enables the interior collision group, shows scoped NPCs and sets
door-dark flag1. It halves party hitboxes, moves Mario to parameter5 at135,
and moves both party slots beside Mario. Its ordinary door animation uses
interpolation5 from0→180 over500ms; after WAIT500ms it calls party-door-move
to parameter4 at135; its return interpolation2 is180→0 over500ms. Subsequent
WAIT500ms, camera release(0,type11), DoorFLAG=0, WAIT2frames, keys on and
letterbox restoration finish the ordinary path. Sounds/dark effects are
separate original event calls, not generated replacements.

Open interior switches are at 0x8032f820–0x8032f8b0. Close switches are at
0x80330c70–0x80330d10: set interior flag1 unless LW15 bit1 suppresses it,
clear0x800/0x1000 and disable interior collision. Close also clears exterior
0x800 and door0x800/0x1000 and restores saved exterior mapflags.

Close 0x80330724 (0x6fc) acquires the same input/DoorFLAG ownership and
RUN_EVENTs its close callback first. Ordinary camera targets parameter5 with
MarioY+2/150ms. Its ordinary open and close rotations use interpolation5 over
500ms each, with intervening WAIT500ms. Generic party movement to parameter5
at135 is suppressed when LW15 bit8 is set. Camera release is100ms/type11;
darkflag0, NPC hide, the visibility/collision restoration above, DoorFLAG=0,
WAIT2frames, keys on and letterbox restoration finish. Brother event scheduling,
interpolator semantics, hitbox restoration, dark/light/material rendering,
audio macros and camera projection still require their own native owners.

## Mandatory source story callbacks

Professor open callback0x4599c checks raw story operand0xf5de0180 exactly8
and RUN_EVENTs `kurihakase_first_talk` at0x45a84. Its close callback0x45b98
checks exactly9 and RUN_EVENTs `kurihakase_wall_remove` at0x45c4c; story19
instead dispatches `kurihakase_lecture_badge`. Common callback tail WAIT500ms
then parameter5/party-door-move at135 explains LW15 bit8 suppression above.
Do not replace these with notices or invented dialogue. Original message IDs,
speaker order and conditions remain the scene owners' responsibility.

Inn ground-floor open callback0x36380 also manipulates upstairs door flags
0x1800, briefly hides `S_i03_f2_soto`, waits for the door end in a brother
event, starts the source floor fade events and indoor NPC ambient event.
Close0x36480 stops those events, closes message `mac_4_004`, restores door
flags/color and exterior audio filtering. The upper-floor transition and
material fade implementation is still required; the table alone cannot prove
the entire inn behaves correctly.

## Reproduction and limits

`rel/gor.rel` read directly from the ISO was compared byte-for-byte with the
local extracted research file:642564 bytes, SHA256
`d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a`.
Use `tools/rel_event_inventory.py --module gor --symbols
artifacts/reference/us_symbols.csv --output /tmp/doors.json` with
`gor_01_init_evt gor_02_init_evt evt_door_02_entry
evt_door_02_open@gor_02.o evt_door_02_close@gor_02.o
house_03_f1_open house_03_f1_close`. `tools/dol_event_inventory.py` inventories
`evt_door_entry evt_door_open evt_door_close`; `tools/inspect_dol_function.py`
disassembles `caseMain evt_door_param door_position_sub` individually.
External symbols are annotations corroborated by actual commands, self/DOL
ADDR32 relocations and machine instructions; symbolic labels alone are not
evidence. Local reports remain ignored. No original executable/dialogue bytes
are added to source history.

This scoped delivery does not establish all indoor accessibility gates, all
room NPC/item visibility or complete prologue story completion. It establishes
the two requested same-map door identities, current source binding points,
case admission, movement target algorithm and mandatory story callbacks.
