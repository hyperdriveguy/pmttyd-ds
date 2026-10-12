# Original ordinary Mario low-HP idle pose and live HP ownership

US G8ME01 revision0, source reconstruction from mot_stay800a6c50,
marioChkKey8005c47c, original a/a_mario and explicit pose conversion owners.
This closes a renderable low-HP pose and bounded request lifetime. It does not
replace the complete field controller, pouch/save services or sound backend.

## Entry request

For ordinary character3c=0, unlocked requests, main80000000 and main2000
clear, mot_stay admission is motion0c bit1 at800a6cc0–d0. The entry resets
signed counter5a=0 at800a6d5c. At800a6dd4–e58:

1. Read current HP. Exactly0 requests **M_I_S**, then sound371.
2. Otherwise read HP again. HP<=5 and marioChkKey()!=0 sets display4,
   requests **M_S_3**, and resets signed counter58=0.
3. Otherwise retain exact current name M_I_Y, or request **M_S_1**.

The bounded HP domain is ordinary nonnegative pouch HP; no invented
max-HP percentage threshold. Main80000000 takes its separate S1 branch;
main2000 requests M_U_2 with start-frame10. Ordinary ChangedPose compares
against current name18 and queues frame0/restart only on different strings,
as detailed in [clock/restart](OVERWORLD_MARIO_ANIMATION_CLOCK_AND_LOOPS.md).

## Ongoing idle refresh

This is not a per-tick threshold replacement. In the ordinary branch, every
callback increments signed16 counter5a (800a6f0c–18). At800a6f04–fc4,
refresh requires **counter5a > sysMsec2Frame(1000)**, main2000 clear, and
marioChkKey()!=0. sysMsec2Frame800059cc multiplies the millisecond argument
by the signed global rate word at its work+4 and divides by1000 with integer
truncation. For rate60 the threshold is60, first admitted callback61 after
entry reset. This call is not RNG and the comparison is strict.

On admitted refresh, live HP<=5 requests M_S_3/display4; if current name18
is not already M_S_3, reset58=0 first. This refresh does **not** separately
request M_I_S at HP0; that is the entry route. Healthy HP>5 reverts toward
S1 only when phase44==0 and counter5a!=0: reset5a=0, preserve exact M_I_Y,
otherwise request S1. Repeated same-name S3 requests do not restart its
animation. Signed narrowing/overflow and event-altered phase are outside
ordinary short idle traces; do not silently replace their service behavior.

While exact current name is M_S_3 and ChkKey remains admitted, callback
increments signed58, resets58 and start-frame28 at **58>=globalRate*4**,
requests sound225 on counter0 and sound226 at globalRate*2
(800a6fc8–7060). No motion1000 queue is written in this counter-reset block:
frame28 reset alone does not force SetLocalTime in PreDisp. Sound/service
handles remain separate dependencies.

## Key admission and HP lifetime

marioChkKey is a service predicate, not whether the player held a button.
The complete ordered predicate is:

1. Call marioGetPartyId then partyGetPtr, retain partner pointer (may be null).
2. seqGetSeq must return2; otherwise return0.
3. Signed Mario39 must equal0; otherwise return0.
4. marioShipChanging: return0 if1; otherwise call it again and return0 if2.
   Preserve two calls if the service is stateful.
5. marioRollKeyDisable must return nonzero, else return0.
6. marioSlitKeyDisable must return nonzero, else return0.
7. If retained partner pointer is null, **return1 now**; later gates are skipped.
8. christineGetStatus==1 returns0 (other values pass).
9. mario_bomhei_keychk2!=0 returns0.
10. yoshiGetStatus!=0 returns0.
11. If partner flags+0 bit100 is set and signed partner motion byte+31 equals
    5,6 or7, return0. Without that flag or those motions, continue.
12. nokonoko_holdItem!=0 returns0; otherwise return1.

These are source service results/fields at8005c48c–5a0. A portable predicate
can take these explicit inputs, including the two ship results and retained
partner fields. Native missing services/default values remain **harness
adapters**, not recovered original initialized values. Simple follower or
input-enabled state is not proof of this predicate.

pouchGetHP800d3d30 reads **signed16 current HP at pouch+70** from the
current global pouch pointer (r13+1be0); Mario pose selection does not snapshot
HP on room/pose load. pouchSetHP800d3d00 narrows to16 bits, then upper-caps
against signed maxHP+72; it does not lower-clamp. pouchAddHP800d3d3c adds
signed16 argument, narrows the result, lower-clamps0 and upper-caps to maxHP,
then returns current HP. Battle resource import/export owns separate writes:
[unit/pouch snapshot contract](../unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md).
No save/story HP inference or new harness initialization is implied here.

## Healthy long-idle phase44 and callback schedule

The [motion dispatcher](OVERWORLD_MOTION_ARBITRATION.md) calls mot_stay for
motion0 and may re-enter it in the same dispatch after a motion request. Entry
resets word phase44=0 and phase counter48=0 at800a6cf0–f4. HP requests above
occur before ordinary ground command processing and the late idle phase owner.
Counter5a increments even on the entry callback after that reset. Earlier jump,
partner/action or motion exits may prevent reaching the late phase block.
Main80000000 bypasses this ordinary refresh/phase path; main2000 exits before
late phase scheduling at800a7064–88. Actual admitted caller gates remain
those of the existing dispatcher contract.

At800a71a8–73d0, after the preceding command gates:

- If main8 or display40000000 is set: phase<=0 returns unchanged. For phase>0,
  exact S1 or S1R returns unchanged; otherwise request S1, set phase0, return.
- Otherwise HP<=5 returns before phase scheduling. For HP>5:
  - Phase0 increments48; when48>=sysMsec2Frame(50000), set phase1 and return.
    It does not request the next pose on that callback (ordinary60Hz:3000).
  - Phase1 calls **marioChgEvtPose(M_I_5)**, marioAdjustMoveDir, sound239;
    sets48=50 and phase2, then **falls through phase2 in this callback**.
  - Phase2 decrements48. At48>=0 return; at48<0 request S1, set48=100,
    phase3 and return. The phase1 callback consequently leaves48=49.
  - Phase3 decrements48. At48>=0 return; at48<0 call
    **marioChgEvtPose(M_N_5A)**, set28=0,48=0,phase4,2b4=0, sound228, return.
  - Phase4 increments2b4, caps it atsysMsec2Frame(10000). Independently
    increments48 and at48>=globalRate*2 resets48=0 and plays sound228.
    At its tail, main8 set or current HP<=5 requests S1 and sets phase0;
    otherwise retains phase4. This tail reread is distinct from the earlier
    HP gate; with stable HP<=5, scheduling exits before reaching it.

Phases outside0..4 return unchanged. This owner does not select M_I_Y;
that name is retained by the earlier exact-name checks when another owner
already supplied it. Do not invent a yawn schedule to produce it.

marioChgEvtPose80059124 skips display2 locks, main80000000 and same-name
requests. A changed request stores18, clears display40000000, sets
**display10000000** and queues1000; it does not zero28. Thus M_I_5/M_N_5A
use effect handle234/file e_mario, not front22c or rear230. Phase3 explicitly
zeros28 after that service call; phase1 does not. ChgPose(S1) clears the
special pose-selection bits through its existing ordinary request owner.
Idle post only cancels sound handles (with motion19/20 exceptions); it does
not reset44,48,58 or5a. Re-entry owns the resets listed above.

## Original pose and facing

| Front file a/a_mario | Directory index | Loop | Key times |
| --- | --- | --- | --- |
| M_S_3 | 3 | 0..120, looping | 0,28,40,50,54,60,70,80,90,100,110,120 |
| M_I_S | 37 | 0..96, looping | 0,13,26,32,34,36,40,46,54,62,64,77,78,80,84,86,96 |
| M_I_Y | 58 | 0..8, nonlooping | 0,1,2,4,5,6,8 |

S3 has276 position,117 node and5 texture sparse records; I_S has497
position,199 node and4 texture records. Both have no normal or visibility
updates. Their streams pass the existing bounded sparse decoder, including
finite fractional samples; original bases/hierarchy/bindings must be retained.
[Interpolation and buffer ownership](OVERWORLD_MARIO_ANIMATION_SPARSE_UPDATES.md)
apply, not a cropped texture or substitute billboard.

The ordinary toRearPose80056a04–bc0 comparison list includes S1 but **no
S3 or I_S**; neither has an inverse front mapping. a/a_mario_r contains no
M_S_3R or M_I_SR. In the rear sector, the missing conversion followed by
inverse miss clears display80000000 at80057f68–80, preserving the original
front name and selecting front handle22c. Front sector also clears that bit.
Use the original front geometry and ordinary draw transform even when facing
is in the rear sector; do not manufacture a rear pose or switch model files.

Effect file a/e_mario contains M_I_5 index6, nonloop0..50 (18keys),
M_I_Y index13, nonloop0..8 (7keys), and M_N_5A index21, loop0..140
(12keys). Front M_I_Y has408 POS/85NODE/6TEX, including16 gated OUT
records; its effect counterpart has409POS/87NODE/6TEX and16 gated OUT.
M_I_5 has532POS/184NODE/4TEX,2 gated OUT; M_N_5A has258POS/85NODE/2TEX.
All have no NRM/VIS streams. Exact key times/metadata are pinned in the
manifest; no broad special/effect geometry/runtime closure is implied by
these six parsed poses.

## Validation and limits

Run `python3 tools/verify_overworld_mario_low_hp_idle.py`. It pins12 source
bodies,6 name spans,402 source instruction anchors, three original file
identities, exact six-pose metadata and absence of invented rear names;
42 finite pose samples and bounded selector/threshold cases are host-derived.
It does not execute the original binary, establish Gekko arithmetic parity,
close all key/status/audio dependencies or measure original rendering.
