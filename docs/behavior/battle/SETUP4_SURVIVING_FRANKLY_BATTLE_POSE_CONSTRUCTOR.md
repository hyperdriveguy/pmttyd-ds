# Setup4 surviving Frankly battle pose constructor

2026-10-14. Original US G8ME01 disc0 revision0, read-only tik REL and
c_kuri_h resource. Native source read-only. This identifies the real surviving
nonplayer owner at the finishing-display constructor boundary. It does not
skip it or claim full NPC battle drawing/field return.
Existing request scope: [EVT-01](../../requests/EVT-01.md),
[trio setup4](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md),
[pose entry/release](../display/BATTLE_PART_POSE_ENTRY_RELEASE_AND_OFFSET_SERVICES.md).

## Actual surviving registry owner

Setup4's fourth original group row at tik file26430 points to original
unit_lecture_frankli_kuriboo kind file3d990, type0xb4 (decimal180).
Native currently maps that owner to unit6. It has max move count0 and is not
an opponent to remove on ordinary enemy defeat. Once the three enemies have
actually been deleted from their registry slots, it remains after Mario and
Goombella in the display loop; this is the concrete remaining constructor,
subject to the live registry rather than an assumed global empty-unit domain.

Original imported address mapping uses section base93000000+i*100000:

| Source owner | Original file offset | Relocated native bank address |
| --- | --- | --- |
| Kind180 | 3d990 | 9352ff60 |
| Part table/sole part1 | 3daac | 9353007c |
| Pose table | 3da7c | 9353004c |
| Init event | 3daf8 | 935300c8 |
| Entry event | 3dbcc | 9353019c |
| Phase event | 3dc40 | 93530210 |
| Damage event | 3dec0 | 93530490 |
| Wait event | 3dee8 | 935304b8 |

Part model pointer S8 resolves93405a3c, string **c_kuri_h**. Source descriptor
P4 must be the actual table9353007c, not the synthetic kind+100 placeholder.
At inspection the native unit6 constructor copies the real kind pointer but
leaves that placeholder part descriptor; this cannot supply the actual model
name/table merely because source part attributes were copied separately.

## Kind/part fields and source pose table

Original kind HP10/FP0, kind width HALF16=28,height HALF18=24;
unit attribute12000000. Part ID1, attribute00890009,counter0, local XYZ0,
hit offset(0,25,0), sampling offset(0,30,0), signed S30/S32=20/30,
default alpha signedS34=255. Defense and defense-attribute arrays are five
zero bytes each, pointers93530024/9353002c. Kind/part/pose pointers are already
available in imported tik data; no invented definition is needed.

| Ordered pose row type | Original source string |
| --- | --- |
| 27 | D_1 |
| 28 | S_1 |
| 31 | D_2 |
| 39 | D_2 |
| 69 | S_1 |
| 0 | null terminator |

Strings D_1,S_1,D_2 resolve93405a30/34/38. Preserve source case. Unlike the
ordinary field tribe's lowercase d_1 discrepancy, these battle table requests
actually match uppercase D_1/D_2 in the model directory. Do not apply the field
tribe fallback rule to the battle table's correct spelling.

Apply the original common part defaults from
[part Entry scalar contract](TRIO_PART_ENTRY_SCALE_RADIUS_AND_ROTATION_DEFAULTS.md):
both part scales1,P78=1, actual radii20/30, rotation/pivot0, source side,
local/hit/sampling triplets, pose rates1,colorffffffff and registered handle.
Kind width/height copy to Ucc/Uce; they are distinct from part radius/height.
Source model attrs/frame drawing remain their own services.

## Constructor selection and actually used source animations

btlDispEntAnime801011fc reads actual P1bc and S8, searches type28→S_1,
copies S_1 into P1c4, then requests animPoseEntry(c_kuri_h,mode1) and publishes
the returned distinct live pose handle. It does not call SetAnim. Original
Entry's initial selector is directory index0 **Z_1**, independent of that copied
idle string. Later original pending/automatic pose service controls animation
selection; do not force S_1 during factory admission merely because it is idle.

Kind init installs wait/entry/phase/damage/confusion scripts, publishes lecture
unit ID, work0=0, places Frankly(0,-1000,0), faces1, setsmaxmovecount0 and
starts wait. lecture_init itself returns2; an inventory's speculative ASCII
identifier from code bytes does not replace the actual function symbol.
Entry waits900ms, positions(-250,0,-30), requests **R_1**, moves to
(-60,0,-30) with original duration45/speed-1, then requests **S_1**.
Wait event requests AnimeChangePoseFromTable(-2,1). First-turn phase uses
source **T_1/S_1** message talk/stay callbacks and source first-turn conditions;
later phases skip tutorial dialogue. These are authentic animations already
used before ending, not substitutes for unported enemy art.

Automatic status selection can request table27 D_1,31/39 D_2; current no-status
ordinary idle table28 uses S_1. A native resource owner should retain unavailable
unconverted names explicitly rather than mapping D_2 to another descriptor.
Source missing-name fallback is descriptor0 only for a name genuinely absent
from the full original directory, not a converted-subset omission.

## Existing original resources and smallest native bank admission

Private local a/c_kuri_h is24372 bytes, SHA256
5ca1795ce8a9729e661da14742178fe7264674861a4e7101506238cc0ba46247.
Its17 source descriptors in order are:

```
Z_1 S_1 W_1 R_1 T_1 A_1 W_2 A_2 A_3 A_4 S_2 T_2 S_3 T_3 D_1 T_4 D_2
```

Original MODEL+c4=1; opaque TestXLU(mask8) false. Every descriptor has nonnull
animation data. Z_1 serialized timeline offset30bc; S_1 3158; R_1 3a08;
T_1 4010; D_1 5a7c; D_2 5dc0. These are file offsets, not loaded pointers.

`tools/professor_assets.py` already extracts a/c_kuri_h and a/c_kuri_h-, and
converts **Z_1,S_1,T_1,W_1,R_1,D_1** to private romfs/professor.pmanim with
professor.t3x atlas and professor.bin UV metadata. All required files existed
at inspection. It explicitly verifies source descriptor0 leads the native bank.
This is the same authentic art/resource, usable by the battle typed CPU owner
with a new complete PMPOSE02 original name/modelflags/discrete timeline sidecar.
No current professor.pose sidecar exists. Existing BattlePosePool immutable-bank
loader can share this converted resource/atlas rather than duplicate art or
invent an NPC image. D_2 is original but absent from the converted six-pose
subset; import it when those source statuses are admitted, or retain that exact
evaluation boundary. Reusing the bank does not equate field mode2 pose handles
with battle mode1 ownership/refcounts or automatically bind current pose history.

## Source evidence and remaining boundaries

Read-only local tik REL source SHA256 is retained in the existing setup4
contract. Kind file3d990:c4 hash:
2d06f47a8156403a179f215cf9d1fd22f167810d92ca992682ac425b8ee60cfe.
Part file3daac:4c hash:
379025de8f831a48f00c84232480b8857ec50823f8e8ff88c4332ff7494ee480.
Bindings/table rows were resolved with tools/rel_event_inventory.py's actual
local/import relocation map; source event listings from private trio-init-events
inventory retain source callback addresses/operands and chronology.

Typed resource admission, original part metadata/default initialization and
remaining actor pose history are the next concrete constructor work. Complete
status-specific effects, original unit-part GX/shadows, resource release/global
cadence and field-return sequence remain separate providers. No native code,
shared index/progress/assets changes or commit for this documentation task.
