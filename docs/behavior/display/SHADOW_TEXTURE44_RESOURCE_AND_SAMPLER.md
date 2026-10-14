# Shadow texture44 resource and sampler

US G8ME01 disc0 revision0, read-only static source/resource inspection
2026-10-14. Refines [shadow admission](ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md)
and [request lifetime](BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md).

## Source identity and descriptor index

shadowCharShadowDisp_Texture8002c484/394 emits `li r3,44` at8002c508,
bytes3860002c, calls effGetTexObjN64 at8002c50c with stack GXTexObj at
r1+18hex, then GXLoadTexObj at8002c518 for map slot0. Thus texture ID is
**decimal44, hexadecimal2c**, not hexadecimal44/decimal68.

effGetTexObjN64801d78b0/60 moves incoming IDr3 unchanged into r5. When
retained work.word4 is nonzero, it loads the work's palette pointer and calls
TEXGetGXTexObjFromPalette80037a80. That function indexes descriptor array
with `id<<3` at80037a90..aa4. There is no remap, effect-name-table lookup
or base offset. Therefore this is TPL descriptor44, zero based. The separate
eff_set_n64[] table of effect names is unrelated to the texture descriptor ID.

effTexSetupN64801d7910/110 first tries arcOpen("effect_n64.tpl",0,0).
Its disc fallback formats original string802fbb0c
`%s/e/%s/effect_n64.tpl`, with getMarioStDvdRoot result and language string
`us`80425b0c when GP.word16c is nonzero, otherwise `jp`80425b08.
Archive success unpacks and publishes loadedword4=1; async disc callback
unpacks before publishing loaded. A failed/not-yet-loaded owner instead uses
effGetTexObjN64's actual 1x1 I4 clamp/clamp non-mipmap dummy object. Do not
claim descriptor44 was consumed before the loaded branch is admitted.

## Original US resource and sampler

Read-only ISO file **e/us/effect_n64.tpl**, offset944965424,size235776,
SHA2563202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef.
TPL magic0020af30,count151,descriptor table byte12. Descriptor44 has
image header offset2804,palette pointer0,data offset66496.

| Field | Original value |
| --- | --- |
| Dimensions | 16x16 |
| GX format | 0, I4; intensity replicated to RGB and alpha by existing GX decoder |
| Palette | None |
| wrapS / wrapT | 1 /1, REPEAT /REPEAT |
| minFilter /magFilter | 1 /1, LINEAR /LINEAR |
| minLOD /maxLOD | 0 /0 |
| LOD bias | binary32 +0 |
| edgeLODEnable | 0 |
| TPL unpacked byte | 0 in on-disc resource |
| Base tiled payload extent | 128bytes, four8x8 I4 tiles |

TEXGetGXTexObjFromPalette's actual source compares unsigned header bytes21/22
(minLOD/maxLOD), so this object has mipmap=false. It passes source wrap and
format directly to GXInitTexObj. The following GXInitTexObjLOD uses original
min/mag, unsigned min/max converted to floats, bias, literal biasClamp=false,
header edgeLOD and literal anisotropy0 (GX_ANISO_1). There is no shadow-specific
sampler override between this object creation and GXLoadTexObj in this path.

Header36-byte SHA256:
d653f4609866ad85b6eecd8605261031bb4b660189b9a90e117dc603f40ea3dc.
Base128-byte tiled-payload SHA256:
4250b256fe6cf80a68e743dcb95be6167853d71ad6d5d08130292f4053ee1acb.
Conversion must retain source intensity/alpha, repeat and linear sampling;
scaling or a native sampler substitution must be declared separately.
Original rights are retained; this metadata grants no distribution license.

## Evidence and boundary

Fresh ignored listings/inspection ELFs and resource metadata are under
artifacts/reference/shadow-texture44/. Complete body SHA256:

| Body | SHA256 |
| --- | --- |
| effGetTexObjN64/60 | 3532b321a207e74d818bdabd00a2a0415261ead29507030b0eac166bd6f5d08f |
| effTexSetupN64/110 | 3f4919a9fa6a18a1ccb3ae7facb84121bd3f3e6c38e89cfb8f3357b52222459c |
| TEXGetGXTexObjFromPalette/dc | 7c513078ebe80f85256108702b3b74d65b9036b6a1115b88c8a3750d19819fe6 |
| shadowDisp/7b4 | ade549cabd0dcc3d9d69acec14ed8f09c77ff4b4c2e6e063a1c7ac1ce36819ad |
| shadowCharShadowDisp_Texture/394 | 93e96eb0a02ed5da8e7d0d4d05b85446b8a6bede90fb668f083fc022f25a403f |

This proves the loaded texture branch's asset identity and sampler, not a
replacement for ordinary type2 cylinder shadows. Texture-path type1,
nonzero radius, preparedflag2 and captured camera equality still govern
admission. Full projection/collision/color/lifetime and native raster parity
remain their own owners. No code/index/progress edit, emulator or hardware run.
