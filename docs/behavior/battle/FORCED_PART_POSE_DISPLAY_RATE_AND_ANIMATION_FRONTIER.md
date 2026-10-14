# Forced part-pose display rate and animation frontier

2026-10-14 US G8ME01 disc0 revision0, read-only source inspection. Used branch
after [unit gauge/status/icon prefix](SURVIVING_UNIT_STATUS_EFFECT_ICON_AND_PART_FRONTIER.md)
when original bodypart P204 bit2 is set. No native/shared documentation edit,
index change, or commit.

## First real provider is the local-time-rate setter

btlDispMain801024f0 walks linked parts U14→P0. For P1c0!=-1 it first retains
r20=P+1c4, the part's NUL-terminated authored pose-name buffer. P204 bit2 clear
enters automatic status-pose selection; bit2 SET jumps to801029ec without
_GetStatusPoseType. Its first actual call is:

```
801029ec f1 = literal1.f from8042224c
801029f0 r3 = P
801029f4 btlDispAnimeSpeed80100a4c(P,1.f)
```

btlDispAnimeSpeed80100a4c/34 stores FLOAT P208=incoming f1, reloads P208 and
P20c, computes single P208*P20c, reloads pose handleWORD P1c0 and calls
**animPoseSetLocalTimeRate8004142c(handle,f1=product)**. This is the concrete
next animation-owner frontier; no generic whole btlDispMain pending is required.

The complete setter8004142c/18 reads mutable work pointer80415dc0
(r13-7160), pool WORD work+10, then writes incoming binary32 rate at
pool+wrapping32(handle*170hex)+44. It supplies no return/readiness value,
checks no active flag/ID/domain, propagates no linked pose and does not reset
timestamps or advance animation. Valid native mapped/typed pool/handle ownership
must justify the write. Exact original handle-1 is excluded by caller; numeric0
is an ordinary potential pose index, not proof the native animation registry
actually owns that ID.

The existing [animation local-time contract](../display/ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md)
owns this leaf and time consumers. Preserve rate as pose-local owner state;
simply changing scene-global animation speed does not implement the original
record addressing or linked/resource lifetime.

## Subsequent pending-name consumption and exact force operand

At801029f8 reread P204. If bit1 clear, skip directly to gravityOffsetControl
80101060(P), floatOffsetControl80100f34(P), statusPoseControl80100e6c(P), then
parts world position80128b10(P,&XYZ) and animPoseMain8003ed2c(P1c0).

If bit1 set, source clears ONLY bit1 and stores P204 at80102a0c. It then
freshly reloads flags and executes instruction540005eb at80102a14:
`rlwinm. r0,r0,0,23,21`. This clears numeric mask200, stores the result back
to P204 at80102a18, and tests the **entire resulting word**. Branch-equal
selects force0; nonzero selects force1. It does NOT select force by testing
bit8. Since this used path retains bit2, the resulting flags are nonzero and
force1 is admitted even when the event's changePose force argument was0.

Then animPoseSetAnim80041170(r3=handle P1c0,r4=retained pointer P1c4,r5=force)
is called at80102a2c or2a40. P1c4 bytes are read as a signed-negative original
string selector (ordinary0x8xxxxxxx mapped address); use the exact original
pose name and resource bank. The [selector/restart authority](../display/ANIMATION_SELECTOR_AND_RESTART.md)
owns name search, descriptor0 fallback and forced restart timestamps/caches.
Force1 can restart an equal pose; suppressing it by comparing native scene
strings changes the source consumer behavior.

After selection source clears BYTE P214, computes animation/effect-mode P215
through flags/status18/status7/color-alpha gates, then continues the three
offset-control leaves and animPoseMain. These are next real providers, not
implicitly completed by SetAnim or the existing converted-mesh renderer.

## Exact producers: part defaults, copied names, and native adapter gaps

Original BtlUnit_Entry part construction initializes:

- P4f0=ffffffff at8012955c from originalWORD804226a0, so BYTE P4f3=ff.
- P1c0=-1 at80129570; BYTE P1c4=0 initially at29574.
- FLOAT P208=1.f at29578, then FLOAT P20c=1.f at2957c (804226a8).
- BYTE P215=0, then P214=0.

Later btlDispEntAnime801011fc copies the exact idle source pose string to
P1c4 and animPoseEntry(original model,mode1) publishes P1c0. Source
GetPoseNameFromType/btlDispChangeAnime80101168 subsequent calls strcpy name
to P1c4 and publish request flags in their real order: OR2,OR1,clear80,
clear200,clear10, then set/clear8 from force. No immediate animPoseMain occurs
in that producer. Exact name changes reach the display consumer above.

Read-only current source/trio_attack.cpp changePose copies exact name bytes
and approximates these final flag values, updating native scene pose/poseAt
immediately. That scene projection is separate from original registered pose
record/time history. Its custom part constructor currently starts mapped bytes0
and does not set source P208/P20c/P4f0 or obtain mode1 pose registry handles.
Consequently first source speed request would multiply1*0 and freeze a pose
if the missing P20c were accepted as source; color BYTE4f3=0 would admit the
later nonopaque mode path. Bind the original once-only constructor defaults
and genuine typed converted pose owner rather than fabricating runtime handle0
as success or erasing flags to skip this used branch.

## Evidence and validation

Ignored complete bodies/listings display-forced-* and original Entry listing
display-alpha-BtlUnit_Entry.dis under artifacts/recovered-temp-evidence/.

| Body/address/size(hex) | SHA256 |
| --- | --- |
| btlDispAnimeSpeed80100a4c/34 | d980c1934cccb3a52c5ed54aeecb69a546f66ce235ba2a0febd6c76447af207d |
| LocalTimeRate8004142c/18 | 0a8c7c1e7fdd37b8b00afd57907a9203f190cf6aec33571d56f963232ee1b756 |
| btlDispChangeAnime80101168/94 | 42b9a3e555f3bab2bbedea23ddf1d5d0602374a820d78c4c903a29b0966d93c3 |
| animPoseSetAnim80041170/17c | 50eb5f2d85826255dde8679786dd13b2e4fa0dc50eb07cbb3c43f087f66b4541 |

Read-only whole-source inspection, flag instruction decoding, constructor and
current native history audit, scoped documentation diff check. No native
test/build/render equivalence or animation-backend completion claim.
