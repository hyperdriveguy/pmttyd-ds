# Ordinary NPC dimensions, talk anchor and display registration

US G8ME01 disc0 revision0 read-only static DOL evidence. Scoped follow-up to
[ordinary yaw/matrix](NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md),
[ordinary room bindings](../field/ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). These are local owner
requests, not an original-runtime or complete npcMain claim.

## Dimensions remain separate from rendering scale

`evt_npc_set_tribe`0x8004e5c0 installs the resolved tribe pointer at NPC+0x28,
loads tribe+0x28/+0x2c and stores their floats directly at NPC+0x14c/+0x150
(0x8004e618..624). It does not multiply by model/draw scale or infer dimensions
from a bounding box. Source selected tribe widths/heights:

| Tribe record | Width +0x28 / height +0x2c |
| --- | --- |
| Professor c_kuri_h / 0x80333014 | 30 / 30 |
| Chu c_tyusan / 0x80333554 | 30 / 30 |
| NPC Goomba c_kuribo_n / 0x8033335c | 30 / 30 |
| Sailor Bob-omb c_suifu_b / 0x8033320c | 20 / 20 |

`npcEntry` initially queries animation height/radius into these fields; later
tribe assignment supplies the values above. NPC actual render scaleXYZ is
+0xe0/+0xe4/+0xe8, initialized1, separate from dimension fields and the
additional animPoseDrawMtx scalar2. Neither dimension should replace the
retained source geometry scale. `evt_npc_set_tribe` additionally copies
pose pointer+0x10 to NPC stay-name buffer+0x4c, +0x14 to talk-name+0x6c,
and +0x8 into current pose-name+0x2c and requests restart1 when nonnull.

## Exact ordinary NPC print anchor

`_evt_msg_print`0x800d28d4 initializes its message state. If command flag2
is clear it admits field speaker resolution: event byte+0x10==6 supplies
NPC pointer from event+0x170; a nonnull explicit speaker overrides that pointer
through evtNpcNameToPtr(0x8004f5d8). Mario and party sentinel paths have their
own branches; only the ordinary nonnull NPC branch is claimed below.

At0x800d2b50..2bac:

1. Null resolved NPC skips this branch entirely.
2. Load EventEntry+0x180; if AND0x1effffff is nonzero, skip anchor copy.
3. Otherwise copy current NPC rootXYZ+0x8c/+0x90/+0x94 to message
   stateXYZ+0x30/+0x34/+0x38, then add NPC height+0x150 to Y.
4. In either mask branch, copy NPC name+0x8 into the message owner's
   current-speaker buffer (owner base+0xf204).

The height is added once, directly, without bbox top, model scale, pivot or
camera correction. Under native map-units*.01, ordinary Chu/Frankly anchorY
is rootY+.30; sailor anchorY is rootY+.20. These adapter units do not change
source dimensions. Explicit event anchor override, projection, bubble placement
and later updates remain separate owners; do not apply this NPC branch to
Mario/party speech or overwrite a caller-supplied anchor.

## npcMain pre-display admission and color

Bounded block0x8004acd0..0x8004b10c of `npcMain`0x80049a7c:

- NPC flags+0 mask0x800 skips the display section to0x8004b10c.
- It projects current root with camera4 matrices+0x11c and+0x15c. Screen
  admission requires projected Z strictly(-1,0), X and Y inclusively[-2,2].
  When admission fails, flags mask0x40 permits continuation; otherwise skip.
- RGBA bytes come from NPC+0x114..117. If any differs from255, pose flag0x40
  is turned on and animPoseSetColor receives the packed current RGBA. All255
  turns pose flag0x40 off. These are source color requests, separate from
  the earlier NPC+0x1d4 mask0x400000 darkening inside npcDisp.
- Display sort key is `-clamp(fmadds(5000,projectedZ,5000),0,10000)`;
  preserve the fused operation/order rather than treating depth as world Z.
- NPC flags mask0x80 skips main geometry registration to offscreen handling.
  HalfwordNPC+0x320 mask2 also suppresses main registration unless
  NPC+0x1d4 mask0x10000 is set.

For admitted main geometry, alphaNPC+0x117!=255 registers npcDisp_xlu once
with dispEntry(NPC+0x1f4,type2,callback,NPC,sortKey). Alpha255 registers
npcDisp three times in order types0,1,2. Each source callback's inner pass
admission is still governed by the animation resource and current display mode;
registration itself does not prove a particular pass emits geometry.

After main registration, flags mask0x100 independently registers npcDisp_blur
with type2. Offscreen handling is disabled when NPC+0x110==-1. Otherwise
alpha!=255 registers npcDisp_offscreen_xlu once at layer1,type2; alpha255
registers npcDisp_offscreen three times at layer1,types0,1,2. These are raw
source flag relationships, not inferred semantic names for every flag.

## Stay/talk pose request helpers

`npcSetStayPose`0x80049644 and `npcSetTalkPose`0x8004979c perform name routing
before ordinary NPC lookup. For the ordinary NPC case, both scan the live
832-byte registry for active flag1 plus matching name; only NPC flags mask0x200
permits a pose request. Stay requests NPC+0x4c, talk requests NPC+0x6c,
each through animPoseSetAnim(poseID,name,restart1). They do not lowercase
pose names or derive a talking pose from the bank. Mario/party/name-sentinel
routes require their respective services and are outside this ordinary branch.
These helpers are request leaves, not evidence of when interaction should
invoke them or how window lifetime returns to stay.

## Facing entry state and camera-correction orientation

Fresh `animPoseEntry`0x8004176c selects a pose slot whose flags word is0,
ORs active1 at0x80041828, and writes float0 to facing input+0x70,
retained angle+0x74 and target angle+0x7c at0x80041b1c/20/24. The zero
literal is0x8041fb28. **It does not initialize sign+0x78.** Retained raw
pool/previous-slot content cannot be promoted to a source +1 initializer;
an admitted Main call owns the sign before the ordinary draw path.
`npcEntry` calls animPosePeraOn, whose local leaf ORs flag8; the stable
fresh ordinary flags therefore become9, with snap mask0x100 clear. Other
writers/reused-resource ownership and original first-Main ordering remain
explicit dependencies.

`mtx_setup` uses system `angleABf`0x8000703c, not the same-name event.o
body0x800dcbc8. It subtracts endpoint-startpoint in binary32: deltaX=f3-f1,
deltaZ=f4-f2. Source orientation is -Z0,+X90,+Z180,-X270, coincident0.
Thus an atan2-equivalent orientation is atan2(deltaX,-deltaZ), followed by
normalization; this does not establish atan2 numeric equivalence to the
original lookup routine. The routine splits at abs(deltaX)>abs(deltaZ),
uses a minor/major ratio, binary32 constants45 and2 to select a rounded
index into table0x803041e0, and uses the ratio times selected table value
before quadrant reconstruction. Constants:45 at0x8041f484,2 at0x8041f488,
90 at0x8041f48c,270 at0x8041f490,180 at0x8041f464,0 at0x8041f448.
Its index rounding adds double0.5 at0x802bf1f8 before integer truncation;
this source approximation is a numeric dependency, not measured behavior.

Camera correction request order is unchanged: current-camera(c,+14) to
camera target(+18,+20); same camera(c,+14) to NPC(+8c,+94); subtract
first-minus-second with fsubs, then reviseAngle. NPC flags0x02000000 or
0x08000000 suppress both calls and retain correction0. Matrix extra yaw
NPC+f0 is independent of source event ry+144 and animation-facing input.

## Read-only reproduction and boundaries

Fresh `tools/inspect_dol_function.py` disassembled npcMain,evt_npc_set_tribe,
_evt_msg_print,npcSetStayPose,npcSetTalkPose against the supplied read-only ISO
and existing external US symbols. Direct ISO reads checked all four tribe
float pairs and SDA constants with r2 base0x80427440: -1 at0x8041fcd8,
0 at0x8041fc7c,-2 at0x8041fd5c,2 at0x8041fd34,5000 at0x8041fd60,
10000 at0x8041fd64. Inspection ELFs/listings remain /tmp, outside history.

No native code changed and no build/hardware claim is made. This closes the
local dimension/anchor-copy/registration questions, leaving full npcMain
movement/flags/lifetime, interaction arbitration, exact event/window anchor
updates, ordinary material/shadow services and matched original-runtime
verification open. Display matrix and camera-facing animation consumers retain
their separate linked contracts; event yaw is not world matrix yaw.
