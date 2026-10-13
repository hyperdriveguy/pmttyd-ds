# Ordinary NPC talk candidate and A admission

US G8ME01 disc0 revision0; read-only static DOL evidence, scoped partial answer
for [OW-INTERACT-01](../../requests/OW-INTERACT-01.md). This is a bounded
ordinary branch of fbatEncountCheck0x8004727c,size0xb80; full encounter/event
construction remains separate. Source predicates must not become fabricated
story dialogue or conditions.

## Outer admission and ownership

fbatEncountCheck selects the ordinary/alternate NPC work via GP+14; signed
count+4,array+c,stride0x340. At entry it clears work+10 (talk candidate).
It skips work when marioStGetSystemLevel!=0; when work+8 bit1 is clear it
also skips on marioKeyOffChk or marioCtrlOffChk. Item pickup check runs next;
a nonzero N_itemPickUpFromFieldCheck skips encounters/talk. Then
fbatHitCheck(0x13f1,fieldBattleWork+10) handles preceding encounter/first-strike
construction. The ordinary talk branch0x80047848 is reached after that prior
encounter path does not commit. It requires marioChkTalkable!=0 and Mario
signed byte39==0. This ordering is source-derived, not native menu/proximity
behavior.

`marioChkTalkable`0x80094830,size0xbc returns0 when active party byte31==4
and partyflags0x100 set. Otherwise ordinary admission needs Mario motion+2e
in0..2, Mario+0c bit1 clear, marioBgmodeChk!=1, Mario+4 mask0x01000000 clear;
finally nonzero player-control-mode helper0x801865b8 forces0. It reads no A
trigger or candidate distance. `fbatNpcTalkCheck`0x8004662c,size0x60 returns
work+10 only when nonnull AND this talkable query succeeds; otherwise0.

## Ordinary dimensions, heading and NPC flags publication

`marioInit`0x8005b5c8 loads width20 from0x80420088 and height37 from
0x804200c0, writes Mario+1b8=20 at0x8005b72c,+1bc=37 at0x8005b734,
and+1c0=20 at0x8005b774. These are source initialization values, not fixed
values for every later transformation/pose state. The talk query reads the
current dimensions; ordinary Chu/Frankly width30 therefore gives radius35
original units (native*.01=.35), height intervals37 and30 respectively.

`npcEntry`0x8004b894 clears the full832-byte record at0x8004b944, then
writes flags3 at0x8004b950: active1 and query-admission2 are published together.
`evt_npc_setup` later ORs source registration flags (selected ordinary rows
0x80600). Do not infer mask2 from talkEvent installation alone or leave it clear
because a native slot only has active1. Presence of talkEvent+128 remains an
independent predicate.

Camera4+114 is source viewYaw produced by
angleABf(eyeX+0c,eyeZ+14,targetX+18,targetZ+20), written by cam3dMain
0x8000f150..164. The system compass is -Z0,+X90,+Z180,-X270; unchanged
native eye-to-target inputs can produce this source degree orientation.
The exact system table approximation is retained in
[dimensions/facing](../display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md).
[sincosf](FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md) returns sin first and
negativecos second, yielding ray direction(sinHeading,0,-cosHeading).
Use current viewYaw in the scan, not a fabricated constant0 or reversed
camera-to-eye heading. MarioDispDir is its separate live display-angle owner.

## Ordinary candidate scan0x80047874..47c98

Candidates require NPC flags+0 mask2 set,mask8 clear,talkEvent+128 nonnull.
No all-purpose native visible/notvisible rule should replace this source
admission; per-scene actor lifetime is a separate owner.

Vertical overlap is inclusive: reject NPCrootY>MarioRootY+MarioHeight, or
MarioRootY>NPCrootY+NPCheight. Horizontal distance is PSVECDistance with Mario
XZ at NPCrootY, eliminating vertical distance after that gate. Radius is
`FMA(0.5,MarioWidth+NPCwidth,10)`; distance equality is admitted.

Before facing/occlusion checks, an admitted radial actor with distance less
than the current candidate distance assigns a separate near-actor temporary
r25. That distance bound changes only after complete candidate admission;
r25 is not an independent nearest reduction. At scan end r25 writesMario+1d8,
separately from work+10. Do not merge these pointer roles.

Directional admission tests up to three rays from MarioXZ: NPC centerXZ,
then NPCX+0.5*width with sameZ, then NPCX-0.5*width with sameZ. Each ray uses
system angleABf, subtracts current camera4 facing+114, normalizes. Compare
against normalized(MarioDispDir-camera4Facing): ray<180 admits when Mario
relative angle<=180; ray>=180 admits when Mario relative angle>=180.
Any of the three admits; exact180 boundaries are asymmetric. This is a
camera-relative half-plane test with the NPC's width span, not a authored
30-degree cone or unconditional distance-only query.

After facing admission recompute horizontal PSVECDistance and require<=radius.
Compute system heading MarioXZ→NPCXZ, call sincosf, then derive true horizontal
norm from deltaXZ through original three-refinement root. Request
hitCheckFilter(scope0,origin MarioXYZ withY=rootY+0.75*MarioHeight,
sinHeading,vertical0,cosHeading,length=horizontalNorm,output pointers).
Nonzero hit rejects talk: this is map occlusion between actors. Original
filter attributes/backend/math are dependencies; do not ignore walls.

No-hit candidates compete on **strictly smaller distance**, initialized1000;
first registry candidate wins equal-distance ties. Store chosen NPC at work+10.
This differs from the partner query's last-active-horizontal-match behavior.
The same branch also owns nearby actor auto-turn/event-stop flags; their source
lifecycle must be preserved separately when binding the complete dispatcher.

## A event transition and priority against jump

After scan, write r25 toMario+1d8. If chosen work+10 exists, query
keyGetButtonTrg(0), admit mask0x100 (A). Require fieldBattleWork halfword+0==1;
otherwise exit this owner. Commit state2,byte+2=0, actorpointer+4=chosenNPC,
chosenNPC word+318=0; then return before later special encounter checks.
There is no requirement to wait for a new nearest query after A, and no
story-state write in this ordinary admission block.

npcMain's field dispatcher invokes fbatEncountCheck in fieldstate1 and
fbatTalkMode in states2/3. Fresh field-root inspection pins local owner order:
marioStMain0x8006f904 calls camMain at0x8006f944,EVT main0x8006f954,
Mario main0x8006f95c,mobjMain0x8006f960,npcMain0x8006f964,caseMain0x8006f97c.
NPC talk candidate and actor collision response therefore run after this
root's current Mario movement/motion query; a following Mario jump query can
read the retained candidate. Pause/retrace/event scheduling and other sequence
paths remain external, but these direct owner calls must not be reversed.
The existing [motion-arbitration contract](../overworld/OVERWORLD_MOTION_ARBITRATION.md)
records marioChkJump rejecting when fbatNpcTalkCheck is nonzero. This candidate
can therefore suppress A jump; jump does not automatically win an NPC proximity
contest. Exact event scheduling/window-return lifecycle and map-object/door
priority require the remaining callers before complete native arbitration.

## Complete local talk-event start/wait/return owner

`fbatTalkMode`0x800470dc,size0x1a0 takes the selected actor from fieldBattleWork
+4 and switches byte+2. Stage0 stops actor regular event+11c, turns actor
source ry+144 towardMario unless NPCflags0x01000000, locks Mario control and
party-switch service. Fieldstate3 starts actor event+12c; other states start
ordinary talk+128 and additionally call marioChgTalkMotion. Both use
`evtEntry(script,0,0)`, set event byte10=6, event+170=NPC, store eventID+15c
into fieldBattleWork+c and advance stage1. These actor contexts explain the
original print speaker"me" and ordinary NPC anchor resolution.

Stage1 waits until evtCheckID(fieldBattleWork+c)==0. When actor+1d4 mask
0x20000000 is clear it resumes regular event+11c and restores ry+144 from
saved+148; if mask is set the surrounding auto-turn owner retains that control.
NPCflags0x400 clearsNPC+108;flags0x200 requests current saved pose-name+2c
with restart1. Then unlock Mario/party service, call marioChgStayMotion,
set fieldstate1 and clear selected NPC-work+10. Other stage values no-op.
Event lifetime/regular-stop locks and window consumers remain dependencies of
this complete local request graph. Stage1 waits on event termination, not a
native fixed number of message pages or simulation ticks.

## Validation and limits

Fresh read-only DOL extraction of fbatEncountCheck,fbatHitCheck,
fbatNpcTalkCheck,marioChkTalkable,npcMain. Direct constants verify initial1000,
width coefficient0.5,extra10,vertical ray coefficient0.75 and180 split with
r2 base0x80427440. All inspection files remain /tmp; no original dialogue or
assets copied into source history. Documentation-only: no native test/build
or runtime/hardware claim.

Remaining: full fbatHitCheck/type/status/first-strike domains, candidate auto-
turn/stop restoration lifecycle, full event/regular-stop lifetimes surrounding recovered fbatTalkMode,
map-object/A arbitration and event scheduler/global frame ordering. Source US
IDs/attribution/branches must follow their actor talk events, not fabricated
native dialogue. [NPC collision](FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md)
owns its distinct halfwidth/height/response predicates.
