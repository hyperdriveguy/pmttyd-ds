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
