# Underground battle camera4 entry and inherited lens history

2026-10-14. Original US G8ME01 disc0 revision0, read-only. This identifies
camera4 ownership across setup4 transition and necessary action modes before
victory. It is not a complete original camera trace or a full mode-controller
port. Offsets/addresses are hexadecimal; mode numbers are decimal.

## Boot entry and actual retained fields

`camInit8000fd9c` calls `camEntryPersp8000fac8` at80010404 with callback
cam3dMain8000ea68, source camera name, f1 FOV25,f2 aspect3fa22222,
f3 near1,f4 far32768,f5 depth0,f6 depth1. Returned pointer is published into
camera4 slot803c9608 at80010418. Camera4 is a persistent global object, not a
new per-BattleWork record. Source field owner/link:
[camera lifecycle](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md).

Entry allocates heap0 length260, explicitly memset0 across260, builds
projection C15c with supplied FOV/aspect/near/far, then writes projectiontype
C19c=0. It copies mutable SDATA vectors80414fb0,80414fbc,80414fc8 into eye,
at,up respectively. Loaded fresh data is eye(0,0,1000),at(0,0,0),up(0,1,0).
This pins boot data, not immutable live values after later writers.

Entry stores near/far C30/34, FOV/aspect C38/3c, callback C1ec, mode HALF2=0,
submode HALF4=0, and explicitly zeroes C14c roll and C158/154/150 translation
in that order at8000fc0c..18. The view matrix C11c remains zero from memset
until actual camMain constructs it. C1a0 projection-extra matrix is also zero
from memset, but flag HALF0 mask8 remains clear, so that matrix is not admitted.
Do not confuse C1e8=8 with HALF0 flags: these are different fields.

Viewport fields are derived from DEMOGetRenderModeObj, not hard-coded by the
consumer: source NTSC framebuffers supply608x480. Initial viewport depth is
0..1 from caller; viewport/projection upload belongs to camera display. Native
400x240 or tilted projection is a presentation adaptation, not this source data.

## Actual battle transition does not reset the lens

Sequence `battle_init800738ac:278` first obtains FieldBattleInfo and pouch.
At800738d8 it invokes `camSetMode8000e95c(4,2)`. This18-byte leaf resolves
the existing pointer from the global camera table and stores HALF(C+2)=2.
It does **nothing else**: no camera allocation, FOV reset, perspective-type
change, near/far reset, roll/translation zeroing, flag cleanup or matrix copy.

Later BattleInit800f8f04 stores incoming Work at B2738 and calls
`btlDispInit801030f8` at800f8f6c. That wrapper first calls
`battleCameraInit8012dee8` at8010310c, then display init80119da0, then ORs
Bef8 mask2. BattleCameraInit clears B2754..2857 only. It does not touch the
global camera4 record. Its initializer's sole direct DOL caller is this
btlDispInit site. camSetMode's direct DOL callers are battle_init and the two
battle_exit sites80073738/48; this census does not include indirect/REL calls.

Consequently underground arrival, authored field roads and field camera EVT
history precede the inherited lens/roll/translation/projection-type used by
mode2 battle. Fresh camera-entry values may be used only with proven no-later-
writer history, or as an explicitly supplied native snapshot adaptation.
Instantiating camera4 anew at battle entry and presenting its boot lens as the
original underground transition would erase real field history.

In admitted mode2 and GP14 nonzero, cam3dMain calls battleCameraMain then
builds C15c perspective from **current** C38 FOV,C30 near,C34 far and fixed
original aspect8041f6d8. C3c is retained but not the aspect operand in that
particular rebuild. camMain then builds C11c LookAt and applies current roll
and translation. See [exact depth/matrix services](../display/BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md).
That is the missing publication producer, not a direct write in the mode setter.

## Concrete pre-victory action mode requirements

Existing typed native camera supports default14/15/16, timeline3 and victory
17/18. Those handlers cannot establish complete earlier history because original
used action/lesson graphs contain additional modes. Static original script
inventory gives the following exact immediate requests; branch admissions and
runtime tick timing remain separate:

| Original event | Mode requests and command addresses |
| --- | --- |
| marioAttackEvent_NormalJump | mode10 at80355d64; mode11 at80355e30; mode0 at80356640,80356890,803568d4,80356b38 |
| partyChristineAttack_NormalAttack | mode11 at8039423c; mode0 at80394ba4,80394bf0,80394dd8 |
| marioAttackEvent_NormalHammer_Core | mode7 at8035d724; mode11 at8035d834; mode0 at8035e4b8,8035eacc |
| Frankly setup4 phase_event in tik REL | mode3 at file3dcd8 and3de18; mode0 at3ddc0 and3de90 |

All listed requests use priority0. NormalJump's mode10/11 homing/zoom requests,
Headbonk's mode11 and Hammer's mode7/11 are authored attack consumers, not
evidence all their branches execute for every target/result. The actual source
attack regression and native request history can resolve which are visited.
Frankly's first-turn phase hook supplies real mode3 pan targets and returns
mode0; later turns skip that tutorial branch. Existing
[setup4 contract](../battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md)
retains its complete command list and conditions.

Smallest concrete next history handlers are therefore mode0,7,10,11 plus
their original homing/zoom/speed inputs, followed by any admitted enemy-mode
requests. This source census does not justify demanding an unrelated complete
0..13 port before reviewing those specific handlers. It also cannot justify
marking native cameraHistoryComplete true merely when ending overwrites mode17:
mode17 smooths from its retained prior eye/at and thus consumes earlier history.

## Native owner guidance and evidence

Retain an explicit global camera4 snapshot with lens/roll/translation/type,
up/eye/at and actual matrices. Import source field-camera history or declare
its numerical/presentation adaptation. Run source mode2 callback at simulation
cadence; depth/draw requests only read its published matrices. Preserve incomplete
history admission until the actual visited original handlers have providers.
Separate boot initialization, field state, B camera request state, mode update,
and display matrix publication; a C3D rendering camera cannot silently become
all five owners because its screen output looks plausible.

Read-only evidence under ignored artifacts/recovered-temp-evidence:
camera4entry-camInit/persp, camera4transition-battle_init/BattleInit/btlDispInit/
camSetMode and previously recovered calcdepth matrix listings. Existing private
JSON source inventories trio-jump-events,trio-headbonk-events,trio-hammer-events,
trio-init-events supplied the static callback rows, not fabricated commands.

| Source body | SHA256 |
| --- | --- |
| camEntryPersp8000fac8:2d4 | 8124e9f8dda6fb8323a6b001a4d5cf9ea9b7669bfdf8ee1e666f0e28f253931b |
| battle_init800738ac:278 | ea1bbc5bf6aef6b62b5ec39a508c0d05777f97aa8648389a01a2e71ceaf214de |
| btlDispInit801030f8:3c | 5804a4be99cfe95361eb72dd0ac12d93746eebc064217a2b6bfbd9f29557dbf2 |
| camSetMode8000e95c:18 | 1eac0aa2835f9aea8242d69f156ebdaf33726b856050ca787907a6e769ba41de |

Documentation-only; no native/shared documentation changes, original execution
claim or commit.
