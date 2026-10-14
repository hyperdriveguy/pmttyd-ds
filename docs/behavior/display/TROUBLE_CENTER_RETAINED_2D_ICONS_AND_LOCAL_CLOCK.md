# Original Trouble Center retained2D icons and local clock

US G8ME01 revision0; read-only recovery2026-10-14 for
[ITEM-01](../../requests/ITEM-01.md). Supplements
[board producer](../field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md),
[window drawing](TROUBLE_CENTER_ORIGINAL_WINDOW_DISPLAY_AND_US_CACHE.md),
[complete Entry2D/Main](EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md) and
[icon geometry/flags](ITEM_SELECTOR_ICON_ANCHORS_AND_FLAGS.md).
Static source/asset evidence, not original execution or native raster parity.
Addresses/flags/offsets hexadecimal; IDs/counts/dimensions decimal.

## Descriptor identities

Read original icon.bin's U16 pointer at2*(ID+1); descriptors consist of
loop/reservedWidth/reservedHeight/frameCount, then image/delay U16 pairs.
Both reserved dimensions are0 for these four entries, so actual TPL dimensions
supply geometry. Independent descriptor/TPL-header inspection:

| ID | Descriptor offset | Loop,count | Original images/delays | Actual dimensions |
| --- | --- | --- | --- | --- |
| 504 | 1c8e | 1,10 | 533..541 each2;542 delay28 | All40x40 |
| 524 | 1da2 | 0,1 | 562,0 | 24x32 |
| 134 | ac2 | 0,1 | 134,0 | 60x48 |
| 136 | ada | 0,1 | 136,0 | 60x48 |

These are icon descriptor IDs, not raw TPL indices. The static image delay0
is not an instruction to animate on each visit: these entries lack loop flag1.
504's complete delay sum is46 admitted iconMain visits. No measured60Hz/
wall-clock duration follows from that count.

## Source record construction and per-icon animation

Complete iconEntry2D8001aeb0/1e4 selects field/battle icon pool through GP14,
finds inactive56-byte record, clears it and assigns original default position,
scale1/colorffffffff/name. It stores icon ID at+28. Both icon resource pointers
must be ready before descriptor loading. Missing readiness sets2000; ready
loop1 sets1000. Final active/2D flags OR11 (active1,camera8/full-size10).
Descriptor metadata publishes count+2e, frame+30=0, image+32 and delay+34.
There is no name-deduplicating constructor; later named setters/deletion choose
first active match. Native finite pool/name/resource admission remains explicit.

Menu state0 creates504 under iraiYubi and ORs4, creates524 under iraiArrowUp
and ORs8, creates another524 under iraiArrowDown, then134/136 as
iraiLButton/iraiRButton with scale.8. It ORs hidden2 into each. Ready flags
are therefore Yubi1017,Up1b,Down/L/R13. State1 clears Yubi2 every visit, and
sets/clears arrow/button hidden2 using retained top/count boundary predicates.
Do not recreate the records or restart504 animation on each cursor movement.

iconMain8001b2d8/374 returns before record processing when either resource is
unready. For each active record in pool order, resolve pending2000 and
publish frame0/image/delay; then process animated1000 in the **same** visit.
Decrement delay if nonzero; if now0 increment frame,wrap>=count to0,load
new image/delay. Hidden2 is tested only afterward, so hidden504 still ages.
First Main changes2→1; second choosesframe1/image534 and reloads2.
Visit18 choosesframe9/image542 with28; visit46 wrapsframe0/image533 with2.
No GP1c modulo/render-frame clock exists in this retained record path.
The immediate iconDispGx helper's different modulo path is not this owner.

## Scheduling and original registration

[Outer loop](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) calls
EVT before iconMain; psndMain/window manager/effect manager follow iconMain.
A menu state0 icon created in EVT therefore receives its first decrement in
that same outer pass while hidden. Ordinary next state1 visit unhides Yubi
before iconMain's second decrement. Consequently first visible Yubi image is
534/frame1 in that uninterrupted path, rather than resetting to533 on reveal.
Blocked manager/resource/alternate scheduling domains remain separate.

Active,unhidden bit10 icons request exactly
`dispEntry(8,1,iconDisp8001abf0,record,200.0)` at8001b530.
Source layer float8041f8a8 is43480000. Window callback registration is its
separate `(8,0,irai_window_disp,work,300)` request. These fields establish
camera/priority/layer requests, not an inferred compositor sort direction.

## Bottom anchors, mirror flags and original matrix

iconDisp8001abf0/b8 reads actual current display camera and requests
camera.view * T(recordXYZ) * Ry(-camera114*0.01745329238474369) * S(record.scale).
Then iconGX800197f8/9a4 adds X=-1 forflag4,Y=-1 forflag8.
Flag10 skips ordinary.6 reduction and uses full TPL dimensions. If current
camera differs from actual camera8, flag10 applies an additional Y inversion;
ordinary registration above supplies camera8. Do not reinterpret10 as a
numeric camera argument or omit original Up8 merely because it is named Up.

Before mirror/uniformscale, geometry is centered X and **bottom-anchored Y**:
(-width/2,height),(width/2,height),(width/2,0),(-width/2,0),Z0;
UV(0,0),(1,0),(1,1),(0,1). Yubi flag4 mirrors40x40; Up flag8 vertically flips
24x32; Down uses ordinary24x32. L/R scale.8 gives approximately48x38.4 from
60x48. Geometry/material/UV filtering/alpha/depth details retain their linked
source providers; a centered-height window icon would move the original anchors.

## Provenance and remaining validation

Original icon.bin ISOoffset948139664,size8174,SHA256
0be6b95a7e45e5d932641f24f8240a289a852889f1906d32be839046a680532b.
icon.tpl offset948147840,size677792,SHA256
fb8e99398fd5ef3c36cc518077748405949c004fcc5c515cc8828a64340b74f9.
Descriptor span hashes504/524/134/136 respectively:
29148c76bbd83d33f62f3c5cf48c2cd2b2bb2c37525621c7c6017e7952bc4115,
64b25cd14a778523ba7af469f79065aadc8f1add53e09fd317797f8e850b3a9c,
982222f68ff83114e1537639439c4b0ff24e7785ec04e3c78c6a9054c94f8941,
03fd962483d9213ac55c8d2729aec7d73dbab0bb0da0ccea1d4f5b818fad09c7.
Temporary `/tmp/trouble-icon-sfx-bindings.json` records every descriptor/frame
and asset identity. Fresh complete iconMain/iconDisp disassembly remains/tmp.
No extracted art/audio, implementation or shared index/progress/inbox changed.
Nintendo/Intelligent Systems assets remain local, with no redistribution rights.

The [finite native presentation](NATIVE_TROUBLE_CENTER_RETAINED_PRESENTATION.md)
now supplies the five-record bridge and original atlas imports524/134/136.
Remaining: native case/global pool/scheduling admission, source camera/renderer/
GPU parity, original matching icon/raster traces and New2DSXL performance/
hardware validation. Original sound resource
bindings are [separately documented](../audio/TROUBLE_CENTER_MENU_SFX_BINDINGS.md).
