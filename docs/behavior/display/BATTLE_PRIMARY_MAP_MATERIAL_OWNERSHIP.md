# Battle primary-map material ownership and fog retention

US G8ME01 disc0 revision0, static read-only ISO verification, 2026-10-14.
This narrows the current rank0 underground trio integration of
[last-stage color/fog](MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md) and the
[battle light/fog lifecycle](BATTLE_MAP_LIGHT_BANK_AND_PRESET_LIFECYCLE.md).
No original instructions were executed and no image parity is established.

## Primary record and fresh-load state

Material helpers resolve G=WORD[8041e660]+0x2f4*WORD[8041e664]. In bmapLoad,
80028a44 writes selector1, 80028a4c selects base+0x2f4 and 80028a58 clears
its complete0x2f4 bytes. Slot0 loads at80028a68; optional slot1 loads at80028a88.
Each slot has stride0x178. The material helpers use G, not G+0x178: loading
stg_00_1 alongside bti_01 does not make its enabled fog the character fog.

| Slot | Source | Fog words(enabled,type,start,end,RGBA) |
| --- | --- | --- |
| 0 | m/bti_01/d | 0,0,0,1000,000000ff |
| 1 | m/stg_00_1/d | 1,0,300,1500,1a1e05ff |

The fresh clear leaves G flag bits2/4/8 clear and all corresponding blend
color bytes zero. Their zero values are not white defaults; with flags clear,
last-stage blending does not read them. Do not invent or read admitted colors
from the other slot. Current trio loading has no implemented later map-color
writer, so a native fresh projection may admit this exact cleared state.
Future original event writers must mutate the same owner or hold until bound.
The separate pose translucency projection must share actual map flags and the
first two color alpha bytes; its two separate group reloads remain as documented
in [inner draw admission](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md).

The color leaf setters8001d99c and8001d9c4 write words at G+1a and G+16,
respectively, and OR flag4 or2. Their clear leaves8001d8f4 and8001d914 only
clear those flags; they do not erase the stored bytes. A disabled flag does not
prove its payload is unknown or white, and a fresh zero payload does not remain
constant after a real writer.

## Fog loading precision

When a fog table exists, _mapLoad8002962c..80029664 copies all five table words
to the chosen slot+24..34. At8002966c..80029680 it ORs flag1 only if enabled
is nonzero. **A present table with enabled0 does not clear a preexisting flag1.**
The separate absent-table branch80029684..800296b0 writes defaults and clears
flag1. The fresh bmapLoad memset is why bti_01's enabled0 gives disabled fog;
this result must not be generalized to reloading arbitrary uncleared records.
This refines the broad setting/clearing wording in the lifecycle contract.

The native PMLTSC01 pack retains both table records in source order. Use its
primary record only with the real selected battle-map owner/lifetime. A valid
background record or a generic texture binding is not a substitute for that owner.

## Camera and held request ownership

mapSetMaterialFog calls camGetCurPtr at800217d4, then saves its returned pointer
in r5 at800217dc. Only then does it resolve G and test flag1. For enabled fog,
it loads near/far from the retained camera+30/+34 at8002181c/80021820 after
reading group fields and resolving the five-entry fog type table. Retain that
camera pointer across a provider hold; do not prefetch near/far before the getter
or replace it with a later current camera. Disabled fog never dereferences the
camera fields, but the getter still occurs and completes before fog-off.

A resumable native helper should retain resolved group identity and captured
flags at their source points, capture an admitted complete GX request once,
and retry that exact request until acknowledged. Last-stage blend captures its
G pointer before arithmetic and its group flags after the C2 red-byte alpha
stage; admitted group color bytes are read at their own stage. These captures
must not be flattened into an eager snapshot of all mutable inputs.

The outer shape renderer separately calls camGetPtr(4), then camGetCurPtr and
compares pointer identity before invoking the fog helper. That outer getter does
not replace the helper's own getter. Lighting's earlier fog-off request also
does not replace the admitted helper invocation.

## Rechecked evidence

Private artifacts/material-group-evidence/current-map-original-check.json checks
917 original instruction words directly against the read-only ISO: bmapLoad95,
_mapLoad760 and material color leaves62. It also rereads both original map fog
chunks. Body hashes for those exact spans are:

| Span | SHA256 |
| --- | --- |
| 8002892c..80028aa8 | db1c37ba1723eb44e26223fdc78d595381d71abe136fa9c7987691c0024e55d7 |
| 80028f18..80029af8 | 59a90a079ae4874e80bed0a4803949763badc1f2f20f55b7a31565e747cf8621 |
| 8001d8f4..8001d9ec | 0f1eb7097b2725668193e8b8a790ee4171fae6c3bf5f5056ddc8507566667769 |

`python3 tools/verify_animation_texture_light.py --output artifacts/material-group-evidence/verified-original-material.json`
exited0:14 complete bodies,1189 words,11 constants,8 spans,2 imports,41 direct
sites and65536 host byte-product arithmetic cases. Source identities for both
map assets remain in the lifecycle contract. The checked-in text contains no
original asset payload. Real GX/PICA fog/blend behavior, later mutable writers,
geometry submission and hardware comparisons remain open.
