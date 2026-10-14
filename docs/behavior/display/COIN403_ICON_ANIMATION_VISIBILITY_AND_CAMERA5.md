# Original coin403 icon animation, visibility and camera5

US G8ME01 revision0; read-only DOL/icon asset recovery2026-10-14 for
[ITEM-01](../../requests/ITEM-01.md). Supplements
[coin service](../field/GOOMBA3_COIN20_PRESENTATION_CLOCK_WINDOW_AND_RETIREMENT.md),
[shared icon registry/draw](../field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md)
and [camera5 producer](../camera/FIELD_ITEM_GET_CAMERA5_PRODUCER_AND_PROJECTION.md).
Addresses, offsets and masks hexadecimal; animation counts decimal.
Source admission and operation order, not observed native/GX parity.

## Entry and descriptor differences from item95

Coin state0 calls iconEntry(iraiCoin,403),iconSetPos(once),iconFlagOn102.
Original iconEntry8001b094/244 selects ordinary or alternate registry via
GP14. It scans active duplicate names but subsequently allocates first inactive
slot regardless of the match. No duplicate update/dedup or bounds-safe full-pool
failure exists locally. Native fresh finite admission must retain a free slot
and correct first-active named setter ownership. A simultaneous existing
iraiCoin can change which icon receives caller setters; do not silently assume
all duplicate records are updated. SetPos/FlagOn/Off/Delete affect first match.

Entry memsets56-byte record,sets root from originalzero vector,scale1,color
ffffffff,name18,descriptorID403 atunsignedhalf28. Ready descriptor copies:

| Record field | Initial403 value |
| --- | --- |
| half2a width | 0 |
| half2c height | 0 |
| half2e frame count | 4 |
| half30 frame index | 0 |
| half32 current TPL index | 414 |
| half34 remaining delay | 6 |

icon.bin offset178a rawhalves are(1,0,0,4,414,6,415,6,416,6,417,6).
Descriptor half0 bit1 sets record flag1000(animated). Entry then sets active1.
In ordinary fresh plaza mapblend2clear,entry flags1001; caller OR102 yields
**1103**. Item95 descriptor307 has nonanimated type0/single312/delay0;
borrowing that static icon owner would discard coin's authored frames.
Readiness absence queues flag2000 for later descriptor resolution rather
than inventing a loaded frame. Existing mapblend branch can add40; finite
plaza clear admission is [source-pinned](ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md).

## Exact animation owner precedes visibility

iconMain8001b2d8/374 requires both original ready buffers (r13+1718/+171c),
selects registry viaGP14 and iteratesactive1 records in physical slot order.
At8001b33c..40c a queued2000 resolves metadata,clears2000,resetsframe0,
loads firsttexture/delay and adds1000 when descriptor animated. It then
continues ordinary animation in THIS invocation; no extra ready frame wait.

At8001b410..500 animated1000 drives this order:

1. Read unsigned remaining half34. If nonzero,decrement/storelow16.
2. Reread34. If nonzero,skip frame advance.
3. At0,increment unsignedframe30/storelow16. Reread; when>=framecount2e,
   storeframe0.
4. Resolve descriptor again; load currenttexture from+8+4*frame and remaining
   duration from+a+4*frame. Storehalf32 thenhalf34.

Initial ready403 therefore has frame0/delay6 before its first owner call;
passes1..5 retain414 withdelay5..1;pass6 selects415/delay6;pass12 selects416;
pass18 selects417;pass24 wraps414. These are admitted **iconMain invocation**
counts. No elapsed GP clock, simulation-rate conversion or draw callback drives
the counters. Native scheduler-to-simulation adaptation must state its cadence;
rendering must only consume publishedframe/texture.

Only AFTER all of this, at8001b504..50c,flag2 suppresses display admission.
Thus initial hidden icon continues animating throughout coin state's strict
200ms wait. State1 clearing2 at80054630 does NOT reset frame/delay or select414.
A visible-start animation restart would contradict source. No recurring
SetPos/Scale/Alpha writes exist in the coin callback after state0; its position
is the original sampled camera-relative placement,not liveMario follow.

State100 iconDelete clears active ownership immediately. Later iconMain then
skips it; no closing-window-bound icon lifetime or return-to-camera4 phase.
Hidden flag2 does not itself delete/shrink/reset the icon or effect.

## Camera5 and geometry

Visible camera selection priority isflag10→camera8,then100→camera5,
then200→camera7,elsecamera4. Coin1103 lacks10/200/80 and retains100 when
2 is cleared: visible animation uses **camera5**,displaytype1,iconDisp callback.
It requests separate shadowEntry(XYZ,radius10),then dispCalcZ(XYZ),then
source dispEntry(5,1,iconDisp,record,Z). Hidden flag2 suppresses that shadow/
display request but does not suppress animation.

Actual camGetPtr table base is**803c95f8** (lis803d + signed95f8); camera5
selects+14. A camera5 producer contract line naming803d95f8 has a transcription
error; it is not an alternate pointer table. Other original camera5 initialization,
25degree lens/100..10000nearfar,viewportdepth.2...3 and camEffMain copying
source camera4 view inputs retain the linked producer contract. Coin's flag100
selects this owner; it does not produce the camera projection itself.

Original icon.tpl414/415/416/417 are each40x40CMPR(format14). Descriptorwidth/
height0 use currenttexture dimensions. flag10clear applies originalfloat
.6000000238418579: scale1 yields about24x24 sourceunits,bottomanchored,
left−12,right12,top24,bottom0. Source operations/rounding are inherited from
iconGX800197f8; do not use centeredheight or a full40x40 window icon.
Coin flags4/8 mirror absent; scale1/colorffffffff retained from construction.

iconDisp8001abf0 forms cameraView*T(XYZ)*Ry(−camera114*originalRAD)*S(scale),
then iconGX. Source UV/vertices,linearfilter/LODzero,opaquealpha255 depthwrite/
LEQUAL/GEQUAL128 and ordinary texture×color shader are shared with linked
item95 draw. Flag100 skips ordinary conditional map-lighting extra stage,
but does not disable depth,change dimensions or bypass earlier mapflag40 shader.
Original separate shadow remains a provider; it is not baked into coin frames.

## Evidence and boundaries

Fresh complete iconEntry/iconMain named extraction and raw icon.bin/TPL
headers reread from ISO. Ignored disassembly is
artifacts/reference/goomba3-services/coin_iconEntry/coin_iconMain.{elf,txt}.
Parent source contracts pin iconDisp/GX, mapblend and camera5; no original
binary was executed, no generated/renamed icon assets were substituted.

Native finite coin needs four originalframe resources,retained unsignedcounters,
owner-before-display scheduling,hiddenanimation and sourcecamera5 projection.
Shared pool exhaustion/duplicate admission,metadatareadiness,mapblend40,
shadow rendering,fullGX/PICA filtering/raster parity and physicalhardware
performance remain separate providers. No shared docs/index/progress/runtime
edits were made.
