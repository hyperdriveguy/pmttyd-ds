# Original item95 Mario motion15 entry, display and stay request

US G8ME01 revision0, recovered2026-10-14 from read-only DOL and original
animation metadata. Scoped follow-up for [ITEM-01](../../requests/ITEM-01.md).
Complements [item sequence](GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md),
[field service order](GOOMBA3_ITEMGET_FIELD_SERVICE_AND_EVENT_RESUME_ORDER.md),
and [window/pose assets](ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md).
Addresses, field offsets and masks are hexadecimal unless explicitly decimal.
This pins source requests and admission; it does not execute original code.

## Get-item request and previous motion flags

marioChgGetItemMotion80094c90/100 reads current unsigned motion+2e.
Decimal22 calls800a15a8 and returns;21/25 clear base horizontal speed float+180 and return;
26 returns unchanged. Remaining motion requests obtain Mario again and return
unchanged when main word0 has10000000. These returns do not queue15.

Before replacing current motion, read **retained previous motion+30**.
When this is decimal16 or17, conditionally clear display word4 mask08000000
(at80094d10–20), then conditionally clear mask4(at80094d24–34).
The operand is previous+30, not current+2e or new15. Neither flag is
unconditionally cleared for an ordinary previous motion. The two clears use
fresh reads of word4 and precede updating the motion pair.

At80094d38–78: save current+2e to previous+30; store decimal15 to current+2e;
load previous row80310030+12*previous and call its post callback+8 if nonnull;
then reload word0c,OR1 and store. The post sees the new current motion.
There is no same-motion comparison and no immediate pose request here.
Original row803100e4 is80092a3c/0/0(main/reserved/post): motion15 has no post.

## Entry callback and exact M_I_U producer

marioMotion8009461c clears word200, dispatches current row main, and redispatches
while word0c bit1 remains set. mot_getItem80092a3c/58 obtains Mario and returns
without local mutation when pending1 is clear. With pending1 set, order is:

1. Clear pending1 in word0c.
2. Clear word48, signed-half storage50, word44, then base horizontal speed float180 to0.
3. Call marioAdjustMoveDir800965fc.
4. Call marioChgPose80058d84 with original80420a14 string **M_I_U**.

There is no local countdown, automatic StayMotion, ongoing pose re-request,
item pointer or item95-specific branch in this callback. Word48 is a motion
counter and float180 a movement component; this callback does not zero all
velocity/position/air flags. Preserve unrelated flags rather than importing
idle-entry reset rules into15.

In the bounded normal main00100000-clear domain, AdjustMoveDir reads current
**display angle1ac**, chooses D=180 for inclusive90..270, otherwiseD=0;
computes H=wrap(Q1000(single(single(270-D)+offset19c))) and writes
1a0=H,1a4=H,1ac=D,1b0=D. Q1000 uses single fused1000*a+signed.5,
fctiwz and division1000 before repeated360 wrap. Source writes are
80096770–84 or800968bc–d0. It writes no display flags or base-speed/rate.
The main00100000-set domain has its own retained angle aliases and classification;
[display-to-movement heading](../overworld/OVERWORLD_DISPLAY_TO_MOVE_HEADING.md)
and [low-HP/idle AdjustMoveDir ownership](../overworld/OVERWORLD_MARIO_LOW_HP_IDLE.md)
pin that boundary. Never replace this service with position-facing-to-item.

## Admission and display selection

marioMain requires main1. Its main2 path bypasses marioMotion/input/Boots and
continues the common suffix including PreDisp. marioCtrlOff8005c2c4 sets main2
and increments nested signed byte38; marioCtrlOn8005c290 decrements byte38 and
clears main2 only when the stored signed remainder is exactly0.
These are retained admission inputs, not a Boolean pose-playing test.

Finite itemseq substate1 requests motion15, creates presentation and grant,
then exits at800ac264 with substate2. Substate2 ControlOff is800ac274 on a
later item invocation; it sets item status01000000 and falls through substate3.
Thus when Mario motion dispatch is admitted between those item calls,15
initialization runs before this getowner's CtrlOff. Preexisting main2 or other
owner gates can defer it further: source does not authorize forcibly consuming
pending1 merely because an item requests15. Field owner order is linked above.

ChgPose display2 locks return without requesting M_I_U; same current name18
also returns without restart. A changed ordinary request stores name18,
zeros start-frame28, clears display30000000 and queues word0c1000.
PreDisp applies RearAnime before direction update and pending upload.
The complete original toRearPose800566cc and toFrontPose800561b8 comparison
lists contain no M_I_U. Rear-sector conversion miss then inverse miss clears
display80000000 at80057f68–80; front-sector routing also clears it. Therefore
ordinary unlocked M_I_U uses front handle22c/file a/a_mario for either sector,
not rear230 or effect234. Source front↔rear rules remain explicit mappings.

Pending upload clears bits5000, calls SetAnim(force1), SetLocalTime(start28),
then Main on this same PreDisp. Original front M_I_U is index39, nonloop0..0,
with one keytime0; it is absent from a_mario_r/e_mario. Clock processing selects
that static frame through its zero-length descriptor. There is no authored
waving duration to invent. [Ordinary selection](../overworld/OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md)
and [animation clock](../overworld/OVERWORLD_MARIO_ANIMATION_CLOCK_AND_LOOPS.md)
retain the separate pose lock, queue, time and handle owners.

## StayMotion restores a motion owner

marioChgStayMotion80094e48/e0 returns unchanged for decimal current25,26,21,22,
then returns for fresh Mario main10000000. It applies the same retained previous
16/17 conditional display08000000 then4 cleanup described above. It saves
current+2e to previous+30, writes current0 BEFORE previous post callback,
then ORs pending1. From15 the post is null. It does not recover a saved pose,
restore pre-item heading or call ChgPose directly.

The next admitted motion0 callback owns entry resets and pose choice. Ordinary
character0/main80000000-clear/main2000-clear/unlocked/liveHP>5 requests M_S_1
unless current name is exactly M_I_Y; after successful ordinary M_I_U this
requests M_S_1. Source lowHP/HP0, transformations and command transitions have
separate predicates in the linked idle contract. Display subsequently selects
its actual front/rear counterpart from retained facing; do not save/restore an
arbitrary pre-get animation as a substitute for that owner.

Success item cleanup calls CtrlOn/KeyOn then StayMotion; the next admitted
MarioMain consumes it. Failure substate3 also calls StayMotion but retains
control-off at the discard frontier: pending stay may remain unconsumed while
main2 remains set. Do not immediately switch to idle for this failure request.

## Finite ordinary post callbacks and item entry movement clear

Fresh raw original table80310030 rows,12 bytes(main,reserved,post):

| Decimal current motion | Row | Main | Reserved | Post |
| --- | --- | --- | --- | --- |
| 0 | 80310030 | 800a6c50 | 0 | 800a6b10 |
| 1 | 8031003c | 80087994 | 0 | 0 |
| 2 | 80310048 | 8008763c | 0 | 0 |
| 3 | 80310054 | 8009b480 | 0 | 0 |
| 10 | 803100a8 | 8009af1c | 0 | 8009aed4 |
| 11 | 803100b4 | 8009af1c | 0 | 0 |
| 15 | 803100e4 | 80092a3c | 0 | 0 |

**Ordinary idle0 has a nonnull post**. For its new15, mot_stay_post800a6b10
cancels retained word27c if not−1: psndSFXOff(handle),then27c=−1. Next apply
the same rule to word280. Handle−1 skips both call and write; new19/20's
retain280 exception does not apply to15. Read each handle at its source stage;
post does not reset motion phases, pose or movement fields. Full cancellation
and cue ownership is [idle sound](../overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md).
A native idle-field adapter must retain actual provider handle lifetime, rather
than substitute a cue ID or cancel an arbitrary last-played sound.

Motion10 post mot_fall_post8009aed4/48 obtains Mario, reads word284;
if not−1 calls psndSFXOff284 then writes284=−1. Otherwise returns unchanged.
Motion11 shares the fall main but deliberately has no post. Null rows do not
implicitly run idle/fall cleanup. Get/Stay requests must dispatch the post of
actual old current motion after installing their new current motion.

The itemseq substate0 clear at800abcc4 loads original804210b4 float0 and
writes, in order,174/17c/180/18c(at800abcc8/ccc/cd0/cd4). Source roles:

| Mario float offset | Role in the original movement owner |
| --- | --- |
| 174 | External/additive X displacement |
| 17c | External/additive Z displacement |
| 180 | Retained base horizontal travel speed at heading1a4 |
| 18c | Computed combined horizontal displacement norm |

This is an inline item-owner clear; no DOL service named clearMarioMotion is
called. In marioMoveMain800594a8,59808–1c adds17c to combinedZ and174 to
combinedX;59818–38 separately adds178 to rootY. Combined norm is stored18c
at59988. Item clear does **not** write178(additiveY), vertical jump/fall
components, current/previous motion, heading, pending entry or air flags.
See [movement fields](../overworld/OVERWORLD_MARIO_MOVEMENT.md) and
[direction composition](../overworld/OVERWORLD_MARIO_DISPLAY_DIRECTION.md).

Erratum: earlier window/pose contract described180 as vertical. Its operand
label is corrected to base horizontal speed; raw addresses and clears remain
unchanged. Portable provider names must not imply a vertical reset from this
four-float source operation.

## Evidence and limits

Fresh named disassembly: marioChgGetItemMotion, mot_getItem,
marioChgStayMotion, marioMotion, marioCtrlOff/On, marioRearAnime,
toRearPose/toFrontPose. Existing complete MarioMain and motion/pose verifiers
pin their caller/upload/idle providers. Raw motion row and literal were reread
from ISO; comparison-name operands were decoded against loaded DOL spans.
Inspection artifacts are ignored under artifacts/reference/goomba3-services.

| Original span | SHA256 |
| --- | --- |
| 80094c90/100 | 559a7ead06d17124ab742423eb1c61f89c45244e51281032235bd8ba690733a3 |
| 80092a3c/58 | af95a0892c7760570745f9f5288ad3e1c3fa83bd8a4dc0039577b1d5457d003e |
| 80094e48/e0 | 2bdf307b14482899000609f65ee163864c887d8beba475eb0a1464a8f43552b6 |

Native caller cadence/control nesting, complete special-motion providers,
original SDK numeric execution, renderer parity and hardware performance are
separate validation boundaries. Source order does not imply rendered-frame
cadence or allow bypassing MarioMain admission for instant requestPose.
