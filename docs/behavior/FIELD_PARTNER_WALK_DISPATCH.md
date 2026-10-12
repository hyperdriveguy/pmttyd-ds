# Partner walkMain motion and substate dispatch

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
walkMain at0x800c8568,size0x570,and eight-word table0x80315070.
This body is a **dispatcher**,not a complete position integrator. It is called
by [partyWalkMain](FIELD_PARTNER_HISTORY_WRITER.md) after history update/skip.
All addresses below are DOL virtual addresses;no original execution is claimed.

## Three separate state bytes

Read unsigned **motion-selector byte P+0x3b** once for the outer dispatch.
Selected arms then read unsigned **substate byte P+0x36**. The driver-mode
byte **P+0x34** is separate and is reread for hazard admission. Do not collapse
these into one enum merely because some numeric values coincide.

| Motion selector | Entry | Common local policy |
| --- | --- | --- |
| 0 |0x800c85b0| Consume pose marker with pose/kind5 requests;simple substate graph;angle copy,splash/countdown and hazards |
| 1 |0x800c8760| Consume marker only;eight-word substate graph;angle copy,splash/countdown and hazards |
| 2 |0x800c88dc| Consume marker only;behind/jump2 substate graph;angle copy |
| 3 |0x800c8988| Consume marker only;beside/jump2 graph;angle copy |
| 6 |0x800c8a1c| Consume marker only;continue/fly-behind/jump2 graph;angle copy |
| Others |0x800c8ac4| Return without these handler writes or calls |

The marker is **word P+8 mask4**,distinct from pending mode-change mask8.
Only selected arms consume it. Consumption clears mask4,not all word+8 flags.
Selectors2/3/6 do not run this body's splash/hazard tail.

## Selector0

When consuming marker4,request partyChgPoseId(P,1),then reread unsigned kind
byte P+0x31 and request unk_80187d74(P) if5. Dispatch substate:

| Substate | Local request |
| --- | --- |
| 0 | [party_motion_stay(P)](FIELD_PARTNER_STAY_MOTION.md),then reread kind and call unk_80187d10(P) if5 |
| 1,2,6 | Clear substate to0 and OR2 into P+8 |
| 3 | party_motion_fall(P),0x8014cbb8 |
| Others | No substate call |

The1/2/6 arm retains an additional test of its cached substate against3;
under this local dispatch it cannot match. Its conditional clears of P+0
masks0x70/0x03000000/0x04000000 are therefore not evidence of a reachable
3→0 cleanup. Substate3 selected another arm,and this test does not reread
the byte or make an intervening service call.

## Selectors1/2/3/6

Every handler below receives P. Mobility selection means unsigned byte
**P+0x32==0** versus nonzero;the caller does not validate or normalize it.

| Substate | Selector1 | Selector2 | Selector3 | Selector6 |
| --- | --- | --- | --- | --- |
| 0 |party_motion_stay|Same as its substate1|Same as its substate1|Same as its substate1|
| 1 |Mobility0:party_motion_homing_walk0x8014fd4c;otherwise homing_fly0x8014f048|Mobility0:behind_mario0x8014e8ac;otherwise behind_mario_fly0x8014defc|beside_mario0x8014da8c|Mobility0:behind_mario_continue0x8014e3dc;otherwise behind_mario_fly|
| 2 |party_motion_jump0x8014d2f0|party_motion_jump2,0x8014cf54|jump2|jump2|
| 3 |party_motion_fall0x8014cbb8|fall|fall|fall|
| 4 or5 |No call|No call|No call|No call|
| 6 |party_motion_upstairs0x8014cb28|upstairs|upstairs|upstairs|
| 7 |party_motion_damage0x8014c858|damage|damage|damage|
| Others |No call|No call|No call|No call|

Selector1 uses the pinned eight-word table:substate4/5 targets its common
continuation;substate>7 bypasses the table. Other selectors use explicit
comparisons. None recursively reruns its dispatch when a called handler changes
the outer selector/substate. Preserve the post-call reads below instead of
restarting from a newly selected state in the same invocation.

## Angle, splash and hazard continuation

All five selected outer arms copy the post-handler float **P+0x100→P+0xfc**,
including when their substate had no call. Selectors2/3/6 then return.

Selectors0/1 separately execute equivalent local splash/hazard tails at their
own instruction addresses. If word P+8 mask0x01000000 is set,consume it,
capture current P+0x58/+0x5c/+0x60 and request marioSetSplash(0,&vector).
Store byte20 at+0x4c. Always decrement the unsigned byte,store its low byte,
then clamp it to0 only when that low byte sign-extends negative. A newly
requested20 is thus19 before returning;raw128 becomes127. This is the same
arithmetic policy as the [mode1 body](FIELD_PARTNER_MODE1_SUBSTATES.md),but
it is a different caller path and does not itself request vivianShadow.

If the reread **P+0x34 mode==13**,return without hazard checks. Otherwise
check nonnull hit pointers at **+0x138,+0x12c,+0x140**,in that priority order.
For each,hitGetAttr returns the hit word+4. If mask0x800 is set,request
partyChgRunMode(P,13) and return,skipping the later pointers. Null or clear
mask continues. No local HP/FP/saved-kind write occurs here.

## Verification and remaining work

tools/verify_field_partner_executor.py pins walkMain and stay body hashes,66
instructions,nine numeric constants,five data spans (eight dispatch words,
three map IDs and one pose key) and two actual lecture stop/run imports.
Whole hashes verify local bytes,not all called motion/terrain engines.
Original listings and metadata reports remain ignored.

Next:homing/jump/fall/behind/upstairs/damage bodies,pose/splash/hit producers,
state writers and original partner movement/hazard traces. This dispatcher
contract alone cannot reproduce all partner locomotion or the complete game.
