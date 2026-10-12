# Ordinary overworld airborne and landing ownership

Source: US G8ME01 disc0 revision0, read-only DOL. This is static reconstruction,
not a Dolphin observation or numerical-equivalence claim. The accompanying
`tools/verify_overworld_airborne_and_landing.py` pins shipped bodies, instruction
anchors and literal data. The slice is ordinary Mario, no transformation,
root lock, roll event, Jabara, ride, background/Paper, special surface or map
physics override, ordinary screen position and input/control service state.
Special branches are identified below rather than assigned invented behavior.

## Separate fields and late pending requests

Retain motion+2e, previous motion+30, pending-entry bit0 of+0c, phase+44,
countdown+48, signed16 counter+50, signed16 peak counter+52, jump/fall flags
0x00010000/0x00020000, vertical tuple(V,A,J,S) at+7c/+80/+84/+88, rootY+90,
takeoffY+120, retained base+180/travel+1a4, takeoff base+190, current magnitude+194,
and support+1e8/previous support+1ec as separate state.

A motion request changes+2e immediately and sets pending bit0. Its main callback
runs when marioMotion8009461c dispatches, not whenever a later physics owner
requests a motion. See [ground arbitration](OVERWORLD_MOTION_ARBITRATION.md).
The normal marioMain calls marioMotion before these physics branches, with no
second dispatch in their tail. Therefore motion10 can be pending while jump
flag is still set, and motion0/1 can be pending while fall flag is still set.
Do not derive the active physics branch solely from the newly requested motion.

## Jump callback and ascent-to-fall call

mot_jump8009b480 consumes pending bit0; sets phase0 and countdown2
(8009b4cc–8009b4e0); clears fall flag and sets jump flag; zeros counters+50/+52
(8009b4f0–8009b50c); copies support+1e8 to+1ec; calls marioSetJumpPara800941bc;
copies takeoff position to+11c/+120/+124 and base+180 to+190. Ground arbitration
has already established surviving launch base/travel. This callback does not
replace them with current input.

In that SAME callback, phase0 decrements countdown2→1 and counter0→1. Next
callback decrements1→0, sets phase10, counter1→2 (8009b618–8009b66c).
Thereafter phase10 skips the countdown and increments counter once per callback.
The launch call has horizontal movement but no vertical integration: marioJump
explicitly returns for phase<10 at80093d84–80093d8c. There is no counter3 gate.

For plain scale+228=1 and mainflag0x1000 clear, marioJump performs sequential
single-float updates J=J+S, A=A+J, V=V+A (80093db0–80093dec).
If updated V>0, signed counter>=4 and heldA(+24a&0x100)==0, it adds the literal
-0.5260000228881836 to V (80093df0–80093e1c). It tests the resulting V<=0.

At the first nonpositive V, if prior V was positive, peak counter+52 receives
current+50 (80093e34–80093e40). It calls marioSetPeakYpos800968f0; loads the
ordinary FALL tuple from marioJumpData80416110+16; integrates that new tuple
J+=S,A+=J,V+=A on this same call (80093e6c–80093f10). Its first ordinary
transition V is approximately -0.280 original units. This does not use
marioFall's positive-J clamp on the transition call.

With marioChkSts(16)==0, root-lock clear and ordinary previous-motion cleanup,
80093f74–80093fb4 requests pending motion10. marioChkSts16 checks motion22,
not a badge (8005bfa4–8005bfb8). Exclude that motion22 alternative. Jump flag
remains active until next mot_fall entry. This same call adds the new V to rootY
at80093ff0–80093ffc; the normal caller continues overhead then horizontal
movement, with no marioFall landing query on that call.

## Support retention during no-contact ascent and apex

mot_jump copies current support+1e8 to previous support+1ec without clearing
current support. Neither marioJump nor ordinary no-hit marioChkOverhead clears
+1e8 locally. Even when signed counter>5 makes marioMain call marioChkGnd2,
8008f100–8008f104 returns immediately when jumpflag0x00010000 is set. Thus
ordinary no-contact ascent AND the apex pending10 call retain current support;
clearing it on every ascent tick is unsupported. This bounded local retention
ends when marioFall clears+1e8 at80093ae0 before its new landing query.

Ordinary idle pending entry does not zero takeoffY+120: mot_stay800a6c50 copies
current actual position+8c/+90/+94 to+11c/+120/+124 at800a6d38–800a6d4c.
In particular800a6d44 snapshots actual rootY into+120. It clears vertical tuple,
counter+50, countdown+48 and phase+44 at800a6ce0–800a6cf4. Preserve that root
snapshot independently of clearing jump integration state.

## Ordinary ceiling response differs from the apex

marioChkOverhead8008c904 ordinary hit retains current motion3 until its
response test, provided the low-ceiling ground-arbitration branch did not change
it first. It sets contact flag0x08000000 at8008cb40. For a static non-NPC
ceiling it retains base/travel. Special hidden-block NPC classification at
8008caa0–8008cb38 instead can set base0 and mainflag0x80.

The proposed root correction at8008cb94–8008cbdc is
`Q10plus(ceilingHitY-bodyHeight)-1`, where Q10plus=trunc(fma(10,x,.5))/10 uses fixed+.5 rounding,
not symmetric rounding. If fall flag is clear and this is below takeoffY+120,
8008cbec–8008cbfc clamps it to takeoffY; the subsequent ordinary low-clearance
branch can request idle/walk/dash via marioChgStayMotion or base/previous-motion
comparison BEFORE testing motion3. For ordinary no-NPC motion3, root-lock clear, mainflag0x01000000 clear and
marioChkSlitEnd==0, the bounded low-clearance arbitration is:

- It runs only when fallflag is clear and proposed correctedY<takeoffY, strictly.
  Equality skips it. The local proposedY becomes takeoffY at8008cbfc.
- If base+180==0, current motion3 calls marioChgStayMotion80094e48 at8008cc54.
  That owner requests pending IDLE0, changes current motion immediately and
  invokes the previous motion's post callback; it does not enter idle main.
  Ordinary motion3 has no post callback in the shipped dispatch table.
- If base!=0, desired motion is2 when base>walk threshold+184, else1. At
  8008cc74–8008cc7c it compares desired with PREVIOUS motion+30. If equal,
  there is no request and motion3 survives. If different,8008cc98 requests
  pending desired ground motion through marioChgMot8009455c.
- Motion changed to0/1/2 fails the following current-motion3/6 test at
  8008cc9c–8008ccac. It skips the tuple clear, airflag clear, fall request,
  fall parameter load AND actual rootY writer. The clamped proposedY is only
  returned in f1 at8008cd80; normal jump caller8005b000 ignores that result.
  Consequently the just-integrated actual rootY remains until its next owner,
  rather than universally being clamped by this head callback.
- If current motion3 survives (notably desired==previous motion), the normal
  ceiling-fall response below runs and writes the clamped proposedY to root.

Both low-clearance routes leave signed counters, phase/countdown, and current
support node unchanged locally. The ground-request route also retains current
vertical tuple and jumpflag for the rest of this call; next pending idle/walk/dash
entry owns their clearing according to ground arbitration. Normal late
marioChkGnd2 still exits while jumpflag is set. In the no-NPC bounded domain,
base0/previous idle produces pending0 without immediate fall; retained walk/dash
matching previous motion produces pending10 and the ordinary fall tuple.
These are source-derived control-flow cases, not observed render timing.

For the surviving motion3 hit,8008ccbc calls marioClearJumpPara800943a8;
8008ccc4 clears ALL main airflags0x000f0000 immediately;8008ccdc requests
pending10;8008cce0 calls marioSetFallPara;8008cce8 writes corrected actual rootY.
Phase/countdown/counters are not reset by these local calls. Clearing then
loading the FALL tuple means no fall-tuple integration is performed by this
response. The rest of the current jump branch still runs horizontal movement,
but now with airflags clear. Next dispatch enters mot_fall and sets its fallflag.
This differs from apex pending10, which retains jumpflag and has already
integrated the new fall tuple. Do not immediately run an extra fall callback,
reset counter2, or infer the airborne movement policy solely from pending10.

## Ordinary head query and service gates

For normal nontransformed, non-Paper motion3, marioChkOverhead passes a local
copy of current position, local bodyHeight+1bc, and f1=current vertical V when
jumpflag is set; otherwise f1=literal10 (8008ca18–8008ca78). Special motion8
returns early; motion20 uses its extra front query; mainflag0x01000000 routes
through roll search; char2 uses its own service. Normal query is
marioSearchHead8008c1a8. It returns head node stored+1f8, local point/height
results; current rootX is copied back at8008ca80–8008ca84. RootY is not copied
back generically.

marioSearchHead uses radius.5*(baseWidth-.5), heading retained travel when
base!=0 else converted facing, five center/rotated cardinal offsets in the
source order, upward origin rootY+.5*bodyHeight, ray length bodyHeight+passedV.
It invokes marioHitCheck2/chkFilterAttr2 via8009054c. Source face/cull admission
precedes this leaf's normal angle gate. It processes probes in order, not a
highest/lowest winner across all five: the first nonrejected triangle yields a
contact or near-miss return. Ordinary normal inclination in inclusive[50,130]
is rejected at8008c5a0–8008c5c4; main+4 flag0x01000000 changes these endpoints
to[75,105]. Those projected normals write slope+21c/+220/+224 at8008c440,
8008c444,8008c49c before normal rejection; this head leaf does not write+218.

For an admitted ray, contactY=trunc(fma(100,rayPointY,bodyHeight/2))/100
at8008c668–8008c6a4 (literal100 at80420900, bias is half-height). Contact node
is returned only if actual V>=-10 AND rootY+bodyHeight>=contactY
(8008c6a8–8008c6ec). On contact it writes local pointY=contactY and local
height output0. Otherwise it returns null with signed clearance
contactY-rootY-bodyHeight. This response uses ACTUAL V+7c even if the caller
passed literal10 for ray length. The hit owner sets contactflag0x08000000;
no-hit near clearance in[0,5] also sets it, otherwise clears it
(8008ccf0–8008cd38). Full node/NPC side effects and special branches remain
outside this no-NPC contract.

Head-search miss/raw-output clarification: entry does not reset projected
angles. Only processed ray normals write+21c/+220/+224; all probes missing
preserve the prior slope record. Ray normal components load directly from
marioHitCheck2 at8008c3c0–8008c3c8 and feed100*n angle construction at
8008c3dc/8008c3e4, without Q1000 rounding. The ray point is likewise raw before
the separate contactY Q100 calculation. The initial clearance output is -1.0
from804208e4 loaded8008c2b0 and stored8008c2c0; all-ray miss leaves it at -1.
Do not reset it to0, quantize head normals using the foot-search policy, or
clear the slope record merely because this query begins.

Ordinary position-output alias: marioSearchHead captures position pointer in
r26 at8008c228 and clearance pointer in r27 at8008c22c. It never writes
positionX+0 or positionZ+8. Contact writes only positionY+4 at8008c6e0; near-miss
writes only clearance at8008c6c4/8008c700 and leaves position XYZ unchanged.
The separate Paper branch also writes only Y at8008c638. Consequently ordinary
marioChkOverhead's rootX copy8008ca80–8008ca84 is an identity copy, not a hidden
horizontal correction. Do not move root XZ to an offset probe's ray hit point.

After a low-ceiling pending WALK1/DASH2 request, ordinary mot_walk80087994 and
mot_dash8008763c pending entry consume bit0 and clear airflags0x000f0000
(80087a64–80087a74 and8008770c–8008771c), but neither whole local callback
clears vertical tuple+7c/+80/+84/+88 or signed counter+50. These are retained
until another documented owner writes them. Thus the next ordinary ground
query can see nonzero vertical input and the FrontStep counter==0 gate can
still suppress stair admission. This differs from pending idle, whose entry
explicitly clears tuple/counter and snapshots takeoff position. External
pose/special-motion services remain excluded from this local no-clear claim.

## Fall entry, phase and counter preservation

mot_fall8009af1c pending entry consumes bit0, clears jump flag, sets fall flag,
clears command bits0x00100000/0x00200000 and sets phase0. It does NOT reset
countdown+48, either signed counter, vertical tuple, base or travel. If motion11,
it sets mainflag0x80 at8009b008–8009b010, disabling input drift in movement.
Motion10 does not set that bit here. Return-position capture happens only when
retained+50==0. These assignments are at8009af60–8009b034.

Phase0 becomes10 when rootY<=takeoffY+120 (8009b048–8009b070), with a camera
service. This is not a vertical integration delay: marioFall has NO phase gate.
Then mot_fall increments retained counter+50 once at8009b074–8009b07c. Thus an
apex transition retains its jump counter/peak counter; ledge entry retains the
preceding owner's counter. Do not replace either with a fabricated counter2.

Ground support-loss marioChkGnd requests10 for negative distance/all-ray miss,
11 for positive recognized drop, and calls marioSetFallPara8009412c. That
parameter service loads the tuple but does not reset motion counters or phase.
A late support-loss request does not run fall physics in the finished ground
branch. Next manager call processes pending fall entry, then integrates fall
without waiting for phase10. See [support/steps](OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md).

## Caller order and retained-base damping

Normal marioMain jump-flag branch: marioJump at8005affc → marioChkOverhead at8005b000
→ marioMove at8005b004 → marioChkGnd2 only when signed counter>5 (8005b028–34).
Normal fall-flag branch: marioMove at8005b04c → marioChkOverhead at8005b074 →
marioFall at8005b078 → marioChkGnd2 when counter>5 (8005b0b0–bc).
Consequently fall's retained-base mutation applies AFTER this call's horizontal
movement, before its vertical landing search, and affects next horizontal call.
Do not apply this damping before the current marioMove. Overhead forced entries,
roll16/17 and motion25 have separate branches.

In marioFall, with mainflag0x20 clear, on-screen coordinates+168/+16c within
inclusive X[0,600],Y[0,535], and not BOTH marioKeyOffChk and marioCtrlOffChk true,
80093ab4–80093ad8 updates retained base:

```
base = max(0, single_fma(0.009999999776482582, single(-0.5-base), base))
```

This mutation is separate from mot_fall, which locally retains base. Off-screen
with mainflag0x20 clear,80093a38–80093a68 sets mainflag0x80 and performs
`base=single_fma(0.10000000149011612,-base,base)`. The ordinary native screen
adapter is a dependency; don't silently infer original screen coordinates from
3DS screen dimensions. With mainflag0x20 set, these base mutations are bypassed.

## Fall integration and authoritative landing query

Ordinary marioFall clears support+1e8, then at80093b50–80093bc4 performs
J=J+S, clamps positive J to0, A=A+J, V=V+A. At V<=terminal V=-18 from
marioJumpData+36, it sets V=-18 and zeros A,J,S. This terminal cap happens before
the landing query. The FALL tuple words are`bdcccccd be4ccccd 3cc49ba6 bb83126f`,
approximately(-.1,-.2,.024,-.004);
use the pinned IEEE32 words, not decimal arithmetic as an equivalence oracle.

Before rootY integration,80093bc4–80093bd0 calls marioChkLandon8008dc70 with
updated V and a height-output pointer. For ordinary transformbyte+3c!=2,
mainflag0x01000000 clear, marioChkLandon at8008dca8–8008dcbc calls
**marioSearchGround8008e694 with D=V and vertical input=V**. It does not call
marioChkLandon2. Flag0x01000000 selects marioSearchGround4; char2 selects its
own landing query. Both are excluded here.

Reuse the exact [five-foot SearchGround contract](OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md):
heading/base-dependent footprint radius.375*baseWidth, ordered five offsets,
originY=rootY+5+abs(V), downward length5+2*abs(V)+.1, nearest admitted triangle
per probe, Q1000 returned point/normal, inclination>=50 rejection, independent
height/delta/angle/slope fields, strict highest admitted floor. In particular:
for a below-root floor distance>=2, it must not exceed abs(V); other height
admission compares abs(floorY-rootY)<=11, a literal constant at8008efd4
(804208b4), independently of D. Thus small drops<2 and above-root hits do not
use abs(V) as this alternative height bound. D controls ray length, while V
controls the recognized larger-drop branch. Do not invent root±abs(V)
ray bounds or use a different footprint because the caller is airborne.
The slope record can belong to a later processed/nonwinning ray; projected tilts
can be written before cone rejection, overall inclination only after it.

If no node, rootY+=V at80093cdc–80093ce4. If node,80093bdc–80093be4 stores
support+1e8=node, previous support+1ec=0. For ordinary surface attr0x200 clear,
80093c04–80093c08 sets actual rootY=returned Q1000 height. Attr0x200 instead
integrates rootY+=V even on hit. It then calls marioLandOn for ordinary support
at80093cf4–80093d00. Fall does not perform ground's Q100 height metadata+1c4
writer: keep landing support/slope/height outputs separate from ground metadata.

## Landing tuple-clear owner and next dispatch

marioLandOn80092c94 reads support attributes. If attr0x200 clear, it zeros the
entire tuple at80092cc4–80092cd4, on the actual landing call. If set, it retains
that tuple. It clears mainbits0x80/0x40 and zeros signed counters+50/+52 at
80092cd8–80092cf8. The exact masks are rlwinm25,23 at80092ce0, which clears
PPC bit24=0x80, and rlwinm26,24 at80092cec, which clears PPC bit25=0x40.
Mainflag0x20 is PPC bit26 and survives BOTH masks. A portable fall-damping
bypass mapped to that flag must not reset merely because marioLandOn ran;
its original writer remains a separate dependency. It does not universally enter motion13, clear phase/countdown,
replace base/travel or immediately dispatch the new ground callback.

Its ordinary route selects late pending WALK1 when current magnitude+194>0
and command0x00400000 clear (8009365c/668/694), else neutral IDLE0
(800937dc/7e8/814), subject to excluded Jabara/special domains. Base/travel
remain until next ground entry; flags likewise await that dispatch. The next
manager call samples CURRENT input; strong input can select dash and a new A
trigger can launch. Prior airborne A is not buffered by this owner.

## Bounded native acceptance and dependencies

Native ordinary binding should reproduce launch phase0/counter1 without ascent;
next-call phase10/counter2 first ascent; peak-counter capture and late pending10
with jump flags on the transition call; retained fall-entry counters/tuple;
fall movement BEFORE its base damping and vertical query; exact SearchGround
D=V,V=V; no-hit integration versus hit snap; attr0x200 tuple/root exceptions;
late landed ground dispatch and counter reset by marioLandOn.

Mainflag0x1000 divided jump integration, nonunit scale+228, root locks, background,
roll event/states16/17/22/25, char2, Paper query4, off-screen projection services,
Jabara command interactions, attributes0x200 and other event/special landing
routes, overhead/contact service overrides, moving platforms, and external tuple
writers require their own binding. This contract exposes their gates; it does
not claim a complete full-game airborne owner or Gekko numerical fidelity.
