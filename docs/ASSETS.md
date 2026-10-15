# Asset sources, organization, and provenance

## Sources and storage

| Source or output | Location | Contents and handling |
| --- | --- | --- |
| US GameCube source | `/home/carson/games/roms/G8ME01.iso` | Open read-only. Do not copy the ISO into the repository or distribute it. `TTYD_ISO=/path/to/G8ME01.iso` selects another local image. |
| Editable original sources | tracked `assets/original/` | PNG textures/atlases, lossless model/animation JSON and PCM16 WAVs for the imported slice, with initial hashes and provenance. |
| Custom sources | tracked `assets/custom/` | Authored textures, atlas layouts, models, animations and audio; per-entry author/license. |
| Temporary GameCube extraction | ignored `local-assets/` | Extraction/compiler caches and converter manifests, grouped by room or subsystem. |
| Extracted Switch comparison data | ignored `local-assets/switch/` | Separate Switch 1.0.0 assets/audio; not evidence of GameCube behavior. |
| Converted build products | ignored `romfs/` | Runtime packs, textures, messages, audio and metadata consumed by native builds. Generated outputs are not source history. |
| Research captures and inventories | ignored `artifacts/` | Hashes, inventories, verifier output, emulator captures and temporary probes. |

The user explicitly changed the asset-history rule on 2026-10-14: editable
original and custom sources are now tracked. The ISO and compiled packs remain
excluded. The project has no redistribution license for Nintendo/Intelligent Systems
assets. Original dialogue remains source faithful. The default imported audio stays
byte faithful; explicitly authored audio can be selected through the asset
manifest with its own attribution. Story dialogue/IDs/control tokens are not
rewritten through this pipeline. Conversion is limited to native format, texture sizing, geometry
reduction, and presentation adjustments documented per asset family.

## Local asset groups

The ignored `local-assets/` tree is already separated by room and feature:

- **Shared field and map:** `map/`, `map-objects/`, `field-actors/`, `gor_00/`,
  `gor_02/`, `gor_03/`, `gor_04/`, `stage/`, `stg_00_0/`, `stg_01/`.
- **Opening and presentation:** `opening/`, `opening-backgrounds/`,
  `opening-book-pages/`, `opening-map-transition/`, `opening-ship/`,
  `opening-sounds/`, `demo-book/`, `demo-map/`, `demo-effects/`, `heading/`,
  `title-curtains/`.
- **Characters and story:** `harbor-actors/`, `harbor-event/`, `harbor-tutorial/`,
  `harbor-tutorial-combat/`, `harbor-tutorial-stage/`, `underground/`,
  `professor/`, `professor-scene/`, `professor-fence/`, `goomba-quest/`.
- **Battle and effects:** `audience/`, `battle-ready/`, `harbor-curtain/`,
  `trio-battle/`, `field-item-get/`, `save-block/`.
- **Messages and sound:** `messages/`, `fonts/`, `message-icons/`,
  `message-indicator/`, `audio/`, `audio-map/`.
- **Comparison/reference:** `original/` and `switch/`.

These folders are generated, ignored working data and may be absent in a clean
checkout. `romfs/` is the build-facing package; converter scripts under `tools/`
produce its files from the ISO or local Switch extraction.

## Documentation indexes

- [Audio assets](assets/AUDIO.md) and [audio mapping](AUDIO_MAPPING.md)
- [Camera assets](assets/CAMERA.md)
- [Field meshes and roads](assets/FIELD_MESHES.md)
- [Mario models and poses](assets/MARIO_AND_POSES.md)
- [Scripts, messages, and effects](assets/SCRIPTS_MESSAGES_EFFECTS.md)
- [Switch asset and audio research](switch/README.md)
- [Complete dated conversion/provenance ledger](ASSETS_ARCHIVE.md)

The archive records exact ISO paths, byte spans, conversion choices, fingerprints,
rights notes, and limitations for individual packs. Keep new asset provenance
there and add a link here when a new source family or storage group is introduced.

## Item-get audio control

Original fanfare and plaza stream provenance/rights remain recorded in the
[asset ledger](ASSETS_ARCHIVE.md). Final GameCube plaza PCM is original
`stg_gor2_32k`,32000Hz/stereo/3284008frames, hash
`d83abec5e38338a1740e5644523bcbddf34fc879d0af4a6906a6aeba7897fc6d`.
The final audio_map/config step selects that stream. New control envelopes
add no asset conversion and retain field PCM cursor/buffers through resume.
Original Nintendo/Intelligent Systems local-use/no-redistribution restrictions
and ignored extraction/runtime packs remain in effect.
[Exact control/asset validation](behavior/audio/NATIVE_ITEM_GET_AUDIO_VALIDATION_2026_10_14.md).

## 2026-10-14 — Original battle defaults and pose directories

Read-only US ISO sources `a/b_mario`, `a/a_mario-`, `a/c_pkr`, `a/c_pkr-`
remain private in `local-assets/trio-battle/`. The existing converter now includes
original `M_Z_1` and `PKR_Z_1` defaults alongside its selected animations.
Existing texture scaling, original geometry evaluation and tex3ds conversion
remain in use. Private `romfs/trio-attack-{mario,goombella}.pose-directory.bin`
sidecars retain original model/texture names and all85/32 descriptor names in
source order; converted subsets must not redefine original numeric indices.
Nintendo/Intelligent Systems assets; no redistribution rights. Neither model
payloads, packs nor the ISO are committed. See the
[storage/directory contract](behavior/display/NATIVE_BATTLE_PART_STORAGE_AND_POSE_DIRECTORY.md).

## Tutorial native menu labels and pouch state (2026-10-14)

No additional ISO payload was extracted. Existing original weapon/ItemData,
msg/US and icon packs supply native table pointers, labels and icons unchanged.
The field's complete original pouch snapshot is copied at battle entry; new-game
grants are not repeated. Mapped message RAM copies existing localized label
bytes/control tokens and retains original key pointers internally. Licensing
restrictions and ignored private pack paths remain unchanged. No texture,
geometry, lighting or generated dialogue conversion was added.

## Native weapon-menu decision evidence (2026-10-14)

Shared setup80122068/6c and decision80123210/150 follow the existing full
read-only ISO verifier and reuse original weapon, message and candidate data.
The tutorial decision writes actual target state without new game payload,
rewritten labels, texture/geometry/lighting conversion or generated dialogue.
Private asset paths and Nintendo/Intelligent Systems licensing restrictions
remain unchanged.

## Original weapon-submenu control and sound requests (2026-10-14)

Shared Main/End and audience interruption use source bodies pinned by existing
read-only US verifiers. Raw source sound IDs/keys are retained for playback; no
generated tone, rewritten label or new game payload was added. Original menu/
HUD resources remain in existing ignored packs; rendering/animation integration
is still required. Texture, geometry and lighting conversion are unchanged;
Nintendo/Intelligent Systems licensing restrictions remain.

## 2026-10-14 — Book GPU capture and 400x240 framing

Reused original demo.tpl0..12, a/OFF_d_meku_right/A_1, companion rune back,
demo-page.pmanim and demo-page.bin. No extraction, texture scaling, source pose,
UV/material or dialogue conversion changed. Captured kind1 surfaces now use
runtime256-square GPU VRAM render textures instead of CPU-read framebuffer
images. Original386x480 capture rectangle is adapted to193x240; the cover/paper
viewport exposes the full native400x240 aspect, and the existing desktop raster
fills it with its original aspect and vertical cropping. All source artwork is
original Nintendo/Intelligent Systems material for private local use; no new
redistribution rights or committed ISO/extracted payload. Diagnostic captures
and native binaries remain ignored project artifacts.

Follow-up book framing reuses these same asset paths/conversions: the opening
pan centers the spine, then fades back toward the illustrated page during the
final zoom. Left-folded paper uses the same native pixel scale in X/Y to retain
the rune back's portrait proportions; captured-front dimensions stay193x240.
This changes native placement only, not source geometry samples, UVs, texture
scaling, original English story or licensing. See the
[book layout adapter](behavior/display/NATIVE_OPENING_BOOK_COVER.md).


## 2026-10-14 — Battle discrete state and model flags sidecars

Private romfs/trio-attack-mario.pose-directory.bin and
romfs/trio-attack-goombella.pose-directory.bin now use PMPOSE02. The
read-only ISO a/b_mario and a/c_pkr serialized models supply MODEL wordc4 and
each descriptor's independent T20-count/T40-relative discrete timeline.
Original order and binary32/uint32 bits are preserved by reversing individual
words into the little-endian native format. Names retain original directory
order; the existing converted geometry subset is unchanged.

Local read-only extracts local-assets/trio-battle/b_mario and c_pkr have SHA256
1f29b8e93280bcccf6a488c99a5f4addc35d5604e80d9afcbf5efcec64df51e4
and d712df83d7b70b64a90d45551b39ecf7adf277b9572f6931daae7792b52d69e7.
Their85/32 descriptors each have one time0/value0 state key; both model flags
are1. Sidecars are6604/2576 bytes, SHA256
fb67440a029810951f51896c2a3d3a958913ce0bdee44d2a99ce711fb957caf9
and131914fd21e94436191777c44b1c803336a5b4aee4ba1b42f175c04f0d6c689a.
Converter: tools/battle_pose_directory.py, invoked by trio_attack_assets.py;
[native format](behavior/format/NATIVE_BATTLE_POSE_DIRECTORY.md).
Nintendo/Intelligent Systems original assets; no redistribution rights. ISO,
raw models and generated sidecars remain outside source history. No geometry,
texture, lighting or dialogue conversion change in this step.

## Native parcel presentation (2026-10-14)

Native acquisition reuses private original US msg/US, Mario M_I_U, item95 icon,
window/font, effect.tpl images42/44 and original ff_get2_32k/stg_gor2 audio packs.
Exact ISO paths/conversion hashes remain in [the asset ledger](ASSETS_ARCHIVE.md)
and existing linked item-get contracts. Runtime defaults now select original
ISO music. No new texture scaling, geometry reduction, lighting conversion or
asset payload is introduced. Extracted local-assets/, compiled romfs/ and
capture/build artifacts/ remain ignored; Nintendo/Intelligent Systems assets
have no redistribution license. Isolated audio validation uses existing private
artifacts/roms/dspfirm.cdc copied only to that test profile, never committed.
[Native acquisition validation](behavior/field/NATIVE_GOOMBA3_PARCEL_ACQUISITION.md).

## Original coin reward source (2026-10-14)

Read-only ISO research pins `sound/stream/ff_get4_32k.stm` for original row219
BGM_FF_GET_ITEM1, SHA256
9abc00c564a50d6b3dadc5a76ff11ab428a0556152a7d1550502bf03451d38de.
It is stereo32000Hz/65352 frames, nonlooping by COMMON restartFFFFFFFF despite
DSP loop flags. [Exact resource/container evidence](behavior/audio/GOOMBA3_COIN_FANFARE_ROW219_RESOURCE_AND_NONLOOP_RETIREMENT.md).
Original msg/US/global.txt coin title and original gor_01 reward text retain
IDs/control tokens. Icon.bin403 refers to icon.tpl414..417, duration6 each;
effect7 reuses original effect.tpl42/44 with its own scale/color. Private
inspection listings remain ignored artifacts/reference/goomba3-services/coin*.
These increments add portable logic/text expansion; no new PCM/Tex3DS pack,
texture scaling, geometry reduction or lighting conversion is introduced.
Native conversion/playback is next. Original Nintendo/Intelligent Systems
local-use/no-redistribution restrictions remain; no extracted payload committed.

## Native coin window and effect7 (2026-10-14)

`f82d32cc` reuses existing original msg/US/global.txt coin title, original
window/font packs and effect.tpl42/44. Only source kind7 scale/palette and
camera selection differ from kind6; no generated texture/color bake, new
resampling, geometry reduction or lighting conversion. New icon403 research
confirms original icon.tpl414..417,40x40CMPR/duration6; those frames are not yet
added to the runtime atlas by this increment. [Source icon evidence](behavior/display/COIN403_ICON_ANIMATION_VISIBILITY_AND_CAMERA5.md).
[Native rendering validation](behavior/display/NATIVE_COIN_WINDOW_AND_EFFECT7_PROVIDERS.md)
records ignored private binaries/profiles/captures; no extracted payload is
committed. Existing Nintendo/Intelligent Systems local-use/no-redistribution
restrictions and ignored local-assets/,romfs/,artifacts/ remain in effect.

## 2026-10-14 — Original TimingA A-button HUD art

Read-only US G8ME01 icon.bin descriptors108/109 at offsets2442/2454 select unchanged
icon.tpl images108/109, each one non-looping48x48 frame with delay0. Added these
IDs to tools/message_icon_assets.py. Existing conversion packs the original
RGBA pixels into the512x512 atlas with2px transparent padding and no resampling;
private outputs remain local-assets/message-icons/{108,109}.png and atlas.png,
romfs/message-icons.{bin,t3x,json}. No payload is added to source history.

Native camera8 icon requests retain original IDs, scale1, flags16 and source
position arithmetic. Presentation adapts centered UI coordinates at half scale
to the top400x240 screen. Physical A remains the action-command input; no touch
requirement or rewritten story/control-token payload was introduced. Original
Nintendo/Intelligent Systems resources remain copyright-protected with no
redistribution rights. The prepared isolated review atlas/build is recorded in
PROGRESS; original ISO is never modified or committed.

## Original coin403 frame pack (2026-10-14)

Read-only ISO `icon.bin` descriptor403 at178a selects actual `icon.tpl` images
414..417, each40x40CMPR/duration6. Converter now includes all four in the existing
message/UI icon atlas. Private extraction is local-assets/message-icons/;
compiled romfs/message-icons.bin/.json/.t3x remain ignored. Original pixels are
RGBA8 decoded, packed with2px padding without resampling; atlas stays512x512,
1MiB native texture. No authored replacement, geometry or lighting bake.
Converted original RGBA byte hashes:

| Original TPL index | RGBA SHA256 |
| --- | --- |
| 414 | 7f85c9f3cbcb704994ef58023af5823e04d6ed5dc1261a151ff7d606e74b0a03 |
| 415 | 125d5a4232222ea046562186884f5c19003a7b1839ab3b3622f25a693e1b6de7 |
| 416 | 61ebc34fea5111bb00248f45c4c9f437c17b0f415de4a4ea82acc2f8d368ceff |
| 417 | fd057d9f758f01a8c1b59a15a3d947b257e430c78afff6bdaed95eeb7993f7c1 |

Hashes/audit also live in ignored artifacts/coin-icon-pixel-audit.json. Native
animation/preview evidence: [provider validation](behavior/display/NATIVE_COIN403_ANIMATION_AND_CAMERA_PUBLICATION.md).
Original Nintendo/Intelligent Systems local-use/no-redistribution restrictions
remain; extracted/compiled artwork and embedded native binaries are not committed.


## 2026-10-14 — Complete Frankly animation bank and battle metadata

Read-only ISO a/c_kuri_h and a/c_kuri_h- remain under ignored
local-assets/professor/c_kuri_h and c_kuri_h.tpl. tools/professor_assets.py now
converts all17 original descriptors in source order, retaining Z_1 atindex0
and including original D_2. It emits ignored romfs/professor.pose-directory.bin
(PMPOSE02 original model flags/names/discrete timelines) alongside the shared
professor.pmanim, professor.pmmesh, professor.t3x and professor.bin. Existing
surface-mesh/texture conversion is reused:15 original textures,128-pixel tiles,
1024x256 RGBA8 atlas, no additional rescale or invented animation. Native pack
is211407 bytes; sidecar1436 bytes. Source/conversion and binary hashes are in
artifacts/frankly-bank-validated/ASSET_SHA256SUMS; private professor.json retains
provenance and converter fingerprint. Neither extracted assets nor runtime
packs are committed. Nintendo/Intelligent Systems assets have no redistribution
license. [Native scope](behavior/display/NATIVE_FRANKLY_BANK_AND_FIRST_DISPLAY_FOOTERS.md).

## Original coin reward fanfare219 (2026-10-14)

Read-only ISO sound/stream/ff_get4_32k.stm at offset1337188296,107744 bytes,
SHA2569abc00c564a50d6b3dadc5a76ff11ab428a0556152a7d1550502bf03451d38de,
is extracted under ignored local-assets/field-item-get/. Existing original DSP
coefficient/history decoding produces stereo32000 PCM,65352 frames without
resampling. COMMON restartFFFFFFFF overrides DSP loop flags: no.loop is emitted.
Ignored romfs/field-item-get-coin-fanfare.wav is261452 bytes, SHA256
abac6f4f04a56bad20ba8f3a5f1621cabcc1aaa8a809f445416a730bcc424467;
romfs/field-item-get.json retains source provenance and row219 volume80/pan64/
global-volume bypass. Private reviewed packs also reside under ignored
artifacts/source-checks/quest13-field/. No generated replacement or ISO mutation.
Nintendo/Intelligent Systems original assets have no redistribution license;
extracted streams, PCM packs and embedded binaries stay outside source history.
[Provider and validation limits](behavior/audio/NATIVE_COIN_FANFARE_ROW219_PROVIDER.md).

## 2026-10-14 — Original Mario command pose for physical target cancellation

The actual original Mario pose-table lookup for type67 selects M_C_3. Added
that unchanged animation from read-only ISO a/b_mario and textures a/a_mario-
to the tutorial actor converter, alongside M_S_1/M_N_2/M_D_1. Private outputs
remain local-assets/harbor-tutorial/ and romfs/harbor-tutorial-mario.{pmmesh,
pmanim,t3x,bin,json}. Existing mesh/animation conversion and1024 atlas crop to
512 via LANCZOS are retained; no new geometry reduction, lighting override,
generated art, dialogue or invented pose name. Original animation is sampled
by the existing tutorial renderer; host validation checks real vertex output
under its12000-vertex budget. Nintendo/Intelligent Systems copyright applies;
no redistribution rights. Original ISO and converted payloads stay outside
source history. Native source command-pose helpers copy the selected original
name; physical B remains the cancellation input.

## Trouble Center map inspection and menu references (2026-10-14)

Read-only original m/gor_02/d at ISOoffset991830984,size1513698,SHA256
 d738bff28f92ed2a2d0658fb836b0a6e91b915a32553897b47032febe2f62353,
was inspected for authored A_i04_keizi/A_i04_kunter hierarchy transforms.
Source inspection listings stay temporary/ignored; no new map conversion or
native interaction anchor is inferred from bbox midpoints. Portable menu
references original icon descriptors504/524/134/136; no new native menu atlas
or generated artwork is packaged in this step. Existing local original maps/
compiled packs remain outside history. Nintendo/Intelligent Systems assets have
no redistribution license; no ISO/payload/binary committed.
[Source paths and limits](behavior/field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md).


Original US msg/US/gor_02.txt inspection: ISOoffset1177698108,size128972,
SHA2561958e3d22bb27b01332e08b67f279a33f21e0e8a3aba2b947ad8c2a446144f16.
All64 menu title/client/content IDs exist and fit source21/31-byte cache slots;
per-ID original byte offsets/hashes remain in temporary inspection JSON. This
is source resource inspection, not a new text conversion or authored replacement.
No original message payload enters source history. [Display/US bindings](behavior/display/TROUBLE_CENTER_ORIGINAL_WINDOW_DISPLAY_AND_US_CACHE.md).

## 2026-10-14 — Original class-menu icons334..343

Read-only US icon.bin descriptors334..343 select original icon.tpl images339..
348, each one non-looping48x48 frame with delay0. tools/battle_class_icon_assets.py
packs their exact RGBA pixels into a256x256 sheet with2px transparent borders
and no resampling/geometry/lighting changes. Existing message/HUD sheet remains
512-square; the separate class sheet costs262144 bytes of RGBA8 texture storage.
Private outputs: local-assets/battle-class-icons/{334..343}.png and atlas.png;
romfs/battle-class-icons.{bin,t3x,json}. The metadata uses the existing PMICON1
frame format so future UI consumers retain IDs and frames. No generated icons,
rewritten labels or ISO/source-payload commit. Native class texture binding is
not yet implemented. Nintendo/Intelligent Systems resources have no granted
redistribution rights; all extracted/compiled payloads stay ignored.

## 2026-10-14 — Animated Crump opening field actors

Read-only `/home/carson/games/roms/G8ME01.iso` sources `a/c_kanbu_ibt`,
`a/c_kanbu-`, `a/c_zako_n`, `a/c_zako_n-` now produce private
`romfs/harbor-crump.{pmanim,t3x,bin,json}` and
`romfs/harbor-xnaut.{pmanim,t3x,bin,json}`. Original Z_1, S_1, T_1, W_1 and R_1
clips retain authored geometry, discrete visibility and continuous animation.
Existing converter places original textures into a bordered atlas, crops unused
rows and scales1024-wide art to512 with Lanczos; the binary retains the UV scale.
Independent actor pose/facing owners share immutable enemy banks and texture
resources. Original Goombella clips already exist in `partner.pmanim`.
Harbor Mario adds original M_R_1 from `a/a_mario`, sharing `a/a_mario-` with the
existing ordinary/effect bank conversion. The old baked enemy pack remains
only for legacy message-anchor metrics; live enemy art uses animated geometry.

Source tribe records `0x803316d0`, `0x80332690`, `0x803326e4` establish the pose
profiles/run thresholds; private JSON retains enemy source hashes and profiles.
Extraction/conversion stays in ignored `local-assets/harbor-actors/`,
`local-assets/harbor-event/` and `romfs/`. Original Nintendo/Intelligent Systems
copyrighted assets: local use only; no redistribution rights granted. No ISO,
extracted originals or compiled packs enter source history.

## Trouble Center original cached window backend (2026-10-14)

Existing ignored romfs/messages/gor_02.txt, font.pmfont/font.t3x and original
message-window-panel.t3x are reused. All30 original client/contents strings copy
verbatim including NUL into21/31-byte caches; four header pointers retain the
original message bank. No text/art replacement, resampling, new atlas or payload
conversion. Original f/msgWindow.tpl descriptor0 centerRGBA(216,219,216,255)
modulates source white/cyan primitive colors; the native preview matches that
texture color, rather than generated purewhite. All original asset/binary outputs
remain ignored and Nintendo/Intelligent Systems no-redistribution restrictions
apply. Private packs under artifacts/source-checks/quest13-field reuse these
resources. Existing battle_class_icon_assets.py additionally generated its own
ignored original icon pack for an independent host validation dependency.
[Exact source and preview scope](behavior/display/NATIVE_TROUBLE_CENTER_WINDOW_AND_US_CACHE.md).

## 2026-10-14 — Book cinematic visible-region framing

Same original demo.tpl0..12 and OFF_d_meku_right/A_1 assets and existing
conversions. No raster, UV, source pose, dialogue or music asset changes. Native
cover/paper consume the original settled49/44 letterbox visible region as a
uniform outer viewport transform; captures retain193x240 content in their
existing256-square textures. Original Nintendo/Intelligent Systems licensing
and private-local-use restrictions remain. Dolphin/native comparison PNGs and
playtest binary stay in ignored artifacts; ISO remains read-only.

## Trouble Center retained original paging icons (2026-10-14)

Read-only original icon.bin/icon.tpl descriptors524/134/136 are added to the
used-icon converter alongside existing504. 524 uses TPL56224x32;134/136 use
TPL134/13660x48. 504 retains ten40x40 frames533..542 with delays2 for nine and
28 for the last. The512x512 RGBA8 atlas remains1MiB. Identical decoded pixel
cells share storage without collapsing descriptor frames/timing; two-pixel
transparent padding/no resampling remain. Outputs stay in ignored local-assets
and romfs packs; the ISO and extracted assets never enter history. Nintendo/
Intelligent Systems resources have no redistribution rights. [Source hashes,
clock and geometry](behavior/display/TROUBLE_CENTER_RETAINED_2D_ICONS_AND_LOCAL_CLOCK.md).
Original menu cue samples are documented in the [audio contract](behavior/audio/TROUBLE_CENTER_MENU_SFX_BINDINGS.md);
no new PCM pack or native audio admission is included in this step.

## 2026-10-14 — Original class circle and speech-bubble surfaces

Read-only `/home/carson/games/roms/G8ME01.iso` resource
`battle/common/battle_common.tpl` (SHA256
`8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1`)
provides original palette indices0(40x40 circle quadrant) and1(128x80 bubble).
The original battleMenuDispInit pathname and DrawMainIconCircle/Hukidasi indices
are pinned by tools/verify_battle_class_raster.py. The separate
battle/win/menuwin.tpl was inspected privately and is not selected.

`tools/battle_class_surface_assets.py` emits ignored
`romfs/battle-class-surfaces.{bin,t3x,json}` and original decoded images/atlas
under ignored `local-assets/battle-class-ui/`. Packing uses256x128 RGBA8,
131072 texture bytes,2px clamp-edge extrusion and no source-image resampling. The existing
class icon atlas remains separate. Geometry/UV quadrant mirroring, source
shadow/dimming and label scaling are consumer behavior, not painted into the
textures. No generated art or rewritten labels were added. Nintendo/Intelligent
Systems assets have no granted redistribution rights; do not commit or distribute
original ISO, extracted images or compiled packs. GPU texture binding remains a
native renderer task; imported assets alone do not prove visible menu parity.

Original class surface/icon GPU lifetime admission now has a dedicated owner
(`battle_class_surface_gpu.hpp/.cpp`). It reuses the exact unchanged
battle-class-surfaces.{bin,t3x} (256x128 RGBA8) and
battle-class-icons.{bin,t3x} (256x256 RGBA8, IDs334..343) products. Both Tex3DS
imports must describe one unrotated whole image with exact native dimensions,
format and UV orientation; exact existing atlas-region metadata is checked.
Native sampling remains linear and clamp-to-edge for these padded atlases.
One atomic pair generation owns both imports and immutable catalog; copied
menu publications reject retired/replaced generations before GPU effects.
No new pixels, scaling, atlases, source TPL identity or licensing are introduced.
The original battle/common/battle_common.tpl source hash above and original
icon.bin/icon.tpl descriptor/image indices remain authoritative. Their read-only
ISO hashes are icon.bin8174 bytes SHA256
0be6b95a7e45e5d932641f24f8240a289a852889f1906d32be839046a680532b and
icon.tpl677792 bytes SHA256
fb8e99398fd5ef3c36cc518077748405949c004fcc5c515cc8828a64340b74f9. Lifecycle
admission does not initialize original battle file resources, schedule a class
callback or establish an original camera. See
[GPU ownership contract](behavior/display/ORIGINAL_CLASS_SURFACE_GPU_OWNERSHIP.md).
Original Nintendo/Intelligent Systems rights remain; these private packs grant
no redistribution rights and are not committed.

## 2026-10-14 — Tracked editable assets and native source pipeline

[Asset editing/build guide](../assets/README.md),
[active recipes](../assets/manifest.json),
[import selection](../assets/import.json) and
[original provenance](../assets/original/provenance.json) are source history.
The first import includes **420** assets: 323 original PNG textures/atlases,
six static model JSONs, 21 actor pose JSONs, 42 animation JSONs and 28 PCM16
music/effect WAVs. No ISO or compiled runtime pack is included.

Texture sources include `a/a_mario[-]`, `a/a_mario_r[-]`, `a/e_mario`,
`a/c_pkr[-]`, `a/c_kanbu[-]`, `a/c_kinopio[-]`, `b/gor_01.tpl` and
`m/{gor_00,gor_01,gor_02,gor_03,gor_04,stg_01}/t`. Existing converter PNGs
retain their resizing and cell/border layout; the new compiler performs no
additional scaling. Static geometry comes from those rooms' `d` resources
through the existing map converter. Named PMCHAR1 poses and PMANIM3/4 clip
arrays retain the existing original model/animation conversion, exported into
lossless JSON. Native export paths and initial source SHA256s are recorded in
the tracked ledger; detailed ISO paths/conversion choices remain in the
[earlier ledger](ASSETS_ARCHIVE.md) and subsystem asset documents.

Audio imports copy the slice's current original GameCube PCM16 WAVs and
retain loop frame intervals in recipes. Sources include original
`sound/stream/*.stm` and primary samples from `sound/proj/pmario.samp` plus
its `sdir/pool/proj` mappings. Original rate, channels, samples and selected
source loops are unchanged. This does not import the separate Switch music
library or all 1247 GameCube samples; existing extraction targets remain available
for adding them. Every original entry retains Nintendo/Intelligent Systems
rights with no redistribution license. Custom entries carry their own license.

Normal builds restore their prior base files, run remaining ISO converters,
then compile `assets/manifest.json`. Staged compilation, content/tool/dependency
cache hashes and a reversible install ledger prevent stale replacements after
an entry is removed. `assets-import` is explicit and refuses to overwrite
authored sources. The remaining base converters still require the local
read-only ISO; this is not yet a complete ISO-independent checkout. New asset
names require native resource/trigger bindings, and changed render geometry
requires separately updating collision/animation companions.

Initial import verification found **454/454 generated outputs byte-identical**
to the native base pack, including texture compression, geometry, pose/animation
bytes and audio/loop data (`artifacts/asset-pipeline-roundtrip.json`).

## 2026-10-14 — Private icon refresh for code-documentation validation

The Game documentation snapshot uses a private RomFS copy under
`artifacts/game-code-documentation-validation/source/romfs/`. Its three
`message-icons.{bin,t3x,json}` files were regenerated by the frozen existing
`tools/message_icon_assets.py` from read-only `/home/carson/games/roms/G8ME01.iso`.
Source resources remain `icon.bin`, `icon.tpl` and `e/us/effect_us.tpl`; temporary
extraction/atlas work is under
`artifacts/game-code-documentation-validation/asset-refresh/local-assets/message-icons/`.
The validation snapshot also retains the matching regenerated extraction/atlas
cache at `artifacts/game-code-documentation-validation/source/local-assets/message-icons/`
so pixel verifiers read the same atlas as the private UV metadata. Other local
asset directories remain shared read dependencies.
Existing conversion is unchanged: original RGBA pixels/delays/dimensions,
identical-cell aliases, rectangle packing into512x512 with2-pixel transparent
padding, no resampling, Tex3DS RGBA8. The shared pack lacked icon IDs524/134/136
required by already-committed Trouble Center tests; only the isolated validation
copy was refreshed. `manifest.json` records refreshed-file and copied-RomFS hashes.
Nintendo/Intelligent Systems resources remain copyrighted with no redistribution
rights granted. ISO, extracted resources, textures and compiled packs stay outside
source history; this entry records provenance only.

Final frozen validation passed 263/263 CTest and 159/159 Python checks and
normal `make all`. Regenerating the base pack still produced 454/454 identical
outputs from tracked sources; exact logs, frozen source/native binary hashes and
comparison are in `artifacts/asset-pipeline-validation/`. The tracked
[upstream conversion records](../assets/original/conversion-provenance.json)
retain detailed original source identities/settings. No emulator/hardware parity
is asserted for this pipeline change.

## 2026-10-14 — Original deferred shadow texture44

Read-only US ISO `e/us/effect_n64.tpl`, descriptor44 (decimal/hex2c),
16x16 GX_I4. `tools/shadow_texture_assets.py` verifies the source bank, header
and raw RGBA hashes; converts intensity to exact RGBA8 native tiling without
resize, padding, mipmaps or replacement art. Private source/PNG/provenance:
`local-assets/shadow/`; compiled texture and sampler/provenance metadata:
`romfs/shadow-texture44.t3x` / `.json`. Repeat S/T, linear min/mag and LOD0
must be applied by the eventual native binding. Full identities and original
loaded/dummy branch are in [the resource contract](behavior/display/SHADOW_TEXTURE44_RESOURCE_AND_SAMPLER.md).
Rights remain Nintendo/Intelligent Systems; private local conversion grants no
redistribution license. No original payloads or compiled packs are committed.

## Original Trouble Center front door and kanban sign (2026-10-14)

The existing original gor_02 map supplies both front hits,18-node A_ie_04C and
69-node outer cutaway collision scope. Converter group13 tags S_ie_04_soto and the actual animated wall nodes
S_ie_04_B/S_ie_04_ura_B for the declared binary cutaway;
no geometry/texture substitution or resampling occurs. Door PMDOOR2 retains
original EVT integer destinations and source story23 access metadata.

New tools/message_kanban_assets.py reads f/msgWindow.tpl read-only, selecting
original29/31/30/32 for type4. Actual72x176/416x40 pixels are composed at560x176
with source upper-middle CLAMP edge extended96 rows; no stretch/resampling.
Packed1024x256 RGBA8 costs1MiB. Temporary extraction/decoded PNG/atlas remains
local-assets/message-kanban, compiled message-kanban.t3x/json remain ignored
RomFS products. No ISO/extracted art/message payload enters history. Nintendo/
Intelligent Systems resources have no redistribution rights. [Hashes, palette/
sampling/notice identity](behavior/display/ORIGINAL_KANBAN_TYPE4_WINDOW_AND_LOCK_NOTICE.md)
and [native scope](behavior/field/NATIVE_TROUBLE_CENTER_FRONT_DOOR_AND_LOCK_NOTICE.md).

## 2026-10-14 — Original audience frame atlas

`tools/audience_assets.py` reads the ISO read-only and preserves the unchanged
`battle/audience/audience_normal.tpl` (205 images; SHA256
`16f8cdda26bb3727aeb9c2d7b494d66c1822d2ac94705dbad07b09d319e5e4f2`)
and `battle/audience/audience_luigi.tpl` (19 images; SHA256
`3f556a8d9bd7cbfbba33ce3ab1a31531f174273451fe620f54182523533c48ef`).
Every original64x64 CMPR frame is decoded without resizing and packed into a
1024x1024 RGBA8 atlas with68px cells and2px transparent padding. Allocated native
texture storage is4MiB; compression of the T3X changes file size only. Mipmaps
are not generated. Existing native bank handles1/2 and original image indices
remain distinct; no type, animation timing or frame-selection table is invented.

Private source banks/PNG: `local-assets/audience/`. Private compiled products:
`romfs/audience-frames.t3x`, `audience-frames.bin`, `audience-resources.json`,
and existing unchanged `audience-normal.tpl`/`audience-luigi.tpl`. Metadata records
per-frame pixel hashes and top-left/bottom-right native UVs. Catalog PMAUD001
has a20-byte little-endian header (magic, width, height, frame count), followed
by224 records of32 bytes (bank,index,width,height as4 uint32, UVs as4 float32).
`BattleAudienceResources::loadFrames` validates the complete fixed layout and
publishes atomically; failed loads retain the previous valid catalog. `frame`
rejects unknown banks and out-of-range indices. This is a native adaptation,
not the original fileAllocf/GX pointer layout.

These assets remain Nintendo/Intelligent Systems resources; local extraction
and conversion grant no redistribution rights. No ISO, original pixels, compiled
catalog or texture is committed. Population, animation selection, texture/GPU
binding, original draw behavior and throw events remain separate work. Current
source setup4 tutorial capacity is zero; preparing resources does not change it.

## 2026-10-14 — Native shadow texture44 binding

The finishing renderer lazily imports private `romfs/shadow-texture44.t3x`,
validates16x16 native RGBA8/no mip levels/LOD bias0, and binds repeat S/T with
linear min/mag. The resource persists across frame restoration and is deleted
after GPU synchronization during renderer shutdown. This is an explicit native
loaded44 resource adapter; original full N64 palette/async/language lifecycle
and its 1x1 unloaded dummy remain unbound. [Native scope](behavior/display/NATIVE_ORDERED_SHADOW_SETUP_AND_CAMERA2_RAY_FRONTIER.md).
Original paths/hash/conversion/rights remain those in the prior texture44 entry;
no new original payload, resampling or redistribution license.

## Original diary/type7 and Trouble Center event data (2026-10-14)

tools/message_diary_assets.py reads original f/msgWindow.tpl index6, actual
560x176 CMPR; decoded pixels retain size/color/transparency. RGBA8 atlas1024x256
contains unchanged pixels at(1,1) plus one edge/corner CLAMP texel; native linear
filtering samples the original edge footprint. Texture costs1MiB; extractor/
PNGs live in ignored local-assets/message-diary, compiled t3x/json in RomFS.
No generated art or resampled texture replaces the original diary frame.

trouble_board_event_assets.py reads original rel/gor.rel and uses rel_data_pack.py
for205 bounded data objects/5segments/5464-byte PMEVT001 closure. Original event
words/message IDs and actual pointer-table dependencies are retained; executable
sections excluded, ADDR32 pointers relocated to the explicit native section map.
Temporary REL/metadata and compiled packs remain ignored. Both resource groups
are Nintendo/Intelligent Systems original assets with no redistribution rights.
[Source and native scope](behavior/field/NATIVE_TROUBLE_CENTER_DIARY_AND_SCOPED_EVENT_DATA.md).

## 2026-10-14 — Original audience animation and item-table data

Read-only US DOL `battle_audience_kind_data.o` owns182 sdata symbols in exact
address span8041c638/14e8, ending8041db20. SHA256:
`27efa23577efb6deb3204baf406fdda61546d5700ec4c25f04e8b59fad724cc4`.
`tools/harbor_tutorial_dol_assets.py` now imports only this hash-pinned immutable
extent beyond the existing80418000 cutoff. `tools/trio_attack_assets.py` reuses
that same original span. Private compiled products:
`romfs/harbor-tutorial-events.pmevt` and `romfs/trio-attack-events.pmevt`; existing
private metadata records pack extents/hashes. Original big-endian addresses,
frame indices, counters, loops, waits and item weights are unchanged. Native
mutable globals/pouch/current-battle/SDK heap descriptors stay outside the import.
No source executable text or replacement animation table is imported.

This binds source audience record animation pointers to original data used by
its existing interpreter and the prepared224-frame atlas. It does not bind
original crowd GX drawing or known-key positional audio. No additional raster
art or music was extracted. Nintendo/Intelligent Systems rights remain inherited;
local conversion grants no redistribution license. Original payloads, compiled
packs and evidence stay ignored. See
[population/animation ownership](behavior/battle/AUDIENCE_POPULATION_SEATS_ENTRY_AND_ANIMATION_OWNERS.md).

## Trouble Center event callback descriptor (2026-10-14)

trouble_board_event_assets.py now writes ignored romfs/trouble-board-program.bin,
196-byte PMIRAI01 containing source board/counter/installer/table addresses and
21 independently relocated callback addresses/tags. Original data-only PMEVT001
closure stays205objects/5464bytes. Original PPC functions are external service
identities, never exported/executed code. Existing read-only US gor.rel hash,
local extraction and Nintendo/Intelligent Systems licensing remain unchanged.
[Runtime/provenance scope](behavior/field/ORIGINAL_TROUBLE_CENTER_EVENT_RUNTIME_ADAPTER.md).

## 2026-10-14 — Independent original battle collision registry

`tools/battle_hit_assets.py` reads US ISO `m/bti_01/d` and `m/stg_00_1/d`
read-only, verifying bank SHA256213c62e758be15bc0731b3fa58558d1b2c565ea1c96e91111ea07b52cf751ca8
and8d749bd8caad7fced1596231b755dc4e372e938816e10add8a8e0d7a69f5a2e1.
Private originals/provenance: `local-assets/battle-hit/`. Compiled runtime
pack and metadata: `romfs/battle-hit.bin` / `.json`. Contains original selected
collision roots bti_A and A, five initial hierarchy/mesh slots and166 complete
source-winding triangles in engine units (source parent Scale10). No geometry
reduction/render offsets/native-coordinate round-trip. Single-precision initial
axis-aligned transforms/bbox expansion; declared native sqrt radius adaptation.
Initial flags0 snapshot and later unknown-history invalidation are separate
from original live bmapLoad/hitMain. [Native scope](behavior/battle/NATIVE_BATTLE_HIT_REGISTRY_AND_SHADOW_PREPARATION.md).
Rights remain Nintendo/Intelligent Systems; private local conversion grants no
redistribution license. No original payload or compiled pack is committed.

### 2026-10-14 — Original Trouble Center native UI primary samples

tools/trouble_board_audio_assets.py opens the supplied US ISO read-only and
converts sound/proj/pmario.sdir plus pmario.samp entries1041/1040/1064/1063
(original rows5/19/42/43) to ignored romfs/trouble-sfx-{row}.wav. Original mono
22050Hz DSP ADPCM becomes PCM16 with original sample IDs, counts and nonloop
metadata; romfs/trouble-sfx.json records bank/sample hashes and provenance.
Additional resident PCM is43314 bytes across four cues. Existing confirm.wav
is bit-identical to sample1043 (row18) and is borrowed without another copy.
Native NDSP20..23 use linearAlloc/cache flush and a conservative primary-sample
mix; full MusyX macro/envelope/voice/mix parity is not claimed. Original Nintendo/
Intelligent Systems assets are local-only; no redistribution rights are granted.
No extracted PCM, bank, ISO, pack or 3DSX enters these source commits.

## 2026-10-14 — Source-matched audience pack for shadow-pass validation

The isolated artifacts/shadow-pass-source validation uses its frozen converter
from tools/audience_assets.py to regenerate read-only US ISO
battle/audience/audience_normal.tpl and audience_luigi.tpl in
artifacts/shadow-pass-source/local-assets/audience/. Private RomFS overlays
contain audience-normal.tpl,audience-luigi.tpl,audience-frames.bin/.t3x and
audience-resources.json. Original64x64 CMPR pixels remain unchanged; this
frozen recipe has2px transparent guards and a1024-square RGBA8 atlas.
The live shared converter/pack retains its newer original-CLAMP edge extrusion;
its two original-resource tests pass after regeneration. No tracked recipe or
shared source asset was rolled back. Snapshot-only files and compiled packs
remain ignored. Existing bank hashes/license are retained in the manifests:
Nintendo/Intelligent Systems original assets, local use only, no redistribution
rights. This validation variant supplies no claim of audience draw fidelity.

## 2026-10-14 — Original audience sampler and atlas edge correction

The224 original normal/Luigi TPL headers uniformly specify CLAMP S/T, linear
min/mag (0,0,1,1). Source hashes and64x64 CMPR frame pixels remain unchanged.
Normal bank frame92 has a nontransparent image border. `tools/audience_assets.py`
now repeats exact original edge texels in each2px gutter instead of universally
transparent padding. Interior frame RGBA/indices/catalog coordinates are identical;
there is no frame resize, added artwork, additional atlas page or mipmap. Native
allocation remains1024x1024 RGBA8/4MiB. Private products retain the existing
`local-assets/audience/atlas.png` and `romfs/audience-frames.t3x`/catalog/metadata
paths. Compiled T3X changes because of correct sampler gutters; catalog is unchanged.
Native UV-boundary tessellation preserves the source1.075 end-pose coordinates
without atlas bleed or compressing in-range interpolation. Rights remain original
Nintendo/Intelligent Systems; no redistribution license is granted and no original
pixels/compiled packs enter history. [Draw/sampler contract](behavior/battle/AUDIENCE_DRAW_NORMALS_FLIP_AND_NATIVE_ATLAS_CLAMP.md).


## Original Borodo2 field actor (2026-10-14)

Read-only G8ME01 paths `a/c_borodo_n` (ISO871076364,21636 bytes, SHA256
`bdf18b38f90a584a2c4c9712f1720d80f7814f872da7cf284ac16f9e59b27747`) and
`a/c_borodo_n-` (ISO871098000,59520 bytes, SHA256
`17be5af817f6cd6f60208dcdb596393407a4f031f98c30a9fa6e2cb80b06664c`).
The gor_02 source actor is ボロドー２; tribe row80332d20 supplies the original
resource and S_1/T_1/W_1/R_1 poses. Index0 remains original Z_1.

`tools/field_actor_assets.py` uses the existing original geometry/sparse pose,
RGBA/pass/cull converter. Twenty used textures become124px cells with original
wrap-aware gutters in a1024x512 source atlas (UV scale2), reduced to512x256 RGBA8/512KiB
for the resident native room; normalized UVs remain unchanged. Untextured draws retain
neutral white. Native T3X/packed meshes use the existing Citro3D actor path.
No replacement illustration or generated sound is added. PMNPC009 records exact
original Shift-JIS names and finite init profiles separately from model identity.

Extracted inputs remain under ignored `local-assets/field-actors/`; generated
`romfs/field-borodo.*`, `field-actors.pmnpc` and metadata remain outside source
history. Original Nintendo/Intelligent Systems assets; local conversion does
not grant redistribution rights. See the [native contract](behavior/field/NATIVE_BORODO_POSITION_AND_ALL_TASK_CANCELLATION.md).


## Original Garf/c_monban actor (2026-10-14)

Read-only G8ME01 `a/c_monban` at ISO890977624,40764 bytes, SHA256
`595e3b0b8e905675490d2d0e1c808f18cf67114ecab16b024e6ca07d8a3e323b`;
`a/c_monban-` at891018388,47904 bytes, SHA256
`8a2433ec795c1ef5b140dcf4f4bc4879e8936c4fc3f82208580cd1e79611a68c`.
Source gor_02 盗賊団２ uses tribe row80333404, original idle/talk/walk/run
S_1/T_1/W_1/R_1 and30x30 dimensions; Z_1 remains animation index0.

The existing sparse original mesh/animation/material converter retains26 used
texture cells and original RGBA. Its wrap-aware1024x512 atlas is reduced to
512x256/512KiB for the resident room, preserving normalized UV ratios.
`local-assets/field-actors/c_monban*` and generated `romfs/field-garf.*`/catalogs
remain ignored, outside source history. Existing Citro3D model batches share
this one texture bank across actors. Original Nintendo/Intelligent Systems
resources; local conversion grants no redistribution rights. No new music or
replacement art was introduced. [Native scope](behavior/field/NATIVE_GARF_INIT_AND_ORDINARY_TALK.md).


### 2026-10-14 — Reuse original cylinder shadow geometry on native GPU

The native finishing provider reuses the existing ignored romfs/shadows.pmshadow
pack extracted from the read-only US G8ME01 ISO: original60 float positions,
five INDEX8 fan/strip lists and ordinary/no-decay RGBA palettes. No new payload
was imported or converted; original index order/winding and authored floats are
preserved. Prepared source matrices supply scale/translation. Citro3D consumes
palette bytes as normalized vertex colors; RGBA8/depth24 remains a declared
native target adaptation. Original game assets retain their original copyright;
private packs/ISO are excluded from source history and redistribution.


### 2026-10-14 — Retained empty audience callback resources

The native retained callback uses the original initialized actor bank and no
texture/geometry payload for all-rejected actors. No new art or original asset
conversion was introduced by this implementation. Isolated validation refreshed
its existing field-actors.pmnpc and original NPC packs with the committed
field_actor_assets.py recipe against the read-only US ISO to match its newer
source base. These private packs remain excluded from history/distribution;
original copyrights and the existing conversion ledger apply unchanged.


### 2026-10-14 — Original house05 mutable door-data closure

`tools/door_data_assets.py` opens the US G8ME01 ISO read-only and extracts
`rel/gor.rel` to ignored `local-assets/door-data/gor.rel` (SHA256
`d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a`).
The existing REL data packer follows original relocations from door_data_05 and
door_data_05_npc_01, retaining18 record words and exact SJIS name pointers:
27 objects/1048 packed bytes. Ignored outputs: romfs/door-data.pmevt, .json and
.bin. PMDRDT01 is native room/work/quest metadata for the source copy/list init
prefix; no executable PPC or replacement art/music is generated. Original
Nintendo/Intelligent Systems resources retain original copyright; local
conversion grants no redistribution rights. ISO/extracted data/packs remain
outside source history. [Native scope](behavior/field/NATIVE_AREA_DOOR_DATA_AND_GW11.md).


### 2026-10-14 — Retained item/AP draw dependencies

No new art payload was imported for these callbacks. Original item table
803108a8, immutable constants and DATA color80424978 are consumed through the
existing private trio-attack-events.pmevt DOL-data imports. Matrix/icon identities
remain source addresses, never ARM pointers. Native inactive banks require no
icon texture lookup; active original icon metadata/palette/geometry ownership
remains unbound. Isolated validation refreshed the newer base's existing
 door-data recipe against the read-only ISO, writing only its private packs.
All original assets retain their original copyrights and local-only handling;
ISO, extracted data, compiled packs and binaries remain excluded from history.


### 2026-10-14 — Original named field map/hit flag bindings

The existing read-only map conversion now also emits ignored PMFLG001 packs
from m/gor_01/d,m/gor_00/d,m/gor_02/d,m/gor_03/d,m/gor_04/d in the US ISO.
Original node names, parent relationships, source-relative node IDs and collision
membership accompany16-bit converted vertex owners. Existing vertex, batch,
ground, wall and head resources remain byte-identical; no new art, texture,
music or geometry reduction was introduced. Local outputs: local-assets/<map>/map.mapflags;
RomFS map.mapflags,gor_00.mapflags,gor_02.mapflags,gor_03.mapflags,gor_04.mapflags
and field-map-flags.bin. The five packs total1472942 bytes; PMFLDS01 room index
adds352 bytes. The semantic overlay is native storage policy, not original
MapObj/HitObj layout. Original Nintendo/Intelligent Systems copyrights and
existing source-map provenance apply; conversion grants no redistribution
rights. ISO/extracted/generated resources remain outside history.
[Native scope](behavior/field/NATIVE_NAMED_MAP_FLAGS_AND_HOUSE05_INIT.md).


### 2026-10-14 — Exact original Bero entry tables and query constants

`tools/bero_info_assets.py` reads US G8ME01 ISO read-only, extracts rel/gor.rel
to ignored local-assets/bero-info/gor.rel and verifies SHA256
`d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a`.
It selects bero_entry_data by exact gor_01.o/gor_00.o/gor_02.o/gor_03.o/gor_04.o
namespace, retaining15 original60-byte rows plus source terminators and relevant
strings with ADDR32 relocation. DOL data includes bero_null8030cd78/60,
prefix table802c2c28/32,strings804204c0/32 and original default_entevt80416060/20.
Per-span hashes and table provenance are recorded in ignored romfs/bero-info.json.
Outputs: romfs/bero-info.pmevt1828 bytes and bero-info.bin144 bytes; no executable
PPC, new art or music. Referenced other event bodies remain outside this pack.
Native section addresses/GP string alias are storage adaptations. Original
Nintendo/Intelligent Systems copyrights apply; conversion grants no distribution
rights. ISO/extracted/generated resources remain outside source history.
[Native scope](behavior/field/NATIVE_BERO_TABLES_AND_INCOMING_QUERY.md).


### 2026-10-14 — Ordinary battle part callback validation packs

No new art/audio is imported for this change. Source logic/constants remain the
read-only US DOL evidence in `artifacts/battle-display-queue-evidence/`; native
characters use the existing original pose resource packs. Private validation
source is `artifacts/part-display-source/`, with copied RomFS from the prior
audience-aux snapshot. To match its committed baseline, original Bero queries,
mutable door data and Trouble Center events were regenerated there only from
`rel/gor.rel`; field-map flag catalogs were regenerated with the baseline map
converter from existing read-only original `gor_00`..`gor_04` map data. No root
asset cache or user emulator profile was changed. Source/converted copyright
remains Nintendo/Intelligent Systems; local use only, no redistribution rights.
No original payload or compiled pack belongs in source history.

### 2026-10-14 — Original default Bero exit event

The read-only US DOL conversion now retains default_outevt80416074/20 alongside
original default_entevt80416060/20. PMBRIO02 adds the original exit pointer to
the descriptor: bero-info.bin148 bytes; bero-info.pmevt1848 bytes. Existing15
REL entries, provenance and relocation remain unchanged. No new art/audio or
executable PPC is imported. Original Nintendo/Intelligent Systems rights apply;
conversion grants no distribution rights. ISO/extracted/compiled resources stay
ignored and outside history. [Native scope](behavior/field/NATIVE_BERO_REGISTRATION_AND_COLLISION_ROLLBACK.md).

### 2026-10-14 — Retained case installer source use

No asset conversion or pack format changes. Case services use original recovered
US DOL logic; Trouble installer/board/counter use the existing205-object,
5464-byte original REL event closure with unchanged US message IDs/tokens. Source
listings remain ignored under artifacts/bero-case-*. Native contiguous case
records are storage adaptation, not extracted executable PPC. Existing original
Nintendo/Intelligent Systems rights apply; no redistribution rights are granted.
ISO/extracted packs and compiled native artifacts stay outside history.


### 2026-10-14 — Inner pose cache owners and private validation packs

No character art/audio was newly imported. PMANIM3/4 source position/normal
arrays remain complete unfiltered Df0/Df8 output buffers, now used for native
cache flushes. Source work/map/flush evidence is private under
`artifacts/battle-display-queue-evidence/` (documented in the inner prefix
contract). Validation freezes source at `artifacts/pose-inner-source/`; it copies
the previous part-display RomFS and refreshes original Bero data with that
baseline's converter. The live Bero cache was also regenerated from the read-only
ISO using the live converter during validation setup; no generated data is
committed. Existing Nintendo/Intelligent Systems copyright/local-use-only
restrictions apply; no redistribution rights are granted.

### 2026-10-14 — Original ROAD leaf admission for placement

No pack format, image, audio or original payload change. Existing Bero15-entry
catalog remains1848 bytes plus148-byte descriptor; existing ROADv1 geometry
and original node IDs remain unchanged. tools/bero_info_assets.py now rejects
kind0 entries whose matching original collision owner is missing, ambiguous,
meshless or has children. All8 current ROAD markers are verified transformed
leaves; the read-only raw-map check and center comparison are recorded in
ignored artifacts/outer-bank-road-leaf-centers.json. Future hierarchy owners
require original leaf-center traversal, not an aggregate bbox substitution.
Source/converted Nintendo/Intelligent Systems rights apply; no distribution
rights are granted. ISO/extracted resources and compiled binary remain ignored.
[Placement scope](behavior/field/NATIVE_BERO_POSITION_AND_START_PUBLICATION.md).

### 2026-10-14 — Ordered actor-position and camera callback source use

No original payload, pack format, image or audio conversion changes. Native
services implement recovered US DOL callbacks800e7180,800eb558,800a785c and the
semantic camera-road/shift reset owners. Read-only source listings remain ignored
under artifacts/bero-local-evt_* and bero-camera-camRoadReset.txt. Characters,
geometry, messages and audio retain existing original resource packs. Original
Nintendo/Intelligent Systems rights apply; no redistribution rights are granted.
ISO/extracted/compiled resources remain outside source history.


### 2026-10-14 — Unchanged ordinary MODEL draw companions

Original read-only ISO data resources `a/b_mario`, `a/c_pkr` and `a/c_kuri_h` are
retained unchanged beside their converted pose directories as
`trio-attack-mario.draw-model.bin`, `trio-attack-goombella.draw-model.bin` and
`professor.draw-model.bin`. Validation outputs are private under
`artifacts/draw-traversal-source/romfs/`; local original input paths remain
`local-assets/trio-battle/b_mario`, `local-assets/trio-battle/c_pkr` and
`local-assets/professor/c_kuri_h`. No art/audio is redrawn or newly synthesized.
The standard pose-directory writer emits these companions on future asset builds;
Professor's required-product list now checks its companion. They preserve full
file-relative group/shape/draw/fan/index tables dropped by the animation subset.
Payload sizes are285640/120932/24372 bytes. SHA256 respectively:
`1f29b8e93280bcccf6a488c99a5f4addc35d5604e80d9afcbf5efcec64df51e4`,
`d712df83d7b70b64a90d45551b39ecf7adf277b9572f6931daae7792b52d69e7`,
`5ca1795ce8a9729e661da14742178fe7264674861a4e7101506238cc0ba46247`.
Nintendo/Intelligent Systems copyrighted resources, local use only; no
redistribution rights granted. No payload/compiled pack/ISO is committed.
Private RomFS was copied from the prior pose-inner snapshot; Bero data was
refreshed only there using its frozen baseline converter. User profiles and
source-root asset caches were not modified by validation in this change.

## Audience member control source inspection (2026-10-14)

Read `/home/carson/games/roms/G8ME01.iso` read-only with
`tools/inspect_dol_function.py`; ignored `artifacts/BattleAudienceCtrlProcessKinopio*.elf/.txt`
contain inspection-only source bytes/disassembly. No original executable code
is linked or committed. The portable source consumes existing unchanged
`romfs/trio-attack-events.pmevt` data: dispatch table80397b20 and motion/reaction
constants804244e0..804245d0. No pack format, texture, HUD, scaling or licensing
choice changed. Function/data source pins and scope are in
[the control contract](behavior/battle/AUDIENCE_MEMBER_CONTROL_AND_IDLE_REACTIONS.md).
Original Nintendo game material remains private under the existing ISO asset
licensing policy; extracted/compiled material stays outside source history.

## Audience idle decision source inspection (2026-10-14)

Read-only US ISO DOL Wait80192260/7ac, offset-seat801a2768/80 and slot
BreakTurn80203370/24 were inspected with `tools/inspect_dol_function.py`.
Ignored inspection files stay under `artifacts/`. Portable source reuses existing
unchanged PMEVT type table80397af8 and SDATA2 float constants804244e8..804245cc;
no source PPC text is linked or committed. No new pack format, art conversion,
HUD asset, scale, or licensing choice. See
[the source contract](behavior/battle/AUDIENCE_IDLE_BREATHING_AND_DECISIONS.md).
Private validation also regenerated existing standard pose-directory/model
companions with the committed extractor for the current source baseline; these
unchanged original model bytes retain the existing private-use licensing and
stay outside source history.

### 2026-10-14 — Original initial-position EVT words

Read-only `/home/carson/games/roms/G8ME01.iso` US DOL8030f060 supplies the
unchanged632-byte evt_bero_initial_pos plus its actual adjacent4-byte scanner
prefetch context at8030f2d8. Converter `tools/bero_info_assets.py` pins script
SHA256214cdff31534df948655149d8ba53441124be40ce6b980d353dee0b48952c519.
`bero-info.bin` remains148 bytes; `bero-info.pmevt` becomes2496 bytes. Mutable
Bero globals are separate guest RAM, not immutable asset data. Conversion and
validation outputs stay under ignored `artifacts/source-checks/trouble-world/`.
No texture, geometry, audio or dialogue substitution is introduced. Original
Nintendo/Intelligent Systems rights apply; no redistribution rights are granted.
ISO, original extracted resources and compiled packs/binaries are not committed.
[Native execution scope](behavior/field/NATIVE_RETAINED_BERO_INITIAL_POSITION.md).

### 2026-10-14 — Original indexed shape renderer

The shape continuation consumes the unchanged original MODEL companions recorded
above, including separate normal/color/UV index streams, shape bases and fan
counts. No new image/payload is imported or committed. The explicit test fixture
alters only an in-memory local model copy to exercise two UV selectors and zero
counts; it is not original evidence and is never shipped. The private validation
pack refreshes baseline Bero records with its own extraction directory. An initial
wrong-working-directory setup assertion was followed by a converter invocation
that re-extracted identical original gor.rel into the existing ignored cache,
then stopped at missing reference metadata; no root pack/source/saves changed.
The retry owns a separate cache and copies reference inputs read-only.
The copied pack also contained door-data, whose paired private REL extraction
was absent; the baseline door converter supplied it in the isolated tree. All
pack hashes stayed unchanged. Both private extractors retain original ISO bytes.

## Audience cheering/jump/booing source inspection (2026-10-14)

Read-only US ISO DOL JoyRaiseArm80191e18/108, JoyJump80191cec/12c,
JoyJump2 at80191bb0/13c and Booing80191884/fc were inspected using
`tools/inspect_dol_function.py`. Inspection ELF/text remains ignored in
`artifacts/`. Portable code consumes unchanged existing PMEVT constants,
including32767 at80424554 and20 at8042453c. Existing interpolation helper/source
pins are reused; no original executable code is linked or committed. No new
asset format, HUD art, texture/geometry conversion, scale or licensing choice.
[Control contract](behavior/battle/AUDIENCE_CHEERING_JUMPS_AND_BOOING.md).
Private validation refreshes the original Bero pack with its committed baseline
extractor; all extracted/compiled material retains its private-use licensing and
stays outside source history. ISO access remains read-only.

### 2026-10-14 — Original door installer closure

Read-only US ISO rel/gor.rel supplies namespace-qualified gor_02.o door02,
door03,door03_roll entry scripts/records and door04 record, retaining actual
ADDR32 imports and script-label prefetch context. The expanded door-data.pmevt
contains183 original objects/14304 bytes. Unchanged PMDRDT01 remains36 bytes;
new field-area-events.bin PMFAEV02 is60 bytes of area/script/record metadata.
US DOL8033156c supplies the unchanged308-byte generic door entry, SHA256
6269747e7ff14d9effbd84761663f25e880e3ed3c191c842e974db0559e78281,
plus actual4-byte adjacent scanner context at803316a0. Original executable PPC
is not linked. No geometry, image, music or dialogue replacement is introduced.
Outputs remain ignored under artifacts/source-checks/trouble-world. Original
Nintendo/Intelligent Systems rights apply; no redistribution rights are granted.
No ISO/extracted payload/compiled pack is committed. [Execution scope](behavior/field/NATIVE_SHARED_FIELD_DOOR_INSTALLERS.md).

### 2026-10-14 — Original battle material-light scene bytes

`tools/material_light_assets.py` reads the ISO read-only and emits ignored
romfs/material-light.bin(PMLTSC01,64 bytes) plus provenance JSON. It retains
unchanged zero light-count words and20-byte fog records from m/bti_01/d and
m/stg_00_1/d, with exact map SHA256 pins inherited from battle_hit_assets.py.
The final8 bytes are original DOL8041f90c/8, two707070ff fallback ambient words.
Original chunk offsets/hashes and ambient-span SHA256 are recorded in the JSON.
32 zero96-byte native light records are replayed allocation/init state, never
imported BSS. The pack supports only the actual two admitted no-authored-light
maps and rejects a different revision/count rather than dropping light entries.
No ISO, extracted payload or compiled pack is committed. Original Nintendo/
Intelligent Systems licensing remains local-use-only, with no redistribution
rights granted. Normal asset builds invoke the importer after battle-hit assets.
Validation uses its own material-light cache and private RomFS; source maps and
constants are unchanged. No new image conversion or external code was used.

## Audience holding/throwing/damage/escape source inspection (2026-10-14)

Read-only US ISO DOL ItemOn80191a48/168, ItemThrow80191980/c8,
Damage8019173c/148, Escape80191290/1e0, ItemGetPtr801a8624/40,
GetEscapeChangeOK801a27e8/fc, GetSysCtrl801a2960/60 and
SoundItemThrow801a044c/f0 were inspected with `tools/inspect_dol_function.py`.
Ignored inspection ELF/text stays under `artifacts/`. Source pins and exact
native scope are in the
[control contract](behavior/battle/AUDIENCE_ITEM_HOLD_THROW_DAMAGE_AND_ESCAPE.md).
Code uses unchanged existing PMEVT constants and original sound-key addresses;
no executable source text is linked or committed. No new art/HUD conversion,
asset format, scale or licensing choice was introduced. Native sound requests
reuse existing original public SFX metadata; real voice playback remains open.
Private validation refreshes original door/Bero/Trouble/material-light packs
using the committed baseline's extractors, with the ISO read-only. Generated
material retains the existing private-use licensing and stays outside history.

### Original area parent and immutable DOL EVT closure (2026-10-14)

Read-only G8ME01 disc0/rev0 gor.rel source gor_02_init_evt (2172 bytes,
SHA256649d12889da97f2b7b29aa59f59f58671dfb916195160127f594df927e84109e)
now roots the private door event bank. The converter also reads original main.dol
section-backed EVT bodies reachable through known EVT address references:
61 objects/23436 body bytes, unchanged with real adjacent scanner words.
The bank includes970 relocated original REL data objects and totals100520 bytes.
New field-area-init.bin uses PMFAIN01, room2/root,20 bytes. Existing door-data.bin
and field-area-events.bin formats remain36/60 bytes. This is finite EVT closure,
not all native callbacks/constants/resources or mutable GP state.

No art/audio was regenerated. The next source prefetch needs original a/p_dokan_x
(41756 bytes), a/p_dokan_y (18576 bytes) and shared texture alias a/p_slit-
(22592 bytes); this phase does not bind or claim readiness for those resources.
Exact source offsets/hashes and ordering are in the Bero prefix contract.
ISO, extracted bytes, compiled packs, fixture profiles and probe binaries stay
ignored outside history. Nintendo/Intelligent Systems assets retain original
licensing; this conversion grants no distribution rights. Private validation
identities are in artifacts/area-parent-validation-manifest.json.

### 2026-10-14 — Unchanged original animation TPL companions

The standard pose-directory writer accepts the matching original texture source
and emits .draw-textures.bin beside the unchanged MODEL companion. Trio and
Professor import/cache paths now require/produce these private files:

| Private companion | Original ISO source | Bytes | SHA256 |
| --- | --- | ---: | --- |
| trio-attack-mario.draw-textures.bin | a/a_mario- | 731296 | 2ccf4f212340bb9cce33cd8457ac92a24ca3fc0db4fc95237224775e9534845e |
| trio-attack-goombella.draw-textures.bin | a/c_pkr- | 90432 | 162b5151601b129fc667f51b90ac6124de863ab2dff79b997cd143ed4d5c4b88 |
| professor.draw-textures.bin | a/c_kuri_h- | 24896 | c4114e4c5991d0cc19fd9c5b97849369cc5eb361b7e723d3f2ee1b69135e12ef |

Bytes are unchanged; native CPU retains only validated header projections after
import, while resident existing converted atlases supply GPU pixels. No image
was regenerated for this change. Existing scaling/border/cell conversions remain
in force: Mario512x512(scale1),Goombella512x256(scale2),Frankly1024x256(scale4).
Specific package palettes, not original texture names alone, identify cells.
The ISO stays read-only and all companions/packs/extracts remain ignored. Original
Nintendo/Intelligent Systems licensing grants no redistribution rights. Tests use
an in-memory altered model/CI provider only; no synthetic image is shipped.
Private validation emits the three companions from existing read-only extracts
and refreshes baseline Bero/door data only in owned private cache directories.

## Audience item creation and lifecycle source inspection (2026-10-14)

Read-only US ISO DOL BattleAudienceItemOn801a5aac/1fc,
ItemCtrlProcess801a4f0c/958, DetectTargetPlayer801a5864/d0 and
pouchGetEmptyHaveItemCnt800d5614/60 were inspected with
`tools/inspect_dol_function.py`, together with their source getters/bit/status
leaves. Inspection ELF/text stays ignored under `artifacts/`. The portable code
uses unchanged PMEVT species weights, dispatch8039cd34, SDATA2 literals and the
original four c0-byte harmful-item descriptors at8039c880 (SHA256
b10aa7c23a8d3d4e8aeab030ea0771400131d1a10e17ef319bd86c949ad711e1).
[Source contract](behavior/battle/AUDIENCE_ITEM_CREATION_SELECTION_AND_LIFECYCLE.md).
No source PPC text, ISO, generated pack or extracted asset is committed. No new
art/HUD conversion, asset format, scaling or licensing choice. Validation reuses
the prior isolated original packs; all generated material remains private under
the existing asset provenance policy. ISO access remains read-only.

### Original animation prefetch source payloads (2026-10-14)

The door-data pipeline now invokes tools/animation_prefetch_assets.py. It reads
only the supplied ISO and emits unchanged private romfs/field-prefetch/a files:
a/ag2tg.bin41984 bytes, a/p_dokan_x41756, a/p_dokan_y18576, a/p_slit-22592.
The fixed SHA256/FST provenance is recorded in the Bero prefix contract and
romfs/field-prefetch/sources.json. Both groups resolve the actual shared p_slit
alias from original table rows; no authored alias table or pixels are substituted.
Native cooperative prefetch retains actual raw bytes; it performs no decode,
pose registration, texture conversion or GPU upload. Payload residency82924 bytes
plus table41984. ROMFS staging, source extracts, binaries and probe profiles stay
ignored. Original Nintendo/Intelligent Systems assets remain locally licensed
with no redistribution grant. No original asset bytes are committed.

### Mario preparation constants and unchanged field assets (2026-10-14)

Mario key/background callbacks now use the original source flag/byte operations
and exact binary32 dimension/movement constants documented in the Bero prefix
contract (Mario sdata2 addresses80420004/08/78/88/b8/bc/c0/c4). No graphics,
animations, music, dialogue, ISO bytes or packs were changed by this phase.
Existing read-only original groups/texture and event packs are reused. Conversion
validation refreshed only ignored private door/Bero-prefetch outputs. The native
key adapter uses existing Game state; it does not create a substitute actor asset.
Original Nintendo/Intelligent Systems licensing and local-only handling remain.

### Original audience matrix-icon service recovery (2026-10-14)

Read-only US ISO DOL bodies iconDispGxCol8001a4bc/224 and
iconDispGx2 8001a6e0/238 now supply portable retained matrix-icon services.
[Source spans, hashes and original globals](behavior/display/ORIGINAL_MATRIX_ICON_SERVICES.md)
record descriptor/frame/GP ownership and the exact original white RGBA constant.
No asset extraction, conversion, resampling, atlas changes or new generated art
were needed. Original icon.bin/icon.tpl resources and their native loaded-buffer
owners remain required; this service does not synthesize replacements or claim
new native icon emission. Original Nintendo/Intelligent Systems resources retain
their existing licensing and private ignored-asset restrictions.

### Original audience iconGX recovery (2026-10-14)

Whole local iconGX800197f8/9a4 is reconstructed from the read-only US ISO.
[Source hash, original numeric/color constants and texture/material requests](behavior/display/ORIGINAL_ICON_GX_DRAW_SERVICE.md)
record provenance. No original resources were converted, rescaled or committed.
Actual icon.bin/icon.tpl loaded ownership, original dummy image80415080 and
selected texture objects remain required native inputs. Opaque resource tokens
are adapter identities, not synthetic textures or ARM casts of source pointers.
Original Nintendo/Intelligent Systems resources retain existing private-pack
licensing restrictions. Portable geometry does not establish native pixels.

### Ordinary Mario body integration (2026-10-14)

No original asset payload or conversion changed. Native body defaults use source
MarioInit20/37 dimensions and1/2.25 base speeds, with form0 from source memset.
Unit movement multiplier1 is source marioResetGravity8009696c's float80420a74
store to Mario+228. Original event/prefetch/texture companions remain ignored,
locally reused and subject to the existing Nintendo/Intelligent Systems license.
No new geometry, raster art, audio or dialogue was authored.

### Original previous-party and attached position prefix (2026-10-14)

No original asset bytes or conversion changed. Source callback800e3058 and length
8007a76c use recovered original logic over existing Game storage and the retained
original gor02/DOL event catalogs. The attached initial_pos runs from unchanged
private event words. No synthetic partner, pose, dialogue or sound was generated.
Existing original event/prefetch/texture packs remain private and retain their
Nintendo/Intelligent Systems licensing and local-only handling.

### Future native ROAD leaf admission (2026-10-14)

The native room converter now rejects selected road/pipe hit markers with
children, retaining the original static transformed-leaf center domain. It does
not export a drawable parent's bounding-box center as original hitObjGetPos.
No original asset payload, scaling, pack format, art, audio or dialogue changed;
existing original source leaf centers and licensing remain unchanged. Hierarchy
and runtime-transform support require future genuine providers. Original ISO,
extracted assets and generated packs stay private/ignored.

### Native complete original icon directory and cells (2026-10-14)

`tools/source_icon_assets.py` opens the US ISO read-only and keeps unchanged
icon.bin/icon.tpl in ignored romfs/source-icons. The complete572-entry directory
and613 original images feed real matrix-icon/native resource binding. Converted
previews stay in local-assets/source-icons; source cells retain original pixels
and sizes, with2px source-wrap gutters inside lossless power-of-two RGBA8 T3X
files. No replacement art or resampling. sources.json records every original/
converted hash and DOL constant spans.
[Paths, original hashes, conversion, native residency and limitations](behavior/display/NATIVE_ORIGINAL_ICON_RESOURCES.md).
Renderer imports cells outside submitted frames with101 slots for the full item bank/AP image and retains
actual bound texture ownership; that cache policy is an adaptation. Original
Nintendo/Intelligent Systems licensing remains inherited with no redistribution
rights. No raw ISO, extracted original bytes, previews, packs or compiled native
assets were committed. Native resource/binding proof is separate from raster
parity and playable audience items.

### 2026-10-14 — Frankly atlas residency for native battle finishing draws

Reused existing private professor assets for an independent battle texture owner:
read-only ISO a/c_kuri_h and a/c_kuri_h- -> local-assets/professor/c_kuri_h and
c_kuri_h.tpl -> romfs/professor.{pose-directory.bin,draw-model.bin,draw-textures.bin,
pmanim,t3x,bin,json}. No conversion or new extraction was performed in this step.
Original MODEL/TPL metadata and17 animation descriptors are unchanged. Existing
surface converter keeps15 image cells in a1024x256 RGBA8 atlas,124px images with2px
borders and UVscale4; the native texture consumes1MiB before descriptor overhead.
It uses a separate import during trio battle residency because the field room's
professor texture is retired on battle entry. Resource identity, cell mapping and
UV correction remain tied to the exact converted professor package, not a model
name match against another atlas. GPU work synchronizes before retirement.
Original Nintendo/Intelligent Systems assets remain private; no redistribution
rights are granted and no raw ISO/derived pack is committed.

### Shared Bero normal-position and collision provenance (2026-10-14)

Original event words, ROAD leaf centers and existing collision packs are reused
unchanged. Native adapters restore engine units from existing scaled coordinates,
publish mutable S/E runtime globals and retain complete collision room provenance.
No pack format, texture/geometry reduction, image, dialogue, music or sound changed.
Original Nintendo/Intelligent Systems assets retain existing private licensing;
ISO access stays read-only, and packs/extracts/binaries remain ignored.

### Original NPC setup and populated battle data research (2026-10-14)

No original asset bytes or conversion changed.8005323c uses unchanged original
setup rows and init/regular script addresses from the private retained gor02/DOL
closure. Read-only DOL/REL inspection recovered actual fifteen-row tribe/model
and populated battle0 dependencies; source paths/addresses are in the field
contracts. Inspection ELF/disassembly remains private. General battle-table data
closure is still incomplete and no empty substitute was exported. Synthetic
actor memory/battle/pose effects occur only in explicit validation fixtures;
Game receives no fabricated actor registration or resource-success provider.
Original Nintendo/Intelligent Systems data retain existing private licensing;
ISO/extracts/packs/probes/binaries stay ignored,with no distribution grant.

### Native icon material program binding (2026-10-14)

[Native original icon material programs](behavior/display/NATIVE_ICON_MATERIAL_PROGRAMS.md)
now consume the retained original icon cells through actual flat light, map
blend/fog-off and ordered PICA tint/grayscale/blend programs. Read-only source
review pins mapGetBlend8001d968 and __GXInitGX's identity swap table0; source
spans/hashes and original default white are in the contract. No new extraction,
conversion, resampling or generated artwork. The isolated native fixture uses
original icon108 (A button); its captured interior pixels match scalar RGBA
references within1 level. This is separate from original GX hardware parity or
normal scene composition. Existing original-resource licensing/private-pack
restrictions remain unchanged; no original bytes, probe main or binary committed.

NPC battle numeric setup phase: no new extracted assets, conversion or pack payload. Reused private original harbor-tutorial-events.pmevt with its95 guest address domain for setup2 verification; shared area packs retain90. Corrected source C+18→information+4 mapping (harbor value20000010). The reusable registry code does not fabricate initialized area rows or import immutable DOL data as mutable RAM. Original Nintendo/Intelligent Systems asset licensing/local-use restrictions remain unchanged.

Original gor setup0 field-reader export (2026-10-14): tools/field_battle_setup_assets.py is called by tools/door_data_assets.py. Reads rel/gor.rel from read-only /home/carson/games/roms/G8ME01.iso; extract remains ignored at local-assets/door-data/gor.rel. Exact REL SHA256 d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a. Explicit original views: REL60e30/44,60da0/18,60d80/20,60d50/30,61c40/4,1dbc8/10,1dbd8/14; original DOL80417880/8. Hex offsets/sizes;220 decimal payload bytes. Native section90 ADDR32 relocation only; no texture/audio/geometry change or executed PPC. Numeric index0 only; stage/auxiliary target identities preserved without claiming full battle resources.

Generated ignored romfs/door-data.pmevt is100800 bytes; romfs/door-data.json field_battle_setup records source/relocated hashes, owner symbols, relocations and domain. Only these two outputs changed; door-data.bin, field-area-init.bin, field-area-events.bin and existing field-prefetch outputs retained exact bytes. Validation copies live under artifacts/source-checks/trouble-world, with full identities in artifacts/area-battle-data-validation-manifest.json. Original Nintendo/Intelligent Systems resources remain local-use only; no redistribution rights or ISO/raw asset commit. [Behavior scope](behavior/field/BOUNDED_ORIGINAL_GOR_SETUP_ZERO_DATA.md).

## Retained icon scene replay (2026-10-14)

Reuses the private original `icon.bin`/`icon.tpl`/constant and lossless padded T3X bank documented above. No icon extraction/conversion/image or licensing change. The isolated validation tree restored its missing `professor.draw-textures.bin` as a byte-identical copy of the existing original `c_kuri_h.tpl`; this previously documented sidecar is included in the native pack. It also refreshed `door-data.pmevt` and associated metadata using the committed read-only ISO converter after integrating the newer original field setup changes. Those are existing original field dependencies, not new icon art. Native draw packets copy original XYZ/ST/material/raster/texture identities; source indices pin both displayed and building GPU working sets (202 on-demand slots). Original assets remain outside source history and retain their original rights restrictions. The lower scene adapts the existing original view into the audience continuation; no replacement icon art is introduced. See [retained replay contract](behavior/display/RETAINED_ICON_SCENE_REPLAY.md).

Original VM battle-registry phase (2026-10-14): tools/field_battle_registry_assets.py reads read-only US G8ME01 revision0 ISO DOL spans80305320/5cc,80415b30/270,8041fa60/a4 (hex;2272 decimal bytes), pinning all three exact source hashes. It exports all33 original record/name pointers in source order, with genuinely zero setup/number words and original terminator. rel/gor.rel remains the ignored local-assets/door-data/gor.rel extract, exact prior SHA256; bounded REL views add module name13d60/4, number table6100c/38 and strings1dc80..1dce8. Original ADDR32 relocation only; no PPC execution or initialized registry pointer fabricated in source data.

Generated ignored romfs/door-data.pmevt is103308 bytes and door-data.json records field_battle_registry provenance and prolog fingerprint. New romfs/field-area-battle.bin (PMFABT01) is a36-byte native room/profile descriptor retaining original prolog name/setup/number identities and explicit supported numeric0 bounds. Existing door-data.bin, field-area-init.bin, field-area-events.bin and field-prefetch hashes remain unchanged. Native EventAddressSpace copies source pack bytes into VM-owned RAM and the registration helper mutates that copy. Original Nintendo/Intelligent Systems resources remain local-use only; no redistribution rights or raw asset/ISO commit. Full validation hashes artifacts/area-battle-registry-validation-manifest.json. [Behavior/lifetime limits](behavior/field/NATIVE_VM_BATTLE_MODULE_REGISTRATION.md).

## Native unused sampler residency (2026-10-14)

Reuses the already required original `font.t3x` for native texture units1/2 when their TEV program does not sample them. No image/conversion/licensing change, substitute art or new dummy texture. The original item-get pattern/opacity and message-window masks still occupy their actual sampled units. The resident font outlives the transient banks. See the [native sampler contract](behavior/display/NATIVE_UNUSED_SAMPLER_RESIDENCY.md).

Packed NPC constructor phase (2026-10-14): no new extracted assets, conversion or pack payload. Source evidence reads original US DOL functions/constants from read-only /home/carson/games/roms/G8ME01.iso; complete fresh disassemblies remain ignored under artifacts/npc-registration-*.txt. Existing private shared area/event/prefetch/texture packs from the original VM registry phase are reused unchanged. Constructor consumes original-address vectors802c11f8/802c1204, float/word constants8041fc94/8041fc7c/8041fc6c and strings8041fd6c/8041fd74/802c1468 through caller-owned services; real Game data/resource admission remains pending, so fixture constants are not exported as invented game assets.

Read-only pose evidence identifies real MODEL+c8/+cc dimension words and Vivian model identity mapping; actual original field banks and PMPOSE/draw sidecars still need conversion/admission. [Required resources](behavior/field/ORIGINAL_FIELD_NPC_POSE_LEAF_SERVICES.md). No art/audio/dialogue edits or raw asset/ISO commit. Original Nintendo/Intelligent Systems local-use/no-redistribution licensing remains unchanged.


Stock EXP positional-icon constants (2026-10-14): tools/source_icon_assets.py
read_constants opens /home/carson/games/roms/G8ME01.iso read-only, US disc0/rev0.
Original DOL8041f860/4 (ffffff00 RGBA) and8041f8a0/4 (3c8efa35 radians/degree)
append to ignored romfs/source-icons/constants.bin,116→124 bytes. The original
116-byte prefix and all613 icon images/cells remain byte-identical: no resizing,
resampling, new art or audio. romfs/source-icons/sources.json records the new
spans and constant hash; original icon.bin/icon.tpl and all lossless .t3x cells
are reused unchanged. Complete builder uses the same read_constants function;
validation updates only constants/metadata without reconverting unchanged cells.
Existing original Nintendo/Intelligent Systems local-use/no-redistribution
restrictions apply. No ISO/raw asset/generated pack or compiled binary committed.
Original stock icon ID404 and actual camera/frame/GX providers are used; visual
parity and whole-scene publication are separate from resource admission. Frozen
source's committed door-data builder was also run to match its existing area
registry schema; that is a prerequisite pack refresh, not a new Stock EXP asset.

### Original NPC constructor constants and field CPU pose bridge

Read-only G8ME01.iso DOL export tools/field_npc_constructor_assets.py adds five
unchanged spans:802c11f8/18,802c1468/8,8041fc6c/2c,8041fd6c/10,8041fb64/4.
These96 payload bytes retain zero/unit vectors, c_luigi, source scalar/gap bytes,
hoshi/M_I_2 and float2; each span is SHA256 pinned. The shared private
romfs/door-data.pmevt grows103308→103464bytes including segment headers;
romfs/door-data.json records field_npc_constructor.dol_views and source hashes.
Only those two pack/metadata files changed; door-data.bin, field-area-init/events/
battle.bin and field-prefetch products remain byte-identical. No synthetic BSS
registry or pose handle is packaged.

No new art conversion this step. Real field bridge uses existing original
professor.pose-directory.bin/.pmanim/.draw-model.bin/.draw-textures.bin from
a/c_kuri_h and a/c_kuri_h-. Raw MODEL/TPL remain unchanged, full descriptor
identity/order retained, descriptor0 Z_1 evaluated on CPU; signed metadata is
read from original+c8/+cc rather than inferred bounds. Synthetic metadata
rounding variants exist only in explicitly test-owned fixtures, never packs.

Nintendo/Intelligent Systems copyrighted game data is retained privately for
local development; no redistribution license is asserted. Extracts/compiled
packs/binaries remain outside source history. ISO opened rb and not modified.
Paths/hashes in artifacts/area-field-pose-assets-after.json and validation
manifest. Missing six gor02 banks/hoshi and complete field-facing/GPU owners
remain unavailable; this bridge does not claim rendered NPC registration.

## Audience target/status data admission (2026-10-14)

Reuses the existing private original `harbor-tutorial-events.pmevt` without conversion changes. Read-only ISO comparison confirmed exact vectors802fa104/0x30, zero80424988/4 and query Z offset804249cc/4 in that pack. Addresses identify original source data, not native pointers. No new art, dialogue or original bytes enter source history; existing original licensing restrictions apply. Checks are recorded in `artifacts/audience-interrupt-original-data.json`.

### Original tagged-map NPC resource banks

New tools/field_npc_resource_assets.py exports original dependencies from
m/AREA/s and first-match npcTribe803316d0 in read-only US G8ME01 revision0 DOL.
Default m/gor_02/s has15 actors/12 model banks. Original a/hoshi fallback is
included independently. Models: c_deale,c_kuri_h,c_joho_b,c_monban,c_binbo,
c_bomt_n,c_kuribo_n,c_tyusan,c_roten_g,c_borodo_n,c_f_mura,c_luigi,hoshi.
All173 descriptors retain exact original names/order and actual animation data;
no lowercase/uppercase alias or generated pose replaces a source request.

Each private romfs/field-native-MODEL publishes .pmanim (all source CPU clips),
.pose-directory.bin (PMPOSE02 complete names/discrete/model flags), unchanged
.draw-model.bin and .draw-textures.bin, plus .json content/source provenance.
romfs/field-npc-resources.json maps selected original areas/actors/tribes to
shared resource names and original descriptor rows/roles/hashes. New areas can
use repeated --area; converter domains must actually admit their models.
local-assets/field-native/ retains original model/TPL/map extractions and atlas
PNG evidence. Exact a/MODEL and a/TEXTURE- paths/hashes are in per-bank assets;
TPL name is taken from the actual model header rather than assumed.

66 new ignored products3813960bytes (CPU2845047, raw MODEL/TPL843632).
Raw companions are byte-identical to original extracts. Existing finite field
presentation packs are unchanged. Native CPU library remains the adapter for
sparse geometry/hierarchy/color/sampler conversion; atlas gutters/resizing are
inherited conversion choices, not original GPU publication. No new T3X or
Game actor visibility registration is claimed. Full CPU/raw banks coexist
with existing packs pending migration to one retained actor resource owner.

Conversion fingerprint includes original MODEL/TPL and helper sources; required
products and private atlas evidence are SHA256 checked. Verified missing-atlas
recovery reproduces identical image and all66 production products. Products/
asset hashes: artifacts/area-field-resources-products.json and validation
manifest. ISO opened rb and not modified. Nintendo/Intelligent Systems original
copyrighted resources remain private; no redistribution permission asserted.
No raw assets, extracted source or compiled packs are committed.

### Original reel display constants

The private source-icons constants pack grows124 to188 bytes with original DOL spans80426884+28 and80419f30+18 (hex). Existing124-byte prefix and613 image cells are unchanged. Added RGBA/f32 values and small65,67,69,68,66 / big70,72,74,73,71 U16 tables support original802036d4. These indices refer to battle_common.tpl; no substitute texture is exported. Read-only source `/home/carson/games/roms/G8ME01.iso`; Nintendo/Intelligent Systems ownership, local use only and no redistribution rights. Generated constants.bin/sources.json remain ignored.

## Opening-renderer documentation validation cache (2026-10-14)

Private validation source: artifacts/scene-code-documentation-validation/source.
Its initial RomFS copy lacked newer original model companions/event payloads.
Refreshed 109 outputs from artifacts/source-checks/trouble-world/romfs, whose
previous validation is recorded in artifacts/area-field-pose-validation-manifest.json;
its matching local-assets/message-icons cache accompanies those textures. This
copies prior conversion output; it does not import over checked-in editable assets.
Per-output SHA256 values and the prior cache path are in the scene validation
manifest's asset_refresh.copied_outputs.

The frozen tools/source_icon_assets.py read_constants helper reread five admitted
DOL spans from /home/carson/games/roms/G8ME01.iso using read-only access. Private
romfs/source-icons/constants.bin is 124 bytes, SHA256
c5ec5344a42fd999c7113e153ecc422bfb8c46078a000ce4bc4cf15b9cec5aa1.
The concurrently edited root cache had extra spans incompatible with this frozen
loader; the private constants record and sources.json were updated together.
Frozen tools/material_light_assets.py also extracted original m/bti_01/d and
m/stg_00_1/d to private local-assets/material-light/*.d, retaining original bytes
and DOL ambient data in private romfs/material-light.bin (64 bytes) and its JSON
provenance. No image resizing, art/music/dialogue replacement or shared cache write
was performed. Native validation reused this private RomFS with assets-ready
regeneration skipped; it did not re-import assets/.

Original Nintendo/Intelligent Systems rights remain reserved. These original and
derived files stay in ignored local caches; recording paths/hashes grants no new
license or redistribution rights. No ISO content was staged or committed.

## Present-item data admission and validation dependencies (2026-10-14)

The existing private harbor event pack already has original conversion bias802efe78/8 and5.0 at804225f8/4, both checked against the read-only ISO. No present-item image, audio or dialogue conversion changed. Validation refreshed the already committed source-icon constant spans from116 to124 bytes using `read_constants`, retaining the identical613 image cells/catalog, and refreshed door/NPC-constructor data with its committed converter. These are current baseline dependency repairs, not new gift art. Paths/identities are recorded under `artifacts/present-item-*`; original rights restrictions and the no-distribution policy remain unchanged.

### Retained Game NPC registry resource use

The Game-owned cold registry/binding reuses ignored door-data.pmevt original
constructor constants/name views and existing field-native-c_kuri_h PMPOSE02,
PMANIM4, unchanged MODEL/TPL for strict host/ARM construction checks. Fixture
model bytes are copied from the unchanged original MODEL header into explicitly
fixture-owned guest RAM; this is not packaged source BSS or an invented pose
handle. The production owner requires actual readable name/model/constants
and installed field resources before entry. All13 original map/fallback banks
remain available for later resource composition; no eager loading is added.

No asset conversion or pack changes in this step:85 private product hashes
match previous resource/source manifests (artifacts/area-npc-owner-assets.json).
Original ISO remains read-only, extracted/compiled data outside history, original
Nintendo/Intelligent Systems data retained privately with no redistribution
rights asserted. New source registry BSS zero comes from verified original
startup clear, not extracted synthetic memory. Native allocated buffers/guest
IDs are platform ownership adapters, not original allocator content/address
claims. No authored art/audio/dialogue or GPU/raster readiness claim.

## Render-adapter documentation validation constants (2026-10-14)

Private cache: artifacts/render-adapter-code-documentation-validation/source/romfs.
The base RomFS and matching message-icons/material-light caches were copied from
prior accepted artifacts/scene-code-documentation-validation/source; no new
editable-asset import or image/audio conversion was performed.

Frozen code 513661e0ad7afbfd3f7860627ba5d233a02cc25b expects a newer original icon
constant domain than the previous documentation baseline. Its own frozen
tools/source_icon_assets.py read_constants helper read the supplied
/home/carson/games/roms/G8ME01.iso read-only and produced private
romfs/source-icons/constants.bin: 188 bytes, SHA256
b2126d655f8bebc5abcca2fda33ec490267827f5dccfccd40aa2a410b3dbf5f2.
The seven DOL spans are (0x80415040,24),(0x80415080,32),(0x8041f864,60),
(0x8041f860,4),(0x8041f8a0,4),(0x80426884,40),(0x80419f30,24).
Private sources.json sha256/constant_spans were updated together. Thirteen matching original atlas evidence images were also copied into private
local-assets/field-native from artifacts/source-checks/trouble-world. Each image
was checked against its existing atlas_evidence SHA256 before use;the per-image
paths/hashes are in the validation manifest. The 613 cached
original image cells remain unchanged; native validation reuses this cache with
assets-ready regeneration skipped. Initial fixture failures and final full test/
build evidence are retained in the render-adapter validation manifest/logs.

Original Nintendo/Intelligent Systems rights remain reserved. All original and
derived pack bytes stay in ignored local caches; path/hash tracking grants no
new license or redistribution rights. Shared packs and assets/ edits were not
overwritten, and no ISO content was committed.

### Outer update/input owner implementation

No new asset conversion. Validation uses the existing private original packs matching basea7d17d20, including prior field-bank exports and188-byte source icon constants. Source ISO remains read-only; original payloads stay ignored and retain the licensing/provenance of their existing ledger entries. Native outer/Main and input GP aliases add no replacement art/audio.

## Retained display-state documentation validation cache (2026-10-14)

artifacts/display-state-code-documentation-validation/source reuses a private
copy of the accepted render-adapter-code-documentation-validation/source/romfs
plus matching message-icons, material-light and field-native evidence caches.
No new extraction/conversion or editable-asset import occurred. Cache provenance,
including original 188-byte icon constants and atlas evidence hashes, remains in
the prior validation manifest and entries above; the new manifest records the
copied cache dependency. Other large local-assets dependencies are shared read
inputs. Original Nintendo/Intelligent Systems rights remain reserved; ignored
local copies grant no new license or redistribution rights. Source assets and
shared RomFS were not overwritten; no original pack/ISO content was committed.

### Original tagged-map registration inputs and DOL views

New tools/field_map_npc_assets.py reads original m/AREA/s, preserving tagged
order, duplicate names, exact Shift-JIS fields and parsed float32 words. Default
m/gor_02/s supplies15 rows; romfs/field-npc-map-gor_02.bin is5896bytes
(PMFMNPC1 header16 plus15*392). Each native adapter record holds ordinal,
17float32 words and five64-byte string fields. This is an adapted parsed-input
layout, not an original source allocation or fabricated stack snapshot. Original
source path/hash and adapter SHA are in its .json; extracts remain ignored under
local-assets/field-map-npc/. Runtime converts numeric lanes LE→BE for the VM.
No missing tag field is invented. Resource census now uses this same ordered
parser; model sharing does not collapse actor/name rows.

The shared original door-data.pmevt103464→148048bytes now merges1169 aligned
DOL views (raw view sizes sum44276 including possible overlap): original
npcTribe803316d0/6ba0 SHA2569951ac6fcdfd82b738c99148210f79af3e54fc94f7bbcaa0effc8007f8569ca9,
npc_ai_type_table80338270/804 SHA256c01757d66166cab51c43b2d1620610d64cf96eedeefbd2bfd3aff3655cbde1ac,
territory80416580/20 SHA2562bcb3897e23a9041e954c7b17a19eb5bfcd25313368cda02edf30178f6d20b4b,
and actual table strings, BTLNO_INVALID802c3b10 and single0 at804207b4.
All views are unchanged original data; alignment includes real source neighbors,
never synthetic zeros. No PPC bodies, registry BSS or readiness results are
packaged. Per-view addresses/sizes/hashes in door-data.json.field_map_npc.
The original map AI address is80338270; prior80348270 prose was a transcription
error corrected in the source contracts.

All52 original field PMANIM/directory/raw MODEL/TPL products remain identical.
13 per-bank metadata fingerprints and resource-census metadata refresh because
the shared parser dependency changed; source/cell/clip data is unchanged. Other
original area descriptors/prefetch products unchanged.87 product hashes and
16 changed existing metadata/source-pack paths plus2 new parsed products are
recorded in artifacts/area-map-npc-assets-after.json. Original copyrighted
Nintendo/Intelligent Systems resources remain private with no redistribution
rights asserted; ISO opened rb and unchanged, extracts/packs/binaries outside
source history. No authored story dialogue, art/audio replacement or GPU/raster
publication claim is added.

### Recovery effect embedded resources and lifecycle

Read-only `/home/carson/games/roms/G8ME01.iso` (US G8ME01 revision0) feeds
`tools/recovery_effect_assets.py`. Private source JSON at
`artifacts/recovery-effect-import-v2/recovery-effect.json` retains original
integer values, addresses and per-span SHA256; compiling it reproduces
`recovery-effect.bin` byte-for-byte. Nine spans retain conversion bias802c1f18/8,
recovery vectors/SFX names802f3b20/170, keys80371fa8/6f4, dispatch8037269c/88,
colors804179d8/24, face804179fc/2a, descriptor graph80417a28/28, Maya constants
8042019c/1c and recovery constants80422db0/9c (hex extents).
PMREC001 adds only a little-endian bounded span directory; original payloads
remain big-endian with unchanged guest pointers. No texture resizing, geometry
reduction, lighting edits, new artwork or audio conversion occurred.

Private validation copy: `artifacts/battle-lens-qwwrj2n7/romfs/recovery-effect.bin`,
2678 bytes, SHA256 `553c3a347e1f88c4206fd3a26a0531c5a6ba673ca3bb63183846dd2ba1d7138f`.
`tools/verify_recovery_effect.py` pins12 whole recovery/Maya bodies and9 data
spans; this is static identity evidence, not effect admission/raster proof.
The empty-destination importer never rewrites editable sources; any future
tracked editable-source migration must follow assets/manifest provenance rules.
No extracted source/compiled pack is committed here. Nintendo/Intelligent Systems
own the original resources; local use only, no redistribution rights. Native
Maya curve evaluation, original textures18–21/108, sound playback, effect driver,
displays and terminal N64 stardust32 remain separately owned integration work.

## Draw-pipeline documentation validation cache (2026-10-14)

artifacts/draw-pipeline-code-documentation-validation/source starts from the
accepted display-state-code-documentation-validation/source RomFS and matching
message-icons/material-light/field-native evidence caches. It privately overlays
87 NPC products from artifacts/source-checks/trouble-world only after every
length/SHA256 matches artifacts/area-map-npc-assets-after.json; the new manifest
records each output. Matching local-assets/field-map-npc parser source evidence
was copied for the newly committed fixtures. This includes the already validated
expanded door-data/parsed-map products documented above, not a new conversion.
Other large local-assets inputs remain shared read dependencies. No extraction,
conversion, editable import or shared pack write occurred. Original Nintendo/
Intelligent Systems rights remain reserved; ignored private copies grant no new
license or redistribution rights. No original pack/ISO bytes were committed.

### Original battle-sequence entry constants

Read-only US ISO DOL spans802c2abc+70 and804203b0+20 (hex) preserve112 bytes of source room/stage/audience strings and32 bytes of color/format/listener data. Private battle-sequence-entry.pmevt is180 bytes, SHA2567a5295a1050b3672569655c8f637aebd65aa69924acc700db5e18de8d5cf8349. Metadata records span hashes. No code, BSS, saved mode, work state, readiness, texture scaling or replacement art/audio is exported. tools/battle_sequence_entry_assets.py participates in assets-base. Original Nintendo/Intelligent Systems resources, local use only; no redistribution rights. Packs stay ignored.


### Original map-definition/history source views

`tools/field_map_npc_assets.py::source_segments` now includes read-only US
G8ME01 _map_attr_data8030f668/0x954,198 named12-byte records plus actual
terminator8030ffb0={0,ffffffff,0}, SHA256
5c76b1a7b2073b6ddbfbeac319e786c9b44b77330e9bd7c062e0f5b8661dd4cc,
all referenced source strings and empty literal804207a8. Source neighbors for
PMEVT alignment remain actual DOL bytes. No gor02 entry is invented. Private
`romfs/door-data.pmevt` grows148048→152004bytes; metadata in door-data.json
records1362aligned views totaling48196bytes with overlaps possible. Retained
history is native live F storage, not packaged fabricated BSS/save defaults.

Native area preparation uses the existing parsed gor02 tags unchanged and
explicit adapted guest identities.13 bank metadata fingerprints refresh from
the exporter dependency change; all52 animation/directory/MODEL/TPL products
remain byte-identical, as does field-npc-map-gor_02.bin.87 product hashes and
16 metadata/source-pack changes are recorded in ignored
artifacts/area-map-history-assets-after.json. Original Nintendo/Intelligent
Systems resources remain local only; no redistribution rights are asserted.
ISO and compiled/extracted packs stay outside source history. See
[history contract](behavior/field/TAGGED_MAP_NPC_HISTORY_GATE_AND_DEFINITION_SOURCE.md)
and [native composition](behavior/field/NATIVE_MAP_NPC_HISTORY_BOOTSTRAP.md).

## Field-motion documentation validation cache (2026-10-14)

artifacts/field-motion-code-documentation-validation/source starts with the
accepted draw-pipeline documentation RomFS and evidence caches. The frozen
converters read /home/carson/games/roms/G8ME01.iso read-only and regenerated
private door-data.pmevt/door-data.json (152004-byte mutable door data, 970 objects)
and battle-sequence-entry.pmevt/battle-sequence-entry.json (180-byte pack).
The expanded door data retains the original map-history attribute table; the
battle-entry pack retains original strings/colors/listener data. Converter logs
and output SHA256 values are in the validation manifest. No editable asset import
or shared cache writes occurred. Mutable message-icons/material-light/field-native/
field-map-npc/door-data evidence directories were copied privately; remaining
large local-assets dependencies were shared read inputs.

recovery-effect.bin was copied from artifacts/battle-lens-qwwrj2n7/romfs only after
matching artifacts/recovery-lifecycle-source-manifest.json SHA256
553c3a347e1f88c4206fd3a26a0531c5a6ba673ca3bb63183846dd2ba1d7138f.
No image/audio conversion or original binary/ISO bytes were committed. Original
Nintendo/Intelligent Systems rights remain reserved; these private validation
copies confer no new license or redistribution rights.

Battle initialization/storage adds no asset conversion or extraction. Validation
uses the previously validated battle-entry2098 RomFS products overlaid with the
87 hash-matched products in artifacts/area-map-history-assets-after.json for the
current committed map-history code. All products are private originals/metadata;
source ISO remains read-only. Native arena, globals and pouch aliases are mutable
runtime storage and are never exported as original asset/readiness fixtures.

The isolated parsed-map tests also require the9322-byte private source
local-assets/field-map-npc/gor_02.s, original m/gor_02/s, SHA256
dd1dd8400fa0b94dbd6d266155dd1e9caba82176e7eec38e46acf0f3328ef2ec. It was
copied from matching prior evidence and verified against parsed-product metadata;
no new ISO extraction or redistribution.

## Field-entry documentation validation cache (2026-10-14)

artifacts/field-entry-code-documentation-validation/source privately copies the
accepted field-motion documentation RomFS and matching evidence inputs. Before
copying, door-data.pmevt/json, battle-sequence-entry.pmevt/json and recovery-effect.bin
were SHA256-checked against that validation manifest. The new manifest records
parent provenance and the accepted hashes. Mutable message-icons/material-light/
field-native/field-map-npc/door-data evidence directories are private copies;
remaining large local-assets dependencies are shared read inputs. No new ISO
read, extraction, conversion, editable import or shared pack write occurred.
Original Nintendo/Intelligent Systems rights remain reserved; private validation
copies confer no new license or redistribution rights. No original binary/ISO
bytes were committed.


### NPC reset and retained room-VM validation assets

No new original resource conversion or asset selection was introduced for
code290e015d. The isolated reset/room-retirement fixtures use existing local
romfs/door-data.pmevt, field-npc-map-gor_02.bin and original field-native model/
pose/texture banks. All87 previously journaled private products are byte-identical
in artifacts/area-npc-reset-assets-after.json. Original-data inspection for
teardown and RNG lifecycle remains ignored under artifacts/npc-room-teardown/
and artifacts/field-rng-lifecycle/; the ISO is read-only. No original assets,
inspection ELF/listings or compiled packs are committed. Existing Nintendo/
Intelligent Systems local-use/no-redistribution provenance remains unchanged.

## NPC/object documentation validation cache (2026-10-14)

artifacts/npc-object-code-documentation-validation/source privately copies the
accepted field-entry documentation RomFS/evidence. Door-data.pmevt/json,
battle-sequence-entry.pmevt/json and recovery-effect.bin matched parent manifest
SHA256 values before copying; the new manifest records parent provenance and
accepted hashes. Mutable message-icons/material-light/field-native/field-map-npc/
door-data evidence directories are private copies; other large local-assets
inputs are shared read dependencies. No new ISO read, extraction, conversion,
editable import or shared pack write occurred. Original Nintendo/Intelligent
Systems rights remain reserved; these private validation copies confer no new
license or redistribution rights. No original binary/ISO bytes were committed.

Animation refresh/scratch recovery adds no extraction or conversion. Validation
reuses the frozen battle-init2100 private RomFS products and original gor_02.s
source fixture with its previously recorded hash. Original MODEL header counts
come from existing original CPU resources; native test W/file/holder/heap views
are explicitly runtime fixture policy, not distributable original readiness
assets. ISO remains read-only, and generated packs stay outside source history.

## Map-lifecycle documentation validation cache (2026-10-14)

artifacts/map-lifecycle-code-documentation-validation/source privately copies
accepted NPC/object documentation RomFS/evidence inputs. Door-data.pmevt/json,
battle-sequence-entry.pmevt/json and recovery-effect.bin matched recorded parent
SHA256 values before copying; manifest.json records parent provenance and hashes.
Mutable message-icons/material-light/field-native/field-map-npc/door-data evidence
directories are private copies; other large local-assets inputs are shared read
dependencies. No new ISO read, extraction, conversion, editable import or shared
pack write occurred. Original Nintendo/Intelligent Systems rights remain reserved;
private validation copies confer no new license or redistribution rights. No
original binary/ISO bytes were committed.

## Field-item documentation validation cache (2026-10-14)

artifacts/field-items-code-documentation-validation/source privately copies the
accepted map-lifecycle documentation RomFS/evidence. Door-data.pmevt/json,
battle-sequence-entry.pmevt/json and recovery-effect.bin matched parent manifest
SHA256 values before copying; the new manifest records parent provenance and
accepted hashes. Mutable message-icons/material-light/field-native/field-map-npc/
door-data evidence directories are private copies; remaining large local-assets
inputs are shared read dependencies. Cache assembly needed no new ISO read,
original-asset extraction/conversion, editable import or shared pack write. Original Nintendo/Intelligent
Systems rights remain reserved; private validation copies confer no new license
or redistribution rights. No original binary/ISO bytes were committed.


## Registered NPC setup world validation (2026-10-14)

Code65d1b669 reuses original door-data.pmevt scripts/table rows, parsed gor02
map records, numeric gor battle closure and thirteen retained NPC resource
families from the previous private reset/history validation cache. All87 owned
products match prior hashes; no extraction, texture/geometry/music conversion,
editable import or shared pack write. Source/normal/ARM fixture artifacts and
hashes are recorded in artifacts/area-npc-world-validation-manifest.json;
compiled packs/binaries remain outside source history. The ISO was not modified
or committed. Original Nintendo/Intelligent Systems rights remain reserved;
private validation copies confer no redistribution license. Actor/pose loading
and event setup tests do not establish new NPC raster or hardware parity.

## Trouble Center documentation validation cache (2026-10-14)

artifacts/trouble-center-code-documentation-validation/source privately copies
accepted field-item documentation RomFS/evidence inputs. Door-data.pmevt/json,
battle-sequence-entry.pmevt/json and recovery-effect.bin matched recorded parent
SHA256 values before copying; manifest.json records parent provenance and hashes.
Mutable message-icons/material-light/field-native/field-map-npc/door-data evidence
directories are private copies; other large local-assets inputs are shared read
dependencies. Cache assembly needed no new ISO read, original-asset extraction/
conversion, editable import or shared pack write. Original Nintendo/Intelligent
Systems rights remain reserved; private validation copies confer no new license
or redistribution rights. No original binary/ISO bytes were committed.


### Shared original field player pose resources (2026-10-14)

`tools/field_player_pose_assets.py` reads `a/a_mario`, `a/a_mario_r`,
`a/e_mario`, `a/c_pkr` and each MODEL's named `a/<texture-bank>-` from the
read-only US ISO. It retains complete original MODEL/TPL/descriptor order and
converts current prologue clips plus actual descriptor0 using the existing
surface converter. Full a_mario conversion exceeds the current64-cell atlas;
this export bounds clips instead of relaxing the schema or aliasing unavailable
poses. Private generated metadata records source ISO offsets, sizes and SHA256,
selected clips and product hashes. Original defaults: M_Z_1/M_Z_1R/M_S_5/PKR_Z_1.

Products live privately at artifacts/shared-pose-source/romfs/field-player-*,
with source/atlas evidence under its local-assets/field-player-poses/. Matching
c_pkr and field-native-c_kuri_h GPU atlases are resized with Lanczos to512px
width and half logical height, encoded RGBA8 tex3ds; PMSUV1 preserves V scale.
Renderer selects matching resource identity. Frankly retains the existing field
CPU descriptor set and a matching shared-field-frankly GPU pack. No generated
assets or ISO are committed. Nintendo/Intelligent Systems resources; local use
only, no redistribution rights granted. Original art only; no generated artwork.

## Original NPC actor-leaf data closure (2026-10-14)

Codeb385793a adds tools/field_npc_actor_assets.py to the canonical door exporter.
It opens /home/carson/games/roms/G8ME01.iso read-only and pins nine unchanged
DOL views:802c1470/c,802c14b0/8,802c14f0/40,8041fd80/4,8041fdcc/10,
8041fde0/8,8041f448/4,8041f45c/4,802bf240/8 (hex address/length).
These128 bytes retain original name/numeric constants and real neighboring
padding; no binary bytes are fabricated or committed. The previous linked
script pack omitted `me`, although npc_init_evt referenced8041fd80.

Privately regenerated using python3 tools/door_data_assets.py inside
artifacts/source-checks/trouble-world. All extracted gor REL/prefetch data and
new compiled packs remain ignored there, without modifying shared products.
New door-data.pmevt is152228 bytes (previous152004); original addresses and
source values are retained. Its metadata records each DOL span digest. Of87
prior owned product hashes, only romfs/door-data.pmevt and romfs/door-data.json
change. No texture/geometry/music conversion;13 retained NPC resource families
are reused. Exact hashes/build log and isolated normal/ARM fixtures are in
artifacts/area-npc-leaves-validation-manifest.json. Original Nintendo/Intelligent
Systems rights remain reserved; local extraction conveys no redistribution
license. ISO was not modified or committed. This validates source data and
scoped callback use, not new NPC GPU raster or hardware performance.


### Pose-buffer allocator validation (2026-10-14)

No new art/audio conversion or shared pack writes. Validation retains2125
private RomFS products; only door-data.pmevt/json are regenerated with the
committed base extractor to include its newer NPC actor leaves. All other2123
products, including original MODEL/TPL/player/field NPC resources, are byte-identical
to the previous shared-pose manifest. Original allocator fixture reads actual
MODEL counts and same typed pose bytes; test W/file/holder/scratch views do not
certify file loading or original startup. ISO remains read-only; no generated
assets committed.

## Pouch/field-session documentation validation inputs (2026-10-14)

artifacts/pouch-selection-code-documentation-validation/source starts from
accepted Trouble Center documentation RomFS/evidence. It overlays 25 new original
player products and updated Frankly metadata only after SHA256 matches
artifacts/shared-pose-validation-manifest.json and artifacts/shared-pose-source.
This retains a_mario/a_mario_r/e_mario/c_pkr complete MODEL/TPL/descriptors,
bounded CPU clips plus true descriptor0, and matching canonical Goombella/Frankly
GPU atlases. Conversion choices are unchanged from the shared-pose asset entry.

It also copies door-data.pmevt/json from artifacts/source-checks/trouble-world
only after length/SHA256 matches artifacts/area-npc-leaves-assets-after.json.
The 152228-byte pack retains nine original hash-pinned DOL actor-event string/
numeric views (128 payload bytes), including actual me/default-init dependencies.
No PPC/BSS or invented success records were added. The new manifest journals all
28 product overlays and accepted hashes. Initial Python fixture failure exposed
the metadata's relocated PNG path; private atlas evidence was copied after matching
226126 bytes/SHA256 9fb226b8d49f4a8ce80301a75f96eba44728e782de0454f660e6aabd71c96bf3.

Cache assembly needed no ISO read, new original conversion, editable import or
shared cache write. Existing Python player-converter validation independently
reads the original ISO read-only and rebuilds four banks into empty temporary
directories, then discards them. Other large local-assets dependencies remain
shared read inputs; mutable evidence directories are private copies. Original
Nintendo/Intelligent Systems rights remain reserved; private validation copies
confer no new license or redistribution rights. No original binary/ISO bytes
were committed.


### Original cold animation auxiliary payloads (2026-10-14)

`tools/animation_startup_assets.py` reads a/vivian.bin (3088 bytes; ISO offset
926901544; SHA256478c261680ace260c1a56b4ca1a16c7076209bd0d60c3b5a54c1d7cfcf0064b1)
and a/ag2tg.bin (41984 bytes; SHA256d756d29d6e8eb7848bf2ff96b1c463223c140f0c33f7385ffac1ca40918125ba)
from the read-only US ISO. Full source offsets/sizes/hashes are also recorded in
the generated per-file provenance and animation-startup/sources.json. Outputs
are unchanged private payloads under ignored romfs/animation-startup/a/ and the
isolated artifacts/animation-cold-source/romfs/animation-startup/a/; no conversion,
resizing, generated artwork, licensing change or asset commit. ag2tg duplicates
the already retained prefetch bytes by identity; it does not create a new table.
Original Nintendo/Intelligent Systems data, local use only, no redistribution
rights granted. Availability does not certify original file-handle/lifetime
or cold startup. Existing packs remain unchanged.

## Packed NPC wander validation cache (2026-10-14)

Code3342657e uses original existing gor regular scripts and registered NPC
resources. New isolated artifacts/source-checks/npc-wander starts from
committed sourcea3a0a5d9 and hash-verified2125 products from the accepted
shared-pose validation cache, plus private local evidence/accepted read-only
large dependencies. Source actor pose handles are resolved through real+104
records; no synthetic pose0-based numbering or substitute models are used.

Canonical python3 tools/door_data_assets.py ran only in this private checkout
against read-only /home/carson/games/roms/G8ME01.iso. It restores the already
delivered nine actor-leaf DOL string/numeric views to that older cache. Only
romfs/door-data.pmevt and romfs/door-data.json differ from the2125 parent hashes;
all other products and parsed gor_02.s hash match. No new art, texture/geometry,
animation/music/SFX conversion or shared pack write. Ignored private extracted
REL/prefetch bytes and compiled packs/binaries stay outside source history.
Exact source/asset/log/capture/normal/ARM hashes are recorded in
artifacts/area-npc-wander-validation-manifest.json. Source body evidence for
Mario pointer/radial hint/material getters remains ignored separately.
Original Nintendo/Intelligent Systems rights remain reserved; local extraction
provides no redistribution license. ISO unchanged/uncommitted. This validates
scoped script/memory use, not full NPC GPU raster or hardware performance.

### Original common-menu file backing (2026-10-14)

`tools/battle_class_surface_assets.py` now also packages the exact original
`battle/common/battle_common.tpl` as ignored `romfs/battle-class-common.tpl`.
Read-only source: `/home/carson/games/roms/G8ME01.iso`; temporary extraction:
`local-assets/battle-class-ui/battle_common.tpl`. Size217920, SHA256
`8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1`.
No resampling, decoding, pointer relocation or geometry/lighting changes apply
to this raw copy. Existing circle/bubble atlas products remain byte-identical.
The new raw bank permits real file/holder/TPL ownership; it does not activate
ordinary menu scheduling. Native source Set relocates a separate mutable copy.
Original Nintendo/Intelligent Systems rights retained; no redistribution license
is granted. ISO, extraction, raw bank and compiled products remain uncommitted.
See [packaging contract](behavior/display/BATTLE_CLASS_COMMON_TPL_PACKAGING.md).

The HUD coin/Star Point groups use already packaged original IDs475/443/444/478;
no new texture allocation/conversion is needed. The converter records original
status/getter source pins; see [HUD counters](behavior/display/NATIVE_BATTLE_HUD_STATUS_ELEMENTS.md).

## Packed battle-entry documentation validation inputs (2026-10-14)

artifacts/battle-entry-code-documentation-validation/source reuses accepted
pouch/field-session documentation RomFS/evidence. Cache assembly checks all 36
accepted product hashes (including five cold-animation auxiliary files matched
to artifacts/animation-cold-validation-manifest.json) plus the relocated original
Frankly PNG hash, then copies
RomFS and mutable message/material/field/map/door/player evidence privately.
Other large local-assets directories remain shared read inputs. Conversion
choices and per-asset provenance/licensing in generated metadata are unchanged.
Native validation skips asset regeneration. Cache assembly performs no new
original conversion/import/shared write; existing Python converter fixtures
separately read the ISO read-only and generate only in empty temporary dirs.
Original Nintendo/Intelligent Systems rights remain reserved; private validation
copies confer no new license or redistribution rights. No original binary/ISO
bytes were committed.

### NPC radial-hint and Main-parent validation (2026-10-14)

Code8618d376 changes no original art, music, dialogue or compiled asset format.
Private validation reuses all2125 products from the prior packed-wander asset
baseline, verified byte-for-byte unchanged in
artifacts/npc-radial-hint-assets-after.json. Read-only original DOL inspection
adds ignored npcMain/direct-callee and reviseAngle evidence under
artifacts/npc-main-parent-evidence/. No new conversion or scaling choice;
original Nintendo/Intelligent Systems rights and existing private-use licensing
notes remain unchanged. No original ISO/extracted binary/compiled pack is
committed or distributed. Native build skips regeneration; Python converter
fixtures read the original ISO read-only and write temporary outputs only.

## Wander/animation/audio documentation validation inputs (2026-10-14)

artifacts/actor-services-code-documentation-validation/source reuses accepted
battle-entry documentation RomFS/evidence. Cache assembly verifies 38 accepted
product hashes and the relocated Frankly PNG. Mutable evidence is copied
privately, including common-menu source pixels; other large local-assets inputs
remain shared reads. Two committed menu products are copied only after matching
artifacts/menu-resource-source-manifest.json against artifacts/menu-resource-validation:
romfs/battle-class-common.tpl and romfs/battle-class-surfaces.json. The raw TPL
retains 217920 bytes/SHA256 8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1;
its private source copy is independently matched to the same hash.

No conversion, resampling, geometry/lighting adjustment, editable import or shared
pack write occurs during cache assembly. Existing Python validation independently
reads the original ISO read-only and generates in empty temporary directories.
Native validation deliberately skips regeneration. Provenance/conversion choices
in accepted generated metadata are unchanged. Original Nintendo/Intelligent
Systems rights remain reserved; private validation copies grant no redistribution
license. No ISO/extraction/original binary bytes were committed.


### Shared animation memory validation assets (2026-10-14)

This change adds no extracted or converted assets. Its frozen build uses2130
private RomFS products from the accepted animation-cold validation manifest;
all remain byte-identical. Original c_pkr/a_mario/c_kuri_h animation data and
original packed door event descriptors feed shared-owner tests. The fixture's
MODEL header views copy only original0x1b0 bytes from accepted resource packs;
virtual file/holder addresses are test-owned and do not assert original file
manager/type5 relocation history. The ISO remains read-only. No original bytes
are committed or redistributed; original rights/local-use restrictions remain.

### Original NPC Main clock literal (2026-10-14)

The existing tools/field_npc_actor_assets.py unchanged-data closure now adds four
original big-endian bytes at US revision0 DOL8041fd50:49742400 (1000000.0).
SHA256db3aefef05f0dfc29a110d15dbd860630fa5173d7f5d908a57a60984e7495322.
Extraction opens /home/carson/games/roms/G8ME01.iso read-only. No scaling,
geometry/lighting/audio/dialogue conversion or new pack format is involved.
The literal is read live through the VM facade after each original conversion.

Regenerated private products:artifacts/source-checks/npc-wander/romfs/door-data.pmevt
and door-data.json; mutable gor.rel work stays in that private checkout's
local-assets/door-data. All other2123 accepted RomFS products are unchanged;
artifacts/npc-clock-assets-after.json records complete hashes and the two
expected changes. Numeric helper ELF/disassembly inspection is ignored under
artifacts/npc-main-child-abi-audit; it is evidence only, never executable PPC in
the native pack. Native validation skips asset regeneration after this private
conversion; Python converter fixtures separately read the ISO and use temporary
outputs. Original Nintendo/Intelligent Systems rights/licensing remain unchanged.
No ISO, extracted original binary or compiled asset pack enters source history.

## Battle-controller documentation validation inputs (2026-10-14)

artifacts/battle-control-code-documentation-validation/source reuses accepted
actor/audio documentation RomFS/evidence. Cache assembly verifies all 38 accepted
product hashes, relocated Frankly PNG and the private common-menu raw TPL against
the packaged original hash. Mutable icon/material/field/map/door/player/menu
evidence is privately copied; other large local-assets directories remain shared
read inputs. No additional pack overlay, conversion, resampling, geometry/lighting
adjustment, editable import or shared cache write is needed. Existing Python
fixtures independently read the original ISO read-only and generate into empty
temporary destinations. Native validation deliberately skips asset regeneration.
Accepted provenance/conversion/licensing metadata is unchanged. Original
Nintendo/Intelligent Systems rights remain reserved; private copies grant no
redistribution rights. No ISO/extraction/original binary bytes were committed.


### Type5 model relocation validation (2026-10-14)

No new original asset extraction/conversion. Full unchanged private
field-player-a_mario.draw-model.bin (215124 bytes) and
field-player-c_pkr.draw-model.bin (120932 bytes) exercise loaded-model pointer
setup/clear in temporary writable copies. They retain their accepted source
provenance, original raw offsets and embedded model/animation payloads. Tests
restore the complete original bytes after Clear. The2130 prior RomFS products remain unchanged except the common-menu metadata,
and one accepted raw common TPL is added for newer committed base requirements.
Both menu products match the accepted menu-resource-source-manifest.json. The original ISO
is read-only; no original payload is committed/distributed. Nintendo/Intelligent
Systems rights and existing local-use-only restrictions remain unchanged.

### Original class footer cues (2026-10-14)

`tools/battle_class_footer_assets.py` extracts common-bank indices4..16 from
read-only `/home/carson/games/roms/G8ME01.iso`, path
`battle/common/battle_common.tpl` (217920 bytes, SHA256
8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1).
Ignored work cache: `local-assets/battle-class-footers`; compiled products:
`romfs/battle-class-footers.bin`, `.t3x`, `.json`. Original cue4/5 images are80x32,
danger6..10 are112x48, critical11..16 are112x56. All six language table mappings
are retained. Conversion preserves original decoded RGBA pixels without
resampling, places two-pixel clamp-edge extrusion in512x256 RGBA8 backing
(524288 texture bytes), and performs no geometry/lighting change. Metadata
records source-body verification and per-image pixel hashes. Normal assets-base
invokes the narrow converter; no editable source is overwritten. The native
footer owner now imports this exact atlas with linear filtering and clamped
atlas edges; the retained TexturePlane2 and direct pinch-Quad providers append
original cue geometry in class-menu order. No assets are imported inside an
active GPU frame. Ordinary battle scheduling remains a separate integration. Original Nintendo/
Intelligent Systems rights retained; no redistribution license is granted. ISO,
extracts, original images and compiled packs remain outside source history.

## Packed battle-state/motion documentation validation inputs (2026-10-14)

artifacts/battle-packed-code-documentation-validation/source reuses accepted
battle-controller documentation RomFS/evidence. Cache assembly verifies all 38
accepted product hashes, relocated Frankly PNG and privately copied common-menu
raw TPL against its packaged original hash. Mutable evidence remains private;
other large local-assets directories are shared read inputs. After one initial NPC-clock fixture failure, the cache overlays only
door-data.pmevt/json matching artifacts/npc-clock-assets-after.json, whose own
hash matches artifacts/npc-clock-validation-manifest.json. Matching copies
come from artifacts/animation-files-source; the moving npc-wander cache had
changed further and was rejected. Pack152244 bytes includes original4-byte
clock literal8041fd50; no runtime/BSS/PPC state is fabricated. Final native
repack includes these accepted products. No new conversion, resampling,
geometry/lighting change, editable import or
shared cache write occurs. Existing Python fixtures independently read the ISO
read-only and generate in empty temporary directories. Native validation skips
regeneration. Accepted per-asset provenance/conversion/licensing is unchanged;
Nintendo/Intelligent Systems rights remain reserved. Private copies grant no
redistribution license. No original ISO/extraction/binary bytes were committed.


### Shared animation/menu raw file catalog (2026-10-14)

No new original extraction/conversion. The shared native file cache stages
complete unchanged accepted draw-model.bin/draw-textures.bin files and original
animation-startup/a/vivian.bin/ag2tg.bin beside the accepted common-menu TPL.
Exact ISO names/types,8raw MODEL/TPL pair equalities and source offsets/hashes
are recorded in ignored artifacts/animation-file-path-evidence/provenance.json.
a_mario/e_mario/b_mario share exact a/a_mario- bytes; no duplicate filenames or
converted GPU/CPU packs substitute for raw source files. Runtime copies alone
are relocated; immutable originals remain available. Native32-byte backing tail
padding is zero policy, not original DVD-byte provenance. Of2131private RomFS products, only door-data.pmevt/json were regenerated by the
frozen base converter for its newly committed NPC clock constant; all2129other
products retain the accepted model-relocation hashes. ISO remains
read-only; no original bytes committed/distributed and no rights change.
### Original NPC Main special-room data (2026-10-14)

The existing unchanged-data closure in tools/field_npc_actor_assets.py adds
US revision0 DOL802c1394 length24(hex): exact kpa_03 and
SFX_KUPPA_3RD_WATER_FALL1 strings plus original padding, SHA256
ff015cac7de04f35d881bd70a7ff41f26ec3f63aabfe7f07757e1a7be56a4b49.
DOL8041fcf0 length4 retains original float100 bits42c80000, SHA256
9dc1b62d60acff118f222dc22cb76a1c7e9f2fcc1ae7d16c56861eb0301bd59a.
Existing constructor spans already cover365/.5/1. Extraction opens
/home/carson/games/roms/G8ME01.iso read-only. PMEVT segments retain source
word alignment; no fabricated padding, executable PPC, new format, art/audio/
dialogue conversion or rights change. Original Nintendo/Intelligent Systems
rights remain reserved; original data/ISO/compiled packs remain outside history.

Private outputs:artifacts/source-checks/npc-wander/romfs/door-data.pmevt and
door-data.json, with mutable extraction in that checkout's local-assets/door-data.
The private cache first verifies the accepted animation-live2130 RomFS product
hashes, then regenerates only this source closure. Native builds skip subsequent
regeneration. Python converter fixtures read the ISO and write temporary outputs.
Read-only getter listing/ELF and hashes remain ignored under
artifacts/npc-main-entry-evidence/; no original binaries are committed.

The first full validation run exposed the newer committed menu loader's additional
raw romfs/battle-class-common.tpl dependency, absent from the older animation
cache. Added its217920 unchanged bytes from artifacts/menu-resource-validation,
verified against artifacts/menu-resource-source-manifest.json and the producer's
original SHA2568eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1.
No re-extraction or relocation was performed by this cache repair. The completed
private cache has2131 RomFS products; its two regenerated source-data products
and one accepted supplemental TPL are recorded separately in
artifacts/npc-main-entry-assets-after.json and npc-main-entry-extra-cache.json.

## Typed battle-support documentation validation inputs (2026-10-14)

artifacts/battle-support-code-documentation-validation/source reuses accepted
packed-battle documentation RomFS/evidence. Cache assembly verifies 41 accepted
product hashes plus relocated Frankly/common-menu source evidence and 14 private
footer PNG/atlas hashes. It copies only 5 additional/changed cache products:

- door-data.pmevt/json match artifacts/npc-main-entry-assets-after.json, whose
  own SHA256 matches artifacts/npc-main-entry-validation-manifest.json.
  Pack 152308 bytes retains the committed original NPC-entry data closure.
- battle-class-footers.bin/t3x/json match dependencies recorded in
  artifacts/class-frame-source-manifest.json, from artifacts/class-frame-validation.
  Metadata pins all 13 source image pixel hashes; private common-4..16.png images
  and atlas crops are independently checked against them before building.

Manifest journals exact sources, old/new product hashes and authority hashes.
Source bank remains original battle/common/battle_common.tpl; footer products
retain prior conversion choices. Mutable evidence is private; other large local-
assets inputs remain shared reads. No new conversion, resampling, geometry/lighting
change, editable import or shared pack write occurs. Existing Python fixtures
read ISO read-only and generate only in empty temporary destinations; native
regeneration is skipped. Original Nintendo/Intelligent Systems rights remain
reserved; private copies grant no redistribution license. No original ISO/
extraction/binary bytes were committed.

### NPC Main movement subsection (2026-10-14)

Movement49e8c..49fbc reuses the existing source zero/.5 words and admitted
NPC/pose/map assets. No new asset conversion, geometry, sound or story text.
Original PSVECSubtract8029eba4 (36 bytes) was inspected read-only from the US
ISO; ignored evidence resides in artifacts/npc-main-parent-evidence/.
SHA2565997a5a32c7df8eb2a03901039345f35a84f00b017545800218ef21bdbfaf81a.
Only its semantic implementation/contract is committed. Extracted helper ELF,
listing, ISO and compiled packs remain outside source history. Existing rights
and private local-use restrictions are unchanged; this grants no redistribution
license. Current private2131-product baseline is recorded in
artifacts/npc-main-entry-assets-after.json; see PROGRESS for hash verification.


### Cold animation bootstrap data image (2026-10-14)

New private animation-boot/memory.pmevt and memory.json contain14 original US
revision0 DOL data/BSS ranges. tools/animation_boot_image.py pins address, size
and SHA256 for every range and reads the ISO read-only. Initialized DOL data
wins over overlapping broad BSS; Vivian's original0xc00 data is retained.
No executable, ISO or packed original bytes enter source history. Manifest
includes origins and extraction choices. Nintendo/Intelligent Systems rights
remain unchanged; no redistribution rights are granted.

Validation snapshot also regenerates accepted door-data.pmevt/json for committed
NPC clock constants and imports battle-class-footers.bin/json/t3x using the
already committed original-pixel footer converter. These are prerequisites from
the committed base, not animation art changes. Other accepted RomFS products
remain byte-identical; exact hashes/diff are in ignored
artifacts/animation-process-validation-manifest.json. All products stay private.

### Original NPC automatic-pose constants (2026-10-14)

field_npc_actor_assets.py reads the US ISO read-only and adds unchanged DOL
spans802c13c8/24 bytes (binary64 .5,3,0),8041e3c8/4 (canonicalNaN7fffffff),
and8041fcdc/4 (idle3dcccccd). SHA256 identities are in
[the behavior contract](behavior/field/NATIVE_NPC_MAIN_AUTOMATIC_POSE_SECTION.md)
and ignored artifacts/npc-auto-pose-constants.json. The private door-data.pmevt/
json closure is regenerated with these spans; no code/BSS, fabricated strings,
resampling, geometry or audio changes. Native pose tests select actual original
c_kuri_h animation names/indices from the existing unchanged directory/pack.
Source extraction/evidence, compiled packs and test executables remain outside
Git. Existing Nintendo/Intelligent Systems rights and private local-use
restrictions are unchanged; no redistribution license or original binary/ISO
is committed. See PROGRESS for complete cache hash checks and validation.

## Battle command/menu documentation validation inputs (2026-10-14)

artifacts/battle-commands-code-documentation-validation/source reuses the accepted
typed battle-support documentation cache. Assembly verifies all 41 accepted
product SHA256 values plus relocated Frankly/common-menu evidence and 14 footer
PNG/atlas hashes, copying mutable evidence privately and sharing only large
read inputs. The first host gate exposed stale inherited door data required by
baseline b04c612f's automatic-pose fixture. Retained failed host/native logs;
overlaid only door-data.pmevt/json from artifacts/source-checks/npc-wander after
byte size/SHA256 matched artifacts/npc-auto-pose-assets-after.json and that
manifest matched its recorded validation authority. Pack 152376 bytes, SHA256
371c6d0c6a5a4d47f1d33f027be7738f2861de72e81df2387334a3f6e6ab0662.
The documentation manifest records old/new product and authority hashes.
Full host/native gates reran after repair. No new conversion/resampling, geometry/lighting edits,
editable imports or shared pack writes. The manifest pins the parent and exact
input hashes. Existing Python fixtures independently read the ISO read-only
and convert into empty temporary destinations; native build deliberately skips
asset regeneration. All original provenance/conversion choices remain as
previously recorded. Nintendo/Intelligent Systems rights remain reserved;
private validation copies grant no redistribution license. No ISO, extracted
original assets or compiled binaries were committed.

### NPC cloud/territory detection evidence (2026-10-14)

No new asset conversion or data closure. The current private2131-product
RomFS baseline remains artifacts/npc-auto-pose-assets-after.json. Original
US ISO helpers evtGetPtrID80035a28 (96 bytes) and cloudGetBreathPower80187f10
(368 bytes) were inspected read-only; ignored listings/ELFs and exact hashes
are recorded in [the contract](behavior/field/NATIVE_NPC_CLOUD_AND_TERRITORY_DETECTION.md).
The implementation shares the previously extracted norm constants without
changing caller-specific arithmetic graphs. No fabricated event pointers,
script/dialogue replacements, art/audio/geometry edits or compiled original
assets enter Git. Existing Nintendo/Intelligent Systems rights and private
local-use restrictions remain unchanged; no redistribution license is granted.


### Original animation Entry defaults and formats (2026-10-14)

Extended private animation-boot/memory.pmevt/json from14 to20 original US
revision0 data/BSS ranges. New spans are colors8041fb0c/8, one/zero8041fb24/8,
model format/prefix8041fb34/c, texture format802c1068/8, sortstrings802c11a8/30
and defaultsort80415dc4/4. Exact source offsets/range hashes and register-base
proof live in ignored artifacts/animation-entry-evidence/. The extractor pins
every SHA256, reads ISO only, and reuses existing live Vivian-tail storage.
Animation changes only the two private boot products. Validation also
regenerates door-data.pmevt/json for already committed NPC automatic-pose
constants; all other2132 accepted products remain byte-identical. No new art conversion, redistribution or rights change;
Nintendo/Intelligent Systems originals remain private and uncommitted.

## Stage/camera documentation validation inputs (2026-10-14)

artifacts/battle-stage-camera-code-documentation-validation/source reuses accepted
battle-command documentation RomFS/evidence. Assembly verifies 41 product hashes,
common-menu source/relocated Frankly evidence and 14 private footer PNG/atlas
hashes. Inherited door-data pack 152376 bytes retains automatic-pose numeric
closure accepted in the preceding validation. The first host gate exposed missing
cold-image data required by committed animation_startup_process. Failed log is
retained. Only animation-boot/memory.pmevt/json are added from
artifacts/animation-process-source after hashes matched
artifacts/animation-process-validation-manifest.json. Four frozen startup
source/header/tool/fixture hashes match that authority. Pack 4456 bytes SHA256
e0fc4934db86df43558001ae2dac5a72915cdd305b70559a956f7acc67f499ae; metadata 5135
bytes SHA256 f4ab76fd2efbf8f30c836cda517bdc00d41a452d49b92f4b14bec1aa43cae225.
Final cache verifies 43 product hashes and manifest records exact sources/authority.
Both full gates reran. Original image retains 14 US DOL data/BSS ranges, with
initialized DOL data winning overlap, as documented in the original asset notes.
No new conversion,
resampling, geometry/lighting changes, editable imports or shared pack writes.
Small mutable evidence is copied privately; other large local-assets inputs are
shared reads. Existing Python fixtures independently read ISO read-only and
convert into empty temporary outputs; native regeneration is deliberately skipped.
Parent/input hashes are recorded in the documentation validation manifest.
Original conversion choices/provenance and Nintendo/Intelligent Systems reserved
rights remain unchanged; private tracking/copying grants no redistribution
license. No ISO, extracted original asset or compiled binary was committed.
## 2026-10-14 — Retained battle Main and original HP drawing source

Read-only source: `/home/carson/games/roms/G8ME01.iso`, US GameCube DOL.
`artifacts/retained-main-source-verification.json` checks20 bodies/117 instruction
anchors, two direct call sites and one data span for the recovered Main/manager
contracts. `artifacts/hp-presentation-source-evidence.json` pins the original
HP parent8011fde0/244 and bar8011fc3c/1a4;
`artifacts/hp-raster-source-evidence.json` pins TexPlane801032c8/11c,
PlainGX801033e4/300, InitRasta80103b48/f4, InitSub80103c3c/94,
QuadRasta80103918/d4 and PointRasta801039ec/28.
`artifacts/icon-number-source-evidence.json` pins iconNumberDispGx80019120/284
(SHA25617d2306ea5857b5a64e428ae24292ba9991cbd8d08ec01ddfac766a0604f645e),
the40-byte numeral tables80415084,12-byte scalars8041f87c and8-byte signed
bias802bf848. Existing tutorial packed views independently match those original
constant ranges (`artifacts/icon-number-existing-resource-proof.json`), so this
source increment adds no original binary/texture pack and performs no texture
resampling or geometry conversion. Original data and derived private dumps
remain ignored outside source history. Nintendo/Intelligent Systems retain
rights to the original game assets/code; no redistribution license is granted.

## Action UI/class raster documentation validation inputs (2026-10-14)

artifacts/battle-action-ui-code-documentation-validation/source reuses accepted
stage/camera documentation RomFS/evidence. Assembly verifies 43 product hashes,
common-menu/relocated Frankly evidence and 14 private footer PNG/atlas hashes.
Proactively replaces only animation-boot/memory.pmevt/json from
artifacts/animation-live-entry-source after both SHA256 values match
artifacts/animation-live-entry-validation-manifest.json. Five frozen startup/
Entry source/header/tool/fixture hashes also match that authority. Exact sources,
old/new product hashes, byte counts and authority SHA256 are in the manifest.
Memory pack SHA256 5365973450f39d9ce334692ba14f13350f8f3fc0c8cd14b370dfb4c849b1db57;
metadata SHA256 0305a9358bd344fba520a93123c97f5236e4a0c452d7fb3efff6d788ef0bb261.
Original image adds six source Entry color/one-zero/format/sort spans to prior
DOL data/BSS image; initialized DOL data wins overlaps as before. No new
conversion/resampling, geometry/lighting changes, editable imports or shared
pack writes. Mutable evidence is private; other large inputs remain shared reads.
Existing Python fixtures independently read ISO read-only and use empty temporary
outputs; native regeneration is deliberately skipped. Nintendo/Intelligent
Systems rights remain reserved. Private tracking/copying grants no new license;
no ISO, extracted original asset or compiled binary was committed.

### Original NPC Mario detection constants and helper provenance (2026-10-14)

field_npc_actor_assets.py reads the US ISO read-only and pins seven unchanged
spans:802c1210/16 (Kpa radii0,0,30,100),802c1460/8 (double.75),8041fd54/8
(.05/.25),8041fcb0/4 (90),8041fcc4/4 (270),8041fccc/4 (180),8041fcd4/4
(float.75). Exact byte/SHA256 identities:ignored artifacts/npc-mario-constants.json.
Private door-data.pmevt/json is regenerated; no mutable sequence/Mario success
values, PPC, script/story changes, art/audio/geometry resampling or replacement.
seqGetSeq/KpaGetLevel/VivianGetStatus bodies were inspected read-only; sizes/
hashes and ignored evidence paths are in
[the contract](behavior/field/NATIVE_NPC_MARIO_DETECTION.md). Runtime sequence
word80415190 is NOT imported from its ISO initialffffffff value. Extracted
source/evidence, compiled packs and executables remain outside Git. Existing
Nintendo/Intelligent Systems rights and private local-use restrictions remain
unchanged; no redistribution license or original binary/ISO is committed.
## 2026-10-14 — Original enemy HP17/18 surfaces

Read-only `/home/carson/games/roms/G8ME01.iso` path
`battle/common/battle_common.tpl` (217920 bytes;SHA256
8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1).
`tools/battle_enemy_hp_assets.py` verifies original descriptors17/18:40x16 CMPR,
320 bytes each at0xb100/0xb240. Compressed hashes:
1968aec5606a2dc326d272efa0602a98ebd08ab15a99b53014e8bedd26a73ddd and
21c140ae6dae0bda018f9468b364fb733789d5ba60deacb8879acc8e516bc202.
RGBA pixel hashes and original descriptor metadata are retained in the private
`battle-enemy-hp-surfaces.json`. Conversion preserves exact decoded pixels,
adds2px transparent padding and places them in a128x32 RGBA8 atlas (16KiB);
no resizing, lighting change or geometry reduction. All original edge pixels
have zero alpha, preserving present clamp boundaries with this padding.
Separate68-byte PMBEHP01 catalog and `.t3x`/`.json` products live in ignored
RomFS/work directories; footer4..16 catalog remains unchanged.
Nintendo/Intelligent Systems retain original rights; no redistribution license.
The native GPU owner executes source HP textured/raster/solid material state and
validates generation/whole bar batches before drawing. This alone does not prove
normal battle activation or unified number/bar order; those require the actual
queued callback and native ordered publication owner.


### Runtime animation tangent alias (2026-10-14)

No extraction/conversion or new original product. The actual cold initializer's
178 runtime words at803d9580 now alias CPU tangent consumers directly. Original
factor/native tan adapter and source initialization cadence are unchanged; no
static copied tangent asset substitutes for live storage. Existing20-span boot
image and all accepted private RomFS products remain byte-identical. Read-only
output/tangent layout evidence is artifacts/animation-output-evidence/; no ISO,
original bytes or packs committed/distributed and rights remain unchanged.

## Audience-code documentation validation inputs (2026-10-14)

artifacts/battle-audience-code-documentation-validation/source reuses accepted
action-UI documentation cache, then adds 5 hash-verified prerequisite overlays:

- door-data.pmevt/json from artifacts/source-checks/npc-wander match
  artifacts/npc-mario-assets-after.json; its SHA256 matches
  artifacts/npc-mario-validation-manifest.json. Pack 152472 bytes SHA256
  6a13a31fe9e979c1a9008d394390f8a49e91ca2a8982739c3434b98e2bc2a9f3
  retains committed source Mario-detection numeric spans.
- battle-enemy-hp-surfaces.bin/t3x/json from artifacts/hp-stage-validation match
  artifacts/hp-stage-source-manifest.json. Five frozen GPU/header/tool/fixture
  hashes match its recorded inputs. Source battle/common/battle_common.tpl
  SHA256 8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1,
  original indices 17/18 (40x16 CMPR) retain decoded pixels with 2px transparent
  padding in 128x32 atlas, no resampling. Compressed-source and decoded pixel
  hashes plus exact atlas crops independently verify;common-17/18.png and
  atlas.png copies live under private local-assets/documentation-hp-evidence.

Manifest journals exact sources, old/new product and authority hashes; final
accepted count 46, plus 14 existing footer PNG/atlas and 3 HP PNG evidence hashes,
common-menu/relocated Frankly evidence. No new conversion/resampling, geometry/
lighting changes, editable imports or shared pack writes. Existing Python
fixtures independently read ISO read-only and generate into empty temporary
destinations; native asset regeneration is deliberately skipped. Original
Nintendo/Intelligent Systems rights remain reserved; tracking/copying grants no
new license. No ISO, extracted original asset or compiled binary was committed.

### NPC display cull/depth constants (2026-10-14)

`tools/field_npc_actor_assets.py` reads G8ME01.iso read-only and pins unchanged
DOL spans8041fcd8/4 (-1),8041fd5c/4 (-2),8041fd34/4 (2),8041fd60/8
(5000,10000). SHA256 values are recorded in SPANS and independently matched by
the display source audit. Existing zero8041fc7c is reused. No numeric replacement,
resampling, geometry/lighting conversion or asset substitution. Private frozen
RomFS contains2131 products; only door-data.json/pmevt differ from the accepted
Mario baseline; pack152492 bytes SHA256
98cba3c1443f1c23a7ca3bb9b38f7a06c9b705262e6b974c7b11824bc72f7fe4.
Source helper identities are recorded in the behavior contract; no executable
PPC is added to the pack. Nintendo/Intelligent Systems rights remain reserved;
local extraction grants no redistribution rights. No ISO, original extracted
asset, compiled pack or binary is committed. Exact hashes/evidence remain in
artifacts/npc-display-assets-after.json and the validation manifest.

### NPC shadow radius constants (2026-10-14)

Read-only G8ME01.iso DOL spans8041fce0/4 raw40400000 (3) and8041fd68/4
raw3faa3d71 (1.3300000429) are unchanged in field_npc_actor_assets.SPANS.
SHA256 respectively638354a0b185ec05318029e52610c8de48d33523e2b4854e71154187583d1037
andff83f8ff106269769ede5d0da22e7f511640e2146d3bfe363f9a295b9bf00414.
Existing zero/one/100/-1/90/180/270 spans are reused. sinfd's original TAU/360
body is documented but delegated to a real numeric provider; no +/-1 replacement
or executable PPC is packed. No texture, geometry, lighting or audio conversion.
Private frozen door pack152488 bytes SHA256
842715d5cd84974d7de49898eda7441d018400a8cd7d6c5634d5c8babca941b7.
The span merger joins contiguous data, so the pack shrinks4 bytes despite added
constants. All2131 products audited against accepted display baseline; only
door-data.json/pmevt change. Local gor_02.s unchanged. Helper hashes/paths are in
the behavior contract and ignored source audit; original body/ISO/pack/binary
are not committed. Nintendo/Intelligent Systems rights remain reserved. Local
use grants no redistribution rights. Exact private product hashes are recorded
in artifacts/npc-shadow-assets-after.json and the validation manifest.

## Battle presentation-code validation inputs (2026-10-14)

artifacts/battle-presentation-code-documentation-validation/source inherits 46
accepted products and original common-menu/Frankly evidence,14 private footer
PNG/atlas hashes and 3 private HP PNG hashes. Recorded NPC-display door products
were absent from 126 existing cache candidates. A failed preflight copied nothing;
initial compilation was already launched. Targeted regeneration runs the FROZEN
source/tools/door_data_assets.py with ISO /home/carson/games/roms/G8ME01.iso
read-only in empty artifacts/battle-presentation-code-documentation-validation/
cache-rebuild. It uses the frozen symbol catalog and converters; it imports no
editable assets or shared output. Original rel/gor.rel and linked US DOL data
retain raw source words/provenance/licenses. No texture/geometry/lighting edits.

Generated door-data.pmevt/json exactly match artifacts/npc-display-assets-after.json,
whose own SHA256 matches artifacts/npc-display-validation-manifest.json. Frozen
actor-data converter also matches the source code commit recorded there. Pack
152492 bytes SHA256 98cba3c1443f1c23a7ca3bb9b38f7a06c9b705262e6b974c7b11824bc72f7fe4;
metadata 479622 bytes SHA256 7ff8bed4b1c1cc61de7432a8af65bdfb8b08c83f0dc24ab8335a8e2c6c5464b2.
Only these two replace private validation inputs; generated ancillary original
field/animation-prefetch outputs stay unused in the empty private rebuild tree.
Manifest records command, source/authority/product hashes and all generated outputs.
Initial/final gate logs are retained; both gates rerun after initial completion.
Native asset regeneration remains skipped; existing Python fixtures independently
read ISO read-only and use empty temporary outputs. Nintendo/Intelligent Systems
rights remain reserved; private tracking/copying grants no new license. No ISO,
extracted original asset or compiled binary was committed.

### NPC history/traversal asset audit (2026-10-14)

No new extraction, pack data or asset conversion. All2131 frozen RomFS products
match the accepted shadow baseline byte-for-byte, including original messages,
audio and door-data.pmevt152488 bytes SHA256
842715d5cd84974d7de49898eda7441d018400a8cd7d6c5634d5c8babca941b7.
Local gor_02.s unchanged. Read-only DOL sign audit of8041fce4/8 verified
c416000044160000 (-600,+600); only documentation was corrected, no numeric pack
replacement. Existing extraction/conversion paths and Nintendo/Intelligent
Systems rights stay unchanged. No ISO, original body/asset, pack or binary
committed. Exact hashes:evidence artifacts/npc-history-assets-after.json and
npc-history-validation-manifest.json.


### Source animation scratch readers (2026-10-14)

No animation asset conversion or geometry/art/audio/dialogue change; no new
animation pack. Validation repaired the existing HP/door fixture assets below.
The actual relocated original MODEL files staged by NativeFileCache supply raw
controls,44-byte keys, discrete rows and five sparse channels. Original source
scratch allocations supply authoritative cache/output bytes, including24-byte
texture records; native snapshots do not overwrite them. Existing original-data
licensing/provenance restrictions continue. ISO is read-only; extracted assets,
RomFS packs, disassembly and probes stay ignored. Validation uses the complete
accepted live-tangent RomFS snapshot plus three hash-matched accepted HP GPU
files needed by the newer frozen source base. Its own door-data converter was
rerun read-only against the ISO to include current NPC numeric views (152492
bytes/970 objects). Final RomFS has2139 files; only those five paths differ
from the preceding animation validation. The changing shared RomFS was not
used as a substitute fixture.

## Damage/retained-owner documentation validation inputs (2026-10-14)

artifacts/battle-damage-owners-code-documentation-validation/source inherits 46
accepted products, original common-menu/Frankly evidence, 14 private footer PNG/
atlas hashes and 3 private HP PNG hashes. Before gates, only door-data.pmevt/json
from artifacts/source-checks/npc-wander replace private validation inputs after
exact size/SHA256 matches to artifacts/npc-shadow-assets-after.json; its own
hash matches artifacts/npc-shadow-validation-manifest.json. Frozen
field_npc_actor_assets.py bytes match its recorded source code commit.
Pack 152488 bytes SHA256 842715d5cd84974d7de49898eda7441d018400a8cd7d6c5634d5c8babca941b7;
metadata 479926 bytes SHA256 8484a6a64155718257f8e19b80414cfa04293a5cd160041e78753aa20c1fb4a5.
Exact sources, old/new hashes and validation authorities are journaled in the
manifest. Existing original shadow DOL span choices remain unchanged. No cache
rebuild, new conversion/resampling, geometry/lighting changes, editable imports
or shared pack writes. Existing Python fixtures independently read ISO read-only
and use empty temporary outputs; native full asset regeneration is deliberately
skipped. Nintendo/Intelligent Systems rights remain reserved; private tracking/
copying grants no new license. No ISO, extracted original asset or compiled
binary was committed.

## Harbor service documentation validation inputs (2026-10-14)

artifacts/harbor-services-code-documentation-validation/source reuses 46 accepted
products from battle-damage-owners documentation validation, plus common-menu/
Frankly evidence, 14 private footer PNG/atlas and three HP PNG hashes. All hashes
were checked before and after gates. No converter/assets/Makefile changes
occurred between parent and current baselines, so no new overlays/conversion
were required. Existing original source ranges and conversion choices remain
unchanged; no resampling, geometry/lighting changes, editable imports or shared
pack writes. Existing Python converter fixtures independently read the ISO
read-only and use empty temporary outputs; native full asset regeneration is
deliberately skipped. Nintendo/Intelligent Systems rights remain reserved;
private tracking/copying grants no new license. No ISO, original extracted asset
or compiled binary committed. Exact dependency/log/native hashes and source
baseline are journaled in the ignored validation manifest.

### NPC field-tail constants (2026-10-14)

field_npc_actor_assets.SPANS pins unchanged DOL8041fce4/8 rawc416000044160000
(-600,+600), SHA2564d3b57e63f076c32b53d1b26264b1a272d87587d5b4dfef339ca8c9eb350ae2c;
and8041fc84/4 raw42700000 (60), SHA256
b1d2919619c1f7fac6b930d389c96f873d849fdf909b58027ee4c97148483777.
Read-only /home/carson/games/roms/G8ME01.iso matched independently. Existing60
bytes were already packed; -600/+600 add8 unchanged bytes. Private door pack
152496 bytes SHA256
917782e901710c8c85af5bcf172c3b1baa03d23b66faf16aa4b9997112b7aed3.
All2131 products audited against accepted history baseline; only door-data.json/
pmevt differ. Local gor_02.s unchanged. No texture/geometry/lighting/audio or
message conversion, numeric replacement or executable PPC added. Source/helper
paths/hashes are in the behavior contract and ignored tail audit. Nintendo/
Intelligent Systems rights remain reserved; local extraction grants no new
redistribution rights. No ISO/original body/asset, pack or binary committed.
Exact product hashes:artifacts/npc-tail-assets-after.json and validation manifest.

## Harbor composition documentation validation inputs (2026-10-14)

artifacts/harbor-composition-code-documentation-validation/source reuses 46
accepted products from harbor-services documentation validation, plus original
common-menu/Frankly evidence, 14 private footer PNG/atlas and three HP PNG hashes.
All hashes were checked before and after gates. Converter/assets/Makefile inputs
are unchanged between parent and current baselines; no new overlays/conversion
were needed. Original source ranges/conversion choices remain unchanged; no
resampling, geometry/lighting changes, editable imports or shared-pack writes.
Existing Python converter fixtures independently read the ISO read-only and use
empty temporary outputs; native full asset regeneration deliberately skipped.
Nintendo/Intelligent Systems rights remain reserved; private tracking/copying
grants no new license. No ISO, original extracted asset or compiled binary was
committed. Exact dependency/log/native hashes and source baseline are journaled
in the ignored validation manifest.
## Trio common-menu file-owner binding (2026-10-14)

The native Trio now acquires `battle/common/battle_common.tpl` through the original
menu Init/End and the real process file cache, using the unchanged US format at
`802ef810` and original root at `8041f494`. The existing original common-bank
extract and HP17/18 atlas retain their conversion and rights terms above.
`tools/trio_attack_assets.py` now imports only72 immutable bytes at `80418450`;
the next four bytes at `80418498` belong to the live file-manager global and
must be supplied by that owner. This removes a copied-data/live-owner overlap.
A private pack-only rebuild from the read-only ISO confirms all other segment
payloads and the68-byte unit definitions are unchanged. New event pack size
1357332 bytes; SHA256 `0a737b207a9321678398c12c0d7b0bef8ad123ed97cc748f5e7d68b9ffb2123d`.
Evidence: `artifacts/hp-runtime-assets-proof.json`; private outputs remain ignored.
No actor texture/geometry conversion changed and no ISO or extracted/compiled
asset bytes enter source history. Original Nintendo/Intelligent Systems rights
remain unchanged; this project gains no redistribution rights.
### Original shared NPC matrix constant (2026-10-14)

Read-only US G8ME01 disc0 revision0 `/home/carson/games/roms/G8ME01.iso`
DOL8041fd7c..fd7f supplies unchanged big-endian bytes3c8efa35, the original
rounded radians factor0.01745329238474369. The extraction view SHA256 is
7388adb66bc2fb8cdd730d8148adcae18abd8bb48b05b7bd9b9e05efa6c09702.
`tools/field_npc_actor_assets.py` records and verifies that range. Regenerating
`tools/door_data_assets.py` in the isolated accepted source tree updates only
private `romfs/door-data.json` and `romfs/door-data.pmevt` among2131 products.
The pack is152488 bytes, SHA256
62929efd5f0115a23ec550bc5c6bd7ffe886b0a332fc02c1df8384688ecfca8a.
Four added source bytes bridge two spans, removing one12-byte index row; pack
size therefore decreases8. Removing only these four bytes and remerging recovers
the exact prior152496-byte pack SHA256
917782e901710c8c85af5bcf172c3b1baa03d23b66faf16aa4b9997112b7aed3.
The local gor_02.s and all other RomFS product hashes remain unchanged.
No art resampling, geometry reduction, lighting or audio conversion changes.
Original Nintendo/Intelligent Systems resources remain local use only, with no
redistribution rights. ISO, extracted resources, source listings and binaries
are ignored/private and were not committed. Callback/matrix source evidence and
exact validation/product hashes are in `artifacts/npc-matrix-validation-manifest.json`.

## Trio provider documentation validation inputs (2026-10-14)

artifacts/trio-providers-code-documentation-validation/source inherits 46 accepted
products plus original common/Frankly/footer14/HP 3 evidence. Frozen committed
NPC-tail converter adds original DOL -600/+600/60 spans; existing cached outputs
did not match its accepted authority, so frozen door_data_assets.py ran into empty
private cache-rebuild from read-only ISO. All 15 generated hashes recorded; only
hash-matched door-data.pmevt/json admitted. Source/authority hashes are journaled;
NPC-tail manifest pins exact after-product inventory. Pack 152496 bytes SHA256
917782e901710c8c85af5bcf172c3b1baa03d23b66faf16aa4b9997112b7aed3.

Four Trio pack/provenance products are also checked against
artifacts/hp-runtime-assets-proof.json, whose source converter hash matches frozen
code. Two were unchanged; event pack/provenance replaced. New event pack 1357332
bytes SHA256 0a737b207a9321678398c12c0d7b0bef8ad123ed97cc748f5e7d68b9ffb2123d
excludes only original live file-manager global 80418498 after 72 immutable bytes
at 80418450. Total explicitly hash-pinned accepted products 50. Conversion choices
for textures/geometry/lighting/audio/messages unchanged. No editable import or
shared pack writes. Python fixtures independently read ISO read-only into empty
temporary outputs; native full asset regeneration skipped. Nintendo/Intelligent
Systems rights remain reserved; private tracking/copying grants no new license.
No ISO/original extracted asset/compiled binary committed.
### Original ordinary NPC callback table and strings (2026-10-14)

Read-only US G8ME01 disc0 revision0 `/home/carson/games/roms/G8ME01.iso`
provides three unchanged DOL views, verified by `field_npc_actor_assets.py`:

- 803066f8+70 source luigi_pose pointer table, SHA256
8852534105c3a34d4502e358073ddb34badadbe8f1cabc9e78b6247e3af5df0f.
- 8041fbb8+b0 source facing-name strings and original alignment padding, SHA256
7bbfc161f7c122cf76249066d996595b892fdd8f1f588eb48f05944a9302343c.
- 8041fcb8+14 source -90/radians-to-degrees/360/270/-270 constants, SHA256
e60211f64c283101dcfedbcd2c23ed918c1d02ebefcf037602e8f15d75e9993d.

The original table is .data; the strings are .sdata2. These are source-backed
input views, copied into actual VM-owned memory at import, with fresh source
pointer reads retained. No names, pointers or numeric values are synthesized or
normalized. The original unusual J_1A→J_1RR pair is preserved. Existing270 bytes
overlap consistently. Net new source bytes304; segment header changes add12.
Private `romfs/door-data.pmevt` is152804 bytes, SHA256
e9e85d212180b28087c7c8c4d904f82417c1a0ea30f1271ab10c2a3d0c9a78ab.
Removing exactly the newly covered304 source bytes and remerging recovers the
prior152488-byte pack and SHA256
62929efd5f0115a23ec550bc5c6bd7ffe886b0a332fc02c1df8384688ecfca8a.
Only door-data.json/pmevt changed among2131 RomFS products. Local gor_02.s and
all other product hashes remain unchanged. No art/audio resampling, geometry
reduction or lighting changes were made. Nintendo/Intelligent Systems rights
remain reserved; local extraction grants no redistribution rights. ISO, raw
original assets/listings and compiled packs/binaries remain ignored/private.
Exact source/product/native validation is journaled in
`artifacts/npc-draw-validation-manifest.json`.

### Ordinary field Mario source-bank GPU packages (2026-10-14)

`tools/field_player_pose_assets.py` now exports matching local `.bin`/`.t3x`
packages for original a_mario, a_mario_r and e_mario, alongside c_pkr. These
use each bank's actual converted texture-cell layout, not the combined legacy
characters atlas. Retain RGBA8, original cells/UV coefficients, 512-wide Lanczos
downscaling and linear filtering. Added original M_U_2/M_I_U front and
M_N_5B/M_N_C effect clips for retained native requests. Original MODEL/TPL and
source-order descriptor bytes are unchanged. No synthesized art, dialogue,
geometry, lighting or audio conversion in this step.

The read-only US rev0 DOL boot image additionally pins normal model names
802c18a0/28, triplet80415eb0/12, c_pkr token804213e0/8 and original
frame-to-ms8041fb40/4 (41855555), with hashes/section offsets in ignored
animation-boot/memory.json. Native bus/timebase/arena/caller RAM are explicit
runtime inputs and are not presented as original initialized data.

Frozen local validation retains2145 RomFS products:17 differences from the
accepted source-scratch baseline, including six new Mario `.bin`/`.t3x` files,
boot metadata/pack, updated field-player metadata/CPU banks and door-data
metadata/pack regenerated with the frozen baseline converter. The latter
restores that baseline's original NPC shadow constants; no helper/test assertions
were weakened. Exact hashes,
original provenance and product differences are recorded privately in
`artifacts/field-source-animation-asset-diff.json` and the validation manifest.
All extraction used the supplied ISO read-only; generated products and extracted
originals remain ignored. Nintendo/Intelligent Systems rights unchanged; local
use only, no redistribution rights. See
[ordinary native binding](behavior/display/NATIVE_ORDINARY_MARIO_SOURCE_ANIMATION.md).

## Trio event/ending documentation validation inputs (2026-10-14)

artifacts/trio-events-code-documentation-validation/source inherits 50 accepted
products, original common/Frankly/footer 14/HP 3 evidence. Frozen committed NPC actor
converter adds original DOL 8041fd7c..fd7f raw 3c8efa35, radians factor
0.01745329238474369, SHA256 7388adb66bc2fb8cdd730d8148adcae18abd8bb48b05b7bd9b9e05efa6c09702.
Required accepted matrix door outputs absent in existing caches, so frozen
source/tools/door_data_assets.py ran into an empty private cache-rebuild directory
from the read-only ISO. All generated output hashes recorded; only door pair
admitted after exact size/hash matches to artifacts/npc-matrix-assets-after.json,
whose hash matches npc-matrix-validation-manifest.json. Pack 152488 bytes SHA256
62929efd5f0115a23ec550bc5c6bd7ffe886b0a332fc02c1df8384688ecfca8a;
metadata 480382 bytes SHA256 f2657acf544314fbb96396b15bd354f73443fd8b4ad791d61eb669066b01179d.
Four source bytes bridge spans, removing one 12-byte index row; other source bytes
remain unchanged. Two private overlays retain 50 explicit accepted products.
No editable import/shared pack write, texture/geometry/lighting/audio/message
conversion change or new license. Python fixtures independently read ISO into
empty temporary outputs; native full regeneration skipped. Nintendo/Intelligent
Systems rights reserved. No ISO/original extracted asset/compiled binary committed.
### Original NPC blur color and polynomial literals (2026-10-15)

Read-only US G8ME01 disc0 revision0 `/home/carson/games/roms/G8ME01.iso`
provides unchanged DOL8041fc68+4 color bytesffffff00, SHA256
6fc6f4524161c3ae0d316812d7088e3fcd372023edaea2d7821093be40ae1060,
and DOL8041fd10+20 original polynomial/quadrant constants, SHA256
4ddafcc41cea1d3c6c6b18c0945cf7e58cb2ad26c03e662e8803c708bad73157.
`field_npc_actor_assets.py` verifies both views before private packing. Exactly
36 source bytes were added. Private `romfs/door-data.pmevt` is152840 bytes,
SHA256cf1404103dc9ab8a6c5325aaa0f1a72f72e8b9778b130677d7072766bbca3369.
Removing only those36 bytes and remerging recovers the prior152804-byte pack
and SHA256e9e85d212180b28087c7c8c4d904f82417c1a0ea30f1271ab10c2a3d0c9a78ab.
Only door-data.json/pmevt changed among2131 private RomFS products. Local
gor_02.s and all other product hashes remain unchanged. No art/audio resampling,
geometry reduction, lighting or dialogue changes were made. Source constant
loads, single-precision/FMA graph and original color/alpha behavior are retained;
no generic sin/cos or generated replacement assets were introduced.
Nintendo/Intelligent Systems rights remain reserved; local extraction grants
no redistribution rights. ISO, raw original assets/listings and compiled packs/
binaries remain ignored/private. Exact source/product/native validation is
journaled in `artifacts/npc-effects-validation-manifest.json`.

## Trio scheduler documentation validation inputs (2026-10-15)

artifacts/trio-scheduler-code-documentation-validation/source inherits 50 explicit
accepted products plus original common/Frankly/footer 14/HP 3 evidence. Frozen
committed animation_boot_image.py/field_player_pose_assets.py hashes match
artifacts/field-source-animation-validation-manifest.json; its asset_diff agrees
with field-source-animation-asset-diff.json. NPC-draw after-product inventory is
hash-pinned by npc-draw-validation-manifest.json. Before gates, 33 exact matching
cache inputs were checked: field-player/boot products and two door products.
Seventeen actual changes, including six new Mario bin/t3x files, bring explicit
accepted coverage to 56. All source/product/authority hashes journaled and rechecked.
No extraction, new conversion, editable import or shared-pack write in this batch.

Retain prior original field-bank texture layout, RGBA8, 512-wide Lanczos scaling,
linear filtering, source MODEL/TPL/descriptor order, added original clips and pinned
boot tokens/triplet/frame-to-ms. NPC-draw pack 152804 bytes SHA256
e9e85d212180b28087c7c8c4d904f82417c1a0ea30f1271ab10c2a3d0c9a78ab.
No geometry/lighting/audio/message changes. Python fixtures independently read
ISO read-only into empty temporary outputs; native full asset regeneration skipped.
Nintendo/Intelligent Systems rights retained; private tracking/copying grants no
new license. No ISO/original extracted asset/compiled binary committed.## 2026-10-14 — Original Trio enemy draw resources

The read-only US ISO producer `tools/trio_enemy_pose_assets.py` extracts
`a/c_kuribo` (125524 bytes, SHA256
9b934dee83eb489f5151d8a497ac59611e402b7db786c9e82b4809d7d7a475e4)
and `a/c_kuribo-` (23680 bytes, SHA256
bccf3f47fd1f713cf8c86a162f9b8a2f86af0b4398a9806e57b114baaf9fc692).
Original tik setup ids3/4/5 select KUR_S_1/TGK_S_1/PTK_S_1 on that shared
model. All45 descriptors, source draw MODEL and TPL companions remain complete.
Private output prefix is `local-assets/trio-enemy-native/pack/trio-enemy-kuribo`;
ordinary build output prefix is ignored `romfs/trio-enemy-kuribo`. The producer
records exact source byte offsets, hashes, relocations, descriptor order, all14
original TPL headers and every converted product hash. PMANIM4 CPU data and
PMPOSE02 source directory accompany unchanged MODEL/TPL. Dedicated native RGBA8
atlas1024x256 uses existing124px Lanczos cells with2px clamp gutters and UVscale4;
original per-image dimensions/samplers remain in the raw TPL. No source geometry
reduction, generated art or replacement sound is introduced in this batch.

The native resource and atlas binding uses actual model/pose/resource identity
and separate pose handles for all three units. This supplies dependencies;
source Shape buffers, active camera scheduling and visible battle HP still need
runtime proof. Original HUD/common-menu/HP17/18 asset rights and conversions are
unchanged. Nintendo/Intelligent Systems retain original rights; extraction grants
no redistribution license. ISO, extracted originals, atlas and compiled packs
remain outside source history. See [enemy resource contract](behavior/display/TRIO_ENEMY_ORIGINAL_POSE_RESOURCES.md).

## 2026-10-15 — Source animation RAM acceleration validation cache

The callback-scoped RAM optimization changes no original asset, source clip,
texture, dialogue or conversion policy. Validation uses original read-only
`/home/carson/games/roms/G8ME01.iso` products in ignored private trees. Baseline
652dded0 required its door converter output (152804-byte `door-data.pmevt`,
SHA256 e9e85d212180b28087c7c8c4d904f82417c1a0ea30f1271ab10c2a3d0c9a78ab)
and JSON, regenerated privately. Stale inherited battle packs were replaced
with hash-verified accepted `trio-attack-events.pmevt` (SHA256
0a737b207a9321678398c12c0d7b0bef8ad123ed97cc748f5e7d68b9ffb2123d)
and `recovery-effect.bin` (SHA256
553c3a347e1f88c4206fd3a26a0531c5a6ba673ca3bb63183846dd2ba1d7138f)
from the matching Trio scheduler validation authority. This repairs the private
baseline cache; it does not alter original content. The full product inventory
and provenance hashes are in the ignored animation-ram-borrow validation
manifest. Native probe binaries, extracts and packs remain uncommitted.
Original Nintendo rights/licensing restrictions and existing provenance apply.
### Original shared animation-facing literals and accepted dependencies (2026-10-15)

Read-only US G8ME01 disc0 revision0 `/home/carson/games/roms/G8ME01.iso`
provides unchanged animation-local literals8041fb24+c (one/zero/radians), SHA256
782014c141964e6e2c573fa2cf76ac830d29948d46f62ec53a01531a27958882,
and8041fb6c+2c (sign/heading/wrap/smoothing), SHA256
18c2acf85532159d4157b603544711245e68b9f6fd3132b106a3802ae739555b.
Actor extraction verifies both original spans; the cold animation image adds
only missing fb2c+4 radians and fb6c+2c facing views, preserving existing
one/zero. Their original addresses and repeated loads are retained; equal
NPC-local constants are not substituted. Private door-data.pmevt is 152920 bytes,
SHA256 42eff18104303e162533727e7e3dc4faafa6269b55b398ad25d16b9b75044378;
animation-boot/memory.pmevt is 4788 bytes, SHA256
be0dfa08ab0ed46e45b7b9d2432a6e22f0c4991f4f50e08d56dbc0ce7c7986be.
Removing just the new source bytes/rows reproduces both exact prior packs.

Validation updates its committed-code baseline to include the landed native
field animation process. 22 prerequisite products were refreshed byte-for-byte
from hash-verified accepted field-source-animation validation, retaining the
previous NPC door-data baseline. Four further prerequisites were regenerated with the committed trio-attack extractor: the old cached partner span included the live file-manager word at 80418498; the current 80418450+48 span excludes it. The battle pack and three JSON provenance files changed; mesh, texture and animation payloads did not. This dependency refresh is journaled separately
in artifacts/animation-facing-seed-assets.json and references
artifacts/field-source-animation-validation-manifest.json. It includes original
Mario resource/texture/raw-file and existing footer/HP dependencies, with no new
conversion choices by this feature. The combined baseline has 2145 products;
only door-data JSON/pmevt and cold animation memory JSON/pmevt then changed for
this feature. All other hashes and local gor_02.s remain unchanged.
No art/audio resampling, geometry/lighting or story/dialogue changes were made.
Nintendo/Intelligent Systems rights remain reserved; local extraction grants
no redistribution rights. ISO, original resources/listings and compiled packs/
binaries remain ignored/private. Exact dependency/source/product/native logs
are in artifacts/animation-facing-validation-manifest.json.

## Trio facade documentation validation inputs (2026-10-15)

artifacts/trio-facade-code-documentation-validation/source inherits 56 accepted
products, original common/Frankly/footer/HP evidence. Frozen NPC-effect converter
reproduces exact door-data.pmevt/json from read-only ISO into empty private output
because required pair absent in old caches. Only authority-matched pair copied;
ancillary generated hashes recorded/unused. npc-effects after-inventory hash is
pinned by its validation manifest. Pack 152840 bytes SHA256
cf1404103dc9ab8a6c5325aaa0f1a72f72e8b9778b130677d7072766bbca3369.

Seven original trio-enemy-kuribo products are copied from existing private pack:
six payloads plus metadata. Frozen converter matches owned source journal; helper+
original tik.rel/c_kuribo MODEL/TPL fingerprint matches metadata. Original source
hashes, roles/initial names, 45-descriptor order, 14 textures, per-product hashes and
atlas PNG are independently checked; private atlas evidence retained. Source
MODEL SHA256 9b934dee83eb489f5151d8a497ac59611e402b7db786c9e82b4809d7d7a475e4;
TPL SHA256 bccf3f47fd1f713cf8c86a162f9b8a2f86af0b4398a9806e57b114baaf9fc692.
Eleven extra hash-matched private fixture paths (three raw inputs, seven pack/
metadata, atlas) also admit default Python local-data checks; first skipped logs
retained, full gates rerun. No test changes, new enemy conversion/resampling or
editable import/shared-pack write. Total
explicit accepted products 63. Source CPU descriptors/raw draw metadata/dedicated
RGBA8 atlas retain their prior original conversion and rights terms. All source/
product/authority/evidence hashes are journaled. Python fixtures independently read
ISO into empty temporary outputs; native full regeneration deliberately skipped.
Nintendo/Intelligent Systems rights retained; private tracking/copying grants no
new license. No ISO/original extracted asset/compiled binary committed.

## Original HitN64 texture evidence, 2026-10-15

The read-only `tools/hit_n64_source_evidence.py` verifies the original US
`effHitEntry` wrapper, HitN64 constructor/Main and the still separate Display/
main_dl bodies. It extracts `e/us/effect_n64.tpl` into private verification
output; SHA256 `3202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef`.
Index74 is original I4,32x128,no palette,repeat/linear. Pixels decode without
resizing; no atlas/native texture import is admitted by this batch.

The independent verification output is
`artifacts/battle-action-native-validation/hit-source/texture74.png`, SHA256
`c80e1cca9619ca95b6758b2d4778f2722b63705d1f512b97f919e09552be22be`;
its report is `artifacts/battle-action-native-validation/hit-source-proof.json`.
Original vertex bank `803a0458/118` has280 bytes, SHA256
`8778157212c6fd114b2746aefd029272202764db44511a746d3d168530feef9b`.
Constructor/Main CPU ownership does not establish queued display or GPU use.
Original Nintendo/Intelligent Systems rights remain; private extraction grants
no redistribution rights. No original pixels, ISO, packs or binaries are committed.
### Retained Main validation prerequisites (2026-10-15)

The retained Main entry and paper-sampling change introduces no asset format or
conversion changes. Its isolated committed-code baseline requires the existing
original `c_kuribo` resource catalog. Seven missing products under private
`romfs/trio-enemy-kuribo.*` were generated using the already committed
`tools/trio_enemy_pose_assets.py`, reading `/home/carson/games/roms/G8ME01.iso`
read-only. Existing conversion choices are recorded in the original enemy
resource manifest: all 45 source descriptors, unchanged draw/TPL metadata,
dedicated RGBA8 atlas and surface-aware resource identity. This is prerequisite
refresh for landed code, not newly invented art or a new conversion policy.
The previous 2145 products remain byte-identical; the combined 2152-product
inventory and seven additions are pinned in
`artifacts/animation-main-assets-after.json`. No story/dialogue, music/sound,
geometry reduction or lighting changes. All source/extracted files, generated
packs and executables remain private/ignored. Original Nintendo/Intelligent
Systems rights remain reserved; extraction grants no redistribution rights.
Exact source/build/native evidence is in
`artifacts/animation-main-validation-manifest.json`.

## Retained battle documentation validation inputs (2026-10-15)

artifacts/battle-retained-code-documentation-validation/source inherits 63
accepted products, original footer/HP/enemy atlas evidence and private default
Python enemy fixtures from the prior Trio facade snapshot. New baseline converter
changes are tools/animation_boot_image.py and tools/field_npc_actor_assets.py.
Their bytes match facing authority tree f939e93610545b5ae2510f9adbf4cbda990a2503.
The authority manifest pins animation-facing-assets-after.json; its four changed
door-data and animation-boot JSON/PMEVT files are hash-matched before private copy.
Door pack 152920 bytes SHA256
42eff18104303e162533727e7e3dc4faafa6269b55b398ad25d16b9b75044378;
boot pack 4788 bytes SHA256
be0dfa08ab0ed46e45b7b9d2432a6e22f0c4991f4f50e08d56dbc0ce7c7986be.
All 63 accepted hashes and copied evidence/fixture hashes verified after gates.
No new conversion, resampling, editable import or shared-pack write. Native full
regeneration skipped; Python fixtures independently use read-only ISO and empty
temporary outputs. Original Nintendo/Intelligent Systems rights and recorded
conversion/provenance remain unchanged; private tracking grants no new license.
No ISO, extracted original or compiled binary committed.

## 2026-10-15 — Source pose release and original enemy file staging

The shared native animation file catalog now stages original ISO `a/c_kuribo`
(type5 MODEL) and `a/c_kuribo-` (type4 TPL) through the existing accepted
`trio-enemy-kuribo.draw-model.bin` and `.draw-textures.bin` products. Original
45-descriptor/shape/index/TPL bytes and prior conversion choices remain unchanged.
The private validation tree imports all seven hash-verified accepted enemy
products from the turn-ui validation authority; no new asset extraction or
conversion is needed for this staging change. Actual source Entry/File cache
now owns those files, and real fileFree updates their shared BE references.
Release does not delete source payloads, run collection, release GPU textures
or free the whole source scratch arena. Original rights/licensing restrictions
remain unchanged; no ISO, extracted originals or compiled packs are committed.

The frozen baseline includes the newly committed paper-facing consumer and
requires its four accepted door/animation-boot JSON/PMEVT products. Their hashes
were checked against animation-facing-assets-after.json before private import;
boot was also independently regenerated by the frozen converter/read-only ISO.
Other products inherit the preceding animation-ram-borrow accepted snapshot.
Full product/overlay/converter authority hashes are retained under ignored
artifacts/field-source-release-*.json. Shared runtime source and isolated probe
binaries remain separate; no user emulator profile/save is touched.
### Original Main clock and sample-time literals (2026-10-15)

The read-only US G8ME01 disc0 revision0 at
`/home/carson/games/roms/G8ME01.iso` supplies unchanged original rate literals
8041fba4+8 (42700000/447a0000), SHA256
`ddc0adc8aa32f0760b2cecfb24f4467cc09eff8318c59d91bfb09c2ba30e2057`,
and original double bias802c1070+8 (4330000080000000), SHA256
`07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43`.
Actor extraction adds these exact source-address views to private door-data;
the cold animation image already owns the bias and adds only the missing rates.
Its exact runtime layout and critical-word checks include those new literals.
Equal NPC/effect constants at other addresses are not substituted.

Private door-data.pmevt is 152960 bytes, SHA256
`ea3f9a6aaf9ad351a43afcc06cd8442c13eb47e20d486fdd842e81bc15dde292`;
private animation-boot/memory.pmevt is 4808 bytes, SHA256
`6f1ec8abd7fefc0ae22aed3b1e87330049d48dd73d480f43f22d8780eac5ed3a`.
Among 2152 RomFS products only the two packs and their JSON provenance changed.
Removing exactly the new source ranges/row reproduces both prior byte/hash
identities. All other product bytes and local gor_02.s are unchanged; no new
prerequisite refresh, art/audio resampling, geometry/lighting or US story/
dialogue changes. Original Nintendo/Intelligent Systems rights remain reserved;
local extraction grants no redistribution rights. ISO, extracted resources,
original source listings and compiled packs/executables remain ignored/private.
Exact source/product/native logs and hashes are in
`artifacts/animation-time-validation-manifest.json`.

## Message documentation validation inputs (2026-10-15)

artifacts/message-code-documentation-validation/source inherits 63 accepted
products and original footer/HP/enemy evidence/default Python fixture paths.
Two frozen converter changes (animation_boot_image.py/field_npc_actor_assets.py)
match clock authority tree 179bfa0bdcf66a8eee269a1d2a068a657ecea89a. Its validation
manifest pins animation-time-assets-after.json. Four changed door-data and
animation-boot JSON/PMEVT products are hash-matched before private copy.
Door pack 152960 bytes SHA256 ea3f9a6aaf9ad351a43afcc06cd8442c13eb47e20d486fdd842e81bc15dde292;
boot 4808 bytes SHA256 6f1ec8abd7fefc0ae22aed3b1e87330049d48dd73d480f43f22d8780eac5ed3a.
All 63 accepted and copied evidence/fixture hashes verified after gates. No new
conversion, resampling, editable import or shared-pack write. Native full
regeneration skipped; Python fixtures independently use read-only ISO/empty
temporary outputs. Existing message/font/icon bytes/conversions unchanged.
Nintendo/Intelligent Systems rights retained; local tracking grants no new license.
No ISO, extracted original or compiled binary committed.
### Retained timeline/scalar evaluation asset check (2026-10-15)

This source-only continuation changes no extraction, conversion, asset paths,
original story/dialogue, art/audio, geometry or lighting. All 2152 private RomFS
products and local gor_02.s retain the exact prior time-continuation hashes.
The current check is pinned in artifacts/animation-timeline-assets-after.json
and artifacts/animation-timeline-validation-manifest.json. Original Nintendo/
Intelligent Systems rights remain reserved; original resources, source listings,
packs, ISO and executables remain private/ignored.

## 2026-10-15 — Party removal source evidence

Retained Kill/Kill2 and their EVT/Mario callers use complete fresh read-only
US revision0 DOL listings, hashes and outer-order notes under ignored
`artifacts/field-party-lifetime-evidence/`. Original function addresses/hashes
are recorded in [the removal contract](behavior/partner/RETAINED_PARTY_REMOVAL.md).
No art, audio, message or new runtime pack was added. Validation regenerated
the existing animation-boot memory.pmevt/memory.json from the read-only ISO
using the frozen committed converter:27 original spans, including Main clock
8041fba4/8, are required by this baseline. The inherited26-span pack was stale;
the matching current door-data.pmevt/door-data.json were also regenerated
with the frozen converter to include original bias802c1070/8 and Main clock
8041fba4/8. Exactly these four products differ from the preceding accepted
cache; both inherited packs preceded current committed animation fixtures. Original ISO,
ELF/listing extracts and compiled products remain outside source history;
original Nintendo material retains its original ownership/licensing. These
controllers do not replace original records with authored story logic.

### 2026-10-15 Main replay scale admission

Read-only G8ME01.iso word 8041fbac+4 (3d800000, 1/16), SHA256
2625276c57610c65a55868a40c431d1fd6651984b8ee92c081b49a98a0e9d884,
added to the original startup/actor executable-memory whitelist. Exactly four
RomFS products change: door-data.json (481910 bytes,
75e1fa355d4ff6d5e88731aaf21c603333ed3b28a8cb8cfeb0fdc9c9cfb59884),
door-data.pmevt (152964, dc5602e2e1ec5845c3d58e50295bdc45046631607b425e98d1ecd31d2da46acd),
animation-boot/memory.json (9806,
1364c6f147c015823a0aff92593fb4ca2191e2c15edc4dc0f1ca43c112c443f9),
animation-boot/memory.pmevt (4824,
8e54d3126b4166c39f6c6217bf4c009ad491a06f154b1a175a06b68cbf51e01a).
Exact product names/hashes and removal-to-prior-pack proof are in ignored
artifacts/animation-replay-assets-after.json and animation-replay-products.py.
All 2152 product paths are retained; removing only this span reconstructs the
prior packs. Original gor_02.s remains unchanged. No texture scaling, geometry,
lighting, dialogue, music or SFX conversion in this step. Nintendo/Intelligent
Systems original proprietary data: local development only; existing licensing
restrictions still apply. ISO/extractions/packs/binaries remain outside history.
## Original battle class-menu message cache (2026-10-15)

Source: read-only `/home/carson/games/roms/G8ME01.iso`, original US
`msg/US/global.txt`. The existing native `romfs/messages/global.txt` is verified
byte-for-byte against that complete source resource by
`tools/verify_battle_class_menu_messages.py`. Seven original ClassTable keys
(`btl_cmd_act_jump`, `btl_cmd_act_hammer`, `btl_cmd_act_item`,
`btl_cmd_act_operation`, `btl_cmd_act_super_action`, `btl_cmd_act_tech_party`,
`btl_cmd_act_lecture_end`) retain their source IDs, text, control tokens, raw
bytes, and NUL terminators. The original ISO census locates these keys only in
`msg/US/global.txt`; no label rewriting, texture conversion, resampling, or
new editable extraction occurs.

Native field snapshots copy the actual configured `messages/global.txt` and
`messages/<roomAsset>.txt` resources through existing MessageAssets lookup,
preserving room/global precedence and ambiguity. Only fresh Trio VM entry
acquires a snapshot. The private immutable class cache at guest `0x85a00000`
registers copied original label bytes once, rejects overlaps, and remains owned
by that actual VM until teardown; no unregister, rebinding, remapping, or
source writes are admitted. This private-VM policy does not claim a generic
registration-generation lease. Missing original resources or keys remain
unavailable. Equal text from distinct source entries is not deduplicated.

Provenance and per-label hashes:
`artifacts/class-menu-original-us-label-evidence.json`; actual mapping tests
and lifetime contract:
`docs/behavior/battle/NATIVE_CLASS_MENU_MESSAGES.md`. Original Nintendo /
Intelligent Systems rights remain reserved; local provenance tracking grants
no new license. ISO, original extracted resources, and compiled packs remain
private and are not committed or distributed by this change.

### 2026-10-15 Main cubic interpolation literals

Read-only original US ISO DOL constants TWO8041fb64+4, NEGATIVE_TWO8041fb98+4,
THREE8041fbb0+4 are pinned by hashes
c31425b44a7ed682a3356bf6820141e1584a15c43b6d6793f552e5d4621586f7,
60f2e332c71bcda81951ea38d3c52d4a9b1a2056b334f99001f41c588afd0892,
638354a0b185ec05318029e52610c8de48d33523e2b4854e71154187583d1037.
Original bits40000000/c0000000/40400000 are admitted to the cold image;
actor executable data adds only NEGATIVE_TWO/THREE because TWO already exists
through NPC constructor extraction. Original scale/tangent aliases are reused.
No generated tangent substitute is introduced. Exact source addresses matter.

Exactly four products change among2152: romfs/door-data.json (482214bytes,
63b84412756ea43faf1a3ec0e2e05a9d2deaa1c3f00101b08934cea9f7a0f1e1),
romfs/door-data.pmevt (152972,
90992f21cca1bdcc34c2a0e4cfbb0913c1bba59aadb5e221ef58ba82dc0e26dd),
romfs/animation-boot/memory.json (10810,
87fd5c255d0939e0f88599417fbf0c588fa7f8a1e94c716fc9e55e86686c60fd),
romfs/animation-boot/memory.pmevt (4872,
aa06fab33e43734314a99f0aa864bdb99041b144e720b4b7b8563419f11cfd62).
Removing only these new spans reconstructs both prior packs byte-for-byte;
see ignored animation-interpolation-products.py/assets-after.json. Original
local-assets/field-map-npc/gor_02.s remains unchanged. No texture scaling,
geometry reduction, lighting, original US dialogue/story, art/music/SFX change.
Nintendo/Intelligent Systems original proprietary data remain local development
only under the existing licensing restrictions. No ISO, extracted PPC/assets,
compiled packs or binaries are committed.

## Opening frontend documentation validation inputs (2026-10-15)

artifacts/opening-frontend-code-documentation-validation/source inherits 63
accepted products and original footer/HP/enemy evidence/default fixture paths.
Two replay-span converter changes match authority tree
bbb0083142fe15c8fde95b407a174e73b07c831a; its validation manifest pins
animation-replay-assets-after.json. Four door-data/animation-boot JSON/PMEVT
products were absent in existing caches. Frozen converters ran in empty private
cache-rebuild from read-only ISO; only exact authority hashes admitted. Ancillary
outputs remain ignored/unused, with all hashes recorded. Door pack 152964 bytes
SHA256 dc5602e2e1ec5845c3d58e50295bdc45046631607b425e98d1ecd31d2da46acd;
boot 4824 bytes SHA256 8e54d3126b4166c39f6c6217bf4c009ad491a06f154b1a175a06b68cbf51e01a.
All 63 accepted products/evidence/fixtures checked after gates. No editable import,
shared-pack write, new artwork/audio or changed story payloads. Native full
regeneration skipped; Python fixtures independently use read-only ISO/empty
outputs. Nintendo/Intelligent Systems rights retained; local extraction/tracking
confers no new license. No ISO, extracted original or compiled binary committed.

### 2026-10-15 Native retained Main ownership

No asset extraction, conversion, scaling, geometry/lighting, story/dialogue,
music or SFX changes. All2152 private RomFS products and original local
field-map-npc/gor_02.s remain byte-identical to the accepted interpolation
pack; source hashes/licensing remain as recorded above. Main consumes the
actual original VM/model/texture/timeline/tangent/scratch aliases through
existing source-backed bindings. Shared GQR state and semantic camera tokens
are explicit native platform adaptations, not generated replacement assets.
Original ISO/PPC/extractions/packs/binaries remain outside history under the
existing Nintendo/Intelligent Systems local-use licensing restrictions.
The isolated validation tree additionally admits exact committed lifecycle
code/tests from ed888f61 (original pose Release/shared file ownership) because
the current native interface depends on them; no private artifact is committed.
Exact source/product hashes and dependency paths are in ignored
artifacts/animation-native-main-validation-manifest.json.

## New Game documentation validation inputs (2026-10-15)

artifacts/opening-newgame-code-documentation-validation/source inherits 63
accepted products and original footer/HP/enemy evidence/default fixture paths.
Two interpolation-span converters (animation_boot_image.py/field_npc_actor_assets.py)
match authority tree eeee7ce233970b0ae6eff60d108a225975736d35. The validation manifest
pins animation-interpolation-assets-after.json; both reports are privately archived
by hash. Four existing door-data/animation-boot JSON/PMEVT products matched before
private copy; per-product/source/converter/authority hashes are journaled.
Door-data.pmevt 152972 bytes SHA256 90992f21cca1bdcc34c2a0e4cfbb0913c1bba59aadb5e221ef58ba82dc0e26dd;
animation-boot/memory.pmevt 4872 bytes SHA256 aa06fab33e43734314a99f0aa864bdb99041b144e720b4b7b8563419f11cfd62.
All 63
accepted/evidence/fixture hashes checked after gates. No new conversion, editable
import, shared-pack write or changed opening art/audio/story assets. Native full
regeneration skipped; Python fixtures independently use read-only ISO/empty outputs.
Nintendo/Intelligent Systems rights retained; local tracking/copying grants no new
license. No ISO, extracted original or compiled binary committed.

## 2026-10-15 — Original party definition records and native removal

`tools/field_party_definition_assets.py` reads the US revision0 DOL read-only and
packages unchanged40-byte kinds1/8/13 records at80314410/80314528/803145f0.
Ignored `romfs/field-party-definitions.pmevt` (168 bytes) and its JSON provenance
are the only new runtime products. [The native removal contract](behavior/partner/NATIVE_PARTY_REMOVAL.md)
records complete source hashes/model pointers: kind8 is c_babyyoshi via802cb47c;
kinds1/13 are c_pkr via804213e0. Shared init800c4fa0 does not canonicalize kind
or model. Exit+24 is original0 in all three records. No executable data, model
conversion, art/audio/dialogue rewrite or story unlock is added.

Native loading imports initialized records once into the same animation VM;
later adapters preserve its live modifications. Model-string dependencies and
complete constructors are outside this small data pack. The Makefile assets
phase generates it; existing accepted2153 products/768 private fixtures are
inherited with hashes for isolated checks. Originals remain Nintendo/Intelligent
Systems material for local use, without redistribution rights. ISO, original
extracts, packs, provenance products and compiled binaries remain uncommitted.

## Original HitN64 drawing resources (2026-10-15)

Read-only source `/home/carson/games/roms/G8ME01.iso`: original
`e/us/effect_n64.tpl`, image74, original32x128 I4.
`tools/hit_n64_texture_assets.py` preserves the complete raw TPL as private
`romfs/hit-n64-bank.tpl` and converts image74 into unscaled RGBA8
`romfs/hit-n64-texture74.t3x`, with original REPEAT S/T and LINEAR min/mag.
No texture resize, authored replacement, padding, atlas or mip generation.
Private `local-assets/hit-n64/` retains source evidence/PNG/conversion scratch.

`romfs/hit-n64-texture74.bin` is native PMHITX01 metadata: original36-byte
descriptor,280-byte indexed vertex bank at803a0458, and immutable original
spans802fb0a0/48,80419888/52 and80425458/40. Total476bytes. Actual VM
reads take precedence; this companion supplies no mutable BSS, source pool,
State64 global/palette, camera or synthetic allocation. Provenance and hashes
are in private `hit-n64-texture74.json` and the converter's read-only ISO
checks. Original Nintendo/Intelligent Systems rights remain reserved; these
local conversions grant no license. ISO, extracts, private packs and compiled
products are excluded from source history and are not distributed by this change.

Separate isolated native import/render evidence is recorded in
`artifacts/hit-n64-native-probe/manifest.json`; it tests real texture/shader/
ordered submission and retirement, without claiming original framebuffer
parity, normal battle activation or hardware performance.

Validation dependency refresh for this drawing integration uses the frozen
committed animation-boot and NPC actor converters. Their original Main replay
scale at8041fbac/4 (SHA256
2625276c57610c65a55868a40c431d1fd6651984b8ee92c081b49a98a0e9d884)
postdates the preceding accepted cache. Four door/boot PMEVT/JSON products are
regenerated privately from the read-only ISO with those exact converters;
other inherited products remain hash-matched. This is source-data closure,
not new authored art or a measured animation constant. Original rights above
remain unchanged.

## Harbor/professor documentation validation inputs (2026-10-15)

artifacts/harbor-professor-code-documentation-validation/source inherits the
previous private 63-product cache and original enemy/footer/HP evidence/default
fixtures. Baseline f71f6785a5b0caef08f49fecfa34a48d9bb3887e adds the independent
field-party definition recipe. Frozen tools/field_party_definition_assets.py
SHA256 ce8c0e524a18e6cff4e16b3bda9939a304bb85d4d958d7a37cf943b08c31277f
reads the original ISO read-only and checks three DOL records (kinds1/8/13)
against its pinned source hashes. It ran once into an empty private cache-rebuild
output; no editable assets or shared cache were overwritten.

Two generated products admitted into the private RomFS:
field-party-definitions.pmevt, 168 bytes,
SHA256 cd6d758b411d507c6a8a17fb9cc03e582b9e086053d3032af215e0781703a750;
field-party-definitions.json, 1427 bytes,
SHA256 d8991b30cd9d1d9e73102ad6bdbd7bd1f4192cc0694d4cc8bbbb1547ddc72c80.
All 65 accepted products and inherited evidence/fixtures checked after gates.
Native full regeneration skipped; Python fixtures independently use read-only ISO
and empty temporary outputs. Source/build products remain ignored; no ISO,
extracted original or compiled binary committed. Nintendo/Intelligent Systems
rights retained; local tracking/conversion grants no new license.

## NPC numeric documentation validation inputs (2026-10-15)

artifacts/npc-numeric-code-documentation-validation/source inherits the previous
private 65-product cache and original enemy/footer/HP/default test evidence.
Only new independent HitN64 texture recipe is added to the frozen baseline.
Frozen hit_n64_texture_assets.py SHA256
074f8f4def9ba3723752a9450c6c50059644dd58de28ca48a9984ef3a7649b89
and hit_n64_source_evidence.py SHA256
c6ecdb099479a7c3b33531315d5f58fa8741dc886fd6abd2be7a84ae1ac42f3c
ran against the read-only original ISO into empty private local-assets/hit-n64.
Source function/table/TPL/vertex hashes checked; original image74 I4 decoded
32x128 without resizing, tex3ds RGBA8 conversion; raw bank/metadata retained.
Full default Python fixture privately retained, including decoded PNG/provenance.

New private RomFS products (bytes, SHA256):

- romfs/hit-n64-bank.tpl: 235776, 3202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef

- romfs/hit-n64-texture74.bin: 476, 8bc99f42c0846680b282d6ac5aa4ac0e7fcd3f2d2c546ad6292f75539c6bad0e

- romfs/hit-n64-texture74.json: 4372, 1db10702cbdd2ca10966688988c9111b90601f41344c442670110af488380a08

- romfs/hit-n64-texture74.t3x: 1365, 57f38334766192dfae0fe8522c40cb63b82ba15b1ae99f0630dd2b4b60a1f1bf

All 69 accepted products and original evidence/fixture hashes checked after gates.
Native full regeneration skipped; Python fixtures independently use read-only ISO
and empty temporary destinations. No editable import, shared-pack write or ISO,
extracted original/compiled product commit. Nintendo/Intelligent Systems rights
retained; local extraction/conversion grants no new license.

## Retained ordinary Goombella construction data (2026-10-15)

`tools/field_party_entry_assets.py` reads the original US revision0 ISO read-only
and verifies six SHA256 spans: normal kind1 pose row80314744/3c, strings
802cb4dc/8 and802cb52c/8, vector802cb178/0c, dimensions80421584/8, and
Hello dispatch803141c4/7c. Unchanged bytes become ignored
`romfs/field-party-entry.pmevt` (304bytes) plus provenance JSON. No resampling,
rewritten strings, canonicalized kinds or generated art. Existing three original
party definition records remain separate/unchanged. Native construction imports
missing complete source spans once and preserves already mapped live bytes.

Rights remain Nintendo/Intelligent Systems; local authorized ISO use does not
grant redistribution rights. No ISO/extracted original/compiled pack is committed.
Private validation inherits accepted native party assets and770 fixture inputs;
current frozen interpolation converters regenerate only boot memory and door
data JSON/PMEVT (four changed accepted products). Two construction products are
new. Detailed source/product/fixture hashes live in the ignored entry validation
manifest. Original c_pkr mode2 animation uses the shared native factory; legacy
host checkpoints keep the pre-existing CPU workspace fallback. Neither supplies
the separate d_mario resource or full original rendering/field return.

## Original raw US BFN and retained source setup (2026-10-15)

`tools/source_font_resources_assets.py` exports exact unmodified original
`f/papermarioset_US.bfn` from read-only US revision0 ISO offset948050384,
length66560, SHA256
`bb9cf410e5d916a1268ff503fc4f1ad7e8630b381cd781958eb0be32abb56b45`.
Ignored output `romfs/source-font/papermarioset_US.bfn` and
`source-font.json` retain original extent/hash and converter identity.
No original bytes/header/mappings/pixels are changed and no width patch occurs
in this converter. Existing `font_assets.py`, `font.pmfont` and `font.t3x`
remain unchanged. A new import needs an empty destination; exact matching
products may be reused, while changed payload/metadata/extra files are preserved
and rejected rather than overwritten. Converter revisions need a fresh cache.

The retained original source Setup controller performs real mutable width
writes7 then9 in actual native-backed BFN heap storage. The original file stays
width1. Actual existing process font globals/palette/spacing/name pointers,
fog and selected color can be captured/shared across VM bindings without guessed
constructor BSS or per-VM resets. File loading/mapping is not native font setup:
root/formatter/DVD/heap and GX InitTexObj/LOD effects remain genuine provider
boundaries, including actual cold-process initialization and texture ownership.
The private host fixture reads original BFN/cold DOL bytes, executes source setup
and validates shared mutable BFNs; host GX callbacks are explicit fixtures.
No normal activation, native glyph/material/framebuffer or original execution
parity claim is made. [Resource contract](behavior/display/NATIVE_SOURCE_FONT_RESOURCE_OWNERSHIP.md).

Original Nintendo/Intelligent Systems rights are retained. Source tracking
confers no new license or redistribution rights. No ISO/raw original payload
is committed; generated caches and packs remain ignored.

## Original battle menu font readers (2026-10-15)

Retained matrix/scalar menu-font tests read the US ISO read-only at the source
extent recorded in existing ignored romfs/font.json: f/papermarioset_US.bfn,
SHA256 bb9cf410e5d916a1268ff503fc4f1ad7e8630b381cd781958eb0be32abb56b45.
They reuse existing romfs/font.pmfont/font.t3x and original DOL/event constants;
this change performs no new production font conversion, resizing, atlas packing
or authored replacement. Runtime plain-US glyph admission compares live source
WID/GLY metadata against the same original native atlas before consuming RNG.

The raw BFN hash is checked against untouched ISO bytes. A private host fixture
then reproduces original setup's two ordered glyph0 width writes7followed9;
all256 mapping comparisons use original MAP data, and all162 width comparisons
use the post-setup private BFN matching the existing font pack. This is not a
claim that raw on-disc widths already contain those mutations or that the
original constructor was executed. Matrix and scalar string layouts retain
separate original pen/camera/outline/control-token policies. Missing multibyte,
raw memory, material or GPU owners are not replaced with invented font data.

See the [retained matrix font](behavior/display/RETAINED_MATRIX_FONT_RASTER.md),
[scalar font](behavior/display/RETAINED_SCALAR_FONT.md) and
[shared font-state helpers](behavior/display/RETAINED_SHARED_FONT_STATE.md).
Original Nintendo/Intelligent Systems rights remain reserved. Raw ISO resources,
private patched fixtures, metadata, packs and compiled binaries remain ignored;
this note grants no redistribution license and adds no extracted asset to git.

## NPC Main entry documentation validation inputs (2026-10-15)

artifacts/npc-main-entry-code-documentation-validation/source inherits all 69
accepted products and original enemy/footer/HP/HitN64/default test fixtures from
the prior private snapshot. Dependency comparison finds no converter/assets/
Makefile/CMake changes. Small mutable evidence/default fixtures copied privately;
unchanged HitN64 metadata retains a prior frozen private decoded-PNG path, whose
actual read bytes are independently hash-pinned and equal the new private copy.
All accepted/evidence/fixture hashes checked after gates. No conversion, editable
import, overlay or shared-cache write. Native full regeneration skipped; Python
fixtures independently read ISO read-only into empty temporary destinations.
No ISO, original extracted asset or compiled product committed; original
Nintendo/Intelligent Systems rights retained and no new license granted.

## NPC suffix documentation validation inputs (2026-10-15)

artifacts/npc-main-suffix-code-documentation-validation/source inherits the
previous private 69-product cache and original enemy/footer/HP/HitN64 fixtures.
Frozen field_party_entry_assets.py SHA256
be4292ad59633d4a7b509d8e5de1de0c71ba26174ac68549c2f8faf3aea6b916
reads original ISO read-only and verifies six ordinary-Goombella construction DOL
spans. It wrote two products into empty private cache-rebuild output; no editable
asset or shared cache overwritten. This baseline uses six spans/304-byte pack;
concurrent later twelve-span converter/test changes are excluded.

New private RomFS products (bytes, SHA256):

- romfs/field-party-entry.json: 1219, 89629e1c1f56d1fa23428b4b7f31655a4771328865913ad28410a69619250a11

- romfs/field-party-entry.pmevt: 304, 54af6a7381c0fd763e340f387b38ac0af4b2eabc5f3665c4f02c1846267d07d0

All 71 accepted products and inherited evidence/default fixtures checked after
gates. Actual inherited HitN64 decoded-PNG read path independently pinned and
identical to current private copy. Native full regeneration skipped; Python
fixtures independently read ISO read-only into empty temporary destinations.
No editable import/shared-pack write or ISO, extracted original/compiled product
commit. Nintendo/Intelligent Systems rights retained; local conversion grants
no new license.

## Party position constructor source data (2026-10-15)

The ordinary/auxiliary position binding extends field-party-entry.pmevt from
304 to488 bytes (12 unchanged DOL spans). Added exact spans: normal kind13
row80314a14/3c, first/final ordinary vectors802cb184/0c and802cb19c/0c,
overwritten stack seeds802cb190/0c and802cb220/0c, and unit scalar80421200/4.
The converter verifies each original SHA256 and keeps independent source addresses
for equal-valued rows/vectors. The three original definition records remain
unchanged. No image, animation, dialogue or event operand is fabricated.

The default host fixture reads original gor.rel operands at275b4,374bc and498e8
from the accepted ignored local-assets/door-data fixture. Private ARM verification
stages only that same full hash-matched original REL as an extra private RomFS
fixture; production RomFS does not add it. The frozen native build also imports
four prerequisite HitN64 products using its current original-data converter and
creates its default private test fixture. These are separate accepted upstream
requirements, not new party visuals. Three binary products match the accepted upstream bytes; JSON differs only
in its decoded_png path, now relative to the private fixture. All other accepted
products/fixture inputs remain pinned in the position validation manifest.

Original Nintendo/Intelligent Systems rights and local-only provenance remain.
No ISO/extracted source/compiled pack is committed; no redistribution rights
are implied. Complete story dispatch, kind11/other source resources, source GPU
parity and hardware performance remain separate implementation work.

## Queued NPC/party-removal documentation validation inputs (2026-10-15)

artifacts/npc-draw-party-code-documentation-validation/source inherits all 71
accepted products and original enemy/footer/HP/HitN64/default test fixtures.
Dependency comparison finds no converter/assets/Makefile/CMake changes since the
prior frozen baseline. Small mutable evidence/default fixtures copied privately;
actual inherited HitN64 decoded-PNG read path independently pinned and equal to
the current private copy. All product/evidence/fixture hashes checked after gates.
No conversion, editable import, overlay or shared-pack write. Native full
regeneration skipped; Python fixtures independently read ISO read-only into empty
temporary outputs. No ISO, original extracted asset or compiled product committed;
Nintendo/Intelligent Systems rights retained and no new license granted.

## Party/Maya documentation validation inputs (2026-10-15)

artifacts/party-maya-code-documentation-validation/source inherits the previous
private 71-product cache and original enemy/footer/HP/HitN64/default fixtures.
Only party-entry converter changes: six-to-twelve source DOL span expansion.
Frozen tools/field_party_entry_assets.py SHA256
8ec447095262f1f601197ad5ebe26c24c0ee22729310568a659fc40438c41667
ran with original ISO read-only into empty private cache-rebuild output. Each
original span hash checked; two outputs replace prior six-span products:

- romfs/field-party-entry.json: 2076 bytes, SHA256 5e36581af231c023d6117deeee32320e957f855b68c037d688ab65b7b71fadd1 (previous 89629e1c1f56d1fa23428b4b7f31655a4771328865913ad28410a69619250a11)

- romfs/field-party-entry.pmevt: 488 bytes, SHA256 9c844b997e0aa4ddfa907132278908f2db0b3e454619d97e32b6b74e0cfd6086 (previous 54af6a7381c0fd763e340f387b38ac0af4b2eabc5f3665c4f02c1846267d07d0)

All 71 products and original evidence/default fixture hashes checked after gates.
Actual party-position fixture local-assets/door-data/gor.rel SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Original Maya float1000 at8042019c read directly; four-byte SHA256
2412b43a42f11c1cb3ef7956d3e293016e671a9061fbb76a4282c8b15139cc48. No source bytes committed.
Actual inherited HitN64 decoded-PNG read path independently pinned. Native full
regeneration skipped; Python fixtures independently use read-only ISO and empty
temporary destinations. No editable import/shared-pack write or ISO, extracted
original/compiled product commit. Nintendo/Intelligent Systems rights retained;
local conversion grants no new license.

## Effect pool/HitN64 documentation validation inputs (2026-10-15)

artifacts/effect-pool-hit-code-documentation-validation/source inherits all 71
accepted products and original enemy/footer/HP/HitN64/party-position fixtures.
Dependency comparison finds no converter/assets/Makefile/CMake changes since the
prior frozen baseline. Small mutable evidence/default fixtures copied privately;
actual inherited HitN64 decoded-PNG read path independently pinned. All product/
evidence/fixture hashes checked after gates. No conversion, editable import,
overlay or shared-pack write. Native full regeneration skipped; Python fixtures
independently read ISO read-only into empty temporary outputs. No ISO, original
extracted asset or compiled product committed; Nintendo/Intelligent Systems
rights retained and no new license granted.

## Original submenu surfaces2/3 (retained weapon UI)

Source: read-only US ISO `battle/common/battle_common.tpl`, 217920 bytes,
SHA256 `8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1`.
`tools/battle_submenu_surface_assets.py` decodes original CMPR indices2
(16×24) and3 (8×24), places unchanged pixels with two-pixel
clamp-edge extrusion in a32×32 RGBA8 native atlas, and writes
`battle-submenu-surfaces.{bin,t3x,json}` into an empty output directory.
Private extraction cache must also be empty. No resizing, redrawn frame or
editable-asset overwrite. The whole original bank and prior class/footer packs
are unchanged. New compiled products belong after ISO-backed base conversion
and before normal editable-manifest compilation. The distinct source help
frame remains original `f/msgWindow.tpl` descriptor0; submenu images do not
replace its texture/readiness owner. Original Nintendo/Intelligent Systems
rights retained; provenance grants no new redistribution rights.


The submenu build wrapper compiles in fresh empty temporary output/cache
directories before publishing its three ignored RomFS products. Unchanged
products retain their bytes/mtime; interrupted compiled products are replaced.
No editable originals or shared extraction caches are modified. Native resident
font leases reuse the existing original512x256 font.t3x/font.pmfont pair without
conversion or additional GPU texture allocation; only metadata is copied.

## Recovery documentation validation inputs (2026-10-15)

artifacts/recovery-code-documentation-validation/source inherits all 71 accepted
products and original enemy/footer/HP/HitN64/party-position/default test fixtures.
Only CMake adds a mesh-publication target/test; converter/assets/Makefile bytes
are unchanged. Small mutable evidence/default fixtures copied privately; actual
HitN64 decoded-PNG read path independently pinned. All product/evidence/fixture
hashes checked after gates. No conversion, editable import, overlay or shared-pack
write. Native full regeneration skipped; Python fixtures independently read ISO
read-only into empty temporary outputs. No ISO, original extracted asset or
compiled product committed; Nintendo/Intelligent Systems rights retained and no
new license granted.

## 2026-10-15 — Cross-VM native story party services

No new runtime asset products, original dialogue or conversion choices. Frozen
source uses all 2161 accepted field-party-position runtime products and 778 private
inputs unchanged. Original lecture instructions and US message/control bytes
remain in the accepted Professor packs. Original assets retain their owners'
rights; no ISO, extracted source body or compiled binary is committed.

Fresh read-only G8ME01 DOL evidence under artifacts/field-party-story-evidence
covers complete join800f0a8c/partyJoin80140f74/pouchGetPtr800d5eb0,
run800eae70/partyRun800becdc, stop800eaeb8/partyStop800becb8 and
SlitOff8014c448 bodies. ELF/listing/provenance hashes are pinned in the session
manifest. This source inspection is not a hardware or pixel-parity observation.
The private ARM fixture uses the same production RomFS bytes, with only a test
entrypoint/strict marker injection in its private main.cpp.

## 2026-10-15 — Atomic native mesh publication (no asset changes)

The native consumer reads the existing original a/ model resources and retained
source outputs through their actual shared VM file mappings. Immutable original
model identity plus live static geometry/material/index/UV/directory tables guard
the trusted conversion-to-assembly relationship. The existing original Mario pose
banks, texture atlases and citro3d submission remain in use. No extraction,
rescaling, geometry reduction, lighting adjustment, music, dialogue or sound-effect
conversion was added. The read-only ISO verifier passed; all 2152 inherited RomFS
products and gor_02.s shader source remain byte-identical to the accepted baseline.
Original rights/licensing are unchanged. ISO, extracted originals, converted packs,
private fixtures and validation binaries remain outside source history. Exact
validation snapshot and hashes are in progress and the ignored artifact manifest.

## Runtime bridge documentation validation inputs (2026-10-15)

artifacts/runtime-bridges-code-documentation-validation/source freezes baseline
7ce245e9653440e613346bc2eaa2a69066c9b4f8. Relative to recovery validation,
Makefile adds submenu and unchanged-original BFN staging; surface/footer
converters add two-pixel clamp-edge atlas padding with source image pixels and
catalog bins unchanged. Four frozen converters read the original ISO read-only
into new empty private product/cache destinations; twelve targeted products
copied only into frozen build outputs/default fixture copies. No shared cache
or checked-in editable asset was overwritten. Original per-product licensing
remains in metadata. Footer atlas evidence hashes updated; other inherited
HP/enemy/HitN64/party fixtures and actual HitN64 external PNG path rechecked.

Accepted map now pins 78 products: 71 prior entries, three new submenu products,
two new exact raw-font products and two previously present surface bin/t3x paths
newly included in hash inventory. Font source f/papermarioset_US.bfn is66560
bytes, SHA256bb9cf410e5d916a1268ff503fc4f1ad7e8630b381cd781958eb0be32abb56b45;
conversion none, byte77 decimal stays1 (runtime width patches are separate).
Submenu atlas32x32 RGBA8/4096 decoded bytes; surface256x128/131072 and
footer512x256/524288 retain original image interiors/UV coordinates, with
clamp-edge gutters and no resampling. Manifest records converter/product and
original PNG/atlas evidence hashes. Native full regeneration skipped; existing
Python fixtures independently use read-only ISO and empty temporary outputs.
No ISO, extracted original asset or compiled binary committed; Nintendo/
Intelligent Systems rights retained, no new license or redistribution rights.

## Native admission documentation validation inputs (2026-10-15)

artifacts/native-admission-code-documentation-validation/source freezes baseline
18c4f9de839539bb2b6d4893f5d4d4a483ed49da. Compared with runtime-bridge validation,
only CMake adds existing host party-script/bridge/native-graphics sources and a
new read-only tool pins installed SDK archives. Converters/assets/Makefile unchanged.
All 78 accepted products and original surface/footer/HP/enemy/HitN64/party evidence
and test fixtures inherited by hash; small mutable/default evidence copied
privately. Actual HitN64 metadata PNG path remains a hash-pinned earlier private
fixture identical to current copy. All evidence rechecked after gates. No new
conversion/import/overlay/shared-cache writes; native full asset regeneration
skipped. Python fixtures independently read original ISO read-only into empty
temporary outputs. Original BFN remains exact; comments explain mutable runtime
width patches without editing packaged input. Nintendo/Intelligent Systems rights
retained; no new license/redistribution rights, no ISO/extracted original or compiled
binary committed.

## 2026-10-15 — Original animation selection (no conversion)

The retained selector reads the existing relocated original a/ MODEL+148 count
and MODEL+1ac 64-byte name directory through the actual file/VM mappings. Native
Mario supplies its existing name-to-index bridge; raw original string-pointer
selectors are also tested against the cold original Mario model. No extraction,
texture scaling, geometry/lighting, audio, dialogue or shader conversion changed.
All 2152 inherited RomFS products remain byte-identical to the accepted baseline.
Original rights/licensing are unchanged; ISO, extracted originals, compiled packs,
fixtures and validation binaries stay outside source history.

## 2026-10-15 — Original temporary Professor party data

Native auxiliary11 uses existing accepted field-native-c_kuri_h MODEL/TPL,
pose-directory/17-pose animation/draw resources. No Professor artwork or dialogue
is replaced. Read-only original record803145a0/28 is added to the definition pack
(now4 records/220 bytes). Construction pack adds aligned model802cb4bc/12,
normal row8031499c/60, S_1 at804213f4/4 and null-position80421d60/4
(now16 spans/616 bytes). All bytes, including source model padding, are exact
original DOL data with per-span hashes in the frozen converters/provenance.
Fresh source/evidence stays under artifacts/field-party-professor-evidence.

Dependency preflight against the previous accepted party cache found upstream
menu/font changes already in baseline5f49d704. Frozen upstream converters import
exact original papermarioset_US.bfn plus metadata, generate three submenu
products, and update footer/surface atlases to their committed clamp-edge
extrusion behavior. Four upstream runtime products change; five are added.
Four party products change; all other accepted runtime products are unchanged
(2166 total). Two private atlas PNGs change;13 footer provenance sidecars differ
only in the corrected private relative source path. All other private input
bytes remain unchanged (778 inputs). Original interiors/catalog UV and original
raw common TPL bytes remain unchanged. No shared/editable cache is an input to
the upstream submenu importer. Exact delta is pinned in the session manifest.

Original Nintendo/Intelligent Systems rights retained; local supplied ISO opened
read-only. No ISO, extracted source body, asset pack or compiled binary committed.
This is not a hardware/pixel/audio or complete prologue parity claim.

## Original camera8 window0 clamp-correct native atlas

`tools/battle_window_panel_assets.py` reads `f/msgWindow.tpl` descriptor0 from
local US revision0 ISO (offset947396944,size321984,SHA256
`2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67`). It
preserves decoded original72x72 CMPR pixels at0,0 in128x128 RGBA8. All right,
bottom and corner padding repeats the last source edge, with no source pixel
resize/rewrite. Full atlas RGBA SHA256
`7cab96f83790614593000d0918ed6119ec7bc45f1349741864ec5aadc68d5b45`,
FNV64`caadb76327b7d2be`; source RGBA SHA256
`40bb9c0b9723e7dcc916cc079f9d400287c96b7fc5d9b57039f6a14da5bf07c6`.
Source UV scale72/128 plus native V inversion/clamp preserves linear edges,
including source UV outside0..1. Existing messagePanel/composed-dialogue assets
and all their converters are unchanged. The distinct resource is
`battle-window-panel.t3x` plus provenance JSON; native validation checks actual
full tiled allocation pixels, import facts and opaque generation before replay.

`tools/build_battle_window_panel_assets.py` exports into an empty temporary
private directory and copies only T3X/JSON into RomFS after ISO base conversion,
before editable-manifest compilation. PNG/source/compiled private packs remain
ignored. Focused Python tests verify every original/padding pixel, independent
bilinear clamp samples and preservation of nonempty export destinations. Actual
T3X decompression independently matches the full image; strict host/ARM tests
are not native import/render or original framebuffer parity evidence.
Original Nintendo/Intelligent Systems rights remain; no redistribution license
is granted and no ISO, extracted original or compiled pack is committed. See
[the source/window ownership contract](behavior/display/ORIGINAL_CAMERA8_WINDOW_PACKETS.md).

## Battle UI bridge documentation validation inputs (2026-10-15)

artifacts/battle-ui-bridges-code-documentation-validation/source freezes baseline
5699eb7cb4efbf66b3aa3a9a64630dd717142de5. Relative to native-admission validation,
only CMake adds the original animation selector/native retained owner/test.
Tools/converters/assets/Makefile unchanged. All 78 accepted products and original
surface/footer/HP/enemy/HitN64/party evidence/default test fixtures inherited by
hash; small mutable/default evidence copied privately. Actual HitN64 metadata PNG
read path remains a pinned earlier private fixture identical to current copy.
All evidence rechecked after gates. No conversion/import/overlay/shared-cache
write. Native full regeneration skipped; Python fixtures independently read ISO
read-only into empty temporary destinations. Guides explain existing submenu
32x32/gutter and original US512x256 font adapters without altering pixels,
packaged BFN or editable art. Nintendo/Intelligent Systems rights retained; no
new license/redistribution rights and no ISO/extracted original or compiled
binary committed.

## 2026-10-15 — Private native capture optimization (no asset changes)

The decoder still consumes existing actual source output arrays and original
relocated a/ MODEL geometry/name resources. No extraction, texture scaling,
geometry/lighting, audio, dialogue or shader conversion changed. All 2152 inherited
RomFS products and the original gor_02.s source remain byte-identical to the
accepted baseline. Original rights/licensing are unchanged. ISO, extracted
originals, compiled packs, fixtures, profiler builds and validation binaries stay
outside source history.
## Original raw process window TPL (2026-10-15)

`tools/source_window_resources_assets.py` exports unchanged original
`f/msgWindow.tpl`, read-only US revision0 ISO offset947396944,321984 bytes,
SHA256 `2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67`.
Ignored products `romfs/source-window/msgWindow.tpl` and `source-window.json`
record source extent/hash and converter identity. There is no image conversion,
palette relocation, texture object patch or readiness write at export. Empty
new destinations only; exact matching packs may be reused, while changed or
extra existing files are preserved and rejected. Native private allocation
backing permits the actual source Unpack mutations without changing the file.
Actual process window globals/unknowns share history across VM bindings; real
Setup/DVD/heap/delivery/GPU producer proofs remain external. Neither raw object
bytes nor native panel residency creates source completion.
[Ownership contract](behavior/display/NATIVE_SOURCE_WINDOW_MEMORY.md).
Original Nintendo/Intelligent Systems rights are retained; no new license or
redistribution rights are granted. No original payload or ISO is committed.

Harbor battle callback recovery (2026-10-15): original US ISO opened read-only for complete DOL start8004ef54/b8, wait8004eed8/7c, get_reglid8004f894/58 and original gor.rel beforebattle inventory. Evidence/provenance stays in artifacts/harbor-battle-handoff-evidence; no new runtime pack, asset conversion, dialogue/art/audio edit or redistribution grant. Existing Nintendo/Intelligent Systems original rights unchanged.
