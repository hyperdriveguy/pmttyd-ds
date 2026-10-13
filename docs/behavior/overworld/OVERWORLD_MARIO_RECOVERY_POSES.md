# Mario water, void and bounded pipe pose ownership

US G8ME01 revision0, original read-only DOL and PMANIM assets. Native-usable
closure is normal Mario (`+3c=0`) water motion31 and bottomless motion33, with
main80000000/display2 pose locks clear. This documents requests and presentation,
not an independent reimplementation of the recovery movement controller.
Existing [ordinary selection](OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md),
[clock](OVERWORLD_MARIO_ANIMATION_CLOCK_AND_LOOPS.md),
[sparse updates](OVERWORLD_MARIO_ANIMATION_UPDATES_AND_INTERPOLATION.md) and
[bottomless screen probe](OVERWORLD_BOTTOMLESS_SCREEN_START_PROBE.md) remain the
shared owners. Pipe entry is a bounded resource contract; its complete movement,
linked paper rendering and exit lifecycle remain open.

## Water and bottomless request schedule

| Owner / event | Literal request | Source site | State after callback |
| --- | --- | --- | --- |
| `mot_damage`31 pending-entry bit1 | `marioChgEvtPose("M_F_1")` | 8013aeac–b0, literal80422ba4 | entry phase0 starts in same callback; no repeated pose request during sink/rise/travel/fall |
| water phase30 admitted ground contact | `marioChgPose("M_D_2")` | 8013b4f4–f8, literal80422b90 | supported landing sets phase31/count48=50, then decrements immediately to49 |
| water phase31 countdown expires, HP after penalty >0 | `marioChgPose("M_U_3")` | 8013b5a0–a4, literal80422b98 | phase32/count12 decrements immediately to11 |
| `mot_bottomless`33 pending-entry bit1 | **no pose request** | 80091940–80091b14 | preceding pose/resource retained during screen placement and fall |
| bottomless phase10 admitted ground contact | `marioChgPose("M_D_2")` | 80091cbc–c0, literal804209dc | phase11/count50 decrements immediately to49 |
| bottomless phase11 countdown expires, HP after penalty >0 | `marioChgPose("M_U_3")` | 80091d38–3c, literal804209e4 | phase12/count12 decrements immediately to11 |

The water D2 call follows support/rootY stores8013b4c8–d8. Bottomless D2 call
precedes its support/rootY stores80091cd4–e4. Both use the source admitted ground
contact, not a fabricated timer-only landing. The clocks of nonloop D2/U3 reach
their last keys and hold while countdown continues.

Water ordinary Mario subtracts1 HP at8013b570–74, then reads live HP. Bottomless
subtracts1 at80091d14–18 and reads HP atd1c. HP<=0 branches to recovery terminal
phase33(water) /13(bottomless), restores camera publication, and **does not**
request U3 or an idle/death replacement here. D2 remains selected until another
owner requests a pose. Character2 alternate requests are outside this contract.

Water phase32 expiry requests motion0 (`marioChgMot`,8013b5c8–cc), restores camera
publication atb5d0, then `npcStartForEvent` atb5d4. No direct S1 request occurs.
Bottomless phase12 expiry with main10000000 clear directly changes current motion
to0, records previous motion, calls its post callback, and queues entry bit1 at
80091dac–dec; UpdateCamPos follows atdf0. With that main lock set it retains
recovery motion. Prior motion16/17 transformation cleanup at80091d70–a8 is not a
plain Mario entry assumption. The eventual motion0 owner chooses idle from its
live HP/key/phase contract; do not insert an unconditional S1 request at recovery
completion.

## Display and frame protocol

Water entry clears display100 at8013ae78–80, calls `marioAdjustMoveDir` atae84,
and `marioNoUpdateCamPos` atae90 (sets display10000). The adjustment snaps the
ordinary display direction to the already documented0/180 target; it does not
make water display100 turn-locked. `marioOfsRotReset` atae74 copies four triples
from802c1928..1957 into Mario+bc/+b0/+98/+a4 (8005bb08–74); recovery's later
explicit+98/+a4 writes8013af2c onward still apply. Draw uses actual Mario rootXYZ
at+8c/+90/+94 (PreDisp80057a04–1c), including recovery's current rootY; a saved
floor or followY cannot replace it.

Water phase10/11 preparation sets display1 at8013b2fc–304 and selects travel
phase12 or14. Those phases retain that hide bit. Phase12 completion clears it
at8013b3ac–b8 and chooses phase13/count1; phase14 completion clears it at
8013b420–42c and chooses phase20. Both end their callbacks there. No pose change
occurs at these visibility transitions. Landing, hurt hold, positive-HP get-up,
terminal-HP and completion have no separate display100 write in these owners.
Their camera10000 guard is restored only by the documented UpdateCamPos calls;
it is not a model hide or animation pause flag.

`marioChgEvtPose`80059124–b0 rejects display2 or main80000000, and equal requested
string against current+18. A changed request stores+18, clears display40000000,
sets display10000000, and queues pending1000. **It does not zero start-frame+28.**
Water F1 therefore uses retained+28, not an invented unconditional frame0.
`marioChgPose` changed ordinary request stores+18, zeros+28, clears
**display30000000**, and queues pending1000 (80058dd8–fc). Thus landing D2 leaves
effect selection and starts at frame0; U3 similarly starts0 when changed.
Neither helper clears display1/100/10000/80000000. Equal requests do not restart.

RearAnime's initial display mask is70000004 (80057d04–28): effect10000000 bypasses
rear conversion. Effect F1 uses handle+234 even if a prior rear bit is retained.
D2/U3 have neither forward/rear conversions nor rear assets; after normal request
clears effect, conversion fails and the ordinary fallback clears rear80000000,
selecting front handle+22c. No invented F1R/D2R/U3R poses exist.

PreDisp selects effect+234 before rear+230 before front+22c. Pending1000/4000
triggers force=1 SetAnim, then SetLocalTime from retained+28 (8005793c–44,
8005798c–b8). It always calls selected `animPoseMain` at800579f4 **before** testing
display1 at579f8–7a00. Hidden travel continues the original millisecond-clock
animation update even though the actor display request is omitted. Existing
clock/loop contracts govern interpolation; motion phase and visible rendering
frames are not replacement animation clocks.

## Exact assets and finite update domain

| Pose | Original PMANIM | Loop / keys | Sparse POS / NRM / TEX / VIS / NODE |
| --- | --- | --- | --- |
| M_F_1 | a/e_mario index33, effect+234 | loop0..16; integer keys0 through16 | 86 /51 /14 /0 /335 |
| M_D_2 | a/a_mario index29, front+22c | nonloop0..8; keys0,1,2,3,4,5,6,8 | 437 /0 /6 /0 /94 |
| M_U_3 | a/a_mario index30, front+22c | nonloop0..12; keys0,1,2,3,4,5,6,8,12 | 548 /0 /8 /0 /250 |

U3 contains one node OUT outside the interpolation tangent admission and holds
that partial node update as in the existing source owner. F1 contains51 normal
updates: apply original signed sparse deltas/16 as for positions; a prior
zero-normal-update pose subset cannot justify silently discarding these.
Both file headers+44 declare texture stem `a_mario`; the original source
animPoseEntry path uses **a/a_mario-**. There is no a/e_mario- file. These files
retain separate animation/model bases and effect handle ownership despite that
shared TPL. Neither recovery pose is found in a/a_mario_r.

## Pipe29 resource boundary and remaining motion coverage

`marioMotTbl`80310030 contains41 records of three callback pointers (stride12),
and pins29→mot_dokan8014a564,31→mot_damage8013ae04,33→mot_bottomless80091904.
The manifest preserves all41 literal records/owner names; shared callback names
alone do not establish equal arguments or entry behavior.

Bounded pipe entry style+3e=1 requests `marioPaperOn("p_dokan_y")` at8014a61c,
`marioChgPaper("PM_D_1A")` at624, `marioChgPose("M_Z_1")` at62c and
PaperLightOff at630. Other documented entry branches request `p_dokan_x`,
PM_D_1A/M_Z_1 at8014a750–64 and824–38. Literals are802f45fc/802f4610,
802f4608,80423044. Styles2/3 skip these entry requests. M_Z_1 is static original
front index0; it is **not** a substitute for the linked pipe paper geometry.

PaperOn80058a90 releases existing handle+240 when nonnegative, sets it−1, sets
display40000000, allocates named paper file with type2 (80041620) into+240,
and links it to front+22c through animPosePeraOn80040e04. ChgPaper80058c68
requires that display40000000 and clear locks, compares against paper name+1c,
and on change stores+1c, zeros+28 and queues4000. PreDisp passes paper name+1c
to animPaperPoseSetAnim on front+22c with force1. Completing original pipe paper
asset decoding/link deformation, exit/unlink, transform and movement is still
required before claiming pipe rendering fidelity.

Other table owners with unresolved pose domains include4..9 NPC/switch/stand/
small jump,14talk,15item,16..17hip,18..20hammer/jabara,21..28paper/partner
transformations,30grasp,32spike damage,34reset,35shadow,36party use and37..40
Bowser/dummy. Previously closed ordinary0/1/2/3/10 movement,12stairs/13landing
and this31/33 pose slice do not close those owners' poses or admission services.

## Verification and limits

Run `python3 tools/verify_overworld_mario_recovery_poses.py`. It checks original
DOL bodies/instructions/literals/table, hashes original model files, reparses
three recovery streams and their sparse counts, audits missing rear assets,
and evaluates finite source-derived request/visibility/counter cases. No ISO,
raw model or extracted code is committed. Host cases are local contract checks,
not Dolphin execution or raster/lighting/material/audio equivalence. GX six-bit
precision remains unresolved by this pose work.
