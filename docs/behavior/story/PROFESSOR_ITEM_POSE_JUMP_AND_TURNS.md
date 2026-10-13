# Professor scene item, Mario pose bank, hop and turn services

US G8ME01 disc0 revision0, read-only source DOL/REL/icon resources.
Scoped follow-up to [first-talk](PROFESSOR_FIRST_TALK_AND_FENCE.md) and
[OW-ROAD-01](../../requests/OW-ROAD-01.md). Timing is reconstructed from
source commands/instructions, not original runtime observation.

## Display item00 kind0x60

Original itemDataTable0x803108a8 uses40-byte records. Kind0x60 record
0x803117a8 names original Japanese identifier `宝の地図うしろ`, name key
in_treasure_map, description key msg_treasure_map. Its u16+20 is0x13a=314.
`itemEntry` 0x800ad7f0/0x7e0 at0x800adce8–0x800add08 reads this halfword
and calls iconEntry, then iconSetPos,iconSetScale,iconSetAlpha. This item uses
an icon, not a character animation model or a guessed paper texture.

Source `icon.bin` offset-table u16 at2*(314+1) points to0x1346; its six
halfwords are(0,0,0,1,319,0). iconEntry0x8001b094/0x244 copies descriptor+8
to icon record+32; iconGX0x800197f8/0x9a4 reads that at0x80019914 into the
TPL texture-selection argument. Therefore source visual is **icon.tpl image319**.
Descriptor metadata other than the traced texture selector needs its own
consumer contract. Exact ordinary icon projection/size,
lighting and alpha owners remain separate from this resource binding.

The original first-talk event creates item00 with flag0x10 in evt_item_entry,
then item_flag_on2,scale0xf24a7cb3=0.5498046875. It appears at
MarioXYZ+(20,20,10), moves to(-140,0,-220), returns to MarioXYZ+(20,20,10),
then hides at(0,-1000,0). Source positions/waits are already in first-talk's
full command timeline. A display entry is not evidence of an inventory grant.

## M_C_3 selects the field effect bank

`evt_mario_set_pose` 0x800e6840/0x154 excludes special current motions
24/25/22/21, then resolves the original pose string. Ordinary path searches
the null-terminated basic-name table0x8032dd60 with strcmp. M_C_3 is absent;
the unmatched path at0x800e693c stores requested name to Mario+18,
sets pending0x1000 in Mario+0c and sets display0x10000000 in Mario+4.
Matched basic names instead clear that display bit. Main special flag0x80000000
has its separate current-character path and is outside ordinary Mario here.

[Ordinary pose selection](../overworld/OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md)
recovers normal variant0's triplet a_mario,a_mario_r,e_mario and the retained
front/rear/effect handles22c/230/234. PreDisp selects effect handle234 when
display0x10000000 is set, so this scene's M_C_3 uses **a/e_mario**, irrespective
of a same-named animation in b_mario. Texture companion fallback is original
**a/a_mario-**; there is no a/e_mario- file. Do not resolve duplicate names
across arbitrary banks without this source selector.

## Professor no-hit hop225/30

`evt_npc_jump_position_nohit` 0x80051868/0x364 resolves actor and targetXYZ
as integer event operands, duration225 as integer, height30 as integer→float.
Duration is **milliseconds from the event clock**, not frames or speed.
First call saves event clock words into event+198/+19c; snapshots NPCXYZ
into NPC+158/+15c/+160; sets C=-4*height into event+7c, B=targetY-startY-C
into event+80; saves NPC flag0x00080000 in event+84 then clears that flag.
Source -4.0 binary32 is0x8041fdfc (r2-30276).

On every invocation elapsed milliseconds are integer division of the unsigned
low-word event-clock delta by ((busClock>>2)/1000). If elapsed<duration:

```
r = single(float(elapsed) / float(duration))
X = intplGetValue(kind0,startX,targetX,elapsed,duration)
Z = intplGetValue(kind0,startZ,targetZ,elapsed,duration)
innerY = fma(C,r,B)
Y = fma(r,innerY,startY)
return 0
```

For the used same-position hop B=120,C=-120, so mathematical Y excursion is
120*r*(1-r), apex30 original units. Numeric operations retain the two FMAs.
When elapsed>=225, it assigns targetXYZ exactly, OR-restores saved flag0x80000,
requests landing sound when configured, returns2. The first call can request
configured jump sound; see source sound masks, not generated tones. No explicit
pose switch occurs in this command. NPC motion/pose selection remains the NPC
main owner. Original event clock cadence is needed for faithful225ms.

## Party and Mario turns are blocking source callbacks

`sysMsec2Frame` 0x800059cc/0x28 computes signed low-word ms*gpFPS then signed
division1000 (truncation toward0). Used200ms at FPS60 givesN12. This is a
source derivation, not a universal fixed12-frame observed duration.

`evt_party_set_dir` 0x800eb21c/0x1dc first resolves party slot, saves the member
ID to event+78, snapshots current direction partner+10c to event+7c and zeros
counter event+80. Missing partner returns2. Each invocation resolves angleFLOAT
and durationINT; nonnegative duration goes through sysMsec2Frame, negative is
retained. For N>0 target=revise360(360-requestedYaw). It computes
single(initial+(target-initial)*single(counter/N)), wraps through revise360,
writes partner+10c and+110, and toMovedirSimple(result) into+100. It increments
counter, returns0 while counter<=N and returns1 when counter>N. Thus calls
counter0 throughN include exact start and endpoint. This is a blocking callback
with normal source VM return ownership, not an asynchronous animation request.
No shortest-arc correction is applied to target-initial.

For negative duration (used-1), party writes only partner+110 and derived+100,
preserving current+10c, then returns2. For duration0 it sets both+10c/+110 and
derived+100,returns2. Do not collapse-1 and0 into the same snap operation.

`evt_mario_set_dir` 0x800e6de8/0x1e0 resolves angleFLOAT,durationINT then
sysMsec2Frame,third modeINT. First call zeros event counter+78/finish+80,
snapshots current Mario+1ac to event+7c; if requestedYaw>180 subtract360.
Positive-duration target=revise360(360-requestedYaw), then if target>180
subtract360. It linearly interpolates raw target-initial at counter/N, wraps
result and writes Mario+1ac/+1b0 and derived toMovedirSimple into+1a4/+1a0.
Do not substitute shortest-arc interpolation: source target wrapping differs
from party and retained initial direction is not recentered around the target.

Incremented counter>N with mode0 returns1; earlier calls return0. With nonzero
mode it sets finishmarker1,returns0; the next invocation clears+1a4/+1a0 and
returns2. A pre-existing finishmarker1 takes that cleanup path before movement.
DurationN<=0 wraps target,sets direction/current/derived fields and returns2.
Original turns can span N+1 calls; preserve source VM immediate-versus-yielded
continuation semantics. Story request(90,200,0) uses mode0; a later separate
WAIT200ms is additional source waiting, not redundant turn duration.

## Validation and native boundaries

Read directly from source ISO using tools/inspect_dol_function.py and external
US symbols for the named functions. Item table pointers, icon.bin descriptor,
source basic-pose table membership and -4.0 literal were read directly.
No native source changes or original asset payload added to history.
Source numeric parity, event clock/frame scheduling, icon GPU projection,
NPC motion-pose ownership and original runtime image/audio comparison remain
dependencies. This contract supplies bindings for retained requests; it does
not establish full prologue presentation correctness.
