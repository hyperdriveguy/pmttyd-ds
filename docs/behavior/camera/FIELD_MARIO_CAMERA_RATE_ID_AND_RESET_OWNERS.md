# Mario camera follow rate, ID and reset owners

US G8ME01 revision0; recovered2026-10-13 for
[OW-CAMERA-01](../../requests/OW-CAMERA-01.md). Eight complete local leaves
801a9434..801a9604 (116 instructions), plus a direct DOL caller census.
Addresses/masks/fields hexadecimal; counts decimal. Original static deductions;
no measured camera/pause/cadence or native visual equivalence. Read
[shift/target/update controls](FIELD_MARIO_CAMERA_SHIFT_TARGETS_AND_CONTROLS.md)
and [camera lifecycle/projection](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md).
The latter already describes bounded ordinary follow and projection; this
contract closes complete reset/write bodies, raw templates and direct owners.

## Follow rate is separate from the three shift rates

Every leaf requests marioGetPtr8005e0c0 exactly once, using its returned owner
without a null/active/generation/extent check. No IRQ guard or call-result gate.

| Leaf / complete address:size | Request and read/write |
| --- | --- |
| marioGetCamFollowRate801a9434:24 | Request at9440, load owner.float15c into f1 at9448 |
| marioSetCamFollowRate801a9458:30 | Preserve supplied f1 in f31 BEFORE request946c; store single to returned owner15c at9470 |
| marioResetCamFollowRate801a9488:38 | Load original float3e4ccccd at80424adc into f31 BEFORE request94a0; store single to returned owner15c at94a4 |
| marioSetCamId801a94c0:30 | Preserve supplied raw r3 word in r31 BEFORE request94d4; store to returned owner134 at94d8 |
| marioGetCamId801a94f0:24 | Request94fc; load returned owner134 into r3 at9504 |
| marioInitCamId801a9514:30 | Preserve literal4 in r31 BEFORE request9528; store returned owner134 at952c |

The follow-rate reset word is binary32 approximately0.20000000298. Neither
setter clamps sign/range/finite values or rejects camera IDs. Set/get134 is a
stored ID field, not a request to camGetPtr or proof that camera4 exists.
The setter stores after the child, so supplied value and returned owner have
different capture times. GetFollow returns f1; GetId returns r3. Other leaves
have no independently defined scalar result. No implicit reset of other rates.

Camera follow rate15c is distinct from the shift rates150/154/158 consumed by
marioUpdateCam. The lifecycle contract links15c to cam3dMain vertical target
follow; it does not authorize a universal .2 rate for every axis or rendered
frame. These leaves do not write final camera matrices/eye/at or native bounds.

## Shift rate reset preserves current and target channels

marioResetCamShiftRate801a9544:64 requests owner at801a9550, then writes in this
exact order, with original load/copy sites pinned:

| Write order | Owner field | Original raw word / source |
| --- | --- | --- |
| 1 | 150 | 3cf5c28f (.02999999933),80424ae0 |
| 2 | 154 | 3d75c28f (.05999999866),80424ae4 |
| 3 | 158 | Same retained float as154 |
| 4 | 64 | 00000000,80424aa4 |
| 5 | 68 | 40000000 (2.0),80424ae8 |
| 6 | 6c | ffffffff, raw word8030a754 |
| 7 | 70 | 00000000, raw word8030a758 |
| 8 | 74 | 00000000, raw word8030a75c |

This body does NOT write target138/140/148 or current13c/144/14c. Values6c/70
are loaded together before their stores;74 is loaded later. Preserve the raw
word copy, including ffffffff; interpreting the template as three ordinary
float zeros changes the source. Semantic meaning/other consumers of64..74
remain separate. No memset of a larger range or whole camera structure.

## Shift target reset does not snap current channels

marioResetCamShift801a95a8:5c requests owner at801a95b4. Ordered writes:
138=0,140=0,148=0,64=0,68=2.0; then copy raw template8030a748/4c/50 to6c/70/74,
respectively, all three ffffffff. Same word-before-store order as rate reset.
The original zero/2.0 float words are shared with that reset above.
No write to13c/144/14c,150/154/158 or15c. An independently admitted reset does
not erase retained shifts or copy target into current; later update approaches
the targets according to existing rates. No local heading/flags/ID reset.

ABI: all leaves use16-byte frames except ResetFollow32 bytes; follow setters
save f31, ID setters r31; ResetFollow additionally preserves paired-single f31
state. Each restores its saved LR/frame/registers. No wait/timing counter.

## Direct source owner census and boundary

[Manifest](../../../tools/mario_camera_defaults_evidence.json) scans all seven
DOL text sections for direct branch-with-link requests to these eight entries:
40 sites, each original call word pinned. Caller names resolved against the US
text-symbol ranges; census counts requests, not runtime admission. Not a REL,
indirect pointer, tail-branch or full caller-branch census.

- marioInit requests InitCamId at8005b628; later ResetShift/ResetShiftRate/
  ResetFollow at8005ba0c/10/14. marioReInit requests GetCamId8005b21c and
  ResetShift/ResetShiftRate/ResetFollow at8005b558/5c/60. Their calls alone do
  not prove other initial values or all invocation conditions.
- _unload requests InitCamId80007b00; EVT wrappers evt_mario_set_camid/
  init_camid request setters at800e72d8/72fc. Item sequence and stone_bg also
  request ID writes; requested numeric IDs/branches remain caller-owned.
- Ordinary motion owners dash/walk/land/fall/jumpSmall/jump/jumpNpc/jumpSw/
  stay request ResetFollow. mot_fall additionally requests GetFollow8009b11c
  and SetFollow8009b13c; a reset default must not override those later writes.
- cam3dMain requests GetFollow at8000ed28. marioPreDisp and party constructors/
  reinitialization read the ID. kpa_jumpNpc requests ResetFollow; kpa_powdown/
  powup request ResetShiftRate. Full source sites are in the manifest.

Validation:16 spans/167 anchors (all116 leaf words,11 constant/template words,
40 direct caller words),eight local direct calls, no indirect calls. Complete
write layouts/word order/target preservation, body sizes/source hashes/constants/
caller census checked independently. Reset follow/rate leaves link the original
values already recorded by lifecycle documentation; no original execution claim.
Raw ELF/listing/generator stay `/tmp`; no binary assets/raw bodies committed.

Remaining: actual caller admission/ID/value/owner/FPSCR/alias/lifetime domains,
other direct field writers and REL/indirect owners, authored road solver/limits,
zoom/event/camera4/renderer consumers, original follow/room/pause/control traces
and native numeric/visual binding. OW-CAMERA-01 remains OPEN.
