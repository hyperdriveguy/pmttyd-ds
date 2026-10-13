# Lecture field actor visibility and required battle backend

US G8ME01 disc0 revision0, read-only static DOL/gor source evidence. Scoped
follow-up to [OW-ROAD-01](../../requests/OW-ROAD-01.md) and
[AC-01](../../requests/AC-01.md). Use with the existing
[lecture field graph](LECTURE_FIELD_ENTRY_AND_RETURN.md),
[battle helper identities](CHRISTINE_HELPER_ROLES.md),
[coordinator modes](LECTURE_COORDINATOR_MODES.md) and
[player attack graph](LECTURE_PLAYER_ATTACK_FLOW.md).
This binding map does not prove original battle execution or full backend parity.

## NPC mask0x80 has a concrete draw admission consumer

`evt_npc_flag_onoff` DOL0x800500b0,size0x9c resolves(onoff,actor,mask) through
INTEGER event values and evtNpcNameToPtr. For onoff1 it ORs mask intoNPC+0;
for0 it AND-clears that mask; other values write nothing. Return2.

`npcMain` 0x80049a7c,size0x19a4 at0x8004ae08 testsNPC+0 mask0x80;
set branches to0x8004b080, skipping the ordinary npcDisp/npcDisp_xlu
registrations at0x8004ae2c..0x8004aeac. Thus the lecture conversion's
set0x8280 hides the fieldNPC from these draws, and clear0x80 re-admits it.
This does not mean all NPC updates/collisions/events stop or every shadow
service is removed. NPC main still has its independent enabled/event gates.

Mask0x8000 in the same combined request has a distinct consumer at
0x80049b14..0x80049b44: it sets or clears animation-poseflags0x100, the
recovered [camera-facing snap flag](../display/ANIMATION_INNER_DRAW_FACING.md).
Do not merge0x8000 with hide0x80 into a single boolean. Combined0x8280 also
contains0x200; its full semantics remain an explicit dependency here.

## Goombella NPC hop does not request a partner jump pose

Original Goombella NPC tribe at0x803316d0 names c_pkr, initial/stay pose
PKR_S_1 (tribe+8/+10), talkPKR_T_1(+14), walkPKR_W_1(+18),
runPKR_R_1(+1c), damagePKR_D_1(+20/+24). evt_npc_set_tribe installs the
initial pose through its source selector. These are fieldNPC bindings, separate
from the ordinary partner's PKR_J_1A/PKR_J_1B selectors.

`evt_npc_jump_position_nohit` 0x80051868 performs the previously
[recovered clock/parabola movement](PROFESSOR_ITEM_POSE_JUMP_AND_TURNS.md)
and configured jump/landing sounds. It contains **no animation selection**.
Lecture field script requests no explicit jump-pose callback either. Therefore
retain original NPC pose ownership rather than invent a PKR_J_* request.
The local NPC auto-pose branch0x80049fbc..0x8004a160 requiresNPCflag0x400
and positive motion-speed field+1a4, then selects stay/walk/run from horizontal
XZ displacement; it does not select an airborne pose from a Y-only hop.
Other parallel pose writers still require their own admission evidence.

## Exact accepted-lesson field conversion and restoration

At battle label50 the source event:

1. Reads party0 XYZ; sets Goombella NPC0x8280.
2. Reads party0 XYZ again; positions the NPC there.
3. WAIT_FRAME1; clearsNPC0x80; calls evt_mario_kill_party(0).
4. Requests NPC no-hit hop(targetXYZ,200ms,height20), clearsNPC0x8000.
5. Prints Goombella mac_0_084_06, selects Professor battle-info3,
   battle_start,wait_battle_end.

The repeated position read and one-frame visible-owner overlap are source
requests, not permission to simplify into a single copied sprite. Killing
party0 and clearingNPC flags are independent lifetime/presentation owners.

[Field party callbacks](../partner/FIELD_PARTY_EVENT_CALLBACKS.md) establish
raw killslot0 reads signedMario+245; nonnegative calls partyKill(that index).
[Destruction](../partner/FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md) saves the
linked kind toMario+247, writesMario+245=0xff,clears pointer-table entry,
releases animation pose and frees the object. This differs from clear(1)'s
partyKill2 and saved-kind0 behavior.

After field wait accepts sequence2 plus nonzero battle result, the lecture
clearsNPC0x8000, calls evt_mario_set_prev_party_pos(0,LV0,LV1,LV2), hides
Goombella NPC(0,-1000,0),prints Professor084_07. Previous-position callback
is first-call-only: if current slotnegative and savedkindnonzero, INTEGER
resolve/truncateXYZ then partyEntryPos(savedkind,XYZ),store returnedindex.
No separate explicitcreate opcode is needed, and no invented HP/pouch reset,
reward or victory-only test belongs here. Skip and accepted-return paths join
label100,write story11,release camera500ms,type11,runparty0,keyson.

## Setup3 is a distinct teaching battle

Original setup table index3 atREL0x60efc selects list0x60de8,weight50,
group0x60ce0,unitsetup0x60cb0,lectureGoombella kind0x66fe8/raw0xb1,
stage0x60aac. Crump's opening setupindex2 instead uses Crump and a different
spawned NPC helper kind0x66d78/raw0xdd. Sharing c_pkr art does not merge roles.
The current native Crump battle harness cannot establish setup3 correctness.

The minimal source teaching graph requires these concrete owners:

| Owner | Required source behavior |
| --- | --- |
| Entry/init | LectureGoombella init67160 installs Wait/Phase/Attack/Damage/Confuse; spawns Frankly kind67fb8 viaentry67130; G.work0=P.ID,P.work0=G.ID; registersP atBattleWork+424/selector-6 |
| Lecture state | _set_lecture_mode11930 sets Mario unit+104 HP-skip mask40000000 andW+2740 status suppression; coordinatorinit682b8 setswork1=0,moveCount0,wait hooks |
| Exact phase dispatch | Coordinator phase68378 snapshots modes0/2/4/6/8/9 and requires exact phases04000001/04000003; preserve original messages/rotations/window requests |
| Initial Jump | Mode0 writes1; defaultJump weapon substituted through flags09 anddata44; player Jump68e48 prints source001/002, polls actualA,TimingA tutorial result route; commoncleanup writes1→2 |
| Guard lesson | Phase mode2 writes3 and prepares original demonstration; Gattack674b4 runsheadbonkchild67680 with sourceprivate-pad injection; completion writes3→4 |
| Hammer lesson | Phase mode4 writes5; defaultHammer replaced viaflags0a/data45; playerHammer699c4 preserves held-left/start/rehold/release polling and retry-event ownership; cleanup5→6 |
| B lesson | Phase mode6 writes7; headbonk injection usesB; actual counterdata672c0 completion writes7→8; ordinary attack wrapper has no7→8 fallback |
| Practice/exit | Phase mode8 writes9 and exact018 choice; mode9 reports work2/work3 feedback; sourceenddata46→6a3f0 prints024 andsetsbattleflag20; result/teardown/field-return owners complete exit |

These roles already have detailed static contracts at the links above. Native
bindings still need battle selector IDs/work storage,unit spawn/hooks,eventphase
scheduling,defaultweapon substitution,actual pad/AC injection owners,damage and
counter dispatch,original messages/selection/audio/window/camera presentation,
exit controller and resource restoration. Showing an ordinary fight with Crump
or incrementing mode after a generic button press does not implement this graph.

At an unimplemented source battle_start, retain the blocked callback/story10
ownership while adding the backend. Do not dispatch the unrelated harness or
fabricate a successful battle result. The original DECLINE branch is different:
it legitimately skips battle and reachesstory11 through its own choice graph.
No original command says accepting the lesson may silently behave as declining.

## Reproduction and remaining evidence

Disassembled evt_npc_flag_onoff/npcMain directly with tools/inspect_dol_function.py;
tribe model/pose pointers read from DOL; lecture event/import graph from source
gor.rel. Existing tools/verify_lecture_field_flow.py,
verify_field_lecture_callbacks.py and documented lecture/helper verifiers pin
their specific graphs, not full native execution. No native code or source
payload changes. Remaining work is binding the real teaching backend and
verifying accepted/declined/return/reload behavior against original runtime.
