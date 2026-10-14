# Audience item creation, selection and lifecycle

US G8ME01 revision0, read-only DOL reconstruction, not Dolphin measurement.
Implementation: [battle_audience_item_main.cpp](../../../source/battle/battle_audience_item_main.cpp).
This extends the [actor hold/throw/damage controllers](AUDIENCE_ITEM_HOLD_THROW_DAMAGE_AND_ESCAPE.md).

| Original / data | Address / bytes | SHA256 |
| --- | --- | --- |
| BattleAudienceItemOn | 801a5aac / 1fc | 732ac58f704352a0fa32ce40edf6da5dedafd0674c28c86f66d8389769b96a21 |
| BattleAudienceItemCtrlProcess | 801a4f0c / 958 | 5e30b324f7fff2f6ad60bdb9756b2c248407af39f7edc8cfb6aef9c323e6ce82 |
| BattleAudienceDetectTargetPlayer | 801a5864 / d0 | 8db8c2b2cc1a7efa6ce5a7903831833c33ab1e17d2e35c51531e86dd765ff334 |
| pouchGetEmptyHaveItemCnt | 800d5614 / 60 | 63a6a3cfab4882b69fc1220a2ef1b6c3c510c05a94141525f8231962642717d7 |
| Four harmful-item descriptors | 8039c880 / 300 | b10aa7c23a8d3d4e8aeab030ea0771400131d1a10e17ef319bd86c949ad711e1 |

All addresses/offsets below are hex unless a count/value is explicitly decimal.
Numeric constants are read from unchanged existing PMEVT data, not inferred from
rendering. Simulation calls own the counters; no rendering-frame clock is added.

## Creation and pointer ownership

ItemOn captures A then member R. Reject A flags40000, a species item table whose
first kind is0, and explicit low16 requested kinds121..235 when swGet(235)==0.
That switch request occurs before seat/existence validation. Missing switch
service is unavailable, not a false flag. Kind0 requests later weighted selection;
its creation path does not consult this explicit-kind switch gate.

Seat is signed slot%20 and must be4..15. Require fresh GetExist, then reject
captured actor states7/8/13/12/11/18/17/21/10 and require actor flags8. Resolve
ItemGetPtr(0), then scan exactly the caller's positive signed limit at stride48.
The native Ctrl caller passes(index,0,1), so its ordinary allocation scans one
record; this does not silently become a100-record scan. No free record returns0.

On first flags1-clear record, retain other flag bits and set1, write state4=0,
owner+c=slot and kind10=low16 argument. ChangeStatus(slot,7), then write the
allocated signed-half index to **captured** R1c. Return1. Timer/position/velocity
fields are not cleared here. They belong to item Main. Successful source return
still owns the outer Ctrl throw-count increment.

## Item Main admission and states

Capture B, A and ItemGetPtr(0); visit100 records stride48. Flags1-clear skips all
item work. Each admitted record captures its owner member and species metadata.
Owner states13/19/21/12 or fresh GetExist0 clear item flags/count/state and unlink
R1c without the ordinary stop/reset calls. Unsigned item state>7 skips motion.
Table8039cd34 maps0..7 to801a4fcc/50f4/52c8/53a4/5654/56fc/5788/57d4.

| State | Original operation |
| --- | --- |
| 0 | Set visible flag2, choose kind when0, initialize timer180+itemIndex, size1, rotation0 and initial position; fall into1 |
| 1 | Track holder while timer>0; otherwise choose player target and prepare flight, then increment state |
| 2 | Move position XYZ, then velocity XYZ, spin kind239, decrement positive timer; on a later zero-count call advance to3 without motion |
| 3 | Evaluate receipt, publish presentation metadata or reject; eligible paths can fall into4 immediately |
| 4 | Move/fall; at Y<=0 stop throw-sound slots10/11, set5, Y=.1, vy=1,vz=0 |
| 5 | Move/bounce; at Y<=0 set6,timer60,Y=0 |
| 6 | Wait, toggle visibility by timer low2 bits below30, decrement; next nonpositive call cleans up |
| 7 | Decrement, then clean up when nonpositive |

All integer adds/subtracts wrap32. Each float add/multiply/divide has a separate
single store. Motion updates all three positions before all three velocities.
Ordinary cleanup first stops slots10/11, conditionally ChangeStatus(owner,0),
then clears flags/count/state and unlinks fresh owner R1c. It retains other item
fields, including owner/kind. A failed linked stop retains earlier stores.

### Weighted kind and telegraph

State0 scans captured species table kind/weight pairs through kind0, computes
wrapped signed total and requests irand(total) once. It uses cumulative inclusive
lower/exclusive upper ranges. Shipped tables and source RNG satisfy the positive
weight domain. An invalid supplied draw/table domain returns unavailable rather
than fabricating a kind or looping into unowned memory.

Initial XYZ use member position plus source species offsets; Y subtracts10,
Z adds1. State1 follows member derived C0/C4/C8: X is captured before intpl,
Y/Z read after it. Its lift is intplGetValue(0,min(180-timer,5),5,-10,0).
There is no lower clamp: itemIndex1 starts with elapsed-1. Item Main does not
count down this held timer; the original actor7 controller does so under its
battle/global gates. It therefore does not substitute180 rendering frames.

### Player target and launch

Scan64 units from captured B. For each present unit obtain original body ID and
parts pointer before testing signed belong==0, signed HP>0 and part flags10000
clear. Preserve original head fallback for body ID-1. No candidates returns
actual0. Otherwise irand(count) chooses a stored registry ID and explicit
captured B resolves the returned unit, even if the RNG service changes globals.
A null part at the later original dereference is outside the mapped domain;
invalid supplied RNG results remain unavailable, not a made-up target.

Before target selection, another active state1 item sets receipt mode44=2;
otherwise mode44=1. Store actual target pointer40, obtain its body part/hit XYZ.
Define single dx=targetX-currentX and n=30+abs(dx)/15. Store vx=dx/n first;
then fctiwz(n) is required. Set ax=0,vy=7, ay=2*(dy-vy*n)/(n*n),
vz=(targetZ-currentZ)/n,az=0; store truncated timer last and increment state.
The dy-vy*n term uses original fnmsubs through a fused numeric service. n is not
rounded before the velocity/acceleration calculations, and is not changed to a
ceil or a clamped generic trajectory. Native hit position reuses the existing
packed unit/part position owner. Source Gekko FPSCR/GQR parity remains unproven.

State2 disables receipt when captured B flagsEF4 lacks80; timer20 resets the
actor to0. A timer1 call moves and stores0 but stays in2; only the next call
advances3. Kind239 rotation adds source18 per moving call, without normalization.

## Arrival and original presentation owner

State3 disables receipt under the original turn flag, positive signed status18
duration, shadow guard, item flags4, or full inventory for kinds128..235.
Shadow guard is source unit flags104&10 or kind228 with signed work228>0.
Inventory space checks key item1 (10 slots normally,20 with the key) and counts
zero signed halfwords from current pouch+192. No resource is granted here.

With nonzero receipt, increment captured A137f8 and cap at3. Receipt mode2 sets
state4 and goes directly into motion, retaining that counter and leaving present
metadata untouched. Single-item receipt publishes through original fresh-base
SetPresentItemType/TargetUnitId/ItemNo fields137e8/137ec/137e4. Kinds121..235
set type0;236..239 set type1; other kinds retain the original prior type field.
For236..239 copy the exact c0-byte descriptor from8039c880+(kind-236)*c0 to
captured A137fc, then adjust descriptor word1381c by throw counter minus1.

Type0 hides flag2, sets timer5/state7. Type1 stops slots10/11, scales a negative
vx by source-.2, sets vy1.5,vz0/state4 and falls into motion. Other types do not
invent a new state. Reset A137f8 after this single-item arm. A disabled receipt
sets4 without publishing presentation metadata. The original presentation
system must still consume these fields and apply inventory/player damage;
this controller is not an alternate presentation event.

## Native scope and verification

Native creation replaces the old type0-only rejection prefix with this whole
local body and the current VM saved-global-flag view for swGet235. That view is
not proof of complete original save/legacy flag import. Native ItemMain replaces
the flags-clear-only provider with all eight local states, shared RNG, existing
packed body/hit/status helpers, original built-in interpolation, finite signed
float conversion, fused arithmetic and real sound-stop provider. Missing linked
services preserve their actual boundary. No earlier Main provider is bypassed.

Host tests cover creation guards/all256 actor states, low16 IDs, requested scan
limit, source weighted selection, state0/1 stores, negative lift step, target
filters/head fallback/captured battle, no-target cleanup, launch partial stores,
flight count/order, presentation fields/all four descriptors, counter cap,
status/shadow/full-inventory rejection, bounce/blink cleanup and invalid owner/
unsigned-state admission. A coupled fixture creates a real item and advances
existing actor7/8 plus ItemMain through receipt/bounce/deletion, checking one
actual target RNG draw. Its hit point and sound callback are explicit fixtures;
it does not execute a full BattleMain, renderer or presentation event.

Item rendering and source present-item execution, physical X and optional
lower-screen audience taps, normal Main/camera cadence, actor Enter11/special
states, idle low-ratio register provenance, full tutorial followups and field
return remain open. This is not visual telegraph or playable-throw acceptance.
Original menu/HUD art, dialogue/story and two-screen layout are unchanged.
