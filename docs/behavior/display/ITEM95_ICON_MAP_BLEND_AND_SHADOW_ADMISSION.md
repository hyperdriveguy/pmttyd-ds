# Item95 icon map blend, deletion synchronization and shadow admission

US G8ME01 disc0 revision0; read-only source recovery2026-10-13. Supplemental
source contract to [entry/icon ownership](../field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md).
No native code, asset conversion or original execution evidence supplied here.

## Fresh ordinary plaza blend state

The flag read by iconEntry is U16[current MapWork+4]&2. This is runtime map
blend state, not authored texture/material metadata. mapLoad80028d08/68 clears
all2f4hex bytes of ordinary MapWork at80028d34..3c, selects map index0,
then calls _mapLoad80028f18/be0. The reviewed loader's direct entry-flag
writers set/clear8000 (map-version branch800294b8..d8) and1 (fog branch
80029674..b0). Neither writes bit2. mapSetBlend8001d9c4/28 writes RGBA at
current-group+16hex then ORs flag2. mapSetBlend2 instead ORs4 and writes+1a;
its name does not mean the flag2 writer.

Fresh gor.rel ADDR32 import census finds evt_map_set_blend80038ac8 only at
2cd2c/2cd48 in gor_00 epilogue_evt and3f120/3f934 in gor_02
powerupya_party_lank_rtn. Its evt_map_blend_set_flag imports likewise belong
to those powerup events/epilogue_npc_entry. There is no blend call in the
complete gor_01_init_evt, Goomba3 talk or quest2 acquisition event span.
Thus the reviewed fresh ordinary plaza load/room initialization does not set
flag2 and does not add iconflag40. This is a finite admission statement:
unreviewed shared/system event writers or explicit later native blend commands
can still change the runtime flag. Preserve map blend state as a provider input
for future rooms; do not globally discard iconflag40 or declare every story
state of every room free of this branch.

## Flag40 is a separate three-stage shader

iconGX tests flag40 at80019a04..0c, before ordinary shader and the later
flags10/80/100 map extra-stage bypass. Flags100/80 therefore **do not bypass**
the flag40 shader. The branch80019a10..c50 requests three TEV stages, register1
RGBA555555ff from8041f86c and register2 equal retained icon color.
Swap tables1/2/3 select (R,R,R,A), (G,G,G,A), (B,B,B,A); each used
stage requests raster swap0 and texture swap1/2/3 respectively.

Raw GX requests, with stage number as first argument:

| Stage | ColorIn(A,B,C,D) | AlphaIn(A,B,C,D) | TevOrder(coord,map,color) |
| --- | --- | --- | --- |
| 0 | (15,2,8,15) | (7,7,7,4) | (0,0,255) |
| 1 | (15,2,8,0) | (7,7,7,4) | (0,0,255) |
| 2 | (15,2,8,0) | (7,2,4,7) | (0,0,255) |

All color/alpha operations use ADD0,bias0,scale0,clamp1,outreg0.
The request graph sums three separately sampled texture channels weighted by
register1=85, then multiplies final texture alpha by register2 alpha. Its RGB
is gray; register2 RGB does not appear in these ColorIn requests. Translating
this to one floating-point average does not prove the same staged fixed-point
rounding. Geometry, UV and later alpha/depth/blend requests remain the same
as the [ordinary icon contract](../field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md).
The branch resets texture swap tables1/2/3 to0/0 after drawing at8001a134..160.
The finite ordinary plaza flag-clear path can use ordinary rendering;
a future flag40 path needs this shader or an explicit unsupported-provider
boundary, not a silently colored icon.

## No post-delete icon synchronization

After dispatching itemseq_GetItem at800ae340, itemMain converges800ae45c.
800ae45c..464 reloads item mainhalf0 and tests activebit1 again. If successful
Get deleted the matching item names, it jumps directly to next-record iteration
at800ae5f4; it does **not** run later iconSetPos/Scale/Alpha. For surviving95,
800ae508..530 syncs item XYZ, scale and alpha into first-active named icon.
Success clears iconflag100 and80 before name-wide deletion in the same Get
owner call. There is no source-supported same-call camera4 return icon draw
or extra icon synchronization after deletion.

## Shadow request is independently owned

iconMain's admitted camera5/7/default branches call shadowEntry8002be98/ec
with current iconXYZ and radius10 (8041f874=10). This request is independent
of the icon GX quad; do not bake a synthetic ellipse into the40x40 texture.
shadowEntry allocates the next queued5chex-byte record at owner+108, advancing
count+104, without a local full-pool guard. It copies XYZ, radius+10, owner
vertical size+114 into+14, owner type+110 lowbyte into+2, zero+3,
collision index-1 at+18, and **camera4** at+1a. The source request does not
inherit the icon's camera5. It copies owner+118 into recordflag1, then stores
+1c=1, +20=.20000000298023224, +24=.4000000059604645, alpha+28=255.

shadowInit8002dcec/4f8 sets owner type2, owner collision-enable+118=1 and
count0, allocating1700hex bytes (64 records). Later setters can change these
inputs; default initialization is not a proof every room retains them.
shadowMain8002db8c/160 registers shadowDisp8002d3d8/7b4 at camera4/1/5/7,
display type9; additional camera2 passes own preparation. After registration
it resets queued count and owner flags/counters. The preparation/submission
schedule must preserve request lifetime through drawing; calling it in the
wrong order would lose the queue.

shadowDisp camera1/4/5/7 invokes polygon service, cylinderShadowDraw8002bf84,
and texture service. Polygon8002c818/2dc requires nonzero radius, type0,
preparedflag2 and recordcamera==supplied camera. Texture8002c484/394 requires
nonzero radius, type1, preparedflag2 and the same camera equality; it loads
base effect texture44. Default queued type2 selects the separate cylinder
owner, not these polygon/texture paths. Collision preparation8002d16c/214
writes the retained matrix and marksflag2, with a separate collision-enabled
branch and authored scene-hit providers. Full cylinder projection, collision,
shadow color, view state and queue ordering remain open here. These bounded
requests do not establish a native matched shadow or license a screen-space
ellipse approximation as source fidelity.

## Evidence and validation

Original DOL spans reread into ignored /tmp/item95-* listings/ELFs: mapLoad,
_mapLoad, mapSetBlend/2, iconGX, itemMain, shadowEntry, shadowMain, shadowDisp,
shadowInit and polygon/texture/collision shadow leaves. Original constants
were mapped through DOL sections; gor.rel ADDR32 bindings and all
gor_01_init_evt calls were inventoried with existing read-only parsers.
The source ISO was opened read-only; no profile/save or implementation change.
Documentation link validation is reported by the integrating session.
