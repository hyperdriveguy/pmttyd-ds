# Flying partner executor: selectors,motion calls and hazard tail

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
flyMain@party_move.o at0x800c7ff8,size0x570. This is distinct from
flyMain@mot_plane.o at0x8009c9b8. The original symbols contain both names;
inspection used a temporary namespace-filtered copy of the annotation CSV.
Actual partyFlyMain calls this body with P at0x800c8eac. Only that caller's
selected tail is included here,not its complete flying-history writer.

Unsigned selector P+3b dispatches0,1,2,3,6. Every other value returns without
local motion,angle-copy,splash/countdown or hazard work. Unsigned motion P+36
is a second dispatch variable. Motion handler integer returns are ignored.
No P/owner pointer guard is added. This is a local ordered request graph,
not original executed movement or a fixed tick/render cadence measurement.

## Selector0: pose marker and stay/reset/fall

If P+8 mask4 is set,clear ONLY that marker,request partyChgPoseId(P,1),then
reread unsigned kind P+31. Kind5 requests unk_80187d74(P),otherwise no extra
kind request. With or without that marker,read unsigned motion P+36:

| Motion | Local action |
| --- | --- |
| 0 | party_motion_stay(P);reread kind and if5 request unk_80187d10(P) |
| 1,2,6 | Store motion0 and OR2 into P+8 |
| 3 | [party_motion_fall](FIELD_PARTNER_FALL_AIR.md)(P),ignore return |
| Other | No motion request |

The reset branch also contains an old-motion==3 cleanup of P+0 masks0x70,
0x03000000,0x04000000. Its captured selector cannot equal3 on the locally
selected1/2/6 route;that redundant compiled test is not another fall path.
All selector0 routes then copy current P+100 to P+fc and enter the splash/
countdown/hazard tail below. Stay,pose and kind5 presentation engines remain
separate contracts;the extra requests are not proof of a rendered effect.

## Selectors1,2,3,6

Each consumes P+8 mask4 if present without selector0's pose/kind requests.
Each reads P+36 and requests exactly one table row below,or none. Mobility is
unsigned P+32;only equality0 selects the first alternative. No1-only domain is
imposed. After the selected call,copy CURRENT P+100 to P+fc.

| Selector | Motion0/1 | Motion2 | Motion3 | Motion6 | Motion7 |
| --- | --- | --- | --- | --- | --- |
| 1 | Motion0:stay;motion1:mobility0 homing_walk,else [homing_fly](FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md) | Main jump | Fall | Upstairs | Damage |
| 2 | Mobility0 behind_mario,else [behind_mario_fly](FIELD_PARTNER_BEHIND_FLY.md) | Jump2 | Fall | Upstairs | Damage |
| 3 | beside_mario,regardless of mobility | Jump2 | Fall | Upstairs | Damage |
| 6 | Mobility0 behind_mario_continue,else behind_mario_fly | Jump2 | Fall | Upstairs | Damage |

For selector1,dispatch table0x80315050 contains eight exact targets:
800c8228,800c8234,800c8258,800c8264,800c8284,800c8284,800c8270,800c827c.
Motion4/5 go to angle-copy with no handler;motion>7 does too. Other selectors
also do nothing for unlisted motion values,including4/5/>7. They do not default
to stay or rewrite motion0. Selectors2/3/6 return after the angle copy and do
not run this executor's splash/countdown/hazard tail.

Call targets:stay8015164c,homing_walk8014fd4c,homing_fly8014f048,
behind8014e8ac,behind_fly8014defc,continue8014e3dc,beside8014da8c,
main jump8014d2f0,jump2 8014cf54,fall8014cbb8,upstairs8014cb28,
damage8014c858. Jump/fall local contracts are linked from the
[walk dispatcher](FIELD_PARTNER_WALK_DISPATCH.md);the other named engines
remain open unless independently linked as complete local contracts.

## Tail runs only for selectors0 and1

After the angle copy,if P+8 mask0x01000000 is set,clear it FIRST,copy current
root XYZ raw words to a local vector,request marioSetSplash(0,&vector),then
store byte20 to P+4c. Regardless of that marker,load P+4c as UNSIGNED,
subtract1,store its low byte,and test the SIGN-EXTENDED low byte. If negative,
store0. Thus the newly assigned20 becomes19 in this same invocation. This is
not a general signed saturating decrement:128 becomes127,while255 becomes0.
The request is a splash binding,not a measured sound/visual-duration contract.

Next,if current unsigned run mode P+34==13,return. Otherwise test contact
pointers in exact order P+138,then+12c,then+140. Skip null pointers;for each
nonnull request hitGetAttr and test mask0x800. The FIRST matching contact
requests partyChgRunMode(P,13),then returns without reading later pointers.
Absent a match,return. No terrain query or root integration is performed by
this tail. Earlier motion/pose/effect calls may alter these later live loads.

## Usage and verification

All seven DOL text sections show one direct linked caller to this executor:
partyFlyMain site800c8eac. Used flying-handler calls also occur in walkMain
and explicit movement-mode wrappers;the exact nine-site census is in
`tools/field_partner_flying_evidence.json`. It establishes static direct references,
not runtime frequency or complete indirect/REL reachability.

Run `python3 tools/verify_field_partner_flying.py`. Six containing hashes cover
five complete local bodies(this executor,two flying handlers,two speed writers)
and selected partyFlyMain tail,869 instruction anchors,34 constants,four spans,
two US lecture imports and nine direct sites. Called ability/pose/effect/hazard/
history lifecycle/source/numeric domains and original trajectories remain open.
