# Underground trio EXP presentation and counting

2026-10-13 documentation-only US read-only source delivery for ending08→0b.
Source listings `/tmp/trio-exp-*` and full controller `/tmp/trio-para-end.dis`;
see [ending ownership](UNDERGROUND_TRIO_VICTORY_CONTROLLER_AND_FIELD_RETURN.md).
Original icon/effect assets inspected in ignored local extracts and `/tmp`;
no native edits or source assets committed. Actual GPU/raster/display clock
parity remains separate from these local source services.

## Exact leaf admissions and common display exit

BattleFogForceStop80147bd0/5c queries global B. B182b8==0 returns0 with no
mutation. Nonzero: B182bc nonnull→effSoftDelete8005e9c0 then clearB182bc;
return1. It never clears B182b8 or creates a replacement fog. Used source
fresh field is0, a closed real no-fog path.

BattleGetStockExp800f7bd4/28 takes B, reads signed32 Bf00. <=0→1;
>=100→100; otherwise actual value. It does not clear stock, grant points,
query pouch or return0 for no accumulated EXP. Original death StoreExp owns
Bf00; do not replace this getter with a native sum of enemy registry stats.

Complete btlseqEnd_DispMain80215088/12c takes cameraID in r3 and B in r4;
captures E=Bf28. Null E skips. It reads E30 masks2,4,8,10,20 in order.
**There is no E0=current Bf24 copy or local scalar/status write.** With all
flags clear, the complete body is a genuine no-op. BattleSetSeq800f8a08
also only stores the selected bank word; bank7 writes Bf24, not E0.
Do not repair `_audience_Whistle_control`'s real E0 range gate by substituting
Bf24. General later E0 producers remain a separate source request.

E30 mask2 calls _GetExpIcon_Main80214760(B), then registers
dispEntry(cameraID,order1,depth900,_GetExpIcon_Disp8021471c,B).
Other bits only request their actual callbacks:4→80214fac;8→802129e0 with
camera4/order2 plus original position-based depth query;10→80214e2c;
20→80214c48. They are not admitted by EXP mask2 alone. No readiness return,
clock reset or global status update occurs locally. On ordinary controller
common exit cameraID=8. ExpMain's actual writes below are required as soon
as mask2 is set; an unconditional trace is only equivalent while flags0.

## Ending08,09,0a,0b source order

State08 calls FogForceStop, clears E0c (level-up marker); calls StockExp→S16
E8. Fortune B18ff9==3 doubles through16-bit storage; pouchlevelP8a>=99
setsE8=0. Upper-cap positive E8 at100, copy it to Ea (remaining display points).
Positive E8 calls ExpIconInit. Set E4=120, camera speed(0,2), mode(0,17)
unless Bflags100 selects18, position offset(0,0,0), set state09; audience
cheer(-1,0), clap(-1,0), whistle; BGMOff400(hex) and BGMOn(1,
BGM_BATTLE_WIN1) unless actual active party has status27, which selectsWIN2.
This state **falls through09 in the same invocation**, decrementing E4 to119.
State09 decrements each subsequent invocation; <=0→0a and common exit.

State0a E8>0: Mario and nonnull party data key40 start their actual priority10,
flags0 victory events; ownerID→entry160 and tracked eventID→U2a8. If E29c
isnull, effPointGetEntry(0,E8,XYZ0/90/0,0)→E29c. Set0b and common exit.
E8<=0 takes the actual no-EXP special/recovery branch in ending contract,
not this positive-count path. Victory events are Mario80417c50 M_V_2/
1000ms; Goombella80396fa0 PKR_Y_1/64ticks/type43; original pose art must exist.

Used original model banks: Mario parts name b_mario at802f3fa8, ISO
`a/b_mario` SHA256
`1f29b8e93280bcccf6a488c99a5f4addc35d5604e80d9afcbf5efcec64df51e4`;
actual M_V_1 nonloop0..60/eight keys and M_V_2 nonloop0..6/six keys.
Goombella resource c_pkr at804243b0, ISO `a/c_pkr` SHA256
`d712df83d7b70b64a90d45551b39ecf7adf277b9572f6931daae7792b52d69e7`;
PKR_Y_1 loop0..14/eleven keys. The six-frame M_V_2 must retain its actual
end pose during the source1000ms wait, not be stretched to that wait length.

Goombella final type43 has no exact record. Original ordered category lookup
8012772c/SetAnimType801276ac uses category69 for requested40..69; table
80393a98 entry80393b58 `(69,802f90e0)` resolves **PKR_S_1**, force0.
This is not animation-name descriptor0 fallback and not the first pose-table
row (type9/PKR_D_5). The earlier ending contract's phrase "first pose entry"
was imprecise and must not select that first row. Preserve this actual source
category branch. M_V_1 later explicit body pose and M_V_2 named requests exist
in b_mario and do not require an e_mario duplicate-bank guess.

State0b does not wait for these victory children. While signed Ea>0:
increment E4; BattlePadCheckNow80119fe0(mask300 hex) nonzero and E4<1000
adds1000. If low2 E4bits!=0 AND E4<1000, no point transfer. Otherwise Ea--
as16-bit, P96++ as16-bit. Every transfer whose counter low3bits==0 requests
SFX_STAR_POINT_GET1 at802feaa8. P96>=100 resets P96=0,E0c=1; if
P8a+1>=99, set Ea=0. Point100 does not itself mutate level/base stats here.

When Ea<=0, set E4: previous counter>=1000→10; else previous<120→140-counter;
otherwise20. Set0c. State0c later drains that timer, calls ExpIconEnd,
effSoftDelete(E29c), then actual later icon/effect/event waits continue.
No fabricated impact/point timer, held general action-command pad or reward
reset may replace these source battle-pad/count branches.

## Original nineteen star icons and exact packed state

_GetExpIcon_Init802149ec/25c takes E. It always creates19 2D icon records,
names `_Bgsp0`..`_Bgsp18` formatted with original key802fe9c0=`_Bgsp%d`,
ID404(decimal)/194(hex). iconEntry2D8001aeb0 creates each, iconNameToPtr
80019754 resolves it, pointer→E34+4*i; subsequent setters use that record's
name at pointer+18. Set scale, hidden flag2, Ecc+4*i=0, retained positions.

| Indices | Scale | Original XYZ in source 2D camera coordinates |
| --- | --- | --- |
| 0..9 | 0.899999976 at80427238 | X=36*i+(24-18*divw(S16 Ea,10)), Y25, Z0 |
| 10..18 | 0.449999988 at8042723c | X=86.8999939-16.1999989*(i-9), Y0,Z0 |

Position triplets are E118+12*i; alpha words E80+4*i; phase words Ecc+4*i.
Init clears phases, writes all positions and icon pointers, then E30 OR2.
It does not explicitly initialize every alpha word locally. Source allocation
initialization and source first Main are distinct ownership steps.

Constants:0=804271c0; Y25=804271e0; X24=8042722c;
half-spacing18=80427230; big spacing36=80427234;
small origin86.9=80427240; spacing16.2=80427244. Source init uses fnmsubs
and fmadds at its documented boundaries; host trig/numeric adapters must
not misread fnmsubs as reversed origin/spacing.

_GetExpIcon_Main80214760/28c captures E=Bf28 and signed Ea. q=divw(Ea,10),
r=Ea-q*10. For i<q in0..9: iconFlagOff(name,2), alpha<=0→64 else add64 and
upper-cap255; phase<16→phase+1; recompute big X using live Ea/10.
For remaining big indices: FlagOn2,alpha=0,phase=0.
For first r small icons10..18: FlagOff2, same alpha ramp; no phase increment.
Remaining small icons: FlagOn2,alpha=0; no phase reset locally.
Finally all19 call iconSetAlpha(name,low8(alpha)) and iconSetPos(name,
retainedX,retainedY+0,retainedZ). No point/stock/resource decrement here.
Exact finite valid domain is Ea0..100; malformed counts/null icons are not
silently repaired by this source loop.

_GetExpIcon_End802146c0/5c clears E30 mask2 before looping19 retained
pointers/name+18→iconDelete8001ae08. It does not null the pointers/alpha/
phase arrays or free E. Native preallocated icon storage can be an explicit
allocator adapter while preserving hidden flags, state and nineteen deletes.

## Source icon assets and background request

Read-only original `icon.bin` ID404 directory entry17a2 has
loop1/reserved0/0/count4: `icon.tpl` images418,419,420,421, each delay6.
All are original40x40 gold-star/green-medallion frames. Source pixel inspected;
native [message icon importer](../../../tools/message_icon_assets.py) already
parses this format and packs original TPL pixels, but currently includes
only129/504/115. Extend the locally generated subset/atlas with all four
404 frames, preserving source ID/loop/delays and alpha; no replacement star art.
Physical source sprite size40 times scale yields36/18 source units before
the explicit 3DS viewport adapter. Resource lifetime, tick clock and original
icon-main cadence are separate from _GetExpIcon_Main's display-position work.

_GetExpIcon_Disp8021471c/44 submits windowDispGX_Waku_col8007b44c:
type0, colorwordFFFFFF80 at80427158, floats
(-76.8999939,21,165.7999878,24.1999989,10), at
8042721c/220/224/228/214. This is the actual original window request;
inner GX window geometry/material interpretation remains a service dependency.
The native renderer may adapt the source 2D camera to the 3DS viewport and
use Citro3D atlas quads/vertex alpha, while retaining this request/ordering
and explicitly recording incomplete window/palette/viewport parity. Source
icon drawing must honor hidden mask2 rather than drawing all19 alpha states.

## Point-get effect art, lifetime and bounded render provider

effPointGetEntry8021b70c/10c creates one36-byte payload under original effect
owner, sets name802fed08=`PointGet`, Main8021b564, flags OR2; used type0,
amount=E8, XYZ0/90/0, duration0. Payload fields:0type,4 X=inputX-2000,
8 Y=inputY-30,c Z,10 amount,14 scale1,18 duration,1c timer0,20 phase0.
Source live E29c retains this effect handle until soft-delete/release.

effPointGetMain8021b564/1a8 copies pre-update XYZ for display-depth request,
increments timer, then phase0 updates X=intplGetValue(mode11,-1000,0,timer,45);
timer>=45 resets0/phase1. Phase1 duration0 waits for EFFECTflags4, clears that
flag then resets timer0/phase2. Nonzero duration instead waits actual counter.
Phase2 X=intplGetValue(11,0,1000,timer,60); timer>=60 effDelete8005ea30 and
returns before registering draw. Otherwise depth snapshot then dispEntry
(camera8,order2,computedDepth,effPointGetDisp8021ac94,effect). Amount remains
the original earned total while separate Ea icons/count decrease.

Used intpl mode11 is jump-table target80006f78. Numeric sequence:
angle=single(single(PI*t)/total), cos80271ea8→frsp;
d=single(end-start); w=single(1-cos); v=single(d*w);
result=fmadds(v,0.5,start). PI original8041f46c=3.141592025756836,
1=8041f444,0.5=8041f47c. total0 returnsend before dispatch. This is a
cosine easing service, not integer powers or the float-event getter loop.
Portable cosine can be an explicitly stated numerical adapter until original
MSL cosine precision is bound; preserve source tick/phase/softdelete timing.

Original US draw selects texid_tbl803b1070 language1 triple, count5;
nonnegative amount>=2 selects803b0f8c (plural), else803b0f50 (singular).
Five records (type,foregroundTexture,shadowTexture):
(1,124,125),(-1,-1,-1),(0,126,127),(-1,-1,-1),
(1,128,129) singular or(1,130,131) plural. Type0 formats decimal amount
using `%d`, slices the original digits atlas; type1 uses whole phrase art,
type-1 is spacing. Original draw renders shadow pass before foreground,
retains total-width adjustment and source matrix/quad layout.

effGetTexObj8005ed58 uses language resource for global IDs>=118 and subtracts
118 into TPL; actual asset `e/us/effect_us.tpl` SHA256
`93ef5d667ded662d6abf13133082bef138f9c88132365ff8b11f8abf2129dd14`.
Thus indices6..13 supply this exact original wording/art:

| Global ID / TPL | Asset / original image dimensions |
| --- | --- |
| 124/6;125/7 | You got foreground200x48 /shadow100x24 |
| 126/8;127/9 | Digits0..9 foreground480x48 /shadow320x32 |
| 128/10;129/11 | Star Point! foreground296x48 /shadow148x24 |
| 130/12;131/13 | Star Points! foreground328x48 /shadow164x24 |

These source images were inspected in `/tmp/trio-exp-effect-124..131.png`;
no generated dialogue/font substitute. Use locally imported original atlas
textures, source decimal digit selection and shadow/foreground blending in
Citro3D. Full effPointGetDisp8021ac94/8d0 GX matrix/quad geometry, inner icon
clock/display and window frame services remain explicit bounded render
dependencies; a retained request/declared numerical viewport adapter is not
proof of original raster parity. [Effect driver](../effect/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md)
establishes actual callback context/softdelete/language resource scheduling.

Validation: complete local service listings, original state/callback table,
read-only icon/effect TPL decoding and source pixel inspection;
`git diff --check`. No native edits/build rerun/commit. Next bind source08
scalar/icon providers and victory event ownership, then effect/counting
presentation before later reward/export/field return is claimed.
