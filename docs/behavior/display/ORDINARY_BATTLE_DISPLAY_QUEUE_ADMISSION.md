# Ordinary battle display queue admission and deferred callback

2026-10-14, original US G8ME01 disc0 revision0, read-only ISO/DOL inspection.
This is static instruction evidence for the current EVT-01 depth frontier. No
original execution trace, source frame cadence or rendered equivalence is
claimed. Addresses and offsets are hexadecimal unless specified otherwise.
Existing [queue/dispatch](DISPLAY_QUEUE_AND_DISPATCH.md) and
[exact sorter](DISPLAY_SORT_IMPLEMENTATION.md) contracts cover the general
owner; this contract identifies the used ordinary battle continuation.

## Opaque registration and the immediate next service

Following the [post-Main alpha/admission](../battle/ORDINARY_MARIO_POST_POSE_MAIN_ALPHA_AND_DRAW_QUEUE.md)
and [original depth](BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md) services,
opaque branch80102cd0 reads P204. Mask100 suppresses registration. Otherwise
80102cdc..2ce0 calls dispCalcZ with the retained depth XYZ temporary at stack54,
then sets r3=4, r4=1, r5=80101774 (btlUnitPartsDisp), r6=P. At80102cf8,
dispEntry80010bc4 consumes the returned f1 priority. The translucent branch
80102cac..2cc8 uses camera4/class2 with the same callback and borrowed P.

Entry has no direct calls, callbacks, matrix/GX work, allocation or sort. It
returns to80102cfc. There P1ac mask04000000 admits a second registration of
callback identity8010128c, camera4/class2, using a separate retained priority
at stackc. Original ordinary Mario part attributes9 omit this bit, so this
registration is skipped. The next block80102d24 tests U104 mask20000000,
then80102d30 tests P1ac mask02000000. If both are clear, the next actual service
is **BtlUnit_GetPartsWorldPos80128b10 at80102d4c**, with r3=P, r4=stack48 (X), r5=stack4c (Y) and r6=stack50 (Z),
three separate float output pointers. Mario U10402040000 and
P1ac9 take this path. Do not substitute deferred btlUnitPartsDisp as the
immediate execution continuation after dispEntry, or claim the enclosing
btlDispMain finished merely because its pose registration returned.

## Record ownership sufficient for the used dispEntry

The original owner retains1024 contiguous16-byte records,1024 pointer slots,
active count and a current-work pointer. Native indices/typed callback IDs can
represent original pointer relationships, but are a declared address-domain
adapter; P and callback words are source identities, not valid ARM addresses.

At80010bc4, load active count and calculate record=count*10 from the record
buffer. At80010bc8 compare priority to itself unordered; NaN returns at
80010bd8 before writes or count increment. Infinity and signed zero are
admitted. A native service needs a distinct admitted/rejected result internally;
original return register r3 is not a boolean success ABI.

For admitted camera4/class1 registration, writes are:

| Record offset | Value |
| --- | --- |
| 0 | camera low byte04 |
| 1 | class low byte01 |
| 2..3 | previous padding, untouched |
| 4 | binary32 rounded 400000 minus priority |
| 8 | callback source identity80101774 |
| c | borrowed source part identity P |

The full class word determines table access and key direction before narrowing.
Class2 instead stores binary32 rounded 600000 plus priority. Eleven actual
offsets and other class directions remain in the general queue contract. After
these stores,80010c44 reloads count and80010c50 stores the record reference to
sortPointers[count]. At80010c54..5c reload count, increment and publish count.
For valid private single-threaded native ownership those reloads can use the
same value; do not invent callback/reentrant work between them.

Entry never owns or copies part state, selected animation, pose outputs or a
render snapshot. The deferred callback reads **live P/U**. Both must remain
resolvable until dispatch; retaining only a pose ID or prebuilt screen sprite
changes the service. Count-only dispReInit80010c64 writes count0 and preserves
record storage, old sorted references and current-work. Native capacity/invalid
address guards may return unavailable outside the supported1024 valid-record
domain; they must not silently drop an admitted source request and report
original service completion. The source itself has no capacity guard.

## Sort, dispatch and the first actual unit-part callback

Caller must explicitly invoke dispSort80010b5c; registration does not sort.
All current pointers are sorted together using original qqsort800064fc and the
record-key comparator. Equal-key behavior is not stable: see the exact sorter
contract, including the three-entry reversal. Camera filtering happens during
dispDraw, not during sorting. Thus a queue owner alone does not establish draw
order without the actual surrounding reset/register/sort/draw cadence.

During dispDraw80010900(4), each iteration reloads current count and the sorted
pointer buffer. Only record camera byte equal to the full draw word4 matches.
The common requests are color-update1, alpha-update0 and Z-scale/offset(1,0).
Class1 then requests, in source order, blend(0,1,0,0), Z-compare-location0,
alpha-compare(6,128,1,0,0), Z-mode(1,3,1). These are GX request values, not a
claim that PICA enum values are interchangeable.

At80010b10 publish the matching record as current-work. At80010b14..24 invoke
callback with r3=4,r4=borrowed P through its retained function identity. Callback
return is ignored. No removal/reset/resort occurs afterward. Count increases
from callbacks can extend this same traversal; later resets or pointer mutation
are visible. Repeated camera draw can call the same record again. There is no
nested current-work save/restore. A native dispatch cursor should therefore
retain its place across an unavailable callback instead of restarting already
completed callbacks each time.

For actual btlUnitPartsDisp80101774, site801017b4 reads
signed P1c0, captures P, global BattleWork and owner pointer P4ec. Negative
pose handles branch to the epilogue801024ac. Nonnegative handles reach
801017d0: load owner rotation pivot U78/7c/80, negate X/Y/Z, then first external
call at801017ec is **PSMTXTrans8029e230(stack220,-Ux,-Uy,-Uz)**. The complete
used unit/part matrix algebra and final animPoseDrawMtx pass1/2/3 calls are in
[ordinary trio draw](../battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#final-used-ordinary-goomba-death-draw-path).
This first provider is a matrix builder, not animPoseDraw or camera4 lookup.
The ordinary path later requests current camera through camGetCurPtr; current
camera is an independent owner from the fixed camera4 depth getter.

## Read-only evidence and limits

Fresh private disassembly resides in ignored
artifacts/battle-display-queue-evidence. Raw function SHA256 identities:

| Original body | SHA256 |
| --- | --- |
| dispEntry80010bc4:a0 | 600861f7e4a1b269f0495e05e48e5467adc8976ed84c3b49c1e6a3419e5980dc |
| dispDraw80010900:25c | 2d84481d96675f38dcabf24a5196d347ca4cded430f39399bc9a38b529a58652 |
| dispSort80010b5c:34 | 1d57805e41de5a2d5ca0fca60a75bb55ab022be0c0b0751ea85cc77326e97158 |
| dispReInit80010c64:c | 0b97ce26efe24eddb3963fe71a2cd4727cabbb206bd02a5c78c146ae1947a060 |
| btlUnitPartsDisp80101774:d7c | b547dee7f1b57c54d12268e328f52a02d0fe0f6c223b05da62aeb93dbfd7e87a |

No native edit/test, original draw execution or prologue completion follows
from this documentation. Camera matrices/framing, enclosing post-registration
branches, valid native lifetime, exact sort/cadence, actual callback matrix/
material/GPU execution and battle return remain separately owned requirements.


## Fresh continuation after the part-position query

A second read-only audit follows80102d4c. Copy returned stack48/4c/50 to
shadow temporary3c/40/44. Test U1c mask00010000. Clear skips the optional
partner midpoint request; set first calls BattleGetPartnerPtr800f86f4(B,U),
then if nonnull gets partner position80128c7c and averages each axis with
single additions and multiplication by source0.5 (80422234). The midpoint
changes only the shadow temporary, not the earlier world-position outputs.
Ordinary fresh Mario without that flag uses its own fresh part world position.

At80102df4 call BattleGetFloorHeight800f7b84(B,shadowX,shadowY,shadowZ).
The [floor contract](../battle/BATTLE_JUMP_EVENT_CALLBACKS.md#used-counter-recovery-stage-floor-wrapper-and-original-initialization)
requires actual stage threshold/low-floor ownership; its fresh values are90
and−50, not an unconditional zero. Retain rounded result floor and overwrite
shadowY with it. Compare ORIGINAL returned world Y (stack4c) with floor. If
originalY<=floor, factor=1; otherwise factor=R(R(100-R(originalY-floor))/100).
Ordered factor>double1 sets1; ordered factor<source binary32.1 sets.1.
The unclamped unordered domain is not a reason to insert a synthetic finite
fallback. Constants1,100,.1 are8042224c/270/274; double1 at802ee450.

Radius computation80102e48..e90 converts signed P1a2 to float through the
source double-bias sequence, then in this exact order:

```
t = R(float(S16(P1a2)) * U90)
t = R(P60 * t)
t = R(U114 * t)
radius = R(factor * t)
radius = R(radius * P78)
```

If U1c00010000 is set and radius<16, replace by16. Independently, if radius<10,
replace by8. These are ordered comparisons; the last replacement is not a
clamp to10. At80102ec8 call shadowEntry8002be98(shadowX,R(floor+10),shadowZ,
radius). Original source constants16,10,8 are80422230/228/278. ShadowEntry
borrows/copies into a separate64-record shadow request owner, whose Main and
Display cadence are [independent](ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md#shadow-request-is-independently-owned).
It does not immediately call a unit-part callback or draw a synthesized ellipse.

After the shadow request, source kind filtering80102ecc..2f04 may request
animPoseSetPaperAnimGroup80039824; ordinary Mario kinddf is outside the listed
paper kinds. The next ordinary part-local service is801261f8 at80102f74,
then follow the live next-part pointer P0. This audit does not claim the body
of801261f8 or enclosing unit-tail/special callback branches is implemented.

Independent code search of include/,source/,tests/ on2026-10-14 found no
qqsort/fsort/display-queue implementation at the time of this delivery. Native
registration/sort/current-work/live-dispatch ownership therefore remains actual
implementation work, not reuse of a tested existing sorter. Documentation
algorithm models remain separate evidence from a native implementation.
