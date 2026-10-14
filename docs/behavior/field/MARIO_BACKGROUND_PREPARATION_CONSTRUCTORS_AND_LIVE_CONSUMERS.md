# Mario background preparation: constructors and live consumers

US G8ME01 disc0 revision0; static read-only recovery for
[EVT-01](../../requests/EVT-01.md). Complements
[Bero preparation semantics](../overworld/BERO_INFO_PREFIX_ASYNC_GROUPS_SYSTEM_FLAGS_AND_ACTOR_PREPARATION.md),
[key/fresh-owner boundary](BERO_KEY_LEAVES_FRESH_ACTOR_OWNERS_AND_NEXT_ROAD_FRONTIER.md)
and [ordinary movement](../overworld/OVERWORLD_MARIO_MOVEMENT.md).
Addresses/offsets/masks are hexadecimal; scalar values and form IDs decimal.
This identifies genuine future Game consumers rather than authorizing unused
raw-word shadows or claiming a full original constructor already executes.

## Correct field meaning

Mario+3c is a signed form/character selector. +1b8/+1bc are current collision
width/height. +184/+188 are base walk/dash speeds, **not render scales**.
Original ordinary values1/2.25 become native .01/.0225 movement per visit.
Mario+228 is a separate movement/model scale multiplier in the speed leaves.
Adding fields named graphicsScale184/188 would misrepresent their consumers.
The existing static preparation constants are correct raw values even where
older prose casually called them scales.

## Original initialization and retained/reset scope

marioInit8005b5c8/488 requests760-byte storage, publishes mp8041e900 at
8005b5f4 and memset-clears all2f8 bytes at8005b600. Thus form+3c starts0 from
an actual source producer. At8005b6d4 it reads that signed form: zero writes
+184=1 atb6e8,+188=2.25 atb6f0;nonzero alternative writes1/2 atb700/b708.
It then initializes slotbytes245/246=-1 and collision width20 atb72c,height37
atb734;these stores use fresh mp reloads between words. This local constructor
body provides normal form/dimensions/speeds rather than guessing unknown state.
Other constructor callbacks/resources remain required for full Mario init.

marioReInit8005b1d8/3f0 is different. It captures existing mp atb208,reads
form+3c atb228,sign-extends atb240,before clearing the2f8 record atb2b0.
It restores the captured form byte atb2f0. It initially writes speeds1/2.25
when form0,1/2 otherwise atb32c..b358,then freshly reads current mp/form and
applies the same form/small-flag dimension/base-speed table used by background
preparation atb35c..b3ec. Main flags are not simply preserved wholesale: its
captured/restored main bit in this prefix is80000000. Do not retain background
02000000 or small04000000 merely because the form byte is retained.

Form1 table writes width20,height47,walk1,dash2. Form2 writes only walk1,
dash2.25 in that immediate table;other fields are not restored from preclear
storage there. Later reInit calls marioSetCharMode8005d740(0) for form0 at
b540 and (2) for form2 atb554. Those actual downstream writers matter before
claiming final transformed dimensions;the immediate table is not proof that
form2 permanently has zero width/height. Form/character changes have their own
writer/resource branches, outside ordinary form0 native admission.

For ordinary form0,explicit native initialization can select the source-proven
20/37/1/2.25 values. It must distinguish that bounded constructor adaptation
from actual execution of every original init/reinit effect. A generic native
restore that default-constructs all fields would lose retained source form;
one that preserves every prior dimension/speed would ignore source recomputation.

## Actual gameplay consumers

The following are established source consumers, not speculative names:

| Source field | Actual consumer evidence |
| --- | --- |
| +184 walkbase | marioGetWalkSpd80087d84 reads base; marioChgMoveMotion80094f28 reads it at80095050 to compare retained travel+180; jump/ceiling transition uses it too |
| +188 dashbase | marioGetDashSpd80087ce4 reads dashbase (paper-mode branch can choose walkbase);marioMoveMain reads at8005994c with+228 multiplier |
| +1b8 width | searchFrontWall/staying/radius rays;NPC sphere and talk-distance query |
| +1bc height | wall-motion and overhead tests;NPC vertical overlap/talk candidate |
| main02000000 | marioBgmodeChk8005c164;background speed-half branch and talk eligibility |
| camera4 U16flag10 | cam3dMain skips follow-Y approach only when set |
| Mario display+4 mask20000 | marioUpdateCam adds depth-shift XZ instead of ordinary Y update |

[Walk/dash leaves](../overworld/OVERWORLD_SPEED_FALL_LANDING_LEAVES.md) apply
source paper/background branches then multiply by+228. In ordinary nonpaper
mode,background mode halves the selected base speed. Raw base values1/2.25
remain base values;do not prehalve stored fields then halve again in getters.
[Wall contract](../overworld/OVERWORLD_WALL_MOVEMENT.md),
[NPC collision](FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md) and
[talk query](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) use current dimensions;
default20/37 are not universal after form changes.

Actual marioGetScale80058908/20 independently tests main04000000 and returns
1.2000000476837158 when set,2 otherwise. This is further proof that +184/188
are not the source scale getter. Do not apply these values to a native model
without its existing original-model coordinate conversion/presentation contract.

## Camera-follow ownership and retained state

Background on/off ends in camFollowYOff801a8eb4 / On801a8ef4. Each first gets
camera4,reads/writes its U16flags setting/clearing10,then gets Mario afresh and
sets/clears display+4 mask20000. These preserve all other flag bits and do not
reset view/target blend timers,road work,shift work or clock. The normal follow
consumer is camera+98 smoothing,conditioned onflag10;the display20000 consumer
is the separate retained Mario camera-target producer. See
[camera lifecycle](../camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md).
Source reset lifetimes must not be inferred from a native room reset.

## Current native equivalents and required wiring

As inspected in this task:

- FieldMovement stores retained speed/headings but Walk/Dash are static
  .01/.0225;grounded,groundedStick and airborne cap read those constants.
  Game uses static WalkSpeed for prepareCeiling and static DashSpeed for motion
  classification. Live +184/+188 need meaningful reads at those consumers,
  keeping stored base speed distinct from current travel speed.
- FieldWallActor defaults20/37;Game::move and NPC side-response construct it
  without assigning live Mario dimensions. NpcTalkPlayer likewise defaults20/37.
  BodyRadius=.1 and HeadBodyHeight=.37 feed broadphase/head helpers. A future
  changing-width/height owner must update actual narrowphase and conservative
  broadphase domains,not just an unrelated pair of floats.
- No explicit form+3c owner exists yet. Native fieldMarioBattle.flags supplies
  some main flags while fieldIdle.mainFlags supplies others;consumers sometimes
  OR them. Source background eligibility/speed/talk must consult the intended
  shared mainflag owner,not an unused isolated flag word.
- fieldCameraFlags is consumed by FieldCameraTarget and is a meaningful flag10
  target. Mario display flags are currently split between
  fieldCameraDisplayFlags (publishFieldCameraTarget/MarioCameraTarget) and
  marioPresentation.displayFlags (render/pose),with additional idle state.
  Source display20000 must reach its real camera producer;fieldCameraEvents
  is a separate owner and must not be reset as a shortcut.
- Native restore currently retains camera events/clocks and selected
  presentation fields while explicitly clearing camera flag10. That is an
  existing room adapter policy,not evidence for arbitrary source follow-reset
  lifetime or retained transformed form.

Smallest faithful admission can remain ordinary form0 with known normal
flags and real live dimension/speed/camera consumers. Background-off in that
domain writes existing normal dimensions/speeds but still has real shared
flag/follow side effects. Background-on additionally affects speed/talk and
paper camera composition,so a full generic success claim requires those real
consumers. Keep unsupported transformed/unknown owners pending rather than
populate a byte image which no gameplay code reads.

## Evidence

Fresh complete DOL listings artifacts/bgmode-marioInit.txt,
bgmode-marioReInit.txt,bgmode-marioMoveMain.txt,bgmode-marioMove.txt,
bgmode-marioChgMoveMotion.txt,bgmode-marioGetScale.txt.
Background/follow exact listings artifacts/bero-prefix-marioBgmodeOff.txt,
bero-prefix-marioBgmodeOn.txt,bero-prefix-camFollowYOn.txt,
bero-prefix-camFollowYOff.txt. Original constants/symbols are pinned by
artifacts/reference/us_symbols.csv. Current native consumers were inspected
read-only;no source/shared-doc/git/build changes or runtime/raster claims.
