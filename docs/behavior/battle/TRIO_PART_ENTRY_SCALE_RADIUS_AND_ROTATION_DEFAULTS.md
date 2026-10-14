# Trio part entry: scale, radius and rotation defaults

2026-10-14. Read-only US G8ME01 disc0 revision0. Source constructor
BtlUnit_Entry80128fe0, used part loop80129398..801295b0. Addresses/offsets
are hexadecimal. This resolves the concrete partial native constructor gap,
not complete unit entry/render/shadow behavior. See
[complete entry](BATTLE_UNIT_ENTRY_COMPLETE.md) and
[part position queries](BATTLE_UNIT_PART_POSITION_QUERIES.md).

## Source owner, copies and ordered setters

Entry allocates count*500 bytes and explicitly bulk clears that whole part
block before publishing U14. Each P4 is the authentic4c-byte descriptor S and
P4ec is unit owner U. Descriptor-derived attributes/defense precede:

| Source order/site | Actual target publication |
| --- | --- |
| 80129438..40 | Read signed HALF(S30), store HALF P1a2 |
| 80129444..4c | Reload descriptor, read signed HALF(S32), store HALF P1a4 |
| 80129450 | Store FLOAT P78=1 |
| 80129454 | BYTE Pbc=0 |
| 80129458..6c | Signed Uc side index; source B+8*side+a byte→Pbd |
| 80129470..90 | Reload descriptor per coordinate, copy FLOAT S+c/10/14→P24/28/2c |
| call80129494 →SetPartsBaseScale801284a4 | P6c/70/74=1,1,1 |
| call801294a8 →SetPartsScale80128470 | P60/64/68=1,1,1 |
| call801294bc →SetPartsBaseRotate80128654 | P3c/40/44=+0,+0,+0 |
| call801294d0 →SetPartsRotate801287b4 | P48/4c/50=+0,+0,+0 |
| call801294e4 →SetPartsRotateOffset801285d0 | P54/58/5c=+0,+0,+0 |
| 801294f0 | BYTE P4fc=0, source rotation-axis selector |
| 801294f8..518 | Reload descriptor per coordinate, FLOAT S18/1c/20→P170/174/178 |
| 8012951c..24 | P17c/180/184=+0,+0,+0 |
| 80129528..548 | Reload descriptor per coordinate, FLOAT S24/28/2c→P188/18c/190 |
| 8012954c..54 | P194/198/19c=+0,+0,+0 |

Authentic ONE804226a8=3f800000 and ZERO804226a4=00000000 are loaded via
r2=80427440. Source f30 retainsONE and f31 ZERO across the loop, but several
setter calls explicitly reload the same constants; retain callback/order where
the constructor models source services. These are source defaults, not a
generic rendering policy or inferred zero allocation.

SetPartsScale writes current scale P60/64/68; SetPartsBaseScale writes
base scale P6c/70/74. Each scalar setter compares incoming axis against source
float sentinel804226c4=cd6e6b28 (-250000000) with fcmpu; equal retains that
axis, otherwise stores. Init's supplied1 is not sentinel, so all axes actually
write. RotateOffset has the same per-axis sentinel gate and writes P54/58/5c;
its supplied0 writes all axes. The two part rotate setters simply store their
three incoming float arguments, without angle normalization or sentinel gate.
Do not import unit rotate normalization into these different part leaves.

The remaining part-tail copies include colorffffffff P4f0, source entry
forward bias into HALF P1a0, authored pose table P1bc, handle-1, empty pending
name and actual P208/P20c=1. These were recovered separately; scale/rotation
defaults do not replace their owner. Source radius P1a2 is descriptor S30,
not forward bias P1a0 or unit width Ucc.

## Actual player source descriptor values

Read-only original player descriptors give:

| Part | Source descriptor | S30→P1a2 / S32→P1a4 |
| --- | --- | --- |
| Mario body1 | 80372a18 | 20 /36; raw00140024 |
| Mario part2 | 80372a64 | 20 /36; raw00140024 |
| Mario part3 | 80372ab0 | 20 /36; raw00140024 |
| Goombella body1 | 80393ba0 | 20 /24; raw00140018 |

All four authored local S+c/10/14 triplets are0,0,0. Native must still own
their scale defaults rather than using this zero offset to skip initialization:
other parts, future offsets and the actual shadow/draw consumers use those
fields independently. Enemy/Frankly parts use their original descriptor copies,
not these player radius constants. Replaced parts need the original replacement
initializer, not an unconditional per-frame reset to fresh Entry values.

## Current native gap and concrete repair inputs

At inspection source/battle/trio_attack.cpp's custom constructor owns U90..a4
unit scale1 and copies P24, P170 and P188 triplets plus several descriptor
words. Its final loop sets pose rates/colors/handles. It omits the above current
and base PART scales, P78 and descriptor P1a2/P1a4. Fresh native storage leaves
those bytes0; unit scale1 does not make part scale1. It also omits explicit
part rotate/pivot zero setter history, though newly cleared bytes numerically
match their supplied zero values. This must remain distinct from proven source
calls or initialized reused records.

GetPartsWorldPos uses the real part-local/owner/facing/scale fields; existing
packed service may be reused after source fields are installed. Current players'
zero local offsets can conceal a missing scale in one position query; they
do not prove full rendering initialization. Shadow radius should read the
actual copied signed halfword. Binding its draw consumer remains a separate
source/render step; this document does not invent a shadow size/opacity or
claim the queued original part drawing is complete.

## Evidence hashes

Ignored original listings: display-alpha-BtlUnit_Entry, partdefaults-scale,
basescale,pivot,baserotate,rotate under artifacts/recovered-temp-evidence.

| Original span | SHA256 |
| --- | --- |
| Entry part allocation/loop80129398:218 | e1a07827c27446916fd131902bab1568bf077bce9864a3f587c1c959a78c8119 |
| SetPartsScale80128470:34 | bfc992858b5aacc35e47d07fa57b612891e26b10356adc8437c58e548c43ffd3 |
| SetPartsBaseScale801284a4:34 | 5fec7daf19c88142b218546959ac23ae88e59f1e22adf9e3d4a4f480de86e013 |
| SetPartsRotateOffset801285d0:34 | c6053f6b4dbb353cc328818925c94b6c566393a9311375ac9e81ed755dda34a0 |
| SetPartsBaseRotate80128654:10 | 473beb07b076bbf52f19537606519838c337da49a6c284fb1761df1a7e936059 |
| SetPartsRotate801287b4:10 | 9093868a805df1a0a39b1f5b01a1f228c175ad0df5f28600b354ec3cb6840db1 |

Source-only documentation. No native/shared docs/index/progress edits or commit.
