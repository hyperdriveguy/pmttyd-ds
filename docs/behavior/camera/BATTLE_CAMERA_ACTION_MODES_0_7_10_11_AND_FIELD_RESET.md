# Battle camera action modes0,7,10,11 and field reset boundaries

2026-10-14. Read-only original US G8ME01 disc0 revision0. Addresses/packed
offsets are hexadecimal; mode and speed numbers decimal. Complete Main owner
8012c8b0 and MoveTo8012c128 are the sources. No runtime observation or native
code modification. This extends [default/victory](BATTLE_CAMERA_INIT_DEFAULT_AND_VICTORY_MODE17.md)
and [timeline3](BATTLE_CAMERA_MODE3_TIMELINE_AND_MOVETO_CAPTURE.md).

## Correct source constant map and speed correction

r2 is80427440. In particular lfs displacement b31c resolves8042275c=15,
while b320 resolves80422760=5. The earlier speed1 divisor5 attribution was
wrong and the owned default/victory contract is corrected to15. Width
compensation uses b314→80422754=1.5, not15. These are distinct original words.

| Used source float | Original address |
| --- | --- |
| 0,.25,600,.5,100,-100,1.5,1 | 8042273c/40/44/48/4c/50/54/58 |
| 15,5,200 | 8042275c/60/64 |
| 110,750,60 | 80422768/6c/70 |
| 50,24,9,80,10 | 80422790/94/98/9c/a0 |

MoveTo signed first speed277c rejects<0 or>=5 to1; second speed277e
rejects<0 or>=6 to1. For every admitted ordinary speed, delta is rounded
target-current, then divided with single rounding except speed3, then stored,
reloaded and single-added to the camera. Source divisors:

| Speed | First X/Y block | Second Z block |
| --- | --- | --- |
| 0 | 1.5 | 1.5 |
| 1 | 15 | 15 |
| 2 | 5 | 5 |
| 3 | no division | no division |
| 4 | 200 | 200 |
| 5 | rejected to1; retained code uses100 but is unreachable after this guard | 100 |

First block order eyeX,eyeY,atX,atY then up(0,1,0); second eyeZ then atZ.
All these are arithmetic services already inside MoveTo; no external callback
is invoked by a speed branch. Source callsites include retained speed2 requests,
so a native1/3-only adapter cannot close ordinary attack framing merely by
adding action-mode formulas.

## Shared lookup/position dependencies

Modes below preserve current speeds; they do not force a level. Each ends with
MoveTo(1,1), applying its exact relative/mirror/offset/X-clamp operations, then
diagnostic sprintf and common Main publication/axis freezes/shake tail.
Missing homing units/parts have the explicit mode0 request paths below; an
admitted mode change does not execute mode0's framing in the same invocation.

BattleGetUnitPtr800f88cc(B,ID) and BattleGetUnitPartsPtr800f8874(ID,partID)
retain their source registry/global-capture semantics. B2764/276c are first
homing IDs; B2768/2770 second. Position dependencies are existing typed services:

- BtlUnit_GetPos80128c7c:1c returns unit current root U3c/40/44, loading
  and storing X,Y,Z in that order. Original loads are80128c7c/84/8c.
  Earlier informal U8c/90/94 attribution mixed field Mario and battle records
  and is explicitly superseded; those are not this GetPos body's operands.
- BtlUnit_GetHeight80128414 returns signed HALF Uce; GetWidth8012841c signed
  HALF Ucc. These do not read part dimensions or model geometry bounding boxes.
- BtlUnit_GetHitPos801289ec composes actual part world position and both
  source hit-offset triplets with U114/side factor. Its existing
  [packed hit contract](../battle/BATTLE_PACKED_HIT_POSITION_CAPTURE_ORDER.md)
  supplies live read/capture/rounding order.
- BtlUnit_GetPartsWorldPos80128b10 uses part's retained U owner, root U3c/40/44,
  local P24/28/2c and signed U189 for X; it does not read P60 part scale.
  It is not a GetPos synonym. Subsequent GetHitPos offset scaling uses U114,
  rather than inventing a dependency on P60.

Construction producer: BtlUnit_Entry reloads kind pointer U10 at80129298;
loads signed HALF(kind+16) at801292a0 and stores Ucc at801292a4; reloads U10
at801292a8, loads signed HALF(kind+18) at801292ac and stores Uce at801292b0.
These are authentic kind metadata width/height scalars. Native imported kind
records can reuse this existing packed entry owner; derived sprite extents or
default generic dimensions would not reproduce the source getter.

## Mode0 and one-owner modes7/10

Mode0 entry8012c9b0 requests eye(0,110,750),at(0,60,0),MoveTo(1,1).
Zoom is not read for this handler and speeds are retained.

Mode7 entry8012cd00: resolve first unit; null requests mode setter(0,0) then
goes to common tail. Otherwise call first part lookup but **discard its result**,
then GetPos(unit), GetHeight(unit), GetWidth(unit), in that order. There is no
missing-part early exit in mode7, and the discarded lookup still occurs.

Mode10 entry8012d214: resolve first unit, then part. Missing either separately
requests mode(0,0) and goes to common tail. Otherwise GetHitPos(unit,part),
GetHeight(unit), GetWidth(unit), in that order. Thus10's center follows actual
part/hit position while7 follows current root.

For either admitted position p, let R round single operations and F be single
fused multiply/add. Signed height/width convert to float through original bias:

```
pY = R(p.y + R(R(height-50)*.5))
W = R(width-24); if W<0: W=0
Q = F(-1.5,W,zoom)                  // actual fnmsubs(1.5,W,zoom)
Y = pY; if pY>100: Y=100; if pY<0: Y=0
T = R((-Q)/9)
eye = (p.x, R(R(110+Y)+T), R(750-Q))
at  = (p.x, R(R(60+Y)+T), -Q)
```

The second clamp test retains original pY rather than an invented reordered
NaN guard. Negative/unordered behavior must retain source comparison semantics.
No X derived from width, bbox or interpolation between actors exists here.

## Mode11 two-owner homing

Entry8012d388 resolves both units first; either null requests mode0 then common
tail. Resolve both parts next; either null similarly requests mode0. Then:
firstA=PartsWorldPos(firstPart), secondB=GetHitPos(secondUnit,secondPart),
GetWidth(firstUnit), GetWidth(secondUnit). Height is not queried.
Let W1=max(width1-24,0),W2=max(width2-24,0), both single subtraction/ordered
clamp. Cached homing first/secondX are B2810/B281c, cached secondY is B2820.

```
D = R(cachedSecondX-cachedFirstX)
N = R(secondB.x-firstA.x)
if D==0: D=10                     // source800? anchor8012d4d0 loads804227a0
ratio = R(N/D)
if ratio>1: ratio=1
if ratio<.5: ratio=.5
G = R(1-ratio)
Q = F(-1.5,R(W1+W2),R(2*zoom))
Z = R(G*Q)
X = R(G*cachedSecondX)
Y = R(R(cachedSecondY/80)*R(firstA.y-cachedSecondY))
if Y>100: Y=100
if Y<0: Y=0
lift = R(doubleFMA(20,-(double(1)-double(ratio)),double(Y)))
T = R((-Z)/9)
eye = (X, R(R(110+lift)+T), R(750-Z))
at  = (X, R(R(60+lift)+T), -Z)
```

Denominator0 substitutes10 **then divides**, not a direct ratio=.5 fallback.
The final lift uses original double1 at802f0378 and double20 at802f0370 with
double fsub/fneg/fmadd then frsp. Do not use rounded single G as that double
factor. X is literally G*cachedSecondX, not a conventional first/second linear
interpolation. Keeping these unusual source operations avoids invented framing.
MoveTo still consumes original relative/mirror and offset and retained speeds.

## Field roll/translation and projection-extra boundary

camLoadRoad8000f36c:e8, called by original map load, unconditionally resets
C38=25 then C14c roll,C158,C154,C150=+0 at8000f404..414, even after failed
road setup. Successful setup makes camera mode1; failure mode3. That is a real
room-init zero producer for gor/tik camera4, not merely boot defaults.
It does not rewrite near/far, projectiontype, or C1a0. Its initial HALF0 clear
at8000f3a0 removes mask10; it **does not clear projection-extra mask8**.

Projection extra has an actual prologue writer: evt_cam_shake800a9144:3dc
gets selected camera and constructs C1a0 with PSMTX44Trans8029ea98 using
alternating +/- supplied XY at800a9238/58, then ORs HALF0 mask8 at800a9270.
The existing gor camera census includes harbor gundan1000tai_yure/afterbattle
and Professor kurihakase_first_talk (file297488; camera4; amplitude0/10;
duration500). These source events prevent a global claim that extra never exists
in the prologue. camMain consumes extra on the left and clears8; shake's actual
completion at800a92d4 also clears8 before return2. The source matrix remains
stored but is unused while flag8 clear.

A bounded zero-extra domain needs latest map-load/reset plus actual subsequent
writer/completion history. Complete field event/camera ownership proof through
trio entry was not established here; no blanket zero projection-extra claim.
Native fieldCameraFrame's already composed view/projection can be imported as a
declared snapshot, but it does not identify whether original roll/translation or
extra have separately been consumed. Avoid applying them a second time, or
inventing component values solely from the final matrix. Preserve explicit
unknown-history admission until source producer records are owned.

## Evidence

Read-only listings: calcdepth-battleCameraMain, endingcamera-MoveTo,
cameramodes-height,width,camLoadRoad,fieldshake in ignored
artifacts/recovered-temp-evidence. Hashes below cover exact original spans,
not executed trajectories or native numeric equality.

| Source span | SHA256 |
| --- | --- |
| Main mode0 8012c9b0:4c | 1a0c60f34032399977fb496a778eccb85c8c8b654df74db586367c08d081f642 |
| Main mode7 8012cd00:15c | 3221ac4a2f25d922fdca4ff741673b5243525a5d0ee5a4d3ce8a29087f541977 |
| Main mode10 8012d214:174 | 4eb6909213889e66e84d7de154bfbacd5aa6397ee93cc77e3959a58b2f88e04f |
| Main mode11 8012d388:24c | e006a32d0e379e700433b58de1cd0f91cceed2c7435f92c921971d39bab6ef46 |
| camLoadRoad8000f36c:e8 | 710f6eb4933ed2e3a699db5ed035f0c397565807f30b9aae7cd5129d92eaa861 |
| evt_cam_shake800a9144:3dc | e1fcae90aa675075bd9211bdfc33128eccf6b9ef2061a01434cd3365d4a47aa7 |

No code/shared documentation edits or commit. The owned prior camera contract
was corrected for the actual speed1 divisor15, with the correction explicitly
sent to the implementer.

## Independent raw ISO verification packet

DOL disc base20300. All original word offsets below are absolute ISO byte
offsets; no extracted or native pack offsets are substituted.

| Lfs site / instruction | Displacement / source VA | ISO offset / original word / float |
| --- | --- | --- |
| XYspeed0 8012c2f8 c062b314 | b314=-19692 →80422754 | 3eefd4 /3fc00000 /1.5 |
| XYspeed1 8012c394 c062b31c | b31c=-19684 →8042275c | 3eefdc /41700000 /15 |
| XYspeed2 8012c440 c062b320 | b320=-19680 →80422760 | 3eefe0 /40a00000 /5 |
| XYspeed4 8012c564 c062b324 | b324=-19676 →80422764 | 3eefe4 /43480000 /200 |
| deadXY5 8012c600 c062b30c | b30c=-19700 →8042274c | 3eefcc /42c80000 /100 |
| Zspeed0 8012c6d0 c022b314 | b314 →80422754 | 3eefd4 /3fc00000 /1.5 |
| Zspeed1 8012c718 c022b31c | b31c →8042275c | 3eefdc /41700000 /15 |
| Zspeed2 8012c760 c022b320 | b320 →80422760 | 3eefe0 /40a00000 /5 |
| Zspeed4 8012c7e4 c022b324 | b324 →80422764 | 3eefe4 /43480000 /200 |
| Zspeed5 8012c82c c022b30c | b30c →8042274c | 3eefcc /42c80000 /100 |
| Qmode7 8012cdc4 c002b314 | b314 →80422754 | 3eefd4 /3fc00000 /1.5 |
| Qmode10 8012d2f0 c002b314 | b314 →80422754 | 3eefd4 /3fc00000 /1.5 |
| Qmode11 8012d50c c022b314 | b314 →80422754 | 3eefd4 /3fc00000 /1.5 |

Speed3 has no divisor load. Body offsets and words were independently reread
from the read-only ISO after discovering the prior numerical attribution error;
the corrections are source evidence, not a performance or presentation choice.
