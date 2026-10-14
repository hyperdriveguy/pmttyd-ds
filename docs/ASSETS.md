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
131072 texture bytes,2px transparent padding and no resampling. The existing
class icon atlas remains separate. Geometry/UV quadrant mirroring, source
shadow/dimming and label scaling are consumer behavior, not painted into the
textures. No generated art or rewritten labels were added. Nintendo/Intelligent
Systems assets have no granted redistribution rights; do not commit or distribute
original ISO, extracted images or compiled packs. GPU texture binding remains a
native renderer task; imported assets alone do not prove visible menu parity.

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
