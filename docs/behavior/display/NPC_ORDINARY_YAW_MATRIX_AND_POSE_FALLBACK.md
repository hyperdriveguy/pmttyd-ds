# Ordinary NPC yaw, draw matrix and exact pose-name fallback

US G8ME01 disc0 revision0, read-only static DOL plus original c_kuri_h resource.
Scoped follow-up for [OW-ROAD-01](../../requests/OW-ROAD-01.md) and the
[Professor first conversation](../story/PROFESSOR_FIRST_TALK_AND_FENCE.md).
No original GPU output or hardware/runtime fidelity is claimed.

## Explicit yaw writer and separate draw rotations

`evt_npc_set_ry` 0x80050988/0x64 resolves actor through integer event lookup,
resolves yaw through FLOAT event lookup, finds the actor with evtNpcNameToPtr,
then stores FLOAT directly to NPC+0x144 at0x800509cc. No normalization,
interpolation, display-matrix assignment or X reflection occurs in this leaf.
Professor scene yaw90/270 are these exact source event requests.

`npcDisp` 0x8004c50c/0x2d4 loads yaw+144, negates it and calls reviseAngle:
incoming animation angle A=reviseAngle(-yaw). It calls
`mtx_setup(NPC,&matrix,-1)`, invokes animPoseMain and requests
animPoseDrawMtx(poseID,matrix,pass,A,2.0). Scale2 is the binary32 at
0x8041fd34 (r2-30476, r2=0x80427440). The matrix's additive Y rotation
NPC+f0 is a distinct field; do not write event yaw+144 into it.

`npcDisp_xlu` 0x8004c7e0/0x2a0 requests pass1,pass2,pass3 consecutively,
each with a freshly loaded poseID, same A and scale2 after its Main call.
Normal npcDisp chooses its requested pass from current display mode. Original
c_kuri_h resource+c4=1 admits pass2 through the wrapper's special mask1 rule;
pass1/3 fail ordinary mask checks. Do not infer the actor is invisible merely
because the pass1 opaque call rejects. See [draw wrapper admission](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md).

## Ordinary matrix input and request order

`mtx_setup` 0x8004ca80/0x2b8 takes NPC,output matrix and history slot(-1 means
current root). NPC scalar fields+e0/+e4/+e8 are its actual scaleXYZ.
`npcEntry` 0x8004b894/0x250 initializes them from(1,1,1); no local factor2
is applied inside mtx_setup. Extra factor2 comes from the animation draw call
above, so unit scale maps model vertices to2 engine units per model unit,
or *.02 under the native engine*.01 adapter. Collision radius/height are
separate and must not replace model scale or geometry bounds.

Source matrix requests, in order:

1. Camera correction starts0. If NPC main flags0x02000000 and0x08000000 are
   both clear, query current camera; compute angle(camera Xc,Z14 → target
   X18,Z20), then angle(camera Xc,Z14 → NPC X8c,Z94); correction is
   reviseAngle(first-second), preserving both calls and their order.
2. Reflection multiplier starts1. Twice revise NPC additive yaw+f0; inclusive
   [90,270] changes it to-1. This reflection tests+f0, not event yaw+144.
3. Identity output; apply scale(+e0,+e4,+e8*reflection) when nonunit.
4. Apply translation(-pivotXYZ), pivot at NPC+f8/+fc/+100, when nonzero.
5. Construct Z rotation(+f4), X rotation(+ec), Y rotation(+f0+correction),
   in that order, each left-concatenated into the retained matrix when needed.
   Degrees multiply binary32 0.01745329238474369 at0x8041fd7c before SDK
   RotRad. This is the original rounded constant, not exact pi/180.
6. Apply positive pivot translation when nonzero.
7. Apply current rootXYZ+8c/+90/+94 for slot-1; otherwise use history entry
   NPC+a4/+a8/+ac plus12*slot. No billboard-origin bbox correction occurs.

Original SDK matrix arithmetic/alias and camera angle services are numeric
dependencies. The request order above does not claim NumPy/3DS operations
already reproduce all original rounding.

## Ordinary camera-facing animation producer

`npcEntry` calls animPosePeraOn at0x8004b9ac. Leaf0x800414cc/0x1c ORs
poseflags8, enabling the original animation camera-facing path. Therefore a
native pure Ry(-yaw) or unconditional X mirror is insufficient for ordinary
Professor presentation. [Main camera-facing sign](ANIMATION_MAIN_CAMERA_FACING.md)
and [inner draw facing](ANIMATION_INNER_DRAW_FACING.md) already recover the
complete local consumers. Incoming A=reviseAngle(-yaw) writes pose+70=A-90;
inner targets front/rear camera headings, unwraps retained angle+74 and advances
by single FMA0.1 per admitted invocation unless poseflag0x100 snaps. It then
rotates retained angle and reflects Z iff the stored Main sign+78 is exactly-1.
Main's sign update and inner smoothing have distinct timing; do not classify
directly from event yaw or promote a per-call recurrence to elapsed-time math.

With source camera heading0, event yaw90 gives incoming A270 and source
pose+70=180, while yaw270 gives incoming A90 and pose+70=0. Their eventual
front/rear sign differs. This conditional example is derived from the source
equations; actual house cameras, retained angles and first-draw ordering must
remain explicit inputs, not assumed fixed headings.

## Tribe dimensions and case-sensitive missing animation

Original Professor tribe0x80333014 names model c_kuri_h. Its+28/+2c binary32
fields are30/30; evt_npc_set_tribe0x8004e5c0/0x104 copies these to NPC+14c/+150
and binds tribe pointer at NPC+28. These are collision dimension fields, not
render scalar. npcEntry initially reads animation height/radius into those
fields; later tribe assignment supplies source dimensions.

The tribe has lowercase `d_1` at both+20/+24; the original model bank contains
uppercase D_1/D_2 and no lowercase d_1. Preserve that discrepancy.
Actual bank order:

```
0 Z_1; 1 S_1; 2 W_1; 3 R_1; 4 T_1; 5 A_1; 6 W_2; 7 A_2;
8 A_3; 9 A_4; 10 S_2; 11 T_2; 12 S_3; 13 T_3; 14 D_1; 15 T_4; 16 D_2
```

`animPoseSetAnim` 0x80041170/0x17c treats negative selector as a string pointer,
scans64-byte descriptors using case-sensitive strcmp, and on no match selects
index0 at0x80041208. Out-of-range nonnegative numeric selectors also select0.
Hence an actual source request `d_1` selects **Z_1**, not D_1 or S_1. If restart
argument is0 and selected index already current, it returns without restarting;
otherwise it assigns the index and resets source animation clock/key/fraction
state. Do not lowercase bank names, normalize source literals or substitute
the apparently intended D_1 without evidence of a separate caller.

## Reproduction and remaining boundaries

Disassembled directly from read-only source ISO with tools/inspect_dol_function.py
and artifacts/reference/us_symbols.csv: evt_npc_set_ry,npcDisp,npcDisp_xlu,
mtx_setup,npcEntry,animPosePeraOn,evt_npc_set_tribe,animPoseSetAnim.
Actor/model/tribe constants and bank descriptors were read directly; local
asset c_kuri_h was source-matched. Existing animation verifiers cover linked
facing/clock/draw consumers; this documentation adds concrete ordinary NPC
producer bindings, not a native implementation or complete scene verification.
NPC setup flag owners, runtime camera cadence, full material/shadow rendering,
source SDK numbers and actual emulator/hardware scene comparison remain open.
